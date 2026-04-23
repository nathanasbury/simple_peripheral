#include <string.h>

#include <icall.h>
#include "util.h"
#include <icall_ble_api.h>

#include "band_profile.h"

#define BAND_NUM_ATTR_SUPPORTED 17

static bandProfileCBs_t *bandProfile_AppCBs = NULL;

static CONST uint8 bandProfileServUUID[ATT_BT_UUID_SIZE] =
{
    LO_UINT16(BAND_SERV_UUID), HI_UINT16(BAND_SERV_UUID)
};

static CONST uint8 bandProfileHrUUID[ATT_BT_UUID_SIZE] =
{
    LO_UINT16(BAND_HR_UUID), HI_UINT16(BAND_HR_UUID)
};

static CONST uint8 bandProfileSpO2UUID[ATT_BT_UUID_SIZE] =
{
    LO_UINT16(BAND_SPO2_UUID), HI_UINT16(BAND_SPO2_UUID)
};

static CONST uint8 bandProfileStepsUUID[ATT_BT_UUID_SIZE] =
{
    LO_UINT16(BAND_STEPS_UUID), HI_UINT16(BAND_STEPS_UUID)
};

static CONST uint8 bandProfileFallUUID[ATT_BT_UUID_SIZE] =
{
    LO_UINT16(BAND_FALL_UUID), HI_UINT16(BAND_FALL_UUID)
};

static CONST gattAttrType_t bandProfileService = { ATT_BT_UUID_SIZE, bandProfileServUUID };

static uint8 bandProfileHrProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static uint8 bandProfileSpO2Props = GATT_PROP_READ | GATT_PROP_NOTIFY;
static uint8 bandProfileStepsProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
static uint8 bandProfileFallProps = GATT_PROP_READ | GATT_PROP_NOTIFY;

static uint8 bandProfileHrVal = 0;
static uint8 bandProfileSpO2Val = 0;
static uint8 bandProfileStepsVal[BAND_STEPS_LEN] = { 0 };
static uint8 bandProfileFallVal = 0;

static gattCharCfg_t *bandProfileHrConfig;
static gattCharCfg_t *bandProfileSpO2Config;
static gattCharCfg_t *bandProfileStepsConfig;
static gattCharCfg_t *bandProfileFallConfig;

static uint8 bandProfileHrUserDesc[] = "Heart Rate";
static uint8 bandProfileSpO2UserDesc[] = "SpO2";
static uint8 bandProfileStepsUserDesc[] = "Steps";
static uint8 bandProfileFallUserDesc[] = "Fall Alert";

static gattAttribute_t bandProfileAttrTbl[BAND_NUM_ATTR_SUPPORTED] =
{
    {
        { ATT_BT_UUID_SIZE, primaryServiceUUID },
        GATT_PERMIT_READ,
        0,
        (uint8 *)&bandProfileService
    },

    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &bandProfileHrProps
    },
    {
        { ATT_BT_UUID_SIZE, bandProfileHrUUID },
        GATT_PERMIT_READ,
        0,
        &bandProfileHrVal
    },
    {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&bandProfileHrConfig
    },
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        bandProfileHrUserDesc
    },

    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &bandProfileSpO2Props
    },
    {
        { ATT_BT_UUID_SIZE, bandProfileSpO2UUID },
        GATT_PERMIT_READ,
        0,
        &bandProfileSpO2Val
    },
    {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&bandProfileSpO2Config
    },
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        bandProfileSpO2UserDesc
    },

    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &bandProfileStepsProps
    },
    {
        { ATT_BT_UUID_SIZE, bandProfileStepsUUID },
        GATT_PERMIT_READ,
        0,
        bandProfileStepsVal
    },
    {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&bandProfileStepsConfig
    },
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        bandProfileStepsUserDesc
    },

    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &bandProfileFallProps
    },
    {
        { ATT_BT_UUID_SIZE, bandProfileFallUUID },
        GATT_PERMIT_READ,
        0,
        &bandProfileFallVal
    },
    {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&bandProfileFallConfig
    },
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        bandProfileFallUserDesc
    },
};

static bStatus_t BandProfile_ReadAttrCB(uint16 connHandle,
                                        gattAttribute_t *pAttr,
                                        uint8 *pValue,
                                        uint16 *pLen,
                                        uint16 offset,
                                        uint16 maxLen,
                                        uint8 method);
static bStatus_t BandProfile_WriteAttrCB(uint16 connHandle,
                                         gattAttribute_t *pAttr,
                                         uint8 *pValue,
                                         uint16 len,
                                         uint16 offset,
                                         uint8 method);

static CONST gattServiceCBs_t bandProfileCBs =
{
    BandProfile_ReadAttrCB,
    BandProfile_WriteAttrCB,
    NULL
};

static bStatus_t BandProfile_readValue(uint8 *pSrc, uint16 valueLen,
                                       uint8 *pDst, uint16 *pLen,
                                       uint16 offset, uint16 maxLen)
{
    if (offset > valueLen)
    {
        *pLen = 0;
        return ATT_ERR_INVALID_OFFSET;
    }

    *pLen = MIN(maxLen, valueLen - offset);
    VOID memcpy(pDst, pSrc + offset, *pLen);

    return SUCCESS;
}

bStatus_t BandProfile_AddService(uint32_t services)
{
    uint8 status = SUCCESS;

    bandProfileHrConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) * linkDBNumConns);
    bandProfileSpO2Config = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) * linkDBNumConns);
    bandProfileStepsConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) * linkDBNumConns);
    bandProfileFallConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) * linkDBNumConns);

    if ((bandProfileHrConfig == NULL) || (bandProfileSpO2Config == NULL) ||
        (bandProfileStepsConfig == NULL) || (bandProfileFallConfig == NULL))
    {
        return bleMemAllocError;
    }

    GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, bandProfileHrConfig);
    GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, bandProfileSpO2Config);
    GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, bandProfileStepsConfig);
    GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, bandProfileFallConfig);

    if (services & BAND_PROFILE_SERVICE)
    {
        status = GATTServApp_RegisterService(bandProfileAttrTbl,
                                             GATT_NUM_ATTRS(bandProfileAttrTbl),
                                             GATT_MAX_ENCRYPT_KEY_SIZE,
                                             &bandProfileCBs);
    }

    return status;
}

bStatus_t BandProfile_RegisterAppCBs(bandProfileCBs_t *appCallbacks)
{
    if (appCallbacks == NULL)
    {
        return INVALIDPARAMETER;
    }

    bandProfile_AppCBs = appCallbacks;
    return SUCCESS;
}

bStatus_t BandProfile_SetParameter(uint8_t param, uint8_t len, void *value)
{
    bStatus_t ret = SUCCESS;
    gattCharCfg_t *pCharCfg = NULL;
    uint8 *pAttrValue = NULL;

    switch (param)
    {
        case BAND_PROFILE_HR:
            if (len == BAND_HR_LEN)
            {
                bandProfileHrVal = *((uint8 *)value);
                pCharCfg = bandProfileHrConfig;
                pAttrValue = &bandProfileHrVal;
            }
            else
            {
                ret = bleInvalidRange;
            }
            break;

        case BAND_PROFILE_SPO2:
            if (len == BAND_SPO2_LEN)
            {
                bandProfileSpO2Val = *((uint8 *)value);
                pCharCfg = bandProfileSpO2Config;
                pAttrValue = &bandProfileSpO2Val;
            }
            else
            {
                ret = bleInvalidRange;
            }
            break;

        case BAND_PROFILE_STEPS:
            if (len == BAND_STEPS_LEN)
            {
                VOID memcpy(bandProfileStepsVal, value, BAND_STEPS_LEN);
                pCharCfg = bandProfileStepsConfig;
                pAttrValue = bandProfileStepsVal;
            }
            else
            {
                ret = bleInvalidRange;
            }
            break;

        case BAND_PROFILE_FALL:
            if (len == BAND_FALL_LEN)
            {
                bandProfileFallVal = *((uint8 *)value);
                pCharCfg = bandProfileFallConfig;
                pAttrValue = &bandProfileFallVal;
            }
            else
            {
                ret = bleInvalidRange;
            }
            break;

        default:
            ret = INVALIDPARAMETER;
            break;
    }

    if ((ret == SUCCESS) && (pCharCfg != NULL))
    {
        GATTServApp_ProcessCharCfg(pCharCfg, pAttrValue, FALSE,
                                   bandProfileAttrTbl,
                                   GATT_NUM_ATTRS(bandProfileAttrTbl),
                                   INVALID_TASK_ID,
                                   BandProfile_ReadAttrCB);
    }

    return ret;
}

bStatus_t BandProfile_GetParameter(uint8_t param, void *value)
{
    switch (param)
    {
        case BAND_PROFILE_HR:
            *((uint8 *)value) = bandProfileHrVal;
            return SUCCESS;

        case BAND_PROFILE_SPO2:
            *((uint8 *)value) = bandProfileSpO2Val;
            return SUCCESS;

        case BAND_PROFILE_STEPS:
            VOID memcpy(value, bandProfileStepsVal, BAND_STEPS_LEN);
            return SUCCESS;

        case BAND_PROFILE_FALL:
            *((uint8 *)value) = bandProfileFallVal;
            return SUCCESS;

        default:
            return INVALIDPARAMETER;
    }
}

static bStatus_t BandProfile_ReadAttrCB(uint16 connHandle,
                                        gattAttribute_t *pAttr,
                                        uint8 *pValue,
                                        uint16 *pLen,
                                        uint16 offset,
                                        uint16 maxLen,
                                        uint8 method)
{
    uint16 uuid;

    VOID connHandle;
    VOID method;

    if (pAttr->type.len != ATT_BT_UUID_SIZE)
    {
        *pLen = 0;
        return ATT_ERR_INVALID_HANDLE;
    }

    uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

    switch (uuid)
    {
        case BAND_HR_UUID:
        case BAND_SPO2_UUID:
        case BAND_FALL_UUID:
            return BandProfile_readValue(pAttr->pValue, 1, pValue, pLen, offset, maxLen);

        case BAND_STEPS_UUID:
            return BandProfile_readValue(pAttr->pValue, BAND_STEPS_LEN, pValue, pLen, offset, maxLen);

        case GATT_CHAR_USER_DESC_UUID:
            return BandProfile_readValue(pAttr->pValue,
                                         (uint16)strlen((char *)pAttr->pValue),
                                         pValue, pLen, offset, maxLen);

        default:
            *pLen = 0;
            return ATT_ERR_ATTR_NOT_FOUND;
    }
}

static bStatus_t BandProfile_WriteAttrCB(uint16 connHandle,
                                         gattAttribute_t *pAttr,
                                         uint8 *pValue,
                                         uint16 len,
                                         uint16 offset,
                                         uint8 method)
{
    uint16 uuid;

    VOID method;

    if (pAttr->type.len != ATT_BT_UUID_SIZE)
    {
        return ATT_ERR_INVALID_HANDLE;
    }

    uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

    if (uuid == GATT_CLIENT_CHAR_CFG_UUID)
    {
        return GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                              offset, GATT_CLIENT_CFG_NOTIFY);
    }

    return ATT_ERR_ATTR_NOT_FOUND;
}
