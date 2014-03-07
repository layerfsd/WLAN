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

netcard.c

Abstract:

Functions to get the driver name of a newly inserted PCCard from the user.

Functions:


Notes:


--*/
#include <windows.h>
#include <types.h>
#include <netui.h>
#include <netuip.h>
#include <resource.h>
#include "wpcpriv.h"

#define DEF_STR_LEN 128

// Font\color table for Card Driver dialog.
static const CCtlTextDesc gs_rgTextDescCardDvr[] =
{
    {IDC_NETUI_CMDTEXT,       8, FW_NORMAL,   STANDARD_TEXT_COLOR },
//    {IDC_NETUI_DRIVERNAME,    9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDCANCEL,          8, FW_BOLD, STANDARD_TEXT_COLOR },
    {IDOK,              8, FW_BOLD, STANDARD_TEXT_COLOR },
};



//
// ----------------------------------------------------------------
//
//	Device driver name dialog box.
//
//  Called by DEVICE.EXE to prompt for the driver name for a newly inserted
//  PCCard.  This only occurs when DEVICE.EXE cannot recognize a card.
//
//  Also called by the USB client loader if an unrecognized device is attached.
//
// ----------------------------------------------------------------
BOOL CALLBACK DlgProcDriverName(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRet = FALSE;

    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescCardDvr
    static void* s_pvFontManager = NULL;

    if (!(WM_INITDIALOG == message) && (NULL != s_pvFontManager))
    {
        ULONG ulFontMgrRetVal;
        
        if (SHRunFontManager(gs_rgTextDescCardDvr,
                             ARRAYSIZEOF(gs_rgTextDescCardDvr),
                             &s_pvFontManager,
                             message,
                             hDlg,
                             wParam,
                             lParam,
                             &ulFontMgrRetVal) == CTLTEXT_EXIT)
        {
            lRet = (BOOL)ulFontMgrRetVal;
            goto Exit;
        }
    }

    switch(message)
    {
        case WM_INITDIALOG:
        {
            PGETDRIVERNAMEPARMS pDriverParms = (PGETDRIVERNAMEPARMS)lParam;
            LPWSTR pDriverName;
            DWORD  dwStyle;
            DWORD  TypeStr;
            DWORD  BufLen;
            DWORD  StrLen;
            WCHAR  TypeName[DEF_STR_LEN];
            WCHAR  SocketString[DEF_STR_LEN];
            SHMENUBARINFO cbi = {0};

            DEBUGMSG(ZONE_DRIVERNAME, (TEXT("DlgProcDriverName: INITDIALOG\r\n")));

            // Center ourself in the desktop work area
    //        CenterWindow (hDlg);

            // Create a blank menu bar, to provide the user with SIP
            // access in all cases.
            cbi.cbSize = sizeof(SHMENUBARINFO);
            cbi.hwndParent = hDlg;
            cbi.dwFlags = SHCMBF_EMPTYBAR;
            SHCreateMenuBarInternal(&cbi);

            pDriverName = pDriverParms->DriverName;

            //
            // Format initial display of driver name to be like:
            // "<card type string> in socket <n>"
            // Note USB also shares this dialog, in this case driver name string
            // will be passed in.
            //

            if (pDriverParms->PCCardType & PCCARDTYPE_USB)
            {
                // Change window title and text for USB
                StrLen = LoadString(HINST_RESDLL, IDS_NETUI_USB_CAPTION,SocketString,DEF_STR_LEN);
                if (StrLen)
                {
                    SetWindowText(hDlg, SocketString);
                }
                StrLen = LoadString(HINST_RESDLL, IDS_NETUI_USB_CMDTEXT,SocketString,DEF_STR_LEN);
                if (StrLen)
                {
                    SetDlgItemText( hDlg, IDC_NETUI_CMDTEXT,SocketString);
                }
            }
            else
            {
                if ((pDriverParms->PCCardType >= 0) &&
                    (pDriverParms->PCCardType < 9))
                {
                    TypeStr = IDS_NETUI_PCCARD_TYPE_MFC + pDriverParms->PCCardType;
                }
                else
                {
                    TypeStr = IDS_NETUI_PCCARD_TYPE_UNKNOWN;
                }

                BufLen = DEF_STR_LEN;
                pDriverName[0] = 0;
                StrLen = LoadString(HINST_RESDLL, TypeStr, TypeName, BufLen);

                if (StrLen)
                {
                    BufLen = DEF_STR_LEN;
                    StrLen = LoadString(HINST_RESDLL, IDS_NETUI_PCCARD_SOCKET, SocketString, BufLen);
                    if (StrLen)
                    {
                        DWORD   rgdwIDList[2];

                        rgdwIDList[0] = (DWORD)TypeName;
                        rgdwIDList[1] = pDriverParms->Socket + 1;
                        FormatMessage(
                            FORMAT_MESSAGE_FROM_STRING |
                            FORMAT_MESSAGE_ARGUMENT_ARRAY,
                            (LPCVOID)SocketString,
                            IDS_NETUI_PCCARD_SOCKET,
                            0,
                            pDriverName,
                            sizeof(pDriverParms->DriverName),
                            (va_list *)rgdwIDList);
                    }
                }
            }

            SetWindowLong(hDlg, DWL_USER, (LONG)pDriverParms);

            dwStyle = GetWindowLong(hDlg, GWL_EXSTYLE);
//            SetWindowLong (hDlg, GWL_EXSTYLE, dwStyle | WS_EX_CAPTIONOKBTN | WS_EX_TOPMOST);
            SetWindowLong(hDlg, GWL_EXSTYLE, dwStyle | WS_EX_TOPMOST);
            SetDlgItemText( hDlg, IDC_NETUI_DRIVERNAME, pDriverParms->DriverName);
            SetFocus(GetDlgItem(hDlg, IDC_NETUI_DRIVERNAME));
            SendDlgItemMessage(hDlg, IDC_NETUI_DRIVERNAME, EM_SETLIMITTEXT,
                               (WPARAM) DRIVER_NAME_LEN, (LPARAM) 0);
            SendMessage(GetDlgItem(hDlg, IDC_NETUI_DRIVERNAME), EM_SETSEL, 0, -1);
            break;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDCANCEL:              // Ignore values
                    EndDialog(hDlg, FALSE);
                    break;

                case IDOK:                  // Get the dialog values
                {
                    PGETDRIVERNAMEPARMS pDriverParms;
                    
                    DEBUGMSG(ZONE_DRIVERNAME, (TEXT("User pressed OK\r\n")));
                    pDriverParms = (PGETDRIVERNAMEPARMS)GetWindowLong(hDlg, DWL_USER);
                    GetDlgItemText(hDlg, IDC_NETUI_DRIVERNAME,
                                   pDriverParms->DriverName, DRIVER_NAME_LEN);
                    EndDialog(hDlg, TRUE);
                    break;
                }
            }
            break;
            

        default:
            break;
    }

Exit:
    return lRet;
}


BOOL GetDriverName(HWND hParent, PGETDRIVERNAMEPARMS pDriverParms)
{
    int iRet;

    DEBUGMSG(ZONE_DRIVERNAME, (TEXT("GetDriverName: Enter\r\n")));

    //Make sure Shell is loaded, so SH* PSL calls will work....
    while (!IsAPIReady(SH_SHELL))
    {
        RETAILMSG(1, (TEXT("NETUI.DLL....sleeping until Shell32 is ready.\r\n")));
        Sleep(1000);
    }

    SHInitExtraControls();

    iRet = DialogBoxParam(HINST_RESDLL, MAKEINTRESOURCE(IDD_NETUI_DRIVERNAME),
                          SHGetLastActiveWindow(hParent), DlgProcDriverName, (LONG)pDriverParms);
    if (iRet == -1)
    {
        DEBUGMSG(ZONE_DRIVERNAME|ZONE_ERROR,
            (TEXT("GetDriverName: DialogBoxParam returned err %u\r\n"), GetLastError()));
        iRet = FALSE;
    }
    
    return iRet;
}
