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
/*


*/
#ifndef _ICON_H_
#define _ICON_H_

#include <windows.h>

HICON
WINAPI
LoadIconW_I(
	HINSTANCE	hinst,
	PCTSTR		pszIcon
    );

BOOL 
WINAPI 
DrawIconEx_I(
	HDC		hdc, 
	int		X, 
	int		Y,
	HICON	hicon, 
	int		cx, 
	int		cy,
	UINT	istepIfAniCur, 
	HBRUSH	hbrFlickerFreeDraw, 
	UINT	diFlags
	);


BOOL
WINAPI
DestroyIcon_I(
	HICON	hicon
    );


HICON
WINAPI
CreateIconIndirect_I(
	PICONINFO	pii
	);

BOOL 
GetIconInfo_I(
	HICON hIcon,
	PICONINFO piconinfo
	);



#endif /* _ICON_H_ */
