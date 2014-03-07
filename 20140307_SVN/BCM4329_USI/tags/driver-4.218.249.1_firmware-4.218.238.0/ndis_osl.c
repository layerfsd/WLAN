/*
 * NDIS Independent Layer
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: ndis_osl.c,v 1.82.14.1.6.7 2010/04/20 07:36:17 Exp $
 */

#define	NDIS_OSL

#include <typedefs.h>
#include <bcmdefs.h>
#include <bcmendian.h>
#include <osl.h>
#include <bcmutils.h>
#include <pcicfg.h>
#include <proto/ethernet.h>
#include <ndiserrmap.h>

#if defined(NDIS60)
#include <DbgZoneToETW.h>
#include <cebuscfg.h>
#endif /* NDIS60 && UNDER_CE */
#define PCI_CFG_RETRY	10

#define LBOFF(lb) ((uintptr)lb->data - (uintptr)lb->head)

#define OS_HANDLE_MAGIC         0xabcd1234 /* Magic # to recognise osh */
#define BCM_MEM_FILENAME_LEN    24	   /* Mem. filename length */

/*
 * The magic numbers below are result of bit positions in struct
 * NDIS_TCP_IP_CHECKSUM_PACKET_INFO in ndis.h
 */
#define BCMNDISPACKETTCPCHECKSUMSUCCEEDED	0x8
#define BCMNDISPACKETIPCHECKSUMSUCCEEDED	0x20

#define BCMRXCHECKSUMSUCCEEDED			(BCMNDISPACKETTCPCHECKSUMSUCCEEDED | \
							BCMNDISPACKETIPCHECKSUMSUCCEEDED)

#define BCMNDISPACKETTCPCHECKSUMFAILED		0x1
#define BCMNDISPACKETUDPCHECKSUMFAILED		0x2
#define BCMNDISPACKETIPCHECKSUMFAILED		0x4

#define BCMRXCHECKSUMFAILED			(BCMNDISPACKETTCPCHECKSUMFAILED	| \
							BCMNDISPACKETUDPCHECKSUMFAILED | \
							BCMNDISPACKETIPCHECKSUMFAILED)

#define BCMNDISPACKETCHECKSUMV4			0x1
#define BCMNDISPACKETCHECKSUMV6			0x2
#define BCMNDISPACKETTCPCHECKSUM		0x4
#define BCMNDISPACKETUDPCHECKSUM		0x8
#define BCMNDISPACKETIPCHECKSUM			0x10

#define BCMTXCHECKSUMSTATE			((BCMNDISPACKETCHECKSUMV4 | \
							BCMNDISPACKETTCPCHECKSUM | \
							BCMNDISPACKETUDPCHECKSUM | \
							BCMNDISPACKETIPCHECKSUM) & \
							~(BCMNDISPACKETCHECKSUMV6))

typedef struct bcm_mem_link {
	struct bcm_mem_link *prev;
	struct bcm_mem_link *next;
	uint    size;
	int     line;
	char    file[BCM_MEM_FILENAME_LEN];
} bcm_mem_link_t;

struct osl_dmainfo {
	uint addrwidth;   /* Maximum addressability */
	uint32 hiaddrmask; /* Mask for upper 32 bits */
	uint32 loaddrmask; /* Mask for lower 32 bits */
};

struct osl_info {
	void *pdev;
	uint magic;
	uint malloced;
	uint failed;
	bcm_mem_link_t *dbgmem_list;
	osldma_t dma_cap;
	pktfree_cb_fn_t tx_fn;
	void *tx_ctx;
};

#ifdef __BBSOFT__
wchar_t g_oslLogFile[512];
#endif //__BBSOFT__

osl_t *
osl_attach(void *pdev, NDIS_HANDLE adapterhandle)
{
	NDIS_STATUS status;
	osl_t *osh;
	int last_error = BCME_LAST;

#ifndef NDIS60
	status = NdisAllocateMemoryWithTag(&osh, sizeof(osl_t), MEMORY_TAG);
#else /* NDIS60 */
	osh = NdisAllocateMemoryWithTagPriority(adapterhandle, sizeof(osl_t),
	      MEMORY_TAG, HighPoolPriority);
	if (!osh)
		status = NDIS_STATUS_FAILURE;
	else
		status = NDIS_STATUS_SUCCESS;
#endif /* NDIS60 */

	ASSERT(status == NDIS_STATUS_SUCCESS);
	ASSERT(osh);
	if (status != NDIS_STATUS_SUCCESS)
		return NULL;

	NdisZeroMemory(osh, sizeof(osl_t));
	/*
	 * check the cases where
	 * 1.Error code Added to bcmerror table, but forgot to add it to the OS
	 * dependent error code
	 * 2. Error code is added to the bcmerror table, but forgot to add the
	 * corresponding errorstring (dummy call to bcmerrorstr)
	 */
	ndisstatus2bcmerror(NDIS_STATUS_SUCCESS);
	bcmerrorstr(0);

	osh->magic = OS_HANDLE_MAGIC;
	osh->malloced = 0;
	osh->failed = 0;
	osh->dbgmem_list = NULL;
	osh->pdev = pdev;

	return osh;
}

void
osl_detach(osl_t *osh)
{
	if (osh == NULL)
		return;

	ASSERT(osh->magic == OS_HANDLE_MAGIC);
	NdisFreeMemory(osh, sizeof(osl_t), 0);
}

void
osl_pktfree_cb_set(osl_t *osh, pktfree_cb_fn_t tx_fn, void *tx_ctx)
{
	osh->tx_fn = tx_fn;
	osh->tx_ctx = tx_ctx;
}

void
osl_dmaddrwidth(osl_t *osh, uint addrwidth)
{
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	shared = osh->pdev;
	osh->dma_cap.addrwidth = addrwidth;
	shared->dmaddrwidth = addrwidth;

	switch (addrwidth) {
	case DMADDRWIDTH_30:
		osh->dma_cap.loaddrmask = DMADDR_MASK_30;
		osh->dma_cap.hiaddrmask = DMADDR_MASK_0;
		break;
	case DMADDRWIDTH_32:
		osh->dma_cap.loaddrmask = DMADDR_MASK_32;
		osh->dma_cap.hiaddrmask = DMADDR_MASK_0;
		break;
	case DMADDRWIDTH_64:
		osh->dma_cap.loaddrmask = DMADDR_MASK_32;
		osh->dma_cap.hiaddrmask = DMADDR_MASK_0;
		break;
	default:
		ASSERT(0);
	}

	shared = osh->pdev;

#ifdef BCMDBG
	printf("osl_dmacap_register: 0x%x 0x%x\n", osh->dma_cap.loaddrmask,
	       osh->dma_cap.hiaddrmask);
#endif
}

/* Return if a address is within the range of DMA's access capability */
bool
osl_dmaddr_valid(osl_t *osh, ulong loPart, ulong hiPart)
{
	ASSERT(osh->dma_cap.addrwidth);
	return !((loPart & osh->dma_cap.loaddrmask) ||
	         (hiPart & osh->dma_cap.hiaddrmask));
}

#ifdef BCMDBG_MEM

void*
osl_debug_malloc(osl_t *osh, uint size, int line, char* file)
{
	bcm_mem_link_t *p;
	char* basename;

	ASSERT(size);

	if ((p = (bcm_mem_link_t*)osl_malloc(osh, sizeof(bcm_mem_link_t) + size)) == NULL)
		return (NULL);

	p->size = size;
	p->line = line;

	basename = strrchr(file, '/');
	/* skip the '/' */
	if (basename)
		basename++;

	if (!basename)
		basename = file;

	strncpy(p->file, basename, BCM_MEM_FILENAME_LEN);
	p->file[BCM_MEM_FILENAME_LEN - 1] = '\0';

	/* link this block */
	p->prev = NULL;
	p->next = osh->dbgmem_list;
	if (p->next)
		p->next->prev = p;
	osh->dbgmem_list = p;

	return p + 1;
}

void
osl_debug_mfree(osl_t *osh, void *addr, uint size, int line, char* file)
{
	bcm_mem_link_t *p = (bcm_mem_link_t *)((int8*)addr - sizeof(bcm_mem_link_t));

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	if (p->size == 0) {
		printf("osl_debug_mfree: double free on addr %p size %d at line %d file %s\n",
			addr, size, line, file);
		ASSERT(p->size);
		return;
	}

	if (p->size != size) {
		printf("osl_debug_mfree: dealloc size %d does not match alloc size %d on addr %p"
		       " at line %d file %s\n",
		       size, p->size, addr, line, file);
		ASSERT(p->size == size);
		return;
	}

	/* unlink this block */
	if (p->prev)
		p->prev->next = p->next;
	if (p->next)
		p->next->prev = p->prev;
	if (osh->dbgmem_list == p)
		osh->dbgmem_list = p->next;
	p->next = p->prev = NULL;

	osl_mfree(osh, (char *)p, size + sizeof(bcm_mem_link_t));
}

int
osl_debug_memdump(osl_t *osh, struct bcmstrbuf *b)
{
	bcm_mem_link_t *p;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	bcm_bprintf(b, "   Address\tSize\tFile:line\n");
	for (p = osh->dbgmem_list; p; p = p->next)
		bcm_bprintf(b, "%p\t%5d\t%s:%d\n",
			p + 1, p->size, p->file, p->line);

	return 0;
}

#endif  /* BCMDBG_MEM */

void*
osl_malloc(osl_t *osh, uint size)
{
	NDIS_STATUS status;
	void *addr;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

#ifdef NDIS60
	{
		shared_info_t *shared = osh->pdev;
		addr = NdisAllocateMemoryWithTagPriority(shared->adapterhandle, size, MEMORY_TAG,
		      NormalPoolPriority);
	}
	if (!addr)
		status = NDIS_STATUS_RESOURCES;
	else
		status = NDIS_STATUS_SUCCESS;
#else /* NDIS60 */
	status = NdisAllocateMemoryWithTag(&addr, size, MEMORY_TAG);
#endif /* NDIS60 */
	if (status != NDIS_STATUS_SUCCESS) {
		osh->failed++;
		return (NULL);
	}

	osh->malloced += size;
	return (addr);
}

void
osl_mfree(osl_t *osh, char *addr, uint size)
{
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	if (!osh)
		return;

#ifdef BCMDBG
	deadbeef(addr, size);
#endif /* BCMDBG */

	osh->malloced -= size;
	NdisFreeMemory(addr, size, 0);
}

uint
osl_malloced(osl_t *osh)
{
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	if (!osh)
		return 0;
	return (osh->malloced);
}

uint
osl_malloc_failed(osl_t *osh)
{
	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	if (!osh)
		return 0;
	return (osh->failed);
}


uint32
osl_pci_read_config(osl_t *osh, uint offset, uint size)
{
	uint32 val = 0;
	uint read;
	uint retry = PCI_CFG_RETRY;
	uint fail;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	ASSERT(size <= 4);

	shared = osh->pdev;

	fail = 0xffffffff >> (4-size)*8;

	if (offset >= PCIE_EXTCFG_OFFSET) {
#ifdef BCMDBG
		printf("Extended configuration space not yet supported\n");
#endif
		return -1;
	}

	do {
#ifndef NDIS60
		read = NdisReadPciSlotInformation(shared->adapterhandle, shared->slot,
			offset, &val, size);
#else
		read = NdisMGetBusData(shared->adapterhandle, PCI_WHICHSPACE_CONFIG,
		       offset, &val, size);
#endif
		if (val != fail)
			break;
	} while (retry--);

#ifdef BCMDBG
	if (retry < PCI_CFG_RETRY)
		printf("PCI CONFIG READ access to %d required %d retries\n", offset,
		       (PCI_CFG_RETRY - retry));

	/* Win98SE: config accesses fail during Windows shutdown */
	if (shared->OS != NdisEnvironmentWindows)
		ASSERT(read == size);
#endif /* BCMDBG */

	return (val);
}

void
osl_pci_write_config(osl_t *osh, uint offset, uint size, uint val)
{
	uint wrote;
	uint retry = PCI_CFG_RETRY;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	shared = osh->pdev;

	if (offset >= PCIE_EXTCFG_OFFSET) {
#ifdef BCMDBG
		printf("Extended configuration space not yet supported\n");
#endif
		return;
	}

	do {
#ifndef NDIS60
		wrote = NdisWritePciSlotInformation(shared->adapterhandle, shared->slot,
			offset, &val, size);
#else /* NDIS60 */
		wrote = NdisMSetBusData(shared->adapterhandle, PCI_WHICHSPACE_CONFIG,
		      offset, &val, size);
#endif /* NDIS60 */

		if (offset != PCI_BAR0_WIN)
			break;

		if (osl_pci_read_config(osh, offset, size) == val)
			break;
	} while (retry--);

#ifdef BCMDBG
	if (retry < PCI_CFG_RETRY)
		printf("PCI CONFIG WRITE access to %d required %d retries\n", offset,
		       (PCI_CFG_RETRY - retry));

	/* Win98SE: config accesses fail during Windows shutdown */
	if (shared->OS != NdisEnvironmentWindows)
		ASSERT(wrote == size);
#endif /* BCMDBG */
}

void
osl_pcmcia_read_attr(osl_t *osh, uint offset, void *buf, int size)
{
	volatile uint8 *sptr;
	volatile uint8 *dptr = (uint8*)buf;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	shared = osh->pdev;
	sptr = (uint8*)shared->attrva + (offset * 2);
	for (; size; size--, sptr += 2, dptr++)
		*dptr = *sptr;
}

void
osl_pcmcia_write_attr(osl_t *osh, uint offset, void *buf, int size)
{
	volatile uint8 *sptr = (uint8*)buf;
	volatile uint8 *dptr;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	shared = osh->pdev;
	dptr = (uint8*)shared->attrva + (offset * 2);
	for (; size; size--, dptr++, sptr += 2)
		*dptr = *sptr;
}


#ifdef BCMDBG_ASSERT
void
osl_assert(char *exp, char *file, int line)
{
	printf("assertion \"%s\" failed: file \"%s\", line %d\n", exp, file, line);
	printf("\n");
#ifndef NOBUGCHECK
	KeBugCheckEx(line, 0, 0, 0, 0);
#endif /* NOBUGCHECK */
}
#endif /* BCMDBG_ASSERT */

char*
osl_strncpy(char *d, char *s, uint n)
{
	return (strncpy(d, s, n));
}

void*
osl_dma_alloc_consistent(osl_t *osh, uint size, ulong *pap)
{
	char	*va;				/* kernel virtual address */
	NDIS_PHYSICAL_ADDRESS	npa;		/* physical address */
	shared_info_t *shared;
	uint alloc_size; /* Keep track of what's actually allocated */

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	ASSERT(osh->dma_cap.addrwidth);

	shared = osh->pdev;
	alloc_size = size;


		NdisMAllocateSharedMemory(shared->adapterhandle, alloc_size, FALSE,
		                          (void **) &va, &npa);

	/* Fail if WAR is not being used and address is not valid */
	if (!(osl_dmaddr_valid(osh, npa.LowPart, npa.HighPart))) {
		NdisMFreeSharedMemory(shared->adapterhandle, alloc_size, FALSE, (PVOID) va, npa);
		*pap = NULL;
		return NULL;
	}

	*pap = (ulong) npa.LowPart;

	return va;
}

void
osl_dma_free_consistent(osl_t *osh, void *va, uint size, ulong pa)
{
	NDIS_PHYSICAL_ADDRESS npa;
	shared_info_t *shared;
	uint free_size;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	shared = osh->pdev;

	npa.LowPart = (ULONG) pa;
	free_size = size;

		NdisMFreeSharedMemory(shared->adapterhandle, free_size, FALSE, (PVOID) va,
		                      npa);
}

uint
osl_dma_map(osl_t *osh, uint size, int direction, struct lbuf *lb)
{
	ULONG pa;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	shared = osh->pdev;

	pa = (ULONG) lb->pa + (ULONG) LBOFF(lb);

	shared_flush(shared, (uchar *) lb->data, pa, (ULONG)size, (BOOLEAN)(direction == DMA_TX));

	return pa;

}

void
osl_dma_unmap(osl_t *osh, uint size, int direction, struct lbuf *lb)
{
	ULONG pa;
	uint offset;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));

	shared = osh->pdev;

	if (lb) {
		/* sync caches and main memory for rxh and max data */
		offset = (uint)LBOFF(lb);
		pa = (ULONG) (lb->pa + offset);
		if (direction == DMA_RX)
			shared_flush(shared, (uchar *) lb->data, pa,
			             (ulong)(size - offset), FALSE);
	}
}

void*
osl_pktget(osl_t *osh, uint len, bool send)
{
	struct lbuf	*lb;
	shared_info_t *shared;

	ASSERT(osh != NULL);
	ASSERT(osh->magic == OS_HANDLE_MAGIC);

	shared = osh->pdev;

	if (len > LBDATASZ)
		return (NULL);

	if (send)
		lb = shared_lb_get(shared, &shared->txfree);
	else
		lb = shared_lb_get(shared, &shared->rxfree);

	if (lb)
		lb->len = len;

	return ((void*) lb);
}

void
osl_pktfree(osl_t *osh, struct lbuf	*lb, bool send)
{
	struct lbuf *next;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	ASSERT(lb);

	if (send && osh->tx_fn)
		osh->tx_fn(osh->tx_ctx, (void *)lb, 0);

	shared = osh->pdev;

	while (lb) {
		next = lb->next;
		lb->next = NULL;

		if (lb->p && send) {
			/* put original packet on the txdone list */
			shared_enq(&shared->txdone, lb->p, FALSE);
			lb->p = NULL;
		}
		ASSERT(lb->p == NULL);

		shared_lb_put(shared, lb->l, lb);

		lb = next;
	}
}

void
osl_pktsetlen(osl_t *osh, struct lbuf *lb, uint len)
{
	ASSERT(len + LBOFF(lb) <= LBDATASZ);

	/* ASSERT(len >= 0); */
	lb->len = len;
	lb->tail = lb->data + len;
}

uchar*
osl_pktpush(osl_t *osh, struct lbuf *lb, uint bytes)
{
	if (bytes) {
		ASSERT(LBOFF(lb) >= bytes);

		lb->data -= bytes;
		lb->len += bytes;
	}

	return (lb->data);
}

uchar*
osl_pktpull(osl_t *osh, struct lbuf *lb, uint bytes)
{
	if (bytes) {
		ASSERT(bytes <= lb->len);

		lb->data += bytes;
		lb->len -= bytes;
	}

	return (lb->data);
}

void*
osl_pktdup(osl_t *osh, struct lbuf *lb)
{
	struct lbuf	*lbcopy;
	shared_info_t *shared;

	ASSERT((osh && (osh->magic == OS_HANDLE_MAGIC)));
	ASSERT(lb);

	shared = osh->pdev;

	lbcopy = shared_lb_get(shared, &shared->txfree);

	if (lbcopy) {
		lbcopy->data = lbcopy->head + LBOFF(lb);
		lbcopy->len = pktcopy(osh, lb, 0, pkttotlen(osh, lb), lbcopy->data);
		lbcopy->tail = lbcopy->data + lbcopy->len;
	}
	return (lbcopy);
}

/* Converts a native (OS) packet to Driver packet.
 * Allocates a new lbuf and copies the contents
 */
void *
osl_pkt_frmnative(osl_t *osh, ND_PKT *p)
{
	struct lbuf* lb;
	shared_info_t *sh;

	ASSERT((osh && osh->magic == OS_HANDLE_MAGIC));

	sh = osh->pdev;

	/* First convert the packet */
	if (!(lb =  shared_txlb_convert(sh, p)))
	    return NULL;

	return ((void *)lb);
}

#ifndef NDIS60
/* Converts a driver packet to native packet.
 * Allocates a PNDIS_PACKET and copies the contents from driver
 */
PNDIS_PACKET
osl_pkt_tonative(osl_t *osh, struct lbuf* lb)
{
	shared_info_t *sh;
	PNDIS_PACKET p;
	PNDIS_BUFFER b;
	NDIS_STATUS status;

	ASSERT((osh && osh->magic == OS_HANDLE_MAGIC));

	sh = osh->pdev;

	/* alloc ndis packet descriptor */
	NdisAllocatePacket(&status, &p, sh->rxpacketpool);
	if (NDIS_ERROR(status)) {
		NdisWriteErrorLogEntry(sh->adapterhandle,
		                       NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 17);
		shared_lb_put(sh, &sh->rxfree, lb);
		return NULL;
	}

	NdisAllocateBuffer(&status, &b, sh->rxbufferpool, lb->data, lb->len);

	if (NDIS_ERROR(status)) {
		NdisWriteErrorLogEntry(sh->adapterhandle,
		                       NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 18);
		shared_lb_put(sh, &sh->rxfree, lb);
		NdisFreePacket(p);
		return NULL;
	}

	/* attach the buffer to the packet */
	NdisChainBufferAtFront(p, b);
	NdisQueryPacket(p, NULL, NULL, &b, NULL);

	lb->p = p;
	NEXTP(p) = (PNDIS_PACKET) lb;

	/* set all the packet attributes */
	NDIS_SET_PACKET_HEADER_SIZE(p, ETHER_HDR_LEN);
	NdisSetPacketFlags(p, 0);

	/* Set the priority of the pkt */
	NDIS_PER_PACKET_INFO_FROM_PACKET(p, Ieee8021pPriority) = (PVOID)(uintptr)PKTPRIO(lb);

#ifdef TOE
	NDIS_PER_PACKET_INFO_FROM_PACKET(p, TcpIpChecksumPacketInfo) =
		(PVOID)(uintptr)((lb->flags & LBF_SUM_GOOD) ?
		BCMRXCHECKSUMSUCCEEDED : BCMRXCHECKSUMFAILED);
#endif /* TOE */
	return (p);
}

int
osl_pktsumneeded(struct lbuf *lb)
{
	ULONG chksumpktinfo;
	PNDIS_TCP_IP_CHECKSUM_PACKET_INFO pchksumpktinfo;
	PNDIS_PACKET p = lb->p;

	if (p == NULL)
		return 0;

	if (NDIS_PROTOCOL_ID_TCP_IP != NDIS_GET_PACKET_PROTOCOL_TYPE(p)) {
#ifdef BCMDBG
		ND_TRACE(("Packet protocol is not TCP.\n"));
#endif /* BCMDBG */
		return 0;
	}
	chksumpktinfo = PtrToUlong(NDIS_PER_PACKET_INFO_FROM_PACKET(p, TcpIpChecksumPacketInfo));
	pchksumpktinfo = (PNDIS_TCP_IP_CHECKSUM_PACKET_INFO) & chksumpktinfo;
	if (pchksumpktinfo->Value & BCMTXCHECKSUMSTATE)
		return  1;
	else
		return 0;
}

#else /* NDIS60 */

int
osl_pktsumneeded(struct lbuf *lb)
{
	return 0;
}

/* Converts a driver packet to native packet.
 * Allocates a PNET_BUFFER_LIST and copies the contents from driver
 */
PNET_BUFFER_LIST
osl_pkt_tonative(osl_t *osh, struct lbuf* lb)
{
	shared_info_t *sh;
	PNET_BUFFER_LIST p;
	PNET_BUFFER nb;
	PMDL b;
#if !defined(EXT_STA)
	NDIS_STATUS status;
#endif

	ASSERT((osh && osh->magic == OS_HANDLE_MAGIC));

	sh = osh->pdev;
	/* alloc MDL */
	b = NdisAllocateMdl(sh->adapterhandle, lb->data, lb->len);
	if (b == NULL) {
		NdisWriteErrorLogEntry(sh->adapterhandle,
		                       NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 17);
		goto free_lb;
	}

	/* alloc NET_BUFFER_LIST */
	p = NdisAllocateNetBufferAndNetBufferList(sh->rxpacketpool, 0, 0, b, 0, 0);
	if (p == NULL) {
		NdisWriteErrorLogEntry(sh->adapterhandle,
		      NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 18);
		goto free_Mdl;
	}

	lb->p = p;

	NEXTP(p) = (PNET_BUFFER_LIST) lb;

	nb = NET_BUFFER_LIST_FIRST_NB(p);
	NET_BUFFER_DATA_LENGTH(nb) = lb->len;
	NET_BUFFER_LIST_NEXT_NBL(p) = NULL;

#ifdef EXT_STA
#ifndef PRE_v1_68_HEADERS
#endif /* PRE_v1_68_HEADERS */
#endif /* EXT_STA */
	/* Set the priority of the pkt */
	NET_BUFFER_LIST_INFO(p, Ieee8021QNetBufferListInfo) = (PVOID)(uintptr)PKTPRIO(lb);

	return (p);

free_Mdl:
	NdisFreeMdl(b);
free_lb:
	shared_lb_put(sh, &sh->rxfree, lb);
	return NULL;
}
#endif /* NDIS60 */


shared_info_t *osl_get_sh(osl_t *osh)
{
	return osh->pdev;
}

uint g_dbgOutputIndex = 0;

void
myPrintf2(const char *fmt, ...)
{
	char myStr[1024];
	TCHAR myTStr[1024];
	int len;
	va_list ap;
	va_start(ap, fmt);
	vsprintf(myStr, fmt, ap);
	va_end(ap);
	MultiByteToWideChar(CP_ACP, 0, myStr, -1, myTStr, 256);
	len = wcslen(myTStr);
	if (len < 1024)
	{
		if (myTStr[len - 1] == '\n' && myTStr[len - 2] != '\r')
		{
			myTStr[len - 1] = L'\r';
			myTStr[len] = L'\n';
			myTStr[len+1] = L'\0';
		}
	}
	NKDbgPrintfW(myTStr);
}

void
ETWPrintf(const char *fmt, ...)
{
	char myStr[1024];
	TCHAR myTStr[1024];
	int len;
	va_list ap;
	va_start(ap, fmt);
	vsprintf(myStr, fmt, ap);
	va_end(ap);
	MultiByteToWideChar(CP_ACP, 0, myStr, -1, myTStr, 256);
	len = wcslen(myTStr);
	if (len < 1024)
	{
		if (myTStr[len - 1] == '\n' && myTStr[len - 2] != '\r')
		{
			myTStr[len - 1] = L'\r';
			myTStr[len] = L'\n';
			myTStr[len+1] = L'\0';
		}
	}
	RETAILMSG(1, (myTStr));
}

void LogToFile(const char *fmt_str, ...)
{
	char        buf[1000];
	TCHAR       tbuf[1000];
	va_list     marker;
	HANDLE      hFile;
	int         len;
	DWORD       bytesWritten;

	/* Add a time stamp in milliseconds */
	len = sprintf(buf, "WLAN [%08d]: ", GetTickCount());

	/* Format the message */
	va_start(marker, fmt_str);
	len += vsnprintf(buf + len, sizeof(buf) - len, fmt_str, marker);
	va_end(marker);

	strcat(buf, "\r\n");

	MultiByteToWideChar(CP_ACP, 0, buf, -1, tbuf, 1000);

#ifndef __BBSOFT__
	hFile = CreateFile(TEXT("bcmsddhdlog.txt"), GENERIC_WRITE, 0,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#else //__BBSOFT__
	if (!g_oslLogFile[0]) {
		memcpy(g_oslLogFile, TEXT("bcmsddhdlog.txt"), 20);
	}

	hFile = CreateFile(g_oslLogFile, GENERIC_WRITE, 0,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hFile) return ;
#endif //__BBSOFT__

	SetFilePointer(hFile, 0, NULL, FILE_END);       /* append! */
	WriteFile(hFile, tbuf, wcslen(tbuf) * 2, &bytesWritten, NULL);
	CloseHandle(hFile);
}

void (*dbgOutputFunc[])(fmt, ...) =
{
	&myPrintf2,
	&LogToFile,
	&ETWPrintf,
	/* Make sure we alwasy keep this as last entry, otherwise
	 * if WLBTCEAMP11 is not difined will alters the index
	 * which doesn't match with regsitry entry published
	 */
#if defined(WLBTCEAMP11)
	&LogViaStack
#endif /* WLBTCEAMP11 */
};
