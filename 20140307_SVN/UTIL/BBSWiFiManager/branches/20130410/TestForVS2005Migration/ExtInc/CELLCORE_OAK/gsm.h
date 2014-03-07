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
//////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999-2000 Microsoft Corporation.  All rights reserved. //
//////////////////////////////////////////////////////////////////////////


#ifndef _GSM_H_
#define _GSM_H_


// GSM SMS header information
// See GSM 03.40 9.2.3.24 for message-header details
#define INDEX_IEI                     (0)
#define INDEX_IEIDL                   (1)
#define INDEX_IEDATA                  (2)

// Concatenated short messages (8-bit reference number)
#define ID_CSM8                       (0)
#define ID_CSM8_NUMBER_OCTETS         (3)
// Reference number for this message
#define INDEX_CSM8_REFERENCE_NUMBER   (2)
// Number of fragments in this message
#define INDEX_CSM8_TOTAL_FRAGMENTS    (3)
// Sequence number of the current fragment
#define INDEX_CSM8_FRAGMENT_NUMBER    (4)

// Concatenated short messages (16-bit reference number)
#define ID_CSM16                      (8)
#define ID_CSM16_NUMBER_OCTETS        (4)
// Reference number for this message
#define INDEX_CSM16_REFERENCE_NUMBER  (2)
// Number of fragments in this message
#define INDEX_CSM16_TOTAL_FRAGMENTS   (4)
// Sequence number of the current fragment
#define INDEX_CSM16_FRAGMENT_NUMBER   (5)

// Application port addressing scheme (8-bit address)
#define ID_APAS8                      (4)
#define ID_APAS8_NUMBER_OCTETS        (2)
// Receiving port (application on the receiving device)
#define INDEX_APAS8_DESTINATION_PORT  (2)
// Sending port (application on the sending device)
#define INDEX_APAS8_ORIGINATOR_PORT   (3)
// Minimum and maximum allowed port values
#define MINIMUM_ALLOWED_PORT_ASPAS8   (240)
#define MAXIMUM_ALLOWED_PORT_ASPAS8   (255)

// Application port addressing scheme (16-bit address)
#define ID_APAS16                     (5)
#define ID_APAS16_NUMBER_OCTETS       (4)
// Receiving port (application on the receiving device)
#define INDEX_APAS16_DESTINATION_PORT (2)
// Sending port (application on the sending device)
#define INDEX_APAS16_ORIGINATOR_PORT  (4)
// Minimum and maximum allowed port values
#define MINIMUM_ALLOWED_PORT_ASPAS16  (0)
#define MAXIMUM_ALLOWED_PORT_ASPAS16  (16999)

// Wireless Control Message Protocol
#define ID_WCMP                       (9)
#define ID_WCMP_NUMBER_OCTETS         (2)
// Type of control message
#define INDEX_WCMP_TYPE               (2)
// Code of control message
#define INDEX_WCMP_CODE               (3)
// Octets of the control message
#define INDEX_WCMP_OCTETS             (4)

// Special SMS message indication
#define ID_SSMI                       (1)
#define ID_SSMI_NUMBER_OCTETS         (2)
// Receiving port (application on the receiving device)
#define INDEX_SSMI_INDICATION_TYPE    (2)
// Sending port (application on the sending device)
#define INDEX_SSMI_MESSAGE_TYPE       (3)
// Minimum and maximum allowed port values
#define MINIMUM_ALLOWED_PORT_ASPAS8   (240)
#define MAXIMUM_ALLOWED_PORT_ASPAS8   (255)
// Indication type
#define SSMI_TYPE_VOICEMAIL           (0)
#define SSMI_TYPE_FAX                 (1)
#define SSMI_TYPE_EMAIL               (2)
#define SSMI_TYPE_OTHER               (3)

#define ID_EMS_MIN                       (0x0A) // see 3GPP TS 23.040 Sec. 9.2.3.24
#define ID_EMS_MAX                       (0x1F)


bool CharacterPresentInGSM(const WCHAR wch);
bool CharacterPresentInGSMExt(const WCHAR wch);
bool ConvertUnicodeToGSM(const LPCWSTR pswInput, const UINT uiInputLength, const LPSTR psOutput, const UINT uiOutputLength, UINT& ruiCharactersUsed, BOOL fCRPad);
// With GSM extended character set support, the number of input characters may not easily be used to figure the number of output septets, so we return them both
bool ConvertUnicodeToGSMExt(const LPCWSTR pswInput, const UINT uiInputLength, const LPSTR psOutput, const UINT uiOutputLength, UINT& rnOutputBytesUsed, UINT& uiOutputSeptetsUsed, BOOL fCRPad, BOOL fSupportGSMExt);
bool ConvertGSMToUnicode(const LPCSTR psInput, const UINT uiInputLength, const LPWSTR pswOutput, const UINT uiOutputLength, UINT& ruiCharactersUsed);
bool ConvertUnicodeToUnpackedGSM(const LPCWSTR pswInput, const UINT uiInputLength, const LPSTR psOutput, const UINT uiOutputLength, UINT& ruiCharactersUsed);
bool ConvertUnpackedGSMToUnicode(const LPCSTR psInput, const UINT uiInputLength, const LPWSTR pswOutput, const UINT uiOutputLength, UINT& ruiCharactersUsed);
bool ConvertUnicodeToUCS2(const LPCWSTR pswInput, const UINT uiInputLength, const LPSTR psOutput, const UINT uiOutputLength, UINT& ruiCharactersUsed);
bool ConvertUCS2ToUnicode(const LPCSTR psInput, const UINT uiInputLength, const LPWSTR pswOutput, const UINT uiOutputLength, UINT& ruiCharactersUsed);
bool ConvertGSMMixedToUnicode(const UINT uiBasePage, const LPCSTR psInput, const UINT uiInputLength, const LPWSTR pswOutput, const UINT uiOutputLength, UINT& ruiCharactersUsed);
bool ConvertSemiOctetToWCHAR(const BYTE* pbSemiOctetAddress, const BYTE bNumberAddressSemiOctets, WCHAR* pwsAddressBuffer, size_t stAddressBufferSize, int* const piBytesUsed);
bool ConvertWCHARToSemiOctet(const WCHAR* pwsAddressBuffer, size_t stAddressBufferSize, BYTE* pbSemiOctetAddress, size_t stSemiOctetAddressSize, int* const piOctetsUsed);
bool ExtractConcatenationInfoFromSMSHeader(const BYTE* const pbHeader, const DWORD dwHeaderSize, DWORD& rdwMessageReference, DWORD& rdwTotalParts, DWORD& rdwPartIndex);
bool ExtractPortInfoFromSMSHeader(const BYTE* const pbHeader, const DWORD dwHeaderSize, DWORD& rdwSourcePort, DWORD& rdwDestinationPort, BYTE& bPortAddressingBits);
bool ExtractWCMPOctetsFromSMSHeader(const BYTE* const pbHeader, const DWORD dwHeaderSize, BYTE& rbMessageType, BYTE& rbMessageCode, const BYTE* &rpbOctets, UINT& ruiOctetCount);
bool ExtractMsgIndicationFromSMSHeader(const BYTE* const pbHeader, const DWORD dwHeaderSize, bool& rfStoreMsg, BYTE& rbIndicationType, BYTE& rbMsgCount);
bool FindEMSInSMSHeader(const BYTE* const pbHeader, const DWORD dwHeaderSize);
bool AddressesMatch(const TCHAR* const ptsNumberA, const TCHAR* const ptsNumberB);


#endif // _GSM_H_
