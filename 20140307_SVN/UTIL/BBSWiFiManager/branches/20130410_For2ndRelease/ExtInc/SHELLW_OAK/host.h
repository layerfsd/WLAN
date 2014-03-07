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
/*---------------------------------------------------------------------------*\
 *
 * (c) Copyright Microsoft Corp. 1994 All Rights Reserved
 *
 *  module: host.h
 *  date:
 *  author: tonykit
 *
 *  purpose: 
 *
\*---------------------------------------------------------------------------*/
#ifndef __HOST_H__
#define __HOST_H__

#ifdef __cplusplus
extern "C" {
#endif


#define HIMETRIC_PER_INCH   2540    //number Himetric units Per Inch

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////


VOID     Host_OleStrCpy(LPOLESTR  szDst, LPCOLESTR szSrc);
VOID     Host_OleStrCat(LPOLESTR  szDst, LPCOLESTR szSrc);
int      Host_OleStrCmp(LPCOLESTR szDst, LPCOLESTR szSrc);
STDAPI   Host_OpenOrCreateStream(LPCOLESTR pname, LPSTORAGE prootstg,
								 LPSTREAM FAR* ppstm);
STDAPI   Host_OpenReadOnlyStream(LPCOLESTR pname, LPSTORAGE prootstg,
								 LPSTREAM FAR* ppstm);
STDAPI   Host_OpenOrCreateStorage(LPCOLESTR pname, LPSTORAGE prootstg,
								  LPSTORAGE FAR* ppstg);
STDAPI   Host_OpenStorage(LPCOLESTR pname, LPSTORAGE prootstg,
						  LPSTORAGE FAR* ppstg);
STDAPI   Host_CreateStorage(LPCOLESTR pname, LPSTORAGE prootstg,
							LPSTORAGE FAR* ppstg);
STDAPI   Host_LoadStringFromStream(IStream * pStm, BSTR * pbstrString);
STDAPI   Host_SaveStringToStream(IStream * pStm, BSTR bstrString);
STDAPI   Host_CreateControl(REFCLSID rclsid, LPUNKNOWN pUnkOuter, REFIID riid,
							LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,
							LPVOID FAR* ppvObj);
STDAPI   Host_LoadControl(LPSTORAGE lpStg, LPUNKNOWN pUnkOuter, REFIID riid,
						  LPOLECLIENTSITE pClientSite, LPVOID FAR* ppvObj);
STDAPI   Host_LoadControlFromStream(IStream * lpStm, LPUNKNOWN pUnkOuter, REFIID riid,
									LPOLECLIENTSITE pClientSite, LPVOID FAR* ppvObj);
STDAPI   Host_GetTypeInfoOfGuid(REFCLSID refclsid, LPTYPEINFO *lplptinfo);
STDAPI   Host_GetTypeInfo(LPTYPEINFO lptinfoCoClass, BOOL fSource,
						  LPTYPEINFO *lplptinfo);
STDAPI   Host_XOrGUID(GUID * pGUID1, GUID * pGUID2);
STDAPI   Host_MergeTypeInfo(LPTYPEINFO lptinfo1, LPTYPEINFO lptinfo2, GUID *lpguid,
							ICreateTypeInfo *lpcreatetinfoMerged);
STDAPI   Host_CreateMergedTypeInfo(LPCOLESTR lpstrName, LPTYPEINFO lptinfo1,
								   LPTYPEINFO lptinfo2, GUID *lpguidEvents,
								   LPTYPEINFO *lplptinfoMerged);
STDAPI   Host_CopyTypeInfo(ITypeInfo *lptinfoSrc, ICreateTypeLib2 *pctypelibDest,
						   ITypeInfo **ppctiCopy);
STDAPI   Host_HiMetricToDevice(LPSIZEL psizel);
STDAPI   Host_DeviceToHiMetric(LPSIZEL psizel);

#ifdef FEATURE_DESIGN_MODE
STDAPI   Host_DrawSelectionTool(HDC hdc, LPRECT lprc);
#endif	// FEATURE_DESIGN_MODE

#ifndef UNDER_CE
STDAPI   Host_SetProp(LPUNKNOWN lpUnk, LPCOLESTR szProperty, LPCOLESTR szValue);
#endif	// !UNDER_CE

/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif	
	
#endif /* __HOST_H__ */
