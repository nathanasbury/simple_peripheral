#include "ble_mock_data.h"
#include <stdlib.h>

static uint8_t mockHR;
static uint8_t mockSpO2;
static uint32_t mockSteps;
static bool mockFall;
static uint8_t fallTimer;

void BLE_MockData_init(void)
{
    mockHR = 72;        // Start at a healthy 72 bpm
    mockSpO2 = 98;      // Start at 98%
    mockSteps = 0;      // Start at 0 steps
    mockFall = false;
    fallTimer = 0;
}

void BLE_MockData_generate(void)
{
    /* 1. Simulate Heart Rate (Wander vaguely between 60 - 120 bpm) */
    int hrShift = (rand() % 5) - 2; // -2 to +2 change
    mockHR = (uint8_t)((int)mockHR + hrShift);
    
    if (mockHR < 60) mockHR = 60;
    if (mockHR > 180) mockHR = 180;

    /* 2. Simulate SpO2 (Healthy person normally stuck at 96-100) */
    int spo2Shift = (rand() % 3) - 1; // -1 to +1 change
    mockSpO2 = (uint8_t)((int)mockSpO2 + spo2Shift);
    
    if (mockSpO2 < 92) mockSpO2 = 92;
    if (mockSpO2 > 100) mockSpO2 = 100;

    /* 3. Simulate Steps (Just steadily increment 1 to 3 steps a second) */
    mockSteps += (rand() % 4);

    /* 4. Simulate a Fall Event randomly every ~15 seconds */
    fallTimer++;
    if (fallTimer > 15) {
        mockFall = true;
        fallTimer = 0;
    } else {
        mockFall = false;
    }
}

uint8_t BLE_MockData_getHeartRate(void) { return mockHR; }
uint8_t BLE_MockData_getSpO2(void) { return mockSpO2; }
uint32_t BLE_MockData_getSteps(void) { return mockSteps; }
bool BLE_MockData_isFallDetected(void) { return mockFall; }
