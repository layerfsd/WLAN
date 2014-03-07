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
// Client specific application metrics

#define AM_NETUI_WZCSTATEICONS_CX        SCALEX(16)

// match SHBox's toast dimensions 2 * ( AM_AYGSHELL_TOAST_GUTTER_CX (4) +
// AM_AYGSHELL_TOAST_INTERNALMARGIN_LEFT (0) +
// AM_AYGSHELL_TOAST_INTERNALMARGIN_RIGHT (0) ) + width_of_radio_button (16)
#define AM_NETUI_WZCBUBBLESSIDPADDING SCALEX(24)
// on FE system, the font will be 9 pt instead of 8 pt WWE.
// Therefore we only have about 88% as much pixel extent as we
// think we have. The padding is adjusted accordingly: 240-24=216px on WWE
// 216x0.88=190px effectively on FE. So padding of 50 is required.
#define AM_NETUI_WZCBUBBLESSIDPADDING_EA SCALEX(50)

// this must be set to 12.  When outputting listview text, all of the item rects are shrunk to 
// pad some space between columns.  This effects all subitems (items not in column 0) and their left and 
// right margins are shrunk by 6 pixels or a total of 12 pixels.
#define AM_NETUI_WZCLISTVIEWMARGIN_CX     SCALEX((2 * 6))

#define AM_TODAY_RADIO_LINEITEMHEIGHT_WWE_CY     SCALEY(13)      // The height of a line in the Today screen
#define AM_TODAY_RADIO_LINEITEMHEIGHT_FE_CY      SCALEY(15)      // The height of a line in the Today screen for Asia
#define AM_TODAY_RADIO_TEXT_TEXTMARGIN_CY        SCALEY(4)       // margin from top of plugin to text start
#define AM_TODAY_RADIO_BT_RIGHT_CX               SCALEX(75)      // offset from the right of the rect to display BT info

#define AM_TODAY_RADIO_ICON_MARGIN_CX            SCALEX(2)       // margin from left side of icon
#define AM_TODAY_RADIO_ICON_MARGIN_CY            SCALEY(2)       // margin from top of icon

#define AM_TODAY_RADIO_TEXT_TEXTMARGIN_CX        SCALEX(26)      // margin from left side of icon to text start
#define AM_TODAY_RADIO_ICON_MARGIN_CX            SCALEX(2)       // margin from left side of icon
#define AM_TODAY_RADIO_TEXT_TEXTMARGIN_WIFI_CX   SCALEX(10)      // margin to leave at the right of the Wi-Fi string

#define AM_TODAY_RADIO_MINITEMHEIGHT_CY          SCALEY(20)      // The minimal height of an item in the Today screen



