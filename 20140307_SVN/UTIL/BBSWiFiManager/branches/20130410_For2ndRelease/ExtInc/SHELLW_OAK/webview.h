
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:32:11 2008
 */
/* Compiler settings for .\webview.idl:
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

#ifndef __webview_h__
#define __webview_h__

/* Forward Declarations */ 

#ifndef __IBrowser_FWD_DEFINED__
#define __IBrowser_FWD_DEFINED__
typedef interface IBrowser IBrowser;
#endif 	/* __IBrowser_FWD_DEFINED__ */


#ifndef __IBrowser2_FWD_DEFINED__
#define __IBrowser2_FWD_DEFINED__
typedef interface IBrowser2 IBrowser2;
#endif 	/* __IBrowser2_FWD_DEFINED__ */


#ifndef __IBrowser3_FWD_DEFINED__
#define __IBrowser3_FWD_DEFINED__
typedef interface IBrowser3 IBrowser3;
#endif 	/* __IBrowser3_FWD_DEFINED__ */


#ifndef __ILegacyHtml_FWD_DEFINED__
#define __ILegacyHtml_FWD_DEFINED__
typedef interface ILegacyHtml ILegacyHtml;
#endif 	/* __ILegacyHtml_FWD_DEFINED__ */


#ifndef __IHTMLHistoryEntry_FWD_DEFINED__
#define __IHTMLHistoryEntry_FWD_DEFINED__
typedef interface IHTMLHistoryEntry IHTMLHistoryEntry;
#endif 	/* __IHTMLHistoryEntry_FWD_DEFINED__ */


#ifndef __IHTMLBackDoor_FWD_DEFINED__
#define __IHTMLBackDoor_FWD_DEFINED__
typedef interface IHTMLBackDoor IHTMLBackDoor;
#endif 	/* __IHTMLBackDoor_FWD_DEFINED__ */


#ifndef __IViewerState_FWD_DEFINED__
#define __IViewerState_FWD_DEFINED__
typedef interface IViewerState IViewerState;
#endif 	/* __IViewerState_FWD_DEFINED__ */


#ifndef __IRequestManager_FWD_DEFINED__
#define __IRequestManager_FWD_DEFINED__
typedef interface IRequestManager IRequestManager;
#endif 	/* __IRequestManager_FWD_DEFINED__ */


#ifndef __IDownloadRequestProvider_FWD_DEFINED__
#define __IDownloadRequestProvider_FWD_DEFINED__
typedef interface IDownloadRequestProvider IDownloadRequestProvider;
#endif 	/* __IDownloadRequestProvider_FWD_DEFINED__ */


#ifndef __IDownloadRequestSink_FWD_DEFINED__
#define __IDownloadRequestSink_FWD_DEFINED__
typedef interface IDownloadRequestSink IDownloadRequestSink;
#endif 	/* __IDownloadRequestSink_FWD_DEFINED__ */


#ifndef __IInetConn_FWD_DEFINED__
#define __IInetConn_FWD_DEFINED__
typedef interface IInetConn IInetConn;
#endif 	/* __IInetConn_FWD_DEFINED__ */


#ifndef __IPIEViewerHost_FWD_DEFINED__
#define __IPIEViewerHost_FWD_DEFINED__
typedef interface IPIEViewerHost IPIEViewerHost;
#endif 	/* __IPIEViewerHost_FWD_DEFINED__ */


#ifndef __IPIEElement_FWD_DEFINED__
#define __IPIEElement_FWD_DEFINED__
typedef interface IPIEElement IPIEElement;
#endif 	/* __IPIEElement_FWD_DEFINED__ */


#ifndef __IFrameHandler_FWD_DEFINED__
#define __IFrameHandler_FWD_DEFINED__
typedef interface IFrameHandler IFrameHandler;
#endif 	/* __IFrameHandler_FWD_DEFINED__ */


#ifndef __IXMLHandler_FWD_DEFINED__
#define __IXMLHandler_FWD_DEFINED__
typedef interface IXMLHandler IXMLHandler;
#endif 	/* __IXMLHandler_FWD_DEFINED__ */


#ifndef __IElementNamespace_FWD_DEFINED__
#define __IElementNamespace_FWD_DEFINED__
typedef interface IElementNamespace IElementNamespace;
#endif 	/* __IElementNamespace_FWD_DEFINED__ */


#ifndef __IElementNamespaceFactory_FWD_DEFINED__
#define __IElementNamespaceFactory_FWD_DEFINED__
typedef interface IElementNamespaceFactory IElementNamespaceFactory;
#endif 	/* __IElementNamespaceFactory_FWD_DEFINED__ */


#ifndef __IElementBehaviorSite_FWD_DEFINED__
#define __IElementBehaviorSite_FWD_DEFINED__
typedef interface IElementBehaviorSite IElementBehaviorSite;
#endif 	/* __IElementBehaviorSite_FWD_DEFINED__ */


#ifndef __IElementBehavior_FWD_DEFINED__
#define __IElementBehavior_FWD_DEFINED__
typedef interface IElementBehavior IElementBehavior;
#endif 	/* __IElementBehavior_FWD_DEFINED__ */


#ifndef __IElementBehaviorFactory_FWD_DEFINED__
#define __IElementBehaviorFactory_FWD_DEFINED__
typedef interface IElementBehaviorFactory IElementBehaviorFactory;
#endif 	/* __IElementBehaviorFactory_FWD_DEFINED__ */


#ifndef __CInetConn_FWD_DEFINED__
#define __CInetConn_FWD_DEFINED__

#ifdef __cplusplus
typedef class CInetConn CInetConn;
#else
typedef struct CInetConn CInetConn;
#endif /* __cplusplus */

#endif 	/* __CInetConn_FWD_DEFINED__ */


#ifndef __CFrameHandler_FWD_DEFINED__
#define __CFrameHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class CFrameHandler CFrameHandler;
#else
typedef struct CFrameHandler CFrameHandler;
#endif /* __cplusplus */

#endif 	/* __CFrameHandler_FWD_DEFINED__ */


#ifndef __CXMLHandler_FWD_DEFINED__
#define __CXMLHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class CXMLHandler CXMLHandler;
#else
typedef struct CXMLHandler CXMLHandler;
#endif /* __cplusplus */

#endif 	/* __CXMLHandler_FWD_DEFINED__ */


#ifndef __PIEHTML_FWD_DEFINED__
#define __PIEHTML_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTML PIEHTML;
#else
typedef struct PIEHTML PIEHTML;
#endif /* __cplusplus */

#endif 	/* __PIEHTML_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "urlmon.h"
#include "webvw.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_webview_0000 */
/* [local] */ 

#pragma once


extern RPC_IF_HANDLE __MIDL_itf_webview_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webview_0000_v0_0_s_ifspec;

#ifndef __IBrowser_INTERFACE_DEFINED__
#define __IBrowser_INTERFACE_DEFINED__

/* interface IBrowser */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IBrowser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("698E3FC9-70C3-11D0-81E8-00A0C90AD20A")
    IBrowser : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE navigate( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ long dwFlags,
            /* [in] */ BSTR bstrTargetFrameName,
            /* [in] */ BSTR bstrPostData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Busy( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_title( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LocationURL( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_IsFrame( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIsFrame) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_IsFrame( 
            /* [in] */ VARIANT_BOOL vbIsFrame) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ScrollStyle( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ScrollStyle( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MarginWidth( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MarginWidth( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MarginHeight( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MarginHeight( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FrameNavigate( 
            BSTR bstrURL,
            BSTR bstrTarget,
            BSTR bstrSource,
            BSTR bstrPostData,
            BOOL __RPC_FAR *Cancel) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SelectedCodepage( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_SelectedCodepage( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CBrowserPointer( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBrowser __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBrowser __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IBrowser __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *navigate )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ long dwFlags,
            /* [in] */ BSTR bstrTargetFrameName,
            /* [in] */ BSTR bstrPostData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IBrowser __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IBrowser __RPC_FAR * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Busy )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_title )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocationURL )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsFrame )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIsFrame);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsFrame )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbIsFrame);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScrollStyle )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScrollStyle )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MarginWidth )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MarginWidth )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MarginHeight )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MarginHeight )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FrameNavigate )( 
            IBrowser __RPC_FAR * This,
            BSTR bstrURL,
            BSTR bstrTarget,
            BSTR bstrSource,
            BSTR bstrPostData,
            BOOL __RPC_FAR *Cancel);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelectedCodepage )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SelectedCodepage )( 
            IBrowser __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CBrowserPointer )( 
            IBrowser __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        END_INTERFACE
    } IBrowserVtbl;

    interface IBrowser
    {
        CONST_VTBL struct IBrowserVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBrowser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBrowser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBrowser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBrowser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IBrowser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IBrowser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IBrowser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IBrowser_navigate(This,bstrURL,dwFlags,bstrTargetFrameName,bstrPostData)	\
    (This)->lpVtbl -> navigate(This,bstrURL,dwFlags,bstrTargetFrameName,bstrPostData)

#define IBrowser_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define IBrowser_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IBrowser_get_Busy(This,pVal)	\
    (This)->lpVtbl -> get_Busy(This,pVal)

#define IBrowser_get_title(This,pVal)	\
    (This)->lpVtbl -> get_title(This,pVal)

#define IBrowser_get_LocationURL(This,pVal)	\
    (This)->lpVtbl -> get_LocationURL(This,pVal)

#define IBrowser_get_IsFrame(This,pvbIsFrame)	\
    (This)->lpVtbl -> get_IsFrame(This,pvbIsFrame)

#define IBrowser_put_IsFrame(This,vbIsFrame)	\
    (This)->lpVtbl -> put_IsFrame(This,vbIsFrame)

#define IBrowser_get_ScrollStyle(This,pVal)	\
    (This)->lpVtbl -> get_ScrollStyle(This,pVal)

#define IBrowser_put_ScrollStyle(This,newVal)	\
    (This)->lpVtbl -> put_ScrollStyle(This,newVal)

#define IBrowser_get_MarginWidth(This,pVal)	\
    (This)->lpVtbl -> get_MarginWidth(This,pVal)

#define IBrowser_put_MarginWidth(This,newVal)	\
    (This)->lpVtbl -> put_MarginWidth(This,newVal)

#define IBrowser_get_MarginHeight(This,pVal)	\
    (This)->lpVtbl -> get_MarginHeight(This,pVal)

#define IBrowser_put_MarginHeight(This,newVal)	\
    (This)->lpVtbl -> put_MarginHeight(This,newVal)

#define IBrowser_FrameNavigate(This,bstrURL,bstrTarget,bstrSource,bstrPostData,Cancel)	\
    (This)->lpVtbl -> FrameNavigate(This,bstrURL,bstrTarget,bstrSource,bstrPostData,Cancel)

#define IBrowser_get_SelectedCodepage(This,pVal)	\
    (This)->lpVtbl -> get_SelectedCodepage(This,pVal)

#define IBrowser_put_SelectedCodepage(This,newVal)	\
    (This)->lpVtbl -> put_SelectedCodepage(This,newVal)

#define IBrowser_get_CBrowserPointer(This,pVal)	\
    (This)->lpVtbl -> get_CBrowserPointer(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IBrowser_navigate_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ long dwFlags,
    /* [in] */ BSTR bstrTargetFrameName,
    /* [in] */ BSTR bstrPostData);


void __RPC_STUB IBrowser_navigate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IBrowser_Refresh_Proxy( 
    IBrowser __RPC_FAR * This);


void __RPC_STUB IBrowser_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IBrowser_Stop_Proxy( 
    IBrowser __RPC_FAR * This);


void __RPC_STUB IBrowser_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_Busy_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_Busy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_title_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_LocationURL_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_LocationURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_IsFrame_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIsFrame);


void __RPC_STUB IBrowser_get_IsFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser_put_IsFrame_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbIsFrame);


void __RPC_STUB IBrowser_put_IsFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_ScrollStyle_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_ScrollStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser_put_ScrollStyle_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IBrowser_put_ScrollStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_MarginWidth_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_MarginWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser_put_MarginWidth_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IBrowser_put_MarginWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_MarginHeight_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_MarginHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser_put_MarginHeight_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IBrowser_put_MarginHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IBrowser_FrameNavigate_Proxy( 
    IBrowser __RPC_FAR * This,
    BSTR bstrURL,
    BSTR bstrTarget,
    BSTR bstrSource,
    BSTR bstrPostData,
    BOOL __RPC_FAR *Cancel);


void __RPC_STUB IBrowser_FrameNavigate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_SelectedCodepage_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_SelectedCodepage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser_put_SelectedCodepage_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IBrowser_put_SelectedCodepage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser_get_CBrowserPointer_Proxy( 
    IBrowser __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IBrowser_get_CBrowserPointer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBrowser_INTERFACE_DEFINED__ */


#ifndef __IBrowser2_INTERFACE_DEFINED__
#define __IBrowser2_INTERFACE_DEFINED__

/* interface IBrowser2 */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IBrowser2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("698E3FFF-70C3-11D0-81E8-00A0C90AD20A")
    IBrowser2 : public IBrowser
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LayoutWidth( 
            /* [retval][out] */ int __RPC_FAR *piLayoutWidth) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LayoutHeight( 
            /* [retval][out] */ int __RPC_FAR *piLayoutHeight) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LocationBaseURL( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrBaseLocationURL) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LocationBaseURL( 
            /* [in] */ BSTR bstrBaseLocationURL) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ClearTypeEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbClearTypeEnabled) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ClearTypeEnabled( 
            /* [in] */ VARIANT_BOOL bClearTypeEnabled) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ScriptingEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbScriptingEnabled) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ScriptingEnabled( 
            /* [in] */ VARIANT_BOOL bScriptingEnabled) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FitToWindow( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbFitToWindow) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FitToWindow( 
            /* [in] */ VARIANT_BOOL bFitToWindow) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowImages( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbShowImages) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowImages( 
            /* [in] */ VARIANT_BOOL bShowImages) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PlaySounds( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbPlaySounds) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PlaySounds( 
            /* [in] */ VARIANT_BOOL bPlaySounds) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ZoomLevel( 
            /* [retval][out] */ DWORD __RPC_FAR *pdwZoomLevel) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ZoomLevel( 
            /* [in] */ DWORD dwZoomLevel) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBrowser2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBrowser2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBrowser2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IBrowser2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *navigate )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ long dwFlags,
            /* [in] */ BSTR bstrTargetFrameName,
            /* [in] */ BSTR bstrPostData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IBrowser2 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IBrowser2 __RPC_FAR * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Busy )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_title )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocationURL )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsFrame )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIsFrame);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsFrame )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbIsFrame);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScrollStyle )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScrollStyle )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MarginWidth )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MarginWidth )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MarginHeight )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MarginHeight )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FrameNavigate )( 
            IBrowser2 __RPC_FAR * This,
            BSTR bstrURL,
            BSTR bstrTarget,
            BSTR bstrSource,
            BSTR bstrPostData,
            BOOL __RPC_FAR *Cancel);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelectedCodepage )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SelectedCodepage )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CBrowserPointer )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LayoutWidth )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piLayoutWidth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LayoutHeight )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piLayoutHeight);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocationBaseURL )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrBaseLocationURL);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LocationBaseURL )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ BSTR bstrBaseLocationURL);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ClearTypeEnabled )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbClearTypeEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ClearTypeEnabled )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bClearTypeEnabled);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScriptingEnabled )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbScriptingEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScriptingEnabled )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bScriptingEnabled);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FitToWindow )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbFitToWindow);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FitToWindow )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bFitToWindow);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShowImages )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbShowImages);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ShowImages )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bShowImages);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PlaySounds )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbPlaySounds);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PlaySounds )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bPlaySounds);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ZoomLevel )( 
            IBrowser2 __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwZoomLevel);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ZoomLevel )( 
            IBrowser2 __RPC_FAR * This,
            /* [in] */ DWORD dwZoomLevel);
        
        END_INTERFACE
    } IBrowser2Vtbl;

    interface IBrowser2
    {
        CONST_VTBL struct IBrowser2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBrowser2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBrowser2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBrowser2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBrowser2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IBrowser2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IBrowser2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IBrowser2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IBrowser2_navigate(This,bstrURL,dwFlags,bstrTargetFrameName,bstrPostData)	\
    (This)->lpVtbl -> navigate(This,bstrURL,dwFlags,bstrTargetFrameName,bstrPostData)

#define IBrowser2_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define IBrowser2_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IBrowser2_get_Busy(This,pVal)	\
    (This)->lpVtbl -> get_Busy(This,pVal)

#define IBrowser2_get_title(This,pVal)	\
    (This)->lpVtbl -> get_title(This,pVal)

#define IBrowser2_get_LocationURL(This,pVal)	\
    (This)->lpVtbl -> get_LocationURL(This,pVal)

#define IBrowser2_get_IsFrame(This,pvbIsFrame)	\
    (This)->lpVtbl -> get_IsFrame(This,pvbIsFrame)

#define IBrowser2_put_IsFrame(This,vbIsFrame)	\
    (This)->lpVtbl -> put_IsFrame(This,vbIsFrame)

#define IBrowser2_get_ScrollStyle(This,pVal)	\
    (This)->lpVtbl -> get_ScrollStyle(This,pVal)

#define IBrowser2_put_ScrollStyle(This,newVal)	\
    (This)->lpVtbl -> put_ScrollStyle(This,newVal)

#define IBrowser2_get_MarginWidth(This,pVal)	\
    (This)->lpVtbl -> get_MarginWidth(This,pVal)

#define IBrowser2_put_MarginWidth(This,newVal)	\
    (This)->lpVtbl -> put_MarginWidth(This,newVal)

#define IBrowser2_get_MarginHeight(This,pVal)	\
    (This)->lpVtbl -> get_MarginHeight(This,pVal)

#define IBrowser2_put_MarginHeight(This,newVal)	\
    (This)->lpVtbl -> put_MarginHeight(This,newVal)

#define IBrowser2_FrameNavigate(This,bstrURL,bstrTarget,bstrSource,bstrPostData,Cancel)	\
    (This)->lpVtbl -> FrameNavigate(This,bstrURL,bstrTarget,bstrSource,bstrPostData,Cancel)

#define IBrowser2_get_SelectedCodepage(This,pVal)	\
    (This)->lpVtbl -> get_SelectedCodepage(This,pVal)

#define IBrowser2_put_SelectedCodepage(This,newVal)	\
    (This)->lpVtbl -> put_SelectedCodepage(This,newVal)

#define IBrowser2_get_CBrowserPointer(This,pVal)	\
    (This)->lpVtbl -> get_CBrowserPointer(This,pVal)


#define IBrowser2_get_LayoutWidth(This,piLayoutWidth)	\
    (This)->lpVtbl -> get_LayoutWidth(This,piLayoutWidth)

#define IBrowser2_get_LayoutHeight(This,piLayoutHeight)	\
    (This)->lpVtbl -> get_LayoutHeight(This,piLayoutHeight)

#define IBrowser2_get_LocationBaseURL(This,bstrBaseLocationURL)	\
    (This)->lpVtbl -> get_LocationBaseURL(This,bstrBaseLocationURL)

#define IBrowser2_put_LocationBaseURL(This,bstrBaseLocationURL)	\
    (This)->lpVtbl -> put_LocationBaseURL(This,bstrBaseLocationURL)

#define IBrowser2_get_ClearTypeEnabled(This,pbClearTypeEnabled)	\
    (This)->lpVtbl -> get_ClearTypeEnabled(This,pbClearTypeEnabled)

#define IBrowser2_put_ClearTypeEnabled(This,bClearTypeEnabled)	\
    (This)->lpVtbl -> put_ClearTypeEnabled(This,bClearTypeEnabled)

#define IBrowser2_get_ScriptingEnabled(This,pbScriptingEnabled)	\
    (This)->lpVtbl -> get_ScriptingEnabled(This,pbScriptingEnabled)

#define IBrowser2_put_ScriptingEnabled(This,bScriptingEnabled)	\
    (This)->lpVtbl -> put_ScriptingEnabled(This,bScriptingEnabled)

#define IBrowser2_get_FitToWindow(This,pbFitToWindow)	\
    (This)->lpVtbl -> get_FitToWindow(This,pbFitToWindow)

#define IBrowser2_put_FitToWindow(This,bFitToWindow)	\
    (This)->lpVtbl -> put_FitToWindow(This,bFitToWindow)

#define IBrowser2_get_ShowImages(This,pbShowImages)	\
    (This)->lpVtbl -> get_ShowImages(This,pbShowImages)

#define IBrowser2_put_ShowImages(This,bShowImages)	\
    (This)->lpVtbl -> put_ShowImages(This,bShowImages)

#define IBrowser2_get_PlaySounds(This,pbPlaySounds)	\
    (This)->lpVtbl -> get_PlaySounds(This,pbPlaySounds)

#define IBrowser2_put_PlaySounds(This,bPlaySounds)	\
    (This)->lpVtbl -> put_PlaySounds(This,bPlaySounds)

#define IBrowser2_get_ZoomLevel(This,pdwZoomLevel)	\
    (This)->lpVtbl -> get_ZoomLevel(This,pdwZoomLevel)

#define IBrowser2_put_ZoomLevel(This,dwZoomLevel)	\
    (This)->lpVtbl -> put_ZoomLevel(This,dwZoomLevel)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_LayoutWidth_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *piLayoutWidth);


void __RPC_STUB IBrowser2_get_LayoutWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_LayoutHeight_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *piLayoutHeight);


void __RPC_STUB IBrowser2_get_LayoutHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_LocationBaseURL_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrBaseLocationURL);


void __RPC_STUB IBrowser2_get_LocationBaseURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser2_put_LocationBaseURL_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [in] */ BSTR bstrBaseLocationURL);


void __RPC_STUB IBrowser2_put_LocationBaseURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_ClearTypeEnabled_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbClearTypeEnabled);


void __RPC_STUB IBrowser2_get_ClearTypeEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser2_put_ClearTypeEnabled_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bClearTypeEnabled);


void __RPC_STUB IBrowser2_put_ClearTypeEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_ScriptingEnabled_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbScriptingEnabled);


void __RPC_STUB IBrowser2_get_ScriptingEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser2_put_ScriptingEnabled_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bScriptingEnabled);


void __RPC_STUB IBrowser2_put_ScriptingEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_FitToWindow_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbFitToWindow);


void __RPC_STUB IBrowser2_get_FitToWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser2_put_FitToWindow_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bFitToWindow);


void __RPC_STUB IBrowser2_put_FitToWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_ShowImages_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbShowImages);


void __RPC_STUB IBrowser2_get_ShowImages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser2_put_ShowImages_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bShowImages);


void __RPC_STUB IBrowser2_put_ShowImages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_PlaySounds_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbPlaySounds);


void __RPC_STUB IBrowser2_get_PlaySounds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser2_put_PlaySounds_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bPlaySounds);


void __RPC_STUB IBrowser2_put_PlaySounds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser2_get_ZoomLevel_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [retval][out] */ DWORD __RPC_FAR *pdwZoomLevel);


void __RPC_STUB IBrowser2_get_ZoomLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser2_put_ZoomLevel_Proxy( 
    IBrowser2 __RPC_FAR * This,
    /* [in] */ DWORD dwZoomLevel);


void __RPC_STUB IBrowser2_put_ZoomLevel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBrowser2_INTERFACE_DEFINED__ */


#ifndef __IBrowser3_INTERFACE_DEFINED__
#define __IBrowser3_INTERFACE_DEFINED__

/* interface IBrowser3 */
/* [unique][dual][uuid][object] */ 


EXTERN_C const IID IID_IBrowser3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("698E3FFE-70C3-11D0-81E8-00A0C90AD20A")
    IBrowser3 : public IBrowser2
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_document( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_script( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_IntegralPaging( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIntegralPaging) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_IntegralPaging( 
            /* [in] */ VARIANT_BOOL pvbIntegralPaging) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ContextMenuEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbContextMenu) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ContextMenuEnabled( 
            /* [in] */ VARIANT_BOOL pvbContextMenu) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ActiveXEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbActiveXEnabled) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ActiveXEnabled( 
            /* [in] */ VARIANT_BOOL vbActiveXEnabled) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SelectionEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbSelectionEnabled) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_SelectionEnabled( 
            /* [in] */ VARIANT_BOOL vbSelectionEnabled) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CanCopy( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbCanCopy) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FocusRectEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbFocusRectEnabled) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FocusRectEnabled( 
            /* [in] */ VARIANT_BOOL bFocusRectEnabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SuperFitToWindow( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuperFitToWindow) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SuperFitToWindow( 
            /* [in] */ VARIANT_BOOL bSuperFitToWindow) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_FourWayNav( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbFourWayNav) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_FourWayNav( 
            /* [in] */ VARIANT_BOOL bFourWayNav) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE addStyle( 
            /* [in] */ BSTR bstrStyle) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBrowser3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IBrowser3 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IBrowser3 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IBrowser3 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *navigate )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ long dwFlags,
            /* [in] */ BSTR bstrTargetFrameName,
            /* [in] */ BSTR bstrPostData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IBrowser3 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IBrowser3 __RPC_FAR * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Busy )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_title )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocationURL )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsFrame )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIsFrame);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IsFrame )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbIsFrame);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScrollStyle )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScrollStyle )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MarginWidth )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MarginWidth )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MarginHeight )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MarginHeight )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FrameNavigate )( 
            IBrowser3 __RPC_FAR * This,
            BSTR bstrURL,
            BSTR bstrTarget,
            BSTR bstrSource,
            BSTR bstrPostData,
            BOOL __RPC_FAR *Cancel);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelectedCodepage )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SelectedCodepage )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CBrowserPointer )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LayoutWidth )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piLayoutWidth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LayoutHeight )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *piLayoutHeight);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocationBaseURL )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrBaseLocationURL);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LocationBaseURL )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ BSTR bstrBaseLocationURL);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ClearTypeEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbClearTypeEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ClearTypeEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bClearTypeEnabled);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScriptingEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbScriptingEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScriptingEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bScriptingEnabled);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FitToWindow )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbFitToWindow);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FitToWindow )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bFitToWindow);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ShowImages )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbShowImages);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ShowImages )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bShowImages);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PlaySounds )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbPlaySounds);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PlaySounds )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bPlaySounds);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ZoomLevel )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwZoomLevel);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ZoomLevel )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ DWORD dwZoomLevel);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_script )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IntegralPaging )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIntegralPaging);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IntegralPaging )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL pvbIntegralPaging);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ContextMenuEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbContextMenu);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ContextMenuEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL pvbContextMenu);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ActiveXEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbActiveXEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ActiveXEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbActiveXEnabled);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelectionEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbSelectionEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SelectionEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbSelectionEnabled);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CanCopy )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbCanCopy);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FocusRectEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbFocusRectEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FocusRectEnabled )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bFocusRectEnabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SuperFitToWindow )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuperFitToWindow);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SuperFitToWindow )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bSuperFitToWindow);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FourWayNav )( 
            IBrowser3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbFourWayNav);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FourWayNav )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bFourWayNav);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addStyle )( 
            IBrowser3 __RPC_FAR * This,
            /* [in] */ BSTR bstrStyle);
        
        END_INTERFACE
    } IBrowser3Vtbl;

    interface IBrowser3
    {
        CONST_VTBL struct IBrowser3Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBrowser3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBrowser3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBrowser3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBrowser3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IBrowser3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IBrowser3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IBrowser3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IBrowser3_navigate(This,bstrURL,dwFlags,bstrTargetFrameName,bstrPostData)	\
    (This)->lpVtbl -> navigate(This,bstrURL,dwFlags,bstrTargetFrameName,bstrPostData)

#define IBrowser3_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define IBrowser3_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IBrowser3_get_Busy(This,pVal)	\
    (This)->lpVtbl -> get_Busy(This,pVal)

#define IBrowser3_get_title(This,pVal)	\
    (This)->lpVtbl -> get_title(This,pVal)

#define IBrowser3_get_LocationURL(This,pVal)	\
    (This)->lpVtbl -> get_LocationURL(This,pVal)

#define IBrowser3_get_IsFrame(This,pvbIsFrame)	\
    (This)->lpVtbl -> get_IsFrame(This,pvbIsFrame)

#define IBrowser3_put_IsFrame(This,vbIsFrame)	\
    (This)->lpVtbl -> put_IsFrame(This,vbIsFrame)

#define IBrowser3_get_ScrollStyle(This,pVal)	\
    (This)->lpVtbl -> get_ScrollStyle(This,pVal)

#define IBrowser3_put_ScrollStyle(This,newVal)	\
    (This)->lpVtbl -> put_ScrollStyle(This,newVal)

#define IBrowser3_get_MarginWidth(This,pVal)	\
    (This)->lpVtbl -> get_MarginWidth(This,pVal)

#define IBrowser3_put_MarginWidth(This,newVal)	\
    (This)->lpVtbl -> put_MarginWidth(This,newVal)

#define IBrowser3_get_MarginHeight(This,pVal)	\
    (This)->lpVtbl -> get_MarginHeight(This,pVal)

#define IBrowser3_put_MarginHeight(This,newVal)	\
    (This)->lpVtbl -> put_MarginHeight(This,newVal)

#define IBrowser3_FrameNavigate(This,bstrURL,bstrTarget,bstrSource,bstrPostData,Cancel)	\
    (This)->lpVtbl -> FrameNavigate(This,bstrURL,bstrTarget,bstrSource,bstrPostData,Cancel)

#define IBrowser3_get_SelectedCodepage(This,pVal)	\
    (This)->lpVtbl -> get_SelectedCodepage(This,pVal)

#define IBrowser3_put_SelectedCodepage(This,newVal)	\
    (This)->lpVtbl -> put_SelectedCodepage(This,newVal)

#define IBrowser3_get_CBrowserPointer(This,pVal)	\
    (This)->lpVtbl -> get_CBrowserPointer(This,pVal)


#define IBrowser3_get_LayoutWidth(This,piLayoutWidth)	\
    (This)->lpVtbl -> get_LayoutWidth(This,piLayoutWidth)

#define IBrowser3_get_LayoutHeight(This,piLayoutHeight)	\
    (This)->lpVtbl -> get_LayoutHeight(This,piLayoutHeight)

#define IBrowser3_get_LocationBaseURL(This,bstrBaseLocationURL)	\
    (This)->lpVtbl -> get_LocationBaseURL(This,bstrBaseLocationURL)

#define IBrowser3_put_LocationBaseURL(This,bstrBaseLocationURL)	\
    (This)->lpVtbl -> put_LocationBaseURL(This,bstrBaseLocationURL)

#define IBrowser3_get_ClearTypeEnabled(This,pbClearTypeEnabled)	\
    (This)->lpVtbl -> get_ClearTypeEnabled(This,pbClearTypeEnabled)

#define IBrowser3_put_ClearTypeEnabled(This,bClearTypeEnabled)	\
    (This)->lpVtbl -> put_ClearTypeEnabled(This,bClearTypeEnabled)

#define IBrowser3_get_ScriptingEnabled(This,pbScriptingEnabled)	\
    (This)->lpVtbl -> get_ScriptingEnabled(This,pbScriptingEnabled)

#define IBrowser3_put_ScriptingEnabled(This,bScriptingEnabled)	\
    (This)->lpVtbl -> put_ScriptingEnabled(This,bScriptingEnabled)

#define IBrowser3_get_FitToWindow(This,pbFitToWindow)	\
    (This)->lpVtbl -> get_FitToWindow(This,pbFitToWindow)

#define IBrowser3_put_FitToWindow(This,bFitToWindow)	\
    (This)->lpVtbl -> put_FitToWindow(This,bFitToWindow)

#define IBrowser3_get_ShowImages(This,pbShowImages)	\
    (This)->lpVtbl -> get_ShowImages(This,pbShowImages)

#define IBrowser3_put_ShowImages(This,bShowImages)	\
    (This)->lpVtbl -> put_ShowImages(This,bShowImages)

#define IBrowser3_get_PlaySounds(This,pbPlaySounds)	\
    (This)->lpVtbl -> get_PlaySounds(This,pbPlaySounds)

#define IBrowser3_put_PlaySounds(This,bPlaySounds)	\
    (This)->lpVtbl -> put_PlaySounds(This,bPlaySounds)

#define IBrowser3_get_ZoomLevel(This,pdwZoomLevel)	\
    (This)->lpVtbl -> get_ZoomLevel(This,pdwZoomLevel)

#define IBrowser3_put_ZoomLevel(This,dwZoomLevel)	\
    (This)->lpVtbl -> put_ZoomLevel(This,dwZoomLevel)


#define IBrowser3_get_document(This,ppDisp)	\
    (This)->lpVtbl -> get_document(This,ppDisp)

#define IBrowser3_get_script(This,ppDisp)	\
    (This)->lpVtbl -> get_script(This,ppDisp)

#define IBrowser3_get_IntegralPaging(This,pvbIntegralPaging)	\
    (This)->lpVtbl -> get_IntegralPaging(This,pvbIntegralPaging)

#define IBrowser3_put_IntegralPaging(This,pvbIntegralPaging)	\
    (This)->lpVtbl -> put_IntegralPaging(This,pvbIntegralPaging)

#define IBrowser3_get_ContextMenuEnabled(This,pvbContextMenu)	\
    (This)->lpVtbl -> get_ContextMenuEnabled(This,pvbContextMenu)

#define IBrowser3_put_ContextMenuEnabled(This,pvbContextMenu)	\
    (This)->lpVtbl -> put_ContextMenuEnabled(This,pvbContextMenu)

#define IBrowser3_get_ActiveXEnabled(This,pvbActiveXEnabled)	\
    (This)->lpVtbl -> get_ActiveXEnabled(This,pvbActiveXEnabled)

#define IBrowser3_put_ActiveXEnabled(This,vbActiveXEnabled)	\
    (This)->lpVtbl -> put_ActiveXEnabled(This,vbActiveXEnabled)

#define IBrowser3_get_SelectionEnabled(This,pvbSelectionEnabled)	\
    (This)->lpVtbl -> get_SelectionEnabled(This,pvbSelectionEnabled)

#define IBrowser3_put_SelectionEnabled(This,vbSelectionEnabled)	\
    (This)->lpVtbl -> put_SelectionEnabled(This,vbSelectionEnabled)

#define IBrowser3_get_CanCopy(This,pvbCanCopy)	\
    (This)->lpVtbl -> get_CanCopy(This,pvbCanCopy)

#define IBrowser3_get_FocusRectEnabled(This,pvbFocusRectEnabled)	\
    (This)->lpVtbl -> get_FocusRectEnabled(This,pvbFocusRectEnabled)

#define IBrowser3_put_FocusRectEnabled(This,bFocusRectEnabled)	\
    (This)->lpVtbl -> put_FocusRectEnabled(This,bFocusRectEnabled)

#define IBrowser3_get_SuperFitToWindow(This,pbSuperFitToWindow)	\
    (This)->lpVtbl -> get_SuperFitToWindow(This,pbSuperFitToWindow)

#define IBrowser3_put_SuperFitToWindow(This,bSuperFitToWindow)	\
    (This)->lpVtbl -> put_SuperFitToWindow(This,bSuperFitToWindow)

#define IBrowser3_get_FourWayNav(This,pbFourWayNav)	\
    (This)->lpVtbl -> get_FourWayNav(This,pbFourWayNav)

#define IBrowser3_put_FourWayNav(This,bFourWayNav)	\
    (This)->lpVtbl -> put_FourWayNav(This,bFourWayNav)

#define IBrowser3_addStyle(This,bstrStyle)	\
    (This)->lpVtbl -> addStyle(This,bstrStyle)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_document_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);


void __RPC_STUB IBrowser3_get_document_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_script_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp);


void __RPC_STUB IBrowser3_get_script_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_IntegralPaging_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIntegralPaging);


void __RPC_STUB IBrowser3_get_IntegralPaging_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser3_put_IntegralPaging_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL pvbIntegralPaging);


void __RPC_STUB IBrowser3_put_IntegralPaging_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_ContextMenuEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbContextMenu);


void __RPC_STUB IBrowser3_get_ContextMenuEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser3_put_ContextMenuEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL pvbContextMenu);


void __RPC_STUB IBrowser3_put_ContextMenuEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_ActiveXEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbActiveXEnabled);


void __RPC_STUB IBrowser3_get_ActiveXEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser3_put_ActiveXEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbActiveXEnabled);


void __RPC_STUB IBrowser3_put_ActiveXEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_SelectionEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbSelectionEnabled);


void __RPC_STUB IBrowser3_get_SelectionEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser3_put_SelectionEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbSelectionEnabled);


void __RPC_STUB IBrowser3_put_SelectionEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_CanCopy_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbCanCopy);


void __RPC_STUB IBrowser3_get_CanCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_FocusRectEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbFocusRectEnabled);


void __RPC_STUB IBrowser3_get_FocusRectEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IBrowser3_put_FocusRectEnabled_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bFocusRectEnabled);


void __RPC_STUB IBrowser3_put_FocusRectEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_SuperFitToWindow_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuperFitToWindow);


void __RPC_STUB IBrowser3_get_SuperFitToWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IBrowser3_put_SuperFitToWindow_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bSuperFitToWindow);


void __RPC_STUB IBrowser3_put_SuperFitToWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IBrowser3_get_FourWayNav_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbFourWayNav);


void __RPC_STUB IBrowser3_get_FourWayNav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IBrowser3_put_FourWayNav_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bFourWayNav);


void __RPC_STUB IBrowser3_put_FourWayNav_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IBrowser3_addStyle_Proxy( 
    IBrowser3 __RPC_FAR * This,
    /* [in] */ BSTR bstrStyle);


void __RPC_STUB IBrowser3_addStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBrowser3_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_webview_0314 */
/* [local] */ 

#define SID_SWebViewBrowser     IID_IBrowser

DEFINE_GUID(CGID_WebView,0x4b5d09, 0xfd9b, 0x4ce3, 0xa4, 0xdb, 0x2e, 0x96, 0x54, 0x34, 0xfc, 0xd7);
typedef 
enum WEBVWCMDID
    {	WEBVWCMDID_BATCHMODE	= 1,
	WEBVWCMDID_FASTMODE	= 2,
	WEBVWCMDID_ROUNDED_DPI_SCALING	= 3,
	WEBVWCMDID_GET_DOCUMENT	= 4,
	WEBVWCMDID_GET_FITTOWINDOW	= 5,
	WEBVWCMDID_PUT_FITTOWINDOW	= 6,
	WEBVWCMDID_GET_SUPERFITTOWINDOW	= 7,
	WEBVWCMDID_PUT_SUPERFITTOWINDOW	= 8,
	WEBVWCMDID_GET_ZOOMLEVEL	= 9,
	WEBVWCMDID_PUT_ZOOMLEVEL	= 10,
	WEBVWCMDID_GET_LAYOUTHEIGHT	= 11,
	WEBVWCMDID_GET_LAYOUTWIDTH	= 12,
	WEBVWCMDID_PUT_IFRAMEENABLED	= 13,
	WEBVWCMDID_PUT_ZOOMSCALES	= 14,
	WEBVWCMDID_GET_2WAYNAVMODE	= 15,
	WEBVWCMDID_PUT_2WAYNAVMODE	= 16,
	WEBVWCMDID_PUT_PUMPMESSAGESENABLED	= 17,
	WEBVWCMDID_PUT_SHOWIMAGES	= 18,
	WEBVWCMDID_GET_VIEWER_STATE	= 19,
	WEBVWCMDID_COPYSELECTIONTONEWISTREAM	= 20,
	WEBVWCMDID_ADDSTYLE	= 21,
	WEBVWCMDID_GET_LOCATIONBASEURL	= 22,
	WEBVWCMDID_ADDUSERSTYLE	= 23,
	WEBVWCMDID_PUT_SELECTEDCODEPAGE	= 24,
	WEBVWCMDID_GET_SELECTEDCODEPAGE	= 25,
	WEBVWCMDID_GET_DOCUMENTCODEPAGE	= 26,
	WEBVWCMDID_PUT_ANIMATIONENABLED	= 27,
	WEBVWCMDID_FITTOWINDOWVIEW	= 28,
	WEBVWCMDID_DESKTOPVIEW	= 29,
	WEBVWCMDID_PUT_IFRAMEDEPTH	= 30,
	WEBVWCMDID_PUT_ACTIVEXENABLED	= 31,
	WEBVWCMDID_SETSELECTIONMODE	= 32,
	WEBVWCMDID_SELECTALLSPECIAL	= 33,
	WEBVWCMDID_SELECTALLABOVEVIEW	= 34,
	WEBVWCMDID_SELECTALLINVIEWANDBELOW	= 35,
	WEBVWCMDID_GET_FOCUSEDELEMENTISTEXTCONTROL	= 36,
	WEBVWCMDID_FORM_CUT	= 37,
	WEBVWCMDID_FORM_COPY	= 38,
	WEBVWCMDID_FORM_PASTE	= 39,
	WEBVWCMDID_FORM_SELECTALL	= 40,
	WEBVWCMDID_FORM_GET_SELECTION	= 41,
	WEBVWCMDID_OPEN_CLIPBOARDAPP	= 42,
	WEBVWCMDID_FOCUSEDTEXTCONTROLISEMPTY	= 43,
	WEBVWCMDID_PAGEOVERVIEW_ZOOMOUT	= 44,
	WEBVWCMDID_PAGEOVERVIEW_ZOOMIN	= 45,
	WEBVWCMDID_PAGEOVERVIEW_CANCEL	= 46,
	WEBVWCMDID_HASTEXT	= 47,
	WEBVWCMDID_GET_FOCUSEDELEMENTISPASSWORDCONTROL	= 48
    }	WEBVWCMDID;

typedef 
enum VIEWCHANGE_RELAYOUT
    {	RELAYOUT_NORMAL	= 0,
	RELAYOUT_MINIMAL	= 1
    }	VIEWCHANGE_RELAYOUT;



extern RPC_IF_HANDLE __MIDL_itf_webview_0314_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webview_0314_v0_0_s_ifspec;

#ifndef __ILegacyHtml_INTERFACE_DEFINED__
#define __ILegacyHtml_INTERFACE_DEFINED__

/* interface ILegacyHtml */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_ILegacyHtml;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("698E3FCB-70C3-11D0-81E8-00A0C90AD20A")
    ILegacyHtml : public IUnknown
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_NotificationHwnd( 
            /* [in] */ HWND newVal) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Redraw( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddText( 
            /* [in] */ LPSTR szText,
            /* [in] */ BOOL bPlainText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddTextW( 
            /* [in] */ LPWSTR szText,
            /* [in] */ BOOL bPlainText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE JumpToAnchor( 
            /* [in] */ LPSTR szAnchor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE JumpToAnchorW( 
            /* [in] */ LPWSTR szAnchor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndOfSource( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ImageFail( 
            /* [in] */ DWORD dwCookie,
            /* [in] */ BOOL bForced) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetImage( 
            /* [in] */ DWORD pIII) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectAll( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopySelectionToNewIStream( 
            /* [retval][out] */ IStream __RPC_FAR *__RPC_FAR *ppStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendTextW( 
            /* [in] */ LPWSTR szText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetText( 
            /* [in] */ const void __RPC_FAR *pvText,
            /* [in] */ UINT cbText,
            /* [in] */ LPCSTR pszEncoding) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_PumpMessagesEnabled( 
            /* [in] */ VARIANT_BOOL vbPumpMessagesEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Dump( 
            LPCWSTR pszFileName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCollectionXML( 
            UINT uCollectionType,
            WCHAR __RPC_FAR *pwcBuf,
            UINT __RPC_FAR *pchBuf) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetViewportOrg( 
            POINT __RPC_FAR *pptViewportOrg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCollectionFocus( 
            UINT uCollectionType,
            UINT uIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetImageIStream( 
            /* [in] */ DWORD dwCookie,
            /* [in] */ IStream __RPC_FAR *pStream) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILegacyHtmlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILegacyHtml __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILegacyHtml __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_NotificationHwnd )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ HWND newVal);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Redraw )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddText )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ LPSTR szText,
            /* [in] */ BOOL bPlainText);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddTextW )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ LPWSTR szText,
            /* [in] */ BOOL bPlainText);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *JumpToAnchor )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ LPSTR szAnchor);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *JumpToAnchorW )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ LPWSTR szAnchor);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndOfSource )( 
            ILegacyHtml __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ImageFail )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ DWORD dwCookie,
            /* [in] */ BOOL bForced);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImage )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ DWORD pIII);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clear )( 
            ILegacyHtml __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectAll )( 
            ILegacyHtml __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopySelectionToNewIStream )( 
            ILegacyHtml __RPC_FAR * This,
            /* [retval][out] */ IStream __RPC_FAR *__RPC_FAR *ppStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendTextW )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ LPWSTR szText);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetText )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ const void __RPC_FAR *pvText,
            /* [in] */ UINT cbText,
            /* [in] */ LPCSTR pszEncoding);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PumpMessagesEnabled )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbPumpMessagesEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            ILegacyHtml __RPC_FAR * This,
            LPCWSTR pszFileName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCollectionXML )( 
            ILegacyHtml __RPC_FAR * This,
            UINT uCollectionType,
            WCHAR __RPC_FAR *pwcBuf,
            UINT __RPC_FAR *pchBuf);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetViewportOrg )( 
            ILegacyHtml __RPC_FAR * This,
            POINT __RPC_FAR *pptViewportOrg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCollectionFocus )( 
            ILegacyHtml __RPC_FAR * This,
            UINT uCollectionType,
            UINT uIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetImageIStream )( 
            ILegacyHtml __RPC_FAR * This,
            /* [in] */ DWORD dwCookie,
            /* [in] */ IStream __RPC_FAR *pStream);
        
        END_INTERFACE
    } ILegacyHtmlVtbl;

    interface ILegacyHtml
    {
        CONST_VTBL struct ILegacyHtmlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILegacyHtml_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILegacyHtml_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILegacyHtml_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILegacyHtml_put_NotificationHwnd(This,newVal)	\
    (This)->lpVtbl -> put_NotificationHwnd(This,newVal)

#define ILegacyHtml_put_Redraw(This,newVal)	\
    (This)->lpVtbl -> put_Redraw(This,newVal)

#define ILegacyHtml_AddText(This,szText,bPlainText)	\
    (This)->lpVtbl -> AddText(This,szText,bPlainText)

#define ILegacyHtml_AddTextW(This,szText,bPlainText)	\
    (This)->lpVtbl -> AddTextW(This,szText,bPlainText)

#define ILegacyHtml_JumpToAnchor(This,szAnchor)	\
    (This)->lpVtbl -> JumpToAnchor(This,szAnchor)

#define ILegacyHtml_JumpToAnchorW(This,szAnchor)	\
    (This)->lpVtbl -> JumpToAnchorW(This,szAnchor)

#define ILegacyHtml_EndOfSource(This)	\
    (This)->lpVtbl -> EndOfSource(This)

#define ILegacyHtml_ImageFail(This,dwCookie,bForced)	\
    (This)->lpVtbl -> ImageFail(This,dwCookie,bForced)

#define ILegacyHtml_SetImage(This,pIII)	\
    (This)->lpVtbl -> SetImage(This,pIII)

#define ILegacyHtml_Clear(This)	\
    (This)->lpVtbl -> Clear(This)

#define ILegacyHtml_SelectAll(This)	\
    (This)->lpVtbl -> SelectAll(This)

#define ILegacyHtml_CopySelectionToNewIStream(This,ppStream)	\
    (This)->lpVtbl -> CopySelectionToNewIStream(This,ppStream)

#define ILegacyHtml_AppendTextW(This,szText)	\
    (This)->lpVtbl -> AppendTextW(This,szText)

#define ILegacyHtml_SetText(This,pvText,cbText,pszEncoding)	\
    (This)->lpVtbl -> SetText(This,pvText,cbText,pszEncoding)

#define ILegacyHtml_put_PumpMessagesEnabled(This,vbPumpMessagesEnabled)	\
    (This)->lpVtbl -> put_PumpMessagesEnabled(This,vbPumpMessagesEnabled)

#define ILegacyHtml_Dump(This,pszFileName)	\
    (This)->lpVtbl -> Dump(This,pszFileName)

#define ILegacyHtml_GetCollectionXML(This,uCollectionType,pwcBuf,pchBuf)	\
    (This)->lpVtbl -> GetCollectionXML(This,uCollectionType,pwcBuf,pchBuf)

#define ILegacyHtml_GetViewportOrg(This,pptViewportOrg)	\
    (This)->lpVtbl -> GetViewportOrg(This,pptViewportOrg)

#define ILegacyHtml_SetCollectionFocus(This,uCollectionType,uIndex)	\
    (This)->lpVtbl -> SetCollectionFocus(This,uCollectionType,uIndex)

#define ILegacyHtml_SetImageIStream(This,dwCookie,pStream)	\
    (This)->lpVtbl -> SetImageIStream(This,dwCookie,pStream)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE ILegacyHtml_put_NotificationHwnd_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ HWND newVal);


void __RPC_STUB ILegacyHtml_put_NotificationHwnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ILegacyHtml_put_Redraw_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB ILegacyHtml_put_Redraw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_AddText_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ LPSTR szText,
    /* [in] */ BOOL bPlainText);


void __RPC_STUB ILegacyHtml_AddText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_AddTextW_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ LPWSTR szText,
    /* [in] */ BOOL bPlainText);


void __RPC_STUB ILegacyHtml_AddTextW_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_JumpToAnchor_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ LPSTR szAnchor);


void __RPC_STUB ILegacyHtml_JumpToAnchor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_JumpToAnchorW_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ LPWSTR szAnchor);


void __RPC_STUB ILegacyHtml_JumpToAnchorW_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_EndOfSource_Proxy( 
    ILegacyHtml __RPC_FAR * This);


void __RPC_STUB ILegacyHtml_EndOfSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_ImageFail_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ DWORD dwCookie,
    /* [in] */ BOOL bForced);


void __RPC_STUB ILegacyHtml_ImageFail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_SetImage_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ DWORD pIII);


void __RPC_STUB ILegacyHtml_SetImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_Clear_Proxy( 
    ILegacyHtml __RPC_FAR * This);


void __RPC_STUB ILegacyHtml_Clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_SelectAll_Proxy( 
    ILegacyHtml __RPC_FAR * This);


void __RPC_STUB ILegacyHtml_SelectAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_CopySelectionToNewIStream_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [retval][out] */ IStream __RPC_FAR *__RPC_FAR *ppStream);


void __RPC_STUB ILegacyHtml_CopySelectionToNewIStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_AppendTextW_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ LPWSTR szText);


void __RPC_STUB ILegacyHtml_AppendTextW_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_SetText_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ const void __RPC_FAR *pvText,
    /* [in] */ UINT cbText,
    /* [in] */ LPCSTR pszEncoding);


void __RPC_STUB ILegacyHtml_SetText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ILegacyHtml_put_PumpMessagesEnabled_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbPumpMessagesEnabled);


void __RPC_STUB ILegacyHtml_put_PumpMessagesEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_Dump_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    LPCWSTR pszFileName);


void __RPC_STUB ILegacyHtml_Dump_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_GetCollectionXML_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    UINT uCollectionType,
    WCHAR __RPC_FAR *pwcBuf,
    UINT __RPC_FAR *pchBuf);


void __RPC_STUB ILegacyHtml_GetCollectionXML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_GetViewportOrg_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    POINT __RPC_FAR *pptViewportOrg);


void __RPC_STUB ILegacyHtml_GetViewportOrg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_SetCollectionFocus_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    UINT uCollectionType,
    UINT uIndex);


void __RPC_STUB ILegacyHtml_SetCollectionFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILegacyHtml_SetImageIStream_Proxy( 
    ILegacyHtml __RPC_FAR * This,
    /* [in] */ DWORD dwCookie,
    /* [in] */ IStream __RPC_FAR *pStream);


void __RPC_STUB ILegacyHtml_SetImageIStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILegacyHtml_INTERFACE_DEFINED__ */


#ifndef __IHTMLHistoryEntry_INTERFACE_DEFINED__
#define __IHTMLHistoryEntry_INTERFACE_DEFINED__

/* interface IHTMLHistoryEntry */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IHTMLHistoryEntry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("698E3FCD-70C3-11D0-81E8-00A0C90AD20A")
    IHTMLHistoryEntry : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *ppCloned) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHTMLHistoryEntryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHTMLHistoryEntry __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHTMLHistoryEntry __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHTMLHistoryEntry __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IHTMLHistoryEntry __RPC_FAR * This,
            IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *ppCloned);
        
        END_INTERFACE
    } IHTMLHistoryEntryVtbl;

    interface IHTMLHistoryEntry
    {
        CONST_VTBL struct IHTMLHistoryEntryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHTMLHistoryEntry_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHTMLHistoryEntry_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHTMLHistoryEntry_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHTMLHistoryEntry_Clone(This,ppCloned)	\
    (This)->lpVtbl -> Clone(This,ppCloned)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IHTMLHistoryEntry_Clone_Proxy( 
    IHTMLHistoryEntry __RPC_FAR * This,
    IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *ppCloned);


void __RPC_STUB IHTMLHistoryEntry_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHTMLHistoryEntry_INTERFACE_DEFINED__ */


#ifndef __IHTMLBackDoor_INTERFACE_DEFINED__
#define __IHTMLBackDoor_INTERFACE_DEFINED__

/* interface IHTMLBackDoor */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IHTMLBackDoor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("698E3FCC-70C3-11D0-81E8-00A0C90AD20A")
    IHTMLBackDoor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Open( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddFavorite( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrTitle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Redirect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetHTMLHistoryData( 
            /* [in] */ IHTMLHistoryEntry __RPC_FAR *pHTMLHistory) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetHTMLHistoryData( 
            /* [retval][out] */ IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *pHTMLHistory) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPendingHTMLHistoryData( 
            /* [retval][out] */ IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *pHTMLHistory) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FakeNavigateComplete( 
            /* [in] */ BOOL bResetHistoryState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Back( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Forward( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_HistoryLength( 
            /* [retval][out] */ long __RPC_FAR *plHistoryLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHTMLBackDoorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHTMLBackDoor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHTMLBackDoor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHTMLBackDoor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IHTMLBackDoor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddFavorite )( 
            IHTMLBackDoor __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrTitle);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Redirect )( 
            IHTMLBackDoor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHTMLHistoryData )( 
            IHTMLBackDoor __RPC_FAR * This,
            /* [in] */ IHTMLHistoryEntry __RPC_FAR *pHTMLHistory);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHTMLHistoryData )( 
            IHTMLBackDoor __RPC_FAR * This,
            /* [retval][out] */ IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *pHTMLHistory);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPendingHTMLHistoryData )( 
            IHTMLBackDoor __RPC_FAR * This,
            /* [retval][out] */ IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *pHTMLHistory);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FakeNavigateComplete )( 
            IHTMLBackDoor __RPC_FAR * This,
            /* [in] */ BOOL bResetHistoryState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Back )( 
            IHTMLBackDoor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Forward )( 
            IHTMLBackDoor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IHTMLBackDoor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HistoryLength )( 
            IHTMLBackDoor __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plHistoryLength);
        
        END_INTERFACE
    } IHTMLBackDoorVtbl;

    interface IHTMLBackDoor
    {
        CONST_VTBL struct IHTMLBackDoorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHTMLBackDoor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHTMLBackDoor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHTMLBackDoor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHTMLBackDoor_Open(This)	\
    (This)->lpVtbl -> Open(This)

#define IHTMLBackDoor_AddFavorite(This,bstrURL,bstrTitle)	\
    (This)->lpVtbl -> AddFavorite(This,bstrURL,bstrTitle)

#define IHTMLBackDoor_Redirect(This)	\
    (This)->lpVtbl -> Redirect(This)

#define IHTMLBackDoor_SetHTMLHistoryData(This,pHTMLHistory)	\
    (This)->lpVtbl -> SetHTMLHistoryData(This,pHTMLHistory)

#define IHTMLBackDoor_GetHTMLHistoryData(This,pHTMLHistory)	\
    (This)->lpVtbl -> GetHTMLHistoryData(This,pHTMLHistory)

#define IHTMLBackDoor_GetPendingHTMLHistoryData(This,pHTMLHistory)	\
    (This)->lpVtbl -> GetPendingHTMLHistoryData(This,pHTMLHistory)

#define IHTMLBackDoor_FakeNavigateComplete(This,bResetHistoryState)	\
    (This)->lpVtbl -> FakeNavigateComplete(This,bResetHistoryState)

#define IHTMLBackDoor_Back(This)	\
    (This)->lpVtbl -> Back(This)

#define IHTMLBackDoor_Forward(This)	\
    (This)->lpVtbl -> Forward(This)

#define IHTMLBackDoor_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define IHTMLBackDoor_get_HistoryLength(This,plHistoryLength)	\
    (This)->lpVtbl -> get_HistoryLength(This,plHistoryLength)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IHTMLBackDoor_Open_Proxy( 
    IHTMLBackDoor __RPC_FAR * This);


void __RPC_STUB IHTMLBackDoor_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_AddFavorite_Proxy( 
    IHTMLBackDoor __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ BSTR bstrTitle);


void __RPC_STUB IHTMLBackDoor_AddFavorite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_Redirect_Proxy( 
    IHTMLBackDoor __RPC_FAR * This);


void __RPC_STUB IHTMLBackDoor_Redirect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_SetHTMLHistoryData_Proxy( 
    IHTMLBackDoor __RPC_FAR * This,
    /* [in] */ IHTMLHistoryEntry __RPC_FAR *pHTMLHistory);


void __RPC_STUB IHTMLBackDoor_SetHTMLHistoryData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_GetHTMLHistoryData_Proxy( 
    IHTMLBackDoor __RPC_FAR * This,
    /* [retval][out] */ IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *pHTMLHistory);


void __RPC_STUB IHTMLBackDoor_GetHTMLHistoryData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_GetPendingHTMLHistoryData_Proxy( 
    IHTMLBackDoor __RPC_FAR * This,
    /* [retval][out] */ IHTMLHistoryEntry __RPC_FAR *__RPC_FAR *pHTMLHistory);


void __RPC_STUB IHTMLBackDoor_GetPendingHTMLHistoryData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_FakeNavigateComplete_Proxy( 
    IHTMLBackDoor __RPC_FAR * This,
    /* [in] */ BOOL bResetHistoryState);


void __RPC_STUB IHTMLBackDoor_FakeNavigateComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_Back_Proxy( 
    IHTMLBackDoor __RPC_FAR * This);


void __RPC_STUB IHTMLBackDoor_Back_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_Forward_Proxy( 
    IHTMLBackDoor __RPC_FAR * This);


void __RPC_STUB IHTMLBackDoor_Forward_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_Refresh_Proxy( 
    IHTMLBackDoor __RPC_FAR * This);


void __RPC_STUB IHTMLBackDoor_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHTMLBackDoor_get_HistoryLength_Proxy( 
    IHTMLBackDoor __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plHistoryLength);


void __RPC_STUB IHTMLBackDoor_get_HistoryLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHTMLBackDoor_INTERFACE_DEFINED__ */


#ifndef __IViewerState_INTERFACE_DEFINED__
#define __IViewerState_INTERFACE_DEFINED__

/* interface IViewerState */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IViewerState;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E4CA7C27-6EC1-4137-8652-20665B668C4A")
    IViewerState : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnNavigateComplete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Load( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IViewerStateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IViewerState __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IViewerState __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IViewerState __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Save )( 
            IViewerState __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnNavigateComplete )( 
            IViewerState __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Load )( 
            IViewerState __RPC_FAR * This);
        
        END_INTERFACE
    } IViewerStateVtbl;

    interface IViewerState
    {
        CONST_VTBL struct IViewerStateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IViewerState_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IViewerState_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IViewerState_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IViewerState_Save(This)	\
    (This)->lpVtbl -> Save(This)

#define IViewerState_OnNavigateComplete(This)	\
    (This)->lpVtbl -> OnNavigateComplete(This)

#define IViewerState_Load(This)	\
    (This)->lpVtbl -> Load(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IViewerState_Save_Proxy( 
    IViewerState __RPC_FAR * This);


void __RPC_STUB IViewerState_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IViewerState_OnNavigateComplete_Proxy( 
    IViewerState __RPC_FAR * This);


void __RPC_STUB IViewerState_OnNavigateComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IViewerState_Load_Proxy( 
    IViewerState __RPC_FAR * This);


void __RPC_STUB IViewerState_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IViewerState_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_webview_0318 */
/* [local] */ 




enum _CREATE_FLAGS
    {	eCREATE_TOPLEVEL	= 0x1L,
	eCREATE_HIDESTATUSPROGRESS	= 0x2L,
	eCREATE_DEPENDENT	= 0x4L,
	eCREATE_FILEDOWNLOAD	= 0x8L
    };
typedef enum _CREATE_FLAGS CREATE_FLAGS;



extern RPC_IF_HANDLE __MIDL_itf_webview_0318_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webview_0318_v0_0_s_ifspec;

#ifndef __IRequestManager_INTERFACE_DEFINED__
#define __IRequestManager_INTERFACE_DEFINED__

/* interface IRequestManager */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IRequestManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3A4F8F5F-62CE-47e7-815F-4451D414695F")
    IRequestManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IInetConn __RPC_FAR *pConnMgr,
            /* [in] */ HWND hwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSettingChange( 
            /* [in] */ DWORD dwSettingInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StopRequests( 
            /* [in] */ DWORD dwID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateRequest( 
            /* [in] */ DWORD dwID,
            /* [in] */ IStream __RPC_FAR *pPostData,
            /* [in] */ LPCTSTR pszReferrerUrl,
            /* [in] */ IDownloadRequestSink __RPC_FAR *pSink,
            /* [in] */ DWORD dwFlags,
            /* [defaultvalue][in] */ HANDLE hDone = 0) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRequestManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRequestManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRequestManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRequestManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IRequestManager __RPC_FAR * This,
            /* [in] */ IInetConn __RPC_FAR *pConnMgr,
            /* [in] */ HWND hwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSettingChange )( 
            IRequestManager __RPC_FAR * This,
            /* [in] */ DWORD dwSettingInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopRequests )( 
            IRequestManager __RPC_FAR * This,
            /* [in] */ DWORD dwID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateRequest )( 
            IRequestManager __RPC_FAR * This,
            /* [in] */ DWORD dwID,
            /* [in] */ IStream __RPC_FAR *pPostData,
            /* [in] */ LPCTSTR pszReferrerUrl,
            /* [in] */ IDownloadRequestSink __RPC_FAR *pSink,
            /* [in] */ DWORD dwFlags,
            /* [defaultvalue][in] */ HANDLE hDone);
        
        END_INTERFACE
    } IRequestManagerVtbl;

    interface IRequestManager
    {
        CONST_VTBL struct IRequestManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRequestManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRequestManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRequestManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRequestManager_Initialize(This,pConnMgr,hwnd)	\
    (This)->lpVtbl -> Initialize(This,pConnMgr,hwnd)

#define IRequestManager_OnSettingChange(This,dwSettingInfo)	\
    (This)->lpVtbl -> OnSettingChange(This,dwSettingInfo)

#define IRequestManager_StopRequests(This,dwID)	\
    (This)->lpVtbl -> StopRequests(This,dwID)

#define IRequestManager_CreateRequest(This,dwID,pPostData,pszReferrerUrl,pSink,dwFlags,hDone)	\
    (This)->lpVtbl -> CreateRequest(This,dwID,pPostData,pszReferrerUrl,pSink,dwFlags,hDone)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRequestManager_Initialize_Proxy( 
    IRequestManager __RPC_FAR * This,
    /* [in] */ IInetConn __RPC_FAR *pConnMgr,
    /* [in] */ HWND hwnd);


void __RPC_STUB IRequestManager_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRequestManager_OnSettingChange_Proxy( 
    IRequestManager __RPC_FAR * This,
    /* [in] */ DWORD dwSettingInfo);


void __RPC_STUB IRequestManager_OnSettingChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRequestManager_StopRequests_Proxy( 
    IRequestManager __RPC_FAR * This,
    /* [in] */ DWORD dwID);


void __RPC_STUB IRequestManager_StopRequests_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRequestManager_CreateRequest_Proxy( 
    IRequestManager __RPC_FAR * This,
    /* [in] */ DWORD dwID,
    /* [in] */ IStream __RPC_FAR *pPostData,
    /* [in] */ LPCTSTR pszReferrerUrl,
    /* [in] */ IDownloadRequestSink __RPC_FAR *pSink,
    /* [in] */ DWORD dwFlags,
    /* [defaultvalue][in] */ HANDLE hDone);


void __RPC_STUB IRequestManager_CreateRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRequestManager_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_webview_0319 */
/* [local] */ 

EXTERN_C const CLSID CLSID_CRequestManager;

enum _REQUEST_STATE
    {	eSTATE_UNKNOWN	= 0,
	eSTATE_NOTSTARTED	= eSTATE_UNKNOWN + 1,
	eSTATE_STARTED	= eSTATE_NOTSTARTED + 1,
	eSTATE_STARTBINDING	= eSTATE_STARTED + 1,
	eSTATE_ONPROGRESS	= eSTATE_STARTBINDING + 1,
	eSTATE_READINGDATA	= eSTATE_ONPROGRESS + 1,
	eSTATE_ONSTOPBINDING	= eSTATE_READINGDATA + 1,
	eSTATE_NEEDSCOMPLETION	= eSTATE_ONSTOPBINDING + 1,
	eSTATE_COMPLETING	= eSTATE_NEEDSCOMPLETION + 1,
	eSTATE_COMPLETED	= eSTATE_COMPLETING + 1
    };
typedef enum _REQUEST_STATE REQUEST_STATE;



extern RPC_IF_HANDLE __MIDL_itf_webview_0319_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webview_0319_v0_0_s_ifspec;

#ifndef __IDownloadRequestProvider_INTERFACE_DEFINED__
#define __IDownloadRequestProvider_INTERFACE_DEFINED__

/* interface IDownloadRequestProvider */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IDownloadRequestProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B0DEB0F7-4971-4e9b-B519-3176CDB5233E")
    IDownloadRequestProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Abort( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ IDownloadRequestSink __RPC_FAR *pSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Detach( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsBindComplete( 
            /* [out] */ BOOL __RPC_FAR *pbCompleted) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsAborted( 
            /* [out] */ BOOL __RPC_FAR *pbAborted) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Complete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [out] */ REQUEST_STATE __RPC_FAR *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContentLength( 
            /* [out] */ DWORD __RPC_FAR *pdwcbContentLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContentType( 
            /* [out] */ LPWSTR __RPC_FAR *ppszContentType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContentEncoding( 
            /* [out] */ LPWSTR __RPC_FAR *ppszContentEncoding) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCacheFileName( 
            /* [out] */ LPWSTR __RPC_FAR *ppszCacheFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPostData( 
            /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppPostData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRedirectedUrl( 
            /* [out] */ LPWSTR __RPC_FAR *ppszRedirectedUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDataStream( 
            /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetReferrerUrl( 
            /* [out] */ LPWSTR __RPC_FAR *ppszReferrerUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetActualLength( 
            /* [out] */ DWORD __RPC_FAR *pdwcbActualLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDownloadRequestProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDownloadRequestProvider __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDownloadRequestProvider __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Abort )( 
            IDownloadRequestProvider __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Attach )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [in] */ IDownloadRequestSink __RPC_FAR *pSink);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Detach )( 
            IDownloadRequestProvider __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsBindComplete )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pbCompleted);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsAborted )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pbAborted);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Complete )( 
            IDownloadRequestProvider __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ REQUEST_STATE __RPC_FAR *pState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContentLength )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwcbContentLength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContentType )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ LPWSTR __RPC_FAR *ppszContentType);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContentEncoding )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ LPWSTR __RPC_FAR *ppszContentEncoding);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCacheFileName )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ LPWSTR __RPC_FAR *ppszCacheFile);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPostData )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppPostData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRedirectedUrl )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ LPWSTR __RPC_FAR *ppszRedirectedUrl);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDataStream )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetReferrerUrl )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ LPWSTR __RPC_FAR *ppszReferrerUrl);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetActualLength )( 
            IDownloadRequestProvider __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwcbActualLength);
        
        END_INTERFACE
    } IDownloadRequestProviderVtbl;

    interface IDownloadRequestProvider
    {
        CONST_VTBL struct IDownloadRequestProviderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDownloadRequestProvider_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDownloadRequestProvider_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDownloadRequestProvider_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDownloadRequestProvider_Abort(This)	\
    (This)->lpVtbl -> Abort(This)

#define IDownloadRequestProvider_Attach(This,pSink)	\
    (This)->lpVtbl -> Attach(This,pSink)

#define IDownloadRequestProvider_Detach(This)	\
    (This)->lpVtbl -> Detach(This)

#define IDownloadRequestProvider_IsBindComplete(This,pbCompleted)	\
    (This)->lpVtbl -> IsBindComplete(This,pbCompleted)

#define IDownloadRequestProvider_IsAborted(This,pbAborted)	\
    (This)->lpVtbl -> IsAborted(This,pbAborted)

#define IDownloadRequestProvider_Complete(This)	\
    (This)->lpVtbl -> Complete(This)

#define IDownloadRequestProvider_GetState(This,pState)	\
    (This)->lpVtbl -> GetState(This,pState)

#define IDownloadRequestProvider_GetContentLength(This,pdwcbContentLength)	\
    (This)->lpVtbl -> GetContentLength(This,pdwcbContentLength)

#define IDownloadRequestProvider_GetContentType(This,ppszContentType)	\
    (This)->lpVtbl -> GetContentType(This,ppszContentType)

#define IDownloadRequestProvider_GetContentEncoding(This,ppszContentEncoding)	\
    (This)->lpVtbl -> GetContentEncoding(This,ppszContentEncoding)

#define IDownloadRequestProvider_GetCacheFileName(This,ppszCacheFile)	\
    (This)->lpVtbl -> GetCacheFileName(This,ppszCacheFile)

#define IDownloadRequestProvider_GetPostData(This,ppPostData)	\
    (This)->lpVtbl -> GetPostData(This,ppPostData)

#define IDownloadRequestProvider_GetRedirectedUrl(This,ppszRedirectedUrl)	\
    (This)->lpVtbl -> GetRedirectedUrl(This,ppszRedirectedUrl)

#define IDownloadRequestProvider_GetDataStream(This,ppstm)	\
    (This)->lpVtbl -> GetDataStream(This,ppstm)

#define IDownloadRequestProvider_GetReferrerUrl(This,ppszReferrerUrl)	\
    (This)->lpVtbl -> GetReferrerUrl(This,ppszReferrerUrl)

#define IDownloadRequestProvider_GetActualLength(This,pdwcbActualLength)	\
    (This)->lpVtbl -> GetActualLength(This,pdwcbActualLength)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_Abort_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This);


void __RPC_STUB IDownloadRequestProvider_Abort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_Attach_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [in] */ IDownloadRequestSink __RPC_FAR *pSink);


void __RPC_STUB IDownloadRequestProvider_Attach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_Detach_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This);


void __RPC_STUB IDownloadRequestProvider_Detach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_IsBindComplete_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pbCompleted);


void __RPC_STUB IDownloadRequestProvider_IsBindComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_IsAborted_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pbAborted);


void __RPC_STUB IDownloadRequestProvider_IsAborted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_Complete_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This);


void __RPC_STUB IDownloadRequestProvider_Complete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetState_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ REQUEST_STATE __RPC_FAR *pState);


void __RPC_STUB IDownloadRequestProvider_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetContentLength_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwcbContentLength);


void __RPC_STUB IDownloadRequestProvider_GetContentLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetContentType_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ LPWSTR __RPC_FAR *ppszContentType);


void __RPC_STUB IDownloadRequestProvider_GetContentType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetContentEncoding_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ LPWSTR __RPC_FAR *ppszContentEncoding);


void __RPC_STUB IDownloadRequestProvider_GetContentEncoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetCacheFileName_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ LPWSTR __RPC_FAR *ppszCacheFile);


void __RPC_STUB IDownloadRequestProvider_GetCacheFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetPostData_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppPostData);


void __RPC_STUB IDownloadRequestProvider_GetPostData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetRedirectedUrl_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ LPWSTR __RPC_FAR *ppszRedirectedUrl);


void __RPC_STUB IDownloadRequestProvider_GetRedirectedUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetDataStream_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm);


void __RPC_STUB IDownloadRequestProvider_GetDataStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetReferrerUrl_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ LPWSTR __RPC_FAR *ppszReferrerUrl);


void __RPC_STUB IDownloadRequestProvider_GetReferrerUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestProvider_GetActualLength_Proxy( 
    IDownloadRequestProvider __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwcbActualLength);


void __RPC_STUB IDownloadRequestProvider_GetActualLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDownloadRequestProvider_INTERFACE_DEFINED__ */


#ifndef __IDownloadRequestSink_INTERFACE_DEFINED__
#define __IDownloadRequestSink_INTERFACE_DEFINED__

/* interface IDownloadRequestSink */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDownloadRequestSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8136A912-B1CA-4670-A390-205A2F8200E2")
    IDownloadRequestSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Start( 
            /* [in] */ IDownloadRequestProvider __RPC_FAR *pProvider,
            /* [in] */ DWORD __RPC_FAR *pBindFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUrl( 
            /* [in] */ LPCWSTR pszUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUrl( 
            LPWSTR __RPC_FAR *ppszUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAdditionalHeaders( 
            /* [in] */ LPCWSTR pwszCurrentHeaders,
            /* [out][in] */ LPWSTR __RPC_FAR *ppwszAdditionalHeaders) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnDataStart( 
            /* [in] */ IBinding __RPC_FAR *pBinding) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnData( 
            /* [in] */ IStream __RPC_FAR *pstm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Complete( 
            /* [in] */ HRESULT hrBindResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearRequestFlags( 
            /* [in] */ DWORD dwfFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnPolicyCheck( 
            /* [in] */ DWORD dwUrlAction) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDownloadRequestSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDownloadRequestSink __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDownloadRequestSink __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Start )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ IDownloadRequestProvider __RPC_FAR *pProvider,
            /* [in] */ DWORD __RPC_FAR *pBindFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUrl )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ LPCWSTR pszUrl);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUrl )( 
            IDownloadRequestSink __RPC_FAR * This,
            LPWSTR __RPC_FAR *ppszUrl);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAdditionalHeaders )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ LPCWSTR pwszCurrentHeaders,
            /* [out][in] */ LPWSTR __RPC_FAR *ppwszAdditionalHeaders);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDataStart )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ IBinding __RPC_FAR *pBinding);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnData )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ IStream __RPC_FAR *pstm);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Complete )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ HRESULT hrBindResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearRequestFlags )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ DWORD dwfFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPolicyCheck )( 
            IDownloadRequestSink __RPC_FAR * This,
            /* [in] */ DWORD dwUrlAction);
        
        END_INTERFACE
    } IDownloadRequestSinkVtbl;

    interface IDownloadRequestSink
    {
        CONST_VTBL struct IDownloadRequestSinkVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDownloadRequestSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDownloadRequestSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDownloadRequestSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDownloadRequestSink_Start(This,pProvider,pBindFlags)	\
    (This)->lpVtbl -> Start(This,pProvider,pBindFlags)

#define IDownloadRequestSink_SetUrl(This,pszUrl)	\
    (This)->lpVtbl -> SetUrl(This,pszUrl)

#define IDownloadRequestSink_GetUrl(This,ppszUrl)	\
    (This)->lpVtbl -> GetUrl(This,ppszUrl)

#define IDownloadRequestSink_SetAdditionalHeaders(This,pwszCurrentHeaders,ppwszAdditionalHeaders)	\
    (This)->lpVtbl -> SetAdditionalHeaders(This,pwszCurrentHeaders,ppwszAdditionalHeaders)

#define IDownloadRequestSink_OnDataStart(This,pBinding)	\
    (This)->lpVtbl -> OnDataStart(This,pBinding)

#define IDownloadRequestSink_OnData(This,pstm)	\
    (This)->lpVtbl -> OnData(This,pstm)

#define IDownloadRequestSink_Complete(This,hrBindResult)	\
    (This)->lpVtbl -> Complete(This,hrBindResult)

#define IDownloadRequestSink_ClearRequestFlags(This,dwfFlags)	\
    (This)->lpVtbl -> ClearRequestFlags(This,dwfFlags)

#define IDownloadRequestSink_OnPolicyCheck(This,dwUrlAction)	\
    (This)->lpVtbl -> OnPolicyCheck(This,dwUrlAction)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDownloadRequestSink_Start_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ IDownloadRequestProvider __RPC_FAR *pProvider,
    /* [in] */ DWORD __RPC_FAR *pBindFlags);


void __RPC_STUB IDownloadRequestSink_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_SetUrl_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ LPCWSTR pszUrl);


void __RPC_STUB IDownloadRequestSink_SetUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_GetUrl_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    LPWSTR __RPC_FAR *ppszUrl);


void __RPC_STUB IDownloadRequestSink_GetUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_SetAdditionalHeaders_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ LPCWSTR pwszCurrentHeaders,
    /* [out][in] */ LPWSTR __RPC_FAR *ppwszAdditionalHeaders);


void __RPC_STUB IDownloadRequestSink_SetAdditionalHeaders_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_OnDataStart_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ IBinding __RPC_FAR *pBinding);


void __RPC_STUB IDownloadRequestSink_OnDataStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_OnData_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ IStream __RPC_FAR *pstm);


void __RPC_STUB IDownloadRequestSink_OnData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_Complete_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ HRESULT hrBindResult);


void __RPC_STUB IDownloadRequestSink_Complete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_ClearRequestFlags_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ DWORD dwfFlags);


void __RPC_STUB IDownloadRequestSink_ClearRequestFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDownloadRequestSink_OnPolicyCheck_Proxy( 
    IDownloadRequestSink __RPC_FAR * This,
    /* [in] */ DWORD dwUrlAction);


void __RPC_STUB IDownloadRequestSink_OnPolicyCheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDownloadRequestSink_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_webview_0321 */
/* [local] */ 


enum _INET_STATUS
    {	INET_DISCONNECTED	= 0,
	INET_CONNECTED	= 0x10,
	INET_CANCONNECT	= 0x20
    };
typedef enum _INET_STATUS INET_STATUS;

struct _URL_INFO
    {
    LPTSTR m_pszUrl;
    LPSTR m_pszPostData;
    DWORD m_dwContentType;
    DWORD m_dwCookie;
    DWORD m_dwFlags;
    DWORD_PTR m_dwExtra;
    };
typedef struct _URL_INFO URL_INFO;



extern RPC_IF_HANDLE __MIDL_itf_webview_0321_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webview_0321_v0_0_s_ifspec;

#ifndef __IInetConn_INTERFACE_DEFINED__
#define __IInetConn_INTERFACE_DEFINED__

/* interface IInetConn */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IInetConn;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F459AD1E-98E5-42a4-91B9-793630A483A9")
    IInetConn : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [defaultvalue] */ HWND hwnd = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetConnectStatus( 
            LPCTSTR pszURL,
            INET_STATUS __RPC_FAR *pInetStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReadPathSettings( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StopConn( 
            HWND hwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StopConnByID( 
            DWORD dwID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EstablishConn( 
            HWND hwnd,
            BOOL bHwndIsThreadId,
            const URL_INFO __RPC_FAR *pUrlInfo,
            /* [defaultvalue] */ LPDWORD pdwID = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsPendingURLSet( 
            HWND hwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPendingURL( 
            DWORD dwID,
            URL_INFO __RPC_FAR *__RPC_FAR *ppUrlInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProxyConfig( 
            LPCTSTR pszUrl,
            BOOL bGetCredentials,
            BYTE __RPC_FAR *pbProxyConfig,
            ULONG __RPC_FAR *pcbProxyConfig) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInetConnVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IInetConn __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IInetConn __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IInetConn __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IInetConn __RPC_FAR * This,
            /* [defaultvalue] */ HWND hwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetConnectStatus )( 
            IInetConn __RPC_FAR * This,
            LPCTSTR pszURL,
            INET_STATUS __RPC_FAR *pInetStatus);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadPathSettings )( 
            IInetConn __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopConn )( 
            IInetConn __RPC_FAR * This,
            HWND hwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopConnByID )( 
            IInetConn __RPC_FAR * This,
            DWORD dwID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EstablishConn )( 
            IInetConn __RPC_FAR * This,
            HWND hwnd,
            BOOL bHwndIsThreadId,
            const URL_INFO __RPC_FAR *pUrlInfo,
            /* [defaultvalue] */ LPDWORD pdwID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPendingURLSet )( 
            IInetConn __RPC_FAR * This,
            HWND hwnd);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPendingURL )( 
            IInetConn __RPC_FAR * This,
            DWORD dwID,
            URL_INFO __RPC_FAR *__RPC_FAR *ppUrlInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProxyConfig )( 
            IInetConn __RPC_FAR * This,
            LPCTSTR pszUrl,
            BOOL bGetCredentials,
            BYTE __RPC_FAR *pbProxyConfig,
            ULONG __RPC_FAR *pcbProxyConfig);
        
        END_INTERFACE
    } IInetConnVtbl;

    interface IInetConn
    {
        CONST_VTBL struct IInetConnVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInetConn_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInetConn_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInetConn_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInetConn_Init(This,hwnd)	\
    (This)->lpVtbl -> Init(This,hwnd)

#define IInetConn_GetConnectStatus(This,pszURL,pInetStatus)	\
    (This)->lpVtbl -> GetConnectStatus(This,pszURL,pInetStatus)

#define IInetConn_ReadPathSettings(This)	\
    (This)->lpVtbl -> ReadPathSettings(This)

#define IInetConn_StopConn(This,hwnd)	\
    (This)->lpVtbl -> StopConn(This,hwnd)

#define IInetConn_StopConnByID(This,dwID)	\
    (This)->lpVtbl -> StopConnByID(This,dwID)

#define IInetConn_EstablishConn(This,hwnd,bHwndIsThreadId,pUrlInfo,pdwID)	\
    (This)->lpVtbl -> EstablishConn(This,hwnd,bHwndIsThreadId,pUrlInfo,pdwID)

#define IInetConn_IsPendingURLSet(This,hwnd)	\
    (This)->lpVtbl -> IsPendingURLSet(This,hwnd)

#define IInetConn_GetPendingURL(This,dwID,ppUrlInfo)	\
    (This)->lpVtbl -> GetPendingURL(This,dwID,ppUrlInfo)

#define IInetConn_GetProxyConfig(This,pszUrl,bGetCredentials,pbProxyConfig,pcbProxyConfig)	\
    (This)->lpVtbl -> GetProxyConfig(This,pszUrl,bGetCredentials,pbProxyConfig,pcbProxyConfig)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IInetConn_Init_Proxy( 
    IInetConn __RPC_FAR * This,
    /* [defaultvalue] */ HWND hwnd);


void __RPC_STUB IInetConn_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_GetConnectStatus_Proxy( 
    IInetConn __RPC_FAR * This,
    LPCTSTR pszURL,
    INET_STATUS __RPC_FAR *pInetStatus);


void __RPC_STUB IInetConn_GetConnectStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_ReadPathSettings_Proxy( 
    IInetConn __RPC_FAR * This);


void __RPC_STUB IInetConn_ReadPathSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_StopConn_Proxy( 
    IInetConn __RPC_FAR * This,
    HWND hwnd);


void __RPC_STUB IInetConn_StopConn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_StopConnByID_Proxy( 
    IInetConn __RPC_FAR * This,
    DWORD dwID);


void __RPC_STUB IInetConn_StopConnByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_EstablishConn_Proxy( 
    IInetConn __RPC_FAR * This,
    HWND hwnd,
    BOOL bHwndIsThreadId,
    const URL_INFO __RPC_FAR *pUrlInfo,
    /* [defaultvalue] */ LPDWORD pdwID);


void __RPC_STUB IInetConn_EstablishConn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_IsPendingURLSet_Proxy( 
    IInetConn __RPC_FAR * This,
    HWND hwnd);


void __RPC_STUB IInetConn_IsPendingURLSet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_GetPendingURL_Proxy( 
    IInetConn __RPC_FAR * This,
    DWORD dwID,
    URL_INFO __RPC_FAR *__RPC_FAR *ppUrlInfo);


void __RPC_STUB IInetConn_GetPendingURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IInetConn_GetProxyConfig_Proxy( 
    IInetConn __RPC_FAR * This,
    LPCTSTR pszUrl,
    BOOL bGetCredentials,
    BYTE __RPC_FAR *pbProxyConfig,
    ULONG __RPC_FAR *pcbProxyConfig);


void __RPC_STUB IInetConn_GetProxyConfig_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInetConn_INTERFACE_DEFINED__ */


#ifndef __IPIEViewerHost_INTERFACE_DEFINED__
#define __IPIEViewerHost_INTERFACE_DEFINED__

/* interface IPIEViewerHost */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IPIEViewerHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D1F845F1-B819-499e-9E7A-47F5975D6E45")
    IPIEViewerHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE HandleUnknownProtocol( 
            /* [in] */ HWND hwnd,
            /* [in] */ LPCWSTR pwszUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DismissModalDialogs( 
            /* [out][in] */ void __RPC_FAR *__RPC_FAR *ppCookie) = 0;
        
        virtual int STDMETHODCALLTYPE ShowErrorMessage( 
            LPCWSTR pwszError,
            UINT uMBFlags,
            /* [defaultvalue] */ HRESULT hr = 0) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEViewerHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEViewerHost __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEViewerHost __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEViewerHost __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleUnknownProtocol )( 
            IPIEViewerHost __RPC_FAR * This,
            /* [in] */ HWND hwnd,
            /* [in] */ LPCWSTR pwszUrl);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DismissModalDialogs )( 
            IPIEViewerHost __RPC_FAR * This,
            /* [out][in] */ void __RPC_FAR *__RPC_FAR *ppCookie);
        
        int ( STDMETHODCALLTYPE __RPC_FAR *ShowErrorMessage )( 
            IPIEViewerHost __RPC_FAR * This,
            LPCWSTR pwszError,
            UINT uMBFlags,
            /* [defaultvalue] */ HRESULT hr);
        
        END_INTERFACE
    } IPIEViewerHostVtbl;

    interface IPIEViewerHost
    {
        CONST_VTBL struct IPIEViewerHostVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEViewerHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEViewerHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEViewerHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEViewerHost_HandleUnknownProtocol(This,hwnd,pwszUrl)	\
    (This)->lpVtbl -> HandleUnknownProtocol(This,hwnd,pwszUrl)

#define IPIEViewerHost_DismissModalDialogs(This,ppCookie)	\
    (This)->lpVtbl -> DismissModalDialogs(This,ppCookie)

#define IPIEViewerHost_ShowErrorMessage(This,pwszError,uMBFlags,hr)	\
    (This)->lpVtbl -> ShowErrorMessage(This,pwszError,uMBFlags,hr)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPIEViewerHost_HandleUnknownProtocol_Proxy( 
    IPIEViewerHost __RPC_FAR * This,
    /* [in] */ HWND hwnd,
    /* [in] */ LPCWSTR pwszUrl);


void __RPC_STUB IPIEViewerHost_HandleUnknownProtocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEViewerHost_DismissModalDialogs_Proxy( 
    IPIEViewerHost __RPC_FAR * This,
    /* [out][in] */ void __RPC_FAR *__RPC_FAR *ppCookie);


void __RPC_STUB IPIEViewerHost_DismissModalDialogs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


int STDMETHODCALLTYPE IPIEViewerHost_ShowErrorMessage_Proxy( 
    IPIEViewerHost __RPC_FAR * This,
    LPCWSTR pwszError,
    UINT uMBFlags,
    /* [defaultvalue] */ HRESULT hr);


void __RPC_STUB IPIEViewerHost_ShowErrorMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEViewerHost_INTERFACE_DEFINED__ */


#ifndef __IPIEElement_INTERFACE_DEFINED__
#define __IPIEElement_INTERFACE_DEFINED__

/* interface IPIEElement */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IPIEElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5222498-8AEB-4F72-B378-7CDE845EF881")
    IPIEElement : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IPIEElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEElement __RPC_FAR * This);
        
        END_INTERFACE
    } IPIEElementVtbl;

    interface IPIEElement
    {
        CONST_VTBL struct IPIEElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPIEElement_INTERFACE_DEFINED__ */


#ifndef __IFrameHandler_INTERFACE_DEFINED__
#define __IFrameHandler_INTERFACE_DEFINED__

/* interface IFrameHandler */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IFrameHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("18FB9B22-3CF3-4853-A790-697B8AF1DA77")
    IFrameHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetParentBrowser( 
            void __RPC_FAR *pParentBrowser) = 0;
        
        virtual void __RPC_FAR *STDMETHODCALLTYPE GetParentBrowser( void) = 0;
        
        virtual void __RPC_FAR *STDMETHODCALLTYPE GetTopBrowser( void) = 0;
        
        virtual void __RPC_FAR *STDMETHODCALLTYPE GetFramesetElement( void) = 0;
        
        virtual void __RPC_FAR *STDMETHODCALLTYPE GetFrame( 
            void __RPC_FAR *pFrameBrowser) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsBusy( void) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsFrameset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Refresh( 
            void __RPC_FAR *pBrowser,
            UINT uFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( 
            void __RPC_FAR *pBrowser,
            BOOL bFlagsOnly) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessFrameset( 
            void __RPC_FAR *pRootElem) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ResizeFrameset( 
            LPCRECT prcWindow) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetElementSize( 
            void __RPC_FAR *pElement) = 0;
        
        virtual void STDMETHODCALLTYPE FrameLoaded( void) = 0;
        
        virtual void STDMETHODCALLTYPE FrameDestroyed( 
            void __RPC_FAR *pBrowser) = 0;
        
        virtual BOOL STDMETHODCALLTYPE SizeFrames( 
            void __RPC_FAR *pHTMLControlBase,
            int xPos,
            int yPos) = 0;
        
        virtual void STDMETHODCALLTYPE NotifyFrames( 
            DWORD dwSetting,
            DWORD dwValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FrameNavigate( 
            void __RPC_FAR *pSourceBrowser,
            LPWSTR pwszUrl,
            LPCWSTR pwszTarget,
            IStream __RPC_FAR *pPostData,
            LPCWSTR pwszHeaders,
            long lFlags,
            BOOL __RPC_FAR *pbHandled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NavigateToHistoryEntry( 
            void __RPC_FAR *pBrowser) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HistoryNavigate( 
            void __RPC_FAR *pBrowser,
            IHTMLBackDoor __RPC_FAR *pBackDoor,
            UINT uFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeforeNavigate( 
            void __RPC_FAR *pBrowser,
            BSTR bstrUrl,
            IStream __RPC_FAR *pPostData,
            BSTR bstrHeaders,
            UINT uFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NavigateComplete( 
            void __RPC_FAR *pBrowser,
            BSTR bstrUrl) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DocumentComplete( 
            void __RPC_FAR *pBrowser,
            HRESULT hrBindResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMarginWidth( 
            long lWidth) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMarginWidth( 
            long __RPC_FAR *plWidth) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMarginHeight( 
            long lHeight) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMarginHeight( 
            long __RPC_FAR *plHeight) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetNextFocus( 
            DWORD dwUpDown,
            DWORD dwFrontBack) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoCrossFrameNavCheck( 
            void __RPC_FAR *pTgtDoc,
            void __RPC_FAR *pSrcDoc,
            void __RPC_FAR *pSrcBrowser) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PerformCrossFramePolicyCheck( 
            LPCWSTR pwszTargetFrameDomain,
            LPCWSTR pwszSourceFrameDomain,
            LPCWSTR pwszSourceFrameUrl,
            void __RPC_FAR *pSrcBrowser) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFrameHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFrameHandler __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFrameHandler __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFrameHandler __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetParentBrowser )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pParentBrowser);
        
        void __RPC_FAR *( STDMETHODCALLTYPE __RPC_FAR *GetParentBrowser )( 
            IFrameHandler __RPC_FAR * This);
        
        void __RPC_FAR *( STDMETHODCALLTYPE __RPC_FAR *GetTopBrowser )( 
            IFrameHandler __RPC_FAR * This);
        
        void __RPC_FAR *( STDMETHODCALLTYPE __RPC_FAR *GetFramesetElement )( 
            IFrameHandler __RPC_FAR * This);
        
        void __RPC_FAR *( STDMETHODCALLTYPE __RPC_FAR *GetFrame )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pFrameBrowser);
        
        BOOL ( STDMETHODCALLTYPE __RPC_FAR *IsBusy )( 
            IFrameHandler __RPC_FAR * This);
        
        BOOL ( STDMETHODCALLTYPE __RPC_FAR *IsFrameset )( 
            IFrameHandler __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IFrameHandler __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser,
            UINT uFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reset )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser,
            BOOL bFlagsOnly);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessFrameset )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pRootElem);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResizeFrameset )( 
            IFrameHandler __RPC_FAR * This,
            LPCRECT prcWindow);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetElementSize )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pElement);
        
        void ( STDMETHODCALLTYPE __RPC_FAR *FrameLoaded )( 
            IFrameHandler __RPC_FAR * This);
        
        void ( STDMETHODCALLTYPE __RPC_FAR *FrameDestroyed )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser);
        
        BOOL ( STDMETHODCALLTYPE __RPC_FAR *SizeFrames )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pHTMLControlBase,
            int xPos,
            int yPos);
        
        void ( STDMETHODCALLTYPE __RPC_FAR *NotifyFrames )( 
            IFrameHandler __RPC_FAR * This,
            DWORD dwSetting,
            DWORD dwValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FrameNavigate )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pSourceBrowser,
            LPWSTR pwszUrl,
            LPCWSTR pwszTarget,
            IStream __RPC_FAR *pPostData,
            LPCWSTR pwszHeaders,
            long lFlags,
            BOOL __RPC_FAR *pbHandled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NavigateToHistoryEntry )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HistoryNavigate )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser,
            IHTMLBackDoor __RPC_FAR *pBackDoor,
            UINT uFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeforeNavigate )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser,
            BSTR bstrUrl,
            IStream __RPC_FAR *pPostData,
            BSTR bstrHeaders,
            UINT uFlags);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NavigateComplete )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser,
            BSTR bstrUrl);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DocumentComplete )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pBrowser,
            HRESULT hrBindResult);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMarginWidth )( 
            IFrameHandler __RPC_FAR * This,
            long lWidth);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMarginWidth )( 
            IFrameHandler __RPC_FAR * This,
            long __RPC_FAR *plWidth);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMarginHeight )( 
            IFrameHandler __RPC_FAR * This,
            long lHeight);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMarginHeight )( 
            IFrameHandler __RPC_FAR * This,
            long __RPC_FAR *plHeight);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetNextFocus )( 
            IFrameHandler __RPC_FAR * This,
            DWORD dwUpDown,
            DWORD dwFrontBack);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoCrossFrameNavCheck )( 
            IFrameHandler __RPC_FAR * This,
            void __RPC_FAR *pTgtDoc,
            void __RPC_FAR *pSrcDoc,
            void __RPC_FAR *pSrcBrowser);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PerformCrossFramePolicyCheck )( 
            IFrameHandler __RPC_FAR * This,
            LPCWSTR pwszTargetFrameDomain,
            LPCWSTR pwszSourceFrameDomain,
            LPCWSTR pwszSourceFrameUrl,
            void __RPC_FAR *pSrcBrowser);
        
        END_INTERFACE
    } IFrameHandlerVtbl;

    interface IFrameHandler
    {
        CONST_VTBL struct IFrameHandlerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFrameHandler_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFrameHandler_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFrameHandler_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFrameHandler_SetParentBrowser(This,pParentBrowser)	\
    (This)->lpVtbl -> SetParentBrowser(This,pParentBrowser)

#define IFrameHandler_GetParentBrowser(This)	\
    (This)->lpVtbl -> GetParentBrowser(This)

#define IFrameHandler_GetTopBrowser(This)	\
    (This)->lpVtbl -> GetTopBrowser(This)

#define IFrameHandler_GetFramesetElement(This)	\
    (This)->lpVtbl -> GetFramesetElement(This)

#define IFrameHandler_GetFrame(This,pFrameBrowser)	\
    (This)->lpVtbl -> GetFrame(This,pFrameBrowser)

#define IFrameHandler_IsBusy(This)	\
    (This)->lpVtbl -> IsBusy(This)

#define IFrameHandler_IsFrameset(This)	\
    (This)->lpVtbl -> IsFrameset(This)

#define IFrameHandler_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IFrameHandler_Refresh(This,pBrowser,uFlags)	\
    (This)->lpVtbl -> Refresh(This,pBrowser,uFlags)

#define IFrameHandler_Reset(This,pBrowser,bFlagsOnly)	\
    (This)->lpVtbl -> Reset(This,pBrowser,bFlagsOnly)

#define IFrameHandler_ProcessFrameset(This,pRootElem)	\
    (This)->lpVtbl -> ProcessFrameset(This,pRootElem)

#define IFrameHandler_ResizeFrameset(This,prcWindow)	\
    (This)->lpVtbl -> ResizeFrameset(This,prcWindow)

#define IFrameHandler_SetElementSize(This,pElement)	\
    (This)->lpVtbl -> SetElementSize(This,pElement)

#define IFrameHandler_FrameLoaded(This)	\
    (This)->lpVtbl -> FrameLoaded(This)

#define IFrameHandler_FrameDestroyed(This,pBrowser)	\
    (This)->lpVtbl -> FrameDestroyed(This,pBrowser)

#define IFrameHandler_SizeFrames(This,pHTMLControlBase,xPos,yPos)	\
    (This)->lpVtbl -> SizeFrames(This,pHTMLControlBase,xPos,yPos)

#define IFrameHandler_NotifyFrames(This,dwSetting,dwValue)	\
    (This)->lpVtbl -> NotifyFrames(This,dwSetting,dwValue)

#define IFrameHandler_FrameNavigate(This,pSourceBrowser,pwszUrl,pwszTarget,pPostData,pwszHeaders,lFlags,pbHandled)	\
    (This)->lpVtbl -> FrameNavigate(This,pSourceBrowser,pwszUrl,pwszTarget,pPostData,pwszHeaders,lFlags,pbHandled)

#define IFrameHandler_NavigateToHistoryEntry(This,pBrowser)	\
    (This)->lpVtbl -> NavigateToHistoryEntry(This,pBrowser)

#define IFrameHandler_HistoryNavigate(This,pBrowser,pBackDoor,uFlags)	\
    (This)->lpVtbl -> HistoryNavigate(This,pBrowser,pBackDoor,uFlags)

#define IFrameHandler_BeforeNavigate(This,pBrowser,bstrUrl,pPostData,bstrHeaders,uFlags)	\
    (This)->lpVtbl -> BeforeNavigate(This,pBrowser,bstrUrl,pPostData,bstrHeaders,uFlags)

#define IFrameHandler_NavigateComplete(This,pBrowser,bstrUrl)	\
    (This)->lpVtbl -> NavigateComplete(This,pBrowser,bstrUrl)

#define IFrameHandler_DocumentComplete(This,pBrowser,hrBindResult)	\
    (This)->lpVtbl -> DocumentComplete(This,pBrowser,hrBindResult)

#define IFrameHandler_SetMarginWidth(This,lWidth)	\
    (This)->lpVtbl -> SetMarginWidth(This,lWidth)

#define IFrameHandler_GetMarginWidth(This,plWidth)	\
    (This)->lpVtbl -> GetMarginWidth(This,plWidth)

#define IFrameHandler_SetMarginHeight(This,lHeight)	\
    (This)->lpVtbl -> SetMarginHeight(This,lHeight)

#define IFrameHandler_GetMarginHeight(This,plHeight)	\
    (This)->lpVtbl -> GetMarginHeight(This,plHeight)

#define IFrameHandler_SetNextFocus(This,dwUpDown,dwFrontBack)	\
    (This)->lpVtbl -> SetNextFocus(This,dwUpDown,dwFrontBack)

#define IFrameHandler_DoCrossFrameNavCheck(This,pTgtDoc,pSrcDoc,pSrcBrowser)	\
    (This)->lpVtbl -> DoCrossFrameNavCheck(This,pTgtDoc,pSrcDoc,pSrcBrowser)

#define IFrameHandler_PerformCrossFramePolicyCheck(This,pwszTargetFrameDomain,pwszSourceFrameDomain,pwszSourceFrameUrl,pSrcBrowser)	\
    (This)->lpVtbl -> PerformCrossFramePolicyCheck(This,pwszTargetFrameDomain,pwszSourceFrameDomain,pwszSourceFrameUrl,pSrcBrowser)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IFrameHandler_SetParentBrowser_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pParentBrowser);


void __RPC_STUB IFrameHandler_SetParentBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void __RPC_FAR *STDMETHODCALLTYPE IFrameHandler_GetParentBrowser_Proxy( 
    IFrameHandler __RPC_FAR * This);


void __RPC_STUB IFrameHandler_GetParentBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void __RPC_FAR *STDMETHODCALLTYPE IFrameHandler_GetTopBrowser_Proxy( 
    IFrameHandler __RPC_FAR * This);


void __RPC_STUB IFrameHandler_GetTopBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void __RPC_FAR *STDMETHODCALLTYPE IFrameHandler_GetFramesetElement_Proxy( 
    IFrameHandler __RPC_FAR * This);


void __RPC_STUB IFrameHandler_GetFramesetElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void __RPC_FAR *STDMETHODCALLTYPE IFrameHandler_GetFrame_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pFrameBrowser);


void __RPC_STUB IFrameHandler_GetFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


BOOL STDMETHODCALLTYPE IFrameHandler_IsBusy_Proxy( 
    IFrameHandler __RPC_FAR * This);


void __RPC_STUB IFrameHandler_IsBusy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


BOOL STDMETHODCALLTYPE IFrameHandler_IsFrameset_Proxy( 
    IFrameHandler __RPC_FAR * This);


void __RPC_STUB IFrameHandler_IsFrameset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_Stop_Proxy( 
    IFrameHandler __RPC_FAR * This);


void __RPC_STUB IFrameHandler_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_Refresh_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser,
    UINT uFlags);


void __RPC_STUB IFrameHandler_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_Reset_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser,
    BOOL bFlagsOnly);


void __RPC_STUB IFrameHandler_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_ProcessFrameset_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pRootElem);


void __RPC_STUB IFrameHandler_ProcessFrameset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_ResizeFrameset_Proxy( 
    IFrameHandler __RPC_FAR * This,
    LPCRECT prcWindow);


void __RPC_STUB IFrameHandler_ResizeFrameset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_SetElementSize_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pElement);


void __RPC_STUB IFrameHandler_SetElementSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void STDMETHODCALLTYPE IFrameHandler_FrameLoaded_Proxy( 
    IFrameHandler __RPC_FAR * This);


void __RPC_STUB IFrameHandler_FrameLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void STDMETHODCALLTYPE IFrameHandler_FrameDestroyed_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser);


void __RPC_STUB IFrameHandler_FrameDestroyed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


BOOL STDMETHODCALLTYPE IFrameHandler_SizeFrames_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pHTMLControlBase,
    int xPos,
    int yPos);


void __RPC_STUB IFrameHandler_SizeFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void STDMETHODCALLTYPE IFrameHandler_NotifyFrames_Proxy( 
    IFrameHandler __RPC_FAR * This,
    DWORD dwSetting,
    DWORD dwValue);


void __RPC_STUB IFrameHandler_NotifyFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_FrameNavigate_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pSourceBrowser,
    LPWSTR pwszUrl,
    LPCWSTR pwszTarget,
    IStream __RPC_FAR *pPostData,
    LPCWSTR pwszHeaders,
    long lFlags,
    BOOL __RPC_FAR *pbHandled);


void __RPC_STUB IFrameHandler_FrameNavigate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_NavigateToHistoryEntry_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser);


void __RPC_STUB IFrameHandler_NavigateToHistoryEntry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_HistoryNavigate_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser,
    IHTMLBackDoor __RPC_FAR *pBackDoor,
    UINT uFlags);


void __RPC_STUB IFrameHandler_HistoryNavigate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_BeforeNavigate_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser,
    BSTR bstrUrl,
    IStream __RPC_FAR *pPostData,
    BSTR bstrHeaders,
    UINT uFlags);


void __RPC_STUB IFrameHandler_BeforeNavigate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_NavigateComplete_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser,
    BSTR bstrUrl);


void __RPC_STUB IFrameHandler_NavigateComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_DocumentComplete_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pBrowser,
    HRESULT hrBindResult);


void __RPC_STUB IFrameHandler_DocumentComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_SetMarginWidth_Proxy( 
    IFrameHandler __RPC_FAR * This,
    long lWidth);


void __RPC_STUB IFrameHandler_SetMarginWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_GetMarginWidth_Proxy( 
    IFrameHandler __RPC_FAR * This,
    long __RPC_FAR *plWidth);


void __RPC_STUB IFrameHandler_GetMarginWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_SetMarginHeight_Proxy( 
    IFrameHandler __RPC_FAR * This,
    long lHeight);


void __RPC_STUB IFrameHandler_SetMarginHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_GetMarginHeight_Proxy( 
    IFrameHandler __RPC_FAR * This,
    long __RPC_FAR *plHeight);


void __RPC_STUB IFrameHandler_GetMarginHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_SetNextFocus_Proxy( 
    IFrameHandler __RPC_FAR * This,
    DWORD dwUpDown,
    DWORD dwFrontBack);


void __RPC_STUB IFrameHandler_SetNextFocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_DoCrossFrameNavCheck_Proxy( 
    IFrameHandler __RPC_FAR * This,
    void __RPC_FAR *pTgtDoc,
    void __RPC_FAR *pSrcDoc,
    void __RPC_FAR *pSrcBrowser);


void __RPC_STUB IFrameHandler_DoCrossFrameNavCheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IFrameHandler_PerformCrossFramePolicyCheck_Proxy( 
    IFrameHandler __RPC_FAR * This,
    LPCWSTR pwszTargetFrameDomain,
    LPCWSTR pwszSourceFrameDomain,
    LPCWSTR pwszSourceFrameUrl,
    void __RPC_FAR *pSrcBrowser);


void __RPC_STUB IFrameHandler_PerformCrossFramePolicyCheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFrameHandler_INTERFACE_DEFINED__ */


#ifndef __IXMLHandler_INTERFACE_DEFINED__
#define __IXMLHandler_INTERFACE_DEFINED__

/* interface IXMLHandler */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IXMLHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C67B6DCE-BB98-44b4-8B4F-185B683CE4B2")
    IXMLHandler : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_XMLDocument( 
            /* [retval][out] */ IXMLDOMDocument __RPC_FAR *__RPC_FAR *ppXMLDocument) = 0;
        
        virtual void __RPC_FAR *STDMETHODCALLTYPE Initialize( 
            /* [in] */ void __RPC_FAR *pBrowser,
            /* [in] */ BOOL bLoadStylesheet) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IXMLHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IXMLHandler __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IXMLHandler __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IXMLHandler __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_XMLDocument )( 
            IXMLHandler __RPC_FAR * This,
            /* [retval][out] */ IXMLDOMDocument __RPC_FAR *__RPC_FAR *ppXMLDocument);
        
        void __RPC_FAR *( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IXMLHandler __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pBrowser,
            /* [in] */ BOOL bLoadStylesheet);
        
        END_INTERFACE
    } IXMLHandlerVtbl;

    interface IXMLHandler
    {
        CONST_VTBL struct IXMLHandlerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IXMLHandler_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IXMLHandler_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IXMLHandler_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IXMLHandler_get_XMLDocument(This,ppXMLDocument)	\
    (This)->lpVtbl -> get_XMLDocument(This,ppXMLDocument)

#define IXMLHandler_Initialize(This,pBrowser,bLoadStylesheet)	\
    (This)->lpVtbl -> Initialize(This,pBrowser,bLoadStylesheet)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IXMLHandler_get_XMLDocument_Proxy( 
    IXMLHandler __RPC_FAR * This,
    /* [retval][out] */ IXMLDOMDocument __RPC_FAR *__RPC_FAR *ppXMLDocument);


void __RPC_STUB IXMLHandler_get_XMLDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


void __RPC_FAR *STDMETHODCALLTYPE IXMLHandler_Initialize_Proxy( 
    IXMLHandler __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *pBrowser,
    /* [in] */ BOOL bLoadStylesheet);


void __RPC_STUB IXMLHandler_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IXMLHandler_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_webview_0326 */
/* [local] */ 

typedef 
enum _BEHAVIOR_EVENT
    {	BEHAVIOREVENT_FIRST	= 0,
	BEHAVIOREVENT_CONTENTREADY	= 0,
	BEHAVIOREVENT_DOCUMENTREADY	= 1,
	BEHAVIOREVENT_APPLYSTYLE	= 2,
	BEHAVIOREVENT_DOCUMENTCONTEXTCHANGE	= 3,
	BEHAVIOREVENT_CONTENTSAVE	= 4,
	BEHAVIOREVENT_LAST	= 4,
	BEHAVIOR_EVENT_Max	= 2147483647L
    }	BEHAVIOR_EVENT;



extern RPC_IF_HANDLE __MIDL_itf_webview_0326_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webview_0326_v0_0_s_ifspec;

#ifndef __IElementNamespace_INTERFACE_DEFINED__
#define __IElementNamespace_INTERFACE_DEFINED__

/* interface IElementNamespace */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IElementNamespace;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3050f671-98b5-11cf-bb82-00aa00bdce0b")
    IElementNamespace : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddTag( 
            /* [in] */ BSTR bstrTagName,
            /* [in] */ LONG lFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IElementNamespaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IElementNamespace __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IElementNamespace __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IElementNamespace __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddTag )( 
            IElementNamespace __RPC_FAR * This,
            /* [in] */ BSTR bstrTagName,
            /* [in] */ LONG lFlags);
        
        END_INTERFACE
    } IElementNamespaceVtbl;

    interface IElementNamespace
    {
        CONST_VTBL struct IElementNamespaceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IElementNamespace_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IElementNamespace_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IElementNamespace_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IElementNamespace_AddTag(This,bstrTagName,lFlags)	\
    (This)->lpVtbl -> AddTag(This,bstrTagName,lFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IElementNamespace_AddTag_Proxy( 
    IElementNamespace __RPC_FAR * This,
    /* [in] */ BSTR bstrTagName,
    /* [in] */ LONG lFlags);


void __RPC_STUB IElementNamespace_AddTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IElementNamespace_INTERFACE_DEFINED__ */


#ifndef __IElementNamespaceFactory_INTERFACE_DEFINED__
#define __IElementNamespaceFactory_INTERFACE_DEFINED__

/* interface IElementNamespaceFactory */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IElementNamespaceFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3050f672-98b5-11cf-bb82-00aa00bdce0b")
    IElementNamespaceFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ IElementNamespace __RPC_FAR *pNamespace) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IElementNamespaceFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IElementNamespaceFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IElementNamespaceFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IElementNamespaceFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Create )( 
            IElementNamespaceFactory __RPC_FAR * This,
            /* [in] */ IElementNamespace __RPC_FAR *pNamespace);
        
        END_INTERFACE
    } IElementNamespaceFactoryVtbl;

    interface IElementNamespaceFactory
    {
        CONST_VTBL struct IElementNamespaceFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IElementNamespaceFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IElementNamespaceFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IElementNamespaceFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IElementNamespaceFactory_Create(This,pNamespace)	\
    (This)->lpVtbl -> Create(This,pNamespace)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IElementNamespaceFactory_Create_Proxy( 
    IElementNamespaceFactory __RPC_FAR * This,
    /* [in] */ IElementNamespace __RPC_FAR *pNamespace);


void __RPC_STUB IElementNamespaceFactory_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IElementNamespaceFactory_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_webview_0328 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_webview_0328_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webview_0328_v0_0_s_ifspec;

#ifndef __IElementBehaviorSite_INTERFACE_DEFINED__
#define __IElementBehaviorSite_INTERFACE_DEFINED__

/* interface IElementBehaviorSite */
/* [uuid][unique][object] */ 


EXTERN_C const IID IID_IElementBehaviorSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3050f427-98b5-11cf-bb82-00aa00bdce0b")
    IElementBehaviorSite : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetElement( 
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppElement) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IElementBehaviorSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IElementBehaviorSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IElementBehaviorSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IElementBehaviorSite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetElement )( 
            IElementBehaviorSite __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppElement);
        
        END_INTERFACE
    } IElementBehaviorSiteVtbl;

    interface IElementBehaviorSite
    {
        CONST_VTBL struct IElementBehaviorSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IElementBehaviorSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IElementBehaviorSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IElementBehaviorSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IElementBehaviorSite_GetElement(This,ppElement)	\
    (This)->lpVtbl -> GetElement(This,ppElement)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IElementBehaviorSite_GetElement_Proxy( 
    IElementBehaviorSite __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppElement);


void __RPC_STUB IElementBehaviorSite_GetElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IElementBehaviorSite_INTERFACE_DEFINED__ */


#ifndef __IElementBehavior_INTERFACE_DEFINED__
#define __IElementBehavior_INTERFACE_DEFINED__

/* interface IElementBehavior */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IElementBehavior;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3050f425-98b5-11cf-bb82-00aa00bdce0b")
    IElementBehavior : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ IElementBehaviorSite __RPC_FAR *pBehaviorSite) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Notify( 
            /* [in] */ LONG lEvent,
            /* [out][in] */ VARIANT __RPC_FAR *pVar) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Detach( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IElementBehaviorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IElementBehavior __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IElementBehavior __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IElementBehavior __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IElementBehavior __RPC_FAR * This,
            /* [in] */ IElementBehaviorSite __RPC_FAR *pBehaviorSite);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Notify )( 
            IElementBehavior __RPC_FAR * This,
            /* [in] */ LONG lEvent,
            /* [out][in] */ VARIANT __RPC_FAR *pVar);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Detach )( 
            IElementBehavior __RPC_FAR * This);
        
        END_INTERFACE
    } IElementBehaviorVtbl;

    interface IElementBehavior
    {
        CONST_VTBL struct IElementBehaviorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IElementBehavior_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IElementBehavior_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IElementBehavior_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IElementBehavior_Init(This,pBehaviorSite)	\
    (This)->lpVtbl -> Init(This,pBehaviorSite)

#define IElementBehavior_Notify(This,lEvent,pVar)	\
    (This)->lpVtbl -> Notify(This,lEvent,pVar)

#define IElementBehavior_Detach(This)	\
    (This)->lpVtbl -> Detach(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IElementBehavior_Init_Proxy( 
    IElementBehavior __RPC_FAR * This,
    /* [in] */ IElementBehaviorSite __RPC_FAR *pBehaviorSite);


void __RPC_STUB IElementBehavior_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IElementBehavior_Notify_Proxy( 
    IElementBehavior __RPC_FAR * This,
    /* [in] */ LONG lEvent,
    /* [out][in] */ VARIANT __RPC_FAR *pVar);


void __RPC_STUB IElementBehavior_Notify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IElementBehavior_Detach_Proxy( 
    IElementBehavior __RPC_FAR * This);


void __RPC_STUB IElementBehavior_Detach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IElementBehavior_INTERFACE_DEFINED__ */


#ifndef __IElementBehaviorFactory_INTERFACE_DEFINED__
#define __IElementBehaviorFactory_INTERFACE_DEFINED__

/* interface IElementBehaviorFactory */
/* [uuid][unique][object] */ 


EXTERN_C const IID IID_IElementBehaviorFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3050f429-98b5-11cf-bb82-00aa00bdce0b")
    IElementBehaviorFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE FindBehavior( 
            /* [in] */ BSTR bstrBehavior,
            /* [in] */ BSTR bstrBehaviorUrl,
            /* [in] */ IElementBehaviorSite __RPC_FAR *pSite,
            /* [retval][out] */ IElementBehavior __RPC_FAR *__RPC_FAR *ppBehavior) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IElementBehaviorFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IElementBehaviorFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IElementBehaviorFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IElementBehaviorFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindBehavior )( 
            IElementBehaviorFactory __RPC_FAR * This,
            /* [in] */ BSTR bstrBehavior,
            /* [in] */ BSTR bstrBehaviorUrl,
            /* [in] */ IElementBehaviorSite __RPC_FAR *pSite,
            /* [retval][out] */ IElementBehavior __RPC_FAR *__RPC_FAR *ppBehavior);
        
        END_INTERFACE
    } IElementBehaviorFactoryVtbl;

    interface IElementBehaviorFactory
    {
        CONST_VTBL struct IElementBehaviorFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IElementBehaviorFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IElementBehaviorFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IElementBehaviorFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IElementBehaviorFactory_FindBehavior(This,bstrBehavior,bstrBehaviorUrl,pSite,ppBehavior)	\
    (This)->lpVtbl -> FindBehavior(This,bstrBehavior,bstrBehaviorUrl,pSite,ppBehavior)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IElementBehaviorFactory_FindBehavior_Proxy( 
    IElementBehaviorFactory __RPC_FAR * This,
    /* [in] */ BSTR bstrBehavior,
    /* [in] */ BSTR bstrBehaviorUrl,
    /* [in] */ IElementBehaviorSite __RPC_FAR *pSite,
    /* [retval][out] */ IElementBehavior __RPC_FAR *__RPC_FAR *ppBehavior);


void __RPC_STUB IElementBehaviorFactory_FindBehavior_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IElementBehaviorFactory_INTERFACE_DEFINED__ */



#ifndef __WEBVWLib_LIBRARY_DEFINED__
#define __WEBVWLib_LIBRARY_DEFINED__

/* library WEBVWLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_WEBVWLib;

EXTERN_C const CLSID CLSID_CInetConn;

#ifdef __cplusplus

class DECLSPEC_UUID("43F14462-EBC1-421b-8F98-2DB81293AA77")
CInetConn;
#endif

EXTERN_C const CLSID CLSID_CFrameHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("870C7D98-D943-4cf5-A297-5C4C2E63CB5E")
CFrameHandler;
#endif

EXTERN_C const CLSID CLSID_CXMLHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("182D0A92-2F8E-410a-A8CA-2B23AAAD4893")
CXMLHandler;
#endif

EXTERN_C const CLSID CLSID_PIEHTML;

#ifdef __cplusplus

class DECLSPEC_UUID("698E3FCA-70C3-11D0-81E8-00A0C90AD21A")
PIEHTML;
#endif
#endif /* __WEBVWLib_LIBRARY_DEFINED__ */

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


