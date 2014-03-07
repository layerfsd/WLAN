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
#ifndef __SYNC__
#define __SYNC__


#if 0
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 1. Using critical section and gate
/**/    
/**/    ce::critical_section cs;
/**/    
/**/    void foo()
/**/    {
/**/        // Enter critical section. This will block if critical section
/**/        // is owned by another thread. 
/**/        // The critical section is automatically released when gate 
/**/        // goes out of scope (in this case when the function foo returns).
/**/        ce::gate<ce::critical_section>  gate(cs);
/**/
/**/        // operations that require critical section
/**/        bar();
/**/    } 
/**/
/**/    void bar()
/**/    {
/**/        // bar expects to be locked. Will throw an assert if lock not taken
/**/        cs.assert_locked();
/**/        
/**/    }
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 2. Using mutex
/**/
/**/    ce::mutex m;
/**/    
/**/    main()
/**/    {
/**/        // Create a named mutex
/**/        m.create(L"MutexName");
/**/
/**/        // ...
/**/    }
/**/
/**/    void foo()
/**/    {
/**/        // Obtain ownership of mutex. This will block if mutex
/**/        // is owned by another thread. 
/**/        // The mutex is automatically released when gate 
/**/        // goes out of scope (in this case when the function foo returns).
/**/        ce::gate<ce::mutext>  gate(m);
/**/
/**/        // operations that require mutex ownership
/**/    }
/**/    
/**/    ///////////////////////////////////////////////////////
/**/    // 2. Using semaphore
/**/
/**/    ce::semaphore s;
/**/    
/**/    main()
/**/    {
/**/        // Create an unnamed semaphore
/**/        s.create(10);
/**/
/**/        // ...
/**/    }
/**/
/**/    void foo()
/**/    {
/**/        // Wait until semaphore count is available. This will block if semaphore
/**/        // count is 0. 
/**/        // The semaphore is automatically released (count incremented by 1) when gate 
/**/        // goes out of scope (in this case when the function foo returns).
/**/        ce::gate<ce::semaphore>  gate(s);
/**/    }
/**/    
/**/
#endif


#include <windows.h>
#include <assert.h>

namespace ce
{

namespace detail
{

    // In non-debug builds, (when assert is removed) the compiler generates a
    // C4100 "unreferenced parameter" warning, because cs isn't used in the
    // body of the function. Disable that warning around the definition of the
    // function so that this header file compiles cleanly at /W4.
#pragma warning(push)
#pragma warning(disable:4100)
inline void assert_locked(const CRITICAL_SECTION* cs)
{
#ifdef UNDER_CE 
        assert(cs->OwnerThread == reinterpret_cast<HANDLE>(GetCurrentThreadId()));
#else
#   pragma warning(push)
#   pragma warning(disable : 4312)
        assert(cs->OwningThread == reinterpret_cast<HANDLE>(GetCurrentThreadId()));
#   pragma warning(pop)        
#endif 
}
#pragma warning(pop)
}


// critical_section
class critical_section : CRITICAL_SECTION
{
    void operator=(const critical_section&);
    critical_section(const critical_section&);
public:
    critical_section()
        {InitializeCriticalSection(this); }

    ~critical_section()
        {DeleteCriticalSection(this); }

    void lock()
        {EnterCriticalSection(this); }
        
#if (defined UNDER_CE) || (_WIN32_WINNT >= 0x0400)
    bool try_lock()
        {return FALSE != TryEnterCriticalSection(this); }
#endif        

    void unlock()
        {LeaveCriticalSection(this); }

    void assert_locked() const
    {
        detail::assert_locked(this);
    }
};


// critical_section_with_copy
// 
// Objects of this class can be copied, so for example they
// can be used in collections. Before using it, make sure that
// you want the semantics you get, the copy will manage
// a completely  new critical section.
class critical_section_with_copy : CRITICAL_SECTION
{
    void operator=(const critical_section_with_copy&);
public:
    critical_section_with_copy()
        {InitializeCriticalSection(this); }

    critical_section_with_copy(const critical_section_with_copy&)
        {InitializeCriticalSection(this); }

    ~critical_section_with_copy()
        {DeleteCriticalSection(this); }

    void lock()
        {EnterCriticalSection(this); }
        
#if (defined UNDER_CE) || (_WIN32_WINNT >= 0x0400)
    bool try_lock()
        {return FALSE != TryEnterCriticalSection(this); }
#endif        

    void unlock()
        {LeaveCriticalSection(this); }

    void assert_locked() const
    {
        detail::assert_locked(this);
    }
};


// gate
template <class T>
class gate
{
    gate<T>& operator=(const gate<T>&);
public:
    gate(T& SyncObject)
        : m_pSyncObject(&SyncObject)
        {m_pSyncObject->lock(); }
    gate(T *pSyncObject)
        : m_pSyncObject(pSyncObject)
        { if (m_pSyncObject) m_pSyncObject->lock(); }

    ~gate()
    {
        // Do not call leave from dtor so that compiler can better optimize code
        if(m_pSyncObject)
            m_pSyncObject->unlock();
    }

    void leave()
    {
        if(m_pSyncObject)
        {
            m_pSyncObject->unlock();

            m_pSyncObject = NULL;
        }
    }

private:
    T*      m_pSyncObject;
};

}; // namespace ce

#endif // __SYNC__


#ifndef __SYNC2__
#define __SYNC2__

#pragma warning(push)

#include "auto_xxx.hxx"

namespace ce
{

// mutex
class mutex
{
public:
    bool create(LPCTSTR lpName = NULL, BOOL bInitialOwner = FALSE, LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL)
    {
        h = CreateMutex(lpMutexAttributes, bInitialOwner, lpName);
        
        return FALSE != h.valid();
    }
    
    bool open(LPCTSTR lpName = NULL, DWORD dwDesiredAccess = MUTANT_ALL_ACCESS, BOOL bInheritHandle = FALSE)
    {
#ifdef UNDER_CE
        UNREFERENCED_PARAMETER(dwDesiredAccess); // supresses  warning  in /W4
        UNREFERENCED_PARAMETER(bInheritHandle); // supresses  warning  in /W4
        h = CreateMutex(NULL, FALSE, lpName); 
        
        if(h.valid() && ERROR_ALREADY_EXISTS != GetLastError())
            h.close();
#else        
        h = OpenMutex(dwDesiredAccess, bInheritHandle, lpName);
#endif        
        
        return FALSE != h.valid();
    }

    void lock()
    {
        assert(h.valid());
        
        WaitForSingleObject(h, INFINITE);
    }
        
    bool try_lock(DWORD dwMilliseconds)
    {
        assert(h.valid());
        
        return WAIT_TIMEOUT != WaitForSingleObject(h, dwMilliseconds);
    }

    void unlock()
    {
        assert(h.valid());
        
        ReleaseMutex(h);
    }
        
private:
    auto_handle h;
};


// semaphore
class semaphore
{
public:
    bool create(LONG lMaximumCount, LONG lInitialCount = 0, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = NULL)
    {
        h = CreateSemaphore(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName); 
        
        return FALSE != h.valid();
    }

    bool open(LPCTSTR lpName, DWORD dwDesiredAccess = SEMAPHORE_ALL_ACCESS, BOOL bInheritHandle = FALSE)
    {
#ifdef UNDER_CE
        UNREFERENCED_PARAMETER(dwDesiredAccess); // supresses  warning  in /W4
        UNREFERENCED_PARAMETER(bInheritHandle); // supresses  warning  in /W4
        h = CreateSemaphore(NULL, 0, 0, lpName); 
        
        if(h.valid() && ERROR_ALREADY_EXISTS != GetLastError())
            h.close();
#else
        h = OpenSemaphore(dwDesiredAccess, bInheritHandle, lpName);
#endif        
        
        return FALSE != h.valid();
    }
    
    void lock()
    {
        assert(h.valid());
        
        WaitForSingleObject(h, INFINITE);
    }
        
    bool try_lock(DWORD dwMilliseconds)
    {
        assert(h.valid());
        
        return WAIT_TIMEOUT != WaitForSingleObject(h, dwMilliseconds); 
    }

    void unlock()
    {
        assert(h.valid());
        
        ReleaseSemaphore(h, 1, NULL);
    }
    
    bool release(LONG lReleaseCount, LPLONG lpPreviousCount = NULL)
    {
        assert(h.valid());
        
        return TRUE == ReleaseSemaphore(h, lReleaseCount, lpPreviousCount);
    }
        
private:
    auto_handle h;
};

}; // namespace ce

#pragma warning(push)

#endif // __SYNC2__
