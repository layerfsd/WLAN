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
//+---------------------------------------------------------------------------
//
//  Microsoft OLE DB CE
//  Copyright (C) Microsoft Corporation, 2000
//
//  Contains specification for IRowsetPosition interface.  This interface
//  is pending approval for inclusion into the OLE DB specification.
//
//----------------------------------------------------------------------------

#ifndef __oledbirp_h__
#define __oledbirp_h__

#include "oledb.h"

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IRowsetPosition_FWD_DEFINED__
#define __IRowsetPosition_FWD_DEFINED__
typedef interface IRowsetPosition IRowsetPosition;
#endif 	/* __IRowsetPosition_FWD_DEFINED__ */


#ifndef __IRowsetPosition_INTERFACE_DEFINED__
#define __IRowsetPosition_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IRowsetPosition
 * at Thu Jul 20 09:22:49 2000
 * using MIDL 5.01.0164
 ****************************************/


EXTERN_C const IID IID_IRowsetPosition;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("c19f4b47-ab5e-49a9-9f2e-ab7ab5c55914")
    IRowsetPosition : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRecordCount( 
            /* [in] */ HCHAPTER hChapter,
            /* [out] */ ULONG __RPC_FAR *pcRows) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentPosition( 
            /* [in] */ HCHAPTER hChapter,
            /* [out] */ ULONG __RPC_FAR *pulPosition) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRowsetPositionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRowsetPosition __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRowsetPosition __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRowsetPosition __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRecordCount )( 
            IRowsetPosition __RPC_FAR * This,
            /* [in] */ HCHAPTER hChapter,
            /* [out] */ ULONG __RPC_FAR *pcRows);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentPosition )( 
            IRowsetPosition __RPC_FAR * This,
            /* [in] */ HCHAPTER hChapter,
            /* [out] */ ULONG __RPC_FAR *pulPosition);
        
        END_INTERFACE
    } IRowsetPositionVtbl;

    interface IRowsetPosition
    {
        CONST_VTBL struct IRowsetPositionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRowsetPosition_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRowsetPosition_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRowsetPosition_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRowsetPosition_GetRecordCount(This,hChapter,pcRows)	\
    (This)->lpVtbl -> GetRecordCount(This,hChapter,pcRows)

#define IRowsetPosition_GetCurrentPosition(This,hChapter,pulPosition)	\
    (This)->lpVtbl -> GetCurrentPosition(This,hChapter,pulPosition)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRowsetPosition_GetRecordCount_Proxy( 
    IRowsetPosition __RPC_FAR * This,
    /* [in] */ HCHAPTER hChapter,
    /* [out] */ ULONG __RPC_FAR *pcRows);


void __RPC_STUB IRowsetPosition_GetRecordCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRowsetPosition_GetCurrentPosition_Proxy( 
    IRowsetPosition __RPC_FAR * This,
    /* [in] */ HCHAPTER hChapter,
    /* [out] */ ULONG __RPC_FAR *pulPosition);


void __RPC_STUB IRowsetPosition_GetCurrentPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


#endif 	/* __IRowsetPosition_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
