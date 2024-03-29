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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

Abstract:


Notes: 
--*/
#ifndef __DEFBUS_H_
#define __DEFBUS_H_
#define DBG_DEVICE_ERROR     0x1
#define DBG_DEVICE_INIT      0x2
#define DBG_DEVICE_ENUM      0x4

#include <windows.h>
#include <CeBusCfg.h>
#include <CeDDK.h>
#include <CRefCon.h>
#include <CRegEdit.h>
#include <CSync.h>

#define MAX_INIT_REG  8
class DeviceFolder :public CRefObject,public CRegistryEdit,public CLockObject  {
public :
    DeviceFolder(LPCTSTR lpBusName,LPCTSTR lpTemplateRegPath,DWORD dwBusType, DWORD BusNumber,DWORD DeviceNumber,DWORD FunctionNumber,HANDLE hParent,DWORD dwMaxInitReg=MAX_INIT_REG,LPCTSTR lpDeviceBusName = NULL);
    virtual BOOL Init() { return (m_dwInitRegArray != NULL); }
    virtual ~DeviceFolder();
    DWORD   GetLoadOrder() {  return m_dwLoadOrder; };
    DWORD   GetLoadFlag() {  return m_dwLoadFlag; };
    BOOL    AddInitReg( int nCount, REGINI *pReg );
    virtual BOOL  LoadDevice();
    virtual BOOL  UnloadDevice();
    BOOL    IsDriverLoaded() { return m_fDriverLoaded; };
    
    // Public Function.
    LPCTSTR GetDeviceBusName() { return m_lpDeviceBusName; };
    virtual BOOL SetPowerState(CEDEVICE_POWER_STATE newPowerState) { 
        m_CurPowerState = newPowerState;
        return TRUE;
    }
    virtual CEDEVICE_POWER_STATE  GetPowerState() { return m_CurPowerState; };

    virtual BOOL SetConfigurationData(PCE_BUS_DEVICE_CONFIGURATION_DATA pDeviceConfigData);
    virtual BOOL GetConfigurationData(PCE_BUS_DEVICE_CONFIGURATION_DATA pDeviceConfigData);
    
    //
    DeviceFolder * GetNextDeviceFolder() { return m_pNextFolder; };
    LPCTSTR GetRegPath() { return m_lpTemplateRegPath; };
    void SetNextDeviceFolder(DeviceFolder * pDevice) { m_pNextFolder = pDevice; };
    HANDLE  GetDeviceHandle () { return m_hDevice; };
    DWORD GetDeviceNumber() { return m_DeviceNumber; };
    DWORD GetFunctionNumber() { return m_FunctionNumber; };
    DWORD   GetBusType() { return m_dwBusType; };
    DWORD   GetBusNumber() { return m_BusNumber; };
#ifdef DEBUG
    virtual DWORD DebugZoneChk(DWORD dwDeviceZone);
#endif
protected:
    CEDEVICE_POWER_STATE m_CurPowerState;
private:
    HANDLE  m_hDevice;
    BOOL    m_fDriverLoaded;
    DWORD   m_dwBusType;
    DWORD   m_BusNumber;
    DWORD   m_DeviceNumber;
    DWORD   m_FunctionNumber;
    DWORD   m_dwLoadOrder;
    DWORD   m_dwLoadFlag;
    
    const DWORD   m_dwMaxInitReg;
    DWORD   m_dwInitRegCount;
    REGINI  *m_dwInitRegArray;
    LPTSTR m_lpDeviceBusName;
    LPTSTR m_lpTemplateRegPath;
    HANDLE m_hParent;
    DeviceFolder * m_pNextFolder;
};

class DefaultBusDriver :  CLockObject {
public:
    DefaultBusDriver(LPCTSTR lpActiveRegPath);
    virtual BOOL Init();
    virtual BOOL PostInit() { return TRUE; };
    virtual ~DefaultBusDriver();
    virtual BOOL TranslateChildBusAddr(PCE_BUS_TRANSLATE_BUS_ADDR);
    virtual BOOL TranslateChildSystemAddr(PCE_BUS_TRANSLATE_SYSTEM_ADDR);
    virtual BOOL SetChildDevicePowerState(PCE_BUS_POWER_STATE,DeviceFolder **ppDeviceFoler = NULL ) ;
    virtual BOOL GetChildDevicePowerState(PCE_BUS_POWER_STATE,DeviceFolder **ppDeviceFoler = NULL) ;
    
    virtual BOOL SetChildDeviceConfigurationData(PCE_BUS_DEVICE_CONFIGURATION_DATA,DeviceFolder **ppDeviceFoler  = NULL );
    virtual BOOL GetChildDeviceConfigurationData(PCE_BUS_DEVICE_CONFIGURATION_DATA,DeviceFolder **ppDeviceFoler = NULL);
    virtual BOOL ActivateChild(LPCTSTR lpChildBusName);
    virtual BOOL DeactivateChild(LPCTSTR lpChildBusName);

    virtual BOOL PowerUp() { return TRUE;};
    virtual BOOL PowerDown(){ return TRUE;};
    virtual BOOL  Open( DWORD   AccessCode,   DWORD   Share) ;
    virtual BOOL  Close() { return TRUE; };
    virtual BOOL IOControl(DWORD dwCode, PBYTE pBufIn, DWORD dwLenIn, PBYTE pBufOut, DWORD dwLenOut,
              PDWORD pdwActualOut) {
        return FastIOControl(dwCode,pBufIn,dwLenIn, pBufOut, dwLenOut, pdwActualOut, NULL) ;
    }

    virtual DeviceFolder * GetChildByName(LPCTSTR lpChildName, DeviceFolder **ppDeviceFoler = NULL );
    virtual BOOL InsertChild( DeviceFolder * pDeviceFolder);
    virtual BOOL RemoveChildByName(LPCTSTR lpChildName);
    virtual BOOL RemoveChildByFolder(DeviceFolder * pDeviceFolder);


    virtual BOOL IsChildRemoved(LPCTSTR lpChildName) { return FALSE; }; // Can not be removed for default driver.
    
    // Returns the count of characters including the NULL terminator.
    virtual DWORD GetBusNamePrefix(LPTSTR lpReturnBusName,DWORD dwSizeInUnit);
    //
    HANDLE GetDeviceHandle() { return m_hDevice; };
    DeviceFolder * GetDeviceList () { return  m_pDeviceList; };

    // Bus Access Function.
    HANDLE GetParentBusHandle() { return m_hParent; };
    BOOL SetDevicePowerState(CEDEVICE_POWER_STATE PowerState) {
        return (m_hParent!=0?(::SetDevicePowerState(m_hParent,PowerState,NULL)):FALSE);
    }
    BOOL GetDevicePowerState(PCEDEVICE_POWER_STATE pPowerState) {
        return (m_hParent!=0?(::GetDevicePowerState(m_hParent, pPowerState, NULL)):FALSE) ;
    }
    BOOL TranslateBusAddr(INTERFACE_TYPE  InterfaceType,ULONG BusNumber,PHYSICAL_ADDRESS BusAddress,PULONG AddressSpace,PPHYSICAL_ADDRESS TranslatedAddress ) {
        if (m_hParent!=0)
            return ::TranslateBusAddr(m_hParent,InterfaceType,BusNumber,BusAddress,AddressSpace,TranslatedAddress );
        else
            return HalTranslateBusAddress(InterfaceType,BusNumber,BusAddress,AddressSpace, TranslatedAddress);
    }
    BOOL TranslateSystemAddr(INTERFACE_TYPE  InterfaceType,ULONG BusNumber,PHYSICAL_ADDRESS SystemAddress,PPHYSICAL_ADDRESS TranslatedAddress ) {
        if (m_hParent!=0)
            return ::TranslateSystemAddr(m_hParent,InterfaceType,BusNumber,SystemAddress,TranslatedAddress );
        else
            return HalTranslateSystemAddress(InterfaceType,BusNumber,SystemAddress, TranslatedAddress);
    }
    BOOL SetDeviceConfigurationData(DWORD dwSpace,DWORD BusNumber, DWORD SlotNumber,DWORD dwOffset, DWORD dwLength, PVOID pBuffer) {
        if (m_hParent!=0)
            return ::SetDeviceConfigurationData(m_hParent,dwSpace,BusNumber,SlotNumber,dwOffset,dwLength, pBuffer) ;
        else 
        if ( dwSpace == PCI_WHICHSPACE_CONFIG || dwSpace == PCCARD_PCI_CONFIGURATION_SPACE)
            return HalSetBusDataByOffset(PCIConfiguration,BusNumber,SlotNumber,pBuffer,dwOffset,dwLength);
        else 
            return FALSE;
    }
    BOOL GetDeviceConfigurationData(DWORD dwSpace,DWORD BusNumber, DWORD SlotNumber,DWORD dwOffset, DWORD dwLength, PVOID pBuffer) {
        if (m_hParent!=0)
            return ::GetDeviceConfigurationData(m_hParent, dwSpace,BusNumber,SlotNumber,dwOffset,dwLength,pBuffer);
        else
        if ( dwSpace == PCI_WHICHSPACE_CONFIG || dwSpace == PCCARD_PCI_CONFIGURATION_SPACE)
            return HalGetBusDataByOffset(PCIConfiguration,BusNumber,SlotNumber,pBuffer,dwOffset,dwLength);
        else 
            return FALSE;
    }
    BOOL GetParentDeviceInfo(PDEVMGR_DEVICE_INFORMATION pDeviceInfo) {
        return ::GetParentDeviceInfo(m_hParent, pDeviceInfo);
    }
    virtual BOOL FastIOControl(DWORD dwCode, PBYTE pBufIn, DWORD dwLenIn, PBYTE pBufOut, DWORD dwLenOut,
              PDWORD pdwActualOut,DeviceFolder **ppDeviceFoler );

    void TakeReadLock() {
        Lock();
        m_dwReadRef++;
        Unlock();
    }
    void LeaveReadLock() {
        Lock();
        if (m_dwReadRef != 0)
            m_dwReadRef--;
        Unlock();
    }
    void TakeWriteLock() {
        Lock();
        while (m_dwReadRef!=0) {
            Unlock();
            Sleep(10);
            Lock();
        }
    }
    void LeaveWriteLock() {
        Unlock();
    }
    
private:
    CEDEVICE_POWER_STATE m_LastDx;
    DeviceFolder * m_pDeviceList;
    HANDLE  m_hDevice;
    HANDLE  m_hParent;
    volatile DWORD   m_dwReadRef;
};

#endif

