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
//=========================================================================
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//
//
//=========================================================================

#pragma once

#include <windows.h>


// Find Item Attribute Mask

// these mask bits determine how the find is filtered or which fields are to
// be filled when requesting attributes

#define FIAM_TEXT                      0x0001
#define FIAM_TITLE                     FIAM_TEXT
#define FIAM_TYPENAME                  0x0002
#define FIAM_TYPES                     0x0004
#define FIAM_SCOPE                     0x0008
#define FIAM_SIZE                      0x0010
#define FIAM_TIMESTAMP                 0x0020
#define FIAM_ICON                      0x0040
#define FIAM_CUSTOM                    0x8000
#define FIAM_ALL                       0xFFFF


// Find Item Scope

// these bits determine the scope filter or specify in which scope the item
// resides

#define FIS_INTERNALMEMORY             0x0001
#define FIS_SECONDARYSTORAGE           0x0002
#define FIS_PRIMARYPROVIDERS           0x0004
#define FIS_LOCALNETWORK               0x0008
#define FIS_INTERNET                   0x0010


// Find Item Flags

// these bits specify flags to determine the usage of attributes in a
// filter

#define FIF_SIZE_SMALLERTHAN           0x0001
#define FIF_SIZE_LARGERTHAN            0x0002
#define FIF_TIMESTAMP_BEFORE           0x0004
#define FIF_TIMESTAMP_AFTER            0x0008


// attributes of found items

// this struct is used both as a filter definition and a specifier for
// attributes of an individual item

typedef struct _FINDITEMATTRIBUTES
{
    DWORD       cbStruct;
    DWORD       grfiamMask;
    union
    {
        LPTSTR  pszFindText;
        LPTSTR  pszTitle;
    };
    LPTSTR      pszTypeName;
    CLSID       *prgclsidTypes;
    DWORD       cclsidTypes;
    DWORD       grfisScope;
    SIZE_T      sizeItem;
    FILETIME    ftTimeStamp;
    HICON       hicon;
    void        *pvCustom;

} FINDITEMATTRIBUTES;


//===========================================================================
//
// Interface: IShellFindItem
//
// The IShellFindItem interface is used to reference a single item in a find.
// Items are allocated by the client and passed back to Find through the
// callback interface IShellFindCallback.  
//
// [Member functions]
//
// IShellFindItem::GetAttributes
// 
// This member function is called to retrieve attributes of the item.
//
// IShellFindItem::Invoke
//
// This member function is called when the user selects the item with a
// tap or hardware button.
//
//===========================================================================

#undef  INTERFACE
#define INTERFACE IShellFindItem

DECLARE_INTERFACE_(IShellFindItem, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    STDMETHOD(GetAttributes) (THIS_ FINDITEMATTRIBUTES *pfia) PURE;
    STDMETHOD(SetAttributes) (THIS_ FINDITEMATTRIBUTES *pfia) PURE;

    STDMETHOD(Invoke)(THIS) PURE;

};


DECLARE_INTERFACE(IShellFindCallback);  // used by IShellFind


//===========================================================================
//
// Interface: IShellFind
//
// The IShellFind interface is used by the Find application to control
// a custom Find handler.
//
// [Member functions]
//
// IShellFind::Initialize
// 
// This member function is called to initialize the handler.
//
// IShellFind::SetCallback
//
// This member function is called to attach the handler to a different
// callback interfacte.
//
// IShellFind::LaunchCustomOptions
//
// This member function is called when the user taps the Options button in
// the Find application.
//
// IShellFind::StartFind
//
// This member function is called by the Find application to start a search.
//
// IShellFind::AbortFind
//
// This member function is called by the Find application to abort a search.
//
//===========================================================================

#undef  INTERFACE
#define INTERFACE IShellFind

DECLARE_INTERFACE_(IShellFind, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    STDMETHOD(Initialize) (THIS_ IShellFindCallback *pisfcNotify) PURE;

    STDMETHOD(SetCallback) (THIS_ IShellFindCallback *pisfcNew) PURE;

    STDMETHOD(LaunchCustomOptions)(THIS_ HWND hwndParent,
        FINDITEMATTRIBUTES *pfia) PURE;

    STDMETHOD(StartFind)(THIS_ FINDITEMATTRIBUTES *pfiaFilter,
        DWORD grfifFlags) PURE;

    STDMETHOD(AbortFind)(THIS) PURE;

};


//===========================================================================
//
// Interface: IShellFindCallback
//
// The IShellFindCallback interface is used by the custom Find handler to
// communicate back to the Find application.
//
// [Member functions]
//
// IShellFindCallback::OnFindProgress
// 
// This member function is called to notify the Find application of progress
// in the search.
//
// IShellFindItem::OnItemChanged
//
// This member function is called by the Find custom handler to notify the
// Find application that an item has changed.
//
// IShellFindItem::OnItemDeleted
//
// This member function is called by the Find custom handler to notify the
// Find application that an item has been deleted.
//
//===========================================================================

#undef  INTERFACE
#define INTERFACE IShellFindCallback

DECLARE_INTERFACE_(IShellFindCallback, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;

    STDMETHOD(OnFindProgress) (THIS_ DWORD dwPercent, int cNewItems,
        IShellFindItem **prgpisfiNew) PURE;

    STDMETHOD(OnItemChanged)(THIS_ IShellFindItem *pisfiChanged) PURE;

    STDMETHOD(OnItemDeleted)(THIS_ IShellFindItem *pisfiDeleted) PURE;

};

