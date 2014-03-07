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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

CABAPI.h

Abstract:  

definitions for the Windows CE CABAPI APIs

Notes: 


--*/

#include <WinCrypt.h>
#include <wintrust.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef VOID*   HCAB;

enum CAB_ExtractEX_Flags
{
    CAB_DEFAULT_FLAGS =         0x0,//flags unset
    CAB_REUSE_DECOMPRESSOR =    0x1,//keep cab decompressor alive to uncab more files
    //
    //Add other flags here
    //
};

/// <summary>
/// - API to open a CAB file. Returns a HANDLE to the opened CAB file in hCab.
/// </summary>
/// <param name="szFileName"></param>
/// <param name="hCab"></param>
/// <returns></returns>
HRESULT CAB_Open( LPCTSTR szFileName, /*[OUT]*/ HCAB *hCab );

/// <summary>
/// - API to close a CAB file.
/// </summary>
/// <param name="hCab"></param>
/// <returns></returns>
HRESULT CAB_Close( HCAB hCab );

/// <summary>
/// - Validates the HCAB and then returns the name of the first file in the
/// specified buffer. By passing in NULL for the buffer, the size in bytes will
/// be returned in cbBuffer. If the buffer is not large enough, the function
/// returns SEC_E_BUFFER_TOO_SMALL and places the required number of bytes in
/// cbBuffer. If the function is successful, cbBuffer will contain the number
/// of bytes copied into the buffer.
/// </summary>
/// <param name="hcab"></param>
/// <param name="szBuffer"></param>
/// <param name="cbBuffer"></param>
/// <returns></returns>
HRESULT CAB_FindFirstFile( HCAB hCab, LPTSTR szBuffer, /* [IN] [OUT] */ DWORD *cbBuffer );

/// <summary>
/// - Validates the HCAB and then returns the name of the next file in the
/// specified buffer. By passing in NULL for the buffer, the size in bytes
/// will be returned in cbBuffer. If the buffer is not large enough, the 
/// function returns SEC_E_BUFFER_TOO_SMALL and places the required number 
/// of bytes in cbBuffer. If the function is successful, cbBuffer will contain
/// the number of bytes copied into the buffer and the return result will be
/// NOERROR. If there are no more files in the CAB, the buffer will not be
/// modified, cbBuffer will be set to zero and S_FALSE will be returned.
/// </summary>
/// <param name="hcab"></param>
/// <param name="szBuffer"></param>
/// <param name="cbBuffer"></param>
/// <returns></returns>
HRESULT CAB_FindNextFile( HCAB hCab, /* [OUT] */ LPTSTR szBuffer, /* [IN] [OUT] */ DWORD* cbBuffer );

/// <summary>
/// - API to extract a specific file from the CAB file and store it to the
/// specified path. hCab is verified to be a valid HCAB. If the file is
/// successfully extracted, NOERROR is returned. If the file is not found
/// in the CAB E_FAIL is returned.
/// </summary>
/// <param name="hcab"></param>
/// <param name="szFileName"></param>
/// <param name="szPath"></param>
/// <returns></returns>
HRESULT CAB_Extract( HCAB hCab, LPCTSTR szFileName, LPCTSTR szPath );

/// <summary>
/// Extended version of CAB_Extract function
/// - flags are passed to customize the operation
/// </summary>
HRESULT CAB_ExtractEx( HCAB hCab, LPCTSTR szFileName, LPCTSTR szPath, DWORD customFlags );

/// <summary>
/// - API to extract a specific file from the CAB file and store it to the
/// specified path. hCab is verified to be a valid HCAB. If the file is
/// successfully extracted, NOERROR is returned. If the file is not found
/// in the CAB E_FAIL is returned.
/// </summary>
/// <param name="hcab"></param>
/// <param name="szFileName"></param>
/// <param name="szPath"></param>
/// <returns></returns>
HRESULT CAB_ExtractAndDeleteAll( HCAB hCab, LPCTSTR szPath, size_t cchPath );

/// <summary>
///- Helper function to extract all files in the specified CAB. First
/// validates that hCab is valid and then extracts all files to the specified
/// path. If szPath does not exist, it is created. If the files are
/// successfully extracted NOERROR is returned, failure to extract a file will
/// result in E_FAIL being returned.
/// </summary>
/// <param name="hcab"></param>
/// <param name="szPath"></param>
/// <returns></returns>
HRESULT CAB_ExtractAll(HCAB hCab, LPCTSTR szPath );

/// <summary>
/// Retrieve the certificate data attached to the CAB. Memory will be allocated
/// by this API. CAB_FreeCertificateData should be called to release this memory.
/// </summary>
/// <param name="pFileBlob"></param>
/// <param name="dwCertificateIndex"></param>
/// <param name="pCertificate"></param>
HRESULT CAB_GetCertificateData( /* [IN] */ HCAB hCab, /* [IN] */ PCRYPT_DATA_BLOB pFileBlob, /* [IN] */ DWORD dwCertificateIndex, /* [OUT] */ LPWIN_CERTIFICATE * pCertificate );


/// <summary>
/// Free the memory allocated to hold the certificate data.
/// </summary>
/// <param name="hCab"></param>
/// <param name="pCertificate"></param>
HRESULT CAB_FreeCertificateData( /* [IN] */ HCAB hCab, /* [OUT] */ LPWIN_CERTIFICATE * pCertificate );

/// <summary >
///
/// </summary>
/// <param name="szFileName"></param>
/// <param name="lpCERTData"></param>
/// <param name="cbCERTData"></param>
HRESULT CAB_VerifyFileSignature( /* [IN] */ LPCTSTR szFileName, /* [IN] */ void const * lpCERTData,  /* [IN] */ size_t cbCERTData );

#ifdef __cplusplus
}
#endif
