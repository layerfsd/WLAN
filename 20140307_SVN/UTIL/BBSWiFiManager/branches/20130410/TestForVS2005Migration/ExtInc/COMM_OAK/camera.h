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
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include <windows.h>
#include <Cs.h>
#include <Csmedia.h>

#define DRIVER_VERSION       1

#define DEVCLASS_CAMERA_GUID { 0xCB998A05, 0x122C, 0x4166, {0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86 }}
#define DEVCLASS_PIN_GUID    { 0xC9D092D6, 0x827A, 0x45E2, {0x81, 0x44, 0xDE, 0x19, 0x82, 0xBF, 0xC3, 0xA8 }}

// The following GUID must be exposed by all 3rd party camera drivers. At the moment this GUID is only consumed
// by Device Manager while disabling camera devices.
#define DEVCLASS_3PT_CAMERA_GUID { 0xcb0f3e02, 0x534f, 0x4826, {0xa0, 0x44, 0xf0, 0x8f, 0x10, 0xe6, 0x44, 0x8d }}


#define IOCTL_STREAM_INSTANTIATE          CTL_CODE(FILE_DEVICE_KS, 0x007, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CS_ALLOCATE_BUFFERS         CTL_CODE(FILE_DEVICE_KS, 0x008, METHOD_NEITHER, FILE_ANY_ACCESS)

// {443A0706-9554-41f4-BB70-ECB5D210FB3B}
#define STATIC_CSPROPSETID_StreamEx\
    0x443a0706L, 0x9554, 0x41f4, 0xbb, 0x70, 0xec, 0xb5, 0xd2, 0x10, 0xfb, 0x3b
DEFINE_GUIDSTRUCT("443A0706-9554-41f4-BB70-ECB5D210FB3B", CSPROPSETID_StreamEx);	
#define CSPROPSETID_StreamEx DEFINE_GUIDNAMED(CSPROPSETID_StreamEx)

typedef enum {
    CSPROPERTY_BUFFER_DRIVER = 1,
    CSPROPERTY_BUFFER_CLIENT_LIMITED = 2,
    CSPROPERTY_BUFFER_CLIENT_UNLIMITED = 4
} CSPROPERTY_BUFFER_MODE;


typedef enum
{
    CSPROPERTY_STREAMEX_INIT = 0,
} CSPROPERTY_STREAMEX;

typedef struct _tagCSPROPERTY_StreamEx
{
    CSP_PIN CsPin;
    HANDLE  hMsgQueue;
} CSPROPERTY_STREAMEX_S, * PCSPROPERTY_STREAMEX_S;

typedef struct _tagMessageQueueHeader
{
    ULONG  Size;
    ULONG  Flags;
    LPVOID Context;
} CS_MSGQUEUE_HEADER, * PCS_MSGQUEUE_HEADER;

#define FLAG_MSGQ_FRAME_BUFFER    0x00000001

typedef struct _tagStreamDescriptor
{
    CSSTREAM_HEADER CsStreamHeader;
    CS_FRAME_INFO   CsFrameInfo;

} CS_STREAM_DESCRIPTOR, * PCS_STREAM_DESCRIPTOR;


typedef struct _tagMessageQueueData
{
    CS_MSGQUEUE_HEADER    CsMsgQueueHeader;
    PCS_STREAM_DESCRIPTOR pStreamDescriptor;
} CS_MSGQUEUE_BUFFER, * PCS_MSGQUEUE_BUFFER;

// CSSTREAM_ALLOCATOR is used by the client to specify, in case of driver being the allocator, the number of buffers the driver should create.
// When the client itself is the alloctor, this structure is used to pass down the number of buffers allocated by the client and the pointer to
// these buffers is stored in ppBuffers structure.
typedef struct
{
    ULONG    DesiredCount;
    ULONG    ActualCount;
    LPVOID   pBuffers;
} CSSTREAM_ALLOCATOR, * PCSSTREAM_ALLOCATOR;

typedef struct _Stream_Buffer STREAM_BUFFERNODE, *PSTREAM_BUFFERNODE;
struct _Stream_Buffer
{
    PCS_STREAM_DESCRIPTOR pCsStreamDesc;
    PSTREAM_BUFFERNODE    pNext;
    PVOID                 pMappedData;
    PVOID                 pUnmappedData;
};

//TODO : POOL_TYPE is also defined in public\common\ddk\inc\ndis.h. Later we need to unify the two.
typedef enum _POOL_TYPE 
{
    NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed,
    DontUseThisType,
    NonPagedPoolCacheAligned,
    PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS,
    MaxPoolType
} POOL_TYPE;

//
// Define alignment requirement values
//

#define FILE_BYTE_ALIGNMENT             0x00000000
#define FILE_WORD_ALIGNMENT             0x00000001
#define FILE_LONG_ALIGNMENT             0x00000003
#define FILE_QUAD_ALIGNMENT             0x00000007
#define FILE_OCTA_ALIGNMENT             0x0000000f
#define FILE_32_BYTE_ALIGNMENT          0x0000001f
#define FILE_64_BYTE_ALIGNMENT          0x0000003f
#define FILE_128_BYTE_ALIGNMENT         0x0000007f
#define FILE_256_BYTE_ALIGNMENT         0x000000ff
#define FILE_512_BYTE_ALIGNMENT         0x000001ff


#endif //__CAMERA_H__
