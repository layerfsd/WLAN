
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:21:59 2008
 */
/* Compiler settings for .\cfgmgr.idl:
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

#ifndef __cfgmgr_h__
#define __cfgmgr_h__

/* Forward Declarations */ 

#ifndef __IConfigTransaction_FWD_DEFINED__
#define __IConfigTransaction_FWD_DEFINED__
typedef interface IConfigTransaction IConfigTransaction;
#endif 	/* __IConfigTransaction_FWD_DEFINED__ */


#ifndef __IConfigSession_FWD_DEFINED__
#define __IConfigSession_FWD_DEFINED__
typedef interface IConfigSession IConfigSession;
#endif 	/* __IConfigSession_FWD_DEFINED__ */


#ifndef __IConfigServiceProviderBase_FWD_DEFINED__
#define __IConfigServiceProviderBase_FWD_DEFINED__
typedef interface IConfigServiceProviderBase IConfigServiceProviderBase;
#endif 	/* __IConfigServiceProviderBase_FWD_DEFINED__ */


#ifndef __IConfigServiceProviderRaw_FWD_DEFINED__
#define __IConfigServiceProviderRaw_FWD_DEFINED__
typedef interface IConfigServiceProviderRaw IConfigServiceProviderRaw;
#endif 	/* __IConfigServiceProviderRaw_FWD_DEFINED__ */


#ifndef __IConfigServiceProvider_FWD_DEFINED__
#define __IConfigServiceProvider_FWD_DEFINED__
typedef interface IConfigServiceProvider IConfigServiceProvider;
#endif 	/* __IConfigServiceProvider_FWD_DEFINED__ */


#ifndef __IConfigManager_FWD_DEFINED__
#define __IConfigManager_FWD_DEFINED__
typedef interface IConfigManager IConfigManager;
#endif 	/* __IConfigManager_FWD_DEFINED__ */


#ifndef __IConfigMetabaseEntry_FWD_DEFINED__
#define __IConfigMetabaseEntry_FWD_DEFINED__
typedef interface IConfigMetabaseEntry IConfigMetabaseEntry;
#endif 	/* __IConfigMetabaseEntry_FWD_DEFINED__ */


#ifndef __IConfigMetabase_FWD_DEFINED__
#define __IConfigMetabase_FWD_DEFINED__
typedef interface IConfigMetabase IConfigMetabase;
#endif 	/* __IConfigMetabase_FWD_DEFINED__ */


#ifndef __IEnumConfigMetabaseEntryLocales_FWD_DEFINED__
#define __IEnumConfigMetabaseEntryLocales_FWD_DEFINED__
typedef interface IEnumConfigMetabaseEntryLocales IEnumConfigMetabaseEntryLocales;
#endif 	/* __IEnumConfigMetabaseEntryLocales_FWD_DEFINED__ */


#ifndef __IConfigMetabaseEntry2_FWD_DEFINED__
#define __IConfigMetabaseEntry2_FWD_DEFINED__
typedef interface IConfigMetabaseEntry2 IConfigMetabaseEntry2;
#endif 	/* __IConfigMetabaseEntry2_FWD_DEFINED__ */


#ifndef __IConfigUninstallHost_FWD_DEFINED__
#define __IConfigUninstallHost_FWD_DEFINED__
typedef interface IConfigUninstallHost IConfigUninstallHost;
#endif 	/* __IConfigUninstallHost_FWD_DEFINED__ */


#ifndef __IConfigInstallResult_FWD_DEFINED__
#define __IConfigInstallResult_FWD_DEFINED__
typedef interface IConfigInstallResult IConfigInstallResult;
#endif 	/* __IConfigInstallResult_FWD_DEFINED__ */


#ifndef __CConfigManager_FWD_DEFINED__
#define __CConfigManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class CConfigManager CConfigManager;
#else
typedef struct CConfigManager CConfigManager;
#endif /* __cplusplus */

#endif 	/* __CConfigManager_FWD_DEFINED__ */


#ifndef __CConfigMetabase_FWD_DEFINED__
#define __CConfigMetabase_FWD_DEFINED__

#ifdef __cplusplus
typedef class CConfigMetabase CConfigMetabase;
#else
typedef struct CConfigMetabase CConfigMetabase;
#endif /* __cplusplus */

#endif 	/* __CConfigMetabase_FWD_DEFINED__ */


#ifndef __CConfigMetabaseEntry_FWD_DEFINED__
#define __CConfigMetabaseEntry_FWD_DEFINED__

#ifdef __cplusplus
typedef class CConfigMetabaseEntry CConfigMetabaseEntry;
#else
typedef struct CConfigMetabaseEntry CConfigMetabaseEntry;
#endif /* __cplusplus */

#endif 	/* __CConfigMetabaseEntry_FWD_DEFINED__ */


#ifndef __CConfigMetabaseEntry2_FWD_DEFINED__
#define __CConfigMetabaseEntry2_FWD_DEFINED__

#ifdef __cplusplus
typedef class CConfigMetabaseEntry2 CConfigMetabaseEntry2;
#else
typedef struct CConfigMetabaseEntry2 CConfigMetabaseEntry2;
#endif /* __cplusplus */

#endif 	/* __CConfigMetabaseEntry2_FWD_DEFINED__ */


#ifndef __CConfigUninstallHost_FWD_DEFINED__
#define __CConfigUninstallHost_FWD_DEFINED__

#ifdef __cplusplus
typedef class CConfigUninstallHost CConfigUninstallHost;
#else
typedef struct CConfigUninstallHost CConfigUninstallHost;
#endif /* __cplusplus */

#endif 	/* __CConfigUninstallHost_FWD_DEFINED__ */


/* header files for imported files */
#include "msxml2.h"
#include "cfgdata.h"
#include "cmgruri.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_cfgmgr_0000 */
/* [local] */ 

#pragma once
#include <cfgmgrapi.h>
#define CONFIG_S_METABASEQUERY           _HRESULT_TYPEDEF_(0x00042003)
#define CONFIG_S_NOTFOUND                _HRESULT_TYPEDEF_(0x00042006)
#define CONFIG_S_REBOOTREQUIRED          _HRESULT_TYPEDEF_(0x00042010)
#define CONFIG_E_CABVERSIONINVALID       _HRESULT_TYPEDEF_(0x80042011)
#define CONFIG_E_CABPLATFORMINVALID      _HRESULT_TYPEDEF_(0x80042012)
#define CONFIG_E_CABPROCESSORINVALID     _HRESULT_TYPEDEF_(0x80042013)
#define CONFIG_E_CABOSVERSIONINVALID     _HRESULT_TYPEDEF_(0x80042014)
#define CONFIG_E_PREPROCESSINSTALL       _HRESULT_TYPEDEF_(0x80042015)
#define CONFIG_E_CABPLATFORMNOTSUPPORTED _HRESULT_TYPEDEF_(0x80042016)
#define CONFIG_E_PREPROCESSUNINSTALL     _HRESULT_TYPEDEF_(0x80042017)
#define CONFIG_E_NEEDUNINSTALLHOST       _HRESULT_TYPEDEF_(0x80042018)
#define SECROLE_NONE                     0x00
#define SECROLE_OEM                      0x02
#define SECROLE_CARRIER                  0x04
#define SECROLE_MANAGER                  0x08
#define SECROLE_USER_AUTH                0x10
#define SECROLE_ENTERPRISE               0x20
#define SECROLE_USER_UNAUTH              0x40
#define SECROLE_CARRIER_TPS              0x80
#define SECROLE_KNOWN_PPG                0x100
#define SECROLE_TRUSTED_PPG              0x200
#define SECROLE_PPG_AUTH                 0x400
#define SECROLE_PPG_TRUSTED              0x800
#define SECROLE_ANY_PUSH_SOURCE          0x1000
#define AUTHENTICATION_ROLES             (SECROLE_OEM|SECROLE_CARRIER|SECROLE_MANAGER|SECROLE_USER_AUTH|SECROLE_ENTERPRISE|SECROLE_CARRIER_TPS|SECROLE_USER_UNAUTH)
#define ORIGIN_ROLES                     (SECROLE_CARRIER_TPS|SECROLE_KNOWN_PPG|SECROLE_TRUSTED_PPG|SECROLE_PPG_AUTH|SECROLE_PPG_TRUSTED|SECROLE_ANY_PUSH_SOURCE)
#define METABASE_QF_EXACT          0x00000001
#define METABASE_QF_NEAR           0x00000002
#define METABASE_QF_SUB_PATH       0x00000004
#define METABASE_QF_ALL            (METABASE_QF_EXACT | METABASE_QF_NEAR | METABASE_QF_SUB_PATH)
#define OPTIONS_EQUALS_SEPARATOR         (0xF001)
#define OPTIONS_EQUALS_SEPARATOR_STRING  TEXT("\xF001")
#define OPTIONS_ITEM_SEPARATOR           (0xF000)
#define OPTIONS_ITEM_SEPARATOR_STRING    TEXT("\xF000")
typedef 
enum ConfigTransactioningMode
    {	CFGTRANSACTIONING_CFGMGR	= 0,
	CFGTRANSACTIONING_CSPINTERNAL	= CFGTRANSACTIONING_CFGMGR + 1,
	CFGTRANSACTIONING_HYBRID	= CFGTRANSACTIONING_CSPINTERNAL + 1,
	CFGTRANSACTIONING_CSPINTERNALHEADER	= CFGTRANSACTIONING_HYBRID + 1,
	CFGTRANSACTIONING_NOTALLOWED	= CFGTRANSACTIONING_CSPINTERNALHEADER + 1
    }	CONFIGTRANSACTIONINGMODE;

typedef enum ConfigTransactioningMode __RPC_FAR *PCONFIGTRANSACTIONINGMODE;



extern RPC_IF_HANDLE __MIDL_itf_cfgmgr_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cfgmgr_0000_v0_0_s_ifspec;

#ifndef __IConfigTransaction_INTERFACE_DEFINED__
#define __IConfigTransaction_INTERFACE_DEFINED__

/* interface IConfigTransaction */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigTransaction;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D83013B6-9EC6-464f-A188-1854CD5B4E99")
    IConfigTransaction : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Begin( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Commit( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Rollback( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RecoveryRollback( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUninstallNode( 
            /* [in] */ IXMLDOMDocument __RPC_FAR *pDoc,
            /* [out] */ IXMLDOMNode __RPC_FAR *__RPC_FAR *ppNode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigTransactionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigTransaction __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigTransaction __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigTransaction __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Begin )( 
            IConfigTransaction __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Commit )( 
            IConfigTransaction __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Rollback )( 
            IConfigTransaction __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RecoveryRollback )( 
            IConfigTransaction __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUninstallNode )( 
            IConfigTransaction __RPC_FAR * This,
            /* [in] */ IXMLDOMDocument __RPC_FAR *pDoc,
            /* [out] */ IXMLDOMNode __RPC_FAR *__RPC_FAR *ppNode);
        
        END_INTERFACE
    } IConfigTransactionVtbl;

    interface IConfigTransaction
    {
        CONST_VTBL struct IConfigTransactionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigTransaction_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigTransaction_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigTransaction_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigTransaction_Begin(This)	\
    (This)->lpVtbl -> Begin(This)

#define IConfigTransaction_Commit(This)	\
    (This)->lpVtbl -> Commit(This)

#define IConfigTransaction_Rollback(This)	\
    (This)->lpVtbl -> Rollback(This)

#define IConfigTransaction_RecoveryRollback(This)	\
    (This)->lpVtbl -> RecoveryRollback(This)

#define IConfigTransaction_GetUninstallNode(This,pDoc,ppNode)	\
    (This)->lpVtbl -> GetUninstallNode(This,pDoc,ppNode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigTransaction_Begin_Proxy( 
    IConfigTransaction __RPC_FAR * This);


void __RPC_STUB IConfigTransaction_Begin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigTransaction_Commit_Proxy( 
    IConfigTransaction __RPC_FAR * This);


void __RPC_STUB IConfigTransaction_Commit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigTransaction_Rollback_Proxy( 
    IConfigTransaction __RPC_FAR * This);


void __RPC_STUB IConfigTransaction_Rollback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigTransaction_RecoveryRollback_Proxy( 
    IConfigTransaction __RPC_FAR * This);


void __RPC_STUB IConfigTransaction_RecoveryRollback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigTransaction_GetUninstallNode_Proxy( 
    IConfigTransaction __RPC_FAR * This,
    /* [in] */ IXMLDOMDocument __RPC_FAR *pDoc,
    /* [out] */ IXMLDOMNode __RPC_FAR *__RPC_FAR *ppNode);


void __RPC_STUB IConfigTransaction_GetUninstallNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigTransaction_INTERFACE_DEFINED__ */


#ifndef __IConfigSession_INTERFACE_DEFINED__
#define __IConfigSession_INTERFACE_DEFINED__

/* interface IConfigSession */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D83013B7-9EC6-464f-A188-1854CD5B4E99")
    IConfigSession : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSecurityRoleMask( 
            /* [out] */ DWORD __RPC_FAR *pdwRoleMask) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBaseRoleMask( 
            /* [out] */ DWORD __RPC_FAR *pdwMappedRoles) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProcessingFlags( 
            /* [out] */ DWORD __RPC_FAR *pdwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryHost( 
            /* [in] */ REFIID riid,
            /* [out] */ LPUNKNOWN __RPC_FAR *ppUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSessionVariable( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ResolveSessionVariables( 
            /* [in] */ BSTR szText,
            /* [out] */ BSTR __RPC_FAR *pbstrResolvedText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigSession __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigSession __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigSession __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSecurityRoleMask )( 
            IConfigSession __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwRoleMask);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBaseRoleMask )( 
            IConfigSession __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwMappedRoles);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProcessingFlags )( 
            IConfigSession __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryHost )( 
            IConfigSession __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ LPUNKNOWN __RPC_FAR *ppUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSessionVariable )( 
            IConfigSession __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResolveSessionVariables )( 
            IConfigSession __RPC_FAR * This,
            /* [in] */ BSTR szText,
            /* [out] */ BSTR __RPC_FAR *pbstrResolvedText);
        
        END_INTERFACE
    } IConfigSessionVtbl;

    interface IConfigSession
    {
        CONST_VTBL struct IConfigSessionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigSession_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigSession_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigSession_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigSession_GetSecurityRoleMask(This,pdwRoleMask)	\
    (This)->lpVtbl -> GetSecurityRoleMask(This,pdwRoleMask)

#define IConfigSession_GetBaseRoleMask(This,pdwMappedRoles)	\
    (This)->lpVtbl -> GetBaseRoleMask(This,pdwMappedRoles)

#define IConfigSession_GetProcessingFlags(This,pdwFlags)	\
    (This)->lpVtbl -> GetProcessingFlags(This,pdwFlags)

#define IConfigSession_QueryHost(This,riid,ppUnk)	\
    (This)->lpVtbl -> QueryHost(This,riid,ppUnk)

#define IConfigSession_SetSessionVariable(This,bstrName,bstrValue)	\
    (This)->lpVtbl -> SetSessionVariable(This,bstrName,bstrValue)

#define IConfigSession_ResolveSessionVariables(This,szText,pbstrResolvedText)	\
    (This)->lpVtbl -> ResolveSessionVariables(This,szText,pbstrResolvedText)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigSession_GetSecurityRoleMask_Proxy( 
    IConfigSession __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwRoleMask);


void __RPC_STUB IConfigSession_GetSecurityRoleMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession_GetBaseRoleMask_Proxy( 
    IConfigSession __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwMappedRoles);


void __RPC_STUB IConfigSession_GetBaseRoleMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession_GetProcessingFlags_Proxy( 
    IConfigSession __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwFlags);


void __RPC_STUB IConfigSession_GetProcessingFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession_QueryHost_Proxy( 
    IConfigSession __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [out] */ LPUNKNOWN __RPC_FAR *ppUnk);


void __RPC_STUB IConfigSession_QueryHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession_SetSessionVariable_Proxy( 
    IConfigSession __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrValue);


void __RPC_STUB IConfigSession_SetSessionVariable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigSession_ResolveSessionVariables_Proxy( 
    IConfigSession __RPC_FAR * This,
    /* [in] */ BSTR szText,
    /* [out] */ BSTR __RPC_FAR *pbstrResolvedText);


void __RPC_STUB IConfigSession_ResolveSessionVariables_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigSession_INTERFACE_DEFINED__ */


#ifndef __IConfigServiceProviderBase_INTERFACE_DEFINED__
#define __IConfigServiceProviderBase_INTERFACE_DEFINED__

/* interface IConfigServiceProviderBase */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigServiceProviderBase;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D83013B3-9EC6-464f-A188-1854CD5B4E99")
    IConfigServiceProviderBase : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryTransactioningMode( 
            /* [out] */ PCONFIGTRANSACTIONINGMODE pTransactioningMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelProcessing( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginProcessing( 
            /* [in] */ IConfigSession __RPC_FAR *pSession) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndProcessing( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginTransaction( 
            /* [out] */ IConfigTransaction __RPC_FAR *__RPC_FAR *ppTransaction) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigServiceProviderBaseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigServiceProviderBase __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigServiceProviderBase __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigServiceProviderBase __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryTransactioningMode )( 
            IConfigServiceProviderBase __RPC_FAR * This,
            /* [out] */ PCONFIGTRANSACTIONINGMODE pTransactioningMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelProcessing )( 
            IConfigServiceProviderBase __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginProcessing )( 
            IConfigServiceProviderBase __RPC_FAR * This,
            /* [in] */ IConfigSession __RPC_FAR *pSession);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndProcessing )( 
            IConfigServiceProviderBase __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginTransaction )( 
            IConfigServiceProviderBase __RPC_FAR * This,
            /* [out] */ IConfigTransaction __RPC_FAR *__RPC_FAR *ppTransaction);
        
        END_INTERFACE
    } IConfigServiceProviderBaseVtbl;

    interface IConfigServiceProviderBase
    {
        CONST_VTBL struct IConfigServiceProviderBaseVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigServiceProviderBase_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigServiceProviderBase_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigServiceProviderBase_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigServiceProviderBase_QueryTransactioningMode(This,pTransactioningMode)	\
    (This)->lpVtbl -> QueryTransactioningMode(This,pTransactioningMode)

#define IConfigServiceProviderBase_CancelProcessing(This)	\
    (This)->lpVtbl -> CancelProcessing(This)

#define IConfigServiceProviderBase_BeginProcessing(This,pSession)	\
    (This)->lpVtbl -> BeginProcessing(This,pSession)

#define IConfigServiceProviderBase_EndProcessing(This)	\
    (This)->lpVtbl -> EndProcessing(This)

#define IConfigServiceProviderBase_BeginTransaction(This,ppTransaction)	\
    (This)->lpVtbl -> BeginTransaction(This,ppTransaction)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigServiceProviderBase_QueryTransactioningMode_Proxy( 
    IConfigServiceProviderBase __RPC_FAR * This,
    /* [out] */ PCONFIGTRANSACTIONINGMODE pTransactioningMode);


void __RPC_STUB IConfigServiceProviderBase_QueryTransactioningMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProviderBase_CancelProcessing_Proxy( 
    IConfigServiceProviderBase __RPC_FAR * This);


void __RPC_STUB IConfigServiceProviderBase_CancelProcessing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProviderBase_BeginProcessing_Proxy( 
    IConfigServiceProviderBase __RPC_FAR * This,
    /* [in] */ IConfigSession __RPC_FAR *pSession);


void __RPC_STUB IConfigServiceProviderBase_BeginProcessing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProviderBase_EndProcessing_Proxy( 
    IConfigServiceProviderBase __RPC_FAR * This);


void __RPC_STUB IConfigServiceProviderBase_EndProcessing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProviderBase_BeginTransaction_Proxy( 
    IConfigServiceProviderBase __RPC_FAR * This,
    /* [out] */ IConfigTransaction __RPC_FAR *__RPC_FAR *ppTransaction);


void __RPC_STUB IConfigServiceProviderBase_BeginTransaction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigServiceProviderBase_INTERFACE_DEFINED__ */


#ifndef __IConfigServiceProviderRaw_INTERFACE_DEFINED__
#define __IConfigServiceProviderRaw_INTERFACE_DEFINED__

/* interface IConfigServiceProviderRaw */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigServiceProviderRaw;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D83013B5-9EC6-464f-A188-1854CD5B4E99")
    IConfigServiceProviderRaw : public IConfigServiceProviderBase
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ProcessNode( 
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PopulateMetadata( 
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigServiceProviderRawVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigServiceProviderRaw __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigServiceProviderRaw __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigServiceProviderRaw __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryTransactioningMode )( 
            IConfigServiceProviderRaw __RPC_FAR * This,
            /* [out] */ PCONFIGTRANSACTIONINGMODE pTransactioningMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelProcessing )( 
            IConfigServiceProviderRaw __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginProcessing )( 
            IConfigServiceProviderRaw __RPC_FAR * This,
            /* [in] */ IConfigSession __RPC_FAR *pSession);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndProcessing )( 
            IConfigServiceProviderRaw __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginTransaction )( 
            IConfigServiceProviderRaw __RPC_FAR * This,
            /* [out] */ IConfigTransaction __RPC_FAR *__RPC_FAR *ppTransaction);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessNode )( 
            IConfigServiceProviderRaw __RPC_FAR * This,
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PopulateMetadata )( 
            IConfigServiceProviderRaw __RPC_FAR * This,
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML);
        
        END_INTERFACE
    } IConfigServiceProviderRawVtbl;

    interface IConfigServiceProviderRaw
    {
        CONST_VTBL struct IConfigServiceProviderRawVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigServiceProviderRaw_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigServiceProviderRaw_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigServiceProviderRaw_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigServiceProviderRaw_QueryTransactioningMode(This,pTransactioningMode)	\
    (This)->lpVtbl -> QueryTransactioningMode(This,pTransactioningMode)

#define IConfigServiceProviderRaw_CancelProcessing(This)	\
    (This)->lpVtbl -> CancelProcessing(This)

#define IConfigServiceProviderRaw_BeginProcessing(This,pSession)	\
    (This)->lpVtbl -> BeginProcessing(This,pSession)

#define IConfigServiceProviderRaw_EndProcessing(This)	\
    (This)->lpVtbl -> EndProcessing(This)

#define IConfigServiceProviderRaw_BeginTransaction(This,ppTransaction)	\
    (This)->lpVtbl -> BeginTransaction(This,ppTransaction)


#define IConfigServiceProviderRaw_ProcessNode(This,nodeXML)	\
    (This)->lpVtbl -> ProcessNode(This,nodeXML)

#define IConfigServiceProviderRaw_PopulateMetadata(This,nodeXML)	\
    (This)->lpVtbl -> PopulateMetadata(This,nodeXML)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigServiceProviderRaw_ProcessNode_Proxy( 
    IConfigServiceProviderRaw __RPC_FAR * This,
    /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML);


void __RPC_STUB IConfigServiceProviderRaw_ProcessNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProviderRaw_PopulateMetadata_Proxy( 
    IConfigServiceProviderRaw __RPC_FAR * This,
    /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML);


void __RPC_STUB IConfigServiceProviderRaw_PopulateMetadata_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigServiceProviderRaw_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_cfgmgr_0154 */
/* [local] */ 

#define CFGFLAG_RECURSIVE            0x0004
#define CFGFLAG_RECOVERYROLLBACK     0x0008
#define CFGFLAG_PROCESSINSECURE      0x0010
#define CFGFLAG_IN_UNINSTALL         0x0020
#define CFGFLAG_IN_TRANSACTIONING    0x1000
#define CFGFLAG_IN_ROLLBACK          0x2000


extern RPC_IF_HANDLE __MIDL_itf_cfgmgr_0154_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cfgmgr_0154_v0_0_s_ifspec;

#ifndef __IConfigServiceProvider_INTERFACE_DEFINED__
#define __IConfigServiceProvider_INTERFACE_DEFINED__

/* interface IConfigServiceProvider */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigServiceProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D83013B4-9EC6-464f-A188-1854CD5B4E99")
    IConfigServiceProvider : public IConfigServiceProviderBase
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE HandleParm( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HandleNoParm( 
            /* [in] */ BSTR bstrName,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HandleParmQuery( 
            /* [in] */ BSTR bstrName,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HandleCharacteristic( 
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [out] */ DWORD __RPC_FAR *pdwUserDataChild,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndCharacteristic( 
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwUserDataChild,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HandleNoCharacteristic( 
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HandleCharacteristicQuery( 
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetErrorDescription( 
            /* [out] */ BSTR __RPC_FAR *pbstrError) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigServiceProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigServiceProvider __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigServiceProvider __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryTransactioningMode )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [out] */ PCONFIGTRANSACTIONINGMODE pTransactioningMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelProcessing )( 
            IConfigServiceProvider __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginProcessing )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ IConfigSession __RPC_FAR *pSession);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndProcessing )( 
            IConfigServiceProvider __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeginTransaction )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [out] */ IConfigTransaction __RPC_FAR *__RPC_FAR *ppTransaction);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleParm )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleNoParm )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleParmQuery )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleCharacteristic )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [out] */ DWORD __RPC_FAR *pdwUserDataChild,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndCharacteristic )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwUserDataChild,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleNoCharacteristic )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleCharacteristicQuery )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [in] */ BSTR bstrType,
            /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwUserData,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetErrorDescription )( 
            IConfigServiceProvider __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pbstrError);
        
        END_INTERFACE
    } IConfigServiceProviderVtbl;

    interface IConfigServiceProvider
    {
        CONST_VTBL struct IConfigServiceProviderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigServiceProvider_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigServiceProvider_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigServiceProvider_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigServiceProvider_QueryTransactioningMode(This,pTransactioningMode)	\
    (This)->lpVtbl -> QueryTransactioningMode(This,pTransactioningMode)

#define IConfigServiceProvider_CancelProcessing(This)	\
    (This)->lpVtbl -> CancelProcessing(This)

#define IConfigServiceProvider_BeginProcessing(This,pSession)	\
    (This)->lpVtbl -> BeginProcessing(This,pSession)

#define IConfigServiceProvider_EndProcessing(This)	\
    (This)->lpVtbl -> EndProcessing(This)

#define IConfigServiceProvider_BeginTransaction(This,ppTransaction)	\
    (This)->lpVtbl -> BeginTransaction(This,ppTransaction)


#define IConfigServiceProvider_HandleParm(This,bstrName,bstrValue,pNode,bstrPath,dwUserData,dwFlags)	\
    (This)->lpVtbl -> HandleParm(This,bstrName,bstrValue,pNode,bstrPath,dwUserData,dwFlags)

#define IConfigServiceProvider_HandleNoParm(This,bstrName,pNode,bstrPath,dwUserData,dwFlags)	\
    (This)->lpVtbl -> HandleNoParm(This,bstrName,pNode,bstrPath,dwUserData,dwFlags)

#define IConfigServiceProvider_HandleParmQuery(This,bstrName,pvarValue,pNode,bstrPath,dwUserData,dwFlags)	\
    (This)->lpVtbl -> HandleParmQuery(This,bstrName,pvarValue,pNode,bstrPath,dwUserData,dwFlags)

#define IConfigServiceProvider_HandleCharacteristic(This,bstrType,pNode,bstrPath,dwUserData,pdwUserDataChild,dwFlags)	\
    (This)->lpVtbl -> HandleCharacteristic(This,bstrType,pNode,bstrPath,dwUserData,pdwUserDataChild,dwFlags)

#define IConfigServiceProvider_EndCharacteristic(This,bstrType,pNode,bstrPath,dwUserData,dwUserDataChild,dwFlags)	\
    (This)->lpVtbl -> EndCharacteristic(This,bstrType,pNode,bstrPath,dwUserData,dwUserDataChild,dwFlags)

#define IConfigServiceProvider_HandleNoCharacteristic(This,bstrType,pNode,bstrPath,dwUserData,dwFlags)	\
    (This)->lpVtbl -> HandleNoCharacteristic(This,bstrType,pNode,bstrPath,dwUserData,dwFlags)

#define IConfigServiceProvider_HandleCharacteristicQuery(This,bstrType,pNode,bstrPath,dwUserData,dwFlags)	\
    (This)->lpVtbl -> HandleCharacteristicQuery(This,bstrType,pNode,bstrPath,dwUserData,dwFlags)

#define IConfigServiceProvider_GetErrorDescription(This,pbstrError)	\
    (This)->lpVtbl -> GetErrorDescription(This,pbstrError)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigServiceProvider_HandleParm_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrValue,
    /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwUserData,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigServiceProvider_HandleParm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider_HandleNoParm_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwUserData,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigServiceProvider_HandleNoParm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider_HandleParmQuery_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [out][in] */ VARIANT __RPC_FAR *pvarValue,
    /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwUserData,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigServiceProvider_HandleParmQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider_HandleCharacteristic_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwUserData,
    /* [out] */ DWORD __RPC_FAR *pdwUserDataChild,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigServiceProvider_HandleCharacteristic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider_EndCharacteristic_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwUserData,
    /* [in] */ DWORD dwUserDataChild,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigServiceProvider_EndCharacteristic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider_HandleNoCharacteristic_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwUserData,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigServiceProvider_HandleNoCharacteristic_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider_HandleCharacteristicQuery_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [in] */ BSTR bstrType,
    /* [in] */ IXMLDOMElement __RPC_FAR *pNode,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwUserData,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigServiceProvider_HandleCharacteristicQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigServiceProvider_GetErrorDescription_Proxy( 
    IConfigServiceProvider __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pbstrError);


void __RPC_STUB IConfigServiceProvider_GetErrorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigServiceProvider_INTERFACE_DEFINED__ */


#ifndef __IConfigManager_INTERFACE_DEFINED__
#define __IConfigManager_INTERFACE_DEFINED__

/* interface IConfigManager */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("483B6126-59B4-493b-A9E3-972BDAFEB341")
    IConfigManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ProcessDocument( 
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessXML( 
            /* [out][in] */ BSTR __RPC_FAR *pbstrXML,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveMetadata( 
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelProcessing( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSecurityRoleMask( 
            /* [in] */ DWORD dwRoleMask) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetHost( 
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRecoveryFilePath( 
            /* [in] */ BSTR bstrFilePath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRecoveryFileDeletion( 
            /* [in] */ BOOL fDelete) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUninstallDocument( 
            /* [out] */ IXMLDOMDocument __RPC_FAR *__RPC_FAR *ppDoc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPretransactioningResult( 
            /* [out] */ HRESULT __RPC_FAR *pHR) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessDocument )( 
            IConfigManager __RPC_FAR * This,
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessXML )( 
            IConfigManager __RPC_FAR * This,
            /* [out][in] */ BSTR __RPC_FAR *pbstrXML,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveMetadata )( 
            IConfigManager __RPC_FAR * This,
            /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelProcessing )( 
            IConfigManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSecurityRoleMask )( 
            IConfigManager __RPC_FAR * This,
            /* [in] */ DWORD dwRoleMask);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHost )( 
            IConfigManager __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRecoveryFilePath )( 
            IConfigManager __RPC_FAR * This,
            /* [in] */ BSTR bstrFilePath);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRecoveryFileDeletion )( 
            IConfigManager __RPC_FAR * This,
            /* [in] */ BOOL fDelete);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUninstallDocument )( 
            IConfigManager __RPC_FAR * This,
            /* [out] */ IXMLDOMDocument __RPC_FAR *__RPC_FAR *ppDoc);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPretransactioningResult )( 
            IConfigManager __RPC_FAR * This,
            /* [out] */ HRESULT __RPC_FAR *pHR);
        
        END_INTERFACE
    } IConfigManagerVtbl;

    interface IConfigManager
    {
        CONST_VTBL struct IConfigManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigManager_ProcessDocument(This,nodeXML,dwFlags)	\
    (This)->lpVtbl -> ProcessDocument(This,nodeXML,dwFlags)

#define IConfigManager_ProcessXML(This,pbstrXML,dwFlags)	\
    (This)->lpVtbl -> ProcessXML(This,pbstrXML,dwFlags)

#define IConfigManager_RemoveMetadata(This,nodeXML)	\
    (This)->lpVtbl -> RemoveMetadata(This,nodeXML)

#define IConfigManager_CancelProcessing(This)	\
    (This)->lpVtbl -> CancelProcessing(This)

#define IConfigManager_SetSecurityRoleMask(This,dwRoleMask)	\
    (This)->lpVtbl -> SetSecurityRoleMask(This,dwRoleMask)

#define IConfigManager_SetHost(This,pUnk)	\
    (This)->lpVtbl -> SetHost(This,pUnk)

#define IConfigManager_SetRecoveryFilePath(This,bstrFilePath)	\
    (This)->lpVtbl -> SetRecoveryFilePath(This,bstrFilePath)

#define IConfigManager_SetRecoveryFileDeletion(This,fDelete)	\
    (This)->lpVtbl -> SetRecoveryFileDeletion(This,fDelete)

#define IConfigManager_GetUninstallDocument(This,ppDoc)	\
    (This)->lpVtbl -> GetUninstallDocument(This,ppDoc)

#define IConfigManager_GetPretransactioningResult(This,pHR)	\
    (This)->lpVtbl -> GetPretransactioningResult(This,pHR)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigManager_ProcessDocument_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigManager_ProcessDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_ProcessXML_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [out][in] */ BSTR __RPC_FAR *pbstrXML,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigManager_ProcessXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_RemoveMetadata_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [in] */ IXMLDOMNode __RPC_FAR *nodeXML);


void __RPC_STUB IConfigManager_RemoveMetadata_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_CancelProcessing_Proxy( 
    IConfigManager __RPC_FAR * This);


void __RPC_STUB IConfigManager_CancelProcessing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_SetSecurityRoleMask_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [in] */ DWORD dwRoleMask);


void __RPC_STUB IConfigManager_SetSecurityRoleMask_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_SetHost_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB IConfigManager_SetHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_SetRecoveryFilePath_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [in] */ BSTR bstrFilePath);


void __RPC_STUB IConfigManager_SetRecoveryFilePath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_SetRecoveryFileDeletion_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [in] */ BOOL fDelete);


void __RPC_STUB IConfigManager_SetRecoveryFileDeletion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_GetUninstallDocument_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [out] */ IXMLDOMDocument __RPC_FAR *__RPC_FAR *ppDoc);


void __RPC_STUB IConfigManager_GetUninstallDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager_GetPretransactioningResult_Proxy( 
    IConfigManager __RPC_FAR * This,
    /* [out] */ HRESULT __RPC_FAR *pHR);


void __RPC_STUB IConfigManager_GetPretransactioningResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigManager_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_cfgmgr_0156 */
/* [local] */ 

typedef 
enum ConfigNodeType
    {	CFGNODE_PARM	= 0,
	CFGNODE_NOPARM	= CFGNODE_PARM + 1,
	CFGNODE_PARM_QUERY	= CFGNODE_NOPARM + 1,
	CFGNODE_CHARACTERISTIC	= CFGNODE_PARM_QUERY + 1,
	CFGNODE_NOCHARACTERISTIC	= CFGNODE_CHARACTERISTIC + 1,
	CFGNODE_CHARACTERISTIC_QUERY	= CFGNODE_NOCHARACTERISTIC + 1,
	CFGNODE_PARM_ERROR	= CFGNODE_CHARACTERISTIC_QUERY + 1,
	CFGNODE_NOPARM_ERROR	= CFGNODE_PARM_ERROR + 1,
	CFGNODE_PARM_QUERY_ERROR	= CFGNODE_NOPARM_ERROR + 1,
	CFGNODE_CHARACTERISTIC_ERROR	= CFGNODE_PARM_QUERY_ERROR + 1,
	CFGNODE_NOCHARACTERISTIC_ERROR	= CFGNODE_CHARACTERISTIC_ERROR + 1,
	CFGNODE_CHARACTERISTIC_QUERY_ERROR	= CFGNODE_NOCHARACTERISTIC_ERROR + 1,
	CFGNODE_UNKNOWN	= CFGNODE_CHARACTERISTIC_QUERY_ERROR + 1
    }	CFGNODE;

typedef enum ConfigNodeType __RPC_FAR *PCFGNODE;

typedef 
enum ConfigParmAttributeType
    {	CFGPARM_ATTR_NAME	= 0,
	CFGPARM_ATTR_VALUE	= CFGPARM_ATTR_NAME + 1,
	CFGPARM_ATTR_DATA_TYPE	= CFGPARM_ATTR_VALUE + 1,
	CFGPARM_ATTR_SEMANTIC_TYPE	= CFGPARM_ATTR_DATA_TYPE + 1,
	CFGPARM_ATTR_MAX_VALUE	= CFGPARM_ATTR_SEMANTIC_TYPE + 1,
	CFGPARM_ATTR_MIN_VALUE	= CFGPARM_ATTR_MAX_VALUE + 1,
	CFGPARM_ATTR_MAX_LENGTH	= CFGPARM_ATTR_MIN_VALUE + 1,
	CFGPARM_ATTR_OPTIONS	= CFGPARM_ATTR_MAX_LENGTH + 1,
	CFGPARM_ATTR_MULTIPLE	= CFGPARM_ATTR_OPTIONS + 1,
	CFGPARM_ATTR_RW_ACCESS	= CFGPARM_ATTR_MULTIPLE + 1,
	CFGPARM_ATTR_LABEL	= CFGPARM_ATTR_RW_ACCESS + 1,
	CFGPARM_ATTR_ACL	= CFGPARM_ATTR_LABEL + 1,
	CFGPARM_ATTR_UNKNOWN	= CFGPARM_ATTR_ACL + 1
    }	CFGPARM_ATTR;

typedef enum ConfigParmAttributeType __RPC_FAR *PCFGPARM_ATTR;

typedef 
enum ConfigSemanticType
    {	CFG_SEMANTICTYPE_EMAIL	= 0,
	CFG_SEMANTICTYPE_FILENAME	= CFG_SEMANTICTYPE_EMAIL + 1,
	CFG_SEMANTICTYPE_TEXT	= CFG_SEMANTICTYPE_FILENAME + 1,
	CFG_SEMANTICTYPE_URL	= CFG_SEMANTICTYPE_TEXT + 1,
	CFG_SEMANTICTYPE_PHONENUMBER	= CFG_SEMANTICTYPE_URL + 1,
	CFG_SEMANTICTYPE_PASSWORD_ALPHANUMERIC	= CFG_SEMANTICTYPE_PHONENUMBER + 1,
	CFG_SEMANTICTYPE_CURRENCY	= CFG_SEMANTICTYPE_PASSWORD_ALPHANUMERIC + 1,
	CFG_SEMANTICTYPE_LABEL	= CFG_SEMANTICTYPE_CURRENCY + 1,
	CFG_SEMANTICTYPE_LINK	= CFG_SEMANTICTYPE_LABEL + 1,
	CFG_SEMANTICTYPE_LEVEL	= CFG_SEMANTICTYPE_LINK + 1,
	CFG_SEMANTICTYPE_SHORTDATETIME	= CFG_SEMANTICTYPE_LEVEL + 1,
	CFG_SEMANTICTYPE_PASSWORD_NUMERIC	= CFG_SEMANTICTYPE_SHORTDATETIME + 1,
	CFG_SEMANTICTYPE_INVISIBLE	= CFG_SEMANTICTYPE_PASSWORD_NUMERIC + 1,
	CFG_SEMANTICTYPE_UNKNOWN	= CFG_SEMANTICTYPE_INVISIBLE + 1
    }	CFG_SEMANTICTYPE;

typedef enum ConfigSemanticType __RPC_FAR *PCFG_SEMANTICTYPE;

typedef 
enum ConfigTranslationType
    {	CFG_TRANSLATIONTYPE_INSTALL	= 0,
	CFG_TRANSLATIONTYPE_FILESYSTEM	= CFG_TRANSLATIONTYPE_INSTALL + 1,
	CFG_TRANSLATIONTYPE_UNKNOWN	= CFG_TRANSLATIONTYPE_FILESYSTEM + 1
    }	CFG_TRANSLATIONTYPE;

typedef enum ConfigTranslationType __RPC_FAR *PCFG_TRANSLATIONTYPE;

typedef 
enum ConfigRWFlags
    {	CFGFLAG_READ_ENABLED	= 1,
	CFGFLAG_WRITE_ENABLED	= 2,
	CFGFLAG_READWRITE	= 3
    }	CFG_RW_ACCESS;

typedef enum ConfigRWFlags __RPC_FAR *PCFG_RW_ACCESS;

#define DATATYPE_PRESENT        0x0001
#define SEMANTICTYPE_PRESENT    0x0002
#define MAXVALUE_PRESENT        0x0004
#define MINVALUE_PRESENT        0x0008
#define MAXLENGTH_PRESENT       0x0010
#define OPTIONS_PRESENT         0x0020
#define LABEL_PRESENT           0x0040
#define MULTIPLE_PRESENT        0x0080
#define RW_ACCESS_PRESENT       0x0100
#define ACCESSROLE_PRESENT      0x0400
#define ACL_PRESENT             0x0800
#define TSTAMP_PRESENT          0x1000
#define SIZE_PRESENT            0x2000


extern RPC_IF_HANDLE __MIDL_itf_cfgmgr_0156_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cfgmgr_0156_v0_0_s_ifspec;

#ifndef __IConfigMetabaseEntry_INTERFACE_DEFINED__
#define __IConfigMetabaseEntry_INTERFACE_DEFINED__

/* interface IConfigMetabaseEntry */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigMetabaseEntry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("516AB1E0-2906-467a-8F3F-2CAB5F3D7B57")
    IConfigMetabaseEntry : public IUnknown
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_DataType( 
            /* [in] */ enum ConfigDataType DataType) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DataType( 
            /* [retval][out] */ enum ConfigDataType __RPC_FAR *pDataType) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SemanticType( 
            /* [in] */ BSTR bstrSemanticType) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SemanticType( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrSemanticType) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_MaxValue( 
            /* [in] */ int iMaxValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MaxValue( 
            /* [retval][out] */ int __RPC_FAR *piMaxValue) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_MinValue( 
            /* [in] */ int iMinValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MinValue( 
            /* [retval][out] */ int __RPC_FAR *piMinValue) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_MaxLength( 
            /* [in] */ int iMaxLength) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MaxLength( 
            /* [retval][out] */ int __RPC_FAR *piMaxLength) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Options( 
            /* [in] */ BSTR bstrOptions) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Options( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOptions) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Multiple( 
            /* [in] */ BOOL fMultiple) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Multiple( 
            /* [retval][out] */ BOOL __RPC_FAR *pfMultiple) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_RWAccess( 
            /* [in] */ DWORD dwRWAccess) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RWAccess( 
            /* [retval][out] */ DWORD __RPC_FAR *pdwRWAccess) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Label( 
            /* [in] */ BSTR bstrLabel) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Label( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLabel) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AccessRole( 
            /* [in] */ DWORD dwAccessRole) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AccessRole( 
            /* [retval][out] */ DWORD __RPC_FAR *pdwAccessRole) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ACL( 
            /* [in] */ BSTR bstrACL) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ACL( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrACL) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SemanticTypeFromMimeType( 
            /* [in] */ BSTR bstrType) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SemanticTypeAsMimeType( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PresenceFlags( 
            /* [retval][out] */ DWORD __RPC_FAR *pfFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigMetabaseEntryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigMetabaseEntry __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigMetabaseEntry __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DataType )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ enum ConfigDataType DataType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DataType )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ enum ConfigDataType __RPC_FAR *pDataType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SemanticType )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ BSTR bstrSemanticType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SemanticType )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrSemanticType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxValue )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ int iMaxValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxValue )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piMaxValue);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MinValue )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ int iMinValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MinValue )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piMinValue);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxLength )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ int iMaxLength);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxLength )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piMaxLength);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Options )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ BSTR bstrOptions);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Options )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOptions);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Multiple )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ BOOL fMultiple);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Multiple )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pfMultiple);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_RWAccess )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ DWORD dwRWAccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RWAccess )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRWAccess);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Label )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ BSTR bstrLabel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Label )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLabel);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AccessRole )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ DWORD dwAccessRole);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AccessRole )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwAccessRole);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ACL )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ BSTR bstrACL);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ACL )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrACL);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SemanticTypeFromMimeType )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [in] */ BSTR bstrType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SemanticTypeAsMimeType )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PresenceFlags )( 
            IConfigMetabaseEntry __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pfFlags);
        
        END_INTERFACE
    } IConfigMetabaseEntryVtbl;

    interface IConfigMetabaseEntry
    {
        CONST_VTBL struct IConfigMetabaseEntryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigMetabaseEntry_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigMetabaseEntry_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigMetabaseEntry_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigMetabaseEntry_put_DataType(This,DataType)	\
    (This)->lpVtbl -> put_DataType(This,DataType)

#define IConfigMetabaseEntry_get_DataType(This,pDataType)	\
    (This)->lpVtbl -> get_DataType(This,pDataType)

#define IConfigMetabaseEntry_put_SemanticType(This,bstrSemanticType)	\
    (This)->lpVtbl -> put_SemanticType(This,bstrSemanticType)

#define IConfigMetabaseEntry_get_SemanticType(This,bstrSemanticType)	\
    (This)->lpVtbl -> get_SemanticType(This,bstrSemanticType)

#define IConfigMetabaseEntry_put_MaxValue(This,iMaxValue)	\
    (This)->lpVtbl -> put_MaxValue(This,iMaxValue)

#define IConfigMetabaseEntry_get_MaxValue(This,piMaxValue)	\
    (This)->lpVtbl -> get_MaxValue(This,piMaxValue)

#define IConfigMetabaseEntry_put_MinValue(This,iMinValue)	\
    (This)->lpVtbl -> put_MinValue(This,iMinValue)

#define IConfigMetabaseEntry_get_MinValue(This,piMinValue)	\
    (This)->lpVtbl -> get_MinValue(This,piMinValue)

#define IConfigMetabaseEntry_put_MaxLength(This,iMaxLength)	\
    (This)->lpVtbl -> put_MaxLength(This,iMaxLength)

#define IConfigMetabaseEntry_get_MaxLength(This,piMaxLength)	\
    (This)->lpVtbl -> get_MaxLength(This,piMaxLength)

#define IConfigMetabaseEntry_put_Options(This,bstrOptions)	\
    (This)->lpVtbl -> put_Options(This,bstrOptions)

#define IConfigMetabaseEntry_get_Options(This,pbstrOptions)	\
    (This)->lpVtbl -> get_Options(This,pbstrOptions)

#define IConfigMetabaseEntry_put_Multiple(This,fMultiple)	\
    (This)->lpVtbl -> put_Multiple(This,fMultiple)

#define IConfigMetabaseEntry_get_Multiple(This,pfMultiple)	\
    (This)->lpVtbl -> get_Multiple(This,pfMultiple)

#define IConfigMetabaseEntry_put_RWAccess(This,dwRWAccess)	\
    (This)->lpVtbl -> put_RWAccess(This,dwRWAccess)

#define IConfigMetabaseEntry_get_RWAccess(This,pdwRWAccess)	\
    (This)->lpVtbl -> get_RWAccess(This,pdwRWAccess)

#define IConfigMetabaseEntry_put_Label(This,bstrLabel)	\
    (This)->lpVtbl -> put_Label(This,bstrLabel)

#define IConfigMetabaseEntry_get_Label(This,pbstrLabel)	\
    (This)->lpVtbl -> get_Label(This,pbstrLabel)

#define IConfigMetabaseEntry_put_AccessRole(This,dwAccessRole)	\
    (This)->lpVtbl -> put_AccessRole(This,dwAccessRole)

#define IConfigMetabaseEntry_get_AccessRole(This,pdwAccessRole)	\
    (This)->lpVtbl -> get_AccessRole(This,pdwAccessRole)

#define IConfigMetabaseEntry_put_ACL(This,bstrACL)	\
    (This)->lpVtbl -> put_ACL(This,bstrACL)

#define IConfigMetabaseEntry_get_ACL(This,pbstrACL)	\
    (This)->lpVtbl -> get_ACL(This,pbstrACL)

#define IConfigMetabaseEntry_put_SemanticTypeFromMimeType(This,bstrType)	\
    (This)->lpVtbl -> put_SemanticTypeFromMimeType(This,bstrType)

#define IConfigMetabaseEntry_get_SemanticTypeAsMimeType(This,pbstrType)	\
    (This)->lpVtbl -> get_SemanticTypeAsMimeType(This,pbstrType)

#define IConfigMetabaseEntry_get_PresenceFlags(This,pfFlags)	\
    (This)->lpVtbl -> get_PresenceFlags(This,pfFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_DataType_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ enum ConfigDataType DataType);


void __RPC_STUB IConfigMetabaseEntry_put_DataType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_DataType_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ enum ConfigDataType __RPC_FAR *pDataType);


void __RPC_STUB IConfigMetabaseEntry_get_DataType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_SemanticType_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ BSTR bstrSemanticType);


void __RPC_STUB IConfigMetabaseEntry_put_SemanticType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_SemanticType_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrSemanticType);


void __RPC_STUB IConfigMetabaseEntry_get_SemanticType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_MaxValue_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ int iMaxValue);


void __RPC_STUB IConfigMetabaseEntry_put_MaxValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_MaxValue_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *piMaxValue);


void __RPC_STUB IConfigMetabaseEntry_get_MaxValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_MinValue_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ int iMinValue);


void __RPC_STUB IConfigMetabaseEntry_put_MinValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_MinValue_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *piMinValue);


void __RPC_STUB IConfigMetabaseEntry_get_MinValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_MaxLength_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ int iMaxLength);


void __RPC_STUB IConfigMetabaseEntry_put_MaxLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_MaxLength_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *piMaxLength);


void __RPC_STUB IConfigMetabaseEntry_get_MaxLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_Options_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ BSTR bstrOptions);


void __RPC_STUB IConfigMetabaseEntry_put_Options_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_Options_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOptions);


void __RPC_STUB IConfigMetabaseEntry_get_Options_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_Multiple_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ BOOL fMultiple);


void __RPC_STUB IConfigMetabaseEntry_put_Multiple_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_Multiple_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pfMultiple);


void __RPC_STUB IConfigMetabaseEntry_get_Multiple_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_RWAccess_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ DWORD dwRWAccess);


void __RPC_STUB IConfigMetabaseEntry_put_RWAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_RWAccess_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ DWORD __RPC_FAR *pdwRWAccess);


void __RPC_STUB IConfigMetabaseEntry_get_RWAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_Label_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ BSTR bstrLabel);


void __RPC_STUB IConfigMetabaseEntry_put_Label_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_Label_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrLabel);


void __RPC_STUB IConfigMetabaseEntry_get_Label_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_AccessRole_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ DWORD dwAccessRole);


void __RPC_STUB IConfigMetabaseEntry_put_AccessRole_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_AccessRole_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ DWORD __RPC_FAR *pdwAccessRole);


void __RPC_STUB IConfigMetabaseEntry_get_AccessRole_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_ACL_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ BSTR bstrACL);


void __RPC_STUB IConfigMetabaseEntry_put_ACL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_ACL_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrACL);


void __RPC_STUB IConfigMetabaseEntry_get_ACL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_put_SemanticTypeFromMimeType_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [in] */ BSTR bstrType);


void __RPC_STUB IConfigMetabaseEntry_put_SemanticTypeFromMimeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_SemanticTypeAsMimeType_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrType);


void __RPC_STUB IConfigMetabaseEntry_get_SemanticTypeAsMimeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry_get_PresenceFlags_Proxy( 
    IConfigMetabaseEntry __RPC_FAR * This,
    /* [retval][out] */ DWORD __RPC_FAR *pfFlags);


void __RPC_STUB IConfigMetabaseEntry_get_PresenceFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigMetabaseEntry_INTERFACE_DEFINED__ */


#ifndef __IConfigMetabase_INTERFACE_DEFINED__
#define __IConfigMetabase_INTERFACE_DEFINED__

/* interface IConfigMetabase */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigMetabase;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("53AE45E6-F372-461f-9FE3-801D6669DBCC")
    IConfigMetabase : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE PopulateMetadata( 
            /* [in] */ IXMLDOMElement __RPC_FAR *pElemCharacteristic,
            /* [in] */ BSTR bstrBasePath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PopulateParm( 
            /* [in] */ IXMLDOMElement __RPC_FAR *pElemParm,
            /* [in] */ BSTR bstrBasePath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryEntry( 
            /* [in] */ BSTR bstrPath,
            /* [out] */ IConfigMetabaseEntry __RPC_FAR *__RPC_FAR *ppEntry) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEntry( 
            /* [in] */ BSTR bstrPath,
            /* [in] */ IConfigMetabaseEntry __RPC_FAR *pEntry) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteEntry( 
            /* [in] */ BSTR bstrPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteEntryProps( 
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD grfPropsToDel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryEntryEx( 
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwQueryFlags,
            /* [out] */ IConfigMetabaseEntry __RPC_FAR *__RPC_FAR *ppEntry,
            /* [out] */ DWORD __RPC_FAR *pdwRetMatchFlag) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE VerifyRoleRecursive( 
            /* [in] */ BSTR bstrRootPath,
            /* [in] */ DWORD dwRoleToMatch,
            /* [in] */ DWORD dwRWAccessToMatch) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckRecursiveSecurityAccessForUri( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ DWORD dwCSPRootNode,
            /* [in] */ DWORD dwRWDesired,
            /* [in] */ DWORD dwRolesDesired) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckSecurityAccessForPath( 
            /* [in] */ LPCTSTR pszPath,
            /* [in] */ DWORD dwRWDesired,
            /* [in] */ DWORD dwRolesDesired,
            /* [out] */ DWORD __RPC_FAR *pdwRWFound,
            /* [out] */ DWORD __RPC_FAR *pdwRolesFound) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckSecurityAccessForUri( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ DWORD dwCSPRootNode,
            /* [in] */ DWORD dwRWDesired,
            /* [in] */ DWORD dwRolesDesired) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigMetabaseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigMetabase __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigMetabase __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PopulateMetadata )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ IXMLDOMElement __RPC_FAR *pElemCharacteristic,
            /* [in] */ BSTR bstrBasePath);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PopulateParm )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ IXMLDOMElement __RPC_FAR *pElemParm,
            /* [in] */ BSTR bstrBasePath);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryEntry )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ BSTR bstrPath,
            /* [out] */ IConfigMetabaseEntry __RPC_FAR *__RPC_FAR *ppEntry);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEntry )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ BSTR bstrPath,
            /* [in] */ IConfigMetabaseEntry __RPC_FAR *pEntry);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteEntry )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ BSTR bstrPath);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteEntryProps )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD grfPropsToDel);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryEntryEx )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ BSTR bstrPath,
            /* [in] */ DWORD dwQueryFlags,
            /* [out] */ IConfigMetabaseEntry __RPC_FAR *__RPC_FAR *ppEntry,
            /* [out] */ DWORD __RPC_FAR *pdwRetMatchFlag);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *VerifyRoleRecursive )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ BSTR bstrRootPath,
            /* [in] */ DWORD dwRoleToMatch,
            /* [in] */ DWORD dwRWAccessToMatch);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckRecursiveSecurityAccessForUri )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ DWORD dwCSPRootNode,
            /* [in] */ DWORD dwRWDesired,
            /* [in] */ DWORD dwRolesDesired);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckSecurityAccessForPath )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ LPCTSTR pszPath,
            /* [in] */ DWORD dwRWDesired,
            /* [in] */ DWORD dwRolesDesired,
            /* [out] */ DWORD __RPC_FAR *pdwRWFound,
            /* [out] */ DWORD __RPC_FAR *pdwRolesFound);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CheckSecurityAccessForUri )( 
            IConfigMetabase __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ DWORD dwCSPRootNode,
            /* [in] */ DWORD dwRWDesired,
            /* [in] */ DWORD dwRolesDesired);
        
        END_INTERFACE
    } IConfigMetabaseVtbl;

    interface IConfigMetabase
    {
        CONST_VTBL struct IConfigMetabaseVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigMetabase_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigMetabase_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigMetabase_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigMetabase_PopulateMetadata(This,pElemCharacteristic,bstrBasePath)	\
    (This)->lpVtbl -> PopulateMetadata(This,pElemCharacteristic,bstrBasePath)

#define IConfigMetabase_PopulateParm(This,pElemParm,bstrBasePath)	\
    (This)->lpVtbl -> PopulateParm(This,pElemParm,bstrBasePath)

#define IConfigMetabase_QueryEntry(This,bstrPath,ppEntry)	\
    (This)->lpVtbl -> QueryEntry(This,bstrPath,ppEntry)

#define IConfigMetabase_SetEntry(This,bstrPath,pEntry)	\
    (This)->lpVtbl -> SetEntry(This,bstrPath,pEntry)

#define IConfigMetabase_DeleteEntry(This,bstrPath)	\
    (This)->lpVtbl -> DeleteEntry(This,bstrPath)

#define IConfigMetabase_DeleteEntryProps(This,bstrPath,grfPropsToDel)	\
    (This)->lpVtbl -> DeleteEntryProps(This,bstrPath,grfPropsToDel)

#define IConfigMetabase_QueryEntryEx(This,bstrPath,dwQueryFlags,ppEntry,pdwRetMatchFlag)	\
    (This)->lpVtbl -> QueryEntryEx(This,bstrPath,dwQueryFlags,ppEntry,pdwRetMatchFlag)

#define IConfigMetabase_VerifyRoleRecursive(This,bstrRootPath,dwRoleToMatch,dwRWAccessToMatch)	\
    (This)->lpVtbl -> VerifyRoleRecursive(This,bstrRootPath,dwRoleToMatch,dwRWAccessToMatch)

#define IConfigMetabase_CheckRecursiveSecurityAccessForUri(This,pURI,dwCSPRootNode,dwRWDesired,dwRolesDesired)	\
    (This)->lpVtbl -> CheckRecursiveSecurityAccessForUri(This,pURI,dwCSPRootNode,dwRWDesired,dwRolesDesired)

#define IConfigMetabase_CheckSecurityAccessForPath(This,pszPath,dwRWDesired,dwRolesDesired,pdwRWFound,pdwRolesFound)	\
    (This)->lpVtbl -> CheckSecurityAccessForPath(This,pszPath,dwRWDesired,dwRolesDesired,pdwRWFound,pdwRolesFound)

#define IConfigMetabase_CheckSecurityAccessForUri(This,pURI,dwCSPRootNode,dwRWDesired,dwRolesDesired)	\
    (This)->lpVtbl -> CheckSecurityAccessForUri(This,pURI,dwCSPRootNode,dwRWDesired,dwRolesDesired)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigMetabase_PopulateMetadata_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ IXMLDOMElement __RPC_FAR *pElemCharacteristic,
    /* [in] */ BSTR bstrBasePath);


void __RPC_STUB IConfigMetabase_PopulateMetadata_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_PopulateParm_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ IXMLDOMElement __RPC_FAR *pElemParm,
    /* [in] */ BSTR bstrBasePath);


void __RPC_STUB IConfigMetabase_PopulateParm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_QueryEntry_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ BSTR bstrPath,
    /* [out] */ IConfigMetabaseEntry __RPC_FAR *__RPC_FAR *ppEntry);


void __RPC_STUB IConfigMetabase_QueryEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_SetEntry_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ BSTR bstrPath,
    /* [in] */ IConfigMetabaseEntry __RPC_FAR *pEntry);


void __RPC_STUB IConfigMetabase_SetEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_DeleteEntry_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ BSTR bstrPath);


void __RPC_STUB IConfigMetabase_DeleteEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_DeleteEntryProps_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD grfPropsToDel);


void __RPC_STUB IConfigMetabase_DeleteEntryProps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_QueryEntryEx_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ BSTR bstrPath,
    /* [in] */ DWORD dwQueryFlags,
    /* [out] */ IConfigMetabaseEntry __RPC_FAR *__RPC_FAR *ppEntry,
    /* [out] */ DWORD __RPC_FAR *pdwRetMatchFlag);


void __RPC_STUB IConfigMetabase_QueryEntryEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_VerifyRoleRecursive_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ BSTR bstrRootPath,
    /* [in] */ DWORD dwRoleToMatch,
    /* [in] */ DWORD dwRWAccessToMatch);


void __RPC_STUB IConfigMetabase_VerifyRoleRecursive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_CheckRecursiveSecurityAccessForUri_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
    /* [in] */ DWORD dwCSPRootNode,
    /* [in] */ DWORD dwRWDesired,
    /* [in] */ DWORD dwRolesDesired);


void __RPC_STUB IConfigMetabase_CheckRecursiveSecurityAccessForUri_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_CheckSecurityAccessForPath_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ LPCTSTR pszPath,
    /* [in] */ DWORD dwRWDesired,
    /* [in] */ DWORD dwRolesDesired,
    /* [out] */ DWORD __RPC_FAR *pdwRWFound,
    /* [out] */ DWORD __RPC_FAR *pdwRolesFound);


void __RPC_STUB IConfigMetabase_CheckSecurityAccessForPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigMetabase_CheckSecurityAccessForUri_Proxy( 
    IConfigMetabase __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
    /* [in] */ DWORD dwCSPRootNode,
    /* [in] */ DWORD dwRWDesired,
    /* [in] */ DWORD dwRolesDesired);


void __RPC_STUB IConfigMetabase_CheckSecurityAccessForUri_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigMetabase_INTERFACE_DEFINED__ */


#ifndef __IEnumConfigMetabaseEntryLocales_INTERFACE_DEFINED__
#define __IEnumConfigMetabaseEntryLocales_INTERFACE_DEFINED__

/* interface IEnumConfigMetabaseEntryLocales */
/* [object][uuid] */ 


EXTERN_C const IID IID_IEnumConfigMetabaseEntryLocales;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2ECB8A29-9507-4762-88F7-782F5AFEB0A5")
    IEnumConfigMetabaseEntryLocales : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG cLocales,
            /* [size_is][out] */ WORD __RPC_FAR *pwLCID,
            /* [out] */ ULONG __RPC_FAR *pcFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG cLocales) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumConfigMetabaseEntryLocales __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumConfigMetabaseEntryLocalesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEnumConfigMetabaseEntryLocales __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEnumConfigMetabaseEntryLocales __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEnumConfigMetabaseEntryLocales __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Next )( 
            IEnumConfigMetabaseEntryLocales __RPC_FAR * This,
            /* [in] */ ULONG cLocales,
            /* [size_is][out] */ WORD __RPC_FAR *pwLCID,
            /* [out] */ ULONG __RPC_FAR *pcFetched);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Skip )( 
            IEnumConfigMetabaseEntryLocales __RPC_FAR * This,
            /* [in] */ ULONG cLocales);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IEnumConfigMetabaseEntryLocales __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IEnumConfigMetabaseEntryLocales __RPC_FAR * This,
            /* [out] */ IEnumConfigMetabaseEntryLocales __RPC_FAR *__RPC_FAR *ppEnum);
        
        END_INTERFACE
    } IEnumConfigMetabaseEntryLocalesVtbl;

    interface IEnumConfigMetabaseEntryLocales
    {
        CONST_VTBL struct IEnumConfigMetabaseEntryLocalesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumConfigMetabaseEntryLocales_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumConfigMetabaseEntryLocales_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumConfigMetabaseEntryLocales_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumConfigMetabaseEntryLocales_Next(This,cLocales,pwLCID,pcFetched)	\
    (This)->lpVtbl -> Next(This,cLocales,pwLCID,pcFetched)

#define IEnumConfigMetabaseEntryLocales_Skip(This,cLocales)	\
    (This)->lpVtbl -> Skip(This,cLocales)

#define IEnumConfigMetabaseEntryLocales_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumConfigMetabaseEntryLocales_Clone(This,ppEnum)	\
    (This)->lpVtbl -> Clone(This,ppEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEnumConfigMetabaseEntryLocales_Next_Proxy( 
    IEnumConfigMetabaseEntryLocales __RPC_FAR * This,
    /* [in] */ ULONG cLocales,
    /* [size_is][out] */ WORD __RPC_FAR *pwLCID,
    /* [out] */ ULONG __RPC_FAR *pcFetched);


void __RPC_STUB IEnumConfigMetabaseEntryLocales_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigMetabaseEntryLocales_Skip_Proxy( 
    IEnumConfigMetabaseEntryLocales __RPC_FAR * This,
    /* [in] */ ULONG cLocales);


void __RPC_STUB IEnumConfigMetabaseEntryLocales_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigMetabaseEntryLocales_Reset_Proxy( 
    IEnumConfigMetabaseEntryLocales __RPC_FAR * This);


void __RPC_STUB IEnumConfigMetabaseEntryLocales_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumConfigMetabaseEntryLocales_Clone_Proxy( 
    IEnumConfigMetabaseEntryLocales __RPC_FAR * This,
    /* [out] */ IEnumConfigMetabaseEntryLocales __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IEnumConfigMetabaseEntryLocales_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumConfigMetabaseEntryLocales_INTERFACE_DEFINED__ */


#ifndef __IConfigMetabaseEntry2_INTERFACE_DEFINED__
#define __IConfigMetabaseEntry2_INTERFACE_DEFINED__

/* interface IConfigMetabaseEntry2 */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigMetabaseEntry2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2842C0FF-7591-4615-895E-487456DE0BF4")
    IConfigMetabaseEntry2 : public IConfigMetabaseEntry
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumLocales( 
            /* [out] */ IEnumConfigMetabaseEntryLocales __RPC_FAR *__RPC_FAR *ppEnum) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_OptionsForLocale( 
            /* [in] */ WORD lcid,
            /* [in] */ BSTR bstrOption) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_OptionsForLocale( 
            /* [in] */ WORD lcid,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOption) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_LabelForLocale( 
            /* [in] */ WORD lcid,
            /* [in] */ BSTR bstrLabel) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LabelForLocale( 
            /* [in] */ WORD lcid,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLabel) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigMetabaseEntry2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigMetabaseEntry2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigMetabaseEntry2 __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DataType )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ enum ConfigDataType DataType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DataType )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ enum ConfigDataType __RPC_FAR *pDataType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SemanticType )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ BSTR bstrSemanticType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SemanticType )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrSemanticType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxValue )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ int iMaxValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxValue )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piMaxValue);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MinValue )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ int iMinValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MinValue )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piMinValue);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MaxLength )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ int iMaxLength);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MaxLength )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piMaxLength);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Options )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ BSTR bstrOptions);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Options )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOptions);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Multiple )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ BOOL fMultiple);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Multiple )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pfMultiple);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_RWAccess )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ DWORD dwRWAccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RWAccess )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRWAccess);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Label )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ BSTR bstrLabel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Label )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLabel);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AccessRole )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ DWORD dwAccessRole);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AccessRole )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwAccessRole);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ACL )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ BSTR bstrACL);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ACL )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrACL);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SemanticTypeFromMimeType )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ BSTR bstrType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SemanticTypeAsMimeType )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PresenceFlags )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pfFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumLocales )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [out] */ IEnumConfigMetabaseEntryLocales __RPC_FAR *__RPC_FAR *ppEnum);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_OptionsForLocale )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ WORD lcid,
            /* [in] */ BSTR bstrOption);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_OptionsForLocale )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ WORD lcid,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOption);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LabelForLocale )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ WORD lcid,
            /* [in] */ BSTR bstrLabel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LabelForLocale )( 
            IConfigMetabaseEntry2 __RPC_FAR * This,
            /* [in] */ WORD lcid,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLabel);
        
        END_INTERFACE
    } IConfigMetabaseEntry2Vtbl;

    interface IConfigMetabaseEntry2
    {
        CONST_VTBL struct IConfigMetabaseEntry2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigMetabaseEntry2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigMetabaseEntry2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigMetabaseEntry2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigMetabaseEntry2_put_DataType(This,DataType)	\
    (This)->lpVtbl -> put_DataType(This,DataType)

#define IConfigMetabaseEntry2_get_DataType(This,pDataType)	\
    (This)->lpVtbl -> get_DataType(This,pDataType)

#define IConfigMetabaseEntry2_put_SemanticType(This,bstrSemanticType)	\
    (This)->lpVtbl -> put_SemanticType(This,bstrSemanticType)

#define IConfigMetabaseEntry2_get_SemanticType(This,bstrSemanticType)	\
    (This)->lpVtbl -> get_SemanticType(This,bstrSemanticType)

#define IConfigMetabaseEntry2_put_MaxValue(This,iMaxValue)	\
    (This)->lpVtbl -> put_MaxValue(This,iMaxValue)

#define IConfigMetabaseEntry2_get_MaxValue(This,piMaxValue)	\
    (This)->lpVtbl -> get_MaxValue(This,piMaxValue)

#define IConfigMetabaseEntry2_put_MinValue(This,iMinValue)	\
    (This)->lpVtbl -> put_MinValue(This,iMinValue)

#define IConfigMetabaseEntry2_get_MinValue(This,piMinValue)	\
    (This)->lpVtbl -> get_MinValue(This,piMinValue)

#define IConfigMetabaseEntry2_put_MaxLength(This,iMaxLength)	\
    (This)->lpVtbl -> put_MaxLength(This,iMaxLength)

#define IConfigMetabaseEntry2_get_MaxLength(This,piMaxLength)	\
    (This)->lpVtbl -> get_MaxLength(This,piMaxLength)

#define IConfigMetabaseEntry2_put_Options(This,bstrOptions)	\
    (This)->lpVtbl -> put_Options(This,bstrOptions)

#define IConfigMetabaseEntry2_get_Options(This,pbstrOptions)	\
    (This)->lpVtbl -> get_Options(This,pbstrOptions)

#define IConfigMetabaseEntry2_put_Multiple(This,fMultiple)	\
    (This)->lpVtbl -> put_Multiple(This,fMultiple)

#define IConfigMetabaseEntry2_get_Multiple(This,pfMultiple)	\
    (This)->lpVtbl -> get_Multiple(This,pfMultiple)

#define IConfigMetabaseEntry2_put_RWAccess(This,dwRWAccess)	\
    (This)->lpVtbl -> put_RWAccess(This,dwRWAccess)

#define IConfigMetabaseEntry2_get_RWAccess(This,pdwRWAccess)	\
    (This)->lpVtbl -> get_RWAccess(This,pdwRWAccess)

#define IConfigMetabaseEntry2_put_Label(This,bstrLabel)	\
    (This)->lpVtbl -> put_Label(This,bstrLabel)

#define IConfigMetabaseEntry2_get_Label(This,pbstrLabel)	\
    (This)->lpVtbl -> get_Label(This,pbstrLabel)

#define IConfigMetabaseEntry2_put_AccessRole(This,dwAccessRole)	\
    (This)->lpVtbl -> put_AccessRole(This,dwAccessRole)

#define IConfigMetabaseEntry2_get_AccessRole(This,pdwAccessRole)	\
    (This)->lpVtbl -> get_AccessRole(This,pdwAccessRole)

#define IConfigMetabaseEntry2_put_ACL(This,bstrACL)	\
    (This)->lpVtbl -> put_ACL(This,bstrACL)

#define IConfigMetabaseEntry2_get_ACL(This,pbstrACL)	\
    (This)->lpVtbl -> get_ACL(This,pbstrACL)

#define IConfigMetabaseEntry2_put_SemanticTypeFromMimeType(This,bstrType)	\
    (This)->lpVtbl -> put_SemanticTypeFromMimeType(This,bstrType)

#define IConfigMetabaseEntry2_get_SemanticTypeAsMimeType(This,pbstrType)	\
    (This)->lpVtbl -> get_SemanticTypeAsMimeType(This,pbstrType)

#define IConfigMetabaseEntry2_get_PresenceFlags(This,pfFlags)	\
    (This)->lpVtbl -> get_PresenceFlags(This,pfFlags)


#define IConfigMetabaseEntry2_EnumLocales(This,ppEnum)	\
    (This)->lpVtbl -> EnumLocales(This,ppEnum)

#define IConfigMetabaseEntry2_put_OptionsForLocale(This,lcid,bstrOption)	\
    (This)->lpVtbl -> put_OptionsForLocale(This,lcid,bstrOption)

#define IConfigMetabaseEntry2_get_OptionsForLocale(This,lcid,pbstrOption)	\
    (This)->lpVtbl -> get_OptionsForLocale(This,lcid,pbstrOption)

#define IConfigMetabaseEntry2_put_LabelForLocale(This,lcid,bstrLabel)	\
    (This)->lpVtbl -> put_LabelForLocale(This,lcid,bstrLabel)

#define IConfigMetabaseEntry2_get_LabelForLocale(This,lcid,pbstrLabel)	\
    (This)->lpVtbl -> get_LabelForLocale(This,lcid,pbstrLabel)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry2_EnumLocales_Proxy( 
    IConfigMetabaseEntry2 __RPC_FAR * This,
    /* [out] */ IEnumConfigMetabaseEntryLocales __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IConfigMetabaseEntry2_EnumLocales_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry2_put_OptionsForLocale_Proxy( 
    IConfigMetabaseEntry2 __RPC_FAR * This,
    /* [in] */ WORD lcid,
    /* [in] */ BSTR bstrOption);


void __RPC_STUB IConfigMetabaseEntry2_put_OptionsForLocale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry2_get_OptionsForLocale_Proxy( 
    IConfigMetabaseEntry2 __RPC_FAR * This,
    /* [in] */ WORD lcid,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOption);


void __RPC_STUB IConfigMetabaseEntry2_get_OptionsForLocale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry2_put_LabelForLocale_Proxy( 
    IConfigMetabaseEntry2 __RPC_FAR * This,
    /* [in] */ WORD lcid,
    /* [in] */ BSTR bstrLabel);


void __RPC_STUB IConfigMetabaseEntry2_put_LabelForLocale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IConfigMetabaseEntry2_get_LabelForLocale_Proxy( 
    IConfigMetabaseEntry2 __RPC_FAR * This,
    /* [in] */ WORD lcid,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrLabel);


void __RPC_STUB IConfigMetabaseEntry2_get_LabelForLocale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigMetabaseEntry2_INTERFACE_DEFINED__ */


#ifndef __IConfigUninstallHost_INTERFACE_DEFINED__
#define __IConfigUninstallHost_INTERFACE_DEFINED__

/* interface IConfigUninstallHost */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigUninstallHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("08A9104D-19AD-4fb3-88FA-11D918FC7F21")
    IConfigUninstallHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetAppName( 
            /* [in] */ BSTR bstrAppName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSetupDllPath( 
            /* [in] */ BSTR bstrSetupDllPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetInstallDir( 
            /* [in] */ BSTR bstrInstallDir) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetHwnd( 
            /* [in] */ HWND hWnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFlags( 
            /* [in] */ DWORD grfFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFlags( 
            /* [out] */ DWORD __RPC_FAR *pgrfFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoUninstallInitProcessing( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoUninstallExitProcessing( 
            /* [in] */ BOOL fSuccess) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoXMLUninstall( 
            /* [in] */ IConfigManager __RPC_FAR *pConfigMan,
            /* [out][in] */ BSTR __RPC_FAR *pbstrXML,
            /* [in] */ DWORD dwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigUninstallHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigUninstallHost __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigUninstallHost __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAppName )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ BSTR bstrAppName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSetupDllPath )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ BSTR bstrSetupDllPath);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetInstallDir )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ BSTR bstrInstallDir);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHwnd )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ HWND hWnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFlags )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ DWORD grfFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFlags )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pgrfFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoUninstallInitProcessing )( 
            IConfigUninstallHost __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoUninstallExitProcessing )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ BOOL fSuccess);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoXMLUninstall )( 
            IConfigUninstallHost __RPC_FAR * This,
            /* [in] */ IConfigManager __RPC_FAR *pConfigMan,
            /* [out][in] */ BSTR __RPC_FAR *pbstrXML,
            /* [in] */ DWORD dwFlags);
        
        END_INTERFACE
    } IConfigUninstallHostVtbl;

    interface IConfigUninstallHost
    {
        CONST_VTBL struct IConfigUninstallHostVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigUninstallHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigUninstallHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigUninstallHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigUninstallHost_SetAppName(This,bstrAppName)	\
    (This)->lpVtbl -> SetAppName(This,bstrAppName)

#define IConfigUninstallHost_SetSetupDllPath(This,bstrSetupDllPath)	\
    (This)->lpVtbl -> SetSetupDllPath(This,bstrSetupDllPath)

#define IConfigUninstallHost_SetInstallDir(This,bstrInstallDir)	\
    (This)->lpVtbl -> SetInstallDir(This,bstrInstallDir)

#define IConfigUninstallHost_SetHwnd(This,hWnd)	\
    (This)->lpVtbl -> SetHwnd(This,hWnd)

#define IConfigUninstallHost_SetFlags(This,grfFlags)	\
    (This)->lpVtbl -> SetFlags(This,grfFlags)

#define IConfigUninstallHost_GetFlags(This,pgrfFlags)	\
    (This)->lpVtbl -> GetFlags(This,pgrfFlags)

#define IConfigUninstallHost_DoUninstallInitProcessing(This)	\
    (This)->lpVtbl -> DoUninstallInitProcessing(This)

#define IConfigUninstallHost_DoUninstallExitProcessing(This,fSuccess)	\
    (This)->lpVtbl -> DoUninstallExitProcessing(This,fSuccess)

#define IConfigUninstallHost_DoXMLUninstall(This,pConfigMan,pbstrXML,dwFlags)	\
    (This)->lpVtbl -> DoXMLUninstall(This,pConfigMan,pbstrXML,dwFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigUninstallHost_SetAppName_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [in] */ BSTR bstrAppName);


void __RPC_STUB IConfigUninstallHost_SetAppName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_SetSetupDllPath_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [in] */ BSTR bstrSetupDllPath);


void __RPC_STUB IConfigUninstallHost_SetSetupDllPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_SetInstallDir_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [in] */ BSTR bstrInstallDir);


void __RPC_STUB IConfigUninstallHost_SetInstallDir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_SetHwnd_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [in] */ HWND hWnd);


void __RPC_STUB IConfigUninstallHost_SetHwnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_SetFlags_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [in] */ DWORD grfFlags);


void __RPC_STUB IConfigUninstallHost_SetFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_GetFlags_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pgrfFlags);


void __RPC_STUB IConfigUninstallHost_GetFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_DoUninstallInitProcessing_Proxy( 
    IConfigUninstallHost __RPC_FAR * This);


void __RPC_STUB IConfigUninstallHost_DoUninstallInitProcessing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_DoUninstallExitProcessing_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [in] */ BOOL fSuccess);


void __RPC_STUB IConfigUninstallHost_DoUninstallExitProcessing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigUninstallHost_DoXMLUninstall_Proxy( 
    IConfigUninstallHost __RPC_FAR * This,
    /* [in] */ IConfigManager __RPC_FAR *pConfigMan,
    /* [out][in] */ BSTR __RPC_FAR *pbstrXML,
    /* [in] */ DWORD dwFlags);


void __RPC_STUB IConfigUninstallHost_DoXMLUninstall_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigUninstallHost_INTERFACE_DEFINED__ */


#ifndef __IConfigInstallResult_INTERFACE_DEFINED__
#define __IConfigInstallResult_INTERFACE_DEFINED__

/* interface IConfigInstallResult */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigInstallResult;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("60F75EA1-6D7B-4563-BBC4-B4F3CB251330")
    IConfigInstallResult : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetRebootRequiredStatus( 
            /* [in] */ BOOL fRebootReqd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRebootRequiredStatus( 
            /* [out] */ BOOL __RPC_FAR *pfRebootReqd) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigInstallResultVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigInstallResult __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigInstallResult __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigInstallResult __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRebootRequiredStatus )( 
            IConfigInstallResult __RPC_FAR * This,
            /* [in] */ BOOL fRebootReqd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRebootRequiredStatus )( 
            IConfigInstallResult __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfRebootReqd);
        
        END_INTERFACE
    } IConfigInstallResultVtbl;

    interface IConfigInstallResult
    {
        CONST_VTBL struct IConfigInstallResultVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigInstallResult_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigInstallResult_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigInstallResult_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigInstallResult_SetRebootRequiredStatus(This,fRebootReqd)	\
    (This)->lpVtbl -> SetRebootRequiredStatus(This,fRebootReqd)

#define IConfigInstallResult_GetRebootRequiredStatus(This,pfRebootReqd)	\
    (This)->lpVtbl -> GetRebootRequiredStatus(This,pfRebootReqd)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigInstallResult_SetRebootRequiredStatus_Proxy( 
    IConfigInstallResult __RPC_FAR * This,
    /* [in] */ BOOL fRebootReqd);


void __RPC_STUB IConfigInstallResult_SetRebootRequiredStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigInstallResult_GetRebootRequiredStatus_Proxy( 
    IConfigInstallResult __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pfRebootReqd);


void __RPC_STUB IConfigInstallResult_GetRebootRequiredStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigInstallResult_INTERFACE_DEFINED__ */



#ifndef __ConfigurationManager_LIBRARY_DEFINED__
#define __ConfigurationManager_LIBRARY_DEFINED__

/* library ConfigurationManager */
/* [helpstring][uuid] */ 


EXTERN_C const IID LIBID_ConfigurationManager;

EXTERN_C const CLSID CLSID_CConfigManager;

#ifdef __cplusplus

class DECLSPEC_UUID("67C73FBF-999E-44dd-AF32-480E86CB7EA3")
CConfigManager;
#endif

EXTERN_C const CLSID CLSID_CConfigMetabase;

#ifdef __cplusplus

class DECLSPEC_UUID("A9CBC292-0A92-4fe7-AF32-24531AB92EA2")
CConfigMetabase;
#endif

EXTERN_C const CLSID CLSID_CConfigMetabaseEntry;

#ifdef __cplusplus

class DECLSPEC_UUID("22359130-347A-4711-9D85-7FAD66F89CEB")
CConfigMetabaseEntry;
#endif

EXTERN_C const CLSID CLSID_CConfigMetabaseEntry2;

#ifdef __cplusplus

class DECLSPEC_UUID("EFAE9AA5-38F0-481a-92EE-F07713914F32")
CConfigMetabaseEntry2;
#endif

EXTERN_C const CLSID CLSID_CConfigUninstallHost;

#ifdef __cplusplus

class DECLSPEC_UUID("EB0AE09E-5DDC-4e40-AB81-5668F012EC97")
CConfigUninstallHost;
#endif
#endif /* __ConfigurationManager_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long __RPC_FAR *, unsigned long            , HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long __RPC_FAR *, HWND __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


