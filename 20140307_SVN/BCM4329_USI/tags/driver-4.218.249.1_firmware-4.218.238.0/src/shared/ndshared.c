/*
 * NDIS-specific routines shared by several HWNBU NDIS device drivers.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: ndshared.c,v 1.66.6.1.2.1.4.5 2010/03/26 21:34:29 Exp $
 */

#include <typedefs.h>
#include <bcmdefs.h>
#include <bcmendian.h>
#include <osl.h>
#include <bcmutils.h>

#include <bldver.h>
extern void dhd_rx_flow(void *dhdp, bool enable);

/* reserve 1/8th of the tx free pool  free pool for use by the driver common code */
#define TXLB_FREEPOOL_THREHOLD(x)	((x) >> 3)

/* allocate one page of cached, shared (dma-able) or regular memory */
NDIS_STATUS
shared_allocpage(
	IN shared_info_t *shared,
	IN BOOLEAN shared_mem,
	IN BOOLEAN cached,
	OUT page_t *page
)
{
	NDIS_STATUS status;

	/* uncached not supported */
	ASSERT(cached);

	bzero(page, sizeof(page_t));
	if (shared_mem) {
			NdisMAllocateSharedMemory(shared->adapterhandle, PAGE_SIZE, cached,
			                          (void **) &page->va, &page->pa);

		/* Make sure that we got valid address */
		if (!osl_dmaddr_valid(shared->osh, page->pa.LowPart, page->pa.HighPart)) {
			ND_ERROR(("%s%d: shared_allocpage: pa not valid \n", shared->id,
			          shared->unit));
			NdisMFreeSharedMemory(shared->adapterhandle, PAGE_SIZE,
			                      cached, (PVOID)page->va, page->pa);
			return (NDIS_STATUS_RESOURCES);
		}
	} else
		page->va = MALLOC(shared->osh, PAGE_SIZE);
	if (page->va == NULL) {
		ND_ERROR(("%s%d: shared_allocpage: out of memory, malloced %d bytes\n", shared->id,
		          shared->unit, MALLOCED(shared->osh)));
		return (NDIS_STATUS_RESOURCES);
	}
	ASSERT(!shared_mem || ISALIGNED((uintptr)page->va, PAGE_SIZE));
	ASSERT(page->pa.HighPart == 0);
	ASSERT(ISALIGNED(page->pa.LowPart, PAGE_SIZE));
	status = NDIS_STATUS_SUCCESS;

	return (status);
}

NDIS_STATUS
shared_lb_alloc(
	IN shared_info_t *shared,
	IN struct lbfree *l,
	IN uint total,
	IN BOOLEAN shared_mem,
	IN BOOLEAN cached,
	IN BOOLEAN piomode,
	IN BOOLEAN data_buf
)
{
	NDIS_STATUS status;
	page_t page;
	int maxpages;
	int i;
	uint ipp, lbdatasz;

	/* uncached not supported */
	ASSERT(cached);

	if (data_buf)
		total = ROUNDUP(total, BPP);
	else
		/* add one if LBPP is not page aligned */
		total = ROUNDUP(total, (LBPP + ((PAGE_SIZE % sizeof(struct lbuf)) ? 1 : 0)));

	ND_TRACE(("%s%d: shared_lb_alloc: total %d\n", shared->id, shared->unit, total));

	l->free = NULL;
	l->total = total;
	l->count = 0;
	l->size = data_buf ? LBUFSZ : sizeof(struct lbuf);
	l->pages = NULL;
	l->npages = 0;
	l->headroom = 0;
	NdisAllocateSpinLock(&l->queue_lock);

	maxpages = (l->total * l->size) / PAGE_SIZE;

	/* allocate page list memory */
	if ((l->pages = (page_t*) MALLOC(shared->osh, maxpages * sizeof(page_t))) == NULL)
		goto enomem;
	bzero(l->pages, maxpages * sizeof(page_t));

	/* set item per page number and data size */
	if (data_buf) {
		ipp = BPP;
		lbdatasz = LBDATASZ;
	} else {
		ipp = LBPP;
		lbdatasz = 0;
	}

	/* fill the freelist */
	for (i = 0; i < maxpages; i++) {
		status = shared_allocpage(shared, shared_mem, cached, &page);
		if (NDIS_ERROR(status))
			goto enomem;
		status = shared_lb_addpage(shared, l, piomode, &page, ipp, lbdatasz);
		if (NDIS_ERROR(status))
			goto enomem;
	}

	return (NDIS_STATUS_SUCCESS);

enomem:
	ND_ERROR(("%s%d: shared_lb_alloc: out of memory, malloced %d bytes\n", shared->id,
	          shared->unit, MALLOCED(shared->osh)));
	shared_lb_free(shared, l, shared_mem, TRUE);
	return (NDIS_STATUS_RESOURCES);
}

void
shared_lb_free(
	IN shared_info_t *shared,
	IN struct lbfree *l,
	IN BOOLEAN shared_mem,
	IN BOOLEAN cached
)
{
	uint i;
	int maxpages;

	ND_TRACE(("%s%d: shared_lb_free\n", shared->id, shared->unit));

	ASSERT(l->count <= l->total);
	/* uncached not supported */
	ASSERT(cached);

	/* free all buffer memory pages */
	for (i = 0; i < l->npages; i++) {
		if (!shared_mem)
			MFREE(shared->osh, l->pages[i].va, PAGE_SIZE);
		else {
				NdisMFreeSharedMemory(shared->adapterhandle, PAGE_SIZE, cached,
				                      (PVOID)l->pages[i].va, l->pages[i].pa);
		}
	}
	/* free the page list */
	if (l->pages) {
		maxpages = (l->total * l->size) / PAGE_SIZE;
		MFREE(shared->osh, (uchar *) l->pages, maxpages * sizeof(page_t));
	}
	l->free = NULL;
	l->total = 0;
	l->count = 0;
	l->pages = NULL;
	l->npages = 0;
	NdisFreeSpinLock(&l->queue_lock);
}

/* add a page worth of packets to a freelist */
NDIS_STATUS
shared_lb_addpage(
	IN shared_info_t *shared,
	IN struct lbfree *l,
	IN BOOLEAN piomode,
	page_t *page,
	IN uint ipp,
	IN uint lbdatasz
)
{
	struct lbuf *lb;
	char *va;
	ULONG pa;
	uint i;

	ASSERT(l->npages < (ROUNDUP(l->total, ipp) / ipp));

	/* add it to the page list */
	l->pages[l->npages] = *page;
	l->npages++;

	/*
	 * Split each page into one or more LBUFSZ chunks,
	 * link them together, and put each on the freelist.
	 */

	va = page->va;
	pa = page->pa.LowPart;

	for (i = 0; i < ipp; i++) {
		char *vbuf = NULL;
		uint pbuf = 0;
		lb = (struct lbuf*)(va + lbdatasz);
		if (lbdatasz) {
			vbuf = va;
			pbuf = pa;
		}

		/* initialize lbuf fields */
		lb->link = lb->next = NULL;
		lb->head = lb->data = lb->tail = vbuf;
		lb->end = (uchar*)lb;
		lb->len = 0;
		lb->pa = piomode? 0xdeadbeef: pbuf;
		lb->p = NULL;
		lb->l = l;

		/* put it on the freelist */
		shared_lb_put(shared, l, lb);

		if (lbdatasz) {
			va += LBUFSZ;
			pa += LBUFSZ;
		} else
			va += sizeof(struct lbuf);
	}

	return (NDIS_STATUS_SUCCESS);
}

int
shared_lb_freecount(
	IN struct lbfree *l
)
{
		return l->count;
}

struct lbuf *
shared_lb_get(
	IN shared_info_t *shared,
	IN struct lbfree *l
)
{
	struct lbuf	*lb;

	NdisAcquireSpinLock(&l->queue_lock);
	ASSERT(l->count <= l->total);

	if (lb = l->free) {
		l->free = lb->link;
		lb->link = lb->next = NULL;
		lb->data = lb->tail = lb->head;
		lb->priority = 0;
		lb->len = 0;
		l->count--;
		bzero(lb->pkttag, OSL_PKTTAG_SZ);
	}
	if ((uint)shared_lb_freecount(&shared->rxfree) < shared->rxflowLow) {
		if (shared->rxflowMode ==  RX_FLOW_CONTROL_WATERMARK && shared->rxflow == FALSE) {
			shared->rxflow = TRUE;
			ND_TRACE(("%s: rx flow ON, free count = %d\n", __FUNCTION__,
				shared_lb_freecount(&shared->rxfree)));
			dhd_rx_flow(shared->dhd, TRUE);
		}
	}
	NdisReleaseSpinLock(&l->queue_lock);
	return (lb);
}

void
shared_lb_put(
	IN shared_info_t *shared,
	IN struct lbfree *l,
	IN struct lbuf *lb
)
{
	NdisAcquireSpinLock(&l->queue_lock);
	ASSERT(l->count <= l->total);
	ASSERT(lb->p == NULL);
	ASSERT(lb->link == NULL);
	ASSERT(lb->next == NULL);
	ASSERT(lb->l == l);

	lb->data = lb->tail = (uchar*)(uintptr)0xdeadbeef;
	lb->len = 0;
	lb->link = l->free;
	l->free = lb;
	l->count++;
	if ((uint)shared_lb_freecount(&shared->rxfree) > shared->rxflowHi) {
		if (shared->rxflowMode == RX_FLOW_CONTROL_WATERMARK && shared->rxflow == TRUE) {
			shared->rxflow = FALSE;
			ND_TRACE(("%s: rx flow OFF, free count = %d\n", __FUNCTION__,
				shared_lb_freecount(&shared->rxfree)));
			dhd_rx_flow(shared->dhd, FALSE);
		}
	}
	NdisReleaseSpinLock(&l->queue_lock);
}

NDIS_STATUS
shared_flush(
	IN shared_info_t *shared,
	IN uchar *va,
	IN ULONG pa,
	IN ULONG len,
	IN BOOLEAN writetodevice
)
{
#ifndef NDIS60
	PNDIS_BUFFER b;
	NDIS_STATUS status;
	NDIS_PHYSICAL_ADDRESS npa;

	/* if receive, buffer must begin and end on a cacheline boundary */
	if (!writetodevice) {
		ASSERT(ISALIGNED((uintptr)va, shared->cacheline));
		len = ROUNDUP(len, shared->cacheline);
	}

	/* alloc a temp buffer descriptor */
	NdisAllocateBuffer(&status, &b, shared->rxbufferpool, va, len);
	if (status != NDIS_STATUS_SUCCESS) {
		ND_ERROR(("%s%d: shared_flush: NdisAllocateBuffer error 0x%x\n",
			shared->id, shared->unit, status));
		return status;
	}

	/* flush processor cache */
	NdisAdjustBufferLength(b, len);
	NdisFlushBuffer(b, writetodevice);

	npa.HighPart = 0;
	npa.LowPart = pa;

#ifndef USEWDK


	if (!writetodevice)
		NdisMUpdateSharedMemory(shared->adapterhandle, len, va, npa);

#endif /* USEWDK */

	/* free the temp buffer descriptor */
	NdisFreeBuffer(b);
#else /* NDIS60 */
	PMDL b;

	/* if receive, buffer must begin and end on a cacheline boundary */
	if (!writetodevice) {
		ASSERT(ISALIGNED((uintptr)va, shared->cacheline));
		len = ROUNDUP(len, shared->cacheline);
	}

	/* alloc a temp MDL */
	b = NdisAllocateMdl(shared->adapterhandle, va, len);
	if (b == NULL) {
		ND_ERROR(("%s%d: shared_flush: NdisAllocateMdl error\n", shared->id, shared->unit));
		return NDIS_STATUS_FAILURE;
	}

	/* flush processor cache */
	NdisAdjustMdlLength(b, len);
	NdisFlushBuffer(b, writetodevice);

	/* free the temp MDL */
	NdisFreeMdl(b);
#endif /* NDIS60 */
	return NDIS_STATUS_SUCCESS;
}

void shared_queue_init(IN struct npq *q)
{
	q->tail = q->head = NULL;
	NdisAllocateSpinLock(&q->queue_lock);
}

void shared_queue_deinit(IN struct npq *q)
{
	ASSERT(q->tail == NULL);
	ASSERT(q->head == NULL);
	q->tail = q->head = NULL;
	NdisFreeSpinLock(&q->queue_lock);
}

void
shared_enq(
	IN struct npq *q,
	IN ND_PKT *p,
	bool lifo
)
{
	NdisAcquireSpinLock(&q->queue_lock);

	NEXTP(p) = NULL;

	if (q->tail == NULL) {
		ASSERT(q->head == NULL);
		q->head = q->tail = p;
	}
	else {
		ASSERT(q->head);
		ASSERT(NEXTP(q->tail) == NULL);
		if (lifo) {
			NEXTP(p) = q->head;
			q->head = p;
		} else {
			NEXTP(q->tail) = p;
			q->tail = p;
		}
	}

	NdisReleaseSpinLock(&q->queue_lock);
}

ND_PKT *
shared_deq(
	IN struct npq *q
)
{
	ND_PKT *p;

	NdisAcquireSpinLock(&q->queue_lock);

	if (p = q->head) {
		ASSERT(q->tail);
		q->head = NEXTP(p);
		NEXTP(p) = NULL;
		if (q->head == NULL)
			q->tail = NULL;
	}
	else {
		ASSERT(q->tail == NULL);
	}

	NdisReleaseSpinLock(&q->queue_lock);

	return (p);
}

static void
shared_free_pkt(ND_PKT* p)
{
#ifndef NDIS60
	PNDIS_BUFFER b;

	NdisQueryPacket(p, NULL, NULL, &b, NULL);
	ASSERT(b);
	NdisFreeBuffer(b);
	NdisFreePacket(p);
#else /* NDIS60 */
	PNET_BUFFER nb;
	PMDL b;

	nb = NET_BUFFER_LIST_FIRST_NB(p);
	b = NET_BUFFER_FIRST_MDL(nb);
	ASSERT(b);
	NdisFreeMdl(b);
	NdisFreeNetBufferList(p);
#endif /* NDIS60 */
}

/* bust apart and free an rx ndis packet and buffer descriptor and lbuf */
void
shared_free_ndispacket(
	IN shared_info_t *shared,
	IN ND_PKT *p
)
{
	struct lbuf *lb;

	/* pick the associated lbuf off the ndis packet */
	lb = (struct lbuf*) NEXTP(p);
	ASSERT(lb);
	ASSERT(lb->p == p);
	NEXTP(p) = NULL;
	lb->p = NULL;

	/* free the ndis packet and buffer descriptors */
	shared_free_pkt(p);

	/* put our lbuf back on the freelist */
	shared_lb_put(shared, lb->l, lb);
}

/*
 * Convert the ndis packet chain into an lbuf .
 */
struct lbuf*
shared_txlb_convert(shared_info_t *sh, ND_PKT *p)
{
#ifndef NDIS60
	struct lbuf *lb;
	PNDIS_BUFFER b, next;
	uchar *bdata, *buf;
	uint blen, tot;
	struct lbfree *txlbfree;

	ASSERT(p);

	NdisQueryPacket(p, NULL, NULL, &b, &tot);

	ASSERT(b);
	ASSERT(tot <= LBDATASZ);

	if ((b == NULL) || (tot > LBDATASZ))
		return (NULL);

	txlbfree = &sh->txfree;
	/* txqueue free buffer count shouldn't go below threshold */
	if (txlbfree->count <= TXLB_FREEPOOL_THREHOLD(txlbfree->total))
		return (NULL);

	/* alloc lbuf */
	if ((lb = shared_lb_get(sh, txlbfree)) == NULL)
		return (NULL);

	/* Adjust for the head room requested */
	ASSERT(txlbfree->size > txlbfree->headroom);
	lb->data += txlbfree->headroom;

	/*
	 * In case of dongle, make sure the begining of the buffer is
	 * aligned at 32 bytes for DMA efficiency, after inserting
	 * header of 16 bytes later in DHD layer
	 */
	if (((uintptr)lb->data % 32) <= 16)
		lb->data += 16 - (uintptr)lb->data % 32;
	else
		lb->data -= (uintptr)lb->data % 32 - 16;

	buf = lb->data;

	while (b && tot) {
#if defined(NDIS51)
		NdisQueryBufferSafe(b, &bdata, &blen, NormalPagePriority);
#else
		NdisQueryBuffer(b, &bdata, &blen);
#endif /* defined (NDIS51) */

		blen = MIN(blen, tot);

		if (blen) {
			bcopy(bdata, buf, blen);
			lb->tail += blen;
			lb->len += blen;
			buf += blen;
			tot -= blen;
		}

		NdisGetNextBuffer(b, &next);
		b = next;
	}
	/* save a pointer to the ndis packet for later sendcomplete */
	lb->p = p;
	return (lb);

#else /* !NDIS60 */

	struct lbuf *lb;
	PNET_BUFFER nb;
	PMDL b, next;
	uint offset;
	uchar *bdata, *buf;
	uint blen, tot;
	struct lbfree *txlbfree;

	ASSERT(p);

	tot = 0;
	for (nb = NET_BUFFER_LIST_FIRST_NB(p); nb; nb = NET_BUFFER_NEXT_NB(nb))
		tot += NET_BUFFER_DATA_LENGTH(nb);

	nb = NET_BUFFER_LIST_FIRST_NB(p);
	if (nb == NULL)
		return (NULL);
	b = NET_BUFFER_CURRENT_MDL(nb);
	offset = NET_BUFFER_CURRENT_MDL_OFFSET(nb);

	ASSERT(b);
	ASSERT(tot <= LBDATASZ);

	if ((b == NULL) || (tot > LBDATASZ))
		return (NULL);

	txlbfree = &sh->txfree;
	/* txqueue free buffer count shouldn't go below threshold */
	if (txlbfree->count <= TXLB_FREEPOOL_THREHOLD(txlbfree->total))
		return (NULL);

	/* alloc lbuf */
	if ((lb = shared_lb_get(sh, txlbfree)) == NULL)
		return (NULL);

#if defined(NDIS60)
	/* Adjust for the head room requested */
	/* ASSERT(txlbfree->size > txlbfree->headroom); */
	lb->data += txlbfree->headroom;

	/*
	 * In case of dongle, make sure the begining of the buffer is
	 * aligned at 32 bytes for DMA efficiency, after inserting
	 * header of 16 bytes later in DHD layer
	 */
	if (((uintptr)lb->data % 32) <= 16)
		lb->data += 16 - (uintptr)lb->data % 32;
	else
		lb->data -= (uintptr)lb->data % 32 - 16;
#endif /* UNDER_CE && NDIS60 */

	buf = lb->data;

	while (b && tot) {
		NdisQueryMdl(b, &bdata, &blen, NormalPagePriority);

		if (bdata == NULL)
			goto next_mdl;

		if (blen > offset) {
			bdata += offset;
			blen -= offset;
		} else {
			offset -= blen;
			goto next_mdl;
		}

		blen = MIN(blen, tot);

		if (blen) {
			bcopy(bdata, buf, blen);
			lb->tail += blen;
			lb->len += blen;
			buf += blen;
			tot -= blen;
			offset = 0;
		}

	next_mdl:
		NdisGetNextMdl(b, &next);
		if (!next) {
			nb = NET_BUFFER_NEXT_NB(nb);
			if (nb) {
				next = NET_BUFFER_CURRENT_MDL(nb);
				offset = NET_BUFFER_CURRENT_MDL_OFFSET(nb);
			}
		}
		b = next;
	}

	/* save a pointer to the ndis packet for later sendcomplete */
	lb->p = p;

	return (lb);
#endif /* !NDIS60 */
}

/* the WinCE C compiler defines __FUNCTION__ as a null string.
 * this function returns file name and line number instead
 */
#if defined(BCMDBG)
#if _WIN32_WCE == 420
static char line_buf[100];
char*
str_line(char *file, uint line)
{
	sprintf(line_buf, "%s:%d", file, line);
	return line_buf;
}
#endif /* _WIN32_WCE == 420 */
#endif 

void
shared_indicate_status(NDIS_HANDLE adapterhandle, NDIS_STATUS StatusCode,
	void *StatusBuffer,	ULONG StatusBufferSize)
{
#ifndef NDIS60
/*	NdisMIndicateStatus(wl->sh.adapterhandle, 
		NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
	  StatusBuffer, StatusBufferSize);
	NdisMIndicateStatusComplete(wl->sh.adapterhandle);
 */
	NdisMIndicateStatus(adapterhandle,
		NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
	  StatusBuffer, StatusBufferSize);
	NdisMIndicateStatusComplete(adapterhandle);
#else
	NDIS_STATUS_INDICATION ind;

	bzero(&ind, sizeof(NDIS_STATUS_INDICATION));

	ind.Header.Type = NDIS_OBJECT_TYPE_STATUS_INDICATION;
	ind.Header.Revision = NDIS_STATUS_INDICATION_REVISION_1;
	ind.Header.Size = sizeof(NDIS_STATUS_INDICATION);

	ind.SourceHandle = adapterhandle;
	ind.DestinationHandle = NULL;
	/* [N.B.: Jiandong says the RequestID field is not needed...] */

	ind.StatusCode = StatusCode;
	ind.StatusBuffer = StatusBuffer;
	ind.StatusBufferSize = StatusBufferSize;

	NdisMIndicateStatusEx(adapterhandle, &ind);
#endif /* NDIS60 */
}

static void
shared_isr(void *h)
{
	BOOLEAN InterruptRecognized;
	BOOLEAN QueueMiniportHandleInterrupt;
	NDIS_HANDLE MiniportAdapterContext;
	shared_info_t *sh = (shared_info_t *)h;

	MiniportAdapterContext = (NDIS_HANDLE) sh->isr_arg;

	(sh->isr_cb)(&InterruptRecognized, &QueueMiniportHandleInterrupt, MiniportAdapterContext);

	if (QueueMiniportHandleInterrupt)
		NdisSetEvent(&sh->dpc_event);
}


static void
shared_dpc_thread(void *h)
{
	shared_info_t *sh = (shared_info_t *)h;

	/* Read the priority from registry */
	CeSetThreadPriority(GetCurrentThread(), sh->DPCPriority);

	while (TRUE) {
		NdisWaitEvent(&sh->dpc_event, 0);	/* wait forever */

		NdisResetEvent(&sh->dpc_event);		/* reset the event */

		(sh->dpc_cb)(sh->dpc_arg);
	}
}

void
shared_dpc_schedule(shared_info_t *sh)
{
	NdisSetEvent(&sh->dpc_event);
}

void
shared_dpc_reschedule(PVOID systemspecific1, NDIS_HANDLE context,
	PVOID systemspecific2, PVOID systemspecific3)
{
	shared_info_t *sh = (shared_info_t *)context;

	if (sh) {
		if ((sh->dpc_cb && sh->dpc_arg))
			(sh->dpc_cb)(sh->dpc_arg);
	}
}

#ifndef NDIS60

NDIS_STATUS
shared_interrupt_register(OUT PNDIS_MINIPORT_INTERRUPT Interrupt,
NDIS_HANDLE MiniportAdapterHandle,
UINT InterruptVector,
UINT InterruptLevel,
BOOLEAN RequestIsr,
BOOLEAN SharedInterrupt,
NDIS_INTERRUPT_MODE InterruptMode,
shared_info_t *sh,
void (*isr_cb)(PBOOLEAN, PBOOLEAN, NDIS_HANDLE),
void *isr_arg,
void (*dpc_cb)(NDIS_HANDLE),
void *dpc_arg)
{
	if (sh->BusType == NdisInterfacePci || sh->BusType == NdisInterfacePcMcia) {
		return NdisMRegisterInterrupt(Interrupt, MiniportAdapterHandle,
		                        InterruptVector, InterruptLevel,
		                        RequestIsr,
		                        SharedInterrupt,
		                        InterruptMode);
	}
	else if (sh->BusType == NdisInterfaceSDIO) {
		sh->isr_cb  = isr_cb;
		sh->isr_arg = isr_arg;

		NdisInitializeEvent(&sh->dpc_event);
		NdisResetEvent(&sh->dpc_event);
		sh->dpc_handle = CreateThread(NULL,		/* security atributes */
						0,		/* initial stack size */
			(LPTHREAD_START_ROUTINE)shared_dpc_thread,	/* Main() function */
						sh,		/* arg to reader thread */
						0,		/* creation flags */
						&sh->dpc_thread_id); /* returned thread id */
		if (!sh->dpc_handle)
			return NDIS_STATUS_FAILURE;

		if (bcmsdh_intr_reg(sh->sdh, shared_isr, sh))
			return NDIS_STATUS_FAILURE;

	}

	sh->dpc_cb  = dpc_cb;
	sh->dpc_arg = dpc_arg;

	/* Initilize the timer required to reschdule the DPC, if necessary */
	NdisInitializeTimer(&sh->dpc_reshed_timer, shared_dpc_reschedule, (PVOID)sh);

	return NDIS_STATUS_SUCCESS;
}
#else /* !NDIS60 */
NDIS_STATUS
shared_interrupt_register(NDIS_HANDLE MiniportAdapterHandle,
shared_info_t *sh,
void (*isr_cb)(PBOOLEAN, PBOOLEAN, NDIS_HANDLE),
void *isr_arg,
void (*dpc_cb)(NDIS_HANDLE),
void *dpc_arg)
{
	if (sh->BusType == NdisInterfaceSDIO) {
		sh->isr_cb  = isr_cb;
		sh->isr_arg = isr_arg;

		sh->dpc_handle = CreateThread(NULL,		/* security atributes */
						0,		/* initial stack size */
			(LPTHREAD_START_ROUTINE)shared_dpc_thread,	/* Main() function */
						sh,		/* arg to reader thread */
						0,		/* creation flags */
						&sh->dpc_thread_id); /* returned thread id */
		if (!sh->dpc_handle)
		{
			 printf("---> shared_interrupt_register error 1\n");

			return NDIS_STATUS_FAILURE;
		}

		NdisInitializeEvent(&sh->dpc_event);
		NdisResetEvent(&sh->dpc_event);

		if (bcmsdh_intr_reg(sh->sdh, shared_isr, sh))
		{
			 printf("---> shared_interrupt_register Error 2\n");

			return NDIS_STATUS_FAILURE;
		}
	}

	sh->dpc_cb  = dpc_cb;
	sh->dpc_arg = dpc_arg;
	/* WM7 TODO: do we need this? */
	/* Initilize the timer required to reschdule the DPC, if necessary */
	NdisInitializeTimer(&sh->dpc_reshed_timer, shared_dpc_reschedule, (PVOID)sh);

	return NDIS_STATUS_SUCCESS;
}

#endif /* !NDIS60 */

#ifndef NDIS60
void
shared_interrupt_deregister(PNDIS_MINIPORT_INTERRUPT pintr, shared_info_t *sh)
{
	if (sh->BusType == NdisInterfacePci || sh->BusType == NdisInterfacePcMcia) {
		NdisMDeregisterInterrupt(pintr);
	}
	else if (sh->BusType == NdisInterfaceSDIO) {
		if (sh->sdh != NULL) {
			bcmsdh_intr_disable(sh->sdh);
			bcmsdh_intr_dereg(sh->sdh);
			if (sh->dpc_handle)
				CloseHandle(sh->dpc_handle);

			NdisFreeEvent(&sh->dpc_event);
		}
		sh->isr_cb = sh->isr_arg = NULL;
	}

	{
		BOOLEAN cancelled;

		sh->dpc_cb = sh->dpc_arg = NULL;
		NdisCancelTimer(&sh->dpc_reshed_timer, &cancelled);
	}
}
#else
void
shared_interrupt_deregister(NDIS_HANDLE intr_handle)
{
		NdisMDeregisterInterruptEx(intr_handle);
}
#endif /* !NDIS60 */

#ifndef NDIS60
BOOLEAN
shared_interrupt_synchronize(PNDIS_MINIPORT_INTERRUPT pintr,
	shared_info_t *sh,
	void (*SyncFunc)(void *),
	void *SyncCxt)
{
	if (sh->BusType == NdisInterfacePci || sh->BusType == NdisInterfacePcMcia) {
		return NdisMSynchronizeWithInterrupt(pintr, (void*)SyncFunc, (void*)SyncCxt);
	}
	else if (sh->BusType == NdisInterfaceSDIO) {

		{
			BOOLEAN intr_on;
			/* disable intr and reenable after reset if intr was on */
			intr_on = bcmsdh_intr_query(sh->sdh);
			if (intr_on)
				bcmsdh_intr_disable(sh->sdh);
			(SyncFunc)(SyncCxt);
			if (intr_on)
				bcmsdh_intr_enable(sh->sdh);
		}
	}
	return TRUE;
}
#else
BOOLEAN
shared_interrupt_synchronize(NDIS_HANDLE intr,
	void (*SyncFunc)(void *),
	void *SyncCxt)
{
	return NdisMSynchronizeWithInterruptEx(intr, 0, SyncFunc, SyncCxt);
}
#endif /* !NDIS60 */

