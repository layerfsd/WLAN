/*++
Copyright (c) 2008 Atheros Communications Inc.
All rights reserved.


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


 Simplified, single threaded (micro) implementation of HTC (for bootloaders)


--*/

#include "system.h"
#include "u_hif.h"
#include "u_htc.h"
#include "u_htc_internal.h"

#define HTC_TARGET_READY_TIMEOUT 2000
#define MAX_CONTROL_MESSAGE_BUFFER_SIZE (256 + HTC_HDR_LENGTH)
         
A_UCHAR     g_ControlMessageBuffer[MAX_CONTROL_MESSAGE_BUFFER_SIZE];
A_UCHAR     *g_ControlMessagePayloadStart = &g_ControlMessageBuffer[HTC_HDR_LENGTH];   

HTC_TARGET  g_HTCTarget;
                       
void HTCShutDown(void)
{
    
    
    
}

/* wait for the target to arrive (sends HTC Ready message)
 * this operation is fully synchronous and the message is polled for */
static A_STATUS HTCWaitTarget()
{
    A_STATUS                 status;
    HTC_READY_MSG            *pRdyMsg;
    HTC_HANDLE               HTCHandle = (HTC_HANDLE)1;
    HTC_ENDPOINT_ID          endpoint;
    int                      length;
    
    DBG_LOG_PRINT(DBG_ZONE_HTC_TRC, ("+HTCWaitTarget \n"));

    do {
          
            /* we should be getting 1 control message that the target is ready */
        status = HTCGetMessage(HTCHandle,                
                               &endpoint, 
                               g_ControlMessagePayloadStart, 
                               MAX_CONTROL_MESSAGE_BUFFER_SIZE,
                               &length,
                               HTC_TARGET_READY_TIMEOUT);
        
        if (A_FAILED(status)) {
            DBG_LOG_PRINT(ATH_DEBUG_ERR, (" Target Not Available!!\n"));
            break;    
        }
        
        if (endpoint != ENDPOINT_0) {
            status = A_EPROTO;
            A_ASSERT(FALSE);
            break;    
        }
        
        pRdyMsg = (HTC_READY_MSG *)g_ControlMessagePayloadStart;
        
        if ((pRdyMsg->MessageID != HTC_MSG_READY_ID) ||
            (length < sizeof(HTC_READY_MSG))) {
                /* this message is not valid */
            AR_DEBUG_ASSERT(FALSE);
            status = A_EPROTO;
            break;    
        }
        
        if (pRdyMsg->CreditCount == 0 || pRdyMsg->CreditSize == 0) {
              /* this message is not valid */
            AR_DEBUG_ASSERT(FALSE);
            status = A_EPROTO;    
            break;
        }
        
        g_HTCTarget.TotalCredits = pRdyMsg->CreditCount;
        g_HTCTarget.CreditsAvailable = pRdyMsg->CreditCount;
        g_HTCTarget.TargetCreditSize = pRdyMsg->CreditSize;
        
        DBG_LOG_PRINT(DBG_ZONE_HTC, (" Target Ready: credits: %d credit size: %d\n",
                pRdyMsg->CreditCount, pRdyMsg->CreditSize));
               
    } while (FALSE);
    
        
    DBG_LOG_PRINT(DBG_ZONE_HTC_TRC, ("-HTCWaitTarget - Exit\n"));
        
    return status;

}

HTC_HANDLE  HTCConnect(HIF_DEVICE *hifDevice)
{
    A_STATUS status = A_OK;
    A_UINT32 mailboxaddrs[4];
    A_UINT32 blocksizes[4];
    
    A_MEMZERO(&g_HTCTarget,sizeof(g_HTCTarget));
    
    g_HTCTarget.HifDevice = hifDevice;
    
    do {
          /* get the addresses for all 4 mailboxes */
        status = HIFConfigureDevice(hifDevice, HIF_DEVICE_GET_MBOX_ADDR,
                                    mailboxaddrs, sizeof(mailboxaddrs));

        if (status != A_OK) {
            AR_DEBUG_ASSERT(FALSE);
            break;
        }
            /* get the address of the mailbox we are using */
        g_HTCTarget.HifMailboxAddress = mailboxaddrs[HTC_MAILBOX];

            /* get the block sizes */
        status = HIFConfigureDevice(hifDevice, HIF_DEVICE_GET_MBOX_BLOCK_SIZE,
                                    blocksizes, sizeof(blocksizes));

        if (status != A_OK) {
            AR_DEBUG_ASSERT(FALSE);
            break;
        }

            /* note: we actually get the block size of a mailbox other than 0, for SDIO the block
             * size on mailbox 0 is artificially set to 1.  So we use the block size that is set
             * for the other 3 mailboxes */
        g_HTCTarget.HifBlockSize = blocksizes[1];
            /* must be a power of 2 */
        AR_DEBUG_ASSERT((g_HTCTarget.HifBlockSize & (g_HTCTarget.HifBlockSize- 1)) == 0);

            /* assemble mask, used for padding to a block */
        g_HTCTarget.HifBlockMask = g_HTCTarget.HifBlockSize - 1;

        AR_DEBUG_PRINTF(DBG_ZONE_HTC,("HTC BlockSize: %d, MailboxAddress:0x%X \n",
                    g_HTCTarget.HifBlockSize, g_HTCTarget.HifMailboxAddress));
                            
        status = HTCWaitTarget();
    
        if (A_FAILED(status)) {
            break;    
        }
    
    } while (FALSE);
    
    if (A_FAILED(status)) {
        return NULL;    
    }
    
    return (HTC_HANDLE)1;    
} 

A_STATUS HTCConnectService(HTC_HANDLE               HTCHandle, 
                           A_UINT16                 ServiceID,
                           HTC_ENDPOINT_ID          *pEndpoint)
{
    A_STATUS                            status = A_OK;
    HTC_CONNECT_SERVICE_RESPONSE_MSG    *pResponseMsg;
    HTC_CONNECT_SERVICE_MSG             *pConnectMsg;
    HTC_ENDPOINT_ID                     endpointID;
    int                                 length;
    int                                 maxBufLength = MAX_CONTROL_MESSAGE_BUFFER_SIZE;
    A_UINT8                             *pBuffer = g_ControlMessagePayloadStart;
    
    AR_DEBUG_PRINTF(ATH_DEBUG_TRC, ("+HTCConnectService, SvcID:0x%X \n",ServiceID));
      
    do {
            /* assemble connect service message */
        pConnectMsg = (HTC_CONNECT_SERVICE_MSG *)pBuffer;
        A_MEMZERO(pConnectMsg,sizeof(HTC_CONNECT_SERVICE_MSG));         
        pConnectMsg->MessageID = HTC_MSG_CONNECT_SERVICE_ID;
        pConnectMsg->ServiceID = ServiceID;
                
        status = HTCIssueMsg(HTCHandle, 
                             ENDPOINT_0, 
                             pBuffer,
                             sizeof(HTC_CONNECT_SERVICE_MSG));
                    
        if (A_FAILED(status)) {
            break;    
        }
        
            /* wait for response */
        status = HTCGetMessage(HTCHandle,                
                               &endpointID, 
                               pBuffer,  /* reuse transmit buffer */
                               maxBufLength,
                               &length,
                               HTC_TARGET_READY_TIMEOUT);
        
        
        if (A_FAILED(status)) {
            break;    
        }            
        
        if (endpointID != ENDPOINT_0) {
            A_ASSERT(FALSE);
            status = A_EPROTO;
            break;    
        }
        
            /* we controlled the buffer creation so it has to be properly aligned */    
        pResponseMsg = (HTC_CONNECT_SERVICE_RESPONSE_MSG *)pBuffer;
        
        if ((pResponseMsg->MessageID != HTC_MSG_CONNECT_SERVICE_RESPONSE_ID) ||
            (length < sizeof(HTC_CONNECT_SERVICE_RESPONSE_MSG))) {
                /* this message is not valid */
            AR_DEBUG_ASSERT(FALSE);
            status = A_EPROTO;
            break;    
        }      
                 
            /* check response status */           
        if (pResponseMsg->Status != HTC_SERVICE_SUCCESS) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,
                (" Target failed service 0x%X connect request (status:%d)\n",
                            pResponseMsg->ServiceID, pResponseMsg->Status));
            status = A_EPROTO;                
            break;    
        }
                       
        
            /* the rest of these are parameter checks so set the error status */
        status = A_EPROTO;
        
        if (pResponseMsg->EndpointID >= ENDPOINT_MAX) {
            AR_DEBUG_ASSERT(FALSE);
            break;        
        }
           
        if (0 == pResponseMsg->MaxMsgSize) {
            AR_DEBUG_ASSERT(FALSE);
            break;    
        }    
        
        *pEndpoint = pResponseMsg->EndpointID;
        
        status = A_OK;
        
    } while (FALSE);

    AR_DEBUG_PRINTF(ATH_DEBUG_TRC, ("-HTCConnectService \n"));
    
    return status;    
}

A_STATUS HTCStart(HTC_HANDLE HTCHandle)
{
    HTC_SETUP_COMPLETE_MSG *pSetupComplete;
    
        /* assemble setup complete message */
    pSetupComplete = (HTC_SETUP_COMPLETE_MSG *)g_ControlMessagePayloadStart; 
    A_MEMZERO(pSetupComplete,sizeof(HTC_SETUP_COMPLETE_MSG));         
    pSetupComplete->MessageID = HTC_MSG_SETUP_COMPLETE_ID;
                          
    return HTCIssueMsg(HTCHandle, 
                       ENDPOINT_0, 
                       g_ControlMessagePayloadStart,
                       sizeof(HTC_SETUP_COMPLETE_MSG));
 
}
