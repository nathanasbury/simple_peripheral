#include "afe.h"
#include <string.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>
#include <unistd.h>
#include "ti_drivers_config.h"

#define AFE_GPIO_INT_PIN                  CONFIG_GPIO_AFE_INT
#define AFE_GPIO_CS_PIN                   CONFIG_GPIO_AFE_CS
#define AFE_SAMPLE_RATE_HZ                100U
#define AFE_WINDOW_SIZE                   200U
#define AFE_FIFO_MAX_ITEMS                128U
#define AFE_DC_SHIFT                      4U
#define AFE_SIGNAL_SHIFT                  2U
#define AFE_ENVELOPE_SHIFT                5U
#define AFE_MIN_SIGNAL_AMPLITUDE          120U
#define AFE_MIN_DC_LEVEL                  2000U
#define AFE_MIN_BEAT_INTERVAL_SAMPLES     ((AFE_SAMPLE_RATE_HZ * 60U) / 180U)
#define AFE_MAX_BEAT_INTERVAL_SAMPLES     ((AFE_SAMPLE_RATE_HZ * 60U) / 35U)
#define AFE_BEAT_HISTORY_SIZE             6U
#define AFE_FIFO_MIN_ITEMS                2U
#define AFE_SIGNAL_TIMEOUT_SAMPLES        (AFE_SAMPLE_RATE_HZ * 4U)
#define AFE_SPO2_RECALC_INTERVAL          20U
#define AFE_EMPTY_POLL_TIMEOUT            200U

/* Register Map for MAX86140 */
#define MAX86140_INT_STAT_1     0x00
#define MAX86140_INT_EN_1       0x02
#define MAX86140_FIFO_WR_PTR    0x04
#define MAX86140_FIFO_RD_PTR    0x05
#define MAX86140_OVF_COUNTER    0x06
#define MAX86140_FIFO_DATA_CNTR 0x07
#define MAX86140_FIFO_DATA      0x08
#define MAX86140_SYS_CTRL       0x0D
#define MAX86140_PPG_CFG1       0x11
#define MAX86140_PPG_CFG2       0x12
#define MAX86140_LED_SEQ1       0x20
#define MAX86140_LED_SEQ2       0x21
#define MAX86140_LED1_PA        0x23
#define MAX86140_LED2_PA        0x24
#define MAX86140_PART_ID        0xFF

static uint32_t redRawWindow[AFE_WINDOW_SIZE];
static uint32_t irRawWindow[AFE_WINDOW_SIZE];
static int16_t redAcWindow[AFE_WINDOW_SIZE];
static int16_t irAcWindow[AFE_WINDOW_SIZE];
static uint64_t redRawSum = 0;
static uint64_t irRawSum = 0;
static uint64_t redAcSqSum = 0;
static uint64_t irAcSqSum = 0;
static uint16_t windowCount = 0;
static uint16_t windowIndex = 0;

static uint32_t sampleIndex = 0;
static uint32_t lastBeatSampleIndex = 0;
static uint32_t lastHeartRateUpdateIndex = 0;
static uint32_t lastSpO2UpdateIndex = 0;
static uint32_t lastSpO2RecalcIndex = 0;
static uint16_t emptyPollCount = 0;
static uint16_t beatIntervals[AFE_BEAT_HISTORY_SIZE] = { 0 };
static uint8_t beatIntervalCount = 0;
static uint8_t beatIntervalIndex = 0;

static int32_t redDcEstimate = 0;
static int32_t irDcEstimate = 0;
static int32_t redSignal = 0;
static int32_t irSignal = 0;
static int32_t prevIrSignal = 0;
static uint32_t irEnvelope = 0;
static bool prevIrRising = false;
static bool beatArmed = false;

static uint8_t current_hr = 0;
static uint8_t current_spo2 = 0;
static uint8_t afePartId = 0;
static uint8_t afeLastFifoCount = 0;
static uint8_t afeStatusFlags = 0;
static uint32_t afeLastRedSample = 0;
static uint32_t afeLastIrSample = 0;
static SPI_Handle spiAfeHandle;
static uint8_t fifoTxBuffer[2U + (AFE_FIFO_MAX_ITEMS * 3U)] = { 0 };
static uint8_t fifoRxBuffer[2U + (AFE_FIFO_MAX_ITEMS * 3U)] = { 0 };
static uint8_t rawFifoDataBuffer[AFE_FIFO_MAX_ITEMS * 3U] = { 0 };

static volatile bool afeDataReady = false;
static volatile uint32_t afeInterruptCount = 0;

static uint32_t AFE_abs32(int32_t value)
{
    return (value < 0) ? (uint32_t)(-value) : (uint32_t)value;
}

static uint64_t AFE_square32(int32_t value)
{
    int64_t temp = value;
    return (uint64_t)(temp * temp);
}

static uint32_t AFE_u64Sqrt(uint64_t value)
{
    uint64_t bit = (uint64_t)1 << 62;
    uint64_t result = 0;

    while (bit > value) {
        bit >>= 2;
    }

    while (bit != 0U) {
        if (value >= (result + bit)) {
            value -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }

    return (uint32_t)result;
}

static void AFE_resetState(void)
{
    memset(redRawWindow, 0, sizeof(redRawWindow));
    memset(irRawWindow, 0, sizeof(irRawWindow));
    memset(redAcWindow, 0, sizeof(redAcWindow));
    memset(irAcWindow, 0, sizeof(irAcWindow));
    memset(beatIntervals, 0, sizeof(beatIntervals));

    redRawSum = 0;
    irRawSum = 0;
    redAcSqSum = 0;
    irAcSqSum = 0;
    windowCount = 0;
    windowIndex = 0;

    sampleIndex = 0;
    lastBeatSampleIndex = 0;
    lastHeartRateUpdateIndex = 0;
    lastSpO2UpdateIndex = 0;
    lastSpO2RecalcIndex = 0;
    emptyPollCount = 0;
    beatIntervalCount = 0;
    beatIntervalIndex = 0;

    redDcEstimate = 0;
    irDcEstimate = 0;
    redSignal = 0;
    irSignal = 0;
    prevIrSignal = 0;
    irEnvelope = 0;
    prevIrRising = false;
    beatArmed = false;

    current_hr = 0;
    current_spo2 = 0;
    afeLastFifoCount = 0;
    afeLastRedSample = 0;
    afeLastIrSample = 0;
    afeDataReady = false;
    afeInterruptCount = 0;
}

static void AFE_storeWindowSample(uint32_t redRaw, uint32_t irRaw, int16_t redAc, int16_t irAc)
{
    if (windowCount == AFE_WINDOW_SIZE) {
        redRawSum -= redRawWindow[windowIndex];
        irRawSum -= irRawWindow[windowIndex];
        redAcSqSum -= AFE_square32(redAcWindow[windowIndex]);
        irAcSqSum -= AFE_square32(irAcWindow[windowIndex]);
    } else {
        windowCount++;
    }

    redRawWindow[windowIndex] = redRaw;
    irRawWindow[windowIndex] = irRaw;
    redAcWindow[windowIndex] = redAc;
    irAcWindow[windowIndex] = irAc;

    redRawSum += redRaw;
    irRawSum += irRaw;
    redAcSqSum += AFE_square32(redAc);
    irAcSqSum += AFE_square32(irAc);

    windowIndex++;
    if (windowIndex >= AFE_WINDOW_SIZE) {
        windowIndex = 0;
    }
}

static void AFE_updateHeartRate(int32_t filteredIr)
{
    uint32_t highThreshold;
    uint32_t lowThreshold;
    bool isRising;

    highThreshold = irEnvelope / 3U;
    if (highThreshold < AFE_MIN_SIGNAL_AMPLITUDE) {
        highThreshold = AFE_MIN_SIGNAL_AMPLITUDE;
    }

    lowThreshold = highThreshold / 3U;
    if (filteredIr <= (int32_t)lowThreshold) {
        beatArmed = true;
    }

    isRising = (filteredIr > prevIrSignal);

    if (beatArmed && prevIrRising && !isRising &&
        (prevIrSignal > 0) && ((uint32_t)prevIrSignal >= highThreshold)) {
        uint32_t peakIndex = sampleIndex - 1U;

        if (lastBeatSampleIndex != 0U) {
            uint32_t interval = peakIndex - lastBeatSampleIndex;

            if ((interval >= AFE_MIN_BEAT_INTERVAL_SAMPLES) &&
                (interval <= AFE_MAX_BEAT_INTERVAL_SAMPLES)) {
                uint32_t sumIntervals = 0;
                uint32_t bpm;
                uint8_t i;

                beatIntervals[beatIntervalIndex] = (uint16_t)interval;
                if (beatIntervalCount < AFE_BEAT_HISTORY_SIZE) {
                    beatIntervalCount++;
                }
                beatIntervalIndex = (beatIntervalIndex + 1U) % AFE_BEAT_HISTORY_SIZE;

                for (i = 0U; i < beatIntervalCount; i++) {
                    sumIntervals += beatIntervals[i];
                }

                if (sumIntervals != 0U) {
                    bpm = (AFE_SAMPLE_RATE_HZ * 60U * beatIntervalCount + (sumIntervals / 2U)) / sumIntervals;
                    if ((bpm >= 40U) && (bpm <= 180U)) {
                        current_hr = (current_hr == 0U) ? (uint8_t)bpm
                                                        : (uint8_t)((3U * current_hr + bpm + 2U) / 4U);
                        lastHeartRateUpdateIndex = peakIndex;
                    }
                }
            }
        }

        lastBeatSampleIndex = peakIndex;
        beatArmed = false;
    }

    prevIrRising = isRising;
    prevIrSignal = filteredIr;

    if ((lastHeartRateUpdateIndex != 0U) &&
        ((sampleIndex - lastHeartRateUpdateIndex) > AFE_SIGNAL_TIMEOUT_SAMPLES)) {
        current_hr = 0U;
    }
}

static void AFE_updateSpO2(void)
{
    uint32_t redDc;
    uint32_t irDc;
    uint32_t redAcRms;
    uint32_t irAcRms;
    uint32_t ratioScaled;
    uint32_t spo2Estimate;
    uint32_t adjustment;

    if (windowCount < (AFE_WINDOW_SIZE / 2U)) {
        if ((sampleIndex - lastSpO2UpdateIndex) > AFE_SIGNAL_TIMEOUT_SAMPLES) {
            current_spo2 = 0U;
        }
        return;
    }

    if ((sampleIndex - lastSpO2RecalcIndex) < AFE_SPO2_RECALC_INTERVAL) {
        return;
    }

    lastSpO2RecalcIndex = sampleIndex;

    redDc = (uint32_t)(redRawSum / windowCount);
    irDc = (uint32_t)(irRawSum / windowCount);
    redAcRms = AFE_u64Sqrt(redAcSqSum / windowCount);
    irAcRms = AFE_u64Sqrt(irAcSqSum / windowCount);

    if ((redDc < AFE_MIN_DC_LEVEL) || (irDc < AFE_MIN_DC_LEVEL) ||
        (redAcRms < AFE_MIN_SIGNAL_AMPLITUDE) || (irAcRms < AFE_MIN_SIGNAL_AMPLITUDE)) {
        if ((sampleIndex - lastSpO2UpdateIndex) > AFE_SIGNAL_TIMEOUT_SAMPLES) {
            current_spo2 = 0U;
        }
        return;
    }

    ratioScaled = (uint32_t)(((uint64_t)redAcRms * irDc * 1000U) /
                             ((uint64_t)irAcRms * redDc));

    if ((ratioScaled < 200U) || (ratioScaled > 1800U)) {
        if ((sampleIndex - lastSpO2UpdateIndex) > AFE_SIGNAL_TIMEOUT_SAMPLES) {
            current_spo2 = 0U;
        }
        return;
    }

    adjustment = (23U * ratioScaled + 500U) / 1000U;
    spo2Estimate = (adjustment >= 110U) ? 0U : (110U - adjustment);
    if (spo2Estimate > 100U) {
        spo2Estimate = 100U;
    }
    if (spo2Estimate < 70U) {
        spo2Estimate = 70U;
    }

    current_spo2 = (current_spo2 == 0U) ? (uint8_t)spo2Estimate
                                        : (uint8_t)((3U * current_spo2 + spo2Estimate + 2U) / 4U);
    lastSpO2UpdateIndex = sampleIndex;
}

static void AFE_processPair(uint32_t redSample, uint32_t irSample)
{
    int32_t redAc;
    int32_t irAc;
    int32_t filteredRed;
    int32_t filteredIr;

    if (sampleIndex == 0U) {
        redDcEstimate = (int32_t)redSample;
        irDcEstimate = (int32_t)irSample;
    }

    redDcEstimate += ((int32_t)redSample - redDcEstimate) >> AFE_DC_SHIFT;
    irDcEstimate += ((int32_t)irSample - irDcEstimate) >> AFE_DC_SHIFT;

    redAc = (int32_t)redSample - redDcEstimate;
    irAc = (int32_t)irSample - irDcEstimate;

    redSignal += (redAc - redSignal) >> AFE_SIGNAL_SHIFT;
    irSignal += (irAc - irSignal) >> AFE_SIGNAL_SHIFT;
    irEnvelope += (AFE_abs32(irSignal) - irEnvelope) >> AFE_ENVELOPE_SHIFT;

    filteredRed = redSignal;
    filteredIr = irSignal;
    afeLastRedSample = redSample;
    afeLastIrSample = irSample;

    if (filteredRed > 32767) {
        filteredRed = 32767;
    } else if (filteredRed < -32768) {
        filteredRed = -32768;
    }

    if (filteredIr > 32767) {
        filteredIr = 32767;
    } else if (filteredIr < -32768) {
        filteredIr = -32768;
    }

    AFE_storeWindowSample(redSample, irSample, (int16_t)filteredRed, (int16_t)filteredIr);

    sampleIndex++;
    AFE_updateHeartRate(filteredIr);
    AFE_updateSpO2();
}

/* MAX86140 Helper Functions */
static void MAX86140_writeReg(uint8_t reg, uint8_t data)
{
    SPI_Transaction transaction = { 0 };
    uint8_t txBuffer[3];

    txBuffer[0] = reg;
    txBuffer[1] = 0x00;
    txBuffer[2] = data;

    transaction.count = sizeof(txBuffer);
    transaction.txBuf = (void *)txBuffer;
    transaction.rxBuf = NULL;

    GPIO_write(AFE_GPIO_CS_PIN, 0);
    SPI_transfer(spiAfeHandle, &transaction);
    GPIO_write(AFE_GPIO_CS_PIN, 1);
}

static uint8_t MAX86140_readReg(uint8_t reg)
{
    SPI_Transaction transaction = { 0 };
    uint8_t txBuffer[3];
    uint8_t rxBuffer[3];

    txBuffer[0] = reg;
    txBuffer[1] = 0x80;
    txBuffer[2] = 0x00;

    transaction.count = sizeof(txBuffer);
    transaction.txBuf = (void *)txBuffer;
    transaction.rxBuf = (void *)rxBuffer;

    GPIO_write(AFE_GPIO_CS_PIN, 0);
    SPI_transfer(spiAfeHandle, &transaction);
    GPIO_write(AFE_GPIO_CS_PIN, 1);

    return rxBuffer[2];
}

static uint8_t MAX86140_readFifoBurst(uint8_t itemCount, uint8_t *buffer)
{
    SPI_Transaction transaction = { 0 };
    uint16_t transferCount;

    if ((itemCount == 0U) || (itemCount > AFE_FIFO_MAX_ITEMS)) {
        return 0U;
    }

    transferCount = (uint16_t)(2U + ((uint16_t)itemCount * 3U));
    fifoTxBuffer[0] = MAX86140_FIFO_DATA;
    fifoTxBuffer[1] = 0x80;

    GPIO_write(AFE_GPIO_CS_PIN, 0);
    transaction.count = transferCount;
    transaction.txBuf = (void *)fifoTxBuffer;
    transaction.rxBuf = (void *)fifoRxBuffer;
    SPI_transfer(spiAfeHandle, &transaction);
    GPIO_write(AFE_GPIO_CS_PIN, 1);

    memcpy(buffer, &fifoRxBuffer[2], (size_t)itemCount * 3U);
    return itemCount;
}

static void afe_gpio_callback(uint_least8_t index)
{
    (void)index;
    afeDataReady = true;
    afeInterruptCount++;
    afeStatusFlags |= AFE_STATUS_INT_SEEN;
}

bool AFE_init(void)
{
    SPI_Params spiParams;

    GPIO_setConfig(AFE_GPIO_CS_PIN, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_write(AFE_GPIO_CS_PIN, 1);

    SPI_Params_init(&spiParams);
    spiParams.bitRate = 1000000;
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.mode = SPI_CONTROLLER;

    spiAfeHandle = SPI_open(CONFIG_SPI_AFE, &spiParams);
    if (spiAfeHandle == NULL) {
        return false;
    }

    afeStatusFlags = AFE_STATUS_SPI_OPEN;

    GPIO_setConfig(AFE_GPIO_INT_PIN, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(AFE_GPIO_INT_PIN, afe_gpio_callback);
    GPIO_enableInt(AFE_GPIO_INT_PIN);

    MAX86140_writeReg(MAX86140_SYS_CTRL, 0x01);
    usleep(10000);

    afePartId = MAX86140_readReg(MAX86140_PART_ID);
    if ((afePartId == 0x24U) || (afePartId == 0x25U)) {
        afeStatusFlags |= AFE_STATUS_PART_OK;
    } else {
        GPIO_disableInt(AFE_GPIO_INT_PIN);
        GPIO_setCallback(AFE_GPIO_INT_PIN, NULL);
        SPI_close(spiAfeHandle);
        spiAfeHandle = NULL;
        afeStatusFlags = 0U;
        return false;
    }

    MAX86140_writeReg(MAX86140_PPG_CFG1, 0x1A);
    MAX86140_writeReg(MAX86140_PPG_CFG2, 0x00);
    MAX86140_writeReg(MAX86140_LED_SEQ1, 0x21);
    MAX86140_writeReg(MAX86140_LED_SEQ2, 0x00);
    MAX86140_writeReg(MAX86140_LED1_PA, 0x33);
    MAX86140_writeReg(MAX86140_LED2_PA, 0x33);

    MAX86140_writeReg(MAX86140_FIFO_WR_PTR, 0x00);
    MAX86140_writeReg(MAX86140_FIFO_RD_PTR, 0x00);
    MAX86140_writeReg(MAX86140_OVF_COUNTER, 0x00);
    MAX86140_writeReg(MAX86140_FIFO_DATA_CNTR, 0x00);

    MAX86140_writeReg(MAX86140_INT_EN_1, 0x80);
    MAX86140_writeReg(MAX86140_SYS_CTRL, 0x04);

    AFE_resetState();
    return true;
}

void AFE_processSamples(void)
{
    uint8_t status;
    uint8_t numItems;
    uint8_t itemIndex;

    numItems = MAX86140_readReg(MAX86140_FIFO_DATA_CNTR);
    afeLastFifoCount = numItems;
    if (numItems < AFE_FIFO_MIN_ITEMS) {
        if (emptyPollCount < AFE_EMPTY_POLL_TIMEOUT) {
            emptyPollCount++;
        } else {
            current_hr = 0U;
            current_spo2 = 0U;
        }
        afeStatusFlags &= (uint8_t)(~AFE_STATUS_FIFO_ACTIVE);
        if (afeDataReady) {
            afeDataReady = false;
            status = MAX86140_readReg(MAX86140_INT_STAT_1);
            (void)status;
        }
        return;
    }

    emptyPollCount = 0;
    afeDataReady = false;
    afeStatusFlags |= AFE_STATUS_FIFO_ACTIVE;
    status = MAX86140_readReg(MAX86140_INT_STAT_1);
    (void)status;

    if (numItems > AFE_FIFO_MAX_ITEMS) {
        numItems = AFE_FIFO_MAX_ITEMS;
    }

    numItems &= 0xFEU;
    if (numItems == 0U) {
        return;
    }

    if (MAX86140_readFifoBurst(numItems, rawFifoDataBuffer) != numItems) {
        return;
    }

    for (itemIndex = 0U; itemIndex < numItems; itemIndex += 2U) {
        uint8_t redOffset = (uint8_t)(itemIndex * 3U);
        uint8_t irOffset = (uint8_t)((itemIndex + 1U) * 3U);
        uint32_t redSample = ((uint32_t)rawFifoDataBuffer[redOffset] << 16) |
                             ((uint32_t)rawFifoDataBuffer[redOffset + 1U] << 8) |
                             rawFifoDataBuffer[redOffset + 2U];
        uint32_t irSample = ((uint32_t)rawFifoDataBuffer[irOffset] << 16) |
                            ((uint32_t)rawFifoDataBuffer[irOffset + 1U] << 8) |
                            rawFifoDataBuffer[irOffset + 2U];

        redSample &= 0x07FFFFU;
        irSample &= 0x07FFFFU;

        AFE_processPair(redSample, irSample);
    }
}

uint8_t AFE_getHeartRate(void)
{
    return current_hr;
}

uint8_t AFE_getSpO2(void)
{
    return current_spo2;
}

uint32_t AFE_getInterruptCount(void)
{
    return afeInterruptCount;
}

uint8_t AFE_getPartId(void)
{
    return afePartId;
}

uint8_t AFE_getLastFifoCount(void)
{
    return afeLastFifoCount;
}

uint8_t AFE_getStatus(void)
{
    return afeStatusFlags;
}

uint32_t AFE_getLastRedSample(void)
{
    return afeLastRedSample;
}

uint32_t AFE_getLastIrSample(void)
{
    return afeLastIrSample;
}

uint32_t AFE_getSampleCount(void)
{
    return sampleIndex;
}
