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
/*****************************************************************************

    File: AppExt.h

    Some applications that support context menu extensions expose their
    object abstraction as an IDataObject.  The following definitions can be 
    used to describe the contents of that IDataObject.

    For example, the Contacts application exposes an IDataObject that contains
    a CEOID.  The CEOID can then be used in calls to Pocket Outlook Object Model
    (POOM) function calls, which are declared in the header file pimstore.h. 

    For example:

    #include "appext.h"
    #include "pimstore.h"
    
    FORMATETC fmt;
    STGMEDIUM stgmed;
    ItemRefArray *pira;
    CEOID oid;
    GUID * pguidObjectType;

    fmt.cfFormat = RegisterClipboardFormat(CFNAME_ITEMREFARRAY);
    fmt.lindex = -1;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.tymed = TYMED_HGLOBAL;

    ((IDataObject*)GetSite())->GetData(&fmt, &stgmed);
    pira = (ItemRefArray*) stgmed->hGlobal;
    pguidObjectType = (GUID*) pira->rgRefs[0].pType;
    assert(*pguidObjectType == ITI_PimItemRef);
    oid = (CEOID)pira->rgRefs[0].pRef;

*/


#pragma once


typedef GUID ITEM_TYPE_ID;

struct ItemRef
{
    ITEM_TYPE_ID* pType; // identifies the type of object, and therefore the type of pRef
    void* pRef;
};

struct ItemRefArray
{
    int cRefs;
    ItemRef rgRefs[1]; // rgRefs has cRefs entries
};

#define CFNAME_ITEMREFARRAY TEXT("ItemRefArray")



