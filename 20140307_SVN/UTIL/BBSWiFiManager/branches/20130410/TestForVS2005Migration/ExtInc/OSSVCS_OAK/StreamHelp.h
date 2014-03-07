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
/***********************************************************
    StreamHelp.h

    Helper for streaming class/structure members.
    

*/
#pragma once

const DWORD SHF_LOAD        = 0x00000001; // Copy information from the stream to the locations in the table
const DWORD SHF_SAVE        = 0x00000002; // Copy information from the locations in the table to the stream
const DWORD SHF_GETSIZE     = 0x00000004; // Get the size of the element in the stream.


typedef HRESULT (*PFN_STREAMER)(
    DWORD dwFlags, // combination of SHF_* flags.  SHF_LOAD and SHF_SAVE are obviously mutually exclusive
    IStream* pStm, // stream to use in your streaming operation
    void* pElement, // pointer to the element to stream in/out
    LPARAM lParam, // extra info about the element, element type dependent
    UINT* pcb // in/out - increment this by the count of bytes in the element.  Only used with SHF_GETSIZE.
    );

HRESULT STHStreamBinary(DWORD dwFlags, IStream* pStm, void* pElement, LPARAM lParam, UINT* pcb);
HRESULT STHStreamBufferAlloc(DWORD dwFlags, IStream* pStm, void* pElement, LPARAM lParam, UINT* pcb);
HRESULT STHStreamStringAlloc(DWORD dwFlags, IStream* pStm, void* pElement, LPARAM lParam, UINT* pcb);

struct SEElement
    {
    PFN_STREAMER pfn; // Pointer to function that streams this element type.
    void* pElement; // pointer to element to be streamed.
    LPARAM lParam; // lParam passed to streaming function
    };

#define SSEL_BINARY(x) {STHStreamBinary, &(x), sizeof(x)},
#define SSEL_ALLOC_STRING(x) {STHStreamStringAlloc, &(x), 0},
#define SSEL_CUSTOM(pfn, pElement, lParam) {(pfn), (pElement), (lParam)},
#define SSEL_END {NULL, NULL, NULL}

HRESULT STHStreamElements(DWORD dwFlags, IStream* pStm, void* pElement, LPARAM lParam, UINT* pcb);

