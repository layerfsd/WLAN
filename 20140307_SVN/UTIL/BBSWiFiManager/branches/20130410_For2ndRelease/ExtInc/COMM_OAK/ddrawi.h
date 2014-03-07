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
/*==========================================================================;
 *
 *  File:       ddrawi.h
 *  Content:    DirectDraw DDK include file
 *
 ***************************************************************************/
#pragma once

#include <windows.h>
#include <ddraw.h>

// These GUIDs identify extended callback arrays available from the driver's
// GetDriverInfo callback, not interfaces.

DEFINE_GUID(GUID_MiscellaneousCallbacks,    0xefd60cc0,0x49e7,0x11d0,0x88,0x9d,0x00,0xaa,0x00,0xbb,0xb7,0x6a);
DEFINE_GUID(GUID_VideoPortCallbacks,        0xefd60cc1,0x49e7,0x11d0,0x88,0x9d,0x00,0xaa,0x00,0xbb,0xb7,0x6a);
DEFINE_GUID(GUID_ColorControlCallbacks,     0xefd60cc2,0x49e7,0x11d0,0x88,0x9d,0x00,0xaa,0x00,0xbb,0xb7,0x6a);

// The maximum lengths of the device name and description strings.

#define DD_DEVICE_ENUM_NAME_LENGTH 256
#define DD_DEVICE_ENUM_DESC_LENGTH 256

// Forward internal structure declerations.

typedef struct _DDRAWI_DIRECTDRAW_GBL        * LPDDRAWI_DIRECTDRAW_GBL;
typedef struct _DDRAWI_DDRAWSURFACE_LCL      * LPDDRAWI_DDRAWSURFACE_LCL;
typedef struct _DDRAWI_DDRAWPALETTE_GBL      * LPDDRAWI_DDRAWPALETTE_GBL;
typedef struct _DDRAWI_DDRAWCLIPPER_GBL      * LPDDRAWI_DDRAWCLIPPER_GBL;
typedef struct _DDRAWI_DDRAWCOLORCONTROL_LCL * LPDDRAWI_DDRAWCOLORCONTROL_LCL;
typedef struct _DDRAWI_DDRAWGAMMACONTROL_LCL * LPDDRAWI_DDRAWGAMMACONTROL_LCL;

// DirectDraw HAL entry point possible return values.

#define DDHAL_DRIVER_NOTHANDLED 0x00000000
#define DDHAL_DRIVER_HANDLED    0x00000001

// These structures are the parameter blocks for the various driver entry
// points used by DirectDraw.

// Structure for passing information to DDHAL Lock function.

typedef struct _DDHAL_LOCKDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSurface;
    BOOL                        bHasRect;
    RECT                        rArea;
    DWORD                       dwFlags;

    // Output parameters.

    LPVOID                      lpSurfData;
    HRESULT                     ddRVal;

} DDHAL_LOCKDATA, * LPDDHAL_LOCKDATA;

// Structure for passing information to DDHAL Unlock function.

typedef struct _DDHAL_UNLOCKDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSurface;

    // Output parameters.

    HRESULT                     ddRVal;

} DDHAL_UNLOCKDATA, * LPDDHAL_UNLOCKDATA;

// Structure for passing information to DDHAL UpdateOverlay function.

typedef struct _DDHAL_UPDATEOVERLAYDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDDestSurface;
    RECT                        rDest;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSrcSurface;
    RECT                        rSrc;
    DWORD                       dwFlags;
    DDOVERLAYFX                 overlayFX;

    // Output parameters.

    HRESULT                     ddRVal;

} DDHAL_UPDATEOVERLAYDATA, * LPDDHAL_UPDATEOVERLAYDATA;

// Structure for passing information to DDHAL SetOverlayPosition function.

typedef struct _DDHAL_SETOVERLAYPOSITIONDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSrcSurface;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDDestSurface;
    LONG                        lXPos;
    LONG                        lYPos;

    // Output parameters.

    HRESULT                     ddRVal;

} DDHAL_SETOVERLAYPOSITIONDATA, * LPDDHAL_SETOVERLAYPOSITIONDATA;

// Structure for passing information to DDHAL SetPalette function.

typedef struct _DDHAL_SETPALETTEDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSurface;
    LPDDRAWI_DDRAWPALETTE_GBL   lpDDPalette;
    BOOL                        Attach;

    // Output parameters.

    HRESULT                     ddRVal;
    
} DDHAL_SETPALETTEDATA, * LPDDHAL_SETPALETTEDATA;

// Structure for passing information to DDHAL Flip function.

typedef struct _DDHAL_FLIPDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpSurfCurr;
    LPDDRAWI_DDRAWSURFACE_LCL   lpSurfTarg;
    DWORD                       dwFlags;

    // Output parameters.

    HRESULT                     ddRVal;

} DDHAL_FLIPDATA, * LPDDHAL_FLIPDATA;

// Structure for passing information to DDHAL DestroySurface function.

typedef struct _DDHAL_DESTROYSURFACEDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSurface;

    // Output parameters.

    HRESULT                     ddRVal;
    
} DDHAL_DESTROYSURFACEDATA, * LPDDHAL_DESTROYSURFACEDATA;

// Structure for passing information to DDHAL SetColorKey function.

typedef struct _DDHAL_SETCOLORKEYDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSurface;
    DWORD                       dwFlags;
    DDCOLORKEY                  ckNew;

    // Output parameters.

    HRESULT                     ddRVal;

} DDHAL_SETCOLORKEYDATA, * LPDDHAL_SETCOLORKEYDATA;

// Structure for passing information to DDHAL GetBltStatus function.

typedef struct _DDHAL_GETBLTSTATUSDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSurface;
    DWORD                       dwFlags;

    // Output parameters.

    HRESULT                     ddRVal;
    
} DDHAL_GETBLTSTATUSDATA, * LPDDHAL_GETBLTSTATUSDATA;

// Structure for passing information to DDHAL GetFlipStatus function.

typedef struct _DDHAL_GETFLIPSTATUSDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL   lpDDSurface;
    DWORD                       dwFlags;

    // Output parameters.

    HRESULT                     ddRVal;
    
} DDHAL_GETFLIPSTATUSDATA, * LPDDHAL_GETFLIPSTATUSDATA;

// Structure for passing information to DDHAL DestroyPalette function.

typedef struct _DDHAL_DESTROYPALETTEDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWPALETTE_GBL   lpDDPalette;

    // Output parameters.

    HRESULT                     ddRVal;
    
} DDHAL_DESTROYPALETTEDATA, * LPDDHAL_DESTROYPALETTEDATA;

// Structure for passing information to DDHAL SetEntries function.

typedef struct _DDHAL_SETENTRIESDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDRAWI_DDRAWPALETTE_GBL   lpDDPalette;
    DWORD                       dwBase;
    DWORD                       dwNumEntries;
    LPPALETTEENTRY              lpEntries;

    // Output parameters.

    HRESULT                     ddRVal;
    
} DDHAL_SETENTRIESDATA, * LPDDHAL_SETENTRIESDATA;

// Structure for passing information to DDHAL CanCreateSurface function.

typedef struct _DDHAL_CANCREATESURFACEDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDSURFACEDESC             lpDDSurfaceDesc;
    DWORD                       bIsDifferentPixelFormat;

    // Output parameters.

    HRESULT			            ddRVal;

} DDHAL_CANCREATESURFACEDATA, * LPDDHAL_CANCREATESURFACEDATA;

// Structure for passing information to DDHAL CreateSurface function.

typedef struct _DDHAL_CREATESURFACEDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPDDSURFACEDESC             lpDDSurfaceDesc;
    DWORD                       dwSCnt;

    // Output parameters.

    LPDDRAWI_DDRAWSURFACE_LCL * lplpSList;
    HRESULT                     ddRVal;
    
} DDHAL_CREATESURFACEDATA, * LPDDHAL_CREATESURFACEDATA;

// Structure for passing information to DDHAL CreatePalette function.

typedef struct _DDHAL_CREATEPALETTEDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    LPPALETTEENTRY              lpColorTable;

    // Output parameters.

    LPDDRAWI_DDRAWPALETTE_GBL   lpDDPalette;
    HRESULT                     ddRVal;

} DDHAL_CREATEPALETTEDATA, * LPDDHAL_CREATEPALETTEDATA;

// Return if the vertical blank is in progress.

#define DDWAITVB_I_TESTVB           0x80000006

// Structure for passing information to DDHAL WaitForVerticalBlank function.

typedef struct _DDHAL_WAITFORVERTICALBLANKDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    DWORD                       dwFlags;

    // Output parameters.

    DWORD                       bIsInVB;
    HRESULT                     ddRVal;

} DDHAL_WAITFORVERTICALBLANKDATA, * LPDDHAL_WAITFORVERTICALBLANKDATA;

// Structure for passing information to DDHAL GetScanLine function.

typedef struct _DDHAL_GETSCANLINEDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL	    lpDD;

    // Output parameters.

    DWORD                       dwScanLine;
    HRESULT                     ddRVal;
    
} DDHAL_GETSCANLINEDATA, * LPDDHAL_GETSCANLINEDATA;

// Structure for passing information to the DDHAL GetAvailDriverMemory
// function.

typedef struct _DDHAL_GETAVAILDRIVERMEMORYDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    DDSCAPS                     DDSCaps;

    // Output parameters.

    DWORD                       dwTotal;
    DWORD                       dwFree;
    HRESULT                     ddRVal;

} DDHAL_GETAVAILDRIVERMEMORYDATA, * LPDDHAL_GETAVAILDRIVERMEMORYDATA;

// Structure for passing information to the DDHAL GetDeviceIdentifier 
// function.

typedef struct _DDHAL_GETDEVICEIDENTIFIERDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;

    // Output parameters.

    LPDDDEVICEIDENTIFIER        lpDeviceId;
    HRESULT                     ddRVal;

} DDHAL_GETDEVICEIDENTIFIERDATA, * LPDDHAL_GETDEVICEIDENTIFIERDATA;

// Structure for passing information to DDHAL ColorControl function
 
typedef struct _DDHAL_COLORCONTROLDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL	  lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL lpDDSurface;
    LPDDCOLORCONTROL          lpColorData;
    DWORD                     dwFlags;

    // Output parameters.

    HRESULT                   ddRVal;

} DDHAL_COLORCONTROLDATA, * LPDDHAL_COLORCONTROLDATA;

#define DDCOLORC_I_GETCOLOR 0x00000001
#define DDCOLORC_I_SETCOLOR 0x00000002

// Structure for passing information to DDHAL GammaControl function
 
typedef struct _DDHAL_GAMMACONTROLDATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL	  lpDD;
    LPDDRAWI_DDRAWSURFACE_LCL lpDDSurface;
    LPDDGAMMARAMP             lpGammaData;
    DWORD                     dwFlags;

    // Output parameters.

    HRESULT                   ddRVal;

} DDHAL_GAMMACONTROLDATA, * LPDDHAL_GAMMACONTROLDATA;

#define DDGAMMAC_I_GETGAMMA 0x00000001
#define DDGAMMAC_I_SETGAMMA 0x00000002

// Structure for passing information to the DDHAL GetDriverInfo function.

typedef struct _DDHAL_GETDRIVERINFODATA {

    // Input parameters.

    LPDDRAWI_DIRECTDRAW_GBL     lpDD;
    DWORD                       dwSize;
    DWORD                       dwFlags;
    GUID                        guidInfo;
    DWORD                       dwExpectedSize;
    LPVOID                      lpvData;

    // Output parameters.

    DWORD                       dwActualSize;
    HRESULT                     ddRVal;

} DDHAL_GETDRIVERINFODATA, * LPDDHAL_GETDRIVERINFODATA;

// DirectDraw Object Driver Calls. The driver is expected to return
// one of these structures with valid function pointers to it's
// internal functions as part of HALInit.

typedef DWORD (* LPDDHAL_CANCREATESURFACE)(LPDDHAL_CANCREATESURFACEDATA );
typedef DWORD (* LPDDHAL_CREATESURFACE)(LPDDHAL_CREATESURFACEDATA);
typedef DWORD (* LPDDHAL_CREATEPALETTE)(LPDDHAL_CREATEPALETTEDATA);
typedef DWORD (* LPDDHAL_WAITFORVERTICALBLANK)(LPDDHAL_WAITFORVERTICALBLANKDATA );
typedef DWORD (* LPDDHAL_GETSCANLINE)(LPDDHAL_GETSCANLINEDATA);

typedef struct _DDHAL_DDCALLBACKS {

    DWORD                           dwSize;
    DWORD                           dwFlags;
    LPDDHAL_CREATESURFACE           CreateSurface;
    LPDDHAL_WAITFORVERTICALBLANK    WaitForVerticalBlank;
    LPDDHAL_CANCREATESURFACE        CanCreateSurface;
    LPDDHAL_CREATEPALETTE           CreatePalette;
    LPDDHAL_GETSCANLINE             GetScanLine;
    
} DDHAL_DDCALLBACKS, * LPDDHAL_DDCALLBACKS;

#define DDCALLBACKSSIZE sizeof(DDHAL_DDCALLBACKS)

#define DDHAL_CB32_CREATESURFACE        0x00000001
#define DDHAL_CB32_WAITFORVERTICALBLANK 0x00000002
#define DDHAL_CB32_CANCREATESURFACE     0x00000004
#define DDHAL_CB32_CREATEPALETTE        0x00000008
#define DDHAL_CB32_GETSCANLINE          0x00000010

// DirectDraw Surface Object Driver Calls. The driver must return one of
// these in HALInit.

typedef DWORD (* LPDDHALSURFCB_LOCK)(LPDDHAL_LOCKDATA);
typedef DWORD (* LPDDHALSURFCB_UNLOCK)(LPDDHAL_UNLOCKDATA);
typedef DWORD (* LPDDHALSURFCB_UPDATEOVERLAY)(LPDDHAL_UPDATEOVERLAYDATA);
typedef DWORD (* LPDDHALSURFCB_SETOVERLAYPOSITION)(LPDDHAL_SETOVERLAYPOSITIONDATA);
typedef DWORD (* LPDDHALSURFCB_SETPALETTE)(LPDDHAL_SETPALETTEDATA);
typedef DWORD (* LPDDHALSURFCB_FLIP)(LPDDHAL_FLIPDATA);
typedef DWORD (* LPDDHALSURFCB_DESTROYSURFACE)(LPDDHAL_DESTROYSURFACEDATA);
typedef DWORD (* LPDDHALSURFCB_SETCOLORKEY)(LPDDHAL_SETCOLORKEYDATA);
typedef DWORD (* LPDDHALSURFCB_GETBLTSTATUS)(LPDDHAL_GETBLTSTATUSDATA);
typedef DWORD (* LPDDHALSURFCB_GETFLIPSTATUS)(LPDDHAL_GETFLIPSTATUSDATA);

typedef struct _DDHAL_DDSURFACECALLBACKS {

    DWORD                               dwSize;
    DWORD                               dwFlags;
    LPDDHALSURFCB_DESTROYSURFACE        DestroySurface;
    LPDDHALSURFCB_FLIP                  Flip;
    LPDDHALSURFCB_LOCK                  Lock;
    LPDDHALSURFCB_UNLOCK                Unlock;
    LPDDHALSURFCB_SETCOLORKEY           SetColorKey;
    LPDDHALSURFCB_GETBLTSTATUS          GetBltStatus;
    LPDDHALSURFCB_GETFLIPSTATUS         GetFlipStatus;
    LPDDHALSURFCB_UPDATEOVERLAY         UpdateOverlay;
    LPDDHALSURFCB_SETOVERLAYPOSITION    SetOverlayPosition;
    LPDDHALSURFCB_SETPALETTE            SetPalette;

} DDHAL_DDSURFACECALLBACKS, * LPDDHAL_DDSURFACECALLBACKS;

#define DDSURFACECALLBACKSSIZE sizeof(DDHAL_DDSURFACECALLBACKS)

#define DDHAL_SURFCB32_DESTROYSURFACE       0x00000001
#define DDHAL_SURFCB32_FLIP                 0x00000002
#define DDHAL_SURFCB32_LOCK                 0x00000004
#define DDHAL_SURFCB32_UNLOCK               0x00000008
#define DDHAL_SURFCB32_SETCOLORKEY          0x00000010
#define DDHAL_SURFCB32_GETBLTSTATUS         0x00000020
#define DDHAL_SURFCB32_GETFLIPSTATUS        0x00000040
#define DDHAL_SURFCB32_UPDATEOVERLAY        0x00000080
#define DDHAL_SURFCB32_SETOVERLAYPOSITION   0x00000100
#define DDHAL_SURFCB32_SETPALETTE           0x00000200

// DirectDraw Surface Object Driver Calls. The driver must return one of
// these in HALInit.

typedef DWORD (* LPDDHALPALCB_DESTROYPALETTE)(LPDDHAL_DESTROYPALETTEDATA);
typedef DWORD (* LPDDHALPALCB_SETENTRIES)(LPDDHAL_SETENTRIESDATA);

typedef struct _DDHAL_DDPALETTECALLBACKS {

    DWORD                       dwSize;
    DWORD                       dwFlags;
    LPDDHALPALCB_DESTROYPALETTE DestroyPalette;
    LPDDHALPALCB_SETENTRIES     SetEntries;

} DDHAL_DDPALETTECALLBACKS, * LPDDHAL_DDPALETTECALLBACKS;

#define DDPALETTECALLBACKSSIZE sizeof(DDHAL_DDPALETTECALLBACKS)

#define DDHAL_PALCB32_DESTROYPALETTE    0x00000001
#define DDHAL_PALCB32_SETENTRIES        0x00000002

// The driver passes this pointer in the DDHALINFO structure it is used
// by the middleware to query additional data and function pointers
// from the driver.

typedef DWORD (* LPDDHAL_GETDRIVERINFO)(LPDDHAL_GETDRIVERINFODATA);

// Extended callback structures available from GetDriverInfo.

// Calling GetDriverInfo with GUID_MiscellaneousCallbacks returns the
// drivers miscellaneous calls.

typedef DWORD (* LPDDHAL_GETAVAILDRIVERMEMORY)(LPDDHAL_GETAVAILDRIVERMEMORYDATA);
typedef DWORD (* LPDDHAL_GETDEVICEIDENTIFIER)(LPDDHAL_GETDEVICEIDENTIFIERDATA);

typedef struct _DDHAL_DDMISCELLANEOUSCALLBACKS {

    DWORD                        dwSize;
    DWORD                        dwFlags;
    LPDDHAL_GETAVAILDRIVERMEMORY GetAvailDriverMemory;
    LPDDHAL_GETDEVICEIDENTIFIER  GetDeviceIdentifier;

} DDHAL_DDMISCELLANEOUSCALLBACKS, * LPDDHAL_DDMISCELLANEOUSCALLBACKS;

#define DDHAL_MISCCB32_GETAVAILDRIVERMEMORY 0x00000001
#define DDHAL_MISCCB32_GETDEVICEIDENTIFIER  0x00000002

// Calling GetDriverInfo with GUID_ColorControlCallbacks returns the driver's
// color and gamma control object calls.

typedef DWORD (FAR PASCAL *LPDDHALCOLORCB_COLORCONTROL)(LPDDHAL_COLORCONTROLDATA);
typedef DWORD (FAR PASCAL *LPDDHALCOLORCB_GAMMACONTROL)(LPDDHAL_GAMMACONTROLDATA);

typedef struct _DDHAL_DDCOLORCONTROLCALLBACKS
{
    DWORD                       dwSize;
    DWORD                       dwFlags;
    LPDDHALCOLORCB_COLORCONTROL ColorControl;
    LPDDHALCOLORCB_GAMMACONTROL GammaControl;

} DDHAL_DDCOLORCONTROLCALLBACKS, * LPDDHAL_DDCOLORCONTROLCALLBACKS;

#define DDHAL_COLORCB32_COLORCONTROL 0x00000001
#define DDHAL_COLORCB32_GAMMACONTROL 0x00000002

/*
 * Internal GDI objects referenced here forward decls.
 */

#ifdef __cplusplus
class Driver_t;
class BitmapBase_t;
class DC;
#else
typedef void Driver_t;
typedef void BitmapBase_t;
typedef void DC;
#endif

/*
 * A pointer to one of these structures is passed to the driver's HALinit
 * entry point. The driver is expected to fill it out as part of the
 * HALInit process.
 */

typedef struct _DDHALINFO {

    DWORD                       dwSize;
    DWORD                       dwFlags;

    LPDDHAL_DDCALLBACKS         lpDDCallbacks;
    LPDDHAL_DDSURFACECALLBACKS  lpDDSurfaceCallbacks;
    LPDDHAL_DDPALETTECALLBACKS  lpDDPaletteCallbacks;
    LPDDHAL_GETDRIVERINFO       GetDriverInfo;

    DDCAPS                      ddCaps;
    DDCAPS                      ddHelCaps;
    LPDWORD                     lpdwFourCC;

} DDHALINFO, * LPDDHALINFO;

/*
 * Internal DirectDraw data structures.
 */

typedef struct _DDRAWI_DIRECTDRAW_GBL {

    wchar_t                   DeviceName[DD_DEVICE_ENUM_NAME_LENGTH];
    DWORD                     ProcessFlags;
    Driver_t *                Device;

    DDHAL_DDCALLBACKS         DDCallbacks;
    DDHAL_DDSURFACECALLBACKS  DDSurfaceCallbacks;
    DDHAL_DDPALETTECALLBACKS  DDPaletteCallbacks;
    LPDDHAL_GETDRIVERINFO     GetDriverInfo;

    DDHAL_DDMISCELLANEOUSCALLBACKS DDMiscCallbacks;
    DDHAL_DDCOLORCONTROLCALLBACKS  DDColorCallbacks;

    DDCAPS                    ddCaps;
    DDCAPS                    ddHelCaps;
    LPDWORD                   lpdwFourCC;

    LPDDRAWI_DDRAWSURFACE_LCL      pSurfaceList;
    LPDDRAWI_DDRAWCOLORCONTROL_LCL pColorControlList;
    LPDDRAWI_DDRAWGAMMACONTROL_LCL pGammaControlList;

    LPDDRAWI_DDRAWSURFACE_LCL     *pOverlays;
    RECT                          *rcOverlayDest;

    DWORD                     dwGDISurfReserved1;

} DDRAWI_DIRECTDRAW_GBL;


typedef struct _DDRAWI_DDRAWCLIPPER_GBL {

    LPDDRAWI_DIRECTDRAW_GBL   pDD;
    DWORD                     ProcessOwner;

    HWND                      hWnd;

    DWORD                     dwClipperHandle;

} DDRAWI_DDRAWCLIPPER_GBL;

typedef struct _DDRAWI_DDRAWCOLORCONTROL_LCL {

    LPDDRAWI_DIRECTDRAW_GBL   pDD;
    DWORD                     ProcessOwner;

    LPDDRAWI_DDRAWCOLORCONTROL_LCL pNext;

    DWORD                     dwHandle;

    LPDDRAWI_DDRAWSURFACE_LCL pSurface;

    LPDDCOLORCONTROL          pOldColorData;

} DDRAWI_DDRAWCOLORCONTROL_LCL;

typedef struct _DDRAWI_DDRAWGAMMACONTROL_LCL {

    LPDDRAWI_DIRECTDRAW_GBL   pDD;
    DWORD                     ProcessOwner;

    LPDDRAWI_DDRAWGAMMACONTROL_LCL pNext;

    DWORD                     dwHandle;

    LPDDRAWI_DDRAWSURFACE_LCL pSurface;

    LPDDGAMMARAMP             pOldGammaData;

} DDRAWI_DDRAWGAMMACONTROL_LCL;

typedef struct _DDRAWI_DDRAWSURFACE_LCL {

    LPDDRAWI_DIRECTDRAW_GBL   pDD;
    DWORD                     ProcessOwner;

    DDSCAPS                   ddsCaps;
    BitmapBase_t *            Bitmap;
    DWORD                     dwReserved1;

    LPDDRAWI_DDRAWSURFACE_LCL pNext;
    LPDDRAWI_DDRAWSURFACE_LCL pAttached;
    
    DWORD                     dwSurfaceHandle;

    DWORD                     dwFlags;

    RECT                      LockedRect;

    DC *                      pDc;

    DDCOLORKEY                ddckCKDestBlt;
    DDCOLORKEY                ddckCKSrcBlt;

    // These fields are used by overlay surfaces.

    DDCOLORKEY                ddckCKSrcOverlay;
    DDCOLORKEY                ddckCKDestOverlay;

    LPDDRAWI_DDRAWSURFACE_LCL pSurfaceOverlaying;
    RECT                      rcOverlaySource;

    DWORD                     dwOverlayFlags;
    DDOVERLAYFX               ddOverlayFX;

    // The current color and gamma controls.

    LPDDRAWI_DDRAWCOLORCONTROL_LCL pColorControl;
    LPDDRAWI_DDRAWGAMMACONTROL_LCL pGammaControl;

    LPDDRAWI_DDRAWCLIPPER_GBL   pClipper;
    DWORD                       dwClientData;

} DDRAWI_DDRAWSURFACE_LCL;

/*
 * DDRAWI_DDRAWSURFACE_LCL dwFlags values.
 */

#define DDRAWISURF_DDCREATED            0x00000001
#define DDRAWISURF_LOCKED               0x00000002
#define DDRAWISURF_LOST                 0x00000004
#define DDRAWISURF_HASCKEYDESTOVERLAY   0x00000100
#define DDRAWISURF_HASCKEYDESTBLT       0x00000200
#define DDRAWISURF_HASCKEYSRCOVERLAY    0x00000400
#define DDRAWISURF_HASCKEYSRCBLT        0x00000800
#define DDRAWISURF_VISIBLEOVERLAY       0x00001000

/*
 * This macro is the mask for the flags that we store in the
 * DDRAWI_DDRAWSURFACE_LCL structure's dwOverlayFlags field.
 * It's inappropriate to store the DDOVER_SHOW flag for
 * example. The reason is to keep features that would
 * otherwise be turned off (incorrectly) by 
 * UpdateOverlayZOrder on.
 */

#define DDOVER_STOREDFLAGS (DDOVER_ALPHADEST          | \
                            DDOVER_ALPHADESTNEG       | \
                            DDOVER_ALPHASRC           | \
                            DDOVER_ALPHASRCNEG        | \
                            DDOVER_ALPHACONSTOVERRIDE | \
                            DDOVER_KEYDEST            | \
                            DDOVER_KEYDESTOVERRIDE    | \
                            DDOVER_KEYSRC             | \
                            DDOVER_KEYSRCOVERRIDE     | \
                            DDOVER_MIRRORLEFTRIGHT    | \
                            DDOVER_MIRRORUPDOWN)
