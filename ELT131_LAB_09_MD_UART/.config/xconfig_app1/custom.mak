## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,28FP linker.cmd package/cfg/app1_p28FP.o28FP

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/app1_p28FP.xdl
	$(SED) 's"^\"\(package/cfg/app1_p28FPcfg.cmd\)\"$""\"/home/mainster/CODES_local/ccs7_workspace/ELT131_LAB_09_MD_UART/.config/xconfig_app1/\1\""' package/cfg/app1_p28FP.xdl > $@
	-$(SETDATE) -r:max package/cfg/app1_p28FP.h compiler.opt compiler.opt.defs
