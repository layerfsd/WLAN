/*
 * Copyright (c) 2008 Atheros Communications Inc.
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
// Endpoint ping implementation

#include <windows.h>
#include <types.h>
#include <ceddk.h>
#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include "a_debug.h"
#include "htc_api.h"
#include "common_drv.h"
#include "hif.h"
#include "bmi.h" 
#include "dl_list.h"
#include "wince_common_drv.h"

#ifdef HTC_EPPING_TEST

#define EPPING_PACKET_SIZE  1664
#define EPPING_RECV_BUFFERS 64
#define EPPING_SEND_BUFFERS 64

/* these limits define the max buffer on the endpoint ping application */
#define EPPING_MAX_RECV_PKTS_FOR_RECV_ONLY  12

#define EPPING_MAX_RECV_PKTS_FOR_SEND_ONLY  EPPING_MAX_RECV_PKTS_FOR_RECV_ONLY

#define EPPING_MAX_SEND_DEPTH  (EPPING_SEND_BUFFERS + 1)

#define A_ROUND_UP_PWR2(x, align)    (((int) (x) + ((align)-1)) & ~((align)-1))

#define TX_BUFFER_ALIGNMENT_BYTES 32

typedef struct AR6K_PKT_MGT_INFO {
    DL_LIST         ListEntry;
    HTC_PACKET      HtcPacket;
} AR6K_PKT_MGT_INFO;

#define HTC_DATA_FRAME_PAD (((sizeof(AR6K_PKT_MGT_INFO) + TX_BUFFER_ALIGNMENT_BYTES) & (~(TX_BUFFER_ALIGNMENT_BYTES - 1)))  \
                            - sizeof(AR6K_PKT_MGT_INFO))

#pragma pack(1)

    /* padded data fram that follows management info */
typedef struct _AR6K_DATAFRAME {
    A_UCHAR     _PadUnused[HTC_DATA_FRAME_PAD];
    A_UCHAR     Buffer[EPPING_PACKET_SIZE]; 
} AR6K_DATAFRAME;

#pragma pack()

typedef enum EPPING_TEST_MODE {
    EPPING_TEST_MODE_SEND_ANDDROP_ONLY,
    EPPING_TEST_MODE_SEND_ONLY,
    EPPING_TEST_MODE_RECV_ONLY,
    EPPING_TEST_MODE_BIDIR,
} EPPING_TEST_MODE;

typedef struct AR6K_HTC_BUFFER
{
    AR6K_PKT_MGT_INFO      MgtInfo;     // management info not part of the data frame
    AR6K_DATAFRAME         DataFrame;   // padded data frame
} AR6K_HTC_BUFFER;

typedef struct _EPPING_INSTANCE {
    HTC_HANDLE          HtcHandle;
    void                *HIFDevice;
    HTC_ENDPOINT_ID     Endpoint;
    COMMON_CREDIT_STATE_INFO CreditStateInfo;
    DL_LIST             PacketList;
    void                *pPacketBufferBlob;
    CRITICAL_SECTION    CritSection;
    A_STATUS            TestStatus;
    LONG                CurrentSendCount;
    LONG                CurrentRecvCount;
    LONG                PacketCount;
    A_BOOL              HtcStarted;
    HANDLE              hTestCompleteEvent;
    A_UINT32            SendSeqNum;
    EPPING_TEST_MODE    TestMode;
    A_UINT32            ExpectedRecvSize;
    A_BOOL              VerifyMode;
    A_UINT32            TargetType;
    A_UINT32            TargetVersion;   
}EPPING_INSTANCE;

#define LOCK_EPPING(pInstance)   EnterCriticalSection(&(pInstance)->CritSection)
#define UNLOCK_EPPING(pInstance) LeaveCriticalSection(&(pInstance)->CritSection)

EPPING_INSTANCE g_EpPing;

static void AR6KHTCBufferSentEventHandler(void              *Context,
                                          HTC_PACKET        *pPacket);
                                              
static void AR6KHTCBufferReceivedEventHandler(void            *Context,
                                              HTC_PACKET      *pPacket);

static void CleanupEpPing(EPPING_INSTANCE *pInstance);

static A_STATUS VerifyBuffer(A_UCHAR *buffer, int length);


void CleanupEpTest()
{
    EPPING_INSTANCE *pInstance = &g_EpPing;
    CleanupEpPing(pInstance);
}


static void CleanupEpPing(EPPING_INSTANCE *pInstance)
{
    if (pInstance->HtcStarted) {
        HTCStop(pInstance->HtcHandle);
        pInstance->HtcStarted = FALSE;
    }
    
    if (pInstance->pPacketBufferBlob != NULL) {
        LocalFree(pInstance->pPacketBufferBlob);
        pInstance->pPacketBufferBlob = NULL;
    }
    
    if (pInstance->hTestCompleteEvent != NULL) {
        CloseHandle(pInstance->hTestCompleteEvent);
        pInstance->hTestCompleteEvent = NULL;    
    }
    
}

static A_STATUS ConnectService(EPPING_INSTANCE         *pInstance,
                              HTC_SERVICE_CONNECT_REQ *pConnect)
{
    A_STATUS                 status;
    HTC_SERVICE_CONNECT_RESP response;

    do {

        A_MEMZERO(&response,sizeof(response));

        status = HTCConnectService(pInstance->HtcHandle,
                                   pConnect,
                                   &response);

        if (A_FAILED(status)) {
            break;
        }

        switch (pConnect->ServiceID) {
            case WMI_CONTROL_SVC :
            case WMI_DATA_BK_SVC :
            case WMI_DATA_VI_SVC :
            case WMI_DATA_VO_SVC :
                break;
            case WMI_DATA_BE_SVC :
                pInstance->Endpoint = response.Endpoint;
                break;
           default:
                status = A_EINVAL;
                break;
        }

    } while (FALSE);

    return status;
}


AR6K_HTC_BUFFER *AllocHTCBuffer(EPPING_INSTANCE *pInstance)
{
    AR6K_HTC_BUFFER *pHtcBuffer = NULL;
    DL_LIST         *pItem;
    
    LOCK_EPPING(pInstance);
    
    pItem = DL_ListRemoveItemFromHead(&pInstance->PacketList);
    
    if (pItem != NULL) {
        pHtcBuffer = A_CONTAINING_STRUCT(pItem, AR6K_HTC_BUFFER, MgtInfo.ListEntry);
    }
    
    UNLOCK_EPPING(pInstance);
    
    return pHtcBuffer; 
}

void FreeHTCBufferToList(EPPING_INSTANCE *pInstance, AR6K_HTC_BUFFER *pHtcBuffer)
{
    LOCK_EPPING(pInstance);
    
    DL_ListAdd(&pInstance->PacketList,&pHtcBuffer->MgtInfo.ListEntry);
    
    UNLOCK_EPPING(pInstance);
}

A_STATUS AllocateHTCPacketBuffers(EPPING_INSTANCE *pInstance, int NumBuffers)
{
    ULONG             totalBytes;
    ULONG             alignedLength;
    A_UCHAR           *pBuffer;
    AR6K_HTC_BUFFER   *pHtcBuffer;
    int               i;
   
    alignedLength = A_ROUND_UP_PWR2(sizeof(AR6K_HTC_BUFFER), TX_BUFFER_ALIGNMENT_BYTES);
   
    totalBytes = NumBuffers *  alignedLength;
   
    pInstance->pPacketBufferBlob = LocalAlloc(LPTR, totalBytes);
    
    if (NULL == pInstance->pPacketBufferBlob) {
        return A_NO_MEMORY;  
    }
    
    pBuffer = (A_UCHAR *)pInstance->pPacketBufferBlob;
    
        /* carve up buffers */
    for (i = 0; i < NumBuffers; i++, pBuffer += alignedLength) {
        pHtcBuffer = (AR6K_HTC_BUFFER *)pBuffer;
        FreeHTCBufferToList(pInstance, pHtcBuffer);
    }
   
    return A_OK;
}

static void GiveRecvBuffer(EPPING_INSTANCE  *pInstance, 
                           AR6K_HTC_BUFFER  *pHTCBuffer, 
                           A_BOOL           Return)
{
    if (!Return) {
        
        SET_HTC_PACKET_INFO_RX_REFILL(&pHTCBuffer->MgtInfo.HtcPacket,
                                      pHTCBuffer,
                                      pHTCBuffer->DataFrame.Buffer,
                                      EPPING_PACKET_SIZE,
                                      pInstance->Endpoint);
    } else {
        
        HTC_PACKET_RESET_RX(&pHTCBuffer->MgtInfo.HtcPacket);    
    }

    HTCAddReceivePkt(pInstance->HtcHandle, &pHTCBuffer->MgtInfo.HtcPacket);

}

void SetupHTC(EPPING_INSTANCE *pInstance, A_UINT32 TargetType, A_UINT32 TargetVersion)
{
    A_STATUS                status;
    HTC_SERVICE_CONNECT_REQ connect;
    int                     i;
    AR6K_HTC_BUFFER         *pHtcBuffer;
    
    do {
        
        pInstance->hTestCompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        
        if (NULL == pInstance->hTestCompleteEvent) {
            status = A_NO_MEMORY;
            break;    
        }
        
        if (TargetType == TARGET_TYPE_AR6002) {
            status =  configure_ar6000(pInstance->HIFDevice,
                                       TargetType,
                                       TargetVersion,
                                       TRUE,
                                       FALSE,
                                       26000000,
                                       TEXT("\\endpointping2_0.bin"),
                                       TEXT("\\Windows"),
                                       FALSE,
                                       TRUE,
                                       TEXT("\0"));
                               
            if (A_FAILED(status)) {
                break;    
            }         
        }
           
        status = ar6000_set_htc_params(pInstance->HIFDevice,
                                       TargetType,
                                       0,
                                       0);  
                                       
        if (A_FAILED(status)) {
            break;
        }
                          
        status = BMIDone(pInstance->HIFDevice);
        
        if (A_FAILED(status)) {
            break;
        }
        
            /*
             * The target is now running, wait for HTC to come on-line
             */
        status = HTCWaitTarget(pInstance->HtcHandle);

        if (A_FAILED(status)) {
            break;
        }

        A_MEMZERO(&connect,sizeof(connect));
            /* meta data is unused for now */
        connect.pMetaData = NULL;
        connect.MetaDataLength = 0;
        connect.EpCallbacks.pContext = pInstance;
        connect.EpCallbacks.EpTxComplete = AR6KHTCBufferSentEventHandler;
        connect.EpCallbacks.EpRecv = AR6KHTCBufferReceivedEventHandler;
        connect.EpCallbacks.EpRecvRefill = NULL;
        connect.EpCallbacks.EpSendFull = NULL;
        connect.MaxSendQueueDepth = EPPING_MAX_SEND_DEPTH;
        connect.EpCallbacks.EpSendFull = NULL;
        
          /* connect to control service */
        connect.ServiceID = WMI_CONTROL_SVC;
        status = ConnectService(pInstance, &connect);
        
        if (A_FAILED(status)) {
            break;
        }
        
            /* connect to best-effort service */
        connect.ServiceID = WMI_DATA_BE_SVC;
        status = ConnectService(pInstance, &connect);
        
        if (A_FAILED(status)){
            break;
        }
        
        connect.ServiceID = WMI_DATA_BK_SVC;
        status = ConnectService(pInstance, &connect);
        if (A_FAILED(status)){
            break;
        }
        
        connect.ServiceID = WMI_DATA_VI_SVC;
        status = ConnectService(pInstance, &connect);
        if (A_FAILED(status)){
            break;
        }

        connect.ServiceID = WMI_DATA_VO_SVC;
        status = ConnectService(pInstance, &connect);
        if (A_FAILED(status)){
            break;
        }
        
        ar6000_setup_credit_dist(pInstance->HtcHandle, 
                                 &pInstance->CreditStateInfo);

        status = HTCStart(pInstance->HtcHandle);
        
        if (A_OK != status) {
            break;
        }
        
        pInstance->HtcStarted = TRUE;
        
        status = AllocateHTCPacketBuffers(pInstance, 
                                          EPPING_RECV_BUFFERS + EPPING_SEND_BUFFERS);
        
        if (A_FAILED(status)) {
            break;    
        }
        
            /* give HTC some packet buffers */
        for (i = 0; i < EPPING_RECV_BUFFERS ; i++) {
            pHtcBuffer = AllocHTCBuffer(pInstance);
            if (pHtcBuffer != NULL) {
                GiveRecvBuffer(pInstance, 
                               pHtcBuffer, 
                               FALSE);
            } else {
                break;    
            }
        }
                   
    } while (FALSE);
    
    if (A_FAILED(status)) {
        CleanupEpTest(pInstance);    
    }
}


static DWORD EpTestSetupThread(PVOID pContext) 
{
    SetupHTC(&g_EpPing,g_EpPing.TargetType,g_EpPing.TargetVersion);
    return 0;
}

void SetupEpTest(void *HtcHandle, A_UINT32 TargetType, A_UINT32 TargetVersion)
{
    A_STATUS      athStatus = A_OK;
    HANDLE        hThread;
    
    A_ASSERT(HtcHandle != NULL);
    
    A_MEMZERO(&g_EpPing,sizeof(g_EpPing));
    
    g_EpPing.HtcHandle = HtcHandle;
    g_EpPing.HIFDevice = HTCGetHifDevice(HtcHandle);
    g_EpPing.TargetType = TargetType;
    g_EpPing.TargetVersion = TargetVersion;
    
    DL_LIST_INIT(&g_EpPing.PacketList);
    InitializeCriticalSection(&g_EpPing.CritSection);
    
        /* start a thread to do the initialization, 
         * *** Note:  a thread is used because a thread created by this
         * driver will have the proper access permissions to use stack-local memory for
         * I/O calls to undelying bus drivers */
         
    hThread = CreateThread (NULL, 0, EpTestSetupThread, NULL, 0, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    
}

static void CheckCompletion(EPPING_INSTANCE *pInstance)
{
    A_BOOL setEvent = FALSE;
    
    do {
        
        if (A_FAILED(pInstance->TestStatus)) {
            setEvent = TRUE;
            break;    
        }
        
        switch (pInstance->TestMode) {
            case EPPING_TEST_MODE_SEND_ONLY:
            case EPPING_TEST_MODE_SEND_ANDDROP_ONLY:
                    /* send mode only */
                if (pInstance->PacketCount == pInstance->CurrentSendCount) {
                    setEvent = TRUE;
                }  
                break;
                
            case EPPING_TEST_MODE_RECV_ONLY:
                    /* recv mode only */
                if (pInstance->PacketCount == pInstance->CurrentRecvCount) {
                    setEvent = TRUE;
                }   
                break;
            
            default:
                    /* bidir ping test */
                if ((pInstance->PacketCount == pInstance->CurrentSendCount) &&
                    (pInstance->PacketCount == pInstance->CurrentRecvCount)) {
                    setEvent = TRUE;
                }
                break;    
        }
        
    } while (FALSE);
    
    if (setEvent) {
        SetEvent(pInstance->hTestCompleteEvent);    
    }
}

static void AR6KHTCBufferReceivedEventHandler(void            *Context,
                                              HTC_PACKET      *pPacket)
{
    EPPING_INSTANCE *pInstance = (EPPING_INSTANCE *)Context;
                 
    if (A_FAILED(pPacket->Status)) {
        pInstance->TestStatus = A_ERROR;      
    } else {
        
        A_ASSERT((pInstance->TestMode == EPPING_TEST_MODE_RECV_ONLY) ||  
                 (pInstance->TestMode == EPPING_TEST_MODE_BIDIR));
             
        if (pPacket->ActualLength != pInstance->ExpectedRecvSize) {
            A_ASSERT(FALSE);    
        }
        if (pInstance->VerifyMode) {
            VerifyBuffer(pPacket->pBuffer,pPacket->ActualLength);
        }
        
        InterlockedIncrement(&pInstance->CurrentRecvCount);  
    }

    if (pPacket->Status != A_ECANCELED) {
            // recycle packet
        GiveRecvBuffer(pInstance, 
                      (AR6K_HTC_BUFFER  *)pPacket->pPktContext, 
                      TRUE);
    }
                              
    CheckCompletion(pInstance);
    
}

static void AR6KHTCBufferSentEventHandler(void              *Context,
                                          HTC_PACKET        *pPacket)
{
    EPPING_INSTANCE *pInstance = (EPPING_INSTANCE *)Context;
             
    if (A_FAILED(pPacket->Status)) {
        pInstance->TestStatus = A_ERROR;
    }
    
    FreeHTCBufferToList(pInstance, (AR6K_HTC_BUFFER  *)pPacket->pPktContext);
    
    InterlockedIncrement(&pInstance->CurrentSendCount);
    CheckCompletion(pInstance);
    
}

static unsigned int crc16table[256] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, 
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, 
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};    

unsigned short CalcCRC16(unsigned char *pBuffer, int length)
{
    unsigned int ii;
    unsigned int index;
    unsigned short crc16 = 0x0;
    
    for(ii = 0; ii < (unsigned int)length; ii++) {
        index = ((crc16 >> 8) ^ pBuffer[ii]) & 0xff;
        crc16 = ((crc16 << 8) ^ crc16table[index]) & 0xffff;
    }
    return crc16;
}

#define HDR_SIZE          20
#define DATA_OFFSET      (HDR_SIZE + 2)

static A_STATUS VerifyBuffer(A_UCHAR *buffer, int length)
{
    A_STATUS status = A_OK;
    unsigned char *pBuffer = &buffer[DATA_OFFSET];
    unsigned short crc16,verifyCrc16; 
    int crcLength;
    
        /* last 2 bytes is the CRC */ 
    crc16 = ((unsigned short)buffer[length - 1]) | (((unsigned short)buffer[length - 2]) << 8);
                 
        /* compute CRC , do not include all the header info and do not include CRC bytes*/
    crcLength = length - DATA_OFFSET - 2;
    if (crcLength > 0) {
        verifyCrc16 = CalcCRC16(pBuffer,crcLength);
        if (crc16 != verifyCrc16) {
            RETAILMSG(1,(TEXT
                ("Sequence Number: buffer (%d bytes) CRC error (got:0x%4.4X expecting:0x%4.4X) \r\n"), 
                 length, verifyCrc16, crc16));
            status = A_ERROR;
        }
    } else {
       A_ASSERT(FALSE);
       status = A_ERROR;
    }
    
    return status;
}

 /*
    0         1         2         3     7      15        16    20         22  ..... 
    | mboxRxH | mboxTxT | mboxRxT | PID | TxTS | mboxTxH | Seq | hdr CRC  |   DATA
    */
    
void FillDataBuffer(EPPING_INSTANCE *pInstance, A_UCHAR *pBuffer, int FillLength)
{
    A_UCHAR *txpacket = pBuffer;
    int     length = 0;
    int     mboxRx = 0;
    int     mboxTx = 0;
    
    
    if (FillLength < 32) {
        A_ASSERT(FALSE);
        return;    
    }
    
    txpacket[length++] = mboxRx; /* Receive stream number */

    if (pInstance->TestMode == EPPING_TEST_MODE_SEND_ANDDROP_ONLY) {
            /* set magic code to tell target to drop this packet, the host
             * only wants to measure send operations without having recvs get in the
             * way */
        txpacket[length++] = 0xAA; 
        txpacket[length++] = 0x56; 
    } else {
        txpacket[length++] = 0xDE; /* mboxTx, filled by target */
        txpacket[length++] = 0xAD; /* mboxRx, filled by target */
    }
    
        /* dummy PID */
    length += 4;
        /* dummy TxTS */
    length += 8;
    
    txpacket[length++] = (mboxTx << 1); /* send stream number ... IP TOS Offset */
    memcpy(&txpacket[length], &pInstance->SendSeqNum, sizeof(A_UINT32));
    length += sizeof(A_UINT32); /* Sequence counter. Unique for every txpacket sent */
    pInstance->SendSeqNum++;

        /* dummy header CRC */
    length += 2;
        
    if (pInstance->VerifyMode) {
        unsigned char* pBuffer;
        int verifylength = 0;
        unsigned short CRC16;
            
            /* save start of buffer */
        pBuffer = &txpacket[length];
            /* seed the random number generator */
        srand(GetTickCount());
            /* fill the remainder with data */
        while (length < (FillLength - 2)) {
                /* stick in some random data */
            //if (verfRandomData) {
                txpacket[length] = (unsigned char)rand() + length;
            //} else {
            //    txpacket[length] = length;
            //}
            length++;
            verifylength++;
        }
       
        CRC16 = CalcCRC16(pBuffer,verifylength);
            /* append 16 bit CRC at the end */
        txpacket[length++] = ((unsigned char *)(&CRC16))[1];
        txpacket[length++] = ((unsigned char *)(&CRC16))[0];
        
    } else {
            /* Fill the rest of the txpacket */
        while (length < FillLength) {
            txpacket[length] = length;
            length += 1;
        }
    }
}

static A_STATUS SendBuffers(EPPING_INSTANCE *pInstance, int NumPackets, int PktLength)
{
    int               i;
    AR6K_HTC_BUFFER  *pHTCBuffer;
    A_STATUS          status = A_OK;
    
    for (i = 0; i < NumPackets; i++) {
        
        pHTCBuffer = AllocHTCBuffer(pInstance);
        
        if (NULL == pHTCBuffer) {
            status = A_NO_MEMORY;   
            break; 
        }
        
        FillDataBuffer(pInstance, &pHTCBuffer->DataFrame.Buffer[HTC_HDR_LENGTH], PktLength);
        
        SET_HTC_PACKET_INFO_TX(&pHTCBuffer->MgtInfo.HtcPacket,
                               pHTCBuffer,
                               &pHTCBuffer->DataFrame.Buffer[HTC_HDR_LENGTH],
                               PktLength,
                               pInstance->Endpoint,
                               AR6K_DATA_PKT_TAG);
        
        HTCSendPkt(pInstance->HtcHandle, &pHTCBuffer->MgtInfo.HtcPacket);
        
    }
    
    return status;
}

A_STATUS RunOnePass(EPPING_INSTANCE     *pInstance, 
                    A_UINT32            Packets, 
                    A_UINT32            PayloadSize,
                    EPPING_TEST_MODE    TestMode) 
{
    pInstance->PacketCount = Packets;
    pInstance->CurrentSendCount = 0;
    pInstance->CurrentRecvCount = 0;
    pInstance->SendSeqNum = 0;
    pInstance->TestStatus = A_OK;
    pInstance->TestMode = TestMode;
    pInstance->ExpectedRecvSize = PayloadSize;
    
    ResetEvent(pInstance->hTestCompleteEvent);
    
    if (TestMode == EPPING_TEST_MODE_RECV_ONLY) {
            /* the caller has queued up packets and disabled recv so that the pings pile up
             * in the target */ 
            /* reenable recv to go fetch them */
        HTCEnableRecv(pInstance->HtcHandle);
    } else {
        SendBuffers(pInstance, pInstance->PacketCount, PayloadSize);
    }
   
        /* wait for all echos */
    WaitForSingleObject(pInstance->hTestCompleteEvent, INFINITE);
    return pInstance->TestStatus;
}


static void OutputStats(TCHAR         *pDescription,
                        A_UINT32      Operations, 
                        LARGE_INTEGER *pDeltaTicks, 
                        LARGE_INTEGER *pFreq,
                        A_UINT32       BytesPerOp)
{
    float timeDeltaMS;
    float totalOps;
    float opsPerSecond;
    
    totalOps = (float)Operations;
    timeDeltaMS = (float)pDeltaTicks->QuadPart;
    timeDeltaMS *= 1000;
    timeDeltaMS /= (float)pFreq->LowPart;
    
    opsPerSecond = ((totalOps * 1000)/timeDeltaMS);
    
    RETAILMSG(1, (TEXT 
       (" ---->  %s Stats : %d packets, %d bytes per packet, time:%d Ticks\n"),
               pDescription,
               (UINT32)totalOps, 
               BytesPerOp,
               pDeltaTicks->LowPart));
               
    RETAILMSG(1, (TEXT 
       ("          (%d packets/sec) (%d bytes/sec):  (%d bits/sec) \n"),
               (A_UINT32)(opsPerSecond),
               ((A_UINT32)opsPerSecond) * BytesPerOp,
               ((A_UINT32)opsPerSecond) * BytesPerOp * 8
               )); 
    
    
    //((totalOps * payloadSize * 1000)/timeDeltaMS) / (float)1000000
    
}
    

#define CAPTURE_TIMESTAMP(time,usetick)   \
{                                         \
     if ((usetick)) {                     \
        (time)->LowPart = GetTickCount(); \
    } else {                              \
        QueryPerformanceCounter((time));  \
    }                                     \
}

#define CALC_DELTA(delta,start,end)                          \
{                                                            \
    (delta)->QuadPart = (end)->QuadPart - (start)->QuadPart; \
}


static void DoPingTest(EPPING_INSTANCE *pInstance, 
                       A_UINT32         PacketSize,
                       A_UINT32         Packets, 
                       A_UINT32         Iterations) 
{
    LARGE_INTEGER  freq; 
    LARGE_INTEGER  delta;
    BOOL           useTick = FALSE;  
    LARGE_INTEGER  start,end;
    A_UINT32       payloadSize = PacketSize;
    A_UINT32       iterations = Iterations;
    A_UINT32       packets = Packets;
    A_UINT32       count;
    A_STATUS       status;
    
    A_MEMZERO(&start,sizeof(start));
    A_MEMZERO(&end,sizeof(end));
    
    if (QueryPerformanceFrequency(&freq)) {
        RETAILMSG(1, (TEXT(" %s mode , Counter Freq : %d \r\n"),
          pInstance->VerifyMode ? TEXT("Verify") : TEXT("Performance"),
          freq.LowPart));
    } else {
        freq.LowPart = 1000;
        RETAILMSG(1, (TEXT(" No performance counters, using 1MS tick \r\n"))); 
        useTick = TRUE;
    }
    
    do {
        
        if (A_FAILED(pInstance->TestStatus)) {
                /* abort test */
            break;    
        } 
        
        /********** start recv only test *************/

        /* first send one echo packet to flush out any stale credits on the target */
        status = RunOnePass(pInstance, 
                            1,   /* just 1 packet, the echo reply will return credits */
                            64,  /* just make it a small packet */
                            EPPING_TEST_MODE_BIDIR);     
          
        if (A_FAILED(status)) {
            A_ASSERT(FALSE);
            break;    
        }

            /* first disable recvs so that the buffers build up on the target */
        HTCDisableRecv(pInstance->HtcHandle);
        
            /* first send some packets to build up the ping packets in the RECV queue on the target */
        status = RunOnePass(pInstance, 
                            EPPING_MAX_RECV_PKTS_FOR_RECV_ONLY, 
                            payloadSize, 
                            EPPING_TEST_MODE_SEND_ONLY);     
        
        if (A_FAILED(status)) {
            A_ASSERT(FALSE);
            break;    
        }
                 
            /* now that there are packets queued on the target, go and fetch them */                    
        CAPTURE_TIMESTAMP(&start,useTick);
        
        status = RunOnePass(pInstance, 
                            EPPING_MAX_RECV_PKTS_FOR_RECV_ONLY, 
                            payloadSize, 
                            EPPING_TEST_MODE_RECV_ONLY);     
           
        if (A_FAILED(status)) {
            A_ASSERT(FALSE);
            break;    
        }
        
        CAPTURE_TIMESTAMP(&end,useTick);
        CALC_DELTA(&delta,&start,&end);
    
        OutputStats( TEXT("Recv Only"),
                     EPPING_MAX_RECV_PKTS_FOR_RECV_ONLY, 
                     &delta, 
                     &freq,
                     payloadSize);  
       
        /********** end recv only test *************/
        
        /********** start send only test *************/
        
        CAPTURE_TIMESTAMP(&start,useTick);   
             
        status = RunOnePass(pInstance, 
                            EPPING_MAX_RECV_PKTS_FOR_SEND_ONLY, 
                            payloadSize, 
                            EPPING_TEST_MODE_SEND_ANDDROP_ONLY);   
                            
        if (A_FAILED(status)) {
            A_ASSERT(FALSE);
            break;    
        }   
            
        CAPTURE_TIMESTAMP(&end,useTick);
        CALC_DELTA(&delta,&start,&end);
        
        OutputStats( TEXT("Send Only"),
                     EPPING_MAX_RECV_PKTS_FOR_SEND_ONLY, 
                     &delta, 
                     &freq,
                     payloadSize);    
        
        /********** end send only test *************/
    
        
        /********** start  bidirectional PING test *************/
        
        count = 0;
        status = A_OK;
        
        CAPTURE_TIMESTAMP(&start,useTick);
        
        while (A_SUCCESS(status) && (count < iterations)) {
            status = RunOnePass(pInstance, packets, payloadSize, EPPING_TEST_MODE_BIDIR);     
            count++;
        }
       
        CAPTURE_TIMESTAMP(&end,useTick);
        CALC_DELTA(&delta,&start,&end);
    
        if (A_FAILED(status)) {
            A_ASSERT(FALSE);
            break;    
        }
        
        OutputStats(TEXT("Bi-directional Ping"),
                    (packets * 2) * iterations, 
                    &delta, 
                    &freq,
                    payloadSize);   
        
        /********** end bidirectional PING test *************/
 
    } while (FALSE);  
}

#define ITERATIONS_PER_PASS   100
#define PACKETS_PER_ITERATION 32

static DWORD RunPingTestThread(PVOID pContext)
{
    
    if (g_EpPing.HtcStarted) {
        //g_EpPing.VerifyMode = TRUE; 
        DoPingTest(&g_EpPing,1500,PACKETS_PER_ITERATION,ITERATIONS_PER_PASS);    
#if 1        
        DoPingTest(&g_EpPing,1200,PACKETS_PER_ITERATION,ITERATIONS_PER_PASS);
        DoPingTest(&g_EpPing,900,PACKETS_PER_ITERATION,ITERATIONS_PER_PASS);
        DoPingTest(&g_EpPing,600,PACKETS_PER_ITERATION,ITERATIONS_PER_PASS);
        DoPingTest(&g_EpPing,300,PACKETS_PER_ITERATION,ITERATIONS_PER_PASS);
        DoPingTest(&g_EpPing,100,PACKETS_PER_ITERATION,ITERATIONS_PER_PASS);
#endif

    }
    
    return 0;
}

void RunPingTest()
{
    HANDLE  hThread;
        /* start a thread to do the test, 
         * *** Note:  a thread is used because the caller could be in the context
         * of a user mode thread whose stack memory is not accessible by this
         * driver */
         
    hThread = CreateThread (NULL, 0, RunPingTestThread, NULL, 0, NULL);
    
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    
}
#endif
