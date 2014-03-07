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
/*****************************************************************************

    File: phnumutil.h

    Helper functions for dealing with phone numbers

    History:
        Created - KK 1/4/01
*/
#ifndef _PHNUMUTIL_H_
#define _PHNUMUTIL_H_

// Our Phone Table and Character to Character Table support mapping any
// Unicode character from 0x0000 to 0x052F which is the end of the Extended Cyrillic set.
#define MAXCHARMAPPABLE    0x052F

// Registry data for LoadKeyMap and LoadLangMap
#define REG_szRefreshKeyMap         TEXT("RefreshKeyMap")
#define REG_szTempKeyMap            TEXT("SECURITY\\Phone\\PhoneKeyMatch\\Temp\\KeyboardMappings")

#define REG_szKeyLayoutCode         TEXT("KeyLayoutCode")
#define REG_szRefreshLangMap        TEXT("RefreshLangMap")
#define REG_szTempLangMap           TEXT("SECURITY\\Phone\\PhoneKeyMatch\\Temp\\LanguageMappings")

// there's some skepticism about whether MATCH_SMART will fly
// MATCH_* will let us turn it off easily if need be
// 001020: freshness dating, nuke the #ifdef's if/when MATCH_SMART is proven
#define MATCH_DUMB      0   // full-#
#define MATCH_SMART     1   // #, w/ and w/o country/area-code
#define MATCH_SUPER     2   // NYI, and probably never

// Smart Filter Matching flags
#define SMF_ONSCREENKEYPAD   1   // Enable OSK (On Screen Keypad) mappings.
                                 // 0 means disable until an OSK button is pressed then remain on until that filtering session ends.
                                 // 1 means enable always
#define SMF_NOOSKMAPPINGS    2   // Disable OSK mappings even if an OSK button is pressed.  This setting overrides SMF_ONSCREENKEYPAD.
#define SMF_NOLANGMAPPINGS   4   // Disable language specific alternate mappings

enum OSK_MAP
{
    OSK_MAP_UNINITIALIZED,        // We need to initialize the setting
    OSK_MAP_ALWAYSON,             // OSK mappings should always be on even if an OSK button has not been pressed
    OSK_MAP_ALWAYSOFF,            // OSK mappings should always be off even if an OSK button is pressed
    OSK_MAP_ONDEMAND,             // OSK mappings should be off until an OSK button is pressed and then it should turn off again when the filtering session ends.
};

// Should match CCH_INCREMENTAL in \private\apps\tele\tpcutil\dbfilt.h
// This is the max length of our filter string; clients should not expect the filter
// to be applied beyond CCHMAXFILTER characters.
#define CCHMAXFILTER (250)

// Max extra formatting POOM adds to the fileas field
#define PHNUM_MAX_FILEAS_FORMAT     6

// Should match PIM_CCH_FILEAS_FIELD in \private\apps\outlook\inc\pimutil.h
// The max database string that we will match on; clients should not expect
// any matches beyond PHNUM_MAX_MATCH_BUFFER - 1 characters.
#define PHNUM_MAX_MATCH_BUFFER      ((255 * 4) + PHNUM_MAX_FILEAS_FORMAT + 1)

// The max phone number string buffer we ever allow in the database.
#define PHNUM_MAX_PHONENUM_BUFFER   256

#define FEATURE_MATCH   MATCH_SMART

#if FEATURE_MATCH == MATCH_DUMB
#define IsPNP       IsPhoneNumberPrefix
#endif
#if FEATURE_MATCH == MATCH_SMART
#define IsPNP       IsPhoneNumberPrefixEx
#endif

// flags for DrawStringsWithReason
#define DNWRF_USEELLIPSES      (0x00000001)

BOOL GetLangData(LCID lcid, __out_opt PWORD pcMappingsNeeded, __out_opt PWORD pcchMaxMapping);
HRESULT ValidateLangData(__in const TCHAR *pszMappings, WORD cMappingsNeeded, WORD cchMaxMapping);
OSK_MAP GetOSKMapBehavior();
BOOL OSKMappingsOn();
void SetupPhoneNumberFiltering(DWORD dwSMFMatchType, DWORD dwMask);
void IncrementSettingsVersion();
UINT GetSettingsVersion();
UINT GetSmartFilterSettingsVersion();
int CanonPhoneNumberWithIntlPrefixW(__out_ecount(cchCanon) WCHAR *pszCanon, __in const WCHAR *pszRaw, int cchCanon);
TCHAR ConvertCharToPhoneChar(TCHAR chInput);
void ConvertStringToPhoneChars(__inout TCHAR* pszStr);
TCHAR IntlLowercase(TCHAR chEntry);
TCHAR IntlUppercase(TCHAR chEntry);
BOOL MakeTelLinkFromNumber(__in const TCHAR *pszNumber, __out_ecount(cchOut) TCHAR *pszOut, size_t cchOut);
int CanonPhoneNumberW(__out_ecount(cchCanon) WCHAR *pszCanon, __in const WCHAR *pszRaw, int cchCanon);
int GetCchTailMin();

#define MPF_AMBIGUOUS 0x01    // Client doesn't know what the matchable text is
#define MPF_NAME      0x02    // The matchable text is a name
#define MPF_NUMBER    0x04    // The matchable text is a phone number
#define MPF_TEXT      0x08    // The matchable text is neither a name nor a phone number

BOOL IsSimplePrefix(__in const WCHAR * pszPrefix, __in const WCHAR * pszCheck);

int IsPhoneNumberPrefix(__in const WCHAR* pszPrefix, __in const WCHAR* pszPhoneNumber, __out __opt int *cStartCanonPos);
int IsPhoneNumberPrefixEx(__in const WCHAR* pszPrefix, __in const WCHAR* pszPhoneNumber, __out __opt int *cStartCanonPos);
BOOL IsMatchingPrefix(__in_ecount(CCHMAXFILTER) LPCWSTR pszFilter, __in_opt LPCWSTR pszText, DWORD dwFlags, __out_bcount_opt(PHNUM_MAX_MATCH_BUFFER) BYTE *prgTextMatch);
HRESULT DrawNameWithReason(const TCHAR* pszFilter, HDC hdc, const RECT* prc, const TCHAR* pszName, HFONT hfNormal, HFONT hfReason);
HRESULT DrawNameWithReasonAndReasonColor(const TCHAR* pszFilter, HDC hdc, const RECT* prc, const TCHAR* pszName, HFONT hfNormal, HFONT hfReason, COLORREF crReasonText, COLORREF crReasonBk);
HRESULT DrawNameWithReasonEx(const TCHAR* pszFilter, HDC hdc, const RECT* prc, const TCHAR* pszName, const TCHAR* pszSuffix, HFONT hfNormal, HFONT hfReason, DWORD dwDrawFlags, DWORD dwTextFlags);
HRESULT ExtractReasonStringFromName(__in_ecount(cchFilter) const TCHAR *pszFilter,
                                    UINT cchFilter,
                                    __in_ecount(cchNameLen) const TCHAR *pszName,
                                    UINT cchNameLen,
                                    __in_ecount(cchNameLen) const BYTE*rgNameReason,
                                    __out_ecount(PHNUM_MAX_MATCH_BUFFER) TCHAR* pszNameReason
                                    );
void ConvertToReasonLength(__inout_bcount(cbReason) BYTE *prgReason, int cbReason);

HRESULT Phone_FmtText(const TCHAR *pszInput, TCHAR *pszOut, int cchOut);

HRESULT GetTapiCurrentAreaCode(TCHAR* pszAreaCodeOut, int cchOut);
HRESULT SetTapiCurrentAreaCode(const TCHAR* pszAreaCodeIn);
HRESULT GetTapiCurrentCountryCode(TCHAR* pszCountryCodeOut, int cchOut);
HRESULT SetTapiCurrentCountryCode(const TCHAR* pszCountryCodeIn);


//routines for determining valid phone number characters

//flags for ValidPhoneNumberXXX routines, currently only validation for SIM phone #'s is performed and for CDMA
#define VPNF_GSM_SIM    (0x00000001)
#define VPNF_CDMA       (0x00000002)
#define VPNF_NOMAP      (0x80000000)

#define ISVALIDVPNF(VPNF)     ((VPNF) == VPNF_CDMA || (VPNF) == VPNF_GSM_SIM)

BOOL ValidPhoneNumberIsValidCharacter(DWORD dwFlags, TCHAR ch);
HRESULT ValidPhoneNumberGetValidCharacterList(DWORD dwFlags, TCHAR* pszCharListOut, UINT cchCharListOut);
HRESULT ValidPhoneNumberInplaceStripInvalidCharacters(DWORD dwFlags, TCHAR* pszPhoneNum);

HRESULT GetValidSimPhoneNumberAndAddressType(__in const TCHAR* pszPhoneNum,
                                             __out_ecount(cchValidPhoneNumOut) TCHAR* pszValidPhoneNumOut,
                                             UINT cchValidPhoneNumOut,
                                             __out BOOL* pfInternational);

BOOL IsValidGsmText(__in LPCTSTR pszText);

// Functions for emergency and OTA(over the air provisioning) number handling
const DWORD IEPNF_TAIL_MATCH                = 0x00000002; // Allow a tail match
BOOL IsEmergencyPhoneNumber(DWORD dwFlags, const TCHAR* pszNumber, const TCHAR** ppszEmergencyNumber);
BOOL IsOTAPhoneNumber(DWORD dwFlags, const TCHAR* pszNumber, const TCHAR** ppszOTANumber);

BOOL IsWildcardPhoneNumber(const TCHAR* pszAddress);

void PhNumUtilShutDown();

//determines whether a phone number is suspend-resume (cdma dormant mode) connection
BOOL IsSuspendResumeConnection(const TCHAR* pszAddress, BOOL fIsGPRS);

extern "C" HRESULT FitTextInRect(DWORD dwFlags, HDC hdc, const RECT *prc, LPTSTR pszText, int cchBuffer);

#endif // _PHNUMUTIL_H_
