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

linecnfg.c

Abstract:
  Called by unimodem to when the user wants to configure a
  datamodem device.

Functions:


Notes:


--*/
#include <windows.h>
#include <types.h>
#include <commctrl.h>
#include <wcommctl.h>
#include <netui.h>
#include <netuip.h>
#include <resource.h>
#include "aygshell.h"
#include "wpcpriv.h"

typedef enum {
   SETTINGS_PAGE_PORT = 0,
   SETTINGS_PAGE_DIALING,
   SETTINGS_NUM_PAGES
} PAGE_NUM;



typedef struct {
    PLINECONFIGDATA pLineConfigData;
    RECT            rcDisplay;
    DWORD           fApply;
} TABDIALOGINFO, *PTABDIALOGINFO;


// Font\color table for LineConfigDialog property page 1.
static const CCtlTextDesc gs_rgTextDescPage1[] =
{
    {IDC_NETUI_CONNPROP_PP1TITLE, 8, FW_BOLD, COLOR_ACTIVECAPTION },
    {IDC_NETUI_STATIC_BAUD,       8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_BAUD_RATE,       9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_STATIC_BITS,       8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_DATA_BITS,         9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_STATIC_PARITY,     8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_PARITY,            9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_STATIC_STOP,       8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_STOP_BITS,         9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_STATIC_FLOW,       8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_FLOW_CONTROL,          9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_CHK_PREDIAL,           9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_CHK_POSTDIAL,          9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_CHK_MANUAL,            9, FW_NORMAL, STANDARD_TEXT_COLOR },
};


// Font\color table for LineConfigDialog property page 2.
static const CCtlTextDesc gs_rgTextDescPage2[] =
{
    {IDC_NETUI_CONNPROP_PP2TITLE, 8, FW_BOLD, COLOR_ACTIVECAPTION },
//    {IDC_NETUI_CHK_TIMEOUT,       9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_EDIT_TIMEOUT,      9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_TEXT_SECONDS,      9, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_CHK_BLIND_DIAL,    9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_CARD_STATIC,       8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_EDIT_WAITBONG,     9, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_SEC_STATIC,        8, FW_NORMAL, STANDARD_TEXT_COLOR },
    {IDC_NETUI_EXTRA_STATIC,      8, FW_NORMAL, STANDARD_TEXT_COLOR },
//    {IDC_NETUI_EXTRA,             9, FW_NORMAL, STANDARD_TEXT_COLOR },
};


static const DWORD gs_dwBaudRates[] =
{
    CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800,
    CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_57600, CBR_115200
};

#define DEFAULT_BAUD_INDEX  6

#define LS_TO_TEMP(x)         (LoadString(HINST_RESDLL, x, szParamTemp, sizeof(szParamTemp)/sizeof(szParamTemp[0])) )
#define CB_ADD_ITEM2(w,x)      (SendDlgItemMessage (hDlg, w, \
                 CB_ADDSTRING, (WPARAM)0, (LPARAM)(LPCTSTR) x))
#define CB_ADD_ITEM(w,x)      (SendDlgItemMessage (hDlg, w, \
                 CB_ADDSTRING, (WPARAM)0, (LPARAM)(LPCTSTR)TEXT(x)))
#define CB_SELECT_ITEM(w, x)  (SendDlgItemMessage (hDlg, w, \
                 CB_SETCURSEL, (WPARAM) x, (LPARAM)0))
#define CB_GET_ITEM(w)        (SendDlgItemMessage (hDlg, w, \
                 CB_GETCURSEL, (WPARAM) 0, (LPARAM)0))

#define CHK_CHECK(w)          (SendDlgItemMessage (hDlg, w, \
                 BM_SETCHECK, (WPARAM) 1, (LPARAM)0))
#define CHK_UNCHECK(w)        (SendDlgItemMessage (hDlg, w, \
                 BM_SETCHECK, (WPARAM) 0, (LPARAM)0))

#define CHK_GET_CHECK(w)      (SendDlgItemMessage (hDlg, w, \
                 BM_GETCHECK, (WPARAM) 0, (LPARAM)0))

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
BOOL CALLBACK
PortSettingsDlgProc (
   HWND hDlg,
   UINT message,
   WPARAM wParam,
   LPARAM lParam
   )
{
    PROPSHEETPAGE*  psp;
    TCHAR           szParamTemp[32];
    PTABDIALOGINFO  pTabInfo;
    NMHDR*          header;
    int             iBaudRateIndex;
    static void*    s_pvFontManager;

    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescPage1
    ULONG ulFontMgrRetVal;

    if (SHRunFontManager(gs_rgTextDescPage1,
                         ARRAYSIZEOF(gs_rgTextDescPage1),
                         &s_pvFontManager,
                         message,
                         hDlg,
                         wParam,
                         lParam,
                         &ulFontMgrRetVal) == CTLTEXT_EXIT )
    {
        return (BOOL)ulFontMgrRetVal;
    }

    DEBUGMSG (ZONE_LINECONFIG, (TEXT("+PortSettingsDlgProc(0x%X, 0x%X, 0x%X, 0x%X)\r\n"),
                   hDlg, message, wParam, lParam));

    switch (message)
    {
        case WM_NOTIFY :
            header = (NMHDR*) lParam;

            if (PSN_APPLY != header->code)
            {
                if (header->code == PSN_SETACTIVE)
                {
                    // Put the SIP down.
                    SHSipPreference(hDlg, SIP_FORCEDOWN);
                    SetWindowLong(hDlg, DWL_MSGRESULT, PSNRET_NOERROR);
                }
                break;
            }
            DEBUGMSG (ZONE_LINECONFIG, (TEXT("PSN_APPLY in PortSettingsDlgProc\r\n")));
            pTabInfo = (PTABDIALOGINFO) GetWindowLong (hDlg, GWL_USERDATA);
            DEBUGMSG (ZONE_LINECONFIG, (TEXT("pTabInfo=0x%X\r\n"), pTabInfo));

            ASSERT (pTabInfo);

            pTabInfo->fApply = TRUE;

            iBaudRateIndex = CB_GET_ITEM(IDC_NETUI_BAUD_RATE);

            if (0 <= iBaudRateIndex && iBaudRateIndex < 12)
            {
                pTabInfo->pLineConfigData->dwBaudRate = gs_dwBaudRates[iBaudRateIndex];
            }
            else
            {
                pTabInfo->pLineConfigData->dwBaudRate = gs_dwBaudRates[DEFAULT_BAUD_INDEX];
            }

            pTabInfo->pLineConfigData->bParity   = (BYTE) CB_GET_ITEM (IDC_NETUI_PARITY);
            pTabInfo->pLineConfigData->bByteSize = (BYTE) CB_GET_ITEM (IDC_NETUI_DATA_BITS) + 4;
            pTabInfo->pLineConfigData->bStopBits = (BYTE) CB_GET_ITEM (IDC_NETUI_STOP_BITS);

            pTabInfo->pLineConfigData->dwModemOptions = 0;

            switch (CB_GET_ITEM(IDC_NETUI_FLOW_CONTROL))
            {
                case 0:
                    pTabInfo->pLineConfigData->dwModemOptions |=
                    NETUI_LCD_MDMOPT_SOFT_FLOW;
                    break;

                case 1:
                    pTabInfo->pLineConfigData->dwModemOptions |=
                    NETUI_LCD_MDMOPT_HARD_FLOW;
                    break;

                case 2:
                    // set no bits...
                    break;
            }

            if (1 == CHK_GET_CHECK(IDC_NETUI_CHK_MANUAL))
            {
                pTabInfo->pLineConfigData->dwTermOptions |=
                NETUI_LCD_TRMOPT_MANUAL_DIAL;
            }
            else
            {
                pTabInfo->pLineConfigData->dwTermOptions &=
                ~(NETUI_LCD_TRMOPT_MANUAL_DIAL);
            }

            if (1 == CHK_GET_CHECK(IDC_NETUI_CHK_PREDIAL))
            {
                pTabInfo->pLineConfigData->dwTermOptions |=
                NETUI_LCD_TRMOPT_PRE_DIAL;
            }
            else
            {
                pTabInfo->pLineConfigData->dwTermOptions &=
                ~(NETUI_LCD_TRMOPT_PRE_DIAL);
            }

            if (1 == CHK_GET_CHECK(IDC_NETUI_CHK_POSTDIAL))
            {
                pTabInfo->pLineConfigData->dwTermOptions |=
                NETUI_LCD_TRMOPT_POST_DIAL;
            }
            else
            {
                pTabInfo->pLineConfigData->dwTermOptions &=
                ~(NETUI_LCD_TRMOPT_POST_DIAL);
            }
            break;

        case WM_INITDIALOG :

            DEBUGMSG (ZONE_LINECONFIG, (TEXT("WM_INITDIALOG in PortSettingsDlgProc\r\n")));

            psp = (PROPSHEETPAGE *)lParam;
            pTabInfo = (PTABDIALOGINFO)psp->lParam;
            SetWindowLong (hDlg, GWL_USERDATA, (LONG)pTabInfo);
            DEBUGMSG (ZONE_LINECONFIG, (TEXT("pTabInfo=0x%X\r\n"), pTabInfo));

            ASSERT (pTabInfo);

            // Put down the SIP
            SHSipPreference(hDlg, SIP_DOWN);

            // Create a blank command bar.
            BlankCmdBarCreate(hDlg);

            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "110");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "300");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "600");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "1200");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "2400");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "4800");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "9600");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "14400");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "19200");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "38400");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "57600");
            CB_ADD_ITEM (IDC_NETUI_BAUD_RATE, "115200");

            DEBUGMSG (ZONE_LINECONFIG, (TEXT("added baud rates\r\n")));

            switch (pTabInfo->pLineConfigData->dwBaudRate)
            {
                case CBR_110:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 0);
                    break;

                case CBR_300:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 1);
                    break;

                case CBR_600:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 2);
                    break;

                case CBR_1200:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 3);
                    break;

                case CBR_2400:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 4);
                    break;

                case CBR_4800:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 5);
                    break;

                case CBR_9600:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 6);
                    break;

                case CBR_14400:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 7);
                    break;

                case CBR_19200:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 8);
                    break;

                case CBR_38400:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 9);
                    break;

                case CBR_57600:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 10);
                    break;

                case CBR_115200:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 11);
                    break;

                default:
                    CB_SELECT_ITEM(IDC_NETUI_BAUD_RATE, 6);
                    break;
            }

            LS_TO_TEMP(IDS_NETUI_PARAMETER_NONE);        //PARITY None
            CB_ADD_ITEM2 (IDC_NETUI_PARITY, szParamTemp);
            LS_TO_TEMP(IDS_NETUI_PARAMETER_ODD);         //PARITY Odd
            CB_ADD_ITEM2 (IDC_NETUI_PARITY, szParamTemp);
            LS_TO_TEMP(IDS_NETUI_PARAMETER_EVEN);        //PARITY Even
            CB_ADD_ITEM2 (IDC_NETUI_PARITY, szParamTemp);
            LS_TO_TEMP(IDS_NETUI_PARAMETER_MARK);        //PARITY Mark
            CB_ADD_ITEM2 (IDC_NETUI_PARITY, szParamTemp);
            LS_TO_TEMP(IDS_NETUI_PARAMETER_SPACE);       //PARITY Space
            CB_ADD_ITEM2 (IDC_NETUI_PARITY, szParamTemp);
            CB_SELECT_ITEM (IDC_NETUI_PARITY, pTabInfo->pLineConfigData->bParity);

            LS_TO_TEMP(IDS_NETUI_PARAMETER_SOFTWARE);    //FLOW software
            CB_ADD_ITEM2 (IDC_NETUI_FLOW_CONTROL, szParamTemp);
            LS_TO_TEMP(IDS_NETUI_PARAMETER_HARDWARE);    //FLOW hardware
            CB_ADD_ITEM2 (IDC_NETUI_FLOW_CONTROL, szParamTemp);
            LS_TO_TEMP(IDS_NETUI_PARAMETER_NONE);        //FLOW None
            CB_ADD_ITEM2 (IDC_NETUI_FLOW_CONTROL, szParamTemp);

            if (pTabInfo->pLineConfigData->dwModemOptions &
                NETUI_LCD_MDMOPT_SOFT_FLOW)
            {
                CB_SELECT_ITEM (IDC_NETUI_FLOW_CONTROL, 0);
            }
            else if (pTabInfo->pLineConfigData->dwModemOptions &
                   NETUI_LCD_MDMOPT_HARD_FLOW)
            {
                CB_SELECT_ITEM (IDC_NETUI_FLOW_CONTROL, 1);
            }
            else
            {
                CB_SELECT_ITEM (IDC_NETUI_FLOW_CONTROL, 2);   // no flow control
            }

            CB_ADD_ITEM (IDC_NETUI_DATA_BITS, "4");
            CB_ADD_ITEM (IDC_NETUI_DATA_BITS, "5");
            CB_ADD_ITEM (IDC_NETUI_DATA_BITS, "6");
            CB_ADD_ITEM (IDC_NETUI_DATA_BITS, "7");
            CB_ADD_ITEM (IDC_NETUI_DATA_BITS, "8");
            CB_SELECT_ITEM (IDC_NETUI_DATA_BITS, pTabInfo->pLineConfigData->bByteSize - 4);

            CB_ADD_ITEM (IDC_NETUI_STOP_BITS, "1");
            CB_ADD_ITEM (IDC_NETUI_STOP_BITS, "1.5");
            CB_ADD_ITEM (IDC_NETUI_STOP_BITS, "2");
            CB_SELECT_ITEM (IDC_NETUI_STOP_BITS, pTabInfo->pLineConfigData->bStopBits);

            if (pTabInfo->pLineConfigData->dwTermOptions &
                NETUI_LCD_TRMOPT_MANUAL_DIAL)
            {
                CHK_CHECK (IDC_NETUI_CHK_MANUAL);
            }
            else
            {
                CHK_UNCHECK (IDC_NETUI_CHK_MANUAL);
            }

            if (pTabInfo->pLineConfigData->dwTermOptions &
                NETUI_LCD_TRMOPT_PRE_DIAL)
            {
                CHK_CHECK (IDC_NETUI_CHK_PREDIAL);
            }
            else
            {
                CHK_UNCHECK (IDC_NETUI_CHK_PREDIAL);
            }

            if (pTabInfo->pLineConfigData->dwTermOptions &
                NETUI_LCD_TRMOPT_POST_DIAL)
            {
                CHK_CHECK (IDC_NETUI_CHK_POSTDIAL);
            }
            else
            {
                CHK_UNCHECK (IDC_NETUI_CHK_POSTDIAL);
            }

            DEBUGMSG (ZONE_LINECONFIG,
                      (TEXT("-PortSettingsDlgProc: WM_INITDIALOG Return FALSE\r\n")));
            return FALSE;

        case WM_PAINT:
        {
            HDC             hdcPaint;
            PAINTSTRUCT     PaintStruct;

            // Get the device context.
            hdcPaint = BeginPaint(hDlg, &PaintStruct);

            // Paint the line under the header text.
            SHHdrGrpSepLineDraw(hDlg, NULL, hdcPaint, SHD_HEADER);
            EndPaint(hDlg, &PaintStruct);
            return FALSE;
        }

        default :
            DEBUGMSG (ZONE_LINECONFIG,
                      (TEXT("-PortSettingsDlgProc: (default) Return FALSE\r\n")));
            return (FALSE);
    }
    DEBUGMSG (ZONE_LINECONFIG,
             (TEXT("-PortSettingsDlgProc: Return TRUE\r\n")));
    return TRUE;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
BOOL CALLBACK
CallOptionsDlgProc (
   HWND hDlg,
   UINT message,
   WPARAM wParam,
   LPARAM lParam
   )
{
    PROPSHEETPAGE*  psp;
    TCHAR           szTemp[21];
    UINT            nValue;
    TCHAR           szTemp2[(NETUI_LCD_DIAL_MOD_LEN+1)*2];
    TCHAR           szTemp3[32];
    TCHAR           szTemp4[32];
    PTABDIALOGINFO  pTabInfo;
    NMHDR           *header;
    static void*    s_pvFontManager;

    // FONT HANDLING STUFF
    // set font size, weight, and color per the table gs_rgTextDescPage2
    ULONG ulFontMgrRetVal;

    if (SHRunFontManager(gs_rgTextDescPage2,
                         ARRAYSIZEOF(gs_rgTextDescPage2),
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

        case WM_NOTIFY:
            header = (NMHDR*) lParam;

            if (PSN_APPLY != header->code)
            {
                if (header->code == PSN_SETACTIVE)
                {
                    // Bring up the SIP
                    SHSipPreference(hDlg, SIP_INPUTDIALOGINIT);
                    SetWindowLong(hDlg, DWL_MSGRESULT, PSNRET_NOERROR);
                }
                break;
            }

            DEBUGMSG (ZONE_LINECONFIG, (TEXT("PSN_APPLY in CallOptionsDlgProc\r\n")));
            pTabInfo = (PTABDIALOGINFO) GetWindowLong (hDlg, GWL_USERDATA);
            DEBUGMSG (ZONE_LINECONFIG, (TEXT("pTabInfo=0x%X\r\n"), pTabInfo));
            ASSERT (pTabInfo);

            pTabInfo->fApply = TRUE;

            // LAM - Note that this is backwards.  The box actually gets checked
            // if we do NOT want to do a blind dial.
            if (1 == CHK_GET_CHECK(IDC_NETUI_CHK_BLIND_DIAL))
            {
                pTabInfo->pLineConfigData->dwModemOptions &=
                ~(NETUI_LCD_MDMOPT_BLIND_DIAL);
            }
            else
            {
                pTabInfo->pLineConfigData->dwModemOptions |=
                NETUI_LCD_MDMOPT_BLIND_DIAL;
            }

            //
            // Map Fullwidth to halfwidth numbers
            //
            GetDlgItemText (hDlg, IDC_NETUI_EXTRA, szTemp2, ARRAYSIZE(szTemp2));
            LCMapString (LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH,
                         szTemp2, _tcslen(szTemp2)+1,
                         pTabInfo->pLineConfigData->szDialModifier,
                         pTabInfo->pLineConfigData->dwModMaxLen+1);

            // Check for maximum bong....
            //
            // Map Fullwidth to halfwidth numbers
            //
            GetDlgItemText (hDlg, IDC_NETUI_EDIT_WAITBONG, szTemp3, 16);
            LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH, szTemp3,
                        _tcslen(szTemp3)+1, szTemp4, 16);
            nValue = (UINT)_ttol(szTemp4);

            pTabInfo->pLineConfigData->wWaitBong = (nValue > 0xFFFF) ? 0xFFFF : nValue;

            if (1 == CHK_GET_CHECK(IDC_NETUI_CHK_TIMEOUT))
            {
                //
                // Map Fullwidth to halfwidth numbers
                //
                GetDlgItemText (hDlg, IDC_NETUI_EDIT_TIMEOUT, szTemp3, 16);
                LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH, szTemp3,
                            _tcslen(szTemp3)+1, szTemp4, 16);
                nValue = (UINT)_ttol(szTemp4);

                pTabInfo->pLineConfigData->dwCallSetupFailTimer = nValue;
            }
            else
            {
                pTabInfo->pLineConfigData->dwCallSetupFailTimer = 0;
            }

            DEBUGMSG (ZONE_LINECONFIG, (
                                  TEXT("*** CallOptionsDlgProc ***\r\n")
                                  TEXT("\tExtra Settings  = %s\r\n")
                                  TEXT("\tWait bong       = %d seconds\r\n")
                                  TEXT("\tTimeout         = %d seconds\r\n"),
                                  pTabInfo->pLineConfigData->szDialModifier,
                                  pTabInfo->pLineConfigData->wWaitBong,
                                  pTabInfo->pLineConfigData->dwCallSetupFailTimer
                                  ));

            break;

        case WM_INITDIALOG:
            DEBUGMSG (ZONE_LINECONFIG, (TEXT("WM_INITDIALOG in CallOptionsDlgProc\r\n")));
            psp = (PROPSHEETPAGE *)lParam;
            pTabInfo = (PTABDIALOGINFO)psp->lParam;
            SetWindowLong (hDlg, GWL_USERDATA, (LONG)pTabInfo);
            DEBUGMSG (ZONE_LINECONFIG, (TEXT("pTabInfo=0x%X\r\n"), pTabInfo));

            ASSERT (pTabInfo);

            // Put up the SIP
            SHSipPreference(hDlg, SIP_INPUTDIALOGINIT);

            if (pTabInfo->pLineConfigData->dwCallSetupFailTimer)
            {
                CHK_CHECK (IDC_NETUI_CHK_TIMEOUT);
            }
            else
            {
                CHK_UNCHECK (IDC_NETUI_CHK_TIMEOUT);
            }

            // LAM - Note that this is backwards.  The box actually gets checked
            // if we do NOT want to do a blind dial.
            if (pTabInfo->pLineConfigData->dwModemOptions &
                NETUI_LCD_MDMOPT_BLIND_DIAL)
            {
                CHK_UNCHECK (IDC_NETUI_CHK_BLIND_DIAL);
            }
            else
            {
                CHK_CHECK (IDC_NETUI_CHK_BLIND_DIAL);
            }

            //
            // Don't let the edit fields be huge...
            //
            SendDlgItemMessage (hDlg, IDC_NETUI_EXTRA, EM_SETLIMITTEXT,
                                (WPARAM) pTabInfo->pLineConfigData->dwModMaxLen,
                                (LPARAM) 0);

            SendDlgItemMessage (hDlg, IDC_NETUI_EDIT_TIMEOUT, EM_SETLIMITTEXT,
                                (WPARAM) 5, (LPARAM) 0);

            SendDlgItemMessage (hDlg, IDC_NETUI_EDIT_WAITBONG, EM_SETLIMITTEXT,
                                (WPARAM) 5, (LPARAM) 0);

            SetDlgItemText(hDlg, IDC_NETUI_EXTRA,
                           pTabInfo->pLineConfigData->szDialModifier);

            wsprintf (szTemp, TEXT("%d"), pTabInfo->pLineConfigData->wWaitBong);
            SetDlgItemText(hDlg, IDC_NETUI_EDIT_WAITBONG, szTemp);

            wsprintf (szTemp, TEXT("%u"),
                      pTabInfo->pLineConfigData->dwCallSetupFailTimer);
            SetDlgItemText(hDlg, IDC_NETUI_EDIT_TIMEOUT, szTemp);

            break;

        case WM_PAINT:
        {
            HDC             hdcPaint;
            PAINTSTRUCT     PaintStruct;

            // Get the device context.
            hdcPaint = BeginPaint(hDlg, &PaintStruct);

            // Paint the line under the header text.
            SHHdrGrpSepLineDraw(hDlg, NULL, hdcPaint, SHD_HEADER);
            EndPaint(hDlg, &PaintStruct);
            return FALSE;
        }

        default:
            return (FALSE);

    }
    return FALSE;
}

int CALLBACK PropSheetProc(
   HWND hwndDlg,  // handle to the property sheet dialog box
   UINT uMsg,     // message identifier
   LPARAM lParam  // message parameter
)
{
    UNREFERENCED_PARAMETER(hwndDlg);
    UNREFERENCED_PARAMETER(lParam);
    
    switch(uMsg)
    {
        case PSCB_GETVERSION:
            return COMCTL32_VERSION;

        case PSCB_NOPRECREATE:
            return 1;
    }
    return 0;
}


//
// ----------------------------------------------------------------
//
//  LineConfigEdit
//
//  Called by unimodem to when the user wants to configure a
//  datamodem device.
//
// ----------------------------------------------------------------
BOOL
LineConfigEdit (HWND hParent, PLINECONFIGDATA pLineConfigData)
{

    PROPSHEETPAGE psp[SETTINGS_NUM_PAGES];
    PROPSHEETHEADER psh;
    TCHAR szCaption1[30];
    TCHAR szCaption2[30];
    TCHAR szTitle[30];
    PTABDIALOGINFO  pTabInfo;
    BOOL            fRetVal = FALSE;

    DEBUGMSG (ZONE_FUNCTION|ZONE_LINECONFIG,
              (TEXT("+LineConfigEdit(0x%X, 0x%X)\r\n"),
               hParent, pLineConfigData));

    // We need the common control functions
    if (!ValidCommCtrlFunc(InitCommonControls)) {
        DEBUGMSG (ZONE_ERROR|ZONE_LINECONFIG,
                  (TEXT("!LineConfigEdit: Common Controls not loaded\r\n")));
        return FALSE;
    }

    // Initialize the common controls.
    DoCallCommCtrlFunc(InitCommonControls)();
    SHInitExtraControls();

    // I need PropertySheetW as well.
    if (!ValidCommCtrlFunc(PropertySheetW)) {
        DEBUGMSG (ZONE_ERROR|ZONE_LINECONFIG,
                  (TEXT("!LineConfigEdit: PropertySheetW not available\r\n")));
        return FALSE;
    }

    pTabInfo = LocalAlloc (LPTR, sizeof(TABDIALOGINFO));
    DEBUGMSG (ZONE_LINECONFIG, (TEXT("pTabInfo = 0x%X\r\n"), pTabInfo));

    if (NULL == pTabInfo) {
        SetLastError (ERROR_NOT_ENOUGH_MEMORY);
        return FALSE;
    }
    pTabInfo->pLineConfigData = pLineConfigData;

#define SP_PORT SETTINGS_PAGE_PORT
    psp[SP_PORT].dwSize = sizeof(PROPSHEETPAGE);
    psp[SP_PORT].dwFlags = PSP_USETITLE;
    psp[SP_PORT].hInstance = HINST_RESDLL;
    psp[SP_PORT].pszTemplate = MAKEINTRESOURCE(IDD_NETUI_PORT_SETTINGS2);
    psp[SP_PORT].pszIcon = NULL;
    psp[SP_PORT].pfnDlgProc = PortSettingsDlgProc;
    LoadString(HINST_RESDLL, IDS_NETUI_PORT_SETTINGS, szCaption1,
               sizeof(szCaption1)/sizeof(szCaption1[0]));
    psp[SP_PORT].pszTitle = szCaption1;
    psp[SP_PORT].lParam = (LPARAM)pTabInfo;
    psp[SP_PORT].pfnCallback = NULL;

#define SP_EMUL SETTINGS_PAGE_DIALING
    psp[SP_EMUL].dwSize = sizeof(PROPSHEETPAGE);
    psp[SP_EMUL].dwFlags = PSP_USETITLE;
    psp[SP_EMUL].hInstance = HINST_RESDLL;
    psp[SP_EMUL].pszTemplate = MAKEINTRESOURCE(IDD_NETUI_DIALING_SETTINGS2);
    psp[SP_EMUL].pszIcon = NULL;
    psp[SP_EMUL].pfnDlgProc = CallOptionsDlgProc;
    LoadString(HINST_RESDLL, IDS_NETUI_CALL_OPTIONS, szCaption2,
               sizeof(szCaption2)/sizeof(szCaption2[0]));
    psp[SP_EMUL].pszTitle = szCaption2;
    psp[SP_EMUL].lParam = (LPARAM)pTabInfo;
    psp[SP_EMUL].pfnCallback = NULL;


    psh.dwSize     = sizeof(PROPSHEETHEADER);
    psh.dwFlags    = PSH_PROPSHEETPAGE | PSH_MAXIMIZE | PSH_USECALLBACK;
    psh.hwndParent = hParent;
    psh.hInstance  = HINST_RESDLL;
    psh.pszIcon    = NULL;
    LoadString(HINST_RESDLL, IDS_NETUI_DEVICE_PROPERTIES, szTitle,
               sizeof(szTitle)/sizeof(szTitle[0]));
    psh.pszCaption = (LPTSTR) szTitle;
    psh.nPages     = SETTINGS_NUM_PAGES;
    psh.nStartPage = 0;       // we will always jump to the first page
    psh.ppsp       = psp;
    psh.pfnCallback = PropSheetProc;


    DoCallCommCtrlFunc(PropertySheetW)(&psh);

    // Now what?
    if (pTabInfo->fApply) {
        DEBUGMSG (ZONE_LINECONFIG, (TEXT("Apply the changes\r\n")));
        fRetVal = TRUE;
    }
    LocalFree (pTabInfo);
    DEBUGMSG (ZONE_FUNCTION|ZONE_LINECONFIG,
              (TEXT("-LineConfigEdit: %d\r\n"), fRetVal));
    return fRetVal;

}
