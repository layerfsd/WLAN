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
#include "TodayPlugin.hpp"
#include "Debug.hpp"
#include "appmetrics.h"
#include <StatStor.h>
#include <wpcpriv.h>
#include <appmet.h>
#include <mddres.h>
#include <pimstore.h> // for IPOutlook2*
#include "voipphonecanvas.hpp"
#include "shellapip.h"


// Turn on hi-dpi
HIDPI_ENABLE

#define IDI_VOIP_PHONE          (RES_CTLPNL_NONLOCBASE + 0x82)

#define WM_OVERRIDE_NETWORK (WM_APP + 6)

#define OCN_CELLULAR    0x00000000
#define OCN_VOIP        0x00000001

// Implement PHOverrideCurrentNetwork function locally, to remove dependency on tpcutil
static
HRESULT
PHOverrideCurrentNetwork(
    DWORD dwNetwork
    )
{
    HRESULT hr;

    HWND hwndVoIP = FindWindow(
                    VoIPPhoneCanvas_t::sc_NotificationWindowClassName,
                    VoIPPhoneCanvas_t::sc_NotificationWindowName
                    );
    if (hwndVoIP)
    {
        hr = SendMessage(hwndVoIP, WM_OVERRIDE_NETWORK, dwNetwork, 0);
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}

// Helper function to obtain line height using Today item font
static
int
GetItemLineHeight(
    HFONT TodayItemFont
    )
{
    int Height = 0;

    HDC DeviceContext = GetDC(NULL);
    if (DeviceContext)
    {
        HFONT OldFont;
        SIZE CharacterSize;

        OldFont= reinterpret_cast<HFONT>(SelectObject(DeviceContext, TodayItemFont));
        if (OldFont)
        {
            // Get the height for drawing a character with the current font
            if (GetTextExtentPoint32(DeviceContext, L"0", 1, &CharacterSize))
            {
                Height = CharacterSize.cy;
            }
            SelectObject(DeviceContext, OldFont);
        }

        ReleaseDC(NULL, DeviceContext);
    }

    // Initialize to minimum height if something failed
    if (0 == Height)
    {
        Height = IsFE() ? AM_TODAY_VOIP_LINEITEMHEIGHT_FE_CY :
                        AM_TODAY_VOIP_LINEITEMHEIGHT_WWE_CY ;
    }

    return Height;
}


//constants
const WCHAR TodayVoIPPlugin_t::sc_PluginName[] = L"InternetCallingPlugin";

/*------------------------------------------------------------------------------
    TodayVoIPPlugin_t::TodayVoIPPlugin_t

    Constructor
------------------------------------------------------------------------------*/
TodayVoIPPlugin_t::TodayVoIPPlugin_t(
    )
{
    TRACE(ZONE_VOIP_CTOR);

    m_VoIPServiceStatus[0] = L'\0';

    m_Icon = NULL;
    m_TodayItemWindow = NULL;
    m_TodayItemFont = NULL;
    m_TodayItemHeight = 0;

    m_Selected = FALSE;
}

/*------------------------------------------------------------------------------
    TodayVoIPPlugin_t::~TodayVoIPPlugin_t

    Destructor
------------------------------------------------------------------------------*/
TodayVoIPPlugin_t::~TodayVoIPPlugin_t(
    )
{
    TRACE(ZONE_VOIP_CTOR);

    // Destroy icon
    if (m_Icon != NULL)
    {
        DestroyIcon(m_Icon);
    }
}

/*------------------------------------------------------------------------------
    TodayVoIPPlugin_t::InitializePlugin

    Initialize the plugin.

    Parameters:
        : IN - HWND - Handle to the today item window

    Returns:
        - None
------------------------------------------------------------------------------*/
void
TodayVoIPPlugin_t::InitializePlugin(
    HWND TodayItemWindow
    )
{
    HINSTANCE ResourceDPILibrary;

    m_TodayItemWindow = TodayItemWindow;

    // initialize the font
    m_TodayItemFont = SHGetStyleFont(SHSF_TODAYLVITEMLINE1, NULL);
    ASSERT(NULL != m_TodayItemFont);

    // Might need to load library
    ResourceDPILibrary = DPI_LoadLibraryRes(RES_CTLPNL_DLL);
    if(ResourceDPILibrary == NULL)
    {
        ASSERT(0);
        return;
    }

    // Load the icon
    // Should we support icon override?
    // LoadIconOver(HINST_RESDLLDPI, IDI_TODAY_VOIP, IDI_OVERRIDE_TODAY_VOIP_ICON);
    m_Icon = static_cast<HICON>(LoadImage(
                    ResourceDPILibrary,
                    MAKEINTRESOURCE(IDI_VOIP_PHONE),
                    IMAGE_ICON,
                    AM_SM_CXSMICON,
                    AM_SM_CYSMICON,
                    LR_DEFAULTCOLOR
                    ));
    ASSERT(m_Icon);

    FreeLibrary(ResourceDPILibrary);
}

/*------------------------------------------------------------------------------
    TodayVoIPPlugin_t::OnAction

    Handle user action.

    Parameters:
        : None

    Returns:
        - None
------------------------------------------------------------------------------*/
void
TodayVoIPPlugin_t::OnAction(
    void
    )
{
    enum
        {
        ipropPhoneNetworkPreferences,
        ipropVoIPStatusFlags,
        ipropPhoneStatusFlags,
        ipropPhoneStatusFlagsEx,
        };

    static const StatusDataID s_QueryStatusData[] =
    {
        SDID_PHONE_NETWORK_PREFERENCES,
        SDID_VOIP_STATUS_FLAGS,
        SDID_PHONE_STATUS_FLAGS,
        SDID_PHONE_STATUS_FLAGS_EX,
    };
    HRESULT hr;
    CEPROPVAL* pPropVal = NULL;
    PHONE_NETWORK_PREFERENCES InternetCalling;

    hr = StatStoreGetInfo(ARRAYSIZE(s_QueryStatusData), s_QueryStatusData, &pPropVal);
    if (FAILED(hr))
    {
        return;
    }

    InternetCalling = (pPropVal[ipropPhoneNetworkPreferences].wFlags & CEDB_PROPNOTFOUND) ?
                    PNP_VOIP_OFF : (PHONE_NETWORK_PREFERENCES)pPropVal[ipropPhoneNetworkPreferences].val.ulVal;

    // Toggle between VoIP and Cell network if device is registered with both
    // of them. Otherwise take the user to the control panel settings.
    if ((PNP_VOIP_OFF != InternetCalling) &&
        !(pPropVal[ipropPhoneStatusFlags].wFlags & CEDB_PROPNOTFOUND) &&
        ((pPropVal[ipropPhoneStatusFlags].val.ulVal & PSF_REGISTERED_HOME) ||
        (pPropVal[ipropPhoneStatusFlags].val.ulVal & PSF_REGISTERED_ROAM)) &&
        !(pPropVal[ipropVoIPStatusFlags].wFlags & CEDB_PROPNOTFOUND) &&
        (pPropVal[ipropVoIPStatusFlags].val.ulVal & VSF_REGISTERED))
    {
        // Override the default outoing network
        DWORD dwOutgoingNetwork;

        if (!(pPropVal[ipropPhoneStatusFlagsEx].wFlags & CEDB_PROPNOTFOUND) &&
            (pPropVal[ipropPhoneStatusFlagsEx].val.ulVal & EPSF_LINE_INTERNET))
        {
            dwOutgoingNetwork = OCN_CELLULAR;
        }
        else
        {
            dwOutgoingNetwork = OCN_VOIP;
        }
        PHOverrideCurrentNetwork(dwOutgoingNetwork);
    }
    else
    {
        // Launch the phone settings
        SHRunSafeAppletEx(APPLET_CPL_MAIN, APPLETID_PHONE_CPL, PSPID_PHONE_CPL_INTERNET_TAB);
    }

    LocalFree(pPropVal);
}

/*------------------------------------------------------------------------------
    TodayVoIPPlugin_t::OnPaint

    Draws the plugin item.

    Parameters:
        : NONE

    Returns:
        - TRUE if drawing was succesful, FALSE otherwise.
------------------------------------------------------------------------------*/
BOOL
TodayVoIPPlugin_t::OnPaint(
    void
    )
{
    BOOL Success = FALSE;
    PAINTSTRUCT PaintStruct;
    HDC DeviceContext;
    HFONT OldFont = NULL;
    COLORREF HighlightColor;
    COLORREF HighlightedTextColor;
    COLORREF TextColor;
    RECT ClientArea;
    RECT DrawTextRectangle;
    HWND ParentWindow = GetParent(m_TodayItemWindow);
    TODAYDRAWWATERMARKINFO DrawWatermarkInfo = {0};

    DeviceContext = BeginPaint(m_TodayItemWindow, &PaintStruct);
    if (DeviceContext == NULL)
    {
        ASSERT(0);
        goto error;
    }

    GetClientRect(m_TodayItemWindow, &ClientArea);

    // Draw watermark
    DrawWatermarkInfo.rc = ClientArea;
    DrawWatermarkInfo.hwnd = m_TodayItemWindow;
    DrawWatermarkInfo.hdc = DeviceContext;

    // Fill window with default color on failure
    if (!SendMessage(ParentWindow, TODAYM_DRAWWATERMARK, 0, (LPARAM) &DrawWatermarkInfo))
    {
        SHFillRectClr(DeviceContext, &ClientArea, GetSysColor (COLOR_WINDOW));
    }

    // Get colors for Today screen item
    HighlightColor = SendMessage(
                    ParentWindow,
                    TODAYM_GETCOLOR,
                    TODAYCOLOR_HIGHLIGHT,
                    0
                    );
    HighlightedTextColor = SendMessage(
                    ParentWindow,
                    TODAYM_GETCOLOR,
                    TODAYCOLOR_HIGHLIGHTEDTEXT,
                    0
                    );
    TextColor = SendMessage(
                    ParentWindow,
                    TODAYM_GETCOLOR,
                    TODAYCOLOR_TEXT,
                    0);

    // Setup context based on selection state
    if (m_Selected)
    {
        HBRUSH hBrush;
        SetBkMode(DeviceContext, OPAQUE);
        SetBkColor(DeviceContext, HighlightColor);
        SetTextColor(DeviceContext, HighlightedTextColor);

        hBrush = CreateSolidBrush (HighlightColor);
        FillRect(DeviceContext, &ClientArea, hBrush);
        DeleteObject(hBrush);
    }
    else
    {
        SetBkMode(DeviceContext, TRANSPARENT);
        SetTextColor(DeviceContext, TextColor);
    }

    // draw the icon
    if (!DrawIcon(
                    DeviceContext,
                    ClientArea.left + AM_TODAY_VOIP_ICON_MARGIN_CX,
                    ClientArea.top + AM_TODAY_VOIP_ICON_MARGIN_CX,
                    m_Icon
                    ))
    {
       ASSERT(0);
       VOIP_DEBUGMSG(ZONE_VOIP_ERROR, (L"Error drawing icon - Last Error (decimal) %d\n", GetLastError()));
    }

    // Set Today screen font for this item
    OldFont = reinterpret_cast<HFONT>(SelectObject(DeviceContext, m_TodayItemFont));

    // Draw VoIP Service status string
    DrawTextRectangle = ClientArea;
    DrawTextRectangle.left += AM_TODAY_VOIP_TEXT_TEXTMARGIN_CX + AM_TODAY_VOIP_ICON_MARGIN_CX;
    DrawTextRectangle.bottom = DrawTextRectangle.top + m_TodayItemHeight;

    DrawText(
                    DeviceContext,
                    m_VoIPServiceStatus,
                    -1,
                    &DrawTextRectangle,
                    DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOPREFIX
                    );
    Success = TRUE;

error:
    if (OldFont != NULL)
    {
        SelectObject(DeviceContext, OldFont);
    }

    if (DeviceContext != NULL)
    {
        EndPaint(m_TodayItemWindow, &PaintStruct);
    }

    return Success;
} /* FDrawCustomItem */

/*------------------------------------------------------------------------------
    TodayVoIPPlugin_t::s_TodayItemWindowProc

    Determines if cached data needs to be refreshed.

    Parameters:
        : IN - TODAYLISTITEM* - Today Screen item's information

    Returns:
        - TRUE if the item needs updating (today screen will redraw and
            resize the item as needed)
        - FALSE if the item does not need updating
------------------------------------------------------------------------------*/
BOOL 
TodayVoIPPlugin_t::OnQueryRefreshCache(
    TODAYLISTITEM *pTodayListItem
    )
{
    HRESULT hr;
    WCHAR NewVoIPServiceStatus[MAX_PATH];

    hr = PHGetCurrentVoIPServiceData(
                    0,
                    NewVoIPServiceStatus,
                    ARRAYSIZE(NewVoIPServiceStatus)
                    );
    if (FAILED(hr))
    {
        ASSERT(0);
        VOIP_DEBUGMSG(ZONE_VOIP_ERROR, (L"Failed call to PHGetCurrentVoIPServiceData(). Error 0x%08x -- ", hr));
        // Null terminate status string for safety
        NewVoIPServiceStatus[0] = L'\0';
    }   

    // now compare to see if we need to refresh
    // cached data already existed before, bail if they are identical
    if (!wcscmp(m_VoIPServiceStatus, NewVoIPServiceStatus))
    {
        return FALSE;
    }

    StringCchCopy(
                    m_VoIPServiceStatus,
                    ARRAYSIZE(m_VoIPServiceStatus),
                    NewVoIPServiceStatus
                    );

    pTodayListItem->cyp = GetItemLineHeight(m_TodayItemFont);
    m_TodayItemHeight = pTodayListItem->cyp;

    // Set selectability
    pTodayListItem->dwSelectability = TODAY_SELECTABILITY_CUSTOMSELECTION;

    if (pTodayListItem->cyp < (DWORD)AM_TODAY_VOIP_MINITEMHEIGHT_CY)
    {
        pTodayListItem->cyp = (DWORD)AM_TODAY_VOIP_MINITEMHEIGHT_CY;
        m_TodayItemHeight = pTodayListItem->cyp;
    }

    return TRUE;
} /* FRefreshCustomItemCache */

/*------------------------------------------------------------------------------
    TodayVoIPPlugin_t::s_TodayItemWindowProc

    Handle any messages for the Today Screen plugin

    Parameters:
        : IN - Window - Handle to the today item window
        : IN - UINT - Message sent to the window
        : IN - WPARAM - Specifies additional message-specific info
        : IN - LPARAM - Specifies additional message-specific info

    Returns:
        - LRESULT, value specifies the result of the processed message.
------------------------------------------------------------------------------*/
LRESULT
CALLBACK
TodayVoIPPlugin_t::s_TodayItemWindowProc(
    HWND Window,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam
    )
{
    LRESULT Result = 0;
    TodayVoIPPlugin_t *pThis;

    pThis = reinterpret_cast<TodayVoIPPlugin_t*>(GetWindowLong(Window, GWL_USERDATA));
    if (pThis == NULL)
    {
        goto do_default;
    }

    switch (Message)
    {
        case WM_TODAYCUSTOM_QUERYREFRESHCACHE:
            //check to see if a refresh is required
            Result = pThis->OnQueryRefreshCache(reinterpret_cast<TODAYLISTITEM*>(wParam));
            break;

        case WM_LBUTTONUP:
            pThis->OnAction();
            break;

        case WM_PAINT:
            pThis->OnPaint();
            // return value indicates that the message was processed
            break;

        case WM_TODAYCUSTOM_RECEIVEDSELECTION:
        case WM_TODAYCUSTOM_LOSTSELECTION:
            pThis->m_Selected = (Message == WM_TODAYCUSTOM_RECEIVEDSELECTION);
            InvalidateRect(Window, 0, TRUE);
            UpdateWindow(Window);
            Result = 1;
            break;

        case WM_DESTROY:
            // Delete our object
            SetWindowLong(Window, GWL_USERDATA, 0);
            delete pThis;
            break;

        case WM_ERASEBKGND:
            Result = 1;
            break;

do_default:
        default:
            Result = DefWindowProc(Window, Message, wParam, lParam);
    }

    return Result;
}

/*------------------------------------------------------------------------------
    InitializeCustomItem

    Initialize plugin for the Today Screen by creating a new window

    Parameters:
        : IN - TODAYLISTITEM* - Today Screen item's information
        : IN - HWND - Handle of the parent window

    Returns:
        - HWND of the created window
------------------------------------------------------------------------------*/
extern "C"
HWND
InitializeCustomItem(
    TODAYLISTITEM *pTodayListItem,
    HWND hwndParent
    )
{
    DWORD FeatureLevel;

    // Per documentation, the function should check fEnabled and
    // return NULL immediately if it is FALSE

    if (!pTodayListItem->fEnabled)
    {
        return NULL;
    }

    // Otherwise make sure we really are enabled
    FeatureLevel = SHDFL_NOVOIP;

    SHGetDeviceFeatureLevel(SHDFLI_VOIP, &FeatureLevel);
    if (!(FeatureLevel & SHDFL_VOIP))
    {
        return NULL;
    }

    // VoIP plugin should only be available on GSM phone.
    // Use same logic as Control Panel, where it is assumed that
    // CDMA doesn't necessarily need to support voice privacy.
    FeatureLevel = SHDFL_NOPHONE;

    SHGetDeviceFeatureLevel(SHDFLI_PHONE, &FeatureLevel);
    if (!(FeatureLevel & SHDFL_PHONE_LOCKFACILITY))
    {
        return NULL;
    }

    TodayVoIPPlugin_t *pThisPlugin = new TodayVoIPPlugin_t();
    if (pThisPlugin == NULL)
    {
        ASSERT(FALSE);
        return NULL;
    }

    //Init HIDPI scaling
    HIDPI_InitScaling();

    ASSERT(pTodayListItem->hinstDLL == GlobalData_t::ProgramInstance);

    //register the listener window class
    WNDCLASS WindowClass = {0};
    WindowClass.lpfnWndProc = TodayVoIPPlugin_t::s_TodayItemWindowProc;
    WindowClass.hInstance = GlobalData_t::ProgramInstance;
    WindowClass.lpszClassName = TodayVoIPPlugin_t::sc_PluginName;

    //it's ok if our class already exists...
    if (!RegisterClass(&WindowClass))
    {
        int LastError = GetLastError();
        if (LastError != ERROR_CLASS_ALREADY_EXISTS)
        {
            VOIP_DEBUGMSG(ZONE_VOIP_ERROR, (L"Could not register Today Item window class - LastError=%08X", LastError));
            return NULL;
        }
    }

    // Create a new window
    HWND TodayItemWindow = CreateWindow(
                    TodayVoIPPlugin_t::sc_PluginName,
                    TodayVoIPPlugin_t::sc_PluginName,
                    WS_CHILD,
                    0,0,0,0,
                    hwndParent,
                    NULL,
                    pTodayListItem->hinstDLL,
                    NULL
                    );
    ASSERT(TodayItemWindow);

    pThisPlugin->InitializePlugin(TodayItemWindow);

    // Set user data to be pointer to our object
    SetWindowLong(TodayItemWindow, GWL_USERDATA, (LONG)pThisPlugin);

    // Display the window
    ShowWindow (TodayItemWindow, SW_SHOWNORMAL);
    UpdateWindow (TodayItemWindow) ;

    return TodayItemWindow;
}

