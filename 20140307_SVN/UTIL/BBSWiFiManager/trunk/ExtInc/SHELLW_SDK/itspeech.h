
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:32:41 2008
 */
/* Compiler settings for .\itspeech.idl:
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

#ifndef __itspeech_h__
#define __itspeech_h__

/* Forward Declarations */ 

#ifndef __ISpRecognizerLite_FWD_DEFINED__
#define __ISpRecognizerLite_FWD_DEFINED__
typedef interface ISpRecognizerLite ISpRecognizerLite;
#endif 	/* __ISpRecognizerLite_FWD_DEFINED__ */


/* header files for imported files */
#include "wtypes.h"
#include "oaidl.h"
#include "oleidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_itspeech_0000 */
/* [local] */ 

typedef DWORD SPRECOGNITIONMETHOD;

typedef DWORD __RPC_FAR *PSPRECOGNITIONMETHOD;

#define SPRM_INVALID             0x00000000
#define SPRM_VOICETAG            0x00000001
#define SPRM_CONTINUOUSDIGIT     0x00000002
#define SPRM_ANY                 0xFFFFFFFF
typedef 
enum __SPTYPE
    {	SPT_NULL	= 0,
	SPT_DWORD	= SPT_NULL + 1,
	SPT_BSTR	= SPT_DWORD + 1,
	SPT_PROP	= SPT_BSTR + 1
    }	SPTYPE;

typedef enum __SPTYPE __RPC_FAR *PSPTYPE;

typedef struct __SPTYPEHEADER
    {
    UINT cbSize;
    SPTYPE spt;
    }	SPTYPEHEADER;

typedef struct __SPTYPEHEADER __RPC_FAR *PSPTYPEHEADER;

typedef struct __SPDWORD
    {
    SPTYPEHEADER spth;
    DWORD dwData;
    }	SPDWORD;

typedef struct __SPDWORD __RPC_FAR *PSPDWORD;

typedef struct __SPBSTR
    {
    SPTYPEHEADER spth;
    UINT cbBstr;
    BSTR bstr;
    }	SPBSTR;

typedef struct __SPBSTR __RPC_FAR *PSPBSTR;

typedef 
enum __SPPROPTYPE
    {	SPPT_NULL	= 0,
	SPPT_CUSTOMUI	= SPPT_NULL + 1,
	SPPT_CONTINUOUSDIGIT_TRAINING_PROGRESS	= SPPT_CUSTOMUI + 1
    }	SPPROPTYPE;

typedef enum __SPPROPTYPE __RPC_FAR *PSPPROPTYPE;

typedef struct __SPPROP
    {
    SPTYPEHEADER spth;
    SPPROPTYPE sppt;
    DWORD dwValue;
    }	SPPROP;

typedef struct __SPPROP __RPC_FAR *PSPPROP;



extern RPC_IF_HANDLE __MIDL_itf_itspeech_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_itspeech_0000_v0_0_s_ifspec;

#ifndef __ISpRecognizerLite_INTERFACE_DEFINED__
#define __ISpRecognizerLite_INTERFACE_DEFINED__

/* interface ISpRecognizerLite */
/* [object][uuid] */ 


EXTERN_C const IID IID_ISpRecognizerLite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("85140985-7A18-4009-B5FB-43268FD154F8")
    ISpRecognizerLite : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Abort( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Train( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Recognize( 
            /* [out][in] */ PSPRECOGNITIONMETHOD peRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Playback( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteAll( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReTrain( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumItems( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [in] */ UINT index,
            /* [out] */ SPTYPEHEADER __RPC_FAR *psth) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableItem( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ BOOL fEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVoiceInfo( 
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [in] */ SPTYPEHEADER __RPC_FAR *psthID,
            /* [out] */ SPTYPEHEADER __RPC_FAR *psthSampleInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpRecognizerLiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpRecognizerLite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpRecognizerLite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Abort )( 
            ISpRecognizerLite __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Train )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Recognize )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [out][in] */ PSPRECOGNITIONMETHOD peRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Playback )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Delete )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteAll )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReTrain )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ IUnknown __RPC_FAR *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumItems )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [in] */ UINT index,
            /* [out] */ SPTYPEHEADER __RPC_FAR *psth);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableItem )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
            /* [in] */ BOOL fEnable);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVoiceInfo )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [in] */ SPRECOGNITIONMETHOD eRM,
            /* [in] */ SPTYPEHEADER __RPC_FAR *psthID,
            /* [out] */ SPTYPEHEADER __RPC_FAR *psthSampleInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetProperty )( 
            ISpRecognizerLite __RPC_FAR * This,
            /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth);
        
        END_INTERFACE
    } ISpRecognizerLiteVtbl;

    interface ISpRecognizerLite
    {
        CONST_VTBL struct ISpRecognizerLiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpRecognizerLite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpRecognizerLite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpRecognizerLite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpRecognizerLite_Initialize(This,psth)	\
    (This)->lpVtbl -> Initialize(This,psth)

#define ISpRecognizerLite_Abort(This)	\
    (This)->lpVtbl -> Abort(This)

#define ISpRecognizerLite_Train(This,eRM,psth,pUnk)	\
    (This)->lpVtbl -> Train(This,eRM,psth,pUnk)

#define ISpRecognizerLite_Recognize(This,peRM,psth,pUnk)	\
    (This)->lpVtbl -> Recognize(This,peRM,psth,pUnk)

#define ISpRecognizerLite_Playback(This,eRM,psth,pUnk)	\
    (This)->lpVtbl -> Playback(This,eRM,psth,pUnk)

#define ISpRecognizerLite_Delete(This,eRM,psth,pUnk)	\
    (This)->lpVtbl -> Delete(This,eRM,psth,pUnk)

#define ISpRecognizerLite_DeleteAll(This,eRM,pUnk)	\
    (This)->lpVtbl -> DeleteAll(This,eRM,pUnk)

#define ISpRecognizerLite_ReTrain(This,eRM,psth,pUnk)	\
    (This)->lpVtbl -> ReTrain(This,eRM,psth,pUnk)

#define ISpRecognizerLite_EnumItems(This,eRM,index,psth)	\
    (This)->lpVtbl -> EnumItems(This,eRM,index,psth)

#define ISpRecognizerLite_EnableItem(This,eRM,psth,fEnable)	\
    (This)->lpVtbl -> EnableItem(This,eRM,psth,fEnable)

#define ISpRecognizerLite_GetVoiceInfo(This,eRM,psthID,psthSampleInfo)	\
    (This)->lpVtbl -> GetVoiceInfo(This,eRM,psthID,psthSampleInfo)

#define ISpRecognizerLite_GetProperty(This,psth)	\
    (This)->lpVtbl -> GetProperty(This,psth)

#define ISpRecognizerLite_SetProperty(This,psth)	\
    (This)->lpVtbl -> SetProperty(This,psth)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISpRecognizerLite_Initialize_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth);


void __RPC_STUB ISpRecognizerLite_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_Abort_Proxy( 
    ISpRecognizerLite __RPC_FAR * This);


void __RPC_STUB ISpRecognizerLite_Abort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_Train_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ISpRecognizerLite_Train_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_Recognize_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [out][in] */ PSPRECOGNITIONMETHOD peRM,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ISpRecognizerLite_Recognize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_Playback_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ISpRecognizerLite_Playback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_Delete_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ISpRecognizerLite_Delete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_DeleteAll_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ISpRecognizerLite_DeleteAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_ReTrain_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
    /* [in] */ IUnknown __RPC_FAR *pUnk);


void __RPC_STUB ISpRecognizerLite_ReTrain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_EnumItems_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [in] */ UINT index,
    /* [out] */ SPTYPEHEADER __RPC_FAR *psth);


void __RPC_STUB ISpRecognizerLite_EnumItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_EnableItem_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth,
    /* [in] */ BOOL fEnable);


void __RPC_STUB ISpRecognizerLite_EnableItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_GetVoiceInfo_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [in] */ SPRECOGNITIONMETHOD eRM,
    /* [in] */ SPTYPEHEADER __RPC_FAR *psthID,
    /* [out] */ SPTYPEHEADER __RPC_FAR *psthSampleInfo);


void __RPC_STUB ISpRecognizerLite_GetVoiceInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_GetProperty_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth);


void __RPC_STUB ISpRecognizerLite_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpRecognizerLite_SetProperty_Proxy( 
    ISpRecognizerLite __RPC_FAR * This,
    /* [out][in] */ SPTYPEHEADER __RPC_FAR *psth);


void __RPC_STUB ISpRecognizerLite_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpRecognizerLite_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


