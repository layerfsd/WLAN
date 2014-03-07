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
#ifndef __STATCTLVIEW_HPP_INCLUDED__
#define __STATCTLVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "stcctlview".
This component implements the customizable static control user interface. 

The "stcctlview" component works in conjunction with the "stcctl" component.

*/

enum
StaticControlType
	{
	sctLeft				= SS_LEFT,
	sctCenter			= SS_CENTER,
	sctRight			= SS_RIGHT,
	sctIcon				= SS_ICON,
	sctLeftNoWordWrap	= SS_LEFTNOWORDWRAP,
	sctBitmap			= SS_BITMAP,
	sctIconCenter		= SS_ICON|SS_CENTERIMAGE,
	sctBitmapCenter		= SS_BITMAP|SS_CENTERIMAGE
	};

class StaticControlView_t
{
private:    
	StaticControlView_t();										//	NOT IMPLEMENTED default constructor
	StaticControlView_t(const StaticControlView_t&);			//	NOT IMPLEMENTED default copy constructor
	StaticControlView_t& operator=(const StaticControlView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~StaticControlView_t();							   			//	NOT IMPLEMENTED default destructor

public:
	// computes rectangle used to resize the static 
	// control window to the image size
	static
	bool
	ResizeRectToImage(
		HWND					hwnd,		// [in]
		unsigned int			Style,		// [in]
		enum StaticControlType	sct,		// [in]
		HANDLE      			hImage,		// [in]
		RECT					*prc		// [out]
		);
	
	static
	void
	DrawStaticText(
		HWND 						hwnd,	// [in] 
		unsigned int				Style,	// [in]
		enum StaticControlType		sct,   	// [in]
		HDC 						hdc,   	// [in]
		HBRUSH 						hbrBk  	// [in]
		);

	static
	void 
	DrawStaticImage(
		HWND 					hwnd, 		// [in]
		enum StaticControlType	sct,		// [in]
		HDC 					hdc, 		// [in]
		HBRUSH 					hbrBk,		// [in]
		HANDLE      			hImage		// [in]
		);
};

#endif /* __STATCTLVIEW_HPP_INCLUDED__ */