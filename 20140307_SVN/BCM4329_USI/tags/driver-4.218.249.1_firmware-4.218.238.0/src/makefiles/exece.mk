
REQUIRE_WINCE=1

define c-obj-command
    $(CC) -c $(C$(TTYPE)FLAGS) -I$(OUTDIR_$(TTYPE)) $(CPPFLAGS) $(F$(TTYPE)) -Fo$@ $<
endef

C_DEFINES +=-DUNICODE -D_UNICODE -DUNDER_CE -D_WIN32_WCE=$(WINCEVER) -DWIN32 -DSTRICT 
C_DEFINES +=-DINTERNATIONAL -DL0409 -DINTLMSG_CODEPAGE=1252
ifeq ($(PROCESSOR), MIPS)
C_DEFINES +=-DMIPS -DMIPSII -D_MIPS_ -DR4000 
endif
ifeq ($(PROCESSOR), ARM)
ifeq ($(WINCEVER), 420)
C_DEFINES +=-DARMV4 -DARM -D_ARM_
else
C_DEFINES +=-DARMV4 -DARM -D_ARM_ /QRinterwork-return /QRxscale
endif
endif

#typically for test purpose
ifeq ($(PROCESSOR), X86)
C_DEFINES +=-D_X86_ -Dx86
endif

ifeq ($(WINCEVER), 500)
_WINCE_VERSION=5.00
else
ifeq ($(WINCEVER), 600)
_WINCE_VERSION=6.00
else
_WINCE_VERSION=4.20
endif
endif

COPTFLAGS  = -nologo -Gy -W3 -Oxs -GF -Zi  $(C_DEFINES) $(C_DEFINES_$(TTYPE))
CDBGFLAGS  = -nologo -Gy -W3 -Od -GF -Zi -DDEBUG=1 -DDBG=1 -DBCMDBG -Fd$(@D)/ $(C_DEFINES) $(C_DEFINES_$(TTYPE))

INCLUDES += $(_PUBLICROOT)/speech/sdk/inc;$(_PUBLICROOT)/directx/sdk/inc;$(_PUBLICROOT)/wceappsfe/sdk/inc;$(_PUBLICROOT)/wceshellfe/sdk/inc;$(_PUBLICROOT)/shellsdk/sdk/inc;$(_PUBLICROOT)/rdp/sdk/inc;$(_PUBLICROOT)/servers/sdk/inc;$(_PUBLICROOT)/ie/sdk/inc;$(_PUBLICROOT)/dcom/sdk/inc;$(_WINCEROOT)/PUBLIC/T1/WINCE420/BCM94710AP/cesysgen/sdk/inc;$(_WINCEROOT)/sdk/CE/inc;./Resource/0409

CPPFLAGS = $(patsubst %,-I%,$(subst ;, ,$(INCLUDES)))


goo $(OUTDIR_$(TTYPE))/$(TARGET) :: $(DEPENDENCIES) $(DEFFILE)
	#
	# now create the .exe
	#
	@echo -subsystem:windowsce,$(_WINCE_VERSION) > _cebuild.tmp

	@$(foreach dep,$(DOS_DEPS),echo $(dep) >> _cebuild.tmp;)
	@echo -nodefaultlib > _cebuild.tmp
	@echo -entry:main /SUBSYSTEM:CONSOLE >> _cebuild.tmp
	@echo -debug -debugtype:cv -incremental:no /opt:ref -pdb:$(OUTDIR_$(TTYPE))/$(TARGETNAME).pdb >> _cebuild.tmp
	@echo -map:$(OUTDIR_$(TTYPE))/$(TARGETNAME).map >> _cebuild.tmp
	@echo -savebaserelocations:$(OUTDIR_$(TTYPE))/$(TARGETNAME).rel >> _cebuild.tmp
	@echo -MERGE:.rdata=.text -align:4096 -ignore:4001,4070,4078,4086,4089,4096,4099,4108 /STACK:65536,4096 >> _cebuild.tmp

	@echo -subsystem:windowsce,$(_WINCE_VERSION) >> _cebuild.tmp
	@echo /base:0x00010000 /entry:"mainACRTStartup" >> _cebuild.tmp
	@$(foreach dep,$(DOS_DEPS),echo $(dep)  >> _cebuild.tmp;)
	@$(foreach lib,$(TARGETLIBS),echo $(lib)  >> _cebuild.tmp;)
	#@echo $(OUTDIR_$(TTYPE))/$(TARGETNAME).exp >> _cebuild.tmp
ifeq ($(PROCESSOR), ARM)
	$(LD) /LARGEADDRESSAWARE -OUT:$(OUTDIR_$(TTYPE))/$(TARGETNAME).exe -debug -debugtype:cv -pdb:$(OUTDIR_$(TTYPE))/$(TARGETNAME).pdb -machine:THUMB @_cebuild.tmp
else
	$(LD) /LARGEADDRESSAWARE -OUT:$(OUTDIR_$(TTYPE))/$(TARGETNAME).exe -machine:$(PROCESSOR) @_cebuild.tmp
endif
	rm _cebuild.tmp
