################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
NPI/npi_frame_hci.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/npi/src/npi_frame_hci.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"NPI/npi_frame_hci.d_raw" -MT"NPI/npi_frame_hci.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

NPI/npi_rxbuf.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/npi/src/npi_rxbuf.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"NPI/npi_rxbuf.d_raw" -MT"NPI/npi_rxbuf.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

NPI/npi_task.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/npi/src/npi_task.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"NPI/npi_task.d_raw" -MT"NPI/npi_task.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

NPI/npi_tl.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/npi/src/npi_tl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"NPI/npi_tl.d_raw" -MT"NPI/npi_tl.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

NPI/npi_tl_uart.o: C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/npi/src/npi_tl_uart.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt" @"ccsIncludes.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -MMD -MP -MF"NPI/npi_tl_uart.d_raw" -MT"NPI/npi_tl_uart.o" -I"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


