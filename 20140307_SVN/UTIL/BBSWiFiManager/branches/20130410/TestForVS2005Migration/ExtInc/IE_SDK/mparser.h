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
/* at Tue Feb 05 14:33:30 2002
 */
/* Compiler settings for .\mparser.idl:
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

#ifndef __mparser_h__
#define __mparser_h__

/* Forward Declarations */ 

#ifndef __IParserVerify_FWD_DEFINED__
#define __IParserVerify_FWD_DEFINED__
typedef interface IParserVerify IParserVerify;
#endif 	/* __IParserVerify_FWD_DEFINED__ */


#ifndef __IParserTreeProperties_FWD_DEFINED__
#define __IParserTreeProperties_FWD_DEFINED__
typedef interface IParserTreeProperties IParserTreeProperties;
#endif 	/* __IParserTreeProperties_FWD_DEFINED__ */


#ifndef __IParserSession_FWD_DEFINED__
#define __IParserSession_FWD_DEFINED__
typedef interface IParserSession IParserSession;
#endif 	/* __IParserSession_FWD_DEFINED__ */


#ifndef __IParser_FWD_DEFINED__
#define __IParser_FWD_DEFINED__
typedef interface IParser IParser;
#endif 	/* __IParser_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_mparser_0000 */
/* [local] */ 

//
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
//+---------------------------------------------------------------------------
//
//  Microsoft Content Index SQL Parser
//  Copyright (C) Microsoft Corporation, 1997 - 1998.
//
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//
//  Function Prototypes 
//
//----------------------------------------------------------------------------
 HRESULT __stdcall MakeIParser(IParser** ppIParser); 



enum PTPROPS
    {	PTPROPS_SCOPE	= 1,
	PTPROPS_DEPTH	= PTPROPS_SCOPE + 1,
	PTPROPS_CATALOG	= PTPROPS_DEPTH + 1,
	PTPROPS_MACHINE	= PTPROPS_CATALOG + 1,
	PTPROPS_CIRESTRICTION	= PTPROPS_MACHINE + 1,
	PTPROPS_ERR_IDS	= PTPROPS_CIRESTRICTION + 1,
	PTPROPS_ERR_HR	= PTPROPS_ERR_IDS + 1,
	PTPROPS_ERR_DISPPARAM	= PTPROPS_ERR_HR + 1
    };
#ifdef DBINITCONSTANTS
extern const GUID DBGUID_MSSQLTEXT = {0x03fb9b70,0x7fba,0x11d0,{0xa2,0x59,0x00,0x80,0xc7,0x8c,0x88,0x95}};
extern const GUID DBGUID_MSSQLJAWS = {0xe4ab8511,0x4a52,0x11d1,{0xb6,0xd5,0x00,0xc0,0x4f,0xd6,0x11,0xd0}};
#else // !DBINITCONSTANTS
extern const GUID DBGUID_MSSQLTEXT;
extern const GUID DBGUID_MSSQLJAWS;
#endif // DBINITCONSTANTS
typedef struct tagDBCOMMANDTREE DBCOMMANDTREE;




extern RPC_IF_HANDLE __MIDL_itf_mparser_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mparser_0000_v0_0_s_ifspec;

#ifndef __IParserVerify_INTERFACE_DEFINED__
#define __IParserVerify_INTERFACE_DEFINED__

/* interface IParserVerify */
/* [unique][uuid][object][local] */ 


EXTERN_C const IID IID_IParserVerify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("186442B3-472E-11d1-8952-00C04FD611D7")
    IParserVerify : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE VerifyMachine( 
            /* [in] */ LPCWSTR pcwszMachine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE VerifyCatalog( 
            /* [in] */ LPCWSTR pcwszMachine,
            /* [in] */ LPCWSTR pcwszCatalog) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParserVerifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParserVerify __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParserVerify __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParserVerify __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifyMachine )( 
            IParserVerify __RPC_FAR * This,
            /* [in] */ LPCWSTR pcwszMachine);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifyCatalog )( 
            IParserVerify __RPC_FAR * This,
            /* [in] */ LPCWSTR pcwszMachine,
            /* [in] */ LPCWSTR pcwszCatalog);
        
        END_INTERFACE
    } IParserVerifyVtbl;

    interface IParserVerify
    {
        CONST_VTBL struct IParserVerifyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParserVerify_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParserVerify_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParserVerify_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParserVerify_VerifyMachine(This,pcwszMachine)	\
    (This)->lpVtbl -> VerifyMachine(This,pcwszMachine)

#define IParserVerify_VerifyCatalog(This,pcwszMachine,pcwszCatalog)	\
    (This)->lpVtbl -> VerifyCatalog(This,pcwszMachine,pcwszCatalog)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IParserVerify_VerifyMachine_Proxy( 
    IParserVerify __RPC_FAR * This,
    /* [in] */ LPCWSTR pcwszMachine);


void __RPC_STUB IParserVerify_VerifyMachine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IParserVerify_VerifyCatalog_Proxy( 
    IParserVerify __RPC_FAR * This,
    /* [in] */ LPCWSTR pcwszMachine,
    /* [in] */ LPCWSTR pcwszCatalog);


void __RPC_STUB IParserVerify_VerifyCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParserVerify_INTERFACE_DEFINED__ */


#ifndef __IParserTreeProperties_INTERFACE_DEFINED__
#define __IParserTreeProperties_INTERFACE_DEFINED__

/* interface IParserTreeProperties */
/* [unique][uuid][object][local] */ 


EXTERN_C const IID IID_IParserTreeProperties;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("186442B2-472E-11d1-8952-00C04FD611D7")
    IParserTreeProperties : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetProperties( 
            /* [in] */ ULONG eParseProp,
            /* [out][in] */ VARIANT __RPC_FAR *vParseProp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParserTreePropertiesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParserTreeProperties __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParserTreeProperties __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParserTreeProperties __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperties )( 
            IParserTreeProperties __RPC_FAR * This,
            /* [in] */ ULONG eParseProp,
            /* [out][in] */ VARIANT __RPC_FAR *vParseProp);
        
        END_INTERFACE
    } IParserTreePropertiesVtbl;

    interface IParserTreeProperties
    {
        CONST_VTBL struct IParserTreePropertiesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParserTreeProperties_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParserTreeProperties_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParserTreeProperties_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParserTreeProperties_GetProperties(This,eParseProp,vParseProp)	\
    (This)->lpVtbl -> GetProperties(This,eParseProp,vParseProp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IParserTreeProperties_GetProperties_Proxy( 
    IParserTreeProperties __RPC_FAR * This,
    /* [in] */ ULONG eParseProp,
    /* [out][in] */ VARIANT __RPC_FAR *vParseProp);


void __RPC_STUB IParserTreeProperties_GetProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParserTreeProperties_INTERFACE_DEFINED__ */


#ifndef __IParserSession_INTERFACE_DEFINED__
#define __IParserSession_INTERFACE_DEFINED__

/* interface IParserSession */
/* [unique][uuid][object][local] */ 


EXTERN_C const IID IID_IParserSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("186442B1-472E-11d1-8952-00C04FD611D7")
    IParserSession : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ToTree( 
            /* [in] */ LCID lcidVal,
            /* [in] */ LPCWSTR pcwszText,
            /* [out] */ DBCOMMANDTREE __RPC_FAR *__RPC_FAR *ppTree,
            /* [out] */ IParserTreeProperties __RPC_FAR *__RPC_FAR *ppPTProperties) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FreeTree( 
            /* [out][in] */ DBCOMMANDTREE __RPC_FAR *__RPC_FAR *ppTree) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCatalog( 
            /* [in] */ LPCWSTR pcwszCatalog) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParserSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParserSession __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParserSession __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParserSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToTree )( 
            IParserSession __RPC_FAR * This,
            /* [in] */ LCID lcidVal,
            /* [in] */ LPCWSTR pcwszText,
            /* [out] */ DBCOMMANDTREE __RPC_FAR *__RPC_FAR *ppTree,
            /* [out] */ IParserTreeProperties __RPC_FAR *__RPC_FAR *ppPTProperties);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FreeTree )( 
            IParserSession __RPC_FAR * This,
            /* [out][in] */ DBCOMMANDTREE __RPC_FAR *__RPC_FAR *ppTree);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCatalog )( 
            IParserSession __RPC_FAR * This,
            /* [in] */ LPCWSTR pcwszCatalog);
        
        END_INTERFACE
    } IParserSessionVtbl;

    interface IParserSession
    {
        CONST_VTBL struct IParserSessionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParserSession_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParserSession_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParserSession_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParserSession_ToTree(This,lcidVal,pcwszText,ppTree,ppPTProperties)	\
    (This)->lpVtbl -> ToTree(This,lcidVal,pcwszText,ppTree,ppPTProperties)

#define IParserSession_FreeTree(This,ppTree)	\
    (This)->lpVtbl -> FreeTree(This,ppTree)

#define IParserSession_SetCatalog(This,pcwszCatalog)	\
    (This)->lpVtbl -> SetCatalog(This,pcwszCatalog)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IParserSession_ToTree_Proxy( 
    IParserSession __RPC_FAR * This,
    /* [in] */ LCID lcidVal,
    /* [in] */ LPCWSTR pcwszText,
    /* [out] */ DBCOMMANDTREE __RPC_FAR *__RPC_FAR *ppTree,
    /* [out] */ IParserTreeProperties __RPC_FAR *__RPC_FAR *ppPTProperties);


void __RPC_STUB IParserSession_ToTree_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IParserSession_FreeTree_Proxy( 
    IParserSession __RPC_FAR * This,
    /* [out][in] */ DBCOMMANDTREE __RPC_FAR *__RPC_FAR *ppTree);


void __RPC_STUB IParserSession_FreeTree_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IParserSession_SetCatalog_Proxy( 
    IParserSession __RPC_FAR * This,
    /* [in] */ LPCWSTR pcwszCatalog);


void __RPC_STUB IParserSession_SetCatalog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParserSession_INTERFACE_DEFINED__ */


#ifndef __IParser_INTERFACE_DEFINED__
#define __IParser_INTERFACE_DEFINED__

/* interface IParser */
/* [unique][uuid][object][local] */ 


EXTERN_C const IID IID_IParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("186442B0-472E-11d1-8952-00C04FD611D7")
    IParser : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateSession( 
            /* [in] */ const GUID __RPC_FAR *pguidDialect,
            /* [in] */ LPCWSTR pwszMachine,
            /* [in] */ IParserVerify __RPC_FAR *ppIParserVerfiy,
            /* [in] */ IColumnMapperCreator __RPC_FAR *pIColMapCreator,
            /* [out] */ IParserSession __RPC_FAR *__RPC_FAR *ppIParserSession) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSession )( 
            IParser __RPC_FAR * This,
            /* [in] */ const GUID __RPC_FAR *pguidDialect,
            /* [in] */ LPCWSTR pwszMachine,
            /* [in] */ IParserVerify __RPC_FAR *ppIParserVerfiy,
            /* [in] */ IColumnMapperCreator __RPC_FAR *pIColMapCreator,
            /* [out] */ IParserSession __RPC_FAR *__RPC_FAR *ppIParserSession);
        
        END_INTERFACE
    } IParserVtbl;

    interface IParser
    {
        CONST_VTBL struct IParserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParser_CreateSession(This,pguidDialect,pwszMachine,ppIParserVerfiy,pIColMapCreator,ppIParserSession)	\
    (This)->lpVtbl -> CreateSession(This,pguidDialect,pwszMachine,ppIParserVerfiy,pIColMapCreator,ppIParserSession)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IParser_CreateSession_Proxy( 
    IParser __RPC_FAR * This,
    /* [in] */ const GUID __RPC_FAR *pguidDialect,
    /* [in] */ LPCWSTR pwszMachine,
    /* [in] */ IParserVerify __RPC_FAR *ppIParserVerfiy,
    /* [in] */ IColumnMapperCreator __RPC_FAR *pIColMapCreator,
    /* [out] */ IParserSession __RPC_FAR *__RPC_FAR *ppIParserSession);


void __RPC_STUB IParser_CreateSession_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParser_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


