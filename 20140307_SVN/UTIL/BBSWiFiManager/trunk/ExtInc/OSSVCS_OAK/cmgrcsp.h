
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:22:10 2008
 */
/* Compiler settings for .\cmgrcsp.idl:
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

#ifndef __cmgrcsp_h__
#define __cmgrcsp_h__

/* Forward Declarations */ 

#ifndef __IConfigNodeState_FWD_DEFINED__
#define __IConfigNodeState_FWD_DEFINED__
typedef interface IConfigNodeState IConfigNodeState;
#endif 	/* __IConfigNodeState_FWD_DEFINED__ */


#ifndef __IConfigSession2_FWD_DEFINED__
#define __IConfigSession2_FWD_DEFINED__
typedef interface IConfigSession2 IConfigSession2;
#endif 	/* __IConfigSession2_FWD_DEFINED__ */


#ifndef __IConfigServiceProvider2_FWD_DEFINED__
#define __IConfigServiceProvider2_FWD_DEFINED__
typedef interface IConfigServiceProvider2 IConfigServiceProvider2;
#endif 	/* __IConfigServiceProvider2_FWD_DEFINED__ */


#ifndef __ICSPNode_FWD_DEFINED__
#define __ICSPNode_FWD_DEFINED__
typedef interface ICSPNode ICSPNode;
#endif 	/* __ICSPNode_FWD_DEFINED__ */


#ifndef __ICSPValidate_FWD_DEFINED__
#define __ICSPValidate_FWD_DEFINED__
typedef interface ICSPValidate ICSPValidate;
#endif 	/* __ICSPValidate_FWD_DEFINED__ */


#ifndef __ICSPNodeTransactioning_FWD_DEFINED__
#define __ICSPNodeTransactioning_FWD_DEFINED__
typedef interface ICSPNodeTransactioning ICSPNodeTransactioning;
#endif 	/* __ICSPNodeTransactioning_FWD_DEFINED__ */


/* header files for imported files */
#include "cfgmgr2.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_cmgrcsp_0000 */
/* [local] */ 

#define CSPNODE_OPTION_NATIVESECURITY        0x01
#define CSPNODE_OPTION_INTERNALTRANSACTION   0x02
#define CSPNODE_OPTION_HANDLEALLPROPERTIES   0x04
#define CSPNODE_OPTION_SECRETDATA            0x08





typedef 
enum ConfigManager2Notification
    {	CFGMGR_NOTIFICATION_LOAD	= 0,
	CFGMGR_NOTIFICATION_BEGINCOMMANDPROCESSING	= CFGMGR_NOTIFICATION_LOAD + 1,
	CFGMGR_NOTIFICATION_ENDCOMMANDPROCESSING	= CFGMGR_NOTIFICATION_BEGINCOMMANDPROCESSING + 1,
	CFGMGR_NOTIFICATION_UNLOAD	= CFGMGR_NOTIFICATION_ENDCOMMANDPROCESSING + 1,
	CFGMGR_NOTIFICATION_SETSESSIONOBJ	= CFGMGR_NOTIFICATION_UNLOAD + 1,
	CFGMGR_NOTIFICATION_BEGINCOMMIT	= CFGMGR_NOTIFICATION_SETSESSIONOBJ + 1,
	CFGMGR_NOTIFICATION_ENDCOMMIT	= CFGMGR_NOTIFICATION_BEGINCOMMIT + 1,
	CFGMGR_NOTIFICATION_BEGINROLLBACK	= CFGMGR_NOTIFICATION_ENDCOMMIT + 1,
	CFGMGR_NOTIFICATION_ENDROLLBACK	= CFGMGR_NOTIFICATION_BEGINROLLBACK + 1,
	CFGMGR_NOTIFICATION_BEGINTRANSACTIONING	= CFGMGR_NOTIFICATION_ENDROLLBACK + 1,
	CFGMGR_NOTIFICATION_ENDTRANSACTIONING	= CFGMGR_NOTIFICATION_BEGINTRANSACTIONING + 1,
	CFGMGR_NOTIFICATION_LAST	= CFGMGR_NOTIFICATION_ENDTRANSACTIONING
    }	CFGMGR_NOTIFICATION;



extern RPC_IF_HANDLE __MIDL_itf_cmgrcsp_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cmgrcsp_0000_v0_0_s_ifspec;

#ifndef __IConfigNodeState_INTERFACE_DEFINED__
#define __IConfigNodeState_INTERFACE_DEFINED__

/* interface IConfigNodeState */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigNodeState;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D18B9F92-A955-478b-9038-08B849C2B514")
    IConfigNodeState : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetLocationURI( 
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTreeDepth( 
            /* [out][in] */ ULONG __RPC_FAR *pulTreeDepth) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigNodeStateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigNodeState __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigNodeState __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigNodeState __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLocationURI )( 
            IConfigNodeState __RPC_FAR * This,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetName )( 
            IConfigNodeState __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTreeDepth )( 
            IConfigNodeState __RPC_FAR * This,
            /* [out][in] */ ULONG __RPC_FAR *pulTreeDepth);
        
        END_INTERFACE
    } IConfigNodeStateVtbl;

    interface IConfigNodeState
    {
        CONST_VTBL struct IConfigNodeStateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigNodeState_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigNodeState_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigNodeState_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigNodeState_GetLocationURI(This,ppURI)	\
    (This)->lpVtbl -> GetLocationURI(This,ppURI)

#define IConfigNodeState_GetName(This,pbstrName)	\
    (This)->lpVtbl -> GetName(This,pbstrName)

#define IConfigNodeState_GetTreeDepth(This,pulTreeDepth)	\
    (This)->lpVtbl -> GetTreeDepth(This,pulTreeDepth)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigNodeState_GetLocationURI_Proxy( 
    IConfigNodeState __RPC_FAR * This,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);


void __RPC_STUB IConfigNodeState_GetLocationURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNodeState_GetName_Proxy( 
    IConfigNodeState __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IConfigNodeState_GetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigNodeState_GetTreeDepth_Proxy( 
    IConfigNodeState __RPC_FAR * This,
    /* [out][in] */ ULONG __RPC_FAR *pulTreeDepth);


void __RPC_STUB IConfigNodeState_GetTreeDepth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigNodeState_INTERFACE_DEFINED__ */


#ifndef __IConfigSession2_INTERFACE_DEFINED__
#define __IConfigSession2_INTERFACE_DEFINED__

/* interface IConfigSession2 */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigSession2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8D31FC7E-B285-49a2-B38C-6E0EF9D99CDB")
    IConfigSession2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetAccessRoles( 
            /* [out] */ DWORD __RPC_FAR *pdwSecRole) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBaseAccessRoles( 
            /* [out] */ DWORD __RPC_FAR *pdwSecRole) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetHost( 
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppHost) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPropertyFromMetabase( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ REFGUID guidProperty,
            /* [out] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSessionVariable( 
            /* [in] */ BSTR bstrName,
            /* [out] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigSession2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigSession2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigSession2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigSession2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAccessRoles )( 
            IConfigSession2 __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwSecRole);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBaseAccessRoles )( 
            IConfigSession2 __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwSecRole);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHost )( 
            IConfigSession2 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppHost);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPropertyFromMetabase )( 
            IConfigSession2 __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ REFGUID guidProperty,
            /* [out] */ VARIANT __RPC_FAR *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSessionVariable )( 
            IConfigSession2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [out] */ VARIANT __RPC_FAR *pvarValue);
        
        END_INTERFACE
    } IConfigSession2Vtbl;

    interface IConfigSession2
    {
        CONST_VTBL struct IConfigSession2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigSession2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigSession2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigSession2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigSession2_GetAccessRoles(This,pdwSecRole)	\
    (This)->lpVtbl -> GetAccessRoles(This,pdwSecRole)

#define IConfigSession2_GetBaseAccessRoles(This,pdwSecRole)	\
    (This)->lpVtbl -> GetBaseAccessRoles(This,pdwSecRole)

#define IConfigSession2_GetHost(This,ppHost)	\
    (This)->lpVtbl -> GetHost(This,ppHost)

#define IConfigSession2_GetPropertyFromMetabase(This,pURI,guidProperty,pvarValue)	\
    (This)->lpVtbl -> GetPropertyFromMetabase(This,pURI,guidProperty,pvarValue)

#define IConfigSession2_GetSessionVariable(This,bstrName,pvarValue)	\
    (This)->lpVtbl -> GetSessionVariable(This,bstrName,pvarValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigSession2_GetAccessRoles_Proxy( 
    IConfigSession2 __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwSecRole);


void __RPC_STUB IConfigSession2_GetAccessRoles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession2_GetBaseAccessRoles_Proxy( 
    IConfigSession2 __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwSecRole);


void __RPC_STUB IConfigSession2_GetBaseAccessRoles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession2_GetHost_Proxy( 
    IConfigSession2 __RPC_FAR * This,
    /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppHost);


void __RPC_STUB IConfigSession2_GetHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession2_GetPropertyFromMetabase_Proxy( 
    IConfigSession2 __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
    /* [in] */ REFGUID guidProperty,
    /* [out] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB IConfigSession2_GetPropertyFromMetabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession2_GetSessionVariable_Proxy( 
    IConfigSession2 __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [out] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB IConfigSession2_GetSessionVariable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigSession2_INTERFACE_DEFINED__ */


#ifndef __IConfigServiceProvider2_INTERFACE_DEFINED__
#define __IConfigServiceProvider2_INTERFACE_DEFINED__

/* interface IConfigServiceProvider2 */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigServiceProvider2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F35E39DC-E18A-48c2-88CB-B3CF48CA6E83")
    IConfigServiceProvider2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetNode( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [out] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNode,
            /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConfigManagerNotification( 
            /* [in] */ CFGMGR_NOTIFICATION cmnfyState,
            /* [in] */ LPARAM lpParam) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigServiceProvider2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigServiceProvider2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigServiceProvider2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigServiceProvider2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNode )( 
            IConfigServiceProvider2 __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [out] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNode,
            /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConfigManagerNotification )( 
            IConfigServiceProvider2 __RPC_FAR * This,
            /* [in] */ CFGMGR_NOTIFICATION cmnfyState,
            /* [in] */ LPARAM lpParam);
        
        END_INTERFACE
    } IConfigServiceProvider2Vtbl;

    interface IConfigServiceProvider2
    {
        CONST_VTBL struct IConfigServiceProvider2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigServiceProvider2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigServiceProvider2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigServiceProvider2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigServiceProvider2_GetNode(This,pURI,ppNode,pgrfNodeOptions)	\
    (This)->lpVtbl -> GetNode(This,pURI,ppNode,pgrfNodeOptions)

#define IConfigServiceProvider2_ConfigManagerNotification(This,cmnfyState,lpParam)	\
    (This)->lpVtbl -> ConfigManagerNotification(This,cmnfyState,lpParam)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigServiceProvider2_GetNode_Proxy( 
    IConfigServiceProvider2 __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
    /* [out] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNode,
    /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions);


void __RPC_STUB IConfigServiceProvider2_GetNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider2_ConfigManagerNotification_Proxy( 
    IConfigServiceProvider2 __RPC_FAR * This,
    /* [in] */ CFGMGR_NOTIFICATION cmnfyState,
    /* [in] */ LPARAM lpParam);


void __RPC_STUB IConfigServiceProvider2_ConfigManagerNotification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigServiceProvider2_INTERFACE_DEFINED__ */


#ifndef __ICSPNode_INTERFACE_DEFINED__
#define __ICSPNode_INTERFACE_DEFINED__

/* interface ICSPNode */
/* [object][uuid] */ 


EXTERN_C const IID IID_ICSPNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8A13633C-797D-46e9-B602-D982B8EC9847")
    ICSPNode : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetChildNodeNames( 
            /* [out] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][size_is][out] */ BSTR __RPC_FAR *__RPC_FAR *pbstrNodeNames) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pChildName,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [out][in] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNewNode,
            /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [out][in] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNewNode,
            /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DetachChild( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChildToDelete) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Execute( 
            /* [in] */ VARIANT varUserData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetValue( 
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetValue( 
            /* [in] */ VARIANT varValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [in] */ REFGUID guidProperty,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteProperty( 
            /* [in] */ REFGUID guidProperty) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPropertyIdentifiers( 
            /* [out] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][size_is][out] */ GUID __RPC_FAR *__RPC_FAR *pguidProperties) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICSPNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICSPNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICSPNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetChildNodeNames )( 
            ICSPNode __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][size_is][out] */ BSTR __RPC_FAR *__RPC_FAR *pbstrNodeNames);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pChildName,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [out][in] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNewNode,
            /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Copy )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [out][in] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNewNode,
            /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DetachChild )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChildToDelete);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Delete )( 
            ICSPNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Execute )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ VARIANT varUserData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Move )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetValue )( 
            ICSPNode __RPC_FAR * This,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetValue )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ VARIANT varValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperty )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteProperty )( 
            ICSPNode __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPropertyIdentifiers )( 
            ICSPNode __RPC_FAR * This,
            /* [out] */ ULONG __RPC_FAR *pulCount,
            /* [size_is][size_is][out] */ GUID __RPC_FAR *__RPC_FAR *pguidProperties);
        
        END_INTERFACE
    } ICSPNodeVtbl;

    interface ICSPNode
    {
        CONST_VTBL struct ICSPNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICSPNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICSPNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICSPNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICSPNode_GetChildNodeNames(This,pulCount,pbstrNodeNames)	\
    (This)->lpVtbl -> GetChildNodeNames(This,pulCount,pbstrNodeNames)

#define ICSPNode_Add(This,pChildName,DataType,varValue,ppNewNode,pgrfNodeOptions)	\
    (This)->lpVtbl -> Add(This,pChildName,DataType,varValue,ppNewNode,pgrfNodeOptions)

#define ICSPNode_Copy(This,puriDestination,ppNewNode,pgrfNodeOptions)	\
    (This)->lpVtbl -> Copy(This,puriDestination,ppNewNode,pgrfNodeOptions)

#define ICSPNode_DetachChild(This,puriChildToDelete)	\
    (This)->lpVtbl -> DetachChild(This,puriChildToDelete)

#define ICSPNode_Delete(This)	\
    (This)->lpVtbl -> Delete(This)

#define ICSPNode_Execute(This,varUserData)	\
    (This)->lpVtbl -> Execute(This,varUserData)

#define ICSPNode_Move(This,puriDestination)	\
    (This)->lpVtbl -> Move(This,puriDestination)

#define ICSPNode_GetValue(This,pvarValue)	\
    (This)->lpVtbl -> GetValue(This,pvarValue)

#define ICSPNode_SetValue(This,varValue)	\
    (This)->lpVtbl -> SetValue(This,varValue)

#define ICSPNode_GetProperty(This,guidProperty,pvarValue)	\
    (This)->lpVtbl -> GetProperty(This,guidProperty,pvarValue)

#define ICSPNode_SetProperty(This,guidProperty,varValue)	\
    (This)->lpVtbl -> SetProperty(This,guidProperty,varValue)

#define ICSPNode_DeleteProperty(This,guidProperty)	\
    (This)->lpVtbl -> DeleteProperty(This,guidProperty)

#define ICSPNode_GetPropertyIdentifiers(This,pulCount,pguidProperties)	\
    (This)->lpVtbl -> GetPropertyIdentifiers(This,pulCount,pguidProperties)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICSPNode_GetChildNodeNames_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulCount,
    /* [size_is][size_is][out] */ BSTR __RPC_FAR *__RPC_FAR *pbstrNodeNames);


void __RPC_STUB ICSPNode_GetChildNodeNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_Add_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pChildName,
    /* [in] */ CFG_DATATYPE DataType,
    /* [in] */ VARIANT varValue,
    /* [out][in] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNewNode,
    /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions);


void __RPC_STUB ICSPNode_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_Copy_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
    /* [out][in] */ ICSPNode __RPC_FAR *__RPC_FAR *ppNewNode,
    /* [out][in] */ DWORD __RPC_FAR *pgrfNodeOptions);


void __RPC_STUB ICSPNode_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_DetachChild_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChildToDelete);


void __RPC_STUB ICSPNode_DetachChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_Delete_Proxy( 
    ICSPNode __RPC_FAR * This);


void __RPC_STUB ICSPNode_Delete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_Execute_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ VARIANT varUserData);


void __RPC_STUB ICSPNode_Execute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_Move_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination);


void __RPC_STUB ICSPNode_Move_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_GetValue_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [out][in] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB ICSPNode_GetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_SetValue_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ VARIANT varValue);


void __RPC_STUB ICSPNode_SetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_GetProperty_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty,
    /* [out][in] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB ICSPNode_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_SetProperty_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty,
    /* [in] */ VARIANT varValue);


void __RPC_STUB ICSPNode_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_DeleteProperty_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty);


void __RPC_STUB ICSPNode_DeleteProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNode_GetPropertyIdentifiers_Proxy( 
    ICSPNode __RPC_FAR * This,
    /* [out] */ ULONG __RPC_FAR *pulCount,
    /* [size_is][size_is][out] */ GUID __RPC_FAR *__RPC_FAR *pguidProperties);


void __RPC_STUB ICSPNode_GetPropertyIdentifiers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICSPNode_INTERFACE_DEFINED__ */


#ifndef __ICSPValidate_INTERFACE_DEFINED__
#define __ICSPValidate_INTERFACE_DEFINED__

/* interface ICSPValidate */
/* [object][uuid] */ 


EXTERN_C const IID IID_ICSPValidate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B3A813D0-2E93-48df-AA22-E46A19F91DBC")
    ICSPValidate : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ValidateAdd( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateCopy( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateDetachChild( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateDelete( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateExecute( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ VARIANT varUserData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateMove( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateSetValue( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ VARIANT varValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateSetProperty( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ValidateDeleteProperty( 
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ REFGUID guidProperty) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICSPValidateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICSPValidate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICSPValidate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateAdd )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateCopy )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateDetachChild )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateDelete )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateExecute )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ VARIANT varUserData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateMove )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateSetValue )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ VARIANT varValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateSetProperty )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ValidateDeleteProperty )( 
            ICSPValidate __RPC_FAR * This,
            /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
            /* [in] */ REFGUID guidProperty);
        
        END_INTERFACE
    } ICSPValidateVtbl;

    interface ICSPValidate
    {
        CONST_VTBL struct ICSPValidateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICSPValidate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICSPValidate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICSPValidate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICSPValidate_ValidateAdd(This,pNodeState,puriChild,DataType,varValue)	\
    (This)->lpVtbl -> ValidateAdd(This,pNodeState,puriChild,DataType,varValue)

#define ICSPValidate_ValidateCopy(This,pNodeState,puriDestination)	\
    (This)->lpVtbl -> ValidateCopy(This,pNodeState,puriDestination)

#define ICSPValidate_ValidateDetachChild(This,pNodeState,puriChild)	\
    (This)->lpVtbl -> ValidateDetachChild(This,pNodeState,puriChild)

#define ICSPValidate_ValidateDelete(This,pNodeState)	\
    (This)->lpVtbl -> ValidateDelete(This,pNodeState)

#define ICSPValidate_ValidateExecute(This,pNodeState,varUserData)	\
    (This)->lpVtbl -> ValidateExecute(This,pNodeState,varUserData)

#define ICSPValidate_ValidateMove(This,pNodeState,puriDestination)	\
    (This)->lpVtbl -> ValidateMove(This,pNodeState,puriDestination)

#define ICSPValidate_ValidateSetValue(This,pNodeState,varValue)	\
    (This)->lpVtbl -> ValidateSetValue(This,pNodeState,varValue)

#define ICSPValidate_ValidateSetProperty(This,pNodeState,guidProperty,varValue)	\
    (This)->lpVtbl -> ValidateSetProperty(This,pNodeState,guidProperty,varValue)

#define ICSPValidate_ValidateDeleteProperty(This,pNodeState,guidProperty)	\
    (This)->lpVtbl -> ValidateDeleteProperty(This,pNodeState,guidProperty)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateAdd_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
    /* [in] */ CFG_DATATYPE DataType,
    /* [in] */ VARIANT varValue);


void __RPC_STUB ICSPValidate_ValidateAdd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateCopy_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination);


void __RPC_STUB ICSPValidate_ValidateCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateDetachChild_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChild);


void __RPC_STUB ICSPValidate_ValidateDetachChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateDelete_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState);


void __RPC_STUB ICSPValidate_ValidateDelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateExecute_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ VARIANT varUserData);


void __RPC_STUB ICSPValidate_ValidateExecute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateMove_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination);


void __RPC_STUB ICSPValidate_ValidateMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateSetValue_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ VARIANT varValue);


void __RPC_STUB ICSPValidate_ValidateSetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateSetProperty_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ REFGUID guidProperty,
    /* [in] */ VARIANT varValue);


void __RPC_STUB ICSPValidate_ValidateSetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPValidate_ValidateDeleteProperty_Proxy( 
    ICSPValidate __RPC_FAR * This,
    /* [in] */ IConfigNodeState __RPC_FAR *pNodeState,
    /* [in] */ REFGUID guidProperty);


void __RPC_STUB ICSPValidate_ValidateDeleteProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICSPValidate_INTERFACE_DEFINED__ */


#ifndef __ICSPNodeTransactioning_INTERFACE_DEFINED__
#define __ICSPNodeTransactioning_INTERFACE_DEFINED__

/* interface ICSPNodeTransactioning */
/* [object][uuid] */ 


EXTERN_C const IID IID_ICSPNodeTransactioning;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("55261A46-0F96-4f7f-95A5-A73A978B047B")
    ICSPNodeTransactioning : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackAddState( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackCopyState( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackDetachChildState( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackDeleteState( 
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackExecuteState( 
            /* [in] */ VARIANT varUserData,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackMoveState( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackSetValueState( 
            /* [in] */ VARIANT varValue,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackSetPropertyState( 
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistRollbackDeletePropertyState( 
            /* [in] */ REFGUID guidProperty,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackAdd( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackCopy( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackDetachChild( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackDelete( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackExecute( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackMove( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackSetValue( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackSetProperty( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RollbackDeleteProperty( 
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Commit( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICSPNodeTransactioningVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICSPNodeTransactioning __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICSPNodeTransactioning __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackAddState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ CFG_DATATYPE DataType,
            /* [in] */ VARIANT varValue,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackCopyState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackDetachChildState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackDeleteState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackExecuteState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ VARIANT varUserData,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackMoveState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackSetValueState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ VARIANT varValue,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackSetPropertyState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty,
            /* [in] */ VARIANT varValue,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PersistRollbackDeletePropertyState )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ REFGUID guidProperty,
            /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
            /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackAdd )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackCopy )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackDetachChild )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackDelete )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackExecute )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackMove )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackSetValue )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackSetProperty )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RollbackDeleteProperty )( 
            ICSPNodeTransactioning __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
            /* [in] */ BOOL fRecoveryRollback);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Commit )( 
            ICSPNodeTransactioning __RPC_FAR * This);
        
        END_INTERFACE
    } ICSPNodeTransactioningVtbl;

    interface ICSPNodeTransactioning
    {
        CONST_VTBL struct ICSPNodeTransactioningVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICSPNodeTransactioning_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICSPNodeTransactioning_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICSPNodeTransactioning_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICSPNodeTransactioning_PersistRollbackAddState(This,puriChild,DataType,varValue,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackAddState(This,puriChild,DataType,varValue,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackCopyState(This,puriDestination,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackCopyState(This,puriDestination,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackDetachChildState(This,puriChild,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackDetachChildState(This,puriChild,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackDeleteState(This,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackDeleteState(This,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackExecuteState(This,varUserData,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackExecuteState(This,varUserData,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackMoveState(This,puriDestination,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackMoveState(This,puriDestination,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackSetValueState(This,varValue,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackSetValueState(This,varValue,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackSetPropertyState(This,guidProperty,varValue,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackSetPropertyState(This,guidProperty,varValue,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_PersistRollbackDeletePropertyState(This,guidProperty,pRollbackStream,pUninstallStream)	\
    (This)->lpVtbl -> PersistRollbackDeletePropertyState(This,guidProperty,pRollbackStream,pUninstallStream)

#define ICSPNodeTransactioning_RollbackAdd(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackAdd(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackCopy(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackCopy(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackDetachChild(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackDetachChild(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackDelete(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackDelete(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackExecute(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackExecute(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackMove(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackMove(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackSetValue(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackSetValue(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackSetProperty(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackSetProperty(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_RollbackDeleteProperty(This,pUndoStream,fRecoveryRollback)	\
    (This)->lpVtbl -> RollbackDeleteProperty(This,pUndoStream,fRecoveryRollback)

#define ICSPNodeTransactioning_Commit(This)	\
    (This)->lpVtbl -> Commit(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackAddState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
    /* [in] */ CFG_DATATYPE DataType,
    /* [in] */ VARIANT varValue,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackAddState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackCopyState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackCopyState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackDetachChildState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriChild,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackDetachChildState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackDeleteState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackDeleteState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackExecuteState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ VARIANT varUserData,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackExecuteState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackMoveState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriDestination,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackMoveState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackSetValueState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ VARIANT varValue,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackSetValueState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackSetPropertyState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty,
    /* [in] */ VARIANT varValue,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackSetPropertyState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_PersistRollbackDeletePropertyState_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ REFGUID guidProperty,
    /* [in] */ ISequentialStream __RPC_FAR *pRollbackStream,
    /* [in] */ ISequentialStream __RPC_FAR *pUninstallStream);


void __RPC_STUB ICSPNodeTransactioning_PersistRollbackDeletePropertyState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackAdd_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackAdd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackCopy_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackDetachChild_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackDetachChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackDelete_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackDelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackExecute_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackExecute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackMove_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackSetValue_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackSetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackSetProperty_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackSetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_RollbackDeleteProperty_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pUndoStream,
    /* [in] */ BOOL fRecoveryRollback);


void __RPC_STUB ICSPNodeTransactioning_RollbackDeleteProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPNodeTransactioning_Commit_Proxy( 
    ICSPNodeTransactioning __RPC_FAR * This);


void __RPC_STUB ICSPNodeTransactioning_Commit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICSPNodeTransactioning_INTERFACE_DEFINED__ */


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


