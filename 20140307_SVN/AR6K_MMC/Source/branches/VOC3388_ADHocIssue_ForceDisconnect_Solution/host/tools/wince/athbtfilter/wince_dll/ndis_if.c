/*
 * Bluetooth filter DLL NDIS interface implementation
 *
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
 *
 */

#include "a_types.h"
#include "a_osapi.h"
#include "btfilter_wince.h"
#include <msgqueue.h>
#include <ndis.h>
#include <nuiouser.h>
#include <iphlpapi.h>
#include "athdrv_wince.h"


typedef struct _WLAN_CHANNEL_RANGE {
    int    ChannelNumber;
    ULONG  Center;  /* in Khz */
} WLAN_CHANNEL_RANGE;

#define MAX_WLAN_CHANNELS 14
#define MAX_NDIS_IOCTL_RETRY 10

const WLAN_CHANNEL_RANGE g_ChannelTable[MAX_WLAN_CHANNELS] = {
    { 1  , 2412000},
    { 2  , 2417000},
    { 3  , 2422000},
    { 4  , 2427000},
    { 5  , 2432000},
    { 6  , 2437000},
    { 7  , 2442000},
    { 8  , 2447000},
    { 9  , 2452000},
    { 10 , 2457000},
    { 11 , 2462000},
    { 12 , 2467000},
    { 13 , 2472000},
    { 14 , 2484000},
};

static int LookUpChannel(ULONG FreqKhz)
{
    int i;
    
    if (FreqKhz == 0) {
            /* not connected */
        return 0;    
    }
    
    for (i = 0; i < MAX_WLAN_CHANNELS; i++) {
        if (FreqKhz <= g_ChannelTable[i].Center) {
            break;
        }
    }
    return (i < MAX_WLAN_CHANNELS) ? g_ChannelTable[i].ChannelNumber : 0;
}


static void IndicateWlanOperatingChannel(ATHBT_FILTER_INFO *pInfo, int Channel)
{
    LONG  status;
    HKEY  hOpenKey;
    DWORD regVal = (DWORD)Channel;
    DWORD disposition;

    status = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
                            pInfo->ChannelInfoRegPath,
                            0,      /* reserved */
                            NULL,   /* lpClass */
                            0,      /* options */
                            0,      /* samDesired */
                            NULL,   /* security attributes */
                            &hOpenKey,
                            &disposition);

    if (status != ERROR_SUCCESS) {
        ATH_LOG_MSG(ZONE_ADAPTER, (TEXT("ATHBT: Failed to open path: %s \r\n"),pInfo->ChannelInfoRegPath));
            /* no path, nothing to update */
        return;
    }

    status = RegSetValueEx(hOpenKey,
                           TEXT("WlanChannel"),
                           0,
                           REG_DWORD,
                           (LPBYTE)&regVal,
                           sizeof(regVal));

    RegCloseKey(hOpenKey);

    if (ERROR_SUCCESS == status) {
        ATH_LOG_MSG(ZONE_ADAPTER, (TEXT("ATHBT: Device %s channel set: %d \r\n"),
                pInfo->AdapterName, Channel));
    }

}

static BOOL IssueQueryOid(ATHBT_FILTER_INFO *pInfo,
                          DWORD             Oid,
                          NDISUIO_QUERY_OID *pQueryOidBuffer,
                          DWORD             QueryBufferLength,
                          DWORD             *pRetLength)
{
    memset(pQueryOidBuffer, 0 , QueryBufferLength);
    pQueryOidBuffer->Oid = Oid;
    pQueryOidBuffer->ptcDeviceName = pInfo->AdapterName;

          /* issue OID */
    if (!DeviceIoControl(pInfo->hNdisUIO,
                         IOCTL_NDISUIO_QUERY_OID_VALUE,
                         pQueryOidBuffer,
                         sizeof(NDISUIO_QUERY_OID),
                         NULL,
                         QueryBufferLength,
                         pRetLength,
                         NULL)) {
            /* these can actually fail if the device powers down */
        ATH_LOG_MSG(ZONE_ADAPTER,(TEXT("ATHBT: Failed to issue query OID : 0x%X \r\n"),
                    GetLastError(),
                    Oid));
        return FALSE;
    }

    return TRUE;
}


static void UpdateWlanOperatingChannelInfo(ATHBT_FILTER_INFO *pInfo)
{
    UCHAR                     buffer[sizeof(NDISUIO_QUERY_OID) + sizeof(NDIS_802_11_CONFIGURATION)];
    DWORD                     retLength;
    NDIS_802_11_CONFIGURATION *pConfig;

    if (IssueQueryOid(pInfo,
                      OID_802_11_CONFIGURATION,
                      (NDISUIO_QUERY_OID *)buffer,
                      sizeof(buffer),
                      &retLength)) {

        pConfig = (NDIS_802_11_CONFIGURATION *)&(((NDISUIO_QUERY_OID *)buffer)->Data);
        IndicateWlanOperatingChannel(pInfo,LookUpChannel(pConfig->DSConfig));
    }

}

static void AdapterNotAvailable(ATHBT_FILTER_INFO *pInfo)
{
    ATH_LOG_MSG(ZONE_ADAPTER,(TEXT("ATHBT: Device %s is no longer available \r\n"), pInfo->AdapterName));
    pInfo->AdapterAvailCount = 0;
        /* clear operating channel info */
    IndicateWlanOperatingChannel(pInfo, 0);
        /* tell the filter layer to handle removal */
    HandleAdapterEvent(pInfo, ATH_ADAPTER_REMOVED);
}

static void AdapterAvailable(ATHBT_FILTER_INFO *pInfo)
{
    LONG newVal;

    newVal = InterlockedIncrement(&pInfo->AdapterAvailCount);

    if (newVal != 1) {
        return;
    }

    ATH_LOG_MSG(ZONE_ADAPTER, (TEXT("ATHBT: Device %s available \r\n"),pInfo->AdapterName));
        /* update operating channel info (if any) */
    UpdateWlanOperatingChannelInfo(pInfo);
    HandleAdapterEvent(pInfo, ATH_ADAPTER_ARRIVED);
}

#define NDIS_DEVICE_POWER_GUID          _T("{98C5250D-C29A-4985-AE5F-AFE5367E5006}\\")
#define NDIS_DEVICE_POWER_STR_MAX_LEN   (((sizeof(NDIS_DEVICE_POWER_GUID))/(sizeof(TCHAR))) + 64)

/* check if the adapter is really available and powered-up */
static BOOL IsAdapterAvail(PTCHAR pAdapterName)
{
    ULONG                index;
    TCHAR                ceDeviceName[NDIS_DEVICE_POWER_STR_MAX_LEN];
    BOOL                 retVal = FALSE;
    CEDEVICE_POWER_STATE devicePowerState;
    DWORD                status;
    
    do {
                
        if (_tcsncpy(ceDeviceName, NDIS_DEVICE_POWER_GUID, NDIS_DEVICE_POWER_STR_MAX_LEN) == NULL) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: string copy failed! \r\n")));
            break;    
        }
                
        if ((_tcslen(ceDeviceName) + _tcslen(pAdapterName)) >= NDIS_DEVICE_POWER_STR_MAX_LEN) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Device Name TOO big (%s%s) \r\n"),
                    ceDeviceName, pAdapterName));
            break;        
        }
 
        if ((GetAdapterIndex(pAdapterName, &index) != NO_ERROR)) {
                /* no adapter */
            break;    
        }
 
            /* form the full name */
        _tcsncat(ceDeviceName, 
                 pAdapterName, 
                 (NDIS_DEVICE_POWER_STR_MAX_LEN - _tcslen(ceDeviceName)));
                        
        status = GetDevicePower(ceDeviceName, POWER_NAME, &devicePowerState);
        
        if (status != ERROR_SUCCESS) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to get power state for %s (%d) \r\n"),
                    ceDeviceName, status));
            break;
        }
        
        ATH_LOG_MSG(ZONE_ADAPTER,(TEXT("ATHBT: Device Power State : %d \r\n"),devicePowerState));
        
        if (devicePowerState == D4) {
                /* device is not powered, treat it as not-available */
            ATH_LOG_MSG(ZONE_ADAPTER,(TEXT("ATHBT: Device is not powered. \r\n")));
            break;    
        }
        
            /* if we get here, the device is available and powered up */
        retVal = TRUE;
    
    } while (FALSE);
    

    return retVal;
}

void DisconnectNdisUIO(ATHBT_FILTER_INFO *pInfo)
{
    if (pInfo->hNdisUIO != INVALID_HANDLE_VALUE) {
        if (pInfo->MsgQueueRegistered) {
            pInfo->MsgQueueRegistered = FALSE;
                /* cancel notifications */
            DeviceIoControl(pInfo->hNdisUIO,IOCTL_NDISUIO_CANCEL_NOTIFICATION,
                            NULL,0,NULL,0,NULL,NULL);
        }
        CloseHandle(pInfo->hNdisUIO);
        pInfo->hNdisUIO = INVALID_HANDLE_VALUE;
    }
}

/* connect to the NDIS UIO driver */
static BOOL ConnectNdisUIO(ATHBT_FILTER_INFO *pInfo)
{
    int                             retry = 10;
    BOOL                            retVal = FALSE;
    NDISUIO_REQUEST_NOTIFICATION    ndisNotify;


    while ((retry > 0) && !pInfo->Shutdown && !pInfo->MsgQueueRegistered) {

            /* try to open the NDIS UIO streams driver */
        pInfo->hNdisUIO = CreateFile(NDISUIO_DEVICE_NAME,
                                     GENERIC_READ | GENERIC_WRITE,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                                     NULL,
                                     OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                     INVALID_HANDLE_VALUE);

        if (INVALID_HANDLE_VALUE == pInfo->hNdisUIO) {
            /* NDIS UIO driver may not have installed yet */
            Sleep(1000);
            retry--;
            continue;
        }

        memset(&ndisNotify, 0, sizeof(ndisNotify));

        ndisNotify.hMsgQueue = pInfo->hMsgQueue;
        ndisNotify.dwNotificationTypes =  NDISUIO_NOTIFICATION_BIND   |
                                          NDISUIO_NOTIFICATION_UNBIND |
                                          NDISUIO_NOTIFICATION_MEDIA_CONNECT |
                                          NDISUIO_NOTIFICATION_MEDIA_DISCONNECT |
                                          NDISUIO_NOTIFICATION_DEVICE_POWER_UP  |
                                          NDISUIO_NOTIFICATION_DEVICE_POWER_DOWN;

            /* register notifications through the message queue */
        if (!DeviceIoControl(pInfo->hNdisUIO,
                             IOCTL_NDISUIO_REQUEST_NOTIFICATION,
                             &ndisNotify,
                             sizeof(ndisNotify),
                             NULL,
                             0,
                             NULL,
                             NULL)) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to register NDIS notification (%d) \r\n"), GetLastError()));
            break;
        }

        ATH_LOG_MSG(ZONE_INIT,(TEXT("ATHBT: NDISUIO opened, and msg queue registered \r\n")));

        pInfo->MsgQueueRegistered = TRUE;

            /* see if adapter is already present, if it is we will not get any BIND notifications */
        if (IsAdapterAvail(pInfo->AdapterName)) {
            AdapterAvailable(pInfo);
        }

        retVal = TRUE;

    }

    if (retry <= 0) {
        ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: Failed to open NDIS UIO driver\r\n")));
        retVal = FALSE;
    }

    return retVal;
}

BOOL SetUpNdisIF(ATHBT_FILTER_INFO *pInfo)
{
        /* make sure operating channel info is reset until we can
         * determine what the actual operating channel */
    IndicateWlanOperatingChannel(pInfo, 0);
    return ConnectNdisUIO(pInfo);
}

void ProcessNdisNotifications(ATHBT_FILTER_INFO *pInfo)
{
    NDISUIO_DEVICE_NOTIFICATION notification;
    DWORD                       bytesRead;
    DWORD                       flags;

    while (1) {

        bytesRead = 0;

        if (!ReadMsgQueue(pInfo->hMsgQueue,
                          &notification,
                          sizeof(notification),
                          &bytesRead,
                          0,
                          &flags)) {
            LONG lastError = GetLastError();
                /* check last error, if this was a timeout, then there is no error since we
                 * specified a timeout of zero.  All we determined was that the message queue
                 * is now empty */
            if (lastError != ERROR_TIMEOUT) {
                ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: ReadMsgQueue failed (%d) \r\n"),lastError));
            }
            break;
        }

        if (0 == bytesRead) {
            break;
        }

        if (bytesRead < sizeof(notification)) {
            ATH_LOG_MSG(ZONE_ERROR,(TEXT("ATHBT: bytes from notification message is too small :%d \r\n"), bytesRead));
            continue;
        }

        ATH_LOG_MSG(ZONE_ADAPTER,(TEXT("ATHBT: Device %s , Notification:0x%X \r\n"),
             notification.ptcDeviceName, notification.dwNotificationType));

        if (_tcscmp(notification.ptcDeviceName, pInfo->AdapterName) != 0) {
            /* not our adapter */
            continue;
        }

        /* if we get here, this notification is for the adapter we are looking for */

        switch (notification.dwNotificationType) {
            case NDISUIO_NOTIFICATION_BIND:
                    /* NOTE: for bind notifications, we need to see if the bind may have arrived
                     * late (adapter was removed) -or- the device may be powered down in which the
                     * bind will arrive anyways, but the adapter is actually not available for control */
                if (IsAdapterAvail(pInfo->AdapterName)) {
                    AdapterAvailable(pInfo);
                }
                break;

            case NDISUIO_NOTIFICATION_UNBIND:
                    /* an unbind event occured, the adapter could have unloaded, check to see if the adapter is still there */
                Sleep(1000);
                if (!IsAdapterAvail(pInfo->AdapterName)) {
                    AdapterNotAvailable(pInfo);
                }
                break;

            case NDISUIO_NOTIFICATION_MEDIA_CONNECT:
                ATH_LOG_MSG(ZONE_ADAPTER,
                            (TEXT("ATHBT: Device %s media is now connected \r\n"),
                                pInfo->AdapterName));
                UpdateWlanOperatingChannelInfo(pInfo);
                break;

            case NDISUIO_NOTIFICATION_MEDIA_DISCONNECT:
                ATH_LOG_MSG(ZONE_ADAPTER,
                            (TEXT("ATHBT: Device %s media is now disconnected \r\n"),
                                pInfo->AdapterName));
                IndicateWlanOperatingChannel(pInfo, 0);
                break;

            case NDISUIO_NOTIFICATION_DEVICE_POWER_UP:
                ATH_LOG_MSG(ZONE_ADAPTER, (TEXT("ATHBT: Device %s is powering up \r\n"),
                        pInfo->AdapterName));
                AdapterAvailable(pInfo);
                break;

            case NDISUIO_NOTIFICATION_DEVICE_POWER_DOWN:
                ATH_LOG_MSG(ZONE_ADAPTER, (TEXT("ATHBT: Device %s is powering down \r\n"),
                        pInfo->AdapterName));
                AdapterNotAvailable(pInfo);
                break;
            default:
                break;
        }

    }

}


/* execute the BT action
 * this function is called by the single dispatcher thread
 */
void ExecuteBtAction(ATHBT_FILTER_INFO *pInfo, BT_ACTION_MSG *pBtActionMsg)
{
    UCHAR *pData = NULL;
    NDISUIO_SET_OID  *pSetOID;
    UCHAR            buffer[sizeof(NDISUIO_SET_OID) + sizeof(BT_CONTROL_ACTION)];
    DWORD            inputLength;
    DWORD            i;
	
    memset(buffer,0, sizeof(NDISUIO_SET_OID));

    pSetOID = (NDISUIO_SET_OID *)buffer;

    inputLength = sizeof(NDISUIO_SET_OID); /* set minimal length */

    if (pBtActionMsg->ControlAction.Length > sizeof(pSetOID->Data)) {
        inputLength += pBtActionMsg->ControlAction.Length - sizeof(pSetOID->Data);
    }

    pData = pSetOID->Data;

        /* copy the control action data */
    memcpy(pData, pBtActionMsg->ControlAction.Buffer, pBtActionMsg->ControlAction.Length);

    if (pBtActionMsg->ControlAction.Type == BT_CONTROL_ACTION_STATUS) {
            /* this action issues a STATUS OID command */
        pSetOID->Oid = OID_CAR6K_SET_BT_STATUS;
    } else {
            /* this action issues a PARAMS OID command */
        pSetOID->Oid = OID_CAR6K_SET_BT_PARAMS;
    }

    pSetOID->ptcDeviceName = pInfo->AdapterName;

    /* bug 35152: If NDIS is getting a lot of media connect events, sometimes it fails
           to execute OIDs in the first try. 2nd retry always seem to work. Adding retry
           workaround for now on the NDIS problem */
    for (i = 0; i < MAX_NDIS_IOCTL_RETRY; i++) {
        /* issue OID */
        if (DeviceIoControl(pInfo->hNdisUIO,
                        IOCTL_NDISUIO_SET_OID_VALUE,
                        pSetOID,
                        inputLength,
                        NULL,
                        0,
                        NULL,
                        NULL)) {

            ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: BT Action issued to adapter OID=0x%X  Length:%d (Action Length:%d)\r\n"),
                pSetOID->Oid,
                inputLength,
                pBtActionMsg->ControlAction.Length));

            if (ZONE_ACTION_DUMP) {
                AthFilterDumpBuffer(pBtActionMsg->ControlAction.Buffer,
                                    pBtActionMsg->ControlAction.Length,
                                    TEXT("BT Control Action"));
            }
			return;
        } else {
            /* these can actually fail if the device powers down */
            ATH_LOG_MSG(ZONE_ACTION,(TEXT("ATHBT: BT Action issue failed (%d) OID=0x%X  Total Length:%d (Action Length:%d)\r\n"),
                GetLastError(),
                pSetOID->Oid,
                inputLength,
                pBtActionMsg->ControlAction.Length));
        }
    }
}

