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
#ifndef _W32UTIL_H_
#define _W32UTIL_H_
// Copyright (c) 1994-1999, Microsoft Corp. All rights reserved.
//***   w32util.h --
//

#ifdef __cplusplus
extern "C" {
#endif

// i64 stubs {
#ifndef BOOLFROMPTR
typedef int INT_PTR;
typedef long LONG_PTR;
typedef DWORD DWORD_PTR;
#define BOOLFROMPTR(p)  ((BOOL)(p))
#endif
// }

#define IDM_MAX     0x7fffffff      // max IDM (for menu merge, etc.)

enum
{
    MM_ADDSEPARATOR     = 0x0001,
    MM_SUBMENUSHAVEIDS  = 0x0002,
};

UINT Shell_MergeMenus(HMENU hmDst, HMENU hmSrc, UINT uInsert, UINT uIDAdjust,
    UINT uIDAdjustMax, ULONG uFlags);
BOOL Shell_UnMergeMenus(HMENU hmenuDst, UINT idCmdFirst, UINT idCmdLast);

UINT SHFixMenuIndex(HMENU hmenu, UINT indexMenu);
DWORD SHGetMenuItemData(HMENU hmenu, UINT uItem, BOOL fByPos);
BOOL SHSetMenuItemData(HMENU hmenu, UINT uItem, BOOL fByPos, DWORD dwData);
HWND SHGetParentOrSelf(HWND hwnd);
HMENU SHLoadMenuPopup(HINSTANCE hinst, UINT id);
HRESULT SHCleanMenu(HMENU hmenu);

//$REVIEW: move this to platform.h or other header once we finish merge with aygshell
//         Only referenced outside in shell32g\tpc\start.cpp, which is not compiled.
HFONT CreateFontVariant(int fnObject, int lfWeight, int lfHeight);

//***   SHOn* -- message handler implementations {
LRESULT SHOnSAN_NOTIFY(HWND hWnd, WPARAM wParam, LPARAM lParam, TCHAR *pszApp, BOOL fForceBaseStateOnLaunch);
// }

DWORD GetProcNumber();

#ifdef DEBUG
void Dbg_DumpMenu(LPCTSTR psz, HMENU hmenu);
//*** IS_CDS -- is this a valid COPYDATASTRUCT for my type?
#define IS_CDS(lP, type)    (((COPYDATASTRUCT*)(lP))->cbData == SIZEOF(type))
#else
/* it's an error to reference a Dbg_xxx routine (so no extern)! */
#endif

#ifdef __cplusplus
}
#endif

#endif // _W32UTIL_H_
