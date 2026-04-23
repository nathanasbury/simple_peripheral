/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the LP_EM_CC2674P10
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_LP_EM_CC2674P10
#ifndef DeviceFamily_CC26X4
#define DeviceFamily_CC26X4
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== CCFG ========
 */


/*
 *  ======== AESCCM ========
 */

extern const uint_least8_t                  CONFIG_AESCCM0_CONST;
#define CONFIG_AESCCM0                      0
#define CONFIG_TI_DRIVERS_AESCCM_COUNT      1


/*
 *  ======== AESCTRDRBG ========
 */

extern const uint_least8_t                      CONFIG_AESCTRDRBG_0_CONST;
#define CONFIG_AESCTRDRBG_0                     0
#define CONFIG_TI_DRIVERS_AESCTRDRBG_COUNT      1


/*
 *  ======== AESECB ========
 */

extern const uint_least8_t                  CONFIG_AESECB0_CONST;
#define CONFIG_AESECB0                      0
#define CONFIG_TI_DRIVERS_AESECB_COUNT      1


/*
 *  ======== ECDH ========
 */

extern const uint_least8_t              CONFIG_ECDH0_CONST;
#define CONFIG_ECDH0                    0
#define CONFIG_TI_DRIVERS_ECDH_COUNT    1


/*
 *  ======== GPIO ========
 */
extern const uint_least8_t CONFIG_GPIO_BTN1_CONST;
#define CONFIG_GPIO_BTN1 15

extern const uint_least8_t CONFIG_GPIO_BTN2_CONST;
#define CONFIG_GPIO_BTN2 20

extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
#define CONFIG_GPIO_LED_0 22

extern const uint_least8_t CONFIG_GPIO_ADXL_INT_CONST;
#define CONFIG_GPIO_ADXL_INT 10

extern const uint_least8_t CONFIG_GPIO_ADXL_CS_CONST;
#define CONFIG_GPIO_ADXL_CS 11

extern const uint_least8_t CONFIG_GPIO_AFE_INT_CONST;
#define CONFIG_GPIO_AFE_INT 18

extern const uint_least8_t CONFIG_GPIO_AFE_CS_CONST;
#define CONFIG_GPIO_AFE_CS 21

/* Owned by CONFIG_SPI_ACCEL as  */
extern const uint_least8_t CONFIG_GPIO_SPI_ACCEL_SCLK_CONST;
#define CONFIG_GPIO_SPI_ACCEL_SCLK 14

/* Owned by CONFIG_SPI_ACCEL as  */
extern const uint_least8_t CONFIG_GPIO_SPI_ACCEL_POCI_CONST;
#define CONFIG_GPIO_SPI_ACCEL_POCI 12

/* Owned by CONFIG_SPI_ACCEL as  */
extern const uint_least8_t CONFIG_GPIO_SPI_ACCEL_PICO_CONST;
#define CONFIG_GPIO_SPI_ACCEL_PICO 13

/* Owned by CONFIG_SPI_AFE as  */
extern const uint_least8_t CONFIG_GPIO_SPI_AFE_SCLK_CONST;
#define CONFIG_GPIO_SPI_AFE_SCLK 7

/* Owned by CONFIG_SPI_AFE as  */
extern const uint_least8_t CONFIG_GPIO_SPI_AFE_POCI_CONST;
#define CONFIG_GPIO_SPI_AFE_POCI 6

/* Owned by CONFIG_SPI_AFE as  */
extern const uint_least8_t CONFIG_GPIO_SPI_AFE_PICO_CONST;
#define CONFIG_GPIO_SPI_AFE_PICO 5

/* Owned by /ti/drivers/RF as  */
extern const uint_least8_t CONFIG_RF_24GHZ_CONST;
#define CONFIG_RF_24GHZ 28

/* Owned by /ti/drivers/RF as  */
extern const uint_least8_t CONFIG_RF_HIGH_PA_CONST;
#define CONFIG_RF_HIGH_PA 29

/* The range of pins available on this device */
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== NVS ========
 */

extern const uint_least8_t              CONFIG_NVSINTERNAL_CONST;
#define CONFIG_NVSINTERNAL              0
#define CONFIG_TI_DRIVERS_NVS_COUNT     1




/*
 *  ======== SPI ========
 */

/*
 *  PICO: DIO13
 *  POCI: DIO12
 *  SCLK: DIO14
 */
extern const uint_least8_t              CONFIG_SPI_ACCEL_CONST;
#define CONFIG_SPI_ACCEL                0
/*
 *  PICO: DIO5
 *  POCI: DIO6
 *  SCLK: DIO7
 */
extern const uint_least8_t              CONFIG_SPI_AFE_CONST;
#define CONFIG_SPI_AFE                  1
#define CONFIG_TI_DRIVERS_SPI_COUNT     2


/*
 *  ======== TRNG ========
 */

extern const uint_least8_t              CONFIG_TRNG_0_CONST;
#define CONFIG_TRNG_0                   0
#define CONFIG_TI_DRIVERS_TRNG_COUNT    1


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
