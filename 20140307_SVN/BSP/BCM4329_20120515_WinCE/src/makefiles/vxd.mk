#############################################################################

#   Flags for building VXD targets
#
############################################################################



    REQUIRE_DDK = 1
    REQUIRE_W95DDK = 1
    REQUIRE_VC152 = 1
    REQUIRE_MSDEV = 1

    ifdef ECLOUD_BUILD_ID
      CMDSHELL := $(subst \,/,$(COMSPEC))
      CMDSTART := $(CMDSHELL) /c start /min /separate
    else # ECLOUD_BUILD_ID
      CMDSHELL :=
      CMDSTART :=
    endif # ECLOUD_BUILD_ID

    ifeq ($(origin DEVICE), undefined)
	DEVICE=$(shell echo $(TARGETNAME) | tr 'a-z' 'A-Z')
    endif

    ifeq ($(origin DEFFILE), undefined)
	ifneq ($(wildcard $(DEVICE).def), )
		DEFFILE=$(DEVICE).def
	endif
    endif

    ifeq ($(origin DEBLEVEL), undefined)
	DEBLEVEL=3
    endif
    ifeq ($(strip $(DEBLEVEL)), )
	DEBLEVEL=3
    endif

    FDBG =  -DDEBUG -DDBG=3 -DDEBLEVEL=$(DEBLEVEL)
    FOPT =  -DDEBLEVEL=0

    RC = $(CMDSTART) $(VC152)/bin/rc.exe
    ADRC2VXD = $(CMDSTART) $(W95DDK)/bin/ADRC2VXD.exe

    TARGET=$(TARGETNAME).vxd

    C_DEFINES += -DCHICAGO -DDEVICE=$(DEVICE) -DNDIS_STDCALL 

    COPTFLAGS  = -nologo -O2 -Zp -Gs -DIS_32 -Zl -Gz $(C_DEFINES)
    CDBGFLAGS  = -nologo -Od -Zi -Zp -Gs -DIS_32 -Zl -Gz $(C_DEFINES)

    MSINCLUDE=.;$(strip $(INCLUDES));$(W95DDK)/inc32;$(MSDEV)/VC98/ATL/INCLUDE;$(MSDEV)/VC98/INCLUDE;$(MSDEV)/VC98/MFC/INCLUDE;$(DDK)/inc;

    CPPFLAGS = $(patsubst %,-I%,$(subst ;, ,$(MSINCLUDE)))

    RCFLAGS  = 

    AFLAGS  = -nologo -DIS_32 -W2 -Cx -DMASM6 -DVMMSYS -Zm -DSEGNUM=3
    AFLAGS  += -coff -DBLD_COFF -DDEVICE=$(DEVICE)
    AFLAGS  += -DNDIS_STDCALL

    #AFLAGS	= -nologo -coff -W2 -c -Cx 
    #AFLAGS	+= -DBLD_COFF -DIS_32 -DMASM6 $(C_DEFINES)
    #AFLAGS      += $(GCINCS)
    
    LIBVPATH += $(MSDEV.UNC)/vc98/lib:$(MSDEV.UNC)/vc98/mfc/lib
    LIBS = $(TARGETLIBS)
    LDFLAGS = -nologo -VXD -MACHINE:i386 $(DEFFILE:%=-DEF:%) \
	      -MAP:$(@:.vxd=.map) 


############################################################################
#
#   Rules for building VXD targets
#
############################################################################


$(OUTDIR_FREE)/$(TARGET) :: $(DEPS_OBJ) $(DEFFILE)
	@echo "Linking Executable - $@ for i386"
	$(LD) $(LDFLAGS)  -OUT:$@ \
		$(filter %.obj,$^) $(LIBS)


$(OUTDIR_CHECKED)/$(TARGET) :: $(DEPS_OBJ) $(DEFFILE)
	@echo "Linking Executable - $@ for i386"
	$(LD) $(LDFLAGS) -DEBUG -PDB:$(OUTDIR_CHECKED)/$(DEVICE).pdb \
		-OUT:$@ $(filter %.obj,$^) $(LIBS)
ifdef	NTICE
	@echo "Building SoftICE symbols - $(@:%.vxd=%.nms) for i386"
	"$(NTICE)/nmsym.exe" $(SI_FLAGS)  \
		-SOURCE:. -SOURCE:$(SRCBASE)/vxdutils/wrappers $@
endif

ifneq ($(SOURCES.RES), )

$(OUTDIR_$(TTYPE))/$(TARGET) :: $(patsubst %,$(OUTDIR_$(TTYPE))/%,$(SOURCES.RES))
	$(ADRC2VXD) $@ $^

endif # $(SOURCES.RES) != NULL

