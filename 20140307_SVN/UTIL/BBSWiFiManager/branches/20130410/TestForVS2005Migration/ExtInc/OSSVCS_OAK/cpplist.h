/// <company>Microsoft Corporation</company>
/// <copyright>Copyright (c) Microsoft Corporation.  All rights reserved.</copyright>
/// <summary>
///     cpplist.h
///         Thread-safe list class
/// </summary>
/// <remarks>
///     <para>
///         Use of this source code is subject to the terms of the Microsoft
///         premium shared source license agreement under which you licensed
///         this source code. If you did not accept the terms of the license
///         agreement, you are not authorized to use this source code.
///         For the terms of the license, please see the license agreement
///         signed by you and Microsoft.
///         THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
///     </para>
/// </remarks>
/// <history>
///     <record date="1/1/1994">Created</record>
/// </history>

#ifndef __CPPLIST_H__
#define __CPPLIST_H__

#ifdef __cplusplus

// forward declarations
template<class TYPE> class CObject;  
template<class TYPE> class CTList;

/*---------------------------------------------------------------------------*\
 * CProtect: class definition
\*---------------------------------------------------------------------------*/
class CProtect
{
public:
    CProtect(LPCRITICAL_SECTION lpcs) : m_lpcs(lpcs)
    { 
        EnterCriticalSection(m_lpcs); 
    }
    ~CProtect() 
    { 
        LeaveCriticalSection(m_lpcs); 
    }

private:
    LPCRITICAL_SECTION m_lpcs;
    
}; /* CProtect */


/*---------------------------------------------------------------------------*\
 * CObject: class definition
\*---------------------------------------------------------------------------*/
template<class TYPE>
class CObject
{
public:
    CObject() 
    { 
        CObject<TYPE>::Initialize(); 
    };
    virtual ~CObject() 
    { 
        if (m_pOwner != NULL) 
            m_pOwner->Disconnect((TYPE*)this); 
    };

    void Initialize() 
    { 
        m_pPrev = m_pNext = NULL; 
        m_pOwner = NULL; 
    };
    void SetOwner(CTList<TYPE> *pOwner) 
    { 
        m_pOwner = pOwner; 
    }
    CTList<TYPE> *GetOwner(void) 
    { 
        return m_pOwner; 
    }

    TYPE * GetPrev() { return m_pPrev; }
    TYPE * GetNext() { return m_pNext; }

    VOID SetPrev(TYPE * pObject)  { m_pPrev = pObject; };
    VOID SetNext(TYPE * pObject)  { m_pNext = pObject; };

private:
    TYPE *m_pPrev, *m_pNext;

protected:
    CTList<TYPE> *m_pOwner;
    
}; /* CObject */


/*---------------------------------------------------------------------------*\
 * CTList: class definition
\*---------------------------------------------------------------------------*/
template<class TYPE>
class CTList
{
private:
    TYPE * m_pHead, * m_pTail;
    DWORD m_dwCount;
    CRITICAL_SECTION m_cs;
    
public:
    CTList();
    virtual ~CTList();

    LPCRITICAL_SECTION GetCriticalSection() { return &m_cs; }
    void Lock() { EnterCriticalSection(&m_cs); };
    void Unlock() { LeaveCriticalSection(&m_cs); };
    
    BOOL IsEmpty(VOID) { return (0 == m_dwCount); }
    TYPE * IsValid(TYPE * pObject);
    
    TYPE * AddHead(TYPE * pObject);
    TYPE * AddTail(TYPE * pObject);
    TYPE * MoveToHead(TYPE * pObject);
    TYPE * MoveToTail(TYPE * pObject);
    TYPE * Disconnect(TYPE * pObject);

    VOID DisconnectAll(VOID);
    VOID DeleteAll(VOID);
    DWORD GetCount(VOID) { return m_dwCount; }

    TYPE * InsertAfterObject(TYPE * pObject, TYPE * pInsertAfter);
       
    TYPE * Head() { return m_pHead; }
    TYPE * Tail() { return m_pTail; }

    int GetObjectIndex(TYPE * pObject);
    TYPE *GetAtIndex(int iIndex);
    TYPE *DisconnectAtIndex(int iIndex);
    TYPE *InsertAfterIndex(TYPE * pObject, int iIndex);

}; /* CTList */


template<class TYPE>
CTList<TYPE>::CTList()
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    m_pHead = NULL;
    m_pTail = NULL;
    m_dwCount = 0;

    InitializeCriticalSection(GetCriticalSection());
    
} /* CTList()
   */


template<class TYPE>
CTList<TYPE>::~CTList()
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    /* this actually deletes all items in the list.  Beware all who hold 
     * pointers to items in the list!
     */  
    DeleteAll();
    DeleteCriticalSection(GetCriticalSection());

} /* ~CTList()
   */


template<class TYPE>
TYPE * CTList<TYPE>::
    IsValid(TYPE * pObject)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());
    
    /* all routines should check for null, thus making it a valid
     * value for pointer to object, what this routine actually checks
     * is the validity that this pointer still belongs in this list.
     * and it does a pretty bogus job of that after all.
     */
    if (pObject == NULL) {
        return NULL;
    }

    if(pObject->GetOwner() != this) {
        return NULL;
    }

    TYPE * ptr = Head();
    
    while (ptr != NULL) {
        if (pObject == ptr) {
            return ptr;
        }
        ptr = ptr->GetNext();
    }

    return NULL;   
        
} /* IsValid()
   */


template<class TYPE>
TYPE *
  CTList<TYPE>::AddHead(TYPE * pObject)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    if (pObject == NULL) {
        return NULL;
    }

    pObject->SetOwner(this);
  
    if (IsEmpty()) {
        m_pHead = pObject;
        m_pTail = pObject;

    }else{
        pObject->SetNext(m_pHead);
        m_pHead->SetPrev(pObject);
        m_pHead = pObject;
    }

    m_dwCount++;
    return pObject;
    
} /* AddHead()
   */


template<class TYPE>
TYPE *
  CTList<TYPE>::AddTail(TYPE * pObject)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    if (pObject == NULL) {
        return NULL;
    }

    pObject->SetOwner(this);
  
    if (IsEmpty()) {
        m_pHead = pObject;
        m_pTail = pObject;

    }else{
        m_pTail->SetNext(pObject);
        pObject->SetPrev(m_pTail);
        m_pTail = pObject;
    }

    m_dwCount++;
    return pObject;
    
} /* AddTail()
   */


template<class TYPE>
TYPE *
  CTList<TYPE>::Disconnect(TYPE * pObject)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    TYPE * pPrev, * pNext; 

    m_dwCount--;

    if (IsEmpty()) {
        m_pHead = NULL;
        m_pTail = NULL; 

    }else{
        pPrev = pObject->GetPrev(); 
        pNext = pObject->GetNext();
     
        if (pPrev == NULL) {
            pNext->SetPrev(NULL);
            m_pHead = pNext;

        }else if (pNext == NULL) {
            pPrev->SetNext(pNext);
            m_pTail = pPrev;

        }else{
            pNext->SetPrev(pPrev);
            pPrev->SetNext(pNext);

        }
    }

    pObject->CObject<TYPE>::Initialize();
    return pObject;

} /* Disconnect()
   */


template<class TYPE>
TYPE *
  CTList<TYPE>::MoveToHead(TYPE * pObject)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    if (Disconnect(pObject)) {
        return AddHead(pObject);
    }

    return NULL;

} /* MoveToHead()
   */


template<class TYPE>
TYPE *
  CTList<TYPE>::MoveToTail(TYPE * pObject)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    if (Disconnect(pObject)) {
        return AddTail(pObject);
    }

    return NULL;

} /* MoveToTail()
   */


template<class TYPE>
VOID
  CTList<TYPE>::DisconnectAll(void)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    TYPE * pObject;

    while(pObject = Head()) {
        Disconnect(pObject);
    }
 
} /* DisconnectAll()
   */


template<class TYPE>
VOID
  CTList<TYPE>::DeleteAll(void)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    TYPE * pObject;

    while ((pObject = Head()) != NULL) {
        delete pObject;
    }
 
} /* DeleteAll()
   */


template<class TYPE>
TYPE *
    CTList<TYPE>::InsertAfterObject(TYPE * pObject, TYPE * pInsertAfter)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

#ifdef _DEBUG_  
    /* some quick error checking
     */
    if (IsValid(pInsertAfter) == FALSE) {
        return NULL;
    }
#endif
  
    if (pObject == NULL) {
        return NULL;
    }

    if (pInsertAfter == NULL) {
        return AddHead(pObject);

    }else if (pInsertAfter->GetNext() == NULL) {
        return AddTail(pObject);

    }else{
        TYPE * pInsertBefore = pInsertAfter->GetNext();
    
        pObject->SetPrev(pInsertAfter);
        pObject->SetNext(pInsertAfter->GetNext());
        pObject->SetOwner(this);

        pInsertAfter->SetNext(pObject);
        if (pInsertBefore != NULL) {
            pInsertBefore->SetPrev(pObject);
        }

        m_dwCount++;
    }

    return pObject;
      
} /* InsertAfterObject()
   */

template<class TYPE>
int
CTList<TYPE>::GetObjectIndex(TYPE * pObject)
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());

    if (pObject == NULL) return -1;

    int i=0;
    for (TYPE * p = Head(); p!=NULL,p!=pObject; p=p->GetNext(), i++);

    return i;

} /* CTList<TYPE>::GetObjectIndex() 
   */

template<class TYPE>
TYPE *
CTList<TYPE>::GetAtIndex(int iIndex) 
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());
    if (iIndex < 0 || iIndex > (int)m_dwCount) return NULL;
    int i=0;

    for (TYPE * p = Head(); p!=NULL,i < iIndex; p=p->GetNext(), i++);

    return p;
    
} /* CTList<TYPE>::GetAtIndex() 
   */


template<class TYPE>
TYPE *
CTList<TYPE>::DisconnectAtIndex(int iIndex) 
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());
    TYPE * p;

    if (p = GetAtIndex(iIndex)) {
        p = Disconnect(p);
    }
    return p;
    
} /* CTList<TYPE>::DisconnectAtIndex() 
   */

template<class TYPE>
TYPE *
CTList<TYPE>::InsertAfterIndex(TYPE * pObject, int iIndex) 
/*---------------------------------------------------------------------------*\
 * 
\*---------------------------------------------------------------------------*/
{
    CProtect protect(GetCriticalSection());
    TYPE * pPrev = GetAtIndex(iIndex);

    return InsertAfterObject(pObject, pPrev);
    
} /* CTList<TYPE>::InsertAfterIndex() 
   */



#endif // __cplusplus

#endif // __CPPLIST_H__
