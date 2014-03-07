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
#if !defined(WABMEM_H)
#define WABMEM_H

#if !defined(MAPIX_H)
typedef SCODE (STDMETHODCALLTYPE MAPIALLOCATEBUFFER)(
	ULONG			cbSize,
	LPVOID FAR *	lppBuffer
);

typedef SCODE (STDMETHODCALLTYPE MAPIALLOCATEMORE)(
	ULONG			cbSize,
	LPVOID			lpObject,
	LPVOID FAR *	lppBuffer
);

typedef ULONG (STDAPICALLTYPE MAPIFREEBUFFER)(
	LPVOID			lpBuffer
);

typedef MAPIALLOCATEBUFFER FAR	*LPMAPIALLOCATEBUFFER;
typedef MAPIALLOCATEMORE FAR	*LPMAPIALLOCATEMORE;
typedef MAPIFREEBUFFER FAR 		*LPMAPIFREEBUFFER;

#endif  // MAPIX_H

typedef SCODE (STDMETHODCALLTYPE WABALLOCATEBUFFER)(
   LPWABOBJECT     lpWABObject,
	ULONG			cbSize,
	LPVOID FAR *	lppBuffer
);

typedef SCODE (STDMETHODCALLTYPE WABALLOCATEMORE)(
   LPWABOBJECT     lpWABObject,
	ULONG			cbSize,
	LPVOID			lpObject,
	LPVOID FAR *	lppBuffer
);

typedef ULONG (STDAPICALLTYPE WABFREEBUFFER)(
   LPWABOBJECT     lpWABObject,
	LPVOID			lpBuffer
);

typedef WABALLOCATEBUFFER FAR	*LPWABALLOCATEBUFFER;
typedef WABALLOCATEMORE FAR	*LPWABALLOCATEMORE;
typedef WABFREEBUFFER FAR 		*LPWABFREEBUFFER;
#endif  // WABMEM_H
