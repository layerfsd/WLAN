/*
 * Misc system wide definitions
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: bcmdefs.h,v 13.38.4.10.2.7.6.11 2010/02/01 05:51:55 Exp $
 */

#ifndef	_bcmdefs_h_
#define	_bcmdefs_h_

/*
 * One doesn't need to include this file explicitly, gets included automatically if
 * typedefs.h is included.
 */

/* Reclaiming text and data :
 * The following macros specify special linker sections that can be reclaimed
 * after a system is considered 'up'.
 */



#define bcmreclaimed 		0
#define r2_reclaimed		0
#define BCMATTACHDATA(_data)	_data
#define BCMATTACHFN(_fn)	_fn
#define BCMINITDATA(_data)	_data
#define BCMINITFN(_fn)		_fn
#define BCMUNINITFN(_fn)	_fn
#define BCMPREATTACHDATA(_data)	_data
#define BCMPREATTACHFN(_fn)		_fn
#define CONST	const


/* Put some library data/code into ROM to reduce RAM requirements */
#define BCMROMDATA(_data)	_data
#define BCMROMFN(_fn)		_fn
#define BCMROMFN_NAME(_fn)	_fn
#define STATIC	static

/* Bus types */
#define	SI_BUS			0	/* SOC Interconnect */
#define	PCI_BUS			1	/* PCI target */
#define	PCMCIA_BUS		2	/* PCMCIA target */
#define SDIO_BUS		3	/* SDIO target */
#define JTAG_BUS		4	/* JTAG */
#define USB_BUS			5	/* USB (does not support R/W REG) */
#define SPI_BUS			6	/* gSPI target */

/* Allows size optimization for single-bus image */
#ifdef BCMBUSTYPE
#define BUSTYPE(bus) 	(BCMBUSTYPE)
#else
#define BUSTYPE(bus) 	(bus)
#endif

/* Allows size optimization for single-backplane image */
#ifdef BCMCHIPTYPE
#define CHIPTYPE(bus) 	(BCMCHIPTYPE)
#else
#define CHIPTYPE(bus) 	(bus)
#endif


/* Allows size optimization for SPROM support */
#if defined(BCMSPROMBUS)
#define SPROMBUS	(BCMSPROMBUS)
#elif defined(SI_PCMCIA_SROM)
#define SPROMBUS	(PCMCIA_BUS)
#else
#define SPROMBUS	(PCI_BUS)
#endif

/* Allows size optimization for single-chip image */
#ifdef BCMCHIPID
#define CHIPID(chip)	(BCMCHIPID)
#else
#define CHIPID(chip)	(chip)
#endif

/* Defines for DMA Address Width - Shared between OSL and HNDDMA */
#define DMADDR_MASK_32 0x0		/* Address mask for 32-bits */
#define DMADDR_MASK_30 0xc0000000	/* Address mask for 30-bits */
#define DMADDR_MASK_0  0xffffffff	/* Address mask for 0-bits (hi-part) */

#define	DMADDRWIDTH_30  30 /* 30-bit addressing capability */
#define	DMADDRWIDTH_32  32 /* 32-bit addressing capability */
#define	DMADDRWIDTH_63  63 /* 64-bit addressing capability */
#define	DMADDRWIDTH_64  64 /* 64-bit addressing capability */

/* packet headroom necessary to accommodate the largest header in the system, (i.e TXOFF).
 * By doing, we avoid the need  to allocate an extra buffer for the header when bridging to WL.
 * There is a compile time check in wlc.c which ensure that this value is at least as big
 * as TXOFF. This value is used in dma_rxfill (hnddma.c).
 */
#define BCMEXTRAHDROOM 164

/* Headroom required for dongle-to-host communication.  Packets allocated
 * locally in the dongle (e.g. for CDC ioctls or RNDIS messages) should
 * leave this much room in front for low-level message headers which may
 * be needed to get across the dongle bus to the host.  (These messages
 * don't go over the network, so room for the full WL header above would
 * be a waste.).
 */
#define BCMDONGLEHDRSZ 12
#define BCMDONGLEPADSZ 16

#define BCMDONGLEOVERHEAD	(BCMDONGLEHDRSZ + BCMDONGLEPADSZ)

#ifdef BCMDBG

#ifndef BCMDBG_ERR
#define BCMDBG_ERR
#endif /* BCMDBG_ERR */

#ifndef BCMDBG_ASSERT
#define BCMDBG_ASSERT
#endif /* BCMDBG_ASSERT */

#endif /* BCMDBG */

/* Macros for doing definition and get/set of bitfields
 * Usage example, e.g. a three-bit field (bits 4-6):
 *    #define <NAME>_M	BITFIELD_MASK(3)
 *    #define <NAME>_S	4
 * ...
 *    regval = R_REG(osh, &regs->regfoo);
 *    field = GFIELD(regval, <NAME>);
 *    regval = SFIELD(regval, <NAME>, 1);
 *    W_REG(osh, &regs->regfoo, regval);
 */
#define BITFIELD_MASK(width) \
		(((unsigned)1 << (width)) - 1)
#define GFIELD(val, field) \
		(((val) >> field ## _S) & field ## _M)
#define SFIELD(val, field, bits) \
		(((val) & (~(field ## _M << field ## _S))) | \
		 ((unsigned)(bits) << field ## _S))

/* define BCMSMALL to remove misc features for memory-constrained environments */
#ifdef BCMSMALL
#undef	BCMSPACE
#define bcmspace	FALSE	/* if (bcmspace) code is discarded */
#else
#define	BCMSPACE
#define bcmspace	TRUE	/* if (bcmspace) code is retained */
#endif

/* Max. nvram variable table size */
#define	MAXSZ_NVRAM_VARS	4096

#define LOCATOR_EXTERN static

#endif /* _bcmdefs_h_ */
