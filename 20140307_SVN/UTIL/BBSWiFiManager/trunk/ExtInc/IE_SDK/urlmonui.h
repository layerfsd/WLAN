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

#if !defined(_DEF_URLMONUI_H_)

#define _DEF_URLMONUI_H_ 

// Return values from DialogProc's
enum  {  ZALERT_NO = 0 /* should be 0*/ , ZALERT_YES, ZALERT_YESPERSIST };

#if defined(__cplusplus)
extern "C"{
#endif 

int IsDialogBoxHandled
(
    IN HWND     hWnd,
    IN DWORD    dwAction,
    IN DWORD    dwPermissions,
    IN LPCTSTR  lpszText
);


#if defined(__cplusplus)
}
#endif

#endif


