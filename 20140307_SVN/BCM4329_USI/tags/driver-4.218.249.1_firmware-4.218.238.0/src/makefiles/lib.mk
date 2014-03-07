

############################################################################
#
#   Flags for building LIB targets 
#
############################################################################
    REQUIRE_MSDEV = 1

    TARGET=$(TARGETNAME).lib

    LDFLAGS_OPT += -nologo $(foreach p,$(LIBVPATH.W),"/LIBPATH:$p")
    LDFLAGS_DBG += $(LDFLAGS_OPT) -DEBUG -PDB:$(@D)/

    NTICE_OPT=
    NTICE_DBG=

    RCFLAGS  = -l 409 -z "MS Sans Serif,Helv/MS Shell Dlg"  -r \
		$(C_DEFINES) $(CPPFLAGS)
    # RCFLAGS  =  $(CPPFLAGS)

    LIBVPATH += $(MSDEV.UNC)/vc98/lib:$(MSDEV.UNC)/vc98/mfc/lib
    LIBS = $(TARGETLIBS)

    C_DEFINES += -D_X86_ -D_MMX_

    # removed -MD multi thread support from library compile options.
    #
    COPTFLAGS  = -nologo -W3 -GX -O2 $(C_DEFINES)
    CDBGFLAGS  = -nologo -W3 -GX -Z7 -Od -GZ -Fd$(@D)/ $(C_DEFINES) -DNDEBUG 

    MSINCLUDE=.;$(strip $(INCLUDES));$(MSDEV.INCLUDE)

    CPPFLAGS = $(patsubst %,-I%,$(subst ;, ,$(MSINCLUDE)))



############################################################################
#
#   Rules for building  LIB targets
#
############################################################################


$(OUTDIR_$(TTYPE))/$(TARGET) :: $(DEPENDENCIES) $(LIBS) 
	@echo "Linking Library - $@"
	$(LIBCMD) $(LIBFLAGS_$(TTYPE)) -OUT:$@ $(DOS_DEPS) 
	$(NTICE_$(TTYPE))

