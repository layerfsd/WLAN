/*
 * BCM43XX PCI/E core sw API definitions.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nicpci.h,v 13.1.20.1 2008/05/07 19:52:33 Exp $
 */

#ifndef	_NICPCI_H
#define	_NICPCI_H

#if 1 || (defined(BCMBUSTYPE) && (BCMBUSTYPE == SI_BUS))
#define pcicore_find_pci_capability(a, b, c, d) (0)
#define pcie_readreg(a, b, c, d) (0)
#define pcie_writereg(a, b, c, d, e) (0)

#define pcie_clkreq(a, b, c) (0)

#define pcicore_init(a, b, c) (0x0dadbeef)
#define pcicore_deinit(a)	do { } while (0)
#define pcicore_attach(a, b, c)	do { } while (0)
#define pcicore_hwup(a)		do { } while (0)
#define pcicore_up(a, b)	do { } while (0)
#define pcicore_sleep(a)	do { } while (0)
#define pcicore_down(a, b)	do { } while (0)

#define pcie_war_ovr_aspm_disable(a)	do { } while (0)

#ifdef BCMDBG
#define pcie_lcreg(a, b, c) (0)
#define pcicore_dump(a, b)	do { } while (0)
#endif

#define pcicore_pmecap_fast(a)	(FALSE)
#define pcicore_pmeen(a)	do { } while (0)
#define pcicore_pmeclr(a)	(FALSE)
#else
struct sbpcieregs;

extern uint8 pcicore_find_pci_capability(osl_t *osh, uint8 req_cap_id,
                                         uchar *buf, uint32 *buflen);
extern uint pcie_readreg(osl_t *osh, struct sbpcieregs *pcieregs, uint addrtype, uint offset);
extern uint pcie_writereg(osl_t *osh, struct sbpcieregs *pcieregs, uint addrtype, uint offset,
                          uint val);

extern uint8 pcie_clkreq(void *pch, uint32 mask, uint32 val);

extern void *pcicore_init(si_t *sih, osl_t *osh, void *regs);
extern void pcicore_deinit(void *pch);
extern void pcicore_attach(void *pch, char *pvars, int state);
extern void pcicore_hwup(void *pch);
extern void pcicore_up(void *pch, int state);
extern void pcicore_sleep(void *pch);
extern void pcicore_down(void *pch, int state);

extern void pcie_war_ovr_aspm_disable(void *pch);

#ifdef BCMDBG
extern uint32 pcie_lcreg(void *pch, uint32 mask, uint32 val);
extern void pcicore_dump(void *pch, struct bcmstrbuf *b);
#endif

extern bool pcicore_pmecap_fast(osl_t *osh);
extern void pcicore_pmeen(void *pch);
extern bool pcicore_pmeclr(void *pch);
#endif 

#endif	/* _NICPCI_H */
