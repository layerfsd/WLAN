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

Network.c

Abstract:

Functions:


Notes:


--*/
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "string.h"
#include "memory.h"
#include "commctrl.h"
#include "resource.h"
#include "netui.h"
#include "netuip.h"
#include "wcommctl.h"
#include "aygshell.h"
#include "wpcpriv.h"    // For font manager.


// update IP address every 10 seconds
#define IDT_IPADDRINFO      109
#define TIMEOUT_IPADDRINFO  10000


typedef struct _NETWORK_ADPT_INFO
{
    LPTSTR  szAdapterName;
    BOOL    fUseDHCP;
    BOOL    fSaved;
    DWORD   IPAddr;
    DWORD   SubnetMask;
    DWORD   Gateway;
    DWORD   DNSAddr;
    DWORD   DNSAltAddr;
    DWORD   WINSAddr;
    DWORD   WINSAltAddr;
    TCHAR   szDisplayName[256];
} NETWORK_ADPT_INFO, *PNETWORK_ADPT_INFO;

#define net_long(x) (((((ulong)(x))&0xffL)<<24) | \
                     ((((ulong)(x))&0xff00L)<<8) | \
                     ((((ulong)(x))&0xff0000L)>>8) | \
                     ((((ulong)(x))&0xff000000L)>>24))

// Used in font manager code. If other dialogs need this, move to an .h file.
#define ARRAYSIZEOF(x)  (sizeof(x) / sizeof((x)[0]))

// Font\color table for Network Property Page 1.
static const CCtlTextDesc gs_rgTextDescNetPage1[] =
{
    {IDC_NETUI_NETADAPT_PP1TITLE,    8, FW_BOLD,   COLOR_ACTIVECAPTION },
//    {IDC_NETUI_USEDHCP,              9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_DONTUSEDHCP,          9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_IPADDRLABEL,          8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_IP_ADDR,              9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_SUBNETLABEL,          8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_SUBNET_MASK,           9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_GATEWAYLABEL,         8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_GATEWAY,               9, FW_NORMAL, STANDARD_TEXT_COLOR },
};



// Font\color table for Network Property Page 2.
static const CCtlTextDesc gs_rgTextDescNetPage2[] =
{
    {IDC_NETUI_NETADAPT_PP2TITLE,    8, FW_BOLD,   COLOR_ACTIVECAPTION },
    {IDC_NETUI_AUTOMSG_STATIC,       8, FW_NORMAL, ACCENT_TEXT_COLOR },
//    {IDC_NETUI_DNS_LABEL,            9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_DNS_ADDR,             9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_DNS_ALTLABEL,         9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_DNS_ALTADDR,          9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_WINS_LABEL,           9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_WINS_ADDR,            9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_WINS_ALTLABEL,        9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_WINS_ALTADDR,         9, FW_NORMAL, STANDARD_TEXT_COLOR },
};


// Help link
static const TCHAR c_szHelpFile[] = TEXT("file:connect.htm#ethernet");

//
//  BlankCmdBarCreate
//
//  1) Creates an entirely blank command bar.
//
//  RETURN:
//      void
//
void BlankCmdBarCreate(HWND hDlg)
{
    SHMENUBARINFO cbi = {0};

    cbi.cbSize = sizeof(SHMENUBARINFO);
    cbi.hwndParent = hDlg;
    cbi.dwFlags = SHCMBF_EMPTYBAR;
    SHCreateMenuBarInternal(&cbi);
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
BOOL CALLBACK
NetworkIPDlgProc (
   HWND hDlg,
   UINT message,
   WPARAM wParam,
   LPARAM lParam
   )
{
    PROPSHEETPAGE       *psp;
    NMHDR               *header;
    PNETWORK_ADPT_INFO  pAdptInfo;
    DWORD               Flag;
    TCHAR               szTitle[256];
    TCHAR               szErrMsg[256];
    HWND                hParentWnd;
    static void*        s_pvFontManager;
    static HWND         s_hPropSheet;
    ULONG               ulFontMgrRetVal;
    SHMENUBARINFO       mbi;
    SHINITDLGINFO       shidi;

    BOOL        fRet = TRUE;

    DEBUGMSG (ZONE_NETWORK && ZONE_MISC,
              (TEXT("+NetworkIPDlgProc(0x%X, 0x%X, 0x%X, 0x%X)\r\n"),
               hDlg, message, wParam, lParam));

    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescNetPage1
    if (!s_pvFontManager && message == WM_INITDIALOG)
    {
       s_pvFontManager = SHFontMgrCreate(gs_rgTextDescNetPage1, ARRAYSIZEOF(gs_rgTextDescNetPage1));
    }

    if (s_pvFontManager)
    {
        if ( SHFontMgrManageFonts(s_pvFontManager,
                                  message,
                                  hDlg,
                                  wParam,
                                  lParam,
                                  &ulFontMgrRetVal) == CTLTEXT_EXIT )
        {
            return (BOOL)ulFontMgrRetVal;
        }
    }

    switch (message)
    {
        case WM_NOTIFY:
            header = (NMHDR*) lParam;

            switch (header->code)
            {
                case PSN_HELP:

                    CreateProcess(TEXT("peghelp"), c_szHelpFile,
                                  NULL,NULL,FALSE,0,NULL,NULL,NULL,NULL);
                    break;

                case PSN_SETACTIVE:

                    // Bring up the SIP on activation
                    SHSipPreference(hDlg, SIP_UP);
                    break;

                case PSN_APPLY:

                    DEBUGMSG (ZONE_NETWORK,
                              (TEXT(" NetworkIPDlgProc:Apply Changes....\r\n")));

                    pAdptInfo = (PNETWORK_ADPT_INFO) GetWindowLong (hDlg, GWL_USERDATA);

                    // Save the data into the struct.
                    pAdptInfo->fUseDHCP = SendMessage(GetDlgItem(hDlg, IDC_NETUI_USEDHCP),
                                                  BM_GETCHECK, 0, 0);
                    if (pAdptInfo->fUseDHCP)
                    {
                        // Clear the addresses.
                        pAdptInfo->IPAddr = 0;
                        pAdptInfo->SubnetMask = 0;
                        pAdptInfo->Gateway = 0;
                    }
                    else
                    {
                        SendMessage(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), IP_GETADDRESS, 0,
                                    (LPARAM) &pAdptInfo->IPAddr);
                        SendMessage(GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK), IP_GETADDRESS, 0,
                                    (LPARAM) &pAdptInfo->SubnetMask);
                        SendMessage(GetDlgItem(hDlg, IDC_NETUI_GATEWAY), IP_GETADDRESS, 0,
                                    (LPARAM) &pAdptInfo->Gateway);

                        // Verify that address is correct.
                        if (!pAdptInfo->IPAddr)
                        {
                            LoadString (HINST_RESDLL, IDS_NETUI_ERR_TITLE, szTitle,
                                        sizeof(szTitle)/sizeof(TCHAR));
                            LoadString (HINST_RESDLL, IDS_NETUI_NETWORK_ERR_BADIP, szErrMsg,
                                        sizeof(szErrMsg)/sizeof(TCHAR));
                            MessageBox (hDlg, szErrMsg, szTitle, MB_ICONEXCLAMATION | MB_SETFOREGROUND);
                            SetWindowLong (hDlg, DWL_MSGRESULT,
                                           PSNRET_INVALID_NOCHANGEPAGE);
                            break;
                        }
                    }
                    pAdptInfo->fSaved = TRUE;

                    // Free the font manager object.
                    if (s_pvFontManager)
                    {
                        SHFontMgrDestroy(s_pvFontManager);
                        s_pvFontManager = NULL;
                    }

                    // Put the SIP down on close.
                    SHSipPreference(hDlg, SIP_DOWN);

                    SetWindowLong (hDlg, DWL_MSGRESULT, PSNRET_NOERROR);
                    break;

                default:
                    break;
            }
            break;

        case WM_INITDIALOG:
            // Adding Scroll Bar support
            shidi.dwMask    = SHIDIM_FLAGS;
            shidi.hDlg      = hDlg;
            shidi.dwFlags   = SHIDIF_WANTSCROLLBAR;
                
            SHInitDialog(&shidi);

            // This is always the first window drawn. Get Parent.
            hParentWnd = GetParent(hDlg);

            // If invoked directly (i.e. not from a control panel
            // then make sure that we are marked as topmost
            if (!GetWindow(hParentWnd, GW_OWNER))
            {
                SetWindowPos(hParentWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);
            }

            psp = (PROPSHEETPAGE *)lParam;

            SetWindowLong (hDlg, GWL_USERDATA, psp->lParam);
            pAdptInfo = (PNETWORK_ADPT_INFO)psp->lParam;

            DEBUGMSG (ZONE_NETWORK,
                      (TEXT(" NetworkIPDlgProc: WM_INITDIALOG(lParam=0x%X(%s)\r\n"),
                       lParam, (pAdptInfo) ?
                       pAdptInfo->szAdapterName : TEXT("<NULL>")));

            // Save the name away.
            SetWindowLong (hDlg, DWL_USER, (LONG)pAdptInfo);

            // Create the command bar.
            memset(&mbi, 0, sizeof(SHMENUBARINFO));
            mbi.cbSize     = sizeof(SHMENUBARINFO);
            mbi.hwndParent = hDlg;
            mbi.nToolBarId = IDR_NETUI_CANCEL_BAR;
            mbi.hInstRes   = HINST_RESDLL;
            SHCreateMenuBarInternal(&mbi);
            
            // Set the header text.
            Edit_SetText(GetDlgItem(hDlg, IDC_NETUI_NETADAPT_PP1TITLE),
                         pAdptInfo->szDisplayName[0] ?
                         pAdptInfo->szDisplayName : pAdptInfo->szAdapterName);

            EnableWindow(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), !pAdptInfo->fUseDHCP);
            EnableWindow(GetDlgItem(hDlg, IDC_NETUI_IPADDRLABEL), !pAdptInfo->fUseDHCP);

            EnableWindow(GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK), !pAdptInfo->fUseDHCP);
            EnableWindow(GetDlgItem(hDlg, IDC_NETUI_SUBNETLABEL), !pAdptInfo->fUseDHCP);

            EnableWindow(GetDlgItem(hDlg, IDC_NETUI_GATEWAY), !pAdptInfo->fUseDHCP);
            EnableWindow(GetDlgItem(hDlg, IDC_NETUI_GATEWAYLABEL), !pAdptInfo->fUseDHCP);

            // set the timer
            SetTimer(hDlg, IDT_IPADDRINFO, TIMEOUT_IPADDRINFO, NULL);

            if (pAdptInfo->fUseDHCP)
            {
                // Set the default state.
                CheckRadioButton (hDlg, IDC_NETUI_USEDHCP, IDC_NETUI_DONTUSEDHCP, IDC_NETUI_USEDHCP);
                SendMessage(hDlg, WM_COMMAND, IDC_NETUI_USEDHCP, 0);

            }
            else
            {
                CheckRadioButton (hDlg, IDC_NETUI_USEDHCP, IDC_NETUI_DONTUSEDHCP,
                                  IDC_NETUI_DONTUSEDHCP);

                if (pAdptInfo->IPAddr)
                {
                SendMessage (GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), IP_SETADDRESS, 0,
                             (LPARAM) pAdptInfo->IPAddr);
                }

                if (pAdptInfo->SubnetMask)
                {
                    SendMessage (GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK), IP_SETADDRESS, 0,
                                 (LPARAM) pAdptInfo->SubnetMask);
                }
                if (pAdptInfo->Gateway)
                {
                    SendMessage (GetDlgItem(hDlg, IDC_NETUI_GATEWAY), IP_SETADDRESS, 0,
                                 (LPARAM) pAdptInfo->Gateway);
                }

            }
            
            if (SHSkipDialogInitialFocus())
            {
                fRet = FALSE;
            }
            else
            {
            // Bring up the SIP
            SHSipPreference(hDlg, SIP_INPUTDIALOGINIT);
            }
            break;
        
        case WM_WINDOWPOSCHANGED:
        case WM_SIZE:
            SetDialogAutoScrollBarEx(hDlg);
            break;

        case WM_COMMAND :
            pAdptInfo = (PNETWORK_ADPT_INFO) GetWindowLong (hDlg, GWL_USERDATA);

            DEBUGMSG (ZONE_NETWORK,
                      (TEXT(" NetworkIPDlgProc: WM_COMMAND(wParam=0x%X)\r\n"),
                       wParam));
            if ((HIWORD(wParam) == EN_SETFOCUS) &&
                ((HWND)lParam == GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK)))
            {
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), IP_GETADDRESS, 0,
                            (LPARAM) &pAdptInfo->IPAddr);
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK), IP_GETADDRESS, 0,
                            (LPARAM) &pAdptInfo->SubnetMask);
                if (pAdptInfo->IPAddr && (0 == pAdptInfo->SubnetMask))
                {
                    // Initialize a subnet mask
                    // Get top byte
                    Flag = pAdptInfo->IPAddr >> 24;
                    if (Flag <= 127)
                    {
                        pAdptInfo->SubnetMask = 0xFF000000;
                    }
                    else if (Flag <= 191)
                    {
                        pAdptInfo->SubnetMask = 0xFFFF0000;
                    }
                    else
                    {
                        // Ok I'm ignoring class D or E
                        pAdptInfo->SubnetMask = 0xFFFFFF00;
                    }
                    SendMessage (GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK),
                                 IP_SETADDRESS, 0,
                                 (LPARAM) pAdptInfo->SubnetMask);
                }
                break;
            }

            switch( LOWORD( wParam ) )
            {
                case IDCANCEL:
                    // Forward this to parent (property sheet)
                    SendMessage(GetParent(hDlg), message, wParam, lParam);
                    break;

                case IDC_NETUI_USEDHCP:
                case IDC_NETUI_DONTUSEDHCP:
                {
                    // When either of these change...
                    if (SendMessage(GetDlgItem(hDlg, IDC_NETUI_DONTUSEDHCP),
                                    BM_GETCHECK, 0, 0))
                    {
                        Flag = TRUE;
                    }
                    else
                    {
                        Flag = FALSE;

                        SendMessage(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR),
                                    IP_CLEARADDRESS, 0, 0);
                        SendMessage(GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK),
                                    IP_CLEARADDRESS, 0, 0);
                        SendMessage(GetDlgItem(hDlg, IDC_NETUI_GATEWAY),
                                    IP_CLEARADDRESS, 0, 0);

                        // Clear the values.
                        pAdptInfo->IPAddr = 0;
                        pAdptInfo->SubnetMask = 0;
                        pAdptInfo->Gateway = 0;

                    }
                    EnableWindow(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), Flag);
                    EnableWindow(GetDlgItem(hDlg, IDC_NETUI_IPADDRLABEL), Flag);

                    EnableWindow(GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK), Flag);
                    EnableWindow(GetDlgItem(hDlg, IDC_NETUI_SUBNETLABEL), Flag);

                    EnableWindow(GetDlgItem(hDlg, IDC_NETUI_GATEWAY), Flag);
                    EnableWindow(GetDlgItem(hDlg, IDC_NETUI_GATEWAYLABEL), Flag);

                    // for DHCP, show a standard edit control so we can
                    // fill it with the current IP address
                    ShowWindow(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR_CUR), Flag ?
                        SW_HIDE : SW_SHOW);

                    // for static, show the [  .  .  .  ] controls and labels
                    ShowWindow(GetDlgItem(hDlg, IDC_NETUI_IP_ADDR), Flag ?
                        SW_SHOW : SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_NETUI_SUBNETLABEL),
                        Flag ? SW_SHOW : SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_NETUI_SUBNET_MASK), Flag ?
                        SW_SHOW : SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_NETUI_GATEWAYLABEL), Flag ?
                        SW_SHOW : SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_NETUI_GATEWAY), Flag ?
                        SW_SHOW : SW_HIDE);

                    if (!Flag)
                    {
                        // update now
                        KillTimer(hDlg, IDT_IPADDRINFO);
                        PostMessage(hDlg, WM_TIMER, IDT_IPADDRINFO, 0);
                    }
                    break;
                }
            }
            break;

        case WM_TIMER:
        {
            BOOL                 fConnected = FALSE;
            DWORD                dwResult, cb;
            IP_ADAPTER_ADDRESSES *piaaHead;

            pAdptInfo = (PNETWORK_ADPT_INFO)GetWindowLong(hDlg,
                GWL_USERDATA);

            // update the IP info
            cb = 0;
            dwResult = GetAdaptersAddresses(AF_UNSPEC, 0, NULL, NULL,
                &cb);
            ASSERT(ERROR_BUFFER_OVERFLOW == dwResult);
            piaaHead = (IP_ADAPTER_ADDRESSES *)LocalAlloc(0, cb);
            if (NULL != piaaHead)
            {
                dwResult = GetAdaptersAddresses(AF_UNSPEC, 0, NULL,
                    piaaHead, &cb);
                if (ERROR_SUCCESS == dwResult)
                {
                    TCHAR                szName[MAX_ADAPTER_NAME_LENGTH];
                    IP_ADAPTER_ADDRESSES *piaaCur;

                    for (piaaCur = piaaHead; piaaCur != NULL;
                            piaaCur = piaaCur->Next)
                    {
                        MultiByteToWideChar(CP_ACP, 0, piaaCur->AdapterName,
                            -1, szName, MAX_ADAPTER_NAME_LENGTH);
                        if (0 == _tcsnicmp(szName, pAdptInfo->szAdapterName,
                                min( _tcslen(szName),
                                _tcslen(pAdptInfo->szAdapterName) ) ) )
                        {
                            // found it!
                            break;
                        }
                    }

                    if (NULL != piaaCur)
                    {
                        TCHAR szBuf[INET6_ADDRSTRLEN];
                        DWORD cch;

                        // show the IP address
                        if (NULL != piaaCur->FirstUnicastAddress)
                        {
                            int iResult;

                            fConnected = TRUE;

                            cch = INET6_ADDRSTRLEN;
                            iResult = WSAAddressToString(
                                    piaaCur->FirstUnicastAddress->Address.
                                    lpSockaddr, piaaCur->
                                    FirstUnicastAddress->Address.
                                    iSockaddrLength, NULL, szBuf, &cch);
                            ASSERT(0 == iResult);
                            SetWindowText(GetDlgItem(hDlg,
                                IDC_NETUI_IP_ADDR_CUR), szBuf);
                        }
                    }

                }
                LocalFree(piaaHead);
            }

            if (!fConnected)
            {
                LPTSTR pszNC = (LPTSTR)LoadString(HINST_RESDLL,
                        IDS_NETUI_NOT_CONNECTED, NULL, 0);

                // not connected
                SetWindowText(GetDlgItem(hDlg,
                    IDC_NETUI_IP_ADDR_CUR), pszNC);
            }

            // set the timer for next update
            SetTimer(hDlg, IDT_IPADDRINFO, TIMEOUT_IPADDRINFO, NULL);
            break;
        }

        case WM_ACTIVATE:

            if (LOWORD(wParam) == WA_INACTIVE)
            {
                // Put the SIP down on de-activate.
                SHSipPreference(hDlg, SIP_DOWN);

            }
            break;

        case WM_PAINT:
        {
            HDC             hdcPaint;
            PAINTSTRUCT     PaintStruct;

            // Get the device context.
            hdcPaint = BeginPaint(hDlg, &PaintStruct);

            // Paint the line under the header text.
            SHHdrGrpSepLineDraw(hDlg, GetDlgItem(hDlg, IDC_NETUI_NETADAPT_PP1TITLE), hdcPaint, SHD_HEADER);
            EndPaint(hDlg, &PaintStruct);
            fRet = FALSE;
            break;
        }

        case WM_DESTROY:
            KillTimer(hDlg, IDT_IPADDRINFO);
            // vvv FALL THROUGH vvv

        default :
            DEBUGMSG (ZONE_NETWORK && ZONE_MISC,
                      (TEXT("-NetworkIPDlgProc: (default) Return FALSE\r\n")));
            fRet = FALSE;
    }
    DEBUGMSG (ZONE_NETWORK && ZONE_MISC,
             (TEXT("-NetworkIPDlgProc: Return %d\r\n"), fRet));
    return fRet;

}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
BOOL CALLBACK
NetworkNameServDlgProc (
   HWND hDlg,
   UINT message,
   WPARAM wParam,
   LPARAM lParam
   )
{
    PROPSHEETPAGE       *psp;
    NMHDR               *header;
    PNETWORK_ADPT_INFO  pAdptInfo;
    static void*       s_pvFontManager;
    ULONG               ulFontMgrRetVal;
    BOOL         fRet = TRUE;
    SHINITDLGINFO       shidi;

    static const TCHAR c_szNameServHelp[] = TEXT("file:connect.htm#name_servers");

    DEBUGMSG (ZONE_NETWORK && ZONE_MISC,
              (TEXT("+NetworkNameServDlgProc(0x%X, 0x%X, 0x%X, 0x%X)\r\n"),
               hDlg, message, wParam, lParam));

    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescNetPage2
    if (!s_pvFontManager && message == WM_INITDIALOG)
    {
       s_pvFontManager = SHFontMgrCreate(gs_rgTextDescNetPage2, ARRAYSIZEOF(gs_rgTextDescNetPage2));
    }

    if (s_pvFontManager)
    {
        if ( SHFontMgrManageFonts(s_pvFontManager,
                                  message,
                                  hDlg,
                                  wParam,
                                  lParam,
                                  &ulFontMgrRetVal) == CTLTEXT_EXIT )
        {
            return (BOOL)ulFontMgrRetVal;
        }
    }

    switch (message)
    {
        case WM_NOTIFY:
            header = (NMHDR*) lParam;

            switch (header->code)
            {
                case PSN_HELP:

                    CreateProcess(TEXT("peghelp"), c_szNameServHelp,
                                  NULL,NULL,FALSE,0,NULL,NULL,NULL,NULL);
                    break;

                case PSN_SETACTIVE:

                    // Bring up the SIP on activation
                    SHSipPreference(hDlg, SIP_UP);
                    break;

                case PSN_APPLY:

                    DEBUGMSG (ZONE_NETWORK,
                              (TEXT(" NetworkNameServDlgProc:Apply Changes....\r\n")));

                    pAdptInfo = (PNETWORK_ADPT_INFO) GetWindowLong (hDlg, GWL_USERDATA);

                    SendMessage(GetDlgItem(hDlg, IDC_NETUI_DNS_ADDR), IP_GETADDRESS, 0,
                                (LPARAM) &pAdptInfo->DNSAddr);
                    SendMessage(GetDlgItem(hDlg, IDC_NETUI_DNS_ALTADDR), IP_GETADDRESS, 0,
                                (LPARAM) &pAdptInfo->DNSAltAddr);
                    SendMessage(GetDlgItem(hDlg, IDC_NETUI_WINS_ADDR), IP_GETADDRESS, 0,
                                (LPARAM) &pAdptInfo->WINSAddr);
                    SendMessage(GetDlgItem(hDlg, IDC_NETUI_WINS_ALTADDR), IP_GETADDRESS, 0,
                                (LPARAM) &pAdptInfo->WINSAltAddr);

                    DEBUGMSG (ZONE_NETWORK, (TEXT(" NetworkNameServDlgProc:\r\n")
                                             TEXT("\tDNSAddr=0x%X DNSAltAddr=0x%X\r\n")
                                             TEXT("\tWINSAddr=0x%X WINSAltAddr=0x%X\r\n"),
                                             pAdptInfo->DNSAddr, pAdptInfo->DNSAltAddr,
                                             pAdptInfo->WINSAddr, pAdptInfo->WINSAltAddr));

                    // Free the font manager object.
                    if (s_pvFontManager)
                    {
                        SHFontMgrDestroy(s_pvFontManager);
                        s_pvFontManager = NULL;
                    }

                    // Put the SIP down on close.
                    SHSipPreference(hDlg, SIP_DOWN);

                    SetWindowLong (hDlg, DWL_MSGRESULT, PSNRET_NOERROR);
                    break;

                default:
                    break;
            }
            break;

        case WM_INITDIALOG:
            // Adding Scroll Bar support
            shidi.dwMask    = SHIDIM_FLAGS;
            shidi.hDlg      = hDlg;
            shidi.dwFlags   = SHIDIF_WANTSCROLLBAR;
                
            SHInitDialog(&shidi);

            psp = (PROPSHEETPAGE *)lParam;

            SetWindowLong (hDlg, GWL_USERDATA, psp->lParam);
            pAdptInfo = (PNETWORK_ADPT_INFO)psp->lParam;
            DEBUGMSG (ZONE_NETWORK,
                      (TEXT(" NetworkNameServDlgProc: WM_INITDIALOG(lParam=0x%X(%s)\r\n"),
                       lParam, (pAdptInfo) ?
                       pAdptInfo->szAdapterName : TEXT("<NULL>")));

            // Save the name away.
            SetWindowLong (hDlg, DWL_USER, (LONG)pAdptInfo);

            // Set the header text.
            Edit_SetText(GetDlgItem(hDlg, IDC_NETUI_NETADAPT_PP2TITLE),
                         pAdptInfo->szDisplayName[0] ?
                         pAdptInfo->szDisplayName : pAdptInfo->szAdapterName);

            if (pAdptInfo->DNSAddr)
            {
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_DNS_ADDR), IP_SETADDRESS, 0,
                            (LPARAM) pAdptInfo->DNSAddr);
            }
            if (pAdptInfo->DNSAltAddr)
            {
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_DNS_ALTADDR), IP_SETADDRESS, 0,
                            (LPARAM) pAdptInfo->DNSAltAddr);
            }
            if (pAdptInfo->WINSAddr)
            {
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_WINS_ADDR), IP_SETADDRESS, 0,
                            (LPARAM) pAdptInfo->WINSAddr);
            }
            if (pAdptInfo->WINSAltAddr)
            {
                SendMessage(GetDlgItem(hDlg, IDC_NETUI_WINS_ALTADDR), IP_SETADDRESS, 0,
                            (LPARAM) pAdptInfo->WINSAltAddr);
            }
            
            if (SHSkipDialogInitialFocus())
            {
                fRet = FALSE;
            }
            else
            {
            // Bring up the SIP
            SHSipPreference(hDlg, SIP_INPUTDIALOGINIT);
            }
            break;

        case WM_COMMAND:
            pAdptInfo = (PNETWORK_ADPT_INFO) GetWindowLong (hDlg, GWL_USERDATA);
            break;

        case WM_WINDOWPOSCHANGED:
        case WM_SIZE:
            SetDialogAutoScrollBarEx(hDlg);
            break;

        case WM_PAINT:
        {
            HDC             hdcPaint;
            PAINTSTRUCT     PaintStruct;

            // Get the device context.
            hdcPaint = BeginPaint(hDlg, &PaintStruct);

            // Paint the line under the header text.
            SHHdrGrpSepLineDraw(hDlg, GetDlgItem(hDlg, IDC_NETUI_NETADAPT_PP2TITLE), hdcPaint, SHD_HEADER);
            EndPaint(hDlg, &PaintStruct);
            fRet = FALSE;
            break;
        }

        default:
            DEBUGMSG (ZONE_NETWORK && ZONE_MISC,
                      (TEXT("-NetworkNameServDlgProc: (default) Return FALSE\r\n")));
            fRet = FALSE;
    }
    DEBUGMSG (ZONE_NETWORK && ZONE_MISC,
             (TEXT("-NetworkIPDlgProc: Return %d\r\n"), fRet));
    return fRet;
}

int CALLBACK
AdapterPropSheetProc(
   HWND hwndDlg,  // handle to the property sheet dialog box
   UINT uMsg,     // message identifier
   LPARAM lParam  // message parameter
   )
{
    UNREFERENCED_PARAMETER(hwndDlg);  // unreferenced in retail builds
    UNREFERENCED_PARAMETER(lParam);  // unreferenced in retail builds
    
    DEBUGMSG (ZONE_NETWORK,
              (TEXT("AdapterPropSheetProc(0x%X, 0x%X, 0x%X)\r\n"),
              hwndDlg, uMsg, lParam));

    switch(uMsg)
    {
        case PSCB_GETVERSION:
            return COMCTL32_VERSION;

        case PSCB_NOPRECREATE:
            return 1;
    }
    return 0;
}


void
IPAddrToStr (LPTSTR szStr, DWORD IPAddr)
{
    wsprintf (szStr, TEXT("%d.%d.%d.%d"),
              (IPAddr >> 24) & 0xFF, (IPAddr >> 16) & 0xFF,
              (IPAddr >> 8) & 0xFF,  IPAddr & 0xFF);
}


BOOL StringToAddr(TCHAR *AddressString, DWORD *AddressValue) {
    TCHAR   *pStr = AddressString;
    PUCHAR  AddressPtr = (PUCHAR)AddressValue;
    int     i;
    int     Value;

    // Parse the four pieces of the address.
    for (i=0; *pStr && (i < 4); i++) {
        Value = 0;
        while (*pStr && TEXT('.') != *pStr) {
            if ((*pStr < TEXT('0')) || (*pStr > TEXT('9'))) {
                DEBUGMSG (ZONE_NETWORK,
                          (TEXT("Unable to convert %s to address\r\n"),
                           AddressString));
                return FALSE;
            }
            Value *= 10;
            Value += *pStr - TEXT('0');
            pStr++;
        }
        if (Value > 255) {
            DEBUGMSG (ZONE_NETWORK,
                      (TEXT("Unable to convert %s to address\r\n"),
                       AddressString));
            return FALSE;
        }
        AddressPtr[i] = Value;
        if (TEXT('.') == *pStr) {
            pStr++;
        }
    }

    // Did we get all of the pieces?
    if (i != 4) {
        DEBUGMSG (ZONE_NETWORK,
                  (TEXT("Unable to convert %s to address\r\n"),
                   AddressString));
        return FALSE;
    }

    *AddressValue = net_long (*AddressValue);

    DEBUGMSG (ZONE_NETWORK,
              (TEXT("Converted %s to address %X\r\n"),
               AddressString, *AddressValue));

    return TRUE;
}   // StringToAddr()

VOID
GetMultiSZAddr (HKEY hKey, LPTSTR szValueName, LPDWORD pAddr1, LPDWORD pAddr2)
{
    TCHAR   szTemp[256];
    LPTSTR  szPtr;
    LONG    hRes;
    DWORD   dwSize, dwType;

    dwSize = sizeof(szTemp);
    hRes = RegQueryValueEx (hKey, szValueName, 0, &dwType, (LPBYTE)szTemp,
                            &dwSize);
    if ((hRes == ERROR_SUCCESS) &&
        ((dwType == REG_SZ) || (dwType == REG_MULTI_SZ))) {
        if (pAddr1 && szTemp[0]) {
            StringToAddr (szTemp, pAddr1);
        }
        if (pAddr2 && szTemp[0] && (dwType == REG_MULTI_SZ)) {
            szPtr = szTemp;
            while (*szPtr) {
                szPtr++;
            }
            // Move past the null.
            szPtr++;
            if (*szPtr) {
                StringToAddr (szPtr, pAddr2);
            }
        }
    } else {
        DEBUGMSG (ZONE_NETWORK,
                  (TEXT("GetMultiSZ: '%s' hRes=%d dwType=%d\r\n"),
                   szValueName, hRes, dwType));
    }

}


HANDLE CheckPrevInstance(LPCTSTR pszMutexName/*, int iPage*/)
{
    HANDLE hMutex;

    // try to create a mutex object
    hMutex=CreateMutex(NULL, FALSE, pszMutexName);
    if(!hMutex) {
        // Failed for unknown reasons. We now won't have single-instanceing
        ASSERT(FALSE);
        return (HANDLE)-1;
    }

    // We got the mutex. Now check whether it previously existed
    if(ERROR_ALREADY_EXISTS==GetLastError())
    {
        HWND hwnd;
        int i = 3;
        
        // release the handle so mutex will be freed when the other guy exits
        CloseHandle(hMutex);
        do
        {
            hwnd = FindWindow(TEXT("IPWorkerWindow"), 0);
            if (NULL != hwnd)
            {
                SetForegroundWindow( (HWND)((DWORD)hwnd | 1) );
                return NULL;
            }

            //OK, window not found yet, let's be nice (especially for test and
            //SPIDER.EXE) and give it some time...
            Sleep(2000);
        }
        while (--i);

        // Bad news. Mutex existed but window did not. Carry on anyway
        ASSERT(FALSE);
        return (HANDLE)-1;

    } else {
        // if we got here we have a first-time created mutex. Return the
        // handle so caller can free it before exiting
        return hMutex;
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
BOOL
AdapterIPProperties (HWND hWndOwner, LPTSTR szAdapterName)
{
    BOOL                fRetVal = FALSE;
    TCHAR               szTemp[256];
    HKEY                hKey;
    LONG                hRes;
    DWORD               dwDisp, dwSize, dwType;
    NETWORK_ADPT_INFO   AdptInfo, OldAdptInfo;
    PROPSHEETPAGE       psp[2];
    PROPSHEETHEADER     psh;
    DWORD               Len;
    HANDLE              hMutex;
    BOOL fDestroyOwner = FALSE;

    //This fucntion requires trust. Check the process we are running in.
    if(CeGetCurrentTrust() != OEM_CERTIFY_TRUST)
    {
        ASSERT(FALSE);
        return 0;
    }

    hMutex = CheckPrevInstance((LPTSTR)TEXT("IPProperty"));
    if(hMutex)
    {
        WSADATA wsad;

        SHInitExtraControls();

        DEBUGMSG (ZONE_NETWORK|ZONE_INTERFACE|ZONE_FUNCTION,
                  (TEXT("+AdapterIPProperties(0x%X, 0x%X(%s))\r\n"),
                   hWndOwner, szAdapterName,
                   szAdapterName ? szAdapterName : TEXT("<NULL>")));
        {
            INITCOMMONCONTROLSEX    icce;

            icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icce.dwICC  = ICC_TOOLTIP_CLASSES|ICC_CAPEDIT_CLASS;

            // We need the common control functions
            if (!ValidCommCtrlFunc(InitCommonControlsEx))
            {
                DEBUGMSG (ZONE_ERROR|ZONE_NETWORK,
                          (TEXT("!AdapterIPProperties: Common Controls not loaded\r\n")));
                return FALSE;
            }


            // Initialize the common controls.
            DoCallCommCtrlFunc(InitCommonControlsEx)(&icce);
        }

        // I need PropertySheetW as well.
        if (!ValidCommCtrlFunc(PropertySheetW))
        {
            DEBUGMSG (ZONE_ERROR|ZONE_NETWORK,
                (TEXT("!AdapterIPProperties: Common Controls not loaded\r\n")));
            return FALSE;
        }

        if (0 != WSAStartup(MAKEWORD(2, 0), &wsad))
        {
            DEBUGMSG(ZONE_ERROR | ZONE_NETWORK,
                (TEXT("!AdapterIPProperties: WinSock not loaded\r\n")));
            return FALSE;
        }

        // Ignore errors if it's already registered.
        RegisterIPClass( HINST_RESDLL );

        // Initialize the adapter Info.
        memset ((char *)&AdptInfo, 0, sizeof(AdptInfo));

        AdptInfo.szAdapterName = szAdapterName;
        // By default we enable DHCP.
        AdptInfo.fUseDHCP = TRUE;

        // Get the current registry data.
        _tcscpy (szTemp, TEXT("Comm\\"));
        _tcscat (szTemp, szAdapterName);
        hKey = NULL;
        hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
        if (ERROR_SUCCESS == hRes)
        {
            AdptInfo.szDisplayName[0] = TEXT('\0');
            dwSize = sizeof(AdptInfo.szDisplayName);
            RegQueryValueEx (hKey, TEXT("DisplayName"), NULL, NULL,
                             (LPBYTE)AdptInfo.szDisplayName,
                             &dwSize);
            RegCloseKey (hKey);
        }


        // Get the current registry data.
        _tcscpy (szTemp, TEXT("Comm\\"));
        _tcscat (szTemp, szAdapterName);
        _tcscat (szTemp, TEXT("\\Parms\\TcpIp"));

        hKey = NULL;
        hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
        if (ERROR_SUCCESS == hRes)
        {
            // Read the saved data in.
            dwSize = sizeof(DWORD);
            hRes = RegQueryValueEx (hKey, TEXT("EnableDHCP"), 0, &dwType,
                                    (LPBYTE)&AdptInfo.fUseDHCP,
                                    &dwSize);
            if ((ERROR_SUCCESS == hRes) && (!AdptInfo.fUseDHCP))
            {
                GetMultiSZAddr (hKey, TEXT("IpAddress"),
                                &AdptInfo.IPAddr, NULL);
                GetMultiSZAddr (hKey, TEXT("Subnetmask"),
                                &AdptInfo.SubnetMask, NULL);
                GetMultiSZAddr (hKey, TEXT("DefaultGateway"),
                                &AdptInfo.Gateway, NULL);
            }
            GetMultiSZAddr (hKey, TEXT("DNS"), &AdptInfo.DNSAddr,
                            &AdptInfo.DNSAltAddr);
            GetMultiSZAddr (hKey, TEXT("WINS"), &AdptInfo.WINSAddr,
                            &AdptInfo.WINSAltAddr);

        }
        else
        {
            DEBUGMSG (ZONE_NETWORK, (TEXT(" AdapterIPProperties: Unable to open key, using default\r\n")));
        }

        {
            //Initialize tool tips
            INITCOMMONCONTROLSEX icce;
            icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icce.dwICC  = ICC_TOOLTIP_CLASSES;
            InitCommonControlsEx(&icce);
        }

        if (!hWndOwner)
        {
            // Don't want a NULL parent
            hWndOwner = SHCreateWorkerWindow((LPTSTR)WORKERWINDOW_CLASS,
                                  NULL,
                                  NULL,
                                  WS_EX_NOACTIVATE,
                                  WS_OVERLAPPED | WS_VISIBLE,
                                  NULL,
                                  0, 0);
            SetWindowPos(hWndOwner, NULL, -10, -10, 5, 5, SWP_NOZORDER | SWP_NOACTIVATE);
            fDestroyOwner = TRUE;
        }

        // Save copy of Adptinfo to detect changes
        memcpy(&OldAdptInfo, &AdptInfo, sizeof (AdptInfo));

        // Ok now set up property sheet info.
        psp[0].dwSize = sizeof(PROPSHEETPAGE);
        psp[0].dwFlags = PSP_USETITLE;
        psp[0].hInstance = HINST_RESDLL;
        psp[0].pszTemplate = MAKEINTRESOURCE(IDD_NETUI_NETWORK_IP2);
        psp[0].pszIcon = NULL;
        psp[0].pfnDlgProc = NetworkIPDlgProc;
        psp[0].pszTitle = MAKEINTRESOURCE(IDS_NETUI_NETWORK_IPADDR);
        psp[0].lParam = (LPARAM)&AdptInfo;
        psp[0].pfnCallback = NULL;

        psp[1].dwSize = sizeof(PROPSHEETPAGE);
        psp[1].dwFlags = PSP_USETITLE;
        psp[1].hInstance = HINST_RESDLL;
        psp[1].pszTemplate = MAKEINTRESOURCE(IDD_NETUI_NETWORK_NAMESERV2);
        psp[1].pszIcon = NULL;
        psp[1].pfnDlgProc = NetworkNameServDlgProc;
        psp[1].pszTitle = MAKEINTRESOURCE(IDS_NETUI_NETWORK_NAMESERV);
        psp[1].lParam = (LPARAM)&AdptInfo;
        psp[1].pfnCallback = NULL;


        psh.dwSize     = sizeof(PROPSHEETHEADER);
        psh.dwFlags    = PSH_PROPSHEETPAGE | PSH_USECALLBACK | PSH_MAXIMIZE;
        psh.hwndParent = hWndOwner;
        psh.hInstance  = HINST_RESDLL;
        psh.pszIcon    = NULL;
        psh.pszCaption = MAKEINTRESOURCE(IDS_NETUI_NETWORK_IP_TITLE);
        psh.nPages     = sizeof(psp) / sizeof(PROPSHEETPAGE);
        psh.nStartPage = 0;       // we will always jump to the first page
        psh.ppsp       = psp;
        psh.pfnCallback = AdapterPropSheetProc;

        DoCallCommCtrlFunc(PropertySheetW)(&psh);

        UnregisterIPClass( HINST_THISDLL );

        if (fDestroyOwner)
        {
            DestroyWindow(hWndOwner);
        }

        // Did they hit OK? Also, check whether we had settings before (hKey non NULL)
        // and whether the user changed anything.  If not, return FALSE so that we
        // know not to rebind the interface.
        OldAdptInfo.fSaved = TRUE;  // To allow memcmp below
        if (AdptInfo.fSaved && 
            ((hKey == NULL) || memcmp(&OldAdptInfo,&AdptInfo,sizeof(AdptInfo))))
        {
            DEBUGMSG (ZONE_NETWORK,
                      (TEXT(" AdapterIPProperties: Save settings\r\n")));
            // Did key exist before?
            if (hKey == NULL)
            {
                // Create it.
                _tcscpy (szTemp, TEXT("Comm\\"));
                _tcscat (szTemp, szAdapterName);
                _tcscat (szTemp, TEXT("\\Parms\\TcpIp"));
                hRes = RegCreateKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, NULL,
                                       REG_OPTION_NON_VOLATILE, 0, NULL,
                                       &hKey, &dwDisp);
                if (hRes != ERROR_SUCCESS)
                {
                    DEBUGMSG (ZONE_ERROR|ZONE_NETWORK,
                              (TEXT("!Unable to create reg key '%s'\r\n"),
                               szTemp));
                }
            }
            // Just in case the create failed.
            if (hKey != NULL)
            {
                fRetVal = TRUE;
                // I'd like 1 to mean TRUE.
                if (AdptInfo.fUseDHCP)
                {
                    AdptInfo.fUseDHCP = 1;
                }
                RegSetValueEx (hKey, TEXT("EnableDHCP"), 0, REG_DWORD,
                               (LPBYTE)&(AdptInfo.fUseDHCP), sizeof(DWORD));

                IPAddrToStr (szTemp, AdptInfo.IPAddr);
                Len = _tcslen(szTemp)+1;
                szTemp[Len++] = TEXT('\0');
                RegSetValueEx (hKey, TEXT("IpAddress"), 0, REG_MULTI_SZ,
                               (LPBYTE)szTemp, sizeof(TCHAR)*Len);

                IPAddrToStr (szTemp, AdptInfo.SubnetMask);
                Len = _tcslen(szTemp)+1;
                szTemp[Len++] = TEXT('\0');
                RegSetValueEx (hKey, TEXT("Subnetmask"), 0, REG_MULTI_SZ,
                               (LPBYTE)szTemp, sizeof(TCHAR)*Len);

                IPAddrToStr (szTemp, AdptInfo.Gateway);
                Len = _tcslen(szTemp)+1;
                szTemp[Len++] = TEXT('\0');
                RegSetValueEx (hKey, TEXT("DefaultGateway"), 0, REG_MULTI_SZ,
                               (LPBYTE)szTemp, sizeof(TCHAR)*Len);

                Len = 0;
                if (AdptInfo.DNSAddr)
                {
                    IPAddrToStr (szTemp, AdptInfo.DNSAddr);
                    Len = _tcslen(szTemp)+1;
                    if (AdptInfo.DNSAltAddr)
                    {
                        IPAddrToStr (szTemp+Len, AdptInfo.DNSAltAddr);
                        Len += _tcslen(szTemp+Len);
                    }
                    szTemp[Len++] = TEXT('\0');
                }
                else if (AdptInfo.DNSAltAddr)
                {
                    IPAddrToStr (szTemp, AdptInfo.DNSAltAddr);
                    Len = _tcslen(szTemp)+1;
                    szTemp[Len++] = TEXT('\0');
                }
                if (Len)
                {
                    RegSetValueEx (hKey, TEXT("DNS"), 0, REG_MULTI_SZ,
                                   (LPBYTE)szTemp, sizeof(TCHAR)*Len);
                }
                else
                {
                    RegSetValueEx (hKey, TEXT("DNS"), 0, REG_MULTI_SZ,
                                   (LPBYTE)TEXT(""), sizeof(TCHAR));
                }


                Len = 0;
                if (AdptInfo.WINSAddr)
                {
                    IPAddrToStr (szTemp, AdptInfo.WINSAddr);
                    Len = _tcslen(szTemp)+1;
                    if (AdptInfo.WINSAltAddr)
                    {
                        IPAddrToStr (szTemp+Len, AdptInfo.WINSAltAddr);
                        Len += _tcslen(szTemp+Len);
                    }
                    szTemp[Len++] = TEXT('\0');
                }
                else if (AdptInfo.WINSAltAddr)
                {
                    IPAddrToStr (szTemp, AdptInfo.WINSAltAddr);
                    Len = _tcslen(szTemp)+1;
                    szTemp[Len++] = TEXT('\0');
                }
                if (Len)
                {
                    RegSetValueEx (hKey, TEXT("WINS"), 0, REG_MULTI_SZ,
                                   (LPBYTE)szTemp, sizeof(TCHAR)*Len);
                }
                else
                {
                    RegSetValueEx (hKey, TEXT("WINS"), 0, REG_MULTI_SZ,
                                   (LPBYTE)TEXT(""), sizeof(TCHAR));
                }


                RegCloseKey (hKey);
            }
        }
        else
        {
            DEBUGMSG (ZONE_NETWORK, (TEXT("Don't save settings\r\n")));
        }

        DEBUGMSG (ZONE_NETWORK|ZONE_INTERFACE|ZONE_FUNCTION,
                  (TEXT("-AdapterIPProperties: Returning %d\r\n"), fRetVal));

        if(hMutex != (HANDLE)-1)
            CloseHandle(hMutex);

        WSACleanup();

   }
   return fRetVal;
}

