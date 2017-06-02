################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ELT131_05_02.obj: ../ELT131_05_02.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_common/include" --include_path="/opt/ti/controlSUITE/device_support/f2833x/v141/DSP2833x_headers/include" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01/MD_F2833x_LIB" --include_path="/media/data/CODES/ccs7_workspace/ELT131_LAB_05_01" --include_path="/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS/include" --advice:performance=all --define=FLASH --define=RTOS_DISPATCHER --define=RTOS --define=xdc__strict -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ELT131_05_02.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

build-714807359:
	@$(MAKE) -Onone -f subdir_rules.mk build-714807359-inproc

build-714807359-inproc: ../app.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"/opt/ti/xdctools_3_32_00_06_core/xs" --xdcpath="/opt/ti/tirtos_c2000_2_16_01_14/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/tidrivers_c2000_2_16_01_13/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/bios_6_45_02_31/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/ndk_2_25_00_09/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/uia_2_00_05_50/packages;/opt/ti/bios_6_33_04_39/packages;/opt/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.C28_float -p ti.platforms.tms320x28:TMS320F28335 -r release -c "/opt/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.12.0.STS" --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-714807359 ../app.cfg
configPkg/compiler.opt: build-714807359
configPkg/: build-714807359


