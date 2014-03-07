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
/*****************************************************************************

    File: BkSpec.h
    Purpose: A background drawing helper.
    Author(s): KKennedy, t-HTsai

    History:
        Created from sysplug.cpp - KKennedy 1-21-01

*/

#pragma once

#include "msxml.h"
#include "ImgCache.h"
#include "colorhandler.h"

/*****************************************************************************

 *****************************************************************************/
const DWORD BKSF_BITMAP_HMASK       = 0x00000003; // mask for bits affecting the horizontal placement of the bitmap
const DWORD BKSF_BITMAP_HSHIFT      = 0;
const DWORD BKSF_BITMAP_LEFT        = 0x00000000;
const DWORD BKSF_BITMAP_HCENTER     = 0x00000001;
const DWORD BKSF_BITMAP_RIGHT       = 0x00000002;
const DWORD BKSF_BITMAP_HTILE       = 0x00000003;

const DWORD BKSF_BITMAP_VMASK       = 0x0000000C; // mask for bits affecting the vertical placement of the bitmap
const DWORD BKSF_BITMAP_VSHIFT      = 2; // After masking the bits out, you can shift them right by this constant to make them equivalent to the horizontal alignment values.
const DWORD BKSF_BITMAP_TOP         = 0x00000000;
const DWORD BKSF_BITMAP_VCENTER     = 0x00000004; // only valid with BKSF_BITMAP_NOSCROLL
const DWORD BKSF_BITMAP_BOTTOM      = 0x00000008; // only valid with BKSF_BITMAP_NOSCROLL
const DWORD BKSF_BITMAP_VTILE       = 0x0000000C;

const DWORD BKSF_BITMAP_SCROLLMASK  = 0x00000010;
const DWORD BKSF_BITMAP_SCROLL      = 0x00000000; // background scrolls with contents
const DWORD BKSF_BITMAP_NOSCROLL    = 0x00000010; // background does not scroll with window contents.  This may require more participation from client.

const DWORD BKSF_GRADIENT_MASK              = 0x00000060; // mask for the gradient type.  Gradients will be ignored if a bitmap is specified.  Gradients will override the bitmap alignment flags.
const DWORD BKSF_GRADIENT_NONE              = 0x00000000; // no gradient
const DWORD BKSF_GRADIENT_APP_BACKGROUND    = 0x00000020; // Default app background (like in the dialer, call progress, etc.)
const DWORD BKSF_GRADIENT_TITLE_BACKGROUND  = 0x00000040; // app title background.  Also presently (3/7/1) used in the icon bar plugin.

const DWORD BKSF_MUST_DRAW_MASK     = 0x00000880;
const DWORD BKSF_MUST_DRAW          = 0x00000080; // Set this if the background spec must always draw something.  The background spec for a virtual list view must always draw something.  The background spec for a plugin doesn't.
const DWORD BKSF_MUST_DRAW_HIGHLIGHT = 0x00000800; // Set this to always draw the highlight color, even if a BITMAP is set

const DWORD BKSF_STRETCH_MASK           = 0x00000300;
const DWORD BKSF_STRETCH_SHRINKTOFIT    = 0x00000100; // Set if background bitmap is to be shrunk to fit screen. Will not enlarge bitmaps.
const DWORD BKSF_STRETCH_ENLARGETOFIT   = 0x00000200; // Set if background bitmap is to be enlarge to fit screen. Will not shrink bitmaps.

const DWORD BKSF_FDRM_MASK              = 0x00007000;
const DWORD BKSF_FDRM_VALIDATERIGHTS    = 0x00001000; // Set if we need to FDRM validate rights to the background image on each paint
const DWORD BKSF_FDRM_COMMITPENDING     = 0x00002000; // Set if we need to do a CommitRights when the image becomes available (used for pending rights)
const DWORD BKSF_FDRM_RIGHTSVIOLATION   = 0x00004000; // The image currently has a rights violation

const DWORD BKSF_ALL_FLAGS = BKSF_BITMAP_HMASK | BKSF_BITMAP_VMASK | BKSF_BITMAP_SCROLLMASK | BKSF_GRADIENT_MASK | BKSF_MUST_DRAW_MASK | BKSF_STRETCH_MASK | BKSF_FDRM_MASK;

// bits for BKSSETTINGS::dwValid
const DWORD BKSV_STATE          = 0x00000001; // bstate is valid
const DWORD BKSV_FLAGS          = 0x00000002; // dwFlags* is valid
const DWORD BKSV_CX             = 0x00000004; // cxBackground is valid
const DWORD BKSV_FILENAME       = 0x00000008; // pszBitmapFileName is valid
const DWORD BKSV_TRANS_COLOR    = 0x00000010; // crBitmapTransparency is valid
const DWORD BKSV_BACK_COLOR     = 0x00000020; // crBackground is valid
const DWORD BKSV_BORDER_INFO    = 0x00000040; // border widths and colors are valid
const DWORD BKSV_TARGET         = 0x00000080; // target resolution

enum BKSSTATE
{
    BKSNormal,
    BKSSelected,

    BKSEnd, // invalid, also the count of possible states.
};

struct BKSSETTINGS // clients should memset this structure to 0 before using.
{
    DWORD dwValid; // valid fields in this structure BKSV_*

    BKSSTATE bstate; // state these settings apply to
    
    DWORD dwFlagsMask; // bits in dwFlags that are valid
    DWORD dwFlags; // BKSF_ flags

    int cxBkTarget; // Target background width
    int cyBkTarget; // Target background height
    
    int cxBackground; // Width of the background.  Needed for gradients.

    const TCHAR* pszBitmapFileName; // full path of bitmap file.  Set this to NULL if you don't want a bitmap.
    ColorHandler chBitmapTransparency; // Transparent color of the bitmap.  Reset it if there is no transparent color. (transparent colors are slow...)

    ColorHandler chBackground; // if there is no gradient or bitmap or the bitmap doesn't cover everything, use this color to fill stuff in.  Reset it to not set this.

    int iBorderWidthLeft;
    int iBorderWidthRight;
    int iBorderWidthTop;
    int iBorderWidthBottom;
    ColorHandler clrBorderColorLeft;
    ColorHandler clrBorderColorRight;
    ColorHandler clrBorderColorTop;
    ColorHandler clrBorderColorBottom;
};

class IBackgroundSpec :
    public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize() = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBaseDirectory(const TCHAR* pszDir) = 0;
    virtual HRESULT STDMETHODCALLTYPE UpdateIndirect(const BKSSETTINGS* pbss) = 0;
    virtual HRESULT STDMETHODCALLTYPE UpdateFromXML(IXMLDOMNode* pnodeDefault, IXMLDOMNode* pnode) = 0;
    virtual HRESULT STDMETHODCALLTYPE InitializeFromSAN(const TCHAR* pszAppSAN, int cxScreen) = 0;
    virtual HRESULT STDMETHODCALLTYPE Paint(HDC hdc, const RECT* prcBounds, const RECT* prcDraw, BKSSTATE bstate) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHeight(int* piheight) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetScrollFlags(DWORD* pdwFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetImageCache(IImageCache* pic) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnSysColorChange() = 0;

    // Not implemented on Retail builds (but function will still be here, it just won't do anything)
    virtual HRESULT STDMETHODCALLTYPE DebugDump(int iIndent) = 0;
};

HRESULT CreateBackgroundSpec(IBackgroundSpec** ppbs);

HRESULT STHStreamBackgroundSpec(DWORD dwFlags, IStream* pStm, void* pElement, LPARAM lParam, UINT* pcb);

