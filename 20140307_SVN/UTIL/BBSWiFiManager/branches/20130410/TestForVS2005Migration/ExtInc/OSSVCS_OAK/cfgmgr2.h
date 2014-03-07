
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:22:07 2008
 */
/* Compiler settings for .\cfgmgr2.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __cfgmgr2_h__
#define __cfgmgr2_h__

/* Forward Declarations */ 

#ifndef __IConfigManager2_FWD_DEFINED__
#define __IConfigManager2_FWD_DEFINED__
typedef interface IConfigManager2 IConfigManager2;
#endif 	/* __IConfigManager2_FWD_DEFINED__ */


#ifndef __IConfigNode_FWD_DEFINED__
#define __IConfigNode_FWD_DEFINED__
typedef interface IConfigNode IConfigNode;
#endif 	/* __IConfigNode_FWD_DEFINED__ */


#ifndef __IEnumConfigNodeProperties_FWD_DEFINED__
#define __IEnumConfigNodeProperties_FWD_DEFINED__
typedef interface IEnumConfigNodeProperties IEnumConfigNodeProperties;
#endif 	/* __IEnumConfigNodeProperties_FWD_DEFINED__ */


#ifndef __IEnumConfigNodes_FWD_DEFINED__
#define __IEnumConfigNodes_FWD_DEFINED__
typedef interface IEnumConfigNodes IEnumConfigNodes;
#endif 	/* __IEnumConfigNodes_FWD_DEFINED__ */


#ifndef __IEnumCommandStates_FWD_DEFINED__
#define __IEnumCommandStates_FWD_DEFINED__
typedef interface IEnumCommandStates IEnumCommandStates;
#endif 	/* __IEnumCommandStates_FWD_DEFINED__ */


#ifndef __ConfigManager2_FWD_DEFINED__
#define __ConfigManager2_FWD_DEFINED__

#ifdef __cplusplus
typedef class ConfigManager2 ConfigManager2;
#else
typedef struct ConfigManager2 ConfigManager2;
#endif /* __cplusplus */

#endif 	/* __ConfigManager2_FWD_DEFINED__ */


#ifndef __ConfigManager2URI_FWD_DEFINED__
#define __ConfigManager2URI_FWD_DEFINED__

#ifdef __cplusplus
typedef class ConfigManager2URI ConfigManager2URI;
#else
typedef struct ConfigManager2URI ConfigManager2URI;
#endif /* __cplusplus */

#endif 	/* __ConfigManager2URI_FWD_DEFINED__ */


#ifndef __ConfigManager2MutableURI_FWD_DEFINED__
#define __ConfigManager2MutableURI_FWD_DEFINED__

#ifdef __cplusplus
typedef class ConfigManager2MutableURI ConfigManager2MutableURI;
#else
typedef struct ConfigManager2MutableURI ConfigManager2MutableURI;
#endif /* __cplusplus */

#endif 	/* __ConfigManager2MutableURI_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "oaidl.h"
#include "cfgdata.h"
#include "cmgruri.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_cfgmgr2_0000 */
/* [local] */ 

#include "CfgMgr2Prop.h"
#include "CfgMgr2Err.h"






extern RPC_IF_HANDLE __MIDL_itf_cfgmgr2_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cfgmgr2_0000_v0_0_s_ifspec;

#ifndef __IConfigManager2_INTERFACE_DEFINED__
#define __IConfigManager2_INTERFACE_DEFINED__

/* interface IConfigManager2 */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigManager2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56A4BDD5-835A-4dd5-95B5-44805CA37DB0")
    IConfigManager2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Execute( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR bstrFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Save( 
            /* [in] */ BSTR bstrFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginTransaction( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CommitTransaction( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelCurrentTransaction( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNode( 
            /* [in] */ BSTR bstrLocURI,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNodeFromURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetHost( 
            /* [in] */ IUnknown __RPC_FAR *pHost) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitializeAccessRoles( 
            /* [in] */ DWORD dwSecRole) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSessionVariable( 
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigManager2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigManager2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigManager2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Execute )( 
            IConfigManager2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cancel )( 
            IConfigManager2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ BSTR bstrFileName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ BSTR bstrFileName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginTransaction )( 
            IConfigManager2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CommitTransaction )( 
            IConfigManager2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelCurrentTransaction )( 
            IConfigManager2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNode )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ BSTR bstrLocURI,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNodeFromURI )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHost )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pHost);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitializeAccessRoles )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ DWORD dwSecRole);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSessionVariable )( 
            IConfigManager2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ VARIANT varValue);
        
        END_INTERFACE
    } IConfigManager2Vtbl;

    interface IConfigManager2
    {
        CONST_VTBL struct IConfigManager2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigManager2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigManager2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigManager2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigManager2_Execute(This)	\
    (This)->lpVtbl -> Execute(This)

#define IConfigManager2_Cancel(This)	\
    (This)->lpVtbl -> Cancel(This)

#define IConfigManager2_Load(This,bstrFileName)	\
    (This)->lpVtbl -> Load(This,bstrFileName)

#define IConfigManager2_Save(This,bstrFileName)	\
    (This)->lpVtbl -> Save(This,bstrFileName)

#define IConfigManager2_BeginTransaction(This)	\
    (This)->lpVtbl -> BeginTransaction(This)

#define IConfigManager2_CommitTransaction(This)	\
    (This)->lpVtbl -> CommitTransaction(This)

#define IConfigManager2_CancelCurrentTransaction(This)	\
    (This)->lpVtbl -> CancelCurrentTransaction(This)

#define IConfigManager2_GetNode(This,bstrLocURI,ppNode)	\
    (This)->lpVtbl -> GetNode(This,bstrLocURI,ppNode)

#define IConfigManager2_GetNodeFromURI(This,pURI,ppNode)	\
    (This)->lpVtbl -> GetNodeFromURI(This,pURI,ppNode)

#define IConfigManager2_SetHost(This,pHost)	\
    (This)->lpVtbl -> SetHost(This,pHost)

#define IConfigManager2_InitializeAccessRoles(This,dwSecRole)	\
    (This)->lpVtbl -> InitializeAccessRoles(This,dwSecRole)

#define IConfigManager2_SetSessionVariable(This,bstrName,varValue)	\
    (This)->lpVtbl -> SetSessionVariable(This,bstrName,varValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigManager2_Execute_Proxy( 
    IConfigManager2 __RPC_FAR * This);


void __RPC_STUB IConfigManager2_Execute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_Cancel_Proxy( 
    IConfigManager2 __RPC_FAR * This);


void __RPC_STUB IConfigManager2_Cancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_Load_Proxy( 
    IConfigManager2 __RPC_FAR * This,
    /* [in] */ BSTR bstrFileName);


void __RPC_STUB IConfigManager2_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_Save_Proxy( 
    IConfigManager2 __RPC_FAR * This,
    /* [in] */ BSTR bstrFileName);


void __RPC_STUB IConfigManager2_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_BeginTransaction_Proxy( 
    IConfigManager2 __RPC_FAR * This);


void __RPC_STUB IConfigManager2_BeginTransaction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_CommitTransaction_Proxy( 
    IConfigManager2 __RPC_FAR * This);


void __RPC_STUB IConfigManager2_CommitTransaction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_CancelCurrentTransaction_Proxy( 
    IConfigManager2 __RPC_FAR * This);


void __RPC_STUB IConfigManager2_CancelCurrentTransaction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_GetNode_Proxy( 
    IConfigManager2 __RPC_FAR * This,
    /* [in] */ BSTR bstrLocURI,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNode);


void __RPC_STUB IConfigManager2_GetNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_GetNodeFromURI_Proxy( 
    IConfigManager2 __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNode);


void __RPC_STUB IConfigManager2_GetNodeFromURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_SetHost_Proxy( 
    IConfigManager2 __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pHost);


void __RPC_STUB IConfigManager2_SetHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_InitializeAccessRoles_Proxy( 
    IConfigManager2 __RPC_FAR * This,
    /* [in] */ DWORD dwSecRole);


void __RPC_STUB IConfigManager2_InitializeAccessRoles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2_SetSessionVariable_Proxy( 
    IConfigManager2 __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ VARIANT varValue);


void __RPC_STUB IConfigManager2_SetSessionVariable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigManager2_INTERFACE_DEFINED__ */


#ifndef __IConfigNode_INTERFACE_DEFINED__
#define __IConfigNode_INTERFACE_DEFINED__

/* interface IConfigNode */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D2A685BF-868F-473b-8D37-6DA07DB041BE")
    IConfigNode : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR bstrChildName,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ BSTR bstrDestinationLocURI,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ BSTR bstrDestinationLocURI,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MoveURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ BSTR bstrChildName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Execute( 
            /* [in] */ VARIANT varUserData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetValue( 
            /* [in] */ VARIANT varValue,
            /* [in] */ CFG_DATATYPE DataType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetValue( 
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLocationURI( 
            /* [out] */ BSTR __RPC_FAR *pbstrLocURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetURI( 
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DidCommandsFail( 
            /* [out] */ BOOL __RPC_FAR *pfCmdsFailed) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetActiveCommandCount( 
            /* [out] */ ULONG __RPC_FAR *pcActiveCommands) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumCommandStates( 
            /* [out] */ IEnumCommandStates __RPC_FAR *__RPC_FAR *ppEnumCommands) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [in] */ REFGUID guidProperty,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteProperty( 
            /* [in] */ REFGUID guidProperty) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumProperties( 
            /* [out] */ IEnumConfigNodeProperties __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumChildren( 
            /* [out] */ IEnumConfigNodes __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParent( 
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNodeParent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNode( 
            /* [in] */ BSTR bstrChildLocURI,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppChildNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNodeFromURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppChildNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Exists( 
            /* [out] */ BOOL __RPC_FAR *pfNodeExists) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ BSTR bstrChildName,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddURI )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Copy )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ BSTR bstrDestinationLocURI,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyURI )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Move )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ BSTR bstrDestinationLocURI,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveURI )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ BOOL fOverwrite,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Delete )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ BSTR bstrChildName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteURI )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Execute )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ VARIANT varUserData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetValue )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ VARIANT varValue,
            /* [in] */ CFG_DATATYPE DataType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetValue )( 
            IConfigNode __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLocationURI )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pbstrLocURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetURI )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetName )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DidCommandsFail )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfCmdsFailed);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetActiveCommandCount )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pcActiveCommands);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumCommandStates )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ IEnumCommandStates __RPC_FAR *__RPC_FAR *ppEnumCommands);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperty )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteProperty )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumProperties )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ IEnumConfigNodeProperties __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumChildren )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ IEnumConfigNodes __RPC_FAR *__RPC_FAR *ppEnum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParent )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNodeParent);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNode )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ BSTR bstrChildLocURI,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppChildNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNodeFromURI )( 
            IConfigNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppChildNode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Exists )( 
            IConfigNode __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfNodeExists);
        
        END_INTERFACE
    } IConfigNodeVtbl;

    interface IConfigNode
    {
        CONST_VTBL struct IConfigNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigNode_Add(This,bstrChildName,DataType,varValue,ppNewNode)	\
    (This)->lpVtbl -> Add(This,bstrChildName,DataType,varValue,ppNewNode)

#define IConfigNode_AddURI(This,puriChild,DataType,varValue,ppNewNode)	\
    (This)->lpVtbl -> AddURI(This,puriChild,DataType,varValue,ppNewNode)

#define IConfigNode_Copy(This,bstrDestinationLocURI,fOverwrite,ppNewNode)	\
    (This)->lpVtbl -> Copy(This,bstrDestinationLocURI,fOverwrite,ppNewNode)

#define IConfigNode_CopyURI(This,puriDestination,fOverwrite,ppNewNode)	\
    (This)->lpVtbl -> CopyURI(This,puriDestination,fOverwrite,ppNewNode)

#define IConfigNode_Move(This,bstrDestinationLocURI,fOverwrite,ppNewNode)	\
    (This)->lpVtbl -> Move(This,bstrDestinationLocURI,fOverwrite,ppNewNode)

#define IConfigNode_MoveURI(This,puriDestination,fOverwrite,ppNewNode)	\
    (This)->lpVtbl -> MoveURI(This,puriDestination,fOverwrite,ppNewNode)

#define IConfigNode_Delete(This,bstrChildName)	\
    (This)->lpVtbl -> Delete(This,bstrChildName)

#define IConfigNode_DeleteURI(This,puriChild)	\
    (This)->lpVtbl -> DeleteURI(This,puriChild)

#define IConfigNode_Execute(This,varUserData)	\
    (This)->lpVtbl -> Execute(This,varUserData)

#define IConfigNode_SetValue(This,varValue,DataType)	\
    (This)->lpVtbl -> SetValue(This,varValue,DataType)

#define IConfigNode_GetValue(This,pvarValue)	\
    (This)->lpVtbl -> GetValue(This,pvarValue)

#define IConfigNode_GetLocationURI(This,pbstrLocURI)	\
    (This)->lpVtbl -> GetLocationURI(This,pbstrLocURI)

#define IConfigNode_GetURI(This,ppURI)	\
    (This)->lpVtbl -> GetURI(This,ppURI)

#define IConfigNode_GetName(This,pbstrName)	\
    (This)->lpVtbl -> GetName(This,pbstrName)

#define IConfigNode_DidCommandsFail(This,pfCmdsFailed)	\
    (This)->lpVtbl -> DidCommandsFail(This,pfCmdsFailed)

#define IConfigNode_GetActiveCommandCount(This,pcActiveCommands)	\
    (This)->lpVtbl -> GetActiveCommandCount(This,pcActiveCommands)

#define IConfigNode_EnumCommandStates(This,ppEnumCommands)	\
    (This)->lpVtbl -> EnumCommandStates(This,ppEnumCommands)

#define IConfigNode_GetProperty(This,guidProperty,pvarValue)	\
    (This)->lpVtbl -> GetProperty(This,guidProperty,pvarValue)

#define IConfigNode_SetProperty(This,guidProperty,varValue)	\
    (This)->lpVtbl -> SetProperty(This,guidProperty,varValue)

#define IConfigNode_DeleteProperty(This,guidProperty)	\
    (This)->lpVtbl -> DeleteProperty(This,guidProperty)

#define IConfigNode_EnumProperties(This,ppEnum)	\
    (This)->lpVtbl -> EnumProperties(This,ppEnum)

#define IConfigNode_EnumChildren(This,ppEnum)	\
    (This)->lpVtbl -> EnumChildren(This,ppEnum)

#define IConfigNode_GetParent(This,ppNodeParent)	\
    (This)->lpVtbl -> GetParent(This,ppNodeParent)

#define IConfigNode_GetNode(This,bstrChildLocURI,ppChildNode)	\
    (This)->lpVtbl -> GetNode(This,bstrChildLocURI,ppChildNode)

#define IConfigNode_GetNodeFromURI(This,puriChild,ppChildNode)	\
    (This)->lpVtbl -> GetNodeFromURI(This,puriChild,ppChildNode)

#define IConfigNode_Exists(This,pfNodeExists)	\
    (This)->lpVtbl -> Exists(This,pfNodeExists)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigNode_Add_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ BSTR bstrChildName,
    /* [in] */ CFG_DATATYPE DataType,
    /* [in] */ VARIANT varValue,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);


void __RPC_STUB IConfigNode_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_AddURI_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
    /* [in] */ CFG_DATATYPE DataType,
    /* [in] */ VARIANT varValue,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);


void __RPC_STUB IConfigNode_AddURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_Copy_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ BSTR bstrDestinationLocURI,
    /* [in] */ BOOL fOverwrite,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);


void __RPC_STUB IConfigNode_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_CopyURI_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
    /* [in] */ BOOL fOverwrite,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);


void __RPC_STUB IConfigNode_CopyURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_Move_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ BSTR bstrDestinationLocURI,
    /* [in] */ BOOL fOverwrite,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);


void __RPC_STUB IConfigNode_Move_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_MoveURI_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
    /* [in] */ BOOL fOverwrite,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNewNode);


void __RPC_STUB IConfigNode_MoveURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_Delete_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ BSTR bstrChildName);


void __RPC_STUB IConfigNode_Delete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_DeleteURI_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChild);


void __RPC_STUB IConfigNode_DeleteURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_Execute_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ VARIANT varUserData);


void __RPC_STUB IConfigNode_Execute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_SetValue_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ VARIANT varValue,
    /* [in] */ CFG_DATATYPE DataType);


void __RPC_STUB IConfigNode_SetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetValue_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB IConfigNode_GetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetLocationURI_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pbstrLocURI);


void __RPC_STUB IConfigNode_GetLocationURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetURI_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);


void __RPC_STUB IConfigNode_GetURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetName_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IConfigNode_GetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_DidCommandsFail_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pfCmdsFailed);


void __RPC_STUB IConfigNode_DidCommandsFail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetActiveCommandCount_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pcActiveCommands);


void __RPC_STUB IConfigNode_GetActiveCommandCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_EnumCommandStates_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ IEnumCommandStates __RPC_FAR *__RPC_FAR *ppEnumCommands);


void __RPC_STUB IConfigNode_EnumCommandStates_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetProperty_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty,
    /* [out][in] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB IConfigNode_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_SetProperty_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty,
    /* [in] */ VARIANT varValue);


void __RPC_STUB IConfigNode_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_DeleteProperty_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty);


void __RPC_STUB IConfigNode_DeleteProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_EnumProperties_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ IEnumConfigNodeProperties __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IConfigNode_EnumProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_EnumChildren_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ IEnumConfigNodes __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IConfigNode_EnumChildren_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetParent_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNodeParent);


void __RPC_STUB IConfigNode_GetParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetNode_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ BSTR bstrChildLocURI,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppChildNode);


void __RPC_STUB IConfigNode_GetNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_GetNodeFromURI_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
    /* [out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppChildNode);


void __RPC_STUB IConfigNode_GetNodeFromURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNode_Exists_Proxy( 
    IConfigNode __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pfNodeExists);


void __RPC_STUB IConfigNode_Exists_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigNode_INTERFACE_DEFINED__ */


#ifndef __IEnumConfigNodeProperties_INTERFACE_DEFINED__
#define __IEnumConfigNodeProperties_INTERFACE_DEFINED__

/* interface IEnumConfigNodeProperties */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEnumConfigNodeProperties;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FEB429FE-5AC4-4a63-A8B6-847FB2103C8C")
    IEnumConfigNodeProperties : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cProperties,
            /* [size_is][out] */ GUID __RPC_FAR *pProperty,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cProperties) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumConfigNodeProperties __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumConfigNodePropertiesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumConfigNodeProperties __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumConfigNodeProperties __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumConfigNodeProperties __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumConfigNodeProperties __RPC_FAR * This,
            /* [in] */ ULONG cProperties,
            /* [size_is][out] */ GUID __RPC_FAR *pProperty,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumConfigNodeProperties __RPC_FAR * This,
            /* [in] */ ULONG cProperties);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumConfigNodeProperties __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumConfigNodeProperties __RPC_FAR * This,
            /* [out] */ IEnumConfigNodeProperties __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumConfigNodePropertiesVtbl;

    interface IEnumConfigNodeProperties
    {
        CONST_VTBL struct IEnumConfigNodePropertiesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumConfigNodeProperties_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumConfigNodeProperties_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumConfigNodeProperties_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumConfigNodeProperties_Next(This,cProperties,pProperty,pcFetched)	\
    (This)->lpVtbl -> Next(This,cProperties,pProperty,pcFetched)

#define IEnumConfigNodeProperties_Skip(This,cProperties)	\
    (This)->lpVtbl -> Skip(This,cProperties)

#define IEnumConfigNodeProperties_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumConfigNodeProperties_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumConfigNodeProperties_Next_Proxy( 
    IEnumConfigNodeProperties __RPC_FAR * This,
    /* [in] */ ULONG cProperties,
    /* [size_is][out] */ GUID __RPC_FAR *pProperty,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumConfigNodeProperties_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigNodeProperties_Skip_Proxy( 
    IEnumConfigNodeProperties __RPC_FAR * This,
    /* [in] */ ULONG cProperties);


void __RPC_STUB IEnumConfigNodeProperties_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigNodeProperties_Reset_Proxy( 
    IEnumConfigNodeProperties __RPC_FAR * This);


void __RPC_STUB IEnumConfigNodeProperties_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigNodeProperties_Clone_Proxy( 
    IEnumConfigNodeProperties __RPC_FAR * This,
    /* [out] */ IEnumConfigNodeProperties __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumConfigNodeProperties_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumConfigNodeProperties_INTERFACE_DEFINED__ */


#ifndef __IEnumConfigNodes_INTERFACE_DEFINED__
#define __IEnumConfigNodes_INTERFACE_DEFINED__

/* interface IEnumConfigNodes */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEnumConfigNodes;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF860911-3CDA-4f76-B7F3-85376FBD2068")
    IEnumConfigNodes : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cNodes,
            /* [size_is][out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNodes,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cNodes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumConfigNodes __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumConfigNodesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumConfigNodes __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumConfigNodes __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumConfigNodes __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumConfigNodes __RPC_FAR * This,
            /* [in] */ ULONG cNodes,
            /* [size_is][out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNodes,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumConfigNodes __RPC_FAR * This,
            /* [in] */ ULONG cNodes);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumConfigNodes __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumConfigNodes __RPC_FAR * This,
            /* [out] */ IEnumConfigNodes __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumConfigNodesVtbl;

    interface IEnumConfigNodes
    {
        CONST_VTBL struct IEnumConfigNodesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumConfigNodes_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumConfigNodes_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumConfigNodes_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumConfigNodes_Next(This,cNodes,ppNodes,pcFetched)	\
    (This)->lpVtbl -> Next(This,cNodes,ppNodes,pcFetched)

#define IEnumConfigNodes_Skip(This,cNodes)	\
    (This)->lpVtbl -> Skip(This,cNodes)

#define IEnumConfigNodes_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumConfigNodes_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumConfigNodes_Next_Proxy( 
    IEnumConfigNodes __RPC_FAR * This,
    /* [in] */ ULONG cNodes,
    /* [size_is][out] */ IConfigNode __RPC_FAR *__RPC_FAR *ppNodes,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumConfigNodes_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigNodes_Skip_Proxy( 
    IEnumConfigNodes __RPC_FAR * This,
    /* [in] */ ULONG cNodes);


void __RPC_STUB IEnumConfigNodes_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigNodes_Reset_Proxy( 
    IEnumConfigNodes __RPC_FAR * This);


void __RPC_STUB IEnumConfigNodes_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigNodes_Clone_Proxy( 
    IEnumConfigNodes __RPC_FAR * This,
    /* [out] */ IEnumConfigNodes __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumConfigNodes_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumConfigNodes_INTERFACE_DEFINED__ */


#ifndef __IEnumCommandStates_INTERFACE_DEFINED__
#define __IEnumCommandStates_INTERFACE_DEFINED__

/* interface IEnumCommandStates */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEnumCommandStates;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AEE9A36B-8F63-4e93-AAC2-F9BE3335B9F5")
    IEnumCommandStates : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cCommandStates,
            /* [size_is][out] */ HRESULT __RPC_FAR *phrCommandStates,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cCommandStates) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumCommandStates __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumCommandStatesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumCommandStates __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumCommandStates __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumCommandStates __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumCommandStates __RPC_FAR * This,
            /* [in] */ ULONG cCommandStates,
            /* [size_is][out] */ HRESULT __RPC_FAR *phrCommandStates,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumCommandStates __RPC_FAR * This,
            /* [in] */ ULONG cCommandStates);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumCommandStates __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumCommandStates __RPC_FAR * This,
            /* [out] */ IEnumCommandStates __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumCommandStatesVtbl;

    interface IEnumCommandStates
    {
        CONST_VTBL struct IEnumCommandStatesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumCommandStates_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumCommandStates_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumCommandStates_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumCommandStates_Next(This,cCommandStates,phrCommandStates,pcFetched)	\
    (This)->lpVtbl -> Next(This,cCommandStates,phrCommandStates,pcFetched)

#define IEnumCommandStates_Skip(This,cCommandStates)	\
    (This)->lpVtbl -> Skip(This,cCommandStates)

#define IEnumCommandStates_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumCommandStates_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumCommandStates_Next_Proxy( 
    IEnumCommandStates __RPC_FAR * This,
    /* [in] */ ULONG cCommandStates,
    /* [size_is][out] */ HRESULT __RPC_FAR *phrCommandStates,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumCommandStates_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumCommandStates_Skip_Proxy( 
    IEnumCommandStates __RPC_FAR * This,
    /* [in] */ ULONG cCommandStates);


void __RPC_STUB IEnumCommandStates_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumCommandStates_Reset_Proxy( 
    IEnumCommandStates __RPC_FAR * This);


void __RPC_STUB IEnumCommandStates_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumCommandStates_Clone_Proxy( 
    IEnumCommandStates __RPC_FAR * This,
    /* [out] */ IEnumCommandStates __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumCommandStates_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumCommandStates_INTERFACE_DEFINED__ */



#ifndef __ConfigManager2_LIBRARY_DEFINED__
#define __ConfigManager2_LIBRARY_DEFINED__

/* library ConfigManager2 */
/* [helpstring][uuid] */ 


EXTERN_C const IID LIBID_ConfigManager2;

EXTERN_C const CLSID CLSID_ConfigManager2;

#ifdef __cplusplus

class DECLSPEC_UUID("66D0DB14-5638-475f-A386-629522D8C461")
ConfigManager2;
#endif

EXTERN_C const CLSID CLSID_ConfigManager2URI;

#ifdef __cplusplus

class DECLSPEC_UUID("50A41EF1-6BFA-4b7e-973E-798EA0BEBAD4")
ConfigManager2URI;
#endif

EXTERN_C const CLSID CLSID_ConfigManager2MutableURI;

#ifdef __cplusplus

class DECLSPEC_UUID("C196B2BE-BA06-4049-8518-322E1E04282B")
ConfigManager2MutableURI;
#endif
#endif /* __ConfigManager2_LIBRARY_DEFINED__ */

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


