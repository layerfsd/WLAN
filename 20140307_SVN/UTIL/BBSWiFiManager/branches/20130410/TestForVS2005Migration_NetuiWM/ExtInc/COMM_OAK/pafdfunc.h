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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

ppafdfunc.h

Abstract:

Private version of afdfunc.h

Notes: 


--*/
  
#ifndef _PAFDFUNC_H_
#define _PAFDFUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

SOCKHAND AFDSocket(DWORD AddressFamily, DWORD SocketType, DWORD Protocol,
				DWORD CatId, GUID *pProvId);

DWORD AFDControl(DWORD Protocol, DWORD Action, LPVOID InputBuffer,
				 LPDWORD InputBufferLength, LPVOID OutputBuffer,
				 LPDWORD OutputBufferLength);

DWORD AFDEnumProtocolsW(LPDWORD lpiProtocols, LPVOID lpProtocolBuffer,
						LPDWORD	lpdwBufferLength);

DWORD AFDGetHostentByAttr(LPSOCK_THREAD pThread, LPWSTR Name,
						LPBYTE Address, LPVOID pOptions);

DWORD AFDAddIPHostent(LPWSTR Hostname, char **IpAddr, LPWSTR Aliases,
					  LPFILETIME lpFileTime);

DWORD AFDSelect(UINT ReadCount, LPSOCK_LIST ReadList, UINT WriteCount,
				LPSOCK_LIST WriteList, UINT ExceptCount,
				LPSOCK_LIST ExceptList, const struct timeval *timeout,
				CRITICAL_SECTION *pDllCS);

BOOL APIENTRY AfdAddInterface (PTSTR pAdapter, void *Nte, DWORD Context,
							   int Flags, uint IpAddr, uint SubnetMask,
							   int cDns, uint *pDns, int cWins, uint *pWins);

DWORD NETbios(DWORD x1, DWORD dwOpCode, PVOID pNCB, DWORD cBuf1,
			  PBYTE pBuf1, DWORD cBuf2, PDWORD pBuf2);

int PMInstallProvider(LPGUID lpProviderId,const WCHAR *lpszProviderDllPath,
					  const LPWSAPROTOCOL_INFOW lpProtocolInfoList,
					  DWORD dwNumberOfEntries, DWORD Flags);

int PMEnumProtocols(LPINT lpiProtocols, LPWSAPROTOCOL_INFOW lpProtocolBuffer,
					LPDWORD lpdwBufferLength, LPDWORD pFlags, LPINT lpErrno);

int PMFindProvider(int af, int type, int protocol, DWORD dwFlags,
					DWORD dwCatalogEntryId, LPWSAPROTOCOL_INFOW lpProtocolInfo,
					PWSTR psPath);

int PMInstallNameSpace(LPWSTR lpszIdentifier, LPWSTR lpszPathName, 
	DWORD dwNameSpace, DWORD dwVersion, LPGUID lpProviderId, DWORD *pFlags);

int PMEnumNameSpaceProviders(DWORD *pcBuf, LPWSANAMESPACE_INFOW	pBuf,
							DWORD *pFlags, int *pErr);
							

int PMFindNameSpaces(WSAQUERYSETW *pQuery, void *pBuf, int *pcBuf, int *pErr);

int PMAddrConvert(DWORD Op, DWORD AddrFamily, SOCKADDR *pSockAddr, DWORD *pcSockAddr,
	LPWSAPROTOCOL_INFOW pProtInfo, PWSTR psAddr, DWORD *pcAddr, VOID *Unused);

#ifdef __cplusplus
}
#endif

#endif // __PAFDFUNC.H
