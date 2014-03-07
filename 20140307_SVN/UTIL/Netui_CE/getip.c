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

Module Name:  

getip.c

Abstract:  

Functions:


Notes: 


--*/
#include <windows.h>
#include <types.h>
#include "netui.h"
#include "netuip.h"
#include "resource.h"

// 
// ----------------------------------------------------------------
//
//	IP Address dialog box.
//
//  Called by PPP to prompt for the IP Address if the SLIP
//	connection doesn't have a valid IP address
//
// ----------------------------------------------------------------
LRESULT CALLBACK
DlgProcIPAddr ( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	DWORD	IPAddr, dwStyle;
	
    switch (message) {
    case WM_INITDIALOG :
        dwStyle = GetWindowLong(hDlg,GWL_EXSTYLE);
        SetWindowLong(hDlg,GWL_EXSTYLE,dwStyle|WS_EX_TOPMOST);
		IPAddr = 0;
        SendMessage (GetDlgItem(hDlg, IDC_IP_ADDR), IP_SETADDRESS, 0,
                     (LPARAM) *(int *) &IPAddr);
        EnableWindow(GetDlgItem(hDlg, IDC_IP_ADDR), TRUE);
        SetFocus(GetDlgItem(hDlg, IDC_IP_ADDR));
        return FALSE;
    case WM_COMMAND:
        switch( LOWORD( wParam ) ) {
        case IDCANCEL:              // Ignore values
			DEBUGMSG (ZONE_GETIP,
					  (TEXT("GetIPAddress:User pressed cancel\r\n")));
            EndDialog( hDlg, 0 );
            break;
        case IDOK:                  // User Said OK
            // Get IPAddr
            SendMessage(GetDlgItem(hDlg, IDC_IP_ADDR), IP_GETADDRESS, 0,
						(LPARAM) ((int *) &IPAddr));
			DEBUGMSG (ZONE_GETIP,
					  (TEXT("GetIPAddress:User pressed OK, IPAddr=0x%X\r\n"),
					   IPAddr));
            EndDialog( hDlg, IPAddr );
            break;
        default :
            break;
        }
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
	DWORD	IPAddr;
	
    DEBUGMSG( ZONE_FUNCTION, ( TEXT("GetIPAddress\r\n")));

	// Ignore errors if it's already registered.
    RegisterIPClass( v_hInst );
    
    IPAddr = DialogBox(v_hInst, MAKEINTRESOURCE(IDD_IPADDR),
					   hParent, DlgProcIPAddr);
    return IPAddr;
}

