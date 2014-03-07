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
#ifndef _CWNDPROC_H_
#define _CWNDPROC_H_
//***   cwndproc.h -- c++ WndProc for object
// NOTES
//  extracted (w/ mods) from browseui/cwndproc.cpp

//***   CWndProc
//
// Use this class when you want to associate a window with
// an object using a virtual wndproc.
// 
// NOTE: The window's lifetime must be encompassed by the object.
//       I.E. NO REFCOUNT IS HELD ON THE OBJECT!
//
// Messages after WM_NCCREATE up to and including WM_DESTROY
// are forwarded to v_WndProc.
//
// _hwnd is non-NULL from WM_NCCREATE up to but not during WM_DESTROY.
// (Not during because the final release may be tied to WM_DESTROY
// so we cannot reference member variables after forwarding this message.)
//
class CWndProc
{
public:
    STDMETHOD_(ULONG, AddRef)() = 0;
    STDMETHOD_(ULONG, Release)() = 0;

    STDMETHOD_(HWND, GetWindowWP)() { return _hwnd; }

#if 1 
    // we don't use a clearing allocator
    CWndProc() { _hwnd = NULL; }
    //this will catch the case where the window's lifetime extends past that of the object
    virtual ~CWndProc() { ASSERT(_hwnd == NULL); }
#endif

protected:
    STDMETHOD_(LRESULT, v_WndProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) PURE;
    static LRESULT CALLBACK s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

public:
protected:
    HWND _hwnd;
private:
};

//***   CDlgProc -- like CWndProc, but for dialogs
// NOTES
//  client-side is a bit different since the 'binding' must be done after
// the create.
class CDlgProc
{
public:
    STDMETHOD_(ULONG, AddRef)() = 0;
    STDMETHOD_(ULONG, Release)() = 0;

    STDMETHOD_(BOOL, v_DlgProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    static BOOL s_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static BOOL s_INITDIALOG(CDlgProc *p, HWND hwnd, UINT uMsg, WPARAM wP, LPARAM lP);

#if 1 
    // we don't use a clearing allocator
    CDlgProc() { _hwnd = NULL; }
    //this will catch the case where the window's lifetime extends past that of the object
    virtual ~CDlgProc() { ASSERT(_hwnd == NULL); }
#endif


protected:
    HWND    _hwnd;

private:
};

#define CDlgProc_DLGPROC(clsname, hwnd, uMsg, wP, lP) \
    if (uMsg == WM_INITDIALOG) { \
        clsname *p; \
        p = new clsname; \
        return CDlgProc::s_INITDIALOG(p, hwnd, uMsg, wP, lP); \
    } \
    /*NOTREACHED*/ \
    ASSERT(0); \
    return FALSE;

#if 0 // { currently unused
// CNotifySubclassWndProc
//
// This class subclasses an HWND, registers for SHChangeNotify events,
// and forwards them to the inheritor's IShellChangeNotify implementation.
//
// You need one instance of this class per window you want to subclass
// and register for events against. (So if you need >1 window hooked up
// in this matter, you need to have member implementations that inherit
// from this class.)
//
class CNotifySubclassWndProc
{
public:
    virtual STDMETHODIMP OnChange(LONG lEvent, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2) PURE;

protected:
    BOOL _SubclassWindow(HWND hwnd);
    void _UnsubclassWindow(HWND hwnd);
    void _RegisterWindow(HWND hwnd, LPCITEMIDLIST pidl, long lEvents,
                         UINT uFlags = (SHCNRF_ShellLevel | SHCNRF_InterruptLevel));
    void _UnregisterWindow(HWND hwnd);
    virtual LRESULT _DefWindowProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

    void _FlushNotifyMessages(HWND hwnd);

private:
    static LRESULT CALLBACK _SubclassWndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

    UINT        _uRegister;         // SHChangeNotify id
    WNDPROC     _lpfnWndProc;       // subclassed WndProc

#ifdef DEBUG
    HWND        _hwndSubclassed;
#endif
} ;
#endif // }

#endif // _CWNDPROC_H_
