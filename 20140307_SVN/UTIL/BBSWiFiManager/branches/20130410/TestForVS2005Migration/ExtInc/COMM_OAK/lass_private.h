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
#pragma once 
#include "psl_marshaler.hxx"
#include "lass.h"


#if 0 
   //
   // To interact with the lassd component.
   //  These are private API's not exposed to the outside world.
   //  Currently usable only by LAPs

    // Reload AE

    lassd_srv_proxy lass_service;
    BOOL bRet = lass_service.LASSReloadAE(&AE_STARTUI);
#endif 

#define LASSD_HANDLE_NAME                L"LAS0:" // Keep in sync with lassd_ioctl.h
#define IOCTL_LASSD_MARSHAL_PRIVATE 6679 // this is the IOCTL used by the LASS private interface.

// Defines IOCTLs used to communicate to this service.
// (Currently only used by the private interface)
enum LASSD_PROXY_IOCTL_SUB_VALUES
{
    LASS_RELOAD_AE,
    LASS_GET_VALUE,
    LASS_VERIFY_USER_ASYNC,
    LASS_GET_RESULT,
    LASS_CLOSE,
    LASS_ENROLL,
    LASS_RELOAD,
    LASS_END
} ;


class lassd_srv_proxy
{
public:
    lassd_srv_proxy():m_proxy(LASSD_HANDLE_NAME, IOCTL_LASSD_MARSHAL_PRIVATE, NULL)
    {
    }
    lassd_srv_proxy(HANDLE hLASS):m_proxy(LASSD_HANDLE_NAME, IOCTL_LASSD_MARSHAL_PRIVATE, NULL)
    {
        m_proxy.attach(hLASS);
    }

    BOOL LASSReloadAE(const GUID* pAEToReload) 
    {
        const DWORD ret = m_proxy.call(LASS_RELOAD_AE,pAEToReload);
        return DWORDReturnToBOOL(ret);
    }

    BOOL LASSGetValue(
            DWORD ValueId,PVOID lpvOutBuffer, DWORD cbOutBuffer, DWORD* pcbReturned)
    {
        const DWORD ret =   m_proxy.call(LASS_GET_VALUE,
                ValueId,
                ce::psl_buffer(lpvOutBuffer,cbOutBuffer),
                pcbReturned);

        return DWORDReturnToBOOL(ret);
    }

    HLASS  LASSVerifyUserAsync(
             const GUID *AEKey, /* Authenication Event Identifier */
             LPCWSTR pwszAEDisplayText, /*Text Plugin will display, if null use from registry.*/
             HWND   hWndParent, /*Parent Window if Available-else use desktop window*/
             DWORD  dwOptions, /*Bitmask of possible options.*/
             PVOID pExtended /*Reserved, must be 0*/
             )
    {
        ce::auto_handle h = CreateEvent(0,FALSE,FALSE,NULL);
        if (!h.valid())
        {
            // CreateEvent has SetLastError
            return NULL;
        }

        const DWORD ret = m_proxy.call(LASS_VERIFY_USER_ASYNC,
                AEKey,
                pwszAEDisplayText,
                hWndParent,
                dwOptions,
                pExtended,
                reinterpret_cast<ce::PSL_HANDLE>(static_cast<HANDLE>(h)));
        if (ret == NO_ERROR)
        {
            // No Error, return the handle.
            return h.release();
        }

        SetLastError(ret);
        return NULL;
    }

    BOOL  LASSGetResult(
            HLASS hLASS
             )
    {
        const DWORD ret  = m_proxy.call(LASS_GET_RESULT, reinterpret_cast<ce::PSL_HANDLE>(hLASS));
        return DWORDReturnToBOOL(ret);
    }


    BOOL  LASSClose(
            HLASS hLASS
             )
    {
        CloseHandle(hLASS);
        const DWORD ret = m_proxy.call(LASS_CLOSE,reinterpret_cast<ce::PSL_HANDLE>(hLASS));
        return DWORDReturnToBOOL(ret);
    }

    BOOL  LASSReloadConfig()
    {
        const DWORD ret = m_proxy.call(LASS_RELOAD);
        return DWORDReturnToBOOL(ret);
    }
    BOOL  LASSEnroll(HWND h)
    {
        const DWORD ret = m_proxy.call(LASS_ENROLL,h);
        return DWORDReturnToBOOL(ret);
    }

private:
    BOOL DWORDReturnToBOOL(const DWORD ret)
    {
        if (ret == NO_ERROR)
        {
            return TRUE;
        }
        SetLastError(ret);
        return FALSE;
    }
    ce::psl_proxy<> m_proxy;

};
