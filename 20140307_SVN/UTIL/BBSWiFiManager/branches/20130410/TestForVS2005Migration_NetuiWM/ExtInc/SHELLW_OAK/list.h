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
/*---------------------------------------------------------------------------*\
 *
 * (c) Copyright Microsoft Corp. 1994 All Rights Reserved
 *
 *  module: list.h
 *  date:
 *  author: tonykit
 *
 *  purpose: 
 *
\*---------------------------------------------------------------------------*/
#ifndef __LIST_H__
#define __LIST_H__

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus
extern "C" {
#endif

	
// Internal List Structures

typedef struct tagLISTOBJ {
	struct tagLISTOBJ *lpprev;
	struct tagLISTOBJ *lpnext;
} LISTOBJ, FAR* LPLISTOBJ;

typedef struct tagLIST {
    struct tagLISTOBJ *lphead;
	struct tagLISTOBJ *lptail;
	LONG ncount;
	CRITICAL_SECTION cs;
} LIST, FAR* LPLIST;


// List API's

LPLIST    List_Create(VOID);
VOID      List_Delete(LPLIST plist);
VOID      List_Lock(LPLIST plist, BOOL fLock);
LPLISTOBJ List_IsObjectValid(LPLIST plist, LPLISTOBJ pobject);
LPLISTOBJ List_AddHead(LPLIST plist, LPLISTOBJ pobject);
LPLISTOBJ List_AddTail(LPLIST plist, LPLISTOBJ pobject);
LPLISTOBJ List_Disconnect(LPLIST plist, LPLISTOBJ pobject);
LPLISTOBJ List_MoveToHead(LPLIST plist, LPLISTOBJ pobject);
LPLISTOBJ List_MoveToTail(LPLIST plist, LPLISTOBJ pobject);
LONG      List_GetCount(LPLIST plist);
LPLISTOBJ List_Head(LPLIST plist);
LPLISTOBJ List_Tail(LPLIST plist);
LPLISTOBJ List_Next(LPLIST plist, LPLISTOBJ pobject);
LPLISTOBJ List_Prev(LPLIST plist, LPLISTOBJ pobject);
LONG      List_GetObjectIndex(LPLIST plist, LPLISTOBJ pobject);
LPLISTOBJ List_GetAtIndex(LPLIST plist, LONG iIndex);
LPLISTOBJ List_DisconnectAtIndex(LPLIST plist, LONG iIndex);
LPLISTOBJ List_InsertBeforeIndex(LPLIST plist, LPLISTOBJ pobject, LONG iIndex);
LPLISTOBJ List_InsertAfterIndex(LPLIST plist, LPLISTOBJ pobject, LONG iIndex);
LPLISTOBJ List_InsertAfterObject(LPLIST plist, LPLISTOBJ pobject,
								 LPLISTOBJ pobjectAfter);
BOOL      List_Swap(LPLIST plist, LPLISTOBJ pobject1, LPLISTOBJ pobject2);


/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus
}
#endif

#endif /* __LIST_H__ */
