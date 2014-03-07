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
// Filename:    wzcqcfg.cpp
//
// Purpose:     WZC Quick Configuration
//              This file contains the entry points to the core control
//              functionality of WZC.
//
//==========================================================================*

#include <windows.h>
#include <wpcpriv.h>
#include <windev.h>
#include <ehm.h>

#include "netuip.h"
#include "wzcuip.h"
#include "wzcmgr.h"

#include "nuiouser.h"

// Turn on hi-dpi
HIDPI_ENABLE

#ifndef InvStrCmpI
#define InvStrCmpI _tcsicmp
#endif

// linked manager object list
class CManagerList
{
private:
    static CRITICAL_SECTION s_csLock;
    CWZCManager             *m_pmgrHead;

public:
    static void Initialize() { InitializeCriticalSection(&s_csLock); }
    static void Destroy() { DeleteCriticalSection(&s_csLock); }

    HRESULT Insert(__notnull CWZCManager *pmgr);
    CWZCManager *Remove(__notnull LPCTSTR pszDeviceName);

    CWZCManager *Find(__notnull LPCTSTR pszDeviceName) const;
    HWND FindWindow(__notnull LPCTSTR pszDeviceName) const;
    HRESULT EnumerateWindows(__notnull CWZCManager::PFNENUMCALLBACK pfnCallback, LPVOID pvParam) const;

public:
    CManagerList() : m_pmgrHead(NULL) {}
};


// globals
CRITICAL_SECTION    CManagerList::s_csLock;
static CManagerList g_mgrlist;


/*++

Routine Name:

    InitializeWZCQCFG

Routine Description:

    Initialize WZC resources. This function is called at DLL_PROCESS_ATTACH.

--*/
void WINAPI InitializeWZCQCFG(void)
{
    // Initialize the DPI scaling subsystem
    HIDPI::InitScaling();

    // Open the NDIS UIO device handle
    g_hNdisUio = CreateFile(NDISUIO_DEVICE_NAME, 0, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, (HANDLE)INVALID_HANDLE_VALUE);

    // Initialize the manager list
    g_mgrlist.Initialize();
}

/*++

Routine Name:

    DeleteWZCQCFG

Routine Description:

    Clean up WZC resources. This function is called at DLL_PROCESS_DETACH.

--*/

void WINAPI DeleteWZCQCFG(void)
{
    // Destroy the manager list
    g_mgrlist.Destroy();
    CloseHandle(g_hNdisUio);
}

// If we return false here, we'll completely unload netui for the given adapter
// and not handle it at all (which means we won't get connected/disconnected
// events, etc).
static BOOL IsDeviceSupported(LPCTSTR pszDeviceName, BOOL fWireless)
{
    HRESULT hr = S_OK;

    //$TEMP: We're temporarily putting in code to get rid of the
    // connection bubble for RNDIS devices. All we do is look for "RNDIS"
    // in the device name, and don't show the bubble if we find it. Once we
    // have a better method of detecting RNDIS devices, we'll use that
    // instead.
    CBR(fWireless || !_tcsstr(pszDeviceName, TEXT("RNDIS")));

    // Don't handle WWAN1 adapters, which include NDIS-based GPRS radio
    // adapters.
    CBR(0 == _tcsstr(pszDeviceName, TEXT("WWAN1")));

    // Don't handle internal security VPN virtual adapters
    CBR(0 == _tcsstr(pszDeviceName, TEXT("IPSECVPNVNIC1")));

Error:
    return SUCCEEDED(hr);
}

/*++

Routine Name:

    AddNetUISystrayIcon

Routine Description:

    This function is called by EthMan when a new network card is installed. We
    use it to kick off the WZC processing for that card.

Arguments:

    ptcDeviceName:  String representing the name of the device
    fWireless:      Device is wireless
    phWnd:          A handle to the callback window for the device

Return Value:

    HRESULT to indicate return status

--*/
HRESULT WINAPI AddNetUISystrayIcon(PTCHAR ptcDeviceName, BOOL fWireless, HWND *phWnd)
{
    HRESULT     hr = S_OK;
    CWZCManager *pmgr = NULL;
    BOOL        fInitializedCOM = FALSE;

    // Check for devices that we don't support. If we fail here, it just means
    // that we don't have any UI for the adapter. It won't affect EthMan's
    // handling of the adapter in any way.
    CBREx(IsDeviceSupported(ptcDeviceName, fWireless), E_NOTIMPL);

    // Make sure Shell is ready
    while (!IsAPIReady(SH_SHELL))
    {
        Sleep(500);
    }

    // Fire it up, baby
    DEBUGMSG(TRUE, (TEXT("NETUI: %s network device '%s' added\r\n"),
        fWireless ? TEXT("Wireless") : TEXT("Wired"), ptcDeviceName));

    // Initialize COM on this thread
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    CHR(hr);
    fInitializedCOM = TRUE;

    // Create a new manager object for this device
    hr = CWZCManager::Create(ptcDeviceName, fWireless, &pmgr);
    CHR(hr);

    // Add the device to the linked list (needs to be the last thing we do,
    // since we don't remove it during error handling)
    hr = g_mgrlist.Insert(pmgr);
    CHR(hr);

    // This is the window that will receive callbacks from EthMan
    *phWnd = pmgr->GetSinkWindow();

Error:
    if (FAILED(hr))
    {
        if (fInitializedCOM)
        {
            CoUninitialize();
        }

        if (NULL != pmgr)
        {
            delete pmgr;
        }
    }

    return hr;
}

/*++

Routine Name: 

    UpdateConnectionStatus

Routine Description:

    EthMan calls this function when the connection status of a network card
    changes. This call is NOT in the sink window UI thread context.

Arguments:

    ptcDeviceName: String representing the name of the device
    fConnected:    Connected/disconnected

--*/
void WINAPI UpdateConnectionStatus(PTCHAR ptcDeviceName, BOOL fConnected)
{
    CWZCManager *pmgr;

    DEBUGMSG(TRUE, (TEXT("NETUI: Network device '%s' has been %s\r\n"),
        ptcDeviceName, fConnected ? TEXT("connected") : TEXT("disconnected")));

    pmgr = g_mgrlist.Find(ptcDeviceName);
    if (pmgr)
    {
        pmgr->OnConnectionStatusUpdate(fConnected);
    }

    // Notify the Shell
    PostShellStatusMessage(WM_TASKBAR_NETCONNCHANGED, 0, 0);
}

/*++

Routine Name:

    RemoveNetUISystrayIcon

Routine Description:

    This function is called by EthMan when a network card is removed from the
    system. This call is NOT in the sink window UI thread context.

Arguments:

    ptcDeviceName:  String representing the name of the device

Return Value:

    HRESULT to indicate return status

--*/
HRESULT WINAPI RemoveNetUISystrayIcon(PTCHAR ptcDeviceName)
{
    CWZCManager *pmgr;

    DEBUGMSG(TRUE, (TEXT("NETUI: Network device '%s' removed\r\n"), 
        ptcDeviceName));

    // Tell the UI thread that its associated card has been removed. It's
    // responsible for cleaning up and exiting.
    pmgr = g_mgrlist.Remove(ptcDeviceName);
    if (pmgr)
    {
        pmgr->RemoveCard();
    }

    // Notify the Shell
    PostShellStatusMessage(WM_TASKBAR_NETCONNCHANGED, 0, 0);

    // Success
    return S_OK;
}

/*++

Routine Name:

    IsPropSheetDialogMessage

Routine Description:

    This function is called before each message in the thread message loop to
    determine if the message should NOT be translated and dispatched normally.

Arguments:

    hwnd: The device callback window
    pMsg: The message structure

Return Value:

    HRESULT to indicate return status

--*/
BOOL WINAPI IsPropSheetDialogMessage(HWND hwnd, MSG* pMsg)
{
    return CWZCManager::IsDialogMessage_WZC(hwnd, pMsg);
}

/*++

Routine Name:

    ClosePropSheetDialogIfReady

Routine Description:

    This function is called by EthMan before every message is processed to
    check if the dialog should be closed.

Arguments:

    hwnd:   The device callback window

Return Value:

    HRESULT to indicate return status

--*/
HRESULT WINAPI ClosePropSheetDialogIfReady(HWND hwnd)
{
    UNREFERENCED_PARAMETER(hwnd);

    return S_OK;
}

/*++

Routine Name:

    ClosePropSheetDialog

Routine Description:

    EthMan never calls this function, but theoretically it would be used to
    force the property sheet dialog to close. We don't have a "property sheet
    dialog," so we wouldn't care anyway.

Arguments:

    hwnd:   The device callback window

Return Value:

    HRESULT to indicate return status

--*/
HRESULT WINAPI ClosePropSheetDialog(HWND hwnd)
{
    UNREFERENCED_PARAMETER(hwnd);

    return S_OK;
}

HRESULT CManagerList::EnumerateWindows(CWZCManager::PFNENUMCALLBACK pfnCallback, LPVOID pvParam) const
{
    HRESULT     hr = S_OK;
    CWZCManager *pmgr;
    UINT        cItems = 0;
    UINT        iItem;
    HWND        *phwndList = NULL;

    ASSERT(NULL != pfnCallback);

    // Lock the list
    EnterCriticalSection(&s_csLock);

    // Count the number of items
    for (pmgr = m_pmgrHead, cItems = 0; pmgr; pmgr = pmgr->m_pmgrNext)
    {
        ++cItems;
    }

    if (0 != cItems)
    {
        // Allocate a buffer to hold the window handles
        phwndList = new HWND[cItems];
        CPR(phwndList);

        // Add the items to the list
        for (pmgr = m_pmgrHead, iItem = 0; pmgr; pmgr = pmgr->m_pmgrNext)
        {
            phwndList[iItem++] = pmgr->m_hwndSink;
        }
    }

Error:
    // Unlock the list
    LeaveCriticalSection(&s_csLock);

    if (SUCCEEDED(hr))
    {
        // Perform the callbacks
        for (iItem = 0; iItem != cItems; ++iItem)
        {
            hr = pfnCallback(phwndList[iItem], pvParam);
            if (FAILED(hr))
            {
                break;
            }
        }
    }

    if (phwndList)
    {
        delete[] phwndList;
    }

    return hr;
}

CWZCManager *CManagerList::Find(LPCTSTR pszDeviceName) const
{
    CWZCManager *pmgr;
    CWZCManager *pmgrResult = NULL;

    // Lock the list
    EnterCriticalSection(&s_csLock);

    // Step through the items and look for the matching object
    for (pmgr = m_pmgrHead; pmgr; pmgr = pmgr->m_pmgrNext)
    {
        if (!InvStrCmpI(pmgr->m_pszDeviceName, pszDeviceName))
        {
            pmgrResult = pmgr;
            break;
        }
    }

    // Unlock the list
    LeaveCriticalSection(&s_csLock);

    // Done
    return pmgrResult;
}

HWND CManagerList::FindWindow(LPCTSTR pszDeviceName) const
{
    CWZCManager *pmgr;
    HWND        hwnd = NULL;

    // Lock the list
    EnterCriticalSection(&s_csLock);

    // Get the associated window handle
    pmgr = Find(pszDeviceName);
    if (pmgr)
    {
        hwnd = pmgr->m_hwndSink;
    }

    // Unlock the list
    LeaveCriticalSection(&s_csLock);

    // Done
    return hwnd;
}

HRESULT CManagerList::Insert(CWZCManager *pmgr)
{
    HRESULT hr = S_OK;
    LPVOID lpVoid = NULL;

    // Lock the list
    EnterCriticalSection(&s_csLock);

    // Insert the item at the head of the list
    pmgr->m_pmgrNext = m_pmgrHead;
    m_pmgrHead = pmgr;

    // Handle items that need to be protected by the critical section
    hr = pmgr->OnListInsert(&lpVoid);
    CHR(hr);

Error:
    if (FAILED(hr))
    {
        // Undo the add
        m_pmgrHead = pmgr->m_pmgrNext;
        pmgr->m_pmgrNext = NULL;
    }

    // Unlock the list
    LeaveCriticalSection(&s_csLock);

    if (lpVoid)
    {
        pmgr->OnListCleanup(lpVoid);
    }
    return hr;
}

CWZCManager *CManagerList::Remove(LPCTSTR pszDeviceName)
{
    CWZCManager **ppmgr;
    CWZCManager *pmgrResult = NULL;
    LPVOID      pvPostParam = NULL;

    // Lock the list
    EnterCriticalSection(&s_csLock);

    // Step through the items and look for the matching object
    for (ppmgr = &m_pmgrHead; *ppmgr; ppmgr = &((*ppmgr)->m_pmgrNext))
    {
        if (!InvStrCmpI((*ppmgr)->m_pszDeviceName, pszDeviceName))
        {
            // Handle items that need to be protected by the critical section
            pmgrResult = *ppmgr;
            pvPostParam = pmgrResult->OnListRemove();

            *ppmgr = pmgrResult->m_pmgrNext;
            pmgrResult->m_pmgrNext = NULL;
            break;
        }
    }

    // Unlock the list
    LeaveCriticalSection(&s_csLock);

    // Post process outside the critical section if necessary
    if (pvPostParam)
    {
        pmgrResult->OnListCleanup(pvPostParam);
    }

    // Done
    return pmgrResult;
}

HWND CWZCManagerWireless::FindDevice(LPCTSTR pszDeviceName)
{
    return g_mgrlist.FindWindow(pszDeviceName);
}

HRESULT CWZCManager::EnumerateWindows(PFNENUMCALLBACK pfnCallback, LPVOID pvParam)
{
    return g_mgrlist.EnumerateWindows(pfnCallback, pvParam);
}
