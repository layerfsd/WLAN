/*------------------------------------------------------------------------------ */
/* <copyright file="htc_send.c" company="Atheros"> */
/*    Copyright (c) 2007-2008 Atheros Corporation.  All rights reserved. */
/*  */
/* This program is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License version 2 as */
/* published by the Free Software Foundation; */
/* */
/* Software distributed under the License is distributed on an "AS */
/* IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or */
/* implied. See the License for the specific language governing */
/* rights and limitations under the License. */
/* */
/* */
/*------------------------------------------------------------------------------ */
/*============================================================================== */
/* Author(s): ="Atheros" */
/*============================================================================== */
#include "htc_internal.h"

#define _P	L"\r[AR6K : HtcSend] "L


enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0,
	DBG_DUMP	=0
}HTC_SendDebug;


typedef enum _HTC_SEND_QUEUE_RESULT {
    HTC_SEND_QUEUE_OK = 0,    /* packet was queued */
    HTC_SEND_QUEUE_DROP = 1,  /* this packet should be dropped */
} HTC_SEND_QUEUE_RESULT;

#define DO_EP_TX_COMPLETION(ep,p)                                    \
{                                                                    \
    (p)->Completion = NULL;                                          \
    (ep)->EpCallBacks.EpTxComplete((ep)->EpCallBacks.pContext,(p));  \
}


/* call the distribute credits callback with the distribution */
#define DO_DISTRIBUTION(t,reason,description,pList) \
{                                             \
    NDIS_DEBUG_PRINTF(0,           \
        " calling distribute function (%s) (dfn:0x%X, ctxt:0x%X, dist:0x%X) \n", \
                (description),                                           \
                (A_UINT32)(t)->DistributeCredits,                        \
                (A_UINT32)(t)->pCredDistContext,                         \
                (A_UINT32)pList);                                       \
    (t)->DistributeCredits((t)->pCredDistContext,                        \
                           (pList),                                      \
                           (reason));                                    \
}

/* our internal send packet completion handler when packets are submited to the AR6K device
 * layer */
static void HTCSendPktCompletionHandler(void *Context, HTC_PACKET *pPacket)
{
    HTC_TARGET      *target = (HTC_TARGET *)Context;
    HTC_ENDPOINT    *pEndpoint = &target->EndPoint[pPacket->Endpoint];
	
	
    if (A_FAILED(pPacket->Status)) 
	{
        NDIS_DEBUG_PRINTF(DBG_ERR,"HTCSendPktCompletionHandler: request failed (status:%d, ep:%d), address = 0x%08x \n",
                pPacket->Status, pPacket->Endpoint,(A_UINT32)(pPacket->pBuffer - HTC_HDR_LENGTH));
		
    }
        /* first, fixup the head room we allocated */
    pPacket->pBuffer += HTC_HDR_LENGTH;
        /* do completion */
    DO_EP_TX_COMPLETION(pEndpoint,pPacket);
	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s + Exit \r\n", __FUNCTION__);
}


extern void CheckNDISPktAddress(void *cookie);


A_STATUS HTCIssueSend(HTC_TARGET *target, HTC_PACKET *pPacket, A_UINT8 SendFlags)
{
    A_STATUS status;
    A_UINT8 *pHdrBuf;
    A_BOOL   sync = FALSE;

        /* caller always provides headrooom */
    pPacket->pBuffer -= HTC_HDR_LENGTH;
    pHdrBuf = pPacket->pBuffer;
        /* setup frame header */
    A_SET_UINT16_FIELD(pHdrBuf,HTC_FRAME_HDR,PayloadLen,(A_UINT16)pPacket->ActualLength);
    A_SET_UINT8_FIELD(pHdrBuf,HTC_FRAME_HDR,Flags,SendFlags);
    A_SET_UINT8_FIELD(pHdrBuf,HTC_FRAME_HDR,EndpointID, (A_UINT8)pPacket->Endpoint);

    if (pPacket->Completion == NULL) {
            /* mark that this request was synchronously issued */
        sync = TRUE;
    }

    NDIS_DEBUG_PRINTF(DBG_TRACE,
                    "+-HTCIssueSend: transmit length : %d (%s) \r\n",
                    pPacket->ActualLength + HTC_HDR_LENGTH,
                    sync ? "SYNC" : "ASYNC" );

	/* send message to device */
    status = DevSendPacket(&target->Device,
                           pPacket,
                           pPacket->ActualLength + HTC_HDR_LENGTH);

    if (sync) {
            /* use local sync variable.  If this was issued asynchronously, pPacket is no longer
             * safe to access. */
        pPacket->pBuffer += HTC_HDR_LENGTH;
    }

    /* if this request was asynchronous, the packet completion routine will be invoked by
     * the device layer when the HIF layer completes the request */

    return status;
}


/* try to send the current packet or a packet at the head of the TX queue,
 * if there are no credits, the packet remains in the queue.
 * this function returns the result of the attempt to send the HTC packet */
static HTC_SEND_QUEUE_RESULT HTCTrySend(HTC_TARGET   *target,
                                        HTC_ENDPOINT *pEndpoint,
                                        HTC_PACKET   *pPacketToSend)
{
	HTC_PACKET  *pPacket;
	int         creditsRequired;
	int         remainder;
	A_UINT8     sendFlags;
	HTC_SEND_QUEUE_RESULT result;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "+HTCTrySend (pPkt:0x%X) \r\n",(A_UINT32)pPacketToSend);

	if (pPacketToSend != NULL) 
	{
		/* see if adding this packet hits the max depth */
		NDIS_DEBUG_PRINTF(DBG_LEVEL_WMI_HTC, " CurrentTxQueueDepth = %d, MaxTxQueueDepth = %d \r\n",
								pEndpoint->CurrentTxQueueDepth,pEndpoint->MaxTxQueueDepth);
		
		if ((pEndpoint->CurrentTxQueueDepth + 1) >= pEndpoint->MaxTxQueueDepth) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, " Endpoint %d, TX queue is full, Depth:%d, Max:%d \r\n",
										pPacketToSend->Endpoint, pEndpoint->CurrentTxQueueDepth, pEndpoint->MaxTxQueueDepth);
			/* queue will be full, invoke any callbacks to determine what action to take */
			if (pEndpoint->EpCallBacks.EpSendFull != NULL) 
			{
				NDIS_DEBUG_PRINTF(DBG_LEVEL_WMI_HTC, " Calling driver's send full callback.... \r\n");
				if (pEndpoint->EpCallBacks.EpSendFull(pEndpoint->EpCallBacks.pContext,
	                                                  pPacketToSend) == HTC_SEND_FULL_DROP) 
				{
					/* callback wants the packet dropped */
					INC_HTC_EP_STAT(pEndpoint, TxDropped, 1);
					NDIS_DEBUG_PRINTF(DBG_ERR, "-HTCTrySend:  \r\n");
					return HTC_SEND_QUEUE_DROP;
				}
			}
		}
	}

	LOCK_HTC_TX(target);	//mutex lock

	result = HTC_SEND_QUEUE_OK;
		
	/* insert packet to the queue */
	if (pPacketToSend != NULL) 
	{
		/* packet was supplied to be queued */
		HTC_PACKET_ENQUEUE(&pEndpoint->TxQueue,pPacketToSend);
		pEndpoint->CurrentTxQueueDepth++;
	}

	pEndpoint->TxProcessCount++;    
	
	if (pEndpoint->TxProcessCount > 1) 
	{
		/* another thread is draining the queues, get out */
		pEndpoint->TxProcessCount--;     
		UNLOCK_HTC_TX(target);
		NDIS_DEBUG_PRINTF(DBG_TRACE, "-HTCTrySend:  \r\n");
		return HTC_SEND_QUEUE_OK;        
	}

	/* at this point, only 1 thread may enter to drain this endpoint queue */
	   
	/* now drain the TX queue for transmission as long as we have enough
	* credits */

	while (1) 
	{

		if (HTC_QUEUE_EMPTY(&pEndpoint->TxQueue)) 
		{
			/* nothing in the queue */
			break;
		}

		if (HTC_STOPPING(target)) 
		{
			if (pPacketToSend != NULL) 
			{
				HTC_PACKET_REMOVE(pPacketToSend);
				pEndpoint->CurrentTxQueueDepth--;
				result = HTC_SEND_QUEUE_DROP;
			}
			break;
		}

		sendFlags = 0;

		/* get packet at head, but don't remove it */
		pPacket = HTC_GET_PKT_AT_HEAD(&pEndpoint->TxQueue);
		NDIS_DEBUG_PRINTF(0, " Got head packet:0x%X , Queue Depth: %d \r\n",
									(A_UINT32)pPacket, pEndpoint->CurrentTxQueueDepth);

		/* figure out how many credits this message requires */
		creditsRequired = (pPacket->ActualLength + HTC_HDR_LENGTH) / target->TargetCreditSize;
		remainder = (pPacket->ActualLength + HTC_HDR_LENGTH) % target->TargetCreditSize;

		if (remainder) 
		{
			creditsRequired++;
		}

        NDIS_DEBUG_PRINTF(0, " Creds Required:%d   Got:%d \r\n",
								creditsRequired, pEndpoint->CreditDist.TxCredits);

		if (pEndpoint->CreditDist.TxCredits < creditsRequired) 
		{

			/* not enough credits */

			if (pPacket->Endpoint == ENDPOINT_0) 
			{
				/* leave it in the queue */
				break;
            }
			
			/* invoke the registered distribution function only if this is not
			* endpoint 0, we let the driver layer provide more credits if it can.
			* We pass the credit distribution list starting at the endpoint in question
			* */

			/* set how many credits we need  */
			pEndpoint->CreditDist.TxCreditsSeek = creditsRequired - pEndpoint->CreditDist.TxCredits;
			
			DO_DISTRIBUTION(target,
							HTC_CREDIT_DIST_SEEK_CREDITS,
							"Seek Credits",
							&pEndpoint->CreditDist);
			pEndpoint->CreditDist.TxCreditsSeek = 0;

			if (pEndpoint->CreditDist.TxCredits < creditsRequired) 
			{
				/* still not enough credits to send, leave packet in the queue */
				NDIS_DEBUG_PRINTF(DBG_TRACE, " Not enough credits for ep %d leaving packet in queue..\r\n", pPacket->Endpoint);
				break;
			}

		}

		pEndpoint->CreditDist.TxCredits -= creditsRequired;
		INC_HTC_EP_STAT(pEndpoint, TxCreditsConsummed, creditsRequired);

		/* check if we need credits back from the target */
		if (pEndpoint->CreditDist.TxCredits < pEndpoint->CreditDist.TxCreditsPerMaxMsg) 
		{
			/* we are getting low on credits, see if we can ask for more from the distribution function */
			pEndpoint->CreditDist.TxCreditsSeek =
						pEndpoint->CreditDist.TxCreditsPerMaxMsg - pEndpoint->CreditDist.TxCredits;

			DO_DISTRIBUTION(target, HTC_CREDIT_DIST_SEEK_CREDITS, "Seek Credits", &pEndpoint->CreditDist);

			pEndpoint->CreditDist.TxCreditsSeek = 0;
			
			/* see if we were successful in getting more */
			if (pEndpoint->CreditDist.TxCredits < pEndpoint->CreditDist.TxCreditsPerMaxMsg) 
			{
				/* tell the target we need credits ASAP! */
				sendFlags |= HTC_FLAGS_NEED_CREDIT_UPDATE;
				INC_HTC_EP_STAT(pEndpoint, TxCreditLowIndications, 1);
				NDIS_DEBUG_PRINTF(DBG_TRACE, " Host Needs Credits  \r\n");
			}
		}

		/* now we can fully dequeue */
		pPacket = HTC_PACKET_DEQUEUE(&pEndpoint->TxQueue);
		pEndpoint->CurrentTxQueueDepth--;

		INC_HTC_EP_STAT(pEndpoint, TxIssued, 1);

		UNLOCK_HTC_TX(target);		
		
		HTCIssueSend(target, pPacket, sendFlags);

		LOCK_HTC_TX(target);

		/* go back and check for more messages */
	}
    
        /* we're done, clear count */
	pEndpoint->TxProcessCount = 0;
	UNLOCK_HTC_TX(target);

	NDIS_DEBUG_PRINTF(0, "-HTCTrySend:  result = %d\n" , result);

	return result;
}


/* HTC API - HTCSendPkt */
A_STATUS HTCSendPkt(HTC_HANDLE HTCHandle, HTC_PACKET *pPacket)
{
	HTC_TARGET      *target = GET_HTC_TARGET_FROM_HANDLE(HTCHandle);
	HTC_ENDPOINT    *pEndpoint;
	HTC_ENDPOINT_ID ep;
	A_STATUS        status = A_OK;
	HTC_SEND_QUEUE_RESULT result;
	
		
	NDIS_DEBUG_PRINTF(0, "+HTCSendPkt: Enter endPointId: %d, buffer: 0x%X, length: %d \n",
				pPacket->Endpoint, (A_UINT32)pPacket->pBuffer, pPacket->ActualLength);

	ep = pPacket->Endpoint;
	AR_DEBUG_ASSERT(ep < ENDPOINT_MAX);
	pEndpoint = &target->EndPoint[ep];

	do 
	{

		/* everything sent through this interface is asynchronous */
		/* fill in HTC completion routines */
		pPacket->Completion = HTCSendPktCompletionHandler;
		pPacket->pContext = target;

		result = HTCTrySend(target, pEndpoint, pPacket);

		NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() : pPacket = 0x%08x, Completion = 0x%08x \r\n",__FUNCTION__,pPacket, pPacket->Completion);
		if (HTC_SEND_QUEUE_DROP == result) 
		{
			NDIS_DEBUG_PRINTF(DBG_ERR, " Endpoint %d, TX packet dropped \n", ep);
			if (HTC_STOPPING(target)) {
				status = A_ECANCELED;
			} else {
				status = A_NO_RESOURCE;
			}
		}

    } while (FALSE);

	if (A_FAILED(status)) {
		
		NDIS_DEBUG_PRINTF(DBG_ERR, "-HTCSendPkt Fail !! \n");
		
		pPacket->Status = status;
		DO_EP_TX_COMPLETION(pEndpoint,pPacket);
	}

	NDIS_DEBUG_PRINTF(DBG_TRACE, "-HTCSendPkt \n");

	return status;
}


/* check TX queues to drain because of credit distribution update */
static __inline void HTCCheckEndpointTxQueues(HTC_TARGET *target)
{
    HTC_ENDPOINT                *pEndpoint;
    HTC_ENDPOINT_CREDIT_DIST    *pDistItem;

    NDIS_DEBUG_PRINTF(DBG_TRACE, "+HTCCheckEndpointTxQueues \r\n");
    pDistItem = target->EpCreditDistributionListHead;

        /* run through the credit distribution list to see
         * if there are packets queued
         * NOTE: no locks need to be taken since the distribution list
         * is not dynamic (cannot be re-ordered) and we are not modifying any state */
    while (pDistItem != NULL) {
        pEndpoint = (HTC_ENDPOINT *)pDistItem->pHTCReserved;

        if (pEndpoint->CurrentTxQueueDepth > 0) {
            NDIS_DEBUG_PRINTF(DBG_TRACE, " Ep %d has %d credits and %d Packets in TX Queue \r\n",
                    pDistItem->Endpoint, pEndpoint->CreditDist.TxCredits, pEndpoint->CurrentTxQueueDepth);
			/* try to start the stalled queue, this list is ordered by priority.
                 * Highest priority queue get's processed first, if there are credits available the
                 * highest priority queue will get a chance to reclaim credits from lower priority
                 * ones */
            HTCTrySend(target, pEndpoint, NULL);
        }

        pDistItem = pDistItem->pNext;
    }

    NDIS_DEBUG_PRINTF(DBG_TRACE, "-HTCCheckEndpointTxQueues \r\n");
}

/* process credit reports and call distribution function */
void HTCProcessCreditRpt(HTC_TARGET *target, HTC_CREDIT_REPORT *pRpt, int NumEntries, HTC_ENDPOINT_ID FromEndpoint)
{
    int             i;
    HTC_ENDPOINT    *pEndpoint;
    int             totalCredits = 0;
    A_BOOL          doDist = FALSE;

    NDIS_DEBUG_PRINTF(DBG_TRACE, "+HTCProcessCreditRpt, Credit Report Entries:%d \r\n", NumEntries);

        /* lock out TX while we update credits */
    LOCK_HTC_TX(target);

    for (i = 0; i < NumEntries; i++, pRpt++) {
        if (pRpt->EndpointID >= ENDPOINT_MAX) {
            AR_DEBUG_ASSERT(FALSE);
            break;
        }

        pEndpoint = &target->EndPoint[pRpt->EndpointID];

        NDIS_DEBUG_PRINTF(DBG_TRACE, " Endpoint %d got %d credits \r\n", pRpt->EndpointID, pRpt->Credits);


#ifdef HTC_EP_STAT_PROFILING

        INC_HTC_EP_STAT(pEndpoint, TxCreditRpts, 1);
        INC_HTC_EP_STAT(pEndpoint, TxCreditsReturned, pRpt->Credits);

        if (FromEndpoint == pRpt->EndpointID) {
                /* this credit report arrived on the same endpoint indicating it arrived in an RX
                 * packet */
            INC_HTC_EP_STAT(pEndpoint, TxCreditsFromRx, pRpt->Credits);
            INC_HTC_EP_STAT(pEndpoint, TxCreditRptsFromRx, 1);
        } else if (FromEndpoint == ENDPOINT_0) {
                /* this credit arrived on endpoint 0 as a NULL message */
            INC_HTC_EP_STAT(pEndpoint, TxCreditsFromEp0, pRpt->Credits);
            INC_HTC_EP_STAT(pEndpoint, TxCreditRptsFromEp0, 1);
        } else {
                /* arrived on another endpoint */
            INC_HTC_EP_STAT(pEndpoint, TxCreditsFromOther, pRpt->Credits);
            INC_HTC_EP_STAT(pEndpoint, TxCreditRptsFromOther, 1);
        }

#endif

        if (ENDPOINT_0 == pRpt->EndpointID) {
                /* always give endpoint 0 credits back */
            pEndpoint->CreditDist.TxCredits += pRpt->Credits;
        } else {
                /* for all other endpoints, update credits to distribute, the distribution function
                 * will handle giving out credits back to the endpoints */
            pEndpoint->CreditDist.TxCreditsToDist += pRpt->Credits;
                /* flag that we have to do the distribution */
            doDist = TRUE;
        }
        
            /* refresh tx depth for distribution function that will recover these credits
             * NOTE: this is only valid when there are credits to recover! */
        pEndpoint->CreditDist.TxQueueDepth = pEndpoint->CurrentTxQueueDepth;
        
        totalCredits += pRpt->Credits;
    }

    NDIS_DEBUG_PRINTF(DBG_TRACE, " Report indicated %d credits to distribute \r\n", totalCredits);

    if (doDist) {
            /* this was a credit return based on a completed send operations
             * note, this is done with the lock held */
        DO_DISTRIBUTION(target,
                        HTC_CREDIT_DIST_SEND_COMPLETE,
                        L"Send Complete",
                        target->EpCreditDistributionListHead->pNext);
    }

    UNLOCK_HTC_TX(target);

    if (totalCredits) {
        HTCCheckEndpointTxQueues(target);
    }

    NDIS_DEBUG_PRINTF(DBG_TRACE, "-HTCProcessCreditRpt \n");
}

/* flush endpoint TX queue */
static void HTCFlushEndpointTX(HTC_TARGET *target, HTC_ENDPOINT *pEndpoint, HTC_TX_TAG Tag)
{
    HTC_PACKET          *pPacket;
    HTC_PACKET_QUEUE    discardQueue;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "%s() + Enter \r\n",__FUNCTION__);
        /* initialize the discard queue */
    INIT_HTC_PACKET_QUEUE(&discardQueue);

    LOCK_HTC_TX(target);

        /* interate from the front of the TX queue and flush out packets */
    ITERATE_OVER_LIST_ALLOW_REMOVE(&pEndpoint->TxQueue, pPacket, HTC_PACKET, ListLink) {

            /* check for removal */
        if ((HTC_TX_PACKET_TAG_ALL == Tag) || (Tag == pPacket->PktInfo.AsTx.Tag)) {
                /* remove from queue */
            HTC_PACKET_REMOVE(pPacket);
                /* add it to the discard pile */
            HTC_PACKET_ENQUEUE(&discardQueue, pPacket);
            pEndpoint->CurrentTxQueueDepth--;
        }

    } ITERATE_END;

    UNLOCK_HTC_TX(target);

	/* empty the discard queue */
    while (1) 
	{
        pPacket = HTC_PACKET_DEQUEUE(&discardQueue);
        if (NULL == pPacket) {
            break;
        }
        pPacket->Status = A_ECANCELED;
        NDIS_DEBUG_PRINTF(DBG_LEVEL_WMI_HTC, " Flushing TX packet:0x%X, length:%d, ep:%d tag:0x%X \r\n",
                (A_UINT32)pPacket, pPacket->ActualLength, pPacket->Endpoint, pPacket->PktInfo.AsTx.Tag);
        DO_EP_TX_COMPLETION(pEndpoint,pPacket);
    }

}

void DumpCreditDist(HTC_ENDPOINT_CREDIT_DIST *pEPDist)
{

	NDIS_DEBUG_PRINTF(DBG_DUMP,"--- EP : %d  ServiceID: 0x%X    --------------\n",
						pEPDist->Endpoint, pEPDist->ServiceID);
	NDIS_DEBUG_PRINTF(DBG_DUMP," this:0x%X next:0x%X prev:0x%X\n",
	            (A_UINT32)pEPDist, (A_UINT32)pEPDist->pNext, (A_UINT32)pEPDist->pPrev);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " DistFlags          : 0x%X \n", pEPDist->DistFlags);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCreditsNorm      : %d \n", pEPDist->TxCreditsNorm);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCreditsMin       : %d \n", pEPDist->TxCreditsMin);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCredits          : %d \n", pEPDist->TxCredits);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCreditsAssigned  : %d \n", pEPDist->TxCreditsAssigned);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCreditsSeek      : %d \n", pEPDist->TxCreditsSeek);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCreditSize       : %d \n", pEPDist->TxCreditSize);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCreditsPerMaxMsg : %d \n", pEPDist->TxCreditsPerMaxMsg);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxCreditsToDist    : %d \n", pEPDist->TxCreditsToDist);
	NDIS_DEBUG_PRINTF(DBG_DUMP, " TxQueueDepth       : %d \n", 
						((HTC_ENDPOINT *) pEPDist->pHTCReserved)->CurrentTxQueueDepth);
	NDIS_DEBUG_PRINTF(DBG_DUMP, "----------------------------------------------------\n");
}

void DumpCreditDistStates(HTC_TARGET *target)
{
    HTC_ENDPOINT_CREDIT_DIST *pEPList = target->EpCreditDistributionListHead;

    while (pEPList != NULL) {
        DumpCreditDist(pEPList);
        pEPList = pEPList->pNext;
    }

    if (target->DistributeCredits != NULL) {
        DO_DISTRIBUTION(target,
                        HTC_DUMP_CREDIT_STATE,
                        "Dump State",
                        NULL);
    }
}

/* flush all send packets from all endpoint queues */
void HTCFlushSendPkts(HTC_TARGET *target)
{
    HTC_ENDPOINT    *pEndpoint;
    int             i;

   // DumpCreditDistStates(target);

    for (i = ENDPOINT_0; i < ENDPOINT_MAX; i++) {
        pEndpoint = &target->EndPoint[i];
        if (pEndpoint->ServiceID == 0) {
                /* not in use.. */
            continue;
        }
        HTCFlushEndpointTX(target,pEndpoint,HTC_TX_PACKET_TAG_ALL);
    }


}

/* HTC API to flush an endpoint's TX queue*/
void HTCFlushEndpoint(HTC_HANDLE HTCHandle, HTC_ENDPOINT_ID Endpoint, HTC_TX_TAG Tag)
{
    HTC_TARGET      *target = GET_HTC_TARGET_FROM_HANDLE(HTCHandle);
    HTC_ENDPOINT    *pEndpoint = &target->EndPoint[Endpoint];

	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() Enter \r\n", __FUNCTION__);
    if (pEndpoint->ServiceID == 0) {
        AR_DEBUG_ASSERT(FALSE);
        /* not in use.. */
        return;
    }

    HTCFlushEndpointTX(target, pEndpoint, Tag);
}

/* HTC API to indicate activity to the credit distribution function */
void HTCIndicateActivityChange(HTC_HANDLE      HTCHandle,
                               HTC_ENDPOINT_ID Endpoint,
                               A_BOOL          Active)
{
	HTC_TARGET      *target = GET_HTC_TARGET_FROM_HANDLE(HTCHandle);
	HTC_ENDPOINT    *pEndpoint = &target->EndPoint[Endpoint];
	A_BOOL          doDist = FALSE;

	if (pEndpoint->ServiceID == 0) 
	{
		AR_DEBUG_ASSERT(FALSE);
		/* not in use.. */
		return;
	}

	LOCK_HTC_TX(target);

	if (Active) 
	{
		if (!(pEndpoint->CreditDist.DistFlags & HTC_EP_ACTIVE)) 
		{
			/* mark active now */
			pEndpoint->CreditDist.DistFlags |= HTC_EP_ACTIVE;
			doDist = TRUE;
	    }
	} 
	else 
	{
		if (pEndpoint->CreditDist.DistFlags & HTC_EP_ACTIVE) 
		{
			/* mark inactive now */
			pEndpoint->CreditDist.DistFlags &= ~HTC_EP_ACTIVE;
			doDist = TRUE;
		}
	}

	if (doDist) 
	{
		/* indicate current Tx Queue depth to the credit distribution function */
		pEndpoint->CreditDist.TxQueueDepth = pEndpoint->CurrentTxQueueDepth;
		/* do distribution again based on activity change
		* note, this is done with the lock held */
		DO_DISTRIBUTION(target,
						HTC_CREDIT_DIST_ACTIVITY_CHANGE,
						"Activity Change",
						target->EpCreditDistributionListHead->pNext);
	}

	UNLOCK_HTC_TX(target);

	if (doDist && !Active) 
	{
		/* if a stream went inactive and this resulted in a credit distribution change,
		* some credits may now be available for HTC packets that are stuck in
		* HTC queues */
		HTCCheckEndpointTxQueues(target);
	}
}

