#ifndef __Radioctrl__
#define __Radioctrl__

#include "Wrlspwr.h"
#include "Connmgr.h"
#include "Connmgr_status.h"
#include "Definitions.h"
extern "C" {
#include "NAT.h"
}
#define STATUS_GPRS_BASE			 0x90000000
#define STATUS_NAT_BASE				 0x90001000
#define STATUS_WIFI_BASE			 0x90002000

#define STATUS_NO_PHONE			  STATUS_GPRS_BASE+4
#define STATUS_GPRS_QUERYFAILURE  STATUS_GPRS_BASE+1
#define STATUS_GPRS_GUIDERROR	  STATUS_GPRS_BASE+2
#define STATUS_GPRS_CONNECTFAILRE STATUS_GPRS_BASE+3
#define STATUS_GPRS_INVALIDPARAM  STATUS_GPRS_BASE+0x57

#define STATUS_NAT_DEVICE_ERROR   STATUS_NAT_BASE+1
#define STATUS_NAT_DRIVER_ERROR   STATUS_NAT_BASE+2
#define STATUS_NAT_FAILURE		  STATUS_NAT_BASE+3

#define STATUS_WIFI_NO_DEVICE		STATUS_WIFI_BASE+0x10
#define	STATUS_WIFI_FUNC_ERROR		STATUS_WIFI_BASE+0x11
#define STATUS_WIFI_LOAD_FUNC_ERROR STATUS_WIFI_BASE+0x12
#define STATUS_WIFI_CONNECTFAILURE	STATUS_WIFI_BASE+0x13

#if 0 //Not used
#define PrivateValue				L"AR6K_SD1"
#define PublicValue			        L"WWAN1-Internet-1"
#define PrivateKey					L"PrivateInterface"
#define PublicKey					L"PublicInterface"
#define SharingKey					L"comm\\ConnectionSharing"
#define Ar6kTcpIpKey				L"comm\\AR6K_SD1\\Parms\\Tcpip"
#define Ar6kDHCPKey				    L"EnableDHCP"
#define Ar6KAutocfgkey			    L"Autocfg"
#define Ar6kGWKey				    L"DefaultGateWay"
#define Ar6kIPKey					L"IPAddress"
#define Ar6kSubMaskKey              L"SubnetMask"
#define	Ark6DNS						L"DNS"
#define Ar6kWINS					L"WINS"
#define DefaultIP					L"192.168.0.1"
#define DefaultMask					L"255.255.255.0"
#define DefaultGW					L"0.0.0.0"
#define InternalKey					"comm\\ConnectionSharing\\InternalExpostedHost"
#endif

#define WlanOn  1
#define WlanOff 0
#define GPRS_CONNECT    0
#define WIFI_CONNECT    1
#define GPRS_DISCONNECT	2
#define WIFI_DISCONNECT 4

#define CONNECTED		1
#define DISCONNECTED    0

#define GetWlanDevice_FunctionID 276
#define ChangeWlanState_FunctionID 273
#define FreeWlanList_FunctionID 280

typedef HRESULT (* GetWlanDevices)(RDD **pDevices, DWORD dwFlags);
typedef HRESULT (* ChangeWlanPowerState)(RDD* pDev, DWORD dwState, SAVEACTION sa);
typedef HRESULT (* FreeWlanList)(RDD *pRoot);


typedef struct _ConnectionCtrl{
    //WiFi Connection Control
    HANDLE       Handler;
    TCHAR      ConnectionName[256];
    DWORD        ConnectState;
}ConnectionCtrl, *pConnectionCtrl;


typedef struct _radiodevctrl{
    //WiFI Power control
    HMODULE      dllhandler;
    GetWlanDevices          fnGetWlanDev;
    ChangeWlanPowerState    fnChangeWlanPowerState;
    FreeWlanList            fnFreeWlanList;
}RadioDeviceCtrl,*pRadiodeviceCtrl;


void UnloadWlanFunction(pRadiodeviceCtrl WrAdapter);
bool LoadWlanFunction(pRadiodeviceCtrl WrAdapter);
DWORD WlanEnableDisable(pRadiodeviceCtrl WrAdapter, DWORD dwState);
DWORD RadioConnectDisConnect(pConnectionCtrl pGPRSAdapter,DWORD Flag);

#if 0 //Functions not used
DWORD GPRSThread(LPVOID Context);
HRESULT WiFiCreateProfile(LPWSTR SsidName);
HRESULT WiFiRemoveProfile(LPWSTR SsidName);
#endif //Functions not used

DWORD WiFiOnOff(DWORD Flag);
BOOL WiFiIPConfig(BOOL bEnable, DWORD *pdwPrev);//DWORD WiFiConfigIP(DWORD Flag,DWORD *Previous);//0 Reset,1:SetDefault
DWORD WiFiCheckConnection(pConnectionCtrl pWiFiAdapter);

BOOL SetInterface(TCHAR *pszName, TCHAR *pszValue);
DWORD CheckDevice();
DWORD QueryConnection(DWORD QueryType,DWORD *dwConnectState,TCHAR *ConnectName);
BOOL PublicIPConfig(TCHAR *pszPublicInterface, BOOL bEnable);

BOOL AppRegSetDword (TCHAR *pszName, DWORD dwValue);
BOOL AppRegGetDword (TCHAR *pszName, DWORD *pdwValue, DWORD dwDefault);
BOOL AppRegSetString(TCHAR *pszName, TCHAR *pszValue);
BOOL AppRegGetString(TCHAR *pszName, TCHAR *pszValue, TCHAR *pszDefault);
#if 0 //Functions replaced 
DWORD WiFiDHCPEnableDisable(DWORD Flag);//Replaced by BOOL EnableDHCP(BOOL bEnable) - NAT.c
void SetPrivateInterface();//Replaced by SetInterface("", "") - WiFiCtrl.cpp
void SetPublicInterface();//Replaced by SetInterface("", "") - WiFiCtrl.cpp
void RemoveInterface();//Replaced by SetInterface("", "") - WiFiCtrl.cpp
DWORD IpRouterEnableDisable(BOOLEAN bflag);//Replaced by EnableIPRouiting() - NAT.c
DWORD GPRSIPConfig(DWORD Flag);//Replaced by PublicIPConfig() - WiFiCtrl.cpp
#endif //Functions replaced 

#endif


