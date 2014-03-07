
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:22:12 2008
 */
/* Compiler settings for .\cmgruri.idl:
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

#ifndef __cmgruri_h__
#define __cmgruri_h__

/* Forward Declarations */ 

#ifndef __IConfigManager2URI_FWD_DEFINED__
#define __IConfigManager2URI_FWD_DEFINED__
typedef interface IConfigManager2URI IConfigManager2URI;
#endif 	/* __IConfigManager2URI_FWD_DEFINED__ */


#ifndef __IConfigManager2MutableURI_FWD_DEFINED__
#define __IConfigManager2MutableURI_FWD_DEFINED__
typedef interface IConfigManager2MutableURI IConfigManager2MutableURI;
#endif 	/* __IConfigManager2MutableURI_FWD_DEFINED__ */


#ifndef __ICSPURITranslator_FWD_DEFINED__
#define __ICSPURITranslator_FWD_DEFINED__
typedef interface ICSPURITranslator ICSPURITranslator;
#endif 	/* __ICSPURITranslator_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_cmgruri_0000 */
/* [local] */ 






extern RPC_IF_HANDLE __MIDL_itf_cmgruri_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cmgruri_0000_v0_0_s_ifspec;

#ifndef __IConfigManager2URI_INTERFACE_DEFINED__
#define __IConfigManager2URI_INTERFACE_DEFINED__

/* interface IConfigManager2URI */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigManager2URI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E34E5896-40B2-45c4-A9C0-8A9601C3B0A6")
    IConfigManager2URI : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsAbsoluteURI( 
            /* [out] */ BOOL __RPC_FAR *pfIsAbsolute) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HasQuery( 
            /* [out] */ BOOL __RPC_FAR *pfHasQuery) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitializeFromString( 
            /* [string][in] */ const wchar_t __RPC_FAR *pwszURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitializeFromStream( 
            /* [in] */ ISequentialStream __RPC_FAR *pStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SaveToStream( 
            /* [in] */ ISequentialStream __RPC_FAR *pStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCanonicalRelativeURI( 
            /* [in] */ DWORD idwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ BSTR __RPC_FAR *pbstrRelURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRelativeURI( 
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SplitURI( 
            /* [in] */ DWORD idwSegSplit,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI1,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSegment( 
            /* [in] */ DWORD idwSegIndex,
            /* [out] */ const wchar_t __RPC_FAR *__RPC_FAR *pwszSegment) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSegmentCopy( 
            /* [in] */ DWORD idwSegIndex,
            /* [out] */ BSTR __RPC_FAR *pbstrSegment) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CompareURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI2,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ int __RPC_FAR *pnCompare) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindLastCommonSegment( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ DWORD __RPC_FAR *dwCommonSegIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetJoinedSegments( 
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ wchar_t wchJoin,
            /* [out] */ BSTR __RPC_FAR *pbstrJoined) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetQueryValue( 
            /* [string][in] */ const wchar_t __RPC_FAR *pwszName,
            /* [out] */ BSTR __RPC_FAR *pbstrValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSegmentCount( 
            /* [out] */ DWORD __RPC_FAR *pcdwSegments) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2MutableURI __RPC_FAR *__RPC_FAR *ppMutableURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetHash( 
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ DWORD __RPC_FAR *pdwHash) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendSegmentToCopy( 
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendRelativeURIToCopy( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pRelativeURI,
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigManager2URIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigManager2URI __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigManager2URI __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsAbsoluteURI )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfIsAbsolute);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HasQuery )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfHasQuery);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitializeFromString )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitializeFromStream )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveToStream )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCanonicalRelativeURI )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ BSTR __RPC_FAR *pbstrRelURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRelativeURI )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SplitURI )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ DWORD idwSegSplit,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI1,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI2);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSegment )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex,
            /* [out] */ const wchar_t __RPC_FAR *__RPC_FAR *pwszSegment);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSegmentCopy )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex,
            /* [out] */ BSTR __RPC_FAR *pbstrSegment);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompareURI )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI2,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ int __RPC_FAR *pnCompare);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindLastCommonSegment )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ DWORD __RPC_FAR *dwCommonSegIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetJoinedSegments )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ wchar_t wchJoin,
            /* [out] */ BSTR __RPC_FAR *pbstrJoined);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetQueryValue )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszName,
            /* [out] */ BSTR __RPC_FAR *pbstrValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSegmentCount )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pcdwSegments);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2MutableURI __RPC_FAR *__RPC_FAR *ppMutableURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHash )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ DWORD __RPC_FAR *pdwHash);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendSegmentToCopy )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendRelativeURIToCopy )( 
            IConfigManager2URI __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pRelativeURI,
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);
        
        END_INTERFACE
    } IConfigManager2URIVtbl;

    interface IConfigManager2URI
    {
        CONST_VTBL struct IConfigManager2URIVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigManager2URI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigManager2URI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigManager2URI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigManager2URI_IsAbsoluteURI(This,pfIsAbsolute)	\
    (This)->lpVtbl -> IsAbsoluteURI(This,pfIsAbsolute)

#define IConfigManager2URI_HasQuery(This,pfHasQuery)	\
    (This)->lpVtbl -> HasQuery(This,pfHasQuery)

#define IConfigManager2URI_InitializeFromString(This,pwszURI)	\
    (This)->lpVtbl -> InitializeFromString(This,pwszURI)

#define IConfigManager2URI_InitializeFromStream(This,pStream)	\
    (This)->lpVtbl -> InitializeFromStream(This,pStream)

#define IConfigManager2URI_SaveToStream(This,pStream)	\
    (This)->lpVtbl -> SaveToStream(This,pStream)

#define IConfigManager2URI_GetCanonicalRelativeURI(This,idwSegIndex,fIncludeQuery,pbstrRelURI)	\
    (This)->lpVtbl -> GetCanonicalRelativeURI(This,idwSegIndex,fIncludeQuery,pbstrRelURI)

#define IConfigManager2URI_GetRelativeURI(This,dwSegIndex,fIncludeQuery,pURI)	\
    (This)->lpVtbl -> GetRelativeURI(This,dwSegIndex,fIncludeQuery,pURI)

#define IConfigManager2URI_SplitURI(This,idwSegSplit,fIncludeQuery,pURI1,pURI2)	\
    (This)->lpVtbl -> SplitURI(This,idwSegSplit,fIncludeQuery,pURI1,pURI2)

#define IConfigManager2URI_GetSegment(This,idwSegIndex,pwszSegment)	\
    (This)->lpVtbl -> GetSegment(This,idwSegIndex,pwszSegment)

#define IConfigManager2URI_GetSegmentCopy(This,idwSegIndex,pbstrSegment)	\
    (This)->lpVtbl -> GetSegmentCopy(This,idwSegIndex,pbstrSegment)

#define IConfigManager2URI_CompareURI(This,pURI2,fIgnoreCase,pnCompare)	\
    (This)->lpVtbl -> CompareURI(This,pURI2,fIgnoreCase,pnCompare)

#define IConfigManager2URI_FindLastCommonSegment(This,pURI,fIgnoreCase,dwCommonSegIndex)	\
    (This)->lpVtbl -> FindLastCommonSegment(This,pURI,fIgnoreCase,dwCommonSegIndex)

#define IConfigManager2URI_GetJoinedSegments(This,dwSegIndex,wchJoin,pbstrJoined)	\
    (This)->lpVtbl -> GetJoinedSegments(This,dwSegIndex,wchJoin,pbstrJoined)

#define IConfigManager2URI_GetQueryValue(This,pwszName,pbstrValue)	\
    (This)->lpVtbl -> GetQueryValue(This,pwszName,pbstrValue)

#define IConfigManager2URI_GetSegmentCount(This,pcdwSegments)	\
    (This)->lpVtbl -> GetSegmentCount(This,pcdwSegments)

#define IConfigManager2URI_Clone(This,fIncludeQuery,ppMutableURI)	\
    (This)->lpVtbl -> Clone(This,fIncludeQuery,ppMutableURI)

#define IConfigManager2URI_GetHash(This,fIgnoreCase,pdwHash)	\
    (This)->lpVtbl -> GetHash(This,fIgnoreCase,pdwHash)

#define IConfigManager2URI_AppendSegmentToCopy(This,pwszSegment,fSegmentIsEncoded,fIncludeQuery,ppURI)	\
    (This)->lpVtbl -> AppendSegmentToCopy(This,pwszSegment,fSegmentIsEncoded,fIncludeQuery,ppURI)

#define IConfigManager2URI_AppendRelativeURIToCopy(This,pRelativeURI,dwSegIndex,fIncludeQuery,ppURI)	\
    (This)->lpVtbl -> AppendRelativeURIToCopy(This,pRelativeURI,dwSegIndex,fIncludeQuery,ppURI)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigManager2URI_IsAbsoluteURI_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pfIsAbsolute);


void __RPC_STUB IConfigManager2URI_IsAbsoluteURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_HasQuery_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [out] */ BOOL __RPC_FAR *pfHasQuery);


void __RPC_STUB IConfigManager2URI_HasQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_InitializeFromString_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszURI);


void __RPC_STUB IConfigManager2URI_InitializeFromString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_InitializeFromStream_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pStream);


void __RPC_STUB IConfigManager2URI_InitializeFromStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_SaveToStream_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ ISequentialStream __RPC_FAR *pStream);


void __RPC_STUB IConfigManager2URI_SaveToStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetCanonicalRelativeURI_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ DWORD idwSegIndex,
    /* [in] */ BOOL fIncludeQuery,
    /* [out] */ BSTR __RPC_FAR *pbstrRelURI);


void __RPC_STUB IConfigManager2URI_GetCanonicalRelativeURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetRelativeURI_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ DWORD dwSegIndex,
    /* [in] */ BOOL fIncludeQuery,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI);


void __RPC_STUB IConfigManager2URI_GetRelativeURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_SplitURI_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ DWORD idwSegSplit,
    /* [in] */ BOOL fIncludeQuery,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI1,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI2);


void __RPC_STUB IConfigManager2URI_SplitURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetSegment_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ DWORD idwSegIndex,
    /* [out] */ const wchar_t __RPC_FAR *__RPC_FAR *pwszSegment);


void __RPC_STUB IConfigManager2URI_GetSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetSegmentCopy_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ DWORD idwSegIndex,
    /* [out] */ BSTR __RPC_FAR *pbstrSegment);


void __RPC_STUB IConfigManager2URI_GetSegmentCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_CompareURI_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI2,
    /* [in] */ BOOL fIgnoreCase,
    /* [out] */ int __RPC_FAR *pnCompare);


void __RPC_STUB IConfigManager2URI_CompareURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_FindLastCommonSegment_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
    /* [in] */ BOOL fIgnoreCase,
    /* [out] */ DWORD __RPC_FAR *dwCommonSegIndex);


void __RPC_STUB IConfigManager2URI_FindLastCommonSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetJoinedSegments_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ DWORD dwSegIndex,
    /* [in] */ wchar_t wchJoin,
    /* [out] */ BSTR __RPC_FAR *pbstrJoined);


void __RPC_STUB IConfigManager2URI_GetJoinedSegments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetQueryValue_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszName,
    /* [out] */ BSTR __RPC_FAR *pbstrValue);


void __RPC_STUB IConfigManager2URI_GetQueryValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetSegmentCount_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pcdwSegments);


void __RPC_STUB IConfigManager2URI_GetSegmentCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_Clone_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ BOOL fIncludeQuery,
    /* [out] */ IConfigManager2MutableURI __RPC_FAR *__RPC_FAR *ppMutableURI);


void __RPC_STUB IConfigManager2URI_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_GetHash_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ BOOL fIgnoreCase,
    /* [out] */ DWORD __RPC_FAR *pdwHash);


void __RPC_STUB IConfigManager2URI_GetHash_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_AppendSegmentToCopy_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
    /* [in] */ BOOL fSegmentIsEncoded,
    /* [in] */ BOOL fIncludeQuery,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);


void __RPC_STUB IConfigManager2URI_AppendSegmentToCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2URI_AppendRelativeURIToCopy_Proxy( 
    IConfigManager2URI __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pRelativeURI,
    /* [in] */ DWORD dwSegIndex,
    /* [in] */ BOOL fIncludeQuery,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);


void __RPC_STUB IConfigManager2URI_AppendRelativeURIToCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigManager2URI_INTERFACE_DEFINED__ */


#ifndef __IConfigManager2MutableURI_INTERFACE_DEFINED__
#define __IConfigManager2MutableURI_INTERFACE_DEFINED__

/* interface IConfigManager2MutableURI */
/* [object][uuid] */ 


EXTERN_C const IID IID_IConfigManager2MutableURI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4B965405-F21F-4702-95DD-4E81C3D1BB30")
    IConfigManager2MutableURI : public IConfigManager2URI
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AppendSegment( 
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendRelativeURI( 
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ DWORD idwSegment) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReplaceSegment( 
            /* [in] */ DWORD idwSegIndex,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteSegment( 
            /* [in] */ DWORD idwSegIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InsertSegment( 
            /* [in] */ DWORD idwInsertBefore,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AppendQueryValue( 
            /* [string][in] */ const wchar_t __RPC_FAR *pwszQueryName,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszQueryValue,
            /* [in] */ BOOL fIsEncoded) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateNonMutableURI( 
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConfigManager2MutableURIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConfigManager2MutableURI __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConfigManager2MutableURI __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsAbsoluteURI )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfIsAbsolute);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HasQuery )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [out] */ BOOL __RPC_FAR *pfHasQuery);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitializeFromString )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitializeFromStream )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveToStream )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ ISequentialStream __RPC_FAR *pStream);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCanonicalRelativeURI )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ BSTR __RPC_FAR *pbstrRelURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRelativeURI )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SplitURI )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD idwSegSplit,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI1,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *pURI2);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSegment )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex,
            /* [out] */ const wchar_t __RPC_FAR *__RPC_FAR *pwszSegment);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSegmentCopy )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex,
            /* [out] */ BSTR __RPC_FAR *pbstrSegment);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompareURI )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI2,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ int __RPC_FAR *pnCompare);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindLastCommonSegment )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ DWORD __RPC_FAR *dwCommonSegIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetJoinedSegments )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ wchar_t wchJoin,
            /* [out] */ BSTR __RPC_FAR *pbstrJoined);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetQueryValue )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszName,
            /* [out] */ BSTR __RPC_FAR *pbstrValue);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSegmentCount )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pcdwSegments);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2MutableURI __RPC_FAR *__RPC_FAR *ppMutableURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHash )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ BOOL fIgnoreCase,
            /* [out] */ DWORD __RPC_FAR *pdwHash);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendSegmentToCopy )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendRelativeURIToCopy )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pRelativeURI,
            /* [in] */ DWORD dwSegIndex,
            /* [in] */ BOOL fIncludeQuery,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendSegment )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendRelativeURI )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
            /* [in] */ DWORD idwSegment);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReplaceSegment )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteSegment )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD idwSegIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertSegment )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [in] */ DWORD idwInsertBefore,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
            /* [in] */ BOOL fSegmentIsEncoded);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AppendQueryValue )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszQueryName,
            /* [string][in] */ const wchar_t __RPC_FAR *pwszQueryValue,
            /* [in] */ BOOL fIsEncoded);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateNonMutableURI )( 
            IConfigManager2MutableURI __RPC_FAR * This,
            /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);
        
        END_INTERFACE
    } IConfigManager2MutableURIVtbl;

    interface IConfigManager2MutableURI
    {
        CONST_VTBL struct IConfigManager2MutableURIVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConfigManager2MutableURI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConfigManager2MutableURI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConfigManager2MutableURI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConfigManager2MutableURI_IsAbsoluteURI(This,pfIsAbsolute)	\
    (This)->lpVtbl -> IsAbsoluteURI(This,pfIsAbsolute)

#define IConfigManager2MutableURI_HasQuery(This,pfHasQuery)	\
    (This)->lpVtbl -> HasQuery(This,pfHasQuery)

#define IConfigManager2MutableURI_InitializeFromString(This,pwszURI)	\
    (This)->lpVtbl -> InitializeFromString(This,pwszURI)

#define IConfigManager2MutableURI_InitializeFromStream(This,pStream)	\
    (This)->lpVtbl -> InitializeFromStream(This,pStream)

#define IConfigManager2MutableURI_SaveToStream(This,pStream)	\
    (This)->lpVtbl -> SaveToStream(This,pStream)

#define IConfigManager2MutableURI_GetCanonicalRelativeURI(This,idwSegIndex,fIncludeQuery,pbstrRelURI)	\
    (This)->lpVtbl -> GetCanonicalRelativeURI(This,idwSegIndex,fIncludeQuery,pbstrRelURI)

#define IConfigManager2MutableURI_GetRelativeURI(This,dwSegIndex,fIncludeQuery,pURI)	\
    (This)->lpVtbl -> GetRelativeURI(This,dwSegIndex,fIncludeQuery,pURI)

#define IConfigManager2MutableURI_SplitURI(This,idwSegSplit,fIncludeQuery,pURI1,pURI2)	\
    (This)->lpVtbl -> SplitURI(This,idwSegSplit,fIncludeQuery,pURI1,pURI2)

#define IConfigManager2MutableURI_GetSegment(This,idwSegIndex,pwszSegment)	\
    (This)->lpVtbl -> GetSegment(This,idwSegIndex,pwszSegment)

#define IConfigManager2MutableURI_GetSegmentCopy(This,idwSegIndex,pbstrSegment)	\
    (This)->lpVtbl -> GetSegmentCopy(This,idwSegIndex,pbstrSegment)

#define IConfigManager2MutableURI_CompareURI(This,pURI2,fIgnoreCase,pnCompare)	\
    (This)->lpVtbl -> CompareURI(This,pURI2,fIgnoreCase,pnCompare)

#define IConfigManager2MutableURI_FindLastCommonSegment(This,pURI,fIgnoreCase,dwCommonSegIndex)	\
    (This)->lpVtbl -> FindLastCommonSegment(This,pURI,fIgnoreCase,dwCommonSegIndex)

#define IConfigManager2MutableURI_GetJoinedSegments(This,dwSegIndex,wchJoin,pbstrJoined)	\
    (This)->lpVtbl -> GetJoinedSegments(This,dwSegIndex,wchJoin,pbstrJoined)

#define IConfigManager2MutableURI_GetQueryValue(This,pwszName,pbstrValue)	\
    (This)->lpVtbl -> GetQueryValue(This,pwszName,pbstrValue)

#define IConfigManager2MutableURI_GetSegmentCount(This,pcdwSegments)	\
    (This)->lpVtbl -> GetSegmentCount(This,pcdwSegments)

#define IConfigManager2MutableURI_Clone(This,fIncludeQuery,ppMutableURI)	\
    (This)->lpVtbl -> Clone(This,fIncludeQuery,ppMutableURI)

#define IConfigManager2MutableURI_GetHash(This,fIgnoreCase,pdwHash)	\
    (This)->lpVtbl -> GetHash(This,fIgnoreCase,pdwHash)

#define IConfigManager2MutableURI_AppendSegmentToCopy(This,pwszSegment,fSegmentIsEncoded,fIncludeQuery,ppURI)	\
    (This)->lpVtbl -> AppendSegmentToCopy(This,pwszSegment,fSegmentIsEncoded,fIncludeQuery,ppURI)

#define IConfigManager2MutableURI_AppendRelativeURIToCopy(This,pRelativeURI,dwSegIndex,fIncludeQuery,ppURI)	\
    (This)->lpVtbl -> AppendRelativeURIToCopy(This,pRelativeURI,dwSegIndex,fIncludeQuery,ppURI)


#define IConfigManager2MutableURI_AppendSegment(This,pwszSegment,fSegmentIsEncoded)	\
    (This)->lpVtbl -> AppendSegment(This,pwszSegment,fSegmentIsEncoded)

#define IConfigManager2MutableURI_AppendRelativeURI(This,pURI,idwSegment)	\
    (This)->lpVtbl -> AppendRelativeURI(This,pURI,idwSegment)

#define IConfigManager2MutableURI_ReplaceSegment(This,idwSegIndex,pwszSegment,fSegmentIsEncoded)	\
    (This)->lpVtbl -> ReplaceSegment(This,idwSegIndex,pwszSegment,fSegmentIsEncoded)

#define IConfigManager2MutableURI_DeleteSegment(This,idwSegIndex)	\
    (This)->lpVtbl -> DeleteSegment(This,idwSegIndex)

#define IConfigManager2MutableURI_InsertSegment(This,idwInsertBefore,pwszSegment,fSegmentIsEncoded)	\
    (This)->lpVtbl -> InsertSegment(This,idwInsertBefore,pwszSegment,fSegmentIsEncoded)

#define IConfigManager2MutableURI_AppendQueryValue(This,pwszQueryName,pwszQueryValue,fIsEncoded)	\
    (This)->lpVtbl -> AppendQueryValue(This,pwszQueryName,pwszQueryValue,fIsEncoded)

#define IConfigManager2MutableURI_CreateNonMutableURI(This,ppURI)	\
    (This)->lpVtbl -> CreateNonMutableURI(This,ppURI)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IConfigManager2MutableURI_AppendSegment_Proxy( 
    IConfigManager2MutableURI __RPC_FAR * This,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
    /* [in] */ BOOL fSegmentIsEncoded);


void __RPC_STUB IConfigManager2MutableURI_AppendSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2MutableURI_AppendRelativeURI_Proxy( 
    IConfigManager2MutableURI __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *pURI,
    /* [in] */ DWORD idwSegment);


void __RPC_STUB IConfigManager2MutableURI_AppendRelativeURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2MutableURI_ReplaceSegment_Proxy( 
    IConfigManager2MutableURI __RPC_FAR * This,
    /* [in] */ DWORD idwSegIndex,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
    /* [in] */ BOOL fSegmentIsEncoded);


void __RPC_STUB IConfigManager2MutableURI_ReplaceSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2MutableURI_DeleteSegment_Proxy( 
    IConfigManager2MutableURI __RPC_FAR * This,
    /* [in] */ DWORD idwSegIndex);


void __RPC_STUB IConfigManager2MutableURI_DeleteSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2MutableURI_InsertSegment_Proxy( 
    IConfigManager2MutableURI __RPC_FAR * This,
    /* [in] */ DWORD idwInsertBefore,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszSegment,
    /* [in] */ BOOL fSegmentIsEncoded);


void __RPC_STUB IConfigManager2MutableURI_InsertSegment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2MutableURI_AppendQueryValue_Proxy( 
    IConfigManager2MutableURI __RPC_FAR * This,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszQueryName,
    /* [string][in] */ const wchar_t __RPC_FAR *pwszQueryValue,
    /* [in] */ BOOL fIsEncoded);


void __RPC_STUB IConfigManager2MutableURI_AppendQueryValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IConfigManager2MutableURI_CreateNonMutableURI_Proxy( 
    IConfigManager2MutableURI __RPC_FAR * This,
    /* [out] */ IConfigManager2URI __RPC_FAR *__RPC_FAR *ppURI);


void __RPC_STUB IConfigManager2MutableURI_CreateNonMutableURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConfigManager2MutableURI_INTERFACE_DEFINED__ */


#ifndef __ICSPURITranslator_INTERFACE_DEFINED__
#define __ICSPURITranslator_INTERFACE_DEFINED__

/* interface ICSPURITranslator */
/* [object][uuid] */ 


EXTERN_C const IID IID_ICSPURITranslator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0190E18E-3A45-4634-BB1F-7A5B0C753DED")
    ICSPURITranslator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE TranslateURI( 
            /* [in] */ IConfigManager2MutableURI __RPC_FAR *puriTranslate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TranslateValue( 
            /* [in] */ IConfigManager2URI __RPC_FAR *puriNode,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICSPURITranslatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICSPURITranslator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICSPURITranslator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICSPURITranslator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TranslateURI )( 
            ICSPURITranslator __RPC_FAR * This,
            /* [in] */ IConfigManager2MutableURI __RPC_FAR *puriTranslate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TranslateValue )( 
            ICSPURITranslator __RPC_FAR * This,
            /* [in] */ IConfigManager2URI __RPC_FAR *puriNode,
            /* [out][in] */ VARIANT __RPC_FAR *pvarValue);
        
        END_INTERFACE
    } ICSPURITranslatorVtbl;

    interface ICSPURITranslator
    {
        CONST_VTBL struct ICSPURITranslatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICSPURITranslator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICSPURITranslator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICSPURITranslator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICSPURITranslator_TranslateURI(This,puriTranslate)	\
    (This)->lpVtbl -> TranslateURI(This,puriTranslate)

#define ICSPURITranslator_TranslateValue(This,puriNode,pvarValue)	\
    (This)->lpVtbl -> TranslateValue(This,puriNode,pvarValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICSPURITranslator_TranslateURI_Proxy( 
    ICSPURITranslator __RPC_FAR * This,
    /* [in] */ IConfigManager2MutableURI __RPC_FAR *puriTranslate);


void __RPC_STUB ICSPURITranslator_TranslateURI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICSPURITranslator_TranslateValue_Proxy( 
    ICSPURITranslator __RPC_FAR * This,
    /* [in] */ IConfigManager2URI __RPC_FAR *puriNode,
    /* [out][in] */ VARIANT __RPC_FAR *pvarValue);


void __RPC_STUB ICSPURITranslator_TranslateValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICSPURITranslator_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_cmgruri_0084 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_cmgruri_0084_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cmgruri_0084_v0_0_s_ifspec;

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


