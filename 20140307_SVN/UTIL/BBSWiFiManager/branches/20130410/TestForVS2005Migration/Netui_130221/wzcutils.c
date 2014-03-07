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
//==========================================================================*
//
// Filename:    wzcutils.c
//
// Owner:        
//
// Purpose:     Utils functions for Wireless Zero Config
//
//==========================================================================*

#include <windows.h>
#include <windowsx.h>
#include <ehm.h>
#include <miscsvcs_priv.h>

#define INITGUID
#include <initguid.h>
#include <cshguid.h>

#include "eapol.h"
#include "wzcsapi.h"
#include "netui.h"
#include <netuip.h>
#include <resource.h>
#include "Wpcpriv.h"
#include "wzcuip.h"
//$ PATCH against the OS's lss.h
#pragma warning( push )
#pragma warning(disable: 4100)
#include "ndis.h"
#pragma warning( pop )
#include "nuiouser.h"
#include "connmgr.h"
#include "connmgr_plan.h"
#include "connmgr_priv.h"
#include "msgqueue.h"
#include "wzcmsq.h"
#include "cmnutils.h"


///<file_topic_scope tref="wifi"/>

HANDLE g_hNdisUio;

#define WAZUI_QUERYINTERFACE_TIMEOUT 500


#define DELAYLD_NO_GETPROCFROMDLL
#include <delayld.h>
static HINSTANCE g_hInstAygshell = NULL;
DELAY_LOAD_ORD_ERR(g_hInstAygshell, aygshell, LPTSTR, GetHTMLInputValue, 416,
    (LPCTSTR pszLink, LPCTSTR pszInputName),
    (pszLink, pszInputName), NULL);

// Function prototypes
BOOL GetNICStatistics(LPTSTR pszDeviceName, NIC_STATISTICS* pns);


// added this function to help in detecting valid hex digits (0-9, a-f, A-F)
BOOL IsHexDigit(TCHAR c)
{
    // avoiding tolower() in order to avoid potential loc issues
    if (isdigit(c) ||
        (c >= TEXT('A') && c <=TEXT( 'F')) ||
        (c >= TEXT('a') && c <= TEXT('f')))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL IsAnyWPA(NDIS_802_11_AUTHENTICATION_MODE nam)
{
    return nam == Ndis802_11AuthModeWPA ||  
                nam == Ndis802_11AuthModeWPAPSK || 
                nam == Ndis802_11AuthModeWPANone ||
                nam == Ndis802_11AuthModeWPA2 ||
                nam == Ndis802_11AuthModeWPA2PSK;
}

BOOL IsAnyWPAPSK(NDIS_802_11_AUTHENTICATION_MODE nam)
{
    return nam == Ndis802_11AuthModeWPAPSK ||
                nam == Ndis802_11AuthModeWPA2PSK;
}

BOOL IsAnyWPA2(NDIS_802_11_AUTHENTICATION_MODE nam)
{
    return nam == Ndis802_11AuthModeWPA2 ||
                nam == Ndis802_11AuthModeWPA2PSK;
}

BOOL IsWPAAESCapable(LPBYTE pParam)
{
    DWORD i;
    PINTF_80211_CAPABILITY pCap = (PINTF_80211_CAPABILITY)pParam;
    
    if (! pCap)
    {
        return FALSE;
    }

    for (i = 0 ; i < pCap->dwNumOfAuthEncryptPairs ; i++) 
    {
        NDIS_802_11_AUTHENTICATION_MODE Auth    = pCap->AuthEncryptPair[i].AuthModeSupported;
        NDIS_802_11_ENCRYPTION_STATUS   Encrypt = pCap->AuthEncryptPair[i].EncryptStatusSupported;
            
        if ((Auth == Ndis802_11AuthModeWPA || Auth == Ndis802_11AuthModeWPAPSK) &&
            (Encrypt == Ndis802_11Encryption3Enabled)) {            
            return TRUE;
        }            
    }
    
    return FALSE;
}

HRESULT WZCQueryWirelessInfo(PINTF_ENTRY_EX pIntfEntry, DWORD dwInOIDFlags, PDWORD pdwOutOIDFlags)
{
    HRESULT hr = S_OK;
    UINT    uTimeout = 0;
    DWORD   dwError;

    // When the driver has recently connected to a new network or is starting a
    // re-scan for SSIDs, it enters the {SSr} (Soft State reset) state. We may
    // need to wait up to 5 seconds for it exit this state. While it's in the
    // {SSr} state, we won't get a valid query of OID fields.
    while (uTimeout < TMMS_DEFAULT_TD)
    {
        // Free the interface data if necessary
        if (pIntfEntry->bInitialized)
        {
            WZCDeleteIntfObjEx(pIntfEntry);
        }

        // Make the query
        dwError = WZCQueryInterfaceEx(NULL, dwInOIDFlags, pIntfEntry, pdwOutOIDFlags);
        if (ERROR_SUCCESS != dwError)
        {
            DEBUGMSG(ZONE_ERROR, (TEXT("WZC: WZCQueryInterfaceEx failed (error %u)\r\n"), dwError));
            CBR(FALSE);
        }

        // The {SSr} state only affects OID fields
        if ((dwInOIDFlags & INTF_ALL_OIDS) && !(*pdwOutOIDFlags & INTF_ALL_OIDS))
        {
            Sleep(WAZUI_QUERYINTERFACE_TIMEOUT);
            uTimeout += WAZUI_QUERYINTERFACE_TIMEOUT;
        }
        else
        {
            break;
        }
    }

    if (pIntfEntry->nAuthMode < 0)
        pIntfEntry->nAuthMode = 0;
    if (pIntfEntry->nInfraMode < 0)
        pIntfEntry->nInfraMode = 0;
 
Error:
    return hr;
}


BOOL WZCMatchConfigWithSsid(PWZC_WLAN_CONFIG pConfig, LPCSTR pucSSID)
{
    BOOL fRet = FALSE;
    
    if (pucSSID && pConfig)
    {
       fRet = strlen(pucSSID) == pConfig->Ssid.SsidLength
            && !strncmp(pucSSID, (LPCSTR)pConfig->Ssid.Ssid, pConfig->Ssid.SsidLength);
    }

    return fRet;
}

//==========================================================================*
// Function: WZCCopyConfigs
//
// Purpose:  Copy the WZC config list from pCfgSrcs to pCfgDests
//           pCfgSrcs area and pCfgDests area can be [overlapped]. 
//           We use "memmove" in this function.
//           If cuConfigs == 0, do nothing
//           
// Arguments: 
//   [OUT]   pCfgDests - the destination configuraton list
//    [IN]   pCfgSrcs  - the source configuration list
//    [IN]   cnConfigs - the count of configurations in source list
//
// Return Values:
//      S_OK
//      E_OUTOFMEMORY (only in the deep copy case)
//==========================================================================*
HRESULT WZCCopyConfigs(PWZC_WLAN_CONFIG pCfgDests, const WZC_WLAN_CONFIG *pCfgSrcs, UINT cuConfigs, BOOL fDeepCopy)
{
    HRESULT hr = S_OK;

    if (cuConfigs > 0)
    {
        ASSERT(pCfgDests != NULL);
        ASSERT(pCfgSrcs  != NULL);
    
        memmove(pCfgDests, pCfgSrcs, sizeof(WZC_WLAN_CONFIG) * cuConfigs);

        if (fDeepCopy)
        {
            UINT i;

            for (i = 0; i < cuConfigs; i++)
            {
                // Clear the data pointers
                pCfgDests[i].rdUserData.pData = NULL;
                pCfgDests[i].rdNetworkData.pData = NULL;
                pCfgDests[i].EapolParams.pbAuthData = NULL;

                // Copy the user data and the network data
                CHR(WZCDupRAW_DATA(&pCfgSrcs[i].rdUserData, &pCfgDests[i].rdUserData));
                CHR(WZCDupRAW_DATA(&pCfgSrcs[i].rdNetworkData, &pCfgDests[i].rdNetworkData));

                // Copy the EAP data
                pCfgDests[i].EapolParams.pbAuthData = (LPBYTE)LocalAlloc(LMEM_FIXED,
                    pCfgSrcs[i].EapolParams.dwAuthDataLen);
                CPR(pCfgDests[i].EapolParams.pbAuthData);

                memcpy(pCfgDests[i].EapolParams.pbAuthData,
                    pCfgSrcs[i].EapolParams.pbAuthData,
                    pCfgSrcs[i].EapolParams.dwAuthDataLen);
            }
        }
    }

Error:
    return hr;
}

//==========================================================================*
// Function: WZCGetConnectedConfig
//
// Purpose:  Determine if there is an active, connected config and copy the config to the out variable
//           
// Arguments: 
//    [IN]   pIntfEntry - the interface entry
//    [IN]   dwOIDFlags - this must have the INTF_SSID is on
//    [OUT] psConfig - the WZC configuration of the active config is copied here
//
// Return Values:
//      TRUE: There is an active, connected config and INTF_SSID bit is on.
//      FALSE: There is no active, connected config or there is INTF_SSID bit is not set.
//==========================================================================*
BOOL WZCGetConnectedConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, PWZC_WLAN_CONFIG psConfig)
{
    NIC_STATISTICS ns = {0};
    
    if (   !(dwOIDFlags & INTF_SSID)  // rdSSID is not valid
        || !pIntfEntry
        || !pIntfEntry->rdSSID.pData
        || !pIntfEntry->rdSSID.dwDataLen)
    {
        return FALSE;
    }

    // Get the statistics to see if we're actually connected or not
    if (!GetNICStatistics(pIntfEntry->wszGuid, &ns) || ns.MediaState != MEDIA_STATE_CONNECTED)
    {
        return FALSE;
    }
        
    if (psConfig)
    {
        DWORD cbSSID;
        // There is an active(connected) network
        psConfig->InfrastructureMode = (NDIS_802_11_NETWORK_INFRASTRUCTURE)pIntfEntry->nInfraMode;
        
        ASSERT(pIntfEntry->rdSSID.dwDataLen <= sizeof(psConfig->Ssid.Ssid));    // Is source data too long?        
        cbSSID = MIN(pIntfEntry->rdSSID.dwDataLen, sizeof(psConfig->Ssid.Ssid));
        psConfig->Ssid.SsidLength = cbSSID;
        CopyMemory(psConfig->Ssid.Ssid, pIntfEntry->rdSSID.pData, cbSSID);

        // ssid is 32 chars, length is up to 32, so we may overrun
        // there is no requirement for an ssid to be nul terminated
        // psConfig->Ssid.Ssid[pIntfEntry->rdSSID.dwDataLen] = 0;
    }

    return TRUE;
}

PWZC_WLAN_CONFIG WZCGetPreferredConfigs(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags)
{
    if (   !(dwOIDFlags & INTF_PREFLIST)  // rdStSSIDList is not valid
        || !pIntfEntry 
        || !pIntfEntry->rdStSSIDList.pData)
    {
        return NULL;
    }
        
    return ((PWZC_802_11_CONFIG_LIST)(pIntfEntry->rdStSSIDList.pData))->Config;
}

UINT WZCGetNumPreferredConfigs(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags)
{
    if (   !(dwOIDFlags & INTF_PREFLIST)  // rdStSSIDList is not valid   
        || !pIntfEntry 
        || !pIntfEntry->rdStSSIDList.pData)
    {
        return 0;
    }

    return ((PWZC_802_11_CONFIG_LIST)(pIntfEntry->rdStSSIDList.pData))->NumberOfItems;
}

BOOL WZCIsPreferredConfigsEmpty(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags)
{
    return WZCGetNumPreferredConfigs(pIntfEntry, dwOIDFlags) == 0;
}

HRESULT WZCInsertPreferredConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, INT nIndex, PWZC_WLAN_CONFIG pwzcConfig)
{
    HRESULT                 hr           = S_OK;
    PWZC_WLAN_CONFIG        pConfigsPre  = WZCGetPreferredConfigs(pIntfEntry, dwOIDFlags);
    INT                     cnConfigsPre = WZCGetNumPreferredConfigs(pIntfEntry, dwOIDFlags);
    UINT                    sizeBufferList;
    PWZC_802_11_CONFIG_LIST pConfigList;

    CBR(0 <= nIndex && nIndex <= cnConfigsPre);
    
    sizeBufferList = sizeof(WZC_802_11_CONFIG_LIST) + (cnConfigsPre) * sizeof(WZC_WLAN_CONFIG);
    
    // Check if there is enough space in the perferred network list
    if (sizeBufferList > pIntfEntry->rdStSSIDList.dwDataLen)
    {
        // Create a new preferred list buffer
        sizeBufferList += 10 * sizeof(WZC_WLAN_CONFIG);
        pConfigList = (PWZC_802_11_CONFIG_LIST)LocalAlloc(LPTR, sizeBufferList);
        CPR(pConfigList);

        // Init the new buffer
        WZCCopyConfigs(pConfigList->Config, pConfigsPre, cnConfigsPre, FALSE);
        
        // Update preferred list
        if (pIntfEntry->rdStSSIDList.pData)
        {
            LocalFree(pIntfEntry->rdStSSIDList.pData);
        }
        pIntfEntry->rdStSSIDList.pData     = (LPBYTE)pConfigList;
        pIntfEntry->rdStSSIDList.dwDataLen = sizeBufferList;
        pConfigsPre = WZCGetPreferredConfigs(pIntfEntry, INTF_PREFLIST);
    }
    else
    {
        pConfigList = (PWZC_802_11_CONFIG_LIST)pIntfEntry->rdStSSIDList.pData;
    }

    WZCCopyConfigs(pConfigsPre + nIndex + 1, pConfigsPre + nIndex, cnConfigsPre - nIndex, FALSE);
    WZCCopyConfigs(pConfigsPre + nIndex, pwzcConfig, 1, FALSE);
    pConfigList->NumberOfItems = cnConfigsPre + 1;

Error:
    return hr;
}

//==========================================================================*
// Function: FindTopPosInPreferredList
//
// Purpose:  Find the top(first) position in the preferred wireless network list.
//           For Infrastructure wireless network, the function alwasy return 0
//           For adhoc wirless network, the top position is at the end of the 
//           Infrastructure wireless network list because Infrastructure wireless 
//           networks are always more preferred (try to connect first) than ad-hoc 
//           wireless networks
//           
// Return Values:
//      INT - the index of the top position in the preferred list
//==========================================================================*
INT FindTopPosInPreferredList(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, PWZC_WLAN_CONFIG pwzcConfig)
{
    PWZC_WLAN_CONFIG pConfigsPre  = WZCGetPreferredConfigs(pIntfEntry, dwOIDFlags);
    INT              cnConfigsPre = WZCGetNumPreferredConfigs(pIntfEntry, dwOIDFlags), nIndex;
    BOOL             bAdHoc;

    bAdHoc = pwzcConfig->InfrastructureMode == Ndis802_11IBSS;

    for (nIndex = 0; bAdHoc && nIndex < cnConfigsPre; nIndex++)
    {
        if (pConfigsPre[nIndex].InfrastructureMode == Ndis802_11IBSS)
        {
            break;
        }
    }

    return nIndex;
}

void WZCFreeConfig(PWZC_WLAN_CONFIG pwzcConfig)
{
    if (pwzcConfig && pwzcConfig->rdUserData.dwDataLen)
    {
        LocalFree(pwzcConfig->rdUserData.pData);
        pwzcConfig->rdUserData.pData = NULL;
        pwzcConfig->rdUserData.dwDataLen = 0;
    }

    if (pwzcConfig && pwzcConfig->rdNetworkData.dwDataLen)
    {
        LocalFree(pwzcConfig->rdNetworkData.pData);
        pwzcConfig->rdNetworkData.pData = NULL;
        pwzcConfig->rdNetworkData.dwDataLen = 0;
    }

    if (pwzcConfig && pwzcConfig->EapolParams.dwAuthDataLen)
    {
        LocalFree(pwzcConfig->EapolParams.pbAuthData);
        pwzcConfig->EapolParams.pbAuthData = NULL;
        pwzcConfig->EapolParams.dwAuthDataLen = 0;
    }    
}


BOOL WZCIsConfigInList(const WZC_WLAN_CONFIG *pConfig,
                       const WZC_WLAN_CONFIG *pConfigList, INT sizeConfigList, BOOL fMatchSSIDOnly)
{
    return 0 <= WZCFindConfigInList(pConfig, pConfigList, sizeConfigList, fMatchSSIDOnly);
}

INT WZCFindConfigInList(const WZC_WLAN_CONFIG *pConfig,
                        const WZC_WLAN_CONFIG *pConfigList, INT sizeConfigList, BOOL fMatchSSIDOnly)
{
    INT  nIndex = -1;
    INT  i;

    for (i = 0; i < sizeConfigList; i++)
    {
        if (WZCMatchConfig(pConfigList + i, pConfig, fMatchSSIDOnly) == 0)
        {
            nIndex = i;
            break;
        }
    }

    return nIndex;
}

HRESULT ReAllocAndConcat(__inout LPTSTR *ppszDest, __in LPCTSTR pszAppend)
{
    HRESULT hr = S_OK;
    LPTSTR  pszNew;
    UINT    cchAlloc;
    UINT    cbAlloc;

    // Sanity check
    ASSERT(NULL != ppszDest);
    ASSERT(NULL != pszAppend);

    // Determine what to append
    CBRAEx(UAdd(_tcslen(pszAppend), 1, &cchAlloc), E_INVALIDARG);
    if (NULL == *ppszDest)
    {
        // Allocate a new buffer
        CBRAEx(CharsToBytes(cchAlloc, &cbAlloc), E_INVALIDARG);
        pszNew = (LPTSTR)LocalAlloc(LMEM_MOVEABLE, cbAlloc);
        CPR(pszNew);

        // Copy the string (this is safe)
        _tcscpy(pszNew, pszAppend);
    }
    else
    {
        UINT cchInput;

        // Reallocate the buffer
        cchInput = _tcslen(*ppszDest);
        CBRAEx(UAdd(cchInput, cchAlloc, &cchAlloc), E_INVALIDARG);
        CBRAEx(CharsToBytes(cchAlloc, &cbAlloc), E_INVALIDARG);
        pszNew = (LPTSTR)LocalReAlloc(*ppszDest, cbAlloc, LMEM_MOVEABLE);
        CPR(pszNew);

        // Append the string (this is safe)
        _tcscpy(pszNew + cchInput, pszAppend);
    }

    // Success
    *ppszDest = pszNew;

Error:
    return hr;
}


HRESULT WZCUpdatePreferredConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, PWZC_WLAN_CONFIG pwzcConfig, INT nNewPosition, BOOL fFreeConfig)
{
    PWZC_WLAN_CONFIG pConfigsPre  = WZCGetPreferredConfigs(pIntfEntry, dwOIDFlags);
    INT              cnConfigsPre = WZCGetNumPreferredConfigs(pIntfEntry, dwOIDFlags), i;

    for (i = 0; i < cnConfigsPre; i++)
    {
        if (WZCMatchConfig(pConfigsPre + i, pwzcConfig, FALSE) == 0)
        {
            if (nNewPosition != i)
            {
                WZCRemovePreferredConfig(pIntfEntry, dwOIDFlags, i, fFreeConfig);
                WZCInsertPreferredConfig(pIntfEntry, dwOIDFlags, 
                    nNewPosition > i ? nNewPosition - 1 : nNewPosition, 
                    pwzcConfig);
            }
            else
            {
                if (fFreeConfig)
                {
                    WZCFreeConfig(pConfigsPre + i);
                }
                WZCCopyConfigs(pConfigsPre + i, pwzcConfig, 1, FALSE);
            }
            break;
        }
    }

    return i < cnConfigsPre ? S_OK : E_FAIL;
}

HRESULT WZCRemovePreferredConfig(PINTF_ENTRY_EX pIntfEntry, DWORD dwOIDFlags, INT nIndex, BOOL fFreeConfig)
{
    HRESULT                 hr = S_OK;
    PWZC_WLAN_CONFIG        pConfigsPre  = WZCGetPreferredConfigs(pIntfEntry, dwOIDFlags);
    INT                     cnConfigsPre = WZCGetNumPreferredConfigs(pIntfEntry, dwOIDFlags);
    PWZC_802_11_CONFIG_LIST pConfigList = (PWZC_802_11_CONFIG_LIST)pIntfEntry->rdStSSIDList.pData;

    CBR(0 <= nIndex && nIndex < cnConfigsPre);
    
    // Remove the special item from preferred list 
    if (fFreeConfig)
    {
        WZCFreeConfig(pConfigsPre + nIndex);
    }
    
    WZCCopyConfigs(pConfigsPre + nIndex,
                   pConfigsPre + nIndex + 1, 
                   cnConfigsPre - nIndex - 1, FALSE);
    
    pConfigList->NumberOfItems--;

Error:
    return hr;
}


HRESULT WZCDupRAW_DATA(const RAW_DATA *pRawData, PRAW_DATA pDupRawData)
{
    HRESULT hr = S_OK;

    CBREx(pDupRawData != NULL && pRawData != NULL, E_INVALIDARG);

    if (pDupRawData->pData)
    {
        LocalFree(pDupRawData->pData);
    }

    if (pRawData->dwDataLen)
    {
        pDupRawData->pData = (LPBYTE)LocalAlloc(LMEM_FIXED, pRawData->dwDataLen);
        CPR(pDupRawData->pData);

        memcpy(pDupRawData->pData, pRawData->pData, pRawData->dwDataLen);
        pDupRawData->dwDataLen = pRawData->dwDataLen;
    }
    else
    {
        pDupRawData->pData     = NULL;
        pDupRawData->dwDataLen = 0;
    }

Error:
    return hr;
}

void WZCPageInitCaption( HWND hDlg )
{
    TCHAR szCaption[128];
    GetWindowText( hDlg, szCaption, ARRAYSIZE(szCaption) );
    SetDlgItemText( hDlg, IDC_WZC_HEADING, szCaption );
}


HRESULT DDX_Text(HWND hDlg, INT nEditID,  LPTSTR pszString, INT nMaxLenBuf, BOOL fSetControl)
{
    HRESULT hr = S_OK;
    HWND    hwndEdit = GetDlgItem(hDlg, nEditID);
    
    CWR(hwndEdit);

    if (fSetControl)
    {
        SetWindowText(hwndEdit, pszString);
    }
    else
    {
        GetWindowText(hwndEdit, pszString, nMaxLenBuf);
    }

Error:
    return SUCCEEDED(hr);
}


BOOL DDX_Check(HWND hDlg, INT nCheckID, BOOL*  pfCheck,   BOOL fSetControl)
{
    HRESULT hr        = S_OK;
    HWND    hwndCheck = GetDlgItem(hDlg, nCheckID);
    
    CBR(hwndCheck && pfCheck);

    if (fSetControl)
    {
        SendMessage(hwndCheck, BM_SETCHECK, *pfCheck, 0L);
    }
    else
    {
        *pfCheck = SendMessage(hwndCheck, BM_GETCHECK, 0, 0L);
    }

Error:
    return SUCCEEDED(hr);
}


BOOL DDX_Comb(HWND hDlg, INT nIDComb, INT* pnIndex, BOOL fSetControl)
{
    HRESULT hr = S_OK;

    CBR(hDlg && pnIndex);

    if (fSetControl)
    {
        SendDlgItemMessage(hDlg, nIDComb, CB_SETCURSEL, (WPARAM)*pnIndex, 0L);
    }
    else
    {
        *pnIndex = SendDlgItemMessage(hDlg, nIDComb, CB_GETCURSEL, 0, 0L);
    }

Error:    
    return SUCCEEDED(hr);
}

HRESULT WZCConnect(PINTF_ENTRY_EX pIntfEntry, PDWORD pdwOIDFlags, PWZC_WLAN_CONFIG psConfig)
{
    HRESULT          hr = S_OK;
    PWZC_WLAN_CONFIG pConfigsPre;
    INT              cnConfigsPre, i, nTopPos;

    CBR(pdwOIDFlags != NULL);

    pConfigsPre  = WZCGetPreferredConfigs   (pIntfEntry, *pdwOIDFlags);
    cnConfigsPre = WZCGetNumPreferredConfigs(pIntfEntry, *pdwOIDFlags);

    nTopPos      = FindTopPosInPreferredList(pIntfEntry, *pdwOIDFlags, psConfig);

    i = WZCFindConfigInList(psConfig, pConfigsPre, cnConfigsPre, FALSE);
    if (i >= 0)
    {
        // Move it to the head of the preferred list
        hr = WZCInsertPreferredConfig(pIntfEntry, *pdwOIDFlags, nTopPos, psConfig);
        CHR(hr);

        // Remove the duplicate config in the new position (i + 1)
        WZCRemovePreferredConfig(pIntfEntry, *pdwOIDFlags, i + 1, FALSE);
    }
    else
    {
        // Insert it to the head of the preferred list
        hr = WZCInsertPreferredConfig(pIntfEntry, *pdwOIDFlags, nTopPos, psConfig);
        CHR(hr);
    }

    // There should have a preferred list;
    *pdwOIDFlags |= INTF_PREFLIST;

    // Set the wireless info in the zero config driver
    if(WZCSetInterfaceEx(
                NULL,
                INTF_PREFLIST,
                pIntfEntry,
                NULL) != ERROR_SUCCESS)
    {
        hr = E_FAIL;
        goto Error;
    }

Error:    
    return hr;
}


INT MessageBoxWithResID(HWND hwnd, INT nIDText, INT nIDCaption, UINT uFlags)
{
    LPCTSTR pszTitle   = (LPCTSTR)LoadString(HINST_RESDLL, nIDText, NULL, 0);
    LPCTSTR pszCaption = (LPCTSTR)LoadString(HINST_RESDLL, nIDCaption, NULL, 0);

    ASSERT(pszTitle != NULL && pszCaption != NULL);
    
    return MessageBox(hwnd, pszTitle, pszCaption, uFlags | MB_SETFOREGROUND);
}


BOOL PropPageDialogHelper(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL fRet = FALSE;

    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    
    switch(message)
    {
    case WM_INITDIALOG:
        {
            WZCPageInitCaption(hDlg);
        }
        break;
    }

    return fRet;
}

HRESULT InitMetaNetworkCombo(HWND hwndCombo, UINT iSel)
{
    HRESULT hr = S_OK;
    UINT    i;

    for (i = 0; i < GetMetaNetworkCount(); i++)
    {
        LPCTSTR pszName = NULL;

        hr = GetMetaNetworkName(i, &pszName);
        CHR(hr);

        CBR(0 <= (int)ComboBox_InsertString(hwndCombo, i, pszName));
    }

Error:
    if (i > iSel)
    {
        // If we have enough items, we select the specified index
        ComboBox_SetCurSel(hwndCombo, iSel);
    }
    
    return hr;
}


BOOL GetNICStatistics(LPTSTR pszDeviceName, NIC_STATISTICS* pns)
{
    DWORD dwBytesWritten = 0;

    RETAILMSG(TRUE, (TEXT("NetUi: GetNICStatistics device name = %s"), pszDeviceName));
    
    pns->ptcDeviceName = pszDeviceName;

    return DeviceIoControl(
            g_hNdisUio,
            IOCTL_NDISUIO_NIC_STATISTICS,
            NULL,
            0,
            pns,
            sizeof(NIC_STATISTICS),
            &dwBytesWritten,
            NULL);
}

INT CheckWepKMaterial(PWZC_WLAN_CONFIG pwzcConfig, LPCTSTR pszWepKey)
{
    INT nRet = -1;
    INT i, nLenKey = _tcslen(pszWepKey);

    for (i = 0; i < nLenKey; i++)
    {
        if (pszWepKey[i] != TEXT('*'))
        {
            break;
        }
    }

    if (i >= nLenKey)
    {
        // Invalid Wep Key,  it is just several '*'
        nRet = -2;
    }
    else  if (((nLenKey == 5 || nLenKey == 13) && pwzcConfig->Privacy == Ndis802_11Encryption1Enabled) || // WEP: 5 or 13 ASCII
                    (nLenKey == 16 && pwzcConfig->Privacy == Ndis802_11Encryption3Enabled)) // AES - 16 ASCII
    {
        // WEP - binary entered
        // User can use any character he/she wants
        nRet = 0;
    }
    else if (((nLenKey == 10 || nLenKey == 26) && pwzcConfig->Privacy == Ndis802_11Encryption1Enabled) || // WEP: 10 or 26 hex
                 ((nLenKey == 64) && pwzcConfig->Privacy == Ndis802_11Encryption2Enabled) ||  // TKIP: 64 hex digits
                 ((nLenKey == 32) && pwzcConfig->Privacy == Ndis802_11Encryption3Enabled)) // AES: 32 hex digits
    {
        // The cases for when a hex value is allowed
        for (i = 0; i < nLenKey; i++) 
        {
            // Modified this to be more stringent in its search for hexadecimal characters
            if (!IsHexDigit(pszWepKey[i]))
            {
                nRet = -3;
                break;
            }
        }
        if (i == nLenKey)
        {
            nRet = 0;
        }
    }
    else if ((nLenKey >= 8 && nLenKey <= 63) && pwzcConfig->Privacy == Ndis802_11Encryption2Enabled)
    {
        // If it's TKIP we can allow any number of characters between 8 and 63 inclusive
        nRet = 0;
    }

    return nRet;
}


//+---------------------------------------------------------------------------
// parses & copies the WEP Key material from the parameter into the pWCP->wzcConfig object
// The length should be already the good one, the format is given by pWCP->wzcConfig.dwCtlFlags
// Since we assume a valid key material it means its length is non-zero and it is fitting in
// the configurations key material buffer, and if the formatting is hexadecimal, it 
// contains an even number of hexa digits.
void CopyWepKMaterial(PWZC_WLAN_CONFIG pwzcConfig,LPSTR szKMat)
{
    BYTE     chFakeKeyMaterial[] = {0x56, 0x09, 0x08, 0x98, 0x4D, 0x08, 0x11, 0x66, 0x42, 0x03, 0x01, 0x67, 0x66};
    INT      i, nLenKMat;
   
    nLenKMat = strlen(szKMat);

    // If the privacy type is TKIP or AES and the keylength >=8 and <=63, then we can convert it and be done with it
    if (((pwzcConfig->Privacy == Ndis802_11Encryption2Enabled) ||
        (pwzcConfig->Privacy == Ndis802_11Encryption3Enabled)) &&
        (nLenKMat >= 8 && nLenKMat <= 63))
    {
        pwzcConfig->dwCtlFlags |= WZCCTL_WEPK_XFORMAT;
        WZCPassword2Key(pwzcConfig, szKMat);
        pwzcConfig->KeyLength = WZCCTL_MAX_WEPK_MATERIAL;
    }
    else
    {
        switch(nLenKMat)
        {
        case 5:
        case 13:
        case 16:
            pwzcConfig->dwCtlFlags &= ~WZCCTL_WEPK_XFORMAT;
            break;

        case 10:
        case 26:
        case 32:
        case 64:
            pwzcConfig->dwCtlFlags |= WZCCTL_WEPK_XFORMAT;
            break;
        }        
            
        if (pwzcConfig->dwCtlFlags & WZCCTL_WEPK_XFORMAT)
        {
            UINT  nKMatIdx = 0;

            // we know here we have a valid hexadecimal formatting
            // this implies the string has an even number of digits
            while(*szKMat != '\0')
            {
                pwzcConfig->KeyMaterial[nKMatIdx] = HEX(*szKMat) << 4;
                szKMat++;
                pwzcConfig->KeyMaterial[nKMatIdx] |= HEX(*szKMat);
                szKMat++;
                nKMatIdx++;
            }
            pwzcConfig->KeyLength = nKMatIdx;
        }
        else
        {
            // the key is not in Hex format, so just copy over the bytes
            // we know the length is good so no worries about overwritting the buffer
            pwzcConfig->KeyLength = strlen(szKMat);
            memcpy(pwzcConfig->KeyMaterial, szKMat, pwzcConfig->KeyLength);
        }
    }

    // this is where the key should be encrypted inplace
    {
        for (i = 0; i < WZCCTL_MAX_WEPK_MATERIAL; i++)
            pwzcConfig->KeyMaterial[i] ^= chFakeKeyMaterial[(7*i)%13];
    }
}


LPTSTR StrDup(LPCTSTR pszIn)
{
    LPTSTR pszOut;

    ASSERT(pszIn != NULL);
    
    pszOut = (LPTSTR)LocalAlloc(LPTR, sizeof(TCHAR) * (_tcslen(pszIn) + 1));
    if (pszOut) 
    {
        _tcscpy(pszOut, pszIn);
    }

    return pszOut;
}


///
/// <summary>
///     Checks whether a string contains only HEX characters
/// </summary>
/// <returns>
///   BOOL
///     TRUE if all HEX,
///     FALSE otherwise
/// </returns>
/// <param name="pcszString">
///     The string to check
/// </param>
///

BOOL 
IsHexString(
    __in LPCSTR pcszString
    )
{
    ULONG ulIdx = 0;

    ASSERT(NULL != pcszString);

    while ('\0' != pcszString[ulIdx] && isxdigit(pcszString[ulIdx]))
    {
        ulIdx++;
    }

    return ('\0' == pcszString[ulIdx]);
}


///////////////////////////////////////////////////////////////////////////////////////////
///
DWORD WINAPI WZCUpdateStatusThread(LPVOID pv)
{
    HRESULT         hr                = S_OK;
    MSGQUEUEOPTIONS sOptions;
    HANDLE          hMsgQueue;
    HANDLE          hEventHandles[2];
    LOG_ENTRY       LogEntry          = {0};
    LOG_ENTRY       TmpLogEntry;
    DWORD           dwRetVal          = 0, dwFlags, dwBytesReturned;
    PWUSI           pwusi             = (PWUSI)pv;

    DEBUGMSG(ZONE_MISC, (TEXT("NETUI: Entering ZeroConfig MsgQueue loop")));
    
    //
    //    Open the message queue..
    //
    sOptions.dwSize        = sizeof(MSGQUEUEOPTIONS);
    sOptions.dwFlags       = MSGQUEUE_ALLOW_BROKEN;
    sOptions.dwMaxMessages = DEFAULT_MAX_MESSAGES;
    sOptions.cbMaxMessage  = sizeof(LOG_ENTRY);
    sOptions.bReadAccess   = TRUE;
    
    hMsgQueue = CreateMsgQueue(WZC_MSGQUEUE_NAME, &sOptions);
    CWR(hMsgQueue);

    // Read the initial entries in the queue and only keep the latest entry to display
    while (ReadMsgQueue(
                    hMsgQueue,
                    &TmpLogEntry,
                    sizeof(LOG_ENTRY),
                    &dwBytesReturned,
                    1,
                    &dwFlags))
    {
        // Handle the message but do not display to status field (only logging window)
        SendMessage(pwusi->hwndNotify, WM_WZC_STATUS_UPDATE, 
            LogEntry.dwLogId, (LPARAM)LogEntry.pucSSID);
    }

    if(LogEntry.dwLogId != 0)
    {
        SendMessage(pwusi->hwndNotify, WM_WZC_STATUS_UPDATE, 
            LogEntry.dwLogId, (LPARAM)LogEntry.pucSSID);
    }

    //
    //  Loop retrieving the queue.  If all wireless dialogs are closed there is no point in
    //  continuing.  So close the message queue.
    //

    hEventHandles[0] = pwusi->hEventStop;
    hEventHandles[1] = hMsgQueue;

    while(1) 
    {
        dwRetVal = WaitForMultipleObjects(2, hEventHandles, FALSE, INFINITE);
        if(1 != (dwRetVal - WAIT_OBJECT_0))
        {
            break;
        }

        while (ReadMsgQueue(
                    hMsgQueue,
                    &LogEntry,
                    sizeof(LOG_ENTRY),
                    &dwBytesReturned,
                    1,
                    &dwFlags))
        {
            //
            //    Okay, we have notification..  
            //

            DEBUGMSG(ZONE_MISC, (TEXT("NETUIQC: Received ZeroConfig Status Message")));

            dwRetVal = WaitForSingleObject(pwusi->hEventStop, 0);
            if (dwRetVal != WAIT_TIMEOUT)
            {
               break;
            }

            PostMessage(pwusi->hwndNotify, WM_WZC_STATUS_UPDATE, 
                LogEntry.dwLogId, (LPARAM)LogEntry.pucSSID);

            // Sleep for a bit to give the user time to read the status
            Sleep(WZC_STATUS_UPDATE_TIME);
        }
    }

    CloseMsgQueue(hMsgQueue);

Error:
    DEBUGMSG(ZONE_MISC, (TEXT("NETUI: Leaving ZeroConfig MsgQueue thread")));
    return 0;
} // UpdateStatusThread
