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
#ifndef __UPDATEMGR_H_
#define __UPDATEMGR_H_

#ifdef __cplusplus
extern "C" {
#endif
/************************update app mgr registry keys *************************/
#define UPDATEMGR_ROOT_KEY                      HKEY_LOCAL_MACHINE
#define UPDATEMGR_REGKEY_PATH                   TEXT("System\\UpdateMgr")
#define UPDATEMGR_REGVAL_IMGUPDATE_DWNLDAPP     TEXT("ImgUpdateDwnldApp")
#define UPDATEMGR_REGVAL_IMGUPDATE_UPDATEAPP    TEXT("ImgUpdateUpdateApp")
#define UPDATEMGR_REGVAL_IMGUPDATE_UPDATESETTINGS    TEXT("ImgUpdateUpdateSettings")
/******************************************************************************/

/***************************Error function & macro*****************************/
#define UPDATEMGRMSG(cond, debugparams_in_parens) \
    ((cond)?((g_pfnUpdateMgrMsg!=NULL)?(g_pfnUpdateMgrMsg debugparams_in_parens),1: \
                                 (NKDbgPrintfW debugparams_in_parens),1):0)
                               
typedef void    (* PFN_UPDATEMGRMSG) (LPCWSTR lpszFmt, ...);

/* error function */
extern PFN_UPDATEMGRMSG g_pfnUpdateMgrMsg;
/*****************************************************************************/

#ifdef __cplusplus
}
#endif
#endif
