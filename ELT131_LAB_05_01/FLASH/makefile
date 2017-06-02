################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./ELT131_05_02.obj" \
"./MD_F2833x_LIB/md_bsp_explorer.obj" \
"./MD_F2833x_LIB/md_epwm.obj" \
"./MD_F2833x_LIB/md_gpio.obj" \
"./MD_F2833x_LIB/md_spi.obj" \
"./MD_F2833x_LIB/md_uart_sci.obj" \
"./bsp/DSP2833x_ADC_cal.obj" \
"./bsp/DSP2833x_GlobalVariableDefs.obj" \
"./bsp/DSP2833x_SysCtrl.obj" \
"./bsp/DSP2833x_usDelay.obj" \
"../DSP2833x_Headers_BIOS.cmd" \
"../F28335.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include MD_F2833x_LIB/subdir_vars.mk
-include bsp/subdir_vars.mk
-include subdir_rules.mk
-include MD_F2833x_LIB/subdir_rules.mk
-include bsp/subdir_rules.mk
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
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
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
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
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
ELT131_LAB_05_01.out \

EXE_OUTPUTS__QUOTED += \
"ELT131_LAB_05_01.out" \

BIN_OUTPUTS += \
ELT131_LAB_05_01.hex \

BIN_OUTPUTS__QUOTED += \
"ELT131_LAB_05_01.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ELT131_LAB_05_01.out"

# Tool invocations
ELT131_LAB_05_01.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O2 --opt_for_speed=0 --advice:performance=all --define=FLASH --define=RTOS_DISPATCHER --define=RTOS --define=xdc__strict -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number -z -m"ELT131_LAB_05_01.map" --stack_size=0x300 --warn_sections -i"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/lib" -i"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="ELT131_LAB_05_01_linkInfo.xml" --rom_model -o "ELT131_LAB_05_01.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

ELT131_LAB_05_01.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/bin/hex2000"  -o "ELT131_LAB_05_01.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_CMDS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "ELT131_05_02.obj" "MD_F2833x_LIB/md_bsp_explorer.obj" "MD_F2833x_LIB/md_epwm.obj" "MD_F2833x_LIB/md_gpio.obj" "MD_F2833x_LIB/md_spi.obj" "MD_F2833x_LIB/md_uart_sci.obj" "bsp/DSP2833x_ADC_cal.obj" "bsp/DSP2833x_GlobalVariableDefs.obj" "bsp/DSP2833x_SysCtrl.obj" "bsp/DSP2833x_usDelay.obj" 
	-$(RM) "ELT131_05_02.d" "MD_F2833x_LIB/md_bsp_explorer.d" "MD_F2833x_LIB/md_epwm.d" "MD_F2833x_LIB/md_gpio.d" "MD_F2833x_LIB/md_spi.d" "MD_F2833x_LIB/md_uart_sci.d" "bsp/DSP2833x_GlobalVariableDefs.d" "bsp/DSP2833x_SysCtrl.d" 
	-$(RM) "bsp/DSP2833x_ADC_cal.d" "bsp/DSP2833x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

