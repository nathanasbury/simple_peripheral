################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-2124840782: ../simple_peripheral.syscfg
	@echo 'SysConfig - building file: "$<"'
	"C:/ti/sysconfig_1.21.1/sysconfig_cli.bat" -s "C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/.metadata/product.json" --script "C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/simple_peripheral.syscfg" -o "syscfg" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_ble_config.h: build-2124840782 ../simple_peripheral.syscfg
syscfg/ti_ble_config.c: build-2124840782
syscfg/ti_devices_config.c: build-2124840782
syscfg/ti_radio_config.c: build-2124840782
syscfg/ti_radio_config.h: build-2124840782
syscfg/ti_drivers_config.c: build-2124840782
syscfg/ti_drivers_config.h: build-2124840782
syscfg/ti_utils_build_linker.cmd.genlibs: build-2124840782
syscfg/ti_utils_build_linker.cmd.genmap: build-2124840782
syscfg/ti_utils_build_compiler.opt: build-2124840782
syscfg/syscfg_c.rov.xs: build-2124840782
syscfg/ti_sysbios_config.h: build-2124840782
syscfg/ti_sysbios_config.c: build-2124840782
syscfg: build-2124840782

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


