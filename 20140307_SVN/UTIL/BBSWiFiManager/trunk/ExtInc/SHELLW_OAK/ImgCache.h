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

    File: ImgCache.h
    Purpose: Interface to an ImageCache object.
    Author(s): KK

    History:
        Created - KK 3/9/1

*/



#ifndef __IMGCACHE_H__
#define __IMGCACHE_H__ 1


#ifdef __cplusplus
extern "C"{
#endif 


/*****************************************************************************

 *****************************************************************************/

typedef int ICHANDLE;

const ICHANDLE ICH_ALL = -1; // when you want to do an operation on all the images in the cache

enum IITYPE
    {
    IIT_INVALID,
    IIT_ICON,
    IIT_BITMAPFILE,
    IIT_BLANK
    };

struct IIICON
    {
    HINSTANCE hinst;
    UINT id;
    };

struct IIFILE
    {
    const TCHAR* pszFileName;
    BOOL fUseTransparency;
    COLORREF crTransparency;
    BOOL fShrinkToFit;
    };

struct IIBLANK
    {
    int dx;
    int dy;
    };

struct ICIMAGEINFO
    {
    IITYPE type;
    DWORD dwFlags;

    union
        {
        IIICON icon;
        IIFILE file;
        IIBLANK blank;
        };
    };

// flags for IICSTATS dwFlags member
const DWORD IICSF_DEBUG_DUMP    = 0x00000001; // Output a debug report of the requested object to the debugger
// const DWORD IICSF_    = 0x00000000;

struct IICSTATS
    {
    DWORD dwFlags; // IICSF_* flags
    ICHANDLE ih; // handle of the image you want to get stats for.  Use ICH_ALL to get the total for the cache.

    int cObjects; // number of icons/bitmaps
    int cBytes; // number of bytes used (may be approximate;
    };


// flags for DrawImage (currently, they are the same as the DCI_* flags in private\apps\tele\inc\icocache.h )
const DWORD ICDI_LEFT    = 0x00000001;
const DWORD ICDI_RIGHT   = 0x00000002;
const DWORD ICDI_HCENTER = 0x00000004;
const DWORD ICDI_TOP     = 0x00000008;
const DWORD ICDI_BOTTOM  = 0x00000010;
const DWORD ICDI_VCENTER = 0x00000020;

// Flags for Get/SetImageInfo
const DWORD ICGIF_HANDLE    = 0x00000001;
const DWORD ICGIF_HICON     = 0x00000002;
const DWORD ICGIF_HBITMAP   = 0x00000004;

// Flags for Get/SetImageFlags
const DWORD ICIIF_VALIDATERIGHTS   = 0x00000001;
const DWORD ICIIF_COMMITPENDING    = 0x00000002;
const DWORD ICIIF_RIGHTSVIOLATION  = 0x00000004;

EXTERN_C const IID IID_IImageCache;
interface DECLSPEC_UUID("DBB665B5-22B0-425d-8D18-AFE5AAF9FA93") IImageCache :
        public IUnknown
    {
public:
    virtual HRESULT STDMETHODCALLTYPE GetImage(const ICIMAGEINFO* pii, ICHANDLE* pih) = 0;
    virtual HRESULT STDMETHODCALLTYPE PaintImage(HDC hdc, const RECT* prc, ICHANDLE ih, DWORD dwFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE DeleteImage(ICHANDLE ih) = 0;
    virtual HRESULT STDMETHODCALLTYPE ClearImageBits(ICHANDLE ih) = 0;

    // members that use the cache more like a storage
    virtual HRESULT STDMETHODCALLTYPE GetImageHBITMAP(ICHANDLE ih, HBITMAP* phbmp) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetImageHICON(ICHANDLE ih, HICON* phbmp) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetImageInfo(DWORD dwFlags, void* id, ICIMAGEINFO** ppii) = 0;

    virtual HRESULT STDMETHODCALLTYPE AddImageHICON(UINT idIcon, HICON hIcon) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetImageFlags(ICHANDLE ih, DWORD * pdwFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetImageFlags(ICHANDLE ih, DWORD dwFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE VerifyImageRights(ICHANDLE ih, BOOL * pfRightsChanged) = 0;

    // This is only implemented on debug builds.
    virtual HRESULT STDMETHODCALLTYPE GetStats(IICSTATS* pStats) = 0;
    };

HRESULT CreateImageCache(IImageCache** ppic);

HRESULT DrawAlignedIcon(HDC hdc, const RECT* prc, HICON hicon, DWORD dwFlags);

#ifdef __cplusplus
}
#endif

// {DBB665B5-22B0-425d-8D18-AFE5AAF9FA93}
DEFINE_GUID(IID_IImageCache,
    0xDBB665B5, 0x22B0, 0x425d, 0x8D, 0x18, 0xAF, 0xE5, 0xAA, 0xF9, 0xFA, 0x93);

#endif // __IMGCACHE_H__


