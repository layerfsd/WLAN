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
// Copyright (c) Microsoft Corporation.  All rights reserved. 
//
// Use of this source code is subject to the terms of your Windows Mobile 
// Software Shared Source Premium Derivatives License Form or other 
// applicable Microsoft license agreement for the software. If you did not 
// accept the terms of such a license, you are not authorized to use this 
// source code.
#pragma once

#include "callpol.h"

#define CB_MAX_BLOB (65536)
#define MAX_PHONESTATE_GROUPS 100
#define MAX_PHONESTATE_CALLS 200

/********************************************************************
    Struct that contains PH_VERB parameters
    
 ********************************************************************/
struct CProgVerbStruct
{
    UINT cbSize;
    PH_VERB cpVerb;

    // If fValidateOnly is set, the verb handler will only check to
    // see if it thinks it can do the verb.
    // Returns:
    //  S_OK - this verb will likely succeed (eg. PH_VERB_ACCEPT_INCOMING when there is an incoming call)
    //  E_FAIL - this verb will likely fail (eg. PH_VERB_REJECT_INCOMING when there are no incoming calls)
    //  E_NOTIMPL - this verb will likely never succeed (eg. PH_VERB_FLASH on a GSM phone)
    BOOL fValidateOnly;

    union
    {
        PHADDACCUMULATOR phAddAccum;        // for PH_VERB_ADD_ACCUMULATOR
        BOOL fClearAll;                     // for PH_VERB_REMOVE_ACCUMULATOR
        PHSWITCHVIEW phSwitchView;          // for PH_VERB_SWITCHVIEW
        HPHONECALL callHandle;              // for PH_VERB_PRIVATE
        PH_VIEW idView;                     // for PH_VERB_MINIMIZE 
        int iCplTab;                        // for PH_VERB_LAUNCHOPTIONS
        LPCTSTR pszPhoneNumber;             // for PH_VERB_SENDSMS
        PH_UPDATEINACTIVITYTIMER uitTimerUpdate; // for PH_VERB_UPDATEINACTIVITYTIMER
        PH_GROUPSTATE phEndGroup;           // for PH_VERB_END
        int iSpeedDial;
        CEOID oidContact;                   // for PH_VERB_VIEWCONTACT
        CPCallInfo* pcpci;
        BOOL fHideRejectedCall;             // for PH_VERB_REJECT_INCOMING
        PHDIALSTRING phDialString;          // for PH_VERB_DIAL_STRING
    };
};

#define PMCF_VALID (PMCF_DEFAULT | PMCF_PROMPTBEFORECALLING | PMCF_NOPROGRESSINFOONCALL | PMCF_ALLOWSUPSVCS | PMCF_EDITBEFORECALLING)
CASSERT(PMCF_DEFAULT == SHMCF_DEFAULT);
CASSERT(PMCF_NOPROGRESSINFOONCALL == SHMCF_NOPROGRESSINFOONCALL);
CASSERT(PMCF_PROMPTBEFORECALLING == SHMCF_PROMPTBEFORECALLING);
CASSERT(PMCF_ALLOWSUPSVCS == SHMCF_ALLOWSUPSVCS);
CASSERT(PMCF_EDITBEFORECALLING == SHMCF_EDITBEFORECALLING);

// {27FE7A6F-4EB5-4fee-8E0D-9F903969867B}
DEFINE_GUID(CLSID_CProg, 
    0x27fe7a6f, 0x4eb5, 0x4fee, 0x8e, 0xd, 0x9f, 0x90, 0x39, 0x69, 0x86, 0x7b);

// {5C4FEA9C-16DF-4f65-B170-D3E5588D839F}
DEFINE_GUID(IID_ICProg, 
    0x5c4fea9c, 0x16df, 0x4f65, 0xb1, 0x70, 0xd3, 0xe5, 0x58, 0x8d, 0x83, 0x9f);


/********************************************************************
    Interface to the phone application engine
    
 ********************************************************************/
class ICProg :
    public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE DoVerb(const CProgVerbStruct *pVerbStruct) = 0;

    virtual HRESULT STDMETHODCALLTYPE AddListener(const PH_ID* pData, int cData, HWND hwndNotify, UINT uMsg, HPHONELISTENER* pidListener) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveListener(HPHONELISTENER hListener) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetChangedIDs(HPHONELISTENER hListener, int* pcids, PH_ID** ppids) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetInt(PH_ID phid, int* pi) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDWORD(PH_ID phid, DWORD* pdw) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetStringPtr(PH_ID phid, const TCHAR** ppsz) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetStringToBuffer(PH_ID phid, TCHAR* psz, int* pcch) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBLOBPtr(PH_ID phid, DWORD* pcb, const BYTE** ppb) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetAccumulator(const TCHAR* pszAccumulator) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSmartDialName(const TCHAR* pszSmartDialName) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSmartDialNumber(const TCHAR* pszSmartDialNumber) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSmartDialShown(BOOL fSmartDialShown) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetExtPhoneState(PHPHONESTATE* ppsExt, DWORD cbSize, PH_GROUPINDEX phgiGroup1, PH_GROUPINDEX phgiGroup2) = 0;
    virtual HRESULT STDMETHODCALLTYPE UpdateCallLogInfo(HPHONECALL hCall, PH_CALLLOGFLAGS phclFlags, PH_CALLLOGFLAGS phclMask, PH_CALLBEHAVIORFLAGS phcbFlags, PH_CALLBEHAVIORFLAGS phcbMask) = 0;
    virtual HRESULT STDMETHODCALLTYPE ShowPhoneMsg(PH_MSG phmsg, LPCWSTR pszText, LPCWSTR pszTitle) = 0;
};

typedef HRESULT(*PFNPHEXTINITIALIZE)
(
    IPOutlookApp2 *polApp
);

typedef HRESULT (*PFNPHEXTUNINITIALIZE)
(
    void
);

typedef HRESULT(*PFNPHEXTDOVERB)
(
    PH_VERB     verb, 
    BOOL        fValidateOnly,
    VPARAM      vp
);


typedef HRESULT (*PFNPHEXTSHOWPHONEMSG)
(
    PH_MSG      phmsg, 
    LPCWSTR     psz1, 
    LPCWSTR     psz2,   
    BOOL        fRemove, 
    DWORD       dwSupSvcStatus, 
    BOOL*       pfDial
); 


typedef HRESULT (*PFNPHEXTGETPHONEVIEWINFO)
(
    PH_VIEW     view,
    HINSTANCE*  phInstance,
    PH_VIEWINFOFLAGS* pvif,
    LPPHONEVIEWHOOKPROC* plpfnHook,
    LPCTSTR*    plpTemplateName
); 

typedef HRESULT (*PFNPHEXTGETPHONECONTROLMETRICS)
(
    PH_VIEW     view,
    PH_VIEWINFOFLAGS vif,
    DWORD       idControl,
    LPCTSTR     pszValue,
    LPDWORD     lpData
); 

typedef HRESULT (*PFNPHEXTONPHONESTATECHANGE)
(
    const PHPHONESTATE* c_pps
);

HRESULT InitializePHCanvas(HWND hwndOwner, ICProg* pCprog, IPOutlookApp2 *polApp);
void FreePHCanvas();

HRESULT OEMPHExtInitialize(IPOutlookApp2 *polApp);
HRESULT OEMPHExtDoVerb(PH_VERB verb, VPARAM vp);
HRESULT OEMPHExtShowPhoneMsg(PH_MSG phmsg, LPCWSTR psz1, LPCWSTR psz2, BOOL fRemove, DWORD dwSupSvcStatus, BOOL* pfDial);
HRESULT OEMPHExtGetPhoneViewInfo(
                                PH_VIEW     view,
                                HINSTANCE*  phInstance,
                                PH_VIEWINFOFLAGS* pvif,
                                LPPHONEVIEWHOOKPROC* plpfnHook,
                                LPCTSTR*    plpTemplateName);

HRESULT OEMPHExtGetPhoneControlMetrics(
                                PH_VIEW     view,
                                PH_VIEWINFOFLAGS vif,
                                DWORD       idControl,
                                LPCTSTR     pszValue,
                                LPDWORD     lpData);

HRESULT OEMPHExtOnPhoneStateChange(const PHPHONESTATE* c_pps);

//Additions for the VoIP-Specific Canvas implementation
typedef PFNPHEXTINITIALIZE              PFNPHVOIPINITIALIZE;
typedef PFNPHEXTUNINITIALIZE            PFNPHVOIPUNINITIALIZE;
typedef PFNPHEXTDOVERB                  PFNPHVOIPDOVERB;
typedef PFNPHEXTONPHONESTATECHANGE      PFNPHVOIPONPHONESTATECHANGE;

typedef HRESULT (*PFNPHVOIPSETEXTPHONESTATE)
(
    PHPHONESTATE* ppsExt, 
    DWORD cbSize, 
    PH_GROUPINDEX phgiGroup1, 
    PH_GROUPINDEX phgiGroup2    
);

typedef HRESULT (*PFNPHVOIPGETEXTPHONESTATE)
(
    DWORD*          pcb,
    const BYTE**    ppb
);

HRESULT PHSetMergedExtPhoneState(
    PHPHONESTATE * ppsExt, 
    DWORD cbSize, 
    PH_GROUPINDEX phgiGroup1, 
    PH_GROUPINDEX phgiGroup2
    );

typedef struct tagPHONE_MSG_INFO
{
    ICProg* pcp;
    PH_MSG phmsg;      // IN: The msg
    LPCWSTR psz1;         // IN: phone number for dial request
    LPCWSTR psz2;         // IN: caller id name for Phone Book entry
    BOOL fRemove;         // IN: When TRUE, the Pocket PC notification for this message will be removed.
    DWORD dwSupSvcStatus; // IN: sup svc or sim error code from supsvcs.h or simmgr.h.  
    BOOL fDial;           // OUT: Whether it's OK to dial (only valid for PH_MSG_CONFIRMDIALREQUEST and PH_MSG_SIMPHONEBOOKENTRY)
} PHONE_MSG_INFO, *PPHONE_MSG_INFO;

/********************************************************************
    PHSetSmartDialName : Replaces the contents of SmartDialerName with the
                         specified string. This is here in phcanvas_priv.h
                         instead of phcanvas.h because we do not want to expose
                         this to OEMs
 ********************************************************************/
HRESULT PHSetSmartDialName(__in __opt const TCHAR* pszSmartDialName);
/********************************************************************
    PHSetSmartDialNumber : Replaces the contents of SmartDialerNumber with the
                           specified string. This is here in phcanvas_priv.h
                           instead of phcanvas.h because we do not want to expose
                           this to OEMs
 ********************************************************************/
HRESULT PHSetSmartDialNumber(__in __opt const TCHAR* pszSmartDialNumber);
/********************************************************************
    PHSetSmartDialShown :  Sets PH_ID_SMARTDIAL_SHOWN so that OEMs and
                           we may be able to know when we should listen
                           to changes to PH_ID_SMARTDIAL_NUMBER for verb
                           execution.
 ********************************************************************/
HRESULT PHSetSmartDialShown(BOOL fSmartDialShown);
/********************************************************************
    PHSetSmartDialTopItem : Sets PH_ID_SMARTDIAL_TOPITEM to the top index
                            of the smartdial page. This is here in phcanvas_priv.h
                            instead of phcanvas.h because we do not want to expose
                            this to OEMs.
 ********************************************************************/
HRESULT PHSetSmartDialTopItem(int iLVTopItem);
/********************************************************************
    PHSetSmartDialPageSize : Sets PH_ID_SMARTDIAL_PAGESIZE to the number of rows
                             of the smartdial page. This is here in phcanvas_priv.h
                             instead of phcanvas.h because we do not want to expose
                             this to OEMs.
 ********************************************************************/
HRESULT PHSetSmartDialPageSize(int iLVTopItem);
/********************************************************************
    PHSetSmartDialSeleted : Sets PH_ID_SMARTDIAL_SELECTED to the index
                            of the smartdial listview. This is here in phcanvas_priv.h
                            instead of phcanvas.h because we do not want to expose
                            this to OEMs.
 ********************************************************************/
HRESULT PHSetSmartDialSeleted(int iSelected);


#define OCN_CELLULAR    0x00000000
#define OCN_VOIP        0x00000001

HRESULT PHOverrideCurrentNetwork(
    DWORD dwNetwork
    );

