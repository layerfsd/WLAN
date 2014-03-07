
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:32:39 2008
 */
/* Compiler settings for .\msscript.idl:
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


#ifndef __msscript_h__
#define __msscript_h__

/* Forward Declarations */ 

#ifndef __IScriptProcedure_FWD_DEFINED__
#define __IScriptProcedure_FWD_DEFINED__
typedef interface IScriptProcedure IScriptProcedure;
#endif 	/* __IScriptProcedure_FWD_DEFINED__ */


#ifndef __IScriptProcedureCollection_FWD_DEFINED__
#define __IScriptProcedureCollection_FWD_DEFINED__
typedef interface IScriptProcedureCollection IScriptProcedureCollection;
#endif 	/* __IScriptProcedureCollection_FWD_DEFINED__ */


#ifndef __IScriptModule_FWD_DEFINED__
#define __IScriptModule_FWD_DEFINED__
typedef interface IScriptModule IScriptModule;
#endif 	/* __IScriptModule_FWD_DEFINED__ */


#ifndef __IScriptModuleCollection_FWD_DEFINED__
#define __IScriptModuleCollection_FWD_DEFINED__
typedef interface IScriptModuleCollection IScriptModuleCollection;
#endif 	/* __IScriptModuleCollection_FWD_DEFINED__ */


#ifndef __IScriptError_FWD_DEFINED__
#define __IScriptError_FWD_DEFINED__
typedef interface IScriptError IScriptError;
#endif 	/* __IScriptError_FWD_DEFINED__ */


#ifndef __IScriptControl_FWD_DEFINED__
#define __IScriptControl_FWD_DEFINED__
typedef interface IScriptControl IScriptControl;
#endif 	/* __IScriptControl_FWD_DEFINED__ */


#ifndef __DScriptControlSource_FWD_DEFINED__
#define __DScriptControlSource_FWD_DEFINED__
typedef interface DScriptControlSource DScriptControlSource;
#endif 	/* __DScriptControlSource_FWD_DEFINED__ */


#ifndef __Procedure_FWD_DEFINED__
#define __Procedure_FWD_DEFINED__

#ifdef __cplusplus
typedef class Procedure Procedure;
#else
typedef struct Procedure Procedure;
#endif /* __cplusplus */

#endif 	/* __Procedure_FWD_DEFINED__ */


#ifndef __Procedures_FWD_DEFINED__
#define __Procedures_FWD_DEFINED__

#ifdef __cplusplus
typedef class Procedures Procedures;
#else
typedef struct Procedures Procedures;
#endif /* __cplusplus */

#endif 	/* __Procedures_FWD_DEFINED__ */


#ifndef __Module_FWD_DEFINED__
#define __Module_FWD_DEFINED__

#ifdef __cplusplus
typedef class Module Module;
#else
typedef struct Module Module;
#endif /* __cplusplus */

#endif 	/* __Module_FWD_DEFINED__ */


#ifndef __Modules_FWD_DEFINED__
#define __Modules_FWD_DEFINED__

#ifdef __cplusplus
typedef class Modules Modules;
#else
typedef struct Modules Modules;
#endif /* __cplusplus */

#endif 	/* __Modules_FWD_DEFINED__ */


#ifndef __Error_FWD_DEFINED__
#define __Error_FWD_DEFINED__

#ifdef __cplusplus
typedef class Error Error;
#else
typedef struct Error Error;
#endif /* __cplusplus */

#endif 	/* __Error_FWD_DEFINED__ */


#ifndef __ScriptControl_FWD_DEFINED__
#define __ScriptControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class ScriptControl ScriptControl;
#else
typedef struct ScriptControl ScriptControl;
#endif /* __cplusplus */

#endif 	/* __ScriptControl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __MSScriptControl_LIBRARY_DEFINED__
#define __MSScriptControl_LIBRARY_DEFINED__

/* library MSScriptControl */
/* [helpstringdll][version][lcid][helpfile][uuid] */ 

typedef /* [public][public][public] */ 
enum __MIDL___MIDL_itf_msscript_0080_0001
    {	Initialized	= 0,
	Connected	= 1,
	Disconnected	= 2
    }	ScriptControlStates;


EXTERN_C const IID LIBID_MSScriptControl;


#ifndef __ScriptControlConstants_MODULE_DEFINED__
#define __ScriptControlConstants_MODULE_DEFINED__


/* module ScriptControlConstants */
/* [dllname][uuid] */ 

const LPSTR GlobalModule	=	"Global";

const long NoTimeout	=	-1;

#endif /* __ScriptControlConstants_MODULE_DEFINED__ */

#ifndef __IScriptProcedure_INTERFACE_DEFINED__
#define __IScriptProcedure_INTERFACE_DEFINED__

/* interface IScriptProcedure */
/* [object][uuid][hidden][nonextensible][dual] */ 


EXTERN_C const IID IID_IScriptProcedure;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70841C73-067D-11D0-95D8-00A02463AB28")
    IScriptProcedure : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_NumArgs( 
            /* [retval][out] */ long __RPC_FAR *pcArgs) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_HasReturnValue( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfHasReturnValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptProcedureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScriptProcedure __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScriptProcedure __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScriptProcedure __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScriptProcedure __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScriptProcedure __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScriptProcedure __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScriptProcedure __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IScriptProcedure __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumArgs )( 
            IScriptProcedure __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pcArgs);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HasReturnValue )( 
            IScriptProcedure __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfHasReturnValue);
        
        END_INTERFACE
    } IScriptProcedureVtbl;

    interface IScriptProcedure
    {
        CONST_VTBL struct IScriptProcedureVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptProcedure_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScriptProcedure_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScriptProcedure_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScriptProcedure_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScriptProcedure_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScriptProcedure_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScriptProcedure_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScriptProcedure_get_Name(This,pbstrName)	\
    (This)->lpVtbl -> get_Name(This,pbstrName)

#define IScriptProcedure_get_NumArgs(This,pcArgs)	\
    (This)->lpVtbl -> get_NumArgs(This,pcArgs)

#define IScriptProcedure_get_HasReturnValue(This,pfHasReturnValue)	\
    (This)->lpVtbl -> get_HasReturnValue(This,pfHasReturnValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptProcedure_get_Name_Proxy( 
    IScriptProcedure __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IScriptProcedure_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptProcedure_get_NumArgs_Proxy( 
    IScriptProcedure __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pcArgs);


void __RPC_STUB IScriptProcedure_get_NumArgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptProcedure_get_HasReturnValue_Proxy( 
    IScriptProcedure __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfHasReturnValue);


void __RPC_STUB IScriptProcedure_get_HasReturnValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScriptProcedure_INTERFACE_DEFINED__ */


#ifndef __IScriptProcedureCollection_INTERFACE_DEFINED__
#define __IScriptProcedureCollection_INTERFACE_DEFINED__

/* interface IScriptProcedureCollection */
/* [object][uuid][hidden][nonextensible][dual] */ 


EXTERN_C const IID IID_IScriptProcedureCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70841C71-067D-11D0-95D8-00A02463AB28")
    IScriptProcedureCollection : public IDispatch
    {
    public:
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppenumProcedures) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IScriptProcedure __RPC_FAR *__RPC_FAR *ppdispProcedure) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *plCount) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptProcedureCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScriptProcedureCollection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScriptProcedureCollection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppenumProcedures);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IScriptProcedure __RPC_FAR *__RPC_FAR *ppdispProcedure);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IScriptProcedureCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plCount);
        
        END_INTERFACE
    } IScriptProcedureCollectionVtbl;

    interface IScriptProcedureCollection
    {
        CONST_VTBL struct IScriptProcedureCollectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptProcedureCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScriptProcedureCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScriptProcedureCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScriptProcedureCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScriptProcedureCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScriptProcedureCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScriptProcedureCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScriptProcedureCollection_get__NewEnum(This,ppenumProcedures)	\
    (This)->lpVtbl -> get__NewEnum(This,ppenumProcedures)

#define IScriptProcedureCollection_get_Item(This,Index,ppdispProcedure)	\
    (This)->lpVtbl -> get_Item(This,Index,ppdispProcedure)

#define IScriptProcedureCollection_get_Count(This,plCount)	\
    (This)->lpVtbl -> get_Count(This,plCount)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE IScriptProcedureCollection_get__NewEnum_Proxy( 
    IScriptProcedureCollection __RPC_FAR * This,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppenumProcedures);


void __RPC_STUB IScriptProcedureCollection_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptProcedureCollection_get_Item_Proxy( 
    IScriptProcedureCollection __RPC_FAR * This,
    /* [in] */ VARIANT Index,
    /* [retval][out] */ IScriptProcedure __RPC_FAR *__RPC_FAR *ppdispProcedure);


void __RPC_STUB IScriptProcedureCollection_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptProcedureCollection_get_Count_Proxy( 
    IScriptProcedureCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plCount);


void __RPC_STUB IScriptProcedureCollection_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScriptProcedureCollection_INTERFACE_DEFINED__ */


#ifndef __IScriptModule_INTERFACE_DEFINED__
#define __IScriptModule_INTERFACE_DEFINED__

/* interface IScriptModule */
/* [object][uuid][hidden][nonextensible][dual] */ 


EXTERN_C const IID IID_IScriptModule;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70841C70-067D-11D0-95D8-00A02463AB28")
    IScriptModule : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_CodeObject( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispObject) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Procedures( 
            /* [retval][out] */ IScriptProcedureCollection __RPC_FAR *__RPC_FAR *ppdispProcedures) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddCode( 
            /* [in] */ BSTR Code) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddScriptlet( 
            /* [in] */ BSTR bstrDefaultName,
            /* [in] */ BSTR bstrCode,
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrSubItemName,
            /* [in] */ BSTR bstrEventName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Eval( 
            /* [in] */ BSTR Expression,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ExecuteStatement( 
            /* [in] */ BSTR Statement,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult) = 0;
        
        virtual /* [vararg][id] */ HRESULT STDMETHODCALLTYPE Run( 
            /* [in] */ BSTR ProcedureName,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Parameters,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptModuleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScriptModule __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScriptModule __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScriptModule __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CodeObject )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispObject);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Procedures )( 
            IScriptModule __RPC_FAR * This,
            /* [retval][out] */ IScriptProcedureCollection __RPC_FAR *__RPC_FAR *ppdispProcedures);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddCode )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR Code);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddScriptlet )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR bstrDefaultName,
            /* [in] */ BSTR bstrCode,
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrSubItemName,
            /* [in] */ BSTR bstrEventName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Eval )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR Expression,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExecuteStatement )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR Statement,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);
        
        /* [vararg][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run )( 
            IScriptModule __RPC_FAR * This,
            /* [in] */ BSTR ProcedureName,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Parameters,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);
        
        END_INTERFACE
    } IScriptModuleVtbl;

    interface IScriptModule
    {
        CONST_VTBL struct IScriptModuleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptModule_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScriptModule_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScriptModule_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScriptModule_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScriptModule_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScriptModule_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScriptModule_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScriptModule_get_Name(This,pbstrName)	\
    (This)->lpVtbl -> get_Name(This,pbstrName)

#define IScriptModule_get_CodeObject(This,ppdispObject)	\
    (This)->lpVtbl -> get_CodeObject(This,ppdispObject)

#define IScriptModule_get_Procedures(This,ppdispProcedures)	\
    (This)->lpVtbl -> get_Procedures(This,ppdispProcedures)

#define IScriptModule_AddCode(This,Code)	\
    (This)->lpVtbl -> AddCode(This,Code)

#define IScriptModule_AddScriptlet(This,bstrDefaultName,bstrCode,bstrItemName,bstrSubItemName,bstrEventName,pbstrName)	\
    (This)->lpVtbl -> AddScriptlet(This,bstrDefaultName,bstrCode,bstrItemName,bstrSubItemName,bstrEventName,pbstrName)

#define IScriptModule_Eval(This,Expression,pvarResult)	\
    (This)->lpVtbl -> Eval(This,Expression,pvarResult)

#define IScriptModule_ExecuteStatement(This,Statement,pvarResult)	\
    (This)->lpVtbl -> ExecuteStatement(This,Statement,pvarResult)

#define IScriptModule_Run(This,ProcedureName,Parameters,pvarResult)	\
    (This)->lpVtbl -> Run(This,ProcedureName,Parameters,pvarResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptModule_get_Name_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IScriptModule_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptModule_get_CodeObject_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispObject);


void __RPC_STUB IScriptModule_get_CodeObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptModule_get_Procedures_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [retval][out] */ IScriptProcedureCollection __RPC_FAR *__RPC_FAR *ppdispProcedures);


void __RPC_STUB IScriptModule_get_Procedures_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptModule_AddCode_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [in] */ BSTR Code);


void __RPC_STUB IScriptModule_AddCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptModule_AddScriptlet_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [in] */ BSTR bstrDefaultName,
    /* [in] */ BSTR bstrCode,
    /* [in] */ BSTR bstrItemName,
    /* [in] */ BSTR bstrSubItemName,
    /* [in] */ BSTR bstrEventName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IScriptModule_AddScriptlet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptModule_Eval_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [in] */ BSTR Expression,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IScriptModule_Eval_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptModule_ExecuteStatement_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [in] */ BSTR Statement,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IScriptModule_ExecuteStatement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [vararg][id] */ HRESULT STDMETHODCALLTYPE IScriptModule_Run_Proxy( 
    IScriptModule __RPC_FAR * This,
    /* [in] */ BSTR ProcedureName,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Parameters,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IScriptModule_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScriptModule_INTERFACE_DEFINED__ */


#ifndef __IScriptModuleCollection_INTERFACE_DEFINED__
#define __IScriptModuleCollection_INTERFACE_DEFINED__

/* interface IScriptModuleCollection */
/* [object][uuid][hidden][nonextensible][dual] */ 


EXTERN_C const IID IID_IScriptModuleCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70841C6F-067D-11D0-95D8-00A02463AB28")
    IScriptModuleCollection : public IDispatch
    {
    public:
        virtual /* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppenumContexts) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IScriptModule __RPC_FAR *__RPC_FAR *ppmod) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *plCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT __RPC_FAR *Object,
            /* [retval][out] */ IScriptModule __RPC_FAR *__RPC_FAR *ppmod) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptModuleCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScriptModuleCollection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScriptModuleCollection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppenumContexts);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IScriptModule __RPC_FAR *__RPC_FAR *ppmod);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IScriptModuleCollection __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT __RPC_FAR *Object,
            /* [retval][out] */ IScriptModule __RPC_FAR *__RPC_FAR *ppmod);
        
        END_INTERFACE
    } IScriptModuleCollectionVtbl;

    interface IScriptModuleCollection
    {
        CONST_VTBL struct IScriptModuleCollectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptModuleCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScriptModuleCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScriptModuleCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScriptModuleCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScriptModuleCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScriptModuleCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScriptModuleCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScriptModuleCollection_get__NewEnum(This,ppenumContexts)	\
    (This)->lpVtbl -> get__NewEnum(This,ppenumContexts)

#define IScriptModuleCollection_get_Item(This,Index,ppmod)	\
    (This)->lpVtbl -> get_Item(This,Index,ppmod)

#define IScriptModuleCollection_get_Count(This,plCount)	\
    (This)->lpVtbl -> get_Count(This,plCount)

#define IScriptModuleCollection_Add(This,Name,Object,ppmod)	\
    (This)->lpVtbl -> Add(This,Name,Object,ppmod)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][propget][id] */ HRESULT STDMETHODCALLTYPE IScriptModuleCollection_get__NewEnum_Proxy( 
    IScriptModuleCollection __RPC_FAR * This,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppenumContexts);


void __RPC_STUB IScriptModuleCollection_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptModuleCollection_get_Item_Proxy( 
    IScriptModuleCollection __RPC_FAR * This,
    /* [in] */ VARIANT Index,
    /* [retval][out] */ IScriptModule __RPC_FAR *__RPC_FAR *ppmod);


void __RPC_STUB IScriptModuleCollection_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptModuleCollection_get_Count_Proxy( 
    IScriptModuleCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plCount);


void __RPC_STUB IScriptModuleCollection_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptModuleCollection_Add_Proxy( 
    IScriptModuleCollection __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [optional][in] */ VARIANT __RPC_FAR *Object,
    /* [retval][out] */ IScriptModule __RPC_FAR *__RPC_FAR *ppmod);


void __RPC_STUB IScriptModuleCollection_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScriptModuleCollection_INTERFACE_DEFINED__ */


#ifndef __IScriptError_INTERFACE_DEFINED__
#define __IScriptError_INTERFACE_DEFINED__

/* interface IScriptError */
/* [object][uuid][hidden][nonextensible][dual] */ 


EXTERN_C const IID IID_IScriptError;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70841C78-067D-11D0-95D8-00A02463AB28")
    IScriptError : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Number( 
            /* [retval][out] */ long __RPC_FAR *plNumber) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Source( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSource) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDescription) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpFile( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHelpFile) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpContext( 
            /* [retval][out] */ long __RPC_FAR *plHelpContext) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Text( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Line( 
            /* [retval][out] */ long __RPC_FAR *plLine) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Column( 
            /* [retval][out] */ long __RPC_FAR *plColumn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptErrorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScriptError __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScriptError __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScriptError __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScriptError __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScriptError __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScriptError __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScriptError __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Number )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plNumber);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Source )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSource);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Description )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDescription);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HelpFile )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHelpFile);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HelpContext )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plHelpContext);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Text )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Line )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLine);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Column )( 
            IScriptError __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plColumn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clear )( 
            IScriptError __RPC_FAR * This);
        
        END_INTERFACE
    } IScriptErrorVtbl;

    interface IScriptError
    {
        CONST_VTBL struct IScriptErrorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptError_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScriptError_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScriptError_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScriptError_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScriptError_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScriptError_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScriptError_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScriptError_get_Number(This,plNumber)	\
    (This)->lpVtbl -> get_Number(This,plNumber)

#define IScriptError_get_Source(This,pbstrSource)	\
    (This)->lpVtbl -> get_Source(This,pbstrSource)

#define IScriptError_get_Description(This,pbstrDescription)	\
    (This)->lpVtbl -> get_Description(This,pbstrDescription)

#define IScriptError_get_HelpFile(This,pbstrHelpFile)	\
    (This)->lpVtbl -> get_HelpFile(This,pbstrHelpFile)

#define IScriptError_get_HelpContext(This,plHelpContext)	\
    (This)->lpVtbl -> get_HelpContext(This,plHelpContext)

#define IScriptError_get_Text(This,pbstrText)	\
    (This)->lpVtbl -> get_Text(This,pbstrText)

#define IScriptError_get_Line(This,plLine)	\
    (This)->lpVtbl -> get_Line(This,plLine)

#define IScriptError_get_Column(This,plColumn)	\
    (This)->lpVtbl -> get_Column(This,plColumn)

#define IScriptError_Clear(This)	\
    (This)->lpVtbl -> Clear(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_Number_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plNumber);


void __RPC_STUB IScriptError_get_Number_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_Source_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrSource);


void __RPC_STUB IScriptError_get_Source_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_Description_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrDescription);


void __RPC_STUB IScriptError_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_HelpFile_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHelpFile);


void __RPC_STUB IScriptError_get_HelpFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_HelpContext_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plHelpContext);


void __RPC_STUB IScriptError_get_HelpContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_Text_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrText);


void __RPC_STUB IScriptError_get_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_Line_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLine);


void __RPC_STUB IScriptError_get_Line_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptError_get_Column_Proxy( 
    IScriptError __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plColumn);


void __RPC_STUB IScriptError_get_Column_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptError_Clear_Proxy( 
    IScriptError __RPC_FAR * This);


void __RPC_STUB IScriptError_Clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScriptError_INTERFACE_DEFINED__ */


#ifndef __IScriptControl_INTERFACE_DEFINED__
#define __IScriptControl_INTERFACE_DEFINED__

/* interface IScriptControl */
/* [object][uuid][hidden][nonextensible][dual] */ 


EXTERN_C const IID IID_IScriptControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0E59F1D3-1FBE-11D0-8FF2-00A0D10038BC")
    IScriptControl : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Language( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLanguage) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Language( 
            /* [in] */ BSTR bstrLanguage) = 0;
        
        virtual /* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE get_State( 
            /* [retval][out] */ ScriptControlStates __RPC_FAR *pssState) = 0;
        
        virtual /* [nonbrowsable][propput][id] */ HRESULT STDMETHODCALLTYPE put_State( 
            /* [in] */ ScriptControlStates ssState) = 0;
        
        virtual /* [nonbrowsable][propput][id] */ HRESULT STDMETHODCALLTYPE put_SitehWnd( 
            /* [in] */ long hwnd) = 0;
        
        virtual /* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE get_SitehWnd( 
            /* [retval][out] */ long __RPC_FAR *phwnd) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Timeout( 
            /* [retval][out] */ long __RPC_FAR *plMilleseconds) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Timeout( 
            /* [in] */ long lMilleseconds) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_AllowUI( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfAllowUI) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_AllowUI( 
            /* [in] */ VARIANT_BOOL fAllowUI) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_UseSafeSubset( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfUseSafeSubset) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_UseSafeSubset( 
            /* [in] */ VARIANT_BOOL fUseSafeSubset) = 0;
        
        virtual /* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Modules( 
            /* [retval][out] */ IScriptModuleCollection __RPC_FAR *__RPC_FAR *ppmods) = 0;
        
        virtual /* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Error( 
            /* [retval][out] */ IScriptError __RPC_FAR *__RPC_FAR *ppse) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_CodeObject( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispObject) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Procedures( 
            /* [retval][out] */ IScriptProcedureCollection __RPC_FAR *__RPC_FAR *ppdispProcedures) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddObject( 
            /* [in] */ BSTR Name,
            /* [in] */ IDispatch __RPC_FAR *Object,
            /* [defaultvalue][in] */ VARIANT_BOOL AddMembers = 0) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddCode( 
            /* [in] */ BSTR Code) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddScriptlet( 
            /* [in] */ BSTR bstrDefaultName,
            /* [in] */ BSTR bstrCode,
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrSubItemName,
            /* [in] */ BSTR bstrEventName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Eval( 
            /* [in] */ BSTR Expression,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ExecuteStatement( 
            /* [in] */ BSTR Statement,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult) = 0;
        
        virtual /* [vararg][id] */ HRESULT STDMETHODCALLTYPE Run( 
            /* [in] */ BSTR ProcedureName,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Parameters,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Interrupt( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScriptControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IScriptControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IScriptControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IScriptControl __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Language )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLanguage);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Language )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ BSTR bstrLanguage);
        
        /* [nonbrowsable][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_State )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ ScriptControlStates __RPC_FAR *pssState);
        
        /* [nonbrowsable][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_State )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ ScriptControlStates ssState);
        
        /* [nonbrowsable][propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SitehWnd )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ long hwnd);
        
        /* [nonbrowsable][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SitehWnd )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *phwnd);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Timeout )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plMilleseconds);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Timeout )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ long lMilleseconds);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AllowUI )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfAllowUI);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AllowUI )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL fAllowUI);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UseSafeSubset )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfUseSafeSubset);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UseSafeSubset )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL fUseSafeSubset);
        
        /* [nonbrowsable][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Modules )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ IScriptModuleCollection __RPC_FAR *__RPC_FAR *ppmods);
        
        /* [nonbrowsable][propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Error )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ IScriptError __RPC_FAR *__RPC_FAR *ppse);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CodeObject )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispObject);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Procedures )( 
            IScriptControl __RPC_FAR * This,
            /* [retval][out] */ IScriptProcedureCollection __RPC_FAR *__RPC_FAR *ppdispProcedures);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddObject )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ BSTR Name,
            /* [in] */ IDispatch __RPC_FAR *Object,
            /* [defaultvalue][in] */ VARIANT_BOOL AddMembers);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IScriptControl __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddCode )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ BSTR Code);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddScriptlet )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ BSTR bstrDefaultName,
            /* [in] */ BSTR bstrCode,
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrSubItemName,
            /* [in] */ BSTR bstrEventName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Eval )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ BSTR Expression,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExecuteStatement )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ BSTR Statement,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);
        
        /* [vararg][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run )( 
            IScriptControl __RPC_FAR * This,
            /* [in] */ BSTR ProcedureName,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Parameters,
            /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Interrupt )( 
            IScriptControl __RPC_FAR * This);
        
        END_INTERFACE
    } IScriptControlVtbl;

    interface IScriptControl
    {
        CONST_VTBL struct IScriptControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScriptControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScriptControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScriptControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScriptControl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScriptControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScriptControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScriptControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScriptControl_get_Language(This,pbstrLanguage)	\
    (This)->lpVtbl -> get_Language(This,pbstrLanguage)

#define IScriptControl_put_Language(This,bstrLanguage)	\
    (This)->lpVtbl -> put_Language(This,bstrLanguage)

#define IScriptControl_get_State(This,pssState)	\
    (This)->lpVtbl -> get_State(This,pssState)

#define IScriptControl_put_State(This,ssState)	\
    (This)->lpVtbl -> put_State(This,ssState)

#define IScriptControl_put_SitehWnd(This,hwnd)	\
    (This)->lpVtbl -> put_SitehWnd(This,hwnd)

#define IScriptControl_get_SitehWnd(This,phwnd)	\
    (This)->lpVtbl -> get_SitehWnd(This,phwnd)

#define IScriptControl_get_Timeout(This,plMilleseconds)	\
    (This)->lpVtbl -> get_Timeout(This,plMilleseconds)

#define IScriptControl_put_Timeout(This,lMilleseconds)	\
    (This)->lpVtbl -> put_Timeout(This,lMilleseconds)

#define IScriptControl_get_AllowUI(This,pfAllowUI)	\
    (This)->lpVtbl -> get_AllowUI(This,pfAllowUI)

#define IScriptControl_put_AllowUI(This,fAllowUI)	\
    (This)->lpVtbl -> put_AllowUI(This,fAllowUI)

#define IScriptControl_get_UseSafeSubset(This,pfUseSafeSubset)	\
    (This)->lpVtbl -> get_UseSafeSubset(This,pfUseSafeSubset)

#define IScriptControl_put_UseSafeSubset(This,fUseSafeSubset)	\
    (This)->lpVtbl -> put_UseSafeSubset(This,fUseSafeSubset)

#define IScriptControl_get_Modules(This,ppmods)	\
    (This)->lpVtbl -> get_Modules(This,ppmods)

#define IScriptControl_get_Error(This,ppse)	\
    (This)->lpVtbl -> get_Error(This,ppse)

#define IScriptControl_get_CodeObject(This,ppdispObject)	\
    (This)->lpVtbl -> get_CodeObject(This,ppdispObject)

#define IScriptControl_get_Procedures(This,ppdispProcedures)	\
    (This)->lpVtbl -> get_Procedures(This,ppdispProcedures)

#define IScriptControl_AddObject(This,Name,Object,AddMembers)	\
    (This)->lpVtbl -> AddObject(This,Name,Object,AddMembers)

#define IScriptControl_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IScriptControl_AddCode(This,Code)	\
    (This)->lpVtbl -> AddCode(This,Code)

#define IScriptControl_AddScriptlet(This,bstrDefaultName,bstrCode,bstrItemName,bstrSubItemName,bstrEventName,pbstrName)	\
    (This)->lpVtbl -> AddScriptlet(This,bstrDefaultName,bstrCode,bstrItemName,bstrSubItemName,bstrEventName,pbstrName)

#define IScriptControl_Eval(This,Expression,pvarResult)	\
    (This)->lpVtbl -> Eval(This,Expression,pvarResult)

#define IScriptControl_ExecuteStatement(This,Statement,pvarResult)	\
    (This)->lpVtbl -> ExecuteStatement(This,Statement,pvarResult)

#define IScriptControl_Run(This,ProcedureName,Parameters,pvarResult)	\
    (This)->lpVtbl -> Run(This,ProcedureName,Parameters,pvarResult)

#define IScriptControl_Interrupt(This)	\
    (This)->lpVtbl -> Interrupt(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_Language_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrLanguage);


void __RPC_STUB IScriptControl_get_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_put_Language_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ BSTR bstrLanguage);


void __RPC_STUB IScriptControl_put_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_State_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ ScriptControlStates __RPC_FAR *pssState);


void __RPC_STUB IScriptControl_get_State_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [nonbrowsable][propput][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_put_State_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ ScriptControlStates ssState);


void __RPC_STUB IScriptControl_put_State_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [nonbrowsable][propput][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_put_SitehWnd_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ long hwnd);


void __RPC_STUB IScriptControl_put_SitehWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_SitehWnd_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *phwnd);


void __RPC_STUB IScriptControl_get_SitehWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_Timeout_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plMilleseconds);


void __RPC_STUB IScriptControl_get_Timeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_put_Timeout_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ long lMilleseconds);


void __RPC_STUB IScriptControl_put_Timeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_AllowUI_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfAllowUI);


void __RPC_STUB IScriptControl_get_AllowUI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_put_AllowUI_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL fAllowUI);


void __RPC_STUB IScriptControl_put_AllowUI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_UseSafeSubset_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pfUseSafeSubset);


void __RPC_STUB IScriptControl_get_UseSafeSubset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_put_UseSafeSubset_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL fUseSafeSubset);


void __RPC_STUB IScriptControl_put_UseSafeSubset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_Modules_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ IScriptModuleCollection __RPC_FAR *__RPC_FAR *ppmods);


void __RPC_STUB IScriptControl_get_Modules_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [nonbrowsable][propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_Error_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ IScriptError __RPC_FAR *__RPC_FAR *ppse);


void __RPC_STUB IScriptControl_get_Error_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_CodeObject_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispObject);


void __RPC_STUB IScriptControl_get_CodeObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_get_Procedures_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [retval][out] */ IScriptProcedureCollection __RPC_FAR *__RPC_FAR *ppdispProcedures);


void __RPC_STUB IScriptControl_get_Procedures_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptControl_AddObject_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ BSTR Name,
    /* [in] */ IDispatch __RPC_FAR *Object,
    /* [defaultvalue][in] */ VARIANT_BOOL AddMembers);


void __RPC_STUB IScriptControl_AddObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptControl_Reset_Proxy( 
    IScriptControl __RPC_FAR * This);


void __RPC_STUB IScriptControl_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptControl_AddCode_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ BSTR Code);


void __RPC_STUB IScriptControl_AddCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptControl_AddScriptlet_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ BSTR bstrDefaultName,
    /* [in] */ BSTR bstrCode,
    /* [in] */ BSTR bstrItemName,
    /* [in] */ BSTR bstrSubItemName,
    /* [in] */ BSTR bstrEventName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IScriptControl_AddScriptlet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptControl_Eval_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ BSTR Expression,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IScriptControl_Eval_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptControl_ExecuteStatement_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ BSTR Statement,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IScriptControl_ExecuteStatement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [vararg][id] */ HRESULT STDMETHODCALLTYPE IScriptControl_Run_Proxy( 
    IScriptControl __RPC_FAR * This,
    /* [in] */ BSTR ProcedureName,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Parameters,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IScriptControl_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IScriptControl_Interrupt_Proxy( 
    IScriptControl __RPC_FAR * This);


void __RPC_STUB IScriptControl_Interrupt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScriptControl_INTERFACE_DEFINED__ */


#ifndef __DScriptControlSource_DISPINTERFACE_DEFINED__
#define __DScriptControlSource_DISPINTERFACE_DEFINED__

/* dispinterface DScriptControlSource */
/* [hidden][uuid] */ 


EXTERN_C const IID DIID_DScriptControlSource;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("8B167D60-8605-11d0-ABCB-00A0C90FFFC0")
    DScriptControlSource : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct DScriptControlSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            DScriptControlSource __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            DScriptControlSource __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            DScriptControlSource __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            DScriptControlSource __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            DScriptControlSource __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            DScriptControlSource __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            DScriptControlSource __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } DScriptControlSourceVtbl;

    interface DScriptControlSource
    {
        CONST_VTBL struct DScriptControlSourceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DScriptControlSource_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define DScriptControlSource_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define DScriptControlSource_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define DScriptControlSource_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define DScriptControlSource_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define DScriptControlSource_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define DScriptControlSource_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __DScriptControlSource_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Procedure;

#ifdef __cplusplus

class DECLSPEC_UUID("0E59F1DA-1FBE-11D0-8FF2-00A0D10038BC")
Procedure;
#endif

EXTERN_C const CLSID CLSID_Procedures;

#ifdef __cplusplus

class DECLSPEC_UUID("0E59F1DB-1FBE-11D0-8FF2-00A0D10038BC")
Procedures;
#endif

EXTERN_C const CLSID CLSID_Module;

#ifdef __cplusplus

class DECLSPEC_UUID("0E59F1DC-1FBE-11D0-8FF2-00A0D10038BC")
Module;
#endif

EXTERN_C const CLSID CLSID_Modules;

#ifdef __cplusplus

class DECLSPEC_UUID("0E59F1DD-1FBE-11D0-8FF2-00A0D10038BC")
Modules;
#endif

EXTERN_C const CLSID CLSID_Error;

#ifdef __cplusplus

class DECLSPEC_UUID("0E59F1DE-1FBE-11D0-8FF2-00A0D10038BC")
Error;
#endif

EXTERN_C const CLSID CLSID_ScriptControl;

#ifdef __cplusplus

class DECLSPEC_UUID("0E59F1D5-1FBE-11D0-8FF2-00A0D10038BC")
ScriptControl;
#endif
#endif /* __MSScriptControl_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


