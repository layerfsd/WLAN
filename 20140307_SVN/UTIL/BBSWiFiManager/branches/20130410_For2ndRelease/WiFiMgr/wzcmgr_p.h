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
// Filename:    wzcmgr_p.h
//
// Purpose:     PPC WZC Manager header
//
//==========================================================================*
//==========================================================================*
#pragma once

#include "wzcmgr.h"

const UINT c_csConnectionBubbleRetry = 10;
const UINT c_csConnectionBubbleFail = 180;

class CWZCManagerWiredPPC : public CWZCManagerWired
{
private:
    enum TimerID
    {
        c_tidConnectionBubbleRetry = 370
    };

private:
    UINT m_cConnectionBubbleRetries;

protected:
    // connection functions
    virtual HRESULT OnConnect();
    virtual void OnDisconnect();

    // message handlers
    virtual LRESULT OnMessage(UINT msg, WPARAM wp, LPARAM lp);
    virtual void OnTimer(UINT uTimerID);

    // UI functions
    virtual void DestroyUI() { HideConnectionBubble(); CWZCManagerWired::DestroyUI(); }

private:
    HRESULT ShowConnectionBubble();
    void HideConnectionBubble();

    // message handlers
    void OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl);
    LRESULT OnNotify(int idCtrl, NMSHN *pnmshn);

public:
    CWZCManagerWiredPPC() : m_cConnectionBubbleRetries(0) {}
};

class CWZCManagerWirelessPPC : public CWZCManagerWireless
{
private:
    int m_nKeyState :3;    // the network key bubble state
    int m_fInSelectUI :1;  // the user has selected a network

protected:
    virtual HRESULT OnCheckNewNetworks();
    virtual BOOL IsUIBlocked() const;
    virtual void DestroyUI() { HideBubble(); CWZCManagerWireless::DestroyUI(); }

    virtual HRESULT ShowNewNetworksUI();
    virtual HRESULT ShowPasswordDialog(DWORD dwState);

    // message handlers
    virtual LRESULT OnMessage(UINT msg, WPARAM wp, LPARAM lp);

private:
    void HideBubble();

    HRESULT OnConnectButton(UINT uMeta);
    HRESULT OnMultiSelect(UINT uSel);
    HRESULT OnWepKeyInput(LPCTSTR pszKey);

    // message handlers
    void OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl);
    LRESULT OnNotify(int idCtrl, NMSHN *pnmshn);

public:
    CWZCManagerWirelessPPC() : m_nKeyState(-1), m_fInSelectUI(0) {}
};
