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

#pragma once

//***   fontman.h --  font manager, icon manager
//

//these are style bits, which correspond to BYTEs in the LOGFONT
#define FM_STYLE_UNDERLINE 0x00000001
#define FM_STYLE_ITALIC    0x00000002

struct FontInfo
{
    char cPointsS;
    char cPointsM;
    char cPointsL;
    BYTE padding;
    int lfWeight;
    DWORD dwStyles;
};

struct StandardFontInfo
{
    HFONT hfont;
    int lfHeight;
};


// using char as a signed BYTE
CASSERT(sizeof(BYTE) == sizeof(char));

// Build the enumeration of external font id's.  These are the only
// way to get at fonts; we conceal the actual internal font properties

enum eFontID
{
    FM_LISTUITEXT,
    FM_LISTUIBOLDTEXT,
    FM_LISTUILABEL,
    FM_LISTUILINKTEXT,
    _FM_APPLIST,
    _FM_APPNORMAL,
    _FM_APPREASON,
    FM_DIALERDIRECT,
    FM_DIALERDIRECT_LARGE,
    FM_DIALERDIRECT_MEDIUM,
    FM_DIALERDIRECT_SMALL,
    FM_RINGERCALLER,
    FM_RINGERINFO,
    FM_CONTACTSMESSAGE,
    FM_SOFTKEY,
    FM_CPROG_ALERT_HEADER,
    FM_CPROG_ALERT_NAME,
    FM_CPROG_ALERT_NAME_SM,
    FM_CPROG_ALERT_NUMBER,
    FM_CPROG_ALERT_LINE_ID,
    FM_SHBOX_HEADING,
    FM_SHBOX_HEADING_SMALL,
    FM_VOLDLG_LABEL,
    FM_CPROG_METHOD,
    FM_CPROG_CALL_STATUS,
    FM_CPROG_ACCUMULATOR,
    FM_CPROG_SINGLE_CALL_NAME,
    FM_CPROG_SINGLE_CALL_NAME_SM,
    FM_CPROG_SINGLE_CALL_DURATION,
    FM_CPROG_MULTI_CALL_NAME,
    FM_MAIL_BOLD,
    FM_NOTES_BOLD,
    eFontIDCount,
};

// Font ID aliases -- a lot of the fonts that we use are intentionally
// the same

#define FM_DIALERLIST       _FM_APPLIST
#define FM_DIALERNORMAL     _FM_APPNORMAL
#define FM_DIALERREASON     _FM_APPREASON

#define FM_CONTACTSLIST     _FM_APPLIST
#define FM_CONTACTSNORMAL   _FM_APPNORMAL
#define FM_CONTACTSREASON   _FM_APPREASON

#define FM_EXPLORERLIST     _FM_APPLIST
#define FM_EXPLORERNORMAL   _FM_APPNORMAL
#define FM_EXPLORERREASON   _FM_APPREASON

#define FM_CLOGLIST         _FM_APPLIST
#define FM_CLOGNORMAL       _FM_APPNORMAL
#define FM_CLOGREASON       _FM_APPREASON

#define FM_SCUTLIST         _FM_APPLIST
#define FM_SCUTNORMAL       _FM_APPNORMAL
#define FM_SCUTREASON       _FM_APPREASON

#define FM_RECORDPICKERLIST     _FM_APPLIST
#define FM_RECORDPICKER_NORMAL  FM_DIALERNORMAL
#define FM_RECORDPICKER_REASON  FM_DIALERREASON

#define FM_CPROG_CALLPICKER_LIST     _FM_APPLIST

HRESULT GetStandardFont(eFontID idFont, HFONT* pFontOut);
void ClearFontManager();

