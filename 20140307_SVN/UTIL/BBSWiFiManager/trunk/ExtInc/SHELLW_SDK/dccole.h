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

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    dccole.h

Abstract:

    This file defines the OLE interface to the DCCMAN module (Desktop Only)
    
Environment:

    User Mode - Win32

--*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// {A7B88840-A812-11cf-8011-00A0C90A8F78}
DEFINE_GUID(IID_IDccManSink, 
0xa7b88840, 0xa812, 0x11cf, 0x80, 0x11, 0x0, 0xa0, 0xc9, 0xa, 0x8f, 0x78);
// {A7B88841-A812-11cf-8011-00A0C90A8F78}
DEFINE_GUID(IID_IDccMan, 
0xa7b88841, 0xa812, 0x11cf, 0x80, 0x11, 0x0, 0xa0, 0xc9, 0xa, 0x8f, 0x78);
// {499C0C20-A766-11cf-8011-00A0C90A8F78}
DEFINE_GUID(CLSID_DccMan, 
0x499c0c20, 0xa766, 0x11cf, 0x80, 0x11, 0x0, 0xa0, 0xc9, 0xa, 0x8f, 0x78);

#ifndef _DCCOLEH_
#define _DCCOLEH_

//============================== OLE INTERFACE ===================================

#undef  INTERFACE
#define INTERFACE   IDccManSink

DECLARE_INTERFACE_ (IDccManSink,  IUnknown)
{
 	// These methods corespond to GW_LOG messages generated by the Win95 DCC
    STDMETHOD(OnLogIpAddr) (THIS_ DWORD dwIpAddr) PURE;
    STDMETHOD(OnLogTerminated) (THIS) PURE;
    STDMETHOD(OnLogActive) (THIS) PURE;
	STDMETHOD(OnLogInactive) (THIS) PURE;
	STDMETHOD(OnLogAnswered) (THIS) PURE;
	STDMETHOD(OnLogListen) (THIS) PURE;
	STDMETHOD(OnLogDisconnection) (THIS) PURE;
	STDMETHOD(OnLogError) (THIS) PURE;
};
typedef IDccManSink *LPDCCMANSINK;

#undef  INTERFACE
#define INTERFACE   IDccMan

DECLARE_INTERFACE_ (IDccMan,  IUnknown)
{
    STDMETHOD(Advise) (THIS_
		IN	IDccManSink * pDccSink,	// The advise sink that is requesting notification
		OUT DWORD * pdwContext		// Identifies the context for future calls to the Unadvise method
	) PURE;
	
	STDMETHOD(Unadvise) (THIS_
		DWORD dwContext				// As returned by Advise()
	) PURE;
   
	STDMETHOD(ShowCommSettings) (THIS) PURE;	// Displays the Communication Property Sheet on the screen
												// If a connection is active, the sheet is in read-only mode
	STDMETHOD(AutoconnectEnable) (THIS) PURE;
	STDMETHOD(AutoconnectDisable) (THIS) PURE;

	STDMETHOD(ConnectNow) (THIS) PURE;			// Active only when Autoconnect is Disabled
	STDMETHOD(DisconnectNow) (THIS) PURE;		// Active only when Autoconnect is Disabled
	
	STDMETHOD(SetIconDataTransferring) (THIS) PURE;
	STDMETHOD(SetIconNoDataTransferring) (THIS) PURE;
	STDMETHOD(SetIconError) (THIS) PURE;
};
typedef IDccMan *LPDCCMAN;


#endif /* end, ifdef _DCCOLEH_  */

#ifdef __cplusplus
}
#endif  /* __cplusplus */
