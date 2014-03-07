
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:32:07 2008
 */
/* Compiler settings for .\webvw.idl:
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


#ifndef __webvw_h__
#define __webvw_h__

/* Forward Declarations */ 

#ifndef __IPIEOmNavigator_FWD_DEFINED__
#define __IPIEOmNavigator_FWD_DEFINED__
typedef interface IPIEOmNavigator IPIEOmNavigator;
#endif 	/* __IPIEOmNavigator_FWD_DEFINED__ */


#ifndef __IPIEOmNavigator2_FWD_DEFINED__
#define __IPIEOmNavigator2_FWD_DEFINED__
typedef interface IPIEOmNavigator2 IPIEOmNavigator2;
#endif 	/* __IPIEOmNavigator2_FWD_DEFINED__ */


#ifndef __IPIEOmHistory_FWD_DEFINED__
#define __IPIEOmHistory_FWD_DEFINED__
typedef interface IPIEOmHistory IPIEOmHistory;
#endif 	/* __IPIEOmHistory_FWD_DEFINED__ */


#ifndef __IPIEHTMLElementCollection_FWD_DEFINED__
#define __IPIEHTMLElementCollection_FWD_DEFINED__
typedef interface IPIEHTMLElementCollection IPIEHTMLElementCollection;
#endif 	/* __IPIEHTMLElementCollection_FWD_DEFINED__ */


#ifndef __IPIEHTMLLocation_FWD_DEFINED__
#define __IPIEHTMLLocation_FWD_DEFINED__
typedef interface IPIEHTMLLocation IPIEHTMLLocation;
#endif 	/* __IPIEHTMLLocation_FWD_DEFINED__ */


#ifndef __IPIEHTMLDocument_FWD_DEFINED__
#define __IPIEHTMLDocument_FWD_DEFINED__
typedef interface IPIEHTMLDocument IPIEHTMLDocument;
#endif 	/* __IPIEHTMLDocument_FWD_DEFINED__ */


#ifndef __IPIEHTMLWindow_FWD_DEFINED__
#define __IPIEHTMLWindow_FWD_DEFINED__
typedef interface IPIEHTMLWindow IPIEHTMLWindow;
#endif 	/* __IPIEHTMLWindow_FWD_DEFINED__ */


#ifndef __IPIEHTMLWindow2_FWD_DEFINED__
#define __IPIEHTMLWindow2_FWD_DEFINED__
typedef interface IPIEHTMLWindow2 IPIEHTMLWindow2;
#endif 	/* __IPIEHTMLWindow2_FWD_DEFINED__ */


#ifndef __IPIEHTMLDocument2_FWD_DEFINED__
#define __IPIEHTMLDocument2_FWD_DEFINED__
typedef interface IPIEHTMLDocument2 IPIEHTMLDocument2;
#endif 	/* __IPIEHTMLDocument2_FWD_DEFINED__ */


#ifndef __IPIEHTMLElement_FWD_DEFINED__
#define __IPIEHTMLElement_FWD_DEFINED__
typedef interface IPIEHTMLElement IPIEHTMLElement;
#endif 	/* __IPIEHTMLElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLImgElement_FWD_DEFINED__
#define __IPIEHTMLImgElement_FWD_DEFINED__
typedef interface IPIEHTMLImgElement IPIEHTMLImgElement;
#endif 	/* __IPIEHTMLImgElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLDOMNode_FWD_DEFINED__
#define __IPIEHTMLDOMNode_FWD_DEFINED__
typedef interface IPIEHTMLDOMNode IPIEHTMLDOMNode;
#endif 	/* __IPIEHTMLDOMNode_FWD_DEFINED__ */


#ifndef __IPIEHTMLDocument3_FWD_DEFINED__
#define __IPIEHTMLDocument3_FWD_DEFINED__
typedef interface IPIEHTMLDocument3 IPIEHTMLDocument3;
#endif 	/* __IPIEHTMLDocument3_FWD_DEFINED__ */


#ifndef __IPIEHTMLDocument4_FWD_DEFINED__
#define __IPIEHTMLDocument4_FWD_DEFINED__
typedef interface IPIEHTMLDocument4 IPIEHTMLDocument4;
#endif 	/* __IPIEHTMLDocument4_FWD_DEFINED__ */


#ifndef __IPIEHTMLScreen_FWD_DEFINED__
#define __IPIEHTMLScreen_FWD_DEFINED__
typedef interface IPIEHTMLScreen IPIEHTMLScreen;
#endif 	/* __IPIEHTMLScreen_FWD_DEFINED__ */


#ifndef __IPIEHTMLScreen2_FWD_DEFINED__
#define __IPIEHTMLScreen2_FWD_DEFINED__
typedef interface IPIEHTMLScreen2 IPIEHTMLScreen2;
#endif 	/* __IPIEHTMLScreen2_FWD_DEFINED__ */


#ifndef __IPIEHTMLImageElementFactory_FWD_DEFINED__
#define __IPIEHTMLImageElementFactory_FWD_DEFINED__
typedef interface IPIEHTMLImageElementFactory IPIEHTMLImageElementFactory;
#endif 	/* __IPIEHTMLImageElementFactory_FWD_DEFINED__ */


#ifndef __IPIEHTMLOptionElement_FWD_DEFINED__
#define __IPIEHTMLOptionElement_FWD_DEFINED__
typedef interface IPIEHTMLOptionElement IPIEHTMLOptionElement;
#endif 	/* __IPIEHTMLOptionElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLOptionElementFactory_FWD_DEFINED__
#define __IPIEHTMLOptionElementFactory_FWD_DEFINED__
typedef interface IPIEHTMLOptionElementFactory IPIEHTMLOptionElementFactory;
#endif 	/* __IPIEHTMLOptionElementFactory_FWD_DEFINED__ */


#ifndef __IPIEHTMLWindow3_FWD_DEFINED__
#define __IPIEHTMLWindow3_FWD_DEFINED__
typedef interface IPIEHTMLWindow3 IPIEHTMLWindow3;
#endif 	/* __IPIEHTMLWindow3_FWD_DEFINED__ */


#ifndef __PIEHTMLWindowEvents_FWD_DEFINED__
#define __PIEHTMLWindowEvents_FWD_DEFINED__
typedef interface PIEHTMLWindowEvents PIEHTMLWindowEvents;
#endif 	/* __PIEHTMLWindowEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLWindow_FWD_DEFINED__
#define __PIEHTMLWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLWindow PIEHTMLWindow;
#else
typedef struct PIEHTMLWindow PIEHTMLWindow;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLWindow_FWD_DEFINED__ */


#ifndef __IPIEHTMLElement2_FWD_DEFINED__
#define __IPIEHTMLElement2_FWD_DEFINED__
typedef interface IPIEHTMLElement2 IPIEHTMLElement2;
#endif 	/* __IPIEHTMLElement2_FWD_DEFINED__ */


#ifndef __IPIEHTMLElement3_FWD_DEFINED__
#define __IPIEHTMLElement3_FWD_DEFINED__
typedef interface IPIEHTMLElement3 IPIEHTMLElement3;
#endif 	/* __IPIEHTMLElement3_FWD_DEFINED__ */


#ifndef __PIEHTMLElementEvents_FWD_DEFINED__
#define __PIEHTMLElementEvents_FWD_DEFINED__
typedef interface PIEHTMLElementEvents PIEHTMLElementEvents;
#endif 	/* __PIEHTMLElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLElement_FWD_DEFINED__
#define __PIEHTMLElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLElement PIEHTMLElement;
#else
typedef struct PIEHTMLElement PIEHTMLElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLFrameSetElement_FWD_DEFINED__
#define __IPIEHTMLFrameSetElement_FWD_DEFINED__
typedef interface IPIEHTMLFrameSetElement IPIEHTMLFrameSetElement;
#endif 	/* __IPIEHTMLFrameSetElement_FWD_DEFINED__ */


#ifndef __PIEHTMLFrameSetElementEvents_FWD_DEFINED__
#define __PIEHTMLFrameSetElementEvents_FWD_DEFINED__
typedef interface PIEHTMLFrameSetElementEvents PIEHTMLFrameSetElementEvents;
#endif 	/* __PIEHTMLFrameSetElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLFrameSetElement_FWD_DEFINED__
#define __PIEHTMLFrameSetElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLFrameSetElement PIEHTMLFrameSetElement;
#else
typedef struct PIEHTMLFrameSetElement PIEHTMLFrameSetElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLFrameSetElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLFrameElement_FWD_DEFINED__
#define __IPIEHTMLFrameElement_FWD_DEFINED__
typedef interface IPIEHTMLFrameElement IPIEHTMLFrameElement;
#endif 	/* __IPIEHTMLFrameElement_FWD_DEFINED__ */


#ifndef __PIEHTMLFrameElement_FWD_DEFINED__
#define __PIEHTMLFrameElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLFrameElement PIEHTMLFrameElement;
#else
typedef struct PIEHTMLFrameElement PIEHTMLFrameElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLFrameElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLIFrameElement_FWD_DEFINED__
#define __IPIEHTMLIFrameElement_FWD_DEFINED__
typedef interface IPIEHTMLIFrameElement IPIEHTMLIFrameElement;
#endif 	/* __IPIEHTMLIFrameElement_FWD_DEFINED__ */


#ifndef __PIEHTMLIFrameElement_FWD_DEFINED__
#define __PIEHTMLIFrameElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLIFrameElement PIEHTMLIFrameElement;
#else
typedef struct PIEHTMLIFrameElement PIEHTMLIFrameElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLIFrameElement_FWD_DEFINED__ */


#ifndef __PIEHTMLImgElementEvents_FWD_DEFINED__
#define __PIEHTMLImgElementEvents_FWD_DEFINED__
typedef interface PIEHTMLImgElementEvents PIEHTMLImgElementEvents;
#endif 	/* __PIEHTMLImgElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLImgElement_FWD_DEFINED__
#define __PIEHTMLImgElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLImgElement PIEHTMLImgElement;
#else
typedef struct PIEHTMLImgElement PIEHTMLImgElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLImgElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLAnchorElement_FWD_DEFINED__
#define __IPIEHTMLAnchorElement_FWD_DEFINED__
typedef interface IPIEHTMLAnchorElement IPIEHTMLAnchorElement;
#endif 	/* __IPIEHTMLAnchorElement_FWD_DEFINED__ */


#ifndef __PIEHTMLAnchorElementEvents_FWD_DEFINED__
#define __PIEHTMLAnchorElementEvents_FWD_DEFINED__
typedef interface PIEHTMLAnchorElementEvents PIEHTMLAnchorElementEvents;
#endif 	/* __PIEHTMLAnchorElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLAnchorElement_FWD_DEFINED__
#define __PIEHTMLAnchorElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLAnchorElement PIEHTMLAnchorElement;
#else
typedef struct PIEHTMLAnchorElement PIEHTMLAnchorElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLAnchorElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLFormElement_FWD_DEFINED__
#define __IPIEHTMLFormElement_FWD_DEFINED__
typedef interface IPIEHTMLFormElement IPIEHTMLFormElement;
#endif 	/* __IPIEHTMLFormElement_FWD_DEFINED__ */


#ifndef __PIEHTMLFormElementEvents_FWD_DEFINED__
#define __PIEHTMLFormElementEvents_FWD_DEFINED__
typedef interface PIEHTMLFormElementEvents PIEHTMLFormElementEvents;
#endif 	/* __PIEHTMLFormElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLFormElement_FWD_DEFINED__
#define __PIEHTMLFormElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLFormElement PIEHTMLFormElement;
#else
typedef struct PIEHTMLFormElement PIEHTMLFormElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLFormElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLControlElement_FWD_DEFINED__
#define __IPIEHTMLControlElement_FWD_DEFINED__
typedef interface IPIEHTMLControlElement IPIEHTMLControlElement;
#endif 	/* __IPIEHTMLControlElement_FWD_DEFINED__ */


#ifndef __PIEHTMLControlElement_FWD_DEFINED__
#define __PIEHTMLControlElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLControlElement PIEHTMLControlElement;
#else
typedef struct PIEHTMLControlElement PIEHTMLControlElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLControlElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLInputButtonElement_FWD_DEFINED__
#define __IPIEHTMLInputButtonElement_FWD_DEFINED__
typedef interface IPIEHTMLInputButtonElement IPIEHTMLInputButtonElement;
#endif 	/* __IPIEHTMLInputButtonElement_FWD_DEFINED__ */


#ifndef __PIEHTMLInputButtonElementEvents_FWD_DEFINED__
#define __PIEHTMLInputButtonElementEvents_FWD_DEFINED__
typedef interface PIEHTMLInputButtonElementEvents PIEHTMLInputButtonElementEvents;
#endif 	/* __PIEHTMLInputButtonElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLInputButtonElement_FWD_DEFINED__
#define __PIEHTMLInputButtonElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLInputButtonElement PIEHTMLInputButtonElement;
#else
typedef struct PIEHTMLInputButtonElement PIEHTMLInputButtonElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLInputButtonElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLOptionButtonElement_FWD_DEFINED__
#define __IPIEHTMLOptionButtonElement_FWD_DEFINED__
typedef interface IPIEHTMLOptionButtonElement IPIEHTMLOptionButtonElement;
#endif 	/* __IPIEHTMLOptionButtonElement_FWD_DEFINED__ */


#ifndef __PIEHTMLOptionButtonElementEvents_FWD_DEFINED__
#define __PIEHTMLOptionButtonElementEvents_FWD_DEFINED__
typedef interface PIEHTMLOptionButtonElementEvents PIEHTMLOptionButtonElementEvents;
#endif 	/* __PIEHTMLOptionButtonElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLOptionButtonElement_FWD_DEFINED__
#define __PIEHTMLOptionButtonElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLOptionButtonElement PIEHTMLOptionButtonElement;
#else
typedef struct PIEHTMLOptionButtonElement PIEHTMLOptionButtonElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLOptionButtonElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLSelectElement_FWD_DEFINED__
#define __IPIEHTMLSelectElement_FWD_DEFINED__
typedef interface IPIEHTMLSelectElement IPIEHTMLSelectElement;
#endif 	/* __IPIEHTMLSelectElement_FWD_DEFINED__ */


#ifndef __PIEHTMLSelectElementEvents_FWD_DEFINED__
#define __PIEHTMLSelectElementEvents_FWD_DEFINED__
typedef interface PIEHTMLSelectElementEvents PIEHTMLSelectElementEvents;
#endif 	/* __PIEHTMLSelectElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLSelectElement_FWD_DEFINED__
#define __PIEHTMLSelectElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLSelectElement PIEHTMLSelectElement;
#else
typedef struct PIEHTMLSelectElement PIEHTMLSelectElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLSelectElement_FWD_DEFINED__ */


#ifndef __PIEHTMLOptionElement_FWD_DEFINED__
#define __PIEHTMLOptionElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLOptionElement PIEHTMLOptionElement;
#else
typedef struct PIEHTMLOptionElement PIEHTMLOptionElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLOptionElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLInputTextElement_FWD_DEFINED__
#define __IPIEHTMLInputTextElement_FWD_DEFINED__
typedef interface IPIEHTMLInputTextElement IPIEHTMLInputTextElement;
#endif 	/* __IPIEHTMLInputTextElement_FWD_DEFINED__ */


#ifndef __PIEHTMLInputTextElementEvents_FWD_DEFINED__
#define __PIEHTMLInputTextElementEvents_FWD_DEFINED__
typedef interface PIEHTMLInputTextElementEvents PIEHTMLInputTextElementEvents;
#endif 	/* __PIEHTMLInputTextElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLInputTextElement_FWD_DEFINED__
#define __PIEHTMLInputTextElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLInputTextElement PIEHTMLInputTextElement;
#else
typedef struct PIEHTMLInputTextElement PIEHTMLInputTextElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLInputTextElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLTextAreaElement_FWD_DEFINED__
#define __IPIEHTMLTextAreaElement_FWD_DEFINED__
typedef interface IPIEHTMLTextAreaElement IPIEHTMLTextAreaElement;
#endif 	/* __IPIEHTMLTextAreaElement_FWD_DEFINED__ */


#ifndef __PIEHTMLTextAreaElementEvents_FWD_DEFINED__
#define __PIEHTMLTextAreaElementEvents_FWD_DEFINED__
typedef interface PIEHTMLTextAreaElementEvents PIEHTMLTextAreaElementEvents;
#endif 	/* __PIEHTMLTextAreaElementEvents_FWD_DEFINED__ */


#ifndef __PIEHTMLTextAreaElement_FWD_DEFINED__
#define __PIEHTMLTextAreaElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLTextAreaElement PIEHTMLTextAreaElement;
#else
typedef struct PIEHTMLTextAreaElement PIEHTMLTextAreaElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLTextAreaElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLInputHiddenElement_FWD_DEFINED__
#define __IPIEHTMLInputHiddenElement_FWD_DEFINED__
typedef interface IPIEHTMLInputHiddenElement IPIEHTMLInputHiddenElement;
#endif 	/* __IPIEHTMLInputHiddenElement_FWD_DEFINED__ */


#ifndef __PIEHTMLInputHiddenElement_FWD_DEFINED__
#define __PIEHTMLInputHiddenElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLInputHiddenElement PIEHTMLInputHiddenElement;
#else
typedef struct PIEHTMLInputHiddenElement PIEHTMLInputHiddenElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLInputHiddenElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLBodyElement_FWD_DEFINED__
#define __IPIEHTMLBodyElement_FWD_DEFINED__
typedef interface IPIEHTMLBodyElement IPIEHTMLBodyElement;
#endif 	/* __IPIEHTMLBodyElement_FWD_DEFINED__ */


#ifndef __PIEHTMLBodyElement_FWD_DEFINED__
#define __PIEHTMLBodyElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLBodyElement PIEHTMLBodyElement;
#else
typedef struct PIEHTMLBodyElement PIEHTMLBodyElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLBodyElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLDivElement_FWD_DEFINED__
#define __IPIEHTMLDivElement_FWD_DEFINED__
typedef interface IPIEHTMLDivElement IPIEHTMLDivElement;
#endif 	/* __IPIEHTMLDivElement_FWD_DEFINED__ */


#ifndef __PIEHTMLDivElement_FWD_DEFINED__
#define __PIEHTMLDivElement_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLDivElement PIEHTMLDivElement;
#else
typedef struct PIEHTMLDivElement PIEHTMLDivElement;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLDivElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLXmlElement_FWD_DEFINED__
#define __IPIEHTMLXmlElement_FWD_DEFINED__
typedef interface IPIEHTMLXmlElement IPIEHTMLXmlElement;
#endif 	/* __IPIEHTMLXmlElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLObjectElement_FWD_DEFINED__
#define __IPIEHTMLObjectElement_FWD_DEFINED__
typedef interface IPIEHTMLObjectElement IPIEHTMLObjectElement;
#endif 	/* __IPIEHTMLObjectElement_FWD_DEFINED__ */


#ifndef __IPIEHTMLStyle_FWD_DEFINED__
#define __IPIEHTMLStyle_FWD_DEFINED__
typedef interface IPIEHTMLStyle IPIEHTMLStyle;
#endif 	/* __IPIEHTMLStyle_FWD_DEFINED__ */


#ifndef __PIEHTMLStyle_FWD_DEFINED__
#define __PIEHTMLStyle_FWD_DEFINED__

#ifdef __cplusplus
typedef class PIEHTMLStyle PIEHTMLStyle;
#else
typedef struct PIEHTMLStyle PIEHTMLStyle;
#endif /* __cplusplus */

#endif 	/* __PIEHTMLStyle_FWD_DEFINED__ */


#ifndef __IHomePage_FWD_DEFINED__
#define __IHomePage_FWD_DEFINED__
typedef interface IHomePage IHomePage;
#endif 	/* __IHomePage_FWD_DEFINED__ */


#ifndef __CHomePage_FWD_DEFINED__
#define __CHomePage_FWD_DEFINED__

#ifdef __cplusplus
typedef class CHomePage CHomePage;
#else
typedef struct CHomePage CHomePage;
#endif /* __cplusplus */

#endif 	/* __CHomePage_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "dispex.h"
#include "urlmon.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_webvw_0000 */
/* [local] */ 

#pragma once



extern RPC_IF_HANDLE __MIDL_itf_webvw_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webvw_0000_v0_0_s_ifspec;


#ifndef __WEBVIEWLib_LIBRARY_DEFINED__
#define __WEBVIEWLib_LIBRARY_DEFINED__

/* library WEBVIEWLib */
/* [version][uuid] */ 







EXTERN_C const IID LIBID_WEBVIEWLib;

#ifndef __IPIEOmNavigator_INTERFACE_DEFINED__
#define __IPIEOmNavigator_INTERFACE_DEFINED__

/* interface IPIEOmNavigator */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEOmNavigator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2100")
    IPIEOmNavigator : public IDispatchEx
    {
    public:
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_toString( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_appCodeName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrCodeName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_appName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_appVersion( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVersion) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_userAgent( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrUserAgent) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_platform( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPlatform) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE javaEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbJavaEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE taintEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbTaintEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEOmNavigatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEOmNavigator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEOmNavigator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_appCodeName )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrCodeName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_appName )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_appVersion )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVersion);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_userAgent )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrUserAgent);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_platform )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPlatform);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *javaEnabled )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbJavaEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *taintEnabled )( 
            IPIEOmNavigator __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbTaintEnabled);
        
        END_INTERFACE
    } IPIEOmNavigatorVtbl;

    interface IPIEOmNavigator
    {
        CONST_VTBL struct IPIEOmNavigatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEOmNavigator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEOmNavigator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEOmNavigator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEOmNavigator_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEOmNavigator_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEOmNavigator_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEOmNavigator_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEOmNavigator_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEOmNavigator_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEOmNavigator_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEOmNavigator_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEOmNavigator_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEOmNavigator_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEOmNavigator_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEOmNavigator_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEOmNavigator_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEOmNavigator_get_appCodeName(This,pbstrCodeName)	\
    (This)->lpVtbl -> get_appCodeName(This,pbstrCodeName)

#define IPIEOmNavigator_get_appName(This,pbstrName)	\
    (This)->lpVtbl -> get_appName(This,pbstrName)

#define IPIEOmNavigator_get_appVersion(This,pbstrVersion)	\
    (This)->lpVtbl -> get_appVersion(This,pbstrVersion)

#define IPIEOmNavigator_get_userAgent(This,pbstrUserAgent)	\
    (This)->lpVtbl -> get_userAgent(This,pbstrUserAgent)

#define IPIEOmNavigator_get_platform(This,pbstrPlatform)	\
    (This)->lpVtbl -> get_platform(This,pbstrPlatform)

#define IPIEOmNavigator_javaEnabled(This,pvbJavaEnabled)	\
    (This)->lpVtbl -> javaEnabled(This,pvbJavaEnabled)

#define IPIEOmNavigator_taintEnabled(This,pvbTaintEnabled)	\
    (This)->lpVtbl -> taintEnabled(This,pvbTaintEnabled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator_get_toString_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrString);


void __RPC_STUB IPIEOmNavigator_get_toString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator_get_appCodeName_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrCodeName);


void __RPC_STUB IPIEOmNavigator_get_appCodeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator_get_appName_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IPIEOmNavigator_get_appName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator_get_appVersion_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrVersion);


void __RPC_STUB IPIEOmNavigator_get_appVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator_get_userAgent_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrUserAgent);


void __RPC_STUB IPIEOmNavigator_get_userAgent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator_get_platform_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrPlatform);


void __RPC_STUB IPIEOmNavigator_get_platform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEOmNavigator_javaEnabled_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbJavaEnabled);


void __RPC_STUB IPIEOmNavigator_javaEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEOmNavigator_taintEnabled_Proxy( 
    IPIEOmNavigator __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbTaintEnabled);


void __RPC_STUB IPIEOmNavigator_taintEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEOmNavigator_INTERFACE_DEFINED__ */


#ifndef __IPIEOmNavigator2_INTERFACE_DEFINED__
#define __IPIEOmNavigator2_INTERFACE_DEFINED__

/* interface IPIEOmNavigator2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEOmNavigator2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e208f803-5a65-4ee0-8f8c-6c14f28a071b")
    IPIEOmNavigator2 : public IPIEOmNavigator
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_appMinorVersion( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMinorVersion) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_cpuClass( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrCpuClass) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_userAgentOS( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOS) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_systemLanguage( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLang) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_userLanguage( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLang) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_cookieEnabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbCookieEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEOmNavigator2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEOmNavigator2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEOmNavigator2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_appCodeName )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrCodeName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_appName )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_appVersion )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVersion);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_userAgent )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrUserAgent);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_platform )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPlatform);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *javaEnabled )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbJavaEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *taintEnabled )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbTaintEnabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_appMinorVersion )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMinorVersion);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cpuClass )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrCpuClass);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_userAgentOS )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOS);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_systemLanguage )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLang);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_userLanguage )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLang);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cookieEnabled )( 
            IPIEOmNavigator2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbCookieEnabled);
        
        END_INTERFACE
    } IPIEOmNavigator2Vtbl;

    interface IPIEOmNavigator2
    {
        CONST_VTBL struct IPIEOmNavigator2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEOmNavigator2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEOmNavigator2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEOmNavigator2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEOmNavigator2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEOmNavigator2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEOmNavigator2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEOmNavigator2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEOmNavigator2_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEOmNavigator2_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEOmNavigator2_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEOmNavigator2_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEOmNavigator2_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEOmNavigator2_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEOmNavigator2_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEOmNavigator2_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEOmNavigator2_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEOmNavigator2_get_appCodeName(This,pbstrCodeName)	\
    (This)->lpVtbl -> get_appCodeName(This,pbstrCodeName)

#define IPIEOmNavigator2_get_appName(This,pbstrName)	\
    (This)->lpVtbl -> get_appName(This,pbstrName)

#define IPIEOmNavigator2_get_appVersion(This,pbstrVersion)	\
    (This)->lpVtbl -> get_appVersion(This,pbstrVersion)

#define IPIEOmNavigator2_get_userAgent(This,pbstrUserAgent)	\
    (This)->lpVtbl -> get_userAgent(This,pbstrUserAgent)

#define IPIEOmNavigator2_get_platform(This,pbstrPlatform)	\
    (This)->lpVtbl -> get_platform(This,pbstrPlatform)

#define IPIEOmNavigator2_javaEnabled(This,pvbJavaEnabled)	\
    (This)->lpVtbl -> javaEnabled(This,pvbJavaEnabled)

#define IPIEOmNavigator2_taintEnabled(This,pvbTaintEnabled)	\
    (This)->lpVtbl -> taintEnabled(This,pvbTaintEnabled)


#define IPIEOmNavigator2_get_appMinorVersion(This,pbstrMinorVersion)	\
    (This)->lpVtbl -> get_appMinorVersion(This,pbstrMinorVersion)

#define IPIEOmNavigator2_get_cpuClass(This,pbstrCpuClass)	\
    (This)->lpVtbl -> get_cpuClass(This,pbstrCpuClass)

#define IPIEOmNavigator2_get_userAgentOS(This,pbstrOS)	\
    (This)->lpVtbl -> get_userAgentOS(This,pbstrOS)

#define IPIEOmNavigator2_get_systemLanguage(This,pbstrLang)	\
    (This)->lpVtbl -> get_systemLanguage(This,pbstrLang)

#define IPIEOmNavigator2_get_userLanguage(This,pbstrLang)	\
    (This)->lpVtbl -> get_userLanguage(This,pbstrLang)

#define IPIEOmNavigator2_get_cookieEnabled(This,pvbCookieEnabled)	\
    (This)->lpVtbl -> get_cookieEnabled(This,pvbCookieEnabled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator2_get_appMinorVersion_Proxy( 
    IPIEOmNavigator2 __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrMinorVersion);


void __RPC_STUB IPIEOmNavigator2_get_appMinorVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator2_get_cpuClass_Proxy( 
    IPIEOmNavigator2 __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrCpuClass);


void __RPC_STUB IPIEOmNavigator2_get_cpuClass_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator2_get_userAgentOS_Proxy( 
    IPIEOmNavigator2 __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOS);


void __RPC_STUB IPIEOmNavigator2_get_userAgentOS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator2_get_systemLanguage_Proxy( 
    IPIEOmNavigator2 __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrLang);


void __RPC_STUB IPIEOmNavigator2_get_systemLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator2_get_userLanguage_Proxy( 
    IPIEOmNavigator2 __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrLang);


void __RPC_STUB IPIEOmNavigator2_get_userLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmNavigator2_get_cookieEnabled_Proxy( 
    IPIEOmNavigator2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbCookieEnabled);


void __RPC_STUB IPIEOmNavigator2_get_cookieEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEOmNavigator2_INTERFACE_DEFINED__ */


#ifndef __IPIEOmHistory_INTERFACE_DEFINED__
#define __IPIEOmHistory_INTERFACE_DEFINED__

/* interface IPIEOmHistory */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEOmHistory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2104")
    IPIEOmHistory : public IDispatchEx
    {
    public:
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_toString( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *plLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE back( 
            /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE forward( 
            /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE go( 
            /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEOmHistoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEOmHistory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEOmHistory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *back )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *forward )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *go )( 
            IPIEOmHistory __RPC_FAR * This,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance);
        
        END_INTERFACE
    } IPIEOmHistoryVtbl;

    interface IPIEOmHistory
    {
        CONST_VTBL struct IPIEOmHistoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEOmHistory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEOmHistory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEOmHistory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEOmHistory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEOmHistory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEOmHistory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEOmHistory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEOmHistory_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEOmHistory_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEOmHistory_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEOmHistory_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEOmHistory_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEOmHistory_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEOmHistory_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEOmHistory_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEOmHistory_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEOmHistory_get_length(This,plLength)	\
    (This)->lpVtbl -> get_length(This,plLength)

#define IPIEOmHistory_back(This,pvtDistance)	\
    (This)->lpVtbl -> back(This,pvtDistance)

#define IPIEOmHistory_forward(This,pvtDistance)	\
    (This)->lpVtbl -> forward(This,pvtDistance)

#define IPIEOmHistory_go(This,pvtDistance)	\
    (This)->lpVtbl -> go(This,pvtDistance)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEOmHistory_get_toString_Proxy( 
    IPIEOmHistory __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrString);


void __RPC_STUB IPIEOmHistory_get_toString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEOmHistory_get_length_Proxy( 
    IPIEOmHistory __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLength);


void __RPC_STUB IPIEOmHistory_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEOmHistory_back_Proxy( 
    IPIEOmHistory __RPC_FAR * This,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance);


void __RPC_STUB IPIEOmHistory_back_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEOmHistory_forward_Proxy( 
    IPIEOmHistory __RPC_FAR * This,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance);


void __RPC_STUB IPIEOmHistory_forward_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEOmHistory_go_Proxy( 
    IPIEOmHistory __RPC_FAR * This,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtDistance);


void __RPC_STUB IPIEOmHistory_go_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEOmHistory_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLElementCollection_INTERFACE_DEFINED__
#define __IPIEHTMLElementCollection_INTERFACE_DEFINED__

/* interface IPIEHTMLElementCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLElementCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2105")
    IPIEHTMLElementCollection : public IDispatchEx
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *plLength) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE item( 
            /* [optional][in] */ VARIANT vtName,
            /* [optional][in] */ VARIANT vtIndex,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispItem) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLElementCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLElementCollection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLElementCollection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLength);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *item )( 
            IPIEHTMLElementCollection __RPC_FAR * This,
            /* [optional][in] */ VARIANT vtName,
            /* [optional][in] */ VARIANT vtIndex,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispItem);
        
        END_INTERFACE
    } IPIEHTMLElementCollectionVtbl;

    interface IPIEHTMLElementCollection
    {
        CONST_VTBL struct IPIEHTMLElementCollectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLElementCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLElementCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLElementCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLElementCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLElementCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLElementCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLElementCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLElementCollection_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLElementCollection_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLElementCollection_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLElementCollection_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLElementCollection_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLElementCollection_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLElementCollection_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLElementCollection_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLElementCollection_get_length(This,plLength)	\
    (This)->lpVtbl -> get_length(This,plLength)

#define IPIEHTMLElementCollection_item(This,vtName,vtIndex,ppdispItem)	\
    (This)->lpVtbl -> item(This,vtName,vtIndex,ppdispItem)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElementCollection_get_length_Proxy( 
    IPIEHTMLElementCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLength);


void __RPC_STUB IPIEHTMLElementCollection_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElementCollection_item_Proxy( 
    IPIEHTMLElementCollection __RPC_FAR * This,
    /* [optional][in] */ VARIANT vtName,
    /* [optional][in] */ VARIANT vtIndex,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispItem);


void __RPC_STUB IPIEHTMLElementCollection_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLElementCollection_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLLocation_INTERFACE_DEFINED__
#define __IPIEHTMLLocation_INTERFACE_DEFINED__

/* interface IPIEHTMLLocation */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLLocation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2109")
    IPIEHTMLLocation : public IDispatchEx
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_href( 
            /* [in] */ BSTR bstrHREF) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_href( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHREF) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_protocol( 
            /* [in] */ BSTR bstrProtocol) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_protocol( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrProtocol) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_host( 
            /* [in] */ BSTR bstrHost) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_host( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHost) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_hostname( 
            /* [in] */ BSTR bstrHostName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_hostname( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHostName) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_port( 
            /* [in] */ BSTR bstrPort) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_port( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPort) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_pathname( 
            /* [in] */ BSTR bstrPathName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_pathname( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPathName) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_search( 
            /* [in] */ BSTR bstrSearch) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_search( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSearch) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_hash( 
            /* [in] */ BSTR bstrHash) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_hash( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHash) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE reload( 
            /* [defaultvalue][in] */ VARIANT_BOOL vbReloadSource = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE replace( 
            /* [in] */ BSTR bstrURL) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLLocationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLLocation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLLocation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_href )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrHREF);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_href )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHREF);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_protocol )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrProtocol);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_protocol )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrProtocol);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_host )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrHost);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_host )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHost);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hostname )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrHostName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hostname )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHostName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_port )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrPort);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_port )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPort);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_pathname )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrPathName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_pathname )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPathName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_search )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrSearch);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_search )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSearch);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hash )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrHash);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hash )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHash);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *reload )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [defaultvalue][in] */ VARIANT_BOOL vbReloadSource);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *replace )( 
            IPIEHTMLLocation __RPC_FAR * This,
            /* [in] */ BSTR bstrURL);
        
        END_INTERFACE
    } IPIEHTMLLocationVtbl;

    interface IPIEHTMLLocation
    {
        CONST_VTBL struct IPIEHTMLLocationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLLocation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLLocation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLLocation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLLocation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLLocation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLLocation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLLocation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLLocation_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLLocation_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLLocation_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLLocation_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLLocation_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLLocation_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLLocation_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLLocation_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLLocation_put_href(This,bstrHREF)	\
    (This)->lpVtbl -> put_href(This,bstrHREF)

#define IPIEHTMLLocation_get_href(This,pbstrHREF)	\
    (This)->lpVtbl -> get_href(This,pbstrHREF)

#define IPIEHTMLLocation_put_protocol(This,bstrProtocol)	\
    (This)->lpVtbl -> put_protocol(This,bstrProtocol)

#define IPIEHTMLLocation_get_protocol(This,pbstrProtocol)	\
    (This)->lpVtbl -> get_protocol(This,pbstrProtocol)

#define IPIEHTMLLocation_put_host(This,bstrHost)	\
    (This)->lpVtbl -> put_host(This,bstrHost)

#define IPIEHTMLLocation_get_host(This,pbstrHost)	\
    (This)->lpVtbl -> get_host(This,pbstrHost)

#define IPIEHTMLLocation_put_hostname(This,bstrHostName)	\
    (This)->lpVtbl -> put_hostname(This,bstrHostName)

#define IPIEHTMLLocation_get_hostname(This,pbstrHostName)	\
    (This)->lpVtbl -> get_hostname(This,pbstrHostName)

#define IPIEHTMLLocation_put_port(This,bstrPort)	\
    (This)->lpVtbl -> put_port(This,bstrPort)

#define IPIEHTMLLocation_get_port(This,pbstrPort)	\
    (This)->lpVtbl -> get_port(This,pbstrPort)

#define IPIEHTMLLocation_put_pathname(This,bstrPathName)	\
    (This)->lpVtbl -> put_pathname(This,bstrPathName)

#define IPIEHTMLLocation_get_pathname(This,pbstrPathName)	\
    (This)->lpVtbl -> get_pathname(This,pbstrPathName)

#define IPIEHTMLLocation_put_search(This,bstrSearch)	\
    (This)->lpVtbl -> put_search(This,bstrSearch)

#define IPIEHTMLLocation_get_search(This,pbstrSearch)	\
    (This)->lpVtbl -> get_search(This,pbstrSearch)

#define IPIEHTMLLocation_put_hash(This,bstrHash)	\
    (This)->lpVtbl -> put_hash(This,bstrHash)

#define IPIEHTMLLocation_get_hash(This,pbstrHash)	\
    (This)->lpVtbl -> get_hash(This,pbstrHash)

#define IPIEHTMLLocation_reload(This,vbReloadSource)	\
    (This)->lpVtbl -> reload(This,vbReloadSource)

#define IPIEHTMLLocation_replace(This,bstrURL)	\
    (This)->lpVtbl -> replace(This,bstrURL)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_href_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrHREF);


void __RPC_STUB IPIEHTMLLocation_put_href_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_href_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHREF);


void __RPC_STUB IPIEHTMLLocation_get_href_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_protocol_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrProtocol);


void __RPC_STUB IPIEHTMLLocation_put_protocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_protocol_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrProtocol);


void __RPC_STUB IPIEHTMLLocation_get_protocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_host_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrHost);


void __RPC_STUB IPIEHTMLLocation_put_host_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_host_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHost);


void __RPC_STUB IPIEHTMLLocation_get_host_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_hostname_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrHostName);


void __RPC_STUB IPIEHTMLLocation_put_hostname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_hostname_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHostName);


void __RPC_STUB IPIEHTMLLocation_get_hostname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_port_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrPort);


void __RPC_STUB IPIEHTMLLocation_put_port_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_port_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrPort);


void __RPC_STUB IPIEHTMLLocation_get_port_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_pathname_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrPathName);


void __RPC_STUB IPIEHTMLLocation_put_pathname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_pathname_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrPathName);


void __RPC_STUB IPIEHTMLLocation_get_pathname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_search_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrSearch);


void __RPC_STUB IPIEHTMLLocation_put_search_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_search_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrSearch);


void __RPC_STUB IPIEHTMLLocation_get_search_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_put_hash_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrHash);


void __RPC_STUB IPIEHTMLLocation_put_hash_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_get_hash_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHash);


void __RPC_STUB IPIEHTMLLocation_get_hash_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_reload_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [defaultvalue][in] */ VARIANT_BOOL vbReloadSource);


void __RPC_STUB IPIEHTMLLocation_reload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLLocation_replace_Proxy( 
    IPIEHTMLLocation __RPC_FAR * This,
    /* [in] */ BSTR bstrURL);


void __RPC_STUB IPIEHTMLLocation_replace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLLocation_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLDocument_INTERFACE_DEFINED__
#define __IPIEHTMLDocument_INTERFACE_DEFINED__

/* interface IPIEHTMLDocument */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLDocument;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2101")
    IPIEHTMLDocument : public IDispatchEx
    {
    public:
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_toString( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_links( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_forms( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_anchors( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_frames( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_lastModified( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLastModified) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_referrer( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrReferrer) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_location( 
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_title( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTitle) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_URL( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrURL) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_bgColor( 
            /* [in] */ VARIANT vtBGColor) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_bgColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBGColor) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_fgColor( 
            /* [in] */ VARIANT vtFGColor) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fgColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtFGColor) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_linkColor( 
            /* [in] */ VARIANT vtLinkColor) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_linkColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtLinkColor) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_alinkColor( 
            /* [in] */ VARIANT vtALinkColor) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_alinkColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtALinkColor) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_vlinkColor( 
            /* [in] */ VARIANT vtVLinkColor) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_vlinkColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtVLinkColor) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_cookie( 
            /* [in] */ BSTR bstrCookie) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_cookie( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrCookie) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_domain( 
            /* [in] */ BSTR bstrDomain) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_domain( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDomain) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE open( 
            /* [defaultvalue][in] */ BSTR bstrMimeType,
            /* [optional][in] */ VARIANT vtReplace) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE clear( void) = 0;
        
        virtual /* [id][vararg] */ HRESULT STDMETHODCALLTYPE write( 
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite) = 0;
        
        virtual /* [id][vararg] */ HRESULT STDMETHODCALLTYPE writeln( 
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLDocumentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLDocument __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLDocument __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_links )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_forms )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_anchors )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_frames )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastModified )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLastModified);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_referrer )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrReferrer);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_location )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_title )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTitle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_URL )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrURL);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bgColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ VARIANT vtBGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bgColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fgColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ VARIANT vtFGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fgColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtFGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_linkColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ VARIANT vtLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_linkColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_alinkColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ VARIANT vtALinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_alinkColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtALinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vlinkColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ VARIANT vtVLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vlinkColor )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtVLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_cookie )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ BSTR bstrCookie);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cookie )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrCookie);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_domain )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ BSTR bstrDomain);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_domain )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDomain);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMimeType,
            /* [optional][in] */ VARIANT vtReplace);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IPIEHTMLDocument __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *clear )( 
            IPIEHTMLDocument __RPC_FAR * This);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *write )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *writeln )( 
            IPIEHTMLDocument __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        END_INTERFACE
    } IPIEHTMLDocumentVtbl;

    interface IPIEHTMLDocument
    {
        CONST_VTBL struct IPIEHTMLDocumentVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLDocument_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLDocument_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLDocument_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLDocument_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLDocument_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLDocument_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLDocument_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLDocument_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLDocument_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLDocument_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLDocument_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLDocument_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLDocument_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLDocument_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLDocument_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLDocument_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLDocument_get_links(This,ppElemCollection)	\
    (This)->lpVtbl -> get_links(This,ppElemCollection)

#define IPIEHTMLDocument_get_forms(This,ppElemCollection)	\
    (This)->lpVtbl -> get_forms(This,ppElemCollection)

#define IPIEHTMLDocument_get_anchors(This,ppElemCollection)	\
    (This)->lpVtbl -> get_anchors(This,ppElemCollection)

#define IPIEHTMLDocument_get_frames(This,ppElemCollection)	\
    (This)->lpVtbl -> get_frames(This,ppElemCollection)

#define IPIEHTMLDocument_get_lastModified(This,pbstrLastModified)	\
    (This)->lpVtbl -> get_lastModified(This,pbstrLastModified)

#define IPIEHTMLDocument_get_referrer(This,pbstrReferrer)	\
    (This)->lpVtbl -> get_referrer(This,pbstrReferrer)

#define IPIEHTMLDocument_get_location(This,ppLocation)	\
    (This)->lpVtbl -> get_location(This,ppLocation)

#define IPIEHTMLDocument_get_title(This,pbstrTitle)	\
    (This)->lpVtbl -> get_title(This,pbstrTitle)

#define IPIEHTMLDocument_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IPIEHTMLDocument_put_bgColor(This,vtBGColor)	\
    (This)->lpVtbl -> put_bgColor(This,vtBGColor)

#define IPIEHTMLDocument_get_bgColor(This,pvtBGColor)	\
    (This)->lpVtbl -> get_bgColor(This,pvtBGColor)

#define IPIEHTMLDocument_put_fgColor(This,vtFGColor)	\
    (This)->lpVtbl -> put_fgColor(This,vtFGColor)

#define IPIEHTMLDocument_get_fgColor(This,pvtFGColor)	\
    (This)->lpVtbl -> get_fgColor(This,pvtFGColor)

#define IPIEHTMLDocument_put_linkColor(This,vtLinkColor)	\
    (This)->lpVtbl -> put_linkColor(This,vtLinkColor)

#define IPIEHTMLDocument_get_linkColor(This,pvtLinkColor)	\
    (This)->lpVtbl -> get_linkColor(This,pvtLinkColor)

#define IPIEHTMLDocument_put_alinkColor(This,vtALinkColor)	\
    (This)->lpVtbl -> put_alinkColor(This,vtALinkColor)

#define IPIEHTMLDocument_get_alinkColor(This,pvtALinkColor)	\
    (This)->lpVtbl -> get_alinkColor(This,pvtALinkColor)

#define IPIEHTMLDocument_put_vlinkColor(This,vtVLinkColor)	\
    (This)->lpVtbl -> put_vlinkColor(This,vtVLinkColor)

#define IPIEHTMLDocument_get_vlinkColor(This,pvtVLinkColor)	\
    (This)->lpVtbl -> get_vlinkColor(This,pvtVLinkColor)

#define IPIEHTMLDocument_put_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> put_cookie(This,bstrCookie)

#define IPIEHTMLDocument_get_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> get_cookie(This,bstrCookie)

#define IPIEHTMLDocument_put_domain(This,bstrDomain)	\
    (This)->lpVtbl -> put_domain(This,bstrDomain)

#define IPIEHTMLDocument_get_domain(This,pbstrDomain)	\
    (This)->lpVtbl -> get_domain(This,pbstrDomain)

#define IPIEHTMLDocument_open(This,bstrMimeType,vtReplace)	\
    (This)->lpVtbl -> open(This,bstrMimeType,vtReplace)

#define IPIEHTMLDocument_close(This)	\
    (This)->lpVtbl -> close(This)

#define IPIEHTMLDocument_clear(This)	\
    (This)->lpVtbl -> clear(This)

#define IPIEHTMLDocument_write(This,psaWrite)	\
    (This)->lpVtbl -> write(This,psaWrite)

#define IPIEHTMLDocument_writeln(This,psaWrite)	\
    (This)->lpVtbl -> writeln(This,psaWrite)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_toString_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrString);


void __RPC_STUB IPIEHTMLDocument_get_toString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_links_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLDocument_get_links_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_forms_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLDocument_get_forms_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_anchors_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLDocument_get_anchors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_frames_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLDocument_get_frames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_lastModified_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrLastModified);


void __RPC_STUB IPIEHTMLDocument_get_lastModified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_referrer_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrReferrer);


void __RPC_STUB IPIEHTMLDocument_get_referrer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_location_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);


void __RPC_STUB IPIEHTMLDocument_get_location_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_title_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrTitle);


void __RPC_STUB IPIEHTMLDocument_get_title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_URL_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrURL);


void __RPC_STUB IPIEHTMLDocument_get_URL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_put_bgColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ VARIANT vtBGColor);


void __RPC_STUB IPIEHTMLDocument_put_bgColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_bgColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtBGColor);


void __RPC_STUB IPIEHTMLDocument_get_bgColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_put_fgColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ VARIANT vtFGColor);


void __RPC_STUB IPIEHTMLDocument_put_fgColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_fgColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtFGColor);


void __RPC_STUB IPIEHTMLDocument_get_fgColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_put_linkColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ VARIANT vtLinkColor);


void __RPC_STUB IPIEHTMLDocument_put_linkColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_linkColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtLinkColor);


void __RPC_STUB IPIEHTMLDocument_get_linkColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_put_alinkColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ VARIANT vtALinkColor);


void __RPC_STUB IPIEHTMLDocument_put_alinkColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_alinkColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtALinkColor);


void __RPC_STUB IPIEHTMLDocument_get_alinkColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_put_vlinkColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ VARIANT vtVLinkColor);


void __RPC_STUB IPIEHTMLDocument_put_vlinkColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_vlinkColor_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtVLinkColor);


void __RPC_STUB IPIEHTMLDocument_get_vlinkColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_put_cookie_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ BSTR bstrCookie);


void __RPC_STUB IPIEHTMLDocument_put_cookie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_cookie_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrCookie);


void __RPC_STUB IPIEHTMLDocument_get_cookie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_put_domain_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ BSTR bstrDomain);


void __RPC_STUB IPIEHTMLDocument_put_domain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_get_domain_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrDomain);


void __RPC_STUB IPIEHTMLDocument_get_domain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_open_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [defaultvalue][in] */ BSTR bstrMimeType,
    /* [optional][in] */ VARIANT vtReplace);


void __RPC_STUB IPIEHTMLDocument_open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_close_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This);


void __RPC_STUB IPIEHTMLDocument_close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_clear_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This);


void __RPC_STUB IPIEHTMLDocument_clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][vararg] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_write_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);


void __RPC_STUB IPIEHTMLDocument_write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][vararg] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument_writeln_Proxy( 
    IPIEHTMLDocument __RPC_FAR * This,
    /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);


void __RPC_STUB IPIEHTMLDocument_writeln_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLDocument_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLWindow_INTERFACE_DEFINED__
#define __IPIEHTMLWindow_INTERFACE_DEFINED__

/* interface IPIEHTMLWindow */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2102")
    IPIEHTMLWindow : public IDispatchEx
    {
    public:
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_toString( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_location( 
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_history( 
            /* [retval][out] */ IPIEOmHistory __RPC_FAR *__RPC_FAR *ppOmHistory) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_navigator( 
            /* [retval][out] */ IPIEOmNavigator __RPC_FAR *__RPC_FAR *ppOmNavigator) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_document( 
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parent( 
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowParent) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_self( 
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowSelf) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_top( 
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowTop) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_window( 
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindow) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_frames( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_screen( 
            /* [retval][out] */ IPIEHTMLScreen __RPC_FAR *__RPC_FAR *ppScreen) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *plLength) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_name( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE setTimeout( 
            /* [in] */ BSTR bstrExpression,
            /* [in] */ long lMS,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
            /* [retval][out] */ long __RPC_FAR *plTimerID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE clearTimeout( 
            /* [in] */ long lTimerID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE alert( 
            /* [defaultvalue][in] */ BSTR bstrMessage = L"") = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE confirm( 
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbConfirmed) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE prompt( 
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [defaultvalue][in] */ BSTR bstrDefaultValue,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE focus( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE blur( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE navigate( 
            /* [in] */ BSTR bstrURL) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE open( 
            /* [defaultvalue][in] */ BSTR bstrURL,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtName,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtFeatures,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtReplace,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowResult) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE scroll( 
            /* [in] */ long lX,
            /* [in] */ long lY) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLWindow __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLWindow __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_location )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_history )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEOmHistory __RPC_FAR *__RPC_FAR *ppOmHistory);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_navigator )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEOmNavigator __RPC_FAR *__RPC_FAR *ppOmNavigator);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parent )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_self )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowSelf);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_top )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowTop);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_window )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindow);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_frames )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_screen )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLScreen __RPC_FAR *__RPC_FAR *ppScreen);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLength);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_name )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setTimeout )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ BSTR bstrExpression,
            /* [in] */ long lMS,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
            /* [retval][out] */ long __RPC_FAR *plTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *clearTimeout )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ long lTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alert )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *confirm )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbConfirmed);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *prompt )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [defaultvalue][in] */ BSTR bstrDefaultValue,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IPIEHTMLWindow __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLWindow __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLWindow __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *navigate )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ BSTR bstrURL);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrURL,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtName,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtFeatures,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtReplace,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowResult);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *scroll )( 
            IPIEHTMLWindow __RPC_FAR * This,
            /* [in] */ long lX,
            /* [in] */ long lY);
        
        END_INTERFACE
    } IPIEHTMLWindowVtbl;

    interface IPIEHTMLWindow
    {
        CONST_VTBL struct IPIEHTMLWindowVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLWindow_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLWindow_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLWindow_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLWindow_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLWindow_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLWindow_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLWindow_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLWindow_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLWindow_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLWindow_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLWindow_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLWindow_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLWindow_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLWindow_get_location(This,ppLocation)	\
    (This)->lpVtbl -> get_location(This,ppLocation)

#define IPIEHTMLWindow_get_history(This,ppOmHistory)	\
    (This)->lpVtbl -> get_history(This,ppOmHistory)

#define IPIEHTMLWindow_get_navigator(This,ppOmNavigator)	\
    (This)->lpVtbl -> get_navigator(This,ppOmNavigator)

#define IPIEHTMLWindow_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLWindow_get_parent(This,ppWindowParent)	\
    (This)->lpVtbl -> get_parent(This,ppWindowParent)

#define IPIEHTMLWindow_get_self(This,ppWindowSelf)	\
    (This)->lpVtbl -> get_self(This,ppWindowSelf)

#define IPIEHTMLWindow_get_top(This,ppWindowTop)	\
    (This)->lpVtbl -> get_top(This,ppWindowTop)

#define IPIEHTMLWindow_get_window(This,ppWindow)	\
    (This)->lpVtbl -> get_window(This,ppWindow)

#define IPIEHTMLWindow_get_frames(This,ppElemCollection)	\
    (This)->lpVtbl -> get_frames(This,ppElemCollection)

#define IPIEHTMLWindow_get_screen(This,ppScreen)	\
    (This)->lpVtbl -> get_screen(This,ppScreen)

#define IPIEHTMLWindow_get_length(This,plLength)	\
    (This)->lpVtbl -> get_length(This,plLength)

#define IPIEHTMLWindow_put_name(This,bstrName)	\
    (This)->lpVtbl -> put_name(This,bstrName)

#define IPIEHTMLWindow_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLWindow_setTimeout(This,bstrExpression,lMS,pvtLanguage,plTimerID)	\
    (This)->lpVtbl -> setTimeout(This,bstrExpression,lMS,pvtLanguage,plTimerID)

#define IPIEHTMLWindow_clearTimeout(This,lTimerID)	\
    (This)->lpVtbl -> clearTimeout(This,lTimerID)

#define IPIEHTMLWindow_alert(This,bstrMessage)	\
    (This)->lpVtbl -> alert(This,bstrMessage)

#define IPIEHTMLWindow_confirm(This,bstrMessage,pvbConfirmed)	\
    (This)->lpVtbl -> confirm(This,bstrMessage,pvbConfirmed)

#define IPIEHTMLWindow_prompt(This,bstrMessage,bstrDefaultValue,pvtText)	\
    (This)->lpVtbl -> prompt(This,bstrMessage,bstrDefaultValue,pvtText)

#define IPIEHTMLWindow_close(This)	\
    (This)->lpVtbl -> close(This)

#define IPIEHTMLWindow_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLWindow_blur(This)	\
    (This)->lpVtbl -> blur(This)

#define IPIEHTMLWindow_navigate(This,bstrURL)	\
    (This)->lpVtbl -> navigate(This,bstrURL)

#define IPIEHTMLWindow_open(This,bstrURL,pvtName,pvtFeatures,pvtReplace,ppWindowResult)	\
    (This)->lpVtbl -> open(This,bstrURL,pvtName,pvtFeatures,pvtReplace,ppWindowResult)

#define IPIEHTMLWindow_scroll(This,lX,lY)	\
    (This)->lpVtbl -> scroll(This,lX,lY)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_toString_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrString);


void __RPC_STUB IPIEHTMLWindow_get_toString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_location_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);


void __RPC_STUB IPIEHTMLWindow_get_location_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_history_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEOmHistory __RPC_FAR *__RPC_FAR *ppOmHistory);


void __RPC_STUB IPIEHTMLWindow_get_history_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_navigator_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEOmNavigator __RPC_FAR *__RPC_FAR *ppOmNavigator);


void __RPC_STUB IPIEHTMLWindow_get_navigator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_document_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);


void __RPC_STUB IPIEHTMLWindow_get_document_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_parent_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowParent);


void __RPC_STUB IPIEHTMLWindow_get_parent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_self_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowSelf);


void __RPC_STUB IPIEHTMLWindow_get_self_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_top_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowTop);


void __RPC_STUB IPIEHTMLWindow_get_top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_window_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindow);


void __RPC_STUB IPIEHTMLWindow_get_window_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_frames_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLWindow_get_frames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_screen_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLScreen __RPC_FAR *__RPC_FAR *ppScreen);


void __RPC_STUB IPIEHTMLWindow_get_screen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_length_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLength);


void __RPC_STUB IPIEHTMLWindow_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_put_name_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IPIEHTMLWindow_put_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_get_name_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IPIEHTMLWindow_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_setTimeout_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [in] */ BSTR bstrExpression,
    /* [in] */ long lMS,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
    /* [retval][out] */ long __RPC_FAR *plTimerID);


void __RPC_STUB IPIEHTMLWindow_setTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_clearTimeout_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [in] */ long lTimerID);


void __RPC_STUB IPIEHTMLWindow_clearTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_alert_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [defaultvalue][in] */ BSTR bstrMessage);


void __RPC_STUB IPIEHTMLWindow_alert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_confirm_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [defaultvalue][in] */ BSTR bstrMessage,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbConfirmed);


void __RPC_STUB IPIEHTMLWindow_confirm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_prompt_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [defaultvalue][in] */ BSTR bstrMessage,
    /* [defaultvalue][in] */ BSTR bstrDefaultValue,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtText);


void __RPC_STUB IPIEHTMLWindow_prompt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_close_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This);


void __RPC_STUB IPIEHTMLWindow_close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_focus_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This);


void __RPC_STUB IPIEHTMLWindow_focus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_blur_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This);


void __RPC_STUB IPIEHTMLWindow_blur_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_navigate_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [in] */ BSTR bstrURL);


void __RPC_STUB IPIEHTMLWindow_navigate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_open_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [defaultvalue][in] */ BSTR bstrURL,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtName,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtFeatures,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtReplace,
    /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowResult);


void __RPC_STUB IPIEHTMLWindow_open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow_scroll_Proxy( 
    IPIEHTMLWindow __RPC_FAR * This,
    /* [in] */ long lX,
    /* [in] */ long lY);


void __RPC_STUB IPIEHTMLWindow_scroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLWindow_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLWindow2_INTERFACE_DEFINED__
#define __IPIEHTMLWindow2_INTERFACE_DEFINED__

/* interface IPIEHTMLWindow2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLWindow2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE21F2")
    IPIEHTMLWindow2 : public IPIEHTMLWindow
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onload( 
            /* [in] */ VARIANT vtOnLoad) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onload( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnLoad) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onunload( 
            /* [in] */ VARIANT vtOnUnload) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onunload( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvOnUnload) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onerror( 
            /* [in] */ VARIANT vtOnError) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onerror( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnError) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Image( 
            /* [retval][out] */ IPIEHTMLImageElementFactory __RPC_FAR *__RPC_FAR *ppImageFactory) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Option( 
            /* [retval][out] */ IPIEHTMLOptionElementFactory __RPC_FAR *__RPC_FAR *ppOptionFactory) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE setTimeout( 
            /* [in] */ VARIANT __RPC_FAR *pvtExpression,
            /* [in] */ long lMS,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
            /* [retval][out] */ long __RPC_FAR *plTimerID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE scrollTo( 
            /* [in] */ long lX,
            /* [in] */ long lY) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLWindow2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLWindow2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLWindow2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_location )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_history )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEOmHistory __RPC_FAR *__RPC_FAR *ppOmHistory);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_navigator )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEOmNavigator __RPC_FAR *__RPC_FAR *ppOmNavigator);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parent )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_self )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowSelf);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_top )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowTop);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_window )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindow);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_frames )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_screen )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLScreen __RPC_FAR *__RPC_FAR *ppScreen);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLength);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_name )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setTimeout )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ BSTR bstrExpression,
            /* [in] */ long lMS,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
            /* [retval][out] */ long __RPC_FAR *plTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *clearTimeout )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ long lTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alert )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *confirm )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbConfirmed);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *prompt )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [defaultvalue][in] */ BSTR bstrDefaultValue,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IPIEHTMLWindow2 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLWindow2 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLWindow2 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *navigate )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ BSTR bstrURL);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrURL,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtName,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtFeatures,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtReplace,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowResult);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *scroll )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ long lX,
            /* [in] */ long lY);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onload )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ VARIANT vtOnLoad);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onload )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnLoad);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onunload )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ VARIANT vtOnUnload);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onunload )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvOnUnload);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onerror )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ VARIANT vtOnError);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onerror )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnError);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Image )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLImageElementFactory __RPC_FAR *__RPC_FAR *ppImageFactory);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Option )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLOptionElementFactory __RPC_FAR *__RPC_FAR *ppOptionFactory);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setTimeout )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *pvtExpression,
            /* [in] */ long lMS,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
            /* [retval][out] */ long __RPC_FAR *plTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *scrollTo )( 
            IPIEHTMLWindow2 __RPC_FAR * This,
            /* [in] */ long lX,
            /* [in] */ long lY);
        
        END_INTERFACE
    } IPIEHTMLWindow2Vtbl;

    interface IPIEHTMLWindow2
    {
        CONST_VTBL struct IPIEHTMLWindow2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLWindow2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLWindow2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLWindow2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLWindow2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLWindow2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLWindow2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLWindow2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLWindow2_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLWindow2_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLWindow2_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLWindow2_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLWindow2_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLWindow2_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLWindow2_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLWindow2_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLWindow2_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLWindow2_get_location(This,ppLocation)	\
    (This)->lpVtbl -> get_location(This,ppLocation)

#define IPIEHTMLWindow2_get_history(This,ppOmHistory)	\
    (This)->lpVtbl -> get_history(This,ppOmHistory)

#define IPIEHTMLWindow2_get_navigator(This,ppOmNavigator)	\
    (This)->lpVtbl -> get_navigator(This,ppOmNavigator)

#define IPIEHTMLWindow2_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLWindow2_get_parent(This,ppWindowParent)	\
    (This)->lpVtbl -> get_parent(This,ppWindowParent)

#define IPIEHTMLWindow2_get_self(This,ppWindowSelf)	\
    (This)->lpVtbl -> get_self(This,ppWindowSelf)

#define IPIEHTMLWindow2_get_top(This,ppWindowTop)	\
    (This)->lpVtbl -> get_top(This,ppWindowTop)

#define IPIEHTMLWindow2_get_window(This,ppWindow)	\
    (This)->lpVtbl -> get_window(This,ppWindow)

#define IPIEHTMLWindow2_get_frames(This,ppElemCollection)	\
    (This)->lpVtbl -> get_frames(This,ppElemCollection)

#define IPIEHTMLWindow2_get_screen(This,ppScreen)	\
    (This)->lpVtbl -> get_screen(This,ppScreen)

#define IPIEHTMLWindow2_get_length(This,plLength)	\
    (This)->lpVtbl -> get_length(This,plLength)

#define IPIEHTMLWindow2_put_name(This,bstrName)	\
    (This)->lpVtbl -> put_name(This,bstrName)

#define IPIEHTMLWindow2_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLWindow2_setTimeout(This,bstrExpression,lMS,pvtLanguage,plTimerID)	\
    (This)->lpVtbl -> setTimeout(This,bstrExpression,lMS,pvtLanguage,plTimerID)

#define IPIEHTMLWindow2_clearTimeout(This,lTimerID)	\
    (This)->lpVtbl -> clearTimeout(This,lTimerID)

#define IPIEHTMLWindow2_alert(This,bstrMessage)	\
    (This)->lpVtbl -> alert(This,bstrMessage)

#define IPIEHTMLWindow2_confirm(This,bstrMessage,pvbConfirmed)	\
    (This)->lpVtbl -> confirm(This,bstrMessage,pvbConfirmed)

#define IPIEHTMLWindow2_prompt(This,bstrMessage,bstrDefaultValue,pvtText)	\
    (This)->lpVtbl -> prompt(This,bstrMessage,bstrDefaultValue,pvtText)

#define IPIEHTMLWindow2_close(This)	\
    (This)->lpVtbl -> close(This)

#define IPIEHTMLWindow2_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLWindow2_blur(This)	\
    (This)->lpVtbl -> blur(This)

#define IPIEHTMLWindow2_navigate(This,bstrURL)	\
    (This)->lpVtbl -> navigate(This,bstrURL)

#define IPIEHTMLWindow2_open(This,bstrURL,pvtName,pvtFeatures,pvtReplace,ppWindowResult)	\
    (This)->lpVtbl -> open(This,bstrURL,pvtName,pvtFeatures,pvtReplace,ppWindowResult)

#define IPIEHTMLWindow2_scroll(This,lX,lY)	\
    (This)->lpVtbl -> scroll(This,lX,lY)


#define IPIEHTMLWindow2_put_onload(This,vtOnLoad)	\
    (This)->lpVtbl -> put_onload(This,vtOnLoad)

#define IPIEHTMLWindow2_get_onload(This,pvtOnLoad)	\
    (This)->lpVtbl -> get_onload(This,pvtOnLoad)

#define IPIEHTMLWindow2_put_onunload(This,vtOnUnload)	\
    (This)->lpVtbl -> put_onunload(This,vtOnUnload)

#define IPIEHTMLWindow2_get_onunload(This,pvOnUnload)	\
    (This)->lpVtbl -> get_onunload(This,pvOnUnload)

#define IPIEHTMLWindow2_put_onerror(This,vtOnError)	\
    (This)->lpVtbl -> put_onerror(This,vtOnError)

#define IPIEHTMLWindow2_get_onerror(This,pvtOnError)	\
    (This)->lpVtbl -> get_onerror(This,pvtOnError)

#define IPIEHTMLWindow2_get_Image(This,ppImageFactory)	\
    (This)->lpVtbl -> get_Image(This,ppImageFactory)

#define IPIEHTMLWindow2_get_Option(This,ppOptionFactory)	\
    (This)->lpVtbl -> get_Option(This,ppOptionFactory)

#define IPIEHTMLWindow2_setTimeout(This,pvtExpression,lMS,pvtLanguage,plTimerID)	\
    (This)->lpVtbl -> setTimeout(This,pvtExpression,lMS,pvtLanguage,plTimerID)

#define IPIEHTMLWindow2_scrollTo(This,lX,lY)	\
    (This)->lpVtbl -> scrollTo(This,lX,lY)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_put_onload_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [in] */ VARIANT vtOnLoad);


void __RPC_STUB IPIEHTMLWindow2_put_onload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_get_onload_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtOnLoad);


void __RPC_STUB IPIEHTMLWindow2_get_onload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_put_onunload_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [in] */ VARIANT vtOnUnload);


void __RPC_STUB IPIEHTMLWindow2_put_onunload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_get_onunload_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvOnUnload);


void __RPC_STUB IPIEHTMLWindow2_get_onunload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_put_onerror_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [in] */ VARIANT vtOnError);


void __RPC_STUB IPIEHTMLWindow2_put_onerror_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_get_onerror_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtOnError);


void __RPC_STUB IPIEHTMLWindow2_get_onerror_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_get_Image_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLImageElementFactory __RPC_FAR *__RPC_FAR *ppImageFactory);


void __RPC_STUB IPIEHTMLWindow2_get_Image_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_get_Option_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLOptionElementFactory __RPC_FAR *__RPC_FAR *ppOptionFactory);


void __RPC_STUB IPIEHTMLWindow2_get_Option_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_setTimeout_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *pvtExpression,
    /* [in] */ long lMS,
    /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
    /* [retval][out] */ long __RPC_FAR *plTimerID);


void __RPC_STUB IPIEHTMLWindow2_setTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow2_scrollTo_Proxy( 
    IPIEHTMLWindow2 __RPC_FAR * This,
    /* [in] */ long lX,
    /* [in] */ long lY);


void __RPC_STUB IPIEHTMLWindow2_scrollTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLWindow2_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLDocument2_INTERFACE_DEFINED__
#define __IPIEHTMLDocument2_INTERFACE_DEFINED__

/* interface IPIEHTMLDocument2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLDocument2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE21F1")
    IPIEHTMLDocument2 : public IPIEHTMLDocument
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_images( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_all( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parentWindow( 
            /* [retval][out] */ IPIEHTMLWindow2 __RPC_FAR *__RPC_FAR *ppWindow) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLDocument2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLDocument2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLDocument2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_links )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_forms )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_anchors )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_frames )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastModified )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLastModified);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_referrer )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrReferrer);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_location )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_title )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTitle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_URL )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrURL);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bgColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ VARIANT vtBGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bgColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fgColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ VARIANT vtFGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fgColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtFGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_linkColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ VARIANT vtLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_linkColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_alinkColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ VARIANT vtALinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_alinkColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtALinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vlinkColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ VARIANT vtVLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vlinkColor )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtVLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_cookie )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ BSTR bstrCookie);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cookie )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrCookie);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_domain )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ BSTR bstrDomain);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_domain )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDomain);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMimeType,
            /* [optional][in] */ VARIANT vtReplace);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IPIEHTMLDocument2 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *clear )( 
            IPIEHTMLDocument2 __RPC_FAR * This);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *write )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *writeln )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_images )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_all )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentWindow )( 
            IPIEHTMLDocument2 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow2 __RPC_FAR *__RPC_FAR *ppWindow);
        
        END_INTERFACE
    } IPIEHTMLDocument2Vtbl;

    interface IPIEHTMLDocument2
    {
        CONST_VTBL struct IPIEHTMLDocument2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLDocument2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLDocument2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLDocument2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLDocument2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLDocument2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLDocument2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLDocument2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLDocument2_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLDocument2_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLDocument2_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLDocument2_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLDocument2_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLDocument2_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLDocument2_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLDocument2_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLDocument2_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLDocument2_get_links(This,ppElemCollection)	\
    (This)->lpVtbl -> get_links(This,ppElemCollection)

#define IPIEHTMLDocument2_get_forms(This,ppElemCollection)	\
    (This)->lpVtbl -> get_forms(This,ppElemCollection)

#define IPIEHTMLDocument2_get_anchors(This,ppElemCollection)	\
    (This)->lpVtbl -> get_anchors(This,ppElemCollection)

#define IPIEHTMLDocument2_get_frames(This,ppElemCollection)	\
    (This)->lpVtbl -> get_frames(This,ppElemCollection)

#define IPIEHTMLDocument2_get_lastModified(This,pbstrLastModified)	\
    (This)->lpVtbl -> get_lastModified(This,pbstrLastModified)

#define IPIEHTMLDocument2_get_referrer(This,pbstrReferrer)	\
    (This)->lpVtbl -> get_referrer(This,pbstrReferrer)

#define IPIEHTMLDocument2_get_location(This,ppLocation)	\
    (This)->lpVtbl -> get_location(This,ppLocation)

#define IPIEHTMLDocument2_get_title(This,pbstrTitle)	\
    (This)->lpVtbl -> get_title(This,pbstrTitle)

#define IPIEHTMLDocument2_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IPIEHTMLDocument2_put_bgColor(This,vtBGColor)	\
    (This)->lpVtbl -> put_bgColor(This,vtBGColor)

#define IPIEHTMLDocument2_get_bgColor(This,pvtBGColor)	\
    (This)->lpVtbl -> get_bgColor(This,pvtBGColor)

#define IPIEHTMLDocument2_put_fgColor(This,vtFGColor)	\
    (This)->lpVtbl -> put_fgColor(This,vtFGColor)

#define IPIEHTMLDocument2_get_fgColor(This,pvtFGColor)	\
    (This)->lpVtbl -> get_fgColor(This,pvtFGColor)

#define IPIEHTMLDocument2_put_linkColor(This,vtLinkColor)	\
    (This)->lpVtbl -> put_linkColor(This,vtLinkColor)

#define IPIEHTMLDocument2_get_linkColor(This,pvtLinkColor)	\
    (This)->lpVtbl -> get_linkColor(This,pvtLinkColor)

#define IPIEHTMLDocument2_put_alinkColor(This,vtALinkColor)	\
    (This)->lpVtbl -> put_alinkColor(This,vtALinkColor)

#define IPIEHTMLDocument2_get_alinkColor(This,pvtALinkColor)	\
    (This)->lpVtbl -> get_alinkColor(This,pvtALinkColor)

#define IPIEHTMLDocument2_put_vlinkColor(This,vtVLinkColor)	\
    (This)->lpVtbl -> put_vlinkColor(This,vtVLinkColor)

#define IPIEHTMLDocument2_get_vlinkColor(This,pvtVLinkColor)	\
    (This)->lpVtbl -> get_vlinkColor(This,pvtVLinkColor)

#define IPIEHTMLDocument2_put_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> put_cookie(This,bstrCookie)

#define IPIEHTMLDocument2_get_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> get_cookie(This,bstrCookie)

#define IPIEHTMLDocument2_put_domain(This,bstrDomain)	\
    (This)->lpVtbl -> put_domain(This,bstrDomain)

#define IPIEHTMLDocument2_get_domain(This,pbstrDomain)	\
    (This)->lpVtbl -> get_domain(This,pbstrDomain)

#define IPIEHTMLDocument2_open(This,bstrMimeType,vtReplace)	\
    (This)->lpVtbl -> open(This,bstrMimeType,vtReplace)

#define IPIEHTMLDocument2_close(This)	\
    (This)->lpVtbl -> close(This)

#define IPIEHTMLDocument2_clear(This)	\
    (This)->lpVtbl -> clear(This)

#define IPIEHTMLDocument2_write(This,psaWrite)	\
    (This)->lpVtbl -> write(This,psaWrite)

#define IPIEHTMLDocument2_writeln(This,psaWrite)	\
    (This)->lpVtbl -> writeln(This,psaWrite)


#define IPIEHTMLDocument2_get_images(This,ppElemCollection)	\
    (This)->lpVtbl -> get_images(This,ppElemCollection)

#define IPIEHTMLDocument2_get_all(This,ppElemCollection)	\
    (This)->lpVtbl -> get_all(This,ppElemCollection)

#define IPIEHTMLDocument2_get_parentWindow(This,ppWindow)	\
    (This)->lpVtbl -> get_parentWindow(This,ppWindow)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument2_get_images_Proxy( 
    IPIEHTMLDocument2 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLDocument2_get_images_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument2_get_all_Proxy( 
    IPIEHTMLDocument2 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLDocument2_get_all_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument2_get_parentWindow_Proxy( 
    IPIEHTMLDocument2 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLWindow2 __RPC_FAR *__RPC_FAR *ppWindow);


void __RPC_STUB IPIEHTMLDocument2_get_parentWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLDocument2_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLElement_INTERFACE_DEFINED__
#define __IPIEHTMLElement_INTERFACE_DEFINED__

/* interface IPIEHTMLElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2201")
    IPIEHTMLElement : public IDispatchEx
    {
    public:
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_toString( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_id( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_document( 
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_innerHTML( 
            /* [in] */ BSTR bstrInnerHTML) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_innerHTML( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_innerText( 
            /* [in] */ BSTR bstrInnerText) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_innerText( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parentElement( 
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_children( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_style( 
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_currentStyle( 
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE addBehavior( 
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE removeBehavior( 
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        END_INTERFACE
    } IPIEHTMLElementVtbl;

    interface IPIEHTMLElement
    {
        CONST_VTBL struct IPIEHTMLElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_toString_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrString);


void __RPC_STUB IPIEHTMLElement_get_toString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_id_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrID);


void __RPC_STUB IPIEHTMLElement_get_id_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_document_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);


void __RPC_STUB IPIEHTMLElement_get_document_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_put_innerHTML_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [in] */ BSTR bstrInnerHTML);


void __RPC_STUB IPIEHTMLElement_put_innerHTML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_innerHTML_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);


void __RPC_STUB IPIEHTMLElement_get_innerHTML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_put_innerText_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [in] */ BSTR bstrInnerText);


void __RPC_STUB IPIEHTMLElement_put_innerText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_innerText_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);


void __RPC_STUB IPIEHTMLElement_get_innerText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_parentElement_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);


void __RPC_STUB IPIEHTMLElement_get_parentElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_children_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);


void __RPC_STUB IPIEHTMLElement_get_children_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_style_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);


void __RPC_STUB IPIEHTMLElement_get_style_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_get_currentStyle_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);


void __RPC_STUB IPIEHTMLElement_get_currentStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_addBehavior_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
    /* [retval][out] */ long __RPC_FAR *plID);


void __RPC_STUB IPIEHTMLElement_addBehavior_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement_removeBehavior_Proxy( 
    IPIEHTMLElement __RPC_FAR * This,
    /* [in] */ long lID,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);


void __RPC_STUB IPIEHTMLElement_removeBehavior_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLElement_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLImgElement_INTERFACE_DEFINED__
#define __IPIEHTMLImgElement_INTERFACE_DEFINED__

/* interface IPIEHTMLImgElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLImgElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2204")
    IPIEHTMLImgElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_border( 
            /* [in] */ VARIANT vtBorder) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_border( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBorder) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_vspace( 
            /* [retval][out] */ long __RPC_FAR *plVSpace) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_hspace( 
            /* [retval][out] */ long __RPC_FAR *plHSpace) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_alt( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAlt) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_src( 
            /* [in] */ BSTR bstrSrc) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_src( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSrc) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_width( 
            /* [in] */ long lWidth) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_width( 
            /* [retval][out] */ long __RPC_FAR *plWidth) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_height( 
            /* [in] */ long lHeight) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_height( 
            /* [retval][out] */ long __RPC_FAR *plHeight) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_onload( 
            /* [in] */ VARIANT vtOnLoad) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_onload( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnLoad) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLImgElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLImgElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLImgElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_border )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ VARIANT vtBorder);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_border )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBorder);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vspace )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVSpace);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hspace )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plHSpace);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_alt )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAlt);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_src )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ BSTR bstrSrc);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_src )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSrc);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_width )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ long lWidth);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_width )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plWidth);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_height )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ long lHeight);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_height )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plHeight);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onload )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [in] */ VARIANT vtOnLoad);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onload )( 
            IPIEHTMLImgElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnLoad);
        
        END_INTERFACE
    } IPIEHTMLImgElementVtbl;

    interface IPIEHTMLImgElement
    {
        CONST_VTBL struct IPIEHTMLImgElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLImgElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLImgElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLImgElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLImgElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLImgElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLImgElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLImgElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLImgElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLImgElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLImgElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLImgElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLImgElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLImgElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLImgElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLImgElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLImgElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLImgElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLImgElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLImgElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLImgElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLImgElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLImgElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLImgElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLImgElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLImgElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLImgElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLImgElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLImgElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLImgElement_put_border(This,vtBorder)	\
    (This)->lpVtbl -> put_border(This,vtBorder)

#define IPIEHTMLImgElement_get_border(This,pvtBorder)	\
    (This)->lpVtbl -> get_border(This,pvtBorder)

#define IPIEHTMLImgElement_get_vspace(This,plVSpace)	\
    (This)->lpVtbl -> get_vspace(This,plVSpace)

#define IPIEHTMLImgElement_get_hspace(This,plHSpace)	\
    (This)->lpVtbl -> get_hspace(This,plHSpace)

#define IPIEHTMLImgElement_get_alt(This,pbstrAlt)	\
    (This)->lpVtbl -> get_alt(This,pbstrAlt)

#define IPIEHTMLImgElement_put_src(This,bstrSrc)	\
    (This)->lpVtbl -> put_src(This,bstrSrc)

#define IPIEHTMLImgElement_get_src(This,pbstrSrc)	\
    (This)->lpVtbl -> get_src(This,pbstrSrc)

#define IPIEHTMLImgElement_put_width(This,lWidth)	\
    (This)->lpVtbl -> put_width(This,lWidth)

#define IPIEHTMLImgElement_get_width(This,plWidth)	\
    (This)->lpVtbl -> get_width(This,plWidth)

#define IPIEHTMLImgElement_put_height(This,lHeight)	\
    (This)->lpVtbl -> put_height(This,lHeight)

#define IPIEHTMLImgElement_get_height(This,plHeight)	\
    (This)->lpVtbl -> get_height(This,plHeight)

#define IPIEHTMLImgElement_put_onload(This,vtOnLoad)	\
    (This)->lpVtbl -> put_onload(This,vtOnLoad)

#define IPIEHTMLImgElement_get_onload(This,pvtOnLoad)	\
    (This)->lpVtbl -> get_onload(This,pvtOnLoad)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_put_border_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [in] */ VARIANT vtBorder);


void __RPC_STUB IPIEHTMLImgElement_put_border_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_border_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtBorder);


void __RPC_STUB IPIEHTMLImgElement_get_border_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_vspace_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVSpace);


void __RPC_STUB IPIEHTMLImgElement_get_vspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_hspace_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plHSpace);


void __RPC_STUB IPIEHTMLImgElement_get_hspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_alt_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrAlt);


void __RPC_STUB IPIEHTMLImgElement_get_alt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_put_src_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [in] */ BSTR bstrSrc);


void __RPC_STUB IPIEHTMLImgElement_put_src_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_src_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrSrc);


void __RPC_STUB IPIEHTMLImgElement_get_src_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_put_width_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [in] */ long lWidth);


void __RPC_STUB IPIEHTMLImgElement_put_width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_width_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plWidth);


void __RPC_STUB IPIEHTMLImgElement_get_width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_put_height_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [in] */ long lHeight);


void __RPC_STUB IPIEHTMLImgElement_put_height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_height_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plHeight);


void __RPC_STUB IPIEHTMLImgElement_get_height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_put_onload_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [in] */ VARIANT vtOnLoad);


void __RPC_STUB IPIEHTMLImgElement_put_onload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImgElement_get_onload_Proxy( 
    IPIEHTMLImgElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtOnLoad);


void __RPC_STUB IPIEHTMLImgElement_get_onload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLImgElement_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLDOMNode_INTERFACE_DEFINED__
#define __IPIEHTMLDOMNode_INTERFACE_DEFINED__

/* interface IPIEHTMLDOMNode */
/* [object][uuid] */ 


EXTERN_C const IID IID_IPIEHTMLDOMNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("680b64f4-48d1-42c3-8fa7-7bf4a83a115c")
    IPIEHTMLDOMNode : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLDOMNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLDOMNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLDOMNode __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLDOMNode __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLDOMNode __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLDOMNode __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLDOMNode __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLDOMNode __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IPIEHTMLDOMNodeVtbl;

    interface IPIEHTMLDOMNode
    {
        CONST_VTBL struct IPIEHTMLDOMNodeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLDOMNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLDOMNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLDOMNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLDOMNode_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLDOMNode_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLDOMNode_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLDOMNode_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPIEHTMLDOMNode_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLDocument3_INTERFACE_DEFINED__
#define __IPIEHTMLDocument3_INTERFACE_DEFINED__

/* interface IPIEHTMLDocument3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLDocument3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f1210a64-e50f-49e7-9e3b-81a25415a583")
    IPIEHTMLDocument3 : public IPIEHTMLDocument2
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_documentElement( 
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_uniqueID( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_onrowsdelete( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_onrowsdelete( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_onrowsinserted( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_onrowsinserted( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_oncellchange( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_oncellchange( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondatasetchanged( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondatasetchanged( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondataavailable( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondataavailable( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_ondatasetcomplete( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_ondatasetcomplete( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_onpropertychange( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_onpropertychange( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_dir( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_dir( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_oncontextmenu( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_oncontextmenu( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_onstop( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_onstop( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_parentDocument( 
            /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_enableDownload( 
            /* [in] */ VARIANT_BOOL v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_enableDownload( 
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_baseUrl( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_baseUrl( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_childNodes( 
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_inheritStyleSheets( 
            /* [in] */ VARIANT_BOOL v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_inheritStyleSheets( 
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE put_onbeforeeditfocus( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_onbeforeeditfocus( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE releaseCapture( void) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE recalc( 
            /* [in][defaultvalue] */ VARIANT_BOOL fForce = 0) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE createTextNode( 
            /* [in] */ BSTR text,
            /* [out][retval] */ IPIEHTMLDOMNode __RPC_FAR *__RPC_FAR *newTextNode) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE attachEvent( 
            /* [in] */ BSTR event,
            /* [in] */ IDispatch __RPC_FAR *pDisp,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *pfResult) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE detachEvent( 
            /* [in] */ BSTR event,
            /* [in] */ IDispatch __RPC_FAR *pDisp) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE createDocumentFragment( 
            /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *pNewDoc) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE getElementsByName( 
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getElementById( 
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *pel) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getElementsByTagName( 
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLDocument3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLDocument3 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLDocument3 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_links )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_forms )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_anchors )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_frames )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastModified )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLastModified);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_referrer )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrReferrer);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_location )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_title )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTitle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_URL )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrURL);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bgColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT vtBGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bgColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fgColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT vtFGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fgColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtFGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_linkColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT vtLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_linkColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_alinkColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT vtALinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_alinkColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtALinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vlinkColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT vtVLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vlinkColor )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtVLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_cookie )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR bstrCookie);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cookie )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrCookie);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_domain )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR bstrDomain);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_domain )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDomain);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMimeType,
            /* [optional][in] */ VARIANT vtReplace);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IPIEHTMLDocument3 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *clear )( 
            IPIEHTMLDocument3 __RPC_FAR * This);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *write )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *writeln )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_images )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_all )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentWindow )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow2 __RPC_FAR *__RPC_FAR *ppWindow);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_documentElement )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_uniqueID )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onrowsdelete )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onrowsdelete )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onrowsinserted )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onrowsinserted )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_oncellchange )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_oncellchange )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ondatasetchanged )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ondatasetchanged )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ondataavailable )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ondataavailable )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ondatasetcomplete )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ondatasetcomplete )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onpropertychange )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onpropertychange )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_dir )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_dir )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_oncontextmenu )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_oncontextmenu )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onstop )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onstop )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentDocument )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_enableDownload )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_enableDownload )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_baseUrl )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_baseUrl )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_childNodes )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_inheritStyleSheets )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_inheritStyleSheets )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onbeforeeditfocus )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onbeforeeditfocus )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *releaseCapture )( 
            IPIEHTMLDocument3 __RPC_FAR * This);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *recalc )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in][defaultvalue] */ VARIANT_BOOL fForce);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createTextNode )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR text,
            /* [out][retval] */ IPIEHTMLDOMNode __RPC_FAR *__RPC_FAR *newTextNode);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attachEvent )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR event,
            /* [in] */ IDispatch __RPC_FAR *pDisp,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *pfResult);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *detachEvent )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR event,
            /* [in] */ IDispatch __RPC_FAR *pDisp);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createDocumentFragment )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *pNewDoc);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getElementsByName )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getElementById )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *pel);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getElementsByTagName )( 
            IPIEHTMLDocument3 __RPC_FAR * This,
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl);
        
        END_INTERFACE
    } IPIEHTMLDocument3Vtbl;

    interface IPIEHTMLDocument3
    {
        CONST_VTBL struct IPIEHTMLDocument3Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLDocument3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLDocument3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLDocument3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLDocument3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLDocument3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLDocument3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLDocument3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLDocument3_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLDocument3_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLDocument3_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLDocument3_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLDocument3_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLDocument3_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLDocument3_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLDocument3_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLDocument3_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLDocument3_get_links(This,ppElemCollection)	\
    (This)->lpVtbl -> get_links(This,ppElemCollection)

#define IPIEHTMLDocument3_get_forms(This,ppElemCollection)	\
    (This)->lpVtbl -> get_forms(This,ppElemCollection)

#define IPIEHTMLDocument3_get_anchors(This,ppElemCollection)	\
    (This)->lpVtbl -> get_anchors(This,ppElemCollection)

#define IPIEHTMLDocument3_get_frames(This,ppElemCollection)	\
    (This)->lpVtbl -> get_frames(This,ppElemCollection)

#define IPIEHTMLDocument3_get_lastModified(This,pbstrLastModified)	\
    (This)->lpVtbl -> get_lastModified(This,pbstrLastModified)

#define IPIEHTMLDocument3_get_referrer(This,pbstrReferrer)	\
    (This)->lpVtbl -> get_referrer(This,pbstrReferrer)

#define IPIEHTMLDocument3_get_location(This,ppLocation)	\
    (This)->lpVtbl -> get_location(This,ppLocation)

#define IPIEHTMLDocument3_get_title(This,pbstrTitle)	\
    (This)->lpVtbl -> get_title(This,pbstrTitle)

#define IPIEHTMLDocument3_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IPIEHTMLDocument3_put_bgColor(This,vtBGColor)	\
    (This)->lpVtbl -> put_bgColor(This,vtBGColor)

#define IPIEHTMLDocument3_get_bgColor(This,pvtBGColor)	\
    (This)->lpVtbl -> get_bgColor(This,pvtBGColor)

#define IPIEHTMLDocument3_put_fgColor(This,vtFGColor)	\
    (This)->lpVtbl -> put_fgColor(This,vtFGColor)

#define IPIEHTMLDocument3_get_fgColor(This,pvtFGColor)	\
    (This)->lpVtbl -> get_fgColor(This,pvtFGColor)

#define IPIEHTMLDocument3_put_linkColor(This,vtLinkColor)	\
    (This)->lpVtbl -> put_linkColor(This,vtLinkColor)

#define IPIEHTMLDocument3_get_linkColor(This,pvtLinkColor)	\
    (This)->lpVtbl -> get_linkColor(This,pvtLinkColor)

#define IPIEHTMLDocument3_put_alinkColor(This,vtALinkColor)	\
    (This)->lpVtbl -> put_alinkColor(This,vtALinkColor)

#define IPIEHTMLDocument3_get_alinkColor(This,pvtALinkColor)	\
    (This)->lpVtbl -> get_alinkColor(This,pvtALinkColor)

#define IPIEHTMLDocument3_put_vlinkColor(This,vtVLinkColor)	\
    (This)->lpVtbl -> put_vlinkColor(This,vtVLinkColor)

#define IPIEHTMLDocument3_get_vlinkColor(This,pvtVLinkColor)	\
    (This)->lpVtbl -> get_vlinkColor(This,pvtVLinkColor)

#define IPIEHTMLDocument3_put_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> put_cookie(This,bstrCookie)

#define IPIEHTMLDocument3_get_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> get_cookie(This,bstrCookie)

#define IPIEHTMLDocument3_put_domain(This,bstrDomain)	\
    (This)->lpVtbl -> put_domain(This,bstrDomain)

#define IPIEHTMLDocument3_get_domain(This,pbstrDomain)	\
    (This)->lpVtbl -> get_domain(This,pbstrDomain)

#define IPIEHTMLDocument3_open(This,bstrMimeType,vtReplace)	\
    (This)->lpVtbl -> open(This,bstrMimeType,vtReplace)

#define IPIEHTMLDocument3_close(This)	\
    (This)->lpVtbl -> close(This)

#define IPIEHTMLDocument3_clear(This)	\
    (This)->lpVtbl -> clear(This)

#define IPIEHTMLDocument3_write(This,psaWrite)	\
    (This)->lpVtbl -> write(This,psaWrite)

#define IPIEHTMLDocument3_writeln(This,psaWrite)	\
    (This)->lpVtbl -> writeln(This,psaWrite)


#define IPIEHTMLDocument3_get_images(This,ppElemCollection)	\
    (This)->lpVtbl -> get_images(This,ppElemCollection)

#define IPIEHTMLDocument3_get_all(This,ppElemCollection)	\
    (This)->lpVtbl -> get_all(This,ppElemCollection)

#define IPIEHTMLDocument3_get_parentWindow(This,ppWindow)	\
    (This)->lpVtbl -> get_parentWindow(This,ppWindow)


#define IPIEHTMLDocument3_get_documentElement(This,p)	\
    (This)->lpVtbl -> get_documentElement(This,p)

#define IPIEHTMLDocument3_get_uniqueID(This,p)	\
    (This)->lpVtbl -> get_uniqueID(This,p)

#define IPIEHTMLDocument3_put_onrowsdelete(This,v)	\
    (This)->lpVtbl -> put_onrowsdelete(This,v)

#define IPIEHTMLDocument3_get_onrowsdelete(This,p)	\
    (This)->lpVtbl -> get_onrowsdelete(This,p)

#define IPIEHTMLDocument3_put_onrowsinserted(This,v)	\
    (This)->lpVtbl -> put_onrowsinserted(This,v)

#define IPIEHTMLDocument3_get_onrowsinserted(This,p)	\
    (This)->lpVtbl -> get_onrowsinserted(This,p)

#define IPIEHTMLDocument3_put_oncellchange(This,v)	\
    (This)->lpVtbl -> put_oncellchange(This,v)

#define IPIEHTMLDocument3_get_oncellchange(This,p)	\
    (This)->lpVtbl -> get_oncellchange(This,p)

#define IPIEHTMLDocument3_put_ondatasetchanged(This,v)	\
    (This)->lpVtbl -> put_ondatasetchanged(This,v)

#define IPIEHTMLDocument3_get_ondatasetchanged(This,p)	\
    (This)->lpVtbl -> get_ondatasetchanged(This,p)

#define IPIEHTMLDocument3_put_ondataavailable(This,v)	\
    (This)->lpVtbl -> put_ondataavailable(This,v)

#define IPIEHTMLDocument3_get_ondataavailable(This,p)	\
    (This)->lpVtbl -> get_ondataavailable(This,p)

#define IPIEHTMLDocument3_put_ondatasetcomplete(This,v)	\
    (This)->lpVtbl -> put_ondatasetcomplete(This,v)

#define IPIEHTMLDocument3_get_ondatasetcomplete(This,p)	\
    (This)->lpVtbl -> get_ondatasetcomplete(This,p)

#define IPIEHTMLDocument3_put_onpropertychange(This,v)	\
    (This)->lpVtbl -> put_onpropertychange(This,v)

#define IPIEHTMLDocument3_get_onpropertychange(This,p)	\
    (This)->lpVtbl -> get_onpropertychange(This,p)

#define IPIEHTMLDocument3_put_dir(This,v)	\
    (This)->lpVtbl -> put_dir(This,v)

#define IPIEHTMLDocument3_get_dir(This,p)	\
    (This)->lpVtbl -> get_dir(This,p)

#define IPIEHTMLDocument3_put_oncontextmenu(This,v)	\
    (This)->lpVtbl -> put_oncontextmenu(This,v)

#define IPIEHTMLDocument3_get_oncontextmenu(This,p)	\
    (This)->lpVtbl -> get_oncontextmenu(This,p)

#define IPIEHTMLDocument3_put_onstop(This,v)	\
    (This)->lpVtbl -> put_onstop(This,v)

#define IPIEHTMLDocument3_get_onstop(This,p)	\
    (This)->lpVtbl -> get_onstop(This,p)

#define IPIEHTMLDocument3_get_parentDocument(This,p)	\
    (This)->lpVtbl -> get_parentDocument(This,p)

#define IPIEHTMLDocument3_put_enableDownload(This,v)	\
    (This)->lpVtbl -> put_enableDownload(This,v)

#define IPIEHTMLDocument3_get_enableDownload(This,p)	\
    (This)->lpVtbl -> get_enableDownload(This,p)

#define IPIEHTMLDocument3_put_baseUrl(This,v)	\
    (This)->lpVtbl -> put_baseUrl(This,v)

#define IPIEHTMLDocument3_get_baseUrl(This,p)	\
    (This)->lpVtbl -> get_baseUrl(This,p)

#define IPIEHTMLDocument3_get_childNodes(This,p)	\
    (This)->lpVtbl -> get_childNodes(This,p)

#define IPIEHTMLDocument3_put_inheritStyleSheets(This,v)	\
    (This)->lpVtbl -> put_inheritStyleSheets(This,v)

#define IPIEHTMLDocument3_get_inheritStyleSheets(This,p)	\
    (This)->lpVtbl -> get_inheritStyleSheets(This,p)

#define IPIEHTMLDocument3_put_onbeforeeditfocus(This,v)	\
    (This)->lpVtbl -> put_onbeforeeditfocus(This,v)

#define IPIEHTMLDocument3_get_onbeforeeditfocus(This,p)	\
    (This)->lpVtbl -> get_onbeforeeditfocus(This,p)

#define IPIEHTMLDocument3_releaseCapture(This)	\
    (This)->lpVtbl -> releaseCapture(This)

#define IPIEHTMLDocument3_recalc(This,fForce)	\
    (This)->lpVtbl -> recalc(This,fForce)

#define IPIEHTMLDocument3_createTextNode(This,text,newTextNode)	\
    (This)->lpVtbl -> createTextNode(This,text,newTextNode)

#define IPIEHTMLDocument3_attachEvent(This,event,pDisp,pfResult)	\
    (This)->lpVtbl -> attachEvent(This,event,pDisp,pfResult)

#define IPIEHTMLDocument3_detachEvent(This,event,pDisp)	\
    (This)->lpVtbl -> detachEvent(This,event,pDisp)

#define IPIEHTMLDocument3_createDocumentFragment(This,pNewDoc)	\
    (This)->lpVtbl -> createDocumentFragment(This,pNewDoc)

#define IPIEHTMLDocument3_getElementsByName(This,v,pelColl)	\
    (This)->lpVtbl -> getElementsByName(This,v,pelColl)

#define IPIEHTMLDocument3_getElementById(This,v,pel)	\
    (This)->lpVtbl -> getElementById(This,v,pel)

#define IPIEHTMLDocument3_getElementsByTagName(This,v,pelColl)	\
    (This)->lpVtbl -> getElementsByTagName(This,v,pelColl)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_documentElement_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_documentElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_uniqueID_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_uniqueID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_onrowsdelete_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_onrowsdelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_onrowsdelete_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_onrowsdelete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_onrowsinserted_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_onrowsinserted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_onrowsinserted_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_onrowsinserted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_oncellchange_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_oncellchange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_oncellchange_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_oncellchange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_ondatasetchanged_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_ondatasetchanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_ondatasetchanged_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_ondatasetchanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_ondataavailable_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_ondataavailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_ondataavailable_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_ondataavailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_ondatasetcomplete_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_ondatasetcomplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_ondatasetcomplete_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_ondatasetcomplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_onpropertychange_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_onpropertychange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_onpropertychange_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_onpropertychange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_dir_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLDocument3_put_dir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_dir_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_dir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_oncontextmenu_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_oncontextmenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_oncontextmenu_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_oncontextmenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_onstop_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_onstop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_onstop_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_onstop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_parentDocument_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_parentDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_enableDownload_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL v);


void __RPC_STUB IPIEHTMLDocument3_put_enableDownload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_enableDownload_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_enableDownload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_baseUrl_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLDocument3_put_baseUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_baseUrl_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_baseUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_childNodes_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_childNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_inheritStyleSheets_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL v);


void __RPC_STUB IPIEHTMLDocument3_put_inheritStyleSheets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_inheritStyleSheets_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_inheritStyleSheets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_put_onbeforeeditfocus_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLDocument3_put_onbeforeeditfocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_get_onbeforeeditfocus_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLDocument3_get_onbeforeeditfocus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_releaseCapture_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This);


void __RPC_STUB IPIEHTMLDocument3_releaseCapture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_recalc_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in][defaultvalue] */ VARIANT_BOOL fForce);


void __RPC_STUB IPIEHTMLDocument3_recalc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_createTextNode_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR text,
    /* [out][retval] */ IPIEHTMLDOMNode __RPC_FAR *__RPC_FAR *newTextNode);


void __RPC_STUB IPIEHTMLDocument3_createTextNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_attachEvent_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR event,
    /* [in] */ IDispatch __RPC_FAR *pDisp,
    /* [out][retval] */ VARIANT_BOOL __RPC_FAR *pfResult);


void __RPC_STUB IPIEHTMLDocument3_attachEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_detachEvent_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR event,
    /* [in] */ IDispatch __RPC_FAR *pDisp);


void __RPC_STUB IPIEHTMLDocument3_detachEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_createDocumentFragment_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *pNewDoc);


void __RPC_STUB IPIEHTMLDocument3_createDocumentFragment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_getElementsByName_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR v,
    /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl);


void __RPC_STUB IPIEHTMLDocument3_getElementsByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_getElementById_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR v,
    /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *pel);


void __RPC_STUB IPIEHTMLDocument3_getElementById_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument3_getElementsByTagName_Proxy( 
    IPIEHTMLDocument3 __RPC_FAR * This,
    /* [in] */ BSTR v,
    /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl);


void __RPC_STUB IPIEHTMLDocument3_getElementsByTagName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLDocument3_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLDocument4_INTERFACE_DEFINED__
#define __IPIEHTMLDocument4_INTERFACE_DEFINED__

/* interface IPIEHTMLDocument4 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLDocument4;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6CD4C067-D25B-4558-86FB-0BE801001007")
    IPIEHTMLDocument4 : public IPIEHTMLDocument3
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_body( 
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppBody) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE createElement( 
            /* [in] */ BSTR bstrTag,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *pel) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_title( 
            /* [in] */ BSTR bstrTitle) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLDocument4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLDocument4 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLDocument4 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_links )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_forms )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_anchors )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_frames )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lastModified )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrLastModified);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_referrer )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrReferrer);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_location )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_title )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTitle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_URL )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrURL);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bgColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT vtBGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bgColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fgColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT vtFGColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fgColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtFGColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_linkColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT vtLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_linkColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_alinkColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT vtALinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_alinkColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtALinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vlinkColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT vtVLinkColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vlinkColor )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtVLinkColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_cookie )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR bstrCookie);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cookie )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrCookie);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_domain )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR bstrDomain);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_domain )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDomain);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMimeType,
            /* [optional][in] */ VARIANT vtReplace);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IPIEHTMLDocument4 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *clear )( 
            IPIEHTMLDocument4 __RPC_FAR * This);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *write )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        /* [id][vararg] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *writeln )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * psaWrite);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_images )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_all )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentWindow )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow2 __RPC_FAR *__RPC_FAR *ppWindow);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_documentElement )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_uniqueID )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onrowsdelete )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onrowsdelete )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onrowsinserted )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onrowsinserted )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_oncellchange )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_oncellchange )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ondatasetchanged )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ondatasetchanged )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ondataavailable )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ondataavailable )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ondatasetcomplete )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ondatasetcomplete )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onpropertychange )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onpropertychange )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_dir )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_dir )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_oncontextmenu )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_oncontextmenu )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onstop )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onstop )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentDocument )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_enableDownload )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_enableDownload )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_baseUrl )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_baseUrl )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_childNodes )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_inheritStyleSheets )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_inheritStyleSheets )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *p);
        
        /* [restricted][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onbeforeeditfocus )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onbeforeeditfocus )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *releaseCapture )( 
            IPIEHTMLDocument4 __RPC_FAR * This);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *recalc )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in][defaultvalue] */ VARIANT_BOOL fForce);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createTextNode )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR text,
            /* [out][retval] */ IPIEHTMLDOMNode __RPC_FAR *__RPC_FAR *newTextNode);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *attachEvent )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR event,
            /* [in] */ IDispatch __RPC_FAR *pDisp,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *pfResult);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *detachEvent )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR event,
            /* [in] */ IDispatch __RPC_FAR *pDisp);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createDocumentFragment )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLDocument2 __RPC_FAR *__RPC_FAR *pNewDoc);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getElementsByName )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getElementById )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *pel);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getElementsByTagName )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR v,
            /* [out][retval] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *pelColl);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_body )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppBody);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *createElement )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR bstrTag,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *pel);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_title )( 
            IPIEHTMLDocument4 __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle);
        
        END_INTERFACE
    } IPIEHTMLDocument4Vtbl;

    interface IPIEHTMLDocument4
    {
        CONST_VTBL struct IPIEHTMLDocument4Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLDocument4_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLDocument4_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLDocument4_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLDocument4_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLDocument4_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLDocument4_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLDocument4_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLDocument4_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLDocument4_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLDocument4_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLDocument4_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLDocument4_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLDocument4_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLDocument4_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLDocument4_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLDocument4_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLDocument4_get_links(This,ppElemCollection)	\
    (This)->lpVtbl -> get_links(This,ppElemCollection)

#define IPIEHTMLDocument4_get_forms(This,ppElemCollection)	\
    (This)->lpVtbl -> get_forms(This,ppElemCollection)

#define IPIEHTMLDocument4_get_anchors(This,ppElemCollection)	\
    (This)->lpVtbl -> get_anchors(This,ppElemCollection)

#define IPIEHTMLDocument4_get_frames(This,ppElemCollection)	\
    (This)->lpVtbl -> get_frames(This,ppElemCollection)

#define IPIEHTMLDocument4_get_lastModified(This,pbstrLastModified)	\
    (This)->lpVtbl -> get_lastModified(This,pbstrLastModified)

#define IPIEHTMLDocument4_get_referrer(This,pbstrReferrer)	\
    (This)->lpVtbl -> get_referrer(This,pbstrReferrer)

#define IPIEHTMLDocument4_get_location(This,ppLocation)	\
    (This)->lpVtbl -> get_location(This,ppLocation)

#define IPIEHTMLDocument4_get_title(This,pbstrTitle)	\
    (This)->lpVtbl -> get_title(This,pbstrTitle)

#define IPIEHTMLDocument4_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IPIEHTMLDocument4_put_bgColor(This,vtBGColor)	\
    (This)->lpVtbl -> put_bgColor(This,vtBGColor)

#define IPIEHTMLDocument4_get_bgColor(This,pvtBGColor)	\
    (This)->lpVtbl -> get_bgColor(This,pvtBGColor)

#define IPIEHTMLDocument4_put_fgColor(This,vtFGColor)	\
    (This)->lpVtbl -> put_fgColor(This,vtFGColor)

#define IPIEHTMLDocument4_get_fgColor(This,pvtFGColor)	\
    (This)->lpVtbl -> get_fgColor(This,pvtFGColor)

#define IPIEHTMLDocument4_put_linkColor(This,vtLinkColor)	\
    (This)->lpVtbl -> put_linkColor(This,vtLinkColor)

#define IPIEHTMLDocument4_get_linkColor(This,pvtLinkColor)	\
    (This)->lpVtbl -> get_linkColor(This,pvtLinkColor)

#define IPIEHTMLDocument4_put_alinkColor(This,vtALinkColor)	\
    (This)->lpVtbl -> put_alinkColor(This,vtALinkColor)

#define IPIEHTMLDocument4_get_alinkColor(This,pvtALinkColor)	\
    (This)->lpVtbl -> get_alinkColor(This,pvtALinkColor)

#define IPIEHTMLDocument4_put_vlinkColor(This,vtVLinkColor)	\
    (This)->lpVtbl -> put_vlinkColor(This,vtVLinkColor)

#define IPIEHTMLDocument4_get_vlinkColor(This,pvtVLinkColor)	\
    (This)->lpVtbl -> get_vlinkColor(This,pvtVLinkColor)

#define IPIEHTMLDocument4_put_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> put_cookie(This,bstrCookie)

#define IPIEHTMLDocument4_get_cookie(This,bstrCookie)	\
    (This)->lpVtbl -> get_cookie(This,bstrCookie)

#define IPIEHTMLDocument4_put_domain(This,bstrDomain)	\
    (This)->lpVtbl -> put_domain(This,bstrDomain)

#define IPIEHTMLDocument4_get_domain(This,pbstrDomain)	\
    (This)->lpVtbl -> get_domain(This,pbstrDomain)

#define IPIEHTMLDocument4_open(This,bstrMimeType,vtReplace)	\
    (This)->lpVtbl -> open(This,bstrMimeType,vtReplace)

#define IPIEHTMLDocument4_close(This)	\
    (This)->lpVtbl -> close(This)

#define IPIEHTMLDocument4_clear(This)	\
    (This)->lpVtbl -> clear(This)

#define IPIEHTMLDocument4_write(This,psaWrite)	\
    (This)->lpVtbl -> write(This,psaWrite)

#define IPIEHTMLDocument4_writeln(This,psaWrite)	\
    (This)->lpVtbl -> writeln(This,psaWrite)


#define IPIEHTMLDocument4_get_images(This,ppElemCollection)	\
    (This)->lpVtbl -> get_images(This,ppElemCollection)

#define IPIEHTMLDocument4_get_all(This,ppElemCollection)	\
    (This)->lpVtbl -> get_all(This,ppElemCollection)

#define IPIEHTMLDocument4_get_parentWindow(This,ppWindow)	\
    (This)->lpVtbl -> get_parentWindow(This,ppWindow)


#define IPIEHTMLDocument4_get_documentElement(This,p)	\
    (This)->lpVtbl -> get_documentElement(This,p)

#define IPIEHTMLDocument4_get_uniqueID(This,p)	\
    (This)->lpVtbl -> get_uniqueID(This,p)

#define IPIEHTMLDocument4_put_onrowsdelete(This,v)	\
    (This)->lpVtbl -> put_onrowsdelete(This,v)

#define IPIEHTMLDocument4_get_onrowsdelete(This,p)	\
    (This)->lpVtbl -> get_onrowsdelete(This,p)

#define IPIEHTMLDocument4_put_onrowsinserted(This,v)	\
    (This)->lpVtbl -> put_onrowsinserted(This,v)

#define IPIEHTMLDocument4_get_onrowsinserted(This,p)	\
    (This)->lpVtbl -> get_onrowsinserted(This,p)

#define IPIEHTMLDocument4_put_oncellchange(This,v)	\
    (This)->lpVtbl -> put_oncellchange(This,v)

#define IPIEHTMLDocument4_get_oncellchange(This,p)	\
    (This)->lpVtbl -> get_oncellchange(This,p)

#define IPIEHTMLDocument4_put_ondatasetchanged(This,v)	\
    (This)->lpVtbl -> put_ondatasetchanged(This,v)

#define IPIEHTMLDocument4_get_ondatasetchanged(This,p)	\
    (This)->lpVtbl -> get_ondatasetchanged(This,p)

#define IPIEHTMLDocument4_put_ondataavailable(This,v)	\
    (This)->lpVtbl -> put_ondataavailable(This,v)

#define IPIEHTMLDocument4_get_ondataavailable(This,p)	\
    (This)->lpVtbl -> get_ondataavailable(This,p)

#define IPIEHTMLDocument4_put_ondatasetcomplete(This,v)	\
    (This)->lpVtbl -> put_ondatasetcomplete(This,v)

#define IPIEHTMLDocument4_get_ondatasetcomplete(This,p)	\
    (This)->lpVtbl -> get_ondatasetcomplete(This,p)

#define IPIEHTMLDocument4_put_onpropertychange(This,v)	\
    (This)->lpVtbl -> put_onpropertychange(This,v)

#define IPIEHTMLDocument4_get_onpropertychange(This,p)	\
    (This)->lpVtbl -> get_onpropertychange(This,p)

#define IPIEHTMLDocument4_put_dir(This,v)	\
    (This)->lpVtbl -> put_dir(This,v)

#define IPIEHTMLDocument4_get_dir(This,p)	\
    (This)->lpVtbl -> get_dir(This,p)

#define IPIEHTMLDocument4_put_oncontextmenu(This,v)	\
    (This)->lpVtbl -> put_oncontextmenu(This,v)

#define IPIEHTMLDocument4_get_oncontextmenu(This,p)	\
    (This)->lpVtbl -> get_oncontextmenu(This,p)

#define IPIEHTMLDocument4_put_onstop(This,v)	\
    (This)->lpVtbl -> put_onstop(This,v)

#define IPIEHTMLDocument4_get_onstop(This,p)	\
    (This)->lpVtbl -> get_onstop(This,p)

#define IPIEHTMLDocument4_get_parentDocument(This,p)	\
    (This)->lpVtbl -> get_parentDocument(This,p)

#define IPIEHTMLDocument4_put_enableDownload(This,v)	\
    (This)->lpVtbl -> put_enableDownload(This,v)

#define IPIEHTMLDocument4_get_enableDownload(This,p)	\
    (This)->lpVtbl -> get_enableDownload(This,p)

#define IPIEHTMLDocument4_put_baseUrl(This,v)	\
    (This)->lpVtbl -> put_baseUrl(This,v)

#define IPIEHTMLDocument4_get_baseUrl(This,p)	\
    (This)->lpVtbl -> get_baseUrl(This,p)

#define IPIEHTMLDocument4_get_childNodes(This,p)	\
    (This)->lpVtbl -> get_childNodes(This,p)

#define IPIEHTMLDocument4_put_inheritStyleSheets(This,v)	\
    (This)->lpVtbl -> put_inheritStyleSheets(This,v)

#define IPIEHTMLDocument4_get_inheritStyleSheets(This,p)	\
    (This)->lpVtbl -> get_inheritStyleSheets(This,p)

#define IPIEHTMLDocument4_put_onbeforeeditfocus(This,v)	\
    (This)->lpVtbl -> put_onbeforeeditfocus(This,v)

#define IPIEHTMLDocument4_get_onbeforeeditfocus(This,p)	\
    (This)->lpVtbl -> get_onbeforeeditfocus(This,p)

#define IPIEHTMLDocument4_releaseCapture(This)	\
    (This)->lpVtbl -> releaseCapture(This)

#define IPIEHTMLDocument4_recalc(This,fForce)	\
    (This)->lpVtbl -> recalc(This,fForce)

#define IPIEHTMLDocument4_createTextNode(This,text,newTextNode)	\
    (This)->lpVtbl -> createTextNode(This,text,newTextNode)

#define IPIEHTMLDocument4_attachEvent(This,event,pDisp,pfResult)	\
    (This)->lpVtbl -> attachEvent(This,event,pDisp,pfResult)

#define IPIEHTMLDocument4_detachEvent(This,event,pDisp)	\
    (This)->lpVtbl -> detachEvent(This,event,pDisp)

#define IPIEHTMLDocument4_createDocumentFragment(This,pNewDoc)	\
    (This)->lpVtbl -> createDocumentFragment(This,pNewDoc)

#define IPIEHTMLDocument4_getElementsByName(This,v,pelColl)	\
    (This)->lpVtbl -> getElementsByName(This,v,pelColl)

#define IPIEHTMLDocument4_getElementById(This,v,pel)	\
    (This)->lpVtbl -> getElementById(This,v,pel)

#define IPIEHTMLDocument4_getElementsByTagName(This,v,pelColl)	\
    (This)->lpVtbl -> getElementsByTagName(This,v,pelColl)


#define IPIEHTMLDocument4_get_body(This,ppBody)	\
    (This)->lpVtbl -> get_body(This,ppBody)

#define IPIEHTMLDocument4_createElement(This,bstrTag,pel)	\
    (This)->lpVtbl -> createElement(This,bstrTag,pel)

#define IPIEHTMLDocument4_put_title(This,bstrTitle)	\
    (This)->lpVtbl -> put_title(This,bstrTitle)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument4_get_body_Proxy( 
    IPIEHTMLDocument4 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppBody);


void __RPC_STUB IPIEHTMLDocument4_get_body_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument4_createElement_Proxy( 
    IPIEHTMLDocument4 __RPC_FAR * This,
    /* [in] */ BSTR bstrTag,
    /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *pel);


void __RPC_STUB IPIEHTMLDocument4_createElement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDocument4_put_title_Proxy( 
    IPIEHTMLDocument4 __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle);


void __RPC_STUB IPIEHTMLDocument4_put_title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLDocument4_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLScreen_INTERFACE_DEFINED__
#define __IPIEHTMLScreen_INTERFACE_DEFINED__

/* interface IPIEHTMLScreen */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLScreen;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2120")
    IPIEHTMLScreen : public IDispatchEx
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_colorDepth( 
            /* [retval][out] */ long __RPC_FAR *plColorDepth) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_width( 
            /* [retval][out] */ long __RPC_FAR *plWidth) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_height( 
            /* [retval][out] */ long __RPC_FAR *plHeight) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_availWidth( 
            /* [retval][out] */ long __RPC_FAR *plAvailWidth) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_availHeight( 
            /* [retval][out] */ long __RPC_FAR *plAvailHeight) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLScreenVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLScreen __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLScreen __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_colorDepth )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plColorDepth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_width )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plWidth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_height )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plHeight);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_availWidth )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plAvailWidth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_availHeight )( 
            IPIEHTMLScreen __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plAvailHeight);
        
        END_INTERFACE
    } IPIEHTMLScreenVtbl;

    interface IPIEHTMLScreen
    {
        CONST_VTBL struct IPIEHTMLScreenVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLScreen_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLScreen_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLScreen_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLScreen_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLScreen_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLScreen_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLScreen_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLScreen_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLScreen_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLScreen_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLScreen_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLScreen_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLScreen_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLScreen_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLScreen_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLScreen_get_colorDepth(This,plColorDepth)	\
    (This)->lpVtbl -> get_colorDepth(This,plColorDepth)

#define IPIEHTMLScreen_get_width(This,plWidth)	\
    (This)->lpVtbl -> get_width(This,plWidth)

#define IPIEHTMLScreen_get_height(This,plHeight)	\
    (This)->lpVtbl -> get_height(This,plHeight)

#define IPIEHTMLScreen_get_availWidth(This,plAvailWidth)	\
    (This)->lpVtbl -> get_availWidth(This,plAvailWidth)

#define IPIEHTMLScreen_get_availHeight(This,plAvailHeight)	\
    (This)->lpVtbl -> get_availHeight(This,plAvailHeight)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen_get_colorDepth_Proxy( 
    IPIEHTMLScreen __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plColorDepth);


void __RPC_STUB IPIEHTMLScreen_get_colorDepth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen_get_width_Proxy( 
    IPIEHTMLScreen __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plWidth);


void __RPC_STUB IPIEHTMLScreen_get_width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen_get_height_Proxy( 
    IPIEHTMLScreen __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plHeight);


void __RPC_STUB IPIEHTMLScreen_get_height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen_get_availWidth_Proxy( 
    IPIEHTMLScreen __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plAvailWidth);


void __RPC_STUB IPIEHTMLScreen_get_availWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen_get_availHeight_Proxy( 
    IPIEHTMLScreen __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plAvailHeight);


void __RPC_STUB IPIEHTMLScreen_get_availHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLScreen_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLScreen2_INTERFACE_DEFINED__
#define __IPIEHTMLScreen2_INTERFACE_DEFINED__

/* interface IPIEHTMLScreen2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLScreen2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("400EADB2-B21E-4933-8A99-E852927AE620")
    IPIEHTMLScreen2 : public IPIEHTMLScreen
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_deviceXDPI( 
            /* [retval][out] */ long __RPC_FAR *plDeviceXDPI) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_deviceYDPI( 
            /* [retval][out] */ long __RPC_FAR *plDeviceYDPI) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_logicalXDPI( 
            /* [retval][out] */ long __RPC_FAR *plLogicalXDPI) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_logicalYDPI( 
            /* [retval][out] */ long __RPC_FAR *plLogicalYDPI) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLScreen2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLScreen2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLScreen2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_colorDepth )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plColorDepth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_width )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plWidth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_height )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plHeight);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_availWidth )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plAvailWidth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_availHeight )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plAvailHeight);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_deviceXDPI )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plDeviceXDPI);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_deviceYDPI )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plDeviceYDPI);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_logicalXDPI )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLogicalXDPI);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_logicalYDPI )( 
            IPIEHTMLScreen2 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLogicalYDPI);
        
        END_INTERFACE
    } IPIEHTMLScreen2Vtbl;

    interface IPIEHTMLScreen2
    {
        CONST_VTBL struct IPIEHTMLScreen2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLScreen2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLScreen2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLScreen2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLScreen2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLScreen2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLScreen2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLScreen2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLScreen2_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLScreen2_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLScreen2_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLScreen2_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLScreen2_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLScreen2_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLScreen2_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLScreen2_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLScreen2_get_colorDepth(This,plColorDepth)	\
    (This)->lpVtbl -> get_colorDepth(This,plColorDepth)

#define IPIEHTMLScreen2_get_width(This,plWidth)	\
    (This)->lpVtbl -> get_width(This,plWidth)

#define IPIEHTMLScreen2_get_height(This,plHeight)	\
    (This)->lpVtbl -> get_height(This,plHeight)

#define IPIEHTMLScreen2_get_availWidth(This,plAvailWidth)	\
    (This)->lpVtbl -> get_availWidth(This,plAvailWidth)

#define IPIEHTMLScreen2_get_availHeight(This,plAvailHeight)	\
    (This)->lpVtbl -> get_availHeight(This,plAvailHeight)


#define IPIEHTMLScreen2_get_deviceXDPI(This,plDeviceXDPI)	\
    (This)->lpVtbl -> get_deviceXDPI(This,plDeviceXDPI)

#define IPIEHTMLScreen2_get_deviceYDPI(This,plDeviceYDPI)	\
    (This)->lpVtbl -> get_deviceYDPI(This,plDeviceYDPI)

#define IPIEHTMLScreen2_get_logicalXDPI(This,plLogicalXDPI)	\
    (This)->lpVtbl -> get_logicalXDPI(This,plLogicalXDPI)

#define IPIEHTMLScreen2_get_logicalYDPI(This,plLogicalYDPI)	\
    (This)->lpVtbl -> get_logicalYDPI(This,plLogicalYDPI)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen2_get_deviceXDPI_Proxy( 
    IPIEHTMLScreen2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plDeviceXDPI);


void __RPC_STUB IPIEHTMLScreen2_get_deviceXDPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen2_get_deviceYDPI_Proxy( 
    IPIEHTMLScreen2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plDeviceYDPI);


void __RPC_STUB IPIEHTMLScreen2_get_deviceYDPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen2_get_logicalXDPI_Proxy( 
    IPIEHTMLScreen2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLogicalXDPI);


void __RPC_STUB IPIEHTMLScreen2_get_logicalXDPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLScreen2_get_logicalYDPI_Proxy( 
    IPIEHTMLScreen2 __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLogicalYDPI);


void __RPC_STUB IPIEHTMLScreen2_get_logicalYDPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLScreen2_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLImageElementFactory_INTERFACE_DEFINED__
#define __IPIEHTMLImageElementFactory_INTERFACE_DEFINED__

/* interface IPIEHTMLImageElementFactory */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLImageElementFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE22F4")
    IPIEHTMLImageElementFactory : public IDispatchEx
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE create( 
            /* [in][optional] */ VARIANT vtWidth,
            /* [in][optional] */ VARIANT vtHeight,
            /* [retval][out] */ IPIEHTMLImgElement __RPC_FAR *__RPC_FAR *ppImgElement) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLImageElementFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *create )( 
            IPIEHTMLImageElementFactory __RPC_FAR * This,
            /* [in][optional] */ VARIANT vtWidth,
            /* [in][optional] */ VARIANT vtHeight,
            /* [retval][out] */ IPIEHTMLImgElement __RPC_FAR *__RPC_FAR *ppImgElement);
        
        END_INTERFACE
    } IPIEHTMLImageElementFactoryVtbl;

    interface IPIEHTMLImageElementFactory
    {
        CONST_VTBL struct IPIEHTMLImageElementFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLImageElementFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLImageElementFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLImageElementFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLImageElementFactory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLImageElementFactory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLImageElementFactory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLImageElementFactory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLImageElementFactory_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLImageElementFactory_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLImageElementFactory_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLImageElementFactory_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLImageElementFactory_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLImageElementFactory_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLImageElementFactory_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLImageElementFactory_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLImageElementFactory_create(This,vtWidth,vtHeight,ppImgElement)	\
    (This)->lpVtbl -> create(This,vtWidth,vtHeight,ppImgElement)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLImageElementFactory_create_Proxy( 
    IPIEHTMLImageElementFactory __RPC_FAR * This,
    /* [in][optional] */ VARIANT vtWidth,
    /* [in][optional] */ VARIANT vtHeight,
    /* [retval][out] */ IPIEHTMLImgElement __RPC_FAR *__RPC_FAR *ppImgElement);


void __RPC_STUB IPIEHTMLImageElementFactory_create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLImageElementFactory_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLOptionElement_INTERFACE_DEFINED__
#define __IPIEHTMLOptionElement_INTERFACE_DEFINED__

/* interface IPIEHTMLOptionElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLOptionElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE210B")
    IPIEHTMLOptionElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_selected( 
            /* [in] */ VARIANT_BOOL vbSelected) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_selected( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbSelected) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_value( 
            /* [in] */ BSTR bstrValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_value( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_defaultSelected( 
            /* [in] */ VARIANT_BOOL vbDefaultSelected) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_defaultSelected( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDefaultSelected) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_index( 
            /* [retval][out] */ long __RPC_FAR *plIndex) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_text( 
            /* [in] */ BSTR bstrText) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_text( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLOptionElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLOptionElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLOptionElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_selected )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbSelected);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_selected )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbSelected);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_defaultSelected )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDefaultSelected);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_defaultSelected )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDefaultSelected);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_index )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plIndex);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_text )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [in] */ BSTR bstrText);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_text )( 
            IPIEHTMLOptionElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText);
        
        END_INTERFACE
    } IPIEHTMLOptionElementVtbl;

    interface IPIEHTMLOptionElement
    {
        CONST_VTBL struct IPIEHTMLOptionElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLOptionElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLOptionElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLOptionElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLOptionElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLOptionElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLOptionElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLOptionElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLOptionElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLOptionElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLOptionElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLOptionElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLOptionElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLOptionElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLOptionElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLOptionElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLOptionElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLOptionElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLOptionElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLOptionElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLOptionElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLOptionElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLOptionElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLOptionElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLOptionElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLOptionElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLOptionElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLOptionElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLOptionElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLOptionElement_put_selected(This,vbSelected)	\
    (This)->lpVtbl -> put_selected(This,vbSelected)

#define IPIEHTMLOptionElement_get_selected(This,pvbSelected)	\
    (This)->lpVtbl -> get_selected(This,pvbSelected)

#define IPIEHTMLOptionElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLOptionElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLOptionElement_put_defaultSelected(This,vbDefaultSelected)	\
    (This)->lpVtbl -> put_defaultSelected(This,vbDefaultSelected)

#define IPIEHTMLOptionElement_get_defaultSelected(This,pvbDefaultSelected)	\
    (This)->lpVtbl -> get_defaultSelected(This,pvbDefaultSelected)

#define IPIEHTMLOptionElement_get_index(This,plIndex)	\
    (This)->lpVtbl -> get_index(This,plIndex)

#define IPIEHTMLOptionElement_put_text(This,bstrText)	\
    (This)->lpVtbl -> put_text(This,bstrText)

#define IPIEHTMLOptionElement_get_text(This,pbstrText)	\
    (This)->lpVtbl -> get_text(This,pbstrText)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_put_selected_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbSelected);


void __RPC_STUB IPIEHTMLOptionElement_put_selected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_get_selected_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbSelected);


void __RPC_STUB IPIEHTMLOptionElement_get_selected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_put_value_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [in] */ BSTR bstrValue);


void __RPC_STUB IPIEHTMLOptionElement_put_value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_get_value_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);


void __RPC_STUB IPIEHTMLOptionElement_get_value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_put_defaultSelected_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbDefaultSelected);


void __RPC_STUB IPIEHTMLOptionElement_put_defaultSelected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_get_defaultSelected_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDefaultSelected);


void __RPC_STUB IPIEHTMLOptionElement_get_defaultSelected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_get_index_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plIndex);


void __RPC_STUB IPIEHTMLOptionElement_get_index_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_put_text_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [in] */ BSTR bstrText);


void __RPC_STUB IPIEHTMLOptionElement_put_text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElement_get_text_Proxy( 
    IPIEHTMLOptionElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrText);


void __RPC_STUB IPIEHTMLOptionElement_get_text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLOptionElement_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLOptionElementFactory_INTERFACE_DEFINED__
#define __IPIEHTMLOptionElementFactory_INTERFACE_DEFINED__

/* interface IPIEHTMLOptionElementFactory */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLOptionElementFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE21FB")
    IPIEHTMLOptionElementFactory : public IDispatchEx
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE create( 
            /* [in][optional] */ VARIANT vtText,
            /* [in][optional] */ VARIANT vtValue,
            /* [in][optional] */ VARIANT vtDefaultSelected,
            /* [in][optional] */ VARIANT vtSelected,
            /* [retval][out] */ IPIEHTMLOptionElement __RPC_FAR *__RPC_FAR *ppOptionElement) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLOptionElementFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *create )( 
            IPIEHTMLOptionElementFactory __RPC_FAR * This,
            /* [in][optional] */ VARIANT vtText,
            /* [in][optional] */ VARIANT vtValue,
            /* [in][optional] */ VARIANT vtDefaultSelected,
            /* [in][optional] */ VARIANT vtSelected,
            /* [retval][out] */ IPIEHTMLOptionElement __RPC_FAR *__RPC_FAR *ppOptionElement);
        
        END_INTERFACE
    } IPIEHTMLOptionElementFactoryVtbl;

    interface IPIEHTMLOptionElementFactory
    {
        CONST_VTBL struct IPIEHTMLOptionElementFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLOptionElementFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLOptionElementFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLOptionElementFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLOptionElementFactory_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLOptionElementFactory_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLOptionElementFactory_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLOptionElementFactory_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLOptionElementFactory_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLOptionElementFactory_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLOptionElementFactory_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLOptionElementFactory_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLOptionElementFactory_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLOptionElementFactory_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLOptionElementFactory_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLOptionElementFactory_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLOptionElementFactory_create(This,vtText,vtValue,vtDefaultSelected,vtSelected,ppOptionElement)	\
    (This)->lpVtbl -> create(This,vtText,vtValue,vtDefaultSelected,vtSelected,ppOptionElement)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionElementFactory_create_Proxy( 
    IPIEHTMLOptionElementFactory __RPC_FAR * This,
    /* [in][optional] */ VARIANT vtText,
    /* [in][optional] */ VARIANT vtValue,
    /* [in][optional] */ VARIANT vtDefaultSelected,
    /* [in][optional] */ VARIANT vtSelected,
    /* [retval][out] */ IPIEHTMLOptionElement __RPC_FAR *__RPC_FAR *ppOptionElement);


void __RPC_STUB IPIEHTMLOptionElementFactory_create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLOptionElementFactory_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLWindow3_INTERFACE_DEFINED__
#define __IPIEHTMLWindow3_INTERFACE_DEFINED__

/* interface IPIEHTMLWindow3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLWindow3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d6aedd8-5b4f-4dd7-966c-55d36f048aa7")
    IPIEHTMLWindow3 : public IPIEHTMLWindow2
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_external( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppExternalDispatch) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLWindow3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLWindow3 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLWindow3 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_location )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLLocation __RPC_FAR *__RPC_FAR *ppLocation);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_history )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEOmHistory __RPC_FAR *__RPC_FAR *ppOmHistory);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_navigator )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEOmNavigator __RPC_FAR *__RPC_FAR *ppOmNavigator);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parent )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_self )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowSelf);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_top )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowTop);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_window )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindow);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_frames )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_screen )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLScreen __RPC_FAR *__RPC_FAR *ppScreen);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLength);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_name )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setTimeout )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ BSTR bstrExpression,
            /* [in] */ long lMS,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
            /* [retval][out] */ long __RPC_FAR *plTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *clearTimeout )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ long lTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *alert )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *confirm )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbConfirmed);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *prompt )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrMessage,
            /* [defaultvalue][in] */ BSTR bstrDefaultValue,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close )( 
            IPIEHTMLWindow3 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLWindow3 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLWindow3 __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *navigate )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ BSTR bstrURL);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [defaultvalue][in] */ BSTR bstrURL,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtName,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtFeatures,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtReplace,
            /* [retval][out] */ IPIEHTMLWindow __RPC_FAR *__RPC_FAR *ppWindowResult);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *scroll )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ long lX,
            /* [in] */ long lY);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onload )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ VARIANT vtOnLoad);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onload )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnLoad);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onunload )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ VARIANT vtOnUnload);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onunload )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvOnUnload);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onerror )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ VARIANT vtOnError);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onerror )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtOnError);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Image )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLImageElementFactory __RPC_FAR *__RPC_FAR *ppImageFactory);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Option )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLOptionElementFactory __RPC_FAR *__RPC_FAR *ppOptionFactory);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setTimeout )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *pvtExpression,
            /* [in] */ long lMS,
            /* [optional][in] */ VARIANT __RPC_FAR *pvtLanguage,
            /* [retval][out] */ long __RPC_FAR *plTimerID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *scrollTo )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [in] */ long lX,
            /* [in] */ long lY);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_external )( 
            IPIEHTMLWindow3 __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppExternalDispatch);
        
        END_INTERFACE
    } IPIEHTMLWindow3Vtbl;

    interface IPIEHTMLWindow3
    {
        CONST_VTBL struct IPIEHTMLWindow3Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLWindow3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLWindow3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLWindow3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLWindow3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLWindow3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLWindow3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLWindow3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLWindow3_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLWindow3_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLWindow3_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLWindow3_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLWindow3_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLWindow3_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLWindow3_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLWindow3_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLWindow3_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLWindow3_get_location(This,ppLocation)	\
    (This)->lpVtbl -> get_location(This,ppLocation)

#define IPIEHTMLWindow3_get_history(This,ppOmHistory)	\
    (This)->lpVtbl -> get_history(This,ppOmHistory)

#define IPIEHTMLWindow3_get_navigator(This,ppOmNavigator)	\
    (This)->lpVtbl -> get_navigator(This,ppOmNavigator)

#define IPIEHTMLWindow3_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLWindow3_get_parent(This,ppWindowParent)	\
    (This)->lpVtbl -> get_parent(This,ppWindowParent)

#define IPIEHTMLWindow3_get_self(This,ppWindowSelf)	\
    (This)->lpVtbl -> get_self(This,ppWindowSelf)

#define IPIEHTMLWindow3_get_top(This,ppWindowTop)	\
    (This)->lpVtbl -> get_top(This,ppWindowTop)

#define IPIEHTMLWindow3_get_window(This,ppWindow)	\
    (This)->lpVtbl -> get_window(This,ppWindow)

#define IPIEHTMLWindow3_get_frames(This,ppElemCollection)	\
    (This)->lpVtbl -> get_frames(This,ppElemCollection)

#define IPIEHTMLWindow3_get_screen(This,ppScreen)	\
    (This)->lpVtbl -> get_screen(This,ppScreen)

#define IPIEHTMLWindow3_get_length(This,plLength)	\
    (This)->lpVtbl -> get_length(This,plLength)

#define IPIEHTMLWindow3_put_name(This,bstrName)	\
    (This)->lpVtbl -> put_name(This,bstrName)

#define IPIEHTMLWindow3_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLWindow3_setTimeout(This,bstrExpression,lMS,pvtLanguage,plTimerID)	\
    (This)->lpVtbl -> setTimeout(This,bstrExpression,lMS,pvtLanguage,plTimerID)

#define IPIEHTMLWindow3_clearTimeout(This,lTimerID)	\
    (This)->lpVtbl -> clearTimeout(This,lTimerID)

#define IPIEHTMLWindow3_alert(This,bstrMessage)	\
    (This)->lpVtbl -> alert(This,bstrMessage)

#define IPIEHTMLWindow3_confirm(This,bstrMessage,pvbConfirmed)	\
    (This)->lpVtbl -> confirm(This,bstrMessage,pvbConfirmed)

#define IPIEHTMLWindow3_prompt(This,bstrMessage,bstrDefaultValue,pvtText)	\
    (This)->lpVtbl -> prompt(This,bstrMessage,bstrDefaultValue,pvtText)

#define IPIEHTMLWindow3_close(This)	\
    (This)->lpVtbl -> close(This)

#define IPIEHTMLWindow3_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLWindow3_blur(This)	\
    (This)->lpVtbl -> blur(This)

#define IPIEHTMLWindow3_navigate(This,bstrURL)	\
    (This)->lpVtbl -> navigate(This,bstrURL)

#define IPIEHTMLWindow3_open(This,bstrURL,pvtName,pvtFeatures,pvtReplace,ppWindowResult)	\
    (This)->lpVtbl -> open(This,bstrURL,pvtName,pvtFeatures,pvtReplace,ppWindowResult)

#define IPIEHTMLWindow3_scroll(This,lX,lY)	\
    (This)->lpVtbl -> scroll(This,lX,lY)


#define IPIEHTMLWindow3_put_onload(This,vtOnLoad)	\
    (This)->lpVtbl -> put_onload(This,vtOnLoad)

#define IPIEHTMLWindow3_get_onload(This,pvtOnLoad)	\
    (This)->lpVtbl -> get_onload(This,pvtOnLoad)

#define IPIEHTMLWindow3_put_onunload(This,vtOnUnload)	\
    (This)->lpVtbl -> put_onunload(This,vtOnUnload)

#define IPIEHTMLWindow3_get_onunload(This,pvOnUnload)	\
    (This)->lpVtbl -> get_onunload(This,pvOnUnload)

#define IPIEHTMLWindow3_put_onerror(This,vtOnError)	\
    (This)->lpVtbl -> put_onerror(This,vtOnError)

#define IPIEHTMLWindow3_get_onerror(This,pvtOnError)	\
    (This)->lpVtbl -> get_onerror(This,pvtOnError)

#define IPIEHTMLWindow3_get_Image(This,ppImageFactory)	\
    (This)->lpVtbl -> get_Image(This,ppImageFactory)

#define IPIEHTMLWindow3_get_Option(This,ppOptionFactory)	\
    (This)->lpVtbl -> get_Option(This,ppOptionFactory)

#define IPIEHTMLWindow3_setTimeout(This,pvtExpression,lMS,pvtLanguage,plTimerID)	\
    (This)->lpVtbl -> setTimeout(This,pvtExpression,lMS,pvtLanguage,plTimerID)

#define IPIEHTMLWindow3_scrollTo(This,lX,lY)	\
    (This)->lpVtbl -> scrollTo(This,lX,lY)


#define IPIEHTMLWindow3_get_external(This,ppExternalDispatch)	\
    (This)->lpVtbl -> get_external(This,ppExternalDispatch)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLWindow3_get_external_Proxy( 
    IPIEHTMLWindow3 __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppExternalDispatch);


void __RPC_STUB IPIEHTMLWindow3_get_external_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLWindow3_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLWindowEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLWindowEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLWindowEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLWindowEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE2102")
    PIEHTMLWindowEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLWindowEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLWindowEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLWindowEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLWindowEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLWindowEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLWindowEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLWindowEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLWindowEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLWindowEventsVtbl;

    interface PIEHTMLWindowEvents
    {
        CONST_VTBL struct PIEHTMLWindowEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLWindowEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLWindowEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLWindowEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLWindowEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLWindowEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLWindowEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLWindowEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLWindowEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2102")
PIEHTMLWindow;
#endif

#ifndef __IPIEHTMLElement2_INTERFACE_DEFINED__
#define __IPIEHTMLElement2_INTERFACE_DEFINED__

/* interface IPIEHTMLElement2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLElement2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1173fff4-5e35-442b-97dc-687f5cb1e89f")
    IPIEHTMLElement2 : public IDispatchEx
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetLeft( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetTop( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetWidth( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetHeight( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_offsetParent( 
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_tagName( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getAttribute( 
            /* [in] */ BSTR bstrAttributeName,
            /* [in][defaultvalue] */ LONG lFlags,
            /* [out][retval] */ VARIANT __RPC_FAR *pvarAttributeValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLElement2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLElement2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLElement2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetLeft )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetTop )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetWidth )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetHeight )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetParent )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_tagName )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAttribute )( 
            IPIEHTMLElement2 __RPC_FAR * This,
            /* [in] */ BSTR bstrAttributeName,
            /* [in][defaultvalue] */ LONG lFlags,
            /* [out][retval] */ VARIANT __RPC_FAR *pvarAttributeValue);
        
        END_INTERFACE
    } IPIEHTMLElement2Vtbl;

    interface IPIEHTMLElement2
    {
        CONST_VTBL struct IPIEHTMLElement2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLElement2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLElement2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLElement2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLElement2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLElement2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLElement2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLElement2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLElement2_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLElement2_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLElement2_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLElement2_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLElement2_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLElement2_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLElement2_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLElement2_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLElement2_get_offsetLeft(This,p)	\
    (This)->lpVtbl -> get_offsetLeft(This,p)

#define IPIEHTMLElement2_get_offsetTop(This,p)	\
    (This)->lpVtbl -> get_offsetTop(This,p)

#define IPIEHTMLElement2_get_offsetWidth(This,p)	\
    (This)->lpVtbl -> get_offsetWidth(This,p)

#define IPIEHTMLElement2_get_offsetHeight(This,p)	\
    (This)->lpVtbl -> get_offsetHeight(This,p)

#define IPIEHTMLElement2_get_offsetParent(This,p)	\
    (This)->lpVtbl -> get_offsetParent(This,p)

#define IPIEHTMLElement2_get_tagName(This,p)	\
    (This)->lpVtbl -> get_tagName(This,p)

#define IPIEHTMLElement2_getAttribute(This,bstrAttributeName,lFlags,pvarAttributeValue)	\
    (This)->lpVtbl -> getAttribute(This,bstrAttributeName,lFlags,pvarAttributeValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement2_get_offsetLeft_Proxy( 
    IPIEHTMLElement2 __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLElement2_get_offsetLeft_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement2_get_offsetTop_Proxy( 
    IPIEHTMLElement2 __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLElement2_get_offsetTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement2_get_offsetWidth_Proxy( 
    IPIEHTMLElement2 __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLElement2_get_offsetWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement2_get_offsetHeight_Proxy( 
    IPIEHTMLElement2 __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLElement2_get_offsetHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement2_get_offsetParent_Proxy( 
    IPIEHTMLElement2 __RPC_FAR * This,
    /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB IPIEHTMLElement2_get_offsetParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement2_get_tagName_Proxy( 
    IPIEHTMLElement2 __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLElement2_get_tagName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement2_getAttribute_Proxy( 
    IPIEHTMLElement2 __RPC_FAR * This,
    /* [in] */ BSTR bstrAttributeName,
    /* [in][defaultvalue] */ LONG lFlags,
    /* [out][retval] */ VARIANT __RPC_FAR *pvarAttributeValue);


void __RPC_STUB IPIEHTMLElement2_getAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLElement2_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLElement3_INTERFACE_DEFINED__
#define __IPIEHTMLElement3_INTERFACE_DEFINED__

/* interface IPIEHTMLElement3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLElement3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f6ceb128-db5e-450b-a730-b18aa18b8322")
    IPIEHTMLElement3 : public IPIEHTMLElement2
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE insertBefore( 
            /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
            /* [in][optional] */ VARIANT refChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispNewChild) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE removeChild( 
            /* [in] */ IDispatch __RPC_FAR *pdispOldChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOldChild) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE replaceChild( 
            /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
            /* [in] */ IDispatch __RPC_FAR *pdispOldChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOldChild) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE appendChild( 
            /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispNewChild) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_parentNode( 
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_childNodes( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_id( 
            /* [in] */ BSTR bstrID) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_className( 
            /* [in] */ BSTR bstrClassName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_className( 
            /* [out][retval] */ BSTR __RPC_FAR *pbstrClassName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLElement3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLElement3 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLElement3 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetLeft )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetTop )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetWidth )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetHeight )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_offsetParent )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_tagName )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getAttribute )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ BSTR bstrAttributeName,
            /* [in][defaultvalue] */ LONG lFlags,
            /* [out][retval] */ VARIANT __RPC_FAR *pvarAttributeValue);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *insertBefore )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
            /* [in][optional] */ VARIANT refChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispNewChild);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeChild )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdispOldChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOldChild);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *replaceChild )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
            /* [in] */ IDispatch __RPC_FAR *pdispOldChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOldChild);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *appendChild )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispNewChild);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentNode )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_childNodes )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_id )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ BSTR bstrID);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_className )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [in] */ BSTR bstrClassName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_className )( 
            IPIEHTMLElement3 __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *pbstrClassName);
        
        END_INTERFACE
    } IPIEHTMLElement3Vtbl;

    interface IPIEHTMLElement3
    {
        CONST_VTBL struct IPIEHTMLElement3Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLElement3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLElement3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLElement3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLElement3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLElement3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLElement3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLElement3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLElement3_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLElement3_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLElement3_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLElement3_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLElement3_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLElement3_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLElement3_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLElement3_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLElement3_get_offsetLeft(This,p)	\
    (This)->lpVtbl -> get_offsetLeft(This,p)

#define IPIEHTMLElement3_get_offsetTop(This,p)	\
    (This)->lpVtbl -> get_offsetTop(This,p)

#define IPIEHTMLElement3_get_offsetWidth(This,p)	\
    (This)->lpVtbl -> get_offsetWidth(This,p)

#define IPIEHTMLElement3_get_offsetHeight(This,p)	\
    (This)->lpVtbl -> get_offsetHeight(This,p)

#define IPIEHTMLElement3_get_offsetParent(This,p)	\
    (This)->lpVtbl -> get_offsetParent(This,p)

#define IPIEHTMLElement3_get_tagName(This,p)	\
    (This)->lpVtbl -> get_tagName(This,p)

#define IPIEHTMLElement3_getAttribute(This,bstrAttributeName,lFlags,pvarAttributeValue)	\
    (This)->lpVtbl -> getAttribute(This,bstrAttributeName,lFlags,pvarAttributeValue)


#define IPIEHTMLElement3_insertBefore(This,pdispNewChild,refChild,ppdispNewChild)	\
    (This)->lpVtbl -> insertBefore(This,pdispNewChild,refChild,ppdispNewChild)

#define IPIEHTMLElement3_removeChild(This,pdispOldChild,ppdispOldChild)	\
    (This)->lpVtbl -> removeChild(This,pdispOldChild,ppdispOldChild)

#define IPIEHTMLElement3_replaceChild(This,pdispNewChild,pdispOldChild,ppdispOldChild)	\
    (This)->lpVtbl -> replaceChild(This,pdispNewChild,pdispOldChild,ppdispOldChild)

#define IPIEHTMLElement3_appendChild(This,pdispNewChild,ppdispNewChild)	\
    (This)->lpVtbl -> appendChild(This,pdispNewChild,ppdispNewChild)

#define IPIEHTMLElement3_get_parentNode(This,ppParent)	\
    (This)->lpVtbl -> get_parentNode(This,ppParent)

#define IPIEHTMLElement3_get_childNodes(This,ppChildren)	\
    (This)->lpVtbl -> get_childNodes(This,ppChildren)

#define IPIEHTMLElement3_put_id(This,bstrID)	\
    (This)->lpVtbl -> put_id(This,bstrID)

#define IPIEHTMLElement3_put_className(This,bstrClassName)	\
    (This)->lpVtbl -> put_className(This,bstrClassName)

#define IPIEHTMLElement3_get_className(This,pbstrClassName)	\
    (This)->lpVtbl -> get_className(This,pbstrClassName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_insertBefore_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
    /* [in][optional] */ VARIANT refChild,
    /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispNewChild);


void __RPC_STUB IPIEHTMLElement3_insertBefore_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_removeChild_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdispOldChild,
    /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOldChild);


void __RPC_STUB IPIEHTMLElement3_removeChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_replaceChild_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
    /* [in] */ IDispatch __RPC_FAR *pdispOldChild,
    /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOldChild);


void __RPC_STUB IPIEHTMLElement3_replaceChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_appendChild_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdispNewChild,
    /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispNewChild);


void __RPC_STUB IPIEHTMLElement3_appendChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_get_parentNode_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);


void __RPC_STUB IPIEHTMLElement3_get_parentNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_get_childNodes_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);


void __RPC_STUB IPIEHTMLElement3_get_childNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_put_id_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [in] */ BSTR bstrID);


void __RPC_STUB IPIEHTMLElement3_put_id_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_put_className_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [in] */ BSTR bstrClassName);


void __RPC_STUB IPIEHTMLElement3_put_className_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLElement3_get_className_Proxy( 
    IPIEHTMLElement3 __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *pbstrClassName);


void __RPC_STUB IPIEHTMLElement3_get_className_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLElement3_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2268F6CE-EFF0-4226-BD0A-762B6C4F2769")
    PIEHTMLElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLElementEventsVtbl;

    interface PIEHTMLElementEvents
    {
        CONST_VTBL struct PIEHTMLElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLElement;

#ifdef __cplusplus

class DECLSPEC_UUID("28405462-5A46-453a-83DB-9E928855225C")
PIEHTMLElement;
#endif

#ifndef __IPIEHTMLFrameSetElement_INTERFACE_DEFINED__
#define __IPIEHTMLFrameSetElement_INTERFACE_DEFINED__

/* interface IPIEHTMLFrameSetElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLFrameSetElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2122")
    IPIEHTMLFrameSetElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_rows( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrRows) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_cols( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrCols) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_border( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBorder) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_borderColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBorderColor) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLFrameSetElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_rows )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrRows);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cols )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrCols);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_border )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBorder);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderColor )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtBorderColor);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLFrameSetElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        END_INTERFACE
    } IPIEHTMLFrameSetElementVtbl;

    interface IPIEHTMLFrameSetElement
    {
        CONST_VTBL struct IPIEHTMLFrameSetElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLFrameSetElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLFrameSetElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLFrameSetElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLFrameSetElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLFrameSetElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLFrameSetElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLFrameSetElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLFrameSetElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLFrameSetElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLFrameSetElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLFrameSetElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLFrameSetElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLFrameSetElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLFrameSetElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLFrameSetElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLFrameSetElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLFrameSetElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLFrameSetElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLFrameSetElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLFrameSetElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLFrameSetElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLFrameSetElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLFrameSetElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLFrameSetElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLFrameSetElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLFrameSetElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLFrameSetElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLFrameSetElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLFrameSetElement_get_rows(This,pbstrRows)	\
    (This)->lpVtbl -> get_rows(This,pbstrRows)

#define IPIEHTMLFrameSetElement_get_cols(This,pbstrCols)	\
    (This)->lpVtbl -> get_cols(This,pbstrCols)

#define IPIEHTMLFrameSetElement_get_border(This,pvtBorder)	\
    (This)->lpVtbl -> get_border(This,pvtBorder)

#define IPIEHTMLFrameSetElement_get_borderColor(This,pvtBorderColor)	\
    (This)->lpVtbl -> get_borderColor(This,pvtBorderColor)

#define IPIEHTMLFrameSetElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameSetElement_get_rows_Proxy( 
    IPIEHTMLFrameSetElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrRows);


void __RPC_STUB IPIEHTMLFrameSetElement_get_rows_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameSetElement_get_cols_Proxy( 
    IPIEHTMLFrameSetElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrCols);


void __RPC_STUB IPIEHTMLFrameSetElement_get_cols_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameSetElement_get_border_Proxy( 
    IPIEHTMLFrameSetElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtBorder);


void __RPC_STUB IPIEHTMLFrameSetElement_get_border_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameSetElement_get_borderColor_Proxy( 
    IPIEHTMLFrameSetElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtBorderColor);


void __RPC_STUB IPIEHTMLFrameSetElement_get_borderColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameSetElement_get_name_Proxy( 
    IPIEHTMLFrameSetElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IPIEHTMLFrameSetElement_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLFrameSetElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLFrameSetElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLFrameSetElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLFrameSetElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLFrameSetElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE2122")
    PIEHTMLFrameSetElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLFrameSetElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLFrameSetElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLFrameSetElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLFrameSetElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLFrameSetElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLFrameSetElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLFrameSetElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLFrameSetElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLFrameSetElementEventsVtbl;

    interface PIEHTMLFrameSetElementEvents
    {
        CONST_VTBL struct PIEHTMLFrameSetElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLFrameSetElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLFrameSetElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLFrameSetElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLFrameSetElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLFrameSetElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLFrameSetElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLFrameSetElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLFrameSetElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLFrameSetElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2122")
PIEHTMLFrameSetElement;
#endif

#ifndef __IPIEHTMLFrameElement_INTERFACE_DEFINED__
#define __IPIEHTMLFrameElement_INTERFACE_DEFINED__

/* interface IPIEHTMLFrameElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLFrameElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2121")
    IPIEHTMLFrameElement : public IPIEHTMLElement
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_src( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSRC) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_marginWidth( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtMarginWidth) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_marginHeight( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvtMarginHeight) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_noResize( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbNoResize) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_scrolling( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrScrolling) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLFrameElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLFrameElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLFrameElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_src )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSRC);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_marginWidth )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtMarginWidth);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_marginHeight )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvtMarginHeight);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_noResize )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbNoResize);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_scrolling )( 
            IPIEHTMLFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrScrolling);
        
        END_INTERFACE
    } IPIEHTMLFrameElementVtbl;

    interface IPIEHTMLFrameElement
    {
        CONST_VTBL struct IPIEHTMLFrameElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLFrameElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLFrameElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLFrameElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLFrameElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLFrameElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLFrameElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLFrameElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLFrameElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLFrameElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLFrameElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLFrameElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLFrameElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLFrameElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLFrameElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLFrameElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLFrameElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLFrameElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLFrameElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLFrameElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLFrameElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLFrameElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLFrameElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLFrameElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLFrameElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLFrameElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLFrameElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLFrameElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLFrameElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLFrameElement_get_src(This,pbstrSRC)	\
    (This)->lpVtbl -> get_src(This,pbstrSRC)

#define IPIEHTMLFrameElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLFrameElement_get_marginWidth(This,pvtMarginWidth)	\
    (This)->lpVtbl -> get_marginWidth(This,pvtMarginWidth)

#define IPIEHTMLFrameElement_get_marginHeight(This,pvtMarginHeight)	\
    (This)->lpVtbl -> get_marginHeight(This,pvtMarginHeight)

#define IPIEHTMLFrameElement_get_noResize(This,pvbNoResize)	\
    (This)->lpVtbl -> get_noResize(This,pvbNoResize)

#define IPIEHTMLFrameElement_get_scrolling(This,pbstrScrolling)	\
    (This)->lpVtbl -> get_scrolling(This,pbstrScrolling)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameElement_get_src_Proxy( 
    IPIEHTMLFrameElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrSRC);


void __RPC_STUB IPIEHTMLFrameElement_get_src_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameElement_get_name_Proxy( 
    IPIEHTMLFrameElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IPIEHTMLFrameElement_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameElement_get_marginWidth_Proxy( 
    IPIEHTMLFrameElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtMarginWidth);


void __RPC_STUB IPIEHTMLFrameElement_get_marginWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameElement_get_marginHeight_Proxy( 
    IPIEHTMLFrameElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvtMarginHeight);


void __RPC_STUB IPIEHTMLFrameElement_get_marginHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameElement_get_noResize_Proxy( 
    IPIEHTMLFrameElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbNoResize);


void __RPC_STUB IPIEHTMLFrameElement_get_noResize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFrameElement_get_scrolling_Proxy( 
    IPIEHTMLFrameElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrScrolling);


void __RPC_STUB IPIEHTMLFrameElement_get_scrolling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLFrameElement_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLFrameElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2121")
PIEHTMLFrameElement;
#endif

#ifndef __IPIEHTMLIFrameElement_INTERFACE_DEFINED__
#define __IPIEHTMLIFrameElement_INTERFACE_DEFINED__

/* interface IPIEHTMLIFrameElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLIFrameElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4A7DAB00-3210-4602-B13B-998E06CA8752")
    IPIEHTMLIFrameElement : public IPIEHTMLElement
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_vspace( 
            /* [in] */ long v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_vspace( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_hspace( 
            /* [in] */ long v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_hspace( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_align( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_align( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLIFrameElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLIFrameElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLIFrameElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vspace )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ long v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vspace )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hspace )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ long v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hspace )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_align )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_align )( 
            IPIEHTMLIFrameElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        END_INTERFACE
    } IPIEHTMLIFrameElementVtbl;

    interface IPIEHTMLIFrameElement
    {
        CONST_VTBL struct IPIEHTMLIFrameElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLIFrameElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLIFrameElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLIFrameElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLIFrameElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLIFrameElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLIFrameElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLIFrameElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLIFrameElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLIFrameElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLIFrameElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLIFrameElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLIFrameElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLIFrameElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLIFrameElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLIFrameElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLIFrameElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLIFrameElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLIFrameElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLIFrameElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLIFrameElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLIFrameElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLIFrameElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLIFrameElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLIFrameElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLIFrameElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLIFrameElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLIFrameElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLIFrameElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLIFrameElement_put_vspace(This,v)	\
    (This)->lpVtbl -> put_vspace(This,v)

#define IPIEHTMLIFrameElement_get_vspace(This,p)	\
    (This)->lpVtbl -> get_vspace(This,p)

#define IPIEHTMLIFrameElement_put_hspace(This,v)	\
    (This)->lpVtbl -> put_hspace(This,v)

#define IPIEHTMLIFrameElement_get_hspace(This,p)	\
    (This)->lpVtbl -> get_hspace(This,p)

#define IPIEHTMLIFrameElement_put_align(This,v)	\
    (This)->lpVtbl -> put_align(This,v)

#define IPIEHTMLIFrameElement_get_align(This,p)	\
    (This)->lpVtbl -> get_align(This,p)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLIFrameElement_put_vspace_Proxy( 
    IPIEHTMLIFrameElement __RPC_FAR * This,
    /* [in] */ long v);


void __RPC_STUB IPIEHTMLIFrameElement_put_vspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLIFrameElement_get_vspace_Proxy( 
    IPIEHTMLIFrameElement __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLIFrameElement_get_vspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLIFrameElement_put_hspace_Proxy( 
    IPIEHTMLIFrameElement __RPC_FAR * This,
    /* [in] */ long v);


void __RPC_STUB IPIEHTMLIFrameElement_put_hspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLIFrameElement_get_hspace_Proxy( 
    IPIEHTMLIFrameElement __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLIFrameElement_get_hspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLIFrameElement_put_align_Proxy( 
    IPIEHTMLIFrameElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLIFrameElement_put_align_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLIFrameElement_get_align_Proxy( 
    IPIEHTMLIFrameElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLIFrameElement_get_align_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLIFrameElement_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLIFrameElement;

#ifdef __cplusplus

class DECLSPEC_UUID("D3725B0B-1403-47df-9C8B-AB033B7D3E64")
PIEHTMLIFrameElement;
#endif

#ifndef __PIEHTMLImgElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLImgElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLImgElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLImgElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE2204")
    PIEHTMLImgElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLImgElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLImgElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLImgElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLImgElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLImgElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLImgElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLImgElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLImgElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLImgElementEventsVtbl;

    interface PIEHTMLImgElementEvents
    {
        CONST_VTBL struct PIEHTMLImgElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLImgElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLImgElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLImgElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLImgElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLImgElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLImgElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLImgElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLImgElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLImgElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2204")
PIEHTMLImgElement;
#endif

#ifndef __IPIEHTMLAnchorElement_INTERFACE_DEFINED__
#define __IPIEHTMLAnchorElement_INTERFACE_DEFINED__

/* interface IPIEHTMLAnchorElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLAnchorElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE210A")
    IPIEHTMLAnchorElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_href( 
            /* [in] */ BSTR bstrHREF) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_href( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHREF) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_target( 
            /* [in] */ BSTR bstrTarget) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_target( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_name( 
            /* [in] */ BSTR bstrTarget) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_accessKey( 
            /* [in] */ BSTR bstrTarget) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_accessKey( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_protocol( 
            /* [in] */ BSTR bstrProtocol) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_protocol( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrProtocol) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_host( 
            /* [in] */ BSTR bstrHost) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_host( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHost) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_hostname( 
            /* [in] */ BSTR bstrHostName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_hostname( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHostName) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_port( 
            /* [in] */ BSTR bstrPort) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_port( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPort) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_pathname( 
            /* [in] */ BSTR bstrPathName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_pathname( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPathName) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_search( 
            /* [in] */ BSTR bstrSearch) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_search( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSearch) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_hash( 
            /* [in] */ BSTR bstrHash) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_hash( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHash) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE focus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE blur( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE click( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLAnchorElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLAnchorElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLAnchorElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_href )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrHREF);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_href )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHREF);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_target )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrTarget);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_target )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_name )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrTarget);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrTarget);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_protocol )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrProtocol);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_protocol )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrProtocol);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_host )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrHost);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_host )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHost);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hostname )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrHostName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hostname )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHostName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_port )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrPort);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_port )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPort);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_pathname )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrPathName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_pathname )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrPathName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_search )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrSearch);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_search )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrSearch);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hash )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [in] */ BSTR bstrHash);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hash )( 
            IPIEHTMLAnchorElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrHash);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLAnchorElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLAnchorElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *click )( 
            IPIEHTMLAnchorElement __RPC_FAR * This);
        
        END_INTERFACE
    } IPIEHTMLAnchorElementVtbl;

    interface IPIEHTMLAnchorElement
    {
        CONST_VTBL struct IPIEHTMLAnchorElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLAnchorElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLAnchorElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLAnchorElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLAnchorElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLAnchorElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLAnchorElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLAnchorElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLAnchorElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLAnchorElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLAnchorElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLAnchorElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLAnchorElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLAnchorElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLAnchorElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLAnchorElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLAnchorElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLAnchorElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLAnchorElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLAnchorElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLAnchorElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLAnchorElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLAnchorElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLAnchorElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLAnchorElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLAnchorElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLAnchorElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLAnchorElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLAnchorElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLAnchorElement_put_href(This,bstrHREF)	\
    (This)->lpVtbl -> put_href(This,bstrHREF)

#define IPIEHTMLAnchorElement_get_href(This,pbstrHREF)	\
    (This)->lpVtbl -> get_href(This,pbstrHREF)

#define IPIEHTMLAnchorElement_put_target(This,bstrTarget)	\
    (This)->lpVtbl -> put_target(This,bstrTarget)

#define IPIEHTMLAnchorElement_get_target(This,pbstrTarget)	\
    (This)->lpVtbl -> get_target(This,pbstrTarget)

#define IPIEHTMLAnchorElement_put_name(This,bstrTarget)	\
    (This)->lpVtbl -> put_name(This,bstrTarget)

#define IPIEHTMLAnchorElement_get_name(This,pbstrTarget)	\
    (This)->lpVtbl -> get_name(This,pbstrTarget)

#define IPIEHTMLAnchorElement_put_accessKey(This,bstrTarget)	\
    (This)->lpVtbl -> put_accessKey(This,bstrTarget)

#define IPIEHTMLAnchorElement_get_accessKey(This,pbstrTarget)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrTarget)

#define IPIEHTMLAnchorElement_put_protocol(This,bstrProtocol)	\
    (This)->lpVtbl -> put_protocol(This,bstrProtocol)

#define IPIEHTMLAnchorElement_get_protocol(This,pbstrProtocol)	\
    (This)->lpVtbl -> get_protocol(This,pbstrProtocol)

#define IPIEHTMLAnchorElement_put_host(This,bstrHost)	\
    (This)->lpVtbl -> put_host(This,bstrHost)

#define IPIEHTMLAnchorElement_get_host(This,pbstrHost)	\
    (This)->lpVtbl -> get_host(This,pbstrHost)

#define IPIEHTMLAnchorElement_put_hostname(This,bstrHostName)	\
    (This)->lpVtbl -> put_hostname(This,bstrHostName)

#define IPIEHTMLAnchorElement_get_hostname(This,pbstrHostName)	\
    (This)->lpVtbl -> get_hostname(This,pbstrHostName)

#define IPIEHTMLAnchorElement_put_port(This,bstrPort)	\
    (This)->lpVtbl -> put_port(This,bstrPort)

#define IPIEHTMLAnchorElement_get_port(This,pbstrPort)	\
    (This)->lpVtbl -> get_port(This,pbstrPort)

#define IPIEHTMLAnchorElement_put_pathname(This,bstrPathName)	\
    (This)->lpVtbl -> put_pathname(This,bstrPathName)

#define IPIEHTMLAnchorElement_get_pathname(This,pbstrPathName)	\
    (This)->lpVtbl -> get_pathname(This,pbstrPathName)

#define IPIEHTMLAnchorElement_put_search(This,bstrSearch)	\
    (This)->lpVtbl -> put_search(This,bstrSearch)

#define IPIEHTMLAnchorElement_get_search(This,pbstrSearch)	\
    (This)->lpVtbl -> get_search(This,pbstrSearch)

#define IPIEHTMLAnchorElement_put_hash(This,bstrHash)	\
    (This)->lpVtbl -> put_hash(This,bstrHash)

#define IPIEHTMLAnchorElement_get_hash(This,pbstrHash)	\
    (This)->lpVtbl -> get_hash(This,pbstrHash)

#define IPIEHTMLAnchorElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLAnchorElement_blur(This)	\
    (This)->lpVtbl -> blur(This)

#define IPIEHTMLAnchorElement_click(This)	\
    (This)->lpVtbl -> click(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_href_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrHREF);


void __RPC_STUB IPIEHTMLAnchorElement_put_href_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_href_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHREF);


void __RPC_STUB IPIEHTMLAnchorElement_get_href_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_target_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrTarget);


void __RPC_STUB IPIEHTMLAnchorElement_put_target_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_target_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);


void __RPC_STUB IPIEHTMLAnchorElement_get_target_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_name_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrTarget);


void __RPC_STUB IPIEHTMLAnchorElement_put_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_name_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);


void __RPC_STUB IPIEHTMLAnchorElement_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_accessKey_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrTarget);


void __RPC_STUB IPIEHTMLAnchorElement_put_accessKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_accessKey_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);


void __RPC_STUB IPIEHTMLAnchorElement_get_accessKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_protocol_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrProtocol);


void __RPC_STUB IPIEHTMLAnchorElement_put_protocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_protocol_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrProtocol);


void __RPC_STUB IPIEHTMLAnchorElement_get_protocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_host_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrHost);


void __RPC_STUB IPIEHTMLAnchorElement_put_host_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_host_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHost);


void __RPC_STUB IPIEHTMLAnchorElement_get_host_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_hostname_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrHostName);


void __RPC_STUB IPIEHTMLAnchorElement_put_hostname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_hostname_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHostName);


void __RPC_STUB IPIEHTMLAnchorElement_get_hostname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_port_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrPort);


void __RPC_STUB IPIEHTMLAnchorElement_put_port_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_port_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrPort);


void __RPC_STUB IPIEHTMLAnchorElement_get_port_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_pathname_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrPathName);


void __RPC_STUB IPIEHTMLAnchorElement_put_pathname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_pathname_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrPathName);


void __RPC_STUB IPIEHTMLAnchorElement_get_pathname_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_search_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrSearch);


void __RPC_STUB IPIEHTMLAnchorElement_put_search_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_search_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrSearch);


void __RPC_STUB IPIEHTMLAnchorElement_get_search_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_put_hash_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [in] */ BSTR bstrHash);


void __RPC_STUB IPIEHTMLAnchorElement_put_hash_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_get_hash_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrHash);


void __RPC_STUB IPIEHTMLAnchorElement_get_hash_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_focus_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLAnchorElement_focus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_blur_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLAnchorElement_blur_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLAnchorElement_click_Proxy( 
    IPIEHTMLAnchorElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLAnchorElement_click_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLAnchorElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLAnchorElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLAnchorElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLAnchorElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLAnchorElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE210A")
    PIEHTMLAnchorElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLAnchorElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLAnchorElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLAnchorElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLAnchorElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLAnchorElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLAnchorElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLAnchorElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLAnchorElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLAnchorElementEventsVtbl;

    interface PIEHTMLAnchorElementEvents
    {
        CONST_VTBL struct PIEHTMLAnchorElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLAnchorElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLAnchorElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLAnchorElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLAnchorElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLAnchorElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLAnchorElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLAnchorElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLAnchorElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLAnchorElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE210A")
PIEHTMLAnchorElement;
#endif

#ifndef __IPIEHTMLFormElement_INTERFACE_DEFINED__
#define __IPIEHTMLFormElement_INTERFACE_DEFINED__

/* interface IPIEHTMLFormElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLFormElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE210C")
    IPIEHTMLFormElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_action( 
            /* [in] */ BSTR bstrAction) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_action( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAction) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_encoding( 
            /* [in] */ BSTR bstrEncoding) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_encoding( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrEncoding) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_method( 
            /* [in] */ BSTR bstrMethod) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_method( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_target( 
            /* [in] */ BSTR bstrTarget) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_target( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_elements( 
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *plLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE submit( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE reset( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLFormElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLFormElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLFormElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_action )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAction);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_action )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAction);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_encoding )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrEncoding);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_encoding )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrEncoding);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_method )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrMethod);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_method )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_target )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [in] */ BSTR bstrTarget);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_target )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_elements )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            IPIEHTMLFormElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *submit )( 
            IPIEHTMLFormElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *reset )( 
            IPIEHTMLFormElement __RPC_FAR * This);
        
        END_INTERFACE
    } IPIEHTMLFormElementVtbl;

    interface IPIEHTMLFormElement
    {
        CONST_VTBL struct IPIEHTMLFormElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLFormElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLFormElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLFormElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLFormElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLFormElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLFormElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLFormElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLFormElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLFormElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLFormElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLFormElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLFormElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLFormElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLFormElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLFormElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLFormElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLFormElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLFormElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLFormElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLFormElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLFormElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLFormElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLFormElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLFormElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLFormElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLFormElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLFormElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLFormElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLFormElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLFormElement_put_action(This,bstrAction)	\
    (This)->lpVtbl -> put_action(This,bstrAction)

#define IPIEHTMLFormElement_get_action(This,pbstrAction)	\
    (This)->lpVtbl -> get_action(This,pbstrAction)

#define IPIEHTMLFormElement_put_encoding(This,bstrEncoding)	\
    (This)->lpVtbl -> put_encoding(This,bstrEncoding)

#define IPIEHTMLFormElement_get_encoding(This,pbstrEncoding)	\
    (This)->lpVtbl -> get_encoding(This,pbstrEncoding)

#define IPIEHTMLFormElement_put_method(This,bstrMethod)	\
    (This)->lpVtbl -> put_method(This,bstrMethod)

#define IPIEHTMLFormElement_get_method(This,pbstrMethod)	\
    (This)->lpVtbl -> get_method(This,pbstrMethod)

#define IPIEHTMLFormElement_put_target(This,bstrTarget)	\
    (This)->lpVtbl -> put_target(This,bstrTarget)

#define IPIEHTMLFormElement_get_target(This,pbstrTarget)	\
    (This)->lpVtbl -> get_target(This,pbstrTarget)

#define IPIEHTMLFormElement_get_elements(This,ppElemCollection)	\
    (This)->lpVtbl -> get_elements(This,ppElemCollection)

#define IPIEHTMLFormElement_get_length(This,plLength)	\
    (This)->lpVtbl -> get_length(This,plLength)

#define IPIEHTMLFormElement_submit(This)	\
    (This)->lpVtbl -> submit(This)

#define IPIEHTMLFormElement_reset(This)	\
    (This)->lpVtbl -> reset(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_get_name_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IPIEHTMLFormElement_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_put_action_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [in] */ BSTR bstrAction);


void __RPC_STUB IPIEHTMLFormElement_put_action_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_get_action_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrAction);


void __RPC_STUB IPIEHTMLFormElement_get_action_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_put_encoding_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [in] */ BSTR bstrEncoding);


void __RPC_STUB IPIEHTMLFormElement_put_encoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_get_encoding_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrEncoding);


void __RPC_STUB IPIEHTMLFormElement_get_encoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_put_method_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [in] */ BSTR bstrMethod);


void __RPC_STUB IPIEHTMLFormElement_put_method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_get_method_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod);


void __RPC_STUB IPIEHTMLFormElement_get_method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_put_target_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [in] */ BSTR bstrTarget);


void __RPC_STUB IPIEHTMLFormElement_put_target_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_get_target_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrTarget);


void __RPC_STUB IPIEHTMLFormElement_get_target_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_get_elements_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppElemCollection);


void __RPC_STUB IPIEHTMLFormElement_get_elements_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_get_length_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLength);


void __RPC_STUB IPIEHTMLFormElement_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_submit_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLFormElement_submit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLFormElement_reset_Proxy( 
    IPIEHTMLFormElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLFormElement_reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLFormElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLFormElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLFormElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLFormElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLFormElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE210C")
    PIEHTMLFormElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLFormElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLFormElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLFormElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLFormElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLFormElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLFormElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLFormElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLFormElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLFormElementEventsVtbl;

    interface PIEHTMLFormElementEvents
    {
        CONST_VTBL struct PIEHTMLFormElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLFormElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLFormElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLFormElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLFormElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLFormElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLFormElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLFormElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLFormElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLFormElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE210C")
PIEHTMLFormElement;
#endif

#ifndef __IPIEHTMLControlElement_INTERFACE_DEFINED__
#define __IPIEHTMLControlElement_INTERFACE_DEFINED__

/* interface IPIEHTMLControlElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLControlElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2106")
    IPIEHTMLControlElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_type( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_value( 
            /* [in] */ BSTR bstrValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_value( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_accessKey( 
            /* [in] */ BSTR bstrAccesskey) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_accessKey( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE focus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE blur( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLControlElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLControlElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLControlElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAccesskey);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLControlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLControlElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLControlElement __RPC_FAR * This);
        
        END_INTERFACE
    } IPIEHTMLControlElementVtbl;

    interface IPIEHTMLControlElement
    {
        CONST_VTBL struct IPIEHTMLControlElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLControlElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLControlElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLControlElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLControlElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLControlElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLControlElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLControlElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLControlElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLControlElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLControlElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLControlElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLControlElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLControlElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLControlElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLControlElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLControlElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLControlElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLControlElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLControlElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLControlElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLControlElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLControlElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLControlElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLControlElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLControlElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLControlElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLControlElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLControlElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLControlElement_get_type(This,pbstrType)	\
    (This)->lpVtbl -> get_type(This,pbstrType)

#define IPIEHTMLControlElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLControlElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLControlElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLControlElement_put_accessKey(This,bstrAccesskey)	\
    (This)->lpVtbl -> put_accessKey(This,bstrAccesskey)

#define IPIEHTMLControlElement_get_accessKey(This,pbstrAccesskey)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrAccesskey)

#define IPIEHTMLControlElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLControlElement_blur(This)	\
    (This)->lpVtbl -> blur(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_get_type_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrType);


void __RPC_STUB IPIEHTMLControlElement_get_type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_put_value_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This,
    /* [in] */ BSTR bstrValue);


void __RPC_STUB IPIEHTMLControlElement_put_value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_get_value_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);


void __RPC_STUB IPIEHTMLControlElement_get_value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_get_name_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrName);


void __RPC_STUB IPIEHTMLControlElement_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_put_accessKey_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This,
    /* [in] */ BSTR bstrAccesskey);


void __RPC_STUB IPIEHTMLControlElement_put_accessKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_get_accessKey_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);


void __RPC_STUB IPIEHTMLControlElement_get_accessKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_focus_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLControlElement_focus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLControlElement_blur_Proxy( 
    IPIEHTMLControlElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLControlElement_blur_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLControlElement_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLControlElement;

#ifdef __cplusplus

class DECLSPEC_UUID("555C7786-BA95-11D0-81F1-00A0C90AD21A")
PIEHTMLControlElement;
#endif

#ifndef __IPIEHTMLInputButtonElement_INTERFACE_DEFINED__
#define __IPIEHTMLInputButtonElement_INTERFACE_DEFINED__

/* interface IPIEHTMLInputButtonElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLInputButtonElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2108")
    IPIEHTMLInputButtonElement : public IPIEHTMLControlElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_disabled( 
            /* [in] */ VARIANT_BOOL vbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_disabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_form( 
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE click( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLInputButtonElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAccesskey);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_disabled )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_disabled )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_form )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *click )( 
            IPIEHTMLInputButtonElement __RPC_FAR * This);
        
        END_INTERFACE
    } IPIEHTMLInputButtonElementVtbl;

    interface IPIEHTMLInputButtonElement
    {
        CONST_VTBL struct IPIEHTMLInputButtonElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLInputButtonElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLInputButtonElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLInputButtonElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLInputButtonElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLInputButtonElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLInputButtonElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLInputButtonElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLInputButtonElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLInputButtonElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLInputButtonElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLInputButtonElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLInputButtonElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLInputButtonElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLInputButtonElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLInputButtonElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLInputButtonElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLInputButtonElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLInputButtonElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLInputButtonElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLInputButtonElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLInputButtonElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLInputButtonElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLInputButtonElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLInputButtonElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLInputButtonElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLInputButtonElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLInputButtonElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLInputButtonElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLInputButtonElement_get_type(This,pbstrType)	\
    (This)->lpVtbl -> get_type(This,pbstrType)

#define IPIEHTMLInputButtonElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLInputButtonElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLInputButtonElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLInputButtonElement_put_accessKey(This,bstrAccesskey)	\
    (This)->lpVtbl -> put_accessKey(This,bstrAccesskey)

#define IPIEHTMLInputButtonElement_get_accessKey(This,pbstrAccesskey)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrAccesskey)

#define IPIEHTMLInputButtonElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLInputButtonElement_blur(This)	\
    (This)->lpVtbl -> blur(This)


#define IPIEHTMLInputButtonElement_put_disabled(This,vbDisabled)	\
    (This)->lpVtbl -> put_disabled(This,vbDisabled)

#define IPIEHTMLInputButtonElement_get_disabled(This,pvbDisabled)	\
    (This)->lpVtbl -> get_disabled(This,pvbDisabled)

#define IPIEHTMLInputButtonElement_get_form(This,ppForm)	\
    (This)->lpVtbl -> get_form(This,ppForm)

#define IPIEHTMLInputButtonElement_click(This)	\
    (This)->lpVtbl -> click(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputButtonElement_put_disabled_Proxy( 
    IPIEHTMLInputButtonElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbDisabled);


void __RPC_STUB IPIEHTMLInputButtonElement_put_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputButtonElement_get_disabled_Proxy( 
    IPIEHTMLInputButtonElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);


void __RPC_STUB IPIEHTMLInputButtonElement_get_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputButtonElement_get_form_Proxy( 
    IPIEHTMLInputButtonElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);


void __RPC_STUB IPIEHTMLInputButtonElement_get_form_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLInputButtonElement_click_Proxy( 
    IPIEHTMLInputButtonElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLInputButtonElement_click_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLInputButtonElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLInputButtonElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLInputButtonElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLInputButtonElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLInputButtonElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE2108")
    PIEHTMLInputButtonElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLInputButtonElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLInputButtonElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLInputButtonElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLInputButtonElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLInputButtonElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLInputButtonElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLInputButtonElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLInputButtonElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLInputButtonElementEventsVtbl;

    interface PIEHTMLInputButtonElementEvents
    {
        CONST_VTBL struct PIEHTMLInputButtonElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLInputButtonElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLInputButtonElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLInputButtonElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLInputButtonElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLInputButtonElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLInputButtonElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLInputButtonElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLInputButtonElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLInputButtonElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2108")
PIEHTMLInputButtonElement;
#endif

#ifndef __IPIEHTMLOptionButtonElement_INTERFACE_DEFINED__
#define __IPIEHTMLOptionButtonElement_INTERFACE_DEFINED__

/* interface IPIEHTMLOptionButtonElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLOptionButtonElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2110")
    IPIEHTMLOptionButtonElement : public IPIEHTMLControlElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_disabled( 
            /* [in] */ VARIANT_BOOL vbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_disabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_form( 
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_checked( 
            /* [in] */ VARIANT_BOOL vbChecked) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_checked( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbChecked) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_defaultChecked( 
            /* [in] */ VARIANT_BOOL vbDefaultChecked) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_defaultChecked( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDefaultChecked) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_indeterminate( 
            /* [in] */ VARIANT_BOOL vbIndeterminate) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_indeterminate( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIndeterminate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE click( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLOptionButtonElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAccesskey);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_disabled )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_disabled )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_form )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_checked )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbChecked);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_checked )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbChecked);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_defaultChecked )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDefaultChecked);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_defaultChecked )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDefaultChecked);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_indeterminate )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbIndeterminate);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_indeterminate )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIndeterminate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *click )( 
            IPIEHTMLOptionButtonElement __RPC_FAR * This);
        
        END_INTERFACE
    } IPIEHTMLOptionButtonElementVtbl;

    interface IPIEHTMLOptionButtonElement
    {
        CONST_VTBL struct IPIEHTMLOptionButtonElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLOptionButtonElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLOptionButtonElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLOptionButtonElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLOptionButtonElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLOptionButtonElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLOptionButtonElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLOptionButtonElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLOptionButtonElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLOptionButtonElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLOptionButtonElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLOptionButtonElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLOptionButtonElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLOptionButtonElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLOptionButtonElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLOptionButtonElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLOptionButtonElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLOptionButtonElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLOptionButtonElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLOptionButtonElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLOptionButtonElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLOptionButtonElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLOptionButtonElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLOptionButtonElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLOptionButtonElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLOptionButtonElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLOptionButtonElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLOptionButtonElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLOptionButtonElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLOptionButtonElement_get_type(This,pbstrType)	\
    (This)->lpVtbl -> get_type(This,pbstrType)

#define IPIEHTMLOptionButtonElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLOptionButtonElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLOptionButtonElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLOptionButtonElement_put_accessKey(This,bstrAccesskey)	\
    (This)->lpVtbl -> put_accessKey(This,bstrAccesskey)

#define IPIEHTMLOptionButtonElement_get_accessKey(This,pbstrAccesskey)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrAccesskey)

#define IPIEHTMLOptionButtonElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLOptionButtonElement_blur(This)	\
    (This)->lpVtbl -> blur(This)


#define IPIEHTMLOptionButtonElement_put_disabled(This,vbDisabled)	\
    (This)->lpVtbl -> put_disabled(This,vbDisabled)

#define IPIEHTMLOptionButtonElement_get_disabled(This,pvbDisabled)	\
    (This)->lpVtbl -> get_disabled(This,pvbDisabled)

#define IPIEHTMLOptionButtonElement_get_form(This,ppForm)	\
    (This)->lpVtbl -> get_form(This,ppForm)

#define IPIEHTMLOptionButtonElement_put_checked(This,vbChecked)	\
    (This)->lpVtbl -> put_checked(This,vbChecked)

#define IPIEHTMLOptionButtonElement_get_checked(This,pvbChecked)	\
    (This)->lpVtbl -> get_checked(This,pvbChecked)

#define IPIEHTMLOptionButtonElement_put_defaultChecked(This,vbDefaultChecked)	\
    (This)->lpVtbl -> put_defaultChecked(This,vbDefaultChecked)

#define IPIEHTMLOptionButtonElement_get_defaultChecked(This,pvbDefaultChecked)	\
    (This)->lpVtbl -> get_defaultChecked(This,pvbDefaultChecked)

#define IPIEHTMLOptionButtonElement_put_indeterminate(This,vbIndeterminate)	\
    (This)->lpVtbl -> put_indeterminate(This,vbIndeterminate)

#define IPIEHTMLOptionButtonElement_get_indeterminate(This,pvbIndeterminate)	\
    (This)->lpVtbl -> get_indeterminate(This,pvbIndeterminate)

#define IPIEHTMLOptionButtonElement_click(This)	\
    (This)->lpVtbl -> click(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_put_disabled_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbDisabled);


void __RPC_STUB IPIEHTMLOptionButtonElement_put_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_get_disabled_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);


void __RPC_STUB IPIEHTMLOptionButtonElement_get_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_get_form_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);


void __RPC_STUB IPIEHTMLOptionButtonElement_get_form_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_put_checked_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbChecked);


void __RPC_STUB IPIEHTMLOptionButtonElement_put_checked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_get_checked_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbChecked);


void __RPC_STUB IPIEHTMLOptionButtonElement_get_checked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_put_defaultChecked_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbDefaultChecked);


void __RPC_STUB IPIEHTMLOptionButtonElement_put_defaultChecked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_get_defaultChecked_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDefaultChecked);


void __RPC_STUB IPIEHTMLOptionButtonElement_get_defaultChecked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_put_indeterminate_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbIndeterminate);


void __RPC_STUB IPIEHTMLOptionButtonElement_put_indeterminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_get_indeterminate_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbIndeterminate);


void __RPC_STUB IPIEHTMLOptionButtonElement_get_indeterminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLOptionButtonElement_click_Proxy( 
    IPIEHTMLOptionButtonElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLOptionButtonElement_click_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLOptionButtonElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLOptionButtonElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLOptionButtonElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLOptionButtonElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLOptionButtonElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E0E269F3-87A7-45f8-9200-DDAA2940F78D")
    PIEHTMLOptionButtonElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLOptionButtonElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLOptionButtonElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLOptionButtonElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLOptionButtonElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLOptionButtonElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLOptionButtonElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLOptionButtonElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLOptionButtonElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLOptionButtonElementEventsVtbl;

    interface PIEHTMLOptionButtonElementEvents
    {
        CONST_VTBL struct PIEHTMLOptionButtonElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLOptionButtonElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLOptionButtonElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLOptionButtonElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLOptionButtonElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLOptionButtonElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLOptionButtonElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLOptionButtonElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLOptionButtonElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLOptionButtonElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2110")
PIEHTMLOptionButtonElement;
#endif

#ifndef __IPIEHTMLSelectElement_INTERFACE_DEFINED__
#define __IPIEHTMLSelectElement_INTERFACE_DEFINED__

/* interface IPIEHTMLSelectElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLSelectElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE210F")
    IPIEHTMLSelectElement : public IPIEHTMLControlElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_disabled( 
            /* [in] */ VARIANT_BOOL vbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_disabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_form( 
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_size( 
            /* [retval][out] */ long __RPC_FAR *plSize) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_multiple( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbMultiple) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_options( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOptions) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_selectedIndex( 
            /* [in] */ long lSelectedIndex) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_selectedIndex( 
            /* [retval][out] */ long __RPC_FAR *plSelectedIndex) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_length( 
            /* [in] */ long lLength) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_length( 
            /* [retval][out] */ long __RPC_FAR *plLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add( 
            /* [in] */ IPIEHTMLOptionElement __RPC_FAR *element,
            /* [in][optional] */ VARIANT before) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove( 
            /* [in][defaultvalue] */ long index = -1) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLSelectElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLSelectElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLSelectElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAccesskey);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLSelectElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLSelectElement __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_disabled )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_disabled )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_form )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_size )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plSize);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_multiple )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbMultiple);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_options )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOptions);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_selectedIndex )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ long lSelectedIndex);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_selectedIndex )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plSelectedIndex);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_length )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ long lLength);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_length )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLength);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *add )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in] */ IPIEHTMLOptionElement __RPC_FAR *element,
            /* [in][optional] */ VARIANT before);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *remove )( 
            IPIEHTMLSelectElement __RPC_FAR * This,
            /* [in][defaultvalue] */ long index);
        
        END_INTERFACE
    } IPIEHTMLSelectElementVtbl;

    interface IPIEHTMLSelectElement
    {
        CONST_VTBL struct IPIEHTMLSelectElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLSelectElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLSelectElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLSelectElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLSelectElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLSelectElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLSelectElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLSelectElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLSelectElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLSelectElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLSelectElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLSelectElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLSelectElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLSelectElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLSelectElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLSelectElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLSelectElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLSelectElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLSelectElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLSelectElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLSelectElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLSelectElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLSelectElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLSelectElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLSelectElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLSelectElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLSelectElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLSelectElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLSelectElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLSelectElement_get_type(This,pbstrType)	\
    (This)->lpVtbl -> get_type(This,pbstrType)

#define IPIEHTMLSelectElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLSelectElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLSelectElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLSelectElement_put_accessKey(This,bstrAccesskey)	\
    (This)->lpVtbl -> put_accessKey(This,bstrAccesskey)

#define IPIEHTMLSelectElement_get_accessKey(This,pbstrAccesskey)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrAccesskey)

#define IPIEHTMLSelectElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLSelectElement_blur(This)	\
    (This)->lpVtbl -> blur(This)


#define IPIEHTMLSelectElement_put_disabled(This,vbDisabled)	\
    (This)->lpVtbl -> put_disabled(This,vbDisabled)

#define IPIEHTMLSelectElement_get_disabled(This,pvbDisabled)	\
    (This)->lpVtbl -> get_disabled(This,pvbDisabled)

#define IPIEHTMLSelectElement_get_form(This,ppForm)	\
    (This)->lpVtbl -> get_form(This,ppForm)

#define IPIEHTMLSelectElement_get_size(This,plSize)	\
    (This)->lpVtbl -> get_size(This,plSize)

#define IPIEHTMLSelectElement_get_multiple(This,pvbMultiple)	\
    (This)->lpVtbl -> get_multiple(This,pvbMultiple)

#define IPIEHTMLSelectElement_get_options(This,ppdispOptions)	\
    (This)->lpVtbl -> get_options(This,ppdispOptions)

#define IPIEHTMLSelectElement_put_selectedIndex(This,lSelectedIndex)	\
    (This)->lpVtbl -> put_selectedIndex(This,lSelectedIndex)

#define IPIEHTMLSelectElement_get_selectedIndex(This,plSelectedIndex)	\
    (This)->lpVtbl -> get_selectedIndex(This,plSelectedIndex)

#define IPIEHTMLSelectElement_put_length(This,lLength)	\
    (This)->lpVtbl -> put_length(This,lLength)

#define IPIEHTMLSelectElement_get_length(This,plLength)	\
    (This)->lpVtbl -> get_length(This,plLength)

#define IPIEHTMLSelectElement_add(This,element,before)	\
    (This)->lpVtbl -> add(This,element,before)

#define IPIEHTMLSelectElement_remove(This,index)	\
    (This)->lpVtbl -> remove(This,index)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_put_disabled_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbDisabled);


void __RPC_STUB IPIEHTMLSelectElement_put_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_get_disabled_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);


void __RPC_STUB IPIEHTMLSelectElement_get_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_get_form_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);


void __RPC_STUB IPIEHTMLSelectElement_get_form_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_get_size_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plSize);


void __RPC_STUB IPIEHTMLSelectElement_get_size_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_get_multiple_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbMultiple);


void __RPC_STUB IPIEHTMLSelectElement_get_multiple_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_get_options_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispOptions);


void __RPC_STUB IPIEHTMLSelectElement_get_options_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_put_selectedIndex_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [in] */ long lSelectedIndex);


void __RPC_STUB IPIEHTMLSelectElement_put_selectedIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_get_selectedIndex_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plSelectedIndex);


void __RPC_STUB IPIEHTMLSelectElement_get_selectedIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_put_length_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [in] */ long lLength);


void __RPC_STUB IPIEHTMLSelectElement_put_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_get_length_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLength);


void __RPC_STUB IPIEHTMLSelectElement_get_length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_add_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [in] */ IPIEHTMLOptionElement __RPC_FAR *element,
    /* [in][optional] */ VARIANT before);


void __RPC_STUB IPIEHTMLSelectElement_add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLSelectElement_remove_Proxy( 
    IPIEHTMLSelectElement __RPC_FAR * This,
    /* [in][defaultvalue] */ long index);


void __RPC_STUB IPIEHTMLSelectElement_remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLSelectElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLSelectElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLSelectElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLSelectElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLSelectElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE210F")
    PIEHTMLSelectElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLSelectElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLSelectElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLSelectElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLSelectElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLSelectElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLSelectElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLSelectElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLSelectElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLSelectElementEventsVtbl;

    interface PIEHTMLSelectElementEvents
    {
        CONST_VTBL struct PIEHTMLSelectElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLSelectElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLSelectElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLSelectElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLSelectElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLSelectElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLSelectElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLSelectElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLSelectElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLSelectElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE210F")
PIEHTMLSelectElement;
#endif

EXTERN_C const CLSID CLSID_PIEHTMLOptionElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE210B")
PIEHTMLOptionElement;
#endif

#ifndef __IPIEHTMLInputTextElement_INTERFACE_DEFINED__
#define __IPIEHTMLInputTextElement_INTERFACE_DEFINED__

/* interface IPIEHTMLInputTextElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLInputTextElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE210D")
    IPIEHTMLInputTextElement : public IPIEHTMLControlElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_disabled( 
            /* [in] */ VARIANT_BOOL vbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_disabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_form( 
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_defaultValue( 
            /* [in] */ BSTR bstrDefaultValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_defaultValue( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDefaultValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_size( 
            /* [retval][out] */ long __RPC_FAR *plSize) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_maxLength( 
            /* [in] */ long lMaxLength) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_maxLength( 
            /* [retval][out] */ long __RPC_FAR *plMaxLength) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_readOnly( 
            /* [in] */ VARIANT_BOOL vbReadOnly) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_readOnly( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbReadOnly) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE select( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLInputTextElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLInputTextElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLInputTextElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAccesskey);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLInputTextElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLInputTextElement __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_disabled )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_disabled )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_form )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_defaultValue )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ BSTR bstrDefaultValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_defaultValue )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDefaultValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_size )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plSize);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_maxLength )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ long lMaxLength);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_maxLength )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plMaxLength);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_readOnly )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbReadOnly);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_readOnly )( 
            IPIEHTMLInputTextElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbReadOnly);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *select )( 
            IPIEHTMLInputTextElement __RPC_FAR * This);
        
        END_INTERFACE
    } IPIEHTMLInputTextElementVtbl;

    interface IPIEHTMLInputTextElement
    {
        CONST_VTBL struct IPIEHTMLInputTextElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLInputTextElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLInputTextElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLInputTextElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLInputTextElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLInputTextElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLInputTextElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLInputTextElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLInputTextElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLInputTextElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLInputTextElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLInputTextElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLInputTextElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLInputTextElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLInputTextElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLInputTextElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLInputTextElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLInputTextElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLInputTextElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLInputTextElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLInputTextElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLInputTextElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLInputTextElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLInputTextElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLInputTextElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLInputTextElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLInputTextElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLInputTextElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLInputTextElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLInputTextElement_get_type(This,pbstrType)	\
    (This)->lpVtbl -> get_type(This,pbstrType)

#define IPIEHTMLInputTextElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLInputTextElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLInputTextElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLInputTextElement_put_accessKey(This,bstrAccesskey)	\
    (This)->lpVtbl -> put_accessKey(This,bstrAccesskey)

#define IPIEHTMLInputTextElement_get_accessKey(This,pbstrAccesskey)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrAccesskey)

#define IPIEHTMLInputTextElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLInputTextElement_blur(This)	\
    (This)->lpVtbl -> blur(This)


#define IPIEHTMLInputTextElement_put_disabled(This,vbDisabled)	\
    (This)->lpVtbl -> put_disabled(This,vbDisabled)

#define IPIEHTMLInputTextElement_get_disabled(This,pvbDisabled)	\
    (This)->lpVtbl -> get_disabled(This,pvbDisabled)

#define IPIEHTMLInputTextElement_get_form(This,ppForm)	\
    (This)->lpVtbl -> get_form(This,ppForm)

#define IPIEHTMLInputTextElement_put_defaultValue(This,bstrDefaultValue)	\
    (This)->lpVtbl -> put_defaultValue(This,bstrDefaultValue)

#define IPIEHTMLInputTextElement_get_defaultValue(This,pbstrDefaultValue)	\
    (This)->lpVtbl -> get_defaultValue(This,pbstrDefaultValue)

#define IPIEHTMLInputTextElement_get_size(This,plSize)	\
    (This)->lpVtbl -> get_size(This,plSize)

#define IPIEHTMLInputTextElement_put_maxLength(This,lMaxLength)	\
    (This)->lpVtbl -> put_maxLength(This,lMaxLength)

#define IPIEHTMLInputTextElement_get_maxLength(This,plMaxLength)	\
    (This)->lpVtbl -> get_maxLength(This,plMaxLength)

#define IPIEHTMLInputTextElement_put_readOnly(This,vbReadOnly)	\
    (This)->lpVtbl -> put_readOnly(This,vbReadOnly)

#define IPIEHTMLInputTextElement_get_readOnly(This,pvbReadOnly)	\
    (This)->lpVtbl -> get_readOnly(This,pvbReadOnly)

#define IPIEHTMLInputTextElement_select(This)	\
    (This)->lpVtbl -> select(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_put_disabled_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbDisabled);


void __RPC_STUB IPIEHTMLInputTextElement_put_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_get_disabled_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);


void __RPC_STUB IPIEHTMLInputTextElement_get_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_get_form_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);


void __RPC_STUB IPIEHTMLInputTextElement_get_form_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_put_defaultValue_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [in] */ BSTR bstrDefaultValue);


void __RPC_STUB IPIEHTMLInputTextElement_put_defaultValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_get_defaultValue_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrDefaultValue);


void __RPC_STUB IPIEHTMLInputTextElement_get_defaultValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_get_size_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plSize);


void __RPC_STUB IPIEHTMLInputTextElement_get_size_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_put_maxLength_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [in] */ long lMaxLength);


void __RPC_STUB IPIEHTMLInputTextElement_put_maxLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_get_maxLength_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plMaxLength);


void __RPC_STUB IPIEHTMLInputTextElement_get_maxLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_put_readOnly_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbReadOnly);


void __RPC_STUB IPIEHTMLInputTextElement_put_readOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_get_readOnly_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbReadOnly);


void __RPC_STUB IPIEHTMLInputTextElement_get_readOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPIEHTMLInputTextElement_select_Proxy( 
    IPIEHTMLInputTextElement __RPC_FAR * This);


void __RPC_STUB IPIEHTMLInputTextElement_select_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLInputTextElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLInputTextElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLInputTextElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLInputTextElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLInputTextElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE210D")
    PIEHTMLInputTextElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLInputTextElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLInputTextElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLInputTextElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLInputTextElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLInputTextElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLInputTextElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLInputTextElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLInputTextElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLInputTextElementEventsVtbl;

    interface PIEHTMLInputTextElementEvents
    {
        CONST_VTBL struct PIEHTMLInputTextElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLInputTextElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLInputTextElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLInputTextElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLInputTextElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLInputTextElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLInputTextElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLInputTextElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLInputTextElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLInputTextElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE210D")
PIEHTMLInputTextElement;
#endif

#ifndef __IPIEHTMLTextAreaElement_INTERFACE_DEFINED__
#define __IPIEHTMLTextAreaElement_INTERFACE_DEFINED__

/* interface IPIEHTMLTextAreaElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLTextAreaElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE210E")
    IPIEHTMLTextAreaElement : public IPIEHTMLInputTextElement
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_rows( 
            /* [retval][out] */ long __RPC_FAR *plRows) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_cols( 
            /* [retval][out] */ long __RPC_FAR *plCols) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLTextAreaElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAccesskey);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_disabled )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_disabled )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_form )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_defaultValue )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ BSTR bstrDefaultValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_defaultValue )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDefaultValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_size )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plSize);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_maxLength )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ long lMaxLength);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_maxLength )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plMaxLength);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_readOnly )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbReadOnly);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_readOnly )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbReadOnly);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *select )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_rows )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plRows);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cols )( 
            IPIEHTMLTextAreaElement __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plCols);
        
        END_INTERFACE
    } IPIEHTMLTextAreaElementVtbl;

    interface IPIEHTMLTextAreaElement
    {
        CONST_VTBL struct IPIEHTMLTextAreaElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLTextAreaElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLTextAreaElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLTextAreaElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLTextAreaElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLTextAreaElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLTextAreaElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLTextAreaElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLTextAreaElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLTextAreaElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLTextAreaElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLTextAreaElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLTextAreaElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLTextAreaElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLTextAreaElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLTextAreaElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLTextAreaElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLTextAreaElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLTextAreaElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLTextAreaElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLTextAreaElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLTextAreaElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLTextAreaElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLTextAreaElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLTextAreaElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLTextAreaElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLTextAreaElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLTextAreaElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLTextAreaElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLTextAreaElement_get_type(This,pbstrType)	\
    (This)->lpVtbl -> get_type(This,pbstrType)

#define IPIEHTMLTextAreaElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLTextAreaElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLTextAreaElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLTextAreaElement_put_accessKey(This,bstrAccesskey)	\
    (This)->lpVtbl -> put_accessKey(This,bstrAccesskey)

#define IPIEHTMLTextAreaElement_get_accessKey(This,pbstrAccesskey)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrAccesskey)

#define IPIEHTMLTextAreaElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLTextAreaElement_blur(This)	\
    (This)->lpVtbl -> blur(This)


#define IPIEHTMLTextAreaElement_put_disabled(This,vbDisabled)	\
    (This)->lpVtbl -> put_disabled(This,vbDisabled)

#define IPIEHTMLTextAreaElement_get_disabled(This,pvbDisabled)	\
    (This)->lpVtbl -> get_disabled(This,pvbDisabled)

#define IPIEHTMLTextAreaElement_get_form(This,ppForm)	\
    (This)->lpVtbl -> get_form(This,ppForm)

#define IPIEHTMLTextAreaElement_put_defaultValue(This,bstrDefaultValue)	\
    (This)->lpVtbl -> put_defaultValue(This,bstrDefaultValue)

#define IPIEHTMLTextAreaElement_get_defaultValue(This,pbstrDefaultValue)	\
    (This)->lpVtbl -> get_defaultValue(This,pbstrDefaultValue)

#define IPIEHTMLTextAreaElement_get_size(This,plSize)	\
    (This)->lpVtbl -> get_size(This,plSize)

#define IPIEHTMLTextAreaElement_put_maxLength(This,lMaxLength)	\
    (This)->lpVtbl -> put_maxLength(This,lMaxLength)

#define IPIEHTMLTextAreaElement_get_maxLength(This,plMaxLength)	\
    (This)->lpVtbl -> get_maxLength(This,plMaxLength)

#define IPIEHTMLTextAreaElement_put_readOnly(This,vbReadOnly)	\
    (This)->lpVtbl -> put_readOnly(This,vbReadOnly)

#define IPIEHTMLTextAreaElement_get_readOnly(This,pvbReadOnly)	\
    (This)->lpVtbl -> get_readOnly(This,pvbReadOnly)

#define IPIEHTMLTextAreaElement_select(This)	\
    (This)->lpVtbl -> select(This)


#define IPIEHTMLTextAreaElement_get_rows(This,plRows)	\
    (This)->lpVtbl -> get_rows(This,plRows)

#define IPIEHTMLTextAreaElement_get_cols(This,plCols)	\
    (This)->lpVtbl -> get_cols(This,plCols)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLTextAreaElement_get_rows_Proxy( 
    IPIEHTMLTextAreaElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plRows);


void __RPC_STUB IPIEHTMLTextAreaElement_get_rows_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLTextAreaElement_get_cols_Proxy( 
    IPIEHTMLTextAreaElement __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plCols);


void __RPC_STUB IPIEHTMLTextAreaElement_get_cols_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLTextAreaElement_INTERFACE_DEFINED__ */


#ifndef __PIEHTMLTextAreaElementEvents_DISPINTERFACE_DEFINED__
#define __PIEHTMLTextAreaElementEvents_DISPINTERFACE_DEFINED__

/* dispinterface PIEHTMLTextAreaElementEvents */
/* [uuid][hidden] */ 


EXTERN_C const IID DIID_PIEHTMLTextAreaElementEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-10C04FAE210E")
    PIEHTMLTextAreaElementEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct PIEHTMLTextAreaElementEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            PIEHTMLTextAreaElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            PIEHTMLTextAreaElementEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            PIEHTMLTextAreaElementEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            PIEHTMLTextAreaElementEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            PIEHTMLTextAreaElementEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            PIEHTMLTextAreaElementEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            PIEHTMLTextAreaElementEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } PIEHTMLTextAreaElementEventsVtbl;

    interface PIEHTMLTextAreaElementEvents
    {
        CONST_VTBL struct PIEHTMLTextAreaElementEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define PIEHTMLTextAreaElementEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define PIEHTMLTextAreaElementEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define PIEHTMLTextAreaElementEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define PIEHTMLTextAreaElementEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define PIEHTMLTextAreaElementEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define PIEHTMLTextAreaElementEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define PIEHTMLTextAreaElementEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __PIEHTMLTextAreaElementEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLTextAreaElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE210E")
PIEHTMLTextAreaElement;
#endif

#ifndef __IPIEHTMLInputHiddenElement_INTERFACE_DEFINED__
#define __IPIEHTMLInputHiddenElement_INTERFACE_DEFINED__

/* interface IPIEHTMLInputHiddenElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLInputHiddenElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2111")
    IPIEHTMLInputHiddenElement : public IPIEHTMLControlElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_disabled( 
            /* [in] */ VARIANT_BOOL vbDisabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_disabled( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_defaultValue( 
            /* [in] */ BSTR bstrDefaultValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_defaultValue( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDefaultValue) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_form( 
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLInputHiddenElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrType);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_value )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_value )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_accessKey )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAccesskey);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_accessKey )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAccesskey);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *focus )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *blur )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_disabled )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL vbDisabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_disabled )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_defaultValue )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [in] */ BSTR bstrDefaultValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_defaultValue )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrDefaultValue);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_form )( 
            IPIEHTMLInputHiddenElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);
        
        END_INTERFACE
    } IPIEHTMLInputHiddenElementVtbl;

    interface IPIEHTMLInputHiddenElement
    {
        CONST_VTBL struct IPIEHTMLInputHiddenElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLInputHiddenElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLInputHiddenElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLInputHiddenElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLInputHiddenElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLInputHiddenElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLInputHiddenElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLInputHiddenElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLInputHiddenElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLInputHiddenElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLInputHiddenElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLInputHiddenElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLInputHiddenElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLInputHiddenElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLInputHiddenElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLInputHiddenElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLInputHiddenElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLInputHiddenElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLInputHiddenElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLInputHiddenElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLInputHiddenElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLInputHiddenElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLInputHiddenElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLInputHiddenElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLInputHiddenElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLInputHiddenElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLInputHiddenElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLInputHiddenElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLInputHiddenElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLInputHiddenElement_get_type(This,pbstrType)	\
    (This)->lpVtbl -> get_type(This,pbstrType)

#define IPIEHTMLInputHiddenElement_put_value(This,bstrValue)	\
    (This)->lpVtbl -> put_value(This,bstrValue)

#define IPIEHTMLInputHiddenElement_get_value(This,pbstrValue)	\
    (This)->lpVtbl -> get_value(This,pbstrValue)

#define IPIEHTMLInputHiddenElement_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IPIEHTMLInputHiddenElement_put_accessKey(This,bstrAccesskey)	\
    (This)->lpVtbl -> put_accessKey(This,bstrAccesskey)

#define IPIEHTMLInputHiddenElement_get_accessKey(This,pbstrAccesskey)	\
    (This)->lpVtbl -> get_accessKey(This,pbstrAccesskey)

#define IPIEHTMLInputHiddenElement_focus(This)	\
    (This)->lpVtbl -> focus(This)

#define IPIEHTMLInputHiddenElement_blur(This)	\
    (This)->lpVtbl -> blur(This)


#define IPIEHTMLInputHiddenElement_put_disabled(This,vbDisabled)	\
    (This)->lpVtbl -> put_disabled(This,vbDisabled)

#define IPIEHTMLInputHiddenElement_get_disabled(This,pvbDisabled)	\
    (This)->lpVtbl -> get_disabled(This,pvbDisabled)

#define IPIEHTMLInputHiddenElement_put_defaultValue(This,bstrDefaultValue)	\
    (This)->lpVtbl -> put_defaultValue(This,bstrDefaultValue)

#define IPIEHTMLInputHiddenElement_get_defaultValue(This,pbstrDefaultValue)	\
    (This)->lpVtbl -> get_defaultValue(This,pbstrDefaultValue)

#define IPIEHTMLInputHiddenElement_get_form(This,ppForm)	\
    (This)->lpVtbl -> get_form(This,ppForm)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputHiddenElement_put_disabled_Proxy( 
    IPIEHTMLInputHiddenElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL vbDisabled);


void __RPC_STUB IPIEHTMLInputHiddenElement_put_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputHiddenElement_get_disabled_Proxy( 
    IPIEHTMLInputHiddenElement __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pvbDisabled);


void __RPC_STUB IPIEHTMLInputHiddenElement_get_disabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputHiddenElement_put_defaultValue_Proxy( 
    IPIEHTMLInputHiddenElement __RPC_FAR * This,
    /* [in] */ BSTR bstrDefaultValue);


void __RPC_STUB IPIEHTMLInputHiddenElement_put_defaultValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputHiddenElement_get_defaultValue_Proxy( 
    IPIEHTMLInputHiddenElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrDefaultValue);


void __RPC_STUB IPIEHTMLInputHiddenElement_get_defaultValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLInputHiddenElement_get_form_Proxy( 
    IPIEHTMLInputHiddenElement __RPC_FAR * This,
    /* [retval][out] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *ppForm);


void __RPC_STUB IPIEHTMLInputHiddenElement_get_form_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLInputHiddenElement_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLInputHiddenElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2111")
PIEHTMLInputHiddenElement;
#endif

#ifndef __IPIEHTMLBodyElement_INTERFACE_DEFINED__
#define __IPIEHTMLBodyElement_INTERFACE_DEFINED__

/* interface IPIEHTMLBodyElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLBodyElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FE63F2C0-8F8A-408e-A000-0985F0E266FB")
    IPIEHTMLBodyElement : public IPIEHTMLElement
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_background( 
            /* [in] */ BSTR bstrBackground) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_background( 
            /* [out][retval] */ BSTR __RPC_FAR *pbstrBackground) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_bgProperties( 
            /* [in] */ BSTR bstrBgProperties) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_bgProperties( 
            /* [out][retval] */ BSTR __RPC_FAR *pbstrBgProperties) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_leftMargin( 
            /* [in] */ VARIANT vLeftMargin) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_leftMargin( 
            /* [out][retval] */ VARIANT __RPC_FAR *pLeftMargin) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_topMargin( 
            /* [in] */ VARIANT vTopMargin) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_topMargin( 
            /* [out][retval] */ VARIANT __RPC_FAR *pTopMargin) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_rightMargin( 
            /* [in] */ VARIANT vRightMargin) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_rightMargin( 
            /* [out][retval] */ VARIANT __RPC_FAR *pRightMargin) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_bottomMargin( 
            /* [in] */ VARIANT vBottomMargin) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_bottomMargin( 
            /* [out][retval] */ VARIANT __RPC_FAR *pBottomMargin) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_noWrap( 
            /* [in] */ VARIANT_BOOL fNoWrap) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_noWrap( 
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *pfNoWrap) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_bgColor( 
            /* [in] */ VARIANT vBgColor) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_bgColor( 
            /* [out][retval] */ VARIANT __RPC_FAR *pBgColor) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_text( 
            /* [in] */ VARIANT vText) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_text( 
            /* [out][retval] */ VARIANT __RPC_FAR *pText) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_link( 
            /* [in] */ VARIANT vLink) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_link( 
            /* [out][retval] */ VARIANT __RPC_FAR *pLink) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_vLink( 
            /* [in] */ VARIANT vvLink) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_vLink( 
            /* [out][retval] */ VARIANT __RPC_FAR *pvLink) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_aLink( 
            /* [in] */ VARIANT vaLink) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_aLink( 
            /* [out][retval] */ VARIANT __RPC_FAR *paLink) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onload( 
            /* [in] */ VARIANT vOnload) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onload( 
            /* [out][retval] */ VARIANT __RPC_FAR *pOnload) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onunload( 
            /* [in] */ VARIANT vOnunload) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onunload( 
            /* [out][retval] */ VARIANT __RPC_FAR *pOnunload) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_scroll( 
            /* [in] */ BSTR bstrScroll) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_scroll( 
            /* [out][retval] */ BSTR __RPC_FAR *pbstrScroll) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onselect( 
            /* [in] */ VARIANT vOnselect) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onselect( 
            /* [out][retval] */ VARIANT __RPC_FAR *pOnselect) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onbeforeunload( 
            /* [in] */ VARIANT vOnbeforeunload) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onbeforeunload( 
            /* [out][retval] */ VARIANT __RPC_FAR *pOnbeforeunload) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLBodyElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLBodyElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLBodyElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_background )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstrBackground);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_background )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *pbstrBackground);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bgProperties )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstrBgProperties);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bgProperties )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *pbstrBgProperties);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_leftMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vLeftMargin);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_leftMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pLeftMargin);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_topMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vTopMargin);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_topMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pTopMargin);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_rightMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vRightMargin);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_rightMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pRightMargin);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bottomMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vBottomMargin);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bottomMargin )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pBottomMargin);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_noWrap )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL fNoWrap);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_noWrap )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT_BOOL __RPC_FAR *pfNoWrap);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bgColor )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vBgColor);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bgColor )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pBgColor);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_text )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_text )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pText);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_link )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vLink);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_link )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pLink);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vLink )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vvLink);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vLink )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pvLink);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_aLink )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vaLink);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_aLink )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *paLink);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onload )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vOnload);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onload )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pOnload);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onunload )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vOnunload);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onunload )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pOnunload);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_scroll )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ BSTR bstrScroll);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_scroll )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *pbstrScroll);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onselect )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vOnselect);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onselect )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pOnselect);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onbeforeunload )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [in] */ VARIANT vOnbeforeunload);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onbeforeunload )( 
            IPIEHTMLBodyElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *pOnbeforeunload);
        
        END_INTERFACE
    } IPIEHTMLBodyElementVtbl;

    interface IPIEHTMLBodyElement
    {
        CONST_VTBL struct IPIEHTMLBodyElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLBodyElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLBodyElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLBodyElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLBodyElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLBodyElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLBodyElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLBodyElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLBodyElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLBodyElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLBodyElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLBodyElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLBodyElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLBodyElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLBodyElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLBodyElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLBodyElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLBodyElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLBodyElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLBodyElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLBodyElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLBodyElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLBodyElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLBodyElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLBodyElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLBodyElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLBodyElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLBodyElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLBodyElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLBodyElement_put_background(This,bstrBackground)	\
    (This)->lpVtbl -> put_background(This,bstrBackground)

#define IPIEHTMLBodyElement_get_background(This,pbstrBackground)	\
    (This)->lpVtbl -> get_background(This,pbstrBackground)

#define IPIEHTMLBodyElement_put_bgProperties(This,bstrBgProperties)	\
    (This)->lpVtbl -> put_bgProperties(This,bstrBgProperties)

#define IPIEHTMLBodyElement_get_bgProperties(This,pbstrBgProperties)	\
    (This)->lpVtbl -> get_bgProperties(This,pbstrBgProperties)

#define IPIEHTMLBodyElement_put_leftMargin(This,vLeftMargin)	\
    (This)->lpVtbl -> put_leftMargin(This,vLeftMargin)

#define IPIEHTMLBodyElement_get_leftMargin(This,pLeftMargin)	\
    (This)->lpVtbl -> get_leftMargin(This,pLeftMargin)

#define IPIEHTMLBodyElement_put_topMargin(This,vTopMargin)	\
    (This)->lpVtbl -> put_topMargin(This,vTopMargin)

#define IPIEHTMLBodyElement_get_topMargin(This,pTopMargin)	\
    (This)->lpVtbl -> get_topMargin(This,pTopMargin)

#define IPIEHTMLBodyElement_put_rightMargin(This,vRightMargin)	\
    (This)->lpVtbl -> put_rightMargin(This,vRightMargin)

#define IPIEHTMLBodyElement_get_rightMargin(This,pRightMargin)	\
    (This)->lpVtbl -> get_rightMargin(This,pRightMargin)

#define IPIEHTMLBodyElement_put_bottomMargin(This,vBottomMargin)	\
    (This)->lpVtbl -> put_bottomMargin(This,vBottomMargin)

#define IPIEHTMLBodyElement_get_bottomMargin(This,pBottomMargin)	\
    (This)->lpVtbl -> get_bottomMargin(This,pBottomMargin)

#define IPIEHTMLBodyElement_put_noWrap(This,fNoWrap)	\
    (This)->lpVtbl -> put_noWrap(This,fNoWrap)

#define IPIEHTMLBodyElement_get_noWrap(This,pfNoWrap)	\
    (This)->lpVtbl -> get_noWrap(This,pfNoWrap)

#define IPIEHTMLBodyElement_put_bgColor(This,vBgColor)	\
    (This)->lpVtbl -> put_bgColor(This,vBgColor)

#define IPIEHTMLBodyElement_get_bgColor(This,pBgColor)	\
    (This)->lpVtbl -> get_bgColor(This,pBgColor)

#define IPIEHTMLBodyElement_put_text(This,vText)	\
    (This)->lpVtbl -> put_text(This,vText)

#define IPIEHTMLBodyElement_get_text(This,pText)	\
    (This)->lpVtbl -> get_text(This,pText)

#define IPIEHTMLBodyElement_put_link(This,vLink)	\
    (This)->lpVtbl -> put_link(This,vLink)

#define IPIEHTMLBodyElement_get_link(This,pLink)	\
    (This)->lpVtbl -> get_link(This,pLink)

#define IPIEHTMLBodyElement_put_vLink(This,vvLink)	\
    (This)->lpVtbl -> put_vLink(This,vvLink)

#define IPIEHTMLBodyElement_get_vLink(This,pvLink)	\
    (This)->lpVtbl -> get_vLink(This,pvLink)

#define IPIEHTMLBodyElement_put_aLink(This,vaLink)	\
    (This)->lpVtbl -> put_aLink(This,vaLink)

#define IPIEHTMLBodyElement_get_aLink(This,paLink)	\
    (This)->lpVtbl -> get_aLink(This,paLink)

#define IPIEHTMLBodyElement_put_onload(This,vOnload)	\
    (This)->lpVtbl -> put_onload(This,vOnload)

#define IPIEHTMLBodyElement_get_onload(This,pOnload)	\
    (This)->lpVtbl -> get_onload(This,pOnload)

#define IPIEHTMLBodyElement_put_onunload(This,vOnunload)	\
    (This)->lpVtbl -> put_onunload(This,vOnunload)

#define IPIEHTMLBodyElement_get_onunload(This,pOnunload)	\
    (This)->lpVtbl -> get_onunload(This,pOnunload)

#define IPIEHTMLBodyElement_put_scroll(This,bstrScroll)	\
    (This)->lpVtbl -> put_scroll(This,bstrScroll)

#define IPIEHTMLBodyElement_get_scroll(This,pbstrScroll)	\
    (This)->lpVtbl -> get_scroll(This,pbstrScroll)

#define IPIEHTMLBodyElement_put_onselect(This,vOnselect)	\
    (This)->lpVtbl -> put_onselect(This,vOnselect)

#define IPIEHTMLBodyElement_get_onselect(This,pOnselect)	\
    (This)->lpVtbl -> get_onselect(This,pOnselect)

#define IPIEHTMLBodyElement_put_onbeforeunload(This,vOnbeforeunload)	\
    (This)->lpVtbl -> put_onbeforeunload(This,vOnbeforeunload)

#define IPIEHTMLBodyElement_get_onbeforeunload(This,pOnbeforeunload)	\
    (This)->lpVtbl -> get_onbeforeunload(This,pOnbeforeunload)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_background_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ BSTR bstrBackground);


void __RPC_STUB IPIEHTMLBodyElement_put_background_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_background_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *pbstrBackground);


void __RPC_STUB IPIEHTMLBodyElement_get_background_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_bgProperties_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ BSTR bstrBgProperties);


void __RPC_STUB IPIEHTMLBodyElement_put_bgProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_bgProperties_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *pbstrBgProperties);


void __RPC_STUB IPIEHTMLBodyElement_get_bgProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_leftMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vLeftMargin);


void __RPC_STUB IPIEHTMLBodyElement_put_leftMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_leftMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pLeftMargin);


void __RPC_STUB IPIEHTMLBodyElement_get_leftMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_topMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vTopMargin);


void __RPC_STUB IPIEHTMLBodyElement_put_topMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_topMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pTopMargin);


void __RPC_STUB IPIEHTMLBodyElement_get_topMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_rightMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vRightMargin);


void __RPC_STUB IPIEHTMLBodyElement_put_rightMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_rightMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pRightMargin);


void __RPC_STUB IPIEHTMLBodyElement_get_rightMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_bottomMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vBottomMargin);


void __RPC_STUB IPIEHTMLBodyElement_put_bottomMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_bottomMargin_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pBottomMargin);


void __RPC_STUB IPIEHTMLBodyElement_get_bottomMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_noWrap_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL fNoWrap);


void __RPC_STUB IPIEHTMLBodyElement_put_noWrap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_noWrap_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT_BOOL __RPC_FAR *pfNoWrap);


void __RPC_STUB IPIEHTMLBodyElement_get_noWrap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_bgColor_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vBgColor);


void __RPC_STUB IPIEHTMLBodyElement_put_bgColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_bgColor_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pBgColor);


void __RPC_STUB IPIEHTMLBodyElement_get_bgColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_text_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vText);


void __RPC_STUB IPIEHTMLBodyElement_put_text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_text_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pText);


void __RPC_STUB IPIEHTMLBodyElement_get_text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_link_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vLink);


void __RPC_STUB IPIEHTMLBodyElement_put_link_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_link_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pLink);


void __RPC_STUB IPIEHTMLBodyElement_get_link_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_vLink_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vvLink);


void __RPC_STUB IPIEHTMLBodyElement_put_vLink_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_vLink_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pvLink);


void __RPC_STUB IPIEHTMLBodyElement_get_vLink_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_aLink_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vaLink);


void __RPC_STUB IPIEHTMLBodyElement_put_aLink_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_aLink_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *paLink);


void __RPC_STUB IPIEHTMLBodyElement_get_aLink_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_onload_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vOnload);


void __RPC_STUB IPIEHTMLBodyElement_put_onload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_onload_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pOnload);


void __RPC_STUB IPIEHTMLBodyElement_get_onload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_onunload_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vOnunload);


void __RPC_STUB IPIEHTMLBodyElement_put_onunload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_onunload_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pOnunload);


void __RPC_STUB IPIEHTMLBodyElement_get_onunload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_scroll_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ BSTR bstrScroll);


void __RPC_STUB IPIEHTMLBodyElement_put_scroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_scroll_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *pbstrScroll);


void __RPC_STUB IPIEHTMLBodyElement_get_scroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_onselect_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vOnselect);


void __RPC_STUB IPIEHTMLBodyElement_put_onselect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_onselect_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pOnselect);


void __RPC_STUB IPIEHTMLBodyElement_get_onselect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_put_onbeforeunload_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [in] */ VARIANT vOnbeforeunload);


void __RPC_STUB IPIEHTMLBodyElement_put_onbeforeunload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLBodyElement_get_onbeforeunload_Proxy( 
    IPIEHTMLBodyElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *pOnbeforeunload);


void __RPC_STUB IPIEHTMLBodyElement_get_onbeforeunload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLBodyElement_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLBodyElement;

#ifdef __cplusplus

class DECLSPEC_UUID("47E821F2-2F4A-4af5-9914-DFF3768220F1")
PIEHTMLBodyElement;
#endif

#ifndef __IPIEHTMLDivElement_INTERFACE_DEFINED__
#define __IPIEHTMLDivElement_INTERFACE_DEFINED__

/* interface IPIEHTMLDivElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLDivElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CDD75622-07D9-11D3-9309-00C04FAE2202")
    IPIEHTMLDivElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_align( 
            /* [in] */ BSTR bstrAlign) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_align( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAlign) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLDivElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLDivElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLDivElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_align )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [in] */ BSTR bstrAlign);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_align )( 
            IPIEHTMLDivElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrAlign);
        
        END_INTERFACE
    } IPIEHTMLDivElementVtbl;

    interface IPIEHTMLDivElement
    {
        CONST_VTBL struct IPIEHTMLDivElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLDivElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLDivElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLDivElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLDivElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLDivElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLDivElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLDivElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLDivElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLDivElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLDivElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLDivElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLDivElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLDivElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLDivElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLDivElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLDivElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLDivElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLDivElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLDivElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLDivElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLDivElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLDivElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLDivElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLDivElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLDivElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLDivElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLDivElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLDivElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLDivElement_put_align(This,bstrAlign)	\
    (This)->lpVtbl -> put_align(This,bstrAlign)

#define IPIEHTMLDivElement_get_align(This,pbstrAlign)	\
    (This)->lpVtbl -> get_align(This,pbstrAlign)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDivElement_put_align_Proxy( 
    IPIEHTMLDivElement __RPC_FAR * This,
    /* [in] */ BSTR bstrAlign);


void __RPC_STUB IPIEHTMLDivElement_put_align_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLDivElement_get_align_Proxy( 
    IPIEHTMLDivElement __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrAlign);


void __RPC_STUB IPIEHTMLDivElement_get_align_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLDivElement_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLDivElement;

#ifdef __cplusplus

class DECLSPEC_UUID("CDD75622-07D9-11D3-9309-20C04FAE2202")
PIEHTMLDivElement;
#endif

#ifndef __IPIEHTMLXmlElement_INTERFACE_DEFINED__
#define __IPIEHTMLXmlElement_INTERFACE_DEFINED__

/* interface IPIEHTMLXmlElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLXmlElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D9EFB0B9-68E1-456f-8ACC-9EA9CE88E1E7")
    IPIEHTMLXmlElement : public IPIEHTMLElement
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_XMLDocument( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppXMLDoc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLXmlElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLXmlElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLXmlElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_XMLDocument )( 
            IPIEHTMLXmlElement __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppXMLDoc);
        
        END_INTERFACE
    } IPIEHTMLXmlElementVtbl;

    interface IPIEHTMLXmlElement
    {
        CONST_VTBL struct IPIEHTMLXmlElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLXmlElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLXmlElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLXmlElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLXmlElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLXmlElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLXmlElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLXmlElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLXmlElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLXmlElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLXmlElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLXmlElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLXmlElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLXmlElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLXmlElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLXmlElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLXmlElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLXmlElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLXmlElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLXmlElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLXmlElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLXmlElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLXmlElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLXmlElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLXmlElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLXmlElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLXmlElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLXmlElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLXmlElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLXmlElement_get_XMLDocument(This,ppXMLDoc)	\
    (This)->lpVtbl -> get_XMLDocument(This,ppXMLDoc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLXmlElement_get_XMLDocument_Proxy( 
    IPIEHTMLXmlElement __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppXMLDoc);


void __RPC_STUB IPIEHTMLXmlElement_get_XMLDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLXmlElement_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLObjectElement_INTERFACE_DEFINED__
#define __IPIEHTMLObjectElement_INTERFACE_DEFINED__

/* interface IPIEHTMLObjectElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLObjectElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3050f24f-98b5-11cf-bb82-00aa00bdce0b")
    IPIEHTMLObjectElement : public IPIEHTMLElement
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_object( 
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_classid( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_data( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_recordset( 
            /* [in] */ IDispatch __RPC_FAR *v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_recordset( 
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_align( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_align( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_name( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_codeBase( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_codeBase( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_codeType( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_codeType( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_code( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_code( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BaseHref( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_type( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_type( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_form( 
            /* [out][retval] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_width( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_width( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_height( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_height( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_readyState( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onreadystatechange( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onreadystatechange( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onerror( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_onerror( 
            /* [out][retval] */ VARIANT __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_altHtml( 
            /* [in] */ BSTR v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_altHtml( 
            /* [out][retval] */ BSTR __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_vspace( 
            /* [in] */ long v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_vspace( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_hspace( 
            /* [in] */ long v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_hspace( 
            /* [out][retval] */ long __RPC_FAR *p) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLObjectElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLObjectElement __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLObjectElement __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_id )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_document )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLDocument __RPC_FAR *__RPC_FAR *ppDocument);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerHTML )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerHTML);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerHTML )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerHTML);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_innerText )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_innerText )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrInnerText);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_parentElement )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElement __RPC_FAR *__RPC_FAR *ppParent);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_children )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ IPIEHTMLElementCollection __RPC_FAR *__RPC_FAR *ppChildren);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_style )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_currentStyle )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [retval][out] */ IDispatchEx __RPC_FAR *__RPC_FAR *ppStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *addBehavior )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in][optional] */ VARIANT __RPC_FAR *pvarFactory,
            /* [retval][out] */ long __RPC_FAR *plID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeBehavior )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ long lID,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbSuccess);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_object )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_classid )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_data )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_recordset )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_recordset )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_align )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_align )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_name )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_name )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_codeBase )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_codeBase )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_codeType )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_codeType )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_code )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_code )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BaseHref )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_type )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_type )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_form )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_width )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_width )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_height )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_height )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_readyState )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onreadystatechange )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onreadystatechange )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_onerror )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_onerror )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ VARIANT __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_altHtml )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ BSTR v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_altHtml )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ BSTR __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_vspace )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ long v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_vspace )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hspace )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [in] */ long v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hspace )( 
            IPIEHTMLObjectElement __RPC_FAR * This,
            /* [out][retval] */ long __RPC_FAR *p);
        
        END_INTERFACE
    } IPIEHTMLObjectElementVtbl;

    interface IPIEHTMLObjectElement
    {
        CONST_VTBL struct IPIEHTMLObjectElementVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLObjectElement_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLObjectElement_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLObjectElement_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLObjectElement_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLObjectElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLObjectElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLObjectElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLObjectElement_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLObjectElement_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLObjectElement_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLObjectElement_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLObjectElement_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLObjectElement_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLObjectElement_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLObjectElement_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLObjectElement_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLObjectElement_get_id(This,pbstrID)	\
    (This)->lpVtbl -> get_id(This,pbstrID)

#define IPIEHTMLObjectElement_get_document(This,ppDocument)	\
    (This)->lpVtbl -> get_document(This,ppDocument)

#define IPIEHTMLObjectElement_put_innerHTML(This,bstrInnerHTML)	\
    (This)->lpVtbl -> put_innerHTML(This,bstrInnerHTML)

#define IPIEHTMLObjectElement_get_innerHTML(This,pbstrInnerHTML)	\
    (This)->lpVtbl -> get_innerHTML(This,pbstrInnerHTML)

#define IPIEHTMLObjectElement_put_innerText(This,bstrInnerText)	\
    (This)->lpVtbl -> put_innerText(This,bstrInnerText)

#define IPIEHTMLObjectElement_get_innerText(This,pbstrInnerText)	\
    (This)->lpVtbl -> get_innerText(This,pbstrInnerText)

#define IPIEHTMLObjectElement_get_parentElement(This,ppParent)	\
    (This)->lpVtbl -> get_parentElement(This,ppParent)

#define IPIEHTMLObjectElement_get_children(This,ppChildren)	\
    (This)->lpVtbl -> get_children(This,ppChildren)

#define IPIEHTMLObjectElement_get_style(This,ppStyle)	\
    (This)->lpVtbl -> get_style(This,ppStyle)

#define IPIEHTMLObjectElement_get_currentStyle(This,ppStyle)	\
    (This)->lpVtbl -> get_currentStyle(This,ppStyle)

#define IPIEHTMLObjectElement_addBehavior(This,bstrURL,pvarFactory,plID)	\
    (This)->lpVtbl -> addBehavior(This,bstrURL,pvarFactory,plID)

#define IPIEHTMLObjectElement_removeBehavior(This,lID,vbSuccess)	\
    (This)->lpVtbl -> removeBehavior(This,lID,vbSuccess)


#define IPIEHTMLObjectElement_get_object(This,p)	\
    (This)->lpVtbl -> get_object(This,p)

#define IPIEHTMLObjectElement_get_classid(This,p)	\
    (This)->lpVtbl -> get_classid(This,p)

#define IPIEHTMLObjectElement_get_data(This,p)	\
    (This)->lpVtbl -> get_data(This,p)

#define IPIEHTMLObjectElement_putref_recordset(This,v)	\
    (This)->lpVtbl -> putref_recordset(This,v)

#define IPIEHTMLObjectElement_get_recordset(This,p)	\
    (This)->lpVtbl -> get_recordset(This,p)

#define IPIEHTMLObjectElement_put_align(This,v)	\
    (This)->lpVtbl -> put_align(This,v)

#define IPIEHTMLObjectElement_get_align(This,p)	\
    (This)->lpVtbl -> get_align(This,p)

#define IPIEHTMLObjectElement_put_name(This,v)	\
    (This)->lpVtbl -> put_name(This,v)

#define IPIEHTMLObjectElement_get_name(This,p)	\
    (This)->lpVtbl -> get_name(This,p)

#define IPIEHTMLObjectElement_put_codeBase(This,v)	\
    (This)->lpVtbl -> put_codeBase(This,v)

#define IPIEHTMLObjectElement_get_codeBase(This,p)	\
    (This)->lpVtbl -> get_codeBase(This,p)

#define IPIEHTMLObjectElement_put_codeType(This,v)	\
    (This)->lpVtbl -> put_codeType(This,v)

#define IPIEHTMLObjectElement_get_codeType(This,p)	\
    (This)->lpVtbl -> get_codeType(This,p)

#define IPIEHTMLObjectElement_put_code(This,v)	\
    (This)->lpVtbl -> put_code(This,v)

#define IPIEHTMLObjectElement_get_code(This,p)	\
    (This)->lpVtbl -> get_code(This,p)

#define IPIEHTMLObjectElement_get_BaseHref(This,p)	\
    (This)->lpVtbl -> get_BaseHref(This,p)

#define IPIEHTMLObjectElement_put_type(This,v)	\
    (This)->lpVtbl -> put_type(This,v)

#define IPIEHTMLObjectElement_get_type(This,p)	\
    (This)->lpVtbl -> get_type(This,p)

#define IPIEHTMLObjectElement_get_form(This,p)	\
    (This)->lpVtbl -> get_form(This,p)

#define IPIEHTMLObjectElement_put_width(This,v)	\
    (This)->lpVtbl -> put_width(This,v)

#define IPIEHTMLObjectElement_get_width(This,p)	\
    (This)->lpVtbl -> get_width(This,p)

#define IPIEHTMLObjectElement_put_height(This,v)	\
    (This)->lpVtbl -> put_height(This,v)

#define IPIEHTMLObjectElement_get_height(This,p)	\
    (This)->lpVtbl -> get_height(This,p)

#define IPIEHTMLObjectElement_get_readyState(This,p)	\
    (This)->lpVtbl -> get_readyState(This,p)

#define IPIEHTMLObjectElement_put_onreadystatechange(This,v)	\
    (This)->lpVtbl -> put_onreadystatechange(This,v)

#define IPIEHTMLObjectElement_get_onreadystatechange(This,p)	\
    (This)->lpVtbl -> get_onreadystatechange(This,p)

#define IPIEHTMLObjectElement_put_onerror(This,v)	\
    (This)->lpVtbl -> put_onerror(This,v)

#define IPIEHTMLObjectElement_get_onerror(This,p)	\
    (This)->lpVtbl -> get_onerror(This,p)

#define IPIEHTMLObjectElement_put_altHtml(This,v)	\
    (This)->lpVtbl -> put_altHtml(This,v)

#define IPIEHTMLObjectElement_get_altHtml(This,p)	\
    (This)->lpVtbl -> get_altHtml(This,p)

#define IPIEHTMLObjectElement_put_vspace(This,v)	\
    (This)->lpVtbl -> put_vspace(This,v)

#define IPIEHTMLObjectElement_get_vspace(This,p)	\
    (This)->lpVtbl -> get_vspace(This,p)

#define IPIEHTMLObjectElement_put_hspace(This,v)	\
    (This)->lpVtbl -> put_hspace(This,v)

#define IPIEHTMLObjectElement_get_hspace(This,p)	\
    (This)->lpVtbl -> get_hspace(This,p)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_object_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_object_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_classid_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_classid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_data_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_putref_recordset_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *v);


void __RPC_STUB IPIEHTMLObjectElement_putref_recordset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_recordset_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ IDispatch __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_recordset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_align_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLObjectElement_put_align_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_align_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_align_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_name_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLObjectElement_put_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_name_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_codeBase_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLObjectElement_put_codeBase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_codeBase_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_codeBase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_codeType_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLObjectElement_put_codeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_codeType_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_codeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_code_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLObjectElement_put_code_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_code_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_code_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_BaseHref_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_BaseHref_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_type_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLObjectElement_put_type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_type_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_form_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ IPIEHTMLFormElement __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_form_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_width_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLObjectElement_put_width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_width_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_height_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLObjectElement_put_height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_height_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_readyState_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_readyState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_onreadystatechange_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLObjectElement_put_onreadystatechange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_onreadystatechange_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_onreadystatechange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_onerror_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLObjectElement_put_onerror_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_onerror_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ VARIANT __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_onerror_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_altHtml_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ BSTR v);


void __RPC_STUB IPIEHTMLObjectElement_put_altHtml_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_altHtml_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ BSTR __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_altHtml_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_vspace_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ long v);


void __RPC_STUB IPIEHTMLObjectElement_put_vspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_vspace_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_vspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_put_hspace_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [in] */ long v);


void __RPC_STUB IPIEHTMLObjectElement_put_hspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLObjectElement_get_hspace_Proxy( 
    IPIEHTMLObjectElement __RPC_FAR * This,
    /* [out][retval] */ long __RPC_FAR *p);


void __RPC_STUB IPIEHTMLObjectElement_get_hspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLObjectElement_INTERFACE_DEFINED__ */


#ifndef __IPIEHTMLStyle_INTERFACE_DEFINED__
#define __IPIEHTMLStyle_INTERFACE_DEFINED__

/* interface IPIEHTMLStyle */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IPIEHTMLStyle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56A11E38-2502-4a26-8F74-3B2D50F6D044")
    IPIEHTMLStyle : public IDispatchEx
    {
    public:
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_toString( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_cssText( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_cssText( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_backgroundAttachment( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_backgroundAttachment( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_backgroundImage( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_backgroundImage( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_backgroundColor( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_backgroundColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_backgroundPosition( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_backgroundPosition( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_backgroundRepeat( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_backgroundRepeat( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_behavior( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_behavior( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderBottomColor( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderBottomColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderBottomStyle( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderBottomStyle( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderBottomWidth( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderBottomWidth( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderLeftColor( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderLeftColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderLeftStyle( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderLeftStyle( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderLeftWidth( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderLeftWidth( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderRightColor( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderRightColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderRightStyle( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderRightStyle( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderRightWidth( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderRightWidth( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderTopColor( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderTopColor( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderTopStyle( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderTopStyle( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_borderTopWidth( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_borderTopWidth( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_clear( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_clear( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_color( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_color( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_display( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_display( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_styleFloat( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_styleFloat( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_fontFamily( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fontFamily( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_fontSize( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fontSize( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_fontStyle( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fontStyle( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_fontVariant( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fontVariant( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_fontWeight( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_fontWeight( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_height( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_height( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_listStyleImage( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_listStyleImage( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_listStylePosition( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_listStylePosition( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_listStyleType( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_listStyleType( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_marginBottom( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_marginBottom( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_marginLeft( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_marginLeft( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_marginRight( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_marginRight( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_marginTop( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_marginTop( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_overflow( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_overflow( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_paddingBottom( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_paddingBottom( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_paddingLeft( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_paddingLeft( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_paddingRight( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_paddingRight( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_paddingTop( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_paddingTop( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_textAlign( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_textAlign( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_textDecoration( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_textDecoration( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_textIndent( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_textIndent( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_textOverflow( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_textOverflow( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_textTransform( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_textTransform( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_verticalAlign( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_verticalAlign( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_visibility( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_visibility( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_whiteSpace( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_whiteSpace( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_width( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_width( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_wapAccessKey( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_wapAccessKey( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_wapInputFormat( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_wapInputFormat( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_wapInputRequired( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_wapInputRequired( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_wapMarqueeDir( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_wapMarqueeDir( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_wapMarqueeLoop( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_wapMarqueeLoop( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_wapMarqueeStyle( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_wapMarqueeStyle( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_lineHeight( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_lineHeight( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_left( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_left( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_right( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_right( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_top( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_top( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_bottom( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_bottom( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_zIndex( 
            /* [in] */ VARIANT v) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_zIndex( 
            /* [retval][out] */ VARIANT __RPC_FAR *pv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPIEHTMLStyleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPIEHTMLStyle __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPIEHTMLStyle __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDispID )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ DWORD grfdex,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted][local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeEx )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdp,
            /* [out] */ VARIANT __RPC_FAR *pvarRes,
            /* [out] */ EXCEPINFO __RPC_FAR *pei,
            /* [unique][in] */ IServiceProvider __RPC_FAR *pspCaller);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByName )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ DWORD grfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteMemberByDispID )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ DISPID id);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberProperties )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [in] */ DWORD grfdexFetch,
            /* [out] */ DWORD __RPC_FAR *pgrfdex);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMemberName )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ DISPID id,
            /* [out] */ BSTR __RPC_FAR *pbstrName);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextDispID )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ DWORD grfdex,
            /* [in] */ DISPID id,
            /* [out] */ DISPID __RPC_FAR *pid);
        
        /* [restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNameSpaceParent )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_toString )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrString);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_cssText )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_cssText )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_backgroundAttachment )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_backgroundAttachment )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_backgroundImage )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_backgroundImage )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_backgroundColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_backgroundColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_backgroundPosition )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_backgroundPosition )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_backgroundRepeat )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_backgroundRepeat )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_behavior )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_behavior )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderBottomColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderBottomColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderBottomStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderBottomStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderBottomWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderBottomWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderLeftColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderLeftColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderLeftStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderLeftStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderLeftWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderLeftWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderRightColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderRightColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderRightStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderRightStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderRightWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderRightWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderTopColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderTopColor )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderTopStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderTopStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_borderTopWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_borderTopWidth )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_clear )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_clear )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_color )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_color )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_display )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_display )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_styleFloat )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_styleFloat )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fontFamily )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fontFamily )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fontSize )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fontSize )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fontStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fontStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fontVariant )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fontVariant )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_fontWeight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_fontWeight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_height )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_height )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_listStyleImage )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_listStyleImage )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_listStylePosition )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_listStylePosition )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_listStyleType )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_listStyleType )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_marginBottom )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_marginBottom )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_marginLeft )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_marginLeft )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_marginRight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_marginRight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_marginTop )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_marginTop )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_overflow )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_overflow )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_paddingBottom )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_paddingBottom )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_paddingLeft )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_paddingLeft )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_paddingRight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_paddingRight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_paddingTop )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_paddingTop )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_textAlign )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_textAlign )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_textDecoration )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_textDecoration )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_textIndent )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_textIndent )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_textOverflow )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_textOverflow )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_textTransform )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_textTransform )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_verticalAlign )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_verticalAlign )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_visibility )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_visibility )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_whiteSpace )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_whiteSpace )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_width )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_width )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_wapAccessKey )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_wapAccessKey )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_wapInputFormat )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_wapInputFormat )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_wapInputRequired )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_wapInputRequired )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_wapMarqueeDir )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_wapMarqueeDir )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_wapMarqueeLoop )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_wapMarqueeLoop )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_wapMarqueeStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_wapMarqueeStyle )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_lineHeight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_lineHeight )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_left )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_left )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_right )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_right )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_top )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_top )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bottom )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bottom )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_zIndex )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [in] */ VARIANT v);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_zIndex )( 
            IPIEHTMLStyle __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pv);
        
        END_INTERFACE
    } IPIEHTMLStyleVtbl;

    interface IPIEHTMLStyle
    {
        CONST_VTBL struct IPIEHTMLStyleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPIEHTMLStyle_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPIEHTMLStyle_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPIEHTMLStyle_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPIEHTMLStyle_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPIEHTMLStyle_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPIEHTMLStyle_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPIEHTMLStyle_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPIEHTMLStyle_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IPIEHTMLStyle_InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)	\
    (This)->lpVtbl -> InvokeEx(This,id,lcid,wFlags,pdp,pvarRes,pei,pspCaller)

#define IPIEHTMLStyle_DeleteMemberByName(This,bstr,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstr,grfdex)

#define IPIEHTMLStyle_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IPIEHTMLStyle_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IPIEHTMLStyle_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IPIEHTMLStyle_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IPIEHTMLStyle_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)


#define IPIEHTMLStyle_get_toString(This,pbstrString)	\
    (This)->lpVtbl -> get_toString(This,pbstrString)

#define IPIEHTMLStyle_put_cssText(This,v)	\
    (This)->lpVtbl -> put_cssText(This,v)

#define IPIEHTMLStyle_get_cssText(This,pv)	\
    (This)->lpVtbl -> get_cssText(This,pv)

#define IPIEHTMLStyle_put_backgroundAttachment(This,v)	\
    (This)->lpVtbl -> put_backgroundAttachment(This,v)

#define IPIEHTMLStyle_get_backgroundAttachment(This,pv)	\
    (This)->lpVtbl -> get_backgroundAttachment(This,pv)

#define IPIEHTMLStyle_put_backgroundImage(This,v)	\
    (This)->lpVtbl -> put_backgroundImage(This,v)

#define IPIEHTMLStyle_get_backgroundImage(This,pv)	\
    (This)->lpVtbl -> get_backgroundImage(This,pv)

#define IPIEHTMLStyle_put_backgroundColor(This,v)	\
    (This)->lpVtbl -> put_backgroundColor(This,v)

#define IPIEHTMLStyle_get_backgroundColor(This,pv)	\
    (This)->lpVtbl -> get_backgroundColor(This,pv)

#define IPIEHTMLStyle_put_backgroundPosition(This,v)	\
    (This)->lpVtbl -> put_backgroundPosition(This,v)

#define IPIEHTMLStyle_get_backgroundPosition(This,pv)	\
    (This)->lpVtbl -> get_backgroundPosition(This,pv)

#define IPIEHTMLStyle_put_backgroundRepeat(This,v)	\
    (This)->lpVtbl -> put_backgroundRepeat(This,v)

#define IPIEHTMLStyle_get_backgroundRepeat(This,pv)	\
    (This)->lpVtbl -> get_backgroundRepeat(This,pv)

#define IPIEHTMLStyle_put_behavior(This,v)	\
    (This)->lpVtbl -> put_behavior(This,v)

#define IPIEHTMLStyle_get_behavior(This,pv)	\
    (This)->lpVtbl -> get_behavior(This,pv)

#define IPIEHTMLStyle_put_borderBottomColor(This,v)	\
    (This)->lpVtbl -> put_borderBottomColor(This,v)

#define IPIEHTMLStyle_get_borderBottomColor(This,pv)	\
    (This)->lpVtbl -> get_borderBottomColor(This,pv)

#define IPIEHTMLStyle_put_borderBottomStyle(This,v)	\
    (This)->lpVtbl -> put_borderBottomStyle(This,v)

#define IPIEHTMLStyle_get_borderBottomStyle(This,pv)	\
    (This)->lpVtbl -> get_borderBottomStyle(This,pv)

#define IPIEHTMLStyle_put_borderBottomWidth(This,v)	\
    (This)->lpVtbl -> put_borderBottomWidth(This,v)

#define IPIEHTMLStyle_get_borderBottomWidth(This,pv)	\
    (This)->lpVtbl -> get_borderBottomWidth(This,pv)

#define IPIEHTMLStyle_put_borderLeftColor(This,v)	\
    (This)->lpVtbl -> put_borderLeftColor(This,v)

#define IPIEHTMLStyle_get_borderLeftColor(This,pv)	\
    (This)->lpVtbl -> get_borderLeftColor(This,pv)

#define IPIEHTMLStyle_put_borderLeftStyle(This,v)	\
    (This)->lpVtbl -> put_borderLeftStyle(This,v)

#define IPIEHTMLStyle_get_borderLeftStyle(This,pv)	\
    (This)->lpVtbl -> get_borderLeftStyle(This,pv)

#define IPIEHTMLStyle_put_borderLeftWidth(This,v)	\
    (This)->lpVtbl -> put_borderLeftWidth(This,v)

#define IPIEHTMLStyle_get_borderLeftWidth(This,pv)	\
    (This)->lpVtbl -> get_borderLeftWidth(This,pv)

#define IPIEHTMLStyle_put_borderRightColor(This,v)	\
    (This)->lpVtbl -> put_borderRightColor(This,v)

#define IPIEHTMLStyle_get_borderRightColor(This,pv)	\
    (This)->lpVtbl -> get_borderRightColor(This,pv)

#define IPIEHTMLStyle_put_borderRightStyle(This,v)	\
    (This)->lpVtbl -> put_borderRightStyle(This,v)

#define IPIEHTMLStyle_get_borderRightStyle(This,pv)	\
    (This)->lpVtbl -> get_borderRightStyle(This,pv)

#define IPIEHTMLStyle_put_borderRightWidth(This,v)	\
    (This)->lpVtbl -> put_borderRightWidth(This,v)

#define IPIEHTMLStyle_get_borderRightWidth(This,pv)	\
    (This)->lpVtbl -> get_borderRightWidth(This,pv)

#define IPIEHTMLStyle_put_borderTopColor(This,v)	\
    (This)->lpVtbl -> put_borderTopColor(This,v)

#define IPIEHTMLStyle_get_borderTopColor(This,pv)	\
    (This)->lpVtbl -> get_borderTopColor(This,pv)

#define IPIEHTMLStyle_put_borderTopStyle(This,v)	\
    (This)->lpVtbl -> put_borderTopStyle(This,v)

#define IPIEHTMLStyle_get_borderTopStyle(This,pv)	\
    (This)->lpVtbl -> get_borderTopStyle(This,pv)

#define IPIEHTMLStyle_put_borderTopWidth(This,v)	\
    (This)->lpVtbl -> put_borderTopWidth(This,v)

#define IPIEHTMLStyle_get_borderTopWidth(This,pv)	\
    (This)->lpVtbl -> get_borderTopWidth(This,pv)

#define IPIEHTMLStyle_put_clear(This,v)	\
    (This)->lpVtbl -> put_clear(This,v)

#define IPIEHTMLStyle_get_clear(This,pv)	\
    (This)->lpVtbl -> get_clear(This,pv)

#define IPIEHTMLStyle_put_color(This,v)	\
    (This)->lpVtbl -> put_color(This,v)

#define IPIEHTMLStyle_get_color(This,pv)	\
    (This)->lpVtbl -> get_color(This,pv)

#define IPIEHTMLStyle_put_display(This,v)	\
    (This)->lpVtbl -> put_display(This,v)

#define IPIEHTMLStyle_get_display(This,pv)	\
    (This)->lpVtbl -> get_display(This,pv)

#define IPIEHTMLStyle_put_styleFloat(This,v)	\
    (This)->lpVtbl -> put_styleFloat(This,v)

#define IPIEHTMLStyle_get_styleFloat(This,pv)	\
    (This)->lpVtbl -> get_styleFloat(This,pv)

#define IPIEHTMLStyle_put_fontFamily(This,v)	\
    (This)->lpVtbl -> put_fontFamily(This,v)

#define IPIEHTMLStyle_get_fontFamily(This,pv)	\
    (This)->lpVtbl -> get_fontFamily(This,pv)

#define IPIEHTMLStyle_put_fontSize(This,v)	\
    (This)->lpVtbl -> put_fontSize(This,v)

#define IPIEHTMLStyle_get_fontSize(This,pv)	\
    (This)->lpVtbl -> get_fontSize(This,pv)

#define IPIEHTMLStyle_put_fontStyle(This,v)	\
    (This)->lpVtbl -> put_fontStyle(This,v)

#define IPIEHTMLStyle_get_fontStyle(This,pv)	\
    (This)->lpVtbl -> get_fontStyle(This,pv)

#define IPIEHTMLStyle_put_fontVariant(This,v)	\
    (This)->lpVtbl -> put_fontVariant(This,v)

#define IPIEHTMLStyle_get_fontVariant(This,pv)	\
    (This)->lpVtbl -> get_fontVariant(This,pv)

#define IPIEHTMLStyle_put_fontWeight(This,v)	\
    (This)->lpVtbl -> put_fontWeight(This,v)

#define IPIEHTMLStyle_get_fontWeight(This,pv)	\
    (This)->lpVtbl -> get_fontWeight(This,pv)

#define IPIEHTMLStyle_put_height(This,v)	\
    (This)->lpVtbl -> put_height(This,v)

#define IPIEHTMLStyle_get_height(This,pv)	\
    (This)->lpVtbl -> get_height(This,pv)

#define IPIEHTMLStyle_put_listStyleImage(This,v)	\
    (This)->lpVtbl -> put_listStyleImage(This,v)

#define IPIEHTMLStyle_get_listStyleImage(This,pv)	\
    (This)->lpVtbl -> get_listStyleImage(This,pv)

#define IPIEHTMLStyle_put_listStylePosition(This,v)	\
    (This)->lpVtbl -> put_listStylePosition(This,v)

#define IPIEHTMLStyle_get_listStylePosition(This,pv)	\
    (This)->lpVtbl -> get_listStylePosition(This,pv)

#define IPIEHTMLStyle_put_listStyleType(This,v)	\
    (This)->lpVtbl -> put_listStyleType(This,v)

#define IPIEHTMLStyle_get_listStyleType(This,pv)	\
    (This)->lpVtbl -> get_listStyleType(This,pv)

#define IPIEHTMLStyle_put_marginBottom(This,v)	\
    (This)->lpVtbl -> put_marginBottom(This,v)

#define IPIEHTMLStyle_get_marginBottom(This,pv)	\
    (This)->lpVtbl -> get_marginBottom(This,pv)

#define IPIEHTMLStyle_put_marginLeft(This,v)	\
    (This)->lpVtbl -> put_marginLeft(This,v)

#define IPIEHTMLStyle_get_marginLeft(This,pv)	\
    (This)->lpVtbl -> get_marginLeft(This,pv)

#define IPIEHTMLStyle_put_marginRight(This,v)	\
    (This)->lpVtbl -> put_marginRight(This,v)

#define IPIEHTMLStyle_get_marginRight(This,pv)	\
    (This)->lpVtbl -> get_marginRight(This,pv)

#define IPIEHTMLStyle_put_marginTop(This,v)	\
    (This)->lpVtbl -> put_marginTop(This,v)

#define IPIEHTMLStyle_get_marginTop(This,pv)	\
    (This)->lpVtbl -> get_marginTop(This,pv)

#define IPIEHTMLStyle_put_overflow(This,v)	\
    (This)->lpVtbl -> put_overflow(This,v)

#define IPIEHTMLStyle_get_overflow(This,pv)	\
    (This)->lpVtbl -> get_overflow(This,pv)

#define IPIEHTMLStyle_put_paddingBottom(This,v)	\
    (This)->lpVtbl -> put_paddingBottom(This,v)

#define IPIEHTMLStyle_get_paddingBottom(This,pv)	\
    (This)->lpVtbl -> get_paddingBottom(This,pv)

#define IPIEHTMLStyle_put_paddingLeft(This,v)	\
    (This)->lpVtbl -> put_paddingLeft(This,v)

#define IPIEHTMLStyle_get_paddingLeft(This,pv)	\
    (This)->lpVtbl -> get_paddingLeft(This,pv)

#define IPIEHTMLStyle_put_paddingRight(This,v)	\
    (This)->lpVtbl -> put_paddingRight(This,v)

#define IPIEHTMLStyle_get_paddingRight(This,pv)	\
    (This)->lpVtbl -> get_paddingRight(This,pv)

#define IPIEHTMLStyle_put_paddingTop(This,v)	\
    (This)->lpVtbl -> put_paddingTop(This,v)

#define IPIEHTMLStyle_get_paddingTop(This,pv)	\
    (This)->lpVtbl -> get_paddingTop(This,pv)

#define IPIEHTMLStyle_put_textAlign(This,v)	\
    (This)->lpVtbl -> put_textAlign(This,v)

#define IPIEHTMLStyle_get_textAlign(This,pv)	\
    (This)->lpVtbl -> get_textAlign(This,pv)

#define IPIEHTMLStyle_put_textDecoration(This,v)	\
    (This)->lpVtbl -> put_textDecoration(This,v)

#define IPIEHTMLStyle_get_textDecoration(This,pv)	\
    (This)->lpVtbl -> get_textDecoration(This,pv)

#define IPIEHTMLStyle_put_textIndent(This,v)	\
    (This)->lpVtbl -> put_textIndent(This,v)

#define IPIEHTMLStyle_get_textIndent(This,pv)	\
    (This)->lpVtbl -> get_textIndent(This,pv)

#define IPIEHTMLStyle_put_textOverflow(This,v)	\
    (This)->lpVtbl -> put_textOverflow(This,v)

#define IPIEHTMLStyle_get_textOverflow(This,pv)	\
    (This)->lpVtbl -> get_textOverflow(This,pv)

#define IPIEHTMLStyle_put_textTransform(This,v)	\
    (This)->lpVtbl -> put_textTransform(This,v)

#define IPIEHTMLStyle_get_textTransform(This,pv)	\
    (This)->lpVtbl -> get_textTransform(This,pv)

#define IPIEHTMLStyle_put_verticalAlign(This,v)	\
    (This)->lpVtbl -> put_verticalAlign(This,v)

#define IPIEHTMLStyle_get_verticalAlign(This,pv)	\
    (This)->lpVtbl -> get_verticalAlign(This,pv)

#define IPIEHTMLStyle_put_visibility(This,v)	\
    (This)->lpVtbl -> put_visibility(This,v)

#define IPIEHTMLStyle_get_visibility(This,pv)	\
    (This)->lpVtbl -> get_visibility(This,pv)

#define IPIEHTMLStyle_put_whiteSpace(This,v)	\
    (This)->lpVtbl -> put_whiteSpace(This,v)

#define IPIEHTMLStyle_get_whiteSpace(This,pv)	\
    (This)->lpVtbl -> get_whiteSpace(This,pv)

#define IPIEHTMLStyle_put_width(This,v)	\
    (This)->lpVtbl -> put_width(This,v)

#define IPIEHTMLStyle_get_width(This,pv)	\
    (This)->lpVtbl -> get_width(This,pv)

#define IPIEHTMLStyle_put_wapAccessKey(This,v)	\
    (This)->lpVtbl -> put_wapAccessKey(This,v)

#define IPIEHTMLStyle_get_wapAccessKey(This,pv)	\
    (This)->lpVtbl -> get_wapAccessKey(This,pv)

#define IPIEHTMLStyle_put_wapInputFormat(This,v)	\
    (This)->lpVtbl -> put_wapInputFormat(This,v)

#define IPIEHTMLStyle_get_wapInputFormat(This,pv)	\
    (This)->lpVtbl -> get_wapInputFormat(This,pv)

#define IPIEHTMLStyle_put_wapInputRequired(This,v)	\
    (This)->lpVtbl -> put_wapInputRequired(This,v)

#define IPIEHTMLStyle_get_wapInputRequired(This,pv)	\
    (This)->lpVtbl -> get_wapInputRequired(This,pv)

#define IPIEHTMLStyle_put_wapMarqueeDir(This,v)	\
    (This)->lpVtbl -> put_wapMarqueeDir(This,v)

#define IPIEHTMLStyle_get_wapMarqueeDir(This,pv)	\
    (This)->lpVtbl -> get_wapMarqueeDir(This,pv)

#define IPIEHTMLStyle_put_wapMarqueeLoop(This,v)	\
    (This)->lpVtbl -> put_wapMarqueeLoop(This,v)

#define IPIEHTMLStyle_get_wapMarqueeLoop(This,pv)	\
    (This)->lpVtbl -> get_wapMarqueeLoop(This,pv)

#define IPIEHTMLStyle_put_wapMarqueeStyle(This,v)	\
    (This)->lpVtbl -> put_wapMarqueeStyle(This,v)

#define IPIEHTMLStyle_get_wapMarqueeStyle(This,pv)	\
    (This)->lpVtbl -> get_wapMarqueeStyle(This,pv)

#define IPIEHTMLStyle_put_lineHeight(This,v)	\
    (This)->lpVtbl -> put_lineHeight(This,v)

#define IPIEHTMLStyle_get_lineHeight(This,pv)	\
    (This)->lpVtbl -> get_lineHeight(This,pv)

#define IPIEHTMLStyle_put_left(This,v)	\
    (This)->lpVtbl -> put_left(This,v)

#define IPIEHTMLStyle_get_left(This,pv)	\
    (This)->lpVtbl -> get_left(This,pv)

#define IPIEHTMLStyle_put_right(This,v)	\
    (This)->lpVtbl -> put_right(This,v)

#define IPIEHTMLStyle_get_right(This,pv)	\
    (This)->lpVtbl -> get_right(This,pv)

#define IPIEHTMLStyle_put_top(This,v)	\
    (This)->lpVtbl -> put_top(This,v)

#define IPIEHTMLStyle_get_top(This,pv)	\
    (This)->lpVtbl -> get_top(This,pv)

#define IPIEHTMLStyle_put_bottom(This,v)	\
    (This)->lpVtbl -> put_bottom(This,v)

#define IPIEHTMLStyle_get_bottom(This,pv)	\
    (This)->lpVtbl -> get_bottom(This,pv)

#define IPIEHTMLStyle_put_zIndex(This,v)	\
    (This)->lpVtbl -> put_zIndex(This,v)

#define IPIEHTMLStyle_get_zIndex(This,pv)	\
    (This)->lpVtbl -> get_zIndex(This,pv)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_toString_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrString);


void __RPC_STUB IPIEHTMLStyle_get_toString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_cssText_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_cssText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_cssText_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_cssText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_backgroundAttachment_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_backgroundAttachment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_backgroundAttachment_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_backgroundAttachment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_backgroundImage_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_backgroundImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_backgroundImage_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_backgroundImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_backgroundColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_backgroundColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_backgroundColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_backgroundColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_backgroundPosition_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_backgroundPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_backgroundPosition_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_backgroundPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_backgroundRepeat_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_backgroundRepeat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_backgroundRepeat_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_backgroundRepeat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_behavior_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_behavior_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_behavior_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_behavior_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderBottomColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderBottomColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderBottomColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderBottomColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderBottomStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderBottomStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderBottomStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderBottomStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderBottomWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderBottomWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderBottomWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderBottomWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderLeftColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderLeftColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderLeftColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderLeftColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderLeftStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderLeftStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderLeftStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderLeftStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderLeftWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderLeftWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderLeftWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderLeftWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderRightColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderRightColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderRightColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderRightColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderRightStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderRightStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderRightStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderRightStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderRightWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderRightWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderRightWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderRightWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderTopColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderTopColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderTopColor_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderTopColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderTopStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderTopStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderTopStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderTopStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_borderTopWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_borderTopWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_borderTopWidth_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_borderTopWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_clear_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_clear_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_color_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_color_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_color_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_color_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_display_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_display_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_display_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_display_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_styleFloat_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_styleFloat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_styleFloat_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_styleFloat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_fontFamily_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_fontFamily_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_fontFamily_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_fontFamily_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_fontSize_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_fontSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_fontSize_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_fontSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_fontStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_fontStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_fontStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_fontStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_fontVariant_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_fontVariant_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_fontVariant_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_fontVariant_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_fontWeight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_fontWeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_fontWeight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_fontWeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_height_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_height_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_listStyleImage_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_listStyleImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_listStyleImage_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_listStyleImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_listStylePosition_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_listStylePosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_listStylePosition_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_listStylePosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_listStyleType_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_listStyleType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_listStyleType_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_listStyleType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_marginBottom_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_marginBottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_marginBottom_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_marginBottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_marginLeft_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_marginLeft_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_marginLeft_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_marginLeft_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_marginRight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_marginRight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_marginRight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_marginRight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_marginTop_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_marginTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_marginTop_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_marginTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_overflow_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_overflow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_overflow_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_overflow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_paddingBottom_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_paddingBottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_paddingBottom_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_paddingBottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_paddingLeft_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_paddingLeft_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_paddingLeft_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_paddingLeft_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_paddingRight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_paddingRight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_paddingRight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_paddingRight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_paddingTop_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_paddingTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_paddingTop_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_paddingTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_textAlign_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_textAlign_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_textAlign_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_textAlign_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_textDecoration_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_textDecoration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_textDecoration_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_textDecoration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_textIndent_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_textIndent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_textIndent_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_textIndent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_textOverflow_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_textOverflow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_textOverflow_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_textOverflow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_textTransform_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_textTransform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_textTransform_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_textTransform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_verticalAlign_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_verticalAlign_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_verticalAlign_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_verticalAlign_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_visibility_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_visibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_visibility_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_visibility_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_whiteSpace_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_whiteSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_whiteSpace_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_whiteSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_width_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_width_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_wapAccessKey_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_wapAccessKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_wapAccessKey_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_wapAccessKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_wapInputFormat_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_wapInputFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_wapInputFormat_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_wapInputFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_wapInputRequired_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_wapInputRequired_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_wapInputRequired_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_wapInputRequired_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_wapMarqueeDir_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_wapMarqueeDir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_wapMarqueeDir_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_wapMarqueeDir_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_wapMarqueeLoop_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_wapMarqueeLoop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_wapMarqueeLoop_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_wapMarqueeLoop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_wapMarqueeStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_wapMarqueeStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_wapMarqueeStyle_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_wapMarqueeStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_lineHeight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_lineHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_lineHeight_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_lineHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_left_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_left_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_left_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_left_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_right_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_right_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_right_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_right_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_top_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_top_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_bottom_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_bottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_bottom_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_bottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_put_zIndex_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [in] */ VARIANT v);


void __RPC_STUB IPIEHTMLStyle_put_zIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPIEHTMLStyle_get_zIndex_Proxy( 
    IPIEHTMLStyle __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pv);


void __RPC_STUB IPIEHTMLStyle_get_zIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPIEHTMLStyle_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PIEHTMLStyle;

#ifdef __cplusplus

class DECLSPEC_UUID("B7C8D6E0-C417-4974-8963-748ACDADA4D7")
PIEHTMLStyle;
#endif

#ifndef __IHomePage_INTERFACE_DEFINED__
#define __IHomePage_INTERFACE_DEFINED__

/* interface IHomePage */
/* [unique][uuid][dual][local][object] */ 


EXTERN_C const IID IID_IHomePage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("503f3a42-3267-4a79-96fc-f3b3e422e00a")
    IHomePage : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE isHomePage( 
            /* [in] */ BSTR bstrPageURL,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbReturn) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE navigateHomePage( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setHomePage( 
            /* [in] */ BSTR bstrPageURL) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHomePageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHomePage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHomePage __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHomePage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHomePage __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHomePage __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHomePage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHomePage __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *isHomePage )( 
            IHomePage __RPC_FAR * This,
            /* [in] */ BSTR bstrPageURL,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbReturn);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *navigateHomePage )( 
            IHomePage __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setHomePage )( 
            IHomePage __RPC_FAR * This,
            /* [in] */ BSTR bstrPageURL);
        
        END_INTERFACE
    } IHomePageVtbl;

    interface IHomePage
    {
        CONST_VTBL struct IHomePageVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHomePage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHomePage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHomePage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHomePage_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHomePage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHomePage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHomePage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHomePage_isHomePage(This,bstrPageURL,vbReturn)	\
    (This)->lpVtbl -> isHomePage(This,bstrPageURL,vbReturn)

#define IHomePage_navigateHomePage(This)	\
    (This)->lpVtbl -> navigateHomePage(This)

#define IHomePage_setHomePage(This,bstrPageURL)	\
    (This)->lpVtbl -> setHomePage(This,bstrPageURL)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IHomePage_isHomePage_Proxy( 
    IHomePage __RPC_FAR * This,
    /* [in] */ BSTR bstrPageURL,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *vbReturn);


void __RPC_STUB IHomePage_isHomePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHomePage_navigateHomePage_Proxy( 
    IHomePage __RPC_FAR * This);


void __RPC_STUB IHomePage_navigateHomePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IHomePage_setHomePage_Proxy( 
    IHomePage __RPC_FAR * This,
    /* [in] */ BSTR bstrPageURL);


void __RPC_STUB IHomePage_setHomePage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHomePage_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CHomePage;

#ifdef __cplusplus

class DECLSPEC_UUID("81383c81-ef9a-41e7-ba52-5ec9bfc52ff6")
CHomePage;
#endif
#endif /* __WEBVIEWLib_LIBRARY_DEFINED__ */

/* interface __MIDL_itf_webvw_0307 */
/* [local] */ 

/* For backward compatibility (CLSID name was changed from 'Browser' to 'PIEHTML')*/
#define CLSID_Browser            CLSID_PIEHTML
EXTERN_C const GUID SID_ExternalHostDispatch;


extern RPC_IF_HANDLE __MIDL_itf_webvw_0307_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_webvw_0307_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


