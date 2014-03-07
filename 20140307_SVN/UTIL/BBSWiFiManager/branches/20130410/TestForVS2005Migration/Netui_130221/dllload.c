//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//==========================================================================*
//
// Filename:    dllload.c
//
// Owner:        
//
// Purpose:     delay load dll fnctions
//
//==========================================================================*
//==========================================================================*

#include <windows.h>
#include <windowsx.h>
#include <types.h>
#include "eapol.h"
#include "wzcsapi.h"
#include "netui.h"
#include <netuip.h>
#include <resource.h>
#include "delayld.h"
#include "Wpcpriv.h"
#include "ehm.h"
#include "wzcuip.h"

//***  Delay load functions ------ {

//***  WZCSAPI ---- {
HINSTANCE g_hinstWZCSAPI /* = NULL */;

//DWORD WZCQueryInterfaceEx(LPWSTR pSrvAddr, DWORD dwInFlags, PINTF_ENTRY_EX pIntf, LPDWORD pdwOutFlags);
DELAY_LOAD_DWORD(g_hinstWZCSAPI, wzcsapi, WZCQueryInterfaceEx,
    (LPWSTR pSrvAddr, DWORD dwInFlags, PINTF_ENTRY_EX pIntf, LPDWORD pdwOutFlags),
    (pSrvAddr, dwInFlags, pIntf, pdwOutFlags))

//DWORD WZCSetInterfaceEx(LPWSTR pSrvAddr, DWORD dwInFlags, PINTF_ENTRY_EX pIntf, LPDWORD pdwOutFlags);
DELAY_LOAD_DWORD(g_hinstWZCSAPI, wzcsapi, WZCSetInterfaceEx,
    (LPWSTR pSrvAddr, DWORD dwInFlags, PINTF_ENTRY_EX pIntf, LPDWORD pdwOutFlags),
    (pSrvAddr, dwInFlags, pIntf, pdwOutFlags))

//VOID WZCDeleteIntfObjEx(PINTF_ENTRY_EX pIntf);
DELAY_LOAD_VOID(g_hinstWZCSAPI, wzcsapi, WZCDeleteIntfObjEx, 
    (PINTF_ENTRY_EX pIntf), (pIntf))

//DWORD WZCEnumEapExtensions(DWORD *pdwNumOfExtensions, PEAP_EXTENSION_INFO *pEapExtensions);
DELAY_LOAD_ERR_FREE(wzcsapi, DWORD, WZCEnumEapExtensions,
    (DWORD *pdwNumOfExtensions, PEAP_EXTENSION_INFO *pEapExtensions),
    (pdwNumOfExtensions, pEapExtensions), ERROR_INVALID_FUNCTION)

//DWORD WZCRefreshInterfaceEx(LPWSTR pSrvAddr, DWORD dwInFlags, PINTF_ENTRY_EX pIntf, LPDWORD pdwOutFlags);
DELAY_LOAD_DWORD(g_hinstWZCSAPI, wzcsapi, WZCRefreshInterfaceEx,
    (LPWSTR pSrvAddr, DWORD dwInFlags, PINTF_ENTRY_EX pIntf, LPDWORD pdwOutFlags),
    (pSrvAddr, dwInFlags, pIntf, pdwOutFlags))

//DWORD WZCQueryContext(LPWSTR pSrvAddr, DWORD dwInFlags, PWZC_CONTEXT pWzcContext, LPDWORD pdwOutFlags);
DELAY_LOAD_DWORD(g_hinstWZCSAPI, wzcsapi, WZCQueryContext,
    (LPWSTR pSrvAddr, DWORD dwInFlags, PWZC_CONTEXT pWzcContext, LPDWORD pdwOutFlags),
    (pSrvAddr, dwInFlags, pWzcContext, pdwOutFlags))

//DWORD WZCSetContext(LPWSTR pSrvAddr, DWORD dwInFlags, PWZC_CONTEXT pWzcContext, LPDWORD pdwOutFlags);
DELAY_LOAD_DWORD(g_hinstWZCSAPI, wzcsapi, WZCSetContext,
    (LPWSTR pSrvAddr, DWORD dwInFlags, PWZC_CONTEXT pWzcContext, LPDWORD pdwOutFlags),
    (pSrvAddr, dwInFlags, pWzcContext, pdwOutFlags))

//}

//VOID WZCPassword2Key(PWZC_WLAN_CONFIG pwzcConfig,LPCSTR cszPassword);
DELAY_LOAD_VOID(g_hinstWZCSAPI, wzcsapi, WZCPassword2Key, 
    (PWZC_WLAN_CONFIG pwzcConfig, LPCSTR cszPassword),
    (pwzcConfig, cszPassword))
//}

//*** CPLMAIN.CPL ----{
HINSTANCE g_hinstCPLMain /* = NULL */;

//
//HRESULT PickCertificateDlg(HWND hwndParent, PCTSTR pcszStoreName, PCTSTR pcszHelpLink, PBYTE pbHashIn, DWORD cbHashIn, PBYTE* ppbHashOut, PDWORD pcbHashOut);
DELAY_LOAD_ORD(g_hinstCPLMain, cplmain.cpl, HRESULT, PickCertificateDlg, 160,
    (HWND hwndParent, PCTSTR pcszStoreName, PCTSTR pcszHelpLink, PCTSTR pcszDesc, PBYTE pbHashIn, DWORD cbHashIn, PBYTE* ppbHashOut, PDWORD pcbHashOut),
    (hwndParent, pcszStoreName, pcszHelpLink, pcszDesc, pbHashIn, cbHashIn, ppbHashOut, pcbHashOut))
//}

//BOOL IsNetConnected(PMIB_IPADDRROW pipar, BOOL *pfIsCradled, BOOL *pfDoingDHCP, BOOL *pfIsWiFi);
DELAY_LOAD_ORD(g_hinstCPLMain, cplmain.cpl, BOOL, IsNetConnected, 116,
    (PMIB_IPADDRROW pipar, BOOL *pfIsCradled, BOOL *pfDoingDHCP, BOOL *pfIsWiFi),
    (pipar, pfIsCradled, pfDoingDHCP, pfIsWiFi))

//***   CellCore -- {
HINSTANCE g_hinstCellCore /*= NULL*/;

//HRESULT WINAPI ConnMgrProviderMessage(HANDLE hConnection, const GUID *pguidProvider, DWORD *pdwIndex, DWORD dwMsg1, DWORD dwMsg2, PBYTE pParams, ULONG cbParamSize);
DELAY_LOAD_HRESULT(g_hinstCellCore, CellCore, ConnMgrProviderMessage,
    (HANDLE hConnection, const GUID *pguidProvider, DWORD *pdwIndex, DWORD dwMsg1, DWORD dwMsg2, PBYTE pParams, ULONG cbParamSize),
    (hConnection, pguidProvider, pdwIndex, dwMsg1, dwMsg2, pParams, cbParamSize))

// }

//}

