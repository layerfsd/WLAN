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
#ifndef _CONKEY_H_
#define _CONKEY_H_
//***   COnKeyHold -- WndProc WM_KEY* handler w/ key-hold support
//

#define KHDLY_KEYHOLD   500     // default timeout

// aka _KHM_HOLDxxx = WM_KEYxxx - WM_KEYFIRST
#define _KHM_HOLDDOWN   0       // _uMsgFirst + _KHM_HOLDDOWN
#define _KHM_HOLDUP     1       // _uMsgFirst + _KHM_HOLDUP

class COnKeyHold
{
public:
    STDMETHODIMP_(void) InitializeKH(UINT uMsgFirst, UINT idTimer, UINT uTimeout);
    STDMETHODIMP_(void) SetKeyHoldableKH(const int *pVKeys, const int *pVKData, int cKeys);
    STDMETHODIMP_(int) OnKeyMessageKH(MSG *pmsg);

protected:
private:
    int _GetKeyData(int vkey);
    HWND _GetWindowKH();
    void _OnHold();

public:
protected:
private:
    // hold delay
    // BUGBUG should we make these static (i.e. all clients use same #s)?
    UINT    _dlyHold;           // ...
    UINT    _uHoldStart;        // Time when hold started
    UINT    _uLastKeydown;      // Time when last key down is sent
    UINT    _uMsgFirst;         // base for messages

    // hold map
    const int *   _pVKHoldable;       // holdable keys, VK_*
    const int *   _pVKData;           // when to send non-hold, WM_KEY*
    int     _cHoldable;         // ARRAYSIZE(_pVK*)

    // cached WM_KEYDOWN info
    HWND    _hwndKH;
    WPARAM  _vkKH;
    LRESULT _lresKH;
    int     _eState;
};

extern "C" {
COnKeyHold *_GetKH(BOOL fCreate);
HRESULT InitializeKH(UINT uMsgFirst, UINT idTimer, UINT uTimeout);
int SetKeyHoldableKH(const int *pVKeys, const int *pVKData, int cKeys);
int TranslateMessageKH(HWND hwnd, MSG *pmsg);

BOOL WINAPI DllMainKH(HANDLE hinstDll, DWORD dwReason, LPVOID pvReserved);
};

#if 0 // sample client {
LRESULT xxx::v_WndProc(...)
{
    switch (uMsg) {
    case WM_KEYDOWN:
    case WM_KEYUP:
        return _OnKeyHold(uMsg, wP, lP);

    case WM_TIMER:
        switch (wParam) {
        case IDT_KEYHOLD:
            return _OnKeyHold(uMsg, wP, lP);
        ...
        }
        break;
    ...
    }
    return 0;
}

//***   _OnKeyHold -- WndProc key+hold handling
//
LRESULT CTst::_OnKeyHold(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    KH_OP ko;

    switch (uMsg) {
    // per CKeyHold:
    // - WM_KEYDOWN, WM_KEYUP, WM_TIMER(IDT_KEYHOLD)
    // - WM_KEYDOWN starts our state machine;
    // - WM_KEYUP handles the non-hold case (if IDT_KEYHOLD hasn't come in);
    // - IDT_KEYHOLD handles the hold case (and suppresses WM_KEYUP case);
    // - keys that don't support hold should 'unhold' to re-enable WM_KEYUP
    case WM_KEYDOWN:
        ko = OnKeyDownKH(_hwnd, wParam, lParam);
        if (ko == KHO_NIL) {
            // (handle 'real work' for hold/non-hold case below)
            break;
        }
        // 1st-chance for semi-holdable keys
        _OnKeyTranslate(ko, wParam, lParam);
        break;
    case WM_KEYUP:
        ko = OnKeyUpKH(_hwnd, wParam, lParam);
        if (ko == KHO_NIL) {
            // hold case already handled in WM_TIMER(IDT_KEYHOLD)
            break;
        }

        // handle non-hold case of WM_KEYDOWN,wParam...
        // also handle guys that don't support hold
        _OnKeyTranslate(ko, wParam, lParam);
        break;
    case WM_TIMER:
        switch (wParam) {
        case IDT_KEYHOLD:
            DEBUGMSG(ZONE_KEYS, (TEXT("cd.v_wp: WM_T(IDT_KEYHOLD)\r\n")));
            ko = OnKeyTimerKH(_hwnd, &wParam, &lParam);
            if (ko == KHO_NIL) {
                // all done for guys that don't support hold
                DEBUGMSG(ZONE_KEYS, (TEXT("cd.v_wp: ignore WM_T(IDT_KEYHOLD)\r\n")));
                break;
            }

            // handle hold case of WM_KEYDOWN,wParam...
            _OnKeyTranslate(ko, wParam, lParam);
            break;

        default:
            ASSERT(0);
            goto Ldodef;
        }
        break;
    default:
        ASSERT(0);
Ldodef:
        return DefWindowProc(_hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

//***   _OnKeyTranslate -- key-to-command translation
//
LRESULT CTst::_OnKeyTranslate(KH_OP kop, WPARAM wParam, LPARAM lParam)
{
    BOOL fHold;

#ifdef DEBUG
    TCHAR chKop = Dbg_KopToChar(kop);
#endif
    fHold = ISKHO_HOLD(kop);
    switch (wParam) {
    // non-hold and hold guys
    case VK_TTALK:
    case VK_TEND:
    case 'D':
    case 'U':
        DEBUGMSG(ZONE_KEYS, (TEXT("cda._ok vk=%x('%c') kop=%c fHold=%d\r\n"), wParam, wParam, chKop, fHold));
        break;

    // non-hold only guys
    default:
        ASSERT(!fHold);     // should have been filtered out by OnKeyTimerKH

        // ok, now we'll handle it
        switch (wParam) {
        case 777:   // warn C4065: no case labels
        default:
            DEBUGMSG(ZONE_KEYS, (TEXT("cda._ok vk=%x('%c') kop=%c fHold=%d\r\n"), wParam, wParam, chKop, fHold));
            break;
        }
        break;
    }
    return 0;
}
#endif // }

#endif // _CONKEY_H_
