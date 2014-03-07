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
/*****************************************************************************

    File: DPATemp.h
    
    Template wrapper for DPA's

    History:
        Created - KK 3-27-1

*/

#pragma once

#include "pcommctr.h"
#include "ehm.h"
#include "hack.h"

/****************************************************************************

 ****************************************************************************/

template<class T> class DPATemplate
{
    HDPA m_hdpa;
    static int DestroyCallback(void* p, void* pData);
public:
    typedef int (CALLBACK *PDPATEMPFndCompare)(const T* p1, const T* p2, LPARAM lParam);

    DPATemplate();
    ~DPATemplate();

    // DPATElement members
    void Reset();
    HRESULT Initialize(int cGrow);
    BOOL Valid() const { return (NULL != m_hdpa); }

    T* GetItemPtr(int i);
    const T* GetItemPtrC(int i) const;
    int GetItemCount() const;
    int SortedInsertPtr(__in __notnull T* pItem, __in __notnull PDPATEMPFndCompare pfnCmp, LPARAM lParam);
    HRESULT Append(__in __notnull T* pItem);
    HRESULT AppendNew(__out T** ppItem, __in __notnull int* piItem);
    HRESULT RemoveItemPtr(int iItem);
    HRESULT DeleteItem(int iItem);
    HRESULT GetItemIndex(__in __notnull const T* pItem, __out int* piItem) const;
    HRESULT Swap(int iA, int iB);
    HRESULT TransferItemTo(int iItem, __out DPATemplate<T>* pDest);
    HRESULT Sort(__in __notnull PDPATEMPFndCompare pfnCmp, LPARAM lParam);
    HRESULT Search(__in __notnull const T* pSearch, PDPATEMPFndCompare pfnCmp, LPARAM lParam, BOOL fSorted, __out int* piItem) const;
};

/****************************************************************************

 ****************************************************************************/
/* static */ template<class T> int DPATemplate<T>::DestroyCallback(void* p, void* pData)
{
    UNREFERENCED_PARAMETER(pData);

    delete ((T*)p);
    return(TRUE);
}

/****************************************************************************

 ****************************************************************************/
template<class T> DPATemplate<T>::DPATemplate() :
    m_hdpa(NULL)
{
    Reset();
}

/****************************************************************************

 ****************************************************************************/
template<class T> DPATemplate<T>::~DPATemplate()
{
    Reset();
}

/****************************************************************************

 ****************************************************************************/
template<class T> void DPATemplate<T>::Reset()
{
    if(m_hdpa)
        {
        DPA_DestroyCallback(m_hdpa, DestroyCallback, NULL);
        m_hdpa = NULL;
        }
}

/****************************************************************************

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::Initialize(int cGrow)
{
    HRESULT hr = S_OK;

    Reset();
    m_hdpa = DPA_Create(cGrow);
    CPR(m_hdpa);

Error:
    return(hr);
}

/****************************************************************************

    Will return NULL if i is out of range...

 ****************************************************************************/
template<class T> T* DPATemplate<T>::GetItemPtr(int i)
{
    ASSERT(i >= 0);
    PF_ASSUME_NOTNULL(m_hdpa);

    return((T*)DPA_GetPtr(m_hdpa, i));
}

/****************************************************************************

    Will return NULL if i is out of range...

 ****************************************************************************/
template<class T> const T* DPATemplate<T>::GetItemPtrC(int i) const
{
    ASSERT(i >= 0);
    PF_ASSUME_NOTNULL(m_hdpa);

    return((const T*)DPA_GetPtr(m_hdpa, i));
}

/****************************************************************************

 ****************************************************************************/
template<class T> int DPATemplate<T>::GetItemCount() const
{
    return(m_hdpa ? DPA_GetPtrCount(m_hdpa) : 0);
}

/****************************************************************************

    pItem must be allocated on the heap.  Also, pItem will now be owned
    by the class.

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::Append(T* pItem)
{
    HRESULT hr = S_OK;
    int iItem;

    PF_ASSUME_NOTNULL(pItem);
    PF_ASSUME_NOTNULL(m_hdpa);
    
    iItem = DPA_AppendPtr(m_hdpa, pItem);
    CBR(iItem >= 0);

Error:
    return(hr);
}


/****************************************************************************

    pItem must be allocated on the heap.  Also, pItem will now be owned
    by the class.

 ****************************************************************************/
template<class T> int DPATemplate<T>::SortedInsertPtr(
    T* pItem,                  // item to insert
    PDPATEMPFndCompare pfnCmp, // in - comparison function
    LPARAM lParam              // in - param to pass to compare function
)
{
    int iItem;

    PF_ASSUME_NOTNULL(pItem);
    PF_ASSUME_NOTNULL(m_hdpa);

    iItem = DPA_SortedInsertPtr(m_hdpa, pItem, 0, (PFNDPACOMPARE)pfnCmp, lParam, DSAS_INSERTAFTER, pItem);
    return(iItem);
}

/****************************************************************************

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::AppendNew(T** ppItem, int* piItem)
{
    HRESULT hr = S_OK;
    int iItemNew = -1;
    T* pItemNew = NULL;

    pItemNew = new T;
    CPR(pItemNew);
    iItemNew = DPA_AppendPtr(m_hdpa, pItemNew);
    CBR(iItemNew >= 0);

    PF_ASSUME_NOTNULL(piItem);
    PF_ASSUME_NOTNULL(ppItem);
    // Don't fail after this line
    
    if(ppItem)
        {
        *ppItem = pItemNew;
        }
    if(piItem)
        {
        *piItem = iItemNew;
        }
    pItemNew = NULL;
   
Error:
    delete(pItemNew);
    return(hr);
}


/****************************************************************************
Removes item from the list without deleting it.

NOTE: This does not delete the item

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::RemoveItemPtr(int iItem)
{
    PF_ASSUME_NOTNULL(m_hdpa);
    ASSERT(iItem >= 0);
    ASSERT(iItem < DPA_GetPtrCount(m_hdpa));

    DPA_DeletePtr(m_hdpa, iItem);

    return(S_OK);
}

/****************************************************************************


/****************************************************************************
Removes item from the list and deletes it
 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::DeleteItem(int iItem)
{
    PF_ASSUME_NOTNULL(m_hdpa);
    ASSERT(iItem >= 0);
    ASSERT(iItem < DPA_GetPtrCount(m_hdpa));

    delete((T*)DPA_DeletePtr(m_hdpa, iItem));

    return(S_OK);
}

/****************************************************************************

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::GetItemIndex(const T* pItem,__out int* piItem) const
{
    PF_ASSUME_NOTNULL(pItem);
    PF_ASSUME_NOTNULL(piItem);
    PF_ASSUME_NOTNULL(m_hdpa);

    *piItem = DPA_GetPtrIndex(m_hdpa, (void*)pItem);
    return((*piItem >= 0) ? S_OK : E_FAIL);
}

/****************************************************************************

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::Swap(int iA, int iB)
{
    HRESULT hr = S_OK;
    
    ASSERT(iA >= 0);
    ASSERT(iB >= 0);
    PF_ASSUME_NOTNULL(m_hdpa);

    void* pA;
    void* pB;

    pA = DPA_GetPtr(m_hdpa, iA);
    CBR(pA);
    pB = DPA_GetPtr(m_hdpa, iB);
    CBR(pB);
    DPA_SetPtr(m_hdpa, iA, pB);
    DPA_SetPtr(m_hdpa, iB, pA);
Error:
    return(S_OK);
}


/****************************************************************************

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::TransferItemTo(int iItemSrc, DPATemplate<T>* pDest)
{
    HRESULT hr = S_OK;
    int iItemDest;
    T* pItemSrc;

    PF_ASSUME_NOTNULL(m_hdpa);
    ASSERT(iItemSrc >= 0);
    PF_ASSUME_NOTNULL(pDest);
    ASSERT(pDest->m_hdpa);

    pItemSrc = (T*)DPA_GetPtr(m_hdpa, iItemSrc);
    CBR(pItemSrc);
    iItemDest = DPA_AppendPtr(pDest->m_hdpa, pItemSrc);
    CPR(iItemDest >= 0);
    VBR(DPA_DeletePtr(m_hdpa, iItemSrc));


Error:
    return(hr);
}

/****************************************************************************

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::Sort(PDPATEMPFndCompare pfnCmp, LPARAM lParam)
{
    HRESULT hr = S_OK;

    PF_ASSUME_NOTNULL(m_hdpa);
    PF_ASSUME_NOTNULL(pfnCmp);

    CWR(DPA_Sort(m_hdpa, (PFNDPACOMPARE)pfnCmp, lParam));

Error:
    return(hr);
}

/****************************************************************************

 ****************************************************************************/
template<class T> HRESULT DPATemplate<T>::Search(
    const T* pSearch, // in - pointer to search for
    PDPATEMPFndCompare pfnCmp, // in - comparison function
    LPARAM lParam, // in - param to pass to compare function
    BOOL fSorted, // in - set if the DPA is already sorted
    int* piItem // out - index in the DPA of the item
    ) const
{
    HRESULT hr = S_OK;

    *piItem = DPA_Search(m_hdpa, (T*)pSearch, 0, (PFNDPACOMPARE)pfnCmp, lParam, fSorted ? DPAS_SORTED : 0);
    CWR(*piItem >= 0);
    
Error:
    return(hr);
}
