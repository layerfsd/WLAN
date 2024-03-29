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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

winddi.h

Abstract:  

    Private entry points, defines and types for Windows NK uDDI.
    
Notes: 


--*/

#ifndef _WINDDI_
#define _WINDDI_

#ifdef __cplusplus
    extern "C"
	{
#endif

// This GUID is used by a display driver to call AdvertiseInterface, letting
// EnumDisplayDevices know about this display.
// {4D05073C-3C44-4569-A10B-B18753E0AAAD}
#define DEVCLASS_DISPLAY_GUID { 0x4d05073c, 0x3c44, 0x4569, { 0xa1, 0xb, 0xb1, 0x87, 0x53, 0xe0, 0xaa, 0xad } }


// Allowed values for GDIINFO.ulHTOutputFormat.

#define HT_FORMAT_1BPP          0
#define HT_FORMAT_4BPP          2
#define HT_FORMAT_4BPP_IRGB     3
#define HT_FORMAT_8BPP          4
#define HT_FORMAT_16BPP         5
#define HT_FORMAT_24BPP         6
#define HT_FORMAT_32BPP         7



// Unlike Windows NT DDI, uDDI display drivers *should* also include windows.h

/////////////////////////////////////////////////////////////////////////////
// Common types used as parameters by uDDI interface

typedef ULONG ROP4;
typedef ULONG MIX;
typedef ULONG FLONG;	// Not in NT winddi.h 

DECLARE_HANDLE(HBM);
DECLARE_HANDLE(HDEV);
DECLARE_HANDLE(HSURF);
DECLARE_HANDLE(DHSURF);
DECLARE_HANDLE(DHPDEV);
DECLARE_HANDLE(HDRVOBJ);

// typedef struct _POINTE      /* pte  */
// {
//     FLOAT x;
//     FLOAT y;
// } POINTE, *PPOINTE;

typedef union _FLOAT_LONG
{
   FLOAT   e;
   LONG    l;
} FLOAT_LONG, *PFLOAT_LONG;


// Fixed-point (28.4) types & macros

typedef LONG	FIX;
typedef FIX     *PFIX;

typedef struct _POINTFIX
{
    FIX   x;
    FIX   y;
} POINTFIX, *PPOINTFIX;

typedef struct _RECTFX
{
    FIX   xLeft;
    FIX   yTop;
    FIX   xRight;
    FIX   yBottom;
} RECTFX, *PRECTFX;

#define LTOFX(x)            ((x)<<4)
#define FXTOL(x)            ((x)>>4)
#define FXTOLFLOOR(x)       ((x)>>4)
#define FXTOLCEILING(x)     ((x + 0x0F)>>4)
#define FXTOLROUND(x)       ((((x) >> 3) + 1) >> 1)


/////////////////////////////////////////////////////////////////////////////
// Objects implemented by GDI

// BRUSHOBJ

typedef struct _BRUSHOBJ
{
    ULONG   iSolidColor;
    PVOID   pvRbrush;
} BRUSHOBJ;

// CLIPOBJ

typedef struct _CLIPOBJ
{
    ULONG   iUniq;
    RECTL   rclBounds;
    BYTE    iDComplexity;
    BYTE    iFComplexity;
    BYTE    iMode;
    BYTE    fjOptions;
} CLIPOBJ;

#define DC_TRIVIAL      0
#define DC_RECT         1
#define DC_COMPLEX      3

#define FC_RECT         1
#define FC_RECT4        2
#define FC_COMPLEX      3

#define TC_RECTANGLES   0

#define CT_RECTANGLES   0L

#define CD_RIGHTDOWN    0L
#define CD_LEFTDOWN     1L
#define CD_RIGHTUP      2L
#define CD_LEFTUP       3L
#define CD_ANY          4L

typedef struct _ENUMRECTS
{
    ULONG       c;
    RECTL       arcl[1];
} ENUMRECTS;

// PALOBJ

typedef struct _PALOBJ
{
    ULONG   ulReserved;
} PALOBJ;


// PATHOBJ

typedef struct _PATHOBJ
{
    FLONG   fl;
    ULONG   cCurves;
} PATHOBJ;

#define PD_BEGINSUBPATH   0x00000001
#define PD_ENDSUBPATH     0x00000002
#define PD_RESETSTYLE     0x00000004
#define PD_CLOSEFIGURE    0x00000008
#define PD_BEZIERS        0x00000010
#define PD_ALL           (PD_BEGINSUBPATH | \
                          PD_ENDSUBPATH   | \
                          PD_RESETSTYLE   | \
                          PD_CLOSEFIGURE  | \
                          PD_BEZIERS)

typedef struct  _PATHDATA
{
    FLONG    flags;
    ULONG    count;
    POINTFIX *pptfx;
} PATHDATA, *PPATHDATA;

// SURFOBJ

typedef struct _SURFOBJ
{
    DHSURF  dhsurf;
    HSURF   hsurf;
    DHPDEV  dhpdev;
    HDEV    hdev;
    SIZEL   sizlBitmap;
    ULONG   cjBits;
    PVOID   pvBits;
    PVOID   pvScan0;
    LONG    lDelta;
    ULONG   iUniq;
    ULONG   iBitmapFormat;
    USHORT  iType;
    USHORT  fjBitmap;
} SURFOBJ;

#define BMF_1BPP       1L
#define BMF_2BPP       9L
#define BMF_4BPP       2L
#define BMF_8BPP       3L
#define BMF_16BPP      4L
#define BMF_24BPP      5L
#define BMF_32BPP      6L

#define BMF_USERMEM      0x0008
#define BMF_SRCPREROTATE 0x8000

// XFORMOBJ (compatibility only)

typedef struct _XFORMOBJ
{
    ULONG ulReserved;
} XFORMOBJ;

// XLATEOBJ

typedef struct _XLATEOBJ
{
    ULONG   iUniq;
    FLONG   flXlate;
    USHORT  iSrcType;
    USHORT  iDstType;
    ULONG   cEntries;
    ULONG  *pulXlate;
} XLATEOBJ;

#define PAL_INDEXED       0x00000001
#define PAL_BITFIELDS     0x00000002
#define PAL_RGB           0x00000004
#define PAL_BGR           0x00000008
#define PAL_FOURCC        0x00000010

#define XO_TRIVIAL      0x00000001
#define XO_TABLE        0x00000002
#define XO_TO_MONO      0x00000004

#define XO_SRCPALETTE    1
#define XO_DESTPALETTE   2
#define XO_DESTDCPALETTE 3

typedef struct  tagDEVINFO
{
    FLONG       flGraphicsCaps;
    LOGFONTW     lfDefaultFont;
    LOGFONTW     lfAnsiVarFont;
    LOGFONTW     lfAnsiFixFont;
    ULONG       cFonts;
    ULONG       iDitherFormat;
    USHORT      cxDither;
    USHORT      cyDither;
    HPALETTE    hpalDefault;
} DEVINFO, *PDEVINFO;


typedef struct tagBLENDOBJ
{
    BLENDFUNCTION BlendFunction;
} BLENDOBJ, *PBLENDOBJ;

// The GCAPS_CLEARTYPE_VERTICALY_STRIPED means that the physical
// hardware which the system will be attached to has its pixels
// in vertical stripes. E.G. if it were an RGB display the pixels
// would look like:
//
//         =============
//         | R | G | B |
//         =============
//
// horizontally striped pixels would look like:
//
//            =====
//            | R |
//            =====
//            | G |
//            =====
//            | B |
//            =====

#define GCAPS_GRAY16                            0x01000000
#define GCAPS_CLEARTYPE                         0x00000100 
#define GCAPS_CLEARTYPE_HORIZONTALY_STRIPED     0x00000200
#define GCAPS_TEXT_CAPS               (GCAPS_GRAY16 | GCAPS_CLEARTYPE | GCAPS_CLEARTYPE_HORIZONTALY_STRIPED)

/////////////////////////////////////////////////////////////////////////////
// Line drawing info

typedef struct  _LINEATTRS
{
    FLONG       fl;
    ULONG       iJoin;
    ULONG       iEndCap;
    FLOAT_LONG  elWidth;
    FLOAT       eMiterLimit;
    ULONG       cstyle;
    PFLOAT_LONG pstyle;
    FLOAT_LONG  elStyleState;
} LINEATTRS, *PLINEATTRS;

#define LA_GEOMETRIC        0x00000001
#define LA_ALTERNATE        0x00000002
#define LA_STARTGAP         0x00000004
#define LA_STYLED           0x00000008

// Methods implemented by GDI


#define DEFINE_GDI_ENTRY_POINTS(SCOPE,LEFT,RIGHT)		\
														\
/*  BRUSHOBJ callbacks */								\
														\
SCOPE PVOID LEFT BRUSHOBJ_pvAllocRbrush RIGHT (			\
	BRUSHOBJ *pbo,										\
	ULONG     cj);										\
														\
SCOPE PVOID LEFT BRUSHOBJ_pvGetRbrush RIGHT (			\
	BRUSHOBJ *pbo);										\
														\
/*  CLIPOBJ Callbacks */								\
														\
SCOPE ULONG LEFT CLIPOBJ_cEnumStart RIGHT (				\
	CLIPOBJ *pco,										\
	BOOL     bAll,										\
	ULONG    iType,										\
	ULONG    iDirection,								\
	ULONG    cLimit);									\
														\
SCOPE BOOL LEFT CLIPOBJ_bEnum RIGHT (					\
	CLIPOBJ *pco,										\
	ULONG    cj,										\
	ULONG   *pul);										\
														\
/*  PALOBJ Callbacks */									\
														\
SCOPE ULONG LEFT PALOBJ_cGetColors RIGHT (				\
	PALOBJ *ppalo,										\
	ULONG   iStart,										\
	ULONG   cColors,									\
	ULONG  *pulColors);									\
														\
/*  PATHOBJ Callbacks */								\
														\
SCOPE VOID LEFT PATHOBJ_vEnumStart RIGHT (				\
	PATHOBJ *ppo);										\
														\
SCOPE BOOL LEFT PATHOBJ_bEnum RIGHT (					\
	PATHOBJ  *ppo,										\
	PATHDATA *ppd);										\
														\
SCOPE VOID LEFT PATHOBJ_vGetBounds RIGHT (				\
	PATHOBJ *ppo,										\
	PRECTFX prectfx);									\
														\
/*  XLATEOBJ Callbacks */								\
														\
SCOPE ULONG LEFT  XLATEOBJ_cGetPalette RIGHT (			\
	XLATEOBJ *pxlo,										\
	ULONG     iPal,										\
	ULONG     cPal,										\
	ULONG    *pPal);									\
														\
/*  EngXxx Functions implemented by GDI */				\
														\
SCOPE HSURF LEFT EngCreateDeviceSurface RIGHT (			\
	DHSURF dhsurf,										\
	SIZEL sizl,											\
	ULONG iFormatCompat);								\
														\
SCOPE BOOL LEFT EngDeleteSurface RIGHT (				\
	HSURF hsurf);										\
														\
SCOPE HBITMAP LEFT EngCreateDeviceBitmap RIGHT (		\
    IN DHSURF  dhsurf,									\
    IN SIZEL  sizl,										\
    IN ULONG  iFormatCompat	);							\
                                                        \
SCOPE HPALETTE LEFT EngCreatePalette RIGHT (            \
    IN ULONG  iMode,	                                \
    IN ULONG  cColors,	                                \
    IN PULONG pulColors,	                            \
    IN FLONG  flRed,	                                \
    IN FLONG  flGreen,	                                \
    IN FLONG  flBlue);                                  \



#define EMPTYPARM
#if DDI					// Must be set when compiling driver
#define LEFTENTRY (*
#define RIGHTENTRY )
DEFINE_GDI_ENTRY_POINTS(extern,LEFTENTRY,RIGHTENTRY)
#else
DEFINE_GDI_ENTRY_POINTS(EMPTYPARM,APIENTRY,EMPTYPARM)
#endif
// One driver module should contain  DEFINE_GDI_ENTRY_POINTS(EMPTYPARM,LEFTENTRY,RIGHTENTRY)

/////////////////////////////////////////////////////////////////////////////
// DrvXxx Functions implemented by display driver

// Configuration / Intialization funtions

VOID DrvDrvDisableDriver();

VOID  APIENTRY DrvDisablePDEV(
	DHPDEV dhpdev);

VOID  APIENTRY DrvDisableSurface(
	DHPDEV dhpdev);

DHPDEV APIENTRY DrvEnablePDEV(
	DEVMODEW *pdm,
	LPWSTR    pwszLogAddress,
	ULONG     cPat,
	HSURF    *phsurfPatterns,
	ULONG     cjCaps,
	ULONG    *pdevcaps,
	ULONG     cjDevInfo,
	DEVINFO  *pdi,
	HDEV      hdev,
	LPWSTR    pwszDeviceName,
	HANDLE    hDriver);

HSURF APIENTRY DrvEnableSurface(
	DHPDEV dhpdev);

ULONG APIENTRY DrvGetModes(
	HANDLE    hDriver,
	ULONG     cjSize,
	DEVMODEW *pdm);

// Surface creation

HBITMAP APIENTRY DrvCreateDeviceBitmap(
	DHPDEV dhpdev,
	SIZEL sizl,
	ULONG iFormat);

VOID APIENTRY DrvDeleteDeviceBitmap(
	DHSURF dhsurf);

// Cursor control functions

VOID APIENTRY DrvMovePointer(
	SURFOBJ *pso,
	LONG x,
	LONG y,
	RECTL *prcl);

#define SPS_ERROR               0
#define SPS_DECLINE             1
#define SPS_ACCEPT_NOEXCLUDE    2
#define SPS_ACCEPT_EXCLUDE      3

#define SPS_CHANGE        0x00000001L
#define SPS_ASYNCCHANGE   0x00000002L
#define SPS_ANIMATESTART  0x00000004L
#define SPS_ANIMATEUPDATE 0x00000008L


ULONG APIENTRY DrvSetPointerShape(
	SURFOBJ  *pso,
	SURFOBJ  *psoMask,
	SURFOBJ  *psoColor,
	XLATEOBJ *pxlo,
	LONG      xHot,
	LONG      yHot,
	LONG      x,
	LONG      y,
	RECTL    *prcl,
	FLONG     fl);

// Palette Control Functions

BOOL APIENTRY DrvSetPalette(
	DHPDEV  dhpdev,
	PALOBJ *ppalo,
	FLONG   fl,
	ULONG   iStart,
	ULONG   cColors);

// Blt functions
BOOL APIENTRY DrvBitBlt(
	SURFOBJ  *psoTrg,
	SURFOBJ  *psoSrc,
	SURFOBJ  *psoMask,
	CLIPOBJ  *pco,
	XLATEOBJ *pxlo,
	RECTL    *prclTrg,
	POINTL   *pptlSrc,
	POINTL   *pptlMask,
	BRUSHOBJ *pbo,
	POINTL   *pptlBrush,
	ROP4      rop4);

BOOL APIENTRY DrvCopyBits(
	SURFOBJ  *psoDest,
	SURFOBJ  *psoSrc,
	CLIPOBJ  *pco,
	XLATEOBJ *pxlo,
	RECTL    *prclDest,
	POINTL   *pptlSrc);

// bltFlags values for DrvAnyBlt, DrvTransparentBlt, and DrvAlphaBlend
#define BLT_ALPHABLEND      16
#define BLT_STRETCH          8
#define BLT_TRANSPARENT      4
#define BLT_DSTTRANSPARENT   2

BOOL APIENTRY DrvAnyBlt(
	SURFOBJ         *psoDest,
	SURFOBJ         *psoSrc,
	SURFOBJ         *psoMask,
	CLIPOBJ         *pco,
	XLATEOBJ        *pxlo,
	POINTL          *pptlHTOrg,
	RECTL           *prclDest,
	RECTL           *prclSrc,
	POINTL          *pptlMask,
	BRUSHOBJ        *pbo,
	POINTL          *pptlBrush,
	ROP4             rop4,
	ULONG            iMode,
	ULONG	         bltFlags);

BOOL APIENTRY DrvTransparentBlt(
	SURFOBJ         *psoDest,
	SURFOBJ         *psoSrc,
	CLIPOBJ         *pco,
	XLATEOBJ        *pxlo,
	RECTL           *prclDest,
	RECTL           *prclSrc,
	ULONG            TransColor);

// Drawing functions

BOOL APIENTRY DrvFillPath(
	SURFOBJ  *pso,
	PATHOBJ  *ppo,
	CLIPOBJ  *pco,
	BRUSHOBJ *pbo,
	POINTL   *pptlBrushOrg,
	MIX       mix,
	FLONG     flOptions);

BOOL APIENTRY DrvPaint(
	SURFOBJ  *pso,
	CLIPOBJ  *pco,
	BRUSHOBJ *pbo,
	POINTL   *pptlBrushOrg,
	MIX       mix);

BOOL APIENTRY DrvRealizeBrush(
	BRUSHOBJ *pbo,
	SURFOBJ  *psoTarget,
	SURFOBJ  *psoPattern,
	SURFOBJ  *psoMask,
	XLATEOBJ *pxlo,
	ULONG    iHatch);

BOOL APIENTRY DrvStrokePath(
	SURFOBJ   *pso,
	PATHOBJ   *ppo,
	CLIPOBJ   *pco,
	XFORMOBJ  *pxo,
	BRUSHOBJ  *pbo,
	POINTL    *pptlBrushOrg,
	LINEATTRS *plineattrs,
	MIX        mix);

// Extensions to the DDI made for MGDI

ULONG APIENTRY DrvRealizeColor(
    USHORT     iDstType,
    ULONG      cEntries,
    ULONG     *pPalette,
    ULONG      rgbColor);
    
ULONG *APIENTRY DrvGetMasks(
    DHPDEV     dhpdev);
    
ULONG APIENTRY DrvUnrealizeColor(
    USHORT     iSrcType,
    ULONG      cEntries,
    ULONG     *pPalette,
    ULONG      iRealizedColor);

BOOL APIENTRY DrvContrastControl(
	DHPDEV     dhpdev,
    ULONG      cmd,
    ULONG     *pValue);
    
VOID APIENTRY DrvPowerHandler(
	DHPDEV     dhpdev,
    BOOL       bOff);
    
ULONG APIENTRY DrvEscape(
	DHPDEV     dhpdev,
    SURFOBJ   *pso,
    ULONG      iEsc,
    ULONG      cjIn,
    PVOID      pvIn,
    ULONG      cjOut,
    PVOID      pvOut);


BOOL APIENTRY DrvGradientFill(
		SURFOBJ * pso, 
		CLIPOBJ * pco, 
		XLATEOBJ * pxlo, 
		TRIVERTEX * pVertex, 
		ULONG nVertex, 
		PVOID pMesh, 
		ULONG nMesh, 
		RECTL * prclExtents, 
		POINTL * pptlDitherOrg, 
		ULONG ulMode);

BOOL APIENTRY DrvAlphaBlend(
		SURFOBJ  * psoDest,
		SURFOBJ  * psoSrc,
		CLIPOBJ  * pco,
		XLATEOBJ * pxlo,
		RECTL    * prclDest,
		RECTL    * prclSrc,
		BLENDOBJ * pBlendObj);

/////////////////////////////////////////////////////////////////////////////
// Driver setup & function hook requirements

// typedef int (*PFN)();
// 
// typedef struct  _DRVFN  /* drvfn */
// {
//     ULONG   iFunc;
//     PFN     pfn;
// } DRVFN, *PDRVFN;
// 
// // Function indeces
// 
// #define INDEX_DrvEnablePDEV              0L
// // #define INDEX_DrvCompletePDEV            1L
// #define INDEX_DrvDisablePDEV             2L
// #define INDEX_DrvEnableSurface           3L
// #define INDEX_DrvDisableSurface          4L
// // #define INDEX_DrvAssertMode              5L
// // #define INDEX_DrvResetPDEV               7L
// #define INDEX_DrvCreateDeviceBitmap     10L
// #define INDEX_DrvDeleteDeviceBitmap     11L
// #define INDEX_DrvRealizeBrush           12L
// // #define INDEX_DrvDitherColor            13L
// #define INDEX_DrvStrokePath             14L
// #define INDEX_DrvFillPath               15L
// // #define INDEX_DrvStrokeAndFillPath      16L
// #define INDEX_DrvPaint                  17L
// #define INDEX_DrvBitBlt                 18L
// #define INDEX_DrvCopyBits               19L
// #define INDEX_DrvStretchBlt             20L
// #define INDEX_DrvSetPalette             22L
// // #define INDEX_DrvTextOut                23L
// // #define INDEX_DrvEscape                 24L
// // #define INDEX_DrvDrawEscape             25L
// // #define INDEX_DrvQueryFont              26L
// // #define INDEX_DrvQueryFontTree          27L
// // #define INDEX_DrvQueryFontData          28L
// #define INDEX_DrvSetPointerShape        29L
// #define INDEX_DrvMovePointer            30L
// // #define INDEX_DrvLineTo                 31L
// // #define INDEX_DrvSendPage               32L
// // #define INDEX_DrvStartPage              33L
// // #define INDEX_DrvEndDoc                 34L
// // #define INDEX_DrvStartDoc               35L
// // #define INDEX_DrvGetGlyphMode           37L
// // #define INDEX_DrvSynchronize            38L
// // #define INDEX_DrvSaveScreenBits         40L
// #define INDEX_DrvGetModes               41L
// // #define INDEX_DrvFree                   42L
// // #define INDEX_DrvDestroyFont            43L
// // #define INDEX_DrvQueryFontCaps          44L
// // #define INDEX_DrvLoadFontFile           45L
// // #define INDEX_DrvUnloadFontFile         46L
// // #define INDEX_DrvFontManagement         47L
// // #define INDEX_DrvQueryTrueTypeTable     48L
// // #define INDEX_DrvQueryTrueTypeOutline   49L
// // #define INDEX_DrvGetTrueTypeFile        50L
// // #define INDEX_DrvQueryFontFile          51L
// // #define INDEX_UNUSED5                   52L
// // #define INDEX_DrvQueryAdvanceWidths     53L
// // #define INDEX_DrvSetPixelFormat         54L
// // #define INDEX_DrvDescribePixelFormat    55L
// // #define INDEX_DrvSwapBuffers            56L
// // #define INDEX_DrvStartBanding           57L
// // #define INDEX_DrvNextBand               58L
// 
// // The total number of dispatched functions
// 
// #define INDEX_LAST                      59L
// 
// typedef struct  tagDRVENABLEDATA
// {
//     ULONG   iDriverVersion;
//     ULONG   c;
//     DRVFN  *pdrvfn;
// } DRVENABLEDATA, *PDRVENABLEDATA;

#define DDI_DRIVER_VERSION 0x00040001
#define GDI_DRIVER_VERSION 0x4001   /* for win version > NT 4.0  (i.e 4.0.01) */

typedef struct tagDrvEnableData
{
	// Version control of this structure is performed by incrementing
	// DDI_DRIVER_VERSION whenever alterations are made.

	DHPDEV (* DrvEnablePDEV )
	(
		DEVMODEW *pdm,
		LPWSTR    pwszLogAddress,
		ULONG     cPat,
		HSURF    *phsurfPatterns,
		ULONG     cjCaps,
		ULONG    *pdevcaps,
		ULONG     cjDevInfo,
		DEVINFO  *pdi,
		HDEV      hdev,
		LPWSTR    pwszDeviceName,
		HANDLE    hDriver
	);

	VOID (* DrvDisablePDEV )
	(
		DHPDEV dhpdev
	);

	HSURF (* DrvEnableSurface )
	(
		DHPDEV dhpdev
	);

	VOID (* DrvDisableSurface )
	(
		DHPDEV dhpdev
	);

	HBITMAP (* DrvCreateDeviceBitmap )
	(
		DHPDEV dhpdev,
		SIZEL sizl,
		ULONG iFormat
	);

	VOID (* DrvDeleteDeviceBitmap )
	(
		DHSURF dhsurf
	);

	BOOL (* DrvRealizeBrush )
	(
		BRUSHOBJ *pbo,
		SURFOBJ  *psoTarget,
		SURFOBJ  *psoPattern,
		SURFOBJ  *psoMask,
		XLATEOBJ *pxlo,
		ULONG    iHatch
	);

	BOOL (* DrvStrokePath )
	(
		SURFOBJ   *pso,
		PATHOBJ   *ppo,
		CLIPOBJ   *pco,
		XFORMOBJ  *pxo,
		BRUSHOBJ  *pbo,
		POINTL    *pptlBrushOrg,
		LINEATTRS *plineattrs,
		MIX        mix
	);

	BOOL (* DrvFillPath )
	(
		SURFOBJ  *pso,
		PATHOBJ  *ppo,
		CLIPOBJ  *pco,
		BRUSHOBJ *pbo,
		POINTL   *pptlBrushOrg,
		MIX       mix,
		FLONG     flOptions
	);

	BOOL (* DrvPaint )
	(
		SURFOBJ  *pso,
		CLIPOBJ  *pco,
		BRUSHOBJ *pbo,
		POINTL   *pptlBrushOrg,
		MIX       mix
	);

	BOOL (* DrvBitBlt )
	(
		SURFOBJ  *psoTrg,
		SURFOBJ  *psoSrc,
		SURFOBJ  *psoMask,
		CLIPOBJ  *pco,
		XLATEOBJ *pxlo,
		RECTL    *prclTrg,
		POINTL   *pptlSrc,
		POINTL   *pptlMask,
		BRUSHOBJ *pbo,
		POINTL   *pptlBrush,
		ROP4      rop4
	);

	BOOL (* DrvCopyBits )
	(
		SURFOBJ  *psoDest,
		SURFOBJ  *psoSrc,
		CLIPOBJ  *pco,
		XLATEOBJ *pxlo,
		RECTL    *prclDest,
		POINTL   *pptlSrc
	);

	BOOL (* DrvAnyBlt )
	(
		SURFOBJ         *psoDest,
		SURFOBJ         *psoSrc,
		SURFOBJ         *psoMask,
		CLIPOBJ         *pco,
		XLATEOBJ        *pxlo,
		POINTL          *pptlHTOrg,
		RECTL           *prclDest,
		RECTL           *prclSrc,
		POINTL          *pptlMask,
    	BRUSHOBJ        *pbo,
    	POINTL          *pptlBrush,
    	ROP4             rop4,
    	ULONG            iMode,
    	ULONG	         bltFlags
	);

	BOOL (* DrvTransparentBlt )
	(
		SURFOBJ         *psoDest,
		SURFOBJ         *psoSrc,
		CLIPOBJ         *pco,
		XLATEOBJ        *pxlo,
		RECTL           *prclDest,
		RECTL           *prclSrc,
		ULONG            TransColor
	);

	BOOL (* DrvSetPalette )
	(
		DHPDEV  dhpdev,
		PALOBJ *ppalo,
		FLONG   fl,
		ULONG   iStart,
		ULONG   cColors
	);

	ULONG (* DrvSetPointerShape)
	(
		SURFOBJ  *pso,
		SURFOBJ  *psoMask,
		SURFOBJ  *psoColor,
		XLATEOBJ *pxlo,
		LONG      xHot,
		LONG      yHot,
		LONG      x,
		LONG      y,
		RECTL    *prcl,
		FLONG     fl
	);

	VOID (* DrvMovePointer)
	(
		SURFOBJ *pso,
		LONG x,
		LONG y,
		RECTL *prcl
	);

	ULONG (* DrvGetModes)
	(
		HANDLE    hDriver,
		ULONG     cjSize,
		DEVMODEW *pdm
	);

    ULONG (* DrvRealizeColor )
    (
        USHORT     iDstType,
        ULONG      cEntries,
        ULONG     *pPalette,
        ULONG      rgbColor
    );
    
    ULONG *(* DrvGetMasks )
    (
        DHPDEV     dhpdev
    );
    
    ULONG (* DrvUnrealizeColor )
    (
        USHORT     iSrcType,
        ULONG      cEntries,
        ULONG     *pPalette,
        ULONG      iRealizedColor
    );

    BOOL (* DrvContrastControl)
    (
	    DHPDEV     dhpdev,
        ULONG      cmd,
        ULONG     *pValue
    );
    
    VOID (* DrvPowerHandler)
    (
    	DHPDEV     dhpdev,
        BOOL       bOff
    );
    
    BOOL (* DrvEndDoc )
    (
        IN SURFOBJ  *pso,	
        IN FLONG  fl	
    );

    BOOL (* DrvStartDoc )
    (
        IN SURFOBJ  *pso,	
        IN PWSTR  pwszDocName,	
        IN DWORD  dwJobId	
    );

    BOOL (* DrvStartPage )
    (
        IN SURFOBJ  *pso	
    );

    ULONG (* DrvEscape )
    (
    	IN DHPDEV    dhpdev,
        IN SURFOBJ  *pso,
        IN ULONG     iEsc,
        IN ULONG     cjIn,
        IN PVOID     pvIn,
        OUT ULONG    cjOut,
        OUT PVOID    pvOut
    );

	BOOL (* DrvGradientFill)
	(
		SURFOBJ * pso, 
		CLIPOBJ * pco, 
		XLATEOBJ * pxlo, 
		TRIVERTEX * pVertex, 
		ULONG nVertex, 
		PVOID pMesh, 
		ULONG nMesh, 
		RECTL * prclExtents, 
		POINTL * pptlDitherOrg, 
		ULONG ulMode
		);

	BOOL (* DrvAlphaBlend)
	(
		SURFOBJ  * psoDest,
		SURFOBJ  * psoSrc,
		CLIPOBJ  * pco,
		XLATEOBJ * pxlo,
		RECTL    * prclDest,
		RECTL    * prclSrc,
		BLENDOBJ * pBlendObj
	);

    BOOL (* DrvExclusiveMode)
    (
        DHPDEV dhpdev,
        BOOL   bExclusiveMode
    );
	
} DRVENABLEDATA, *PDRVENABLEDATA;

#define ED_ABORTDOC    1

typedef struct _GDIINFO
{
    ULONG ulVersion;
    ULONG ulTechnology;
    ULONG ulHorzSize;
    ULONG ulVertSize;
    ULONG ulHorzRes;
    ULONG ulVertRes;
    ULONG cBitsPixel;
    ULONG cPlanes;
    ULONG ulNumColors;
    ULONG flRaster;
    ULONG ulLogPixelsX;
    ULONG ulLogPixelsY;
    ULONG flTextCaps;
    ULONG flShadeBlendCaps;

    ULONG ulDACRed;
    ULONG ulDACGreen;
    ULONG ulDACBlue;

    ULONG ulAspectX;
    ULONG ulAspectY;
    ULONG ulAspectXY;

    LONG  xStyleStep;
    LONG  yStyleStep;
    LONG  denStyleStep;

    POINTL ptlPhysOffset;
    SIZEL  szlPhysSize;

    ULONG ulNumPalReg;

// These fields are for halftone initialization.

    // COLORINFO ciDevice;
    ULONG     ulDevicePelsDPI;
    ULONG     ulPrimaryOrder;
    ULONG     ulHTPatternSize;
    ULONG     ulHTOutputFormat;
    ULONG     flHTFlags;

    ULONG ulVRefresh;
    ULONG ulBltAlignment;

    ULONG ulPanningHorzRes;
    ULONG ulPanningVertRes;

    //ULONG ulDesktopHorzRes;
    //ULONG ulDesktopVertRes;

    LONG lStride;

} GDIINFO, *PGDIINFO;


// Directly exported functions

typedef struct _ENGCALLBACKS
{
	PVOID (* BRUSHOBJ_pvAllocRbrush)
	(
		BRUSHOBJ *pbo,
		ULONG     cj
	);

	PVOID (* BRUSHOBJ_pvGetRbrush)
	(
		BRUSHOBJ *pbo
	);

	ULONG (* CLIPOBJ_cEnumStart)
	(
		CLIPOBJ *pco,
		BOOL     bAll,
		ULONG    iType,
		ULONG    iDirection,
		ULONG    cLimit
	);

	BOOL (* CLIPOBJ_bEnum)
	(
		CLIPOBJ *pco,
		ULONG    cj,
		ULONG   *pul
	);

	ULONG (* PALOBJ_cGetColors)
	(
		PALOBJ *ppalo,
		ULONG   iStart,
		ULONG   cColors,
		ULONG  *pulColors
	);


	VOID (* PATHOBJ_vEnumStart)
	(
		PATHOBJ *ppo
	);

	BOOL (* PATHOBJ_bEnum)
	(
		PATHOBJ  *ppo,
		PATHDATA *ppd
	);

	VOID (* PATHOBJ_vGetBounds)
	(
		PATHOBJ *ppo,
		PRECTFX prectfx
	);

	ULONG (* XLATEOBJ_cGetPalette)
	(
		XLATEOBJ *pxlo,
        ULONG     iPal,
        ULONG     cPal,
        ULONG    *pPal
	);

	HSURF (* EngCreateDeviceSurface)
	(
		DHSURF dhsurf,
		SIZEL sizl,
		ULONG iFormatCompat
	);

	BOOL (* EngDeleteSurface)
	(
		HSURF hsurf
	);

	HBITMAP (* EngCreateDeviceBitmap)
	(
	    IN DHSURF  dhsurf,	
	    IN SIZEL  sizl,	
	    IN ULONG  iFormatCompat
	);

    HPALETTE (* EngCreatePalette)
    (
        IN ULONG  iMode,	
        IN ULONG  cColors,	
        IN PULONG pulColors,	
        IN FLONG  flRed,	
        IN FLONG  flGreen,	
        IN FLONG  flBlue	
   );
} ENGCALLBACKS, *PENGCALLBACKS;

// Windows CE uses a extension to the desktop DDI interface for Eng Callbacks.
// The extension is exposed as an extra parameter (pCallbacks) to the function 
// DrvEnableDriver.

BOOL DrvEnableDriver(
	ULONG          iEngineVersion,
	ULONG          cj,
	DRVENABLEDATA *pded,
	PENGCALLBACKS  pCallbacks);

typedef BOOL (* PEntryPointType)(ULONG, ULONG, DRVENABLEDATA *, PENGCALLBACKS  pCallbacks);

// Display drivers that export this entry point will be invoked immediately after they are loaded
// but prior to the first call to DrvEnableDriver().  GWES uses this entry point to notify drivers
// of the location of their instance data in the registry (if it is known) and of the number of 
// monitors they should expect to support.  This API is called only once, regardless of the number
// of instances of a given display card are present.
typedef BOOL (* PDisplayInitType)(LPCTSTR, DWORD);
BOOL DisplayInit(LPCTSTR pszInstance, DWORD dwNumMonitors);

VOID DrvDrvDisableDriver();

//for multimon.  The max number of monitors supported is 4 currently
//driver deals differently when gdi set the single_driver_handle.
#define MONITORS_MAX 4
#define SINGLE_DRIVER_HANDLE 123



#ifdef __cplusplus
    }	// extern "C"
#endif


#endif  //  _WINDDI_






