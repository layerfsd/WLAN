//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/*++

Module Name:

netui.c

Abstract:

Functions:


Notes:


--*/
#include <windows.h>
#include <types.h>
#include <netui.h>
#include <netuip.h>
#include <resource.h>
#include "aygshell.h"
#include "wpcpriv.h"

// Font\color table for IP Address dialog.
static const CCtlTextDesc gs_rgTextDescIPAddr[] =
{
    {IDC_NETUI_IPADDRLABEL,     8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_IP_ADDR,         9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDOK,                8, FW_BOLD, STANDARD_TEXT_COLOR },
    {IDCANCEL,            8, FW_BOLD, STANDARD_TEXT_COLOR },
};


//
// ----------------------------------------------------------------
//
//  IP Address dialog box.
//
//  Called by PPP to prompt for the IP Address if the SLIP
//  connection doesn't have a valid IP address
//
// ----------------------------------------------------------------
BOOL CALLBACK
DlgProcIPAddr ( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    DWORD   IPAddr, dwStyle;


    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescIPAddr
    static  void*   s_pvFontManager;
    ULONG ulFontMgrRetVal;

    if (SHRunFontManager(gs_rgTextDescIPAddr,
                         ARRAYSIZEOF(gs_rgTextDescIPAddr),
                         &s_pvFontManager,
                         message,
                         hDlg,
                         wParam,
                         lParam,
                         &ulFontMgrRetVal) == CTLTEXT_EXIT )
    {
        return (BOOL)ulFontMgrRetVal;
    }

    switch (message)
    {

        case WM_INITDIALOG :
            dwStyle = GetWindowLong(hDlg,GWL_EXSTYLE);
            SetWindowLong(hDlg,GWL_EXSTYLE,dwStyle|WS_EX_TOPMOST);
            IPAddr = 0;
            SendMessage (GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), IP_SETADDRESS, 0,
                         (LPARAM) *(int *) &IPAddr);
            EnableWindow(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), TRUE);
            SetFocus(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR));
            return FALSE;

        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case IDCANCEL:              // Ignore values
                    DEBUGMSG (ZONE_GETIP,
                              (TEXT("GetIPAddress:User pressed cancel\r\n")));
                    EndDialog( hDlg, 0 );
                    break;

                case IDOK:                  // User Said OK
                    // Get IPAddr
                    SendMessage(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), IP_GETADDRESS, 0,
                                (LPARAM) ((int *) &IPAddr));
                    DEBUGMSG (ZONE_GETIP,
                              (TEXT("GetIPAddress:User pressed OK, IPAddr=0x%X\r\n"),
                              IPAddr));
                    EndDialog( hDlg, IPAddr );
                    break;

                default :
                    break;
            }

        case WM_ACTIVATE:

            if (LOWORD(wParam) != WA_INACTIVE)
            {
                // Bring up the SIP on activate.
                SHSipPreference(hDlg, SIP_INPUTDIALOGINIT);
            }
            else
            {
                // Put the SIP down on de-activate
                SHSipPreference(hDlg, SIP_DOWN);
            }
            return FALSE;

        default :
            break;
    }
    return FALSE;
}

// GetIPAddress
//
// Function:    Prompts the user for an IP address.
//
//

DWORD
GetIPAddress( HWND hParent )
{
    DWORD   IPAddr;

    DEBUGMSG( ZONE_FUNCTION, ( TEXT("GetIPAddress\r\n")));

    // Ignore errors if it's already registered.
    RegisterIPClass( HINST_RESDLL );

    IPAddr = DialogBox(HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_IPADDR),
                       SHGetLastActiveWindow(hParent), DlgProcIPAddr);
    return IPAddr;
}

