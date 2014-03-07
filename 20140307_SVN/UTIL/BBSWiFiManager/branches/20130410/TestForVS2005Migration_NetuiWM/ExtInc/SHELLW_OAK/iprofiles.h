
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Jun 11 18:32:30 2008
 */
/* Compiler settings for .\iprofiles.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __iprofiles_h__
#define __iprofiles_h__

/* Forward Declarations */ 

#ifndef __ITPCProfiles_FWD_DEFINED__
#define __ITPCProfiles_FWD_DEFINED__
typedef interface ITPCProfiles ITPCProfiles;
#endif 	/* __ITPCProfiles_FWD_DEFINED__ */


/* header files for imported files */
#include "wtypes.h"
#include "oaidl.h"
#include "oleidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ITPCProfiles_INTERFACE_DEFINED__
#define __ITPCProfiles_INTERFACE_DEFINED__

/* interface ITPCProfiles */
/* [object][uuid] */ 


EXTERN_C const IID IID_ITPCProfiles;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F13D0964-E2DA-4bba-BF1A-EED82BB948D3")
    ITPCProfiles : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetActiveProfile( 
            const OLECHAR __RPC_FAR *wszProfile,
            HWND hwndOwner,
            BOOL bHideOwnerBeforeSet) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RevertToPreviousProfile( 
            const OLECHAR __RPC_FAR *wszFromProfile,
            HWND hwndOwner,
            BOOL bHideOwnerBeforeSet) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateAutoProfileMode( 
            HWND hwndOwner,
            BOOL bHideOwnerBeforeSet) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITPCProfilesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITPCProfiles __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITPCProfiles __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITPCProfiles __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetActiveProfile )( 
            ITPCProfiles __RPC_FAR * This,
            const OLECHAR __RPC_FAR *wszProfile,
            HWND hwndOwner,
            BOOL bHideOwnerBeforeSet);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RevertToPreviousProfile )( 
            ITPCProfiles __RPC_FAR * This,
            const OLECHAR __RPC_FAR *wszFromProfile,
            HWND hwndOwner,
            BOOL bHideOwnerBeforeSet);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateAutoProfileMode )( 
            ITPCProfiles __RPC_FAR * This,
            HWND hwndOwner,
            BOOL bHideOwnerBeforeSet);
        
        END_INTERFACE
    } ITPCProfilesVtbl;

    interface ITPCProfiles
    {
        CONST_VTBL struct ITPCProfilesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITPCProfiles_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITPCProfiles_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITPCProfiles_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITPCProfiles_SetActiveProfile(This,wszProfile,hwndOwner,bHideOwnerBeforeSet)	\
    (This)->lpVtbl -> SetActiveProfile(This,wszProfile,hwndOwner,bHideOwnerBeforeSet)

#define ITPCProfiles_RevertToPreviousProfile(This,wszFromProfile,hwndOwner,bHideOwnerBeforeSet)	\
    (This)->lpVtbl -> RevertToPreviousProfile(This,wszFromProfile,hwndOwner,bHideOwnerBeforeSet)

#define ITPCProfiles_UpdateAutoProfileMode(This,hwndOwner,bHideOwnerBeforeSet)	\
    (This)->lpVtbl -> UpdateAutoProfileMode(This,hwndOwner,bHideOwnerBeforeSet)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITPCProfiles_SetActiveProfile_Proxy( 
    ITPCProfiles __RPC_FAR * This,
    const OLECHAR __RPC_FAR *wszProfile,
    HWND hwndOwner,
    BOOL bHideOwnerBeforeSet);


void __RPC_STUB ITPCProfiles_SetActiveProfile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITPCProfiles_RevertToPreviousProfile_Proxy( 
    ITPCProfiles __RPC_FAR * This,
    const OLECHAR __RPC_FAR *wszFromProfile,
    HWND hwndOwner,
    BOOL bHideOwnerBeforeSet);


void __RPC_STUB ITPCProfiles_RevertToPreviousProfile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITPCProfiles_UpdateAutoProfileMode_Proxy( 
    ITPCProfiles __RPC_FAR * This,
    HWND hwndOwner,
    BOOL bHideOwnerBeforeSet);


void __RPC_STUB ITPCProfiles_UpdateAutoProfileMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITPCProfiles_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long __RPC_FAR *, unsigned long            , HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long __RPC_FAR *, HWND __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


