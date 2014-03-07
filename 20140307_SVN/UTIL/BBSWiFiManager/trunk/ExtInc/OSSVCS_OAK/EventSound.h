
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:21:59 2008
 */
/* Compiler settings for .\EventSound.idl:
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

#ifndef __EventSound_h__
#define __EventSound_h__

/* Forward Declarations */ 

#ifndef __IEventSound_FWD_DEFINED__
#define __IEventSound_FWD_DEFINED__
typedef interface IEventSound IEventSound;
#endif 	/* __IEventSound_FWD_DEFINED__ */


#ifndef __IProtectedEventSound_FWD_DEFINED__
#define __IProtectedEventSound_FWD_DEFINED__
typedef interface IProtectedEventSound IProtectedEventSound;
#endif 	/* __IProtectedEventSound_FWD_DEFINED__ */


#ifndef __IEventSound2_FWD_DEFINED__
#define __IEventSound2_FWD_DEFINED__
typedef interface IEventSound2 IEventSound2;
#endif 	/* __IEventSound2_FWD_DEFINED__ */


#ifndef __IEventSoundExInternal_FWD_DEFINED__
#define __IEventSoundExInternal_FWD_DEFINED__
typedef interface IEventSoundExInternal IEventSoundExInternal;
#endif 	/* __IEventSoundExInternal_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_EventSound_0000 */
/* [local] */ 

#if 0
typedef HANDLE HWAVEOUT;

#endif


extern RPC_IF_HANDLE __MIDL_itf_EventSound_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_EventSound_0000_v0_0_s_ifspec;

#ifndef __IEventSound_INTERFACE_DEFINED__
#define __IEventSound_INTERFACE_DEFINED__

/* interface IEventSound */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IEventSound;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("75B08FA4-B32B-429e-B0FD-7C7FFF211C55")
    IEventSound : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename,
            /* [out] */ HWAVEOUT __RPC_FAR *phDevice) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ HANDLE hDoneEvent,
            /* [in] */ BOOL bLoop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventSoundVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEventSound __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEventSound __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEventSound __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IEventSound __RPC_FAR * This,
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename,
            /* [out] */ HWAVEOUT __RPC_FAR *phDevice);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Play )( 
            IEventSound __RPC_FAR * This,
            /* [in] */ HANDLE hDoneEvent,
            /* [in] */ BOOL bLoop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IEventSound __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            IEventSound __RPC_FAR * This);
        
        END_INTERFACE
    } IEventSoundVtbl;

    interface IEventSound
    {
        CONST_VTBL struct IEventSoundVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventSound_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventSound_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventSound_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventSound_Open(This,pwszFilename,phDevice)	\
    (This)->lpVtbl -> Open(This,pwszFilename,phDevice)

#define IEventSound_Play(This,hDoneEvent,bLoop)	\
    (This)->lpVtbl -> Play(This,hDoneEvent,bLoop)

#define IEventSound_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IEventSound_Close(This)	\
    (This)->lpVtbl -> Close(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEventSound_Open_Proxy( 
    IEventSound __RPC_FAR * This,
    /* [in] */ const WCHAR __RPC_FAR *pwszFilename,
    /* [out] */ HWAVEOUT __RPC_FAR *phDevice);


void __RPC_STUB IEventSound_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound_Play_Proxy( 
    IEventSound __RPC_FAR * This,
    /* [in] */ HANDLE hDoneEvent,
    /* [in] */ BOOL bLoop);


void __RPC_STUB IEventSound_Play_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound_Stop_Proxy( 
    IEventSound __RPC_FAR * This);


void __RPC_STUB IEventSound_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound_Close_Proxy( 
    IEventSound __RPC_FAR * This);


void __RPC_STUB IEventSound_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventSound_INTERFACE_DEFINED__ */


#ifndef __IProtectedEventSound_INTERFACE_DEFINED__
#define __IProtectedEventSound_INTERFACE_DEFINED__

/* interface IProtectedEventSound */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IProtectedEventSound;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3E76E5FD-DDA8-4be1-BEA4-6F3E9F6FF139")
    IProtectedEventSound : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE IsPlayable( 
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MakePlayable( 
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename,
            /* [in] */ HWND hwndOwner) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProtectedEventSoundVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IProtectedEventSound __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IProtectedEventSound __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IProtectedEventSound __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPlayable )( 
            IProtectedEventSound __RPC_FAR * This,
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MakePlayable )( 
            IProtectedEventSound __RPC_FAR * This,
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename,
            /* [in] */ HWND hwndOwner);
        
        END_INTERFACE
    } IProtectedEventSoundVtbl;

    interface IProtectedEventSound
    {
        CONST_VTBL struct IProtectedEventSoundVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProtectedEventSound_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProtectedEventSound_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProtectedEventSound_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProtectedEventSound_IsPlayable(This,pwszFilename)	\
    (This)->lpVtbl -> IsPlayable(This,pwszFilename)

#define IProtectedEventSound_MakePlayable(This,pwszFilename,hwndOwner)	\
    (This)->lpVtbl -> MakePlayable(This,pwszFilename,hwndOwner)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IProtectedEventSound_IsPlayable_Proxy( 
    IProtectedEventSound __RPC_FAR * This,
    /* [in] */ const WCHAR __RPC_FAR *pwszFilename);


void __RPC_STUB IProtectedEventSound_IsPlayable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProtectedEventSound_MakePlayable_Proxy( 
    IProtectedEventSound __RPC_FAR * This,
    /* [in] */ const WCHAR __RPC_FAR *pwszFilename,
    /* [in] */ HWND hwndOwner);


void __RPC_STUB IProtectedEventSound_MakePlayable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProtectedEventSound_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_EventSound_0148 */
/* [local] */ 

#define ES_AUDIO_PROP_FORCE_SPEAKER              0x00000001
DEFINE_GUID(IID_IEventSound2,0X66FE27E3,0XBC32,0X493D,0X85,0XC0,0XCB,0XCC,0X72,0XC2,0XAD,0X1C);


extern RPC_IF_HANDLE __MIDL_itf_EventSound_0148_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_EventSound_0148_v0_0_s_ifspec;

#ifndef __IEventSound2_INTERFACE_DEFINED__
#define __IEventSound2_INTERFACE_DEFINED__

/* interface IEventSound2 */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IEventSound2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("66FE27E3-BC32-493d-85C0-CBCC72C2AD1C")
    IEventSound2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ HANDLE hDoneEvent,
            /* [in] */ BOOL bLoop) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVolume( 
            /* [out] */ DWORD __RPC_FAR *pdwVolume) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetVolume( 
            /* [in] */ DWORD dwVolume) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAudioProperties( 
            /* [in] */ DWORD dwNewProperties,
            /* [in] */ DWORD dwNewPropertiesMask) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSecondaryGainClass( 
            /* [in] */ DWORD dwAttenuationClass) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventSound2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEventSound2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEventSound2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEventSound2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IEventSound2 __RPC_FAR * This,
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Play )( 
            IEventSound2 __RPC_FAR * This,
            /* [in] */ HANDLE hDoneEvent,
            /* [in] */ BOOL bLoop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IEventSound2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            IEventSound2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVolume )( 
            IEventSound2 __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *pdwVolume);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVolume )( 
            IEventSound2 __RPC_FAR * This,
            /* [in] */ DWORD dwVolume);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAudioProperties )( 
            IEventSound2 __RPC_FAR * This,
            /* [in] */ DWORD dwNewProperties,
            /* [in] */ DWORD dwNewPropertiesMask);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSecondaryGainClass )( 
            IEventSound2 __RPC_FAR * This,
            /* [in] */ DWORD dwAttenuationClass);
        
        END_INTERFACE
    } IEventSound2Vtbl;

    interface IEventSound2
    {
        CONST_VTBL struct IEventSound2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventSound2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventSound2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventSound2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventSound2_Open(This,pwszFilename)	\
    (This)->lpVtbl -> Open(This,pwszFilename)

#define IEventSound2_Play(This,hDoneEvent,bLoop)	\
    (This)->lpVtbl -> Play(This,hDoneEvent,bLoop)

#define IEventSound2_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IEventSound2_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IEventSound2_GetVolume(This,pdwVolume)	\
    (This)->lpVtbl -> GetVolume(This,pdwVolume)

#define IEventSound2_SetVolume(This,dwVolume)	\
    (This)->lpVtbl -> SetVolume(This,dwVolume)

#define IEventSound2_SetAudioProperties(This,dwNewProperties,dwNewPropertiesMask)	\
    (This)->lpVtbl -> SetAudioProperties(This,dwNewProperties,dwNewPropertiesMask)

#define IEventSound2_SetSecondaryGainClass(This,dwAttenuationClass)	\
    (This)->lpVtbl -> SetSecondaryGainClass(This,dwAttenuationClass)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEventSound2_Open_Proxy( 
    IEventSound2 __RPC_FAR * This,
    /* [in] */ const WCHAR __RPC_FAR *pwszFilename);


void __RPC_STUB IEventSound2_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound2_Play_Proxy( 
    IEventSound2 __RPC_FAR * This,
    /* [in] */ HANDLE hDoneEvent,
    /* [in] */ BOOL bLoop);


void __RPC_STUB IEventSound2_Play_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound2_Stop_Proxy( 
    IEventSound2 __RPC_FAR * This);


void __RPC_STUB IEventSound2_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound2_Close_Proxy( 
    IEventSound2 __RPC_FAR * This);


void __RPC_STUB IEventSound2_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound2_GetVolume_Proxy( 
    IEventSound2 __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *pdwVolume);


void __RPC_STUB IEventSound2_GetVolume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound2_SetVolume_Proxy( 
    IEventSound2 __RPC_FAR * This,
    /* [in] */ DWORD dwVolume);


void __RPC_STUB IEventSound2_SetVolume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound2_SetAudioProperties_Proxy( 
    IEventSound2 __RPC_FAR * This,
    /* [in] */ DWORD dwNewProperties,
    /* [in] */ DWORD dwNewPropertiesMask);


void __RPC_STUB IEventSound2_SetAudioProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEventSound2_SetSecondaryGainClass_Proxy( 
    IEventSound2 __RPC_FAR * This,
    /* [in] */ DWORD dwAttenuationClass);


void __RPC_STUB IEventSound2_SetSecondaryGainClass_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventSound2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_EventSound_0149 */
/* [local] */ 

DEFINE_GUID(IID_IEventSound,0x75B08FA4,0xB32B,0x429e,0xB0,0xFD,0x7C,0x7F,0xFF,0x21,0x1C,0x55);
DEFINE_GUID(IID_IProtectedEventSound,0x3E76E5FD,0xDDA8,0x4be1,0xBE,0xA4,0x6F,0x3E,0x9F,0x6F,0xF1,0x39);


extern RPC_IF_HANDLE __MIDL_itf_EventSound_0149_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_EventSound_0149_v0_0_s_ifspec;

#ifndef __IEventSoundExInternal_INTERFACE_DEFINED__
#define __IEventSoundExInternal_INTERFACE_DEFINED__

/* interface IEventSoundExInternal */
/* [unique][uuid][local][object] */ 


EXTERN_C const IID IID_IEventSoundExInternal;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("72A57657-DF20-498C-8C26-DCD4A53B6635")
    IEventSoundExInternal : public IEventSound
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetAudioProperties( 
            /* [in] */ DWORD dwNewProperties,
            /* [in] */ DWORD dwNewPropertiesMask) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventSoundExInternalVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEventSoundExInternal __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEventSoundExInternal __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEventSoundExInternal __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IEventSoundExInternal __RPC_FAR * This,
            /* [in] */ const WCHAR __RPC_FAR *pwszFilename,
            /* [out] */ HWAVEOUT __RPC_FAR *phDevice);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Play )( 
            IEventSoundExInternal __RPC_FAR * This,
            /* [in] */ HANDLE hDoneEvent,
            /* [in] */ BOOL bLoop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IEventSoundExInternal __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            IEventSoundExInternal __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAudioProperties )( 
            IEventSoundExInternal __RPC_FAR * This,
            /* [in] */ DWORD dwNewProperties,
            /* [in] */ DWORD dwNewPropertiesMask);
        
        END_INTERFACE
    } IEventSoundExInternalVtbl;

    interface IEventSoundExInternal
    {
        CONST_VTBL struct IEventSoundExInternalVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventSoundExInternal_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEventSoundExInternal_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEventSoundExInternal_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEventSoundExInternal_Open(This,pwszFilename,phDevice)	\
    (This)->lpVtbl -> Open(This,pwszFilename,phDevice)

#define IEventSoundExInternal_Play(This,hDoneEvent,bLoop)	\
    (This)->lpVtbl -> Play(This,hDoneEvent,bLoop)

#define IEventSoundExInternal_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IEventSoundExInternal_Close(This)	\
    (This)->lpVtbl -> Close(This)


#define IEventSoundExInternal_SetAudioProperties(This,dwNewProperties,dwNewPropertiesMask)	\
    (This)->lpVtbl -> SetAudioProperties(This,dwNewProperties,dwNewPropertiesMask)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IEventSoundExInternal_SetAudioProperties_Proxy( 
    IEventSoundExInternal __RPC_FAR * This,
    /* [in] */ DWORD dwNewProperties,
    /* [in] */ DWORD dwNewPropertiesMask);


void __RPC_STUB IEventSoundExInternal_SetAudioProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEventSoundExInternal_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_EventSound_0150 */
/* [local] */ 

DEFINE_GUID(IID_IEventSoundExInternal,0x72A57657,0xDF20,0x498C,0x8C,0x26,0xDC,0xD4,0xA5,0x3B,0x66,0x35);


extern RPC_IF_HANDLE __MIDL_itf_EventSound_0150_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_EventSound_0150_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


