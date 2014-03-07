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
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GXDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GXDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef GXDLL_EXPORTS
#define GXDLL_API __declspec(dllexport)
#else
#define GXDLL_API __declspec(dllimport)
#endif

//
//  IMPORTANT NOTICE:
//
//  GAPI functions related to display have been deprecated.
//
//  Applications compiled for previous versions of Windows Mobile will continue to work
//  in this release. However, new applications must use DirectDraw.
//   

struct GXKeyList {
    short vkUp;             // key for up
    POINT ptUp;             // x,y position of key/button.  Not on screen but in screen coordinates.
    short vkDown;
    POINT ptDown;
    short vkLeft;
    POINT ptLeft;
    short vkRight;
    POINT ptRight;
    short vkA;
    POINT ptA;
    short vkB;
    POINT ptB;
    short vkC;
    POINT ptC;
    short vkStart;
    POINT ptStart;
};

GXDLL_API int GXOpenInput();
GXDLL_API int GXCloseInput();
GXDLL_API GXKeyList GXGetDefaultKeys(int iOptions);
GXDLL_API int GXSuspend();
GXDLL_API int GXResume();


// Orientation key layout constants
#define GX_NORMALKEYS   0x02
#define GX_LANDSCAPEKEYS    0x03



