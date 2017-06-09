#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /opt/ti/tirtos_c2000_2_12_01_33/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/ipc_3_30_01_12/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/uia_2_00_02_39/packages;/opt/ti/bios_6_33_04_39/packages;/home/mainster/CODES_local/ccs7_workspace/ELT131_LAB_09_MD_UART/.config
override XDCROOT = /opt/ti/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /opt/ti/tirtos_c2000_2_12_01_33/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/bios_6_41_04_54/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/ipc_3_30_01_12/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/ndk_2_24_02_31/packages;/opt/ti/tirtos_c2000_2_12_01_33/products/uia_2_00_02_39/packages;/opt/ti/bios_6_33_04_39/packages;/home/mainster/CODES_local/ccs7_workspace/ELT131_LAB_09_MD_UART/.config;/opt/ti/xdctools_3_32_00_06_core/packages;..
HOSTOS = Linux
endif
