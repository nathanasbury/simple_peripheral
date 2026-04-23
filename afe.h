#ifndef AFE_H
#define AFE_H

#include <stdint.h>
#include <stdbool.h>

/* Initialize SPI for the given AFE (MAX86140) */
bool AFE_init(void);

/* Process optical samples periodically */
void AFE_processSamples(void);

/* Retrieve last computed Heart Rate */
uint8_t AFE_getHeartRate(void);

/* Retrieve last computed SpO2 */
uint8_t AFE_getSpO2(void);
uint32_t AFE_getInterruptCount(void);
uint8_t AFE_getPartId(void);
uint8_t AFE_getLastFifoCount(void);
uint8_t AFE_getStatus(void);
uint32_t AFE_getLastRedSample(void);
uint32_t AFE_getLastIrSample(void);
uint32_t AFE_getSampleCount(void);

#define AFE_STATUS_SPI_OPEN      0x01
#define AFE_STATUS_PART_OK       0x02
#define AFE_STATUS_FIFO_ACTIVE   0x04
#define AFE_STATUS_INT_SEEN      0x08

#endif /* AFE_H */
