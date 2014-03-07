//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 5.03.0279 */
/* at Mon Jan 28 11:29:11 2002
 */
/* Compiler settings for .\dxtransp.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AXP64)

#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IDXRasterizer,0x9EA3B635,0xC37D,0x11d1,0x90,0x5E,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IDXTLabel,0xC0C17F0E,0xAE41,0x11d1,0x9A,0x3B,0x00,0x00,0xF8,0x75,0x6A,0x10);


MIDL_DEFINE_GUID(IID, IID_IDX2DDebug,0x03BB2457,0xA279,0x11d1,0x81,0xC6,0x00,0x00,0xF8,0x75,0x57,0xDB);


MIDL_DEFINE_GUID(IID, IID_IDX2D,0x9EFD02A9,0xA996,0x11d1,0x81,0xC9,0x00,0x00,0xF8,0x75,0x57,0xDB);


MIDL_DEFINE_GUID(IID, IID_IDXGradient2,0xd0ef2a80,0x61dc,0x11d2,0xb2,0xeb,0x00,0xa0,0xc9,0x36,0xb2,0x12);


MIDL_DEFINE_GUID(IID, IID_IDXTFilterCollection,0x22B07B33,0x8BFB,0x49d4,0x9B,0x90,0x09,0x38,0x37,0x0C,0x90,0x19);


MIDL_DEFINE_GUID(IID, IID_IDXTFilterBehavior,0x14D7DDDD,0xACA2,0x4E45,0x95,0x04,0x38,0x08,0xAB,0xEB,0x4F,0x92);


MIDL_DEFINE_GUID(IID, IID_IDXTFilterBehaviorSite,0x909B23C2,0x9018,0x499f,0xA8,0x6D,0x4E,0x7D,0xA9,0x37,0xE9,0x31);


MIDL_DEFINE_GUID(IID, IID_IDXTFilter,0xEDA1D7FF,0x4426,0x407a,0x9E,0xA4,0xDE,0x63,0xA4,0x1C,0x09,0xDA);


MIDL_DEFINE_GUID(IID, IID_IDXTRedirectFilterInit,0xD1A57094,0x21F7,0x4e6c,0x93,0xE5,0xF5,0xF7,0x7F,0x74,0x82,0x93);


MIDL_DEFINE_GUID(IID, IID_IDXTClipOrigin,0xEE1663D8,0x0988,0x4C48,0x9F,0xD6,0xDB,0x44,0x50,0x88,0x56,0x68);


MIDL_DEFINE_GUID(IID, LIBID_DXTRANSPLib,0x527A4DA4,0x7F2C,0x11d2,0xB1,0x2D,0x00,0x00,0xF8,0x1F,0x59,0x95);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTLabel,0x54702535,0x2606,0x11D1,0x99,0x9C,0x00,0x00,0xF8,0x75,0x6A,0x10);


MIDL_DEFINE_GUID(CLSID, CLSID_DXRasterizer,0x8652CE55,0x9E80,0x11D1,0x90,0x53,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(CLSID, CLSID_DX2D,0x473AA80B,0x4577,0x11D1,0x81,0xA8,0x00,0x00,0xF8,0x75,0x57,0xDB);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilterBehavior,0x649EEC1E,0xB579,0x4E8C,0xBB,0x3B,0x49,0x97,0xF8,0x42,0x65,0x36);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilter,0x385A91BC,0x1E8A,0x4e4a,0xA7,0xA6,0xF4,0xFC,0x1E,0x6C,0xA1,0xBD);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilterFactory,0x81397204,0xF51A,0x4571,0x8D,0x7B,0xDC,0x03,0x05,0x21,0xAA,0xBD);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilterCollection,0xA7EE7F34,0x3BD1,0x427f,0x92,0x31,0xF9,0x41,0xE9,0xB7,0xE1,0xFE);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AXP64)*/


#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 5.03.0279 */
/* at Mon Jan 28 11:29:11 2002
 */
/* Compiler settings for .\dxtransp.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win64 (32b run,appending), ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AXP64)

#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IDXRasterizer,0x9EA3B635,0xC37D,0x11d1,0x90,0x5E,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(IID, IID_IDXTLabel,0xC0C17F0E,0xAE41,0x11d1,0x9A,0x3B,0x00,0x00,0xF8,0x75,0x6A,0x10);


MIDL_DEFINE_GUID(IID, IID_IDX2DDebug,0x03BB2457,0xA279,0x11d1,0x81,0xC6,0x00,0x00,0xF8,0x75,0x57,0xDB);


MIDL_DEFINE_GUID(IID, IID_IDX2D,0x9EFD02A9,0xA996,0x11d1,0x81,0xC9,0x00,0x00,0xF8,0x75,0x57,0xDB);


MIDL_DEFINE_GUID(IID, IID_IDXGradient2,0xd0ef2a80,0x61dc,0x11d2,0xb2,0xeb,0x00,0xa0,0xc9,0x36,0xb2,0x12);


MIDL_DEFINE_GUID(IID, IID_IDXTFilterCollection,0x22B07B33,0x8BFB,0x49d4,0x9B,0x90,0x09,0x38,0x37,0x0C,0x90,0x19);


MIDL_DEFINE_GUID(IID, IID_IDXTFilterBehavior,0x14D7DDDD,0xACA2,0x4E45,0x95,0x04,0x38,0x08,0xAB,0xEB,0x4F,0x92);


MIDL_DEFINE_GUID(IID, IID_IDXTFilterBehaviorSite,0x909B23C2,0x9018,0x499f,0xA8,0x6D,0x4E,0x7D,0xA9,0x37,0xE9,0x31);


MIDL_DEFINE_GUID(IID, IID_IDXTFilter,0xEDA1D7FF,0x4426,0x407a,0x9E,0xA4,0xDE,0x63,0xA4,0x1C,0x09,0xDA);


MIDL_DEFINE_GUID(IID, IID_IDXTRedirectFilterInit,0xD1A57094,0x21F7,0x4e6c,0x93,0xE5,0xF5,0xF7,0x7F,0x74,0x82,0x93);


MIDL_DEFINE_GUID(IID, IID_IDXTClipOrigin,0xEE1663D8,0x0988,0x4C48,0x9F,0xD6,0xDB,0x44,0x50,0x88,0x56,0x68);


MIDL_DEFINE_GUID(IID, LIBID_DXTRANSPLib,0x527A4DA4,0x7F2C,0x11d2,0xB1,0x2D,0x00,0x00,0xF8,0x1F,0x59,0x95);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTLabel,0x54702535,0x2606,0x11D1,0x99,0x9C,0x00,0x00,0xF8,0x75,0x6A,0x10);


MIDL_DEFINE_GUID(CLSID, CLSID_DXRasterizer,0x8652CE55,0x9E80,0x11D1,0x90,0x53,0x00,0xC0,0x4F,0xD9,0x18,0x9D);


MIDL_DEFINE_GUID(CLSID, CLSID_DX2D,0x473AA80B,0x4577,0x11D1,0x81,0xA8,0x00,0x00,0xF8,0x75,0x57,0xDB);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilterBehavior,0x649EEC1E,0xB579,0x4E8C,0xBB,0x3B,0x49,0x97,0xF8,0x42,0x65,0x36);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilter,0x385A91BC,0x1E8A,0x4e4a,0xA7,0xA6,0xF4,0xFC,0x1E,0x6C,0xA1,0xBD);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilterFactory,0x81397204,0xF51A,0x4571,0x8D,0x7B,0xDC,0x03,0x05,0x21,0xAA,0xBD);


MIDL_DEFINE_GUID(CLSID, CLSID_DXTFilterCollection,0xA7EE7F34,0x3BD1,0x427f,0x92,0x31,0xF9,0x41,0xE9,0xB7,0xE1,0xFE);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* defined(_M_IA64) || defined(_M_AXP64)*/
