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
// Copyright (c) 1999-1999, Microsoft Corp. All rights reserved.
//***   appmet.h -- app metrics
//

// Don't include this directly, it is included from appmet.h.


/**********************************************************************
    To use the application metrics do the following:
      - If your metric is something that you see used in other apps
        (like column separator width) see if there is a constant
        already defined for it in the APPMET_FIRST_INCLUDE section.
        You can use this constant in the metric definition.
      - In the "Application Metrics Definitions" section below, add
        your new metric to the appropriate section (Contacts, Dialer, etc.)
        If there is no appropriate section, create a new one.
      - Use the DCAMI, DCAMC, DVAMI, etc. macros to add your new value.
      - In your app, use the appropriate function to access your metric.

        metric defined with     metric accessed with
        -------------------     --------------------
        DVAMI                   GetAppMetric
        DCAMI                   GetAppMetricConst
        DCAMC                   GetAppColorConst

**********************************************************************/



/**********************************************************************
 macros used to build the enum, the table, and the constant values.

    DCAMI - Define Constant Application Metric Integer
    DCAMC - Define Constant Application Metric COLORREF
    DVAMI - Define Variable Application Metric Integer - variable with respect to system font size
**********************************************************************/
// Only one of APPMET_INIT_ENUM, APPMET_INIT_TABLE, or APPMET_INIT_CONSTANTS
// should be defined at any one time!!!
#ifdef APPMET_INIT_ENUM
#define DCAMI(id, val) ami ## id,
#define DCAMC(id, val)
#define DVAMI(id, valS, valM, valL) ami ## id,
#endif

#ifdef APPMET_INIT_TABLE
#define DCAMI(id, val) {val, val, val},
#define DCAMC(id, val)
#define DVAMI(id, valS, valM, valL) {valS, valM, valL},
#endif

#ifdef APPMET_INIT_CONSTANTS
#define DCAMI(id, val) const int CAM_I_ ## id = val;
#define DCAMC(id, val) const COLORREF CAM_C_## id = val;
#define DVAMI(id, valS, valM, valL) const enAppMetricID id = ami ## id;
#endif


#ifdef APPMET_FIRST_INCLUDE
/**********************************************************************
    Put all values that are generally the same for different metrics here
 **********************************************************************/
#define _APP_COLUMN_SEPARATOR 2
#define _APP_ROW_HEIGHT_S 15
#define _APP_ROW_HEIGHT_M 20
#define _APP_ROW_HEIGHT_L 25
#define _APP_ROW_EDIT_HEIGHT_S 17   //TODO need usability to figure this metric out
#define _APP_ROW_EDIT_HEIGHT_M 22
#define _APP_ROW_EDIT_HEIGHT_L 29   //TODO need usability to figure this metric out
#define _APP_ROW_VMARGIN        0   // top and bottom margins of rows in apps
#define _APP_ROW_HMARGIN        2   // left and right margins of rows in apps

// Two lines per item in the listview
#define _APP_DBLROW_HEIGHT_S   24
#define _APP_DBLROW_HEIGHT_M   36
#define _APP_DBLROW_HEIGHT_L   60

#endif // APPMET_FIRST_INCLUDE


/**********************************************************************
    Support for building the enum or table...
 **********************************************************************/
#ifdef APPMET_INIT_ENUM
typedef enum _enAppMetricID
    {
#endif
#ifdef APPMET_INIT_TABLE
#define NUM_FONT_SIZES      3
const int g_rgAppMetrics[][NUM_FONT_SIZES] = {
#endif

/**********************************************************************
    Application Metrics Definitions

    Use the macros defined above to define the app metrics below.
**********************************************************************/
//
// Softkey control
//
DCAMI(SOFTKEY_TEXT_BASELINE_OFFSET, 6)
DCAMI(APP_BANNER_HEIGHT, 20)

//
// Virtual Listview
//
DVAMI(VIRTLV_CY_DEFAULT_ITEM_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L) // duplicate of LISTUI_CY_READLINEHEIGHT
DVAMI(VIRTLV_CX_PAGING_MENU_WIDTH, 15, 20, 25) // this is the width of the paging numbers when in paging mode

//
// All Apps
//
DCAMI(APP_COLUMN_SEPARATOR, _APP_COLUMN_SEPARATOR)

//
// ListUI (data editor, call progress, ...)
//
DCAMI(LISTUI_CX_ITEMBORDER, 4) // right and left margins of list ui items
DCAMI(LISTUI_CY_ITEMBORDER, 4) // top and bottom margins of list ui items
DVAMI(LISTUI_CY_READLINEHEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DVAMI(LISTUI_CY_EDITLINEHEIGHT, _APP_ROW_EDIT_HEIGHT_S, _APP_ROW_EDIT_HEIGHT_M, _APP_ROW_EDIT_HEIGHT_L)
DCAMI(LISTUI_CY_EDIT_WINDOW_BUTTING, 2) // butting set to shrink ListUI client window when in edit mode
DVAMI(LISTUI_CX_LEFTLABELWIDTH, 35, 42, 49)
DVAMI(LISTUI_CX_RIGHTLABELWIDTH, 33, 40, 47)
DCAMI(LISTUI_CX_LABELRIGHTMARGIN, 4)
DCAMI(LISTUI_CX_LABELLEFTMARGINWHENONLEFT, 4) // left margin we add when the label is against the left edge of the element
DCAMI(LISTUI_CX_TEXTLEFTMARGINWHENONLEFT, 4) // left margin we add when the text is against the left edge of the element
DCAMI(LISTUI_CX_TEXTRIGHTMARGINWHENONRIGHT, 4) // right margin we add when the text is against the right edge of the element
DCAMI(LISTUI_CY_SEPARATORHEIGHT, 1) // separator height
DCAMI(LISTUI_CY_SEPARATORPAD, 0) // separator padding
DCAMC(LISTUI_MAJOR_SEP_COLOR, RGB(0x7f, 0x7f, 0x7f)) //separator color
DCAMI(LISTUI_CX_ICON, 20) // width of an icon when it is on the left or right of a value
DCAMI(LISTUI_CX_UD_ARROW_BUTTON, 13) // width of the updown control's arrow


//
// Dialer
//
DVAMI(AM_DIRECT_DI_HEIGHT, 30, 36, 45) // height of number you are dialing
DVAMI(AM_DIRECT_DI_MATCHES_HEIGHT, 13, 20, 26)   // height of the matching text
DCAMI(AM_DIRECT_DI_BORDER, 2)
DCAMI(AM_DIRECT_DI_BORDER_HPADDING, 1)  // Horizontal thickness of the border
DCAMI(AM_DIRECT_DI_BORDER_VPADDING, 1)  // Vertical thickness of the border
DCAMI(AM_DIRECT_DI_HPADDING, 1) // adjustment to height of the fitted text area to get certain special cases to work.
DCAMI(AM_DIRECT_DI_VPADDING, 2) // adjustment to height of the fitted text area to get certain special cases to work.
DVAMI(DIALER_ROW_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DCAMI(DIALER_ROW_VMARGIN, _APP_ROW_VMARGIN)
DCAMI(DIALER_ROW_HMARGIN, _APP_ROW_HMARGIN)
DCAMI(DIALER_CX_COLUMN1,   20)
// COLUMN2 is the remaining space
DCAMI(DIALER_CX_COLUMN3,   20)

//
// Call Progress (cprog)
//
DCAMI(CPROG_MINOR_SEP_HEIGHT, 3)
DCAMI(CPROG_MINOR_SEP_VMARGIN, 1)
DCAMI(CPROG_MINOR_SEP_HMARGIN, 2)
DCAMC(CPROG_MINOR_SEP_COLOR, RGB(0x7f, 0x7f, 0x7f))
DCAMI(CPROG_MAJOR_SEP_HEIGHT, 1)
DCAMI(CPROG_MAJOR_SEP_VMARGIN, 0)
DCAMI(CPROG_MAJOR_SEP_HMARGIN, 0)
DCAMC(CPROG_MAJOR_SEP_COLOR, RGB(0x7f, 0x7f, 0x7f))

DCAMI(CPROG_SINGLE_CALL_NAME_HEIGHT, 60)
DCAMI(CPROG_SINGLE_CALL_RIGHT_COLUMN_WIDTH, 20)
DCAMI(CPROG_SINGLE_CALL_COLUMN_SEP_WIDTH, 2)
DCAMI(CPROG_SINGLE_CALL_ROW_SEP_HEIGHT, 5)
DCAMI(CPROG_SINGLE_CALL_LEFT_MARGIN, _APP_ROW_HMARGIN)

DCAMI(CPROG_MULTI_CALL_NAME_HEIGHT, 60)

DVAMI(CPROG_CALLPICKER_ROW_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DCAMI(CPROG_CALLPICKER_ROW_VMARGIN, _APP_ROW_VMARGIN)
DCAMI(CPROG_CALLPICKER_ROW_HMARGIN, _APP_ROW_HMARGIN)
DCAMI(CPROG_CALLPICKER_RIGHT_COLUMN_WIDTH, 40)
DCAMI(CPROG_CALLPICKER_COLUMN_SEP_WIDTH, 2)

DCAMI(CPROG_ALERT_HEADERHEIGHT, 60)
DCAMI(CPROG_ALERT_HEADER_VMARGIN, 6)
DCAMI(CPROG_ALERT_CALLER_NAME_HEIGHT, 30)
DCAMI(CPROG_ALERT_CALLER_NUMBER_HEIGHT, 15)
DCAMI(CPROG_ALERT_HMARGIN, 2)

DCAMI(CPROG_ACCUMULATOR_HEIGHT, 38) // height of the accumulator
DCAMI(CPROG_ACCUMULATOR_BORDER, 0) // negative hides the border in the fitted text drawing
DCAMI(CPROG_ACCUMULATOR_HPADDING, 0) // adjustment to height of the fitted text area to get certain special cases to work.
DCAMI(CPROG_ACCUMULATOR_VPADDING, 0) // adjustment to height of the fitted text area to get certain special cases to work.

//
// Text boxes
//
DCAMI(SHBOX_CY_HEADING, 36) // height of the heading

//
// Contacts
//
DVAMI(CONTACTS_ROW_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DCAMI(CONTACTS_APP_ROW_VMARGIN, _APP_ROW_VMARGIN)
DCAMI(CONTACTS_APP_ROW_HMARGIN, 4)
DCAMI(CONTACTS_APP_ROW_INDENT_TPC, 8)      // Company view indent
DCAMI(CONTACTS_APP_ROW_INDENT_PPC, 10) // Company view indent on PocketPC
DCAMI(CONTACTS_APP_CX_CUSTOMICON, 24) // Width of custom icon for source providers
DCAMI(CONTACTS_APP_CY_CUSTOMICON, 16) // Height of custom icon for source providers
DCAMI(CONTACTS_APP_CX_ARROWICON, 16)  // Width of the left and right arrow icons used in contacts
DCAMI(CONTACTS_APP_CY_ARROWICON, 16)  // Height of the left and right arrow icons used in contacts

// COLUMN1 is the remaining space
DCAMI(CONTACTS_CX_COLUMN2, 28)

//
// Contact picker
//
DVAMI(CONTACTS_RECPICK_ROW_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DCAMI(CONTACTS_RECPICK_VMARGIN, _APP_ROW_VMARGIN)
DCAMI(CONTACTS_RECPICK_HMARGIN, _APP_ROW_HMARGIN)

//
// Explorer
//
DVAMI(EXPLORER_ROW_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DCAMI(EXPLORER_ROW_VMARGIN, _APP_ROW_VMARGIN)
DCAMI(EXPLORER_ROW_HMARGIN, _APP_ROW_HMARGIN)

//
// CLog
//
DVAMI(CLOG_ROW_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DCAMI(CLOG_CX_COLUMN1,   20)
// COLUMN2 is the remaining space
DCAMI(CLOG_CX_COLUMN3,   20)

//
// Shortcuts/Favorites/Speed
//
DVAMI(SCUT_ROW_HEIGHT, _APP_ROW_HEIGHT_S, _APP_ROW_HEIGHT_M, _APP_ROW_HEIGHT_L)
DCAMI(SCUT_CX_COLUMN1,   20)
// COLUMN2 is the remaining space
DCAMI(SCUT_CX_COLUMN3,   20)


// Mail
DVAMI(MAIL_ROW_HEIGHT, _APP_DBLROW_HEIGHT_S, _APP_DBLROW_HEIGHT_M, _APP_DBLROW_HEIGHT_L)


/**********************************************************************
    Support for building the enum or table...
 **********************************************************************/
#ifdef APPMET_INIT_ENUM
    amiLast
    } enAppMetricID;
#endif
#ifdef APPMET_INIT_TABLE
    };
#endif

#undef DCAMI
#undef DCAMC
#undef DVAMI

