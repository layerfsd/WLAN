#include "stdafx.h"
#include "WiFiCtrl.h"
#include <Cfgmgrapi.h>

bool LoadWlanFunction (pRadiodeviceCtrl WrAdapter)
{
    bool Status = TRUE;
    HMODULE WlanDevHandler;

    WlanDevHandler = WrAdapter->dllhandler = LoadLibrary (L"Ossvcs.dll");	
    if (NULL == WlanDevHandler)
        Status = FALSE;

    WrAdapter->fnGetWlanDev = (GetWlanDevices)GetProcAddress (WlanDevHandler, MAKEINTRESOURCE (GetWlanDevice_FunctionID));
    if (NULL == WrAdapter->fnGetWlanDev)
        Status = FALSE;

    WrAdapter->fnChangeWlanPowerState = (ChangeWlanPowerState)GetProcAddress (WlanDevHandler, MAKEINTRESOURCE (ChangeWlanState_FunctionID));
    if (NULL == WrAdapter->fnChangeWlanPowerState)
        Status = FALSE;

    WrAdapter->fnFreeWlanList = (FreeWlanList)GetProcAddress (WlanDevHandler, MAKEINTRESOURCE (FreeWlanList_FunctionID));
    if (NULL == WrAdapter->fnFreeWlanList)
        Status = FALSE;

    if ( ! Status)	   
    {
        WrAdapter->fnGetWlanDev = NULL;
        WrAdapter->fnChangeWlanPowerState = NULL;
        WrAdapter->fnFreeWlanList = NULL;
        if (WlanDevHandler != NULL)
            CloseHandle (WlanDevHandler );

        WrAdapter->dllhandler = WlanDevHandler;
    }

    return Status;
}

DWORD WlanStateSet (DWORD dwFlag)
{
    HKEY   KeyHandler;
    DWORD dwData, dwValue, dwRegStatus;

    dwRegStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"System\\State\\Hardware", 0, 0 , &KeyHandler);
    dwRegStatus = RegQueryValueEx (KeyHandler, L"WiFi", NULL, NULL, (BYTE *)&dwData, &dwValue);

    if(WlanOn == dwFlag)
        dwData |= 0x06;
    else
        dwData &= ~0x6;

    dwRegStatus = RegSetValueEx (KeyHandler, L"WiFi", 0, REG_DWORD, (BYTE *)&dwData, dwValue);
    CloseHandle (KeyHandler);

    dwRegStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Power\\State", 0, 0, &KeyHandler);
    if(WlanOn == dwFlag)
        dwData |= 1;
    else
        dwData &= 0;

    dwRegStatus = RegSetValueEx (KeyHandler, L"{98C5250D-C29A-4985-AE5F-AFE5367E5006}\\AR6K_SD1", 0, REG_DWORD, (BYTE *)&dwData, dwValue);
    CloseHandle (KeyHandler);

    return 0;
}

DWORD WlanEnableDisable (pRadiodeviceCtrl WrAdapter, DWORD dwState)//0 for on, 1 for off
{
    DWORD Status = STATUS_SUCCESS;	
    RDD *WiFiDevice = NULL, *pDeviceList = NULL, *pTemp = NULL;
    HRESULT Result;

    if (FALSE == LoadWlanFunction (WrAdapter))
    {
        return STATUS_WIFI_LOAD_FUNC_ERROR;//STATUS_MAPERROR;
    }	

    Result = ( *WrAdapter->fnGetWlanDev) (&pDeviceList,0);
    if (S_OK != Result)
    {
        return ((DWORD) Result);
    }  

    //Check the Device we want.
    if (pDeviceList)
    {
        pTemp = pDeviceList;

        // loop through the linked list of devices
        while (pTemp)
        { 

            if (POWER_MANAGED == pTemp->DeviceType)
            {
                if(memcmp (pTemp->pszDeviceName, _szAR6K_SD1, sizeof (_szAR6K_SD1)))
                {
                    WiFiDevice = pTemp;
                    break;
                }
            }   

            pTemp = pTemp->pNext;

        }
    }

    if (WiFiDevice == NULL)
    {
        Status = STATUS_WIFI_NO_DEVICE;//STATUS_NOWIFIDEVICE;	
    }
    else if (WiFiDevice->dwState != dwState)
    {
        (*WrAdapter->fnChangeWlanPowerState) (WiFiDevice,dwState,POWER_PRE_SAVE);
    }

    if(pDeviceList)
    {
        (*WrAdapter->fnFreeWlanList) (pDeviceList);
    }

    UnloadWlanFunction (WrAdapter);
    return Status;	
}

void UnloadWlanFunction (pRadiodeviceCtrl WrAdapter)
{

    if (WrAdapter->dllhandler)
        CloseHandle (WrAdapter->dllhandler);

    WrAdapter->fnChangeWlanPowerState = NULL;
    WrAdapter->fnFreeWlanList = NULL;
    WrAdapter->fnGetWlanDev = NULL;
}

#if 0
HRESULT WiFiRemoveProfile(LPWSTR SsidName)
{
    HRESULT hr         = E_FAIL;
    LPWSTR wszOutput   = NULL;
    LPWSTR AtherosProfile,temp;
    DWORD  length;

    LPWSTR WiFiProfile = L"<wap-provisioningdoc>"
        L" <characteristic type=\"Wi-Fi\"> "
        L"  <characteristic type=\"ad-hoc\"> ";
    LPWSTR	RemoveEntry    = L"   <nocharacteristic type=\"";
    LPWSTR	EndEntry	   = L"\"/> ";
    LPWSTR	PostProfile     = L"  </characteristic>"
        L" </characteristic>"
        L"</wap-provisioningdoc>";

    length = wcslen(WiFiProfile)+wcslen(RemoveEntry)+wcslen(EndEntry)+wcslen(PostProfile)+wcslen(SsidName)+2;
    length *=sizeof(WCHAR);
    AtherosProfile = temp = (LPWSTR)malloc(length);
    ZeroMemory(AtherosProfile,length);
    memcpy(temp,WiFiProfile,wcslen(WiFiProfile)*sizeof(WCHAR));
    temp+=wcslen(WiFiProfile);
    memcpy(temp,RemoveEntry,wcslen(RemoveEntry)*sizeof(WCHAR));
    temp+=wcslen(RemoveEntry);
    memcpy(temp,SsidName,wcslen(SsidName)*sizeof(WCHAR));
    temp+=wcslen(SsidName);
    memcpy(temp,EndEntry,wcslen(EndEntry)*sizeof(WCHAR));
    temp+=wcslen(EndEntry);
    memcpy(temp,PostProfile,wcslen(PostProfile)*sizeof(WCHAR));
    temp+=wcslen(PostProfile);

    hr = DMProcessConfigXML(AtherosProfile, CFGFLAG_PROCESS, &wszOutput);
    delete [] wszOutput;
    free(AtherosProfile);
    return hr;
}

HRESULT WiFiCreateProfile(LPWSTR SsidName)
{
    HRESULT hr         = E_FAIL;
    LPWSTR wszOutput   = NULL;
    DWORD length;

    LPWSTR AtherosProfile, temp;
    LPWSTR WiFiProfile=L"<wap-provisioningdoc>"
        L" <characteristic type=\"Wi-Fi\"> "
        L"  <characteristic type=\"ad-hoc\"> ";
    LPWSTR	SetEntry     = 	 L"   <characteristic type=\"";
    LPWSTR	EndEntry	 =   L"\"> ";
    LPWSTR PostProfile =    L"     <parm name=\"DestId\" value=\"{436EF144-B4FB-4863-A041-8F905A62C572}\"/>"
        L"     <parm name=\"Adhoc\" value=\"1\"/>"
        L"     <parm name=\"Authentication\" value=\"0\"/>"
        L"     <parm name=\"Encryption\" value=\"1\"/>"
        L"     <parm name=\"KeyProvided\" value=\"0\"/>"
        L"     <parm name=\"NetworkKey\" value=\"\"/>"
        L"     <parm name=\"KeyIndex\" value=\"1\"/>"
        L"     <parm name=\"Use8021x\" value=\"0\"/>"
        L"     <parm name=\"EAPType\" value=\"25\"/>"
        L"   </characteristic>"
        L"  </characteristic>"
        L" </characteristic>"
        L"</wap-provisioningdoc>";
    length = wcslen(WiFiProfile)+wcslen(SetEntry)+wcslen(EndEntry)+wcslen(PostProfile)+wcslen(SsidName)+2;
    length *=sizeof(WCHAR);
    AtherosProfile = temp = (LPWSTR)malloc(length);
    ZeroMemory(AtherosProfile,length);
    memcpy(temp,WiFiProfile,wcslen(WiFiProfile)*sizeof(WCHAR));
    temp+=wcslen(WiFiProfile);
    memcpy(temp,SetEntry,wcslen(SetEntry)*sizeof(WCHAR));
    temp+=wcslen(SetEntry);
    memcpy(temp,SsidName,wcslen(SsidName)*sizeof(WCHAR));
    temp+=wcslen(SsidName);
    memcpy(temp,EndEntry,wcslen(EndEntry)*sizeof(WCHAR));
    temp+=wcslen(EndEntry);
    memcpy(temp,PostProfile,wcslen(PostProfile)*sizeof(WCHAR));
    temp+=wcslen(PostProfile);

    hr = DMProcessConfigXML(AtherosProfile, CFGFLAG_PROCESS, &wszOutput);

    free(AtherosProfile);
    delete [] wszOutput;

    return hr;
}
#endif

DWORD RadioConnectDisConnect (pConnectionCtrl pAdapter, DWORD Flag)
{
    DWORD Status = 0; // STATUC_SUCCESS
    HRESULT Result;
    CONNMGR_CONNECTIONINFO ConnInfo;
    DWORD CurrentState;
    DWORD Type;

    switch (Flag)
    {
    case GPRS_CONNECT:
        if (GPRS_CONNECT == Flag)
            Type = CM_CONNTYPE_CELLULAR;

    case WIFI_CONNECT:
        if (WIFI_CONNECT == Flag)
            Type = CM_CONNTYPE_NIC;

        if (CONNECTED != pAdapter->ConnectState)
        {
            ZeroMemory (&ConnInfo, sizeof (ConnInfo));
            ConnInfo.bDisabled = FALSE;
            ConnInfo.dwFlags = 0;
            ConnInfo.dwParams = CONNMGR_PARAM_GUIDDESTNET;
            ConnInfo.dwPriority = CONNMGR_PRIORITY_USERINTERACTIVE;
            ConnInfo.bExclusive = FALSE; 
            ConnInfo.cbSize = sizeof (ConnInfo);

            Result = QueryConnection (Type, &CurrentState, pAdapter->ConnectionName);
            if(S_OK != Result)
            {
                Status = STATUS_GPRS_GUIDERROR;//Map GIUD error
                goto END;
            }

            Result = ConnMgrMapConRef (ConRefType_NAP, pAdapter->ConnectionName, &ConnInfo.guidDestNet);
            if (CONNMGR_STATUS_CONNECTED == CurrentState)
            {
                if (CM_CONNTYPE_NIC == Type)
                    goto END;

                if (CM_CONNTYPE_CELLULAR == Type)
                {
                    // Do something to disconnect GPRS
                }
            }

            Result = ConnMgrEstablishConnectionSync (&ConnInfo, &pAdapter->Handler, 60000, &Status);

            if(S_OK != Result)
            {
                pAdapter->ConnectState = DISCONNECTED;

                Status = (Flag == GPRS_CONNECT) ? STATUS_GPRS_CONNECTFAILRE : STATUS_WIFI_CONNECTFAILURE;//Can not Connect to GPRS
            }
            else
            {
                Status = 0;
                pAdapter->ConnectState = CONNECTED;
            }
        }
        break;

    case GPRS_DISCONNECT:
    case WIFI_DISCONNECT:   
        if (DISCONNECTED != pAdapter->ConnectState && NULL != pAdapter->Handler)
        {
            ConnMgrReleaseConnection (pAdapter->Handler, 0);
            pAdapter->ConnectState = DISCONNECTED;
            CloseHandle (pAdapter->Handler);
        }
        break;

    default:
        Status = STATUS_GPRS_INVALIDPARAM;
        break;
    }

END:
    return Status;
}

#if 0 //Not used
DWORD GPRSThread(LPVOID Context)
{
    HANDLE SuccessEvent;
    HANDLE FailEvent;
    HRESULT Result;
    HANDLE GPRSHandler = (HANDLE)(Context);
    DWORD  State;
    BOOLEAN StartQuery = TRUE;

    SuccessEvent = CreateEvent(NULL,0,0,L"SuccessEvent");
    FailEvent = CreateEvent(NULL,0,0,L"FailEvent");

    while(StartQuery)
    {
        Result= ConnMgrConnectionStatus(GPRSHandler,&State);
        if(Result!= S_OK)
        {
            StartQuery = FALSE;
            SetEvent(FailEvent);

        }
        else
        {
            switch(State)
            {
            case CONNMGR_STATUS_CONNECTED:
                StartQuery = FALSE;
                SetEvent(SuccessEvent);
                break;  
            case CONNMGR_STATUS_UNKNOWN:
            case CONNMGR_STATUS_SUSPENDED:
            case CONNMGR_STATUS_DISCONNECTED:
            case CONNMGR_STATUS_CONNECTIONFAILED:
            case CONNMGR_STATUS_CONNECTIONCANCELED:
            case CONNMGR_STATUS_CONNECTIONDISABLED:
            case CONNMGR_STATUS_NOPATHTODESTINATION:
            case CONNMGR_STATUS_WAITINGFORPATH:
            case CONNMGR_STATUS_WAITINGFORPHONE:
            case CONNMGR_STATUS_PHONEOFF:
            case CONNMGR_STATUS_EXCLUSIVECONFLICT:
            case CONNMGR_STATUS_NORESOURCES:
            case CONNMGR_STATUS_CONNECTIONLINKFAILED:
            case CONNMGR_STATUS_AUTHENTICATIONFAILED:
            case CONNMGR_STATUS_WAITINGDISCONNECTION:
            case CONNMGR_STATUS_WAITINGCONNECTIONABORT:
                //			 case CONNMGR_STATUS_NOPATHWITHPROPERTY:
                StartQuery = FALSE;
                SetEvent(FailEvent);
                break;
            case CONNMGR_STATUS_WAITINGCONNECTION:
            case CONNMGR_STATUS_WAITINGFORRESOURCE:
            case CONNMGR_STATUS_WAITINGFORNETWORK:
                break;
            }
        }
    }

    CloseHandle(FailEvent);
    CloseHandle(SuccessEvent);
    return 0;
}
#endif //Not used

DWORD WiFiOnOff (DWORD Flag)
{
    DWORD Status=0;
    RadioDeviceCtrl WrAdapter;

    if (LoadWlanFunction (&WrAdapter) != TRUE)
    {
        Status = STATUS_WIFI_LOAD_FUNC_ERROR;
    }
    else
    {
        Status = WlanEnableDisable (&WrAdapter, Flag);
        WlanStateSet (Flag);

        if (S_OK != Status && !(Status != STATUS_WIFI_NO_DEVICE))
        {
            Status = STATUS_WIFI_FUNC_ERROR;
        }
        UnloadWlanFunction (&WrAdapter);
    }
    return Status; 
}

BOOL SetInterface (TCHAR *pszName, TCHAR *pszValue)
{
    HKEY KeyHandler;
    BYTE pData[MAX_PATH];
    DWORD dwValue = MAX_PATH;
    LONG dwRegStatus;
    BYTE pInterface[MAX_PATH];
    int nSize = (_tcslen (pszValue) * sizeof (TCHAR));

    ZeroMemory (pInterface, MAX_PATH);
    ZeroMemory (pData, MAX_PATH);

    memcpy (pInterface, pszValue, nSize);

    dwRegStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE, _szSharingKey, 0, 0 , &KeyHandler);
    _Is_Reg_Err (dwRegStatus);

    dwRegStatus = RegQueryValueEx (KeyHandler, pszName, NULL, NULL, pData, &dwValue);
    _Is_Reg_Err (dwRegStatus);

    if (memcmp (pData, pInterface, nSize) != 0)
    {
        dwValue = nSize; 
        dwRegStatus = RegSetValueEx (KeyHandler, pszName, 0, REG_SZ, pInterface, dwValue);
        _Is_Reg_Err (dwRegStatus);
    }
    RegCloseKey (KeyHandler);

    _tPRINTF (_T ("%sUpdated %s as %s"), _szAppNameW, pszName, pszValue);

    return TRUE;
}

BOOL WiFiIPConfig (BOOL bEnable, DWORD *pdwPrev)
{
    //ar6k/params/tcpip - if (EnableDHCP == 1){make enabledhcp=autocfg=0, set static ip,sm,gw}else nothing
    //and rebing ar6k

    HKEY KeyHandler;
    BYTE pData[MAX_PATH];
    DWORD Value = MAX_PATH;
    DWORD dwData;
    LONG dwRegStatus;
    TCHAR szAR6KDevName[] = {_szDeviceName};

    _tPRINTF (_T ("%sWiFiIPConfig - Opening %s"), _szAppNameW, _szAr6kTcpIpKey);

    dwRegStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE, _szAr6kTcpIpKey, 0, 0 ,&KeyHandler);
    _Is_Reg_Err (dwRegStatus);

    if (bEnable)
    {
        dwRegStatus = RegQueryValueEx (KeyHandler, _szDHCPKey, NULL, NULL, pData, &Value);
        _Is_Reg_Err (dwRegStatus);

        dwData =*(DWORD *)( pData);

        *pdwPrev = dwData;

        if (dwData)
        {
            dwData =0;
            Value = sizeof (dwData); 
            memcpy (pData, &dwData, Value);
            dwRegStatus = RegSetValueEx(KeyHandler, _szDHCPKey, 0, REG_DWORD, pData, Value);
            _Is_Reg_Err (dwRegStatus);

            dwRegStatus = RegSetValueEx(KeyHandler, _szAutocfgkey, 0, REG_DWORD, pData, Value);
            _Is_Reg_Err (dwRegStatus);

            Value = sizeof (_szDefaultIP); 
            memcpy (pData, _szDefaultIP, Value);
            dwRegStatus = RegSetValueEx(KeyHandler, _szIPKey, 0, REG_SZ, pData, Value);
            _Is_Reg_Err (dwRegStatus);

            Value = sizeof (_szDefaultMask); 
            memcpy (pData, _szDefaultMask, Value);
            dwRegStatus = RegSetValueEx(KeyHandler, _szSubMaskKey, 0, REG_SZ, pData, Value);
            _Is_Reg_Err (dwRegStatus);

            Value = sizeof (_szDefaultGW); 
            memcpy (pData, _szDefaultGW, Value);
            dwRegStatus = RegSetValueEx(KeyHandler, _szGWKey, 0, REG_SZ, pData, Value);
            _Is_Reg_Err (dwRegStatus);
        }

    }
    else //bEnable is false
    {
        if ( *pdwPrev)
        {
            dwData = 1;
            Value = sizeof (dwData); 
            memcpy (pData, &dwData, Value);
            dwRegStatus = RegSetValueEx(KeyHandler, _szDHCPKey, 0, REG_DWORD, pData, Value);
            _Is_Reg_Err (dwRegStatus);
        }
    }
    RegCloseKey (KeyHandler);
    RebindAdapter (szAR6KDevName);

    _tPRINTF (_T ("%sWiFiIPConfig - Done"), _szAppNameW);

    return TRUE;
}

DWORD CheckDevice()
{
    RadioDeviceCtrl WrAdapter;
    DWORD Status=STATUS_SUCCESS;	
    RDD *pDeviceList=NULL;
    RDD *pTemp;
    HRESULT Result;
    BOOLEAN PhoneExist = FALSE, WiFiExist=FALSE;
    LPTSTR DATA1;   
    DWORD CurrentState;

    if (FALSE == LoadWlanFunction(&WrAdapter) == FALSE)
    {
        return STATUS_WIFI_LOAD_FUNC_ERROR;//STATUS_MAPERROR;
    }	

    Result = ( *WrAdapter.fnGetWlanDev) (&pDeviceList,0);
    if(S_OK != Result)
    {
        return ((DWORD) Result);
    }  

    //Check the Device we want.
    if (pDeviceList)
    {
        pTemp = pDeviceList;

        // loop through the linked list of devices
        while (pTemp)
        { 
            switch (pTemp->DeviceType)
            {
            case POWER_PHONE:
                PhoneExist = TRUE;
                break;
            case POWER_MANAGED:
                {
                    DATA1 = pTemp->pszDeviceName+2*(sizeof(GUID)+3)+1;
                    if ( ! wcscmp (DATA1, _szAR6K_SD1))
                    {
                        CurrentState = pTemp->dwState;
                        WiFiExist = TRUE;
                    }
                    break;
                }   
            }
            pTemp = pTemp->pNext;
        }
    }

    if (pDeviceList)
    {
        (*WrAdapter.fnFreeWlanList)(pDeviceList);
    }
    UnloadWlanFunction (&WrAdapter);

    if ( ! PhoneExist)
    {
        return STATUS_NO_PHONE;//
    }

    if ( ! WiFiExist)
    {
        return STATUS_WIFI_NO_DEVICE;		
    }

    if (0 == CurrentState)
    {
        WiFiOnOff (WlanOn);
        Sleep (2000);
    }
    return Status;
}

DWORD QueryConnection (DWORD QueryType, DWORD *dwConnectState, TCHAR *ConnectName)
{
    DWORD Status = 0; // STATUC_SUCCESS
    HRESULT Result;
    CONNMGR_CONNECTION_DETAILED_STATUS *pBuffer = NULL;
    CONNMGR_CONNECTION_DETAILED_STATUS *pTemp = NULL;
    DWORD Length = 0;
    BOOLEAN Found = FALSE;

    Result = ConnMgrQueryDetailedStatus (pBuffer,&Length);

    if(HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) == Result)
    {
        pBuffer = (CONNMGR_CONNECTION_DETAILED_STATUS *)malloc (Length);
        Result = ConnMgrQueryDetailedStatus (pBuffer,&Length);
    }

    if (S_OK != Result)
    {
        Status = STATUS_GPRS_QUERYFAILURE;//Can not Query connection
        goto END;    
    }

    pTemp = pBuffer;
    while (pTemp !=NULL)
    {
        if (pTemp->dwType == QueryType)
        { 
            *dwConnectState = pTemp->dwConnectionStatus;
            if(CM_CONNTYPE_NIC == QueryType)
            {
                if ( ! wcscmp(pTemp->szDescription, ConnectName))
                {
                    Found = TRUE;
                    break;
                }
            }
            else if (CM_CONNTYPE_CELLULAR == QueryType)
            {
                wcscpy (ConnectName, pTemp->szDescription);	
                Found = TRUE;
                break;
            }
        }

        pTemp = pTemp->pNext;
    }

    free (pBuffer);
    if ( ! Found)
    {
        Status = 0x80000057;
    }
END:

    return Status;
}

DWORD WiFiCheckConnection (pConnectionCtrl pWiFiAdapter)
{
    DWORD Status=0;
    DWORD CurrentState;

    Status = QueryConnection (CM_CONNTYPE_NIC, &CurrentState, pWiFiAdapter->ConnectionName);
    if(S_OK != Status)
    {
        Status = STATUS_GPRS_GUIDERROR;//Map GIUD error
        goto END;
    }

    if(CONNMGR_STATUS_CONNECTED == CurrentState)
    {
        pWiFiAdapter->ConnectState = CONNECTED; 
        goto END;
    }

END:
    return Status;
}

BOOL PublicIPConfig(TCHAR *pszPublicInterface, BOOL bEnable)
{
    HKEY	KeyHandler;
    DWORD	dwData, dwValue, dwRegStatus, dwType;
    BYTE    pData[MAX_PATH];

    TCHAR szTcpIpInterfaceKey[MAX_PATH] = {0,};

    _stprintf (szTcpIpInterfaceKey, _T ("comm\\%s\\parms\\Tcpip"), pszPublicInterface);

    _tPRINTF (_T ("%sPublic interface key: %s"), _szAppNameW, szTcpIpInterfaceKey);

    dwRegStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTcpIpInterfaceKey, 0, 0 ,&KeyHandler);
    _Is_Reg_Err (dwRegStatus);

    if (bEnable)
    {
        dwData = 0;
        dwValue = sizeof (DWORD);
        dwType = REG_DWORD;

        dwRegStatus = RegQueryValueEx (KeyHandler, _szDHCPKey, NULL, &dwType, (LPBYTE)&dwData, &dwValue);
        //_Is_Reg_Err (dwRegStatus);

        AppRegSetDword(_szAppPublicDhcpStatus, dwData);

        if(dwData)//if DHCP enabled
        {
            dwValue = MAX_PATH;
            dwRegStatus = RegQueryValueEx (KeyHandler, _szDhcpIPAddress, NULL, &dwType, pData, &dwValue);
            _Is_Reg_Err (dwRegStatus);

            dwRegStatus = RegSetValueEx (KeyHandler, _szIPKey, 0, dwType, pData, dwValue);
            _Is_Reg_Err (dwRegStatus);

            dwValue = MAX_PATH;
            dwRegStatus = RegQueryValueEx (KeyHandler, _szDhcpSubnetMask, NULL, &dwType, pData, &dwValue);
            _Is_Reg_Err (dwRegStatus);

            dwRegStatus = RegSetValueEx (KeyHandler, _szSubMaskKey, 0, dwType, pData, dwValue);
            _Is_Reg_Err (dwRegStatus);

            dwValue = MAX_PATH;
            dwRegStatus = RegQueryValueEx (KeyHandler, _szDhcpDNS, NULL, &dwType, pData, &dwValue);
            //_Is_Reg_Err (dwRegStatus);

            dwRegStatus = RegSetValueEx (KeyHandler, _szDNS, 0, dwType, pData, dwValue);
            //_Is_Reg_Err (dwRegStatus);

            dwValue = MAX_PATH;
            dwRegStatus = RegQueryValueEx (KeyHandler, _szDhcpDefaultGateway, NULL, &dwType, pData, &dwValue);
            //_Is_Reg_Err (dwRegStatus);

            dwRegStatus = RegSetValueEx (KeyHandler, _szGWKey, 0, dwType, pData, dwValue);
            //_Is_Reg_Err (dwRegStatus);

            dwData = 0;
            dwValue = sizeof (DWORD);

            dwRegStatus = RegSetValueEx (KeyHandler, _szAutocfgkey, 0, REG_DWORD, (BYTE *)&dwData, dwValue);
            _Is_Reg_Err (dwRegStatus);

            dwRegStatus = RegSetValueEx (KeyHandler, _szDHCPKey, 0, REG_DWORD,( BYTE *)&dwData, dwValue);
            _Is_Reg_Err (dwRegStatus);

            RebindAdapter (pszPublicInterface);	
        }
    }
    else
    {
        AppRegGetDword (_szAppPublicDhcpStatus, &dwData, 0);
        if (dwData)
        {
            dwValue = sizeof (DWORD);

            dwRegStatus = RegSetValueEx (KeyHandler, _szAutocfgkey, 0, REG_DWORD, (BYTE *)&dwData, dwValue);
            _Is_Reg_Err (dwRegStatus);

            dwRegStatus = RegSetValueEx (KeyHandler, _szDHCPKey, 0, REG_DWORD,( BYTE *)&dwData, dwValue);
            _Is_Reg_Err (dwRegStatus);

            RebindAdapter (pszPublicInterface);	
        }
        AppRegSetDword(_szAppPublicDhcpStatus, 0);
    }

    CloseHandle (KeyHandler);

    return TRUE;
}

BOOL AppRegSetDword (TCHAR *pszName, DWORD dwValue)
{
    HKEY KeyHandler = NULL;
    DWORD dwRegStatus = 0, dwSize = MAX_PATH;

    dwRegStatus = RegCreateKeyEx (HKEY_LOCAL_MACHINE, _szAppRegKey, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, 
        NULL, &KeyHandler, NULL);
    _Is_Reg_Err (dwRegStatus);

    dwSize = sizeof (DWORD);

    dwRegStatus = RegSetValueEx (KeyHandler, pszName, NULL, REG_DWORD, (LPBYTE)&dwValue, dwSize);
    _Is_Reg_Err (dwRegStatus);

    RegCloseKey (KeyHandler);

    return TRUE;
}

BOOL AppRegGetDword (TCHAR *pszName, DWORD *pdwValue, DWORD dwDefault)
{
    HKEY KeyHandler = NULL;
    DWORD dwRegStatus = 0, dwSize = sizeof (DWORD), dwType = REG_DWORD;

    dwRegStatus = RegCreateKeyEx (HKEY_LOCAL_MACHINE, _szAppRegKey, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, 
        NULL, &KeyHandler, NULL);
    _Is_Reg_Err (dwRegStatus);

    dwRegStatus = RegQueryValueEx (KeyHandler, pszName, NULL, &dwType, (LPBYTE)pdwValue, &dwSize);
    if (ERROR_SUCCESS != dwRegStatus)
    {
        dwRegStatus = RegSetValueEx (KeyHandler, pszName, NULL, dwType, (LPBYTE)&dwDefault, dwSize);
        _Is_Reg_Err (dwRegStatus);

        *pdwValue = dwDefault;
    }
    RegCloseKey (KeyHandler);

    return TRUE;
}

BOOL AppRegSetString (TCHAR *pszName, TCHAR *pszValue)
{
    BYTE byData[MAX_PATH];
    HKEY KeyHandler = NULL;
    DWORD RegStatus = 0, dwSize = MAX_PATH;

    RegStatus = RegCreateKeyEx (HKEY_LOCAL_MACHINE, _szAppRegKey, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, 
        NULL, &KeyHandler, NULL);
    _Is_Reg_Err (RegStatus);

    dwSize = _tcslen (pszValue) * sizeof (TCHAR);
    memcpy (byData, pszValue, dwSize);

    RegStatus = RegSetValueEx (KeyHandler, pszName, NULL, REG_SZ, byData, dwSize);
    _Is_Reg_Err (RegStatus);

    RegCloseKey (KeyHandler);

    return TRUE;
}

BOOL AppRegGetString (TCHAR *pszName, TCHAR *pszValue, TCHAR *pszDefault)
{
    BYTE byData[MAX_PATH];
    HKEY KeyHandler = NULL;
    DWORD RegStatus = 0, dwSize = MAX_PATH;

    RegStatus = RegCreateKeyEx (HKEY_LOCAL_MACHINE, _szAppRegKey, 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, 
        NULL, &KeyHandler, NULL);
    _Is_Reg_Err (RegStatus);

    memset (byData, NULL, sizeof (byData));
    RegStatus = RegQueryValueEx (KeyHandler, pszName, NULL, NULL /* REG_SZ*/, byData, &dwSize);
    if (ERROR_SUCCESS != RegStatus)
    {
        dwSize = _tcslen (pszDefault) * sizeof (TCHAR);
        memcpy (byData, pszDefault, dwSize);
        RegStatus = RegSetValueEx (KeyHandler, pszName, NULL, REG_SZ, byData, dwSize);
        _Is_Reg_Err (RegStatus);
        _tcscpy (pszValue, pszDefault);
    }
    else
        memcpy (pszValue, byData, dwSize);

    RegCloseKey (KeyHandler);

    return TRUE;
}

#if 0 //Functions replaced 
DWORD WiFiDHCPEnableDisable(DWORD Flag)//Replaced by BOOL EnableDHCP(BOOL bEnable) - NAT.c
{
    DWORD Status = 0;
    HANDLE DevHandler;
    HANDLE NATHandler;

    Status = ActDev(&DevHandler);
    if(Status != 0 && Status != 0x964)
    {
        Status = STATUS_NAT_DEVICE_ERROR;

    }

    Status = OpenNATDrv(&NATHandler);
    if(Status != 0)
    {
        Status = STATUS_NAT_DRIVER_ERROR;
        goto END;

    }
    if(Flag ==1)
    {
        Status = EnableNAT(NATHandler);
    }
    else if(Flag ==0)
    {
        Status = DisableNAT(NATHandler);

    }
    if(Status != 0)
    {
        Status = STATUS_NAT_FAILURE;
        goto END;
    }
    CloseNAT(DevHandler,NATHandler);
END:
    return Status;
}

DWORD IpRouterEnableDisable(BOOLEAN bflag)//Replaced by EnableIPRouting() - NAT.c
{
    DWORD Status = 0;
    HANDLE IPDHandler;

    Status = OpenIPDriver(&IPDHandler);
    if(Status != S_OK)
    {
        Status = 0x90003001;//IP driver load failure.
        goto END;
    }
    if (bflag)
    {
        Status = EnableIPRouting(IPDHandler);
        if(Status != S_OK)
        { 
            Status = 0x90003002;//IP control failure
        }
    }
    else
    {
        Status = DisableIPRouting(IPDHandler);
        if(Status != S_OK)
        { 
            Status = 0x90003002;//IP control failure
        }
    }

END:
    return Status;
}

void SetPublicInterface()//Replaced by SetInterface("", "") - WiFiCtrl.cpp
{

    HKEY KeyHandler;
    BYTE pData[256];
    DWORD Value=256;
    LONG RegStatus;
    BYTE pInterface[256];


    ZeroMemory(pInterface,256);
    ZeroMemory(pData,256);
    memcpy(pInterface,PublicValue,sizeof(PublicValue));
    RegStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,SharingKey,0,0 ,&KeyHandler);
    RegStatus = RegQueryValueEx(KeyHandler,PublicKey,NULL,NULL,pData,&Value);

    if(memcmp(pData,pInterface,sizeof(PublicValue))!=0)
    {
        Value = sizeof(PublicValue); 
        memcpy(pData,PublicValue,Value);
        RegStatus=RegSetValueEx(KeyHandler,PublicKey,0,REG_SZ,pData,Value);
    }
    RegCloseKey(KeyHandler);
}

void SetPrivateInterface()//Replaced by SetInterface("", "") - WiFiCtrl.cpp
{
    HKEY KeyHandler;
    BYTE pData[256];
    DWORD Value=256;
    LONG RegStatus;
    BYTE pInterface[256];


    ZeroMemory(pInterface,256);
    ZeroMemory(pData,256);
    memcpy(pInterface,PrivateValue,sizeof(PrivateValue));
    RegStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,SharingKey,0,0 ,&KeyHandler);
    RegStatus = RegQueryValueEx(KeyHandler,PrivateKey,NULL,NULL,pData,&Value);

    if(memcmp(pData,pInterface,sizeof(PrivateValue))!=0)
    {
        Value = sizeof(PrivateValue); 
        memcpy(pData,PrivateValue,Value);
        RegStatus=RegSetValueEx(KeyHandler,PrivateKey,0,REG_SZ,pData,Value);
    }
    RegCloseKey(KeyHandler);
}

void RemoveInterface()//Replaced by SetInterface("", "") - WiFiCtrl.cpp
{
    HKEY KeyHandler;
    BYTE pData[256];
    DWORD Value=256;
    LONG RegStatus;
    BYTE pInterface[256];


    ZeroMemory(pInterface,256);
    ZeroMemory(pData,256);
    RegStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,SharingKey,0,0 ,&KeyHandler);
    RegStatus=RegSetValueEx(KeyHandler,PrivateKey,0,REG_SZ,pData,Value);
    RegStatus=RegSetValueEx(KeyHandler,PublicKey,0,REG_SZ,pData,Value);
    RegCloseKey(KeyHandler);
}


DWORD GPRSIPConfig(DWORD Flag)//Replaced by BOOL PublicIPConfig(TCHAR *pszPublicInterface, BOOL bEnable) - WifiCtrl.cpp
{
    HKEY   KeyHandler;
    DWORD    Data;
    DWORD    Value;
    DWORD    RegStatus;
    BYTE     pData[256];
    DWORD    Type;

    RegStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"comm\\WWAN1-Internet-1\\parms\\Tcpip",0,0 ,&KeyHandler);
    RegStatus = RegQueryValueEx(KeyHandler,L"EnableDHCP",NULL,NULL,(BYTE *)&Data,&Value);

    if(Flag == 1)
    {
        Value = 256;

        RegStatus = RegQueryValueEx(KeyHandler,L"DhcpIPAddress",NULL,&Type,pData,&Value);
        RegStatus = RegSetValueEx(KeyHandler,L"IPAddress",0,Type,pData,Value);
        Value = 256;

        RegStatus = RegQueryValueEx(KeyHandler,L"DhcpSubnetMask",NULL,&Type,pData,&Value);
        RegStatus = RegSetValueEx(KeyHandler,L"SubnetMask",0,Type,pData,Value);
        Value = 256;

        RegStatus = RegQueryValueEx(KeyHandler,L"DhcpDNS",NULL,&Type,pData,&Value);
        RegStatus = RegSetValueEx(KeyHandler,L"DNS",0,Type,pData,Value);

        Value = 256;
        RegStatus = RegQueryValueEx(KeyHandler,L"DhcpDefaultGateway",NULL,&Type,pData,&Value);
        RegStatus = RegSetValueEx(KeyHandler,L"DefaultGateway",0,Type,pData,Value);
        Data = 0;
        Value = sizeof(DWORD);
    }
    else
    {
        Data =1;
        Value = sizeof(DWORD);

    }

    RegStatus = RegSetValueEx(KeyHandler,L"Autocfg",0,REG_DWORD,(BYTE *)&Data,Value);
    RegStatus = RegSetValueEx(KeyHandler,L"EnableDHCP",0,REG_DWORD,(BYTE *)&Data,Value);
    CloseHandle(KeyHandler);
    RebindGPRSIP();
    return 0;
}
#endif //Functions replaced 
