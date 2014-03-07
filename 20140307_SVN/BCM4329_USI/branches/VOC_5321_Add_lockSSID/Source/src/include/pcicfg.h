/*
 * pcicfg.h: PCI configuration constants and structures.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: pcicfg.h,v 1.41.12.3 2008/06/26 22:49:41 Exp $
 */

#ifndef	_h_pcicfg_
#define	_h_pcicfg_

/* A structure for the config registers is nice, but in most
 * systems the config space is not memory mapped, so we need
 * field offsetts. :-(
 */
#define	PCI_CFG_VID		0
#define	PCI_CFG_CMD		4
#define	PCI_CFG_REV		8
#define	PCI_CFG_BAR0		0x10
#define	PCI_CFG_BAR1		0x14
#define	PCI_BAR0_WIN		0x80	/* backplane addres space accessed by BAR0 */
#define	PCI_INT_STATUS		0x90	/* PCI and other cores interrupts */
#define	PCI_INT_MASK		0x94	/* mask of PCI and other cores interrupts */
/* PCIE Enhanced CAPABILITY DEFINES */
#define PCIE_EXTCFG_OFFSET	0x100
#define	PCI_BAR0_PCIREGS_OFFSET	(6 * 1024)	/* bar0 + 6K accesses pci core registers */
#define	PCI_BAR0_PCISBR_OFFSET	(4 * 1024)	/* pci core SB registers are at the end of the
						 * 8KB window, so their address is the "regular"
						 * address plus 4K
						 */

#define PCI_BAR0_WINSZ		(16 * 1024)	/* bar0 window size Match with corerev 13 */

/* On pci corerev >= 13 and all pcie, the bar0 is now 16KB and it maps: */
#define	PCI_16KB0_PCIREGS_OFFSET (8 * 1024)	/* bar0 + 8K accesses pci/pcie core registers */
#define	PCI_16KB0_CCREGS_OFFSET	(12 * 1024)	/* bar0 + 12K accesses chipc core registers */
#define PCI_16KBB0_WINSZ	(16 * 1024)	/* bar0 window size */
/* PCI_INT_STATUS */
#if 0 || \
	defined(BCMDBG_ERR) || defined(BCMDBG_ASSERT) || defined(BCMDBG_DUMP)
#define	PCI_SBIM_STATUS_SERR	0x4	/* backplane SBErr interrupt status */

/* PCI_INT_MASK */
#define	PCI_SBIM_SHIFT		8	/* backplane core interrupt mask bits offset */
#define	PCI_SBIM_MASK		0xff00	/* backplane core interrupt mask */
#define	PCI_SBIM_MASK_SERR	0x4	/* backplane SBErr interrupt mask */

/* PCI_SPROM_CONTROL */
#define SPROM_SZ_MSK		0x02	/* SPROM Size Mask */
#define SPROM_LOCKED		0x08	/* SPROM Locked */
#define	SPROM_BLANK		0x04	/* indicating a blank SPROM */
#define SPROM_WRITEEN		0x10	/* SPROM write enable */
#define SPROM_BOOTROM_WE	0x20	/* external bootrom write enable */
#define SPROM_BACKPLANE_EN	0x40	/* Enable indirect backplane access */
#define SPROM_OTPIN_USE		0x80	/* device OTP In use */

#define	SPROM_SIZE		256	/* sprom size in 16-bit */
#define SPROM_CRC_RANGE		64	/* crc cover range in 16-bit */

/* Bits in PCI command and status regs */
#define PCI_CMD_IO		0x00000001	/* I/O enable */
#define PCI_CMD_MEMORY		0x00000002	/* Memory enable */
#define PCI_CMD_MASTER		0x00000004	/* Master enable */
#define PCI_CMD_SPECIAL		0x00000008	/* Special cycles enable */
#define PCI_CMD_INVALIDATE	0x00000010	/* Invalidate? */
#define PCI_CMD_VGA_PAL		0x00000040	/* VGA Palate */
#define PCI_STAT_TA		0x08000000	/* target abort status */
#endif 
#endif	/* _h_pcicfg_ */
