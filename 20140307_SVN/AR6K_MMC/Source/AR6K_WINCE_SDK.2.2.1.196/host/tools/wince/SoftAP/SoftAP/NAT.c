#include <windows.h>
#include <Ntddndis.h>
#include <Nuiouser.h>
#include <winioctl.h>

#include "NAT.h"
#include "Definitions.h"

#define IPROUNTER_DISABLE_CODE  0x128088
#define IPROUNTER_ENABLE_CODE	0x128084
#define DHCP_ENABLE_CODE	0x128074
#define AR6K2_CHECK_CODE	0xFF000021
#define AR6K1_CHECK_CODE	0xFF00000E	

BOOL EnableDHCP(BOOL bEnable)
{
    HANDLE DevHandler;
    HANDLE NATHandler;
    DWORD dwBytesReturned;
    BOOL bStatus = FALSE;
    DWORD NATData = (DWORD)bEnable;//Enable/Disable

    //open nat0, send enable dhcp ioctl

    printf (_Trace ("%s DHCP"), (bEnable ? "Enabling" : "Disabling") );

    NATHandler = CreateFile (_szNATDevInstanceName, 0xc0000000, 0, NULL, 3, 0, NULL);

    if (NULL == NATHandler)//if not active
    {
        _ERR ("Enabling NAT - CreateFile failed");

        DevHandler = ActivateDevice (_szConnectShareKey, 0);
        if (NULL == DevHandler)
        {
            //if error other than (2404 - The device is in use by an active process and cannot be disconnected.)
            if(2404 != GetLastError())
            {
                _ERR ("Enabling NAT - ActivateDevice failed");
                return FALSE;
            }
        }
        NATHandler = CreateFile (_szNATDevInstanceName, 0xc0000000, 0, NULL, 3, 0, NULL);
    }

    if (NULL == NATHandler)
    {
        _ERR ("Enabling NAT - CreateFile failed");

        if (DevHandler)
            CloseHandle (DevHandler);

        return FALSE;
    }

    printf(_Trace("Sending _DHCP_ENABLE_CODE to NAT0"));

    bStatus = DeviceIoControl (NATHandler, _DHCP_ENABLE_CODE, &NATData, 4, NULL, 0, &dwBytesReturned, NULL);

    if (NATHandler)
        CloseHandle (NATHandler);

    if (DevHandler)
        CloseHandle (DevHandler);

    return bStatus;
}

BOOL EnableIPRouting(BOOL bEnable)
{	
    HANDLE IPDHandler;
    DWORD dwInData = 0;
    DWORD dwOutData = 0;
    DWORD dwBytesReturned = 0;
    BOOL bStatus = FALSE;

    printf (_Trace ("%s IP routing"), (bEnable ? "Enabling" : "Disabling") );

    IPDHandler = CreateFile (_szIPDevInstanceName, 0xc0000000, 3, NULL, 4, 0, NULL);
    if (NULL == IPDHandler)
    {
        _ERR ("Failed to open IPD0");
        return FALSE;
    }

    printf(_Trace ("Sending IOCTL to IPD0"));

    bStatus = DeviceIoControl (IPDHandler, (bEnable ? _IPROUNTER_ENABLE_CODE : _IPROUNTER_DISABLE_CODE),
        &dwInData, sizeof (DWORD), &dwOutData, sizeof (DWORD), &dwBytesReturned,NULL);

    if(IPDHandler)
        CloseHandle (IPDHandler);

    return bStatus;
}

BOOL RebindAdapter(TCHAR *pszAdapterName)
{
    TCHAR szAdapterName[MAX_PATH] = {0,};
    DWORD dwReturnedBytes;
    HANDLE NDSHandler = INVALID_HANDLE_VALUE;

    _stprintf(szAdapterName, _T("%s\0\0"), pszAdapterName);//multiple null to end multiple string
    _tPRINTF (_T ("%sOpening NDS0 to rebind %s"), _szAppNameW, szAdapterName);

    NDSHandler = CreateFile (L"NDS0:", 0xF0000000, 3, NULL, 3, 0, NULL);

    if (NULL == NDSHandler)
    {
        _ERR ("Error opening NDS0 for rebind");
        return FALSE;
    }

    _tPRINTF (_T ("%sRebinding %s"), _szAppNameW, szAdapterName);

    if (! DeviceIoControl ( NDSHandler, IOCTL_NDIS_REBIND_ADAPTER, szAdapterName,
        (_tcslen (szAdapterName) * sizeof (TCHAR)), NULL, 0, &dwReturnedBytes, NULL)) 
    {
        _ERR ("Rebind ioctl failed");
        CloseHandle(NDSHandler);
        return FALSE;
    }

    _tPRINTF (_T ("%sRebind success: %s"), _szAppNameW, szAdapterName);

    CloseHandle(NDSHandler);
    return TRUE;
}

#if 0 //Not used

DWORD ActDev(HANDLE *DevHandler)
{

    DWORD Status =0;

    *DevHandler = ActivateDevice(L"Comm\\ConnectionSharing\\Driver",0);
    if(*DevHandler==NULL)
    {
        Status = GetLastError();
    }

    return Status;
}

void CloseNAT(HANDLE DevHandler,HANDLE NATHandler)
{
    if(DevHandler !=NULL)	
        CloseHandle(DevHandler);	
    if(NATHandler!=NULL)   
        CloseHandle(NATHandler);
}

DWORD OpenNATDrv(HANDLE *NATHandler)
{
    DWORD Status =0;	
    *NATHandler = CreateFile(L"NAT0:",0xc0000000,0,NULL,3,0,NULL);

    if(*NATHandler ==NULL)
    {
        Status = GetLastError();
    }

    return Status;	
}

DWORD EnableNAT(HANDLE NATHandler)
{
    DWORD Data=1;
    DWORD Status =0;
    BOOLEAN IoState;
    DWORD Bytereturned;
    IoState = DeviceIoControl(NATHandler,DHCP_ENABLE_CODE,&Data,4,NULL,0,&Bytereturned,NULL);
    if(IoState == FALSE)
    {
        Status = GetLastError();
    }

    return Status;
}
DWORD DisableNAT(HANDLE NATHandler)
{
    DWORD Data=0;
    DWORD Status =0;
    BOOLEAN IoState;
    DWORD Bytereturned;
    IoState = DeviceIoControl(NATHandler,DHCP_ENABLE_CODE,&Data,4,NULL,0,&Bytereturned,NULL);
    if(IoState == FALSE)
    {
        Status = GetLastError();
    }

    return Status;
}

DWORD OpenIPDriver(HANDLE *IPDHandler)
{
    DWORD Status =0;	

    *IPDHandler = CreateFile(L"IPD0:",0xc0000000,3,NULL,4,0,NULL);

    if(*IPDHandler ==NULL)
    {
        Status = GetLastError();
    }

    return Status;	
}

DWORD EnableIPRouting(HANDLE IPDHandler)
{
    //BYTE IPaddress[4]={0xC0,0xA8,0x00,0x01};
    DWORD InData=0;
    DWORD OutData=0;
    DWORD Status =0;
    BOOLEAN IoState;
    DWORD Bytereturned;
    IoState = DeviceIoControl(IPDHandler,IPROUNTER_ENABLE_CODE,&InData,sizeof(DWORD),&OutData,sizeof(DWORD),&Bytereturned,NULL);
    if(IoState == FALSE)
    {
        Status = GetLastError();
    }

    return Status;
}
DWORD DisableIPRouting(HANDLE IPDHandler)
{
    DWORD	InData=0;
    DWORD	OutData=0;
    DWORD Status =0;
    BOOLEAN IoState;
    DWORD Bytereturned;
    IoState = DeviceIoControl(IPDHandler,IPROUNTER_DISABLE_CODE,&InData,sizeof(DWORD),&OutData,sizeof(DWORD),&Bytereturned,NULL);
    if(IoState == FALSE)
    {
        Status = GetLastError();
    }

    return Status;
}

DWORD AtheroWiFiCheck()
{ 
    HANDLE WiFiHandler;
    DWORD IOState =0;
    PNDISUIO_QUERY_OID CheckOid;
    PNDISUIO_SET_OID   powerModeOid;
    PUCHAR buffer = NULL;
    DWORD LengthReq;
    DWORD dwReturnedBytes = 0;

    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_QUERY_OID)+sizeof(DWORD) )) == NULL)  
    {
        return 0x90002020;
    }
    ZeroMemory( buffer,sizeof(NDISUIO_QUERY_OID)+sizeof(DWORD) );
    CheckOid = (NDISUIO_QUERY_OID *)buffer;
    LengthReq = sizeof(NDISUIO_QUERY_OID) + sizeof(ULONG);
    CheckOid->Oid = AR6K2_CHECK_CODE;
    CheckOid->ptcDeviceName = (PTCHAR) malloc(_tcslen(TEXT("AR6K_SD1")) * sizeof(TCHAR) + 2);
    if (CheckOid->ptcDeviceName == NULL) {
        free(buffer);
        return 0x90002021;
    }
    wcscpy( CheckOid->ptcDeviceName, TEXT("AR6K_SD1") );


    WiFiHandler = CreateFile( NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        INVALID_HANDLE_VALUE );
    if(WiFiHandler==NULL)
    {
        free(buffer);
        IOState = GetLastError();
        return IOState;	

    }               	

    if(!DeviceIoControl( WiFiHandler, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
        sizeof(NDISUIO_QUERY_OID), buffer, LengthReq,
        &dwReturnedBytes, NULL ) ) 
    {
        CheckOid->Oid = AR6K1_CHECK_CODE;

        if(!DeviceIoControl( WiFiHandler, IOCTL_NDISUIO_QUERY_OID_VALUE, buffer,
            sizeof(NDISUIO_QUERY_OID), buffer, LengthReq,
            &dwReturnedBytes, NULL ) ) 
        {
            IOState = GetLastError();
        }
    }

    free(CheckOid->ptcDeviceName);
    free(buffer);


    // Turn Off power save mode
    if ((buffer = (PUCHAR) malloc( sizeof(NDISUIO_SET_OID) )) == NULL)  {
        IOState = 0x90002020;
        CloseHandle(WiFiHandler);
        return IOState;;
    }
    memset( buffer, 0, sizeof(NDISUIO_SET_OID) );
    powerModeOid = (NDISUIO_SET_OID *)buffer;

    powerModeOid->Oid = OID_802_11_POWER_MODE;
    powerModeOid->ptcDeviceName = (PTCHAR) malloc(_tcslen(TEXT("AR6K_SD1")) * sizeof(TCHAR) + 2);
    if (powerModeOid->ptcDeviceName == NULL) {
        free(buffer);
        CloseHandle(buffer);
        return 0x90002021;
    }
    wcscpy( powerModeOid->ptcDeviceName, TEXT("AR6K_SD1") );

    (*(unsigned int *)(powerModeOid->Data)) =0;

    // Pass the IOCTL to the device
    if (!DeviceIoControl( WiFiHandler, IOCTL_NDISUIO_SET_OID_VALUE,	buffer,
        sizeof(NDISUIO_SET_OID), NULL, 0,
        &dwReturnedBytes, NULL ) ) {
            IOState = GetLastError();
    } 

    free(powerModeOid->ptcDeviceName);
    free(buffer);
    CloseHandle(WiFiHandler);

    return IOState;

}

DWORD RebindWiFiAdapter()
{
    HANDLE NDSHandler;
    DWORD Status =0;	
    TCHAR AdapterName[] =L"AR6K_SD1\0\0";
    DWORD dwReturnedBytes;
    DWORD Length;
    Length = wcslen(L"AR6K_SD1")*sizeof(WCHAR);

    NDSHandler=CreateFile(L"NDS0:",0xF0000000,3,NULL,3,0,NULL);

    if(NDSHandler ==NULL)
    {
        Status = GetLastError();
        return Status;
    }
    if(!DeviceIoControl( NDSHandler, IOCTL_NDIS_REBIND_ADAPTER, AdapterName,
        sizeof(AdapterName),NULL,0,&dwReturnedBytes, NULL )) 
    {
        Status = GetLastError();
        CloseHandle(NDSHandler);
        return (Status |0x02000000);
    }
    CloseHandle(NDSHandler);
    return (Status);
}


DWORD RebindGPRSIP()
{
    HANDLE NDSHandler;
    DWORD Status =0;	
    TCHAR AdapterName[] =L"WWAN1-Internet-1\0\0";
    DWORD dwReturnedBytes;
    DWORD Length;
    Length = wcslen(L"WWAN1-Internet-1")*sizeof(WCHAR);

    NDSHandler=CreateFile(L"NDS0:",0xF0000000,3,NULL,3,0,NULL);

    if(NDSHandler ==NULL)
    {
        Status = GetLastError();
        return Status;
    }
    if(!DeviceIoControl( NDSHandler, IOCTL_NDIS_REBIND_ADAPTER, AdapterName,
        sizeof(AdapterName),NULL,0,&dwReturnedBytes, NULL )) 
    {
        Status = GetLastError();
        CloseHandle(NDSHandler);
        return (Status |0x02000000);
    }
    CloseHandle(NDSHandler);
    return (Status);

}
#endif //Not used


