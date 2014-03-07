//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of your Windows Mobile
// Software Shared Source Premium Derivatives License Form or other
// applicable Microsoft license agreement for the software. If you did not
// accept the terms of such a license, you are not authorized to use this
// source code.
//
#ifndef _CXXUTIL_H_
#define _CXXUTIL_H_
//***   cxxutil.h --
//

//Pull in system wide helper macros from OSSVCS
#include "guts.h"

// unicode stuff
#ifdef UNICODE
/*  should this be wcsncpy or _tcsncpy? */
#define lstrcpyn    wcsncpy
#else
#define lstrcpyn    lstrcpynA
#endif

// for string constants that are always wide
#define __TEXTW(x)  L##x
#define TEXTW(x)    __TEXTW(x)

// DTMF action characters
#define ISDTMFPAUSEACTION(ch)   (((ch) == TEXT('p')) || ((ch) == TEXT(',')))
#define ISDTMFTALKACTION(ch)    ((ch) == TEXT('w'))
#define ISDTMFACTION(ch)        (ISDTMFPAUSEACTION(ch) || ISDTMFTALKACTION(ch))
#define DTMFACTIONS             TEXT("p,w")

//***   IS_TDIGIT, IS_TEXDIGIT -- is this a key on a phone numeric keypad
// IS_TDIGIT: just like isdigit(), but includes '*' and '#'
// IS_TEXDIGIT: additionally includes 'A'/'B'/'C'/'D'.
// NOTES
//  could use/share the t9 char-class table from cdial

// obsolete (instead, use ISVK_* or ISCHAR_*)
#define IS_TDIGIT(c)    ((TEXT('0') <= (c) && (c) <= TEXT('9')) || \
    (c) == TEXT('*') || (c) == TEXT('#'))

#define IS_TEXDIGIT(c)  (IS_TDIGIT(c) || ((TEXT('A') <= (c) && (c) <= TEXT('D'))))

// new
#define ISVK_ALPHA(ch)  ( TEXT('A') <= ch && ch <= TEXT('Z') )
#define ISVK_SYMBOL(ch) ( (VK_SEMICOLON <= ch && ch <= VK_BACKQUOTE) || (VK_LBRACKET <= ch && ch <= VK_APOSTROPHE) )
#define ISVK_TDIGIT(vk)     ((TEXT('0') <= (vk) && (vk) <= TEXT('9')) || \
    (vk) == VK_TSTAR || (vk) == VK_TPOUND)
#define ISVK_TEXDIGIT(vk)   *** NYI ***
#define ISCHAR_TDIGIT(c)    IS_TDIGIT(c)
#define ISCHAR_TDIGIT_OR_DTMFACTION(c)  (ISCHAR_TDIGIT(c) || ISDTMFACTION(c))
#define ISCHAR_TEXDIGIT(c)  *** NYI ***
#define ISCHAR_ALPHA(c)     (TEXT('A') <= (c) && (c) <= TEXT('z'))
#define ISCHAR_FILTER(c)    (_istprint(c) && ((c) != TEXT('\t')))

// Converts a valid ISVK_TDIGIT VKey to a TCHAR.
#define VK_TDIGIT2TCHAR(x) (VK_TSTAR == (x) ? TEXT('*') : (VK_TPOUND == (x) ? TEXT('#') : ((TCHAR)x)))

// Converts a valid ISCHAR_TDIGIT TCHAR to a VKey
#define CHAR_TCHAR2TDIGIT(x) (_T('*') == (x) ? VK_TSTAR : (_T('#') == (x) ? VK_TPOUND : ((TCHAR)x)))

#if 0 // if/when we need the extra abstraction...
//***   IS_PHONE1ST -- is this key 1st digit of a phone # (or speed dial)
//
#define IS_PHONE1ST(c)  IS_TDIGIT(c)
#endif

#endif // _CXXUTIL_H_
