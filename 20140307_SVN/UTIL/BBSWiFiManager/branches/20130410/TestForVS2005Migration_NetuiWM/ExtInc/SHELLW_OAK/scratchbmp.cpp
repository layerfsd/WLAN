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
//***   scratchbmp.cpp -- usef for offscreen drawing to prevent flicker
//

#include "ehm.h"
#include "scratchbmp.h"


/****************************************************************************


    Author(s): KKennedy
 ****************************************************************************/
ScratchBitmap::ScratchBitmap() :
    m_hdc(NULL)
    ,m_hbmpOld(NULL)
    ,m_hbmpScratch(NULL)
{
    Reset();
}

/****************************************************************************

    Author(s): KKennedy
 ****************************************************************************/
ScratchBitmap::~ScratchBitmap()
{
    Reset();
}

/****************************************************************************
    Clears out the bitmap and buffer

    Author(s): KKennedy
 ****************************************************************************/
void ScratchBitmap::Reset()
{
    if(m_hbmpOld)
        {
        VWR(SelectObject(m_hdc, m_hbmpOld));
        }
    if(m_hbmpScratch)
        {
        VWR(DeleteObject(m_hbmpScratch));
        }
    if(m_hdc)
        {
        VWR(DeleteDC(m_hdc));
        }

    m_hdc = NULL;
    m_hbmpScratch = NULL;
    m_hbmpOld = NULL;
    m_dx = 0;
    m_dy = 0;
}

/****************************************************************************
    Returns a DC that can be used for offscreen drawing.

    hdc - in - DC that you will end up drawing to
    prc - in - rectangle in hdc you want to buffer drawing to
    phdcScratch - out - DC you actually draw to.  If we can't actually
            allocate a DC and bitmap for the caller, we pass hdc back out
            in this.  The only difference when we fail should be that you
            get flicker when drawing.

    Author(s): KKennedy
 ****************************************************************************/
HRESULT ScratchBitmap::GetScratchDC(HDC hdc, const RECT* prc, HDC* phdcScratch)
{
    HRESULT hr = S_OK;
    int dx = RECTWIDTH(*prc);
    int dy = RECTHEIGHT(*prc);

    CBRA(hdc && prc && phdcScratch);
    
    if((dx > m_dx) || (dy > m_dy))
        {
        int dxOld = m_dx;
        int dyOld = m_dy;
        Reset();

        DEBUGMSG(TRUE, (TEXT("ScratchBitmap::GetScratchDC - creating new bitmap (%d, %d)\r\n"),
                max(dx, dxOld), max(dy, dyOld)));

        m_hdc = CreateCompatibleDC(hdc);
        CWR(m_hdc);
        m_hbmpScratch = CreateCompatibleBitmap(hdc, max(dx, dxOld), max(dy, dyOld));
        CWR(m_hbmpScratch);
        m_hbmpOld = (HBITMAP)SelectObject(m_hdc, m_hbmpScratch);
        CWRA(m_hbmpOld);
        m_dx = max(dx, dxOld);
        m_dy = max(dy, dyOld);
        }

    CWRA(SetViewportOrgEx(m_hdc, -prc->left, -prc->top, NULL));

    *phdcScratch = m_hdc;

Exit:
    return(hr);

Error:
    Reset();
    if(phdcScratch)
        {
        *phdcScratch = hdc;
        }
    goto Exit;
}

/****************************************************************************
    Puts whatever is in the scratch DC into the destination DC.

    hdc - in - DC that you will end up drawing to
    prc - in - rectangle in hdc you want to buffer drawing to

    Author(s): KKennedy
 ****************************************************************************/
HRESULT ScratchBitmap::Blt(HDC hdc, const RECT* prc) const
{
    HRESULT hr = S_OK;
    
    if(m_hdc) // ifCreateScratchDC failed, our DC will be null but what needed to be drawn has already been drawn to the destination so we are done
        {
        ASSERT(hdc != m_hdc);
        
        CWR(BitBlt(hdc, prc->left, prc->top, RECTWIDTH(*prc), RECTHEIGHT(*prc),
                m_hdc, prc->left, prc->top, SRCCOPY));
        }

Error:
    return(hr);
}
