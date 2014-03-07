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
/*
    Copyright (c) Microsoft Corporation. All rights reserved.
*/

// Copied from tshres.h
#define TSHRES_BASE_CONNPLAN      15000

#define IDS_CONNPLAN_AUTOMATIC    (TSHRES_BASE_CONNPLAN + 0)
#define IDS_CONNPLAN_NONE         (TSHRES_BASE_CONNPLAN + 1)

// resource.h - resource ID definitions

#define IDS_NETWORKCARD         0x9203
#define IDS_DTPT                0x9204
#define IDS_WIRELESSCARD        0x9205
#define IDS_CSPNET_CONNECTED    0x9206
#define IDS_CSPNET_DSTFORMAT    0x9207

// also used in private\shellw\tele\tshres\0409\netui_p.rc
#define IDB_WIRELESS_BARS_BASE  600
#define IDB_WIRELESS_BARS_1     600
#define IDB_WIRELESS_BARS_2     601
#define IDB_WIRELESS_BARS_3     602
#define IDB_WIRELESS_BARS_4     603

// resource.h - resource ID definitions

#define IDS_CSP_PROXY           0x9210

// resource.h - resource ID definitions

#define IDS_CSPRAS_CONNECTGPRS 0x9208
#define IDS_CSPRAS_CONNECTVPN  0x9209
#define IDS_CSPRAS_GPRS        0x920A
#define IDS_CSPRAS_DIALUP      0x920B
#define IDS_CSPRAS_VPN         0x920C
#define IDS_CSPRAS_ENTRYFORMAT 0x920D
#define IDS_CSPRAS_INCALL      0x920E

// Copyright (c) 1999, Microsoft Corp. All rights reserved.
//***   resource.h --
//

#ifdef RC_INVOKED
#undef IDM_YES
#undef IDM_NO
#undef IDC_STATIC
#endif  // RC_INVOKED

// The compiler won't let menu names be additions, so we have to set them to a number
// This makes sure that we won't build if TSHRES_BASE_SIMUI is changed -- if you get
// this, you'll have to manually update IDR_*, IDD_*, and IDI_*, and IDB_* constants yourself (sorry)
#if (TSHRES_BASE_SIMUI == 10000)
#define IDR_MENU_MENUBAR            10200 // (TSHRES_BASE_SIMUI + 200)
#define IDR_TEXT_MENUBAR            10201 // (TSHRES_BASE_SIMUI + 201)
#define IDR_INPUT_MENUBAR           10202 // (TSHRES_BASE_SIMUI + 202)
#define IDR_CALL_MENUBAR            10203 // (TSHRES_BASE_SIMUI + 203)
#define IDR_PROACTIVETEXT_MENUBAR   10204 // (TSHRES_BASE_SIMUI + 204)

#define IDB_HIGHPRIORITY	        10210 // (TSHRES_BASE_SIMUI + 210)

#define IDD_MENU                    10220 // (TSHRES_BASE_SIMUI + 220)
#define IDD_MAIN                    10221 // (TSHRES_BASE_SIMUI + 221)
#define IDD_TEXT                    10222 // (TSHRES_BASE_SIMUI + 222)
#define IDD_INPUT                   10223 // (TSHRES_BASE_SIMUI + 223)
#define IDD_CALL                    10224 // (TSHRES_BASE_SIMUI + 224)
#define IDD_INPUT_PASSWORD          10225 // (TSHRES_BASE_SIMUI + 225)
#endif // (TSHRES_BASE_SIMUI == 10000)

#define IDS_SIMUI_HELP              (TSHRES_BASE_SIMUI + 250)
#define IDS_PLACECALL               (TSHRES_BASE_SIMUI + 251)
#define IDS_SIMUI_REJECT            (TSHRES_BASE_SIMUI + 252)
#define IDS_SIMUI_DONE              (TSHRES_BASE_SIMUI + 254)
// !!! UNUSED !!!                   (TSHRES_BASE_SIMUI + 255)
// !!! UNUSED !!!                   (TSHRES_BASE_SIMUI + 256)
#define IDS_CALLFORMAT              (TSHRES_BASE_SIMUI + 257)
// !!! UNUSED !!!                   (TSHRES_BASE_SIMUI + 258)
#define IDS_UNSPECIFIED_ADDRESS     (TSHRES_BASE_SIMUI + 259)
#define IDS_SIMUI_SELECT            (TSHRES_BASE_SIMUI + 260)
// !!! UNUSED !!!                   (TSHRES_BASE_SIMUI + 261)
#define IDS_SIMUI_MORE              (TSHRES_BASE_SIMUI + 262)
#define IDS_SIMUI_CLOSE             (TSHRES_BASE_SIMUI + 263)

#define IDM_SIMUI_SELECT            (TSHRES_BASE_SIMUI + 270)
#define IDM_QUIT                    (TSHRES_BASE_SIMUI + 271)
#define IDM_SIMUI_HELP              (TSHRES_BASE_SIMUI + 272)
#define IDM_SIMUI_DONE              (TSHRES_BASE_SIMUI + 273)
#define IDM_PLACECALL               (TSHRES_BASE_SIMUI + 274)
#define IDM_SIMUI_REJECT            (TSHRES_BASE_SIMUI + 275)
#define IDM_YES                     (TSHRES_BASE_SIMUI + 276)
#define IDM_NO                      (TSHRES_BASE_SIMUI + 277)

#define IDC_MENULIST                (TSHRES_BASE_SIMUI + 280)
#define IDC_TEXT                    (TSHRES_BASE_SIMUI + 281)
// !!! UNUSED !!!                   (TSHRES_BASE_SIMUI + 282)
#define IDC_EDITPASSWORD            (TSHRES_BASE_SIMUI + 283)
// !!! UNUSED !!!                   (TSHRES_BASE_SIMUI + 284)
#define IDC_INFORMATION             (TSHRES_BASE_SIMUI + 285)
#define IDC_TEXTBITMAP              (TSHRES_BASE_SIMUI + 286)

#define IDS_LAUNCHBROWSER_CSDNAME   (TSHRES_BASE_SIMUI + 290)
#define IDS_LAUNCHBROWSER_GPRSNAME  (TSHRES_BASE_SIMUI + 291)
#define IDS_LAUNCHBROWSER_PROXYNAME (TSHRES_BASE_SIMUI + 292)
#define IDS_LAUNCHBROWSER_NETNAME   (TSHRES_BASE_SIMUI + 293)
#define IDS_LAUNCHBROWSER_GWNETNAME (TSHRES_BASE_SIMUI + 294)

#define IDC_STATIC                  -1


