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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) Microsoft Corporation. All rights reserved.

-----------------------------------------------------------------------------

CONNDLL.H

-----------------------------------------------------------------------------

--*/

#ifndef __CONNDLL_H__
#define __CONNDLL_H__

#include "ras.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CONNECT             128

// Typedefs
// From unimodem\tspip.h in the OS.
#define DEVMINCFG_VERSION 0x0030
#define DIAL_MODIFIER_LEN 256
#define MAX_CFG_BLOB      126
#define MAX_NAME_LENGTH   8
typedef struct  tagDEVMINICFG  {
    WORD  wVersion;
    WORD  wWaitBong;             // DevCfgHdr
    
    DWORD dwCallSetupFailTimer;  // CommConfig.ModemSettings
    DWORD dwModemOptions;        // CommConfig.ModemSettings
                                 // MDM_BLIND_DIAL         MDM_FLOWCONTROL_SOFT
                                 // MDM_CCITT_OVERRIDE MDM_FORCED_EC
                                 // MDM_CELLULAR       MDM_SPEED_ADJUST
                                 // MDM_COMPRESSION    MDM_TONE_DIAL
                                 // MDM_ERROR_CONTROL  MDM_V23_OVERRIDE
                                 // MDM_FLOWCONTROL_HARD
    
    DWORD dwBaudRate;            // DCB

    WORD  fwOptions;             // DevCfgHdr
                                 // TERMINAL_PRE  TERMINAL_POST 
                                 // MANUAL_DIAL

    BYTE  ByteSize;              // DCB
    BYTE  StopBits;              // DCB
    BYTE  Parity;                // DCB

    WCHAR szDialModifier[DIAL_MODIFIER_LEN+1];    // Unique to MiniCfg

    // NOTENOTE.  I should have included a dwDevCapFlags field here so that
    // we know what dial modifiers the modem can handle.  So eventually we may
        // need to bump the version on this struct, and tell unimodem to use
        // defaults for older structs and use registry value for newer structs.

        // Dynamic devices configuration
        WCHAR   wszDriverName[MAX_NAME_LENGTH+1];
        BYTE    pConfigBlob[MAX_CFG_BLOB];
        HANDLE  hPort;
    
} DEVMINICFG, *PDEVMINICFG;

// The data for a connection.
typedef struct tagCONNDATA 
{
        TCHAR           EntryName[RAS_MaxEntryName+1];
        RASENTRY        Entry;                  // Ras Entry
        DEVMINICFG      DevConfig;              // Device Config Data
} CONNDATA, *PCONNDATA;



BOOL ModemConnectionCreateEdit(HWND hWnd, BOOL fCreate, LPTSTR pszConnName);

BOOL VpnConnectionCreateEdit(HWND hWnd, BOOL fCreate, LPTSTR pszConnName);

BOOL ConnectionEdit(HWND hWnd, LPTSTR pszConnName);

BOOL BthDiscoverAndBondToDevices(HWND hWnd);
                               

BOOL AdvancedSettingsModify(HWND hWndParent, PCONNDATA pConnData);

void TrailingSpacesStrip(LPTSTR pszString);

BOOL ActiveConnectionDetect(LPTSTR pszConnectionName);

BOOL ConnectionDelete(HWND hDlg, LPTSTR pszConnName);


#ifdef __cplusplus
}
#endif

#endif // __CONNDLL_H__

