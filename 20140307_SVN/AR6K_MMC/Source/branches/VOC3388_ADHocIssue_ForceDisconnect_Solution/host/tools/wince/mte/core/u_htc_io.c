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


 Input/Output routines for the HTC protocol


--*/

#include "system.h"
#include "u_hif.h"
#include "u_htc.h"
#include "u_htc_internal.h"
#include "AR6001/hw/mbox_host_reg.h"


#define AR6K_TARGET_DEBUG_INTR_MASK     0x01


/* process credit reports and call distribution function */
static void HTCProcessCreditRpt(HTC_TARGET *target, HTC_CREDIT_REPORT *pRpt, int NumEntries, HTC_ENDPOINT_ID FromEndpoint)
{
    int             i;
   
    AR_DEBUG_PRINTF(ATH_DEBUG_SEND, ("+HTCProcessCreditRpt, Credit Report Entries:%d \n", NumEntries));
         
    for (i = 0; i < NumEntries; i++, pRpt++) {
        if (pRpt->EndpointID >= ENDPOINT_MAX) {
            AR_DEBUG_ASSERT(FALSE);
            break;   
        }                     
        AR_DEBUG_PRINTF(ATH_DEBUG_SEND, ("  Endpoint %d got %d credits \n",
                pRpt->EndpointID, pRpt->Credits));

        target->CreditsAvailable += pRpt->Credits;                
    }    
    
    AR_DEBUG_PRINTF(ATH_DEBUG_SEND, ("-HTCProcessCreditRpt \n"));
}

 
static A_STATUS HTCProcessTrailer(HTC_TARGET *target, 
                                  A_UINT8    *pBuffer, 
                                  int         Length, 
                                  A_UINT32   *pNextLookAhead,
                                  HTC_ENDPOINT_ID FromEndpoint)
{
    HTC_RECORD_HDR          *pRecord;
    A_UINT8                 *pRecordBuf;
    HTC_LOOKAHEAD_REPORT    *pLookAhead;
    A_UINT8                 *pOrigBuffer;
    int                     origLength;
    A_STATUS                status;
    
    AR_DEBUG_PRINTF(ATH_DEBUG_RECV, ("+HTCProcessTrailer (length:%d) \n", Length));   
    
    //AR_DEBUG_PRINTBUF(pBuffer,Length,"Recv Trailer");
    
   
    pOrigBuffer = pBuffer;
    origLength = Length;
    status = A_OK;
      
    while (Length > 0) {
       
        if (Length < sizeof(HTC_RECORD_HDR)) {
            status = A_EPROTO;
            break;    
        }
            /* these are byte aligned structs */
        pRecord = (HTC_RECORD_HDR *)pBuffer;
        Length -= sizeof(HTC_RECORD_HDR);
        pBuffer += sizeof(HTC_RECORD_HDR);
        
        if (pRecord->Length > Length) {
                /* no room left in buffer for record */
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, 
                (" invalid record length: %d (id:%d) buffer has: %d bytes left \n", 
                        pRecord->Length, pRecord->RecordID, Length));
            status = A_EPROTO;
            break;    
        }        
            /* start of record follows the header */
        pRecordBuf = pBuffer;
        
        switch (pRecord->RecordID) {
            case HTC_RECORD_CREDITS:
                AR_DEBUG_ASSERT(pRecord->Length >= sizeof(HTC_CREDIT_REPORT));
                HTCProcessCreditRpt(target,
                                    (HTC_CREDIT_REPORT *)pRecordBuf, 
                                    pRecord->Length / (sizeof(HTC_CREDIT_REPORT)),
                                    FromEndpoint);
                break;
            case HTC_RECORD_LOOKAHEAD:
                AR_DEBUG_ASSERT(pRecord->Length >= sizeof(HTC_LOOKAHEAD_REPORT));  
                pLookAhead = (HTC_LOOKAHEAD_REPORT *)pRecordBuf;    
                if ((pLookAhead->PreValid == ((~pLookAhead->PostValid) & 0xFF)) && 
                    (pNextLookAhead != NULL)) {
                                    
                    AR_DEBUG_PRINTF(ATH_DEBUG_RECV, 
                                (" LookAhead Report Found (pre valid:0x%X, post valid:0x%X) \n", 
                                pLookAhead->PreValid,
                                pLookAhead->PostValid));  
                                
                        /* look ahead bytes are valid, copy them over */
                    ((A_UINT8 *)pNextLookAhead)[0] = pLookAhead->LookAhead[0];
                    ((A_UINT8 *)pNextLookAhead)[1] = pLookAhead->LookAhead[1];
                    ((A_UINT8 *)pNextLookAhead)[2] = pLookAhead->LookAhead[2];
                    ((A_UINT8 *)pNextLookAhead)[3] = pLookAhead->LookAhead[3];
                    
                }    
                break;   
            default:
                AR_DEBUG_PRINTF(ATH_DEBUG_ERR, (" unhandled record: id:%d length:%d \n", 
                        pRecord->RecordID, pRecord->Length));
                break;
        }
        
        if (A_FAILED(status)) {
            break;    
        }
        
            /* advance buffer past this record for next time around */
        pBuffer += pRecord->Length;
        Length -= pRecord->Length;
    }

    if (A_FAILED(status)) {
        DebugDumpBytes(pOrigBuffer,origLength,"BAD Recv Trailer");    
    }
        
    AR_DEBUG_PRINTF(ATH_DEBUG_RECV, ("-HTCProcessTrailer \n"));
    return status;
    
}

/* process a received message (i.e. strip off header, process any trailer data) 
 * note : locks must be released when this function is called */
static A_STATUS HTCProcessRecvHeader(HTC_TARGET      *target, 
                                     A_UINT8         *pBuffer,
                                     int             Length,
                                     int             *pPayloadLength)
{
    A_UINT8         temp;
    A_UINT8         *pBuf;
    A_STATUS        status = A_OK;
    A_UINT16        payloadLen;
    HTC_ENDPOINT_ID endpoint;
    
    pBuf = pBuffer;        
    
    AR_DEBUG_PRINTF(ATH_DEBUG_RECV, ("+HTCProcessRecvHeader \n"));  
         
    do {
        /* note, we cannot assume the alignment of pBuffer, so we use the safe macros to 
         * retrieve 16 bit fields */        
        payloadLen = A_GET_UINT16_FIELD(pBuf, HTC_FRAME_HDR, PayloadLen); 
   
#if 0        
     
        ((A_UINT8 *)&lookAhead)[0] = pBuf[0];
        ((A_UINT8 *)&lookAhead)[1] = pBuf[1];
        ((A_UINT8 *)&lookAhead)[2] = pBuf[2];
        ((A_UINT8 *)&lookAhead)[3] = pBuf[3];
        
        if (lookAhead != pPacket->HTCReserved) {
            /* somehow the lookahead that gave us the full read length did not
             * reflect the actual header in the pending message */
             AR_DEBUG_PRINTF(ATH_DEBUG_ERR, 
                    ("HTCProcessRecvHeader, lookahead mismatch! \n")); 
             DebugDumpBytes((A_UINT8 *)&pPacket->HTCReserved,4,"Expected Message LookAhead");
             DebugDumpBytes(pBuf,sizeof(HTC_FRAME_HDR),"Current Frame Header");    
            status = A_EPROTO;
            break;            
        }
#endif
            /* get endpoint ID */
            
        endpoint = A_GET_UINT8_FIELD(pBuf, HTC_FRAME_HDR, EndpointID);
            /* get flags */
        temp = A_GET_UINT8_FIELD(pBuf, HTC_FRAME_HDR, Flags);
        
        if (temp & HTC_FLAGS_RECV_TRAILER) {          
            /* this packet has a trailer */
                   
                /* extract the trailer length in control byte 0 */
            temp = A_GET_UINT8_FIELD(pBuf, HTC_FRAME_HDR, ControlBytes[0]);
            
            if ((temp < sizeof(HTC_RECORD_HDR)) || (temp > payloadLen)) {
                AR_DEBUG_PRINTF(ATH_DEBUG_ERR, 
                    ("HTCProcessRecvHeader, invalid header (payloadlength should be :%d, CB[0] is:%d) \n",
                        payloadLen, temp));
                status = A_EPROTO;
                break;    
            }  
           
                /* process trailer data that follows HDR + application payload */
            status = HTCProcessTrailer(target, 
                                       (pBuf + HTC_HDR_LENGTH + payloadLen - temp), 
                                       temp, 
                                       NULL,
                                       endpoint);
           
            if (A_FAILED(status)) {
                break;    
            }
                   
                /* trim length by trailer bytes */
            Length -= temp; 
        }
        
            /* if we get to this point, the packet is good */
            /* remove header and adjust length */
        Length -= HTC_HDR_LENGTH;
             
        *pPayloadLength = Length;
    
    } while (FALSE);
    
    if (A_FAILED(status)) {
            /* dump the whole packet */
        DebugDumpBytes(pBuf,Length,"BAD HTC Recv PKT");    
    } 
    
    AR_DEBUG_PRINTF(ATH_DEBUG_RECV, ("-HTCProcessRecvHeader \n"));
    return status;             
}

static A_STATUS DevRecvPacket(HTC_TARGET    *target,
                              A_UINT8       *pBuffer, 
                              A_UINT32      RecvLength, 
                              A_UINT32      MaxLength) 
{
    A_UINT32 paddedLength;
    
        /* adjust the length to be a multiple of block size if appropriate */
    paddedLength = (RecvLength + (target->HifBlockMask)) &
                    (~(target->HifBlockMask));   
                        
    if (paddedLength > MaxLength) {
        AR_DEBUG_ASSERT(FALSE);
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, 
                ("DevRecvPacket, Not enough space for padlen:%d recvlen:%d bufferlen:%d \n", 
                    paddedLength,RecvLength,MaxLength));   
        return A_EINVAL;    
    }
                    
    AR_DEBUG_PRINTF(ATH_DEBUG_RECV, 
                    ("DevRecvPacket, Padded Length: %d Mbox:0x%X \n", 
                    paddedLength, 
                    target->HifMailboxAddress));         

    return HIFReadWrite(target->HifDevice, 
                        target->HifMailboxAddress, 
                        pBuffer,  
                        paddedLength,
                        HIF_RD_SYNC_BLOCK_INC,              
                        NULL);  
}

#define DELAY_PER_INTERVAL_MS   1

static A_STATUS DevServiceDebugInterrupt(HTC_TARGET  *target)
{
    A_UINT32 dummy;
    A_STATUS status;

    /* Send a target failure event to the application */
    AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Target debug interrupt\n"));

    /*** TODO .. handle TARGET assertion */

    /* clear the interrupt , the debug error interrupt is
     * counter 0 */
        /* read counter to clear interrupt */
    status = HIFReadWrite(target->HifDevice,
                          COUNT_DEC_ADDRESS,
                          (A_UINT8 *)&dummy,
                          4,
                          HIF_RD_SYNC_BYTE_INC,
                          NULL);

    AR_DEBUG_ASSERT(status == A_OK);
    return status;
}

/* mailbox recv message polling */
A_STATUS DevPollMboxMsgRecv(HTC_TARGET  *target,
                            A_UINT32    *pLookAhead,
                            int         TimeoutMS)
{
    A_STATUS status = A_OK;
    int      timeout = TimeoutMS/DELAY_PER_INTERVAL_MS;
    A_BOOL   noWait = FALSE;
    
    if (timeout == 0) {
        noWait = TRUE;    
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_RECV,("+DevPollMboxMsgRecv \n"));

    while (TRUE) {

            /* this is the standard HIF way.... */
            /* load the register table */
        status = HIFReadWrite(target->HifDevice,
                              HOST_INT_STATUS_ADDRESS,
                              (A_UINT8 *)&target->IrqProcRegisters,
                              AR6K_IRQ_PROC_REGS_SIZE,
                              HIF_RD_SYNC_BYTE_INC,
                              NULL);

        if (A_FAILED(status))
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Failed to read register table \n"));
            break;
        }

            /* check for MBOX data and valid lookahead */
        if (target->IrqProcRegisters.host_int_status & (1 << HTC_MAILBOX))
        {
            if (target->IrqProcRegisters.rx_lookahead_valid & (1 << HTC_MAILBOX))
            {
                /* mailbox has a message and the look ahead is valid */
                *pLookAhead = target->IrqProcRegisters.rx_lookahead[HTC_MAILBOX];
                break;
            }
        }

        if (noWait) {
            *pLookAhead = 0; 
            break;   
        }
        
        timeout--;

        if (timeout <= 0)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, (" Timeout waiting for recv message \n"));
            status = A_ERROR;
              
                /* check if the target asserted */
            if (target->IrqProcRegisters.counter_int_status & AR6K_TARGET_DEBUG_INTR_MASK) {
                    /* target signaled an assert, process this pending interrupt 
                     * this will call the target failure handler */
                DevServiceDebugInterrupt(target);
            }
            
            break;
        }

            /* delay a little  */
         A_MDELAY(DELAY_PER_INTERVAL_MS);
         AR_DEBUG_PRINTF(ATH_DEBUG_RECV,("  Retry Mbox Poll : %d \n",timeout));
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_RECV,("-DevPollMboxMsgRecv \n"));

    return status;
}

/* synchronously wait for a message using polling */
A_STATUS HTCGetMessage(HTC_HANDLE      HTCHandle,                
                       HTC_ENDPOINT_ID *pEndpoint, 
                       A_UINT8         *pBuffer, 
                       int             MaxLength,
                       int             *pActualLength,
                       int             TimeoutMS)
{
    A_STATUS        status;
    A_UINT32        lookAhead;
    HTC_FRAME_HDR   *pHdr;
    A_UINT8         *pHtcMsg;
    
    AR_DEBUG_PRINTF(ATH_DEBUG_RECV,("+HTCGetMessage \n"));
                
    while (1)  { 
        
            /* call the polling function to see if we have a message */
        status = DevPollMboxMsgRecv(&g_HTCTarget, &lookAhead, TimeoutMS);
                                       
        if (A_FAILED(status)) {
            break;    
        }
        
        if ((TimeoutMS == 0) && (lookAhead == 0)) {
            /* this is actually success, caller just wanted to check for a message */
            *pActualLength = 0;
            *pEndpoint = ENDPOINT_MAX;
            break;    
        }            
           
            /* check the lookahead */
        pHdr = (HTC_FRAME_HDR *)&lookAhead;
                
        if (pHdr->EndpointID >= ENDPOINT_MAX) {
            A_ASSERT(FALSE);
            status = A_EPROTO;
            break;    
        }
        
        if ((int)(pHdr->PayloadLen + HTC_HDR_LENGTH) > MaxLength) {
            A_ASSERT(FALSE);
            status = A_EPROTO;
            break;           
        }
        
            /* caller ALWAYS supplies head room */
        pHtcMsg = pBuffer - HTC_HDR_LENGTH;
         
            /* get the message from the device, this will block */
        status = DevRecvPacket(&g_HTCTarget, 
                               pHtcMsg, 
                               pHdr->PayloadLen + HTC_HDR_LENGTH, 
                               MaxLength);
        
        if (A_FAILED(status)) {
            break;    
        }
                                                  
            /* process receive header */     
        status = HTCProcessRecvHeader(&g_HTCTarget,
                                      pHtcMsg,
                                      pHdr->PayloadLen + HTC_HDR_LENGTH,
                                      pActualLength);
       
        if (A_FAILED(status)) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR, 
                    ("HTCProcessRecvHeader failed (status = %d) \n", 
                     status));
            break;
        }
        
        if (*pActualLength == 0) {
            /* this was a credit message and we processed it already, go back and wait for
             * another message */
             continue;    
            
        }
        
        *pEndpoint = pHdr->EndpointID;
     
        break;
                
    }
        
    AR_DEBUG_PRINTF(ATH_DEBUG_RECV,("-HTCGetMessage \n"));
    
    return status;
}


/* internal function to send an HTC packet to the target */
A_STATUS HTCIssueMsg(HTC_HANDLE      HTCHandle, 
                     HTC_ENDPOINT_ID Endpoint, 
                     A_UINT8         *pBuffer, 
                     int             Length)
{
    A_STATUS status;
    A_UINT8 *pHdrBuf;
    A_UINT32 paddedLength;
    A_UINT32 totalLength;
  
    AR_DEBUG_PRINTF(ATH_DEBUG_SEND,("-HTCIssueMsg \n"));
    
        /* caller always provides headrooom */
    pBuffer -= HTC_HDR_LENGTH;
    pHdrBuf = pBuffer;
    totalLength = Length + HTC_HDR_LENGTH;
    
        /* setup frame header */
    A_SET_UINT16_FIELD(pHdrBuf,HTC_FRAME_HDR,PayloadLen,(A_UINT16)Length);
        /* to simplify things, we always set the host needs credits update, this way we get credits 
         * back quickly */
    A_SET_UINT8_FIELD(pHdrBuf,HTC_FRAME_HDR,Flags,HTC_FLAGS_NEED_CREDIT_UPDATE);
    A_SET_UINT8_FIELD(pHdrBuf,HTC_FRAME_HDR,EndpointID,(A_UINT8)Endpoint);
     
       /* adjust the length to be a multiple of block size if appropriate */
    paddedLength = (totalLength + (g_HTCTarget.HifBlockMask)) &
                    (~(g_HTCTarget.HifBlockMask));

    AR_DEBUG_PRINTF(ATH_DEBUG_SEND, 
                ("DevSendPacket, Padded Length: %d Mbox:0x%X \n", 
                paddedLength, 
                g_HTCTarget.HifMailboxAddress));          

    status = HIFReadWrite(g_HTCTarget.HifDevice, 
                          g_HTCTarget.HifMailboxAddress, 
                          pBuffer,  
                          paddedLength,     /* the padded length */ 
                          HIF_WR_SYNC_BLOCK_INC,
                          NULL);
    
    AR_DEBUG_PRINTF(ATH_DEBUG_SEND,("-HTCIssueMsg \n"));
    
    return status;
}

A_STATUS HTCSendMsg(HTC_HANDLE      HTCHandle, 
                    HTC_ENDPOINT_ID Endpoint, 
                    A_UINT8         *pBuffer, 
                    int             Length)
{
    int  creditsRequired;
    

#ifdef TODO_COMPILE_USE_SINGLE_CREDIT    
        /* TODO: compiler seems to link to special division library that causes a link error
         * in the boot loader, look at this later */    
    creditsRequired = 1;
#else    
    creditsRequired = (Length + HTC_HDR_LENGTH) / g_HTCTarget.TargetCreditSize;
   
    if ((Length + HTC_HDR_LENGTH) % g_HTCTarget.TargetCreditSize) {
        creditsRequired++;    
    }
#endif    
    
    if (creditsRequired > g_HTCTarget.CreditsAvailable) {
            /* we should have plenty of credits .. TODO... maybe we should wait? */
        A_ASSERT(FALSE);
        return A_EBUSY;    
    }
    
    g_HTCTarget.CreditsAvailable -= creditsRequired;
    
    return HTCIssueMsg(HTCHandle, 
                       Endpoint, 
                       pBuffer, 
                       Length);                     
}   




