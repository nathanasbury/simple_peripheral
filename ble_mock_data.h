#ifndef BLE_MOCK_DATA_H
#define BLE_MOCK_DATA_H

#include <stdint.h>
#include <stdbool.h>

/* Initialize the mock data simulator */
void BLE_MockData_init(void);

/* Generates new randomized testing values */
void BLE_MockData_generate(void);

/* Getters to mimic sensor outputs */
uint8_t BLE_MockData_getHeartRate(void);
uint8_t BLE_MockData_getSpO2(void);
uint32_t BLE_MockData_getSteps(void);
bool    BLE_MockData_isFallDetected(void);

#endif /* BLE_MOCK_DATA_H */