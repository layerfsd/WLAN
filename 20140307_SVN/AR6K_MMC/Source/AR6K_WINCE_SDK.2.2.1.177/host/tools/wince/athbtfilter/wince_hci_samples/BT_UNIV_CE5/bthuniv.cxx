//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
//------------------------------------------------------------------------------
// 
//      Bluetooth HCI interface
// 
// 
// Module Name:
// 
//      bthuniv.cxx
// 
// Abstract:
// 
//      This file implements a universal transport manager for bluetooth.
//		It forwards calls to the real driver. It first looks for PnP devices, and if 
//		doesn't find any it will try the builtin devices in order of preference 
//		listed in the registry.
//		
// 
//------------------------------------------------------------------------------
#include <windows.h>
#include <windev.h>
#include <dbt.h>
#include <usbdi.h>
#include <pnp.h>
#include <Msgqueue.h>

#include <bt_buffer.h>
#include <bt_hcip.h>
#include <bt_os.h>
#include <bt_debug.h>
#include <svsutil.hxx>
#include <bt_tdbg.h>

#define ATHBT_FILTER

#ifdef ATHBT_FILTER
#include "athbtfilter.h"
#endif

typedef int (* HCI_READHCIPARAMETERS)(HCI_PARAMETERS *);
typedef int (* HCI_STARTHARDWARE )(void);
typedef int (* HCI_STOPHARDWARE )(void);
typedef int (* HCI_OPENCONNECTION)(void);
typedef int (* HCI_READPACKET )(HCI_TYPE *, BD_BUFFER *);
typedef int (* HCI_WRITEPACKET )(HCI_TYPE , BD_BUFFER *);
typedef void (* HCI_CLOSECONNECTION )(void);
typedef int (* HCI_SETCALLBACK )(HCI_TransportCallback );

static HCI_READHCIPARAMETERS	pfnReadHciParameters = NULL;
static HCI_STARTHARDWARE		pfnStartHardware = NULL;
static HCI_STOPHARDWARE			pfnStopHardware = NULL;
static HCI_SETCALLBACK			pfnSetCallback = NULL;
static HCI_OPENCONNECTION		pfnOpenConnection = NULL;
static HCI_READPACKET			pfnReadPacket = NULL;
static HCI_WRITEPACKET			pfnWritePacket = NULL;
static HCI_CLOSECONNECTION		pfnCloseConnection = NULL;

static HCI_TransportCallback	pfnCallback = NULL;


static BOOL InitPointers();
static BOOL DeInitPointers();


DECLARE_DEBUG_VARS();

static HINSTANCE	ghInstance = NULL;
static HANDLE		ghStopNotifyThread;
static HANDLE		ghNotifyThread;

static BOOL			gfInitialized = FALSE;
static BOOL			gfStopHardware = FALSE;

static GUID			gIClassGuid;
static WCHAR		gwszDevName[MAX_DEVCLASS_NAMELEN];

HINSTANCE			ghCurDriver;

#ifdef ATHBT_FILTER
static ATH_BT_FILTER_INSTANCE g_AthBtFilterInstance;
#endif

class CSynch : public SVSSynch, public SVSRefObj
{
};

CSynch	*gpSynch = NULL;

enum InCall
{
	eOther,
	eOpen,
	eClose
};
InCall	geInCall = eOther;

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    switch (dwReason) 
    {
    case DLL_PROCESS_ATTACH:
		svsutil_Initialize ();
		SVSUTIL_ASSERT(!gpSynch);
		gpSynch = new CSynch();
		if (!gpSynch)
			return FALSE;

		ghInstance = (HINSTANCE) hinstDLL;
		DisableThreadLibraryCalls((HMODULE) hinstDLL);
        
#ifdef ATHBT_FILTER        
        memset(&g_AthBtFilterInstance,0,sizeof(g_AthBtFilterInstance));
#endif
        
        break;
	
	case DLL_PROCESS_DETACH:
		SVSUTIL_ASSERT(gpSynch);
		SVSUTIL_ASSERT(!gpSynch->IsLocked());
		delete gpSynch;
		gpSynch = NULL;
		svsutil_DeInitialize ();
		break;

    default:
        break;
    }
    return TRUE ;
}


int GetGUID(WCHAR *psz, GUID *pGUID) {
	int data1, data2, data3;
	int data4[8];

	if (11 ==  swscanf(psz, L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
					&data1, &data2, &data3,
					&data4[0], &data4[1], &data4[2], &data4[3], 
					&data4[4], &data4[5], &data4[6], &data4[7])) {
		pGUID->Data1 = data1;
		pGUID->Data2 = data2 & 0xffff;
		pGUID->Data3 = data3 & 0xffff;

		for (int i = 0 ; i < 8 ; ++i)
			pGUID->Data4[i] = data4[i] & 0xff;

		return TRUE;
	}

	return FALSE;
}

static VOID CopyValue(HKEY hkDest, HKEY hkSrc, LPWSTR pszValueName)
{
	WCHAR szValue[256];
	DWORD dwType = 0;
	DWORD dwSize = sizeof(szValue);
	
	if (!hkDest || !hkSrc || !pszValueName || !(*pszValueName))
		return;

	if (ERROR_SUCCESS == RegQueryValueEx(hkSrc, pszValueName, NULL, &dwType, (LPBYTE )szValue, &dwSize))
	{
		LONG lRet = RegSetValueEx(hkDest, pszValueName, 0, dwType, (LPBYTE )szValue, dwSize);
		SVSUTIL_ASSERT(lRet == ERROR_SUCCESS);
	}
}

static VOID DeleteValues(HKEY hk)
{
	DWORD dwIndex = 0;
	WCHAR szValueName[256];
	DWORD cbValueName = sizeof(szValueName) / sizeof(WCHAR);
	DWORD dwType = 0;

	if (!hk)
		return;

	while(ERROR_SUCCESS==RegEnumValue(hk, 0, szValueName, &cbValueName, NULL, &dwType, NULL, NULL))
	{
		LONG lRet = RegDeleteValue(hk, szValueName);
		SVSUTIL_ASSERT(lRet == ERROR_SUCCESS);
		cbValueName = sizeof(szValueName) / sizeof(WCHAR);
	}
}

static VOID CopyValues(HKEY hkDest, HKEY hkSrc)
{
	DWORD dwIndex = 0;
	WCHAR szValueName[256];
	DWORD cbValueName = sizeof(szValueName) / sizeof(WCHAR);
	DWORD dwType = 0;

	while(ERROR_SUCCESS==RegEnumValue(hkSrc, dwIndex++, szValueName, &cbValueName, NULL, &dwType, NULL, NULL))
	{
		CopyValue (hkDest, hkSrc, szValueName);
		cbValueName = sizeof(szValueName) / sizeof(WCHAR);
	}
}

static VOID CopyRegistryEntry(LPWSTR szTo, LPWSTR szFrom, HKEY hkFrom, int fCleanKey)
{
	HKEY hKey1=NULL, hKey2=NULL;
	DWORD dwDisp = 0;

	LONG lRet = RegCreateKeyEx(HKEY_BASE, szTo, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey1, &dwDisp);
	SVSUTIL_ASSERT(lRet == ERROR_SUCCESS);

	lRet = RegOpenKeyEx(hkFrom, szFrom, 0, 0, &hKey2);
	SVSUTIL_ASSERT(lRet == ERROR_SUCCESS);

	if (hKey1 && hKey2)
	{
		if (fCleanKey)
			DeleteValues(hKey1);
		CopyValues (hKey1, hKey2);
	}
	if (hKey1)
		RegCloseKey(hKey1);
	if (hKey2)
		RegCloseKey(hKey2);
}


/*

This function is called by the current transport driver to indicate
"DEVICE_UP" or "DEVICE_DOWN".  This function will call back into HCI
to notify device insertion.

*/
static int TransportCallback(HCI_EVENT iEvent, void *lpEvent) 
{
	int nRet = S_FALSE;
	
	gpSynch->Lock();
	BOOL fInit = gfInitialized;
	HCI_TransportCallback	pfn = pfnCallback;
	BOOL fStopHardware = gfStopHardware;
	HINSTANCE hLib = ghCurDriver;
	gpSynch->AddRef();
	gpSynch->Unlock();

	if ((fInit) && (pfn) && (!fStopHardware) && (!hLib))
		nRet = pfn(iEvent, lpEvent);

	gpSynch->Lock();
	gpSynch->DelRef();
	gpSynch->Unlock();

	return nRet;
}


/*

Thread which listens for device insertions/removals.

*/
static DWORD WINAPI HciUnivNotifyThread(PVOID pv)
{
	HKEY hkTrans = NULL;
	HANDLE ph[MAXIMUM_WAIT_OBJECTS];
	HANDLE phMsgQueue[MAXIMUM_WAIT_OBJECTS-1];
	HANDLE phDevNotif[MAXIMUM_WAIT_OBJECTS-1];	
	DWORD dwIClassSize = MAX_PATH;
	WCHAR szIClass[MAX_PATH];
	DWORD cdwIClass = 0;
	BYTE pPNPBuf[sizeof(DEVDETAIL) + MAX_DEVCLASS_NAMELEN * sizeof(TCHAR)] = {0};
	DEVDETAIL *pd = (DEVDETAIL*)pPNPBuf;

	//
	// Get IClass list from registry and request notifications
	//	

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_BASE, L"Software\\Microsoft\\Bluetooth\\Transports\\PnP", 0, 0, &hkTrans)) {
		IFDBG(DebugOut(DEBUG_HCI_INIT, L"[HCIUNIV] HciUnivNotifyThread - Could not open PnP transport registry key\n"));
		goto exit;
	}

	//
	// Need to clear the present device from the registry before registering for device
	// notifications.  This could happen if the hardware is pulled out while the device
	// was turned off.
	//
	RegDeleteValue(hkTrans, L"Present");
	
	ph[0] = ghStopNotifyThread;
	while(ERROR_SUCCESS == RegEnumKeyEx(hkTrans, cdwIClass, szIClass, &dwIClassSize, NULL, NULL, NULL, NULL))
	{
		MSGQUEUEOPTIONS msgopts;
		GUID guid = {0};

		if (cdwIClass+1 >= MAXIMUM_WAIT_OBJECTS) {
			// Maximum number of IClasses is MAXIMUM_WAIT_OBJECTS - 1
			IFDBG(DebugOut(DEBUG_HCI_TRANSPORT, L"[HCIUNIV] The number of Bluetooth IClass GUIDs has exceeded the universal transport manager's limit.\n"));
			ASSERT(0);
			goto exit;
		}
		
		if (FALSE == GetGUID(szIClass, &guid)) {
			IFDBG(DebugOut(DEBUG_HCI_TRANSPORT, L"[HCIUNIV] HciUnivNotifyThread - Failed reading IClass.\n"));
			goto exit;
		}			

		memset(&msgopts, 0, sizeof(msgopts));
		msgopts.dwFlags = 0;
		msgopts.dwSize = sizeof(msgopts);
		msgopts.dwMaxMessages = 0; 
		msgopts.cbMaxMessage = sizeof(pPNPBuf);
		msgopts.bReadAccess = TRUE;

		phMsgQueue[cdwIClass] = CreateMsgQueue(NULL, &msgopts);
		if (phMsgQueue[cdwIClass] == 0) {
			IFDBG(DebugOut(DEBUG_HCI_TRANSPORT, L"[HCIUNIV] HciUnivNotifyThread - failed to create message queue.\n"));
			goto exit;
		}

		phDevNotif[cdwIClass] = RequestDeviceNotifications(&guid, phMsgQueue[cdwIClass], TRUE);

		ph[cdwIClass+1] = phMsgQueue[cdwIClass];

		dwIClassSize = MAX_PATH;
		cdwIClass++;
	}

	RegCloseKey(hkTrans);
	hkTrans = NULL;

	//
	// Wait for device notifications to fire
	//

	while (1) {
		DWORD dwSize;
		DWORD dwFlags;
		DWORD dwWait;
		DWORD dwQueue;
		
		dwWait = WaitForMultipleObjects(cdwIClass+1, ph, FALSE, INFINITE);
		if ((dwWait < WAIT_OBJECT_0) || (dwWait >= (WAIT_OBJECT_0 + cdwIClass + 1))) {
			IFDBG(DebugOut(DEBUG_HCI_TRANSPORT, L"[HCIUNIV] HciUnivNotifyThread - WaitForMultipleObjects returned failure.\n"));
			break;
		}

		if (dwWait == WAIT_OBJECT_0) {
			// We have been signalled to shut down this thread
			break;
		}

		dwQueue = (dwWait - WAIT_OBJECT_0) - 1;
		
		if (! ReadMsgQueue(phMsgQueue[dwQueue], pd, sizeof(pPNPBuf), &dwSize, 1, &dwFlags)) {
			IFDBG(DebugOut(DEBUG_HCI_TRANSPORT, L"[HCIUNIV] HciUnivNotifyThread - failed to read message queue.\n"));
			break;
		}

		if (pd->fAttached) {
			// Device has been inserted
			
			memcpy(&gIClassGuid, &pd->guidDevClass, sizeof(GUID));
			wcsncpy(gwszDevName, pd->szName, MAX_DEVCLASS_NAMELEN);
			gwszDevName[MAX_DEVCLASS_NAMELEN-1] = 0;

			if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Bluetooth\\Transports\\PnP", 0, 0, &hkTrans)) {
				RegSetValueEx(hkTrans, L"Present", NULL, REG_BINARY, (PBYTE)&gIClassGuid, sizeof(gIClassGuid));
				RegCloseKey(hkTrans);
				hkTrans = NULL;
			}
				
			TransportCallback(DEVICE_UP, NULL);
		}
		else {
			// Device has been ejected
			
			memset(&gIClassGuid, 0, sizeof(GUID));
			gwszDevName[0] = 0;

			if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Bluetooth\\Transports\\PnP", 0, 0, &hkTrans)) {
				RegDeleteValue(hkTrans, L"Present");
				RegCloseKey(hkTrans);
				hkTrans = NULL;
			}
		}
	}

exit:
	
	//
	// Clean up
	//

	for (DWORD i = 0; i < cdwIClass; i++) {
		StopDeviceNotifications(phDevNotif[i]);
		CloseMsgQueue(phMsgQueue[i]);
	}

	if (hkTrans) {
		RegCloseKey(hkTrans);
		hkTrans = NULL;
	}
	
	return 0;
}


/*

This function is called to initialize the functions in the active transport driver.

*/
static BOOL InitPointers()
{
	SVSUTIL_ASSERT(gpSynch->IsLocked());

	SVSUTIL_ASSERT(gfInitialized);

	if (pfnReadHciParameters && pfnStartHardware && pfnStopHardware && pfnOpenConnection && pfnReadPacket &&
			pfnWritePacket && pfnCloseConnection)
		return TRUE;

	SVSUTIL_ASSERT(ghCurDriver);

	pfnReadHciParameters = (HCI_READHCIPARAMETERS )GetProcAddress(ghCurDriver, L"HCI_ReadHciParameters");
	pfnStartHardware = (HCI_STARTHARDWARE )GetProcAddress(ghCurDriver, L"HCI_StartHardware");
	pfnStopHardware = (HCI_STOPHARDWARE	)GetProcAddress(ghCurDriver, L"HCI_StopHardware");
	pfnOpenConnection = (HCI_OPENCONNECTION	)GetProcAddress(ghCurDriver, L"HCI_OpenConnection");
	pfnReadPacket = (HCI_READPACKET	)GetProcAddress(ghCurDriver, L"HCI_ReadPacket");
	pfnWritePacket = (HCI_WRITEPACKET )GetProcAddress(ghCurDriver, L"HCI_WritePacket");
	pfnCloseConnection = (HCI_CLOSECONNECTION )GetProcAddress(ghCurDriver, L"HCI_CloseConnection");
	pfnSetCallback = (HCI_SETCALLBACK )GetProcAddress(ghCurDriver, L"HCI_SetCallback");

	BOOL bRet = pfnReadHciParameters && pfnStartHardware && pfnStopHardware && pfnOpenConnection && pfnReadPacket &&
			pfnWritePacket && pfnCloseConnection;

	if (!bRet)
	{
		IFDBG(DebugOut(DEBUG_ERROR, L"[HCIUNIV] GetProcAddress failed!\n"));
		DeInitPointers();
	}

#ifdef ATHBT_FILTER
    if (bRet) {
        AthBtFilter_Attach(&g_AthBtFilterInstance);  
    }
#endif            
	return bRet;
}


/*

This function is called when unloading the current transport driver.

*/
static BOOL DeInitPointers()
{
	SVSUTIL_ASSERT(gpSynch->IsLocked());

	SVSUTIL_ASSERT(ghCurDriver);

#ifdef ATHBT_FILTER
    AthBtFilter_Detach(&g_AthBtFilterInstance);  
#endif 

	pfnReadHciParameters = NULL;
	pfnStartHardware = NULL;
	pfnStopHardware = NULL;
	pfnOpenConnection = NULL;
	pfnReadPacket = NULL;
	pfnWritePacket = NULL;
	pfnCloseConnection = NULL;

	HINSTANCE hLib = ghCurDriver;
	ghCurDriver = NULL;

	while(gpSynch->GetRefCount() > 1)
	{
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"[HCIUNIV] Waiting to free library 0x%08x\n", hLib));
		gpSynch->Unlock();
		Sleep(500);
		gpSynch->Lock();
	}
	
	SVSUTIL_ASSERT(hLib);
	
	FreeLibrary(hLib);
	
	IFDBG(DebugOut(DEBUG_HCI_TRANSPORT, L"[HCIUNIV] No active device.\n"));

	return TRUE;
}


/*

This function is called when the universal transport manager is initialized.

*/
static BOOL Initialize()
{
	SVSUTIL_ASSERT(gpSynch->IsLocked());

	if (gfInitialized)
		return TRUE;

	IFDBG(DebugOut(DEBUG_HCI_INIT, L"[HCIUNIV] +Initialize...\n"));

	ghCurDriver = NULL;

	ghStopNotifyThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL == ghStopNotifyThread)
		return FALSE;

	ghNotifyThread = CreateThread(NULL, 0, HciUnivNotifyThread, NULL, 0, NULL);
	if (NULL == ghNotifyThread)
		return FALSE;

	gfInitialized = TRUE;

	IFDBG(DebugOut(DEBUG_HCI_INIT, L"[HCIUNIV] -Initialize...\n"));

	return TRUE;
}


/*

This function is called when the universal transport manager is being shut down.

*/
static void UnInitialize()
{
	SVSUTIL_ASSERT(gpSynch->IsLocked());

	if (!gfInitialized)
		return;
	
	SetEvent(ghStopNotifyThread);
	WaitForSingleObject(ghNotifyThread, INFINITE);

	CloseHandle(ghStopNotifyThread);
	CloseHandle(ghNotifyThread);

	if (ghCurDriver)
		DeInitPointers();

	SVSUTIL_ASSERT(gpSynch->IsLocked());

	gfInitialized = FALSE;
}


/*

This funtion is called by HCI_OpenConnection to get the driver name for the current
PnP device (if one exists) and copy over the registry settings for the driver.

*/
static BOOL GetPnPDriver(LPWSTR pszName)
{
	BOOL fRetVal = TRUE;
	HKEY hkTrans = NULL;
	HKEY hkHCI = NULL;
	DWORD dwEnum = 0;
	WCHAR wszKey[MAX_PATH];
	DWORD dwType;
	DWORD dwSize = sizeof(WCHAR)*MAX_PATH;

	GUID ZeroGuid;
	memset(&ZeroGuid, 0, sizeof(GUID));

	if (0 == memcmp(&gIClassGuid, &ZeroGuid, sizeof(GUID))) {
		return FALSE;
	}

	swprintf(wszKey, L"Software\\Microsoft\\Bluetooth\\Transports\\PnP\\{" SVSUTIL_GUID_FORMAT_W L"}" , SVSUTIL_RGUID_ELEMENTS(gIClassGuid));
	
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_BASE, wszKey, 0, 0, &hkTrans)) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"[HCIUNIV] Could not get PnP device for specified IClass\n"));
		ASSERT(0);
		fRetVal = FALSE;
		goto exit;
	}

	if ((ERROR_SUCCESS != RegQueryValueEx(hkTrans, L"driver", NULL, &dwType, (LPBYTE)pszName, &dwSize)) && (dwType == REG_SZ)) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"[HCIUNIV] Could not get transport driver for PnP device\n"));
		ASSERT(0);
		fRetVal = FALSE;
		goto exit;
	}
		
	IFDBG(DebugOut (DEBUG_HCI_INIT, L"[HCIUNIV] PnP device found.  Using driver %s.\n", pszName));

	WCHAR wszHCI[] = L"Software\\Microsoft\\Bluetooth\\HCI";
	CopyRegistryEntry(wszHCI, wszKey, HKEY_BASE, TRUE);

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_BASE, wszHCI, 0, 0, &hkHCI)) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"[HCIUNIV] Could not open HCI registry key\n"));
		ASSERT(0);
		fRetVal = FALSE;
		goto exit;
	}

	if (ERROR_SUCCESS != RegSetValueEx(hkHCI, L"Name", 0, REG_SZ, (LPBYTE)gwszDevName, (wcslen(gwszDevName)+1)*sizeof(WCHAR))) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"[HCIUNIV] Could not set device name in HCI registry key\n"));
		ASSERT(0);
		fRetVal = FALSE;
		goto exit;
	}

exit:
	
	if (hkTrans) {
		RegCloseKey(hkTrans);
	}
	if (hkHCI) {
		RegCloseKey(hkHCI);
	}	
	
	return fRetVal;
}

/*

This function is called by HCI to set the callback function to indicate device
insertion/removal notifications.

*/
int HCI_SetCallback (HCI_TransportCallback pfCallback) 
{
	int nRet = FALSE;

	gpSynch->Lock();

	if (pfCallback)
	{
		if (!pfnCallback)
			DebugInit();

		nRet = Initialize();
		if (!nRet)
			UnInitialize();
	}
	else
	{
		UnInitialize();
		
		if (pfnCallback)
			DebugDeInit();
	}
	pfnCallback = pfCallback;
	
	gpSynch->Unlock();

	return nRet;
}


/*

This function is called by HCI to start the hardware.  In this case we indicate a
device up notification so HCI will proceed with a call to HCI_OpenConnection.

*/
int HCI_StartHardware (void) 
{
	HCI_TransportCallback pfn = NULL;

	gpSynch->Lock();
	BOOL fInit = gfInitialized;
	pfn = pfnCallback;
	gfStopHardware = FALSE;
	gpSynch->AddRef();
	gpSynch->Unlock();

	int fRet = ((fInit) && (pfn) && (ERROR_SUCCESS == pfn(DEVICE_UP, NULL)));

	gpSynch->Lock();
	gpSynch->DelRef();
	gpSynch->Unlock();

	return fRet;
}


/*

This function is called by HCI to stop the hardware.  In this case we indicate a
device up notification so HCI will proceed with a call to HCI_CloseConnection.

*/
int HCI_StopHardware (void) 
{
	HCI_TransportCallback pfn = NULL;

	gpSynch->Lock();
	BOOL fInit = gfInitialized;
	pfn = pfnCallback;
	gfStopHardware = TRUE;
	gpSynch->AddRef();
	gpSynch->Unlock();

	int fRet = ((fInit) && (pfn) && (ERROR_SUCCESS == pfn(DEVICE_DOWN, NULL)));
	
	gpSynch->Lock();
	gpSynch->DelRef();
	gpSynch->Unlock();

	return fRet;
}


/*

This function is called by HCI to read the current transport driver parameters.

*/
int HCI_ReadHciParameters (HCI_PARAMETERS *pParms) 
{
	HCI_READHCIPARAMETERS pfn = NULL;
	int nRet = FALSE;
	
	gpSynch->Lock();
	BOOL fInit = gfInitialized;
	pfn = pfnReadHciParameters;
	gpSynch->AddRef();
	gpSynch->Unlock();

	if (fInit && pfn)
		nRet = (*pfn)(pParms);

	if (!nRet)
	{
		IFDBG(DebugOut(DEBUG_ERROR, L"[HCIUNIV] HCI_ReadHciParameters failed.\n"));
	}

	gpSynch->Lock();
	gpSynch->DelRef();
	gpSynch->Unlock();

	return nRet;
}


/*

This function is called by HCI to open a connection to a transport driver.  This function first sees
if there is a PnP device present and if not tries to open a built-in driver.

*/
int HCI_OpenConnection (void) 
{
	int nRet = FALSE;
	HCI_OPENCONNECTION pfn = NULL;
	HCI_SETCALLBACK pfncb = NULL;
	HINSTANCE hLib = NULL;
	int nCurrent = -1;
	WCHAR szDriver[MAX_PATH];
	DWORD dwDriver = sizeof(szDriver);
	HKEY hk;

	gpSynch->Lock();

	while(geInCall != eOther)
	{
		gpSynch->Unlock();
		Sleep(2000);
		gpSynch->Lock();
	}

	if (!gfInitialized || ghCurDriver)
	{
		gpSynch->Unlock();
		return FALSE;
	}

	geInCall = eOpen;	

	//
	// Try opening PnP device
	//

	if (GetPnPDriver(szDriver))
	{
		if ((NULL != (hLib = LoadLibrary(szDriver))) && 
			(NULL != (pfn = (HCI_OPENCONNECTION )GetProcAddress(hLib, L"HCI_OpenConnection"))) && 
			(NULL != (pfncb = (HCI_SETCALLBACK )GetProcAddress(hLib, L"HCI_SetCallback"))))
		{
			gpSynch->AddRef();
			gpSynch->Unlock();

			if (ERROR_SUCCESS == (*pfncb)(TransportCallback))
				nRet = (*pfn)();
	
			if (!nRet)
				(*pfncb)(NULL);
			else
				ghCurDriver = hLib;

			gpSynch->Lock();
			gpSynch->DelRef();
		}
	}

	if (nRet)
		goto Done;
	else if (hLib)
	{
		FreeLibrary(hLib);
		hLib = NULL;
	}

	//
	// No PnP devices present.  Try built-in devices.
	//

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_BASE, L"Software\\Microsoft\\Bluetooth\\Transports\\Builtin", 0, 0, &hk))
	{
		//MAX_PATH is way too much for numbers. This is in case somebody decides they dont like numbers...
		WCHAR szId[MAX_PATH];	
		DWORD dwId = MAX_PATH;
		DWORD dwIndex = 0;
		while((ERROR_SUCCESS == RegEnumKeyEx(hk, dwIndex++, szId, &dwId, NULL, NULL, NULL, NULL)) && (dwId < MAX_PATH))
		{
			HKEY hk1;
			DWORD dwType;
			if ((ERROR_SUCCESS == RegOpenKeyEx(hk, szId, 0, 0, &hk1)) && (hk1) && 
				(ERROR_SUCCESS == RegQueryValueEx(hk1, L"driver", NULL, &dwType, (LPBYTE )szDriver, &dwDriver)) && 
				(dwDriver < sizeof(szDriver)) && (dwType == REG_SZ) && 
				(NULL != (hLib = LoadLibrary(szDriver))) && 
				(NULL != (pfn = (HCI_OPENCONNECTION )GetProcAddress(hLib, L"HCI_OpenConnection"))) && 
				(NULL != (pfncb = (HCI_SETCALLBACK )GetProcAddress(hLib, L"HCI_SetCallback"))) 	)
			{
				WCHAR szKey[MAX_PATH];
				WCHAR szHCI[] = L"Software\\Microsoft\\Bluetooth\\HCI";
				wcscpy(szKey, L"Software\\Microsoft\\Bluetooth\\Transports\\Builtin\\");
				wcsncat(szKey, szId, MAX_PATH - wcslen(szKey) - 1);
				CopyRegistryEntry(szHCI, szKey, HKEY_BASE, TRUE);

				gpSynch->AddRef();
				gpSynch->Unlock();

				if (ERROR_SUCCESS == (*pfncb)(TransportCallback))
					nRet = (*pfn)();
				if (!nRet)
					(*pfncb)(NULL);

				gpSynch->Lock();
				gpSynch->DelRef();

				if (!nRet)
				{
					HKEY hk2;
					if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_BASE, szHCI, 0, 0, &hk2))
					{
						SVSUTIL_ASSERT(hk2);
						DeleteValues(hk2);
						RegCloseKey(hk2);
					}
				}
				else
				{
					IFDBG(DebugOut(DEBUG_HCI_INIT, L"[HCIUNIV] HCI_OpenConnection using driver %s(Id=%s).\n", szDriver, szId));
					ghCurDriver = hLib;
					RegCloseKey(hk1);
					break;
				}
			}
			if (hk1)
				RegCloseKey(hk1);
			if (hLib)
			{
				FreeLibrary(hLib);
				hLib = NULL;
			}
			dwId = MAX_PATH;
			dwDriver = sizeof(szDriver);
		}

		RegCloseKey(hk);
	}		
	

Done:
	SVSUTIL_ASSERT(gpSynch->IsLocked());

	if (nRet)
		nRet = InitPointers();
	else
		IFDBG(DebugOut(DEBUG_ERROR, L"[HCIUNIV] HCI_OpenConnection failed.\n"));

	geInCall = eOther;
	gpSynch->Unlock();

	return nRet;
}


/*

This function is called by HCI to close a connection to a transport driver.

*/
void HCI_CloseConnection (void) 
{
	HCI_CLOSECONNECTION pfn = NULL;
	HCI_SETCALLBACK pfncb = NULL;
	
	gpSynch->Lock();

	while(geInCall != eOther)
	{
		gpSynch->Unlock();
		Sleep(2000);
		gpSynch->Lock();
	}
	geInCall = eClose;

	BOOL fInit = gfInitialized;
	pfn = pfnCloseConnection;
	pfncb = pfnSetCallback;
	gpSynch->AddRef();
	gpSynch->Unlock();

	if (fInit && pfn)
	{
		(*pfn)();
		(*pfncb)(NULL);
	}
	
	gpSynch->Lock();
	gpSynch->DelRef();

	if (fInit && pfn)
	{
		DeInitPointers();
	}
	
	SVSUTIL_ASSERT(gpSynch->IsLocked());

	geInCall = eOther;
	gpSynch->Unlock();
}


/*

This function is called by HCI to write a packet.  This function calls directly into
the active transport driver.

*/
int HCI_WritePacket (HCI_TYPE eType, BD_BUFFER *pBuff) 
{
	int nRet = FALSE;
	HCI_WRITEPACKET pfn = NULL;

	gpSynch->Lock();
	BOOL fInit = gfInitialized;
	pfn = pfnWritePacket;
	gpSynch->AddRef();
	gpSynch->Unlock();

#ifdef ATHBT_FILTER

    if (fInit && pfn) {
        if (eType == COMMAND_PACKET) {
            AthBtFilterHciCommand(&g_AthBtFilterInstance, 
                                  (pBuff->pBuffer + pBuff->cStart),
                                  BufferTotal(pBuff));
        } else if (eType == DATA_PACKET_ACL) {
                /* for CE5 we filter ACL packets to detect application traffic flows like A2DP */
            AthBtFilterHciAclDataOut(&g_AthBtFilterInstance,
                                     (pBuff->pBuffer + pBuff->cStart),
                                     BufferTotal(pBuff));
        }
    }
    
#endif

	if (fInit && pfn)
		nRet = (*pfn)(eType, pBuff);
	if (!nRet)
	{
		IFDBG(DebugOut(DEBUG_ERROR, L"[HCIUNIV] HCI_WritePacket failed.\n"));
	}
	gpSynch->Lock();
	gpSynch->DelRef();
	gpSynch->Unlock();

	return nRet;
}


/*

This function is called by HCI to read a packet.  This function calls directly into
the active transport driver.

*/
int HCI_ReadPacket (HCI_TYPE *peType, BD_BUFFER *pBuff) 
{
	int nRet = FALSE;
	HCI_READPACKET pfn = NULL;

	gpSynch->Lock();
	BOOL fInit = gfInitialized;
	pfn = pfnReadPacket;
	gpSynch->AddRef();
	gpSynch->Unlock();

	if (fInit && pfn)
		nRet = (*pfn)(peType, pBuff);
	if (!nRet)
	{
		IFDBG(DebugOut(DEBUG_ERROR, L"[HCIUNIV] HCI_ReadPacket failed.\n"));
	}
    
#ifdef ATHBT_FILTER

    if (fInit && pfn && nRet)
        {
        	if(*peType == EVENT_PACKET) {
		       AthBtFilterHciEvent(&g_AthBtFilterInstance, 
		                           (pBuff->pBuffer + pBuff->cStart),
		                            BufferTotal(pBuff));
    		}
    		else if(*peType == DATA_PACKET_ACL)
	   	{    
	             /* for CE5 we filter ACL packets to detect application traffic flows like A2DP */
	             /* BT device is also possible to create connection automaticlly*/
	            AthBtFilterHciAclDataIn(&g_AthBtFilterInstance,
	                                     (pBuff->pBuffer + pBuff->cStart),
	                                     BufferTotal(pBuff));
		 }
	}
#endif

	gpSynch->Lock();
	gpSynch->DelRef();
	gpSynch->Unlock();

	return nRet;
}


