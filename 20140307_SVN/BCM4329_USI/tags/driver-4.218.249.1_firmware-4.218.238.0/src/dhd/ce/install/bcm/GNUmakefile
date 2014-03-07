##
## MS cabwiz tool does not work inside directories with longer paths.
## Hence create a temporary directory to create the .cab files
##

TEMP     := C:/temp
SRCBASE  := ../../../../../
VARIANTS := sta
TTYPES   := free checked  
SHELL    := bash.exe
CWD      := $(shell pwd)
CWD      := $(shell cygpath -m $(CWD))
TIMESTAMP:= $(shell date '+%Y%m%d%H%M%S')
#TEMPDIR  := $(TEMP)/WCE/$(TIMESTAMP)
#OLD TARGETS  := $(foreach variant,$(VARIANTS),$(foreach type,$(TYPES),$(variant)/$(WINCEVER)/$(PROCESSOR)/$(type)))
DEVPREFIX:= bcmsddhd

DEVTYPE  ?= sdio
DEVNAME  ?= bcm
WINCEVER ?= 600
PROCESSOR?= X86
TEMPDIR  := $(TEMP)/WCE/$(if $(TAG),$(TAG),NIGHTLY)/$(DEVNAME)_$(DEVTYPE)/CE$(WINCEVER)_$(PROCESSOR)_$(TIMESTAMP)
TARGETS  := ce$(WINCEVER)_$(PROCESSOR)
BTARGETS ?= $(foreach ttype,$(TTYPES),$(VARIANTS)/$(WINCEVER)/$(PROCESSOR)/$(ttype))
TARGETEXE:= bcmsddhd_ce$(WINCEVER)_$(PROCESSOR)_setup.exe

# For WinCE Signing, pick a network copy of signing script ahead of local copy
SIGN_CMD ?= $(firstword $(wildcard z:/home/hwnbuild/src/tools/build/WinCE_Sign.sh z:/projects/hnd/tools/win/msdev/WinCE-Cert/WinCE_Sign.sh $(SRCBASE)/dhd/ce/install/common/WinCE_Sign.sh))
ifeq ($(SIGN_CMD),)
        $(error Signing script WinCE_Sign.sh not found)
endif

BCMDL_OBJ_DIR  := $(shell cygpath -m $(CWD)/../../../../dhd/ce/obj)
DONGLE_DIR  := $(shell cygpath -m $(CWD)/../../../../dongle/rte/wl/builds)
NVRAM_DIR  := $(shell cygpath -m $(CWD)/../../../../shared/nvram)
DONGLE_NAME := 4329b1/sdio-g-cdc-full11n-reclaim-roml-ndis-wme/rtecdc.bin
NVRAM_NAME := bcm94329sdagb.txt

## Tools needed to package
EZSETUP        := Z:/projects/hnd/tools/wince420/bin/ipaq/ezsetup.exe
WINCE_TOOL_DIR := Z:/projects/hnd/tools/win/msdev/VS2005/SmartDevices/SDK/SDKTools

all: clean $(BTARGETS)

$(BTARGETS):
	@echo -e "\n-----------------------------------------------"
	@buildnode=$(shell hostname); \
	echo -e "$(if $(BRAND),$(BRAND): )Building variant $@ on $$buildnode\n"
	mkdir -p $@ $(TEMPDIR)
	@echo -e "TAG   = $(if $(TAG),$(TAG),NIGHTLY)"     >  $(TEMPDIR)/bldinfo.txt
	@echo -e "BRAND = $(if $(BRAND),$(BRAND),ce_$(DEVTYPE))" >> $(TEMPDIR)/bldinfo.txt
	# 
	# Copy over tools
	# 
	install -p -v $(WINCE_TOOL_DIR)/cabwiz.* $(TEMPDIR)
	install -p -v $(WINCE_TOOL_DIR)/Makecab.exe $(TEMPDIR)
	install -p -v $(EZSETUP) $(TEMPDIR)
	# 
	# Copy over pre-reqs for cabwiz and ezsetup
	# 
	install -p -v $(CWD)/$(DEVPREFIX).in* $(TEMPDIR)
	install -p -v $(CWD)/readme.txt $(TEMPDIR)
	install -p -v $(CWD)/../license.txt $(TEMPDIR)
	$(SIGN_CMD) $(BCMDL_OBJ_DIR)/$@/$(DEVPREFIX).dll
	install -p -v $(BCMDL_OBJ_DIR)/$@/$(DEVPREFIX).dll $(TEMPDIR)
	install -p -v $(NVRAM_DIR)/$(NVRAM_NAME) $(TEMPDIR)/4329nvram.txt
	install -p -v $(DONGLE_DIR)/$(DONGLE_NAME) $(TEMPDIR)/4329rtecdc.bin
	cd $(TEMPDIR) && ./Cabwiz.exe $(DEVPREFIX).inf /err cabwiz.error /compress
	@cat $(TEMPDIR)/cabwiz.error | col -b
	$(SIGN_CMD) $(TEMPDIR)/$(DEVPREFIX).CAB
	cd $(TEMPDIR) && ./ezsetup.exe -l english -i $(DEVPREFIX).ini -r \
	readme.txt -e license.txt -o $(TARGETEXE)
	cp $(TEMPDIR)/$(TARGETEXE) $(CWD)/$@/$(TARGETEXE)
	$(SIGN_CMD) $(CWD)/$@/$(TARGETEXE)
	@if [ -f "$(CWD)/$@/$(TARGETEXE)" ]; then \
	   if [ -d "$(TEMPDIR)" ]; then rm -vrf $(TEMPDIR); fi; \
	else \
	   echo -e "\nError: $(CWD)/$@/$(TARGETEXE) not built\n"; \
	   echo -e "Error: Verify errors in $(TEMPDIR)\n"; \
	fi

clean:
	rm -rf $(foreach btgt,$(BTARGETS),$(btgt))

