################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Drivers/NV/crc.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/common/nv/crc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"Drivers/NV/crc.d_raw" -MT"Drivers/NV/crc.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Drivers/NV/nvocmp.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/common/nv/nvocmp.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"Drivers/NV/nvocmp.d_raw" -MT"Drivers/NV/nvocmp.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


