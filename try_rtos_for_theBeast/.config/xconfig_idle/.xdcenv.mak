#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /opt/ti/bios_6_33_04_39/packages;/opt/ti/tirtos_c2000_2_16_01_14/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/tidrivers_c2000_2_16_01_13/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/bios_6_45_02_31/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/ndk_2_25_00_09/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/uia_2_00_05_50/packages;/opt/ti/ccsv7/ccs_base;/home/mainster/CODES_local/ccs7_workspace/try_rtos_for_theBeast/.config
override XDCROOT = /opt/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /opt/ti/bios_6_33_04_39/packages;/opt/ti/tirtos_c2000_2_16_01_14/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/tidrivers_c2000_2_16_01_13/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/bios_6_45_02_31/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/ndk_2_25_00_09/packages;/opt/ti/tirtos_c2000_2_16_01_14/products/uia_2_00_05_50/packages;/opt/ti/ccsv7/ccs_base;/home/mainster/CODES_local/ccs7_workspace/try_rtos_for_theBeast/.config;/opt/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Linux
endif
