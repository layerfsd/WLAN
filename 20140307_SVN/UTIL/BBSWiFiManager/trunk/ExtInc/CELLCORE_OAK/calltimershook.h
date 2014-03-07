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
// Copyright (c) 2001, Microsoft Corp. All rights reserved.
#ifndef _CALLTIMERSHOOK_H_
#define _CALLTIMERSHOOK_H_

#define CCTF_INCOMING   0x00000001
#define CCTF_DATA       0x00000002
#define CCTF_ROAMING    0x00000004

//parameter convention:
// pftCallStart && pftCallEnd must always be non NULL
// pftCallConnect == NULL IFF the call didn't connect
EXTERN_C HRESULT CumulativeCallTimers_CallNotify(DWORD dwCallFlags,
                                                 UINT iLine, 
                                                 const FILETIME* pftCallStart, 
                                                 const FILETIME* pftCallConnect,
                                                 const FILETIME* pftCallEnd);

#endif // _CALLTIMERSHOOK_H_
