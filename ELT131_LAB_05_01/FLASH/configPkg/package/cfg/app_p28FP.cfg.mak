# invoke SourceDir generated makefile for app.p28FP
app.p28FP: .libraries,app.p28FP
.libraries,app.p28FP: package/cfg/app_p28FP.xdl
	$(MAKE) -f /media/data/CODES/ccs7_workspace/ELT131_LAB_05_01/src/makefile.libs

clean::
	$(MAKE) -f /media/data/CODES/ccs7_workspace/ELT131_LAB_05_01/src/makefile.libs clean

