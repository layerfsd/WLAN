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

// Generic CE Compressor/Decompressor DLL exports

// typedefs for memory alloc/free functions to be provided by the caller
typedef LPVOID ( *PFNALLOC ) ( DWORD dwBytes );
typedef VOID ( *PFNFREE ) ( LPVOID pMem );

// compressor/decompressor context pseudo-handles
#define HCOMPRESS    DWORD
#define HDECOMPRESS  DWORD

#define INVALID_HCOMPRESS    0
#define INVALID_HDECOMPRESS  0

// encode/decode error return values
#define COMPRESS_FAILED	    0xFFFFFFFF
#define DECOMPRESS_FAILED	0xFFFFFFFF

// prefixes for dll exports
#define LZX_COMPRESSOR_PREFIX	  L"LZX"
#define XPRESS_COMPRESSOR_PREFIX  L"XPR"

// typedefs for dll exports

typedef HCOMPRESS ( *PFNCOMPRESS_OPEN ) ( DWORD dwWindowSize, DWORD dwMaxBlockSize, PFNALLOC pfnAlloc, PFNFREE pfnFree, PVOID pReserved );
typedef DWORD ( *PFNCOMPRESS_ENCODE ) ( HCOMPRESS hContext, PVOID pCompData, DWORD dwCompBytes, PVOID pOrigData, DWORD dwOrigBytes );
typedef BOOL ( *PFNCOMPRESS_CLOSE ) ( HCOMPRESS hContext );

typedef HDECOMPRESS ( *PFNDECOMPRESS_OPEN ) ( DWORD dwWindowSize, DWORD dwMaxBlockSize, PFNALLOC pfnAlloc, PFNFREE pfnFree, PVOID pReserved );
typedef DWORD ( *PFNDECOMPRESS_DECODE ) ( HDECOMPRESS hContext, PVOID pOrigData, DWORD dwOrigBytes, PVOID pCompData, DWORD dwCompBytes );
typedef BOOL ( *PFNDECOMPRESS_CLOSE ) ( HDECOMPRESS hContext );

#ifdef __cplusplus
extern "C" {
#endif

// @CESYSGEN IF CE_MODULES_COMPRESS

// @CESYSGEN IF CECOMPRESS_COMPONENTS_LZX

// 
// Support for LZX compressor/decompressor
//

HCOMPRESS
LZX_CompressOpen ( 
    IN DWORD dwWindowSize,      // compression window size
    IN DWORD dwMaxBlockSize,    // max size of original data block
    IN PFNALLOC pfnAlloc,       // memory allocation function
    IN PFNFREE pfnFree,         // memory deallocation function
    IN PVOID pReserved          // reserved for future use, set to NULL
    );
    
DWORD
LZX_CompressEncode (
    IN HCOMPRESS hContext,      // compressor context returned from XXX_CompressOpen
    OUT PVOID pCompData,        // output buffer to receive encoded data
    IN DWORD dwCompDataBytes,   // size of output buffer pCompData
    IN PVOID pOrigData,         // input buffer containing original source data to encode
    IN DWORD dwOrigDataBytes    // size of input buffer pOrigData
    );

BOOL 
LZX_CompressClose (
    IN HCOMPRESS hContext       // compressor context returned from XXX_CompressOpen
    );

HDECOMPRESS
LZX_DecompressOpen ( 
    IN DWORD dwWindowSize,      // compression window size
    IN DWORD dwMaxBlockSize,    // max size of original data block
    IN PFNALLOC pfnAlloc,       // memory allocation function
    IN PFNFREE pfnFree,         // memory deallocation function
    IN PVOID pReserved          // reserved for future use, set to NULL
    );

DWORD
LZX_DecompressDecode (
    IN HDECOMPRESS hContext,    // decompressor context returned from XXX_DecompressOpen
    OUT PVOID pOrigData,        // output buffer to receive original, decoded data
    IN DWORD dwOrigDataBytes,   // size of output buffer pOrigData
    IN PVOID pCompData,         // input buffer containing the encoded source data to decode
    IN DWORD dwCompDataBytes    // size of input buffer pCompData
    );

BOOL
LZX_DecompressClose (
    IN HDECOMPRESS hContext     // decompressor context returned from XXX_DecompressOpen
    );

// @CESYSGEN ENDIF 

// @CESYSGEN IF CECOMPRESS_COMPONENTS_XPRESS

// 
// Support for XPress compressor/decompressor
//

HCOMPRESS
XPR_CompressOpen ( 
    IN DWORD dwWindowSize,      // compression window size
    IN DWORD dwMaxBlockSize,    // max size of original data block
    IN PFNALLOC pfnAlloc,       // memory allocation function
    IN PFNFREE pfnFree,         // memory deallocation function
    IN PVOID pReserved          // reserved for future use, set to NULL
    );
    
DWORD
XPR_CompressEncode (
    IN HCOMPRESS hContext,      // compressor context returned from XXX_CompressOpen
    OUT PVOID pCompData,        // output buffer to receive encoded data
    IN DWORD dwCompDataBytes,   // size of output buffer pCompData
    IN PVOID pOrigData,         // input buffer containing original source data to encode
    IN DWORD dwOrigDataBytes    // size of input buffer pOrigData
    );

BOOL 
XPR_CompressClose (
    IN HCOMPRESS hContext       // compressor context returned from XXX_CompressOpen
    );

HDECOMPRESS
XPR_DecompressOpen ( 
    IN DWORD dwWindowSize,      // compression window size
    IN DWORD dwMaxBlockSize,    // max size of original data block
    IN PFNALLOC pfnAlloc,       // memory allocation function
    IN PFNFREE pfnFree,         // memory deallocation function
    IN PVOID pReserved          // reserved for future use, set to NULL
    );

DWORD
XPR_DecompressDecode (
    IN HDECOMPRESS hContext,    // decompressor context returned from XXX_DecompressOpen
    OUT PVOID pOrigData,        // output buffer to receive original, decoded data
    IN DWORD dwOrigDataBytes,   // size of output buffer pOrigData
    IN PVOID pCompData,         // input buffer containing the encoded source data to decode
    IN DWORD dwCompDataBytes    // size of input buffer pCompData
    );

BOOL
XPR_DecompressClose (
    IN HDECOMPRESS hContext     // decompressor context returned from XXX_DecompressOpen
    );

// @CESYSGEN ENDIF

// @CESYSGEN ENDIF

#ifdef __cplusplus
}; // extern "C"
#endif
