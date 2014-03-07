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
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  
	CSync.h
Abstract:
	Sync object.

Notes: 
--*/

#ifndef __CSYNC_H_
#define __CSYNC_H_

class CLockObject
{
private:
    CRITICAL_SECTION    m_CSection;
public:
    CLockObject()
    {
        InitializeCriticalSection( &m_CSection );
    };
    ~CLockObject()
    {
        DeleteCriticalSection( &m_CSection );
    };
    void Lock( void )
    {
        EnterCriticalSection( &m_CSection );
    };
    void Unlock( void )
    {
        LeaveCriticalSection( &m_CSection );
    };
};

class CMuTexObject
{
private:
    HANDLE  m_hObject;
public:
    CMuTexObject( BOOL bInitiallyOwn = FALSE,
                  LPCTSTR lpszName = NULL,
                  LPSECURITY_ATTRIBUTES lpsaAttribute = NULL )
    {
        m_hObject = ::CreateMutex( lpsaAttribute, bInitiallyOwn, lpszName );
        DEBUGCHK( m_hObject != NULL );
    }
    ~CMuTexObject()
    {
        if( m_hObject != NULL )
        {
            ::CloseHandle( m_hObject );
            m_hObject = NULL;
        }
    };
    BOOL Lock( DWORD dwTimeOut = INFINITE )
    {
        if( ::WaitForSingleObject( m_hObject, dwTimeOut ) == WAIT_OBJECT_0 )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    };
    BOOL Unlock( void )
    {
        return ::ReleaseMutex( m_hObject );
    };
};


#endif
