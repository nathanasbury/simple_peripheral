#ifndef BAND_PROFILE_H
#define BAND_PROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <bcomdef.h>
#include <stdint.h>

/* UUIDs */
#define BAND_SERV_UUID        0xFFF0
#define BAND_HR_UUID          0xFFF1
#define BAND_SPO2_UUID        0xFFF2
#define BAND_STEPS_UUID       0xFFF3
#define BAND_FALL_UUID        0xFFF4

/* Profile Parameters */
#define BAND_PROFILE_SERVICE  0x00000001
#define BAND_PROFILE_HR       0
#define BAND_PROFILE_SPO2     1
#define BAND_PROFILE_STEPS    2
#define BAND_PROFILE_FALL     3

/* Lengths */
#define BAND_HR_LEN           1
#define BAND_SPO2_LEN         1
#define BAND_STEPS_LEN        4
#define BAND_FALL_LEN         1

typedef void (*bandProfileChange_t)(uint8_t paramID);

typedef struct
{
    bandProfileChange_t pfnChangeCb;
} bandProfileCBs_t;

extern bStatus_t BandProfile_AddService(uint32_t services);
extern bStatus_t BandProfile_RegisterAppCBs(bandProfileCBs_t *appCallbacks);
extern bStatus_t BandProfile_SetParameter(uint8_t param, uint8_t len, void *value);
extern bStatus_t BandProfile_GetParameter(uint8_t param, void *value);

#ifdef __cplusplus
}
#endif

#endif /* BAND_PROFILE_H */
