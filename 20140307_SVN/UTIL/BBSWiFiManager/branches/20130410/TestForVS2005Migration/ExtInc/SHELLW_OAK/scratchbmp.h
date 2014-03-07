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
// Copyright (c) 2000, Microsoft Corp. All rights reserved.
//***   scratchbmp.h - helper class that caches an offscreen bitmap
//

#ifndef _SCRATCHBMP_H_
#define _SCRATCHBMP_H_ 1


class ScratchBitmap
{
    HDC m_hdc;
    HBITMAP m_hbmpOld;
    HBITMAP m_hbmpScratch;
    int m_dx;
    int m_dy;

public:
    ScratchBitmap();
    ~ScratchBitmap();

    void Reset();

    HRESULT GetScratchDC(HDC hdc, const RECT* prc, HDC* phdcScratch);
    HRESULT Blt(HDC hdc, const RECT* prc) const;
};

#endif // _SCRATCHBMP_H_

