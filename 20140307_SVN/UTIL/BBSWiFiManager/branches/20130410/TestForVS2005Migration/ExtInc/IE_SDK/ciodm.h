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

#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Tue Feb 05 14:33:31 2002
 */
/* Compiler settings for .\ciodm.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ciodm_h__
#define __ciodm_h__

/* Forward Declarations */ 

#ifndef __IAdminIndexServer_FWD_DEFINED__
#define __IAdminIndexServer_FWD_DEFINED__
typedef interface IAdminIndexServer IAdminIndexServer;
#endif 	/* __IAdminIndexServer_FWD_DEFINED__ */


#ifndef __ICatAdm_FWD_DEFINED__
#define __ICatAdm_FWD_DEFINED__
typedef interface ICatAdm ICatAdm;
#endif 	/* __ICatAdm_FWD_DEFINED__ */


#ifndef __IScopeAdm_FWD_DEFINED__
#define __IScopeAdm_FWD_DEFINED__
typedef interface IScopeAdm IScopeAdm;
#endif 	/* __IScopeAdm_FWD_DEFINED__ */


#ifndef __AdminIndexServer_FWD_DEFINED__
#define __AdminIndexServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminIndexServer AdminIndexServer;
#else
typedef struct AdminIndexServer AdminIndexServer;
#endif /* __cplusplus */

#endif 	/* __AdminIndexServer_FWD_DEFINED__ */


#ifndef __CatAdm_FWD_DEFINED__
#define __CatAdm_FWD_DEFINED__

#ifdef __cplusplus
typedef class CatAdm CatAdm;
#else
typedef struct CatAdm CatAdm;
#endif /* __cplusplus */

#endif 	/* __CatAdm_FWD_DEFINED__ */


#ifndef __ScopeAdm_FWD_DEFINED__
#define __ScopeAdm_FWD_DEFINED__

#ifdef __cplusplus
typedef class ScopeAdm ScopeAdm;
#else
typedef struct ScopeAdm ScopeAdm;
#endif /* __cplusplus */

#endif 	/* __ScopeAdm_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_ciodm_0000 */
/* [local] */ 

typedef /* [public][public][public][public][public][helpstring] */ 
enum __MIDL___MIDL_itf_ciodm_0000_0001
    {	csStopped	= 1,
	csReadOnly	= 2,
	csWritable	= 4
    }	CatalogStateType;



extern RPC_IF_HANDLE __MIDL_itf_ciodm_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ciodm_0000_v0_0_s_ifspec;

#ifndef __IAdminIndexServer_INTERFACE_DEFINED__
#define __IAdminIndexServer_INTERFACE_DEFINED__

/* interface IAdminIndexServer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdminIndexServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3BC4F3A0-652A-11D1-B4D4-00C04FC2DB8D")
    IAdminIndexServer : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MachineName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MachineName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddCatalog( 
            BSTR bstrCatName,
            BSTR bstrCatLocation,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDsip) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveCatalog( 
            BSTR bstrCatName,
            /* [defaultvalue] */ VARIANT_BOOL fDelDirectory = FALSE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCatalogByName( 
            BSTR bstrCatalogName,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pDisp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindFirstCatalog( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindNextCatalog( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCatalog( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsRunning( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsRunning) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableCI( 
            VARIANT_BOOL fAutoStart) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsPaused( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsPaused) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Continue( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLongProperty( 
            BSTR bstrPropName,
            LONG lVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLongProperty( 
            BSTR bstrPropName,
            /* [retval][out] */ LONG __RPC_FAR *plVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSZProperty( 
            BSTR bstrPropName,
            BSTR bstrVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSZProperty( 
            BSTR bstrPropName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminIndexServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAdminIndexServer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAdminIndexServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MachineName )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MachineName )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddCatalog )( 
            IAdminIndexServer __RPC_FAR * This,
            BSTR bstrCatName,
            BSTR bstrCatLocation,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDsip);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveCatalog )( 
            IAdminIndexServer __RPC_FAR * This,
            BSTR bstrCatName,
            /* [defaultvalue] */ VARIANT_BOOL fDelDirectory);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCatalogByName )( 
            IAdminIndexServer __RPC_FAR * This,
            BSTR bstrCatalogName,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindFirstCatalog )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindNextCatalog )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCatalog )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Start )( 
            IAdminIndexServer __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IAdminIndexServer __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsRunning )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsRunning);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableCI )( 
            IAdminIndexServer __RPC_FAR * This,
            VARIANT_BOOL fAutoStart);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pause )( 
            IAdminIndexServer __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPaused )( 
            IAdminIndexServer __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsPaused);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Continue )( 
            IAdminIndexServer __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLongProperty )( 
            IAdminIndexServer __RPC_FAR * This,
            BSTR bstrPropName,
            LONG lVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLongProperty )( 
            IAdminIndexServer __RPC_FAR * This,
            BSTR bstrPropName,
            /* [retval][out] */ LONG __RPC_FAR *plVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSZProperty )( 
            IAdminIndexServer __RPC_FAR * This,
            BSTR bstrPropName,
            BSTR bstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSZProperty )( 
            IAdminIndexServer __RPC_FAR * This,
            BSTR bstrPropName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);
        
        END_INTERFACE
    } IAdminIndexServerVtbl;

    interface IAdminIndexServer
    {
        CONST_VTBL struct IAdminIndexServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminIndexServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminIndexServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminIndexServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminIndexServer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminIndexServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminIndexServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminIndexServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminIndexServer_get_MachineName(This,pVal)	\
    (This)->lpVtbl -> get_MachineName(This,pVal)

#define IAdminIndexServer_put_MachineName(This,newVal)	\
    (This)->lpVtbl -> put_MachineName(This,newVal)

#define IAdminIndexServer_AddCatalog(This,bstrCatName,bstrCatLocation,pIDsip)	\
    (This)->lpVtbl -> AddCatalog(This,bstrCatName,bstrCatLocation,pIDsip)

#define IAdminIndexServer_RemoveCatalog(This,bstrCatName,fDelDirectory)	\
    (This)->lpVtbl -> RemoveCatalog(This,bstrCatName,fDelDirectory)

#define IAdminIndexServer_GetCatalogByName(This,bstrCatalogName,pDisp)	\
    (This)->lpVtbl -> GetCatalogByName(This,bstrCatalogName,pDisp)

#define IAdminIndexServer_FindFirstCatalog(This,pfFound)	\
    (This)->lpVtbl -> FindFirstCatalog(This,pfFound)

#define IAdminIndexServer_FindNextCatalog(This,pfFound)	\
    (This)->lpVtbl -> FindNextCatalog(This,pfFound)

#define IAdminIndexServer_GetCatalog(This,pIDisp)	\
    (This)->lpVtbl -> GetCatalog(This,pIDisp)

#define IAdminIndexServer_Start(This)	\
    (This)->lpVtbl -> Start(This)

#define IAdminIndexServer_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IAdminIndexServer_IsRunning(This,pfIsRunning)	\
    (This)->lpVtbl -> IsRunning(This,pfIsRunning)

#define IAdminIndexServer_EnableCI(This,fAutoStart)	\
    (This)->lpVtbl -> EnableCI(This,fAutoStart)

#define IAdminIndexServer_Pause(This)	\
    (This)->lpVtbl -> Pause(This)

#define IAdminIndexServer_IsPaused(This,pfIsPaused)	\
    (This)->lpVtbl -> IsPaused(This,pfIsPaused)

#define IAdminIndexServer_Continue(This)	\
    (This)->lpVtbl -> Continue(This)

#define IAdminIndexServer_SetLongProperty(This,bstrPropName,lVal)	\
    (This)->lpVtbl -> SetLongProperty(This,bstrPropName,lVal)

#define IAdminIndexServer_GetLongProperty(This,bstrPropName,plVal)	\
    (This)->lpVtbl -> GetLongProperty(This,bstrPropName,plVal)

#define IAdminIndexServer_SetSZProperty(This,bstrPropName,bstrVal)	\
    (This)->lpVtbl -> SetSZProperty(This,bstrPropName,bstrVal)

#define IAdminIndexServer_GetSZProperty(This,bstrPropName,pbstrVal)	\
    (This)->lpVtbl -> GetSZProperty(This,bstrPropName,pbstrVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_get_MachineName_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IAdminIndexServer_get_MachineName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_put_MachineName_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IAdminIndexServer_put_MachineName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_AddCatalog_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    BSTR bstrCatName,
    BSTR bstrCatLocation,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDsip);


void __RPC_STUB IAdminIndexServer_AddCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_RemoveCatalog_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    BSTR bstrCatName,
    /* [defaultvalue] */ VARIANT_BOOL fDelDirectory);


void __RPC_STUB IAdminIndexServer_RemoveCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_GetCatalogByName_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    BSTR bstrCatalogName,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pDisp);


void __RPC_STUB IAdminIndexServer_GetCatalogByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_FindFirstCatalog_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);


void __RPC_STUB IAdminIndexServer_FindFirstCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_FindNextCatalog_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);


void __RPC_STUB IAdminIndexServer_FindNextCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_GetCatalog_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);


void __RPC_STUB IAdminIndexServer_GetCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_Start_Proxy( 
    IAdminIndexServer __RPC_FAR * This);


void __RPC_STUB IAdminIndexServer_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_Stop_Proxy( 
    IAdminIndexServer __RPC_FAR * This);


void __RPC_STUB IAdminIndexServer_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_IsRunning_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsRunning);


void __RPC_STUB IAdminIndexServer_IsRunning_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_EnableCI_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    VARIANT_BOOL fAutoStart);


void __RPC_STUB IAdminIndexServer_EnableCI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_Pause_Proxy( 
    IAdminIndexServer __RPC_FAR * This);


void __RPC_STUB IAdminIndexServer_Pause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_IsPaused_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsPaused);


void __RPC_STUB IAdminIndexServer_IsPaused_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_Continue_Proxy( 
    IAdminIndexServer __RPC_FAR * This);


void __RPC_STUB IAdminIndexServer_Continue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_SetLongProperty_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    BSTR bstrPropName,
    LONG lVal);


void __RPC_STUB IAdminIndexServer_SetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_GetLongProperty_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    BSTR bstrPropName,
    /* [retval][out] */ LONG __RPC_FAR *plVal);


void __RPC_STUB IAdminIndexServer_GetLongProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_SetSZProperty_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    BSTR bstrPropName,
    BSTR bstrVal);


void __RPC_STUB IAdminIndexServer_SetSZProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminIndexServer_GetSZProperty_Proxy( 
    IAdminIndexServer __RPC_FAR * This,
    BSTR bstrPropName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);


void __RPC_STUB IAdminIndexServer_GetSZProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminIndexServer_INTERFACE_DEFINED__ */


#ifndef __ICatAdm_INTERFACE_DEFINED__
#define __ICatAdm_INTERFACE_DEFINED__

/* interface ICatAdm */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICatAdm;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3BC4F3A2-652A-11D1-B4D4-00C04FC2DB8D")
    ICatAdm : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ForceMasterMerge( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddScope( 
            BSTR bstrScopeName,
            VARIANT_BOOL fExclude,
            /* [optional][in] */ VARIANT vtLogon,
            /* [optional][in] */ VARIANT vtPassword,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveScope( 
            BSTR bstrScopePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScopeByPath( 
            BSTR bstrPath,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScopeByAlias( 
            BSTR bstrAlias,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindFirstScope( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindNextScope( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScope( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CatalogName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CatalogLocation( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WordListCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PersistentIndexCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_QueryCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DocumentsToFilter( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FreshTestCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PctMergeComplete( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FilteredDocumentCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TotalDocumentCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PendingScanCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IndexSize( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UniqueKeyCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_StateInfo( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsUpToDate( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DelayedFilterCount( 
            /* [retval][out] */ LONG __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartCatalog( 
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopCatalog( 
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PauseCatalog( 
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ContinueCatalog( 
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsCatalogRunning( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsRunning) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsCatalogPaused( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsPaused) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsCatalogStopped( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pIsStopped) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICatAdmVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICatAdm __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICatAdm __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICatAdm __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICatAdm __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICatAdm __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICatAdm __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICatAdm __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ForceMasterMerge )( 
            ICatAdm __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddScope )( 
            ICatAdm __RPC_FAR * This,
            BSTR bstrScopeName,
            VARIANT_BOOL fExclude,
            /* [optional][in] */ VARIANT vtLogon,
            /* [optional][in] */ VARIANT vtPassword,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveScope )( 
            ICatAdm __RPC_FAR * This,
            BSTR bstrScopePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetScopeByPath )( 
            ICatAdm __RPC_FAR * This,
            BSTR bstrPath,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetScopeByAlias )( 
            ICatAdm __RPC_FAR * This,
            BSTR bstrAlias,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindFirstScope )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindNextScope )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetScope )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CatalogName )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CatalogLocation )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WordListCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PersistentIndexCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_QueryCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DocumentsToFilter )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FreshTestCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PctMergeComplete )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FilteredDocumentCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TotalDocumentCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PendingScanCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IndexSize )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UniqueKeyCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StateInfo )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsUpToDate )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DelayedFilterCount )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartCatalog )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopCatalog )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PauseCatalog )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ContinueCatalog )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsCatalogRunning )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsRunning);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsCatalogPaused )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsPaused);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsCatalogStopped )( 
            ICatAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pIsStopped);
        
        END_INTERFACE
    } ICatAdmVtbl;

    interface ICatAdm
    {
        CONST_VTBL struct ICatAdmVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICatAdm_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICatAdm_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICatAdm_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICatAdm_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICatAdm_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICatAdm_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICatAdm_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICatAdm_ForceMasterMerge(This)	\
    (This)->lpVtbl -> ForceMasterMerge(This)

#define ICatAdm_AddScope(This,bstrScopeName,fExclude,vtLogon,vtPassword,pIDisp)	\
    (This)->lpVtbl -> AddScope(This,bstrScopeName,fExclude,vtLogon,vtPassword,pIDisp)

#define ICatAdm_RemoveScope(This,bstrScopePath)	\
    (This)->lpVtbl -> RemoveScope(This,bstrScopePath)

#define ICatAdm_GetScopeByPath(This,bstrPath,pIDisp)	\
    (This)->lpVtbl -> GetScopeByPath(This,bstrPath,pIDisp)

#define ICatAdm_GetScopeByAlias(This,bstrAlias,pIDisp)	\
    (This)->lpVtbl -> GetScopeByAlias(This,bstrAlias,pIDisp)

#define ICatAdm_FindFirstScope(This,pfFound)	\
    (This)->lpVtbl -> FindFirstScope(This,pfFound)

#define ICatAdm_FindNextScope(This,pfFound)	\
    (This)->lpVtbl -> FindNextScope(This,pfFound)

#define ICatAdm_GetScope(This,pIDisp)	\
    (This)->lpVtbl -> GetScope(This,pIDisp)

#define ICatAdm_get_CatalogName(This,pVal)	\
    (This)->lpVtbl -> get_CatalogName(This,pVal)

#define ICatAdm_get_CatalogLocation(This,pVal)	\
    (This)->lpVtbl -> get_CatalogLocation(This,pVal)

#define ICatAdm_get_WordListCount(This,pVal)	\
    (This)->lpVtbl -> get_WordListCount(This,pVal)

#define ICatAdm_get_PersistentIndexCount(This,pVal)	\
    (This)->lpVtbl -> get_PersistentIndexCount(This,pVal)

#define ICatAdm_get_QueryCount(This,pVal)	\
    (This)->lpVtbl -> get_QueryCount(This,pVal)

#define ICatAdm_get_DocumentsToFilter(This,pVal)	\
    (This)->lpVtbl -> get_DocumentsToFilter(This,pVal)

#define ICatAdm_get_FreshTestCount(This,pVal)	\
    (This)->lpVtbl -> get_FreshTestCount(This,pVal)

#define ICatAdm_get_PctMergeComplete(This,pVal)	\
    (This)->lpVtbl -> get_PctMergeComplete(This,pVal)

#define ICatAdm_get_FilteredDocumentCount(This,pVal)	\
    (This)->lpVtbl -> get_FilteredDocumentCount(This,pVal)

#define ICatAdm_get_TotalDocumentCount(This,pVal)	\
    (This)->lpVtbl -> get_TotalDocumentCount(This,pVal)

#define ICatAdm_get_PendingScanCount(This,pVal)	\
    (This)->lpVtbl -> get_PendingScanCount(This,pVal)

#define ICatAdm_get_IndexSize(This,pVal)	\
    (This)->lpVtbl -> get_IndexSize(This,pVal)

#define ICatAdm_get_UniqueKeyCount(This,pVal)	\
    (This)->lpVtbl -> get_UniqueKeyCount(This,pVal)

#define ICatAdm_get_StateInfo(This,pVal)	\
    (This)->lpVtbl -> get_StateInfo(This,pVal)

#define ICatAdm_get_IsUpToDate(This,pVal)	\
    (This)->lpVtbl -> get_IsUpToDate(This,pVal)

#define ICatAdm_get_DelayedFilterCount(This,pVal)	\
    (This)->lpVtbl -> get_DelayedFilterCount(This,pVal)

#define ICatAdm_StartCatalog(This,pdwOldState)	\
    (This)->lpVtbl -> StartCatalog(This,pdwOldState)

#define ICatAdm_StopCatalog(This,pdwOldState)	\
    (This)->lpVtbl -> StopCatalog(This,pdwOldState)

#define ICatAdm_PauseCatalog(This,pdwOldState)	\
    (This)->lpVtbl -> PauseCatalog(This,pdwOldState)

#define ICatAdm_ContinueCatalog(This,pdwOldState)	\
    (This)->lpVtbl -> ContinueCatalog(This,pdwOldState)

#define ICatAdm_IsCatalogRunning(This,pfIsRunning)	\
    (This)->lpVtbl -> IsCatalogRunning(This,pfIsRunning)

#define ICatAdm_IsCatalogPaused(This,pfIsPaused)	\
    (This)->lpVtbl -> IsCatalogPaused(This,pfIsPaused)

#define ICatAdm_IsCatalogStopped(This,pIsStopped)	\
    (This)->lpVtbl -> IsCatalogStopped(This,pIsStopped)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_ForceMasterMerge_Proxy( 
    ICatAdm __RPC_FAR * This);


void __RPC_STUB ICatAdm_ForceMasterMerge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_AddScope_Proxy( 
    ICatAdm __RPC_FAR * This,
    BSTR bstrScopeName,
    VARIANT_BOOL fExclude,
    /* [optional][in] */ VARIANT vtLogon,
    /* [optional][in] */ VARIANT vtPassword,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);


void __RPC_STUB ICatAdm_AddScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_RemoveScope_Proxy( 
    ICatAdm __RPC_FAR * This,
    BSTR bstrScopePath);


void __RPC_STUB ICatAdm_RemoveScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_GetScopeByPath_Proxy( 
    ICatAdm __RPC_FAR * This,
    BSTR bstrPath,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);


void __RPC_STUB ICatAdm_GetScopeByPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_GetScopeByAlias_Proxy( 
    ICatAdm __RPC_FAR * This,
    BSTR bstrAlias,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);


void __RPC_STUB ICatAdm_GetScopeByAlias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_FindFirstScope_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);


void __RPC_STUB ICatAdm_FindFirstScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_FindNextScope_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfFound);


void __RPC_STUB ICatAdm_FindNextScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_GetScope_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *pIDisp);


void __RPC_STUB ICatAdm_GetScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_CatalogName_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_CatalogName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_CatalogLocation_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_CatalogLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_WordListCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_WordListCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_PersistentIndexCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_PersistentIndexCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_QueryCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_QueryCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_DocumentsToFilter_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_DocumentsToFilter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_FreshTestCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_FreshTestCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_PctMergeComplete_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_PctMergeComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_FilteredDocumentCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_FilteredDocumentCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_TotalDocumentCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_TotalDocumentCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_PendingScanCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_PendingScanCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_IndexSize_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_IndexSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_UniqueKeyCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_UniqueKeyCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_StateInfo_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_StateInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_IsUpToDate_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_IsUpToDate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICatAdm_get_DelayedFilterCount_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pVal);


void __RPC_STUB ICatAdm_get_DelayedFilterCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_StartCatalog_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);


void __RPC_STUB ICatAdm_StartCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_StopCatalog_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);


void __RPC_STUB ICatAdm_StopCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_PauseCatalog_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);


void __RPC_STUB ICatAdm_PauseCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_ContinueCatalog_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ CatalogStateType __RPC_FAR *pdwOldState);


void __RPC_STUB ICatAdm_ContinueCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_IsCatalogRunning_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsRunning);


void __RPC_STUB ICatAdm_IsCatalogRunning_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_IsCatalogPaused_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfIsPaused);


void __RPC_STUB ICatAdm_IsCatalogPaused_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICatAdm_IsCatalogStopped_Proxy( 
    ICatAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pIsStopped);


void __RPC_STUB ICatAdm_IsCatalogStopped_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICatAdm_INTERFACE_DEFINED__ */


#ifndef __IScopeAdm_INTERFACE_DEFINED__
#define __IScopeAdm_INTERFACE_DEFINED__

/* interface IScopeAdm */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IScopeAdm;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3BC4F3A4-652A-11D1-B4D4-00C04FC2DB8D")
    IScopeAdm : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Rescan( 
            VARIANT_BOOL fFull) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetLogonInfo( 
            BSTR bstrLogon,
            BSTR bstrPassword) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Path( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Alias( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Alias( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExcludeScope( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ExcludeScope( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VirtualScope( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Logon( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScopeAdmVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScopeAdm __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScopeAdm __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScopeAdm __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScopeAdm __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScopeAdm __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScopeAdm __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScopeAdm __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Rescan )( 
            IScopeAdm __RPC_FAR * This,
            VARIANT_BOOL fFull);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLogonInfo )( 
            IScopeAdm __RPC_FAR * This,
            BSTR bstrLogon,
            BSTR bstrPassword);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Path )( 
            IScopeAdm __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Path )( 
            IScopeAdm __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IScopeAdm __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IScopeAdm __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExcludeScope )( 
            IScopeAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ExcludeScope )( 
            IScopeAdm __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VirtualScope )( 
            IScopeAdm __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Logon )( 
            IScopeAdm __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IScopeAdmVtbl;

    interface IScopeAdm
    {
        CONST_VTBL struct IScopeAdmVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScopeAdm_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScopeAdm_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScopeAdm_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScopeAdm_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScopeAdm_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScopeAdm_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScopeAdm_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScopeAdm_Rescan(This,fFull)	\
    (This)->lpVtbl -> Rescan(This,fFull)

#define IScopeAdm_SetLogonInfo(This,bstrLogon,bstrPassword)	\
    (This)->lpVtbl -> SetLogonInfo(This,bstrLogon,bstrPassword)

#define IScopeAdm_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define IScopeAdm_put_Path(This,newVal)	\
    (This)->lpVtbl -> put_Path(This,newVal)

#define IScopeAdm_get_Alias(This,pVal)	\
    (This)->lpVtbl -> get_Alias(This,pVal)

#define IScopeAdm_put_Alias(This,newVal)	\
    (This)->lpVtbl -> put_Alias(This,newVal)

#define IScopeAdm_get_ExcludeScope(This,pVal)	\
    (This)->lpVtbl -> get_ExcludeScope(This,pVal)

#define IScopeAdm_put_ExcludeScope(This,newVal)	\
    (This)->lpVtbl -> put_ExcludeScope(This,newVal)

#define IScopeAdm_get_VirtualScope(This,pVal)	\
    (This)->lpVtbl -> get_VirtualScope(This,pVal)

#define IScopeAdm_get_Logon(This,pVal)	\
    (This)->lpVtbl -> get_Logon(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IScopeAdm_Rescan_Proxy( 
    IScopeAdm __RPC_FAR * This,
    VARIANT_BOOL fFull);


void __RPC_STUB IScopeAdm_Rescan_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IScopeAdm_SetLogonInfo_Proxy( 
    IScopeAdm __RPC_FAR * This,
    BSTR bstrLogon,
    BSTR bstrPassword);


void __RPC_STUB IScopeAdm_SetLogonInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScopeAdm_get_Path_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IScopeAdm_get_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IScopeAdm_put_Path_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IScopeAdm_put_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScopeAdm_get_Alias_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IScopeAdm_get_Alias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IScopeAdm_put_Alias_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IScopeAdm_put_Alias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScopeAdm_get_ExcludeScope_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IScopeAdm_get_ExcludeScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IScopeAdm_put_ExcludeScope_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IScopeAdm_put_ExcludeScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScopeAdm_get_VirtualScope_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IScopeAdm_get_VirtualScope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScopeAdm_get_Logon_Proxy( 
    IScopeAdm __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IScopeAdm_get_Logon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScopeAdm_INTERFACE_DEFINED__ */



#ifndef __CIODMLib_LIBRARY_DEFINED__
#define __CIODMLib_LIBRARY_DEFINED__

/* library CIODMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CIODMLib;

EXTERN_C const CLSID CLSID_AdminIndexServer;

#ifdef __cplusplus

class DECLSPEC_UUID("3BC4F3A1-652A-11D1-B4D4-00C04FC2DB8D")
AdminIndexServer;
#endif

EXTERN_C const CLSID CLSID_CatAdm;

#ifdef __cplusplus

class DECLSPEC_UUID("3BC4F3A3-652A-11D1-B4D4-00C04FC2DB8D")
CatAdm;
#endif

EXTERN_C const CLSID CLSID_ScopeAdm;

#ifdef __cplusplus

class DECLSPEC_UUID("3BC4F3A7-652A-11D1-B4D4-00C04FC2DB8D")
ScopeAdm;
#endif
#endif /* __CIODMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


