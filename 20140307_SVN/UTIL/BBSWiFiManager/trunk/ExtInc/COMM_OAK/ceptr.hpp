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
#ifndef __CE_PTR_HPP_INCLUDED__
#define __CE_PTR_HPP_INCLUDED__

#ifndef __WINDOWS__
#include <windows.h>
#endif

/*

    CePtr's provide encapsulation and manipulation of Windows CE pointers.  

    In CE, the bottom 2 gig of memory is divided into 64 slots of 32 meg each:

        Slot  0, 0x00000000 - 0x01ffffff
        Slot  1, 0x02000000 - 0x03ffffff       << First DLL Slot
        Slot  2, 0x04000000 - 0x05ffffff
        Slot  3, 0x06000000 - 0x07ffffff
            .
            .
            .
        Slot 31, 0x3e000000 - 0x3fffffff
        Slot 32, 0x40000000 - 0x41ffffff
            .
            .
            .
        Slot 60, 0x78000000 - 0x79ffffff       << Second DLL Slot
        Slot 61, 0x7a000000 - 0x7bffffff       << Third DLL Slot
        Slot 62, 0x7c000000 - 0x7dffffff
        Slot 63, 0x7e000000 - 0x7fffffff


    Note that by laying out the bits of the address carefully, it's possible 
    to determine the slot number.

    Slot 0 is special and Slot 1 (and 60 & 61 on ARM) are used to map DLL's.  
    Slot 0 is reserved for the currently running process.  All code has to be run 
    in the context of a particular process.  For addresses located in these special
    slots, there is no way to determine the associated process by looking at just the 
    bits in the address.

    For other addresses, the slot number uniquely identifies the 
    associated process.  These addresses are often called mapped pointers: 
    their values have been mapped to a particular slot.

    Most applications programmers will never use the details of CE pointers.  
    Pointers will work just like regular C pointers so these routines will not 
    be necessary or particularly useful.  System programmers will need to have 
    some knowledge of how the pointers work however and these routines will be 
    very useful.
    
    System programmers will often need to pass pointers to different processes 
    and deal with pointers that have come in from other processes.
    
    The primary operations on the pointers are:
    
        1.  Encoding the pointer's associated process into the pointer, usually 
        called "mapping the pointer".
        
        2.  Extracting the associated process from a mapped pointer, here called 
        "Unpacking the pointer".


    CePtr's lay out the address so that the bits of the address that 
    correspond to the slot number are easily accessible and modifiable.
    
    Mapping slot 0 addresses is simple: the slot number of the process is set into 
    the appropriate bits of the address.  Mapping slot 1 address is only 
    slightly more complex.  If the incoming address is a slot 1 address, this 
    is remembered by setting a normally unused bit of the address.  The index 
    of the process is then set into the appropriate bits of the address.  
    Addresses in other slots do not need any modifications.  They are already 
    mapped.

    Unpacking the mapped pointer is also straightforward.  The OS is queried 
    for the HPROCESS that corresponds to the index bits of the addres and the 
    dll flag bits are checked so the address can be restored.
    
    ARM platforms have the ability to utilize Slot 60 & 61 as upper DLL slots.
    Access to these upper slots is enabled through the IsDllAddress bit PLUS 
    a "borrowed bit" at the lower range of the address. It is the combination
    of the two bits that allow CePtr to differentiate between the following
    four possibilities:

    +---+---+
    | 0 | 0 | The _index_ of the pointer is to 0 to form a Slot 0 address
    +---+---+
    | 1 | 0 | The _index_ is set to 1 to form Slot 1 address
    +---+---+
    | 1 | 1 | The _index_ is set to 0x003C to form Slot 60 address
    +---+---+
    | 0 | 1 | The _index_ is set to 0x003D to form Slot 61 address
    +---+---+

    **IMPORTANT: ARM's "borrowed bit" relies on 0x00000002 bit not being used
    by ARM or THUMB generated code. CePtr makes the assumption that even THUMB 
    generated code DWORD aligns function pointers allowing us to freely modify 
    this bit. 

    **IMPORTANT: Note that CePtr's are the exact size of C pointers.  This 
    allows them to be passed across processes as regular C pointers.  
*/


#ifndef DBGVIRTMEM
#define DBGVIRTMEM  DEBUGZONE(4)
#endif 

// Masks to determine DLL slot
#ifdef ARM
#define MODULE_SLOT_61_MASK  0x80000002
#define MODULE_SLOT_60_MASK  0x00000002
#define MODULE_SLOT_1_MASK   0x80000000
#endif


class CePtrBase_t
{
private:
    union
        {
        void*                m_Ptr;
        long                m_PtrLong;
        unsigned long        m_PtrUnsignedLong;
        struct
            {
            unsigned int    m_Address        : 25;
            unsigned int    m_ProcessIndex    :  6;
            unsigned int    m_IsDllAddress    :  1;
            };
        };

protected:

    /*
        Default CePtr constructor initializes pointer to 0.
    */
    CePtrBase_t(
        void
        )
    {
        m_Ptr = 0;
    }


    /*
        Create a CePtr from the raw bits of a C pointer.
    */
    CePtrBase_t(
        void*    p
        )
    {
        m_Ptr = p;
    }


    /*
        Create a CePtr from the raw bits of an unsigned long.
    */
    CePtrBase_t(
        unsigned long    l
        )
    {
        m_PtrUnsignedLong = l;
    }



    /*
        Create a CePtr from the raw bits of a long.
    */
    CePtrBase_t(
        long    l
        )
    {
        m_PtrLong = l;
    }



    /*
        Create a CePtr and map it to the given process.  If the pointer is already 
        mapped, it will not be modified.  If the process handle is 0, the pointer 
        will be mapped to the current process.  
    */
    CePtrBase_t(
        void*        p,
        HPROCESS    hProcessToMapTo
        )
    {
        m_Ptr = p;
        MapToProcess(hProcessToMapTo);
        return;
    }



    /*
        Unpack the unmapped pointer and process handle from the ceptr.
    */
    void
    Unpack(
        void**        pUnmapped,
        HPROCESS*    hProcessMappedTo
        ) const;


    /*
        Return the pointer as a C++ pointer.
    */
    void*
    AsCppPtr(
        void
        ) const
    {
        return m_Ptr;
    }



public:


    /*
        Map the ceptr to the given process.  If the pointer is already 
        mapped, it will not be modified.  If the process handle is 0, the pointer 
        will be mapped to the current process.  
    */
    void
    MapToProcess(
        HPROCESS    hProcessToMapTo
        );


    /*
        Maps a pointer to the current process.  See MapToProcess.
    */
    void
    MapToCurrentProcess(
        void
        )
    {
        MapToProcess(0);
    }


    /*
        Returns true if the pointer is mapped.
    */
    bool
    IsMapped(
        void
        ) const
    {
        return ( m_Ptr == 0 ) ||
#ifdef ARM
                ( ( m_ProcessIndex != 0 ) && ( m_ProcessIndex != 1 ) && 
                ( m_ProcessIndex != MODULE_SECTION_2 ) && ( m_ProcessIndex != MODULE_SECTION_3) );
#else
                ( ( m_ProcessIndex != 0 ) && ( m_ProcessIndex != 1 ) );
#endif
    }

    bool
    IsMappedOrDll(
        void
        ) const
    {
#ifdef ARM
        return IsMapped() || ( m_ProcessIndex == 1 ) || ( m_ProcessIndex == MODULE_SECTION_2 ) || ( m_ProcessIndex == MODULE_SECTION_3 );
#else
        return IsMapped() || ( m_ProcessIndex == 1 );
#endif
    }



    /*
        Returns true if the pointer is mapped.
        Debug version will assert if the pointer is not mapped.
    */
    bool
    IsMappedAssert(
        void
        ) const;


    /*
        Returns true if the pointer is 0.
    */
    bool
    IsNull(
        void
        ) const
    {
        return m_Ptr == 0;
    }


    /*
        Returns the pointer as a long value.
    */
    long
    AsLong(
        void
        )
    {
        return m_PtrLong;
    }



};


template <class C_t> class CePtr_t : public CePtrBase_t
{
public:

    CePtr_t<C_t>(
        void
        ) : CePtrBase_t()
    {
    }


    explicit
    CePtr_t<C_t>(
        C_t    p
        ) : CePtrBase_t(p)
    {
    }


    explicit
    CePtr_t<C_t>(
        unsigned long    l
        ) : CePtrBase_t(l)
    {
    }



    CePtr_t<C_t>(
        C_t            p,
        HPROCESS    hProcessToMapTo
        ) : CePtrBase_t(p, hProcessToMapTo)
    {
    }


    void
    Unpack(
        C_t*        pUnmapped,
        HPROCESS*    phProcessMappedTo
        ) const
    {
        CePtrBase_t::Unpack(reinterpret_cast<void**>(pUnmapped), phProcessMappedTo);
    }


    CePtr_t<C_t>&
    MapToProcess(
        HPROCESS    hProcessToMapTo
        )
    {
        CePtrBase_t::MapToProcess(hProcessToMapTo);
        return *this;
    }


    CePtr_t<C_t>&
    MapToCurrentProcess(
        void
        )
    {
        CePtrBase_t::MapToProcess(0);
        return *this;
    }


    C_t
    AsCppPtr(
        void
        ) const
    {
        return static_cast<C_t>(CePtrBase_t::AsCppPtr());
    }



    /*
        Maps the given pointer to the current process.
    */
    static
    void
    MapToCurrentProcess(
        C_t*    p
        )
    {
        *p = CePtr_t<C_t>(*p,0).AsCppPtr();
        return;
    }



};


#endif


