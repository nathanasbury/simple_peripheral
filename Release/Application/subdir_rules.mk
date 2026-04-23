################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Application/board_key.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/common/cc26xx/board_key.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"Application/board_key.d_raw" -MT"Application/board_key.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Application/rcosc_calibration.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/common/cc26xx/rcosc/rcosc_calibration.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"Application/rcosc_calibration.d_raw" -MT"Application/rcosc_calibration.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Application/%.o: ../Application/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"Application/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Application/two_btn_menu.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/common/cc26xx/menu/two_btn_menu.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"Application/two_btn_menu.d_raw" -MT"Application/two_btn_menu.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


