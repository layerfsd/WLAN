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

#include <windows.h>
interface IBTDevice;

//initializes the listview containing the discovered
//or bonded bluetooth devices. The listview items will 
//display an icon representing their type
BOOL InitDevicesListView(
    HINSTANCE hInstResources,
    UINT      idBmpDevices,
    HWND hwndLV
    );

//Cleanup resources associated with the devices listview
BOOL UninitDevicesListView(
    HWND hwndLV
    );

//Adds a device to the listview, by adding its name and an 
//icon specified by the device's "class of device" type. 
//Also stores a user-defined LPARAM in the list with the item.
INT AddDeviceToList(
    HWND hwndList, 
    IBTDevice * pDevice, 
    LPARAM lParam
    );

//fills the listview containing the list of services 
//the specified device supports
//NOTE: you must call ClearServicesListView() to free resources
//associated with this api
BOOL FillServicesListView(
    HWND hwndLV,
    BOOL bNewBond,
    IBTDevice *piDevice
    );

//clears the services list view and frees resources associated
//with FillServicesListView
BOOL ClearServicesListView(
    HWND hwndServicesList
    );

//enables/disables services according to whether the user has
//selected/unselected the item's checkbox.
HRESULT SaveServicesListSettings(
    HWND hwndServicesList
    );

//Refreshes the list of services supported by the bonded device.
//NOTE: Currently this is a blocking (synchronous) api - because the underlying 
//bthutil API (IBTDevice::QueryServices) is not-thread safe and 
//the query time is relatively short (< 2 seconds). If the blocking UI becomes
//an issue we can break QueryServices apart into an asyncronous function.
HRESULT RefreshServicesFromDevice(
    IBTDevice * piDevice,
    BOOL bNewBond,
    HWND hwndServicesList
    );

// enables the default services for the bonded device.  This should be called
// after sdp query for the services has occured
HRESULT EnableDefaultServices(
    IBTDevice *pDevice
    );


