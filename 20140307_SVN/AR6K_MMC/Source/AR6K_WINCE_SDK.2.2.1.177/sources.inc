!if 0
------------------------------------------------------------------------------
 <copyright file="sources" company="Atheros">
    Copyright (c) 2005-2009 Atheros Corporation.  All rights reserved.
 
  The software source and binaries included in this development package are
  licensed, not sold. You, or your company, received the package under one
  or more license agreements. The rights granted to you are specifically
  listed in these license agreement(s). All other rights remain with Atheros
  Communications, Inc., its subsidiaries, or the respective owner including
  those listed on the included copyright notices.  Distribution of any
  portion of this package must be in strict compliance with the license
  agreement(s) terms.
  </copyright>
  
  <summary>
  	Wifi driver for AR6002
  </summary>
 
------------------------------------------------------------------------------
==============================================================================
 Author(s): ="Atheros"
==============================================================================
!endif

CPPBROWSE=1
WARNISERROR=1
RELEASETYPE=PLATFORM
EXCEPTION_EXTERN_C=THROW
AR6K_BUS_TYPE=SDIO
CDEFINES = $(CDEFINES) /DSUPPORT_WPA2                  \
                       /DNDIS51_MINIPORT               \
                       /DFLASH_18V                     \
                       /DHTC_RAW_INTERFACE             \
                       /DWINCE_ART                     \
                       /DOS_ROAM_MANAGEMENT            \
                       /DATHR_CE_LEGACY

!IF ("$(AR6K_SDIO_BYPASS)" == "1")
CDEFINES = $(CDEFINES) /DHIF_SDIO_BYPASS
!ENDIF

!IF ("$(IMGSDBUS2)" == "1") && ("$(AR6K_SDIO_BYPASS)" != "1")
CDEFINES = $(CDEFINES) /DHIF_SYNC     
!ENDIF 

!IF ("$(IMGSDBUS2)" == "1")
CDEFINES = $(CDEFINES) /DSD_HIGH_CAPACITY_BUSDRIVER_SUPPORT                                   
!ENDIF

!IF ( ("$(_TGTPROJ)" == "wpc") || ("$(_TGTPROJ)" == "smartfon") )
CDEFINES = $(CDEFINES) /DWINDOWS_MOBILE
!ENDIF
 
!IF ("$(_TGTPLAT)" == "CEPC")
CDEFINES = $(CDEFINES) /DCEPC
!ENDIF

!IF ("$(AR6K_BUILT_IN_FIRMWARE)" == "1")
CDEFINES = $(CDEFINES) /DEMBEDDED_AR6K_FIRMWARE
!ENDIF

!IF ("$(AR6K_HTC_EPPING_SUPPORT)" == "1")
CDEFINES = $(CDEFINES) /DHTC_EPPING_TEST
!ENDIF


