
REQUIRE_WINCE=1

define c-obj-command
    $(CC) -c $(C$(TTYPE)FLAGS) -I$(OUTDIR_$(TTYPE)) $(CPPFLAGS) $(F$(TTYPE)) -Fo$@ $<
endef

ifeq ($(WM7), 1)
C_DEFINES +=-DUNICODE -D_UNICODE -DUNDER_CE=700 -DWIN32 -DSTRICT -D_WIN32_WCE=0x700 -DTHUMBSUPPORT -D_USE_32BIT_TIME_T -D_MIDL_USE_GUIDDEF_
else
C_DEFINES +=-DUNICODE -D_UNICODE -DUNDER_CE -D_WIN32_WCE=$(WINCEVER) -DWIN32 -DSTRICT
endif

C_DEFINES +=-DINTERNATIONAL -DL0409 -DINTLMSG_CODEPAGE=1252

ifeq ($(PROCESSOR), MIPS)
C_DEFINES +=-DMIPS -DMIPSII -D_MIPS_ -DR4000 
endif
ifeq ($(PROCESSOR), ARM)
ifeq ($(WINCEVER), 420)
C_DEFINES +=-DARMV4 -DARM -D_ARM_
else
ifeq ($(WM7), 1)
C_DEFINES +=-DARMV6 -DARM -D_ARM_ -QRarch6 -QRfpe- -QRinterwork-return -Zi -Zc:wchar_t- -Zc:forScope-
else
C_DEFINES +=-DARMV4 -DARM -D_ARM_ /QRinterwork-return /QRxscale
endif
endif
endif
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

COPTFLAGS  = -nologo -W3 -Ox -Os -Zi  $(C_DEFINES) $(C_DEFINES_$(TTYPE))
CDBGFLAGS  = -nologo -W3 -Od -Zi -DDEBUG=1 -DDBG=1 -DBCMDBG -Fd$(@D)/ $(C_DEFINES) $(C_DEFINES_$(TTYPE))

ifeq ($(WINCEFASTCAP),1)
ifneq ($(PROCESSOR), X86)
COPTFLAGS += -fastcap
CDBGFLAGS += -fastcap
endif
endif
ifeq ($(WINCECALLCAP),1)
ifneq ($(PROCESSOR), X86)
COPTFLAGS += -callcap
CDBGFLAGS += -callcap
endif
endif
INCLUDES += $(_PUBLICROOT)/speech/sdk/inc;$(_PUBLICROOT)/directx/sdk/inc;$(_PUBLICROOT)/wceappsfe/sdk/inc;$(_PUBLICROOT)/wceshellfe/sdk/inc;$(_PUBLICROOT)/shellsdk/sdk/inc;$(_PUBLICROOT)/rdp/sdk/inc;$(_PUBLICROOT)/servers/sdk/inc;$(_PUBLICROOT)/ie/sdk/inc;$(_PUBLICROOT)/dcom/sdk/inc;$(_WINCEROOT)/sdk/CE/inc;./Resource/0409

CPPFLAGS = $(patsubst %,-I%,$(subst ;, ,$(INCLUDES)))


goo $(OUTDIR_$(TTYPE))/$(TARGET) :: $(DEPENDENCIES) $(DEFFILE)
	#
	# first create the .lib...
	#
	@echo -subsystem:windowsce,$(_WINCE_VERSION) > _cebuild.tmp

	@$(foreach dep,$(DOS_DEPS),echo $(dep) >> _cebuild.tmp;)
ifeq ($(PROCESSOR), ARM)
	$(LD) -lib -nodefaultlib -ignore:4001 -OUT:$(OUTDIR_$(TTYPE))/$(TARGETNAME).lib -def:$(DEFFILE) -machine:THUMB @_cebuild.tmp
else
	$(LD) -lib -nodefaultlib -ignore:4001 -OUT:$(OUTDIR_$(TTYPE))/$(TARGETNAME).lib -def:$(DEFFILE) -machine:$(PROCESSOR) @_cebuild.tmp
endif
	#
	# now create the .dll
	#	
	@echo -nodefaultlib > _cebuild.tmp
	#@echo -entry:_DllMainCRTStartup >> _cebuild.tmp
	@echo -debug -debugtype:cv -incremental:no /opt:ref -pdb:$(OUTDIR_$(TTYPE))/$(TARGETNAME).pdb >> _cebuild.tmp
	@echo -map:$(OUTDIR_$(TTYPE))/$(TARGETNAME).map >> _cebuild.tmp
	@echo -savebaserelocations:$(OUTDIR_$(TTYPE))/$(TARGETNAME).rel >> _cebuild.tmp
	@echo -MERGE:.rdata=.text -align:4096 -ignore:4001,4070,4078,4086,4089,4096,4099,4108 /STACK:65536,4096 >> _cebuild.tmp

	@echo -subsystem:windowsce,$(_WINCE_VERSION) > _cebuild.tmp

	@echo -nodefaultlib >> _cebuild.tmp
	@echo /base:0x10000000 >> _cebuild.tmp
	@$(foreach dep,$(DOS_DEPS),echo $(dep)  >> _cebuild.tmp;)
	@$(foreach lib,$(TARGETLIBS),echo $(lib)  >> _cebuild.tmp;)
	@echo $(OUTDIR_$(TTYPE))/$(TARGETNAME).exp >> _cebuild.tmp
ifeq ($(PROCESSOR), ARM)
	$(LD) /MAP /LARGEADDRESSAWARE -out:$(OUTDIR_$(TTYPE))/$(TARGETNAME).dll -dll -debug -debugtype:cv -incremental:no /opt:ref -pdb:$(OUTDIR_$(TTYPE))/$(TARGETNAME).pdb -machine:THUMB @_cebuild.tmp
else
	$(LD) /MAP /LARGEADDRESSAWARE -out:$(OUTDIR_$(TTYPE))/$(TARGETNAME).dll -dll -machine:$(PROCESSOR) @_cebuild.tmp
endif
	rm _cebuild.tmp
