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
//
//
// Bitmap helper API, use an alternate resource and dll if available
//

// LoadBitmapOver
// 
// Load a bitmap, or its override. 
// 
// Parameters: 
// HINSTANCE hDllStandard [in] HANDLE to the DLL from which the standard ID can
//                             be loaded. 
// UINT idBmpStandard     [in] ID for the standard resource, to be loaded from the
//                             standard dll. 
// UINT idBmpOverride     [in] ID for the resource which will override the
//                             standard resource according to registry configuration. 
// 
// If hDllStandard is not NULL, idBmpStandard must be non-zero. 
// If idBmpStandard is non-zero, hDllStandard must be non-NULL; 
// If idBmpOverride is non-zero, it will take precedence over idBmpStandard. 
// If a non-zero idBmpOverride is supplied, and can't be loaded due to system
// configuration, use of idBmpStandard and hStandardDll, if supplied will be
// attempted.  
// 
// Registry settings and supllementary DLL 
// 
//     The name of the DLL from which overriding bitmaps will be
//     loaded is read from the following registry location - 
//     [HKEY_LOCAL_MACHINE\Security\ResOver\Bitmaps]
//         "BaseDll"="tshres.dll"
//     
//     An optional base value which will be added to each overriding
//     resource IDs will be read from this location -
//     [HKEY_LOCAL_MACHINE\Security\ResOver\Bitmaps]
//         "BaseId"=dword:10
//     
//     The above example sets the base to 10. If not found, it defaults to zero.
//     


// Resource IDs we currently allow for overriding bitmaps.
#include "OverBmp.h"

#ifdef __cplusplus
extern "C"{
#endif 

HBITMAP LoadBitmapOver(HINSTANCE hDllStandard, UINT idBmpStandard, UINT idBmpOverride);

__inline HBITMAP _dpi_LoadBitmapOver(HINSTANCE hDllStandard, UINT idBmpStandard, UINT idBmpOverride)
{
    HBITMAP hbm = LoadBitmapOver(hDllStandard, idBmpStandard, idBmpOverride);
    // If you hit this ASSERT, that means the bitmap is not in satellite DLL.
    ASSERT(hbm);
    return hbm;
}

// DPI-satellite Instance-loader helper
// If no override bitmap is present, take the bitmap from the DPI-specific satellite DLL
#define DPI_LoadBitmapOver(hDllStandard, idBmpStandard, idBmpOverride) _dpi_LoadBitmapOver(hDllStandard ## DPI, idBmpStandard, idBmpOverride)


// LoadIconOver
//
// Load an icon or its override
//
// This procedure has the same semantics as LoadBitmapOver however it loads and returns
// icons instead of bitmaps.

HICON LoadIconOver(HINSTANCE hDllStandard, UINT idIconStandard, UINT idIconOverride);

#ifdef __cplusplus
}
#endif 

__inline HICON _dpi_LoadIconOver(HINSTANCE hDllStandard, UINT idIconStandard, UINT idIconOverride)
{
    HICON hicon = LoadIconOver(hDllStandard, idIconStandard, idIconOverride);
    // If you hit this ASSERT, that means either override icon doesn't exist or icon is not in satellite DLL.
    ASSERT(hicon);
    return hicon;
}

#define DPI_LoadIconOver(hDllStandard, idIconStandard, idIconOverride) _dpi_LoadIconOver(hDllStandard ## DPI, idIconStandard, idIconOverride)

