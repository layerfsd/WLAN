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
/**************************************************************/
//
//  radiopwr.h: structures and functions for managing radio
//  devices
//
//
/**************************************************************/

#pragma once

// Types of radio device
typedef enum _RADIODEVTYPE
{
    POWER_MANAGED = 1,
    POWER_PHONE,
    POWER_BLUETOOTH,
} RADIODEVTYPE;

// whether to save before or after changing state
typedef enum _SAVEACTION
{
    POWER_DONT_SAVE = 0,
    POWER_PRE_SAVE,
    POWER_POST_SAVE,
} SAVEACTION;

// Flags for GetWirelessDevices 
#define WIRELESS_GET_DESIRED    1       // Read the stored states from the registry whilst enumerating devices
#define WIRELESS_NO_POWER_MAN   2       // DON'T Look for powermanagaed devices
#define WIRELESS_NO_PHONE       4   	// DON'T check the phone device


// Details of radio devices
struct RDD 
{
    RDD() : pszDeviceName(NULL), pNext(NULL), pszDisplayName(NULL) {}
    ~RDD() { LocalFree(pszDeviceName); LocalFree(pszDisplayName); }
    LPTSTR   pszDeviceName;  // Device name for registry setting.
    LPTSTR   pszDisplayName; // Name to show the world
    DWORD    dwState;        // ON/off/[Discoverable for BT]
    DWORD    dwDesired;      // desired state - used for setting registry etc.
    RADIODEVTYPE    DeviceType;         // Managed, phone, BT etc.
    RDD * pNext;    // Next device in list
}; //radio device details


#define THREAD_TIMEOUT  30000 // 30 seconds 

#define WRLS_TERMINATE_EVENT TEXT("WRLS_TERMINATE_EVENT")

// Flags to pass to the notify callback 
#define WLSCB_EMPTY 1       // The message queue is now empty. This may accompany one or none of the following flags
#define WLSCB_ADD   2       // device has been added
#define WLSCB_REMOVE 4       // device has been removed

typedef HRESULT (*PFN_WRLSPOWERSTATUSCHANGE )(RDD *pRD, DWORD dwFlags, LPARAM lParam);

struct WRLSNOTIFY
{
    PFN_WRLSPOWERSTATUSCHANGE pfnCallback;
    BOOL    bGetDesired;
    LPARAM  lParam;
};

#ifdef __cplusplus
extern "C"{
#endif 



// Exported functions

// retrieves the actual state of the phone radio
BOOL GetPhoneState(BOOL *pbState);  

// checks a list of devices to see if any are on
BOOL AnyDevicesOn(RDD *pRootDevice);

// gets the desired state of a device from the registry
HRESULT GetRadioStoredState(RDD*  pDev, DWORD *dwOn);

// updates the desired state of a device in the registry
HRESULT UpdateRadioStoredState(RDD*  pDev, DWORD dwState);

// sets a wireless device's state, optionally saving the desired state before or after the change
HRESULT ChangeRadioState(RDD* pDev, DWORD dwState, SAVEACTION sa);

// Saves the overall 'wireless on/off' state to the registry
HRESULT SaveWirelessState(BOOL bState);

// retrieves the overall 'wireless on/off' state from the registry
HRESULT GetWirelessState(BOOL *bState);

// returns a linked list of the wireless devices in the system. Optionally populates the desired states.
HRESULT GetWirelessDevices(RDD **pDevices, DWORD dwFlags);

// As GetWireless devices, but maintains a thread to monitor wireless devices
HRESULT GetAndMonitorWirelessDevices(RDD **pDevices, DWORD dwFlags, WRLSNOTIFY *pNotify, HANDLE *rghThread);

// Switches the overall wireless state to 'on' or 'off', changing the devices accordingly. When switching on, 
// devices are set to their desired state stored in the registry. 
HRESULT ChangeWirelessState(BOOL bOn);

// checks if any devices in a list are on.
BOOL AnyDevicesOn(RDD *pRootDevice);

// frees a list of radio devices
void FreeDeviceList(RDD *pRoot);

// Gets collective name for a list of wireless devices
HRESULT GetNameForWirelessDevices(RDD *pDevices, LPTSTR *pName);

// Power down any wireless that should be powered down before device power off.
HRESULT SetPowerDownWirelessState();

//
// OnRadioPowerTransition
//
// Purpose: This function allows the OEM to receive notification
//          of a radio power transition before the change is
//          executed by the wireless power manager. This function
//          will be executed in a synchronous context within the shell. 
//
//          If the function is not implemented, fails, or *pfUseDefaultProcessing
//          == TRUE the shell will take normal action based on RadioType
//          and dwNewState. If *pfUseDefaultProcessing == FALSE the shell 
//          will assume that the OEM has taken all action necessary and will 
//          not take any action. 
//
//          To that effect this function can be used in an informational
//          manner or it can be used to allow the OEM to override default
//          wireless power management. 
//
// Setup:   [HKLM\System\CurrentControlSet\Control\Power\OnRadioPowerTransition]
//          "DLLName"=<The DLL that the function will be loaded by name from>
//
//          The power manager will attempt to load OnRadioPowerTransition by name
//          from the DLL referenced in the registry key above. If successful the
//          function will be called before each power state change. 
//
// Note:    This function executes in a synchronous context. It is important to return
//          from this function as soon as possible. 
//
// Inputs:  RadioType -                 The type of radio.
//
//          szDeviceName -              The friendly name of the device. 
//
//          dwNewState -                The new state that is being requested. This is
//                                      dependent on the RadioType. 
//
//          pfUseDefaultProcessing -    If TRUE the shell will proceed with default
//                                      processing. If FALSE the shell will assume
//                                      that the OEM has taken all necessary action for
//                                      this power transition.
//
// Return:  Failure indicates that default processing will be used by the
//          shell. 
//

HRESULT OnRadioPowerTransition(RADIODEVTYPE RadioType, 
                               const TCHAR * szDeviceName,
                               DWORD dwNewState,
                               __out BOOL * pfUseDefaultProcessing
                               );

typedef HRESULT (* PFN_ONRADIOPOWERTRANSITION) (RADIODEVTYPE RadioType, 
                                                const TCHAR * szDeviceName,
                                                DWORD dwNewState,
                                                __out BOOL * pfUseDefaultProcessing
                                                );

#define NAME_ONRADIOPOWERTRANSITION TEXT("OnRadioPowerTransition")

#ifdef __cplusplus
}
#endif
