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
//  Contains OLE DB 2.6 specification for IRowsetBookmark
//
//----------------------------------------------------------------------------


#ifndef __oledb26x_h__
#define __oledb26x_h__

#include "oledb.h"

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IRowsetBookmark_FWD_DEFINED__
#define __IRowsetBookmark_FWD_DEFINED__
typedef interface IRowsetBookmark IRowsetBookmark;
#endif 	/* __IRowsetBookmark_FWD_DEFINED__ */

enum DBPROPENUM26 {
	DBPROP_IRowsetBookmark = 0x124L
	};

/****************************************
 * Generated header for interface: IRowsetBookmark
 * at Thu Jul 20 09:22:49 2000
 * using MIDL 5.01.0164
 ****************************************/

#ifndef __IRowsetBookmark_INTERFACE_DEFINED__
#define __IRowsetBookmark_INTERFACE_DEFINED__


EXTERN_C const IID IID_IRowsetBookmark;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("0c733ac2-2a1c-11ce-ade5-00aa0044773d")
    IRowsetBookmark : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PositionOnBookmark( 
            /* [in] */ HCHAPTER hChapter,
            /* [in] */ ULONG cbBookmark,
            /* [size_is][in] */ const BYTE __RPC_FAR *pBookmark) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRowsetBookmarkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRowsetBookmark __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRowsetBookmark __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRowsetBookmark __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PositionOnBookmark )( 
            IRowsetBookmark __RPC_FAR * This,
            /* [in] */ HCHAPTER hChapter,
            /* [in] */ ULONG cbBookmark,
            /* [size_is][in] */ const BYTE __RPC_FAR *pBookmark);
        
        END_INTERFACE
    } IRowsetBookmarkVtbl;

    interface IRowsetBookmark
    {
        CONST_VTBL struct IRowsetBookmarkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRowsetBookmark_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRowsetBookmark_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRowsetBookmark_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRowsetBookmark_PositionOnBookmark(This,hChapter,cbBookmark,pBookmark)	\
    (This)->lpVtbl -> PositionOnBookmark(This,hChapter,cbBookmark,pBookmark)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRowsetBookmark_PositionOnBookmark_Proxy( 
    IRowsetBookmark __RPC_FAR * This,
    /* [in] */ HCHAPTER hChapter,
    /* [in] */ ULONG cbBookmark,
    /* [size_is][in] */ const BYTE __RPC_FAR *pBookmark);


void __RPC_STUB IRowsetBookmark_PositionOnBookmark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRowsetBookmark_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
