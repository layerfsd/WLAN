/*
 * NDIS OS Independent Layer
 *
 * Copyright(c) 2001 Broadcom Corp.
 * $Id: ndis_osl.h,v 1.109.12.2.6.10 2010/05/28 09:42:58 Exp $
 */

#ifndef _ndis_osl_h_
#define _ndis_osl_h_

#include <typedefs.h>
#include <ndshared.h>
#include <bcmsdh.h>

/* make WHQL happy */
#ifndef MEMORY_TAG
#define MEMORY_TAG 'MCRB'	/* BRCM reversed */
#endif /* MEMORY_TAG */
#ifdef WDM
#define NdisAllocateMemory(__va, __len, __dummy1, __dummy2) \
	NdisAllocateMemoryWithTag(__va, __len, (ULONG) MEMORY_TAG)
#endif /* WDM */

#ifdef _PREFAST_
#define ASSERT(exp)	__assume(exp)
#else /* _PREFAST_ */
#ifndef USE_MS_ASSERT
#ifdef BCMDBG_ASSERT
#undef ASSERT
#define ASSERT(exp)     if (exp); else osl_assert(#exp, __FILE__, __LINE__)
extern void osl_assert(char *exp, char *file, int line);
#else /* BCMDBG_ASSERT */
#undef ASSERT
#define	ASSERT(exp)
#endif /* BCMDBG_ASSERT */
#endif /* USE_MS_ASSERT */
#endif /* _PREFAST_ */

#define	DMA_TX	1	/* TX direction for DMA */
#define	DMA_RX	2	/* RX direction for DMA */

#if !defined(COOKIE_SHARED)
#define COOKIE_SHARED(cookie)		(cookie)
#endif /* !defined (COOKIE_SHARED) */

#define OSL_SDIO_READ_ATTR(osh, offset, buf, size) \
	bcmsdh_cfg_read(NULL, (offset), (buf), (size), NULL)
#define OSL_SDIO_WRITE_ATTR(osh, offset, buf, size) \
	bcmsdh_cfg_write(NULL, (offset), (buf), (size), NULL)

#define OSL_PCMCIA_READ_ATTR(osh, offset, buf, size) osl_pcmcia_read_attr((osh), (offset), (buf), \
				(size))
#define OSL_PCMCIA_WRITE_ATTR(osh, offset, buf, size) osl_pcmcia_write_attr((osh), (offset), \
				(buf), (size))
extern void osl_pcmcia_read_attr(osl_t *osh, uint offset, void *buf, int size);
extern void osl_pcmcia_write_attr(osl_t *osh, uint offset, void *buf, int size);

#define	OSL_PCI_READ_CONFIG(osh, offset, size)		osl_pci_read_config((osh), (offset), (size))
#define OSL_PCI_WRITE_CONFIG(osh, offset, size, val) osl_pci_write_config((osh), (offset), (size), \
				(val))
extern uint32 osl_pci_read_config(osl_t *osh, uint size, uint offset);
extern void osl_pci_write_config(osl_t *osh, uint offset, uint size, uint val);

#define OSL_PCI_BUS(osh)	(0)
#define OSL_PCI_SLOT(osh)	(0)

#define	OSL_UNCACHED(va)		(va)

/* won't need this until we boot NDIS on one of our chips */
#define	REG_MAP(pa, size)		NULL			/* not supported */
#define	REG_UNMAP(va)			0			/* not supported */

#if defined(NDIS40)
extern char * _cdecl strncpy(char *dest, const char *src, uint n);
#endif /* defined (NDIS40) */

#ifndef USEWDK
#ifndef EXT_STA
extern unsigned long _cdecl DbgPrint(char *fmt, ...);
#endif /* EXT_STA */
#endif /* USEWDK */

extern int sprintf(char *buf, const char *fmt, ...);

#define	printf	DbgPrint

/* pick up osl required snprintf/vsnprintf */
#include <bcmstdlib.h>

#if !defined(WLSIM)
static __inline uint32
readl(volatile uint32 *address)
{
	volatile uint32 v;

	NdisReadRegisterUlong((PULONG)address, (PULONG)&v);
	return (v);
}

static __inline uint16
readw(volatile uint16 *address)
{
	volatile uint16 v;

	NdisReadRegisterUshort((PUSHORT)address, (PUSHORT)&v);
	return (v);
}

static __inline uint8
readb(volatile uint8 *address)
{
	volatile uint8 v;

	NdisReadRegisterUchar((PUCHAR)address, (PUCHAR)&v);
	return (v);
}

#undef writew
#define writew(value, address) NdisWriteRegisterUshort((uint16 *)(address), (value))
#undef writel
#define writel(value, address)	NdisWriteRegisterUlong((uint32 *)(address), (value))
#undef writec
#define writeb(value, address)	NdisWriteRegisterUchar((uint8 *)(address), (value))


#define	R_REG(osh, r)	bcmsdh_reg_read(NULL, (uint32)r, sizeof(*(r)))
#define	W_REG(osh, r, v)	bcmsdh_reg_write(NULL, (uint32)r, sizeof(*(r)), (v))

#define	AND_REG(osh, r, v)	W_REG(osh, (r), (R_REG(osh, r) & (v)))
#define	OR_REG(osh, r, v)	W_REG(osh, (r), (R_REG(osh, r) | (v)))

/* Host/Bus architecture specific swap. Noop for little endian systems, possible swap on big endian
 */
#define BUS_SWAP32(v)	(v)

/* bcopy, bcmp, and bzero */
#define	bcopy(src, dst, len)	NdisMoveMemory((dst), (src), (len))
#define	bcmp(b1, b2, len)	(!NdisEqualMemory((b1), (b2), (len)))
#define	bzero(b, len)		NdisZeroMemory((b), (len))
#endif /* WLSIM */

/* OSL initialization */
extern osl_t *osl_attach(void *pdev, NDIS_HANDLE adapter_handle);
extern void osl_detach(osl_t *osh);

extern void osl_pktfree_cb_set(osl_t *osh, pktfree_cb_fn_t tx_fn, void *tx_ctx);
#define PKTFREESETCB(osh, tx_fn, tx_ctx) osl_pktfree_cb_set(osh, tx_fn, tx_ctx)

#ifdef BCMDBG_MEM

#define MALLOC(osh, size)       osl_debug_malloc((osh), (size), __LINE__, __FILE__)
#define MFREE(osh, addr, size)  osl_debug_mfree((osh), (addr), (size), __LINE__, __FILE__)
#define MALLOCED(osh)           osl_malloced((osh))
#define MALLOC_DUMP(osh, b) osl_debug_memdump((osh), (b))
extern void *osl_debug_malloc(osl_t *osh, uint size, int line, char* file);
extern void osl_debug_mfree(osl_t *osh, void *addr, uint size, int line, char* file);
extern int osl_debug_memdump(osl_t *osh, struct bcmstrbuf *b);

#else /* BCMDBG_MEM */

#define	MALLOC(osh, size)	osl_malloc((osh), (size))
#define	MFREE(osh, addr, size)	osl_mfree((osh), (char*)(addr), (size))
#define	MALLOCED(osh)		osl_malloced((osh))
#define	MALLOC_DUMP(osh, b)

#endif  /* BCMDBG_MEM */

#define MALLOC_FAILED(osh)	osl_malloc_failed((osh))

extern void *osl_malloc(osl_t *osh, uint size);
extern void osl_mfree(osl_t *osh, char *addr, uint size);
extern uint osl_malloced(osl_t *osh);
extern uint osl_malloc_failed(osl_t *osh);

#define	DMA_CONSISTENT_ALIGN	sizeof(int)
#define	DMA_ALLOC_CONSISTENT(osh, size, pap, dmah, alignbits)\
	osl_dma_alloc_consistent((osh), (size), (pap))
extern void *osl_dma_alloc_consistent(osl_t *osh, uint size, ulong *pap);
#define	DMA_FREE_CONSISTENT(osh, va, size, pa, dmah)\
	osl_dma_free_consistent((osh), (va), (size), (pa))
extern void osl_dma_free_consistent(osl_t *osh, void *va, uint size, ulong pa);

#define	DMA_MAP(osh, va, size, direction, p, dmah)\
	osl_dma_map((osh), (size), (direction), (p))
extern uint osl_dma_map(osl_t *osh, uint size, int direction, void *lb);
#define	DMA_UNMAP(osh, pa, size, direction, p, dmah)\
	osl_dma_unmap((osh), (size), (direction), (p))
extern void osl_dma_unmap(osl_t *osh, uint size, int direction, void *lb);

/* API for DMA addressing capability */
#define OSL_DMADDRWIDTH(osh, addrwidth) osl_dmaddrwidth((osh), (addrwidth))
extern void osl_dmaddrwidth(osl_t *osh, uint addrwidth);
extern bool osl_dmaddr_valid(osl_t *osh, ulong loPart, ulong hiPart);

#define	OSL_DELAY(usec)	NdisStallExecution(usec)


enum dbg_output {
	DBG_OUTPUT_KITL,
	DBG_OUTPUT_LOG,
	DBG_OUTPUT_ETW,
#if defined(WLBTCEAMP11)
	DBG_OUTPUT_BTSTACK,
#endif /* WLBTCEAMP11 */
	DBG_OUTPUT_MAX
};

#if defined(WLBTCEAMP11)
extern void LogViaStackRB(const char *fmt_str, ...);
#define logf LogViaStackRB
extern void LogViaStack(const char *fmt_str, ...);
extern void LogIntoFile(const char *fmt_str, ...);
#else
extern void LogToFile(const char *fmt_str, ...);
#endif /* WLBTCEAMP11 */

/* Note this works for Intermec CN30 platform for other platforms may require 
 * other implementation?
 */
#undef	printf
extern uint g_dbgOutputIndex;
extern void (*dbgOutputFunc[])(fmt, ...);
#define	printf	dbgOutputFunc[g_dbgOutputIndex]

#if defined(_ARM_) || defined(_X86_)
extern BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);
extern BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency);
INLINE uint
osl_getcycles()
{
	LARGE_INTEGER cnt;
	QueryPerformanceCounter(&cnt);
	return cnt.LowPart;
}
#else /* !_MIPS_ && !_ARM_ && !_X86_ */
#define osl_getcycles() 0
#endif 


#define OSL_GETCYCLES(x) ((x) = osl_getcycles())

#if !defined(WLSIM)	  /* shared memory access macros */
#define	R_SM(a)		*(a)
#define	W_SM(a, v)	(*(a) = (v))
#define	BZERO_SM(a, len)	NdisZeroMemory((a), (len))
#endif /* WLSIM */

/* the largest reasonable packet buffer driver uses for ethernet MTU in bytes */
#define	PKTBUFSZ	LBDATASZ

/* packet primitives */
#define	PKTGET(osh, len, send)		osl_pktget((osh), (len), (send))
extern void *osl_pktget(osl_t *osh, uint len, bool send);
#define	PKTFREE(osh, lb, send)		osl_pktfree((osh), (lb), (send))
extern void osl_pktfree(osl_t *osh, void *p, bool send);
#define	PKTDATA(osh, lb)		((struct lbuf *)(lb))->data
#define	PKTLEN(osh, lb)			((struct lbuf *)(lb))->len
#define PKTHEADROOM(osh, lb)		(PKTDATA(osh, lb)-(((struct lbuf *)(lb))->head))
#define PKTTAILROOM(osh, lb)		((((struct lbuf *)(lb))->end)-(((struct lbuf *)(lb))->tail))
#define	PKTNEXT(osh, lb)		((struct lbuf *)(lb))->next
#define	PKTSETNEXT(osh, lb, x)		((struct lbuf *)(lb))->next = (struct lbuf*)(x)
#define	PKTSETLEN(osh, lb, len)		osl_pktsetlen((osh), (lb), (len))
extern void osl_pktsetlen(osl_t *osh, void *lb, uint len);
#define	PKTPUSH(osh, lb, bytes)		osl_pktpush((osh), (lb), (bytes))
extern uchar *osl_pktpush(osl_t *osh, void *lb, uint bytes);
#define	PKTPULL(osh, lb, bytes)		osl_pktpull((osh), (lb), (bytes))
extern uchar *osl_pktpull(osl_t *osh, void *lb, uint bytes);
#define	PKTDUP(osh, lb)			osl_pktdup((osh), (lb))
extern void *osl_pktdup(osl_t *osh, void *lb);
#define	PKTTAG(lb)			(((void *) ((struct lbuf *)(lb))->pkttag))
#define	PKTLINK(lb)			((struct lbuf *)(lb))->link
#define	PKTSETLINK(lb, x)		((struct lbuf *)(lb))->link = (struct lbuf*)(x)
#define	PKTPRIO(lb)			((struct lbuf *)(lb))->priority
#define	PKTSETPRIO(lb, x)		((struct lbuf *)(lb))->priority = (x)
#define	PKTFRMNATIVE(osh, x)		osl_pkt_frmnative((osh), (x))
#define	PKTTONATIVE(osh, lb)		osl_pkt_tonative((osh), (struct lbuf *)(lb))
#define PKTSHARED(lb)                   (0)
#define PKTALLOCED(osh)			(0)
#define PKTSETPOOL(osh, lb, x, y)	do {} while (0)
#define PKTPOOL(osh, lb)		FALSE
#define PKTPOOLLEN(osh, pktp)		(0)
#define PKTPOOLAVAIL(osh, pktp)		(0)
#define PKTPOOLADD(osh, pktp, p)	BCME_ERROR
#define PKTPOOLGET(osh, pktp)		NULL
#define PKTLIST_DUMP(osh, buf)
#ifdef EXT_STA
#define	PKTEXEMPT(lb)			((struct lbuf *)(lb))->exempt
#define PKTSETEXEMPT(lb, x)		((struct lbuf *)(lb))->exempt = (x)
#endif /* EXT_STA */
#define PKTSETHEADROOM(l, x) (l.headroom = x);
extern int osl_pktsumneeded(struct lbuf *lb);
#define PKTSUMNEEDED(lb)		osl_pktsumneeded((lb))

#define LBF_SUM_GOOD	0x1
#define PKTSETSUMGOOD(lb, x)		((struct lbuf *)(lb))->flags = ((x) ? \
						(((struct lbuf *)(lb))->flags | LBF_SUM_GOOD) : \
						(((struct lbuf *)(lb))->flags & ~LBF_SUM_GOOD))
extern void* osl_pkt_frmnative(osl_t *osh, ND_PKT *p);
extern ND_PKT *osl_pkt_tonative(osl_t *osh, struct lbuf *lb);
#define OSL_ERROR(bcmerror)	bcmerror2ndisstatus(bcmerror)

extern shared_info_t *osl_get_sh(osl_t *osh);

/* get system up time in miliseconds */
INLINE long
osl_sys_time()
{
	LARGE_INTEGER timeStamp;
	NdisGetCurrentSystemTime((PLARGE_INTEGER)&timeStamp);
	return (long)(timeStamp.QuadPart / 10000);
}

#define OSL_SYSTIME(x) ((x) = osl_sys_time())

#define OSL_SYSUPTIME()		(0)
#endif	/* _ndis_osl_h_ */
