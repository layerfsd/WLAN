//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#ifndef _KEYBDDBG_H_
#define _KEYBDDBG_H_


#ifdef DEBUG

    #define SETFNAME(name)          LPCTSTR pszFname = (name)

    #define ZONE_OEM0	    DEBUGZONE(0)
    #define ZONE_OEM1	    DEBUGZONE(1)
    #define ZONE_OEM2	    DEBUGZONE(2)
    #define ZONE_OEM3	    DEBUGZONE(3)
    #define ZONE_OEM4	    DEBUGZONE(4)
    #define ZONE_OEM5	    DEBUGZONE(5)
    #define ZONE_API	    DEBUGZONE(6)
    #define ZONE_PDD	    DEBUGZONE(7)
    #define ZONE_INPUTLANG	    DEBUGZONE(8)
    #define ZONE_DEVICELAYOUT	    DEBUGZONE(9)
    #define ZONE_MOUSEDATA  DEBUGZONE(10)
    #define ZONE_SCANCODES  DEBUGZONE(11)
    #define ZONE_INIT       DEBUGZONE(12)
    #define ZONE_FUNCTION   DEBUGZONE(13)
    #define ZONE_WARN       DEBUGZONE(14)
    #define ZONE_ERROR      DEBUGZONE(15)

#else

    #define SETFNAME(name) 

#endif // DEBUG

#endif // _KEYBDDBG_H_
