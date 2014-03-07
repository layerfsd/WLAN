
//------------------------------------------------------------------------------
// <copyright file="car6koid.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// AR6000 NDIS Miniport class oid definitions
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

#include <windows.h>
#include <ndis.h>

#include "htc_internal.h"
#include "htc.h"
#include "wmi_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "athdrv.h"
#include "host_version.h"
extern "C" {
#include "bmi.h"
}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"
#include "ieee80211.h"
#include "platform.h"
#include "osapi.h"

#ifdef SUPPORT_WPA2
static NDIS_802_11_AUTHENTICATION_ENCRYPTION g_WPA2AuthEncryptPairSupported[] =
{
	{ Ndis802_11AuthModeOpen,    Ndis802_11EncryptionDisabled }, //open no-crypt
	{ Ndis802_11AuthModeOpen,    Ndis802_11Encryption1Enabled }, //open WEP
	{ Ndis802_11AuthModeWPA,     Ndis802_11Encryption2Enabled },
	{ Ndis802_11AuthModeWPA,     Ndis802_11Encryption3Enabled },
	{ Ndis802_11AuthModeWPAPSK,  Ndis802_11Encryption2Enabled },
	{ Ndis802_11AuthModeWPAPSK,  Ndis802_11Encryption3Enabled },
	
	{ (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2,    Ndis802_11Encryption2Enabled },
	{ (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2,    Ndis802_11Encryption3Enabled },
	{ (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2PSK, Ndis802_11Encryption2Enabled },
	{ (NDIS_802_11_AUTHENTICATION_MODE) Ndis802_11AuthModeWPA2PSK, Ndis802_11Encryption3Enabled },
};
#endif //SUPPORT_WPA2

#define ENCRYPT2_GRPWISE_CRYPTOINDEX_MAX 2
#define ENCRYPT3_GRPWISE_CRYPTOINDEX_MAX 3

static CRYPTO_TYPE Encryption2GrpwiseCryptoTable[ENCRYPT2_GRPWISE_CRYPTOINDEX_MAX] =
{
	TKIP_CRYPT,
	WEP_CRYPT
};

static CRYPTO_TYPE Encryption3GrpwiseCryptoTable[ENCRYPT3_GRPWISE_CRYPTOINDEX_MAX] =
{
	AES_CRYPT,
	TKIP_CRYPT,
	WEP_CRYPT
};

#ifdef HTC_RAW_INTERFACE

#define RAW_HTC_READ_BUFFERS_NUM                     16
#define RAW_HTC_WRITE_BUFFERS_NUM                    16

typedef struct {
    int currPtr;
    int length;
    unsigned char data[AR6000_BUFFER_SIZE];
} raw_htc_buffer;

static HANDLE raw_htc_read_sem[HTC_MAILBOX_NUM_MAX];
static HANDLE raw_htc_write_sem[HTC_MAILBOX_NUM_MAX];
static NDIS_EVENT raw_htc_read_queue[HTC_MAILBOX_NUM_MAX];
static NDIS_EVENT raw_htc_write_queue[HTC_MAILBOX_NUM_MAX];

static raw_htc_buffer raw_htc_read_buffer[HTC_MAILBOX_NUM_MAX][RAW_HTC_READ_BUFFERS_NUM];
static raw_htc_buffer raw_htc_write_buffer[HTC_MAILBOX_NUM_MAX][RAW_HTC_WRITE_BUFFERS_NUM];
static A_BOOL write_buffer_available[HTC_MAILBOX_NUM_MAX];
static A_BOOL read_buffer_available[HTC_MAILBOX_NUM_MAX];

static BOOL rawIfInit = false;

static void
ar6000_htc_raw_read_cb(HTC_TARGET *htcTarget, HTC_ENDPOINT_ID endPointId,
                       HTC_EVENT_ID evId, HTC_EVENT_INFO *evInfo, void *arg)
{
    HTC_TARGET *target;
    raw_htc_buffer *busy;

    target = (HTC_TARGET *)arg;
    AR_DEBUG_ASSERT(target != NULL);
    busy = (raw_htc_buffer *)evInfo->cookie;
    AR_DEBUG_ASSERT(busy != NULL);

    if ( WAIT_FAILED == WaitForSingleObject(raw_htc_read_sem[endPointId], INFINITE) ) {

		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "Unable to wait on the Semaphore\n");
	}

	AR_DEBUG_ASSERT(evId == HTC_BUFFER_RECEIVED);
    AR_DEBUG_ASSERT((evInfo->status != A_OK) || 
             (evInfo->buffer == (busy->data + HTC_HEADER_LEN)));

    if (evInfo->status == A_ECANCELED) {
        /*
         * HTC provides A_ECANCELED status when it doesn't want to be refilled
         * (probably due to a shutdown)
         */
        memset(busy, 0, sizeof(raw_htc_buffer));
        return;
    }

    busy->length = evInfo->actualLength + HTC_HEADER_LEN;
    busy->currPtr = HTC_HEADER_LEN;
    read_buffer_available[endPointId] = TRUE;
	
	ReleaseMutex(raw_htc_read_sem[endPointId]);

    //up(&raw_htc_read_sem[endPointId]);

    /* Signal the waiting process */

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Waking up the endpoint(%d) read process\n", endPointId);

    //AR_DEBUG2_PRINTF("Waking up the endpoint(%d) read process\n", endPointId);
	NdisSetEvent(&raw_htc_read_queue[endPointId]);

    //wake_up_interruptible(&raw_htc_read_queue[endPointId]);
}

static void
ar6000_htc_raw_write_cb(HTC_TARGET *htcTarget, HTC_ENDPOINT_ID endPointId,
                        HTC_EVENT_ID evId, HTC_EVENT_INFO *evInfo, void *arg)
{
    HTC_TARGET *target;
    raw_htc_buffer *free;

    target = (HTC_TARGET *)arg;

    AR_DEBUG_ASSERT(target != NULL);
    free = (raw_htc_buffer *)evInfo->cookie;
    AR_DEBUG_ASSERT(free != NULL);

    if ( WAIT_FAILED == WaitForSingleObject(raw_htc_write_sem[endPointId], INFINITE) ) {

		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "Unable to wait on the Semaphore\n");
	}

    AR_DEBUG_ASSERT(evId == HTC_BUFFER_SENT);
    AR_DEBUG_ASSERT(evInfo->buffer == (free->data + HTC_HEADER_LEN));

    free->length = 0;
    write_buffer_available[endPointId] = TRUE;
	ReleaseMutex(raw_htc_write_sem[endPointId]);

    /* Signal the waiting process */
	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Waking up the endpoint(%d) write process\n", endPointId);

	NdisSetEvent(&raw_htc_write_queue[endPointId]);
}

static void
ar6000_htc_raw_unread_cb(HTC_TARGET *htcTarget, HTC_ENDPOINT_ID eid,
                         HTC_EVENT_ID evId, HTC_EVENT_INFO *evInfo, void *arg)
{
    HTC_TARGET *target;

    target = (HTC_TARGET *)arg;
    AR_DEBUG_ASSERT(target != NULL);

	NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "Not Implemented\n");
}

static int
ar6000_htc_raw_open(HTC_TARGET *htcTarget)
{
    A_STATUS status;
    unsigned int count1, count2;
    raw_htc_buffer *buffer;

    for (count1 = 0; count1 < HTC_MAILBOX_NUM_MAX; count1 ++) {
        /* Initialize the data structures */
		if (!(raw_htc_read_sem[count1] = CreateMutex(NULL,FALSE,NULL)) ) {
			return -1;
		}
		if (!(raw_htc_write_sem[count1] = CreateMutex(NULL, FALSE, NULL)) ) {
			return -1;
		}
        
		NdisInitializeEvent(&raw_htc_read_queue[count1]);
		NdisInitializeEvent(&raw_htc_write_queue[count1]);

        /* Register the event handlers */
        if ((status = HTCEventReg(htcTarget, (HTC_ENDPOINT_ID)count1, HTC_BUFFER_RECEIVED,
                                  ar6000_htc_raw_read_cb, htcTarget)) != A_OK)
        {
            BMIInit();
            return -1;
        }
        if ((status = HTCEventReg(htcTarget, (HTC_ENDPOINT_ID)count1, HTC_DATA_AVAILABLE,
                                  ar6000_htc_raw_unread_cb, htcTarget)) != A_OK)
        {
            BMIInit();
            return -1;
        }
        if ((status = HTCEventReg(htcTarget, (HTC_ENDPOINT_ID)count1, HTC_BUFFER_SENT,
                                  ar6000_htc_raw_write_cb, htcTarget)) != A_OK)
        {
            BMIInit();
            return -1;
        }

        for (count2 = 0; count2 < RAW_HTC_READ_BUFFERS_NUM; count2 ++) {
            /* Initialize the receive buffers */
            buffer = &raw_htc_write_buffer[count1][count2];
            memset(buffer, 0, sizeof(raw_htc_buffer));
            buffer = &raw_htc_read_buffer[count1][count2];
            memset(buffer, 0, sizeof(raw_htc_buffer));

            /* Queue buffers to HTC for receive */
            if ((status = HTCBufferReceive(htcTarget, (HTC_ENDPOINT_ID)count1, buffer->data,
                                           AR6000_BUFFER_SIZE, buffer)) != A_OK)
            {
                BMIInit();
                return -1;
            }
        }

        for (count2 = 0; count2 < RAW_HTC_WRITE_BUFFERS_NUM; count2 ++) {
            /* Initialize the receive buffers */
            buffer = &raw_htc_write_buffer[count1][count2];
            memset(buffer, 0, sizeof(raw_htc_buffer));
        }

        read_buffer_available[count1] = FALSE;
        write_buffer_available[count1] = TRUE;
    }

    /* Start the HTC component */
    if ((status = HTCStart(htcTarget)) != A_OK) {
        BMIInit();
        return -1;
    }

    return 0;
}

static int
ar6000_htc_raw_close(HTC_TARGET *htcTarget)
{
    int count;
    A_STATUS status;

    /* Stop the HTC */
    HTCStop(htcTarget);

    /* Unregister the event handlers */
    for (count = 0; count < HTC_MAILBOX_NUM_MAX; count ++) {
        status = HTCEventReg(htcTarget, (HTC_ENDPOINT_ID)count, HTC_BUFFER_RECEIVED,
                             NULL, htcTarget);

        status = HTCEventReg(htcTarget, (HTC_ENDPOINT_ID)count, HTC_DATA_AVAILABLE,
                             NULL, htcTarget);

        status = HTCEventReg(htcTarget, (HTC_ENDPOINT_ID)count, HTC_BUFFER_SENT,
                             NULL, htcTarget);

		/* free the NDIS event structures */
		NdisFreeEvent(&raw_htc_read_queue[count]);
		NdisFreeEvent(&raw_htc_write_queue[count]);
    }

	/* Initialize the BMI component */
    BMIInit();

    return 0;
}

raw_htc_buffer *
get_filled_buffer(HTC_ENDPOINT_ID endPointId)
{
    int count;
    raw_htc_buffer *busy;

    /* Check for data */
    for (count = 0; count < RAW_HTC_READ_BUFFERS_NUM; count ++) {
        busy = &raw_htc_read_buffer[endPointId][count];
        if (busy->length) {
            break;
        }
    }
    if (busy->length) {
        read_buffer_available[endPointId] = TRUE;
    } else {
        read_buffer_available[endPointId] = FALSE;
    }

    return busy;
}

static size_t
ar6000_htc_raw_read(HTC_TARGET *htcTarget, HTC_ENDPOINT_ID endPointId, 
                    char *buffer, int length)
{
    int readPtr;
    raw_htc_buffer *busy;

	if ( WAIT_FAILED == WaitForSingleObject(raw_htc_read_sem[endPointId], INFINITE) ) {
		return -1;
	}

    busy = get_filled_buffer(endPointId);
    while (!read_buffer_available[endPointId]) {
        
		ReleaseMutex(raw_htc_read_sem[endPointId]);
		//up(&raw_htc_read_sem[endPointId]);

        /* Wait for the data */
		NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Sleeping endpoint(%d) read process\n", endPointId);

		NdisWaitEvent(&raw_htc_read_queue[endPointId], 0);

		NdisResetEvent(&raw_htc_read_queue[endPointId]);

		if ( WAIT_FAILED == WaitForSingleObject(raw_htc_read_sem[endPointId], INFINITE) ) {
			return -1;
		}
        
        busy = get_filled_buffer(endPointId);
    }

    /* Read the data */
    readPtr = busy->currPtr;
    if (length > busy->length - HTC_HEADER_LEN) {
        length = busy->length - HTC_HEADER_LEN;
    }

	memcpy(buffer, &busy->data[readPtr], length);

    /* if (copy_to_user(buffer, &busy->data[readPtr], length)) {
        up(&raw_htc_read_sem[endPointId]);
        return -EFAULT;
    } */

    busy->currPtr += length;
    if (busy->currPtr == busy->length)
    {
        /* Packet has been completely read. Queue it with HTC */
        memset(busy, 0, sizeof(raw_htc_buffer));
        HTCBufferReceive(htcTarget, endPointId, busy->data,
                         AR6000_BUFFER_SIZE, busy);
    }
    read_buffer_available[endPointId] = FALSE;

	ReleaseMutex(raw_htc_read_sem[endPointId]);
    //up(&raw_htc_read_sem[endPointId]);

    return length;
}

raw_htc_buffer *
get_free_buffer(HTC_ENDPOINT_ID endPointId)
{
    int count;
    raw_htc_buffer *free;

    free = NULL;
    for (count = 0; count < RAW_HTC_WRITE_BUFFERS_NUM; count ++) {
        free = &raw_htc_write_buffer[endPointId][count];
        if (free->length == 0) {
            break;
        }
    }
    if (!free->length) {
        write_buffer_available[endPointId] = TRUE;
    } else {
        write_buffer_available[endPointId] = FALSE;
    }

    return free;
}

static size_t
ar6000_htc_raw_write(HTC_TARGET *htcTarget, HTC_ENDPOINT_ID endPointId,
                     char *buffer, int length)
{
    int writePtr;
    raw_htc_buffer *free;

   	if ( WAIT_FAILED == WaitForSingleObject(raw_htc_write_sem[endPointId], INFINITE) ) {
		return -1;
	}

    /* Search for a free buffer */
    free = get_free_buffer(endPointId);

    /* Check if there is space to write else wait */
    while (!write_buffer_available[endPointId]) {

		ReleaseMutex(raw_htc_write_sem[endPointId]);

        /* Wait for buffer to become free */
		NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Sleeping endpoint(%d) write process\n", endPointId);

		NdisWaitEvent(&raw_htc_write_queue[endPointId], 0);

		NdisResetEvent(&raw_htc_write_queue[endPointId]);
		if ( WAIT_FAILED == WaitForSingleObject(raw_htc_write_sem[endPointId], INFINITE) ) {
			return -1;
		}

        free = get_free_buffer(endPointId);
    }

    /* Send the data */
    writePtr = HTC_HEADER_LEN;
    if (length > (AR6000_BUFFER_SIZE - HTC_HEADER_LEN)) {
        length = AR6000_BUFFER_SIZE - HTC_HEADER_LEN;
    }

	memcpy(&free->data[writePtr], buffer, length);

    /* if (copy_from_user(&free->data[writePtr], buffer, length)) {
		ReleaseMutex(raw_htc_write_sem[endPointId]);
       return -1;
    } */

    free->length = length;
    HTCBufferSend(htcTarget, endPointId, &free->data[writePtr], length, free);
    write_buffer_available[endPointId] = FALSE;

	ReleaseMutex(raw_htc_write_sem[endPointId]);

    return length;
}

#endif //HTC_RAW_INTERFACE

A_STATUS 
CAR6KMini::ar6000_get_target_stats()
{
	A_STATUS status = A_OK;
	NdisResetEvent(&m_tgtStatsEvent);

	status = wmi_get_stats_cmd((wmi_t*)m_pWMI);
	if ( status != A_OK)
		return status;

	if( !NdisWaitEvent(&m_tgtStatsEvent, AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETSTATS) ) {

		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - No TGT_GETSTATS event after %u ms\n", AR6K_DEFAULT_MS_TO_WAIT_FOR_TGT_GETSTATS);
		status = A_ERROR;
	}

	return status;
}

static MPOidInfo g_Car6kSupportedQueryOids[] =
{
	{ OID_CAR6K_GET_PNP_POWER, sizeof(ULONG) },
    { OID_CAR6K_RESUME_WLAN_STATE, sizeof(ULONG) },
	{ OID_CAR6K_FIRMWARE_VERSION, sizeof(ULONG) },
	{ 0,                             0 }
};

NDIS_STATUS
CAR6KMini::QueryInformation(
	IN  NDIS_OID Oid, 
	OUT PVOID    Buffer, 
	IN  ULONG    cbBuffer,
	OUT PULONG   pcbWritten,
	OUT PULONG   pcbNeeded)
//
// QueryInformation is a required function that returns information
// about the capabilities and staus of the driver and/or its NIC.
//
{
	NDIS_STATUS                 Status = NDIS_STATUS_SUCCESS;

	// Check that OID is supported and buffer size is reasonable.
	Status = CheckOidRequest(&g_Car6kSupportedQueryOids[0], Oid, cbBuffer, pcbNeeded);
	if (NDIS_STATUS_INVALID_OID == Status)
	{
		// OID is not a supported CAR6K OID. Check to see if it is an 802_11 OID.
		Status = C802_11Miniport::QueryInformation(Oid, Buffer, cbBuffer, pcbWritten, pcbNeeded);
		goto done;
	}
	if (NDIS_STATUS_SUCCESS != Status)
		goto done;
	
	switch (Oid)
	{
		case OID_CAR6K_GET_PNP_POWER:
			Status = GetPnpPower((ULONG *)Buffer);
			break;

		case OID_CAR6K_RESUME_WLAN_STATE:
			Status = GetResumeWlanState((ULONG *)Buffer);
			break;

		case OID_CAR6K_FIRMWARE_VERSION:
			Status = GetFirmwareVersion((ULONG *)Buffer);
			break;

		default:
			Status = NDIS_STATUS_INVALID_OID;
			break;
	}
    
	if (NDIS_STATUS_SUCCESS == Status)
	{
		if (*pcbNeeded > cbBuffer)
			Status = NDIS_STATUS_INVALID_LENGTH;
		else
			*pcbWritten = *pcbNeeded;
	}
    
done:
	return Status;
}

static MPOidInfo g_Car6kSupportedSetOids[] =
{
	{ OID_CAR6K_802_11_AUTH_ALG, sizeof(ULONG) },
    { OID_CAR6K_RESUME_WLAN_STATE, sizeof(ULONG) },
	{ OID_CAR6K_BMI_DONE, 0 },
	{ OID_CAR6K_BMI_WRITE_MEMORY, sizeof(BMI_WRITE_MEMORY_PARAM) },
	{ OID_CAR6K_BMI_EXECUTE, sizeof(BMI_EXECUTE_PARAM) },
	{ OID_CAR6K_BMI_SET_APP_START, sizeof(BMI_SET_APP_START_PARAM) },
	{ OID_CAR6K_BMI_WRITE_SOC_REGISTER, sizeof(BMI_WRITE_SOC_REG_PARAM) },
	{ OID_CAR6K_BMI_READ_MEMORY, sizeof(BMI_READ_MEMORY_PARAM) },
	{ OID_CAR6K_BMI_READ_SOC_REGISTER, sizeof(BMI_READ_SOC_REG_PARAM) },
#ifdef HTC_RAW_INTERFACE
	{ OID_CAR6K_HTC_RAW_OPEN, 0 },
	{ OID_CAR6K_HTC_RAW_CLOSE, 0 },
	{ OID_CAR6K_HTC_RAW_READ, sizeof(HTC_RAW_READ_PARAM) },
	{ OID_CAR6K_HTC_RAW_WRITE, sizeof(HTC_RAW_WRITE_PARAM) },
#endif //HTC_RAW_INTERFACE

	{ 0, 0 }
};

NDIS_STATUS
CAR6KMini::SetInformation(
	IN  NDIS_OID Oid, 
	IN  PVOID    Buffer, 
	IN  ULONG    cbBuffer,
	OUT PULONG   pcbRead,
	OUT PULONG   pcbNeeded)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;


	// Check that OID is supported and buffer size is reasonable.
	Status = CheckOidRequest(&g_Car6kSupportedSetOids[0], Oid, cbBuffer, pcbNeeded);
	if (NDIS_STATUS_INVALID_OID == Status)
	{
		// OID is not a supported CAR6K OID. Check to see if it is an 802_11 OID.
		Status = C802_11Miniport::SetInformation(Oid, Buffer, cbBuffer, pcbRead, pcbNeeded);
		goto done;
	}

	if (NDIS_STATUS_SUCCESS != Status)
		goto done;

	switch (Oid)
	{
		case OID_CAR6K_802_11_AUTH_ALG:
			Status = SetCAr6k802_11AuthAlg(*(ULONG *)Buffer);
			break;

		case OID_CAR6K_RESUME_WLAN_STATE:
			Status = SetResumeWlanState(*(ULONG *)Buffer);
			break;

		case OID_CAR6K_BMI_DONE:
			Status = SetCAr6kBmiDone(Buffer);
			break;
		
		case OID_CAR6K_BMI_READ_MEMORY:
			Status = SetCAr6kBmiReadMemory((BMI_READ_MEMORY_PARAM *)Buffer);
			break;

		case OID_CAR6K_BMI_WRITE_MEMORY:
			Status = SetCAr6kBmiWriteMemory((BMI_WRITE_MEMORY_PARAM *)Buffer);
			break;

		case OID_CAR6K_BMI_EXECUTE:
			Status = SetCAr6kBmiExecute((BMI_EXECUTE_PARAM *)Buffer);
			break;

		case OID_CAR6K_BMI_SET_APP_START:
			Status = SetCAr6kBmiSetAppStart((BMI_SET_APP_START_PARAM *)Buffer);
			break;

		case OID_CAR6K_BMI_READ_SOC_REGISTER:
			Status = SetCAr6kBmiReadSocRegister((BMI_READ_SOC_REG_PARAM *)Buffer);
			break;

		case OID_CAR6K_BMI_WRITE_SOC_REGISTER:
			Status = SetCAr6kBmiWriteSocRegister((BMI_WRITE_SOC_REG_PARAM *)Buffer);
			break;

#ifdef HTC_RAW_INTERFACE
		case OID_CAR6K_HTC_RAW_OPEN:
			Status = SetCAr6kHtcRawOpen();
			break;

		case OID_CAR6K_HTC_RAW_CLOSE:
			Status = SetCAr6kHtcRawClose();
			break;

		case OID_CAR6K_HTC_RAW_READ:
			Status = SetCAr6kHtcRawRead((HTC_RAW_READ_PARAM *)Buffer);
			break;

		case OID_CAR6K_HTC_RAW_WRITE:
			Status = SetCAr6kHtcRawWrite((HTC_RAW_WRITE_PARAM *)Buffer);
			break;
#endif //HTC_RAW_INTERFACE

		default:
			ASSERT(FALSE);
			Status = NDIS_STATUS_INVALID_OID;
			break;
	}

done:
	return Status;
}

///////////// OID_GEN Query Handlers ////////////////////


PNDIS_OID
CAR6KMini::GetSupportedOidList(
	OUT     PNDIS_OID  pOidBuffer, 
	IN  OUT PULONG     pMaxOidsToAddToBuffer,
	IN  OUT PULONG     pTotalNumberOfSupportedOids)
//
//  Add the OIDs supported by the driver to pOidBuffer, but
//  do not add more than MaxOidsToAddToBuffer.
//
//  TotalNumberOfSupportedOids is set to the number of Oids
//  that the driver supports.
//
//  Returns a pointer to the next unused slot in pOidBuffer
//  after the oids have been added.
//
{
	PNDIS_OID   p_Car6kOidListStart;
	ULONG       OidListCount = 0;

	// First fill in base class supported OIDs
	p_Car6kOidListStart = C802_11Miniport::GetSupportedOidList(pOidBuffer, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

	// Now append CAR6K OIDs
	AddOidsToList(&g_Car6kSupportedQueryOids[0], p_Car6kOidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);
	AddOidsToList(&g_Car6kSupportedSetOids[0],   p_Car6kOidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

	return p_Car6kOidListStart + OidListCount;
}

ULONG
CAR6KMini::GetGenVendorDriverVersion()
{
	return ATH_SW_VER_MAJOR << 16 | ATH_SW_VER_MINOR;
}

ULONG
CAR6KMini::GetGenMacOptions()
// A bitmask that defines optional properties of the underlying driver or its NIC.
{
	return NDIS_MAC_OPTION_NO_LOOPBACK;
}

NDIS_HARDWARE_STATUS
CAR6KMini::GetGenHardwareStatus()
// The current hardware status of the underlying NIC 
{
	NDIS_HARDWARE_STATUS HardwareStatus;

	if (!m_WMIReady)
		HardwareStatus =  NdisHardwareStatusInitializing;
	else
		HardwareStatus = NdisHardwareStatusReady;

	return HardwareStatus;
}

NDIS_MEDIA_STATE
CAR6KMini::GetGenMediaConnectStatus()
// Whether the media is currently connected and able to Tx/Rx packets.
{
	NDIS_MEDIA_STATE MediaState;

	if (m_Connected)
		MediaState = NdisMediaStateConnected;
	else
		MediaState = NdisMediaStateDisconnected;

	return MediaState;
}

ULONG
CAR6KMini::GetGenLinkSpeed()
// Link speed in units of 100 bps.
{
	return m_BitrateKpbs * 10;
}

ULONG
CAR6KMini::GetGenTransmitBufferSpace()
// The amount of memory, in bytes, on the NIC that is available for buffering Tx data.
{
	return 1512 * 16;
}

ULONG
CAR6KMini::GetGenReceiveBufferSpace()
// The amount of memory, in bytes, on the NIC that is available for buffering Rx data.
{
	return 1512 * 16;
}

ULONG
CAR6KMini::GetGenTransmitBlockSize()
// Minimum amount of storage, in bytes, that a single packet
// occupies in the Tx buffer space of the NIC.
{
	return 1500;
}

ULONG
CAR6KMini::GetGenReceiveBlockSize()
// Minimum amount of storage, in bytes, that a single packet
// occupies in the Rx buffer space of the NIC.
{
	return 1500;
}

#define ATHEROS_IEEE_VENDOR_CODE	       0x00037F
#define ATHEROS_AR6001_NIC_CODE       0x00 // Placeholder

ULONG
CAR6KMini::GetGenVendorId()
// The OID_GEN_VENDOR_ID OID specifies a three-byte IEEE-registered vendor code,
// followed by a single byte that the vendor assigns to identify a particular NIC.
// Vendors without an IEEE registered code should use 0xFFFFFF for the vendor code.
{
	return (ATHEROS_IEEE_VENDOR_CODE << 8) | ATHEROS_AR6001_NIC_CODE;
}

const char *
CAR6KMini::GetGenVendorDescription()
// The OID_GEN_VENDOR_DESCRIPTION OID points to a zero-terminated string describing the NIC.
{
	return "Atheros AR6001 802.11 Network Adapter";
}

USHORT
CAR6KMini::GetGenDriverVersion()
// The NDIS version in use by the NIC driver.
// The high byte is the major version number; the low byte is the minor version number.
{
	return (m_NdisDriverMajorVersion << 8) | m_NdisDriverMinorVersion;
}

ULONG
CAR6KMini::GetSupportedPacketTypes()
// Return the types of packets supported by the miniport,
// used in conjuction with OID_GEN_CURRENT_PACKET_FILTER requests.
{
	return NDIS_PACKET_TYPE_ALL_MULTICAST
		 | NDIS_PACKET_TYPE_ALL_LOCAL
		 | NDIS_PACKET_TYPE_BROADCAST
		 | NDIS_PACKET_TYPE_DIRECTED
		 | NDIS_PACKET_TYPE_MULTICAST
		 ;
}

ULONG
CAR6KMini::GetGenXmitOk()
// Number of packets transmitted ok. 
{
	return 0;
}

ULONG
CAR6KMini::GetGenRcvOk()
// Number of packets received ok. 
{
	return 0;
}

ULONG
CAR6KMini::GetGenXmitError()
// Number of transmitted packets that had errors. 
{
	return 0;
}

ULONG
CAR6KMini::GetGenRcvError()
// Number of received packets that had FCS/framing errors. 
{
	return 0;
}

ULONG
CAR6KMini::GetGenRcvNoBuffer()
// Number of packets that could not be received because no buffer space was available. 
{
	return 0;
}

void
CAR6KMini::GetPnpCapabilities(
	OUT NDIS_PNP_CAPABILITIES *pCaps
	)
{
	// Wake on event not supported but power mgmt is supported
	PNDIS_PM_WAKE_UP_CAPABILITIES pWakeCaps = &pCaps->WakeUpCapabilities;
 
	pWakeCaps->MinMagicPacketWakeUp = NdisDeviceStateUnspecified;
	pWakeCaps->MinPatternWakeUp     = NdisDeviceStateUnspecified;
	pWakeCaps->MinLinkChangeWakeUp  = NdisDeviceStateUnspecified;
 

	return;
}
 
NDIS_STATUS
CAR6KMini::QueryPnpPower(
		IN NDIS_DEVICE_POWER_STATE *pDevPowerState
		)
{
	NDIS_STATUS Status;

	if (*pDevPowerState == NdisDeviceStateD3) {
		Status = NDIS_STATUS_SUCCESS;
	} else {
		Status = NDIS_STATUS_NOT_SUPPORTED;
	}

	return Status;
}

 
BYTE *
CAR6KMini::Get802_3PermanentAddress()
// The address of the NIC encoded in the hardware.
{
	return &m_PermanentAddress[0];
}

BYTE *
CAR6KMini::Get802_3CurrentAddress()
// The address the NIC is currently using.
{
	return &m_CurrentAddress[0];
}


ULONG
CAR6KMini::Get802_3MacOptions()
{
	return 0;
}

ULONG
CAR6KMini::Get802_3RcvErrorAlignment()
// Number of received packets that had alignment errors
{
	return 0;
}

ULONG
CAR6KMini::Get802_3XmitOneCollision()
// Number of transmitted packets that were transmitted after one collision
{
	// Since AR6K is a wireless NIC and not using the Ethernet physical layer
	// there are no collisions.
	return 0;
}

ULONG
CAR6KMini::Get802_3XmitMoreCollision()
// Number of transmitted packets that were transmitted after more than one collision
{
	// Since AR6K is a wireless NIC and not using the Ethernet physical layer
	// there are no collisions.
	return 0;
}

NDIS_STATUS
 CAR6KMini::SetPnpPower(
         IN NDIS_DEVICE_POWER_STATE *pDevPowerState
         )
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	WCHAR       sysPwrStateName[32];
	DWORD       sysPwrState;
	DWORD       result;

	// Already in the requested power state
	if (m_PowerState == *pDevPowerState) {
		return NDIS_STATUS_SUCCESS;
	} 

#ifdef UNDER_CE
	// Don't call the Power Manager APIs directly.
	// Doing so assumes the PM is sysgen'ed into every image.
	typedef DWORD GetSystemPowerStateProto(LPWSTR, DWORD, PDWORD);
	GetSystemPowerStateProto *pfnGetSystemPowerState = NULL;
	
	HMODULE hCoreDll = (HMODULE) LoadLibrary(TEXT("coredll.dll"));
	if (NULL != hCoreDll) {
	    pfnGetSystemPowerState = (GetSystemPowerStateProto *)
	        GetProcAddress(hCoreDll, TEXT("GetSystemPowerState"));
	}

	if (NULL == pfnGetSystemPowerState) {
	    result = ERROR_SUCCESS;
	} else {
	    result = pfnGetSystemPowerState(sysPwrStateName, sizeof(sysPwrStateName) / sizeof(WCHAR), &sysPwrState);
	}
	
	if (NULL != hCoreDll) {
	    FreeLibrary(hCoreDll);
	}
#else /*!UNDER_CE*/
	result = GetSystemPowerState(sysPwrStateName, sizeof(sysPwrStateName) / sizeof(WCHAR), &sysPwrState);
#endif /*UNDER_CE*/

	if (result != ERROR_SUCCESS) {
		return NDIS_STATUS_FAILURE;
    }

	// Coming out of standby and the resume state is off, keep 
	// the WLAN module in off state by failing the OID.
	if ((!(m_LastSysState & POWER_STATE_ON)) && 
		(m_ResumeWlanState == 0) &&
		(*pDevPowerState == NdisDeviceStateD0)) 
	{
			m_LastSysState = sysPwrState;
			return NDIS_STATUS_FAILURE;
	}
	m_LastSysState = sysPwrState;
 
	if (*pDevPowerState == NdisDeviceStateD3) {
		if (!m_WMIReady) {
			Status = NDIS_STATUS_ADAPTER_NOT_READY;
		} else {
			if (m_Connected) {
				NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
				NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
				m_Connected = FALSE;
			}
			StopEndpoints();

			plat_suspend();
		}
	} else if (*pDevPowerState == NdisDeviceStateD0) {
		plat_resume();
 
		Init();
        
		BMIInit();
 
		if (StartEndpoints() != A_OK) {
			Status = NDIS_STATUS_FAILURE;
		} 
	}
 
	if (Status == NDIS_STATUS_SUCCESS) {
		m_PowerState = *pDevPowerState;
	}

	// Set the event and notify the apps abt the status change
	SetEvent(m_PowerChangeEvent);

	return Status;
}



// AR6K 802.11 OID Query Handlers

NDIS_STATUS
CAR6KMini::Get802_11AssociatedAPBSSID(
	OUT NDIS_802_11_MAC_ADDRESS *BSSID)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

	// If the adapter is not associated, return NDIS_STATUS_ADAPTER_NOT_READY
	if (!m_Connected)
		Status = NDIS_STATUS_ADAPTER_NOT_READY;
	else
		memcpy(BSSID, m_PeerBSSID, ETHERNET_MAC_ADDRESS_LENGTH);

	return Status;
}

void
CAR6KMini::Get802_11SSID(
	OUT NDIS_802_11_SSID *pSSID)
{
	// If the adapter is not associated, return 0 for SsidLength.
	if (!m_Connected)
		pSSID->SsidLength = 0;
	else
		*pSSID = m_SSID;
}

static NDIS_802_11_NETWORK_TYPE g_AR6KNetworkTypesSupported[] =
{
	Ndis802_11DS,
	Ndis802_11OFDM24,
};

void
CAR6KMini::Get802_11NetworkTypesSupported(
	OUT NDIS_802_11_NETWORK_TYPE **ppTypes,
	OUT PULONG                     pNumTypes)
{
	*ppTypes = &g_AR6KNetworkTypesSupported[0];
	*pNumTypes = sizeof(g_AR6KNetworkTypesSupported) / sizeof(g_AR6KNetworkTypesSupported[0]);
}

void
CAR6KMini::Get802_11NetworkTypeInUse(
	OUT NDIS_802_11_NETWORK_TYPE *pType)
{
	*pType = m_NetworkTypeInUse;
}

NDIS_STATUS
CAR6KMini::Get802_11TxPowerLevel(
	OUT NDIS_802_11_TX_POWER_LEVEL *pLevel)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11RSSI(
	OUT NDIS_802_11_RSSI *pRSSI)
{
	//Return if WMI is not ready yet
	if (!m_WMIReady)
		return NDIS_STATUS_ADAPTER_NOT_READY;
	
	//get stats from target
	if (ar6000_get_target_stats() != A_OK)
		return NDIS_STATUS_FAILURE;

	//Post the RSSI value relative to the Standard Noise floor value.
	*pRSSI = RSSI_TO_NDIS(m_RSSI);

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11RSSITrigger(
	OUT NDIS_802_11_RSSI *pTrigger)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

void
CAR6KMini::Get802_11InfrastructureMode(
	OUT NDIS_802_11_NETWORK_INFRASTRUCTURE *pMode)
{
	*pMode = m_InfrastructureMode;
}

NDIS_STATUS
CAR6KMini::Get802_11FragmentationThreshold(
	OUT NDIS_802_11_FRAGMENTATION_THRESHOLD *pThreshold)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11RTSThreshold(
	OUT NDIS_802_11_RTS_THRESHOLD *pThreshold)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11NumberOfAntennas(
	OUT ULONG *pNumberOfAntennas)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11RxAntennaSelected(
	OUT NDIS_802_11_ANTENNA *pAntenna)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

NDIS_STATUS
CAR6KMini::Get802_11TxAntennaSelected(
	OUT NDIS_802_11_ANTENNA *pAntenna)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

void
CAR6KMini::Get802_11SupportedRates(
	OUT BYTE *pRates)
//
//  When queried, the OID_802_11_SUPPORTED_RATES OID requests that
//  the miniport driver return the underlying NIC's data rate set, 
//  which includes the data rates that the NIC's radio supports.
//
//  Data rate sets are encoded as eight bytes. Each byte describes 
//  a single rate in units of 0.5 Mbps. Rates from the IEEE 802.11
//  BSSBasicRateSet that are included in the supported rates are used,
//  for example, for control and broadcast frames.
//
//  Each supported rate from the BSSBasicRateSet is encoded as a byte
//  with the most significant bit (bit 7) set to 1. For example, a 1Mbps
//  rate in the BSSBasicRateSet is encoded as 0x82.
//
//  Rates that are not included in the BSSBasicRateSet are encoded with
//  the most significant bit set to zero. For example, a 2Mbps rate
//  that does not belong to the BSSBasicRateSet is encoded as 0x04.
//
//  If the device supports less than eight rates, the driver must set
//  the unused entries at the end of the array to zero.
//
//  If the device supports more than eight rates, the miniport driver returns
//  the highest eight rates.
//
{
	memset(pRates, 0, sizeof(NDIS_802_11_RATES));

	pRates[0] = 0x80 | 22;   // 11 Mbps
	pRates[1] = 0x80 | 108;  // 54 Mbps
}

NDIS_STATUS
CAR6KMini::Get802_11DesiredRates(
	OUT NDIS_802_11_RATES *pRates)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

void
CAR6KMini::Get802_11Configuration(
	OUT NDIS_802_11_CONFIGURATION *pConfiguration)
{
	pConfiguration->Length = sizeof(NDIS_802_11_CONFIGURATION);

	// returns a value for BeaconPeriod based on the following:
	// o If the device is in infrastructure mode and is associated, the driver returns the current beacon period of the associated access point. 
	// o If the device is in ad hoc mode, the driver returns the IBSS beacon period. 
	// o If the device is not associated, the driver returns 0. 
	if (m_Connected) 
		pConfiguration->BeaconPeriod = m_beaconInterval;	
	else 
		pConfiguration->BeaconPeriod = 0;

	// The announcement traffic information message (ATIM) window in Kmicroseconds (1024 microseconds). The ATIM window is a short time period immediately after the transmission of each beacon in an ad hoc network. During the ATIM window, any station within the ad hoc network can indicate the need to transfer data to another station during the following data-transmission window.
	// If the driver's network mode is set for infrastructure networks, the driver returns 0 for ATIMWindow.
	pConfiguration->ATIMWindow = 0;

	// DSConfig contains the current radio frequency in kHz.
	pConfiguration->DSConfig = m_ConnectedChannel * 1000;

	pConfiguration->FHConfig.Length = sizeof(NDIS_802_11_CONFIGURATION_FH);
	pConfiguration->FHConfig.HopPattern = 0;
	pConfiguration->FHConfig.HopSet = 0;
	pConfiguration->FHConfig.DwellTime = 0;
}

NDIS_STATUS
CAR6KMini::Get802_11Statistics(
	OUT NDIS_802_11_STATISTICS *pStatistics)
{
	if (!m_WMIReady) {
		return NDIS_STATUS_ADAPTER_NOT_READY;
	}

	//get stats from target
	if (ar6000_get_target_stats() != A_OK)
		return NDIS_STATUS_FAILURE;

	*pStatistics = m_tgtStats;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11PowerMode(
	OUT NDIS_802_11_POWER_MODE *pMode)
{
	
	*pMode = m_80211_PowerMode;

	return NDIS_STATUS_SUCCESS;
}

void
CAR6KMini::WMIBSSIDInfoAdd(
	IN  bss_t *pWmiBss)
//
//  This function is called after Get802_11BSSIDList is called,
//  to add this node to the BSSID list being returned.
//
{
	NDIS_WLAN_BSSID_EX        *pBSSID = m_pBSSIDList;
	PBYTE                      pIE;
	ULONG                      spaceNeeded;
	NDIS_802_11_FIXED_IEs     *pFixed;
	NDIS_802_11_VARIABLE_IEs  *pVar;

	// Check to see that there is space available to add the node
	spaceNeeded = offsetof(NDIS_WLAN_BSSID_EX, IEs);

	spaceNeeded = spaceNeeded + sizeof(NDIS_802_11_FIXED_IEs);
	

#ifdef SUPPORT_WPA2	
	if (pWmiBss->ni_cie.ie_wpa2) {
		pIE = pWmiBss->ni_cie.ie_wpa2;
		spaceNeeded = spaceNeeded + pIE[1] + 2;
	}
#endif

	if (pWmiBss->ni_cie.ie_wpa) {
		pIE = pWmiBss->ni_cie.ie_wpa;
		spaceNeeded = spaceNeeded + pIE[1] + 2;
	}

	// spaceNeeded must be a multiple of 4 to maintain alignment.
	spaceNeeded = (spaceNeeded + 3) & ~3;

	if (*m_pcbBSSIDListNeeded + spaceNeeded < m_cbBSSIDList)
	{
		memset(pBSSID, 0, spaceNeeded);

		pBSSID->Length = spaceNeeded; // must be multiple of 4
		memcpy(pBSSID->MacAddress, pWmiBss->ni_macaddr, ETHERNET_MAC_ADDRESS_LENGTH);
		pBSSID->Ssid.SsidLength = 0;
		pIE = pWmiBss->ni_cie.ie_ssid;
		if (pIE)
		{
			// Format of SSID IE is:
			//  Type   (1 octet)
			//	Length (1 octet)
			//  SSID (Length octets)
			//
			//  Validation of the IE should have occurred within WMI.
			//
			ASSERT(pIE[0] == IEEE80211_ELEMID_SSID);
			ASSERT(pIE[1] <= 32);

			if (pIE[1] <= 32)
			{
				pBSSID->Ssid.SsidLength = pIE[1];
				memcpy(pBSSID->Ssid.Ssid, &pIE[2], pBSSID->Ssid.SsidLength);
			}
		}
		pBSSID->Privacy = (pWmiBss->ni_cie.ie_capInfo & 0x10) ? 1 : 0;
		//Post the RSSI value relative to the Standard Noise floor value.
		pBSSID->Rssi = RSSI_TO_NDIS(pWmiBss->ni_rssi);
		pBSSID->NetworkTypeInUse = Ndis802_11DS;

		pBSSID->Configuration.Length = sizeof(pBSSID->Configuration);
		pBSSID->Configuration.BeaconPeriod = pWmiBss->ni_cie.ie_beaconInt; // Units are Kmicroseconds (1024 us)
		pBSSID->Configuration.ATIMWindow =  0;
		pBSSID->Configuration.DSConfig =  pWmiBss->ni_cie.ie_chan * 1000;
		pBSSID->InfrastructureMode = ((pWmiBss->ni_cie.ie_capInfo & 0x03) == 0x01 ) ? Ndis802_11Infrastructure : Ndis802_11IBSS;

#if 0
		pIE = pWmiBss->ie_rates;
		if (pIE)
		{
			USHORT length;

			ASSERT(pIE[0] == IEEE80211_ELEMID_RATES);
			pBSSID->SupportedRates = ;
		}
#endif

		// Copy the fixed IEs
		pBSSID->IELength = sizeof(NDIS_802_11_FIXED_IEs);

		pFixed = (NDIS_802_11_FIXED_IEs *)pBSSID->IEs;
		memcpy(pFixed->Timestamp, pWmiBss->ni_cie.ie_tstamp, sizeof(pFixed->Timestamp));
		pFixed->BeaconInterval = pWmiBss->ni_cie.ie_beaconInt;
		pFixed->Capabilities = pWmiBss->ni_cie.ie_capInfo;

		// Copy selected variable IEs

		pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pFixed + sizeof(NDIS_802_11_FIXED_IEs));

#ifdef SUPPORT_WPA2		
		// Copy the WPAv2 IE
		if (pWmiBss->ni_cie.ie_wpa2) {
			pIE = pWmiBss->ni_cie.ie_wpa2;
			pBSSID->IELength += pIE[1] + 2; 
			memcpy(pVar, pIE, pIE[1] + 2);
			pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pVar + pIE[1] + 2);
		}
#endif

		// Copy the WPAv1 IE        
		if (pWmiBss->ni_cie.ie_wpa) {
			pIE = pWmiBss->ni_cie.ie_wpa;
			pBSSID->IELength += pIE[1] + 2; 
			memcpy(pVar, pIE, pIE[1] + 2);
			pVar = (NDIS_802_11_VARIABLE_IEs *)((PBYTE)pVar + pIE[1] + 2);
		}

		// Advance buffer pointer to next space beyond what we just added
		m_pBSSIDList = (NDIS_WLAN_BSSID_EX *)((PBYTE)pBSSID + pBSSID->Length);
		m_BSSIDListNumberOfItems++;
	}
	*m_pcbBSSIDListNeeded += spaceNeeded;
}

void
AR6KWMIIterateNodeCallback(
	IN  PVOID Context,
	IN  bss_t *pBssNode)
//
//  This function is called by WMI after we call wmi_iterate_nodes
//
{
	CAR6KMini *Adapter = (CAR6KMini *)Context;

	Adapter->WMIBSSIDInfoAdd(pBssNode);
}

void
CAR6KMini::Get802_11BSSIDList(
	OUT NDIS_802_11_BSSID_LIST_EX *pList, 
	IN  ULONG                      cbBuffer, 
	OUT ULONG                     *pcbNeeded)
//
//  Return a list containing all of the detected BSSIDs and their attributes
//
{
	pList->NumberOfItems = 0;
	//Return if WMI is not ready yet
	if (!m_WMIReady)
		return;
	
	*pcbNeeded = offsetof(NDIS_802_11_BSSID_LIST_EX, Bssid);

	m_pBSSIDList = &pList->Bssid[0];
	m_cbBSSIDList = cbBuffer;
	m_pcbBSSIDListNeeded = pcbNeeded;
	m_BSSIDListNumberOfItems = 0;

	Lock();

	wmi_iterate_nodes((wmi_t *)m_pWMI, AR6KWMIIterateNodeCallback, this);

	Unlock();

	m_pBSSIDList = NULL;
	m_cbBSSIDList = 0;
	m_pcbBSSIDListNeeded = NULL;
	pList->NumberOfItems = m_BSSIDListNumberOfItems;
}


void
CAR6KMini::Get802_11AuthenticationMode(
	OUT NDIS_802_11_AUTHENTICATION_MODE *pMode)
{
	*pMode = m_AuthenticationMode;
}

NDIS_STATUS
CAR6KMini::Get802_11PrivacyFilter(
	OUT NDIS_802_11_PRIVACY_FILTER *pFilter)
{
	return NDIS_STATUS_NOT_SUPPORTED;
}

void
CAR6KMini::Get802_11EncryptionStatus(
	OUT NDIS_802_11_ENCRYPTION_STATUS *pStatus)
{
	*pStatus = m_EncryptionStatus;
	switch (m_EncryptionStatus) {
	case Ndis802_11WEPEnabled:
		if (m_cbKey == 0) {
			*pStatus = Ndis802_11WEPKeyAbsent;
		}
		break;
	case Ndis802_11Encryption2Enabled:
		if (m_cbKey == 0) {
			*pStatus = Ndis802_11Encryption2KeyAbsent;
		}
		break;
	case Ndis802_11Encryption3Enabled:
		if (m_cbKey == 0) {
			*pStatus = Ndis802_11Encryption3KeyAbsent;
		}
		break;
	} //switch
	return;
}

/*
 * The target combines association request and response
 * into a single byte stream and notifies the same in 
 * connect_event. assocReqLen is the length in bytes of
 * association request and assocRespLen is the length in
 * bytes of association response. There is one glitch 
 * though. The association request that is passed by the
 * target contains only the Variable IEs and the fixed
 * IEs like listen Interval and AP address are passed
 * as arguments to the connect_event. However the association
 * response passed to the host contains the full 802.11
 * frame as seen on the wire. That is, in essence the request
 * and response parsing will not be symmetrical. 
 * One pending kludge to be fixed - WZC requires Capabilities
 * IE for both request and response as it is not returned 
 * in requests, it is hardcoded as 0x3104
 */
NDIS_STATUS
CAR6KMini::Get802_11AssociationInformation(
	OUT NDIS_802_11_ASSOCIATION_INFORMATION *pAssocInfo,
	IN  ULONG                                cbBuffer, 
	OUT ULONG                               *pcbNeeded)
{
	PBYTE  ptr;

	if (m_AssocReqLen == 0 && m_AssocRespLen == 0) {
		*pcbNeeded = 0;
		return NDIS_STATUS_SUCCESS;
	}
	*pcbNeeded = m_AssocReqLen + m_AssocRespLen + sizeof(*pAssocInfo);

	if (cbBuffer < *pcbNeeded) {
		return NDIS_STATUS_INVALID_LENGTH;
	}

	memcpy(pAssocInfo->RequestFixedIEs.CurrentAPAddress,
		m_PeerBSSID, ETHERNET_MAC_ADDRESS_LENGTH);

	pAssocInfo->AvailableRequestFixedIEs = 
		NDIS_802_11_AI_REQFI_CAPABILITIES | 
		NDIS_802_11_AI_REQFI_LISTENINTERVAL |
		NDIS_802_11_AI_REQFI_CURRENTAPADDRESS;

	pAssocInfo->OffsetRequestIEs = sizeof(*pAssocInfo);
	pAssocInfo->RequestIELength = m_AssocReqLen - 4;
	ptr = m_pAssocInfo + m_BeaconIeLen;
	pAssocInfo->RequestFixedIEs.Capabilities = ((*(ptr + 1)) << 8) | *ptr;
	ptr += 2;
	pAssocInfo->RequestFixedIEs.ListenInterval = ((*(ptr + 1)) << 8) | *ptr;
	ptr += 2;
	memcpy((PBYTE)pAssocInfo + pAssocInfo->OffsetRequestIEs, ptr, 
		pAssocInfo->RequestIELength);

	
	pAssocInfo->AvailableResponseFixedIEs = 
		NDIS_802_11_AI_RESFI_CAPABILITIES |
		NDIS_802_11_AI_RESFI_STATUSCODE   |
		NDIS_802_11_AI_RESFI_ASSOCIATIONID;

	pAssocInfo->OffsetResponseIEs = sizeof(*pAssocInfo) + 
						pAssocInfo->RequestIELength;
#ifndef AR6K_FIRMWARE_1_0
	pAssocInfo->ResponseIELength = m_AssocRespLen - 6;
	ptr = m_pAssocInfo + m_BeaconIeLen + m_AssocReqLen;
#else
	pAssocInfo->ResponseIELength = m_AssocRespLen - 
					(sizeof(struct ieee80211_frame) + 6);
	ptr = m_pAssocInfo + m_AssocReqLen + sizeof(struct ieee80211_frame);
#endif
	pAssocInfo->ResponseFixedIEs.Capabilities = ((*(ptr + 1)) << 8) | *ptr;
	ptr += 2;
	pAssocInfo->ResponseFixedIEs.StatusCode = ((*(ptr + 1)) << 8) | *ptr;
	ptr += 2;
	pAssocInfo->ResponseFixedIEs.AssociationId = ((*(ptr + 1)) << 8) | *ptr;
	ptr += 2;
	memcpy((PBYTE)pAssocInfo + pAssocInfo->OffsetResponseIEs, ptr, 
		pAssocInfo->ResponseIELength);

	pAssocInfo->Length = sizeof(*pAssocInfo);

	return NDIS_STATUS_SUCCESS;
}

#ifdef SUPPORT_WPA2
NDIS_STATUS
CAR6KMini::Get802_11PMKID(OUT NDIS_802_11_PMKID *pPMKID, 
						  IN  ULONG              cbBuffer, 
						  OUT ULONG             *pcbNeeded)
{
	unsigned int i = 0;

	if (m_AuthenticationMode != Ndis802_11AuthModeWPA2) {
		return NDIS_STATUS_INVALID_DATA;
	}

	if (m_pPMKID == NULL) {
		return NDIS_STATUS_INVALID_DATA;
	}

    *pcbNeeded = m_pPMKID->Length;

	if (cbBuffer < m_pPMKID->Length) {
		return NDIS_STATUS_INVALID_LENGTH;
	}

    pPMKID->Length = m_pPMKID->Length;
	pPMKID->BSSIDInfoCount = m_pPMKID->BSSIDInfoCount;

	for (i=0; i<pPMKID->BSSIDInfoCount; i++) {
		memcpy(pPMKID->BSSIDInfo[i].BSSID,
			m_pPMKID->BSSIDInfo[i].BSSID, 6);
		memcpy(pPMKID->BSSIDInfo[i].PMKID,
			m_pPMKID->BSSIDInfo[i].PMKID, 16);
	}

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Get802_11Capability(OUT NDIS_802_11_CAPABILITY *pCap, 
						   IN ULONG                   cbBuffer, 
						   OUT ULONG                  *pcbNeeded)
{
    ULONG i = 0;
    ULONG lenNeeded;

	lenNeeded = sizeof(pCap->Version) + sizeof(pCap->NoOfPMKIDs) +
                sizeof(pCap->NoOfAuthEncryptPairsSupported) + sizeof(pCap->Length) +
		        (10 * sizeof(NDIS_802_11_AUTHENTICATION_ENCRYPTION));

    *pcbNeeded = lenNeeded;

    if (cbBuffer < lenNeeded) {
	    return NDIS_STATUS_INVALID_LENGTH;
    }

	pCap->Version = 2;
	pCap->NoOfPMKIDs = 3; //no of PMKIDs that can be cached, min of 3 and max of 16.
	pCap->NoOfAuthEncryptPairsSupported = 10; //excluding WEP shared and WPA-None
	pCap->Length = lenNeeded; 
	for (i = 0; i < 10; i++) {
		pCap->AuthenticationEncryptionSupported[i].AuthModeSupported = 
			g_WPA2AuthEncryptPairSupported[i].AuthModeSupported;
		pCap->AuthenticationEncryptionSupported[i].EncryptStatusSupported = 
			g_WPA2AuthEncryptPairSupported[i].EncryptStatusSupported;
	}

	return NDIS_STATUS_SUCCESS;
}
#endif //SUPPORT_WPA2

// AR6K 802.11 OID Set Handlers

NDIS_STATUS
CAR6KMini::Set802_11AssociatedAPBSSID(
	IN NDIS_802_11_MAC_ADDRESS *BSSID)
{
	memcpy(m_DesiredAPBSSID, BSSID, sizeof(NDIS_802_11_MAC_ADDRESS));

	// Setting the desired BSSID results in the following actions:
	//  o If the device is associated with an AP that has the same BSSID, the device must reassociate with the AP
	//  o If the device is associated with a different AP, then it must dissasociate from it then associate with the desired AP.
	//  o If the device is not associated, then it must attempt to associate to the specified BSSID
	//
	// The desired BSSID is cleared by setting it to MAC address 0xFFFFFFFFFFFF. When set to that broadcast address,
	// the device may associate with any AP that matches the desired SSID.
	//
	return NDIS_STATUS_SUCCESS;
}

bool
IsBroadcastAddress(
	IN PBYTE pMACAddress)
//
//  Return true if the MAC address is all 1's (i.e. FF FF FF FF FF FF)
//
{
	bool IsBroadcast = true;

	for (UINT i = 0; i < sizeof(NDIS_802_11_MAC_ADDRESS); i++)
	{
		if (pMACAddress[i] != 0xFF)
		{
			IsBroadcast = false;
			break;
		}
	}

	return IsBroadcast;
}

NDIS_STATUS
CAR6KMini::SendWMIConnectCommand(
	IN NDIS_802_11_SSID *SSID)
//
//  Request that the Wireless Module associate with the specified SSID.
//
{
	NDIS_STATUS  Status = NDIS_STATUS_SUCCESS;
	A_STATUS     wmiStatus;
	NETWORK_TYPE wmiNetworkType = m_InfrastructureMode == Ndis802_11Infrastructure ? INFRA_NETWORK : ADHOC_NETWORK;
	DOT11_AUTH_MODE wmiDot11AuthMode = OPEN_AUTH;
	AUTH_MODE    wmiAuthMode = NONE_AUTH;
	BYTE        *pAPBSSID;
	CRYPTO_TYPE keyType = NONE_CRYPT;
	BYTE KeyIndex = 0;
        A_UINT16 channel = (m_InfrastructureMode == Ndis802_11IBSS) ? (A_UINT16)(m_Config.ibssChannel/1000) : 0;

	//
	// CONNECT commands can only be sent in the DISCONNECTED state.
	//
	ASSERT(false == m_Connected);
	ASSERT(false == m_ConnectInProgress);


	switch(m_AuthenticationMode)
	{
	case Ndis802_11AuthModeOpen:
		wmiDot11AuthMode = OPEN_AUTH;
		break;

	case Ndis802_11AuthModeShared:
		wmiDot11AuthMode = SHARED_AUTH;
		break;

	case Ndis802_11AuthModeWPA:
		wmiAuthMode = WPA_AUTH;
		break;

	case Ndis802_11AuthModeWPAPSK:
		wmiAuthMode = WPA_PSK_AUTH;
		break;

	case Ndis802_11AuthModeWPA2PSK:
		wmiAuthMode = WPA2_PSK_AUTH;
		break;
		
	case Ndis802_11AuthModeWPA2:
		wmiAuthMode = WPA2_AUTH;
		break;

	default:
		break;
	}
	if (m_AuthAlg == 0x04) { 
		wmiDot11AuthMode = LEAP_AUTH;
	}


	switch (m_EncryptionStatus)
	{
	case Ndis802_11WEPEnabled:
		m_PairwiseCryptoType = WEP_CRYPT;
		m_GrpwiseCryptoType = WEP_CRYPT;
		break;

	case Ndis802_11WEPDisabled:
		m_PairwiseCryptoType = NONE_CRYPT;
		m_GrpwiseCryptoType = NONE_CRYPT;
		break;

	case Ndis802_11Encryption2Enabled:
		m_PairwiseCryptoType = TKIP_CRYPT;
		m_GrpwiseCryptoType = Encryption2GrpwiseCryptoTable[m_GrpwiseCryptoIndex];
		m_GrpwiseCryptoIndex = (m_GrpwiseCryptoIndex++) % ENCRYPT2_GRPWISE_CRYPTOINDEX_MAX;

		break;

	case Ndis802_11Encryption3Enabled:
		m_PairwiseCryptoType = AES_CRYPT;
		m_GrpwiseCryptoType = Encryption3GrpwiseCryptoTable[m_GrpwiseCryptoIndex];
		m_GrpwiseCryptoIndex = (m_GrpwiseCryptoIndex++) % ENCRYPT3_GRPWISE_CRYPTOINDEX_MAX;
				
		break;

	default:
		break;
	}

	pAPBSSID = NULL;
	if (!IsBroadcastAddress(m_DesiredAPBSSID))
		pAPBSSID = m_DesiredAPBSSID;

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Connect command from Supplicant\n");


	if (m_WMIBssFilter !=  ALL_BSS_FILTER) { 
	    wmiStatus = wmi_bssfilter_cmd((struct wmi_t *)m_pWMI, ALL_BSS_FILTER);
	    if (wmiStatus == A_OK) {
		    m_WMIBssFilter = ALL_BSS_FILTER;
	    } else {
		    return NDIS_STATUS_FAILURE;
		}
	}

	//Add the keys to the NIC, before issuing the connect cmd.
	//Add the keys here only if the Auth mode is either SharedAuth or AutoSwitch. Only for
	//Shared Auth the WEP keys are plumbed before the connection.

	if (m_AuthenticationMode == Ndis802_11AuthModeShared ||
		m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch ) {

		for (KeyIndex = 0; KeyIndex < 4 ; KeyIndex++ ) {

			if ( m_cbKey[KeyIndex] != 0 ) {

				if( (m_KeyUsage[KeyIndex] & GROUP_USAGE) == GROUP_USAGE )
				{
					keyType = m_GrpwiseCryptoType;
				} else {

					keyType = m_PairwiseCryptoType;
				}

				wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI,
				       (A_UINT8)KeyIndex,
				       WEP_CRYPT, // Only for WEP the keys are added before connect
				       m_KeyUsage[KeyIndex],
				       (A_UINT8)m_cbKey[KeyIndex],
				       (A_UINT8*)&m_KeyRSC[KeyIndex],
				       m_Key[KeyIndex]);

				if (wmiStatus != A_OK) {
				    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
					return NDIS_STATUS_FAILURE;
				} 
			}
		}
	}

	/* Set the Listen Interval to 1000TUs. This value will be indicated to the
	 * AP in the conn. request. Later set it back locally at the STA to 100/1000 TUs depending on the
	 * power mode.
	 */
	//Increase the listen interval to 1000 TUs to ensure max buffering capacity at the AP.
	
	wmiStatus = wmi_listeninterval_cmd((struct wmi_t *)m_pWMI, 
			MAX_LISTEN_INTERVAL, 0 );

	if (wmiStatus != A_OK) {
	    NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_listeninterval_cmd failed = %u\n", wmiStatus);
		return NDIS_STATUS_FAILURE;
	}
	
	wmiStatus = wmi_connect_cmd((wmi_t *)m_pWMI, wmiNetworkType, 
				wmiDot11AuthMode, wmiAuthMode, 
				m_PairwiseCryptoType, 0, 
				m_GrpwiseCryptoType, 0,
				m_SSID.SsidLength, m_SSID.Ssid, 
				pAPBSSID, channel);
	
	if (wmiStatus != A_OK) {
	    NDIS_DEBUG_PRINTF(ATH_LOG_ERR,"AR6K: ERROR - wmi_connect_cmd failed = %u\n", wmiStatus);
		Status = NDIS_STATUS_FAILURE;
    } else {
		m_ConnectInProgress = true;
	}

	return Status;
}

NDIS_STATUS
CAR6KMini::SendWMIDisconnectCommand()
//
//  Request that the Wireless Module disassociate from the current wireless
//  network. This request may be issued in any state. It will cancel an ongoing
//  CONNECT command if one is in progress. A DISCONNECTED_EVENT will always be
//  generated, even if the current state is DISCONNECTED.
//
{
	NDIS_STATUS  Status = NDIS_STATUS_SUCCESS;
	A_STATUS     wmiStatus;

	wmiStatus = wmi_disconnect_cmd((wmi_t *)m_pWMI);
	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "Disconnect from supplicant\n");

	if (wmiStatus != A_OK) {
	    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_disconnect_cmd failed = %u\n", wmiStatus);
		Status = NDIS_STATUS_FAILURE;
    }

	return Status;
}


NDIS_STATUS
CAR6KMini::Set802_11SSID(
	IN NDIS_802_11_SSID *SSID)
//
//  This request informs the driver of the SSID to associate to. If the first
//  character of the SSID string is zero, then it can associate with any SSID.
//  If the SSID is invalid (contains non printable ASCII characters), then the
//  adapter should not associate with any SSID.
//
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	A_STATUS wmiStatus;
	NDIS_802_11_SSID *lSSID; 
		
	if (!m_WMIReady) {
		Status = NDIS_STATUS_ADAPTER_NOT_READY;
		goto done;
	}

	if (m_RadioDisabled) {
		m_RadioDisabled = FALSE;
		configRegistryParameters();
		configTargetParams();
		wmiStatus = wmi_startscan_cmd((wmi_t *)m_pWMI,WMI_LONG_SCAN);
		if (wmiStatus != A_OK)
			NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_startscan_cmd failed = %u\n", wmiStatus);
	}

	if (SSID->SsidLength > 32)
	{
		// Bad length
		Status = NDIS_STATUS_INVALID_DATA;
		goto done;
	}

	// SSID may be NULL to connect to any available SSID
	if (SSID->SsidLength == 0) {
		ULONG dwSize = 2048;
		ULONG NoOfItems,ssidLength;
		ULONG *pcbNeeded=(ULONG *)malloc(sizeof(ULONG));
		NDIS_802_11_BSSID_LIST_EX *pBSSIDListEx = (NDIS_802_11_BSSID_LIST_EX *)malloc(dwSize);
		this->Get802_11BSSIDList(pBSSIDListEx,dwSize,pcbNeeded);
		if(pBSSIDListEx) {
			NoOfItems = pBSSIDListEx->NumberOfItems;
			NDIS_WLAN_BSSID_EX *pBSSID = (NDIS_WLAN_BSSID_EX *) &pBSSIDListEx->Bssid[0];
			while (NoOfItems) {
				if (pBSSID->Privacy) {
					pBSSID = (NDIS_WLAN_BSSID_EX *)((PBYTE)pBSSID + pBSSID->Length);
					NoOfItems--;
				} else {
					ssidLength=pBSSID->Ssid.SsidLength;
					if(ssidLength) {
						lSSID=&pBSSID->Ssid;
						m_AuthenticationMode=Ndis802_11AuthModeOpen;
						m_EncryptionStatus=Ndis802_11WEPDisabled;
						break;	
					} else {
						pBSSID = (NDIS_WLAN_BSSID_EX *)((PBYTE)pBSSID + pBSSID->Length);
						NoOfItems--;
					}
				} 
			}
		free(pBSSIDListEx);
		}
	} 
	else {
		lSSID=SSID;
	}

	m_WantToBeConnected = SSIDIsValid(SSID);
	if (false == m_WantToBeConnected)
	{
		m_SSID = *lSSID;

		// We want to abort any connection attempt in progress, and
		// dissassociate from any SSID to which we are current associated.
		if (m_ConnectInProgress || m_Connected)
		{
			Status = SendWMIDisconnectCommand();
		}
	}
	else
	{
		// The set SSID maybe same as the one we are already connected or different.
		// In any case we issue a disconnect if we are already connected. This is because
		// there may be a change in the Conn. profile (such as encryption key) that warrants
		// a disconnect & reconnect.

		// We want to associate to the specified SSID
		if (lSSID->SsidLength == m_SSID.SsidLength
		&&  0 == memcmp(lSSID->Ssid, m_SSID.Ssid, m_SSID.SsidLength))
		{
			//TODO: New SSID is the same as the old. No need to indicate MEDIA_DISCONNECT
			// to NDIS immediately. If the subsequent connection attempt fails then we can 
			// indicate MEDIA_DISCONNECT to NDIS.
		}
		
		m_SSID = *lSSID;
		// Initialize the GrpwiseCrypto index to 0 to start afresh.
		m_GrpwiseCryptoIndex = 0;

		if (m_ConnectInProgress || m_Connected)
		{
			Status = SendWMIDisconnectCommand();
		}
		else
		{
			// No current connection, 
			Status = SendWMIConnectCommand(SSID);
		}
	}

done:
 	return Status;
}

NDIS_STATUS
CAR6KMini::Set802_11NetworkTypeInUse(
	IN NDIS_802_11_NETWORK_TYPE Type)
{
	if (Type == Ndis802_11DS || Type == Ndis802_11OFDM24) {
		m_NetworkTypeInUse = Type;
		return NDIS_STATUS_SUCCESS;
	}
	else 
		return NDIS_STATUS_INVALID_DATA;
}

NDIS_STATUS
CAR6KMini::Set802_11TxPowerLevel(
	IN NDIS_802_11_TX_POWER_LEVEL Level)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RSSITrigger(
	IN NDIS_802_11_RSSI Trigger)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11InfrastructureMode(
	IN NDIS_802_11_NETWORK_INFRASTRUCTURE Mode)
{
	int i;
	A_STATUS wmiStatus;
	BYTE KeyIndex;

	if (!m_WMIReady) {
	    return NDIS_STATUS_ADAPTER_NOT_READY;
	}
    
	m_InfrastructureMode = Mode;

	// Clear all the Keys as per OID Recommendation
	for (i=0;i<4;i++) {
		if (m_Key[i]) {
			KeyIndex=(BYTE)(m_KeyIndex[i]);
			memset(m_Key[i],0,sizeof(m_Key[i]));
			m_cbKey[i] = 0;
			m_KeyIndex[i] = 0;
			m_KeyRSC[i] = 0;

			wmiStatus = wmi_deleteKey_cmd((wmi_t *)m_pWMI, KeyIndex);
			if (wmiStatus != A_OK) {
				return NDIS_STATUS_FAILURE;
			}
		}
	}

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11FragmentationThreshold(
	IN NDIS_802_11_FRAGMENTATION_THRESHOLD Threshold)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RTSThreshold(
	IN NDIS_802_11_RTS_THRESHOLD Threshold)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RxAntennaSelected(
	IN ULONG Antenna)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11TxAntennaSelected(
	IN ULONG Antenna)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11DesiredRates(
	IN NDIS_802_11_RATES *Rates)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11Configuration(
	IN NDIS_802_11_CONFIGURATION *pConfiguration)
//
//  Set the underlying NIC's radio configuration parameters to the specified values
//
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

	// This OID can only be set when the device is not associated
	// within the desired SSID. The miniport driver must return
	// NDIS_STATUS_NOT_ACCEPTED if it is currently associated.
	if (m_Connected)
		Status = NDIS_STATUS_NOT_ACCEPTED;
	else
	{
                if ( m_InfrastructureMode == Ndis802_11IBSS ) {
                        m_Config.ibssChannel = pConfiguration->DSConfig; 
                }
		// BeaconPeriod specifies the beacon interval for ad hoc mode. 
		// BeaconPeriod is ignored in infrastructure mode
		// wmi_set_adhoc_beacon_interval(BeaconPeriod);
	}

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11Disassociate()
//
//  Disassociate from the current service set and turn off the radio. 
//
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	A_STATUS wmiStatus;
	m_WantToBeConnected = false;
	m_SSID.SsidLength = 0;

	// Only need to do something if we are connected
	if (m_Connected)
	{
		if (!m_WMIReady) {
			// Hardware is not in a state where we can send it commands
			Status = NDIS_STATUS_ADAPTER_NOT_READY;
		}
		else {
			Status = SendWMIDisconnectCommand();
		}
	}
	wmiStatus =	wmi_scanparams_cmd((wmi_t *)m_pWMI,65535,0,0,0,0,0);
	if ( wmiStatus != A_OK ) {
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_scanparams_cmd failed = %u\n", wmiStatus);
	}
	wmi_free_allnodes((wmi_t *)m_pWMI);
	
	if (m_Connected && Status==NDIS_STATUS_SUCCESS) {
		m_Connected = false;	
		NdisMIndicateStatus(m_MiniportAdapterHandle, NDIS_STATUS_MEDIA_DISCONNECT, 0, 0);
	    NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
	}
	m_RadioDisabled = TRUE;
	return Status;
}

NDIS_STATUS
CAR6KMini::Set802_11PowerMode(
	IN NDIS_802_11_POWER_MODE PowerMode)
{
	A_STATUS Status = A_OK;
	A_UINT8 powerMode = 0;
	A_UINT16 listenInterval = MIN_LISTEN_INTERVAL;

	// return if PowerMode @ target is already the same as what we get here.
	if (PowerMode == m_80211_PowerMode)
		return NDIS_STATUS_SUCCESS;

	switch( PowerMode ) {

	case Ndis802_11PowerModeCAM:

		powerMode = MAX_PERF_POWER;

		break;
	case Ndis802_11PowerModeMAX_PSP:

		//Increase the listen interval to 1000 TUs ensure MAX_PSP.
		listenInterval = MAX_LISTEN_INTERVAL;

		powerMode = REC_POWER;

		break;
	case Ndis802_11PowerModeFast_PSP:

		//Set to default listen interval of 100 TUs.
		listenInterval = MIN_LISTEN_INTERVAL;

		powerMode = REC_POWER;

		break;
	default:
		return NDIS_STATUS_INVALID_DATA;
	}

	//Set the Listen interval if Power mode is other than MAX_PERF_POWER
	if ( powerMode != MAX_PERF_POWER ) {

		Status = wmi_listeninterval_cmd((struct wmi_t *)m_pWMI, listenInterval, 0);
		if (Status != A_OK)
			return NDIS_STATUS_FAILURE;
	}

    Status = wmi_powermode_cmd((struct wmi_t *)m_pWMI, powerMode);

	if ( Status != A_OK ) {
		
		//if the Listen interval was updated, roll back to original.
		if ( powerMode != MAX_PERF_POWER )
			wmi_listeninterval_cmd((struct wmi_t *)m_pWMI, m_ListenInterval, 0);

		return NDIS_STATUS_FAILURE;
	}

	//Update the global power mode & listen Interval vars with the set value;
	m_80211_PowerMode = PowerMode;
	
	if ( powerMode != MAX_PERF_POWER )
		m_ListenInterval = listenInterval;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11BSSIDListScan()
//
//  Perform a network scan of BSSIDs and SSIDs.
//
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	A_STATUS    wmiStatus;

	if (!m_WMIReady)
	{
		Status = NDIS_STATUS_ADAPTER_NOT_READY;
		goto done;
	}

	if (m_ConnectInProgress || m_Connected)
	{
		goto done;
	}
   
    if (m_WMIBssFilter !=  ALL_BSS_FILTER) { 
	    wmiStatus = wmi_bssfilter_cmd((struct wmi_t *)m_pWMI, ALL_BSS_FILTER);
	    if (wmiStatus == A_OK) {
		    m_WMIBssFilter = ALL_BSS_FILTER;
        } else {
		    Status = NDIS_STATUS_FAILURE;
		    goto done;
	    }
    }

	wmiStatus = wmi_startscan_cmd((struct wmi_t *)m_pWMI, WMI_LONG_SCAN);
	if (A_OK != wmiStatus)
		Status = NDIS_STATUS_FAILURE;

done:
	return Status;
}

NDIS_STATUS
CAR6KMini::Set802_11PrivacyFilter(
	IN NDIS_802_11_PRIVACY_FILTER Filter)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11ReloadDefaults(
	IN NDIS_802_11_RELOAD_DEFAULTS Filter)
{
	return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::SetCAr6k802_11AuthAlg(
    IN ULONG AuthAlg)
{
	m_AuthAlg = AuthAlg;
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetResumeWlanState(ULONG state)
{
	m_ResumeWlanState = state;
	return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::Set802_11AuthenticationMode(
	IN NDIS_802_11_AUTHENTICATION_MODE Mode)
{
	if ( m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch ) {
		return NDIS_STATUS_NOT_SUPPORTED;
	}
	if ((Mode == Ndis802_11AuthModeOpen) || (Mode == Ndis802_11AuthModeShared) ||
		(Mode == Ndis802_11AuthModeWPA) || (Mode == Ndis802_11AuthModeWPAPSK) || 
		(Mode == Ndis802_11AuthModeWPA2PSK) || (Mode == Ndis802_11AuthModeWPA2)) {
			m_AuthenticationMode = Mode;
			return NDIS_STATUS_SUCCESS;
	} else {
		return NDIS_STATUS_INVALID_DATA;
	}

}

NDIS_STATUS
CAR6KMini::Set802_11EncryptionStatus(
	IN NDIS_802_11_ENCRYPTION_STATUS EncryptionStatus)
{
	m_EncryptionStatus = EncryptionStatus;
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11AddWEP(
	IN NDIS_802_11_WEP *pWEP)
{
	BYTE         KeyIndex;
	A_UINT8      KeyUsage = 0;
	A_STATUS     wmiStatus;

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: AddWEP from Supplicant\n");
	if (pWEP->KeyIndex & 0x80000000) {
		KeyUsage |= TX_USAGE;
	}

	if (!(pWEP->KeyIndex & 0x40000000)) {
		KeyUsage |= GROUP_USAGE;
	}

	KeyIndex = (BYTE)(pWEP->KeyIndex & 0xff);

	if (KeyUsage & TX_USAGE && !(KeyUsage & GROUP_USAGE)) {
		//pairwise key and index is not 0
		if (KeyIndex != 0) {
			return NDIS_STATUS_INVALID_DATA;
		}
	}

	if (KeyIndex < WMI_MIN_KEY_INDEX || 
		KeyIndex > WMI_MAX_KEY_INDEX) {
		return NDIS_STATUS_INVALID_DATA;
	}

	if (pWEP->KeyLength <= sizeof(m_Key[KeyIndex])) {
		m_cbKey[KeyIndex] = pWEP->KeyLength;
		memcpy(m_Key[KeyIndex], pWEP->KeyMaterial, pWEP->KeyLength);
		m_KeyIndex[KeyIndex] = KeyIndex;
		m_KeyRSC[KeyIndex] = 0;
		m_KeyUsage[KeyIndex] = KeyUsage;
	}

	// Add the key to the target
	if (m_EncryptionStatus == Ndis802_11WEPEnabled) {
		wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI, 
								(A_UINT8)KeyIndex,
								WEP_CRYPT,
								m_KeyUsage[KeyIndex], 
								(A_UINT8)m_cbKey[KeyIndex], 
								(A_UINT8*)&m_KeyRSC[KeyIndex], 
								m_Key[KeyIndex]);
		if (wmiStatus != A_OK) {
		    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
			return NDIS_STATUS_FAILURE;
		}
	}

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RemoveWEP(
	IN NDIS_802_11_KEY_INDEX KeyIndex)
{
	A_STATUS wmiStatus;

	NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: RemoveWEP from Supplicant\n");

	if (KeyIndex >= 4) {
	    return NDIS_STATUS_FAILURE;
	}

	memset(m_Key[KeyIndex], 0, sizeof(m_Key[KeyIndex]));
	m_cbKey[KeyIndex] = 0;

	wmiStatus = wmi_deleteKey_cmd((wmi_t *)m_pWMI, (BYTE)KeyIndex);
	if (wmiStatus != A_OK) {
	    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_deleteKey_cmd failed = %u\n", wmiStatus);
		return NDIS_STATUS_FAILURE;
	}
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11AddKey(
	IN NDIS_802_11_KEY *pKey)
{
	BYTE         KeyIndex, tempKeyVal, tempKeyMaterial[32];
	A_UINT8      KeyUsage = 0;
	A_STATUS     wmiStatus;
	int          i = 0;
	CRYPTO_TYPE	 keyType = NONE_CRYPT;

	if (pKey->KeyIndex & 0x80000000) {
		KeyUsage |= TX_USAGE;
	}

	if (!(pKey->KeyIndex & 0x40000000)) {
		KeyUsage |= GROUP_USAGE;
	}

	KeyIndex = (BYTE)(pKey->KeyIndex & 0xff);
	if (KeyUsage & TX_USAGE && !(KeyUsage & GROUP_USAGE)) {
		//pairwise key and index is not 0
		if (KeyIndex != 0) {
			return NDIS_STATUS_INVALID_DATA;
		}
	}

	if (KeyIndex < WMI_MIN_KEY_INDEX || 
		KeyIndex > WMI_MAX_KEY_INDEX) {
		return NDIS_STATUS_INVALID_DATA;
	}

	if( (KeyUsage & GROUP_USAGE) == GROUP_USAGE )
	{
		keyType = m_GrpwiseCryptoType;
	} else {

		keyType = m_PairwiseCryptoType;
	}

	/* Set the keyType to WEP_CRYPT for the case of this OID being called because of a profile
	 * switch to 802.1x/SharedAuth/AutoSwitch from a connected profile. This is because the encryption status
	 * OID is got only after the AddKey OID.
     */
	if ((m_AuthenticationMode == Ndis802_11AuthModeShared ||
		m_AuthenticationMode == Ndis802_11AuthModeAutoSwitch || m_AuthenticationMode == Ndis802_11AuthModeOpen)) {

		keyType = WEP_CRYPT;
	}

	// TODO - to check BSSID in pKey and the AP address for pairwise key

	memcpy(tempKeyMaterial, pKey->KeyMaterial, pKey->KeyLength);
	if (keyType == TKIP_CRYPT) {

	// if its TKIP and bit 28 is set swap tx and rx mic
		if (!(pKey->KeyIndex & 0x10000000)) {
			for (i=0;i<8; i++) {
				tempKeyVal = tempKeyMaterial[16+i]; //rx
				tempKeyMaterial[16+i] = 
					tempKeyMaterial[24+i]; //rx = tx
				tempKeyMaterial[24+i] = tempKeyVal; //tx = rx
			}
		}
	}

	if (pKey->KeyLength <= sizeof(m_Key[KeyIndex]))
	{
		m_cbKey[KeyIndex] = pKey->KeyLength;
		memcpy(m_Key[KeyIndex], tempKeyMaterial, pKey->KeyLength);
		m_KeyIndex[KeyIndex] = KeyIndex;
		m_KeyRSC[KeyIndex] = pKey->KeyRSC;
		m_KeyUsage[KeyIndex] = KeyUsage;
	}

	// Add the key to the target only if Encryption is enabled.
	if (NONE_CRYPT != m_PairwiseCryptoType) {
 
		wmiStatus = wmi_addKey_cmd((wmi_t *)m_pWMI,
			       (A_UINT8)KeyIndex,
			       keyType,
			       m_KeyUsage[KeyIndex],
			       (A_UINT8)m_cbKey[KeyIndex],
			       (A_UINT8*)&m_KeyRSC[KeyIndex],
			       m_Key[KeyIndex]);

		
		if (wmiStatus != A_OK) {
		    NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_addKey_cmd failed = %u\n", wmiStatus);
			return NDIS_STATUS_FAILURE;
		}
	}
 
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11RemoveKey(
	IN NDIS_802_11_REMOVE_KEY *pKey)
{
	A_STATUS wmiStatus;
	BYTE     KeyIndex;

	KeyIndex = (BYTE)(pKey->KeyIndex & 0xff);

	memset(m_Key[KeyIndex], 0, sizeof(m_Key[KeyIndex]));
	m_cbKey[KeyIndex] = 0;

	wmiStatus = wmi_deleteKey_cmd((wmi_t *)m_pWMI, KeyIndex);
	if (wmiStatus != A_OK) {
        NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - wmi_deleteKey_cmd failed = %u\n", wmiStatus);
		return NDIS_STATUS_FAILURE;
	}

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11AssociationInformation(
	IN NDIS_802_11_ASSOCIATION_INFORMATION *pInfo,
	IN ULONG                                cbInfo)
{
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::Set802_11Test(
	IN NDIS_802_11_TEST *pTest,
	IN ULONG cbBuffer)
{
	switch(pTest->Type) {
		case 1:
			NdisMIndicateStatus(m_MiniportAdapterHandle,
                        NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
						&pTest->AuthenticationEvent, cbBuffer - 2*sizeof(ULONG));
			NdisMIndicateStatusComplete(m_MiniportAdapterHandle);
			break;
		case 2:
			break;
		default:
			return NDIS_STATUS_INVALID_DATA;
	}
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetPnpPower(ULONG *state)
{
	if (m_PowerState == NdisDeviceStateD3) {
        *state = 0; // OFF State
    } else {
        *state = 1; // ON State
    }
	return NDIS_STATUS_SUCCESS;
}


NDIS_STATUS
CAR6KMini::GetResumeWlanState(ULONG *state)
{
	*state = m_ResumeWlanState;
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::GetFirmwareVersion(ULONG *version)
{
	*version = m_TargetId;
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiDone(
	PVOID Buffer)
{
	A_STATUS status;

	// Call StartEndpoints to get the target out of the BMI phase.
	status = StartEndpoints();

	if ( status != A_OK)
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiWriteMemory(
	BMI_WRITE_MEMORY_PARAM *Buffer)
{
	A_STATUS status;

	status = BMIWriteMemory(m_pHIFDevice, Buffer->address, (A_UCHAR *)(&(Buffer->buffer)), Buffer->length);
	if (status != A_OK )
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiReadMemory(
	BMI_READ_MEMORY_PARAM *Buffer)
{
	A_STATUS status;

	status = BMIReadMemory(m_pHIFDevice, Buffer->address, (A_UCHAR *)(&(Buffer->buffer)), Buffer->length);
	if ( status != A_OK)
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiExecute(
	BMI_EXECUTE_PARAM *Buffer)
{
	A_STATUS status;

	status = BMIExecute(m_pHIFDevice, Buffer->address, (A_UINT32 *)&Buffer->param);
	if ( status != A_OK)
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiSetAppStart(
	BMI_SET_APP_START_PARAM *Buffer)
{
	A_STATUS status;

	status = BMISetAppStart(m_pHIFDevice, Buffer->address);
	if ( status != A_OK)
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiWriteSocRegister(
	BMI_WRITE_SOC_REG_PARAM *Buffer)
{
	A_STATUS status;
	
	status = BMIWriteSOCRegister(m_pHIFDevice, Buffer->address, Buffer->param);

	if ( status != A_OK)
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kBmiReadSocRegister(
	BMI_READ_SOC_REG_PARAM *Buffer)
{
	A_STATUS status;

	status = BMIReadSOCRegister(m_pHIFDevice, Buffer->address, (A_UINT32 *)&Buffer->value);

	if ( status != A_OK)
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

#ifdef HTC_RAW_INTERFACE

NDIS_STATUS 
CAR6KMini::SetCAr6kHtcRawOpen(
	void)
{
	A_STATUS status = A_OK;

	if (!rawIfInit) {

#ifdef FLASH_18V
		// Change the flash access time for 1.8V flash to 150ns
		status = BMIWriteSOCRegister(m_pHIFDevice, 
									0xac004004,
									0x920100d1);
#endif
		if ( status == A_OK ) {
			status = BMIDone(m_pHIFDevice);
        }

		if ( status == A_OK ) {
			status = (A_STATUS) ar6000_htc_raw_open(m_pHTCTarget);
		}

		if (status == A_OK) {
			rawIfInit = true;
		} else {
			return NDIS_STATUS_FAILURE;
		}
	}
		
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kHtcRawClose(
	void)
{
	A_STATUS status;

	if (rawIfInit) {
		status = (A_STATUS) ar6000_htc_raw_close(m_pHTCTarget);
		rawIfInit = false;
	} else {
		return NDIS_STATUS_FAILURE;
	}

	if (status != A_OK)
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}		

NDIS_STATUS
CAR6KMini::SetCAr6kHtcRawRead(
	HTC_RAW_READ_PARAM *Buffer)
{
	A_STATUS status;
	unsigned int bufLen=0;

	if (rawIfInit) {
		if ((bufLen = (unsigned int) ar6000_htc_raw_read(m_pHTCTarget, (HTC_ENDPOINT_ID)Buffer->endPointId, \
			(&(Buffer->buffer)), Buffer->length)) != -1 ) {
			//Copy the actual read no. of bytes @ the 4th byte pos in the buffer.
			Buffer->length = bufLen;
			status = A_OK;
		} else {
			status = A_ERROR;
		}
	} else {
		return NDIS_STATUS_FAILURE;
	}

	if ( status != A_OK )
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
CAR6KMini::SetCAr6kHtcRawWrite(
	HTC_RAW_WRITE_PARAM *Buffer)
{
	A_STATUS status;

	if (rawIfInit) {
		if ( ar6000_htc_raw_write(m_pHTCTarget, (HTC_ENDPOINT_ID)Buffer->endPointId, \
			(&(Buffer->buffer)), Buffer->length) != -1) {
			status = A_OK;
		} else {
			status = A_ERROR;
		}
	} else {
		return NDIS_STATUS_FAILURE;
	}

	if ( status != A_OK )
		return NDIS_STATUS_FAILURE;

	return NDIS_STATUS_SUCCESS;
}
#endif //HTC_RAW_INTERFACE

#ifdef SUPPORT_WPA2
NDIS_STATUS
CAR6KMini::Set802_11PMKID(OUT NDIS_802_11_PMKID* pPMKID, 
						  IN  ULONG              cbBuffer)
{
	A_STATUS status;
	unsigned int i = 0, count = 0;
	BSSID_INFO  *Temp;
	NDIS_802_11_PMKID *tempPMKID = NULL;
	
	count = pPMKID->BSSIDInfoCount;

	// Microsoft recommands to overwrite PMKIDs everytime, so flush PMKIDs before setting
	if (m_pPMKID) {
		// This means supplicant is flushing the PMKIDs
		for (i = 0; i < m_pPMKID->BSSIDInfoCount; i++) {
			Temp = &m_pPMKID->BSSIDInfo[i];
			status = wmi_setPmkid_cmd((wmi_t*)m_pWMI, Temp->BSSID, NULL, 0);
		}
	}
		
	if (count == 0) {
		A_FREE(m_pPMKID);
		m_pPMKID = NULL;
		return NDIS_STATUS_SUCCESS;
	}

	tempPMKID = (NDIS_802_11_PMKID *)A_MALLOC(pPMKID->Length);
	tempPMKID->Length = pPMKID->Length;
	tempPMKID->BSSIDInfoCount = count;

	for  (i = 0; i < count; i++) {
		Temp = &pPMKID->BSSIDInfo[i];
		status = wmi_setPmkid_cmd((wmi_t *)m_pWMI, Temp->BSSID, Temp->PMKID, 1);
		
		if (status != A_OK) {
			/*
			 * Should we rollback to the original state??
			 */
			A_FREE(tempPMKID);
			return NDIS_STATUS_FAILURE;
		}
		memcpy(tempPMKID->BSSIDInfo[i].BSSID, Temp->BSSID, 6);
		memcpy(tempPMKID->BSSIDInfo[i].PMKID, Temp->PMKID, 16);
	}

	if (m_pPMKID != NULL) {
		A_FREE(m_pPMKID);
	}
	m_pPMKID = tempPMKID;
	return NDIS_STATUS_SUCCESS;
}
#endif //SUPPORT_WPA2
