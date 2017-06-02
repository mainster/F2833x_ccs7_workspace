################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
bsp/DSP2833x_ADC_cal.obj: ../bsp/DSP2833x_ADC_cal.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01/MD_F2833x_LIB" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01" --include_path="/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/include" --advice:performance=all --define=FLASH --define=RTOS_DISPATCHER --define=RTOS --define=xdc__strict -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="bsp/DSP2833x_ADC_cal.d" --obj_directory="bsp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

bsp/DSP2833x_GlobalVariableDefs.obj: ../bsp/DSP2833x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01/MD_F2833x_LIB" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01" --include_path="/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/include" --advice:performance=all --define=FLASH --define=RTOS_DISPATCHER --define=RTOS --define=xdc__strict -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="bsp/DSP2833x_GlobalVariableDefs.d" --obj_directory="bsp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

bsp/DSP2833x_SysCtrl.obj: ../bsp/DSP2833x_SysCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01/MD_F2833x_LIB" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01" --include_path="/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/include" --advice:performance=all --define=FLASH --define=RTOS_DISPATCHER --define=RTOS --define=xdc__strict -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="bsp/DSP2833x_SysCtrl.d" --obj_directory="bsp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

bsp/DSP2833x_usDelay.obj: ../bsp/DSP2833x_usDelay.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01/MD_F2833x_LIB" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01" --include_path="/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/include" --advice:performance=all --define=FLASH --define=RTOS_DISPATCHER --define=RTOS --define=xdc__strict -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="bsp/DSP2833x_usDelay.d" --obj_directory="bsp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


