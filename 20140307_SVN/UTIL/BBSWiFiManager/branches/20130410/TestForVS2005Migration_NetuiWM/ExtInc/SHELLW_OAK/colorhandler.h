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
// -*- C++ -*-

//+----------------------------------------------------------------
//
//    File: ColorHander.h
//    
//    Private interface to the Color Handler class
//
//    History:
//        Created: Zeeshan Hamid (t-zhamid) June 2001
//
//-----------------------------------------------------------------

#ifndef __COLORHANDLER_H_
#define __COLORHANDLER_H_

#include "xmlhelp.h"
#include "wpcpriv.h"

const DWORD COLORCONSTANT=0x01000000; // Color Constant
const DWORD COLORTRANSPARENT=0x02000000; // Transparent
const DWORD COLORMASK=0x0F000000; // Mask value for back compat

//NOTE:
// 0x00 implies COLORREF
// 0x01 implies Color Constant
// 0x02 implies Transparent
// CLR_INVALID implies Invalid

// 
// Wrapper class to encapsulate the handling of Color Codes
// (eg. COLOR_WINDOW etc.).
//
// 
class ColorHandler
{
    friend HRESULT SHCHSetColorFromAttribute(LPVOID, LPVOID, const TCHAR *);
    friend HRESULT SHCHSetColorFromString(LPVOID, const TCHAR *);
    friend HRESULT SHCHGetCOLORREF(LPVOID, COLORREF *);

    DWORD m_dwColor;

public:
    BOOL IsValid() const
        {
            return (m_dwColor != CLR_INVALID);
        };

    BOOL IsTransparent() const
        {
            return ((m_dwColor & COLORMASK) == COLORTRANSPARENT);
        };   
    
    void Reset()
        {
            m_dwColor = CLR_INVALID;
        };

    void ResetToTransparent()
        {
            m_dwColor = COLORTRANSPARENT;
        };
    
    HRESULT SetColorFromColorHandler(const ColorHandler& rclrColorIn)
        {
            m_dwColor = rclrColorIn.m_dwColor;
            return (S_OK);
        };

    HRESULT SetColorFromCOLORREF(const COLORREF crColorIn)
        {
            m_dwColor = crColorIn;
            return (S_OK);
        };    

    HRESULT SetColorFromAttribute(IXMLDOMNode* pnode,
                                  const TCHAR* pszAttributeName)
        {
            return SHCHSetColorFromAttribute((LPVOID) this,
                                          (LPVOID) pnode, pszAttributeName);
        };

    HRESULT SetColorFromString(const TCHAR* pszColorIn)
        {
            return SHCHSetColorFromString((LPVOID) this, pszColorIn);
        };
 
    HRESULT GetCOLORREF(COLORREF* pcrReturn) const
        {
            return SHCHGetCOLORREF((LPVOID) this, pcrReturn);
        };
};

#endif
