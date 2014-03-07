#ifndef _CSHOBJ_H_
#define _CSHOBJ_H_
// Copyright (c) 1994-1999, Microsoft Corp. All rights reserved.
//***   cshobj.h -- CE shell objects
//

#if 0 //lives in SHLOBJ.H
//***   INewMenuItemServer -- ??? {
//
#undef  INTERFACE
#define INTERFACE   INewMenuItemServer

DECLARE_INTERFACE_(INewMenuItemServer, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    STDMETHOD(CreateNewItem)             (THIS_ HWND hwndParent) PURE;
};
// }
#endif

//***   IContextMenuMulti -- maintain list of IContextMenu's {
// DESCRIPTION
//  Initialize      the usual
//  AddIface        merge in a COM IContextMenu
//  AddCLSID        like AddIface, but create object too
//  AddMenu         merge in a win32 MENU
//  InvokeIDM       do the dispatch (InvokeCommand or ForwardMsg)
//  FindIface       map IDM to iface/MENU and 'real' IDM
//      when InvokeIDM isn't flexible enough client can party on iface himself
// coming soon (maybe):
//  ShowMenu        (?) pop up the menu
//
#undef  INTERFACE
#define INTERFACE   IContextMenu

DECLARE_INTERFACE_(IContextMenuMulti, IUnknown)
{
    //*** IUnknown methods ***
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    //*** THISCLASS methods ***
    STDMETHOD(Initialize)(THIS_ HWND hwnd, HMENU hMenu, UINT idCmdFirst, UINT idCmdLast) PURE;

    STDMETHOD(AddIface)(THIS_ IUnknown *punkCmenu, UINT indexMenu, UINT uFlags, IUnknown *punkOwner) PURE;
    STDMETHOD(AddCLSID)(THIS_ REFIID rclsid, UINT indexMenu, UINT uFlags, IUnknown *punkOwner) PURE;
    STDMETHOD(AddMenu)(THIS_ HWND hwnd, HMENU hmenu, UINT indexMenu, UINT uFlags) PURE;
    STDMETHOD(InvokeIDM)(THIS_ UINT idCmd, HWND hwndOwner) PURE;
    STDMETHOD_(IContextMenu *, FindIface)(THIS_ UINT *pidCmd) PURE;
    STDMETHOD(Close)(THIS) PURE;
};

// }

//***   IPimDataProvider -- ??? {
// DESCRIPTION
//  GetObjectOf     ...
//
#undef  INTERFACE
#define INTERFACE   IPimDataProvider

DECLARE_INTERFACE_(IPimDataProvider, IUnknown)
{
    //*** IUnknown methods ***
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    //*** THISCLASS methods ***
    STDMETHOD(GetObjectOfPDP)(THIS_ CEOID oid, CEPROPID hhpr, REFIID riid, void ** ppv) PURE;
};

// }

//***   IShellService -- ??? {
// DESCRIPTION
//  SetOwner        ...
//
#undef  INTERFACE
#define INTERFACE   IShellService

DECLARE_INTERFACE_(IShellService, IUnknown)
{
    //*** IUnknown methods ***
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    //*** THISCLASS methods ***
    STDMETHOD(SetOwner)(THIS_ IUnknown *punkOwner) PURE;
};

// }

#endif // _CSHOBJ_H_
