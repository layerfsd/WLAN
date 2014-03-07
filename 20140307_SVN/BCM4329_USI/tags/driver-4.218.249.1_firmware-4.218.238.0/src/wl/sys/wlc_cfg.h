/*
 * Configuration-related definitions for
 * Broadcom 802.11abg Networking Device Driver
 *
 * Copyright (C) 2011, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wlc_cfg.h,v 1.176.2.19 2011-01-18 00:34:51 $
 */


#ifndef _wlc_cfg_h_
#define _wlc_cfg_h_


#ifndef LINUX_HYBRID
#include "wlconf.h"
#endif




#ifdef DBAND
#define NBANDS(wlc) ((wlc)->pub->_nbands)
#define NBANDS_PUB(pub) ((pub)->_nbands)
#define NBANDS_HW(hw) ((hw)->_nbands)
#else
#define NBANDS(wlc) 1
#define NBANDS_PUB(wlc) 1
#define NBANDS_HW(hw) 1
#endif 

#define IS_SINGLEBAND_5G(device) \
	((device == BCM4306_D11A_ID) || \
	 (device == BCM4311_D11A_ID) || \
	 (device == BCM4318_D11A_ID) || \
	 (device == BCM4321_D11N5G_ID) || \
	 (device == BCM4328_D11A_ID) || \
	 (device == BCM4325_D11A_ID) || \
	 (device == BCM4322_D11N5G_ID) || \
	 (device == BCM43222_D11N5G_ID) || \
	0)



#define WLC_SPLIT


#define D11_DEFAULT	0xffffffb0	

#define D11_DEFAULT2	0x0000000f	

#define APHY_DEFAULT	0x000001ec	
#define GPHY_DEFAULT	0x000003c6	
#define NPHY_DEFAULT	0x000407ff	
#define HTPHY_DEFAULT	0x00000001	
#define LPPHY_DEFAULT	0x0000001f	

#define SSLPNPHY_DEFAULT 0x0000000f	

#define LCNPHY_DEFAULT	0x0000000f	

#define D11CONF2_BASE	32


#ifndef D11CONF
#define D11CONF	D11_DEFAULT
#else

#if !defined(D11CONF2)
#define D11CONF2    0
#endif 
#endif 

#ifndef D11CONF2
#define D11CONF2	D11_DEFAULT2
#endif

#ifndef ACONF
#define ACONF	APHY_DEFAULT
#endif
#ifndef GCONF
#define GCONF	GPHY_DEFAULT
#endif
#ifndef NCONF
#define NCONF	NPHY_DEFAULT
#endif
#ifndef LPCONF
#define LPCONF	LPPHY_DEFAULT
#endif
#ifndef SSLPNCONF
#define SSLPNCONF	SSLPNPHY_DEFAULT
#endif
#ifndef LCNCONF
#define LCNCONF	LCNPHY_DEFAULT
#endif
#ifndef LCN40CONF
#define LCN40CONF	LCNPHY_DEFAULT
#endif
#ifndef HTCONF
#define HTCONF	HTPHY_DEFAULT
#endif



#if GCONF || NCONF || LPCONF || SSLPNCONF || LCNCONF || HTCONF
#define BAND2G
#endif


#if ACONF || defined(DBAND)
#define BAND5G
#endif

#if NCONF || HTCONF
#define WLANTSEL	1
#endif
#if defined(WLAMSDU)
#define WLAMSDU_TX      1
#endif






#if defined(CCX_SDK)
#error "BCMCCX, EXT_STA, NDIS6x0, BCMCCMP and WLCNT must be defined for CCX_SDK"
#endif 



#if defined(APSTA)
#error "APSTA feature requested without both AP and STA defined"
#endif

#if defined(WLDPT)
#if !defined(BCMAUTH_PSK)
#error	"STA, BCMSUP_PSK and BCMAUTH_PSK must be defined for WLDPT"
#endif
#endif	



#if !defined(WL_ASSOC_RECREATE)
#error "WL_ASSOC_RECREATE should be defined for EXT_STA"
#endif

#if defined(CRAM) && !defined(WLAFTERBURNER)
#error "WLAFTERBURNER must be defined when CRAM is defined"
#endif


#if defined(BAND5G) && !defined(WL11H)
#error	"WL11H is required when 5G band support is configured"
#endif


#if !defined(WME) && defined(WLCAC)
#error	"WME support required"
#endif



#if defined(WLPKTDLYSTAT) && !defined(WLMEDIA_EXT)
#error "WLMEDIA_EXT not defined"
#endif





#define CONF_HAS(config, val)	((config) & (1 << (val)))
#define CONF_MSK(config, mask)	((config) & (mask))
#define MSK_RANGE(low, hi)	((1 << ((hi)+1)) - (1 << (low)))
#define CONF_RANGE(config, low, hi) (CONF_MSK(config, MSK_RANGE(low, high)))
#define MSK_RANGE2(low, hi)	((1U << (((hi)-D11CONF2_BASE) + 1)) - (1U << ((low)-D11CONF2_BASE)))
#define CONF_RANGE2(config, low, hi) (CONF_MSK(config, MSK_RANGE2(low, high)))

#define CONF_IS(config, val)	((config) == (1 << (val)))
#define CONF_GE(config, val)	((config) & (0-(1 << (val))))
#define CONF_GT(config, val)	((config) & (0-2*(1 << (val))))
#define CONF_LT(config, val)	((config) & ((1 << (val))-1))
#define CONF_LE(config, val)	((config) & (2*(1 << (val))-1))



#define ACONF_HAS(val)	CONF_HAS(ACONF, val)
#define ACONF_MSK(mask)	CONF_MSK(ACONF, mask)
#define ACONF_IS(val)	CONF_IS(ACONF, val)
#define ACONF_GE(val)	CONF_GE(ACONF, val)
#define ACONF_GT(val)	CONF_GT(ACONF, val)
#define ACONF_LT(val)	CONF_LT(ACONF, val)
#define ACONF_LE(val)	CONF_LE(ACONF, val)

#define GCONF_HAS(val)	CONF_HAS(GCONF, val)
#define GCONF_MSK(mask)	CONF_MSK(GCONF, mask)
#define GCONF_IS(val)	CONF_IS(GCONF, val)
#define GCONF_GE(val)	CONF_GE(GCONF, val)
#define GCONF_GT(val)	CONF_GT(GCONF, val)
#define GCONF_LT(val)	CONF_LT(GCONF, val)
#define GCONF_LE(val)	CONF_LE(GCONF, val)

#define NCONF_HAS(val)	CONF_HAS(NCONF, val)
#define NCONF_MSK(mask)	CONF_MSK(NCONF, mask)
#define NCONF_IS(val)	CONF_IS(NCONF, val)
#define NCONF_GE(val)	CONF_GE(NCONF, val)
#define NCONF_GT(val)	CONF_GT(NCONF, val)
#define NCONF_LT(val)	CONF_LT(NCONF, val)
#define NCONF_LE(val)	CONF_LE(NCONF, val)

#define LPCONF_HAS(val)	CONF_HAS(LPCONF, val)
#define LPCONF_MSK(mask) CONF_MSK(LPCONF, mask)
#define LPCONF_IS(val)	CONF_IS(LPCONF, val)
#define LPCONF_GE(val)	CONF_GE(LPCONF, val)
#define LPCONF_GT(val)	CONF_GT(LPCONF, val)
#define LPCONF_LT(val)	CONF_LT(LPCONF, val)
#define LPCONF_LE(val)	CONF_LE(LPCONF, val)

#define SSLPNCONF_HAS(val)	CONF_HAS(SSLPNCONF, val)
#define SSLPNCONF_MSK(mask)	CONF_MSK(SSLPNCONF, mask)
#define SSLPNCONF_IS(val)	CONF_IS(SSLPNCONF, val)
#define SSLPNCONF_GE(val)	CONF_GE(SSLPNCONF, val)
#define SSLPNCONF_GT(val)	CONF_GT(SSLPNCONF, val)
#define SSLPNCONF_LT(val)	CONF_LT(SSLPNCONF, val)
#define SSLPNCONF_LE(val)	CONF_LE(SSLPNCONF, val)

#define LCNCONF_HAS(val)	CONF_HAS(LCNCONF, val)
#define LCNCONF_MSK(mask)	CONF_MSK(LCNCONF, mask)
#define LCNCONF_IS(val)		CONF_IS(LCNCONF, val)
#define LCNCONF_GE(val)		CONF_GE(LCNCONF, val)
#define LCNCONF_GT(val)		CONF_GT(LCNCONF, val)
#define LCNCONF_LT(val)		CONF_LT(LCNCONF, val)
#define LCNCONF_LE(val)		CONF_LE(LCNCONF, val)

#define LCN40CONF_HAS(val)	CONF_HAS(LCN40CONF, val)
#define LCN40CONF_MSK(mask)	CONF_MSK(LCN40CONF, mask)
#define LCN40CONF_IS(val)	CONF_IS(LCN40CONF, val)
#define LCN40CONF_GE(val)	CONF_GE(LCN40CONF, val)
#define LCN40CONF_GT(val)	CONF_GT(LCN40CONF, val)
#define LCN40CONF_LT(val)	CONF_LT(LCN40CONF, val)
#define LCN40CONF_LE(val)	CONF_LE(LCN40CONF, val)

#define HTCONF_HAS(val)		CONF_HAS(HTCONF, val)
#define HTCONF_MSK(mask)	CONF_MSK(HTCONF, mask)
#define HTCONF_IS(val)		CONF_IS(HTCONF, val)
#define HTCONF_GE(val)		CONF_GE(HTCONF, val)
#define HTCONF_GT(val)		CONF_GT(HTCONF, val)
#define HTCONF_LT(val)		CONF_LT(HTCONF, val)
#define HTCONF_LE(val)		CONF_LE(HTCONF, val)


#define D11CONF_MSK(mask) \
	(CONF_MSK(D11CONF, mask) || \
	CONF_MSK(D11CONF2, mask))
#define D11CONF_HAS(val) \
	((((val) < D11CONF2_BASE) && CONF_HAS(D11CONF, val)) || \
	(((val) >= D11CONF2_BASE) && CONF_HAS(D11CONF2, (val)-D11CONF2_BASE)))
#define D11CONF_IS(val) \
	((((val) < D11CONF2_BASE) && CONF_IS(D11CONF, val)) || \
	(((val) >= D11CONF2_BASE) && CONF_IS(D11CONF2, (val)-D11CONF2_BASE)))
#define D11CONF_GE(val) \
	((((val) < D11CONF2_BASE) && CONF_GE(D11CONF, val)) || \
	(((val) < D11CONF2_BASE) && D11CONF2) || \
	(((val) >= D11CONF2_BASE) && CONF_GE(D11CONF2, (val)-D11CONF2_BASE)))
#define D11CONF_GT(val) \
	((((val) < D11CONF2_BASE) && CONF_GT(D11CONF, val)) || \
	(((val) < D11CONF2_BASE) && D11CONF2) || \
	(((val) >= D11CONF2_BASE) && CONF_GT(D11CONF2, (val)-D11CONF2_BASE)))
#define D11CONF_LT(val) \
	((((val) < D11CONF2_BASE) && CONF_LT(D11CONF, val)) || \
	(((val) >= D11CONF2_BASE) && CONF_LT(D11CONF2, (val)-D11CONF2_BASE)))
#define D11CONF_LE(val) \
	((((val) < D11CONF2_BASE) && CONF_LE(D11CONF, val)) || \
	(((val) >= D11CONF2_BASE) && CONF_LE(D11CONF2, (val)-D11CONF2_BASE)))

#define PHYCONF_HAS(val) CONF_HAS(PHYTYPE, val)
#define PHYCONF_IS(val)	CONF_IS(PHYTYPE, val)



#define AREV_IS(var, val)	(ACONF_HAS(val) && (ACONF_IS(val) || ((var) == (val))))
#define AREV_GE(var, val)	(ACONF_GE(val) && (!ACONF_LT(val) || ((var) >= (val))))
#define AREV_GT(var, val)	(ACONF_GT(val) && (!ACONF_LE(val) || ((var) > (val))))
#define AREV_LT(var, val)	(ACONF_LT(val) && (!ACONF_GE(val) || ((var) < (val))))
#define AREV_LE(var, val)	(ACONF_LE(val) && (!ACONF_GT(val) || ((var) <= (val))))

#define GREV_IS(var, val)	(GCONF_HAS(val) && (GCONF_IS(val) || ((var) == (val))))
#define GREV_GE(var, val)	(GCONF_GE(val) && (!GCONF_LT(val) || ((var) >= (val))))
#define GREV_GT(var, val)	(GCONF_GT(val) && (!GCONF_LE(val) || ((var) > (val))))
#define GREV_LT(var, val)	(GCONF_LT(val) && (!GCONF_GE(val) || ((var) < (val))))
#define GREV_LE(var, val)	(GCONF_LE(val) && (!GCONF_GT(val) || ((var) <= (val))))

#define NREV_IS(var, val)	(NCONF_HAS(val) && (NCONF_IS(val) || ((var) == (val))))
#define NREV_GE(var, val)	(NCONF_GE(val) && (!NCONF_LT(val) || ((var) >= (val))))
#define NREV_GT(var, val)	(NCONF_GT(val) && (!NCONF_LE(val) || ((var) > (val))))
#define NREV_LT(var, val)	(NCONF_LT(val) && (!NCONF_GE(val) || ((var) < (val))))
#define NREV_LE(var, val)	(NCONF_LE(val) && (!NCONF_GT(val) || ((var) <= (val))))

#define HTREV_IS(var, val)	(HTCONF_HAS(val) && (HTCONF_IS(val) || ((var) == (val))))
#define HTREV_GE(var, val)	(HTCONF_GE(val) && (!HTCONF_LT(val) || ((var) >= (val))))
#define HTREV_GT(var, val)	(HTCONF_GT(val) && (!HTCONF_LE(val) || ((var) > (val))))
#define HTREV_LT(var, val)	(HTCONF_LT(val) && (!HTCONF_GE(val) || ((var) < (val))))
#define HTREV_LE(var, val)	(HTCONF_LE(val) && (!HTCONF_GT(val) || ((var) <= (val))))

#define LPREV_IS(var, val)	(LPCONF_HAS(val) && (LPCONF_IS(val) || ((var) == (val))))
#define LPREV_GE(var, val)	(LPCONF_GE(val) && (!LPCONF_LT(val) || ((var) >= (val))))
#define LPREV_GT(var, val)	(LPCONF_GT(val) && (!LPCONF_LE(val) || ((var) > (val))))
#define LPREV_LT(var, val)	(LPCONF_LT(val) && (!LPCONF_GE(val) || ((var) < (val))))
#define LPREV_LE(var, val)	(LPCONF_LE(val) && (!LPCONF_GT(val) || ((var) <= (val))))

#define SSLPNREV_IS(var, val)	(SSLPNCONF_HAS(val) && (SSLPNCONF_IS(val) || ((var) == (val))))
#define SSLPNREV_GE(var, val)	(SSLPNCONF_GE(val) && (!SSLPNCONF_LT(val) || ((var) >= (val))))
#define SSLPNREV_GT(var, val)	(SSLPNCONF_GT(val) && (!SSLPNCONF_LE(val) || ((var) > (val))))
#define SSLPNREV_LT(var, val)	(SSLPNCONF_LT(val) && (!SSLPNCONF_GE(val) || ((var) < (val))))
#define SSLPNREV_LE(var, val)	(SSLPNCONF_LE(val) && (!SSLPNCONF_GT(val) || ((var) <= (val))))

#define LCNREV_IS(var, val)	(LCNCONF_HAS(val) && (LCNCONF_IS(val) || ((var) == (val))))
#define LCNREV_GE(var, val)	(LCNCONF_GE(val) && (!LCNCONF_LT(val) || ((var) >= (val))))
#define LCNREV_GT(var, val)	(LCNCONF_GT(val) && (!LCNCONF_LE(val) || ((var) > (val))))
#define LCNREV_LT(var, val)	(LCNCONF_LT(val) && (!LCNCONF_GE(val) || ((var) < (val))))
#define LCNREV_LE(var, val)	(LCNCONF_LE(val) && (!LCNCONF_GT(val) || ((var) <= (val))))

#define LCN40REV_IS(var, val)	(LCN40CONF_HAS(val) && (LCN40CONF_IS(val) || ((var) == (val))))
#define LCN40REV_GE(var, val)	(LCN40CONF_GE(val) && (!LCN40CONF_LT(val) || ((var) >= (val))))
#define LCN40REV_GT(var, val)	(LCN40CONF_GT(val) && (!LCN40CONF_LE(val) || ((var) > (val))))
#define LCN40REV_LT(var, val)	(LCN40CONF_LT(val) && (!LCN40CONF_GE(val) || ((var) < (val))))
#define LCN40REV_LE(var, val)	(LCN40CONF_LE(val) && (!LCN40CONF_GT(val) || ((var) <= (val))))

#define D11REV_IS(var, val)	(D11CONF_HAS(val) && (D11CONF_IS(val) || ((var) == (val))))
#define D11REV_GE(var, val)	(D11CONF_GE(val) && (!D11CONF_LT(val) || ((var) >= (val))))
#define D11REV_GT(var, val)	(D11CONF_GT(val) && (!D11CONF_LE(val) || ((var) > (val))))
#define D11REV_LT(var, val)	(D11CONF_LT(val) && (!D11CONF_GE(val) || ((var) < (val))))
#define D11REV_LE(var, val)	(D11CONF_LE(val) && (!D11CONF_GT(val) || ((var) <= (val))))

#define PHYTYPE_IS(var, val)	(PHYCONF_HAS(val) && (PHYCONF_IS(val) || ((var) == (val))))



#define CASECHECK(config, val)	if (!(CONF_HAS(config, val))) break
#define CASEMSK(config, mask)	if (!(CONF_MSK(config, mask))) break


#if defined(MBSS)

#define D11REV_ISMBSS4(rev)  (D11REV_GE(rev, 9) && D11REV_LE(rev, 14) && !D11REV_IS(rev, 13))
#define D11REV_ISMBSS16(rev)  (D11REV_GE(rev, 13) && !D11REV_IS(rev, 14))
#else
#define D11REV_ISMBSS4(rev)    (0)
#define D11REV_ISMBSS16(rev)    (0)
#endif 

#if (D11CONF ^ (D11CONF & D11_DEFAULT))
#error "Unsupported MAC revision configured"
#endif
#if (D11CONF2 ^ (D11CONF2 & D11_DEFAULT2))
#error "Unsupported MAC revision2 configured"
#endif
#if (ACONF ^ (ACONF & APHY_DEFAULT))
#error "Unsupported APHY revision configured"
#endif
#if (GCONF ^ (GCONF & GPHY_DEFAULT))
#error "Unsupported GPHY revision configured"
#endif
#if (NCONF ^ (NCONF & NPHY_DEFAULT))
#error "Unsupported NPHY revision configured"
#endif
#if (LPCONF ^ (LPCONF & LPPHY_DEFAULT))
#error "Unsupported LPPHY revision configured"
#endif
#if (LCNCONF ^ (LCNCONF & LCNPHY_DEFAULT))
#error "Unsupported LCNPHY revision configured"
#endif
#if (HTCONF ^ (HTCONF & HTPHY_DEFAULT))
#error "Unsupported HTPHY revision configured"
#endif


#if (!D11CONF && !D11CONF2)
#error "No MAC revisions configured!"
#endif

#if !ACONF && !GCONF && !NCONF && !LPCONF && !SSLPNCONF && !LCNCONF && !HTCONF
#error "No PHY configured!"
#endif


#if ACONF
#define _PHYCONF_A (1 << PHY_TYPE_A)
#else
#define _PHYCONF_A 0
#endif 

#if GCONF
#define _PHYCONF_G (1 << PHY_TYPE_G)
#else
#define _PHYCONF_G 0
#endif 

#if NCONF
#define _PHYCONF_N (1 << PHY_TYPE_N)
#else
#define _PHYCONF_N 0
#endif 

#if LPCONF
#define _PHYCONF_LP (1 << PHY_TYPE_LP)
#else
#define _PHYCONF_LP 0
#endif 

#if SSLPNCONF
#define _PHYCONF_SSLPN (1 << PHY_TYPE_SSN)
#else
#define _PHYCONF_SSLPN 0
#endif 

#if LCNCONF
#define _PHYCONF_LCN (1 << PHY_TYPE_LCN)
#else
#define _PHYCONF_LCN 0
#endif 

#if HTCONF
#define _PHYCONF_HT (1 << PHY_TYPE_HT)
#else
#define _PHYCONF_HT 0
#endif 


#define PHYTYPE (_PHYCONF_A | _PHYCONF_G | _PHYCONF_N | _PHYCONF_LP | \
	_PHYCONF_SSLPN | _PHYCONF_LCN | _PHYCONF_HT)








#define WLCISAPHY(band)		PHYTYPE_IS((band)->phytype, PHY_TYPE_A)
#define WLCISGPHY(band)		PHYTYPE_IS((band)->phytype, PHY_TYPE_G)
#define WLCISNPHY(band)		PHYTYPE_IS((band)->phytype, PHY_TYPE_N)
#define WLCISLPPHY(band)	PHYTYPE_IS((band)->phytype, PHY_TYPE_LP)
#define WLCISSSLPNPHY(band)	PHYTYPE_IS((band)->phytype, PHY_TYPE_SSN)
#define WLCISLCNPHY(band)	PHYTYPE_IS((band)->phytype, PHY_TYPE_LCN)
#define WLCISHTPHY(band)	PHYTYPE_IS((band)->phytype, PHY_TYPE_HT)

#define WLC_PHY_11N_CAP(band)	(WLCISNPHY(band) || WLCISSSLPNPHY(band) || \
				WLCISLCNPHY(band) || WLCISHTPHY(band))




#ifndef NTXD
#define NTXD		256   
#endif 
#ifndef NRXD
#define NRXD		256   
#endif 

#ifndef NRXBUFPOST
#define	NRXBUFPOST	32		
#endif 

#ifndef MAXSCB				      
#define MAXSCB		32		
#endif 

#ifndef AMPDU_NUM_MPDU
#define AMPDU_NUM_MPDU		16	
#endif 


#ifndef MAXPKTCB
#define MAXPKTCB	MAXSCB	
#endif 

#ifndef WLC_MAXDPT
#ifdef WLDPT
#define WLC_MAXDPT	4		
#else
#define WLC_MAXDPT	0		
#endif 
#endif 

#ifndef CTFPOOLSZ
#define CTFPOOLSZ       128
#endif 


#define WLC_MAX_UCODE_BSS	(16)		
#define WLC_MAX_UCODE_BSS4	(4)		
#ifndef WLC_MAXBSSCFG
#define WLC_MAXBSSCFG		(1 + WLC_MAXDPT)	
#endif 

#ifndef MAXBSS
#define MAXBSS		64	
#endif 

#ifndef WLC_DATAHIWAT
#define WLC_DATAHIWAT		50	
#endif 

#ifndef WLC_AMPDUDATAHIWAT
#define WLC_AMPDUDATAHIWAT 255
#endif 


#ifndef RXBND
#define RXBND		8	
#endif	
#ifndef TXSBND
#define TXSBND		8	
#endif	


#if defined(WL11H) && defined(BAND5G)
#define RADAR
#endif 

#if defined(BAND5G)
#define BAND_5G(bt)	((bt) == WLC_BAND_5G)
#else
#define BAND_5G(bt)	0
#endif

#if defined(BAND2G)
#define BAND_2G(bt)	((bt) == WLC_BAND_2G)
#else
#define BAND_2G(bt)	0
#endif


#if defined(DBAND) || (defined(LPCONF) && (LPCONF & 1))
#define WLBANDINITDATA(_data)	_data
#define WLBANDINITFN(_fn)	_fn
#else
#define WLBANDINITDATA(_data)	_data
#define WLBANDINITFN(_fn)	_fn
#endif


#define FIPS_ENAB(wlc) 0


#ifdef WLANTSEL
#define WLANTSEL_ENAB(wlc)	1
#else
#define WLANTSEL_ENAB(wlc)	0
#endif 

#ifdef WLAMSDU_TX
#define WLAMSDU_TX_ENAB(wlc)    1
#else
#define WLAMSDU_TX_ENAB(wlc)    0
#endif

#define WLC_TXC_ENAB(wlc)	1

#ifdef BCMWAPI_WPI
#define WAPI_HW_ENAB(wlc)       ((D11REV_GE((wlc)->pub->corerev, 24)) ? 1 : 0)
#else
#define WAPI_HW_ENAB(wlc)       0
#endif

#define FORCE_USE_P2P_UCODE(wlc_pub)	0

#endif 
