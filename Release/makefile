################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ti_cgt_arm_llvm_3.2.2.LTS

GEN_OPTS__FLAG := @"syscfg/ti_utils_build_compiler.opt" 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./adxl366.o" \
"./afe.o" \
"./band_profile.o" \
"./ble_mock_data.o" \
"./syscfg/ti_ble_config.o" \
"./syscfg/ti_devices_config.o" \
"./syscfg/ti_radio_config.o" \
"./syscfg/ti_drivers_config.o" \
"./syscfg/ti_sysbios_config.o" \
"./Application/board_key.o" \
"./Application/rcosc_calibration.o" \
"./Application/simple_peripheral.o" \
"./Application/simple_peripheral_menu.o" \
"./Application/two_btn_menu.o" \
"./Drivers/NV/crc.o" \
"./Drivers/NV/nvocmp.o" \
"./NPI/npi_frame_hci.o" \
"./NPI/npi_rxbuf.o" \
"./NPI/npi_task.o" \
"./NPI/npi_tl.o" \
"./NPI/npi_tl_uart.o" \
"./Profiles/devinfoservice.o" \
"./Profiles/simple_gatt_profile.o" \
"./Startup/main.o" \
"./Startup/osal_icall_ble.o" \
"./Startup/rom_init.o" \
"./common/Util/util.o" \
"./iCall/icall.o" \
"./iCall/icall_cc2650.o" \
"./iCall/icall_user_config.o" \
"./iCallBLE/ble_user_config.o" \
"./iCallBLE/ble_user_config_stack.o" \
"./iCallBLE/icall_api_lite.o" \
"./iCallBLE/icall_hci_tl.o" \
"../cc13x4_cc26x4_app.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-lti_utils_build_linker.cmd.genlibs \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Application/subdir_vars.mk
-include Drivers/NV/subdir_vars.mk
-include NPI/subdir_vars.mk
-include Profiles/subdir_vars.mk
-include Startup/subdir_vars.mk
-include common/Util/subdir_vars.mk
-include iCall/subdir_vars.mk
-include iCallBLE/subdir_vars.mk
-include subdir_rules.mk
-include Application/subdir_rules.mk
-include Drivers/NV/subdir_rules.mk
-include NPI/subdir_rules.mk
-include Profiles/subdir_rules.mk
-include Startup/subdir_rules.mk
-include common/Util/subdir_rules.mk
-include iCall/subdir_rules.mk
-include iCallBLE/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
simple_peripheral.out 

EXE_OUTPUTS__QUOTED += \
"simple_peripheral.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "simple_peripheral.out"

# Tool invocations
simple_peripheral.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Arm Linker - building target: "$@"'
	"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmclang.exe" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source/ti/ble5stack_flash/config/factory_config.opt"  -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mlittle-endian -Oz -DCC13XX -DCC13X4 -DFLASH_ONLY_BUILD -DNVOCMP_NWSAMEITEM=1 -DNVOCMP_NVPAGES=4 -Dxdc_target__isaCompatible_v8M -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -DCC13X2P -gdwarf-3 -Wl,-m"simple_peripheral.map" -Wl,-i"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source" -Wl,-i"C:/ti/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/kernel/tirtos7/packages" -Wl,-i"C:/Users/asbur/workspace_ccstheia/New folder/simple_peripheral/Release/syscfg" -Wl,-i"C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/lib" -Wl,--reread_libs -Wl,--define=FLASH_ONLY_BUILD=2 -Wl,--diag_suppress=16002-D -Wl,--diag_suppress=10247-D -Wl,--diag_suppress=10325-D -Wl,--diag_suppress=10229-D -Wl,--diag_suppress=16032-D -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="simple_peripheral_linkInfo.xml" -Wl,--rom_model -o "simple_peripheral.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "adxl366.o" "afe.o" "band_profile.o" "ble_mock_data.o" "syscfg\ti_ble_config.o" "syscfg\ti_devices_config.o" "syscfg\ti_radio_config.o" "syscfg\ti_drivers_config.o" "syscfg\ti_sysbios_config.o" "Application\board_key.o" "Application\rcosc_calibration.o" "Application\simple_peripheral.o" "Application\simple_peripheral_menu.o" "Application\two_btn_menu.o" "Drivers\NV\crc.o" "Drivers\NV\nvocmp.o" "NPI\npi_frame_hci.o" "NPI\npi_rxbuf.o" "NPI\npi_task.o" "NPI\npi_tl.o" "NPI\npi_tl_uart.o" "Profiles\devinfoservice.o" "Profiles\simple_gatt_profile.o" "Startup\main.o" "Startup\osal_icall_ble.o" "Startup\rom_init.o" "common\Util\util.o" "iCall\icall.o" "iCall\icall_cc2650.o" "iCall\icall_user_config.o" "iCallBLE\ble_user_config.o" "iCallBLE\ble_user_config_stack.o" "iCallBLE\icall_api_lite.o" "iCallBLE\icall_hci_tl.o" 
	-$(RM) "adxl366.d" "afe.d" "band_profile.d" "ble_mock_data.d" "syscfg\ti_ble_config.d" "syscfg\ti_devices_config.d" "syscfg\ti_radio_config.d" "syscfg\ti_drivers_config.d" "syscfg\ti_sysbios_config.d" "Application\board_key.d" "Application\rcosc_calibration.d" "Application\simple_peripheral.d" "Application\simple_peripheral_menu.d" "Application\two_btn_menu.d" "Drivers\NV\crc.d" "Drivers\NV\nvocmp.d" "NPI\npi_frame_hci.d" "NPI\npi_rxbuf.d" "NPI\npi_task.d" "NPI\npi_tl.d" "NPI\npi_tl_uart.d" "Profiles\devinfoservice.d" "Profiles\simple_gatt_profile.d" "Startup\main.d" "Startup\osal_icall_ble.d" "Startup\rom_init.d" "common\Util\util.d" "iCall\icall.d" "iCall\icall_cc2650.d" "iCall\icall_user_config.d" "iCallBLE\ble_user_config.d" "iCallBLE\ble_user_config_stack.d" "iCallBLE\icall_api_lite.d" "iCallBLE\icall_hci_tl.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo ' '

post-build:
	-C:/ti/ti_cgt_arm_llvm_3.2.2.LTS/bin/tiarmobjcopy -O ihex simple_peripheral.out simple_peripheral.hex
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

