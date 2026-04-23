#include "adxl366.h"
#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/knl/Clock.h>
#include <unistd.h>
#include "ti_drivers_config.h"

#define ADXL366_SAMPLE_PERIOD_MS            20U
#define ADXL366_STEP_SETTLE_MS              1500U
#define ADXL366_GRAVITY_SHIFT               4U
#define ADXL366_SIGNAL_SHIFT                2U
#define ADXL366_ENVELOPE_SHIFT              3U
#define ADXL366_TOTAL_BASELINE_SHIFT        4U
#define ADXL366_RATIO_DEN                   100U
#define ADXL366_STEP_MIN_MS                 280U
#define ADXL366_STEP_MAX_MS                 1400U
#define ADXL366_STEP_CONFIRM_COUNT          2U
#define ADXL366_STEP_LOW_MIN                10U
#define ADXL366_STEP_HIGH_MIN               28U
#define ADXL366_STEP_TIMEOUT_MS             900U
#define ADXL366_ACTIVITY_MIN                12U
#define ADXL366_FALL_LOW_G_RATIO            40U
#define ADXL366_FALL_IMPACT_RATIO           235U
#define ADXL366_FALL_RECOVERY_RATIO         35U
#define ADXL366_FALL_STILL_THRESHOLD        10U
#define ADXL366_FALL_LOW_G_MIN_SAMPLES      2U
#define ADXL366_FALL_IMPACT_WINDOW_MS       700U
#define ADXL366_FALL_STILL_MS               900U

/* Instruction Commands for ADXL366 */
#define ADXL366_WRITE_CMD      0x0A
#define ADXL366_READ_CMD       0x0B

/* Register Map for ADXL366 (ADXL362 Compatible) */
#define ADXL366_REG_DEVID_AD       0x00
#define ADXL366_REG_DEVID_MST      0x01
#define ADXL366_REG_PART_ID        0x02
#define ADXL366_REG_REV_ID         0x03
#define ADXL366_REG_XDATA_L        0x0E
#define ADXL366_REG_XDATA_H        0x0F
#define ADXL366_REG_YDATA_L        0x10
#define ADXL366_REG_YDATA_H        0x11
#define ADXL366_REG_ZDATA_L        0x12
#define ADXL366_REG_ZDATA_H        0x13
#define ADXL366_REG_SOFT_RESET     0x1F
#define ADXL366_REG_THRESH_ACT_L   0x20
#define ADXL366_REG_THRESH_ACT_H   0x21
#define ADXL366_REG_TIME_ACT       0x22
#define ADXL366_REG_ACT_INACT_CTL  0x27
#define ADXL366_REG_INTMAP1        0x2A
#define ADXL366_REG_FILTER_CTL     0x2C
#define ADXL366_REG_POWER_CTL      0x2D

typedef enum
{
    ADXL366_FALL_IDLE = 0,
    ADXL366_FALL_LOW_G,
    ADXL366_FALL_IMPACT
} adxl366_fall_state_t;

static uint32_t trackedSteps = 0;
static bool fallEventFlag = false;
static SPI_Handle spiAccelHandle;
static uint8_t adxlPartId = 0;
static uint8_t adxlStatusFlags = 0;

static volatile bool accelIntReady = false;
static volatile uint32_t accelInterruptCount = 0;

static int32_t gravityX = 0;
static int32_t gravityY = 0;
static int32_t gravityZ = 0;
static uint32_t motionSignal = 0;
static uint32_t motionEnvelope = 0;
static uint32_t totalBaselineMagSq = 0;
static uint32_t lastDynamicMag = 0;
static uint32_t lastStepTimeMs = 0;
static uint32_t stepSettleUntilMs = 0;
static uint32_t stepCandidateCount = 0;
static uint32_t pendingSteps = 0;
static uint32_t lowGStartMs = 0;
static uint32_t lowGSampleCount = 0;
static uint32_t impactTimeMs = 0;
static uint32_t stillStartMs = 0;
static int32_t prevMotionSignal = 0;
static bool prevMotionRising = false;
static bool stepArmed = false;
static bool walkingActive = false;
static adxl366_fall_state_t fallState = ADXL366_FALL_IDLE;

static int16_t lastAccelX = 0;
static int16_t lastAccelY = 0;
static int16_t lastAccelZ = 0;

static uint8_t ADXL366_readReg(uint8_t reg);
static void ADXL366_writeReg(uint8_t reg, uint8_t data);
static void ADXL366_readSample(int16_t *xData, int16_t *yData, int16_t *zData);

static uint32_t ADXL366_abs32(int32_t value)
{
    return (value < 0) ? (uint32_t)(-value) : (uint32_t)value;
}

static uint32_t ADXL366_maxU32(uint32_t a, uint32_t b)
{
    return (a > b) ? a : b;
}

static uint32_t ADXL366_nowMs(void)
{
    return (uint32_t)(((uint64_t)Clock_getTicks() * Clock_tickPeriod) / 1000U);
}

static int16_t ADXL366_parseAxis(uint8_t dataL, uint8_t dataH)
{
    uint16_t raw = ((uint16_t)dataH << 8) | dataL;

    raw &= 0x0FFFU;
    if ((raw & 0x0800U) != 0U) {
        raw |= 0xF000U;
    }

    return (int16_t)raw;
}

static void ADXL366_resetStepState(void)
{
    stepArmed = false;
    walkingActive = false;
    stepCandidateCount = 0U;
    pendingSteps = 0U;
}

static void ADXL366_registerStep(uint32_t nowMs)
{
    uint32_t deltaMs = nowMs - lastStepTimeMs;

    if ((lastStepTimeMs != 0U) &&
        ((deltaMs < ADXL366_STEP_MIN_MS) || (deltaMs > ADXL366_STEP_MAX_MS))) {
        stepCandidateCount = 1U;
        pendingSteps = 1U;
        walkingActive = false;
        lastStepTimeMs = nowMs;
        return;
    }

    lastStepTimeMs = nowMs;

    if (!walkingActive) {
        if (stepCandidateCount < ADXL366_STEP_CONFIRM_COUNT) {
            stepCandidateCount++;
        }
        pendingSteps++;

        if (stepCandidateCount >= ADXL366_STEP_CONFIRM_COUNT) {
            trackedSteps += pendingSteps;
            pendingSteps = 0U;
            walkingActive = true;
        }
    } else {
        trackedSteps++;
    }
}

static void ADXL366_updateFallState(uint32_t nowMs, uint32_t totalMagSq, uint32_t filteredMotion)
{
    bool lowGDetected = false;
    bool impactDetected = false;

    if (totalBaselineMagSq != 0U) {
        lowGDetected = (((uint64_t)totalMagSq * ADXL366_RATIO_DEN) <
                        ((uint64_t)totalBaselineMagSq * ADXL366_FALL_LOW_G_RATIO));
        impactDetected = (((uint64_t)totalMagSq * ADXL366_RATIO_DEN) >
                          ((uint64_t)totalBaselineMagSq * ADXL366_FALL_IMPACT_RATIO));
    }

    switch (fallState) {
        case ADXL366_FALL_IDLE:
            if (lowGDetected) {
                lowGSampleCount++;
                if (lowGSampleCount >= ADXL366_FALL_LOW_G_MIN_SAMPLES) {
                    fallState = ADXL366_FALL_LOW_G;
                    lowGStartMs = nowMs;
                }
            } else {
                lowGSampleCount = 0U;
            }
            break;

        case ADXL366_FALL_LOW_G:
            if ((nowMs - lowGStartMs) > ADXL366_FALL_IMPACT_WINDOW_MS) {
                fallState = ADXL366_FALL_IDLE;
                lowGSampleCount = 0U;
            } else if (impactDetected && (filteredMotion > (motionEnvelope / 2U + ADXL366_ACTIVITY_MIN))) {
                fallState = ADXL366_FALL_IMPACT;
                impactTimeMs = nowMs;
                stillStartMs = 0U;
            }
            break;

        case ADXL366_FALL_IMPACT:
            if ((nowMs - impactTimeMs) > (ADXL366_FALL_IMPACT_WINDOW_MS + ADXL366_FALL_STILL_MS)) {
                fallState = ADXL366_FALL_IDLE;
                lowGSampleCount = 0U;
                stillStartMs = 0U;
            } else if (filteredMotion <= ADXL366_FALL_STILL_THRESHOLD) {
                if (stillStartMs == 0U) {
                    stillStartMs = nowMs;
                } else if ((nowMs - stillStartMs) >= ADXL366_FALL_STILL_MS) {
                    fallEventFlag = true;
                    fallState = ADXL366_FALL_IDLE;
                    lowGSampleCount = 0U;
                    stillStartMs = 0U;
                }
            } else {
                if ((totalBaselineMagSq != 0U) &&
                    (((uint64_t)totalMagSq * ADXL366_RATIO_DEN) >
                     ((uint64_t)totalBaselineMagSq * ADXL366_FALL_RECOVERY_RATIO))) {
                    fallState = ADXL366_FALL_IDLE;
                    lowGSampleCount = 0U;
                }
                stillStartMs = 0U;
            }
            break;

        default:
            fallState = ADXL366_FALL_IDLE;
            lowGSampleCount = 0U;
            stillStartMs = 0U;
            break;
    }
}

static void ADXL366_writeReg(uint8_t reg, uint8_t data)
{
    SPI_Transaction transaction = { 0 };
    uint8_t txBuffer[3];

    txBuffer[0] = ADXL366_WRITE_CMD;
    txBuffer[1] = reg;
    txBuffer[2] = data;

    transaction.count = sizeof(txBuffer);
    transaction.txBuf = (void *)txBuffer;
    transaction.rxBuf = NULL;

    GPIO_write(CONFIG_GPIO_ADXL_CS, 0);
    SPI_transfer(spiAccelHandle, &transaction);
    GPIO_write(CONFIG_GPIO_ADXL_CS, 1);
}

static void ADXL366_readSample(int16_t *xData, int16_t *yData, int16_t *zData)
{
    SPI_Transaction transaction = { 0 };
    uint8_t txBuffer[8] = {
        ADXL366_READ_CMD,
        ADXL366_REG_XDATA_L,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t rxBuffer[8] = { 0 };

    GPIO_write(CONFIG_GPIO_ADXL_CS, 0);
    transaction.count = sizeof(txBuffer);
    transaction.txBuf = (void *)txBuffer;
    transaction.rxBuf = (void *)rxBuffer;
    SPI_transfer(spiAccelHandle, &transaction);
    GPIO_write(CONFIG_GPIO_ADXL_CS, 1);

    *xData = ADXL366_parseAxis(rxBuffer[2], rxBuffer[3]);
    *yData = ADXL366_parseAxis(rxBuffer[4], rxBuffer[5]);
    *zData = ADXL366_parseAxis(rxBuffer[6], rxBuffer[7]);
}

static void adxl_gpio_callback(uint_least8_t index)
{
    (void)index;
    accelIntReady = true;
    accelInterruptCount++;
    adxlStatusFlags |= ADXL366_STATUS_INT_SEEN;
}

static uint8_t ADXL366_readReg(uint8_t reg)
{
    SPI_Transaction transaction = { 0 };
    uint8_t txBuffer[3] = { ADXL366_READ_CMD, reg, 0x00 };
    uint8_t rxBuffer[3] = { 0 };

    GPIO_write(CONFIG_GPIO_ADXL_CS, 0);
    transaction.count = sizeof(txBuffer);
    transaction.txBuf = (void *)txBuffer;
    transaction.rxBuf = (void *)rxBuffer;
    SPI_transfer(spiAccelHandle, &transaction);
    GPIO_write(CONFIG_GPIO_ADXL_CS, 1);

    return rxBuffer[2];
}

bool ADXL366_init(void)
{
    SPI_Params spiParams;
    uint32_t nowMs;

    GPIO_setConfig(CONFIG_GPIO_ADXL_CS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_write(CONFIG_GPIO_ADXL_CS, 1);

    SPI_Params_init(&spiParams);
    spiParams.bitRate = 1000000;
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.mode = SPI_CONTROLLER;

    spiAccelHandle = SPI_open(CONFIG_SPI_ACCEL, &spiParams);
    if (spiAccelHandle == NULL) {
        return false;
    }

    adxlStatusFlags = ADXL366_STATUS_SPI_OPEN;

    GPIO_setConfig(CONFIG_GPIO_ADXL_INT, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setCallback(CONFIG_GPIO_ADXL_INT, adxl_gpio_callback);
    GPIO_enableInt(CONFIG_GPIO_ADXL_INT);

    ADXL366_writeReg(ADXL366_REG_SOFT_RESET, 0x52);
    usleep(10000);

    if ((ADXL366_readReg(ADXL366_REG_DEVID_AD) == 0xADU) &&
        (ADXL366_readReg(ADXL366_REG_DEVID_MST) == 0x1DU)) {
        adxlPartId = ADXL366_readReg(ADXL366_REG_PART_ID);
        if (adxlPartId == 0xF7U) {
            adxlStatusFlags |= ADXL366_STATUS_PART_OK;
        }
    } else {
        adxlPartId = 0U;
    }

    ADXL366_writeReg(ADXL366_REG_THRESH_ACT_L, 0x30);
    ADXL366_writeReg(ADXL366_REG_THRESH_ACT_H, 0x00);
    ADXL366_writeReg(ADXL366_REG_TIME_ACT, 0x02);
    ADXL366_writeReg(ADXL366_REG_ACT_INACT_CTL, 0x01);
    ADXL366_writeReg(ADXL366_REG_INTMAP1, 0x10);
    ADXL366_writeReg(ADXL366_REG_FILTER_CTL, 0x13);
    ADXL366_writeReg(ADXL366_REG_POWER_CTL, 0x22);

    trackedSteps = 0U;
    fallEventFlag = false;
    accelInterruptCount = 0U;
    gravityX = 0;
    gravityY = 0;
    gravityZ = 0;
    motionSignal = 0U;
    motionEnvelope = 0U;
    totalBaselineMagSq = 0U;
    lastDynamicMag = 0U;
    lastStepTimeMs = 0U;
    nowMs = ADXL366_nowMs();
    stepSettleUntilMs = nowMs + ADXL366_STEP_SETTLE_MS;
    stepCandidateCount = 0U;
    pendingSteps = 0U;
    lowGStartMs = 0U;
    lowGSampleCount = 0U;
    impactTimeMs = 0U;
    stillStartMs = 0U;
    prevMotionSignal = 0;
    prevMotionRising = false;
    stepArmed = false;
    walkingActive = false;
    fallState = ADXL366_FALL_IDLE;
    lastAccelX = 0;
    lastAccelY = 0;
    lastAccelZ = 0;
    accelIntReady = false;

    return true;
}

bool ADXL366_isFallDetected(void)
{
    bool currentFall = fallEventFlag;
    fallEventFlag = false;
    return currentFall;
}

void ADXL366_updateSteps(void)
{
    int16_t xData;
    int16_t yData;
    int16_t zData;
    int32_t linX;
    int32_t linY;
    int32_t linZ;
    int32_t filteredMotion;
    uint32_t linearMagnitude;
    uint32_t totalMagnitudeSq;
    uint32_t lowThreshold;
    uint32_t highThreshold;
    uint32_t nowMs;
    bool motionRising;

    if (accelIntReady) {
        accelIntReady = false;
    }

    ADXL366_readSample(&xData, &yData, &zData);
    adxlStatusFlags |= ADXL366_STATUS_SAMPLE_SEEN;
    lastAccelX = xData;
    lastAccelY = yData;
    lastAccelZ = zData;

    if (gravityX == 0 && gravityY == 0 && gravityZ == 0) {
        gravityX = (int32_t)xData;
        gravityY = (int32_t)yData;
        gravityZ = (int32_t)zData;
    } else {
        gravityX += ((int32_t)xData - gravityX) >> ADXL366_GRAVITY_SHIFT;
        gravityY += ((int32_t)yData - gravityY) >> ADXL366_GRAVITY_SHIFT;
        gravityZ += ((int32_t)zData - gravityZ) >> ADXL366_GRAVITY_SHIFT;
    }

    linX = (int32_t)xData - gravityX;
    linY = (int32_t)yData - gravityY;
    linZ = (int32_t)zData - gravityZ;

    linearMagnitude = ADXL366_abs32(linX) + ADXL366_abs32(linY) + ADXL366_abs32(linZ);
    motionSignal += ((int32_t)linearMagnitude - (int32_t)motionSignal) >> ADXL366_SIGNAL_SHIFT;
    motionEnvelope += ((int32_t)motionSignal - (int32_t)motionEnvelope) >> ADXL366_ENVELOPE_SHIFT;
    lastDynamicMag = motionSignal;

    totalMagnitudeSq = (uint32_t)((int32_t)xData * xData) +
                       (uint32_t)((int32_t)yData * yData) +
                       (uint32_t)((int32_t)zData * zData);
    if (totalBaselineMagSq == 0U) {
        totalBaselineMagSq = totalMagnitudeSq;
    } else {
        totalBaselineMagSq += ((int32_t)totalMagnitudeSq - (int32_t)totalBaselineMagSq) >>
                              ADXL366_TOTAL_BASELINE_SHIFT;
    }

    nowMs = ADXL366_nowMs();

    if (nowMs < stepSettleUntilMs) {
        ADXL366_resetStepState();
        fallState = ADXL366_FALL_IDLE;
        lowGSampleCount = 0U;
        stillStartMs = 0U;
        fallEventFlag = false;
        prevMotionSignal = (int32_t)motionSignal;
        prevMotionRising = false;
        return;
    }

    ADXL366_updateFallState(nowMs, totalMagnitudeSq, motionSignal);

    lowThreshold = ADXL366_maxU32((motionEnvelope / 3U), ADXL366_STEP_LOW_MIN);
    highThreshold = ADXL366_maxU32((motionEnvelope * 3U) / 4U, ADXL366_STEP_HIGH_MIN);
    filteredMotion = (int32_t)motionSignal;
    motionRising = (filteredMotion > prevMotionSignal);

    if (motionSignal < ADXL366_ACTIVITY_MIN) {
        ADXL366_resetStepState();
        prevMotionSignal = filteredMotion;
        prevMotionRising = motionRising;
        return;
    }

    if ((uint32_t)filteredMotion <= lowThreshold) {
        stepArmed = true;
    }

    if (stepArmed && prevMotionRising && !motionRising && ((uint32_t)prevMotionSignal >= highThreshold)) {
        ADXL366_registerStep(nowMs);
        stepArmed = false;
    }

    if ((lastStepTimeMs != 0U) && ((nowMs - lastStepTimeMs) > ADXL366_STEP_MAX_MS)) {
        walkingActive = false;
        stepCandidateCount = 0U;
        pendingSteps = 0U;
    }

    if ((lastStepTimeMs != 0U) && ((nowMs - lastStepTimeMs) > ADXL366_STEP_TIMEOUT_MS) &&
        (motionSignal < lowThreshold)) {
        stepArmed = true;
    }

    prevMotionSignal = filteredMotion;
    prevMotionRising = motionRising;
}

uint32_t ADXL366_getSteps(void)
{
    return trackedSteps;
}

void ADXL366_getLastSample(int16_t *xData, int16_t *yData, int16_t *zData)
{
    if (xData != NULL) {
        *xData = lastAccelX;
    }
    if (yData != NULL) {
        *yData = lastAccelY;
    }
    if (zData != NULL) {
        *zData = lastAccelZ;
    }
}

uint32_t ADXL366_getLastDynamicMagnitude(void)
{
    return lastDynamicMag;
}

uint32_t ADXL366_getBaselineMagnitude(void)
{
    return totalBaselineMagSq;
}

uint32_t ADXL366_getInterruptCount(void)
{
    return accelInterruptCount;
}

uint8_t ADXL366_getPartId(void)
{
    return adxlPartId;
}

uint8_t ADXL366_getStatus(void)
{
    return adxlStatusFlags;
}
