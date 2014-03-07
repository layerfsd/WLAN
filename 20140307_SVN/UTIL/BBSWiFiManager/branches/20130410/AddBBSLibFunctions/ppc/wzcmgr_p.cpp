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
// Filename:    wzcmgr_p.cpp
//
// Purpose:     PPC WZC Manager code
//
//==========================================================================*
//==========================================================================*

#include <windows.h>
#include <windowsx.h>
#include <wpcpriv.h>
#include <ehm.h>

#include "netuip.h"
#include "wzcuip.h"
#include "wzcmgr_p.h"
#include "connmgr_priv.h"
#include "resource.h"

HRESULT CWZCManager::Create(LPCTSTR pszDeviceName, BOOL fWireless, CWZCManager **ppmgr)
{
    HRESULT     hr = S_OK;
    CWZCManager *pmgr = NULL;

    ASSERT(NULL != ppmgr);
    *ppmgr = NULL;

    // Create the object
    if (fWireless)
    {
        pmgr = new CWZCManagerWirelessPPC;
    }
    else
    {
        pmgr = new CWZCManagerWiredPPC;
    }

    CPR(pmgr);

    // Initialize the object
    hr = pmgr->Initialize(pszDeviceName);
    CHR(hr);

    // Success
    *ppmgr = pmgr;
    pmgr = NULL;

Error:
    if (NULL != pmgr)
    {
        delete pmgr;
    }

    return hr;
}

HRESULT CWZCManagerWiredPPC::OnConnect()
{
    HRESULT hr = S_OK;
    TCHAR   szGUID[48];
    LPCTSTR pszText;

    // Default processing
    hr = CWZCManagerWired::OnConnect();
    CHR(hr);

    // Load the wired network description
    pszText = (LPCTSTR)LoadString(HINST_RESDLL, IDS_NIC_DESCRIPTION, NULL, 0);
    CBRA(NULL != pszText);

    // Do we know which metanetwork this connects to?
    if (SUCCEEDED(GetNetworkDestGUID(NULL, FALSE, szGUID, ARRAYSIZE(szGUID))))
    {
        UINT iMeta = 0;

        hr = GetMetaNetworkIndex(szGUID, &iMeta);
        CHR(hr);

        hr = CM_SetCurrentNetwork(m_pszDeviceName, pszText, iMeta);
        CHR(hr);
    }
    else
    {
        // The user has never chosen a destination network for wired
        // cards, so we'll assume Work until they choose something else
        // NOTE: If this fails, we don't really care... there's no
        // reason to tell the user, since they'll normally pick a
        // network from the bubble anyway
        CM_SetCurrentNetwork(m_pszDeviceName, pszText, 1);

        // Bring up the bubble so they can change the selection if necessary
        ShowConnectionBubble();
    }

Error:
    return hr;
}

void CWZCManagerWiredPPC::OnDisconnect()
{
    // Hide the bubble if it's still around
    HideConnectionBubble();

    // Default processing
    CWZCManagerWired::OnDisconnect();
}

LRESULT CWZCManagerWiredPPC::OnMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_NOTIFY:
        return (lp ? OnNotify((int)wp, (NMSHN*)lp) : 0);

    case WM_COMMAND:
        OnCommand(HIWORD(wp), LOWORD(wp), (HWND)lp);
        return 0;
    }

    // Default processing
    return CWZCManagerWired::OnMessage(msg, wp, lp);
}

void CWZCManagerWiredPPC::OnTimer(UINT uTimerID)
{
    HRESULT hr = S_OK;

    switch (uTimerID)
    {
    case c_tidConnectionBubbleRetry:
        KillTimer(m_hwndSink, c_tidConnectionBubbleRetry);
        CBRA(m_cConnectionBubbleRetries++ < c_csConnectionBubbleFail / c_csConnectionBubbleRetry);
        ShowConnectionBubble();
        break;
    }

Error:
    UNREFERENCED_PARAMETER(hr);
}

HRESULT CWZCManagerWirelessPPC::OnCheckNewNetworks()
{
    HRESULT hr = S_OK;

    // Default processing
    hr = CWZCManagerWireless::OnCheckNewNetworks();
    CHR(hr);

Error:
    // If hr is S_FALSE, it means there are no visible networks
    if (S_OK != hr && !IsUIBlocked())
    {
        // If we're not showing UI for any reason other than new UI being
        // blocked, then hide any current UI
        HideBubble();
    }

    return hr;
}

BOOL CWZCManagerWirelessPPC::IsUIBlocked() const
{
    // Anytime you add a new condition to this list, you also need to call
    // OnUnblockUI when the condition changes so that normal processing can
    // resume. For examples, see the places that we call OnUnblockUI.
    return (m_fInSelectUI || -1 != m_nKeyState ||
        CWZCManagerWireless::IsUIBlocked());
}

LRESULT CWZCManagerWirelessPPC::OnMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_NOTIFY:
        return (lp ? OnNotify((int)wp, (NMSHN*)lp) : 0);

    case WM_COMMAND:
        OnCommand(HIWORD(wp), LOWORD(wp), (HWND)lp);
        return 0;
    }

    // Default processing
    return CWZCManagerWireless::OnMessage(msg, wp, lp);
}
