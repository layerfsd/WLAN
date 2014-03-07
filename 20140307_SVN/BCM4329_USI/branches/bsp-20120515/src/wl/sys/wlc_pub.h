/*
 * Common (OS-independent) definitions for
 * Broadcom 802.11abg Networking Device Driver
 *
 * Copyright (C) 2011, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: wlc_pub.h,v 1.426.2.64 2011-02-02 19:39:15 $
 */


#ifndef _wlc_pub_h_
#define _wlc_pub_h_

#include <wlc_types.h>


#define	MAX_TIMERS	(29 + (2 * WLC_MAXDPT))		

#define	WLC_NUMRATES	16	
#define	MAXMULTILIST	32	
#define	D11_PHY_HDR_LEN	6	

#ifndef LINUX_WLUSER_POSTMOGRIFY_REMOVAL

#define	PHY_TYPE_A	0	
#define	PHY_TYPE_G	2	
#define	PHY_TYPE_N	4	
#define	PHY_TYPE_LP	5	
#define	PHY_TYPE_SSN	6	
#define	PHY_TYPE_LCN	8	
#define	PHY_TYPE_LCNXN	9	
#define	PHY_TYPE_HT	7	


#define WLC_10_MHZ	10	
#define WLC_20_MHZ	20	
#define WLC_40_MHZ	40	

#define CHSPEC_WLC_BW(chanspec)	(CHSPEC_IS40(chanspec) ? WLC_40_MHZ : \
				 CHSPEC_IS20(chanspec) ? WLC_20_MHZ : \
							 WLC_10_MHZ)
#endif 

#define	WLC_RSSI_MINVAL		-200	
#define	WLC_RSSI_NO_SIGNAL	-91	
#define	WLC_RSSI_VERY_LOW	-80	
#define	WLC_RSSI_LOW		-70	
#define	WLC_RSSI_GOOD		-68	
#define	WLC_RSSI_VERY_GOOD	-58	
#define	WLC_RSSI_EXCELLENT	-57	

#ifndef LINUX_POSTMOGRIFY_REMOVAL
#define WLC_PHYTYPE(_x) (_x) 

#define MA_WINDOW_SZ		8	

#define WLC_SNR_INVALID		0	


#define WLC_TXPWR_MAX		(127)	


#define	ANT_RX_DIV_FORCE_0		0	
#define	ANT_RX_DIV_FORCE_1		1	
#define	ANT_RX_DIV_START_1		2	
#define	ANT_RX_DIV_START_0		3	
#define	ANT_RX_DIV_ENABLE		3	
#define ANT_RX_DIV_DEF		ANT_RX_DIV_START_0	


#define ANT_TX_FORCE_0		0	
#define ANT_TX_FORCE_1		1	
#define ANT_TX_LAST_RX		3	
#define ANT_TX_DEF			3	

#define TXCORE_POLICY_ALL	0x1	


#define TXCHAIN_DEF		0x1	
#define TXCHAIN_DEF_NPHY	0x3	
#define TXCHAIN_DEF_HTPHY	0x7	
#define RXCHAIN_DEF		0x1	
#define RXCHAIN_DEF_NPHY	0x3	
#define RXCHAIN_DEF_HTPHY	0x7	
#define ANTSWITCH_NONE		0	
#define ANTSWITCH_TYPE_1	1	
#define ANTSWITCH_TYPE_2	2	
#define ANTSWITCH_TYPE_3	3	

#define RXBUFSZ		PKTBUFSZ
#ifndef AIDMAPSZ
#define AIDMAPSZ	(ROUNDUP(MAXSCB, NBBY)/NBBY)	
#endif 
#endif 


struct wlc_info;
struct wlc_hw_info;
struct wlc_bsscfg;
struct wlc_if;

typedef struct wlc_tunables {
	int ntxd;			
	int nrxd;			
	int rxbufsz;		
	int nrxbufpost;		
	int maxscb;			
	int ampdunummpdu;	
	int maxpktcb;		
	int maxdpt;			
	int maxucodebss;	
	int maxucodebss4;	
	int maxbss;			
	int datahiwat;		
	int ampdudatahiwat;	
	int rxbnd;			
	int txsbnd;			
	int memreserved;	
	int eventsz;		
	int pkt_maxsegs;	
	uint32 rx_ba_max_size;	
	int amsdu_resize_buflen;	
} wlc_tunables_t;

typedef struct wlc_rateset {
	uint	count;			
	uint8	rates[WLC_NUMRATES];	
	uint8	htphy_membership;	
	uint8	mcs[MCSSET_LEN];	
} wlc_rateset_t;

struct rsn_parms {
	uint8 flags;		
	uint8 multicast;	
	uint8 ucount;		
	uint8 unicast[4];	
	uint8 acount;		
	uint8 auth[4];		
	uint8 PAD[4];		
};

#ifndef LINUX_POSTMOGRIFY_REMOVAL

#define SSID_FMT_BUF_LEN	((4 * DOT11_MAX_SSID_LEN) + 1)


#define RSN_FLAGS_SUPPORTED		0x1 
#define RSN_FLAGS_PREAUTH		0x2 
#define RSN_FLAGS_FBT		0x4 


#define AMPDU_DEF_MPDU_DENSITY	6	


#define HT_CAP	((HT_CAP_MIMO_PS_OFF << HT_CAP_MIMO_PS_SHIFT) | HT_CAP_40MHZ | \
		  HT_CAP_GF | HT_CAP_MAX_AMSDU | HT_CAP_DSSS_CCK)
#endif 


typedef void *wlc_pkt_t;


typedef struct wlc_event {
	wl_event_msg_t	event;		
	struct ether_addr *addr;	
	int		bsscfgidx;      
	struct wlc_if	*wlcif;		
	void		*data;		
	struct wlc_event *next;		
} wlc_event_t;


typedef struct wlc_bss_info
{
	struct ether_addr BSSID;	
	uint16		flags;		
	uint8		SSID_len;	
	uint8		SSID[32];	
	int16		RSSI;		
	int16		SNR;		
	uint16		beacon_period;	
	uint16		atim_window;	
	chanspec_t	chanspec;	
	int8		infra;		
	wlc_rateset_t	rateset;	
	uint8		dtim_period;	
	int8		phy_noise;	
	uint16		capability;	
#ifdef WLSCANCACHE
	uint32		timestamp;	
#endif
	struct dot11_bcn_prb *bcn_prb;	
	uint16		bcn_prb_len;	
	uint8		wme_qosinfo;	
	struct rsn_parms wpa;
	struct rsn_parms wpa2;
#ifdef BCMWAPI_WAI
	struct rsn_parms wapi;
#endif 
	uint16		qbss_load_aac;	
	
	uint8		qbss_load_chan_free;	
	uint8		mcipher;	
	uint8		wpacfg;		
#ifdef WLFBT
	uint16		mdid;		
#endif
} wlc_bss_info_t;


typedef struct wlc_pub {
	void		*wlc;
	uint		unit;			
	uint		corerev;		
	osl_t		*osh;			
	si_t		*sih;			
	char		*vars;			
	bool		up;			
	bool		hw_off;			
	wlc_tunables_t *tunables;		
	bool		hw_up;			
	bool		_piomode;		 
	uint		_nbands;		
	uint		now;			

	bool		promisc;		
	bool		delayed_down;		
	bool		_ap;			
	bool		_apsta;			
	bool		_assoc_recreate;	
	int		_wme;			
	uint8		_mbss;			
#ifdef WLDPT
	bool		_dpt;			
#endif
#ifdef WLBDD
	bool		_bdd;			
#endif
#ifdef WLMCHAN
	bool            _mchan;                 
	bool            _mchan_active;          
#endif
	bool		allmulti;		
	bool		associated;		
						
	bool            phytest_on;             
	bool		bf_preempt_4306;	

#ifdef WLAMPDU
	bool		_ampdu;			
#endif
#ifdef WLAMSDU
	bool		_amsdu_tx;		
#endif
#ifdef WLBA
	bool		_ba;			
#endif
	bool		_cac;			
#ifdef CRAM
	bool		_cram;			
#endif
#ifdef WL11K
	bool		_rrm;			
#endif
	bool		_n_enab;		
	bool		_n_reqd;		
	int		_ht_enab;

	int8		_coex;			
	bool		_priofc;		

	struct ether_addr	cur_etheraddr;	

	struct ether_addr	*multicast; 	
	uint		nmulticast;		

	uint32		wlfeatureflag;		
	int		psq_pkts_total;		

#ifdef WLLED
	uint		_activity;		
#endif

	uint16		txmaxpkts;		

	
	uint32		swdecrypt;		

	int 		bcmerror;		

	mbool		radio_disabled;		
	bool		radio_active;		
#if defined(WME_PER_AC_TX_PARAMS)
	bool		_per_ac_maxrate;	
#endif
	uint16		roam_time_thresh;	
	bool		align_wd_tbtt;		
	uint		sdiod_drive_strength;	

	uint16		boardrev;		
	uint8		sromrev;		
	uint32		boardflags;		
	uint32		boardflags2;		
	bool		tempsense_disable;	

#ifdef WLCNT
	wl_cnt_t	*_cnt;			
	wl_wme_cnt_t	*_wme_cnt;		
#endif 

	bool		_win7;			
	bool		_extsta;		
	bool		_pkt_filter;		

	bool		_lmac;			
	bool		_lmacproto;		
	bool		phy_11ncapable;		
#ifdef WIFI_ACT_FRAME
	bool		_wifiact_frame_enab;	
#endif
	bool		_ampdumac;	
	bool		wdstop;			
#ifdef BCMAUTH_PSK
	bool		_bcmauth_psk;
#endif
#ifdef WLPFN
	bool		_wlpfn;
#endif
#ifdef IBSS_PEER_GROUP_KEY
	bool		_ibss_peer_group_key;
#endif
#ifdef IBSS_PEER_DISCOVERY_EVENT
	bool		_ibss_peer_discovery_event;
#endif
#ifdef IBSS_PEER_MGMT
	bool		_ibss_peer_mgmt;
#endif
#ifdef PROP_TXSTATUS
	bool		_proptxstatus;
#endif
#if !defined(WLNOEIND)
	bool		_wleind;
#endif
#ifdef DMATXRC
	bool		_dmatxrc;
#endif
#ifdef WLMEDIA_EN
	bool		_wlmedia;
#endif 
	bool		_fbt;			
	uint		driverrev;		

	bool		phy_bw40_capable;	
	uint8		rtmerge;		
#ifdef	WL_MULTIQUEUE
	bool		_mqueue;
#endif 
} wlc_pub_t;

#define WLC_RTMERGE_PRECMASK	0x01
#define WLC_RTMERGE_FLOWBITS	0x02
#define WLC_RTMERGE_SCANSTUCK	0x04


typedef struct	wl_rxsts {
	uint	pkterror;		
	uint	phytype;		
	uint	channel;		
	chanspec_t chanspec;		
	uint	datarate;		
	uint8	mcs;			
	uint8	htflags;		
	uint	antenna;		
	uint	pktlength;		
	uint32	mactime;		
	uint	sq;			
	int32	signal;			
	int32	noise;			
	uint	preamble;		
	uint	encoding;		
	uint	nfrmtype;		
	struct wl_if *wlif;		
} wl_rxsts_t;

#ifndef LINUX_POSTMOGRIFY_REMOVAL

#define WL_RXS_CRC_ERROR		0x00000001 
#define WL_RXS_RUNT_ERROR		0x00000002 
#define WL_RXS_ALIGN_ERROR		0x00000004 
#define WL_RXS_OVERSIZE_ERROR		0x00000008 
#define WL_RXS_WEP_ICV_ERROR		0x00000010 
#define WL_RXS_WEP_ENCRYPTED		0x00000020 
#define WL_RXS_PLCP_SHORT		0x00000040 
#define WL_RXS_DECRYPT_ERR		0x00000080 
#define WL_RXS_OTHER_ERR		0x80000000 


#define WL_RXS_PHY_A			0x00000000 
#define WL_RXS_PHY_B			0x00000001 
#define WL_RXS_PHY_G			0x00000002 
#define WL_RXS_PHY_N			0x00000004 


#define WL_RXS_ENCODING_UNKNOWN		0x00000000
#define WL_RXS_ENCODING_DSSS_CCK	0x00000001 
#define WL_RXS_ENCODING_OFDM		0x00000002 
#define WL_RXS_ENCODING_HT		0x00000003 


#define WL_RXS_UNUSED_STUB		0x0		
#define WL_RXS_PREAMBLE_SHORT		0x00000001	
#define WL_RXS_PREAMBLE_LONG		0x00000002	
#define WL_RXS_PREAMBLE_HT_MM           0x00000003      
#define WL_RXS_PREAMBLE_HT_GF           0x00000004      


#define WL_RXS_HTF_40			0x01
#define WL_RXS_HTF_20L			0x02
#define WL_RXS_HTF_20U			0x04
#define WL_RXS_HTF_SGI			0x08
#define WL_RXS_HTF_STBC_MASK		0x30
#define WL_RXS_HTF_STBC_SHIFT		4
#define WL_RXS_HTF_LDPC			0x40

#define WL_RXS_NFRM_AMPDU_FIRST		0x00000001 
#define WL_RXS_NFRM_AMPDU_SUB		0x00000002 
#define WL_RXS_NFRM_AMSDU_FIRST		0x00000004 
#define WL_RXS_NFRM_AMSDU_SUB		0x00000008 
#endif 

#ifndef LINUX_POSTMOGRIFY_REMOVAL

typedef struct {
	uint32		flags;		
	uint16		seq;		
	uint8		callbackidx;	
	int8		_bsscfgidx;	
	int8		ac_override;	
	int8		rssi;		
	uint16		flags2;		
	uint32		exptime;	
	struct scb	*_scb;		
	uint32		rspec;		
	union {
		uint32		packetid;
#ifdef WLPKTDLYSTAT
		uint32 		enqtime;	
#endif
	} shared;
#if defined(PROP_TXSTATUS)
	
	uint32		wl_hdr_information;
#endif
#ifdef WLMEDIA_HTSF
	bool ucode_tstamp_en;
#endif
} wlc_pkttag_t;

#define WLPKTTAG(p) ((wlc_pkttag_t*)PKTTAG(p))


#define WLF_PSMARK		0x00000001	
#define WLF_PSDONTQ		0x00000002	
#define WLF_MPDU		0x00000004	
#define WLF_NON8023		0x00000008	
#define WLF_8021X		0x00000010	
#define WLF_APSD		0x00000020	
#define WLF_AMSDU		0x00000040	
#define WLF_HWAMSDU		0x00000080	
#define WLF_TXHDR		0x00000080	
#define WLF_FIFOPKT             0x00000100      
#define WLF_BA			0x00000100	
#define WLF_EXPTIME		0x00000200	
#define WLF_AMPDU_MPDU		0x00000400	
#define WLF_MIMO		0x00000800	
#define WLF_RIFS		0x00001000	
#define WLF_VRATE_PROBE		0x00002000	
#define WLF_BSS_DOWN		0x00004000	
#define WLF_UMAC_PKT		0x00008000	
#define WLF_EXEMPT_MASK		0x00030000	
#define WLF_WME_NOACK		0x00040000	
#ifdef PROP_TXSTATUS
#define WLF_CREDITED	0x00100000	
#endif
#if defined(BCMPKTPOOL) && defined(DMATXRC)
#define WLF_PHDR		0x00080000      
#endif 
#define WLF_DPT_TYPE		0x00800000	
#define WLF_DPT_DIRECT		0x01000000	
#define WLF_DPT_APPATH		0x02000000	
#define WLF_USERTS		0x04000000	
#define WLF_RATE_AUTO		0x08000000	


#define WLF_DATA		0x40000000	

#ifdef BCMWAPI_WAI
#define WLF_WAI			0x80000000	
#endif


#define WLF2_TYPE_MASK		0x000f		
#define WLF2_SUPR		0x0010		
#define WLF2_NO_PMCHANGE	0x0040		


#define WLF2_TYPE_APSD		1	
#define WLF2_TYPE_PSP_RSP	2	
#define WLF2_TYPE_PRB		3	
#define WLF2_TYPE_PSP		4	
#define WLF2_TYPE_AF		5	
#define WLF2_TYPE_TKIP_CM	7	
#define WLF2_TYPE_RATE		8	


#define WLPKTFLAG2_TYPEGET(pkt)		(WLPKTTAG(pkt)->flags2 & WLF2_TYPE_MASK)
#define WLPKTFLAG2_TYPESET(pkt, cbn)	(WLPKTTAG(pkt)->flags2 &= ~WLF2_TYPE_MASK, \
					 WLPKTTAG(pkt)->flags2 |= (cbn))
#define WLPKTFLAG2_TYPECLR(pkt)		(WLPKTTAG(pkt)->flags2 &= ~WLF2_TYPE_MASK)

#include <packed_section_start.h>
typedef BWL_PRE_PACKED_STRUCT struct {
	uint phy_type;
	uint8 channel;
	long rssi;
	int rate;
	uint8 numfrags;
	bool prepend_dot11_hdr;
	struct dot11_header	dot11_hdr;
}
BWL_POST_PACKED_STRUCT rx_ctxt_t;
#include <packed_section_end.h>

typedef struct {
	uint16 exempt_type;	
	uint16 exempt_action;	
	uint16 exempt_pkttype;	
} exempt_t;

typedef struct {
	uint entries;		
	exempt_t exempt[1];	
} exempt_list_t;

typedef enum {
	WLC_EXEMPT_ALWAYS,
	WLC_EXEMPT_NO_KEY,
	WLC_EXEMPT_ACTION_MAX
} exempt_action_t;

typedef enum {
	WLC_EXEMPT_UNICAST,
	WLC_EXEMPT_MULTICAST,
	WLC_EXEMPT_BOTH,
	WLC_EXEMPT_PKTTYPE_MAX
} exempt_pkttype_t;

#define WLC_EXEMPT_LIST_LEN(entries) (OFFSETOF(exempt_list_t, exempt) + \
	(sizeof(exempt_t) * entries))

#ifdef WLBA
#define WLPKTFLAG_BA(pkttag)	((pkttag)->flags & WLF_BA)
#else
#define WLPKTFLAG_BA(pkttag)	FALSE
#endif

#ifdef WLAMPDU
#define WLPKTFLAG_AMPDU(pkttag)	((pkttag)->flags & WLF_AMPDU_MPDU)
#else
#define WLPKTFLAG_AMPDU(pkttag)	FALSE
#endif

#ifdef WLAMSDU
#define WLPKTFLAG_AMSDU(pkttag)	((pkttag)->flags & WLF_AMSDU)
#else
#define WLPKTFLAG_AMSDU(pkttag)	FALSE
#endif

#define WLPKTFLAG_PMF(pkttag)	FALSE

#ifdef WLDPT
#define WLPKTFLAG_DPT(pkttag)	((pkttag)->flags & WLF_DPT_TYPE)
#else
#define WLPKTFLAG_DPT(pkttag)	FALSE
#endif


#define WLPKTFLAG_RIFS(pkttag)	((pkttag)->flags & WLF_RIFS)

#define WLPKTFLAG_BSS_DOWN_GET(pkttag) ((pkttag)->flags & WLF_BSS_DOWN)
#define WLPKTFLAG_BSS_DOWN_SET(pkttag, val) (pkttag)->flags |= ((val) ? WLF_BSS_DOWN : 0)

#define WLPKTFLAG_EXEMPT_GET(pkttag) (((pkttag)->flags & WLF_EXEMPT_MASK) >> 16)
#define WLPKTFLAG_EXEMPT_SET(pkttag, val) ((pkttag)->flags = \
			((pkttag)->flags & ~WLF_EXEMPT_MASK) | (val << 16));
#define WLPKTFLAG_NOACK(pkttag)	((pkttag)->flags & WLF_WME_NOACK)


#define BSSCFGIDX_ISVALID(bsscfgidx) (((bsscfgidx >= 0)&&(bsscfgidx < WLC_MAXBSSCFG)) ? 1 : 0)

static INLINE int8
wlc_pkttag_bsscfg_get(void *p)
{
	int8 idx = WLPKTTAG(p)->_bsscfgidx;
#ifdef BCMDBG
	ASSERT(BSSCFGIDX_ISVALID(idx));
#endif
	return idx;
}

#define WLPKTTAGBSSCFGGET(p) (wlc_pkttag_bsscfg_get(p))
#define WLPKTTAGBSSCFGSET(p, bsscfgidx) (WLPKTTAG(p)->_bsscfgidx = bsscfgidx)


#define WLPKTTAG_BSSIDX_GET(pkttag) ((pkttag)->_bsscfgidx)
#endif 




#define	AP_ENAB(pub)	(0)


#define APSTA_ENAB(pub)	(0)


#define STA_ONLY(pub)	(!AP_ENAB(pub))
#define AP_ONLY(pub)	(AP_ENAB(pub) && !APSTA_ENAB(pub))


#if defined(PROP_TXSTATUS)
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define PROP_TXSTATUS_ENAB(pub)		((pub)->_proptxstatus)
	#elif defined(PROP_TXSTATUS_ENABLED)
		#define PROP_TXSTATUS_ENAB(pub)		1
	#else
		#define PROP_TXSTATUS_ENAB(pub)		0
	#endif
	#else
		#define PROP_TXSTATUS_ENAB(pub)		0
#endif 

#ifndef LINUX_POSTMOGRIFY_REMOVAL


#if defined(MBSS)
#define MBSS_OFF		0
#define MBSS4_ENABLED	1
#define MBSS16_ENABLED	2
#define MBSS_ENAB(pub) ((pub)->_mbss)
#define MBSS_ENAB4(pub) ((pub)->_mbss == MBSS4_ENABLED)
#define MBSS_ENAB16(pub) ((pub)->_mbss == MBSS16_ENABLED)
#else 
#define MBSS_ENAB(pub) (0)
#define MBSS_ENAB4(pub) (0)
#define MBSS_ENAB16(pub) (0)
#endif 

#if defined(WME_PER_AC_TX_PARAMS)
#define WME_PER_AC_TX_PARAMS_ENAB(pub) (1)
#define WME_PER_AC_MAXRATE_ENAB(pub) ((pub)->_per_ac_maxrate)
#else 
#define WME_PER_AC_TX_PARAMS_ENAB(pub) (0)
#define WME_PER_AC_MAXRATE_ENAB(pub) (0)
#endif 

#define ENAB_1x1	0x01
#define ENAB_2x2	0x02
#define ENAB_3x3	0x04
#define ENAB_4x4	0x08
#define SUPPORT_11N	(ENAB_1x1|ENAB_2x2)
#define SUPPORT_HT	(ENAB_1x1|ENAB_2x2|ENAB_3x3)

#if ((defined(NCONF) && (NCONF != 0)) || (defined(SSLPNCONF) && (SSLPNCONF != 0)) || \
	(defined(LCNCONF) && (LCNCONF != 0)) || (defined(HTCONF) && (HTCONF != 0)))
#define N_ENAB(pub) ((pub)->_n_enab & SUPPORT_11N)
#define N_REQD(pub) ((pub)->_n_reqd)
#else
#define N_ENAB(pub)	0
#define N_REQD(pub)	0
#endif

#if (defined(HTCONF) && (HTCONF != 0))
#define HT_ENAB(pub) (((pub)->_n_enab & SUPPORT_HT) == SUPPORT_HT)
#else
#define HT_ENAB(pub) 0
#endif


#ifdef WLBA
#define WLBA_ENAB(pub) ((pub)->_ba)
#else
#define WLBA_ENAB(pub) 0
#endif 


#ifdef CRAM
#define CRAM_ENAB(pub) ((pub)->_cram)
#else
#define CRAM_ENAB(pub) 0
#endif 


#ifdef WLAMSDU
#define AMSDU_ENAB(pub) ((pub)->_amsdu_tx)
#else
#define AMSDU_ENAB(pub) 0
#endif 


#ifdef WLAMPDU
#define AMPDU_ENAB(pub) ((pub)->_ampdu)
#else
#define AMPDU_ENAB(pub) 0
#endif 

#define AMPDU_AGG_AUTO  -1
#define AMPDU_AGG_OFF   0

#define AMPDU_AGG_HOST  1
#define AMPDU_AGG_MAC   2
#define AMPDU_AGG_UCODE 1
#define AMPDU_AGG_HW    2

#define AMPDU_AGG_ON		1
#define AMPDU_AGGMODE_HOST	1
#define AMPDU_AGGMODE_MAC	2



#ifdef WLAMPDU_MAC
	#if ((defined(WLAMPDU_HW) && defined(WLAMPDU_UCODE)) || defined(BCMROMBUILD))
		#define AMPDU_UCODE_ENAB(pub) ((pub)->_ampdumac == AMPDU_AGG_UCODE)
		#define AMPDU_HW_ENAB(pub) ((pub)->_ampdumac == AMPDU_AGG_HW)
	#elif defined(WLAMPDU_HW)
		#define AMPDU_UCODE_ENAB(pub) 	(0)
		#define AMPDU_HW_ENAB(pub) 	(1)
	#elif defined(WLAMPDU_UCODE) && defined(WLAMPDU_UCODE_ONLY)
		#define AMPDU_UCODE_ENAB(pub) 	(1)
		#define AMPDU_HW_ENAB(pub) 	(0)
	#elif defined(WLAMPDU_UCODE)
		#define AMPDU_UCODE_ENAB(pub) ((pub)->_ampdumac == AMPDU_AGG_UCODE)
		#define AMPDU_HW_ENAB(pub) 0
	#endif 
#else
	#define AMPDU_UCODE_ENAB(pub) 0
	#define AMPDU_HW_ENAB(pub) 0
#endif 

#define AMPDU_MAC_ENAB(pub) (AMPDU_UCODE_ENAB(pub) || AMPDU_HW_ENAB(pub))
#define AMPDU_HOST_ENAB(pub) (!AMPDU_UCODE_ENAB(pub) && !AMPDU_HW_ENAB(pub))


#define WOWL_ACTIVE(wlc) (0)
#define WOWL_ENAB(pub) (0)


#ifdef WLDPT
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define DPT_ENAB(pub) ((pub)->_dpt)
	#elif defined(WLDPT_ENABLED)
		#define DPT_ENAB(pub) (1)
	#else
		#define DPT_ENAB(pub) (0)
	#endif
#else
	#define DPT_ENAB(pub) 0
#endif 


#ifdef WLBDD
#define BDD_ENAB(pub) ((pub)->_bdd)
#else
#define BDD_ENAB(pub) 0
#endif 


#define P2P_ENAB(pub) 0


#ifdef WLMCHAN
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define MCHAN_ENAB(pub) ((pub)->_mchan)
		#define MCHAN_ACTIVE(pub) ((pub)->_mchan_active)
	#elif defined(WLMCHAN_DISABLED)
		#define MCHAN_ENAB(pub) (0)
		#define MCHAN_ACTIVE(pub) (0)
	#else
		#define MCHAN_ENAB(pub) (1)
		#define MCHAN_ACTIVE(pub) ((pub)->_mchan_active)
	#endif
#else
	#define MCHAN_ENAB(pub) (0)
	#define MCHAN_ACTIVE(pub) (0)
#endif 


#ifdef WL_MULTIQUEUE
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define MQUEUE_ENAB(pub) ((pub)->_mqueue)
	#elif defined(WLMQUEUE_DISABLED)
		#define MQUEUE_ENAB(pub) (0)
	#else
		#define MQUEUE_ENAB(pub) (1)
	#endif
#else
	#define MQUEUE_ENAB(pub) (0)
#endif 




#define BTA_ENAB(pub) (0)


#ifdef WLPIO
#define PIO_ENAB(pub) ((pub)->_piomode)
#else
#define PIO_ENAB(pub) 0
#endif 


#ifdef WLCAC
#define CAC_ENAB(pub) ((pub)->_cac)
#else
#define CAC_ENAB(pub) 0
#endif


#ifdef WLCOEX
#define COEX_ENAB(pub) ((pub)->_coex != OFF)
#define COEX_ACTIVE(wlc) ((wlc)->obss->coex_enab)
#else
#define COEX_ACTIVE(wlc) 0
#define COEX_ENAB(pub) 0
#endif 

#ifdef BCMSPACE
#define	RXIQEST_ENAB(pub)	(1)
#else
#define	RXIQEST_ENAB(pub)	(0)
#endif

#define EDCF_ENAB(pub) (WME_ENAB(pub))
#define QOS_ENAB(pub) (WME_ENAB(pub) || N_ENAB(pub) || WLBA_ENAB(pub))

#define PRIOFC_ENAB(pub) ((pub)->_priofc)

#if defined(WL_MONITOR)
#define MONITOR_ENAB(wlc)	(wlc)->monitor
#else
#define MONITOR_ENAB(wlc)	(bcmspace && (wlc)->monitor)
#endif 

#if defined(WL_PROMISC)
#define PROMISC_ENAB(wlc_pub)	(wlc_pub)->promisc
#else
#define PROMISC_ENAB(wlc_pub)	(bcmspace && (wlc_pub)->promisc)
#endif

#define WLC_SENDUP_MGMT_ENAB(cfg)	((cfg)->sendup_mgmt)

#define TOE_ENAB(pub) 0

#define ARPOE_ENAB(pub) 0

#ifdef PACKET_FILTER
#define PKT_FILTER_ENAB(pub) 	((pub)->_pkt_filter)
#else
#define PKT_FILTER_ENAB(pub)	0
#endif

#ifdef WL_ASSOC_RECREATE
#define ASSOC_RECREATE_ENAB(pub) ((pub)->_assoc_recreate)
#else
#define ASSOC_RECREATE_ENAB(pub) 0
#endif

#ifdef WLFBT
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define WLFBT_ENAB(pub)	((pub)->_fbt)
	#elif defined(WLFBT_ENABLED)
		#define WLFBT_ENAB(pub)	(1)
	#else
		#define WLFBT_ENAB(pub)	(0)
	#endif
#else
	#define WLFBT_ENAB(pub)		(0)
#endif 

#define WIN7_OS(pub)	0

#define WLEXTSTA_ENAB(pub)	((pub)->_extsta)

#ifdef IBSS_PEER_GROUP_KEY
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define IBSS_PEER_GROUP_KEY_ENAB(pub) ((pub)->_ibss_peer_group_key)
	#elif defined(IBSS_PEER_GROUP_KEY_DISABLED)
		#define IBSS_PEER_GROUP_KEY_ENAB(pub) (0)
	#else
		#define IBSS_PEER_GROUP_KEY_ENAB(pub) (1)
	#endif
#else
	#define IBSS_PEER_GROUP_KEY_ENAB(pub) (0)
#endif 

#ifdef IBSS_PEER_DISCOVERY_EVENT
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define IBSS_PEER_DISCOVERY_EVENT_ENAB(pub) ((pub)->_ibss_peer_discovery_event)
	#elif defined(IBSS_PEER_DISCOVERY_EVENT_DISABLED)
		#define IBSS_PEER_DISCOVERY_EVENT_ENAB(pub) (0)
	#else
		#define IBSS_PEER_DISCOVERY_EVENT_ENAB(pub) (1)
	#endif
#else
	#define IBSS_PEER_DISCOVERY_EVENT_ENAB(pub) (0)
#endif 

#ifdef IBSS_PEER_MGMT
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define IBSS_PEER_MGMT_ENAB(pub) ((pub)->_ibss_peer_mgmt)
	#elif defined(IBSS_PEER_MGMT_DISABLED)
		#define IBSS_PEER_MGMT_ENAB(pub) (0)
	#else
		#define IBSS_PEER_MGMT_ENAB(pub) (1)
	#endif
#else
	#define IBSS_PEER_MGMT_ENAB(pub) (0)
#endif 

#if !defined(WLNOEIND)
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define WLEIND_ENAB(pub) ((pub)->_wleind)
	#elif defined(WLEIND_DISABLED)
		#define WLEIND_ENAB(pub) (0)
	#else
		#define WLEIND_ENAB(pub) (1)
	#endif
#else
	#define WLEIND_ENAB(pub) (0)
#endif 

	#define CCX_ENAB(pub) 0

#ifdef BCMAUTH_PSK
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define BCMAUTH_PSK_ENAB(pub) ((pub)->_bcmauth_psk)
	#elif defined(BCMAUTH_PSK_DISABLED)
		#define BCMAUTH_PSK_ENAB(pub) (0)
	#else
		#define BCMAUTH_PSK_ENAB(pub) (1)
	#endif
#else
	#define BCMAUTH_PSK_ENAB(pub) 0
#endif 

#ifdef WL11K
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define WL11K_ENAB(pub)	((pub)->_rrm)
	#elif defined(WL11K_DISABLED)
		#define WL11K_ENAB(pub)	(0)
	#else
		#define WL11K_ENAB(pub)	(1)
	#endif
#else
	#define WL11K_ENAB(pub)	(0)
#endif 

#ifdef WLPFN
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define WLPFN_ENAB(pub) ((pub)->_wlpfn)
	#elif defined(WLPFN_DISABLED)
		#define WLPFN_ENAB(pub) (0)
	#else
		#define WLPFN_ENAB(pub) (1)
	#endif
#else
	#define WLPFN_ENAB(pub) (0)
#endif 



#ifdef WIFI_ACT_FRAME
#define WIFI_ACT_FRAME_ENAB(pub) ((pub)->_wifiact_frame_enab)
#else
#define WIFI_ACT_FRAME_ENAB(pub) 0
#endif 


#ifdef DMATXRC
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define DMATXRC_ENAB(pub)	((pub)->_dmatxrc)
	#elif defined(DMATXRC_ENABLED)
		#define	DMATXRC_ENAB(pub) (1)
	#else
		#define DMATXRC_ENAB(pub) (0)
	#endif
#else
	#define	DMATXRC_ENAB(pub)	0
#endif 


#ifdef WLMEDIA_EN
	#if defined(BCMROMBUILD) || !defined(DONGLEBUILD)
		#define WLMEDIA_ENAB(pub) ((pub)->_wlmedia)
	#elif defined(WLMEDIA_DISABLED)
		#define WLMEDIA_ENAB(pub) (0)
	#else
		#define WLMEDIA_ENAB(pub) (1)
	#endif
#else
	#define WLMEDIA_ENAB(pub) 0
#endif 


#if defined(WL_PM2_RCV_DUR_LIMIT)
#define WLC_PM2_RCV_DUR_MIN		(10)	
#define WLC_PM2_RCV_DUR_MAX		(80)	
#define PM2_RCV_DUR_ENAB(cfg) ((cfg)->pm->pm2_rcv_percent > 0)
#else
#define PM2_RCV_DUR_ENAB(cfg) 0
#endif 


#define PM2_SLEEP_RET_MS_DEFAULT 200


#define PM2_MD_SLEEP_EXT_DISABLED	0
#define PM2_MD_SLEEP_EXT_USE_SHORT_FRTS	1
#define PM2_MD_SLEEP_EXT_USE_BCN_FRTS	2

#ifdef PROP_TXSTATUS
#define WLFC_PKTAG_INFO_MOVE(pkt_from, pkt_to)	\
	do { \
		WLPKTTAG(pkt_to)->wl_hdr_information = WLPKTTAG(pkt_from)->wl_hdr_information; \
		WLPKTTAG(pkt_from)->wl_hdr_information = 0; \
\
		if (wlc->pkt_callback[WLPKTTAG(pkt_to)->callbackidx].arg == pkt_from) \
			wlc->pkt_callback[WLPKTTAG(pkt_to)->callbackidx].arg = pkt_to; \
	} while (0)
#else
#define WLFC_PKTAG_INFO_MOVE(pkt_from, pkt_to) 	do {} while (0)
#endif

#define WLPKTTAGSCB(p) (WLPKTTAG(p)->_scb)

#define	WLC_PREC_COUNT		16 


extern const uint8 wlc_prio2prec_map[];
#define WLC_PRIO_TO_PREC(pri)	wlc_prio2prec_map[(pri) & 7]


#define WLC_PRIO_TO_HI_PREC(pri)	MIN(WLC_PRIO_TO_PREC(pri) + 1, WLC_PREC_COUNT - 1)

extern const uint8 wme_fifo2ac[];
#define WME_PRIO2AC(prio)	wme_fifo2ac[prio2fifo[(prio)]]

#endif 

#ifndef LINUX_WLUSER_POSTMOGRIFY_REMOVAL

#define WLC_PREC_BMP_ALL		MAXBITVAL(WLC_PREC_COUNT)


#define WLC_PREC_BMP_AC_BE	(NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_BE)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_BE)) |	\
				NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_EE)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_EE)))
#define WLC_PREC_BMP_AC_BK	(NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_BK)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_BK)) |	\
				NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_NONE)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_NONE)))
#define WLC_PREC_BMP_AC_VI	(NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_CL)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_CL)) |	\
				NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_VI)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_VI)))
#define WLC_PREC_BMP_AC_VO	(NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_VO)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_VO)) |	\
				NBITVAL(WLC_PRIO_TO_PREC(PRIO_8021D_NC)) |	\
				NBITVAL(WLC_PRIO_TO_HI_PREC(PRIO_8021D_NC)))

#endif 


#ifdef WME
#define WME_ENAB(pub) ((pub)->_wme != OFF)
#define WME_AUTO(wlc) ((wlc)->pub->_wme == AUTO)
#else
#define WME_ENAB(pub) 0
#define WME_AUTO(wlc) (0)
#endif 

#define WLC_USE_COREFLAGS	0xffffffff	

#ifdef WLCNT
#define WLC_UPDATE_STATS(wlc)	1	
#define WLCNTINCR(a)		((a)++)	
#define WLCNTDECR(a)		((a)--)	
#define WLCNTADD(a,delta)	((a) += (delta)) 
#define WLCNTSET(a,value)	((a) = (value)) 
#define WLCNTVAL(a)		(a)	
#define WLCNTINCR_MIN(a)        ((a)++) 
#else 
#define WLC_UPDATE_STATS(wlc)	0	
#define WLCNTINCR(a)			
#define WLCNTDECR(a)			
#define WLCNTADD(a,delta)		
#define WLCNTSET(a,value)		
#define WLCNTVAL(a)		0	
#define WLCNTINCR_MIN(a)
#endif 



#if !defined(LINUX_WLUSER_POSTMOGRIFY_REMOVAL)
#define	MAXBANDS		2	

#define BAND_2G_INDEX		0	
#define BAND_5G_INDEX		1	

#define BAND_2G_NAME		"2.4G"
#define BAND_5G_NAME		"5G"
#endif 



#define WLC_REPLAY_CNTRS_VALUE  WPA_CAP_16_REPLAY_CNTRS



#if WLC_REPLAY_CNTRS_VALUE == WPA_CAP_16_REPLAY_CNTRS
#define PRIO2IVIDX(prio)        (prio)
#elif WLC_REPLAY_CNTRS_VALUE == WPA_CAP_4_REPLAY_CNTRS
#define PRIO2IVIDX(prio)        WME_PRIO2AC(prio)
#else
#error "Neither WPA_CAP_4_REPLAY_CNTRS nor WPA_CAP_16_REPLAY_CNTRS is used"
#endif 

#endif 
