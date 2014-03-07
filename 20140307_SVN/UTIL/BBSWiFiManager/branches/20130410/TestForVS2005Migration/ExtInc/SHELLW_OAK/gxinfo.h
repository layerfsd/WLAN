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
/*********************************************************************
 File:              gxinfo.h
 Description:   GameX device info
 *********************************************************************
	      Copyright (c) 1999 Microsoft Corporation
 *********************************************************************/

#ifndef __GXINFO_H__
#define __GXINFO_H__

struct GXDeviceInfo {
	long idVersion;
	void * pvFrameBuffer;
	unsigned long cbStride;
	unsigned long cxWidth;
	unsigned long cyHeight;
	unsigned long cBPP;
	unsigned long ffFormat;
	short vkButtonUpPortrait;
	short vkButtonUpLandscape;
	POINT ptButtonUp;
	short vkButtonDownPortrait;
	short vkButtonDownLandscape;
	POINT ptButtonDown;
	short vkButtonLeftPortrait;
	short vkButtonLeftLandscape;
	POINT ptButtonLeft;
	short vkButtonRightPortrait;
	short vkButtonRightLandscape;
	POINT ptButtonRight;
	short vkButtonAPortrait;
	short vkButtonALandscape;
	POINT ptButtonA;
	short vkButtonBPortrait;
	short vkButtonBLandscape;
	POINT ptButtonB;
	short vkButtonCPortrait;
	short vkButtonCLandscape;
	POINT ptButtonC;
	short vkButtonStartPortrait;
	short vkButtonStartLandscape;
	POINT ptButtonStart;
	void * pvReserved1;
	void * pvReserved2;
};

struct GXScreenRect {
	DWORD dwTop;
	DWORD dwLeft;
	DWORD dwWidth;
	DWORD dwHeight;
};

#define kidVersion100 100
#define kfLandscape	0x8			// Screen is rotated 270 degrees
#define kfPalette	0x10
#define kfDirect	0x20
#define kfDirect555	0x40
#define kfDirect565	0x80
#define kfDirect888	0x100
#define kfDirect444	0x200

#define GETGXINFO           0x00020000

#ifndef GETRAWFRAMEBUFFER
    #define GETRAWFRAMEBUFFER   0x00020001
    typedef struct _RawFrameBufferInfo
    {
	    WORD wFormat;
	    WORD wBPP;
	    VOID *pFramePointer;
	    int	cxStride;
	    int	cyStride;
        int cxPixels;
        int cyPixels;
    } RawFrameBufferInfo;

    #define FORMAT_565 1
    #define FORMAT_555 2
    #define FORMAT_OTHER 3
#endif

#endif
