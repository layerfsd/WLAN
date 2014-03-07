//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//***   hack.h
//
// This module bears the name that was to be temporarily used while
// miscellaneous macros and functions were added. This header has been
// used by modules outside of TPCUTIL and changing the name now risks
// build breaks. So for now the name remains.
//

#ifndef _HACK_H_
#define _HACK_H_ 1

#include "fontman.h"

//***   Dconst -- const data
// macro so we can turn off const to aid bloat analysis via .map
// use this *only* for allocation of static vars (not for params or locals).
#define Dconst      const
#if 0 // for private builds
#undef Dconst
#define Dconst /*nothing*/
#endif

//***   Contacts related.
//
#define IS_CONTACTCHOOSER_UI_SUPPRESSED(lpcc) \
    ((lpcc)->dwFlags & CCF_NOUIONSINGLEORNOMATCH || \
     (lpcc)->dwFlags & CCF_NOUI)

// Flags indication why navigation is disabled in ContPol and ContUI
#define NAV_DISABLED 0x00000001
#define NAV_DISABLED_FILTERING 0x00000002


//***   misc stuff
//
#define IICON_NIL       (-1)        // ImageList_AddIcon error value

//***   IIDOF, CLSIDOF -- to migrate away from __uuidof
//
#define IIDOF(Ifoo)     IID_##Ifoo
#define CLSIDOF(foo)    CLSID_##foo

//***   PFN_WndCallback, IWndCallback --
//
typedef HRESULT (*PFN_WndCallback)(UINT uMsg, WPARAM wP, LPARAM lP, void *pvData);

class IWndCallback :
    public IUnknown
{
public:
    //*** IUnknown methods ***
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    //*** THISCLASS methods ***
    STDMETHOD(NotifyWCB)(THIS_ UINT uMsg, WPARAM wP, LPARAM lP, void *pvData) PURE;
};

int ShowPopupMenuData(HMENU hmenu, HWND hwndOwner, POINT *ppt);
TCHAR *Proto_GetAddrField(CEOID oidRec, CEPROPID hhpr);
void Proto_Free(TCHAR *psz);

class AutoCriticalSection
{
    CRITICAL_SECTION* m_pcs;
public:
    AutoCriticalSection(CRITICAL_SECTION* pcs) : m_pcs(pcs) {EnterCriticalSection(m_pcs);}
    ~AutoCriticalSection() {LeaveCriticalSection(m_pcs);}
};

#ifdef _CSHOBJ_H_ //  for IContextMenuMulti
HRESULT DbaseUiMergeMenu(HANDLE hDb, CEOID oid, const CLSID **pUiTab,
    HWND hwnd, HMENU hmenu, UINT idCmd1, UINT idCmdN,
    IContextMenuMulti **ppcmm);
int DbaseUiOnXM_INITMENUPOPUP(HANDLE hDb, CEOID oid, const CLSID **pUiTab,
    HWND hwnd, HMENU hmenu, UINT idMerge1, UINT idMergeN,
    IContextMenuMulti **ppcmm);
int DbaseUiOnWM_COMMAND(IContextMenuMulti *pcmm, UINT idCmd, HWND hwndOwner);
int DbaseUiOnExit(IContextMenuMulti **ppcmm);
UINT DbaseUiOnGN_CONTEXTMENU(HANDLE hDb, CEOID oid, const CLSID **pUiTab,
    HWND hwnd, HWND hwndFrom, HMENU hmenu, UINT idrMenu, POINT *ppt);
#endif

const TCHAR *GetStartMenuPath();

//***   IS_B* -- bit operations
//
#define IS_BVAL(dwFlags, msk, val)  (((dwFlags) & (msk)) == (val))

//***   ROUNDPOW2 -- round up to i^p2
//
#define ROUNDPOW2(i, p2)    (((i) + ((p2) - 1)) & ~((p2) - 1))

#define GetPropValSafe(cepv, fld) \
    (((cepv).wFlags & CEDB_PROPNOTFOUND) ? NULL : (cepv).val.fld)
#define GetPropLvalSafe(cepv, fld) \
    (((cepv).wFlags & CEDB_PROPNOTFOUND) ? NULL : &(cepv).val.fld)

// CESeekDatabase is v. fast if you pass NULL as the last param.
// so think twice before you change any CeSeekDatabase call to use non-NULL.
#define NULL_PERF       0

CEOID Dbase_ReadRecordEtc(HANDLE hDbase, CEOID oid,
    WORD *pcProp, const CEPROPID *phhpr, CEPROPVAL **ppBuf);
CEOID Dbase_SeekEtc(HANDLE hDbase, DWORD dwSeekType, DWORD dwValue, DWORD *pdwIndex);
BOOL  Dbase_DeleteRecordEtc(HANDLE hDbase, CEOID oid);

typedef int (*PFN_ENUMCALLBACK)(void *hElt, const void *pElt, void *pData);
void Dbase_EnumCallback(HANDLE hDbase, WORD *pcProp, const CEPROPID *pids,
    PFN_ENUMCALLBACK pfnCB, void *pvData);

void Dbase_ChSize(HANDLE hDbase, int cRec);
#ifdef __PIMUTIL_H__ //  for APPTYPE
CEOID Dbase_ReadRecordEtcAT(APPTYPE atDbase, CEOID oid,
    WORD *pcProp, const CEPROPID *phhpr, CEPROPVAL **ppBuf);
CEOID Dbase_WriteRecordEtcAT(APPTYPE atDbase, CEOID oid,
    WORD cProp, CEPROPVAL *pVals);
CEOID Dbase_SeekEtcAT(APPTYPE atDbase, DWORD dwSeekType, DWORD dwValue, DWORD *pdwIndex);
void Dbase_ChSizeAT(APPTYPE atDbase, int cRec);
CEOID Dbase_FindEtcAT(APPTYPE at, DWORD dwSeekType, DWORD dwValue,
    DWORD *pdwIndex);
HRESULT Dbase_GetCvtTextAT(APPTYPE at, CEOID oid, CEPROPID hhpr,
    TCHAR *pszBuf, int cchBuf);
BOOL  Dbase_DeleteRecordEtcAT(APPTYPE at, CEOID oid);
#endif //__PIMUTIL_H__
HRESULT Dbase_GetCvtText(HANDLE hDbase, CEOID oid, CEPROPID hhpr,
    TCHAR *pszBuf, int cchBuf);
HRESULT Dbase_CvtText(const CEPROPVAL *ppv, TCHAR *pszBuf, int cchBuf);
HRESULT Ftime_CvtText(DWORD dwFlags, const FILETIME *pftIn, TCHAR *pszOut, int cchOut);

// n.b. encoding v. likely to change (see below)
#define DBFMT_XTIME     0              // hh:mm
#define DBFMT_XDELTA    ((DWORD)-1)    // mm:ss
#define DBFMT_XDATE     1              // mm/dd/yy
#define DBFMT_XDATETIMEINTELLI  2      // ...

#if 0 // NYI (and not thought out yet...)
// we need some encoding of
//  abbr    short, long, intelli
//  type    date, time, delta
//  fields  mm/dd/yy hh:mm:ss pm 
#define SHFMT_ADELTA    0x0000      // (should delta go here vs. in type?)
#define SHFMT_ASHORT    0x0000      // ...
#define SHFMT_ALONG     0x0000      // ...
#define SHFMT_AINTELLI  0x0000      // e.g. 2:00 or Tues or 3/23 or 1999

#define SHFMT_TDATE     0x0000      // e.g. mm/dd/yy
#define SHFMT_TTIME     0x0000      // e.g. hh:mm pm
#define SHFMT_TDELTA    0x0000      // e.g. [hh:]mm:ss

#define SHFMT_FMONTH    0x0000      // ...
#define SHFMT_FDAY      0x0000      // ...
#define SHFMT_FYEAR     0x0000      // ...
#define SHFMT_FHOUR     0x0000      // ...
#define SHFMT_FMIN      0x0000      // ...
#define SHFMT_FSEC      0x0000      // ...
#define SHFMT_FAMPM     0x0000      // ...
#endif

HRESULT FakeProp(CEPROPID hhpr, void *pvVal, CEPROPVAL **ppVal);
int DPA_FreeLocalCB(void *pvElt, void *pvData);
HRESULT Dbase_ReAlloc(BYTE **ppVals, UINT ucbBytes, UINT uFlags);

int GetStandardDateTime(TCHAR *pszOut, int cchOut, const SYSTEMTIME* pst, BOOL fTime, BOOL fLongFormat);

// GetTimeDeltaFormat flags
// If the delta has no hours then use a format string that doesn't show hours
#define GTDF_HIDE_NOHOURS       1
// If the delta format has more than single digit hours, then return a shortened 
// string showing just hours and minutes. This should be fairly rare. 
#define GTDF_HIDE_SECSIFLONG    2

int GetTimeDeltaFormat(TCHAR* pszOut, int cchOut, const FILETIME* pft, DWORD grfFlags);
void Ftime_Diff(FILETIME *pdTime, const FILETIME *pft2, const FILETIME *pft1);
int GetDurationString(const ULONGLONG *pllStart, const ULONGLONG *pllEnd, TCHAR *pszBuffer, UINT cchBuffer);

void PreLoadString(const TCHAR **pIdsTab, int cElt);
DWORD TPCFormatMessage(TCHAR* pszOut, UINT cchOut, HINSTANCE hinst, UINT idsFormat, ...);
DWORD TPCVFormatMessage(TCHAR* pszOut, UINT cchOut, HINSTANCE hinst, UINT idsFormat, va_list* pv_list);
int TB_SetButtonText(HWND hwndMB, UINT idmCmd, UINT idsText);
void FmtLayout(int cxTot, const int *fmtTab, int *layTab, int cTab);

#define UNMAKEINTRESOURCE(i) ((WORD)((DWORD)((TCHAR)(i))))

inline int DWORDAlign(int x) { return((x + 3) & ~3); }

// This is extracted from toolbox.h
#define ROUND_UP_TO_NEXT_MULTIPLE(x, Multiple) ((x)+(((Multiple)-((x)%(Multiple)))%(Multiple)))

BOOL EndMenu(void);

LRESULT Dbg_OnQuit(HWND hwnd);

#ifdef DEBUG
void DumpMenu(HMENU hmenu);


// This is not a production quality function but is useful
// for some debug things.
#ifndef lstrncmp
#define lstrncmp(a, b, cch) memcmp((a), (b), (cch) * sizeof(TCHAR))
#endif //lstrncmp

// This is not a production quality function but is useful
// for some debug things.
HRESULT GetNextString(const TCHAR** ppszLine, const TCHAR** ppszLineEnd, const TCHAR* pszDelimiter);
#endif //DEBUG

HRESULT SetListViewSelected(HWND hwndVLV, UINT iElement);
HRESULT SetListViewFocused(HWND hwndLV, UINT iElement);

HRESULT MakeDlgTemplate(const TCHAR *pszTitle, DWORD dwExtraStyles, DLGTEMPLATE **ppdt);

HRESULT DrawTextWithFont(HDC hdc, eFontID idFont,  const TCHAR* pszText, int cchText, RECT* prcDraw, UINT uFlags, int* pdyHeight);
HRESULT DrawTextWithFittingFont(HDC hdc, eFontID idFontBig,  eFontID idFontSmall, const TCHAR* pszText, int cchText, RECT* prcDraw, UINT uFlags, int* pdyHeight);
HRESULT IsChildOrSelf(HWND hwndParent, HWND hwnd);

BOOL DSA_IsItemPtr(HDSA hdsa, void* pItem);
BOOL DSA_GetItemIndex(HDSA hdsa, void* pItem, int* piItem);

HRESULT TPC_InitializeCommonControls();

//used for apps to register with home that they are done booting
#define EVENT_HOMEHWNDREADY      TEXT("HomeWindow_Ready")
#define HM_REGISTER_APP_BOOT_MSG (WM_USER+8088)
void SignalHomeAppStart(HWND hwndDoneApp);

//used for home to notify registered app that shell is ready
#define HM_REGISTER_APP_SHELL_READY (WM_USER+8089)
    
void ForwardMessageToAllChildren(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT SetAutoProfileTime();

BOOL DestroyWindowSafe(HWND* phwnd);

#ifdef __cplusplus
extern "C" {
#endif

// These functions are exported, so wrap with extern C

LONG GetKeypadBeepMode(DWORD *pdwBeepMode);

#ifdef __cplusplus
}
#endif

BOOL HelperCheckIsWndClass(HWND hwnd, const TCHAR* szName);
BOOL FTToLST(const FILETIME *pftIn, SYSTEMTIME *pstOut);
BOOL ShouldShowCumulativeCallTimers();
BOOL ShouldShowSimEntries();
BOOL IsSimAccessible();
BOOL FeatureLevelSpeechReco(); // this should be converted to using SHDFL stuff
void CleanAutoPrefixOverrideChars();
BOOL IsAutoPrefixOverrideChar(TCHAR tch);
void GetAutoPrefixOverrides();
BOOL IsAutoPrefixEnabled();


#define CALLERID_OVERRIDE_INVALID ((const TCHAR*)-1)
const TCHAR *GetCallerIDOverrideString();
void ResetCallerIDOverrideString();

BOOL IsRadioOn();

// macros used to handle SIMMgr API errors
#define MS_IN_SEC           (1000)
#define SIMRETRY_PAUSE_SEC  (2)
#define SIMRETRY_MAX        (4)

/*
    Why is this a macro and not a function?
    Macro is meant to be used with various functions part of the SIMMgr API. The functions
    have different signarture (i.e. take differernt numbers of parameters). Turning this
    into a function would have required solutions that were far more complicated and harder to
    maintain.
    Why do we retry infinitely for SIM_E_NOTREADY & SIM_E_SIMBUSY?
    These 2 are transient errors and can be hit at any time. We cannot assume that this error
    will only be raised while the SIM is initialzed on booting the device.
    Why do we retry for a set number of times on SIM_E_UNDETERMINED?
    USIM phonebook access cellcore code does not preserve error information it receives from the
    RIL. SIM_E_UNDETERMINED is returned in the case of unexpected errors from the UsimPhonebook_t 
    implementation in cellcore. We want to retry on these errors.
*/
#define SIMAPI_RETRY_ON_TRANSIENT_ERROR(hResult, dwMaxRetry, dwPauseSec) \
    do \
    { \
        DWORD dwRetryCount = 0; \
        DWORD dwSleep = MS_IN_SEC; \
        do \
        { \
            hr = (hResult); \
            if(SIM_E_NOTREADY == hr || SIM_E_SIMBUSY == hr) \
            { \
                DEBUGMSG(TRUE, (TEXT("SIMAPI_RETRY_ON_TRANSIENT_ERROR: SIM isn't ready, will try again\r\n"))); \
                Sleep(MS_IN_SEC * dwPauseSec); \
                continue; \
            } \
            else if((SIM_E_UNDETERMINED == hr) && dwMaxRetry >= dwRetryCount) \
            { \
                dwRetryCount++; \
                dwSleep *= dwPauseSec; \
                Sleep(dwSleep); \
                continue; \
            } \
            break;\
        } \
        while (TRUE); \
    } \
    while (0,0)

// Send a message to a menubar.  Needed because PPC doesn't support overriding the back key
LRESULT SendMessageMB(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#ifdef __PIMUTIL_H__ //  for APPTYPE
HRESULT GetRecordByOidAT(APPTYPE at, CEOID oid, const CEPROPID* rgids, UINT cids, CEPROPVAL** ppValsOut);
#endif //__PIMUTIL_H__

#endif // _HACK_H_


