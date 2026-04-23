#ifndef ADXL366_H
#define ADXL366_H

#include <stdint.h>
#include <stdbool.h>

/* Initialize SPI interface for ADXL366 */
bool ADXL366_init(void);

/* Check if a fall interrupt was triggered */
bool ADXL366_isFallDetected(void);

/* Increment internally tracked steps */
void ADXL366_updateSteps(void);
uint32_t ADXL366_getSteps(void);
void ADXL366_getLastSample(int16_t *xData, int16_t *yData, int16_t *zData);
uint32_t ADXL366_getLastDynamicMagnitude(void);
uint32_t ADXL366_getBaselineMagnitude(void);
uint32_t ADXL366_getInterruptCount(void);
uint8_t ADXL366_getPartId(void);
uint8_t ADXL366_getStatus(void);

#define ADXL366_STATUS_SPI_OPEN     0x01
#define ADXL366_STATUS_PART_OK      0x02
#define ADXL366_STATUS_SAMPLE_SEEN  0x04
#define ADXL366_STATUS_INT_SEEN     0x08

#endif /* ADXL366_H */
