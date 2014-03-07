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
#ifndef __UPDOWNVIEW_HPP__
#define __UPDOWNVIEW_HPP__
/*


Abstract:

This header file defines the interface to the UpdownView_t class
which implements the customizable Updown control.

*/

#include <windows.h>

class UpdownView_t
{
private:
    UpdownView_t();									//	NOT IMPLEMENTED default constructor
	UpdownView_t(const UpdownView_t&);				//	NOT IMPLEMENTED default copy constructor
	UpdownView_t& operator=(const UpdownView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~UpdownView_t();								//	NOT IMPLEMENTED default destructor

public:
	static
	void 
	HorzDrawSettings(
	       DWORD dwStyle,
	       int iVersion,
	       LPRECT prcBtn,
	       BOOL bEnabled,
	       BOOL fHasBorder,
	       unsigned fUp,
	       unsigned fDown,
	       int nPos,
	       int nUpper,
	       int nLower,
	       UINT uHot,
	       UINT* puLeftFlags, 
	       LPRECT prcLeft,
	       UINT* puRightFlags,
	       LPRECT prcRight
		);      

	static
	void 
	VertDrawSettings(
	       DWORD dwStyle,
	       int iVersion,
	       LPRECT prcBtn,
	       BOOL bEnabled,
	       BOOL fHasBorder,
	       unsigned fUp,
	       unsigned fDown,
	       int nPos,
	       int nUpper,
	       int nLower,
	       UINT uHot,
	       UINT* puUpFlags, 
	       LPRECT prcUp,
	       UINT* puDownFlags,
	       LPRECT prcDown
		);      

	static
	void
	SetSunkenBorder(
	       DWORD dwStyle,
	       UINT* puBorderType
	);

};

#endif /* __UpdownVIEW_HPP__ */


