!if 0
------------------------------------------------------------------------------
 <copyright file="sources.cmn" company="Atheros and Microsoft">
    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
    Copyright (c) 2006 Atheros Corporation.  All rights reserved.

    The use and distribution terms for this software are covered by the
    Microsoft Limited Permissive License (Ms-LPL) 
    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
    which can be found in the file MS-LPL.txt at the root of this distribution.
    By using this software in any fashion, you are agreeing to be bound by
    the terms of this license.

    You must not remove this notice, or any other, from this software.
 </copyright>
 
 <summary>
    Windows CE Wifi Driver for AR-6000
 </summary>
------------------------------------------------------------------------------
==============================================================================
 Windows CE Wifi Driver for AR-6000

 Author(s): ="Atheros and Microsoft"
==============================================================================
!endif

CPPBROWSE=1

WARNISERROR=1

RELEASETYPE=PLATFORM

EXCEPTION_EXTERN_C=THROW

AR6K_BUS_TYPE=CF

CDEFINES = $(CDEFINES) /DCF /DHTC_SYNC /DNDIS_BUS_DRIVER /DSUPPORT_WPA2 /DWMM /DHTC_RAW_INTERFACE /DNDIS51_MINIPORT

!IF ("$(_TGTPLAT)" == "CEPC")
CDEFINES = $(CDEFINES) /DCEPC
!ENDIF

