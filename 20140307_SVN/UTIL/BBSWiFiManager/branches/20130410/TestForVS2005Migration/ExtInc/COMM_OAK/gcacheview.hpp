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
#ifndef __GCACHEVIEW_HPP__
#define __GCACHEVIEW_HPP__
/*


Component Name:

gcacheview.hpp

Abstract:

This header file defines the interface to the GWE component "gcacheview".
This component sets the customizable minimum window resize values, 
some system metrics values, and GWE UI initialization routine

The "gcacheview" component works in conjunction with the "gcache" component.

*/

class CacheView_t
{
private:
    CacheView_t();								//	NOT IMPLEMENTED default constructor
	CacheView_t(const CacheView_t&);			//	NOT IMPLEMENTED default copy constructor
	CacheView_t& operator=(const CacheView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~CacheView_t();								//	NOT IMPLEMENTED default destructor

public:
	static
	void
	Init(
		HINSTANCE	hInstance,		// [in]
		bool		bLargeUi		// [in]
		);      

	static
	void
	ComputeMinWindowSize(
		LONG* pcxMinWinSize,	// [out]
		LONG* pcyMinWinSize		// [out]
		);
	
	static
	int 
	SystemMetrics(
		int nIndex		// [in]
		);
	
	static
	void 
	GWECacheSetSysColors(
		int cElements,				// [in]
		CONST INT * rgcolor,		// [in]
		CONST COLORREF * rgrgb		// [in]
		);
};


#endif //__GCACHEVIEW_HPP__

