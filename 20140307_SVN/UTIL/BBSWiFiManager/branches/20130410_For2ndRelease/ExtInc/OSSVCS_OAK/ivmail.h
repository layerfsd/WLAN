
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:22:02 2008
 */
/* Compiler settings for .\ivmail.idl:
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

#ifndef __ivmail_h__
#define __ivmail_h__

/* Forward Declarations */ 

#ifndef __IVMailServerCaller_FWD_DEFINED__
#define __IVMailServerCaller_FWD_DEFINED__
typedef interface IVMailServerCaller IVMailServerCaller;
#endif 	/* __IVMailServerCaller_FWD_DEFINED__ */


#ifndef __VMailServerCaller_FWD_DEFINED__
#define __VMailServerCaller_FWD_DEFINED__

#ifdef __cplusplus
typedef class VMailServerCaller VMailServerCaller;
#else
typedef struct VMailServerCaller VMailServerCaller;
#endif /* __cplusplus */

#endif 	/* __VMailServerCaller_FWD_DEFINED__ */


/* header files for imported files */
#include "wtypes.h"
#include "oaidl.h"
#include "oleidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IVMailServerCaller_INTERFACE_DEFINED__
#define __IVMailServerCaller_INTERFACE_DEFINED__

/* interface IVMailServerCaller */
/* [object][uuid] */ 


EXTERN_C const IID IID_IVMailServerCaller;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("78C038E8-F522-41b8-90C9-E57A3CB76E50")
    IVMailServerCaller : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Dial( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVMailServerNumber( 
            /* [out] */ BSTR __RPC_FAR *pbstrNumber) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetVMailServerNumber( 
            /* [in] */ BSTR pbstrNumber) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DialEx( 
            /* [in] */ UINT nLineNumber) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVMailServerNumberEx( 
            /* [out] */ BSTR __RPC_FAR *pbstrNumber,
            /* [in] */ UINT nLineNumber) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetVMailServerNumberEx( 
            /* [in] */ BSTR pbstrNumber,
            /* [in] */ UINT nLineNumber) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVMailServerCallerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVMailServerCaller __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVMailServerCaller __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVMailServerCaller __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dial )( 
            IVMailServerCaller __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVMailServerNumber )( 
            IVMailServerCaller __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pbstrNumber);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVMailServerNumber )( 
            IVMailServerCaller __RPC_FAR * This,
            /* [in] */ BSTR pbstrNumber);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DialEx )( 
            IVMailServerCaller __RPC_FAR * This,
            /* [in] */ UINT nLineNumber);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVMailServerNumberEx )( 
            IVMailServerCaller __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pbstrNumber,
            /* [in] */ UINT nLineNumber);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVMailServerNumberEx )( 
            IVMailServerCaller __RPC_FAR * This,
            /* [in] */ BSTR pbstrNumber,
            /* [in] */ UINT nLineNumber);
        
        END_INTERFACE
    } IVMailServerCallerVtbl;

    interface IVMailServerCaller
    {
        CONST_VTBL struct IVMailServerCallerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVMailServerCaller_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVMailServerCaller_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVMailServerCaller_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVMailServerCaller_Dial(This)	\
    (This)->lpVtbl -> Dial(This)

#define IVMailServerCaller_GetVMailServerNumber(This,pbstrNumber)	\
    (This)->lpVtbl -> GetVMailServerNumber(This,pbstrNumber)

#define IVMailServerCaller_SetVMailServerNumber(This,pbstrNumber)	\
    (This)->lpVtbl -> SetVMailServerNumber(This,pbstrNumber)

#define IVMailServerCaller_DialEx(This,nLineNumber)	\
    (This)->lpVtbl -> DialEx(This,nLineNumber)

#define IVMailServerCaller_GetVMailServerNumberEx(This,pbstrNumber,nLineNumber)	\
    (This)->lpVtbl -> GetVMailServerNumberEx(This,pbstrNumber,nLineNumber)

#define IVMailServerCaller_SetVMailServerNumberEx(This,pbstrNumber,nLineNumber)	\
    (This)->lpVtbl -> SetVMailServerNumberEx(This,pbstrNumber,nLineNumber)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IVMailServerCaller_Dial_Proxy( 
    IVMailServerCaller __RPC_FAR * This);


void __RPC_STUB IVMailServerCaller_Dial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVMailServerCaller_GetVMailServerNumber_Proxy( 
    IVMailServerCaller __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pbstrNumber);


void __RPC_STUB IVMailServerCaller_GetVMailServerNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVMailServerCaller_SetVMailServerNumber_Proxy( 
    IVMailServerCaller __RPC_FAR * This,
    /* [in] */ BSTR pbstrNumber);


void __RPC_STUB IVMailServerCaller_SetVMailServerNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVMailServerCaller_DialEx_Proxy( 
    IVMailServerCaller __RPC_FAR * This,
    /* [in] */ UINT nLineNumber);


void __RPC_STUB IVMailServerCaller_DialEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVMailServerCaller_GetVMailServerNumberEx_Proxy( 
    IVMailServerCaller __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pbstrNumber,
    /* [in] */ UINT nLineNumber);


void __RPC_STUB IVMailServerCaller_GetVMailServerNumberEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IVMailServerCaller_SetVMailServerNumberEx_Proxy( 
    IVMailServerCaller __RPC_FAR * This,
    /* [in] */ BSTR pbstrNumber,
    /* [in] */ UINT nLineNumber);


void __RPC_STUB IVMailServerCaller_SetVMailServerNumberEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVMailServerCaller_INTERFACE_DEFINED__ */



#ifndef __VMailLibrary_LIBRARY_DEFINED__
#define __VMailLibrary_LIBRARY_DEFINED__

/* library VMailLibrary */
/* [uuid] */ 


EXTERN_C const IID LIBID_VMailLibrary;

EXTERN_C const CLSID CLSID_VMailServerCaller;

#ifdef __cplusplus

class DECLSPEC_UUID("09438D64-7037-4d67-A056-7BF3703309A9")
VMailServerCaller;
#endif
#endif /* __VMailLibrary_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


