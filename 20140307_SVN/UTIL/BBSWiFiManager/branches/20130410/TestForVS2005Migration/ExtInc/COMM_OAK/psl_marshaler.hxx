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

#include <windows.h>
#include "auto_xxx.hxx"
#include "hash_map.hxx"
#include "singleton.hxx"
#include "sync.hxx"
#include "string.hxx"

#ifndef __PSL_MARSHALER__
#define __PSL_MARSHALER__

//
// Set of templates implementing generic and extensible proxy and stub for 
// PSL calls in Windows CE. 
//

#if 0
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 1. Implementing "remote" API using psl_proxy and psl_stub
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    #define XYZ_IOCTL_INVOKE    100
/**/    #define FOO                 1
/**/    #define BAR                 2
/**/
/**/    //***************
/**/    // Client process
/**/    //***************
/**/
/**/    //
/**/    // psl_proxy object for server hosted in device/service XYZ1:
/**/    // Lifetime of the object typically should be equal to the lifetime of the client.
/**/    // When the psl_proxy object is destroyed it will close device/service handle 
/**/    // (which potentially will clean client's context on the device/service).
/**/    // Usually psl_proxy object will be declared as global or singleton.
/**/    //
/**/    ce::psl_proxy<> proxy(L"XYZ1:", XYZ_IOCTL_INVOKE, NULL);
/**/
/**/    DWORD Foo(DWORD* pdwSize, VOID* pszBuffer)
/**/    {
/**/        return proxy.call(FOO, pdwSize, pszBuffer);
/**/    }
/**/    
/**/    BOOL Bar(DWORD dwCount, const Record* pRecords)
/**/    {
/**/        // proxy.call returns DWORD error even for functions that return TRUE or FALSE
/**/        // The error code is also available to the caller via GetLastError
/**/        return ERROR_SUCCESS == proxy.call(BAR, dwCount, pRecords);
/**/    }
/**/    
/**/
/**/    //***************
/**/    // Server process
/**/    //***************
/**/    
/**/    // Server side implementation of Foo API.
/**/    // Note that the name of the server side functions may be but don't have to be 
/**/    // the same as name of appropriate client side functions. Signatures of the
/**/    // functions have to be the same on server and client side.
/**/    
/**/    // Server function that returns DWORD indicating success (ERROR_SUCCESS) or failure
/**/    DWORD Foo(DWORD* pdwSize, VOID* pszBuffer)
/**/    {
/**/        ...
/**/    }
/**/    
/**/    // Server function that returns TRUE or FALSE and sets last error in case of failure
/**/    BOOL Bar(DWORD dwCount, const Record* pRecords)
/**/    {
/**/        ...
/**/    }
/**/
/**/    // IOControl function for driver/service that is hosting the server
/**/    BOOL XYZ_IOControl(..., DWORD dwCode, IN PBYTE pBufIn, IN DWORD dwLenIn, ...)
/**/    {
/**/        DWORD dwErr = ERROR_INVALID_PARAMETER;
/**/
/**/        switch(dwCode)
/**/        {
/**/            case XYZ_IOCTL_INVOKE:
/**/            {
/**/                //
/**/                // Initialize stub object from input buffer and validate that input buffer
/**/                // is valid in the context of the calling process.
/**/                //
/**/                ce::psl_stub<> stub(pBufIn, dwLenIn);
/**/                
/**/                // Get ID of API that is being called; will return ce::INVALID_FUNCTION_ID if the
/**/                // validation of the input buffer failed. 
/**/                switch(stub.function())
/**/                {
/**/                    //
/**/                    // Call appropriate API implementation based on the ID.
/**/                    // psl_stub will extract arguments from the input buffer and call the API implementation.
/**/                    // For any arguments that are pointers, psl_stub will validate that the memory
/**/                    // region they are pointing to is valid in context of the client process (by default caller
/**/                    // process is used; handle of another process can be passed as 3rd argument to psl_stub ctor). 
/**/                    // psl_stub will then map the pointer so that it can be accessed by the server process.
/**/                    // psl_stub wraps call to the API implementation in __try{}__except block. If exception 
/**/                    // happens during execution of the API call ERROR_EXCEPTION_IN_SERVICE error is returned 
/**/                    // to the client. Note that exception handler in psl_stub is a last resort. Usually server 
/**/                    // implementation should locally handle exceptions when accessing memory from the client
/**/                    // to avoid leaks.
/**/                    //
/**/                    // IMPORTANT: If an argument points to an array of items or otherwise to a region 
/**/                    //            larger than the type of pointer would indicate, it is responsibility
/**/                    //            of the server to call MapCallerPtr on the pointer with appropriate
/**/                    //            size. For example, for a LPCWSTR argument, psl_stub will only validate
/**/                    //            that the calling process has access to sizeof(wchar_t) bytes.
/**/                    //            If the size of the buffer is passed in the call then psl_buffer_wrapper
/**/                    //            can be used to guarantee proper mapping of the whole buffer (see #5 below).
/**/                    //
/**/                    case FOO:       dwErr = stub.call(Foo);
/**/                                    break;
/**/
/**/                                    // Bar returns TRUE or FALSE and sets last error in case of failure.
/**/                    case BAR:       return stub.call(Bar);
/**/                }
/**/                break;
/**/            }
/**/        }
/**/        
/**/        if(ERROR_SUCCESS == dwErr)
/**/            return TRUE;
/**/        else
/**/        {
/**/            SetLastError(dwErr);
/**/            return FALSE;
/**/        }    
/**/    }
/**/
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 2. Implementing server in a class
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/
/**/    class CServer
/**/    {
/**/    public:
/**/        DWORD Foo(DWORD* pdwSize, VOID* pszBuffer);
/**/        BOOL Bar(DWORD dwCount, const Record* pRecords);
/**/    }*g_pServer, g_Server;
/**/
/**/    // ...
/**/                switch(stub.function())
/**/                {
/**/                    case FOO:       dwErr = stub.call(g_pServer, CServer::Foo);
/**/                                    break;
/**/
/**/                    case BAR:       return stub.call(g_Server, CServer::Bar);
/**/                }
/**/                break;
/**/    // ...
/**/
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 3. Tracking per-client state on the server 
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    // Server often needs to track per-client state so that it can be cleaned if the client
/**/    // process is terminated w/o proper shutdown. Typically server will associate client 
/**/    // state with dwOpenData argument of DeviceIoControl.
/**/    // 
/**/    // This scheme can be implemented with psl marshaler classes in a following 
/**/    // manner (modification to the server side code from example #1):
/**/
/**/
/**/    // Intermediate function that takes additional dwOpenData argument in addition to
/**/    // the parameters of the Bar function.
/**/    BOOL _Bar(DWORD dwOpenData, DWORD dwCount, const Record* pRecords)
/**/    {
/**/        // Map dwOpenData back to the client state; in this example dwOpenData contains a pointer
/**/        // to a ClientState object representing the client's state. 
/**/        ClientState* pClientState = reinterpret_cast<ClientState*>(dwOpenData);
/**/
/**/        // Call actual server implementation of Bar
/**/        BOOL bRet = Bar(dwCount, pRecords);
/**/
/**/        // Update client state in pClientState as appropriate for Bar
/**/
/**/        return bRet;
/**/    }
/**/
/**/    BOOL XYZ_Close(DWORD dwOpenData)
/**/    {
/**/        // Map dwOpenData back to the client state; in this example dwOpenData contains a pointer
/**/        // to a ClientState object representing the client's state. Value of dwOpenData
/**/        // for a client is established in XYZ_Open. 
/**/        ClientState* pClientState = reinterpret_cast<ClientState*>(dwOpenData);
/**/        
/**/        // Cleanup any outstanding state of the client in pClientState
/**/        
/**/        return TRUE;
/**/    }
/**/
/**/    BOOL XYZ_IOControl(DWORD dwOpenData, DWORD dwCode, IN PBYTE pBufIn, IN DWORD dwLenIn, ...)
/**/    {
/**/        switch(dwCode)
/**/        {
/**/            case XYZ_IOCTL_INVOKE:
/**/            {
/**/                ce::psl_stub<> stub(pBufIn, dwLenIn);
/**/                
/**/                switch(stub.function())
/**/                {
/**/                    // dwOpenData can be passed to some (or all) server functions as an extra argument 
/**/                    case BAR:   return stub.call(_Bar, dwOpenData);
/**/
/**/                    // ...
/**/                }
/**/                break;
/**/            }
/**/        }
/**/
/**/        // ...
/**/    }
/**/
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 4. Passing handle via PSL calls 
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    //
/**/    // Handles require special handling when used as arguments to calls made with psl marshaler.  
/**/    // Handles are declared as pointers and they would be mapped by psl_stub. Typically this is 
/**/    // not desired because handles must be treated as opaque values. To prevent treating handles  
/**/    // as regular pointer one needs to provide specialization of psl_data_wrapper class template
/**/    // for the handle type.
/**/    //
/**/    // IMPORTANT: Handles often are declared as simple typedef of void*. Such a handle can't be used
/**/    //            for specialization of psl_data_wrapper because it would result in all void* arguments
/**/    //            not being mapped properly by the server's psl_stub. Only handles declared using
/**/    //            DECLARE_HANDLE macro should be used to specialize psl_data_wrapper.
/**/    //
/**/    // psl_marshaler provides a predefined ce::PSL_HANDLE type and appropriate specialization of 
/**/    // psl_data_wrapper class template:
/**/    //
/**/
/**/    // Server
/**/    DWORD Foo(ce::PSL_HANDLE hpslEvent)
/**/    {
/**/        HANDLE hEvent = reinterpret_cast<HANDLE>(hpslEvent);
/**/    }
/**/
/**/    // Client
/**/    DWORD Foo(HANDLE hEvent)
/**/    {
/**/        return proxy.call(FOO, reinterpret_cast<ce::PSL_HANDLE>(hEvent));
/**/    }
/**/
/**/    // Alternatively user can declare their own handle(s) using DECALRE_HANDLE and specialize
/**/    // psl_data_wrapper class template for it:
/**/
/**/    DECLARE_HANDLE(USER_HANDLE);
/**/
/**/    template<>
/**/    class ce::psl_data_wrapper<USER_HANDLE>
/**/    {
/**/    public:
/**/        void marshal(USER_HANDLE x)
/**/            {data = x; }
/**/    
/**/        USER_HANDLE unmarshal(HANDLE)
/**/            {return data; }
/**/
/**/    private:
/**/        USER_HANDLE data;
/**/    };
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 5. Passing sized buffer 
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    // Server
/**/    DWORD Foo(ce::psl_buffer_wrapper<const int*> numbers)
/**/    {
/**/        return FooImpl(numbers.buffer(), numbers.count());
/**/    }
/**/
/**/    DWORD FooImpl(const int *pNumbers, size_t nCount)
/**/    {
/**/        // buffer pNumbers is valid for nCount elements of type int
/**/    }
/**/
/**/    // Client
/**/    DWORD Foo(const int *pNumbers, size_t nCount)
/**/    {
/**/        return proxy.call(FOO, ce::psl_buffer(pNumbers, nCount));
/**/    }
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 6. Structures with embedded pointers
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    // Structure with embedded pointers
/**/    struct Record
/**/    {
/**/        LPCWSTR pszName;
/**/        LPCWSTR pszValue;
/**/    };
/**/    
/**/    // Following specialization of psl_data_wrapper class template is required if one or more 
/**/    // functions called via psl marshaler have parameter(s) of const Record* type.
/**/    //
/**/    // IMPORTANT: The specialization must be for exact type that is being used as a function argument.
/**/    //            For example in this case it is for const Record* because this is type of argument
/**/    //            of the function Bar. 
/**/    
/**/    template<>
/**/    class ce::psl_data_wrapper<const Record*>
/**/    {
/**/    public:
/**/        void marshal(const Record* pRecord)
/**/        {
/**/            if(pRecord)
/**/            {
/**/                m_Record = *pRecord;
/**/                m_bNull = false;
/**/            }
/**/            else
/**/                m_bNull = true;
/**/        }
/**/        
/**/        const Record* unmarshal(HANDLE hClientProcess)
/**/        {
/**/            if(!m_bNull)
/**/            {
/**/                m_Record.pszName = (LPWSTR)MapPtrToProcWithSize((LPVOID)m_Record.pszName, sizeof(*m_Record.pszName), hClientProcess);
/**/                m_Record.pszValue = (LPWSTR)MapPtrToProcWithSize((LPVOID)m_Record.pszValue, sizeof(*m_Record.pszValue), hClientProcess);
/**/
/**/                return &m_Record;
/**/            }
/**/            else
/**/                return NULL;
/**/        }
/**/
/**/    private:
/**/        Record m_Record;
/**/        bool   m_bNull; 
/**/    };
/**/
/**/
#endif

namespace ce
{

enum {INVALID_FUNCTION_ID = 0xFFFF};

//
// data wrapper for "simple" types
// 
template<typename T>
class psl_data_wrapper
{
public:
    void marshal(T x)
        {data = x; }
    
    T unmarshal(HANDLE)
        {return data; }

protected:
    T data;
};

namespace detail
{

//
// version of sizeof that returns 1 for type void
//
template<typename T>
struct size_of
{
    enum {value = sizeof(T)};
};

template<>
struct size_of<void>
{
    enum {value = 1};
};

template<>
struct size_of<const void>
{
    enum {value = 1};
};


//
// class template removes constness from type T
//
template <typename T>
struct remove_const
{
    typedef T type;
};

template <typename T>
struct remove_const<const T>
{
    typedef T type;
};

}; // namespace detail

//
// partial specialization of data wrapper for pointers
// 
template<typename T>
class psl_data_wrapper<T*>
{
public:
    void marshal(T* x)
        {data = x; }
    
    T* unmarshal(HANDLE hClientProcess)
        {return unmarshal_pointer(hClientProcess, detail::size_of<T>::value); }
    
protected:
    T* unmarshal_pointer(HANDLE hClientProcess, int nSize)
    {
        //
        // Check whether the memory region is valid in context of the client process
        // and map the pointer for access from the server process.
        //
        return (T*)MapPtrToProcWithSize((LPVOID)data, nSize, hClientProcess);
    } 

protected:
    T* data;
};

DECLARE_HANDLE(PSL_HANDLE);

//
// data wrapper specialization for PSL_HANDLE
//
template<>
class psl_data_wrapper<PSL_HANDLE>
{
public:
    void marshal(PSL_HANDLE x)
        {data = x; }
    
    PSL_HANDLE unmarshal(HANDLE)
        {return data; }

private:
    PSL_HANDLE data;
};


//
// wrapper for sized buffer
//
template<typename T>
class psl_buffer_wrapper
{
public:
    typedef T buffer_type;
public:
    psl_buffer_wrapper()
        : _buffer(NULL),
          _count(0)
        {}
    
    psl_buffer_wrapper(T buffer, size_t count)
        : _buffer(buffer),
          _count(buffer ? count : 0)
        {}

    T buffer()
        {return _buffer; }
    
    size_t count()
        {return _count; }
    
protected:
    T       _buffer;
    size_t  _count;
};


//
// creates a wrapped buffer from pointer and count
//
template<typename T>
psl_buffer_wrapper<T*> psl_buffer(T* buffer, size_t count)
{
    return psl_buffer_wrapper<T*>(buffer, count);
}


//
// data wrapper specialization for sized buffers
//
template<typename T>
class psl_data_wrapper<psl_buffer_wrapper<T*> >
{
    typedef psl_buffer_wrapper<T*> buffer_wrapper;
    
public:
    void marshal(buffer_wrapper x)
        {data = x; }
    
    buffer_wrapper unmarshal(HANDLE hClientProcess)
        {return unmarshal_buffer(hClientProcess, detail::size_of<T>::value); }
    
protected:
    buffer_wrapper unmarshal_buffer(HANDLE hClientProcess, int nSize)
    {
        //
        // Check whether the memory region is valid in context of the client process
        // and map the pointer for access from the server process.
        //
        T* buffer = (T*)MapPtrToProcWithSize((LPVOID)data.buffer(), (DWORD)(data.count() * nSize), hClientProcess);
        
        return buffer_wrapper(buffer, buffer ? data.count() : 0);
    } 

protected:
    buffer_wrapper data;
};


// base class for all copy policies
class copy_traits_base
{
protected:
    copy_traits_base(HANDLE hClientProcess)
        : m_hClientProcess(hClientProcess)
    {
    }
    
protected:
    HANDLE m_hClientProcess;    
};


// base class for pointer specializations of copy policies
template <typename T>
class ptr_copy_traits_base
    : public copy_traits_base
{
protected:
    // ctor
    ptr_copy_traits_base(HANDLE hClientProcess)
        : m_pServerPtr(NULL),
          copy_traits_base(hClientProcess)
    {}

public:
    // pFoo->member
    T* operator->()
    {
        return m_pServerPtr;
    }

    // if (!pFoo)
    bool operator!() const
    {
        return !m_pServerPtr;
    }

    // implicit cast to T*
    operator T*() 
    {
        return m_pServerPtr;
    }
    
protected:    
    T* m_pServerPtr;
};


//
// Policy to copy [in] arguments before function call
//
template <typename T>
class copy_in;

// Specialization of copy_in for pointers
template <typename T>
class copy_in<T*>
    : public ptr_copy_traits_base<T>
{
public:
    typedef ptr_copy_traits_base<T> _Mybase;
    
    copy_in(HANDLE hClientProcess)
        : _Mybase(hClientProcess)
    {
    }
    
    void copy_arg_in(T* pClientPtr)
    {
        assert(m_pServerPtr == NULL);
        
        if(pClientPtr)
        {
            m_LocalCopy = *pClientPtr;
            m_pServerPtr = &m_LocalCopy;
        }
    }
    
    void copy_arg_out()
    {
    }

private:
    // removes const from type T
    typedef typename detail::remove_const<T>::type mutable_T;

    mutable_T   m_LocalCopy;
};


//
// Specialization of copy_in for const char*
// Argument of this type is treated as null terminated string
//
template <>
class copy_in<const char*>
    : public ptr_copy_traits_base<const char>
{
public:
    typedef ptr_copy_traits_base<const char> _Mybase;
    
    copy_in(HANDLE hClientProcess)
        : _Mybase(hClientProcess)
    {
    }
    
    void copy_arg_in(const char* pClientPtr)
    {
        assert(m_pServerPtr == NULL);
        
        if(pClientPtr)
        {
            m_strLocalCopy = pClientPtr;
            m_pServerPtr = m_strLocalCopy;
        }
    }
    
    void copy_arg_out()
    {
    }

private:
    // TODO: specify arena allocator for string
    _string_t<char> m_strLocalCopy;
};


//
// Specialization of copy_in for const wchar_t*
// Argument of this type is treated as null terminated string
//
template <>
class copy_in<const wchar_t*>
    : public ptr_copy_traits_base<const wchar_t>
{
public:
    typedef ptr_copy_traits_base<const wchar_t> _Mybase;
    
    copy_in(HANDLE hClientProcess)
        : _Mybase(hClientProcess)
    {
    }
    
    void copy_arg_in(const wchar_t* pClientPtr)
    {
        assert(m_pServerPtr == NULL);
        
        if(pClientPtr)
        {
            m_strLocalCopy = pClientPtr;
            m_pServerPtr = m_strLocalCopy;
        }
    }
    
    void copy_arg_out()
    {
    }

private:
    // TODO: specify arena allocator for string
    _string_t<wchar_t> m_strLocalCopy;
};


//
// Specialization of copy_in for sized buffers
//
template <typename T>
class copy_in<psl_buffer_wrapper<T*> >
    : public copy_traits_base,
      public psl_buffer_wrapper<T*>
{
public:
    typedef copy_traits_base _Mybase;
    
    copy_in(HANDLE hClientProcess)
        : _Mybase(hClientProcess)
    {
    }
    
    void copy_arg_in(psl_buffer_wrapper<T*> ClientBuffer)
    {
        assert(_buffer == NULL);
        assert(_count == 0);

        if (ClientBuffer.buffer() == 0) return;
        
        if((ClientBuffer.count() < UINT_MAX / detail::size_of<T>::value) &&                 // protect against int overflow
           (_buffer = (T*)operator new(ClientBuffer.count() * detail::size_of<T>::value)))  // TODO: use arena allocator
        {
            // copy_in policy copies content of the buffer as memory block
            // to copy each element use copy_array_in policy
            memcpy(_buffer, ClientBuffer.buffer(), ClientBuffer.count() * detail::size_of<T>::value);
            _count = ClientBuffer.count();
        }
    }
    
    void copy_arg_out()
    {
    }
    
    ~copy_in()
    {
        if(_buffer)
            operator delete(_buffer);
    }
};


//
// Policy to copy [out] arguments before function return
//
template <typename T>
class copy_out;

// Specialization of copy_out for pointers
template <typename T>
class copy_out<T*>
    : public ptr_copy_traits_base<T>
{
public:
    typedef ptr_copy_traits_base<T> _Mybase;
    
    copy_out(HANDLE hClientProcess)
        : _Mybase(hClientProcess)
    {
    }
    
    void copy_arg_in(T* pClientPtr)
    {
        assert(m_pServerPtr == NULL);
        m_pClientPtr = pClientPtr;

        // NOTE: In the event the stub function does not
        // modify m_LocalCopy we want to make sure we write 
        // back the original value. Thus we set m_LocalCopy
        // = *pClientPtr at the constructor.

        
        if(pClientPtr)
        {
            m_LocalCopy = *pClientPtr;
            m_pServerPtr = &m_LocalCopy;
        }

    }
    
    void copy_arg_out()
    {
        if(m_pClientPtr)
        {
            // this policy can be used only with non-const types
            // if type T is const, following line won't compile
            *m_pClientPtr = *m_pServerPtr;
        }
    }

protected:
        // NOTE: Due to ARM Compiler bug.  Do no use 
        // m_LocalCopy except for in the constructor.
        // After the constructor, only use *m_pServerPtr

        // EXPLN:   the compiler is doing the following.
        // assume your object is t1 and you're calling F()

        // T t2;        (compiler bug generated code)
        // constructor(t1); 

        // memcpy(t2,t1,sizeof(T)); -- compiler bug generated code

        // invalidate(t1)  (compiler bug generated code) no destructor

        // F(t2); 
        // destructor(t2); -- compiler bug generated code

        //   here's the trick: 
        //     t2.m_pServerPtr == t1.m_pServerPtr == & t1.m_LocalCopy;

        // as long as you don't use m_LocalCopy copy outside of the constructor 
        // you are safe.

    T   m_LocalCopy;

private:
    T*  m_pClientPtr;
};


// Specialization of copy_out for psl_buffer_wrapper

// NOTE:  when using a marshal<copy_out, psl_buffer_wrapper<T> > 
// if the stub function doesn't change the wrapped buffer, random
// values will be written to the clients buffer.

template <typename T>
class copy_out<psl_buffer_wrapper<T*> >
    : public copy_traits_base,
      public psl_buffer_wrapper<T*>
{
public:
    typedef copy_traits_base _Mybase;
    
    copy_out(HANDLE hClientProcess)
        : _Mybase(hClientProcess)
    {
    }
    
    void copy_arg_in(psl_buffer_wrapper<T*> ClientBuffer)
    {
        m_ClientBuffer = ClientBuffer;
        
        assert(_buffer == NULL);
        assert(_count == NULL);

        if (ClientBuffer.buffer() == 0) return;
        
        if((ClientBuffer.count() < UINT_MAX / detail::size_of<T>::value) &&                 // protect against int overflow
           (_buffer = (T*)operator new(ClientBuffer.count() * detail::size_of<T>::value)))  // TODO: use arena allocator
        {
            _count = ClientBuffer.count();
        }
    }
    
    void copy_arg_out()
    {
        if(_buffer && m_ClientBuffer.buffer())
        {
            assert(_count == m_ClientBuffer.count());
            
            memcpy(m_ClientBuffer.buffer(), _buffer, _count * detail::size_of<T>::value);
        }
    }
    
    ~copy_out()
    {
        if(_buffer)
            operator delete(_buffer);
    }
    
private:
    psl_buffer_wrapper<T*>  m_ClientBuffer;
};    


//
// Policy to copy [in out] arguments before function call and before return
//
template <typename T>
class copy_in_out;

// Specialization of copy_in_out for pointers
template <typename T>
class copy_in_out<T*>
    : public copy_out<T*>
{
public:
    copy_in_out(HANDLE hClientProcess)
        : copy_out<T*>(hClientProcess)
    {
    }

    // NOTE: copy_out does a copy on copy_arg_in , so copy_in_out is
    // identical to copy_out.
};


//
// Policy to copy array [in] arguments
// Each element of the array is seperatly copied using appropriate
// instance of copy_in policy; this enables "deep" copying.
//
template <typename T>
class copy_array_in;

template <typename T>
class copy_array_in<psl_buffer_wrapper<T*> >
    : public copy_traits_base,
      public psl_buffer_wrapper<T*>
{
public:
    typedef copy_traits_base _Mybase;
    
    copy_array_in(HANDLE hClientProcess)
        : _Mybase(hClientProcess)
    {
    }
    
    void copy_arg_in(psl_buffer_wrapper<T*> ClientBuffer)
    {
        assert(_buffer == NULL);
        assert(_count == 0);
        
        if (ClientBuffer.buffer() == 0) return;

        if((ClientBuffer.count() < UINT_MAX / detail::size_of<T>::value) &&                 // protect against int overflow
           (_buffer = (T*)operator new(ClientBuffer.count() * detail::size_of<T>::value)))  // TODO: use arena allocator
        {
            if(m_aElementCopies.resize(ClientBuffer.count(), m_hClientProcess))
            {
                for(size_t i = 0; i < ClientBuffer.count(); ++i)
                {
                    // marshal element from client buffer into psl_data wrapper
                    psl_data_wrapper<T> element;
                    element.marshal(ClientBuffer.buffer()[i]);
                    
                    // unmarshal element (will map pointer if necessary) and copy the value
                    m_aElementCopies[i].copy_arg_in(element.unmarshal(m_hClientProcess));
                    
                    // set value in buffer that will be exposed to the server, to be equal to localy copied element
                    _buffer[i] = m_aElementCopies[i];
                }
                
                _count = ClientBuffer.count();
            }
        }
    }
    
    void copy_arg_out()
    {
    }
    
    ~copy_array_in()
    {
        if(_buffer)
            operator delete(_buffer);
    }

private:
    // TODO: use arena allocator
    vector<copy_in<T> > m_aElementCopies;
};




//
// Marshal argument of type T according to copy_traits policy
//
template<template<class U> class copy_traits, typename T>
class marshal_arg : public copy_traits<T>
{
public:
    typedef marshal_arg<copy_traits, T> _Myt;

    //
    // Coping marshal_arg objects is not supported.
    // In context marshal_arg is used copies will be eliminated by 
    // temporary object optimization. Standard however requires that 
    // copy ctor is still accesible (can't be made private).
    // Copy ctor is declared public to satisfy this requirement but
    // it is not defined to make sure that marshal_arg is never copied.
    //
    marshal_arg(const _Myt&);

    //
    // Before a function is executed, arguments are marshaled
    // according to policy defined in copy_traits
    //
    marshal_arg(T pClientPtr, HANDLE hClientProcess)
        : copy_traits<T>(hClientProcess)
    {
        copy_arg_in(pClientPtr);
    }
    
    //
    // Before a function returns, arguments are marshaled
    // according to policy defined in copy_traits
    //
    ~marshal_arg()
    {
        copy_arg_out();
    }
};


//
// operator== to compare native pointers with pointer specializations of marshal_arg
//
// if(NULL == p)
//
template<template<class U> class copy_traits, typename T>
bool operator==(void* p1, marshal_arg<copy_traits, T*> p2)
{
    return p1 == static_cast<T*>(p2);
}

//
// operator!= to compare native pointers with pointer specializations of marshal_arg
//
// if(NULL != p)
//
template<template<class U> class copy_traits, typename T>
bool operator!=(void* p1, marshal_arg<copy_traits, T*> p2)
{
    return p1 != static_cast<T*>(p2);
}


//
// Partial specilaization of psl_data_wrapper for marshal_arg
//
template<template<class U> class copy_traits, typename T>
class psl_data_wrapper< marshal_arg<copy_traits, T> >
    : protected psl_data_wrapper<T>
{
public:
    typedef marshal_arg<copy_traits, T> _Arg;
    
    //
    // Uses specialization of psl_data_wrapper for T to unmarshal value from psl message
    // but returns an instance of marshal_arg to perform proper marshaling of function
    // arguments (for example copying values of [in] arguments, etc.)
    //
    _Arg unmarshal(HANDLE hClientProcess)
    {
        return _Arg(psl_data_wrapper<T>::unmarshal(hClientProcess), hClientProcess);
    }
};


namespace detail
{

//
// psl_message0
//
// This is minimal message send from proxy on the client side to stub on the server side.
// The message carries ID of function and extra info (user defined).
//
template<typename _extra_info_t>
class psl_message0
{
public:
    psl_message0(WORD wFunction, _extra_info_t ExtraInfo)
        : m_wFunction(wFunction)
    {
        // INVALID_FUNCTION_ID is used to indicate invalid message and shouldn't be used 
        assert(m_wFunction != INVALID_FUNCTION_ID);
        
        // Extra info is marshaled using the same mechanism as function parameters
        m_ExtraInfo.marshal(ExtraInfo);
    }
    
    WORD function() const
        {return m_wFunction; }
    
    _extra_info_t unmarshal_extra_info(HANDLE hClientProcess)
        {return m_ExtraInfo.unmarshal(hClientProcess); }
    
protected:
    WORD                            m_wFunction;
    psl_data_wrapper<_extra_info_t> m_ExtraInfo;
};


//
// psl_message1
//
template<typename _extra_info_t, typename T1>
class psl_message1 : public psl_message0<_extra_info_t>
{
public:
    typedef psl_message0<_extra_info_t> my_base;
     
    psl_message1(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1)
        : my_base(wFunction, ExtraInfo)
    {
        m_x1.marshal(arg1);
    }
    
    T1 unmarshal_arg1(HANDLE hClientProcess)
        {return m_x1.unmarshal(hClientProcess); }
        
protected:
    psl_data_wrapper<T1>   m_x1;
};


//
// psl_message2
//
template<typename _extra_info_t, typename T1, typename T2>
class psl_message2 : public psl_message1<_extra_info_t, T1>
{
public:
    typedef psl_message1<_extra_info_t, T1> my_base;
    
    psl_message2(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2)
        : my_base(wFunction, ExtraInfo, arg1)
    {
        m_x2.marshal(arg2);
    }
    
    T2 unmarshal_arg2(HANDLE hClientProcess)
        {return m_x2.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T2>   m_x2;
};


//
// psl_message3
//
template<typename _extra_info_t, typename T1, typename T2, typename T3>
class psl_message3 : public psl_message2<_extra_info_t, T1, T2>
{
public:
    typedef psl_message2<_extra_info_t, T1, T2> my_base;
    
    psl_message3(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3)
        : my_base(wFunction, ExtraInfo, arg1, arg2)
    {
        m_x3.marshal(arg3);
    }
    
    T3 unmarshal_arg3(HANDLE hClientProcess)
        {return m_x3.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T3>   m_x3;
};


//
// psl_message4
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4>
class psl_message4 : public psl_message3<_extra_info_t, T1, T2, T3>
{
public:
    typedef psl_message3<_extra_info_t, T1, T2, T3> my_base;
    
    psl_message4(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3)
    {
        m_x4.marshal(arg4);
    }
    
    T4 unmarshal_arg4(HANDLE hClientProcess)
        {return m_x4.unmarshal(hClientProcess); }
      
protected:
    psl_data_wrapper<T4>   m_x4;
};


//
// psl_message5
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5>
class psl_message5 : public psl_message4<_extra_info_t, T1, T2, T3, T4>
{
public:
    typedef psl_message4<_extra_info_t, T1, T2, T3, T4> my_base;
    
    psl_message5(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4)
    {
        m_x5.marshal(arg5);
    }
    
    T5 unmarshal_arg5(HANDLE hClientProcess)
        {return m_x5.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T5>   m_x5;
};


//
// psl_message6
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class psl_message6 : public psl_message5<_extra_info_t, T1, T2, T3, T4, T5>
{
public:
    typedef psl_message5<_extra_info_t, T1, T2, T3, T4, T5> my_base;
    
    psl_message6(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5)
    {
        m_x6.marshal(arg6);
    }
    
    T6 unmarshal_arg6(HANDLE hClientProcess)
        {return m_x6.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T6>   m_x6;
};


//
// psl_message7
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class psl_message7 : public psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>
{
public:
    typedef psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6> my_base;
    
    psl_message7(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6)
    {
        m_x7.marshal(arg7);
    }
    
    T7 unmarshal_arg7(HANDLE hClientProcess)
        {return m_x7.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T7>   m_x7;
};


//
// psl_message8
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class psl_message8 : public psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>
{
public:
    typedef psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7> my_base;
    
    psl_message8(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
    {
        m_x8.marshal(arg8);
    }
    
    T8 unmarshal_arg8(HANDLE hClientProcess)
        {return m_x8.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T8>   m_x8;
};


//
// psl_message9
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class psl_message9 : public psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>
{
public:
    typedef psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8> my_base;
    
    psl_message9(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
    {
        m_x9.marshal(arg9);
    }
    
    T9 unmarshal_arg9(HANDLE hClientProcess)
        {return m_x9.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T9>   m_x9;
};


//
// psl_message10
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class psl_message10 : public psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>
{
public:
    typedef psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9> my_base;
    
    psl_message10(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
    {
        m_x10.marshal(arg10);
    }
    
    T10 unmarshal_arg10(HANDLE hClientProcess)
        {return m_x10.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T10>   m_x10;
};


//
// psl_message11
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class psl_message11 : public psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
{
public:
    typedef psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> my_base;
    
    psl_message11(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)
    {
        m_x11.marshal(arg11);
    }
    
    T11 unmarshal_arg11(HANDLE hClientProcess)
        {return m_x11.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T11>   m_x11;
};


//
// psl_message12
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class psl_message12 : public psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>
{
public:
    typedef psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> my_base;
    
    psl_message12(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11, T12 arg12)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)
    {
        m_x12.marshal(arg12);
    }
    
    T12 unmarshal_arg12(HANDLE hClientProcess)
        {return m_x12.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T12>   m_x12;
};

//
// Helper that models a simple thread local storage usable within PSL call
//
template<typename T>
class psl_tls
{
    typedef hash_map<DWORD, T> MAP;
    
public:
    T* get_thread_data()
    {
        gate<critical_section> gate(cs);
        
        return map[GetCurrentThreadId()];
    }
    
    bool set_thread_data(const T& x)
    {
        gate<critical_section> gate(cs);
        
        DWORD           dwThreadId = GetCurrentThreadId();
        MAP::iterator   it;
        
        if(map.end() != (it = map.find(dwThreadId)))
        {
            *it = x;
            return true;
        }
        else
            return map.insert(dwThreadId, x);
    }
    
    void clean_thread_data()
    {
        gate<critical_section> gate(cs);
        
        map.erase(GetCurrentThreadId());
    }
    
protected:
    MAP                 map;
    critical_section    cs;
};

}; // namespace detail

#define STUB_EXTRA_ARG arg,

//
// psl_stub
//
template <typename _extra_info_t = void*>
class psl_stub
{
public:
    typedef psl_stub<_extra_info_t> _Myt;
    typedef list<_Myt*>             psl_stub_stack;
    //
    // Singleton instance of thread local storage helper. 
    // Singleton guarantees that the instance is created in an atomic way on the first usage.
    //
    typedef singleton<detail::psl_tls<psl_stub_stack> > psl_stub_tls;

    psl_stub(__in_bcount(nInBufferSize) LPVOID pInBuffer, DWORD nInBufferSize, HANDLE hClientProcess = GetCallerProcess())
        : m_pMessage0(NULL),
          m_hClientProcess(hClientProcess),
          m_nInBufferSize(nInBufferSize)
    {
        //
        // Static instance of singleton_lifetime assures that psl_stub_tls singleton 
        // is destroyed during module unload. singleton_lifetime ctor is thread safe.
        //
        static singleton_lifetime<psl_stub_tls> psl_tls_lifetime;
        
        //
        // Push this instance of psl_stub on the thread local stack.
        //
        if(psl_stub_stack* pPslStubStack = psl_stub_tls::instance().get_thread_data())
        {
            if(pPslStubStack->push_front(this))
            {
                //
                // Check whether the input buffer is valid in context of the caller process
                //
                if(PSLGetCallerTrust() != OEM_CERTIFY_TRUST)
                    pInBuffer = MapCallerPtr(pInBuffer, nInBufferSize);
                        
                //
                // Check if the size of input buffer is at least the size of minimal message
                //
                if(nInBufferSize >= sizeof(detail::psl_message0<_extra_info_t>))
                    m_pMessage0 = reinterpret_cast<detail::psl_message0<_extra_info_t>*>(pInBuffer);
            }
        }
    }
    
    ~psl_stub()
    {
        if(psl_stub_stack* pPslStubStack = psl_stub_tls::instance().get_thread_data())
        {
            // Pop his instance of the stub from the thread local stack
            if(!pPslStubStack->empty())
                pPslStubStack->pop_front();
        
            // Clean the thread local data if the stack is empty
            if(pPslStubStack->empty())
                psl_stub_tls::instance().clean_thread_data();
        }
    }
    
    static _Myt* get_thread_stub()
    {
        //
        // Static instance of singleton_lifetime assures that psl_stub_tls singleton 
        // is destroyed during module unload. singleton_lifetime ctor is thread safe.
        //
        static singleton_lifetime<psl_stub_tls> psl_tls_lifetime;
        
        // Return pointer to the instance of stub on top of thread local stack
        if(psl_stub_stack* pPslStubStack = psl_stub_tls::instance().get_thread_data())
        {
            if(!pPslStubStack->empty())
                return pPslStubStack->front();
        }
        
        return NULL;
    }
    
    WORD function() const
    {
        if(m_pMessage0)
            return m_pMessage0->function();
        else
            return INVALID_FUNCTION_ID;
    }
    
    bool extra_info(_extra_info_t& info) const
    {
        if(m_pMessage0)
            return NULL != (info = m_pMessage0->unmarshal_extra_info(m_hClientProcess));
        else
            return false;
    }
    
    HANDLE client_process() const
    {
        return m_hClientProcess;
    }
    
    
    // 0
#   define STUB_CALL_0_ARG(FUNC, ARG)                                                                                           \
    {                                                                                                                           \
        detail::psl_message0<_extra_info_t>* pMessage = reinterpret_cast<detail::psl_message0<_extra_info_t>*>(m_pMessage0);    \
                                                                                                                                \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                   \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                           \
                                                                                                                                \
        __try                                                                                                                   \
        {                                                                                                                       \
            return (FUNC)(ARG);                                                                                                 \
        }                                                                                                                       \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                     \
        {                                                                                                                       \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                        \
        }                                                                                                                       \
    }

    template<typename T>
    T call(T(*Fun)(void))
    {
        STUB_CALL_0_ARG(*Fun,)
    }    
    
    template<typename T, typename T0>
    T call(T(*Fun)(T0), T0 arg)
    {
        STUB_CALL_0_ARG(*Fun, arg)
    }
    
    template<typename C, typename T>
    T call(C* p, T(C::*Fun)(void))
    {
        STUB_CALL_0_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0>
    T call(C* p, T(C::*Fun)(T0), T0 arg)
    {
        STUB_CALL_0_ARG(p->*Fun, arg)
    }
    
    template<typename C, typename T>
    T call(C& o, T(C::*Fun)(void))
    {
        STUB_CALL_0_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0>
    T call(C& o, T(C::*Fun)(T0), T0 arg)
    {
        STUB_CALL_0_ARG(o.*Fun, arg)
    }
    
    
    // 1
#   define STUB_CALL_1_ARG(FUNC, ARG)                                                                                                   \
    {                                                                                                                                   \
        detail::psl_message1<_extra_info_t, T1>* pMessage = reinterpret_cast<detail::psl_message1<_extra_info_t, T1>*>(m_pMessage0);    \
                                                                                                                                        \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                           \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                   \
                                                                                                                                        \
        __try                                                                                                                           \
        {                                                                                                                               \
            return (FUNC)(ARG                                                                                                           \
                          pMessage->unmarshal_arg1(m_hClientProcess));                                                                  \
        }                                                                                                                               \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                             \
        {                                                                                                                               \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                \
        }                                                                                                                               \
    }
    
    template<typename T, typename T1>
    T call(T(*Fun)(T1))
    {
        STUB_CALL_1_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1>
    T call(T(*Fun)(T0, T1), T0 arg)
    {
        STUB_CALL_1_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1>
    T call(C* p, T(C::*Fun)(T1))
    {
        STUB_CALL_1_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1>
    T call(C* p, T(C::*Fun)(T0, T1), T0 arg)
    {
        STUB_CALL_1_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1>
    T call(C& o, T(C::*Fun)(T1))
    {
        STUB_CALL_1_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1>
    T call(C& o, T(C::*Fun)(T0, T1), T0 arg)
    {
        STUB_CALL_1_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 2
#   define STUB_CALL_2_ARG(FUNC, ARG)                                                                                                           \
    {                                                                                                                                           \
        detail::psl_message2<_extra_info_t, T1, T2>* pMessage = reinterpret_cast<detail::psl_message2<_extra_info_t, T1, T2>*>(m_pMessage0);    \
                                                                                                                                                \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                   \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                           \
                                                                                                                                                \
        __try                                                                                                                                   \
        {                                                                                                                                       \
            return (FUNC)(ARG                                                                                                                   \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                           \
                          pMessage->unmarshal_arg2(m_hClientProcess));                                                                          \
        }                                                                                                                                       \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                     \
        {                                                                                                                                       \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                        \
        }                                                                                                                                       \
    }
    
    template<typename T, typename T1, typename T2>
    T call(T(*Fun)(T1, T2))
    {
        STUB_CALL_2_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2>
    T call(T(*Fun)(T0, T1, T2), T0 arg)
    {
        STUB_CALL_2_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2>
    T call(C* p, T(C::*Fun)(T1, T2))
    {
        STUB_CALL_2_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2>
    T call(C* p, T(C::*Fun)(T0, T1, T2), T0 arg)
    {
        STUB_CALL_2_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2>
    T call(C& o, T(C::*Fun)(T1, T2))
    {
        STUB_CALL_2_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2>
    T call(C& o, T(C::*Fun)(T0, T1, T2), T0 arg)
    {
        STUB_CALL_2_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 3
#   define STUB_CALL_3_ARG(FUNC, ARG)                                                                                                                   \
    {                                                                                                                                                   \
        detail::psl_message3<_extra_info_t, T1, T2, T3>* pMessage = reinterpret_cast<detail::psl_message3<_extra_info_t, T1, T2, T3>*>(m_pMessage0);    \
                                                                                                                                                        \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                           \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                   \
                                                                                                                                                        \
        __try                                                                                                                                           \
        {                                                                                                                                               \
            return (FUNC)(ARG                                                                                                                           \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                   \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                   \
                          pMessage->unmarshal_arg3(m_hClientProcess));                                                                                  \
        }                                                                                                                                               \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                             \
        {                                                                                                                                               \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                \
        }                                                                                                                                               \
    }
    
    template<typename T, typename T1, typename T2, typename T3>
    T call(T(*Fun)(T1, T2, T3))
    {
        STUB_CALL_3_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3>
    T call(T(*Fun)(T0, T1, T2, T3), T0 arg)
    {
        STUB_CALL_3_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3>
    T call(C* p, T(C::*Fun)(T1, T2, T3))
    {
        STUB_CALL_3_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3), T0 arg)
    {
        STUB_CALL_3_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3>
    T call(C& o, T(C::*Fun)(T1, T2, T3))
    {
        STUB_CALL_3_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3), T0 arg)
    {
        STUB_CALL_3_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 4
#   define STUB_CALL_4_ARG(FUNC, ARG)                                                                                                                           \
    {                                                                                                                                                           \
        detail::psl_message4<_extra_info_t, T1, T2, T3, T4>* pMessage = reinterpret_cast<detail::psl_message4<_extra_info_t, T1, T2, T3, T4>*>(m_pMessage0);    \
                                                                                                                                                                \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                   \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                           \
                                                                                                                                                                \
        __try                                                                                                                                                   \
        {                                                                                                                                                       \
            return (FUNC)(ARG                                                                                                                                   \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                           \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                           \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                           \
                          pMessage->unmarshal_arg4(m_hClientProcess));                                                                                          \
        }                                                                                                                                                       \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                     \
        {                                                                                                                                                       \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                        \
        }                                                                                                                                                       \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4>
    T call(T(*Fun)(T1, T2, T3, T4))
    {
        STUB_CALL_4_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
    T call(T(*Fun)(T0, T1, T2, T3, T4), T0 arg)
    {
        STUB_CALL_4_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4))
    {
        STUB_CALL_4_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4), T0 arg)
    {
        STUB_CALL_4_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4))
    {
        STUB_CALL_4_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4), T0 arg)
    {
        STUB_CALL_4_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 5
#   define STUB_CALL_5_ARG(FUNC, ARG)                                                                                                                                   \
    {                                                                                                                                                                   \
        detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5>* pMessage = reinterpret_cast<detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5>*>(m_pMessage0);    \
                                                                                                                                                                        \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                           \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                   \
                                                                                                                                                                        \
        __try                                                                                                                                                           \
        {                                                                                                                                                               \
            return (FUNC)(ARG                                                                                                                                           \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                   \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                   \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                   \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                   \
                          pMessage->unmarshal_arg5(m_hClientProcess));                                                                                                  \
        }                                                                                                                                                               \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                             \
        {                                                                                                                                                               \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                \
        }                                                                                                                                                               \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(T(*Fun)(T1, T2, T3, T4, T5))
    {
        STUB_CALL_5_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5), T0 arg)
    {
        STUB_CALL_5_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5))
    {
        STUB_CALL_5_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5), T0 arg)
    {
        STUB_CALL_5_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5))
    {
        STUB_CALL_5_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5), T0 arg)
    {
        STUB_CALL_5_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 6
#   define STUB_CALL_6_ARG(FUNC, ARG)                                                                                                                                           \
    {                                                                                                                                                                           \
        detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>* pMessage = reinterpret_cast<detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>*>(m_pMessage0);    \
                                                                                                                                                                                \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                                   \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                           \
                                                                                                                                                                                \
        __try                                                                                                                                                                   \
        {                                                                                                                                                                       \
            return (FUNC)(ARG                                                                                                                                                   \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                           \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                           \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                           \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                           \
                          pMessage->unmarshal_arg5(m_hClientProcess),                                                                                                           \
                          pMessage->unmarshal_arg6(m_hClientProcess));                                                                                                          \
        }                                                                                                                                                                       \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                                     \
        {                                                                                                                                                                       \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                        \
        }                                                                                                                                                                       \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6))
    {
        STUB_CALL_6_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6), T0 arg)
    {
        STUB_CALL_6_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5, T6))
    {
        STUB_CALL_6_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6), T0 arg)
    {
        STUB_CALL_6_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5, T6))
    {
        STUB_CALL_6_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6), T0 arg)
    {
        STUB_CALL_6_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 7
#   define STUB_CALL_7_ARG(FUNC, ARG)                                                                                                                                                   \
    {                                                                                                                                                                                   \
        detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>* pMessage = reinterpret_cast<detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>*>(m_pMessage0);    \
                                                                                                                                                                                        \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                                           \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                                   \
                                                                                                                                                                                        \
        __try                                                                                                                                                                           \
        {                                                                                                                                                                               \
            return (FUNC)(ARG                                                                                                                                                           \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                                   \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                                   \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                                   \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                                   \
                          pMessage->unmarshal_arg5(m_hClientProcess),                                                                                                                   \
                          pMessage->unmarshal_arg6(m_hClientProcess),                                                                                                                   \
                          pMessage->unmarshal_arg7(m_hClientProcess));                                                                                                                  \
        }                                                                                                                                                                               \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                                             \
        {                                                                                                                                                                               \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                                \
        }                                                                                                                                                                               \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7))
    {
        STUB_CALL_7_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7), T0 arg)
    {
        STUB_CALL_7_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7))
    {
        STUB_CALL_7_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7), T0 arg)
    {
        STUB_CALL_7_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7))
    {
        STUB_CALL_7_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7), T0 arg)
    {
        STUB_CALL_7_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 8
#   define STUB_CALL_8_ARG(FUNC, ARG)                                                                                                                                                           \
    {                                                                                                                                                                                           \
        detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>* pMessage = reinterpret_cast<detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>*>(m_pMessage0);    \
                                                                                                                                                                                                \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                                                   \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                                           \
                                                                                                                                                                                                \
        __try                                                                                                                                                                                   \
        {                                                                                                                                                                                       \
            return (FUNC)(ARG                                                                                                                                                                   \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                                           \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                                           \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                                           \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                                           \
                          pMessage->unmarshal_arg5(m_hClientProcess),                                                                                                                           \
                          pMessage->unmarshal_arg6(m_hClientProcess),                                                                                                                           \
                          pMessage->unmarshal_arg7(m_hClientProcess),                                                                                                                           \
                          pMessage->unmarshal_arg8(m_hClientProcess));                                                                                                                          \
        }                                                                                                                                                                                       \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                                                     \
        {                                                                                                                                                                                       \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                                        \
        }                                                                                                                                                                                       \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8))
    {
        STUB_CALL_8_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8), T0 arg)
    {
        STUB_CALL_8_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8))
    {
        STUB_CALL_8_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8), T0 arg)
    {
        STUB_CALL_8_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8))
    {
        STUB_CALL_8_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8), T0 arg)
    {
        STUB_CALL_8_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 9
#   define STUB_CALL_9_ARG(FUNC, ARG)                                                                                                                                                                   \
    {                                                                                                                                                                                                   \
        detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>* pMessage = reinterpret_cast<detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>*>(m_pMessage0);    \
                                                                                                                                                                                                        \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                                                           \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                                                   \
                                                                                                                                                                                                        \
        __try                                                                                                                                                                                           \
        {                                                                                                                                                                                               \
            return (FUNC)(ARG                                                                                                                                                                           \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg5(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg6(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg7(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg8(m_hClientProcess),                                                                                                                                   \
                          pMessage->unmarshal_arg9(m_hClientProcess));                                                                                                                                  \
        }                                                                                                                                                                                               \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                                                             \
        {                                                                                                                                                                                               \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                                                \
        }                                                                                                                                                                                               \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        STUB_CALL_9_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), T0 arg)
    {
        STUB_CALL_9_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        STUB_CALL_9_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), T0 arg)
    {
        STUB_CALL_9_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        STUB_CALL_9_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), T0 arg)
    {
        STUB_CALL_9_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 10
#   define STUB_CALL_10_ARG(FUNC, ARG)                                                                                                                                                                              \
    {                                                                                                                                                                                                               \
        detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>* pMessage = reinterpret_cast<detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>*>(m_pMessage0);    \
                                                                                                                                                                                                                    \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                                                                       \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                                                               \
                                                                                                                                                                                                                    \
        __try                                                                                                                                                                                                       \
        {                                                                                                                                                                                                           \
            return (FUNC)(ARG                                                                                                                                                                                       \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg5(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg6(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg7(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg8(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg9(m_hClientProcess),                                                                                                                                               \
                          pMessage->unmarshal_arg10(m_hClientProcess));                                                                                                                                             \
        }                                                                                                                                                                                                           \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                                                                         \
        {                                                                                                                                                                                                           \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                                                            \
        }                                                                                                                                                                                                           \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
    {
        STUB_CALL_10_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), T0 arg)
    {
        STUB_CALL_10_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
    {
        STUB_CALL_10_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), T0 arg)
    {
        STUB_CALL_10_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
    {
        STUB_CALL_10_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), T0 arg)
    {
        STUB_CALL_10_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 11
#   define STUB_CALL_11_ARG(FUNC, ARG)                                                                                                                                                                                      \
    {                                                                                                                                                                                                                       \
        detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>* pMessage = reinterpret_cast<detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>*>(m_pMessage0);  \
                                                                                                                                                                                                                            \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                                                                               \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                                                                       \
                                                                                                                                                                                                                            \
        __try                                                                                                                                                                                                               \
        {                                                                                                                                                                                                                   \
            return (FUNC)(ARG                                                                                                                                                                                               \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg5(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg6(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg7(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg8(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg9(m_hClientProcess),                                                                                                                                                       \
                          pMessage->unmarshal_arg10(m_hClientProcess),                                                                                                                                                      \
                          pMessage->unmarshal_arg11(m_hClientProcess));                                                                                                                                                     \
        }                                                                                                                                                                                                                   \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                                                                                 \
        {                                                                                                                                                                                                                   \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                                                                    \
        }                                                                                                                                                                                                                   \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
    {
        STUB_CALL_11_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11), T0 arg)
    {
        STUB_CALL_11_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
    {
        STUB_CALL_11_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11), T0 arg)
    {
        STUB_CALL_11_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
    {
        STUB_CALL_11_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11), T0 arg)
    {
        STUB_CALL_11_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
    
    // 12
#   define STUB_CALL_12_ARG(FUNC, ARG)                                                                                                                                                                                                  \
    {                                                                                                                                                                                                                                   \
        detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>* pMessage = reinterpret_cast<detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>*>(m_pMessage0);    \
                                                                                                                                                                                                                                        \
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))                                                                                                                                                                           \
            return error<T>(ERROR_INVALID_PARAMETER);                                                                                                                                                                                   \
                                                                                                                                                                                                                                        \
        __try                                                                                                                                                                                                                           \
        {                                                                                                                                                                                                                               \
            return (FUNC)(ARG                                                                                                                                                                                                           \
                          pMessage->unmarshal_arg1(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg2(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg3(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg4(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg5(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg6(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg7(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg8(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg9(m_hClientProcess),                                                                                                                                                                   \
                          pMessage->unmarshal_arg10(m_hClientProcess),                                                                                                                                                                  \
                          pMessage->unmarshal_arg11(m_hClientProcess),                                                                                                                                                                  \
                          pMessage->unmarshal_arg12(m_hClientProcess));                                                                                                                                                                 \
        }                                                                                                                                                                                                                               \
        __except(EXCEPTION_EXECUTE_HANDLER)                                                                                                                                                                                             \
        {                                                                                                                                                                                                                               \
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);                                                                                                                                                                                \
        }                                                                                                                                                                                                                               \
    }
    
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
    {
        STUB_CALL_12_ARG(*Fun,)
    }    
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12), T0 arg)
    {
        STUB_CALL_12_ARG(*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(C* p, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
    {
        STUB_CALL_12_ARG(p->*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(C* p, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12), T0 arg)
    {
        STUB_CALL_12_ARG(p->*Fun, STUB_EXTRA_ARG)
    }
    
    template<typename C, typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(C& o, T(C::*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
    {
        STUB_CALL_12_ARG(o.*Fun,)
    }    
    
    template<typename C, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(C& o, T(C::*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12), T0 arg)
    {
        STUB_CALL_12_ARG(o.*Fun, STUB_EXTRA_ARG)
    }
    
protected:
    // general template not defined
    template<typename T>
    T error(DWORD dwError);
    
    // specialization for DWORD
    template<>
    DWORD error<DWORD>(DWORD dwError)
        {return dwError; }
        
    // specialization for HRESULT
    template<>
    HRESULT error<HRESULT>(DWORD dwError)
        {return HRESULT_FROM_WIN32(dwError); }
    
    // specialization for BOOL
    template<>
    BOOL error<BOOL>(DWORD dwError)
    {
        SetLastError(dwError);
        return dwError == ERROR_SUCCESS;
    }
    
protected:
    detail::psl_message0<_extra_info_t>*    m_pMessage0;
    DWORD                                   m_nInBufferSize;
    HANDLE                                  m_hClientProcess;
};


//
// psl_proxy
//
template<typename _extra_info_t = void*>
class psl_proxy
{
public:
    psl_proxy(LPCTSTR pszDevice, DWORD dwIoControlCode, _extra_info_t ExtraInfo)
        : m_pszDevice(pszDevice),
          m_dwIoControlCode(dwIoControlCode),
          m_ExtraInfo(ExtraInfo),
          m_hDevice(INVALID_HANDLE_VALUE),
          m_bOwnDevice(false)
    {}
    
    ~psl_proxy()
    {
        if(m_bOwnDevice && INVALID_HANDLE_VALUE != m_hDevice)
            CloseHandle(m_hDevice);
    }
    
    void attach(HANDLE hDevice)
    {
        if(m_bOwnDevice && INVALID_HANDLE_VALUE != m_hDevice)
            CloseHandle(m_hDevice);
            
        m_hDevice = hDevice;
        m_bOwnDevice = false;
    }

    HANDLE detach()
    {
        HANDLE hRet = m_hDevice;
        m_hDevice = NULL;
        m_bOwnDevice = false;
        return hRet;
    } 
        
    DWORD call(WORD wFunction)
    {
        detail::psl_message0<_extra_info_t> message(wFunction, m_ExtraInfo);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1>
    DWORD call(WORD wFunction, T1 arg1)
    {
        detail::psl_message1<_extra_info_t, T1> message(wFunction, m_ExtraInfo, arg1);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2)
    {
        detail::psl_message2<_extra_info_t, T1, T2> message(wFunction, m_ExtraInfo, arg1, arg2);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3)
    {
        detail::psl_message3<_extra_info_t, T1, T2, T3> message(wFunction, m_ExtraInfo, arg1, arg2, arg3);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
    {
        detail::psl_message4<_extra_info_t, T1, T2, T3, T4> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
    {
        detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
    {
        detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
    {
        detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
    {
        detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9)
    {
        detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10)
    {
        detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11)
    {
        detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11, T12 arg12)
    {
        detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
        
        return psl_call(&message, sizeof(message));
    };

protected:
    DWORD psl_call(__out_bcount(nInBufferSize) LPVOID pInBuffer, DWORD nInBufferSize)
    {
        if(INVALID_HANDLE_VALUE == m_hDevice)
        {
            HANDLE hDevice = CreateFile(m_pszDevice, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
            
            if(INVALID_HANDLE_VALUE != hDevice)
            {
                m_bOwnDevice = true;
                
                if(INVALID_HANDLE_VALUE != InterlockedCompareExchangePointer(&m_hDevice, hDevice, INVALID_HANDLE_VALUE))
                    CloseHandle(hDevice);
        
                assert(INVALID_HANDLE_VALUE != m_hDevice);
            }
        }
        
        if(DeviceIoControl(m_hDevice, 
                           m_dwIoControlCode, 
                           pInBuffer, 
                           nInBufferSize, 
                           NULL, 
                           0, 
                           NULL, 
                           NULL))
        {
            return ERROR_SUCCESS;
        }
        else
        {
            // Server function returned FALSE but didn't set last error
            assert(ERROR_SUCCESS != GetLastError());
            
            return GetLastError();
        }
    }
    
protected:
    HANDLE          m_hDevice;
    bool            m_bOwnDevice;
    LPCTSTR         m_pszDevice;
    DWORD           m_dwIoControlCode;
    _extra_info_t   m_ExtraInfo;
};

}; // namespace ce

#endif // __PSL_MARSHALER__
