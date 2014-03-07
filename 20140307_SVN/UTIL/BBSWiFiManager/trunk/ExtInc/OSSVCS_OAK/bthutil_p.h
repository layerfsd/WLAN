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

#include <bt_api.h>

#define KNOWN_SERVICE(c)        ((c) < (NUM_SERVICES - 1))
#define CHANNEL_NOT_USED        0xfc
#define UNKNOWN_SERVICE         (NUM_SERVICES - 1)
#define BTH_MAX_NAME            32
#define BTH_MAX_SERVICE_NAME    32
#define DEVICE_MAX              40
#define MAX_PIN_LENGTH          16
#define LV_IMAGE_WIDTH          20
#define MAX_LOC_STRING          256

//Possible results of 'Check for dupes' 
typedef enum _BTN {
    BTN_DUPLICATE,      // Duplicate Name Exists
    BTN_UNIQUE,         // Duplicate Name doesn't exist
    BTN_THIS,           // pDevice already has szName
} BTN;

typedef void (*TONBTHDEVICEADDED)(const BT_ADDR*, const GUID*, LPVOID);
typedef void (*TONBTHDEVICEREMOVED)(const BT_ADDR*, const GUID*, LPVOID); 

// {D567B104-3B4D-4c38-A66B-281302C965A2}
DEFINE_GUID(CLSID_BTService, 0xd567b104, 0x3b4d, 0x4c38, 0xa6, 0x6b, 0x28, 0x13, 0x2, 0xc9, 0x65, 0xa2);
// {0F2F1173-FB86-4244-BCD3-35B7E2E0D418}
DEFINE_GUID(IID_IBTService, 0xf2f1173, 0xfb86, 0x4244, 0xbc, 0xd3, 0x35, 0xb7, 0xe2, 0xe0, 0xd4, 0x18);

DECLARE_INTERFACE_(IBTService, IUnknown)
{
    // IBTService Interface
    STDMETHOD(Init(WCHAR *szName, GUID* ServiceUUID, BYTE bChannel, LPBLOB pSDPRecord)) PURE;
    STDMETHOD(Save(void)) PURE;
    STDMETHOD(NotifyExtensions(void)) PURE;
    STDMETHOD(Load(GUID* pServiceUUID)) PURE;

    // Accessors
    STDMETHOD(GetName( LPWSTR szDeviceName, DWORD dwMaxLen)) PURE;
    STDMETHOD_(void, GetServiceUUID( GUID* pServiceUUID)) PURE;
    STDMETHOD_(BYTE, GetChannel(void)) PURE;
    STDMETHOD(GetSDPRecord( LPBLOB pSDPBlob)) PURE;

    STDMETHOD(WriteCustomProperty(LPWSTR szPropertyName, VARIANT vValue)) PURE;
    STDMETHOD(ReadCustomProperty(LPWSTR szPropertyName, LPVARIANT pvValue)) PURE;

    STDMETHOD_(BOOL, IsEnabled()) PURE;
    STDMETHOD(Enable(BOOL bEnable)) PURE;
};

// {0C6E60A4-3BFC-4c87-AB94-B6F2D02AC466}
DEFINE_GUID(CLSID_BTServiceArray, 0xc6e60a4, 0x3bfc, 0x4c87, 0xab, 0x94, 0xb6, 0xf2, 0xd0, 0x2a, 0xc4, 0x66);
// {8BAE7A04-0E3A-462d-BC32-26B47014B5DF}
DEFINE_GUID(IID_IBTServiceArray, 0x8bae7a04, 0xe3a, 0x462d, 0xbc, 0x32, 0x26, 0xb4, 0x70, 0x14, 0xb5, 0xdf);
DECLARE_INTERFACE_(IBTServiceArray, IUnknown)
{
    //IBTServiceArray Interface
    STDMETHOD_(UINT, GetCount()) PURE;
    STDMETHOD(GetServiceAt(UINT idxService, IBTService **ppiService)) PURE;
};

// {88370D7C-A531-4896-BF61-A9FA2FEF1871}
DEFINE_GUID(CLSID_BTDevice, 0x88370d7c, 0xa531, 0x4896, 0xbf, 0x61, 0xa9, 0xfa, 0x2f, 0xef, 0x18, 0x71);
// {96EA0C3D-560C-4146-B0A2-BBD2E89D24DB}
DEFINE_GUID(IID_IBTDevice, 0x96ea0c3d, 0x560c, 0x4146, 0xb0, 0xa2, 0xbb, 0xd2, 0xe8, 0x9d, 0x24, 0xdb);
DECLARE_INTERFACE_(IBTDevice, IUnknown)
{
    // IBTDevice Interface
    STDMETHOD(Init(const BT_ADDR* pBA, DWORD dwCOD, WCHAR *pszName, BOOL fTrusted)) PURE;
    STDMETHOD(Rename(LPCWSTR szDeviveName)) PURE;
    STDMETHOD_(void, SetTrust(BOOL fTrusted)) PURE;

    STDMETHOD(LoadByName(LPCWSTR pszName)) PURE;
    STDMETHOD(Load(const BT_ADDR *b)) PURE;
    STDMETHOD(Save(void)) PURE;
    STDMETHOD_(void, Delete(void)) PURE;

    STDMETHOD(QueryServices(BOOL bResponse)) PURE;

    // Accessors
    STDMETHOD_(void, GetAddress( BT_ADDR* pBA)) PURE;
    STDMETHOD_(DWORD, GetCOD(void)) PURE;
    STDMETHOD_(BOOL, IsTrusted()) PURE;
    STDMETHOD(GetName(LPWSTR szDeviceName, DWORD dwMaxLen)) PURE;
    STDMETHOD(GetService(const GUID* pServiceUUID, IBTService** ppService)) PURE;

    STDMETHOD(GetAllServices(IBTServiceArray **ppiArray)) PURE;
};


#ifdef __cplusplus
extern "C"{
#endif 
int BthSetLocalName( LPWSTR pwszLocalName );
BOOL BthInitiateBonding( BT_ADDR *pBA, LPCTSTR tszPIN );
BOOL BthAnswerBondingReq( BT_ADDR *pBA, LPCTSTR tszPIN );

// Last Param is a filter on UUID
HRESULT GetDeviceList (IBTDevice **ppBTList, int * pNDevices, const GUID* pServiceUUID);
HRESULT DoInquiry(IBTDevice **pDev, WORD * pwNumDevices, BOOL *pfStopSearch, HWND hWindow, UINT uiMessage, BOOL fAllocate);
HRESULT CancelInquiry();
HRESULT BthSuppressRetry();

HRESULT GetDeviceByName(const TCHAR *pszName, IBTDevice ** pDevice);
HRESULT GetDeviceByAddr(const BT_ADDR *b, IBTDevice ** pDevice);

////////////////////////////////////////////////////////////////////////////////
//
// @func    int | BthGetCachedHardwareStatus |
//          Retrieves the current Hardware state as cached by the bthlink layer. 
//            Identical return values to BthGetHardwareStatus, but returns much
//            faster as the results are cached.
//
// @syntax  BthGetCachedHardwareStatus( int* piStatus )
//
// @parm    DWORD* | piStatus |
//          HCI_HARDWARE_UNKNOWN                                    
//            HCI_HARDWARE_NOT_PRESENT                
//            HCI_HARDWARE_INITIALIZING           
//            HCI_HARDWARE_RUNNING            
//            HCI_HARDWARE_SHUTDOWN           
//            HCI_HARDWARE_ERROR    
//
// @rdesc   int: ERROR_SUCCESS on success.  Error code describing error on failure.
//
////////////////////////////////////////////////////////////////////////////////

int BthGetCachedHardwareStatus( int* piStatus );


////////////////////////////////////////////////////////////////////////////////
//
// @func    int | BthDeviceChanged |
//          Tells bthlink layer that a device has been inserted or removed, so to
//          update its cached hardware status.

// @syntax  BthDeviceChanged(void)
//
//
// @rdesc   int: ERROR_SUCCESS on success.  Error code describing error on failure.
//
////////////////////////////////////////////////////////////////////////////////

int BthDeviceChanged(void);
BOOL BthAGSetAsDefaultHandsfree(BT_ADDR* pBA, const GUID* pServiceGUID);
typedef BOOL (*PFN_PlatformPrompt) (HWND hwndParent, const TCHAR* pszDeviceName); 
HRESULT BthAGFindDeviceGUID(IBTDevice* pDevice, GUID* pGuid, BT_ADDR* pAddr);

/////////////////////////////////////////////////
// OEMSetBthPowerState
//
// Allows the OEM to hook a 3rd party Bluetooth Stack into
// the device Flight Mode code. See OEM extensibility documents
// for details.
#define OEM_SET_BTH_POWER_STATE_NAME TEXT("OEMSetBthPowerState")
typedef HRESULT (* OEM_SET_BTH_POWER_STATE_FUNC) (DWORD dwState);

/////////////////////////////////////////////////
// OEMGetBthPowerState
//
// Allows the OEM to hook a 3rd party Bluetooth Stack into
// the device Flight Mode code. See OEM extensibility documents
// for details.

#define OEM_GET_BTH_POWER_STATE_NAME TEXT("OEMGetBthPowerState")
typedef HRESULT (* OEM_GET_BTH_POWER_STATE_FUNC) (DWORD * pdwCurrentState);

#ifdef __cplusplus
}
#endif


