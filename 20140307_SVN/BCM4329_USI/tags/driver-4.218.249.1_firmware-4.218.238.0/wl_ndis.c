/*
 * Common NDIS interface portion of
 * Broadcom 802.11abg Networking Device Driver
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wl_ndis.c,v 1.828.2.5.2.8.4.102 2010/06/22 19:51:29 Exp $
 */

#include <epivers.h>
#include <bcmdefs.h>
#include <typedefs.h>

#if defined(EXT_STA) || defined(DHD_NDIS_OID) || defined(WLBTAMP_HIGH_ONLY)
#include <wlc_cfg.h>
#endif /* EXT_STA || DHD_NDIS_OID || WLBTAMP_HIGH_ONLY */
#define INITGUID	1	/* To get GUID_PCMCIA_INTERFACE_STANDARD defined */
#include <osl.h>
#include <bcmutils.h>
#include <bcmdevs.h>

#include <oidencap.h>
#include <wlioctl.h>

#ifdef EXT_STA
#include <bcmwpa.h>
#endif /* EXT_STA */

#include <bcmsdh.h>

#include <siutils.h>
#include <bcmendian.h>
#include <sbconfig.h>
#include <sbpcmcia.h>
#include <nicpci.h>
#include <proto/802.11.h>
#include <proto/802.1d.h>
#include <proto/bcmip.h>
#include <sbhndpio.h>
#include <sbhnddma.h>
/* Not needed for dongle/XP and dongle/CE */
#if defined(EXT_STA) || defined(DHD_NDIS_OID) || defined(WLBTAMP_HIGH_ONLY)
#include <hnddma.h>
#include <d11.h>
#include <wlc_rate.h>
#include <wlc_key.h>
#include <wlc_channel.h>
#include <wlc_pub.h>
#include <wlc_bsscfg.h>
#include <wlc.h>
#include <wlc_phy.h>
#include <wlc_ethereal.h>
#include <wlc_wowl.h>
#include <wl_oid.h>
#include <wl_export.h>
#endif /* EXT_STA || DHD_NDIS_OID || WLBTAMP_HIGH_ONLY */


#include <proto/802.1d.h>
#include <bcmendian.h>

#include <wl_dbg.h>
#include <wl_ndis.h>
#include <wl_ndconfig.h>
#include <ndiserrmap.h>


#if defined(WDM)
#include <wl_wmi.h>
#endif /* defined(WDM) */

#ifdef BCMSUP_PSK
#include <proto/eapol.h>
#endif /* BCMSUP_PSK */

#if defined(NDIS60)
#include <cebuscfg.h> /* WM7 TODO: find this file */
#endif /* UNDER_CE && NDIS60 */

#ifdef WLBTAMP
#include <wlc_bta.h>
#include <proto/bt_amp_hci.h>
#include <proto/802.11_bta.h>
#include <dhdioctl.h>
#include <dhd_dbg.h>
#include <dhd_bta.h>
#if defined(WLBTWUSB)
#include <bt_int.h>
#elif defined(WLBTCEAMP11)
#include <btceamp11.h>
#endif
#endif /* WLBTAMP */

#if defined(WL_IW_USE_ISCAN) || defined(SIMPLE_ISCAN)
#ifdef NDIS60
#include <dngl_stats.h>
#include <dhd.h>
#endif /* NDIS60 */

#if defined(__BBSOFT__) && defined(__ZELOS__)
#include <bsp.h>
#include <pmpolicy.h>
#include <drvmsg.h>
#include <drvlib_mem.h>
#include "pinDefines.h"
#include "modelinfo.h"

#define WLAN_WAKEUP			GPG03_Output	// Wlan Wakeup

static volatile GPIO_REG *g_pGPIOReg = NULL;
#endif

#define htod32(i) i
#define htod16(i) i
#define dtoh32(i) i
#define dtoh16(i) i
#define htodchanspec(i) htod16(i)
#define dtohchanspec(i) dtoh16(i)

#ifdef WL_ISCAN
#undef WL_ISCAN
#endif
#define WL_ISCAN(x) WL_TRACE(x)
#endif /* defined(WL_IW_USE_ISCAN) || defined(SIMPLE_ISCAN) */

#ifdef WL_IW_USE_ISCAN
static int wl_iscan_set_scan(wl_info_t *wl);
static int
wl_iscan_get_scan(
	wl_info_t * wl,
	PVOID InfoBuf,
	ULONG InfoBufLen,
	PULONG BytesWritten,
	PULONG BytesNeeded
);
static void wl_iscan_read(wl_info_t *wl);
int wl_iscan_attach(wl_info_t *wl);
void wl_iscan_detach(wl_info_t *wl);
void wl_e_scan_complete(wl_info_t *wl);
int wl_iscan_get_scan_status(wl_info_t *wl);
void wl_iscan_set_device_state(wl_info_t *wl, int dev_state);
void wl_iscan_set_radio_disable(wl_info_t *wl, bool status);
#if defined(NDIS60)
void wl_ind_scan_confirm(wl_info_t *wl, NDIS_STATUS StatusBuffer);
void wl_iscan_set_scan_type(wl_info_t *wl, DOT11_SCAN_TYPE ScanType);
void wl_iscan_set_bss_type(wl_info_t *wl, DOT11_BSS_TYPE BSSType);
static void wl_iscan_flush_scan(wl_info_t *wl);
#endif
#endif /* WL_IW_USE_ISCAN */

#ifdef SIMPLE_ISCAN
extern void dhd_iscan_lock(void);
extern void dhd_iscan_unlock(void);
extern int dhd_iscan_attach(void *dhd);
extern void dhd_iscan_deattach(void *dhd);
extern void dhd_iscan_notify(void);
extern int dhd_iscan_request(void *dhd, uint16 action);
extern iscan_buf_t * dhd_iscan_result_buf(void);
extern void dhd_iscan_free_buf(void *dhdp, iscan_buf_t *iscan_delete);
#endif /* SIMPLE_ISCAN */

#if defined(NDIS60) || defined(WL_IW_USE_ISCAN)
/* Rate info per rate: It tells whether a rate is ofdm or not and its phy_rate value */
const uint8 rate_info[WLC_MAXRATE + 1] = {
	/*  0     1     2     3     4     5     6     7     8     9 */
/*   0 */ 0x00, 0x00, 0x0a, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  10 */ 0x00, 0x37, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0x00,
/*  20 */ 0x00, 0x00, 0x6e, 0x00, 0x8a, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  30 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x00, 0x00, 0x00,
/*  40 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x00,
/*  50 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  60 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  70 */ 0x00, 0x00, 0x8d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  80 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  90 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00,
/* 100 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8c
};
#endif /* (NDIS && UNDER_CE) || WL_IW_USE_ISCAN */

#if defined(NDIS60)
void wlc_rateset_default(wlc_rateset_t *rs_tgt, const wlc_rateset_t *rs_hw, uint phy_type,
	int bandtype, bool cck_only, uint rate_mask, bool mcsallow, uint8 bw);
bool wlc_rate_hwrs_filter_sort_validate(wlc_rateset_t *rs, const wlc_rateset_t *hw_rs,
	bool check_brate);

bool wlc_rateset_valid(wlc_rateset_t *rs, bool check_brate);

void wlc_rateset_filter(wlc_rateset_t *src, wlc_rateset_t *dst, bool basic_only, bool cck_only,
	uint xmask, bool mcsallow);

void wlc_rateset_copy(const wlc_rateset_t *src, wlc_rateset_t *dst);
#ifdef WL11N
void wlc_rateset_mcs_clear(wlc_rateset_t *rateset);
#endif /* WL11N */

const wlc_rateset_t cck_rates = {
	4,
	{ /*	1b,   2b,   5.5,  11 Mbps */
		0x82, 0x84, 0x0b, 0x16
	},
	0x00,
	{ 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
	}
};

const wlc_rateset_t cck_ofdm_rates = {
	12,
	{ /*	1b,   2b,   5.5b, 6,    9,    11b,  12,   18,   24,   36,   48,   54 Mbps */
		0x82, 0x84, 0x8b, 0x0c, 0x12, 0x96, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c
	},
	0x00,
	{ 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
	}
};

const wlc_rateset_t ofdm_rates = {
	8,
	{ /*	6b,   9,    12b,  18,   24b,  36,   48,   54 Mbps */
		0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c
	},
	0x00,
	{ 	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
	}
};

const wlc_rateset_t ofdm_40bw_mimo_rates = {
	8,
	{ /*	6b,   9,    12b,  18,   24b,  36,   48,   54 Mbps */
		0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c
	},
	0x00,
	{	0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
	}
};

/* Default ratesets that include MCS32 for 40BW channels */
const wlc_rateset_t cck_ofdm_40bw_mimo_rates = {
	12,
	{ /*	1b,   2b,   5.5b, 6,    9,    11b,  12,   18,   24,   36,   48,   54 Mbps */
		0x82, 0x84, 0x8b, 0x0c, 0x12, 0x96, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c
	},
	0x00,
	{	0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
	}
};

const wlc_rateset_t cck_ofdm_mimo_rates = {
	12,
	{ /*	1b,   2b,   5.5b, 6,    9,    11b,  12,   18,   24,   36,   48,   54 Mbps */
		0x82, 0x84, 0x8b, 0x0c, 0x12, 0x96, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c
	},
	0x00,
	{	0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
	}
};

const wlc_rateset_t ofdm_mimo_rates = {
	8,
	{ /*	6b,   9,    12b,  18,   24b,  36,   48,   54 Mbps */
		0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c
	},
	0x00,
	{       0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
	}
};

/* rates are in units of Kbps */
const mcs_info_t mcs_table[MCS_TABLE_SIZE] = {
	/* MCS  0: SS 1, MOD: BPSK,  CR 1/2 */
	{6500,   13500,  CEIL(6500*10, 9),   CEIL(13500*10, 9),  0x00, WLC_RATE_6M},
	/* MCS  1: SS 1, MOD: QPSK,  CR 1/2 */
	{13000,  27000,  CEIL(13000*10, 9),  CEIL(27000*10, 9),  0x08, WLC_RATE_12M},
	/* MCS  2: SS 1, MOD: QPSK,  CR 3/4 */
	{19500,  40500,  CEIL(19500*10, 9),  CEIL(40500*10, 9),  0x0A, WLC_RATE_18M},
	/* MCS  3: SS 1, MOD: 16QAM, CR 1/2 */
	{26000,  54000,  CEIL(26000*10, 9),  CEIL(54000*10, 9),  0x10, WLC_RATE_24M},
	/* MCS  4: SS 1, MOD: 16QAM, CR 3/4 */
	{39000,  81000,  CEIL(39000*10, 9),  CEIL(81000*10, 9),  0x12, WLC_RATE_36M},
	/* MCS  5: SS 1, MOD: 64QAM, CR 2/3 */
	{52000,  108000, CEIL(52000*10, 9),  CEIL(108000*10, 9), 0x19, WLC_RATE_48M},
	/* MCS  6: SS 1, MOD: 64QAM, CR 3/4 */
	{58500,  121500, CEIL(58500*10, 9),  CEIL(121500*10, 9), 0x1A, WLC_RATE_54M},
	/* MCS  7: SS 1, MOD: 64QAM, CR 5/6 */
	{65000,  135000, CEIL(65000*10, 9),  CEIL(135000*10, 9), 0x1C, WLC_RATE_54M},
	/* MCS  8: SS 2, MOD: BPSK,  CR 1/2 */
	{13000,  27000,  CEIL(13000*10, 9),  CEIL(27000*10, 9),  0x40, WLC_RATE_6M},
	/* MCS  9: SS 2, MOD: QPSK,  CR 1/2 */
	{26000,  54000,  CEIL(26000*10, 9),  CEIL(54000*10, 9),  0x48, WLC_RATE_12M},
	/* MCS 10: SS 2, MOD: QPSK,  CR 3/4 */
	{39000,  81000,  CEIL(39000*10, 9),  CEIL(81000*10, 9),  0x4A, WLC_RATE_18M},
	/* MCS 11: SS 2, MOD: 16QAM, CR 1/2 */
	{52000,  108000, CEIL(52000*10, 9),  CEIL(108000*10, 9), 0x50, WLC_RATE_24M},
	/* MCS 12: SS 2, MOD: 16QAM, CR 3/4 */
	{78000,  162000, CEIL(78000*10, 9),  CEIL(162000*10, 9), 0x52, WLC_RATE_36M},
	/* MCS 13: SS 2, MOD: 64QAM, CR 2/3 */
	{104000, 216000, CEIL(104000*10, 9), CEIL(216000*10, 9), 0x59, WLC_RATE_48M},
	/* MCS 14: SS 2, MOD: 64QAM, CR 3/4 */
	{117000, 243000, CEIL(117000*10, 9), CEIL(243000*10, 9), 0x5A, WLC_RATE_54M},
	/* MCS 15: SS 2, MOD: 64QAM, CR 5/6 */
	{130000, 270000, CEIL(130000*10, 9), CEIL(270000*10, 9), 0x5C, WLC_RATE_54M},
	/* MCS 16: SS 3, MOD: BPSK,  CR 1/2 */
	{19500,  40500,  CEIL(19500*10, 9),  CEIL(40500*10, 9),  0x80, WLC_RATE_6M},
	/* MCS 17: SS 3, MOD: QPSK,  CR 1/2 */
	{39000,  81000,  CEIL(39000*10, 9),  CEIL(81000*10, 9),  0x88, WLC_RATE_12M},
	/* MCS 18: SS 3, MOD: QPSK,  CR 3/4 */
	{58500,  121500, CEIL(58500*10, 9),  CEIL(121500*10, 9), 0x8A, WLC_RATE_18M},
	/* MCS 19: SS 3, MOD: 16QAM, CR 1/2 */
	{78000,  162000, CEIL(78000*10, 9),  CEIL(162000*10, 9), 0x90, WLC_RATE_24M},
	/* MCS 20: SS 3, MOD: 16QAM, CR 3/4 */
	{117000, 243000, CEIL(117000*10, 9), CEIL(243000*10, 9), 0x92, WLC_RATE_36M},
	/* MCS 21: SS 3, MOD: 64QAM, CR 2/3 */
	{156000, 324000, CEIL(156000*10, 9), CEIL(156000*10, 9), 0x99, WLC_RATE_48M},
	/* MCS 22: SS 3, MOD: 64QAM, CR 3/4 */
	{175500, 364500, CEIL(175500*10, 9), CEIL(175500*10, 9), 0x9A, WLC_RATE_54M},
	/* MCS 23: SS 3, MOD: 64QAM, CR 5/6 */
	{195000, 405000, CEIL(195000*10, 9), CEIL(195000*10, 9), 0x9B, WLC_RATE_54M},
	/* MCS 24: SS 4, MOD: BPSK,  CR 1/2 */
	{26000,  54000,  CEIL(26000*10, 9),  CEIL(260000*10, 9), 0xC0, WLC_RATE_6M},
	/* MCS 25: SS 4, MOD: QPSK,  CR 1/2 */
	{52000,  108000, CEIL(52000*10, 9),  CEIL(108000*10, 9), 0xC8, WLC_RATE_12M},
	/* MCS 26: SS 4, MOD: QPSK,  CR 3/4 */
	{78000,  162000, CEIL(78000*10, 9),  CEIL(162000*10, 9), 0xCA, WLC_RATE_18M},
	/* MCS 27: SS 4, MOD: 16QAM, CR 1/2 */
	{104000, 216000, CEIL(104000*10, 9), CEIL(216000*10, 9), 0xD0, WLC_RATE_24M},
	/* MCS 28: SS 4, MOD: 16QAM, CR 3/4 */
	{156000, 324000, CEIL(156000*10, 9), CEIL(324000*10, 9), 0xD2, WLC_RATE_36M},
	/* MCS 29: SS 4, MOD: 64QAM, CR 2/3 */
	{208000, 432000, CEIL(208000*10, 9), CEIL(432000*10, 9), 0xD9, WLC_RATE_48M},
	/* MCS 30: SS 4, MOD: 64QAM, CR 3/4 */
	{234000, 486000, CEIL(234000*10, 9), CEIL(486000*10, 9), 0xDA, WLC_RATE_54M},
	/* MCS 31: SS 4, MOD: 64QAM, CR 5/6 */
	{260000, 540000, CEIL(260000*10, 9), CEIL(540000*10, 9), 0xDB, WLC_RATE_54M},
	/* MCS 32: SS 1, MOD: BPSK,  CR 1/2 */
	{0,      6000,   0, CEIL(6000*10, 9), 0x00, WLC_RATE_6M},
	};
#endif /* NDIS60 && UNDER_CE */

#define MAXMULTILIST 32
#define DEFAULT_IMAGE_PATH L"\\Windows\\retcdc.bin"
#define DEFAULT_DPC_PRIORITY 105

#define TX_NO_PROGRESS_TIMEOUT	25 /* check for hang timeout in s */

#if defined(EXT_STA)

#define BSS_LIST_REQ_LEN                8192
#define BSS_ENTRY_STRUCT_ADJ            5

/*
 * Tempoaray fix for bug on dongle when handling OID_DOT11_ENUM_BSS_LIST
 * For explanation, see function description
 * for wl_dngl_bsslist_reformat()
 */
#if !defined(EXT_STA)
static NDIS_STATUS
wl_oidreq_async(wl_info_t *wl, NDIS_OID oid, PVOID buf, ULONG bufLen, ULONG outBufLen,
	PULONG bytesWritten, PULONG bytesRead, PULONG bytesNeeded,
	PNDIS_OID_REQUEST oidReq);
#endif
static NDIS_STATUS
wl_oid_post_process(NDIS_HANDLE MiniportAdapterContext, NDIS_OID Oid, PVOID InfoBuf,
	ULONG InBufLen, ULONG OutBufLen, PULONG BytesWritten, PULONG BytesRead,
	PULONG BytesNeeded, PNDIS_OID_REQUEST OidRequest, NDIS_STATUS prevStatus);
#endif /* BCMDONGLEHOST && EXT_STA */

/* !NOTE!: OIDS must be listed in numerical order! */
UINT wl_supported_oids[] = {
	/* GEN mandatory list */
	OID_GEN_SUPPORTED_LIST,
	OID_GEN_HARDWARE_STATUS,
	OID_GEN_MEDIA_SUPPORTED,
	OID_GEN_MEDIA_IN_USE,
	OID_GEN_MAXIMUM_LOOKAHEAD,
	OID_GEN_MAXIMUM_FRAME_SIZE,
	OID_GEN_LINK_SPEED,
	OID_GEN_TRANSMIT_BUFFER_SPACE,
	OID_GEN_RECEIVE_BUFFER_SPACE,
	OID_GEN_TRANSMIT_BLOCK_SIZE,
	OID_GEN_RECEIVE_BLOCK_SIZE,
	OID_GEN_VENDOR_ID,
	OID_GEN_VENDOR_DESCRIPTION,
	OID_GEN_CURRENT_PACKET_FILTER,
	OID_GEN_CURRENT_LOOKAHEAD,
	OID_GEN_DRIVER_VERSION,
	OID_GEN_MAXIMUM_TOTAL_SIZE,
	OID_GEN_PROTOCOL_OPTIONS,
	OID_GEN_MAC_OPTIONS,
	OID_GEN_MEDIA_CONNECT_STATUS,
	OID_GEN_MAXIMUM_SEND_PACKETS,
	OID_GEN_VENDOR_DRIVER_VERSION,

#if defined(WDM)
	OID_GEN_SUPPORTED_GUIDS,
#endif /* defined(WDM) */

#ifdef OID_GEN_PHYSICAL_MEDIUM
	OID_GEN_PHYSICAL_MEDIUM,
#endif /* OID_GEN_PHYSICAL_MEDIUM */

	OID_GEN_XMIT_OK,
	OID_GEN_RCV_OK,
	OID_GEN_XMIT_ERROR,
	OID_GEN_RCV_ERROR,
	OID_GEN_RCV_NO_BUFFER,
#ifdef NDIS60
#ifdef PRE_v1_68_HEADERS
	OID_GEN_STATISTICS,			/* 0x00020106 */
#endif /* PRE_v1_68_HEADERS */
#endif /* NDIS60 */

	OID_802_3_PERMANENT_ADDRESS,		/* MAC address in hardware */
	OID_802_3_CURRENT_ADDRESS,		/* MAC address currently in use */
	OID_802_3_MULTICAST_LIST,		/* Set multicast list */
	OID_802_3_MAXIMUM_LIST_SIZE,		/* Multicast address list size */

#ifndef WDM	  /* WHQL fix: Win98 & NT5 only use OID_GEN_MAC_OPTIONS */
	OID_802_3_MAC_OPTIONS,			/* MAC options: priority */
#endif /* !WDM */

	OID_802_3_RCV_ERROR_ALIGNMENT,		/* Frames received with alignment error */
	OID_802_3_XMIT_ONE_COLLISION,		/* Frames tx after one collision */
	OID_802_3_XMIT_MORE_COLLISIONS, 	/* Frames tx w/more than one collision */

#ifdef STA
#ifndef EXT_STA
	/* these are listed out of order in ntddndis.h */
	OID_802_11_BSSID,			/* 0x0D010101 */
	OID_802_11_SSID,			/* 0x0D010102 */
	OID_802_11_INFRASTRUCTURE_MODE,		/* 0x0D010108 */
	OID_802_11_ADD_WEP,			/* 0x0D010113 */
	OID_802_11_REMOVE_WEP,			/* 0x0D010114 */
	OID_802_11_DISASSOCIATE,		/* 0x0D010115 */
	OID_802_11_AUTHENTICATION_MODE,		/* 0x0D010118 */
	OID_802_11_PRIVACY_FILTER,		/* 0x0D010119 */
	OID_802_11_BSSID_LIST_SCAN,		/* 0x0D01011A */
	OID_802_11_ENCRYPTION_STATUS,		/* 0x0D01011B */
	OID_802_11_RELOAD_DEFAULTS,		/* 0x0D01011C */
	OID_802_11_ADD_KEY,			/* 0x0D01011D */
	OID_802_11_REMOVE_KEY,			/* 0x0D01011E */
	OID_802_11_ASSOCIATION_INFORMATION,	/* 0x0D01011F */
	OID_802_11_TEST,			/* 0x0D010120 */
#ifdef BCMWPA2
	OID_802_11_CAPABILITY,			/* 0x0D010122 */
	OID_802_11_PMKID,			/* 0x0D010123 */
#endif /* BCMWPA2 */
	OID_802_11_NETWORK_TYPES_SUPPORTED,	/* 0x0D010203 */
	OID_802_11_NETWORK_TYPE_IN_USE,		/* 0x0D010204 */
	OID_802_11_TX_POWER_LEVEL,		/* 0x0D010205 */
	OID_802_11_RSSI,			/* 0x0D010206 */
	OID_802_11_RSSI_TRIGGER,		/* 0x0D010207 */
	OID_802_11_FRAGMENTATION_THRESHOLD,	/* 0x0D010209 */
	OID_802_11_RTS_THRESHOLD,		/* 0x0D01020A */
	OID_802_11_NUMBER_OF_ANTENNAS,		/* 0x0D01020B */
	OID_802_11_RX_ANTENNA_SELECTED,		/* 0x0D01020C */
	OID_802_11_TX_ANTENNA_SELECTED,		/* 0x0D01020D */
	OID_802_11_SUPPORTED_RATES,		/* 0x0D01020E */
	OID_802_11_DESIRED_RATES,		/* 0x0D010210 */
	OID_802_11_CONFIGURATION,		/* 0x0D010211 */
	OID_802_11_POWER_MODE,			/* 0x0D010216 */
	OID_802_11_BSSID_LIST,			/* 0x0D010217 */
	OID_802_11_STATISTICS,			/* 0x0D020212 */
#else /* !EXT_STA */
	OID_DOT11_MPDU_MAX_LENGTH,		/* 0x0D010306 */
	OID_DOT11_OPERATION_MODE_CAPABILITY,	/* 0x0D010307 */
	OID_DOT11_SUPPORTED_PHY_TYPES,		/* 0x0D010326 */
	OID_DOT11_CURRENT_REG_DOMAIN,           /* 0x0D010327 */
	OID_DOT11_SUPPORTED_POWER_LEVELS,	/* 0x0D01032C */
	OID_DOT11_CURRENT_TX_POWER_LEVEL,	/* 0x0D01032D */
	OID_DOT11_SUPPORTED_DATA_RATES_VALUE,	/* 0x0D010341 */
	OID_DOT11_SUPPORTED_UNICAST_ALGORITHM_PAIR,	/* 0x0E010186 */
	OID_DOT11_SUPPORTED_MULTICAST_ALGORITHM_PAIR,	/* 0x0E010188 */
	OID_DOT11_DATA_RATE_MAPPING_TABLE,	/* 0x0E010197 */

	OID_DOT11_CURRENT_OPERATION_MODE,	/* 0x0D010308 */
	OID_DOT11_SCAN_REQUEST,			/* 0x0D01030B */
	OID_DOT11_RESET_REQUEST,		/* 0x0D010310 */
	OID_DOT11_NIC_POWER_STATE,		/* 0x0D010311 */
	OID_DOT11_OPERATIONAL_RATE_SET,		/* 0x0D01031A */
	OID_DOT11_BEACON_PERIOD,		/* 0x0D01031B */
	OID_DOT11_MAC_ADDRESS,			/* 0x0D01031E */
	OID_DOT11_RTS_THRESHOLD,		/* 0x0D01031F */
	OID_DOT11_SHORT_RETRY_LIMIT,		/* 0x0D010320 */
	OID_DOT11_FRAGMENTATION_THRESHOLD,	/* 0x0D010322 */
	OID_DOT11_CURRENT_CHANNEL_NUMBER,	/* 0x0D01032F */
	OID_DOT11_CURRENT_CHANNEL,		/* 0x0D010335 */
	OID_DOT11_REG_DOMAINS_SUPPORT_VALUE,    /* 0x0D01033D */
	OID_DOT11_CURRENT_FREQUENCY,		/* 0x0D010342 */
	OID_DOT11_NIC_SPECIFIC_EXTENSION,	/* 0x0D01035C */
	OID_DOT11_RECV_SENSITIVITY_LIST,	/* 0x0D010365 */
	OID_DOT11_CURRENT_ADDRESS,		/* 0x0D010702 */
	OID_DOT11_PERMANENT_ADDRESS,		/* 0x0D010703 */
	OID_DOT11_MULTICAST_LIST,		/* 0x0D010704 */
	OID_DOT11_AUTO_CONFIG_ENABLED,		/* 0x0E010178 */
	OID_DOT11_ENUM_BSS_LIST,		/* 0x0E010179 */
	OID_DOT11_FLUSH_BSS_LIST,		/* 0x0E01017A */
	OID_DOT11_POWER_MGMT_REQUEST,		/* 0x0E01017B */
	OID_DOT11_DESIRED_SSID_LIST,		/* 0x0E01017C */
	OID_DOT11_EXCLUDED_MAC_ADDRESS_LIST,	/* 0x0E01017D */
	OID_DOT11_DESIRED_BSSID_LIST,		/* 0x0E01017E */
	OID_DOT11_DESIRED_BSS_TYPE,		/* 0x0E01017F */
#ifdef BCMWPA2
	OID_DOT11_PMKID_LIST,			/* 0x0E010180 */
#endif /* BCMWPA2 */
	OID_DOT11_CONNECT_REQUEST,		/* 0x0E010181 */
	OID_DOT11_EXCLUDE_UNENCRYPTED,		/* 0x0E010182 */
	OID_DOT11_PRIVACY_EXEMPTION_LIST,	/* 0x0E010184 */
	OID_DOT11_ENABLED_AUTHENTICATION_ALGORITHM,	/* 0x0E010185 */
	OID_DOT11_ENABLED_UNICAST_CIPHER_ALGORITHM,	/* 0x0E010187 */
	OID_DOT11_ENABLED_MULTICAST_CIPHER_ALGORITHM,	/* 0x0E010189 */
	OID_DOT11_CIPHER_DEFAULT_KEY_ID,	/* 0x0E01018A */
	OID_DOT11_CIPHER_DEFAULT_KEY,		/* 0x0E01018B */
	OID_DOT11_CIPHER_KEY_MAPPING_KEY,	/* 0x0E01018C */
	OID_DOT11_ENUM_ASSOCIATION_INFO,	/* 0x0E01018D */
	OID_DOT11_DISCONNECT_REQUEST,		/* 0x0E01018E */
	OID_DOT11_UNICAST_USE_GROUP_ENABLED,   	/* 0x0E01018F */

	OID_DOT11_HARDWARE_PHY_STATE,		/* 0x0E010190 */
	OID_DOT11_DESIRED_PHY_LIST,   		/* 0x0E010191 */
	OID_DOT11_CURRENT_PHY_ID,		/* 0x0E010192 */
	OID_DOT11_MEDIA_STREAMING_ENABLED,	/* 0x0E010193 */
	OID_DOT11_UNREACHABLE_DETECTION_THRESHOLD,	/* 0x0E010194 */
	OID_DOT11_ACTIVE_PHY_LIST,    		/* 0x0E010195 */
	OID_DOT11_EXTSTA_CAPABILITY,		/* 0x0E010196 */
	OID_DOT11_IBSS_PARAMS,			/* 0x0E01019B */
	OID_DOT11_HIDDEN_NETWORK_ENABLED,	/* 0x0E01019E */
	OID_DOT11_STATISTICS,			/* 0x0E020183 */
#endif /* !EXT_STA */
#endif /* STA */
	OID_PNP_CAPABILITIES,
#if defined(WDM)
	OID_PNP_SET_POWER,
	OID_PNP_QUERY_POWER,
	OID_PNP_ADD_WAKE_UP_PATTERN,
	OID_PNP_REMOVE_WAKE_UP_PATTERN,
	OID_PNP_ENABLE_WAKE_UP,
	OID_TCP_TASK_OFFLOAD,
#endif /* defined(WDM) */
	};

#ifdef WLBTAMP
#if defined(WLBTWUSB) || defined(WLBTCEAMP11)
NDIS_HANDLE wl_wrapper_handle;
#endif
#endif /* WLBTAMP */

#define OID_MH_GET_802_1X_SUPPORTED	0xFFEDC100	/* MeetingHouse proprietary licensing OID */


/* Registry config entry defs for wl_readparam() */
ndis_config_t nic_configs[] = {
	{ "ForcePIO", NdisParameterInteger, 0, 0, NULL },
	{ "AllowIntfUp", NdisParameterInteger, 0, 0, NULL },
	{ "MsgLevel", NdisParameterInteger, 0, 0, NULL },
	{ "DriverDesc", NdisParameterString, 0, 0, NULL },
	{ "AdapterDesc", NdisParameterString, 0, 0, NULL },
	{ "Buffers", NdisParameterInteger, 0, 0, NULL },
	{ "Packets", NdisParameterInteger, 0, 0, NULL },
	{ "Loops", NdisParameterInteger, 0, 0, NULL },
	{ "EnableAutoConnect", NdisParameterInteger, 0, 0, NULL },
	{ "WEP", NdisParameterString, 0, 0, NULL },
	{ "NetworkType", NdisParameterInteger, 0, SKIP_MINUS_1, NULL },
	{ "SSID", NdisParameterString, 0, 0, NULL },
	{ "AuthMode", NdisParameterInteger, 0, SKIP_MINUS_1, NULL },
	{ "Brand", NdisParameterString, 0, 0, NULL },
	{ "DPCPriority", NdisParameterInteger, 0, 0, NULL },
	{ "DriveStrength", NdisParameterInteger, 0, 0, NULL },
	{ "WatchdogPriority", NdisParameterInteger, 0, 0, NULL },
	{ "RXThreadPriority", NdisParameterInteger, 0, 0, NULL },
	{ "RxDPCPriority", NdisParameterInteger, 0, 0, NULL },
	{ "EventPriority", NdisParameterInteger, 0, 0, NULL },
	{ "OidPriority", NdisParameterInteger, 0, 0, NULL },
	{ "EnablePoll", NdisParameterInteger, 0, 0, NULL },
	{ "RxflowMode", NdisParameterInteger, 0, 0, NULL},
	{ "RxflowHi", NdisParameterInteger, 0, 0, NULL},
	{ "RxflowLow", NdisParameterInteger, 0, 0, NULL},
	{ "RxflowRxThreadPriority", NdisParameterInteger, 0, 0, NULL},
	{ "RxflowRxDPCPriority", NdisParameterInteger, 0, 0, NULL},
#ifdef EXT_STA
	{ "*PriorityVLANTag", NdisParameterInteger, 0, 0, NULL },
#endif /* EXT_STA */
	{ "WakeupCapabilities", NdisParameterInteger, 0, SKIP_MINUS_1, NULL},
	{ "WowlKeyRot", NdisParameterInteger, 0, SKIP_MINUS_1, NULL},
	{ "DongleImagePath", NdisParameterString, 0, 0, NULL },
	{ "SROMImagePath", NdisParameterString, 0, 0, NULL },
	{ "VirtualDevice", NdisParameterInteger, 0, 0, NULL },
#if defined(BCMDHDUSB)
	{ "DnglResetAfterDisable", NdisParameterInteger, 0, 0, NULL },
#endif /* BCMDONGLEHOST && BCMDHDUSB */
	{ "SDHostCtrl", NdisParameterString, 0, 0, NULL },
	{ NULL, 0, 0, 0, NULL }
};

#if defined(EXT_STA) || defined(__BBSOFT__)
const static struct {
	UINT oidval;
	char *oidstr;
} oid_map[] = {
	{ OID_GEN_SUPPORTED_LIST, "OID_GEN_SUPPORTED_LIST" },
	{ OID_GEN_HARDWARE_STATUS, "OID_GEN_HARDWARE_STATUS" },
	{ OID_GEN_MEDIA_SUPPORTED, "OID_GEN_MEDIA_SUPPORTED" },
	{ OID_GEN_MEDIA_IN_USE, "OID_GEN_MEDIA_IN_USE" },
	{ OID_GEN_MAXIMUM_LOOKAHEAD, "OID_GEN_MAXIMUM_LOOKAHEAD" },
	{ OID_GEN_MAXIMUM_FRAME_SIZE, "OID_GEN_MAXIMUM_FRAME_SIZE" },
	{ OID_GEN_LINK_SPEED, "OID_GEN_LINK_SPEED" },
	{ OID_GEN_TRANSMIT_BUFFER_SPACE, "OID_GEN_TRANSMIT_BUFFER_SPACE" },
	{ OID_GEN_RECEIVE_BUFFER_SPACE, "OID_GEN_RECEIVE_BUFFER_SPACE" },
	{ OID_GEN_TRANSMIT_BLOCK_SIZE, "OID_GEN_TRANSMIT_BLOCK_SIZE" },
	{ OID_GEN_RECEIVE_BLOCK_SIZE, "OID_GEN_RECEIVE_BLOCK_SIZE" },
	{ OID_GEN_VENDOR_ID, "OID_GEN_VENDOR_ID" },
	{ OID_GEN_VENDOR_DESCRIPTION, "OID_GEN_VENDOR_DESCRIPTION" },
	{ OID_GEN_CURRENT_PACKET_FILTER, "OID_GEN_CURRENT_PACKET_FILTER" },
	{ OID_GEN_CURRENT_LOOKAHEAD, "OID_GEN_CURRENT_LOOKAHEAD" },
	{ OID_GEN_DRIVER_VERSION, "OID_GEN_DRIVER_VERSION" },
	{ OID_GEN_MAXIMUM_TOTAL_SIZE, "OID_GEN_MAXIMUM_TOTAL_SIZE" },
	{ OID_GEN_PROTOCOL_OPTIONS, "OID_GEN_PROTOCOL_OPTIONS" },
	{ OID_GEN_MAC_OPTIONS, "OID_GEN_MAC_OPTIONS" },
	{ OID_GEN_MEDIA_CONNECT_STATUS, "OID_GEN_MEDIA_CONNECT_STATUS" },
	{ OID_GEN_MAXIMUM_SEND_PACKETS, "OID_GEN_MAXIMUM_SEND_PACKETS" },
	{ OID_GEN_VENDOR_DRIVER_VERSION, "OID_GEN_VENDOR_DRIVER_VERSION" },
#if defined(WDM)
	{ OID_GEN_SUPPORTED_GUIDS, "OID_GEN_SUPPORTED_GUIDS" },
#endif /* defined(WDM) */
#ifdef OID_GEN_PHYSICAL_MEDIUM
	{ OID_GEN_PHYSICAL_MEDIUM, "OID_GEN_PHYSICAL_MEDIUM" },
#endif /* OID_GEN_PHYSICAL_MEDIUM */
	{ OID_GEN_XMIT_OK, "OID_GEN_XMIT_OK" },
	{ OID_GEN_RCV_OK, "OID_GEN_RCV_OK" },
	{ OID_GEN_XMIT_ERROR, "OID_GEN_XMIT_ERROR" },
	{ OID_GEN_RCV_ERROR, "OID_GEN_RCV_ERROR" },
	{ OID_GEN_RCV_NO_BUFFER, "OID_GEN_RCV_NO_BUFFER" },
#ifdef NDIS60
#ifdef PRE_v1_68_HEADERS
	{ OID_GEN_STATISTICS, "OID_GEN_STATISTICS" },
#endif /* PRE_v1_68_HEADERS */
#endif /* NDIS60 */
	{ OID_802_3_PERMANENT_ADDRESS, "OID_802_3_PERMANENT_ADDRESS" },
	{ OID_802_3_CURRENT_ADDRESS, "OID_802_3_CURRENT_ADDRESS" },
	{ OID_802_3_MULTICAST_LIST, "OID_802_3_MULTICAST_LIST" },
	{ OID_802_3_MAXIMUM_LIST_SIZE, "OID_802_3_MAXIMUM_LIST_SIZE" },
#ifndef WDM	  /* WHQL fix: Win98 & NT5 only use OID_GEN_MAC_OPTIONS */
	{ OID_802_3_MAC_OPTIONS, "OID_802_3_MAC_OPTIONS" },
#endif /* WDM */
	{ OID_802_3_RCV_ERROR_ALIGNMENT, "OID_802_3_RCV_ERROR_ALIGNMENT" },
	{ OID_802_3_XMIT_ONE_COLLISION, "OID_802_3_XMIT_ONE_COLLISION" },
	{ OID_802_3_XMIT_MORE_COLLISIONS, "OID_802_3_XMIT_MORE_COLLISIONS" },
#ifdef STA
#ifndef EXT_STA
	{ OID_802_11_BSSID, "OID_802_11_BSSID" },
	{ OID_802_11_SSID, "OID_802_11_SSID" },
	{ OID_802_11_INFRASTRUCTURE_MODE, "OID_802_11_INFRASTRUCTURE_MODE" },
	{ OID_802_11_ADD_WEP, "OID_802_11_ADD_WEP" },
	{ OID_802_11_REMOVE_WEP, "OID_802_11_REMOVE_WEP" },
	{ OID_802_11_DISASSOCIATE, "OID_802_11_DISASSOCIATE" },
	{ OID_802_11_AUTHENTICATION_MODE, "OID_802_11_AUTHENTICATION_MODE" },
	{ OID_802_11_PRIVACY_FILTER, "OID_802_11_PRIVACY_FILTER" },
	{ OID_802_11_BSSID_LIST_SCAN, "OID_802_11_BSSID_LIST_SCAN" },
	{ OID_802_11_ENCRYPTION_STATUS, "OID_802_11_ENCRYPTION_STATUS" },
	{ OID_802_11_RELOAD_DEFAULTS, "OID_802_11_RELOAD_DEFAULTS" },
	{ OID_802_11_ADD_KEY, "OID_802_11_ADD_KEY" },
	{ OID_802_11_REMOVE_KEY, "OID_802_11_REMOVE_KEY" },
	{ OID_802_11_ASSOCIATION_INFORMATION, "OID_802_11_ASSOCIATION_INFORMATION" },
	{ OID_802_11_TEST, "OID_802_11_TEST" },
#ifdef BCMWPA2
	{ OID_802_11_CAPABILITY, "OID_802_11_CAPABILITY" },
	{ OID_802_11_PMKID, "OID_802_11_PMKID" },
#endif /* BCMWPA2 */
	{ OID_802_11_NETWORK_TYPES_SUPPORTED, "OID_802_11_NETWORK_TYPES_SUPPORTED" },
	{ OID_802_11_NETWORK_TYPE_IN_USE, "OID_802_11_NETWORK_TYPE_IN_USE" },
	{ OID_802_11_TX_POWER_LEVEL, "OID_802_11_TX_POWER_LEVEL" },
	{ OID_802_11_RSSI, "OID_802_11_RSSI" },
	{ OID_802_11_RSSI_TRIGGER, "OID_802_11_RSSI_TRIGGER" },
	{ OID_802_11_FRAGMENTATION_THRESHOLD, "OID_802_11_FRAGMENTATION_THRESHOLD" },
	{ OID_802_11_RTS_THRESHOLD, "OID_802_11_RTS_THRESHOLD" },
	{ OID_802_11_NUMBER_OF_ANTENNAS, "OID_802_11_NUMBER_OF_ANTENNAS" },
	{ OID_802_11_RX_ANTENNA_SELECTED, "OID_802_11_RX_ANTENNA_SELECTED" },
	{ OID_802_11_TX_ANTENNA_SELECTED, "OID_802_11_TX_ANTENNA_SELECTED" },
	{ OID_802_11_SUPPORTED_RATES, "OID_802_11_SUPPORTED_RATES" },
	{ OID_802_11_DESIRED_RATES, "OID_802_11_DESIRED_RATES" },
	{ OID_802_11_CONFIGURATION, "OID_802_11_CONFIGURATION" },
	{ OID_802_11_POWER_MODE, "OID_802_11_POWER_MODE" },
	{ OID_802_11_BSSID_LIST, "OID_802_11_BSSID_LIST" },
	{ OID_802_11_STATISTICS, "OID_802_11_STATISTICS" },
#else /* !EXT_STA */
	{ OID_DOT11_MPDU_MAX_LENGTH, "OID_DOT11_MPDU_MAX_LENGTH" },
	{ OID_DOT11_OPERATION_MODE_CAPABILITY, "OID_DOT11_OPERATION_MODE_CAPABILITY" },
	{ OID_DOT11_SUPPORTED_PHY_TYPES, "OID_DOT11_SUPPORTED_PHY_TYPES" },
	{ OID_DOT11_SUPPORTED_POWER_LEVELS, "OID_DOT11_SUPPORTED_POWER_LEVELS" },
	{ OID_DOT11_CURRENT_TX_POWER_LEVEL, "OID_DOT11_CURRENT_TX_POWER_LEVEL" },
	{ OID_DOT11_CURRENT_REG_DOMAIN, "OID_DOT11_CURRENT_REG_DOMAIN" },
	{ OID_DOT11_SUPPORTED_DATA_RATES_VALUE, "OID_DOT11_SUPPORTED_DATA_RATES_VALUE" },
	{ OID_DOT11_CURRENT_CHANNEL_NUMBER, "OID_DOT11_CURRENT_CHANNEL_NUMBER" },
	{ OID_DOT11_SUPPORTED_UNICAST_ALGORITHM_PAIR,
	"OID_DOT11_SUPPORTED_UNICAST_ALGORITHM_PAIR" },
	{ OID_DOT11_SUPPORTED_MULTICAST_ALGORITHM_PAIR,
	"OID_DOT11_SUPPORTED_MULTICAST_ALGORITHM_PAIR" },
	{ OID_DOT11_DATA_RATE_MAPPING_TABLE, "OID_DOT11_DATA_RATE_MAPPING_TABLE" },
	{ OID_DOT11_CURRENT_OPERATION_MODE, "OID_DOT11_CURRENT_OPERATION_MODE" },
	{ OID_DOT11_SCAN_REQUEST, "OID_DOT11_SCAN_REQUEST" },
	{ OID_DOT11_RESET_REQUEST, "OID_DOT11_RESET_REQUEST" },
	{ OID_DOT11_NIC_POWER_STATE, "OID_DOT11_NIC_POWER_STATE" },
	{ OID_DOT11_OPERATIONAL_RATE_SET, "OID_DOT11_OPERATIONAL_RATE_SET" },
	{ OID_DOT11_BEACON_PERIOD, "OID_DOT11_BEACON_PERIOD" },
	{ OID_DOT11_MAC_ADDRESS, "OID_DOT11_MAC_ADDRESS" },
	{ OID_DOT11_RTS_THRESHOLD, "OID_DOT11_RTS_THRESHOLD" },
	{ OID_DOT11_SHORT_RETRY_LIMIT, "OID_DOT11_SHORT_RETRY_LIMIT" },
	{ OID_DOT11_FRAGMENTATION_THRESHOLD, "OID_DOT11_FRAGMENTATION_THRESHOLD" },
	{ OID_DOT11_CURRENT_CHANNEL, "OID_DOT11_CURRENT_CHANNEL" },
	{ OID_DOT11_REG_DOMAINS_SUPPORT_VALUE, "OID_DOT11_REG_DOMAINS_SUPPORT_VALUE" },
	{ OID_DOT11_CURRENT_FREQUENCY, "OID_DOT11_CURRENT_FREQUENCY" },
	{ OID_DOT11_NIC_SPECIFIC_EXTENSION, "OID_DOT11_NIC_SPECIFIC_EXTENSION" },
	{ OID_DOT11_RECV_SENSITIVITY_LIST, "OID_DOT11_RECV_SENSITIVITY_LIST" },
	{ OID_DOT11_CURRENT_ADDRESS, "OID_DOT11_CURRENT_ADDRESS" },
	{ OID_DOT11_PERMANENT_ADDRESS, "OID_DOT11_PERMANENT_ADDRESS" },
	{ OID_DOT11_MULTICAST_LIST, "OID_DOT11_MULTICAST_LIST" },
	{ OID_DOT11_AUTO_CONFIG_ENABLED, "OID_DOT11_AUTO_CONFIG_ENABLED" },
	{ OID_DOT11_ENUM_BSS_LIST, "OID_DOT11_ENUM_BSS_LIST" },
	{ OID_DOT11_FLUSH_BSS_LIST, "OID_DOT11_FLUSH_BSS_LIST" },
	{ OID_DOT11_POWER_MGMT_REQUEST, "OID_DOT11_POWER_MGMT_REQUEST" },
	{ OID_DOT11_DESIRED_SSID_LIST, "OID_DOT11_DESIRED_SSID_LIST" },
	{ OID_DOT11_EXCLUDED_MAC_ADDRESS_LIST, "OID_DOT11_EXCLUDED_MAC_ADDRESS_LIST" },
	{ OID_DOT11_DESIRED_BSSID_LIST, "OID_DOT11_DESIRED_BSSID_LIST" },
	{ OID_DOT11_DESIRED_BSS_TYPE, "OID_DOT11_DESIRED_BSS_TYPE" },
#ifdef BCMWPA2
	{ OID_DOT11_PMKID_LIST, "OID_DOT11_PMKID_LIST" },
#endif /* BCMWPA2 */
	{ OID_DOT11_CONNECT_REQUEST, "OID_DOT11_CONNECT_REQUEST" },
	{ OID_DOT11_EXCLUDE_UNENCRYPTED, "OID_DOT11_EXCLUDE_UNENCRYPTED" },
	{ OID_DOT11_PRIVACY_EXEMPTION_LIST, "OID_DOT11_PRIVACY_EXEMPTION_LIST" },
	{ OID_DOT11_ENABLED_AUTHENTICATION_ALGORITHM,
	"OID_DOT11_ENABLED_AUTHENTICATION_ALGORITHM" },
	{ OID_DOT11_ENABLED_UNICAST_CIPHER_ALGORITHM,
	"OID_DOT11_ENABLED_UNICAST_CIPHER_ALGORITHM" },
	{ OID_DOT11_ENABLED_MULTICAST_CIPHER_ALGORITHM,
	"OID_DOT11_ENABLED_MULTICAST_CIPHER_ALGORITHM" },
	{ OID_DOT11_CIPHER_DEFAULT_KEY_ID, "OID_DOT11_CIPHER_DEFAULT_KEY_ID" },
	{ OID_DOT11_CIPHER_DEFAULT_KEY, "OID_DOT11_CIPHER_DEFAULT_KEY" },
	{ OID_DOT11_CIPHER_KEY_MAPPING_KEY, "OID_DOT11_CIPHER_KEY_MAPPING_KEY" },
	{ OID_DOT11_ENUM_ASSOCIATION_INFO, "OID_DOT11_ENUM_ASSOCIATION_INFO" },
	{ OID_DOT11_DISCONNECT_REQUEST, "OID_DOT11_DISCONNECT_REQUEST" },
	{ OID_DOT11_UNICAST_USE_GROUP_ENABLED, "OID_DOT11_UNICAST_USE_GROUP_ENABLED" },
	{ OID_DOT11_HARDWARE_PHY_STATE, "OID_DOT11_HARDWARE_PHY_STATE" },
	{ OID_DOT11_DESIRED_PHY_LIST, "OID_DOT11_DESIRED_PHY_LIST" },
	{ OID_DOT11_CURRENT_PHY_ID, "OID_DOT11_CURRENT_PHY_ID" },
	{ OID_DOT11_MEDIA_STREAMING_ENABLED, "OID_DOT11_MEDIA_STREAMING_ENABLED" },
	{ OID_DOT11_UNREACHABLE_DETECTION_THRESHOLD, "OID_DOT11_UNREACHABLE_DETECTION_THRESHOLD" },
	{ OID_DOT11_ACTIVE_PHY_LIST, "OID_DOT11_ACTIVE_PHY_LIST" },
	{ OID_DOT11_EXTSTA_CAPABILITY, "OID_DOT11_EXTSTA_CAPABILITY" },
	{ OID_DOT11_IBSS_PARAMS, "OID_DOT11_IBSS_PARAMS" },
	{ OID_DOT11_HIDDEN_NETWORK_ENABLED, "OID_DOT11_HIDDEN_NETWORK_ENABLED" },
	{ OID_DOT11_STATISTICS, "OID_DOT11_STATISTICS" },
#endif /* !EXT_STA */
#endif /* STA */
	{ OID_PNP_CAPABILITIES, "OID_PNP_CAPABILITIES" },
#if defined(WDM)
	{ OID_PNP_SET_POWER, "OID_PNP_SET_POWER" },
	{ OID_PNP_QUERY_POWER, "OID_PNP_QUERY_POWER" },
	{ OID_PNP_ADD_WAKE_UP_PATTERN, "OID_PNP_ADD_WAKE_UP_PATTERN" },
	{ OID_PNP_REMOVE_WAKE_UP_PATTERN, "OID_PNP_REMOVE_WAKE_UP_PATTERN" },
	{ OID_PNP_ENABLE_WAKE_UP, "OID_PNP_ENABLE_WAKE_UP" },
	{ OID_TCP_TASK_OFFLOAD, "OID_TCP_TASK_OFFLOAD" },
#endif /* defined(WDM) */
	{ OID_BCM_SETINFORMATION, "OID_BCM_SETINFORMATION" },
	{ OID_BCM_GETINFORMATION, "OID_BCM_GETINFORMATION" },
	{ WL_OID_BASE + WLC_GET_RADIO, "WLC_GET_RADIO" },
	{ WL_OID_BASE + WLC_SET_RADIO, "WLC_SET_RADIO" },
	};
#endif 

#if defined(BCMDBG)
int *nd_msg_level = &wl_msg_level;
static struct wl_info *wl_info_dbg = (struct wl_info *)(NULL);
#endif /* defined (BCMDBG) */

#ifdef TOE


/* The offload capabilities of the miniport */
typedef struct _NIC_TASK_OFFLOAD
{
	ULONG   ChecksumOffload:1;
	ULONG   LargeSendOffload:1;
	ULONG   IpSecOffload:1;
} NIC_TASK_OFFLOAD;

/* Checksum offload capabilities */
typedef struct _NIC_CHECKSUM_OFFLOAD
{
	ULONG   DoXmitTcpChecksum:1;
	ULONG   DoRcvTcpChecksum:1;
	ULONG   DoXmitUdpChecksum:1;
	ULONG   DoRcvUdpChecksum:1;
	ULONG   DoXmitIpChecksum:1;
	ULONG   DoRcvIpChecksum:1;
} NIC_CHECKSUM_OFFLOAD;


/* This miniport only supports one encapsulation type: IEEE_802_3_Encapsulation
 * one task version: NDIS_TASK_OFFLOAD_VERSION. Modify the code below OID_TCP_
 * TASK_OFFLOAD in query and setting information functions to make it support
 * more than one encapsulation type and task version

 * Define the task offload the miniport currently supports.
 * This miniport only supports two kinds of offload tasks:
 * TCP/IP checksum offload.
 * Later if it can supports more tasks, just redefine this task array
 */

NDIS_TASK_OFFLOAD OffloadTasks[] = {
	{
		NDIS_TASK_OFFLOAD_VERSION,
		sizeof(NDIS_TASK_OFFLOAD),
		TcpIpChecksumNdisTask,
		0,
		sizeof(NDIS_TASK_TCP_IP_CHECKSUM)
	}
};

/* Get the number of offload tasks this miniport supports */
ULONG OffloadTasksCount = sizeof(OffloadTasks) / sizeof(OffloadTasks[0]);

/*
 * Specify TCP/IP checksum offload task, the miniport can support
 * TCP checksum and IP checksum, also it supports checksum with TCP and IP
 * options.  Udp checksum is optional in IPV4 and larger datagrams'
 * checksum lies in one udp-header in the ip-fragments, for which we
 * need to implement udp-checksum-handling on per udp-port basis.
 * Matrix to enable checksums that we support.
 * 	ipopt tcpopt tcpcsum udpcsum iphdrcsum
 * V4tx  1	1	1	0	1
 * V4rx  1	1	1	0	1
 * V6tx  0	0	0	0	na
 * V6rx  0	0	0	0	na
 */
NDIS_TASK_TCP_IP_CHECKSUM TcpIpChecksumTask = {
	{1, 1, 1, 0, 1},
	{1, 1, 1, 0, 1},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

#endif /* TOE */


#ifdef EXT_STA
#define	IND_BLOCK_CONNECT	(1 << 0)
#define	IND_BLOCK_ROAM		(1 << 1)
#define IND_BLOCK_ASSOC		(1 << 2)

#define	SW_TIMER_LINK_REPORT	10

#ifdef WL11N
/* DataRateIndex values for the mcs rates */
const uint8 mcs_dri[MCS_TABLE_SIZE * 2] = {
	3,   5,		/* MCS 0 */
	7,   8,		/* MCS 1 */
	10,  13,	/* MCS 2 */
	14,  15,	/* MCS 3 */
	16,  17,	/* MCS 4 */
	19,  20,	/* MCS 5 */
	21,  23,	/* MCS 6 */
	25,  26,	/* MCS 7 */
	27,  28,	/* MCS 8 */
	29,  30,	/* MCS 9 */
	31,  32,	/* MCS 10 */
	33,  34,	/* MCS 11 */
	35,  37,	/* MCS 12 */
	38,  39,	/* MCS 13 */
	40,  41,	/* MCS 14 */
	42,  43,	/* MCS 15 */
	45,  46,	/* MCS 16 */
	47,  49,	/* MCS 17 */
	50,  51,	/* MCS 18 */
	52,  53,	/* MCS 19 */
	55,  56,	/* MCS 20 */
	57,  58,	/* MCS 21 */
	59,  60,	/* MCS 22 */
	61,  62,	/* MCS 23 */
	63,  64,	/* MCS 24 */
	65,  67,	/* MCS 25 */
	68,  69,	/* MCS 26 */
	70,  71,	/* MCS 27 */
	73,  74,	/* MCS 28 */
	75,  76,	/* MCS 29 */
	77,  78,	/* MCS 30 */
	79,  80,	/* MCS 31 */
	81,  82		/* MCS 32 */
};
#endif /* WL11N */
#endif /* EXT_STA */

#ifdef NDIS60
static NDIS_HANDLE wl_adapter_handle = NULL;
static NDIS_HANDLE wl_driver_context = NULL;
#endif /* NDIS60 */

#if defined(BCMDHDUSB)
/* When updating Advance properties, add delay to give dongle time
 * to finish rebooting before re-enabling driver; Need global because
 * wl is freed already inside wl_munload().
 */
bool g_dnglResetAfterDisable = FALSE;
#endif /* BCMDONGLEHOST && BCMDHDUSB */

static int wl_instance = 0;		/* driver instance count, incremented in wl_minit */
static int MinorNdisVersion;		/* predefined ndis minor version */

#if !defined(NDIS60)
#ifdef P2P
struct p2p_state gP2P;
#endif /* P2P */
#endif /* UNDER_CE && ! NDIS60 */

#define DLLEXPORT __declspec(dllexport)


/* Driver Entry */
DLLEXPORT
NTSTATUS
DriverEntry(
	PDRIVER_OBJECT DriverObject,
	PUNICODE_STRING RegistryPath
);

#ifdef NDIS60
static NDIS_STATUS
wl_msetoptions(
	IN NDIS_HANDLE NdisMiniportDriverHandle,
	IN NDIS_HANDLE MiniportDriverContext
);

static VOID
wl_munload(IN PDRIVER_OBJECT DriverObject);

static NDIS_STATUS
wl_mpause(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_MINIPORT_PAUSE_PARAMETERS MiniportPauseParameters
);

static NDIS_STATUS
wl_mrestart(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_MINIPORT_RESTART_PARAMETERS MiniportRestartParameters
);

static BOOLEAN
wl_mcheckforhang(IN NDIS_HANDLE MiniportAdapterContext);

static VOID
wl_mPnPeventnotify(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNET_DEVICE_PNP_EVENT NetDevicePnPEvent
);

static VOID
wl_mcanceloid(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_OID_REQUEST OidRequest
);
#endif /* NDIS60 */

/* NDIS miniport functions */
#ifndef NDIS60
static NDIS_STATUS wl_minit(
	PNDIS_STATUS OpenErrorStatus,
	PUINT SelectedMediumIndex,
	PNDIS_MEDIUM MediumArray,
	UINT MediumArraySize,
	NDIS_HANDLE MiniportAdapterHandle,
	NDIS_HANDLE ConfigurationHandle
);

static void wl_mhalt(IN NDIS_HANDLE MiniportAdapterContext);
static NDIS_STATUS wl_mreset(OUT PBOOLEAN AddressingReset, IN NDIS_HANDLE MiniportAdapterContext);
#else /* NDIS60 */
static NDIS_STATUS wl_minit(
	IN NDIS_HANDLE MiniportAdapterHandle,
	IN NDIS_HANDLE MiniportDriverContext,
	IN PNDIS_MINIPORT_INIT_PARAMETERS MiniportInitParameters);
static void wl_mhalt(IN NDIS_HANDLE MiniportAdapterContext, IN NDIS_HALT_ACTION HaltAction);
static NDIS_STATUS wl_mreset(IN NDIS_HANDLE MiniportAdapterContext, OUT PBOOLEAN AddressingReset);
static void wl_mshutdown60(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN NDIS_SHUTDOWN_ACTION ShutdownAction);
static NDIS_STATUS
wl_moidrequest60(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_OID_REQUEST OidRequest
);
#endif /* NDIS60 */
static void wl_mshutdown(IN PVOID ShutdownContext);
#ifndef NDIS60
static void wl_msendpackets(NDIS_HANDLE MiniportAdapterContext,
			    PPNDIS_PACKET PacketArray, UINT NumPackets);
static void wl_mrxreturn(NDIS_HANDLE MiniportAdapterContext, PNDIS_PACKET p);
#else
static void wl_msendpackets(IN NDIS_HANDLE MiniportAdapterContext,
			    IN PNET_BUFFER_LIST NetBufferLists, IN NDIS_PORT_NUMBER PortNumber,
			    IN ULONG SendFlags);

static void wl_mcancelsend(IN NDIS_HANDLE MiniportAdapterContext, IN PVOID CancelId);
static void wl_mrxreturn(NDIS_HANDLE MiniportAdapterContext, IN PNET_BUFFER_LIST NetBufferLists,
			 IN ULONG ReturnFlags);
static void wl_mposttxds(IN PDEVICE_OBJECT pDO, IN PVOID Reserved, IN PSCATTER_GATHER_LIST pSGL,
	IN PVOID Context);
#endif /* !NDIS60 */

extern void sdstd_isr(PBOOLEAN InterruptRecognized, PBOOLEAN QueueMiniportHandleInterrupt,
	NDIS_HANDLE MiniportAdapterContext);
extern void sdstd_dpc(NDIS_HANDLE MiniportAdapterContext);
static void wl_abrate_convert(wl_info_t *wl, ULONG *link_speed);

#if defined(NDIS51)
void
wl_PnP_event(
	IN  NDIS_HANDLE             MiniportAdapterContext,
	IN  NDIS_DEVICE_PNP_EVENT   PnPEvent,
	IN  PVOID                   InformationBuffer,
	IN  ULONG                   InformationBufferLength
);
#endif /* defined (NDIS51) */

/* local helper functions */
static NDIS_STATUS wl_readconfigdata(
	IN wl_info_t *wl,
	IN NDIS_HANDLE ConfigurationHandle,
	IN NDIS_HANDLE MiniportAdapterHandle
);

#ifndef NDIS60
static NDIS_STATUS wl_find_resources(wl_info_t *wl);
#else
static NDIS_STATUS wl_find_resources(wl_info_t *wl, PNDIS_RESOURCE_LIST reslist);
#endif /* !NDIS60 */
static void wl_freeresources(wl_info_t *wl);
static void wl_dev_gone(void *wlh);

#ifndef EXT_STA
static void wl_mic_error(wl_info_t *wl, struct ether_addr *ea, bool group, bool flush_txq);
static void wl_process_link_down(wl_info_t *wl);
#else
static void wl_mic_error(wl_info_t *wl, struct ether_addr *ea, int key_index, bool group,
	bool flush_txq);
#endif /* !EXT_STA */

#if NDIS_WDM
static bool pcmcia_set_16bit(wl_info_t *wl, PDEVICE_OBJECT pdo, PDEVICE_OBJECT fdo, void *va,
	PHYSICAL_ADDRESS pa, uint len);
#endif /* NDIS_WDM */

#ifdef BCMDBG_MEM
static void wl_malloc_dump(shared_info_t *sh);
#endif /* BCMDBG_MEM */

/* Airopeek helper */

#ifdef BCMWPA2
static uint wl_pmkid_cache_ind(pmkid_cand_list_t *pmkid_list, void *cand_buffer);
#endif /* BCMWPA2 */

const char *wl_oid2str(UINT oidval)
{
	static char buffer[100];
#if defined(EXT_STA) || defined(__BBSOFT__)
	int i;

	for (i = 0; i < ARRAYSIZE(oid_map); i++) {
		if (oidval == oid_map[i].oidval) {
			return oid_map[i].oidstr;
		}
	}
#endif 

	sprintf(buffer, "<unknown> 0x%x", (uint)oidval);
	return buffer;
}


#ifdef NDIS60
static NDIS_STATUS
wl_msetoptions(
	IN NDIS_HANDLE NdisMiniportDriverHandle,
	IN NDIS_HANDLE MiniportDriverContext
);

static VOID
wl_munload(IN PDRIVER_OBJECT DriverObject);

static NDIS_STATUS
wl_mpause(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_MINIPORT_PAUSE_PARAMETERS MiniportPauseParameters
);

static NDIS_STATUS
wl_mrestart(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_MINIPORT_RESTART_PARAMETERS MiniportRestartParameters
);

static BOOLEAN
wl_mcheckforhang(IN NDIS_HANDLE MiniportAdapterContext);

static VOID
wl_mPnPeventnotify(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNET_DEVICE_PNP_EVENT NetDevicePnPEvent
);

static VOID
wl_mcanceloid(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_OID_REQUEST OidRequest
);
#endif /* NDIS60 */


static bool
wl_set_ndis_iovar(wl_info_t *wl, PVOID InfoBuf, ULONG InfoBufLen, NDIS_STATUS *status);

#if (defined(EXT_STA) || defined(DHD_NDIS_OID))
uint wl_get_bandstate(wl_info_t *wl);
#endif /* BCMDONGLEHOST && EXT_STA */

#ifdef DHD_NDIS_OID
extern NDIS_STATUS dhd_oid_attach(wl_info_t *wl);
extern void dhd_oid_detach(wl_info_t *wl);
#endif /* DHD_NDIS_OID */

DLLEXPORT
NTSTATUS
DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
)
{
	NDIS_STATUS status;
#ifndef NDIS60
	NDIS_MINIPORT_CHARACTERISTICS wl_char;
#else
	NDIS_MINIPORT_DRIVER_CHARACTERISTICS wl_char;
#endif /* !NDIS60 */
#ifndef WLBTAMP
#if !defined(WLBTWUSB) && !defined(WLBTCEAMP11) && !defined(NDIS60)
	NDIS_HANDLE wl_wrapper_handle;
#endif
#endif /* WLBTAMP */

#ifdef BCMDBG
#ifdef	TEST_N_EXIT
	wl_msg_level = 0xffff;
#endif /* TEST_N_EXIT */
	printf("\nwl: Entered the Broadcom 802.11 DCF-only driver version %s " \
	       "(Compiled at %s on %s)\n",
	       EPI_VERSION_STR, __TIME__, __DATE__);
#endif /* BCMDBG */

#ifndef NDIS60
	NdisMInitializeWrapper(&wl_wrapper_handle, DriverObject, RegistryPath, NULL);

	bzero(&wl_char, sizeof(wl_char));
	wl_char.MajorNdisVersion = MAJOR_NDIS_VERSION;
	wl_char.MinorNdisVersion = MINOR_NDIS_VERSION;
	wl_char.HaltHandler = wl_mhalt;
	wl_char.InitializeHandler = wl_minit;


	wl_char.QueryInformationHandler = wl_mqueryinformation;
	wl_char.ResetHandler = wl_mreset;
	wl_char.SetInformationHandler = wl_msetinformation;
	wl_char.SendPacketsHandler = wl_msendpackets;
	wl_char.ReturnPacketHandler = wl_mrxreturn;
#if defined(NDIS51)
	wl_char.CancelSendPacketsHandler = NULL;
	wl_char.PnPEventNotifyHandler    = wl_PnP_event;
	wl_char.AdapterShutdownHandler   = wl_mshutdown;
#endif /* defined (NDIS51) */


	status = ~NDIS_STATUS_SUCCESS;
	while (NDIS_ERROR(status) && (wl_char.MinorNdisVersion >= 0)) {
		status = NdisMRegisterMiniport(wl_wrapper_handle, &wl_char, sizeof(wl_char));
		if (NDIS_ERROR(status)) {
			WL_ERROR(("NdisMRegisterMiniport Failed with NDIS version %d.%d\n",
				MAJOR_NDIS_VERSION, MINOR_NDIS_VERSION));
			wl_char.MinorNdisVersion--;
		}
	}

	if (NDIS_ERROR(status)) {
		WL_ERROR(("NdisMRegisterMiniport giving up with NDIS version %d.%d\n",
			MAJOR_NDIS_VERSION, wl_char.MinorNdisVersion));
		NdisTerminateWrapper(wl_wrapper_handle, NULL);
		return (STATUS_UNSUCCESSFUL);
	}

	MinorNdisVersion = wl_char.MinorNdisVersion;
	WL_INFORM(("NdisMRegisterMiniport Success with NDIS %d.%d\n", MAJOR_NDIS_VERSION,
		wl_char.MinorNdisVersion));
#else /* !NDIS60 */
	bzero(&wl_char, sizeof(wl_char));


	wl_char.Header.Type = NDIS_OBJECT_TYPE_MINIPORT_DRIVER_CHARACTERISTICS;
	wl_char.Header.Size = sizeof(NDIS_MINIPORT_DRIVER_CHARACTERISTICS);
	wl_char.Header.Revision = NDIS_MINIPORT_DRIVER_CHARACTERISTICS_REVISION_1;

	wl_char.MajorNdisVersion = MAJOR_NDIS_VERSION;
	wl_char.MinorNdisVersion = MINOR_NDIS_VERSION;
	wl_char.MajorDriverVersion = EPI_MAJOR_VERSION;
	wl_char.MinorDriverVersion = EPI_MINOR_VERSION;

	wl_char.SetOptionsHandler = wl_msetoptions;
	wl_char.InitializeHandlerEx = wl_minit;
	wl_char.HaltHandlerEx = wl_mhalt;
	wl_char.UnloadHandler = wl_munload;
	wl_char.PauseHandler = wl_mpause;
	wl_char.RestartHandler = wl_mrestart;
	wl_char.OidRequestHandler = wl_moidrequest60;
	wl_char.SendNetBufferListsHandler = wl_msendpackets;
	wl_char.ReturnNetBufferListsHandler = wl_mrxreturn;
	wl_char.CancelSendHandler = wl_mcancelsend;
	wl_char.CheckForHangHandlerEx = wl_mcheckforhang;
	wl_char.ResetHandlerEx = wl_mreset;
	wl_char.DevicePnPEventNotifyHandler = wl_mPnPeventnotify;
	wl_char.ShutdownHandlerEx = wl_mshutdown60;
	wl_char.CancelOidRequestHandler = wl_mcanceloid;

	status = NdisMRegisterMiniportDriver(DriverObject, RegistryPath,
		wl_driver_context, &wl_char, &wl_adapter_handle);
#ifdef BCMDBG
	if (NDIS_ERROR(status)) {
		WL_ERROR(("NdisMRegisterMiniportDriver Failed, status=0x%x\n", status));
	} else {
		WL_ERROR(("NdisMRegisterMiniportDriver Succeeded\n"));
	}
#endif /* BCMDBG */
#endif /* !NDIS60 */

#ifdef WLBTAMP
#if defined(WLBTWUSB)
	/* Load interface with Bluetooth */
	if (status == NDIS_STATUS_SUCCESS) {
		/* Try finding BTKRNL */
		BtKernFound = FindBtKern2000() ? TRUE : FALSE;
		BTWUSB_KdPrint(("BTKRNL Found flag is %d\n", BtKernFound));
	}
#elif defined(WLBTCEAMP11)
	/* Load interface with Bluetooth */
	if (status == NDIS_STATUS_SUCCESS) {
		/* Try finding BTKRNL */
		BtKernFound = FindBtKern() ? TRUE : FALSE;
		KdPrint("BTKRNL Found flag is %d\n", BtKernFound);
	}
#endif
#endif /* WLBTAMP */

#if defined(__BBSOFT__) && defined(__ZELOS__)
	if (!g_pGPIOReg) {
		g_pGPIOReg = (volatile GPIO_REG *)DrvLib_MapIoSpace(BASE_REG_PA_GPIO, sizeof(GPIO_REG), FALSE);
		if (!g_pGPIOReg) {
			WL_ERROR(("pGPIOReg DrvLib_MapIoSpace() Failed \n"));
			return -1;
		}
	}
#endif
	
	return (status);
}

#ifdef NDIS60
static NDIS_STATUS
wl_msetoptions(
	IN NDIS_HANDLE NdisMiniportDriverHandle,
	IN NDIS_HANDLE MiniportDriverContext
)
{
	return (NDIS_STATUS_SUCCESS);
}

static VOID
wl_munload(IN PDRIVER_OBJECT DriverObject)
{
#if defined(EXT_STA)
#if defined(BCMDHDUSB)
	if (g_dnglResetAfterDisable) {
		/* wait for dongle to reboot */
		NdisMSleep(1000 * 2000); /* 2s */
	}
#endif /* BCMDHDUSB */
#endif /* BCMDONGLEHOST && EXT_STA */

	NdisMDeregisterMiniportDriver(wl_adapter_handle);
	WL_ERROR(("wl_munload\n"));
}

static NDIS_STATUS
wl_mpause(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_MINIPORT_PAUSE_PARAMETERS MiniportPauseParameters
)
{
	return NDIS_STATUS_SUCCESS;
}

static NDIS_STATUS
wl_mrestart(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_MINIPORT_RESTART_PARAMETERS MiniportRestartParameters
)
{
	return NDIS_STATUS_SUCCESS;
}

static BOOLEAN
wl_mcheckforhang(IN NDIS_HANDLE MiniportAdapterContext)
{
	return FALSE;
}

static VOID
wl_mPnPeventnotify(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNET_DEVICE_PNP_EVENT NetDevicePnPEvent
)
{
	wl_info_t *wl = WL_INFO(MiniportAdapterContext);
	NDIS_DEVICE_PNP_EVENT  PnPEvent = NetDevicePnPEvent->DevicePnPEvent;
	IN  PVOID InformationBuffer = NetDevicePnPEvent->InformationBuffer;

	WL_ERROR(("Entered wl_PnP_event\n"));
	ND_LOCK(wl);

	switch (PnPEvent) {
	case NdisDevicePnPEventSurpriseRemoved:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventSurpriseRemoved\n"));
#if defined(BCMDHDUSB)
#if defined(EXT_STA)
		wl->devhalted = TRUE;
#endif /* EXT_STA */
		dhd_pnp_surp_remove(wl->dhd);
#endif	/* BCMDONGLEHOST && BCMDHDUSB */
		wl_down(wl);
		break;

	case NdisDevicePnPEventPowerProfileChanged:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventPowerProfileChanged\n"));
		wl->acpwr = (*((ULONG*)InformationBuffer) != NdisPowerProfileBattery);
		WL_ERROR(("Power profile %d\n", *((ULONG *)InformationBuffer)));
		break;
	default:
		WL_ERROR(("PnPEventNotify: unknown PnP event %x \n", PnPEvent));
		break;
	}

	ND_UNLOCK(wl);
}

static VOID
wl_mcanceloid(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_OID_REQUEST OidRequest
)
{
}
#endif /* NDIS60 */

#ifdef EXT_STA
static DOT11_PHY_TYPE
wl_phytype2ndistype(uint phy_type)
{
	switch (phy_type) {
	case PHY_TYPE_B:
		return dot11_phy_type_hrdsss;

	case PHY_TYPE_LP:
	case PHY_TYPE_G:
		return dot11_phy_type_erp;

	case PHY_TYPE_A:
		return dot11_phy_type_ofdm;

	case PHY_TYPE_SSN:
	case PHY_TYPE_N:
		return dot11_phy_type_ht;

	default:
		return dot11_phy_type_unknown;
	}
}
#if defined(EXT_STA)
const uint8 ofdm_rate_lookup[] = {
	    /* signal */
	96, /* 8: 48Mbps */
	48, /* 9: 24Mbps */
	24, /* A: 12Mbps */
	12, /* B:  6Mbps */
	108, /* C: 54Mbps */
	72, /* D: 36Mbps */
	36, /* E: 18Mbps */
	18  /* F:  9Mbps */
};
#endif /* BCMDONGLEHOST && EXT_STA */
void
wl_build_phy_attrs(DOT11_PHY_ATTRIBUTES *phy_attrs, wl_oid_t *oid, uint phy_id,
    bool radiostate,  bool shortpreamble, bool shortslot)
{
	wlc_rateset_t rs;
	uint phy_type, band;
	uint i;
#if defined(WL11N)
	uint mcs;
#endif /* WL11N */
	phy_attrs->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	phy_attrs->Header.Revision = DOT11_PHY_ATTRIBUTES_REVISION_1;
	phy_attrs->Header.Size = sizeof(DOT11_PHY_ATTRIBUTES);

	phy_type = oid->phy.phy_id2type[phy_id];
	band = oid->phy.nphy_band[phy_id];

	phy_attrs->PhyType = wl_phytype2ndistype(phy_type);
	phy_attrs->bHardwarePhyState = radiostate;
	phy_attrs->bSoftwarePhyState = mboolisset(oid->sw_phy_state, (1 << phy_id));

	phy_attrs->bCFPollable = FALSE;
	phy_attrs->uMPDUMaxLength = ETHER_MAX_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN;

	phy_attrs->TempType = dot11_temp_type_1;
	phy_attrs->DiversitySupport = dot11_diversity_support_dynamic;

	/* phy type-specific phy attributes */
	if ((phy_type == PHY_TYPE_G) || (phy_type == PHY_TYPE_LP)) {
		phy_attrs->ERPAttributes.bShortPreambleOptionImplemented =
			shortpreamble;
		phy_attrs->ERPAttributes.bPBCCOptionImplemented = FALSE;
		phy_attrs->ERPAttributes.bChannelAgilityPresent = FALSE;
		phy_attrs->ERPAttributes.uHRCCAModeSupported = 3;
		phy_attrs->ERPAttributes.bERPPBCCOptionImplemented = FALSE;
		phy_attrs->ERPAttributes.bDSSSOFDMOptionImplemented = FALSE;
		phy_attrs->ERPAttributes.bShortSlotTimeOptionImplemented = shortslot;
	} else if (phy_type == PHY_TYPE_B) {
		phy_attrs->HRDSSSAttributes.bShortPreambleOptionImplemented = shortpreamble;
		phy_attrs->HRDSSSAttributes.bPBCCOptionImplemented = FALSE;
		phy_attrs->HRDSSSAttributes.bChannelAgilityPresent = FALSE;
		phy_attrs->HRDSSSAttributes.uHRCCAModeSupported = 3;
	} else if (phy_type == PHY_TYPE_A) {
		phy_attrs->OFDMAttributes.uFrequencyBandsSupported = 7;
	} else if ((phy_type != PHY_TYPE_N) && (phy_type != PHY_TYPE_SSN)) {
		WL_ERROR(("%s: unknown phy_type %d\n", __FUNCTION__, phy_type));
	}

	phy_attrs->uNumberSupportedPowerLevels = 8;
	/* default */
	phy_attrs->TxPowerLevels[0] = 100;
	for (i = 1; i < 8; i++)
		phy_attrs->TxPowerLevels[i] = 10 * i;

	if (phy_type == PHY_TYPE_B) {
		wlc_rateset_default(&rs, NULL, PHY_TYPE_G, band, TRUE, RATE_MASK_FULL, FALSE,
			WLC_20_MHZ);
	} else {
		wlc_rateset_default(&rs, NULL, phy_type, band, FALSE, RATE_MASK_FULL,
		    ((phy_type == PHY_TYPE_N) || (phy_type == PHY_TYPE_SSN)), WLC_20_MHZ);
	}
	ASSERT(rs.count < DOT11_RATE_SET_MAX_LENGTH);

	phy_attrs->uNumDataRateMappingEntries = rs.count;
	for (i = 0; i < rs.count; i++) {
		phy_attrs->DataRateMappingEntries[i].ucDataRateIndex =
			rs.rates[i] & DOT11_DATA_RATE_INDEX_MASK;
		phy_attrs->DataRateMappingEntries[i].ucDataRateFlag = 0;
		phy_attrs->DataRateMappingEntries[i].usDataRateValue =
			rs.rates[i] & DOT11_DATA_RATE_INDEX_MASK;
	}

	bzero(&phy_attrs->SupportedDataRatesValue, sizeof(DOT11_SUPPORTED_DATA_RATES_VALUE_V2));
	for (i = 0; i < rs.count; i++) {
		phy_attrs->SupportedDataRatesValue.ucSupportedTxDataRatesValue[i] =
			rs.rates[i] & DOT11_DATA_RATE_INDEX_MASK;
		phy_attrs->SupportedDataRatesValue.ucSupportedRxDataRatesValue[i] =
			rs.rates[i] & DOT11_DATA_RATE_INDEX_MASK;
	}

#ifdef WL11N
	if ((phy_type == PHY_TYPE_N) || (phy_type == PHY_TYPE_SSN)) {
		ASSERT((rs.count + (2 * MCS_TABLE_SIZE)) < DOT11_RATE_SET_MAX_LENGTH);
		phy_attrs->uNumDataRateMappingEntries += (2 * MCS_TABLE_SIZE);
		for (mcs = 0, i = rs.count; mcs < MCS_TABLE_SIZE; mcs++, i += 2) {
			phy_attrs->DataRateMappingEntries[i].ucDataRateIndex = mcs_dri[mcs * 2];
			phy_attrs->DataRateMappingEntries[i].ucDataRateFlag = 0;
			phy_attrs->DataRateMappingEntries[i].usDataRateValue =
				(MCS_RATE(mcs, FALSE, FALSE) * 2);
			phy_attrs->SupportedDataRatesValue.ucSupportedTxDataRatesValue[i] =
				(MCS_RATE(mcs, FALSE, FALSE) * 2);
			phy_attrs->SupportedDataRatesValue.ucSupportedRxDataRatesValue[i] =
				(MCS_RATE(mcs, FALSE, FALSE) * 2);

			/* for 40 MHz */
			phy_attrs->DataRateMappingEntries[i + 1].ucDataRateIndex =
				mcs_dri[(mcs * 2) + 1];
			phy_attrs->DataRateMappingEntries[i + 1].ucDataRateFlag = 0;
			phy_attrs->DataRateMappingEntries[i + 1].usDataRateValue =
				(MCS_RATE(mcs, TRUE, FALSE) * 2);
			phy_attrs->SupportedDataRatesValue.ucSupportedTxDataRatesValue[i + 1] =
				(MCS_RATE(mcs, TRUE, FALSE) * 2);
			phy_attrs->SupportedDataRatesValue.ucSupportedRxDataRatesValue[i + 1] =
				(MCS_RATE(mcs, TRUE, FALSE) * 2);
		}
	}
#endif /* WL11N */
}

void
register_ext_sta_events(char *eventmask)
{
	/*
	 * First, we just simply register all events for EXT_STA.
	 */
	memset(eventmask, 0xff, WL_EVENTING_MASK_LEN);

	/*
	 * Let us mule some of the events that we don't need.
	 */
	clrbit(eventmask, WLC_E_RADIO);
	clrbit(eventmask, WLC_E_IF);
	clrbit(eventmask, WLC_E_TXFAIL);
	clrbit(eventmask, WLC_E_PROBREQ_MSG);
}
#endif /* EXT_STA */

#ifdef DHD_NDIS_OID
NDIS_STATUS
wl_band_state_update(wl_info_t *wl)
{
	int nbands;
	uint bandunit;
	wlcband_t *band;
	int i;
	uint j;
	int ch_default = 0;
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;

	wl_iovar_op(wl, "chanspec", &ch_default, sizeof(ch_default), IOV_GET);
	wl->oid->ch_default = \
		wl->wlc->default_bss.chanspec = (chanspec_t)ch_default;

	bandunit = wl_get_bandstate(wl);
	wl->wlc->band = &wl->wlc->bandstate[bandunit];

	nbands = NBANDS_PUB(wl->pub);

	ASSERT(nbands <= MAXBANDS);
	ASSERT(bandunit <= MAXBANDS);

	j = 0;
	for (i = 0; i < nbands; i++) {
		uint phy_channel = 0;

		band = &wl->wlc->bandstate[bandunit];
		if (band->pi)
			NdisFreeMemory(band->pi, sizeof(*band->pi), 0);
		NdisAllocateMemoryWithTag(&band->pi, sizeof(*band->pi), MEMORY_TAG);
		if (band->pi == NULL)
		{
			WL_ERROR(("band->pi == NULL\n"));
			return status;
		}
		wl_ioctl(wl, WLC_GET_BANDSTATES_PI, band->pi, sizeof(*band->pi));

		/* if the user bandlocked, but a super-user twiddled with "wl band" commands
		 * the supported list may be confusing.
		 */
		if (wl->oid->bandlock != WLC_BAND_AUTO)
			break;
		bandunit = ((bandunit == 1) ? 0 : 1);
	}
	wl->oid->phy.num_phy_ids = j;

	return status;
}
#endif /* DHD_NDIS_OID */
#if !defined(NDIS60)
#ifdef P2P
int p2psavestatus(int s)
{
	HKEY hKey;
	NTSTATUS stat;

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("COMM\\BCMSDDHD"),
		0, 0, &hKey)) {
		DWORD dwSize, dwValue, dwType = REG_DWORD;
		dwSize = sizeof(DWORD);

		dwValue = s;
		stat = RegSetValueEx(hKey, TEXT("LastError"),  NULL,  REG_DWORD,
			(LPBYTE)&dwValue, dwSize);

		if (ERROR_SUCCESS == stat)
			RegCloseKey(hKey);
	}

	return s;

}

int p2pgetlaststatus()
{
	HKEY hKey;
	DWORD dwValue = -1;

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("COMM\\BCMSDDHD"),
		0, 0, &hKey)) {
		DWORD dwSize, dwType = REG_DWORD;
		dwSize = sizeof(DWORD);

		if (ERROR_SUCCESS == RegQueryValueEx(hKey, TEXT("LastError"),
			NULL,  &dwType, (LPBYTE)&dwValue, &dwSize))
			RegCloseKey(hKey);
	}

	return dwValue;
}

int p2pisvirtualdevice(NDIS_HANDLE ConfigurationHandle)
{
	NDIS_STATUS status;
	NDIS_STATUS ret = NDIS_STATUS_SUCCESS;
	NDIS_HANDLE confighandle = NULL;
	PNDIS_CONFIGURATION_PARAMETER param;
	NDIS_STRING VirtualDevice = NDIS_STRING_CONST("VirtualDevice");

	/* open the registry */
	NdisOpenConfiguration(&status, &confighandle, ConfigurationHandle);
	if (NDIS_ERROR(status)) {
		return 0;
	}

	ASSERT(confighandle);

	/* read the ActivePath key set by the NDIS loader driver */
	NdisReadConfiguration(&status, &param, confighandle,  &VirtualDevice, NdisParameterInteger);

	NdisCloseConfiguration(confighandle);

	if (NDIS_ERROR(status)) {
		return 0;
	}

	return param->ParameterData.IntegerData;

}
#endif /* P2P */
#endif /* UNDER_CE && !NDIS60 */

#ifdef WLBTAMP_HIGH_ONLY
void
register_bta_events(char *eventmask)
{
	/*
	 * Let us add the events that we want.
	 */
printf("%s: Register BTA MAC Events\n", __FUNCTION__);
	setbit(eventmask, WLC_E_BTA_HCI_EVENT);
#ifndef EXT_STA
	setbit(eventmask, WLC_E_SET_SSID);
	setbit(eventmask, WLC_E_DISASSOC);
	setbit(eventmask, WLC_E_DEAUTH_IND);
	setbit(eventmask, WLC_E_DISASSOC_IND);
#endif /* !EXT_STA */
}
#endif	/* WLBTAMP_HIGH_ONLY */

static NDIS_STATUS
wl_minit(
#ifndef NDIS60
	PNDIS_STATUS OpenErrorStatus,
	PUINT SelectedMediumIndex,
	PNDIS_MEDIUM MediumArray,
	UINT MediumArraySize,
	NDIS_HANDLE MiniportAdapterHandle,
	NDIS_HANDLE ConfigurationHandle
#else
	IN NDIS_HANDLE MiniportAdapterHandle,
	IN NDIS_HANDLE MiniportDriverContext,
	IN PNDIS_MINIPORT_INIT_PARAMETERS MiniportInitParameters
#endif /* !NDIS60 */
)
{
	wl_info_t *wl;
	shared_info_t *sh;
	NDIS_STATUS status;
	int i, infra;
	uint8 bcmbustype;
	NDIS_HANDLE confighandle = NULL;
	void *context;
	uint16 ids[2];

#ifdef NDIS60
	ULONG bytesneeded, bytesused;
#endif /* NDIS60 */

#ifndef NDIS60
	NDIS_MEDIUM desired_medium;
#else /* !NDIS60 */
	NDIS_HANDLE ConfigurationHandle = NULL;
	NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES	RegAttrs;
	NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES	GenAttrs;
	NDIS_PNP_CAPABILITIES				PMCap;
	NDIS_SG_DMA_DESCRIPTION				DmaDescription;
	NET_BUFFER_LIST_POOL_PARAMETERS			NBLPoolParms;
	NDIS_CONFIGURATION_OBJECT			configobj;
	ULONG link_speed;
#endif /* !NDIS60 */
#ifdef EXT_STA
	NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES	NWFAttrs;
	int nbands;
	uint bandunit;
	wlcband_t *band;
	uint j;
	bool radiostate, shortpreamble, shortslot;
	DOT11_AUTH_CIPHER_PAIR ucast_auth_encr[] = {
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_NONE},
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_WEP},
		{DOT11_AUTH_ALGO_80211_SHARED_KEY, DOT11_CIPHER_ALGO_NONE},
		{DOT11_AUTH_ALGO_80211_SHARED_KEY, DOT11_CIPHER_ALGO_WEP},
#ifdef BCMWPA2
		{DOT11_AUTH_ALGO_RSNA, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_RSNA, DOT11_CIPHER_ALGO_CCMP},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_CCMP},
#endif /* BCMWPA2 */
		{DOT11_AUTH_ALGO_WPA, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_WPA_PSK, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_WPA, DOT11_CIPHER_ALGO_CCMP},
		{DOT11_AUTH_ALGO_WPA_PSK, DOT11_CIPHER_ALGO_CCMP}
	};
	DOT11_AUTH_CIPHER_PAIR mcast_auth_encr[] = {
#ifdef BCMWPA2
		{DOT11_AUTH_ALGO_RSNA, DOT11_CIPHER_ALGO_WEP40},
		{DOT11_AUTH_ALGO_RSNA, DOT11_CIPHER_ALGO_WEP104},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_WEP40},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_WEP104},
		{DOT11_AUTH_ALGO_RSNA, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_RSNA, DOT11_CIPHER_ALGO_CCMP},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_CCMP},
#endif /* BCMWPA2 */
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_NONE},
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_WEP},
		{DOT11_AUTH_ALGO_80211_SHARED_KEY, DOT11_CIPHER_ALGO_NONE},
		{DOT11_AUTH_ALGO_80211_SHARED_KEY, DOT11_CIPHER_ALGO_WEP},
		{DOT11_AUTH_ALGO_WPA, DOT11_CIPHER_ALGO_WEP40},
		{DOT11_AUTH_ALGO_WPA, DOT11_CIPHER_ALGO_WEP104},
		{DOT11_AUTH_ALGO_WPA_PSK, DOT11_CIPHER_ALGO_WEP40},
		{DOT11_AUTH_ALGO_WPA_PSK, DOT11_CIPHER_ALGO_WEP104},
		{DOT11_AUTH_ALGO_WPA, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_WPA_PSK, DOT11_CIPHER_ALGO_TKIP},
		{DOT11_AUTH_ALGO_WPA, DOT11_CIPHER_ALGO_CCMP},
		{DOT11_AUTH_ALGO_WPA_PSK, DOT11_CIPHER_ALGO_CCMP}
	};
	DOT11_AUTH_CIPHER_PAIR adhoc_ucast_auth_encr[] = {
#ifdef BCMWPA2
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_CCMP},
#endif /* BCMWPA2 */
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_NONE},
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_WEP}
	};
	DOT11_AUTH_CIPHER_PAIR adhoc_mcast_auth_encr[] = {
#ifdef BCMWPA2
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_WEP40},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_WEP104},
		{DOT11_AUTH_ALGO_RSNA_PSK, DOT11_CIPHER_ALGO_CCMP},
#endif /* BCMWPA2 */
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_NONE},
		{DOT11_AUTH_ALGO_80211_OPEN, DOT11_CIPHER_ALGO_WEP}
	};
#endif /* EXT_STA */

	WL_TRACE(("wl%d: wl_minit\n", wl_instance));

#ifndef NDIS60
	/* search for correct medium */
#ifdef EXT_STA
	desired_medium = NdisMediumNative802_11;
#else
	desired_medium = NdisMedium802_3;
#endif /* EXT_STA */
	for (; MediumArraySize > 0; MediumArraySize--) {
		if (MediumArray[MediumArraySize - 1] == desired_medium) {
			MediumArraySize--;
			break;
		}
	}

	if (MediumArray[MediumArraySize] != desired_medium) {
		WL_ERROR(("wl_minit: unsupported media\n"));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
				NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 1);
		status = NDIS_STATUS_UNSUPPORTED_MEDIA;
		return (P2PSAVESTATUS(status));
	}
	*SelectedMediumIndex = MediumArraySize;

#ifdef P2P
	{
		NDIS_STATUS ret = NDIS_STATUS_SUCCESS;
		NDIS_HANDLE confighandle = NULL;

		if (p2pisvirtualdevice(ConfigurationHandle)) {
			gP2P.wl_instance_rval = p2pgetlaststatus();
			if (!gP2P.wl_instance_rval) {
				NdisMSetAttributesEx(
					MiniportAdapterHandle, (NDIS_HANDLE) P2PCOOKIE, 1,
					(ULONG)gP2P.wl_instance_i,  gP2P.wl_instance_wl->BusType);
				gP2P.p2p_vip_AdapterHandle = MiniportAdapterHandle;
			} else {
				wl_instance = 0;
			}

			return gP2P.wl_instance_rval;
		}
	}
#endif /* P2P */

	wl = NULL;
	status = NdisAllocateMemoryWithTag(&wl, sizeof(wl_info_t), MEMORY_TAG);
#else /* !NDIS60 */
	wl = NdisAllocateMemoryWithTagPriority(MiniportAdapterHandle, sizeof(wl_info_t),
		MEMORY_TAG, HighPoolPriority);
	if (!wl)
		status = NDIS_STATUS_RESOURCES;
	else
		status = NDIS_STATUS_SUCCESS;
#endif /* !NDIS60 */

	if (status != NDIS_STATUS_SUCCESS) {
		WL_ERROR(("wl_minit: NdisAllocateMemoryWithTag error 0x%x\n", status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle,
			NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 2);
		return (P2PSAVESTATUS(status));
	}

	/* zero it out */
	bzero(wl, sizeof(wl_info_t));
	ASSERT(ISALIGNED((uintptr)wl, 4));

	shared_queue_init(&wl->txq);
	shared_queue_init(&wl->sh.txdone);
#ifdef EXT_STA
	shared_queue_init(&wl->tx_mid_q);
#endif /* EXT_STA */

#if (defined(NDIS60) && defined(EXT_STA)) || defined(WLBTAMP_HIGH_ONLY)
	/*
	 * We keep a local copy of oid and pub as cached data
	 * Make sure to refresh the run-time information.
	 */
	if (status == NDIS_STATUS_SUCCESS)
	{
#ifdef NDIS60
		wl->wlc = NdisAllocateMemoryWithTagPriority(MiniportAdapterHandle,
			sizeof(wlc_info_t), MEMORY_TAG, HighPoolPriority);
#else
		wl->wlc = NULL;
		NdisAllocateMemoryWithTag(&wl->wlc, sizeof(wlc_info_t), MEMORY_TAG);
#endif
		if (!wl->wlc) {
			status = NDIS_STATUS_RESOURCES;
			WL_ERROR(("wl_minit: wl->wlc NdisAllocateMemoryWithTag error 0x%x\n",
				status));
			NdisWriteErrorLogEntry(MiniportAdapterHandle,
				NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 28);
			goto error;
		}
		else
		{
			bzero(wl->wlc, sizeof(wlc_info_t));
			wl->pub = (wlc_pub_t *)wl->wlc;
			wl->wlc->wl = wl;
			status = NDIS_STATUS_SUCCESS;
		}
	}
	if (status == NDIS_STATUS_SUCCESS)
	{
#ifdef NDIS60
		wl->oid = NdisAllocateMemoryWithTagPriority(MiniportAdapterHandle,
			sizeof(wl_oid_t), MEMORY_TAG, HighPoolPriority);
#else
		wl->oid = NULL;
		NdisAllocateMemoryWithTag(&wl->oid, sizeof(wl_oid_t), MEMORY_TAG);
#endif
		if (!wl->oid) {
			status = NDIS_STATUS_RESOURCES;
			WL_ERROR(("wl_minit: wl->oid NdisAllocateMemoryWithTag error 0x%x\n",
				status));
			NdisWriteErrorLogEntry(MiniportAdapterHandle,
				NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 29);
			goto error;
		} else {
			bzero(wl->oid, sizeof(wl_oid_t));
			wl->oid->wlc = wl->wlc;
			wl->oid->pub = wl->pub;
			status = NDIS_STATUS_SUCCESS;
		}
	}
#endif /* defined(NDIS60) && BCMDONGLEHOST && EXT_STA */

#ifdef BCMDBG
	wl_info_dbg = wl;
#endif /* BCMDBG */

	sh = &wl->sh;

	sh->osh = osl_attach(sh, MiniportAdapterHandle);
	if (!sh->osh) {
		WL_ERROR(("wl_minit: osl_attach error\n"));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
				NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 27);
		status = NDIS_ERROR_CODE_OUT_OF_RESOURCES;
		goto error;
	}
#if (defined(NDIS60) && defined(EXT_STA)) || defined(WLBTAMP_HIGH_ONLY)
	/*
	 * Attach the osh handle in driver
	 */
	wl->wlc->pub.osh = sh->osh;
#endif

	sh->unit = wl_instance++;
	sh->slot = 0;
	strcpy(sh->id, "wl");

	/* save our adapter handle */
	sh->adapterhandle = MiniportAdapterHandle;
	sh->confighandle = ConfigurationHandle;
#ifdef NDIS60
	sh->adaptercontext = MiniportDriverContext;
#endif /* NDIS60 */


	/* set default buffer numbers */
	wl->ntxbuf = NTXBUF;
	wl->nrxbuf = NRXBUF;

	/* read configuration data from adapter config registry space */
	status = wl_readconfigdata(wl, ConfigurationHandle, MiniportAdapterHandle);

	if (NDIS_ERROR(status))
		goto error;

	WL_INFORM(("wl%d: Bus: ", sh->unit));
	if (wl->BusType == NdisInterfacePcMcia) {
		/* Disregard ForcePIO from the registry */
		bcmbustype = PCMCIA_BUS;
		wl->piomode = TRUE;
		WL_INFORM(("PCMCIA\n"));
	} else if (wl->BusType == NdisInterfacePci) {
		bcmbustype = PCI_BUS;
		WL_INFORM(("PCI/%s\n", wl->piomode ? "PIO" : "DMA"));
	}
	else if (wl->BusType == NdisInterfaceSDIO) {
		bcmbustype = SDIO_BUS;
		/* Disregard ForcePIO from the registry */
		wl->piomode = TRUE;
		WL_INFORM(("SDIO\n"));
	}

	sh->BusType = wl->BusType;

#ifndef NDIS60
	/* we are a DESERIALIZED miniport driver */
	i = NDIS_ATTRIBUTE_DESERIALIZE | ((wl->piomode) ? 0 : NDIS_ATTRIBUTE_BUS_MASTER);
	i |= NDIS_ATTRIBUTE_NO_HALT_ON_SUSPEND;

#else
#if defined(EXT_STA)
	i = NDIS_MINIPORT_ATTRIBUTES_NDIS_WDM;
#else
	i = NDIS_MINIPORT_ATTRIBUTES_HARDWARE_DEVICE;
	i |= ((wl->piomode) ? 0 : NDIS_MINIPORT_ATTRIBUTES_BUS_MASTER);
	i |= NDIS_MINIPORT_ATTRIBUTES_NO_HALT_ON_SUSPEND;
	/* If the device is Wake-on-Lan capable, then don't set this attribute
	 * Note that this is based on capabilities reflected in PCI interface based on SROM
	 * it does not guarantee the actual support, but there isn't sufficient information
	 * at this point to determine that
	 */
	if (bcmbustype == PCI_BUS && pcicore_pmecap_fast(sh->osh) && \
			wl->wowl_cap_override != 0)
		i &= ~NDIS_MINIPORT_ATTRIBUTES_NO_HALT_ON_SUSPEND;
#endif /* BCMDONGLEHOST && EXT_STA */
#endif /* !NDIS60 */

#if defined(NDIS51)
	i |= NDIS_ATTRIBUTE_SURPRISE_REMOVE_OK;
#elif defined(NDIS60)
	i |= NDIS_MINIPORT_ATTRIBUTES_SURPRISE_REMOVE_OK;
#endif /* defined (NDIS51) */

#ifndef NDIS60
#ifdef P2P
	gP2P.wl_instance_wl = wl;
	gP2P.wl_instance_i = i;
#endif
	NdisMSetAttributesEx(MiniportAdapterHandle, wl, 1, (ULONG)i, wl->BusType);
#else
	/* set registration attributes */
	bzero(&RegAttrs, sizeof(NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES));

	RegAttrs.Header.Type = NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES;
	RegAttrs.Header.Revision = NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES_REVISION_1;
	RegAttrs.Header.Size = sizeof(NDIS_MINIPORT_ADAPTER_REGISTRATION_ATTRIBUTES);

	RegAttrs.MiniportAdapterContext = (NDIS_HANDLE)wl;
	RegAttrs.AttributeFlags = i;
	RegAttrs.CheckForHangTimeInSeconds = 1;
	RegAttrs.InterfaceType = wl->BusType;

	sh->adaptercontext = RegAttrs.MiniportAdapterContext;
	status = NdisMSetMiniportAttributes(MiniportAdapterHandle, \
			(NDIS_MINIPORT_ADAPTER_ATTRIBUTES *)&RegAttrs);
	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: wl_minit: NdisMSetMiniportAtributes error 0x%x\n", \
					sh->unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
				NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 11);
		goto error;
	}

#endif /* !NDIS60 */

	/* read PCI/PCMCIA info, make sure it *is* our chip */
	if (wl->BusType == NdisInterfacePcMcia) {
		if (wl->sh.OS == NdisEnvironmentWindowsNt) {
			/* Get and save our PhysicalDeviceObject pointer */
			NdisMGetDeviceProperty(MiniportAdapterHandle, \
					(PDEVICE_OBJECT *)&sh->PDO, (PDEVICE_OBJECT *)&sh->FDO, \
					NULL, NULL, NULL);
		}


		ids[0] = 0;
		ids[1] = 0;
	} else if (wl->BusType == NdisInterfacePci) {
		/* read pci vendor and device id */
#ifndef NDIS60
			i = NdisReadPciSlotInformation(sh->adapterhandle, sh->slot, \
					0, ids, sizeof(ids));
#else
			i = NdisMGetBusData(sh->adapterhandle, PCI_WHICHSPACE_CONFIG, \
					0, ids, sizeof(ids));
#endif	/* !NDIS60 */
			if (i != sizeof(ids)) {
			status = NDIS_STATUS_FAILURE;
			NdisWriteErrorLogEntry(MiniportAdapterHandle, \
					NDIS_ERROR_CODE_HARDWARE_FAILURE, 1, 8);
			goto error;
		}
	}
	else if (wl->BusType == NdisInterfaceSDIO) {


		/* WM7 TODO: TBD */
#if !defined(NDIS60)
		wl->intrvector = wl->intrlevel = 0; /* SDIO registers its own interrupt */
#endif /* !NDIS60 */
		ids[0] = 0;
		ids[1] = 0;

	}
	else {
		ASSERT(0);
	}

	if ((wl->BusType == NdisInterfacePci) || (wl->BusType == NdisInterfacePcMcia)) {
		/* find and map chip registers and interrupt handlers */
#ifndef NDIS60
			status = wl_find_resources(wl);
#else
			status = wl_find_resources(wl, MiniportInitParameters->AllocatedResources);
#endif	 /* !NDIS60 */
			if (NDIS_ERROR(status)) {
			NdisWriteErrorLogEntry(MiniportAdapterHandle, \
					NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 10);
			goto error;
		}
	}

	/* initialize our private spinlocks */
	NdisAllocateSpinLock(&wl->lock);
	NdisAllocateSpinLock(&wl->dpclock);
#ifdef EXT_STA
	NdisAllocateSpinLock(&wl->tx_lock);
#endif /* EXT_STA */
	if (!&wl->tx_lock)
		NdisAllocateSpinLock(&wl->tx_lock);
	NdisAllocateSpinLock(&wl->dhdlock);
	NdisAllocateSpinLock(&wl->dhd_tx_queue_lock);
	NdisAllocateSpinLock(&wl->dhd_rx_queue_lock);
	NdisAllocateSpinLock(&wl->dhd_evq_lock);
	NdisAllocateSpinLock(&wl->rx_lock);

	sh->dpclock = &wl->dpclock;

	wl->gotspinlocks = TRUE;

	if (!wl->piomode) {
		/* allocate dma resources */
#ifndef NDIS60
		status = NdisMAllocateMapRegisters(MiniportAdapterHandle, 0, TRUE, 8, PAGE_SIZE);
		if (NDIS_ERROR(status)) {
			WL_ERROR(("wl%d: wl_minit: NdisMAllocateMapRegisters error 0x%x\n", \
						sh->unit, status));
			NdisWriteErrorLogEntry(MiniportAdapterHandle, \
					NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 11);
			goto error;
		}
		wl->gotmapregs = TRUE;
#else /* !NDIS60 */
#if !defined(BCMDHDUSB)
		bzero(&DmaDescription, sizeof(DmaDescription));

		DmaDescription.Header.Type = NDIS_OBJECT_TYPE_SG_DMA_DESCRIPTION;
		DmaDescription.Header.Revision = NDIS_SG_DMA_DESCRIPTION_REVISION_1;
		DmaDescription.Header.Size = sizeof(NDIS_SG_DMA_DESCRIPTION);

		DmaDescription.Flags = 0;
		DmaDescription.MaximumPhysicalMapping = PAGE_SIZE;
		DmaDescription.ProcessSGListHandler = wl_mposttxds;
		DmaDescription.SharedMemAllocateCompleteHandler = NULL;
		status = NdisMRegisterScatterGatherDma(wl->sh.adapterhandle, \
				&DmaDescription, &wl->sh.dmahandle);
		if (NDIS_ERROR(status)) {
			WL_ERROR(("wl%d: wl_minit: NdisMRegisterScatterGatherDMA error 0x%x\n", \
						sh->unit, status));
			NdisWriteErrorLogEntry(MiniportAdapterHandle, \
					NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 11);
			goto error;
		}

#endif /* !BCMDHDUSB */
#endif /* !NDIS60 */
	}

	/* determine system's cacheline size for alignment purposes */
#if (WINVER == 0x0501) || (defined(NDIS60))
#if !defined(BCMDHDUSB)
	sh->cacheline = NdisMGetDmaAlignment(MiniportAdapterHandle);
#endif /* !BCMDHDUSB */
#else
	/* This is what ndis.h and ntddk.h do for X86. Unfortunately
	 * somebody forgot to put into wdm.h (for the NDIS_WDM case),
	 * so I'm just hardwiring it here. This will need to change
	 * if we ever want to run on other cpu (Itanium?).
	 */
	sh->cacheline = 1;
#endif /* (WINVER == 0x0501) || (defined (NDIS60)) */
	WL_INFORM(("wl%d: cacheline = 0x%x\n", sh->unit, sh->cacheline));

#ifndef NDIS60
	/* allocate rx ndis packet descriptor pool */
	NdisAllocatePacketPool(&status, &sh->rxpacketpool, wl->nrxbuf, 16);
	if (NDIS_ERROR(status))
		goto error;

	/* allocate rx ndis buffer descriptor pool plus a few tmp flush bufs */
	NdisAllocateBufferPool(&status, &sh->rxbufferpool, wl->nrxbuf + 4);
	if (NDIS_ERROR(status))
		goto error;
#else /* !NDIS60 */
	bzero(&NBLPoolParms, sizeof(NET_BUFFER_LIST_POOL_PARAMETERS));

	NBLPoolParms.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	NBLPoolParms.Header.Revision = NET_BUFFER_LIST_POOL_PARAMETERS_REVISION_1;
	NBLPoolParms.Header.Size = sizeof(NET_BUFFER_LIST_POOL_PARAMETERS);

	NBLPoolParms.fAllocateNetBuffer = TRUE;
	NBLPoolParms.PoolTag = MEMORY_TAG;

	sh->rxpacketpool = NdisAllocateNetBufferListPool(MiniportAdapterHandle, &NBLPoolParms);
	if (sh->rxpacketpool == NULL)
		goto error;
#endif /* !NDIS60 */

	/*
	 * For the NIC mode drivers there is explict call order dependency,
	 * where "wl_nicattach" should be called BEFORE calling
	 * "shared_lb_alloc". "wl_nicattach" gets DMA related info which
	 * is going to be used in "shared_lb_alloc" call.
	 */
	/* allocate and fill transmit packet freelist */
#if defined(BCMDHDUSB)
	status = shared_lb_alloc(sh, &sh->txfree, wl->ntxbuf, \
			FALSE, TRUE, FALSE, TRUE);
#else
	status = shared_lb_alloc(sh, &sh->txfree, wl->ntxbuf, \
			(wl->piomode? FALSE: TRUE), TRUE, (wl->piomode? TRUE: FALSE), TRUE);
#endif
	if (NDIS_ERROR(status)) {
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
				NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 12);
		goto error;
	}

#ifdef EXT_STA
	if (!(wl->process_mid_txq_timer = \
			wl_init_timer(wl, wl_process_mid_txq, wl, "process_txq"))) {
		WL_ERROR(("wl%d: %s: wl_init_timer for process_mid_txq failed\n", \
					sh->unit, __FUNCTION__));
		goto error;
	}
	WL_ERROR(("wl%d: %s: Initted timer for process_mid_txq worked\n", sh->unit, __FUNCTION__));
#endif /* EXT_STA */

	/* Sets the value of headroom reserved when a NDIS packet is converted to
	 * lbuf in PKTFRMNATIVE call.
	 */
	PKTSETHEADROOM(sh->txfree, WL_HEADROOM_RSVD);

	/* allocate and fill receive packet freelist */
#if defined(BCMDHDUSB)
	status = shared_lb_alloc(sh, &sh->rxfree, wl->nrxbuf, \
			FALSE, TRUE, FALSE, TRUE);
#else
	status = shared_lb_alloc(sh, &sh->rxfree, wl->nrxbuf, \
			(wl->piomode? FALSE : TRUE), TRUE, (wl->piomode? TRUE : FALSE), TRUE);
#endif
	if (NDIS_ERROR(status)) {
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
				NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 13);
		goto error;
	}

#if defined(__BBSOFT__) && defined(__ZELOS__)
	if (g_pGPIOReg) {
		Set_PinFunction(g_pGPIOReg,  WLAN_WAKEUP);
		Set_PinData(g_pGPIOReg,  WLAN_WAKEUP, TRUE);
	}
#endif

	/*
	 * For the Dongle mode drivers there is explict call order dependency,
	 * which is exactly opposite of NIC mode drivers. "dhd_probe" should
	 * be called AFTER calling "shared_lb_alloc". "dhd_probe" makes call
	 * to the PKTGET which needs "shared_lb_alloc" to be called and pkt
	 *  pool is already created
	 */
#ifdef NDIS60
	if (dhd_probe(sh, (void *)wl, (void *)&wl->bar0, (void **)&wl->regsva,
	              0, (void **)&wl->dhd) != 0) {
#else
	if (dhd_probe(sh, (void *)wl, (void *)&wl->bar0, (void **)&wl->regsva,
	              wl->intrvector, (void **)&wl->dhd) != 0) {
#endif
		status = NDIS_STATUS_FAILURE;
		goto error;
	}
	else {
		context = sh->sdh;
	}

#ifdef SIMPLE_ISCAN
	dhd_iscan_attach(wl->dhd);
#endif /* SIMPLE_ISCAN */

	/* register device removal callback */
	if (wl->BusType == NdisInterfaceSDIO)
		bcmsdh_devremove_reg(context, wl_dev_gone, wl);

#ifdef	TEST_N_EXIT
	printf("Back from wlc_attach, exiting anyway\n");
	status = NDIS_STATUS_FAILURE;
	goto error;
#endif /* TEST_N_EXIT */


#ifdef NDIS60
	/* set general attributes */
	bzero(&GenAttrs, sizeof(NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES));

	GenAttrs.Header.Type = NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES;
	GenAttrs.Header.Revision = NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES_REVISION_1;
	GenAttrs.Header.Size = sizeof(NDIS_MINIPORT_ADAPTER_GENERAL_ATTRIBUTES);

	GenAttrs.MediaType = NdisMediumNative802_11;
	GenAttrs.PhysicalMediumType = NdisPhysicalMediumNative802_11;
	GenAttrs.MtuSize = ETHER_MAX_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN;
	if (wl_ioctl(wl, WLC_GET_RATE, &link_speed, sizeof(link_speed)))
		link_speed = 108;
	if (wl->abrateconvert)
		wl_abrate_convert(wl, &link_speed);
	link_speed *= 500000;
	GenAttrs.MaxXmitLinkSpeed = link_speed;
	GenAttrs.XmitLinkSpeed = NDIS_LINK_SPEED_UNKNOWN;
	GenAttrs.MaxRcvLinkSpeed = link_speed;
	GenAttrs.RcvLinkSpeed = NDIS_LINK_SPEED_UNKNOWN;
#ifndef EXT_STA
	GenAttrs.MediaConnectState = MediaConnectStateUnknown;
#else
	GenAttrs.MediaConnectState = MediaConnectStateConnected;
#endif /* EXT_STA */
	GenAttrs.MediaDuplexState = MediaDuplexStateUnknown;
	GenAttrs.LookaheadSize = LBUFSZ;

	if (wl->wowl_cap && wl->wowl_cap_override != 0) {
		PMCap.WakeUpCapabilities.MinMagicPacketWakeUp = \
			(wl->wowl_cap_override & WL_WOWL_MAGIC)? \
			NdisDeviceStateD3: NdisDeviceStateUnspecified;
		PMCap.WakeUpCapabilities.MinPatternWakeUp = \
			(wl->wowl_cap_override & WL_WOWL_NET)? \
			NdisDeviceStateD3: NdisDeviceStateUnspecified;
		PMCap.WakeUpCapabilities.MinLinkChangeWakeUp = NdisDeviceStateUnspecified;
	} else {
		PMCap.WakeUpCapabilities.MinMagicPacketWakeUp = NdisDeviceStateUnspecified;
		PMCap.WakeUpCapabilities.MinPatternWakeUp = NdisDeviceStateUnspecified;
		PMCap.WakeUpCapabilities.MinLinkChangeWakeUp = NdisDeviceStateUnspecified;
	}
	GenAttrs.PowerManagementCapabilities = &PMCap;

	GenAttrs.MacOptions = NDIS_MAC_OPTION_TRANSFERS_NOT_PEND
			| NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA
			| NDIS_MAC_OPTION_RECEIVE_SERIALIZED
			| NDIS_MAC_OPTION_NO_LOOPBACK;
#define ADVERTISE_8021P_PRIORITY
#ifdef ADVERTISE_8021P_PRIORITY
	if (wl->priority_vlan_tag & 0x01) {
		GenAttrs.MacOptions |= NDIS_MAC_OPTION_8021P_PRIORITY;
	}
#endif /* ADVERTISE_8021P_PRIORITY */
#define ADVERTISE_8021Q_VLAN
#ifdef ADVERTISE_8021Q_VLAN
	if (wl->priority_vlan_tag & 0x02) {
		GenAttrs.MacOptions |= NDIS_MAC_OPTION_8021Q_VLAN;
	}
#endif /* ADVERTISE_8021Q_VLAN */

	GenAttrs.SupportedPacketFilters = NDIS_PACKET_TYPE_DIRECTED | \
		NDIS_PACKET_TYPE_MULTICAST | \
		NDIS_PACKET_TYPE_ALL_MULTICAST | \
		NDIS_PACKET_TYPE_BROADCAST | \
		NDIS_PACKET_TYPE_PROMISCUOUS | \
		NDIS_PACKET_TYPE_ALL_LOCAL;

	wl_iovar_op(wl, "perm_etheraddr", &wl->wlc->perm_etheraddr, ETHER_ADDR_LEN, IOV_GET);
	wl_iovar_op(wl, "cur_etheraddr", &wl->pub->cur_etheraddr, ETHER_ADDR_LEN, IOV_GET);
	GenAttrs.MaxMulticastListSize = MAXMULTILIST;
	GenAttrs.MacAddressLength = ETHER_ADDR_LEN;
	bcopy(&wl->wlc->perm_etheraddr, GenAttrs.PermanentMacAddress, ETHER_ADDR_LEN);
	bcopy(&wl->pub->cur_etheraddr, GenAttrs.CurrentMacAddress, ETHER_ADDR_LEN);

	GenAttrs.RecvScaleCapabilities = NULL;
	GenAttrs.AccessType = NET_IF_ACCESS_BROADCAST;
	GenAttrs.DirectionType = NET_IF_DIRECTION_SENDRECEIVE;
	GenAttrs.ConnectionType = NET_IF_CONNECTION_DEDICATED;
	GenAttrs.IfType = IF_TYPE_IEEE80211;
	GenAttrs.IfConnectorPresent = TRUE;

	GenAttrs.SupportedStatistics = NDIS_STATISTICS_XMIT_OK_SUPPORTED | \
		NDIS_STATISTICS_RCV_OK_SUPPORTED | \
		NDIS_STATISTICS_XMIT_ERROR_SUPPORTED | \
		NDIS_STATISTICS_RCV_ERROR_SUPPORTED | \
		NDIS_STATISTICS_RCV_NO_BUFFER_SUPPORTED;

	GenAttrs.SupportedOidList = wl_supported_oids;
	GenAttrs.SupportedOidListLength = sizeof(wl_supported_oids);

	status = NdisMSetMiniportAttributes(MiniportAdapterHandle, \
	    (NDIS_MINIPORT_ADAPTER_ATTRIBUTES *)&GenAttrs);
	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: wl_minit: NdisMSetMiniportAtributes error 0x%x\n", \
		    sh->unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
		    NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 11);
		goto error;
	}
#endif /* NDIS60 */

#ifdef DHD_NDIS_OID
	status = dhd_oid_attach(wl);
	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: attach ndis oid failed 0x%x\n",
		    sh->unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle,
			NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 2);
		goto error;
	}
#endif /* DHD_NDIS_OID */

#if defined(WL_IW_USE_ISCAN)
	WL_ERROR(("wl%d: %s: calling wl_iscan_attach\n", sh->unit, __FUNCTION__));
	if (wl_iscan_attach(wl))
		return NDIS_STATUS_FAILURE;
#endif /* WL_IW_USE_ISCAN */

	/* default infrastructure mode is Infrastructure */
	infra = 1;
	wl_ioctl(wl, WLC_SET_INFRA, &infra, sizeof(infra));

#ifdef EXT_STA
	/* default association recreation feture enable */
	wl_iovar_setint(wl, "assoc_recreate", 1);
	wl->pwk = 0;
	wl->txq_count = 0;
	wl->tx_mid_q_count = 0;
	wl->pub->_assoc_recreate = 1; /* Host synchronization */
#endif /* EXT_STA */

	/* avoid premature wlc_up() in wl_readconfigoverrides() */
	wl_iovar_setint(wl, "down_override", TRUE);


	/* open the registry */
#ifndef NDIS60
	NdisOpenConfiguration(&status, &confighandle, ConfigurationHandle);
#else
	bzero(&configobj, sizeof(NDIS_CONFIGURATION_OBJECT));

	configobj.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
	configobj.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
	configobj.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT);

	configobj.NdisHandle = MiniportAdapterHandle;
	configobj.Flags = 0;

	status = NdisOpenConfigurationEx(&configobj, &confighandle);
#endif /* !NDIS60 */
	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: wl_minit: NdisOpenConfiguration error 0x%x\n", \
					wl->sh.unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
		    NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER, 1, 24);
		goto error;
	}
	ASSERT(confighandle);
	wl->context = REG_ACCESS_INIT;

#if defined(EXT_STA) || defined(DHD_NDIS_OID)
	/* 
	 * Initialize to -1 before wl_readconfigoverrides() so
	 * if registry NICPowerState is present, it can override sw_phy_state;
	 * If NICPowerState is not present, sw_phy_state remains set to -1.
	 */
	wl->oid->sw_phy_state = (mbool)-1;
#endif /* (BCMDONGLEHOST && EXT_STA) || DHD_NDIS_OID */
	/* read reg entries that may override defaults set in wlc_attach() */
	wl_readconfigoverrides((void *) wl, confighandle, sh->id, sh->unit, sh->OS);
	wl_scanoverrides(wl, confighandle, sh->id, sh->unit);

#if defined(DHD_NDIS_OID)
	{
		int iovar_int;

		/* Save defaults before any changes to ensure Ndis802_11Automode works */
		wl_iovar_getint(wl, "nmode", &iovar_int);
		wl->oid->nmode_default = iovar_int;
		wl_ioctl(wl, WLC_GET_GMODE, &iovar_int, sizeof(iovar_int));
		wl->oid->gmode_default = iovar_int;
	}
#endif /* !BCMDONGLEHOST || DHD_NDIS_OID */
	/* close the registry */
	NdisCloseConfiguration(confighandle);

	/* permits up onwards */
	wl_iovar_setint(wl, "down_override", FALSE);
#ifdef DHD_NDIS_OID
	wl_band_state_update(wl);
#endif /* DHD_NDIS_OID */

#ifdef NDIS60
#ifdef EXT_STA
	{
		int ch_default = 0;

		wl_queryoid(wl, (NDIS_OID)OID_STA_CHANSPEC, \
			(PVOID) &ch_default, (ULONG)sizeof(ch_default), \
			&bytesused, &bytesneeded, P2PUNITNUMBER());
		wl_queryoid(wl, OID_STA_NBANDS, &wl->pub->_nbands, sizeof(&wl->pub->_nbands),
		            &bytesused, &bytesneeded, P2PUNITNUMBER());

		wl->oid->ch_default = \
			wl->wlc->default_bss.chanspec = (chanspec_t)ch_default;

		bandunit = wl_get_bandstate(wl);
		wl->wlc->band = &wl->wlc->bandstate[bandunit];
	}

	nbands = NBANDS_PUB(wl->pub);
	WL_INFORM(("%s: Number of bands=%d\n", __FUNCTION__, nbands));
	ASSERT(nbands <= MAXBANDS);
	ASSERT(bandunit <= MAXBANDS);

	j = 0;
	for (i = 0; i < nbands; i++) {
		uint phy_channel = 0;
		uint phy_type = PHY_TYPE_NULL;

		band = &wl->wlc->bandstate[bandunit];
		if (band->pi)
			NdisFreeMemory(band->pi, sizeof(*band->pi), 0);
#ifdef NDIS60
		band->pi = NdisAllocateMemoryWithTagPriority(MiniportAdapterHandle, \
				sizeof(*band->pi), MEMORY_TAG, HighPoolPriority);
#else
		band->pi = NULL;
		NdisAllocateMemoryWithTag(&band->pi, sizeof(*band->pi), MEMORY_TAG);
#endif
		if (band->pi == NULL)
		{
			WL_ERROR(("band->pi == NULL\n"));
			return (P2PSAVESTATUS(NDIS_STATUS_RESOURCES));
		}
		wl_ioctl(wl, WLC_GET_BANDSTATES_PI, band->pi, sizeof(*band->pi));
		phy_type = band->pi->phy_type;
		if (((phy_type == PHY_TYPE_A) && CHSPEC_IS5G(wl->oid->ch_default)) ||
		    ((phy_type == PHY_TYPE_G) && CHSPEC_IS2G(wl->oid->ch_default)) ||
		    (((phy_type == PHY_TYPE_LP) || (phy_type == PHY_TYPE_N) ||
		    (phy_type == PHY_TYPE_SSN)) &&
		     ((CHSPEC_IS5G(wl->oid->ch_default) && (band->bandtype == WLC_BAND_5G)) ||
		      (CHSPEC_IS2G(wl->oid->ch_default) && (band->bandtype == WLC_BAND_2G)))) ||
		      (phy_type == PHY_TYPE_LP))
		        phy_channel = CHSPEC_CHANNEL(wl->oid->ch_default);
		WL_INFORM(("%s: i %d bandtype %d phy_type %d\n",
			__FUNCTION__, i, band->bandtype, phy_type));
		if (phy_type != PHY_TYPE_LP) {
			wl->oid->phy.phy_channel[j] = phy_channel;
			wl->oid->phy.phy_id2type[j++] = phy_type;
		}

		/* report both 11g and 11b type networks for a G/LP Phy */
		if (phy_type == PHY_TYPE_G) {
			ASSERT((j == 1) || (j == 2));
			wl->oid->phy.phy_channel[j] = phy_channel;
			wl->oid->phy.phy_id2type[j++] = PHY_TYPE_B;
		}

		if ((phy_type == PHY_TYPE_N) ||	(phy_type == PHY_TYPE_LP) ||
			(phy_type == PHY_TYPE_SSN)) {
			if (phy_type == PHY_TYPE_N)
				wl->oid->phy.nphy_band[j - 1] = band->bandtype;
			wl->oid->phy.nphy_band[j] = band->bandtype;

			/* report 11n, 11g, and 11b type networks for a 2.4GHz N Phy */
#if defined(NDIS60) && defined(EXT_STA) && defined(BCM4325_IMAGE)
			if (band->bandtype == WLC_BAND_2G || (phy_type == PHY_TYPE_LP)) {
#else
			if (band->bandtype == WLC_BAND_2G || (phy_type == PHY_TYPE_LP)) {
#endif
				wl->oid->phy.phy_channel[j] = phy_channel;
				wl->oid->phy.phy_id2type[j++] = PHY_TYPE_G;

				wl->oid->phy.phy_channel[j] = phy_channel;
				wl->oid->phy.phy_id2type[j++] = PHY_TYPE_B;
			}

			/* report both 11n and 11a type networks for a 5GHz N Phy */
			else if (band->bandtype == WLC_BAND_5G) {
				wl->oid->phy.phy_channel[j] = phy_channel;
				wl->oid->phy.phy_id2type[j++] = PHY_TYPE_A;
			}
		}

		/* if the user bandlocked, but a super-user twiddled with "wl band" commands
		 * the supported list may be confusing.
		 */
		if (wl->oid->bandlock != WLC_BAND_AUTO)
			break;
		bandunit = ((bandunit == 1) ? 0 : 1);
	}
	wl->oid->phy.num_phy_ids = j;
#if defined(EXT_STA)
	wl_setoid(wl, (NDIS_OID)OID_STA_SET_PHY, \
			(PVOID) &wl->oid->phy, (ULONG) sizeof(wl->oid->phy), \
			&bytesused, &bytesneeded, 0);
#endif /* BCMDONGLEHOST && EXT_STA */

	if (wl->oid->sw_phy_state == (mbool)-1)
		wl->oid->sw_phy_state &= ((1 << j) - 1);

#if defined(EXT_STA)
	/* Update sw_phy_state in the dongle */
	wl_setoid(wl, (NDIS_OID)OID_STA_SET_PHY_STATE, \
		(PVOID)&wl->oid->sw_phy_state, (ULONG)sizeof(wl->oid->sw_phy_state), \
		&bytesused, &bytesneeded, 0);
#endif /* BCMDONGLEHOST && EXT_STA */

	bzero(&NWFAttrs, sizeof(NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES));

	NWFAttrs.Header.Type = NDIS_OBJECT_TYPE_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES;
	NWFAttrs.Header.Revision = NDIS_MINIPORT_ADAPTER_802_11_ATTRIBUTES_REVISION_1;
	NWFAttrs.Header.Size = sizeof(NDIS_MINIPORT_ADAPTER_NATIVE_802_11_ATTRIBUTES);

	NWFAttrs.OpModeCapability = DOT11_OPERATION_MODE_EXTENSIBLE_STATION |
	                            DOT11_OPERATION_MODE_NETWORK_MONITOR;
	NWFAttrs.NumOfTXBuffers = wl->ntxbuf;
	NWFAttrs.NumOfRXBuffers = wl->nrxbuf;
	NWFAttrs.MultiDomainCapabilityImplemented = FALSE;

	NWFAttrs.NumSupportedPhys = wl->oid->phy.num_phy_ids;
	NWFAttrs.SupportedPhyAttributes = MALLOC(sh->osh, \
	    wl->oid->phy.num_phy_ids * sizeof(DOT11_PHY_ATTRIBUTES));
	if (NWFAttrs.SupportedPhyAttributes == NULL) {
		WL_ERROR(("wl%d: wl_minit: Supported PHY Attributes alloc failed\n", sh->unit));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
		    NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 11);
		goto error;
	}

	wl_ioctl(wl, WLC_GET_RADIO, &i, sizeof(i));
	radiostate = !mboolisset((mbool)i, WL_RADIO_HW_DISABLE);
	wl->last_phyind = (mbool)(i & WL_RADIO_HW_DISABLE);
	wl_ioctl(wl, WLC_GET_PLCPHDR, &i, sizeof(i));
	shortpreamble = (i != WLC_PLCP_LONG);
	wl_ioctl(wl, WLC_GET_SHORTSLOT_OVERRIDE, &i, sizeof(i));
	shortslot = (i != WLC_SHORTSLOT_OFF);
	for (i = 0; i < (int)wl->oid->phy.num_phy_ids; i++) {
		wl_build_phy_attrs(&NWFAttrs.SupportedPhyAttributes[i], wl->oid, i, \
				radiostate, shortpreamble, shortslot);
	}

	/* Extensible Station attributes */
	NWFAttrs.ExtSTAAttributes = MALLOC(sh->osh, sizeof(DOT11_EXTSTA_ATTRIBUTES));
	if (NWFAttrs.ExtSTAAttributes == NULL) {
		WL_ERROR(("wl%d: wl_minit: Extensible STA Attributes alloc failed\n", sh->unit));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
		    NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 11);
		MFREE(sh->osh, NWFAttrs.SupportedPhyAttributes, \
		    NWFAttrs.NumSupportedPhys * sizeof(DOT11_PHY_ATTRIBUTES));
		goto error;
	}

	NWFAttrs.ExtSTAAttributes->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	NWFAttrs.ExtSTAAttributes->Header.Revision = DOT11_EXTSTA_ATTRIBUTES_REVISION_1;
	NWFAttrs.ExtSTAAttributes->Header.Size = sizeof(DOT11_EXTSTA_ATTRIBUTES);

	NWFAttrs.ExtSTAAttributes->uScanSSIDListSize = MAXSCAN_SSIDS;
	NWFAttrs.ExtSTAAttributes->uDesiredBSSIDListSize = 1;
	NWFAttrs.ExtSTAAttributes->uDesiredSSIDListSize = 1;
	NWFAttrs.ExtSTAAttributes->uExcludedMacAddressListSize = MAX_EXCLUDED_MAC_ADDRS;
	NWFAttrs.ExtSTAAttributes->uPrivacyExemptionListSize = 1;
	NWFAttrs.ExtSTAAttributes->uKeyMappingTableSize = WSEC_MAX_KEYS - WLC_DEFAULT_KEYS;
	NWFAttrs.ExtSTAAttributes->uDefaultKeyTableSize = WLC_DEFAULT_KEYS;
	NWFAttrs.ExtSTAAttributes->uWEPKeyValueMaxLength = WEP128_KEY_SIZE;
#ifdef BCMWPA2
	NWFAttrs.ExtSTAAttributes->uPMKIDCacheSize = MAXPMKID;
#else
	NWFAttrs.ExtSTAAttributes->uPMKIDCacheSize = 0;
#endif /* BCMWPA2 */
	NWFAttrs.ExtSTAAttributes->uMaxNumPerSTADefaultKeyTables = WSEC_MAX_KEYS / 2;

	NWFAttrs.ExtSTAAttributes->bStrictlyOrderedServiceClassImplemented = FALSE;

	/* Turn on WMM support */
	if (WME_ENAB(wl->pub)) {
		NWFAttrs.ExtSTAAttributes->ucSupportedQoSProtocolFlags =
		        DOT11_QOS_PROTOCOL_FLAG_WMM;
	} else {
		NWFAttrs.ExtSTAAttributes->ucSupportedQoSProtocolFlags = 0;
	}

	NWFAttrs.ExtSTAAttributes->bSafeModeImplemented = FALSE;

	NWFAttrs.ExtSTAAttributes->uNumSupportedCountryOrRegionStrings = 0;
	NWFAttrs.ExtSTAAttributes->pSupportedCountryOrRegionStrings = NULL;

	NWFAttrs.ExtSTAAttributes->uInfraNumSupportedUcastAlgoPairs = ARRAYSIZE(ucast_auth_encr);
	NWFAttrs.ExtSTAAttributes->pInfraSupportedUcastAlgoPairs = ucast_auth_encr;
	NWFAttrs.ExtSTAAttributes->uInfraNumSupportedMcastAlgoPairs = ARRAYSIZE(mcast_auth_encr);
	NWFAttrs.ExtSTAAttributes->pInfraSupportedMcastAlgoPairs = mcast_auth_encr;

	NWFAttrs.ExtSTAAttributes->uAdhocNumSupportedUcastAlgoPairs = \
	    ARRAYSIZE(adhoc_ucast_auth_encr);
	NWFAttrs.ExtSTAAttributes->pAdhocSupportedUcastAlgoPairs = adhoc_ucast_auth_encr;
	NWFAttrs.ExtSTAAttributes->uAdhocNumSupportedMcastAlgoPairs = \
	    ARRAYSIZE(adhoc_mcast_auth_encr);
	NWFAttrs.ExtSTAAttributes->pAdhocSupportedMcastAlgoPairs = adhoc_mcast_auth_encr;

	status = NdisMSetMiniportAttributes(MiniportAdapterHandle, \
	    (NDIS_MINIPORT_ADAPTER_ATTRIBUTES *)&NWFAttrs);

	if (NWFAttrs.ExtSTAAttributes->pSupportedCountryOrRegionStrings != NULL) {
		MFREE(sh->osh, NWFAttrs.ExtSTAAttributes->pSupportedCountryOrRegionStrings, \
		    NWFAttrs.ExtSTAAttributes->uNumSupportedCountryOrRegionStrings * \
		    sizeof(DOT11_COUNTRY_OR_REGION_STRING));
	}
	MFREE(sh->osh, NWFAttrs.ExtSTAAttributes, sizeof(DOT11_EXTSTA_ATTRIBUTES));
	MFREE(sh->osh, NWFAttrs.SupportedPhyAttributes, \
		NWFAttrs.NumSupportedPhys * sizeof(DOT11_PHY_ATTRIBUTES));

	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: wl_minit: NdisMSetMiniportAtributes error 0x%x\n", \
			sh->unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
			NDIS_ERROR_CODE_DRIVER_FAILURE, 1, 11);
		goto error;
	}
#endif /* EXT_STA */
#else /* !NDIS60
	/* register a shutdown handler */
	if (MinorNdisVersion == 0) {
		NdisMRegisterAdapterShutdownHandler(sh->adapterhandle, wl, wl_mshutdown);
		wl->shutdownhandler = TRUE;
	}
#endif /* !NDIS60 */

	/* read the autoconnect configuration params */
#ifndef NDIS60
	wl_read_auto_config(wl, ConfigurationHandle, MiniportAdapterHandle);
#else
	wl_read_auto_config(wl);
#endif /* ! NDIS60 */
	ND_LOCK(wl);

	/*
	   Initialize chip. This could be dummy call in case of dongle,
	   if "AllowIntfUp" registry entry is set to '1'
	*/
	if (wl_up(wl)) {
		ND_UNLOCK(wl);
		status = NDIS_STATUS_RESOURCES;
		WL_ERROR(("wl_minit: wl_up error 0x%x\n", status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle, \
			NDIS_ERROR_CODE_OUT_OF_RESOURCES, 1, 15);
		goto error;
	}

#if defined(EXT_STA) || defined(DHD_NDIS_OID)
	/* Cache this value and assume it's using reclaimed image with NODOWN */
	wl->pub->up = TRUE;
#endif /* (BCMDONGLEHOST && BCMDHDUSB) || DHD_NDIS_OID */

#ifndef EXT_STA
	/* WHQL HCT 11 expects an inital link down indication */
	wl_process_link_down(wl);

#ifdef P2P
	if (P2PHANDLE() != NULL) {
		/*
		 *  Hack to set virtual interface down.
		 */
		wl->isVDev = TRUE;
		wl_process_link_down(wl);
		wl->isVDev = FALSE;
	}
#endif /* P2P */


#endif /* !EXT_STA */

#ifdef STA
	if (!AP_ENAB(wl->pub) && wl->auto_connect) {
		WL_ERROR(("wl_minit: auto connect\n"));
		wl_auto_connect(wl);
	}
#endif /* STA */

	ND_UNLOCK(wl);




#ifdef EXT_STA
	wl->next_wsec = -1; /* This will only be changed by wl.exe */
#endif

#ifdef WLBTAMP
#if defined(WLBTWUSB) || defined(WLBTCEAMP11)
	status = BtInit(wl);
	if (NDIS_ERROR(status))
		goto error;
#endif
#endif /* WLBTAMP */

	return (P2PSAVESTATUS(NDIS_STATUS_SUCCESS));

error:
	wl_freeresources(wl);
	return (P2PSAVESTATUS(status));
}

#define	RESSZ		 (4 * 1024)

static NDIS_STATUS
#ifndef NDIS60
wl_find_resources(wl_info_t *wl)
#else
wl_find_resources(wl_info_t *wl, PNDIS_RESOURCE_LIST reslist)
#endif /* !NDIS60 */
{
#ifndef NDIS60
	PNDIS_RESOURCE_LIST reslist;
#endif /* !NDIS60 */
	PCM_PARTIAL_RESOURCE_DESCRIPTOR presdes;
	NDIS_STATUS status;
	BOOLEAN intrfound;
	uint i;
	shared_info_t *sh = &wl->sh;

	WL_TRACE(("wl%d: wl_find_resources\n", sh->unit));

	intrfound = FALSE;

#ifndef NDIS60
	i = RESSZ;
	reslist = MALLOC(sh->osh, i);
	if (!reslist) {
		WL_ERROR(("wl%d: MALLOC error\n", sh->unit));
		goto fail;
	}
	bzero(reslist, RESSZ);
	NdisMQueryAdapterResources(&status, sh->confighandle, reslist, &i);

	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: NdisM{PciAssign,QueryAdapter}Resources: error 0x%x\n",
			sh->unit, status));
		goto exit;
	}
#else
	if (!reslist) {
		WL_ERROR(("wl%d: NULL resource list passed in\n", sh->unit));
		goto fail;
	}
#endif /* !NDIS60 */

	/* find resources */
	for (i = 0; i < reslist->Count; i++) {
		presdes = &reslist->PartialDescriptors[i];

		switch (presdes->Type) {
		case CmResourceTypeMemory:
			/* map in BAR0 registers (ignore BAR1 for now) */
			if (wl->regsva != NULL)
				continue;
			WL_INFORM(("wl%d: Memory Resource 0x%x-0x%x\n", sh->unit,
				presdes->u.Memory.Start.LowPart,
				presdes->u.Memory.Length));
			status = NdisMMapIoSpace((PVOID*) &wl->regsva, sh->adapterhandle,
				presdes->u.Memory.Start, presdes->u.Memory.Length);
			if (NDIS_ERROR(status)) {
				WL_ERROR(("wl%d: NdisMMapIoSpace error 0x%x\n", sh->unit, status));
				goto exit;
			}

#if NDIS_WDM
#endif /* NDIS_WDM */
			/* save length for the unmap */
			wl->regsmaplen = presdes->u.Memory.Length;
			break;
		case CmResourceTypeInterrupt:	/* save vector and level */
			WL_INFORM(("wl%d: Interrupt resource: %d,%d\n", sh->unit,
				presdes->u.Interrupt.Vector,
				presdes->u.Interrupt.Level));
#ifndef NDIS60
			wl->intrvector = presdes->u.Interrupt.Vector;
			wl->intrlevel = presdes->u.Interrupt.Level;
#endif /* NDIS60 */
			intrfound = TRUE;
			break;
		default:
			WL_INFORM(("wl%d: Other (%d) Resource 0x%x-0x%x\n", sh->unit, presdes->Type,
				presdes->u.Memory.Start, presdes->u.Memory.Length));
			break;
		}
	}

	if (wl->regsva == NULL) {
		WL_ERROR(("wl%d: missing PCI BAR0 resource\n", sh->unit));
		goto fail;
	}

	if (intrfound == FALSE) {
		WL_ERROR(("wl%d: missing PCI interrupt resource\n", sh->unit));
		goto fail;
	}

	status = NDIS_STATUS_SUCCESS;
	goto exit;

fail:
	status = NDIS_STATUS_FAILURE;

exit:
#ifndef NDIS60
	if (reslist)
		MFREE(sh->osh, reslist, RESSZ);
#endif /* NDIS60 */
	return (status);
}

static void
wl_freeresources(wl_info_t *wl)
{
	shared_info_t *sh;

	ASSERT(wl);
	sh = &wl->sh;

#if defined(WL_IW_USE_ISCAN)
	wl_iscan_detach(wl);
#endif

#ifndef NDIS60
	/* deregister shutdown handler */
	if (wl->shutdownhandler) {
		NdisMDeregisterAdapterShutdownHandler(sh->adapterhandle);
		wl->shutdownhandler = FALSE;
	}
#endif /* !NDIS60 */

	dhd_remove(sh->osh, wl->dhd, sh);

#if defined(DHD_NDIS_OID)
	dhd_oid_detach(wl);
#endif /* DHD_NDIS_OID */

#ifdef SIMPLE_ISCAN
	dhd_iscan_deattach(wl->dhd);
#endif /* SIMPLE_ISCAN */

#if defined(EXT_STA)
	if (wl->current_bss.bcn_prb) {
		MFREE(wl->sh.osh, wl->current_bss.bcn_prb, wl->current_bss.bcn_prb_len);
	}
#endif /* EXT_STA */

	/* free the rx ndis packet and buffer descriptors pools */
#ifndef NDIS60
	if (sh->rxpacketpool)	{
		NdisFreePacketPool(sh->rxpacketpool);
		sh->rxpacketpool = NULL;
	}
	if (sh->rxbufferpool)	{
		NdisFreeBufferPool(sh->rxbufferpool);
		sh->rxbufferpool = NULL;
	}
#else
	if (sh->rxpacketpool) {
		NdisFreeNetBufferListPool(sh->rxpacketpool);
		sh->rxpacketpool = NULL;
	}
#endif /* !NDIS60 */

	/* dealloc lbuf freelists */
	if (wl->piomode) {
		shared_lb_free(sh, &sh->rxfree, FALSE, TRUE);
		shared_lb_free(sh, &sh->txfree, FALSE, TRUE);
	} else {
#if defined(BCMDHDUSB)
		shared_lb_free(sh, &sh->rxfree, FALSE, TRUE);
		shared_lb_free(sh, &sh->txfree, FALSE, TRUE);
#else
		shared_lb_free(sh, &sh->rxfree, TRUE, TRUE);
		shared_lb_free(sh, &sh->txfree, TRUE, TRUE);
#endif /* BCMDONGLEHOST && BCMDHDUSB */
	}
#ifdef EXT_STA
	if (wl->process_mid_txq_timer)
		wl_del_timer(wl, wl->process_mid_txq_timer);
#endif

	/* free the spinlocks */
	if (wl->gotspinlocks) {
		NdisFreeSpinLock(&wl->lock);
		NdisFreeSpinLock(&wl->dpclock);
#ifdef EXT_STA
		NdisFreeSpinLock(&wl->tx_lock);
#endif /* EXT_STA */
		NdisFreeSpinLock(&wl->tx_lock);
		NdisFreeSpinLock(&wl->dhdlock);
		NdisFreeSpinLock(&wl->dhd_tx_queue_lock);
		NdisFreeSpinLock(&wl->dhd_rx_queue_lock);
		NdisFreeSpinLock(&wl->dhd_evq_lock);
		NdisFreeSpinLock(&wl->rx_lock);
		wl->gotspinlocks = FALSE;
	}

#ifdef EXT_STA
	shared_queue_deinit(&wl->tx_mid_q);
#endif /* EXT_STA */
	shared_queue_deinit(&wl->sh.txdone);
	shared_queue_deinit(&wl->txq);

	if (sh->osh && MALLOCED(sh->osh)) {
#ifdef BCMDBG
		printf("wl%d: Memory leak of size %d\n", sh->unit, MALLOCED(sh->osh));
#endif /* BCMDBG */
#ifdef BCMDBG_MEM
		wl_malloc_dump(sh);
#else
		ASSERT(!MALLOCED(sh->osh));
#endif /* BCMDBG_MEM */
	}

	/* Arasan PCI SDIO host controller uses the map registers 
	 * and wl->piomode is true
	 */
#ifndef NDIS60
	if (wl->gotmapregs) {
		NdisMFreeMapRegisters(sh->adapterhandle);
		wl->gotmapregs = FALSE;
	}
#endif /* !NDIS60 */

	/* unmap chip registers */
	if ((wl->BusType == NdisInterfacePci) || (wl->BusType == NdisInterfacePcMcia)) {
		if (wl->regsva)
			NdisMUnmapIoSpace(sh->adapterhandle, (PVOID) wl->regsva, wl->regsmaplen);
	}


	if (sh->osh)
		osl_detach(sh->osh);

#if defined(EXT_STA) || defined(WLBTAMP_HIGH_ONLY)
	if (wl->oid)
		NdisFreeMemory(wl->oid, sizeof(wl_oid_t), 0);
	if (wl->wlc)
	{
		uint i;

		for (i = 0; i < NBANDS_PUB(wl->pub); i++)
			if (wl->wlc->bandstate[i].pi)
				NdisFreeMemory(wl->wlc->bandstate[i].pi, \
					sizeof(*wl->wlc->bandstate[i].pi), 0);
		NdisFreeMemory(wl->wlc, sizeof(wlc_info_t), 0);
	}
#endif /* BCMDONGLEHOST && EXT_STA */

	NdisFreeMemory(wl, sizeof(wl_info_t), 0);
}

#ifdef BCMDBG_MEM
static void
wl_malloc_dump(shared_info_t *sh)
{
	char *buf;
	struct bcmstrbuf b;
	NDIS_STATUS status;
	osl_t *osh = sh->osh;

#ifdef NDIS60
	buf = NdisAllocateMemoryWithTagPriority(sh->adapterhandle, WLC_IOCTL_MAXLEN, MEMORY_TAG,
	    NormalPoolPriority);
	if (!buf)
		status = NDIS_STATUS_RESOURCES;
	else
		status = NDIS_STATUS_SUCCESS;
#else
	status = NdisAllocateMemoryWithTag(&buf, WLC_IOCTL_MAXLEN, MEMORY_TAG);
#endif /* NDIS60 */
	if (status != NDIS_STATUS_SUCCESS) {
		printf("Could not allocate memory for dump\n");
		return;
	}

	bcm_binit(&b, buf, WLC_IOCTL_MAXLEN);
	MALLOC_DUMP(osh, &b);

	printf("%s", b.origbuf);

	NdisFreeMemory(buf, WLC_IOCTL_MAXLEN, 0);
}
#endif /* BCMDBG_MEM */

static void
#ifndef NDIS60
wl_mhalt(NDIS_HANDLE MiniportAdapterContext)
#else
wl_mhalt(NDIS_HANDLE MiniportAdapterContext, IN NDIS_HALT_ACTION HaltAction)
#endif /* !NDIS60 */
{
	wl_info_t *wl;

	wl = WL_INFO(MiniportAdapterContext);

	ASSERT(wl);
	WL_TRACE(("wl%d: wl_mhalt\n", wl->sh.unit));

#ifdef WLBTAMP
#if defined(WLBTWUSB) || defined(WLBTCEAMP11)
	BtHalt(wl);
#endif
#endif /* WLBTAMP */

	/* Windows 98Gold seems to call miniport halt at a
	 * raised IRQL so the common path of wl_down is now
	 * avoiding accessing PCI configuration space. See
	 * comment below under wl_mshutdown.
	 */
	ND_LOCK(wl);
	wl_down(wl);
	ND_UNLOCK(wl);
	/* Halt the device after down so wl_ndisevent() can complete all indications
	 * sent up from the host
	 */

#if defined(BCMDHDUSB) && defined(EXT_STA)
	wl->devhalted = TRUE;
#endif /* BCMDONGLEHOST && BCMDHDUSB && EXT_STA */

#ifndef NDIS60
	/* Under Win9X it is possible to issue OIDs and even reopen the adapter
	 * after the driver has been halted.  Null out MiniportAdapterContext
	 * so subsequent calls to wl_mqueryinformation, etc. can detect that
	 * this driver has been halted.
	 */
	if (wl->sh.OS == NdisEnvironmentWindows) {
		ULONG i;
		i = NDIS_ATTRIBUTE_DESERIALIZE | ((wl->piomode) ? 0 : NDIS_ATTRIBUTE_BUS_MASTER);
		NdisMSetAttributesEx(wl->sh.adapterhandle, NULL, 1, (ULONG)i, wl->BusType);
	}
#endif /* NDIS60 */

	wl_freeresources(wl);
}

static NDIS_STATUS
wl_mreset(
#ifndef NDIS60
	PBOOLEAN AddressingReset,
	NDIS_HANDLE MiniportAdapterContext
#else
	NDIS_HANDLE MiniportAdapterContext,
	PBOOLEAN AddressingReset
#endif /* NDIS60 */
)
{
#if defined(NOT_IMPLEMENTED_YET)
#else
	return (NDIS_STATUS_SUCCESS);
#endif /* defined (NOT_IMPLEMENTED_YET) */
}

#ifdef NDIS60
static void
wl_mshutdown60(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN NDIS_SHUTDOWN_ACTION ShutdownAction
)
{
	wl_mshutdown(MiniportAdapterContext);
}
#endif /* NDIS60 */

/*
 * called by NDIS during restart/shutdown/crash, driver maybe in any state
 * need to put chip in known state and other hardware in clean state if possible
 */
static void
wl_mshutdown(PVOID ShutdownContext)
{
	wl_info_t *wl;

	wl = WL_INFO(ShutdownContext);
	ASSERT(wl);
#ifdef WLBTAMP
#if defined(WLBTWUSB)
	/* Close Bluetooth interface */
	BTWUSB_KdPrint(("Enter Unload\n"));
	BtKernFound = FALSE;
	CloseBtKern2000();
#elif defined(WLBTCEAMP11)
	KdPrint("Enter Unload\n");
	BtKernFound = FALSE;
	CloseBtKern();
#endif
#endif /* WLBTAMP */


	WL_TRACE(("wl%d: wl_mshutdown\n", wl->sh.unit));

	wl_shutdown(wl);
}

#ifdef NDIS60
static NDIS_STATUS
wl_moidrequest60(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNDIS_OID_REQUEST OidRequest
)
{
	NDIS_STATUS status;
	NDIS_OID Oid;
	PVOID InfoBuf;
	ULONG InfoBufLen;
	ULONG OutBufLen = 0;
	PULONG BytesRead = NULL;
	PULONG BytesWritten = NULL;
	PULONG BytesNeeded;
	int ori_prio = 0;
	wl_info_t *wl = WL_INFO(MiniportAdapterContext);
	if (wl == NULL) {
		status = NDIS_STATUS_NOT_SUPPORTED;
		goto exit;
	}
	if (wl->sh.OidPriority > 0) {
		ori_prio = CeGetThreadPriority(GetCurrentThread());
		CeSetThreadPriority(GetCurrentThread(), wl->sh.OidPriority);
	}
#if defined(BCMDHDUSB)
	if (wl->devhalted == TRUE) {
		WL_ERROR(("wl_moidrequest60: Dev is halted return abort \n"));
		status = NDIS_STATUS_REQUEST_ABORTED;
		goto exit;
	}
#endif /* BCMDONGLEHOST && BCMDHDUSB */

	ND_LOCK(wl);

	switch (OidRequest->RequestType) {
	case NdisRequestQueryInformation:
	case NdisRequestQueryStatistics:
	{
		InfoBuf = OidRequest->DATA.QUERY_INFORMATION.InformationBuffer;
		InfoBufLen = OidRequest->DATA.QUERY_INFORMATION.InformationBufferLength;
		BytesWritten = &OidRequest->DATA.QUERY_INFORMATION.BytesWritten;
		BytesNeeded = &OidRequest->DATA.QUERY_INFORMATION.BytesNeeded;

		Oid = OidRequest->DATA.QUERY_INFORMATION.Oid;


		status = wl_mqueryinformation(MiniportAdapterContext, Oid, InfoBuf, InfoBufLen,
		    BytesWritten, BytesNeeded);
		break;
	}

	case NdisRequestSetInformation:
	{
		InfoBuf = OidRequest->DATA.SET_INFORMATION.InformationBuffer;
		InfoBufLen = OidRequest->DATA.SET_INFORMATION.InformationBufferLength;
		BytesRead = &OidRequest->DATA.SET_INFORMATION.BytesRead;
		BytesNeeded = &OidRequest->DATA.SET_INFORMATION.BytesNeeded;

		Oid = OidRequest->DATA.SET_INFORMATION.Oid;


		status = wl_msetinformation(MiniportAdapterContext, Oid, InfoBuf, InfoBufLen,
		    BytesRead, BytesNeeded);
		break;
	}

	case NdisRequestMethod:
	{
		InfoBuf = OidRequest->DATA.METHOD_INFORMATION.InformationBuffer;
		InfoBufLen = OidRequest->DATA.METHOD_INFORMATION.InputBufferLength;
		OutBufLen = OidRequest->DATA.METHOD_INFORMATION.OutputBufferLength;
		BytesWritten = &OidRequest->DATA.METHOD_INFORMATION.BytesWritten;
		BytesRead = &OidRequest->DATA.METHOD_INFORMATION.BytesRead;
		BytesNeeded = &OidRequest->DATA.METHOD_INFORMATION.BytesNeeded;


		Oid = OidRequest->DATA.METHOD_INFORMATION.Oid;



		status = wl_method_oid(MiniportAdapterContext, Oid, InfoBuf, InfoBufLen,
		    OutBufLen, BytesWritten, BytesRead, BytesNeeded, OidRequest);
		break;
	}

	default:
		status = NDIS_STATUS_INVALID_OID;
		Oid = OidRequest->DATA.METHOD_INFORMATION.Oid;
		WL_ERROR(("wl%d: OID: unrecognized request type %d requested for %s\n",
		    wl->sh.unit, OidRequest->RequestType, wl_oid2str(Oid)));
		goto exit;
	}


exit:
	ND_UNLOCK(wl);
	if (ori_prio > 0) {
		CeSetThreadPriority(GetCurrentThread(), ori_prio);
	}
	return status;
}
#if defined(EXT_STA)
#ifdef BCMDBG
NDIS_STATUS
wl_bsslist_check(NDIS_HANDLE MiniportAdapterContext, PVOID InfoBuf,
	ULONG OutBufLen, PULONG BytesWritten, PULONG BytesNeeded)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	wl_info_t *wl;
	DOT11_BYTE_ARRAY *byteArray;
	DOT11_BSS_ENTRY *dnglBss;

#if !defined(NDIS60)
	int i;
#endif
	int totBytes, newBytes, nBss;

	wl = WL_INFO(MiniportAdapterContext);
	ASSERT(wl);

	if ((!InfoBuf) || (*BytesWritten == 0)) {
		return NDIS_STATUS_BUFFER_OVERFLOW;
	}

	byteArray = (DOT11_BYTE_ARRAY *)InfoBuf;

	nBss = 0;
	newBytes = 0;
	totBytes = byteArray->uNumOfBytes;
	dnglBss = (DOT11_BSS_ENTRY*)byteArray->ucBuffer;

	if (dnglBss == NULL || totBytes == 0) {
		WL_ERROR(("dnglBss or totBytes is NULL\n"));
		return NDIS_STATUS_INVALID_DATA;
	}

	/* Calculate how many BSSes and the memory we need */
	while (totBytes > 0) {
/*		printf("MAC: %X:%X:%X:%X:%X:%X - IE = %d\n",dnglBss->dot11BSSID[0],
			dnglBss->dot11BSSID[1],dnglBss->dot11BSSID[2],
			dnglBss->dot11BSSID[3], dnglBss->dot11BSSID[4],
			dnglBss->dot11BSSID[5], dnglBss->uBufferLength);

		printf(" dnglBss->uLinkQuality = (%d)\n", dnglBss->uLinkQuality);
*/
		newBytes += (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer)
				+ dnglBss->uBufferLength);
		totBytes -= (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer)
				+ dnglBss->uBufferLength);

		dnglBss = (DOT11_BSS_ENTRY *)
			&dnglBss->ucBuffer[dnglBss->uBufferLength];
		nBss++;

		if (dnglBss == NULL) {
			WL_ERROR(("dnglBss is NULL again\n"));
			ASSERT(0);
			return NDIS_STATUS_INVALID_DATA;
		}
	}
	ASSERT(totBytes == 0); /* Must be exact */

	if ((totBytes != 0) || (nBss == 0)) {
		ASSERT(0);
		/* Something is wrong with the list, reject */
		WL_TRACE(("Invalid BSS list totBytes=%d nBss=%d\n", totBytes, nBss));
		return NDIS_STATUS_INVALID_DATA;
	}

	return (status);
}
#endif /* BCMDBG */

NDIS_STATUS
wl_dngl_bsslist_reformat(NDIS_HANDLE MiniportAdapterContext, PVOID InfoBuf,
	ULONG OutBufLen, PULONG BytesWritten, PULONG BytesNeeded)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	wl_info_t *wl;
	DOT11_BYTE_ARRAY *byteArray;
	DOT11_BSS_ENTRY UNALIGNED *Bss;
	DNGL_PACKED_DOT11_BSS_ENTRY *dnglBss;
	void *newBssList;
	int totBytes, newBytes, nBss, allocSize;

	wl = WL_INFO(MiniportAdapterContext);
	ASSERT(wl);

	if ((!InfoBuf) || (*BytesWritten == 0)) {
		*BytesNeeded = OutBufLen;
		return NDIS_STATUS_BUFFER_OVERFLOW;
	}

	byteArray = (DOT11_BYTE_ARRAY *)InfoBuf;

	nBss = 0;
	newBytes = 0;
	totBytes = byteArray->uNumOfBytes;
	dnglBss = (DNGL_PACKED_DOT11_BSS_ENTRY*)byteArray->ucBuffer;

	if (dnglBss == NULL) {
		WL_ERROR(("dnglBss is NULL\n"));
		return NDIS_STATUS_INVALID_DATA;
	}

	/* Calculate how many BSSes and the memory we need */
	while (totBytes > 0) {
		newBytes += (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer)
				+ dnglBss->uBufferLength);
		totBytes -= (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer)
				+ dnglBss->uBufferLength - BSS_ENTRY_STRUCT_ADJ);
		dnglBss = (DNGL_PACKED_DOT11_BSS_ENTRY *)
			&dnglBss->ucBuffer[dnglBss->uBufferLength];
		nBss++;

		if (dnglBss == NULL) {
			WL_ERROR(("dnglBss is NULL again\n"));
			return NDIS_STATUS_INVALID_DATA;
		}
	}
	ASSERT(totBytes == 0); /* Must be exact */

	if ((totBytes != 0) || (nBss == 0)) {
		/* Something is wrong with the list, reject */
		WL_TRACE(("Invalid BSS list totBytes=%d nBss=%d\n", totBytes, nBss));
		return NDIS_STATUS_INVALID_DATA;
	}

	allocSize = (nBss * BSS_ENTRY_STRUCT_ADJ) + newBytes;
	if (allocSize > (int)OutBufLen) {
		*BytesNeeded = allocSize;
		WL_TRACE(("InfBuf too small, allocSize=%d, newBytes=%d\n", allocSize, newBytes));
		return NDIS_STATUS_BUFFER_OVERFLOW;
	}

	newBssList = MALLOC(wl->sh.osh, allocSize);
	ASSERT(newBssList);
	if (!newBssList)
		return NDIS_STATUS_RESOURCES;

	bzero(newBssList, allocSize);

	Bss = (DOT11_BSS_ENTRY *)newBssList;
	dnglBss = (DNGL_PACKED_DOT11_BSS_ENTRY*)byteArray->ucBuffer;

	/* Build new BSS list for Host */
	while (nBss-- > 0) {
		Bss->uPhyId = dnglBss->uPhyId;
		Bss->PhySpecificInfo.uChCenterFrequency = \
			dnglBss->PhySpecificInfo.uChCenterFrequency;

		bcopy(dnglBss->dot11BSSID, Bss->dot11BSSID, ETHER_ADDR_LEN);
		Bss->dot11BSSType = dnglBss->dot11BSSType;
		Bss->lRSSI = dnglBss->lRSSI;
		Bss->uLinkQuality = dnglBss->uLinkQuality;
		Bss->bInRegDomain = dnglBss->bInRegDomain;
		Bss->usBeaconPeriod = dnglBss->usBeaconPeriod;
		Bss->ullTimestamp = dnglBss->ullTimestamp;
		Bss->ullHostTimestamp = dnglBss->ullHostTimestamp;
		Bss->usCapabilityInformation =  dnglBss->usCapabilityInformation;
		Bss->uBufferLength =  dnglBss->uBufferLength;
		bcopy(dnglBss->ucBuffer, Bss->ucBuffer, Bss->uBufferLength);

		/* next entry */
		Bss = (DOT11_BSS_ENTRY *)&Bss->ucBuffer[Bss->uBufferLength];
		dnglBss = (DNGL_PACKED_DOT11_BSS_ENTRY *)
			&dnglBss->ucBuffer[dnglBss->uBufferLength];
	}

	byteArray->uNumOfBytes = byteArray->uTotalNumOfBytes = newBytes;

	/* Now replace the BSS list from dongle with new list */
	Bss = (DOT11_BSS_ENTRY *)newBssList;
	dnglBss = (DNGL_PACKED_DOT11_BSS_ENTRY*)byteArray->ucBuffer;
	bcopy(Bss, dnglBss, byteArray->uNumOfBytes);

	*BytesWritten = byteArray->uNumOfBytes + OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);

	MFREE(wl->sh.osh, newBssList, allocSize);
	newBssList = NULL;

	return (status);
}

void
wl_oidreq_async_cb(async_req_t *req, void *arg)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	wl_info_t *wl = (wl_info_t *) arg;
	osl_t *osh = wl->sh.osh;
	NDIS_REQUEST_TYPE reqType;
	NDIS_OID Oid;
	PVOID InfoBuf;
	ULONG InfoBufLen, OutBufLen;
	PULONG BytesRead, BytesWritten, BytesNeeded;

	ASSERT(req);
	ASSERT(osh);
	ASSERT(wl);

	reqType = req->parms.oidreq.reqType;
	Oid = req->parms.oidreq.oid;
	InfoBuf = req->parms.oidreq.infoBuf;
	InfoBufLen = req->parms.oidreq.infoBufLen;
	OutBufLen = req->parms.oidreq.outBufLen;
	BytesRead = req->parms.oidreq.bytesRead;
	BytesWritten = req->parms.oidreq.bytesWritten;
	BytesNeeded = req->parms.oidreq.bytesNeeded;

	if (reqType == NdisRequestSetInformation) {

		status = wl_msetinformation(wl->sh.adaptercontext, Oid, InfoBuf, InfoBufLen,
			BytesRead, BytesNeeded);
	} else if ((reqType == NdisRequestQueryInformation) ||
		(reqType == NdisRequestQueryStatistics)) {

		status = wl_mqueryinformation(wl->sh.adaptercontext, Oid, InfoBuf, InfoBufLen,
			BytesWritten, BytesNeeded);

	} else if (reqType == NdisRequestMethod) {

		status = wl_method_oid(wl->sh.adaptercontext, Oid, InfoBuf, InfoBufLen,
			OutBufLen, BytesWritten, BytesRead, BytesNeeded,
			req->parms.oidreq.pendingOidReq);
	} else {
		/* Unknown reqType */
		ASSERT(0);
	}

	WL_TRACE(("wl_oidreq_async_cb OID=0x%x, ReqType=%d, status=0x%x\n",
		req->parms.oidreq.oid, req->parms.oidreq.reqType, status));

#ifdef NDIS60
	NdisMOidRequestComplete(wl->sh.adapterhandle,
		req->parms.oidreq.pendingOidReq, status);
#else
	if (reqType == NdisRequestSetInformation) {
		NdisMSetInformationComplete(wl->sh.adapterhandle, status);
	} else {
		NdisMQueryInformationComplete(wl->sh.adapterhandle, status);
	}
#endif /* NDIS60 */

	MFREE(osh, req, sizeof(async_req_t));
}

static NDIS_STATUS
wl_oidreq_async(wl_info_t *wl, NDIS_OID oid, PVOID buf, ULONG bufLen, ULONG outBufLen,
	PULONG bytesWritten, PULONG bytesRead, PULONG bytesNeeded,
	PNDIS_OID_REQUEST oidReq)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	osl_t *osh = wl->sh.osh;
	async_req_t *req;

	req = MALLOC(osh, sizeof(async_req_t));
	if (!req)
		return NDIS_STATUS_FAILURE;

	bzero(req, sizeof(async_req_t));

	req->cb = wl_oidreq_async_cb;
	req->arg = wl;
	req->parms.oidreq.oid = oid;
	req->parms.oidreq.infoBuf = buf;
	req->parms.oidreq.infoBufLen = bufLen;
	req->parms.oidreq.outBufLen = outBufLen;
	req->parms.oidreq.bytesRead = bytesRead;
	req->parms.oidreq.bytesWritten = bytesWritten;
	req->parms.oidreq.bytesNeeded = bytesNeeded;

	if (oidReq) {
		req->parms.oidreq.pendingOidReq = oidReq;
		req->parms.oidreq.reqType = oidReq->RequestType;
	}

	WL_TRACE(("wl_oidreq_async OID=0x%x, ReqType=%d\n", \
				req->parms.oidreq.oid, req->parms.oidreq.reqType));
	return status;
}

void
wl_ioctl_async_cb(async_req_t *req, void *arg)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	wl_info_t *wl = (wl_info_t *) arg;
	osl_t *osh = wl->sh.osh;
	ULONG cmd;
	PVOID InfoBuf;
	ULONG InfoBufLen;

	ASSERT(req);
	ASSERT(osh);
	ASSERT(wl);


	cmd = req->parms.ioctlreq.cmd;
	InfoBuf = req->parms.ioctlreq.infoBuf;
	InfoBufLen = req->parms.ioctlreq.infoBufLen;

	WL_TRACE(("%s: IOCTL wl=0x%p cmd=0x%x buf=0x%p len=%d\n", \
				__FUNCTION__, wl, cmd, InfoBuf, InfoBufLen));
	wl_ioctl(wl, cmd, InfoBuf, InfoBufLen);

	if (req->parms.ioctlreq.freeInfoBuf)
		MFREE(osh, InfoBuf, InfoBufLen);

	MFREE(osh, req, sizeof(async_req_t));

}

static NDIS_STATUS
wl_ioctl_async(wl_info_t *wl, ULONG cmd, PVOID buf, ULONG bufLen)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	osl_t *osh = wl->sh.osh;
	async_req_t *req;

	req = MALLOC(osh, sizeof(async_req_t));
	if (!req)
		return NDIS_STATUS_FAILURE;

	bzero(req, sizeof(async_req_t));

	if (buf == NULL && bufLen > 0)
	{
		buf = MALLOC(osh, bufLen);
		if (!buf)
		{
			MFREE(osh, req, sizeof(async_req_t));
			return NDIS_STATUS_FAILURE;
		}
		req->parms.ioctlreq.freeInfoBuf = TRUE;
	}

	req->cb = wl_ioctl_async_cb;
	req->arg = wl;
	req->parms.ioctlreq.cmd = cmd;
	req->parms.ioctlreq.infoBuf = buf;
	req->parms.ioctlreq.infoBufLen = bufLen;

	WL_TRACE(("%s: IOCTL wl=0x%p cmd=0x%x buf=0x%p len=%d\n", \
				__FUNCTION__, wl, cmd, buf, bufLen));
	return status;
}
#endif /* BCMDONGLEHOST && EXT_STA */

NDIS_STATUS
wl_method_oid(NDIS_HANDLE MiniportAdapterContext, NDIS_OID Oid, PVOID InfoBuf,
	ULONG InBufLen, ULONG OutBufLen, PULONG BytesWritten, PULONG BytesRead,
	PULONG BytesNeeded, PNDIS_OID_REQUEST OidRequest)
{
	NDIS_STATUS status;
	wl_info_t *wl = WL_INFO(MiniportAdapterContext);

	ASSERT(wl);
	if (Oid == OID_DOT11_ENUM_BSS_LIST) {


		status = wl_mqueryinformation(MiniportAdapterContext, Oid, InfoBuf,
		    OutBufLen, BytesWritten, BytesNeeded);
	} else if (Oid == OID_DOT11_RESET_REQUEST) {
		ND_PKT *p;
		 /* Reset pairwise key flag */
		wl->pwk = 0;
		/*  Reset txq counter (ensure txq processed properly by timer) */
		wl->tx_mid_q_count = 0;
		wl->txq_count = 0;
		if (OutBufLen < sizeof(DOT11_STATUS_INDICATION)) {
			DOT11_BYTE_ARRAY *byte_array;

			byte_array = (DOT11_BYTE_ARRAY *)InfoBuf;
			byte_array->uNumOfBytes = byte_array->uTotalNumOfBytes =
				OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);

			status = NDIS_STATUS_BUFFER_OVERFLOW;
			*BytesWritten = 0;
			*BytesNeeded = sizeof(DOT11_STATUS_INDICATION);
			return status;
		}

		status = wl_msetinformation(MiniportAdapterContext, Oid, InfoBuf,
		    InBufLen, BytesRead, BytesNeeded);

		if (status == NDIS_STATUS_SUCCESS) {
			ND_TXLOCK(wl);
			/* flush NDIS txq */
			while (p = shared_deq(&wl->txq))
				shared_enq(&wl->sh.txdone, p, FALSE);
			ND_TXUNLOCK(wl);
			wl_sendcomplete(wl);

			/* wait for all indicated packets to have been returned */
			if ((wl->rxind - wl->rxreturn) != 0) {
				status = NDIS_STATUS_PENDING;

				ASSERT(!wl->pended_oid);
				wl->pended_oid = OidRequest;
			}

			*BytesRead = sizeof(DOT11_RESET_REQUEST);
		}
	} else if (Oid == OID_DOT11_NIC_SPECIFIC_EXTENSION) {
		struct reqHeader {
			ULONG oid;
			union {
				getinformation_t getinfo;
				setinformation_t setinfo;
			};
		} *req;

		WL_OID(("wl%d: OID: ACM API passed %s\n", wl->sh.unit, wl_oid2str(Oid)));

		req = (struct reqHeader *)InfoBuf;
		if (InBufLen < sizeof(req->oid)) {
			status = NDIS_STATUS_INVALID_LENGTH;
			WL_ERROR(("wl%d: OID_DOT11_NIC_SPECIFIC_EXTENSION "
			    "with invalid length %d\n", wl->sh.unit, InBufLen));
		} else if (req->oid == OID_BCM_GETINFORMATION) {
			status = wl_mqueryinformation(MiniportAdapterContext, req->oid,
			    &req->getinfo, OutBufLen - sizeof(ULONG),
			    BytesWritten, BytesNeeded);
			*BytesWritten += sizeof(ULONG);
		} else if (req->oid == OID_BCM_SETINFORMATION) {
			status = wl_msetinformation(MiniportAdapterContext, req->oid,
			    &req->setinfo, InBufLen - sizeof(ULONG),
			    BytesRead, BytesNeeded);
		} else {
			WL_ERROR(("wl%d: OID_DOT11_NIC_SPECIFIC_EXTENSION "
			    "with unknown OID_BCM 0x%x\n", wl->sh.unit, req->oid));
			status = NDIS_STATUS_SUCCESS;
		}
	} else if (Oid == OID_DOT11_RECV_SENSITIVITY_LIST) {
		status = wl_mqueryinformation(MiniportAdapterContext, Oid, InfoBuf,
		    OutBufLen, BytesWritten, BytesNeeded);
	} else {
		status = NDIS_STATUS_NOT_SUPPORTED;
		WL_ERROR(("wl%d: OID: unexpected Method OID %s\n",
		    wl->sh.unit, wl_oid2str(Oid)));
	}

	return status;
}
#endif /* NDIS60 */

#ifdef WLBTAMP_HIGH_ONLY
void
wl_bta_cmd_async_cb(async_req_t *req, void *arg)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	wl_info_t *wl = (wl_info_t *) arg;
	osl_t *osh = wl->sh.osh;
	PVOID InfoBuf;
	ULONG InfoBufLen;

	ASSERT(req);
	ASSERT(osh);
	ASSERT(wl);

	InfoBuf = req->parms.btareq.infoBuf;
	InfoBufLen = req->parms.btareq.infoBufLen;

	WL_TRACE(("%s: BTA wl=0x%p buf=0x%p len=%d\n", \
				__FUNCTION__, wl, InfoBuf, InfoBufLen));

	ASSERT(wl->wlc && wl->wlc->bta);

	wlc_bta_docmd(wl->wlc->bta, InfoBuf, InfoBufLen);

	if (req->parms.btareq.freeInfoBuf)
		MFREE(osh, InfoBuf, InfoBufLen);

	MFREE(osh, req, sizeof(async_req_t));
}

int
wl_bta_cmd_async(wl_info_t *wl, void *buf, uint32 bufLen)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	osl_t *osh = wl->sh.osh;
	async_req_t *req;
	void *cmdbuf;

	req = MALLOC(osh, sizeof(async_req_t));
	if (!req)
		return NDIS_STATUS_FAILURE;

	bzero(req, sizeof(async_req_t));

	if (buf != NULL && bufLen > 0)
	{
		cmdbuf = MALLOC(osh, bufLen);
		if (!cmdbuf)
		{
			MFREE(osh, req, sizeof(async_req_t));
			return NDIS_STATUS_FAILURE;
		}
		req->parms.btareq.freeInfoBuf = TRUE;
	}

	req->cb = wl_bta_cmd_async_cb;
	req->arg = wl;
	memcpy(cmdbuf, buf, bufLen);
	req->parms.btareq.infoBuf = cmdbuf;
	req->parms.btareq.infoBufLen = bufLen;

	WL_TRACE(("%s: BTA_CMD wl=0x%p buf=0x%p len=%d\n", \
				__FUNCTION__, wl, buf, bufLen));

	dhd_async_req_schedule(wl, req);

	return status;
}

void
wl_bta_evt_async_cb(async_req_t *req, void *arg)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	wl_info_t *wl = (wl_info_t *) arg;
	osl_t *osh = wl->sh.osh;
	PVOID InfoBuf;
	ULONG InfoBufLen;

	ASSERT(req);
	ASSERT(osh);
	ASSERT(wl);

	InfoBuf = req->parms.btareq.infoBuf;
	InfoBufLen = req->parms.btareq.infoBufLen;

	WL_TRACE(("%s: BTA wl=0x%p buf=0x%p len=%d\n", \
				__FUNCTION__, wl, InfoBuf, InfoBufLen));

	ASSERT(wl->wlc && wl->wlc->bta);

	wlc_bta_doevent(wl->wlc->bta, (amp_hci_event_t *)InfoBuf);

	if (req->parms.btareq.freeInfoBuf)
		MFREE(osh, InfoBuf, InfoBufLen);

	MFREE(osh, req, sizeof(async_req_t));

}

int
wl_bta_evt_async(wl_info_t *wl, void *buf, uint32 bufLen)
{
	NDIS_STATUS status = NDIS_STATUS_SUCCESS;
	osl_t *osh = wl->sh.osh;
	async_req_t *req;
	void *evtbuf;

	req = MALLOC(osh, sizeof(async_req_t));
	if (!req)
		return NDIS_STATUS_FAILURE;

	bzero(req, sizeof(async_req_t));

	if (buf != NULL && bufLen > 0)
	{
		evtbuf = MALLOC(osh, bufLen);
		if (!evtbuf)
		{
			MFREE(osh, req, sizeof(async_req_t));
			return NDIS_STATUS_FAILURE;
		}
		req->parms.btareq.freeInfoBuf = TRUE;
	}

	req->cb = wl_bta_evt_async_cb;
	req->arg = wl;
	memcpy(evtbuf, buf, bufLen);
	req->parms.btareq.infoBuf = evtbuf;
	req->parms.btareq.infoBufLen = bufLen;

	WL_TRACE(("%s: BTA_EVT wl=0x%p buf=0x%p len=%d\n", \
				__FUNCTION__, wl, buf, bufLen));

	dhd_async_req_schedule(wl, req);

	return status;
}
#endif /* WLBTAMP_HIGH_ONLY */


#if defined(EXT_STA)
#if defined(BCMDHDUSB)
void
wl_reload_init(wl_info_t *wl)
{
	char eventmask[WL_EVENTING_MASK_LEN];
	char iovbuf[WL_EVENTING_MASK_LEN + 12];	/* Room for "event_msgs" + '\0' + bitvec */
	shared_info_t *sh;
	NDIS_HANDLE confighandle = NULL;
	NDIS_STATUS status;
	NDIS_CONFIGURATION_OBJECT	configobj;
	ULONG bytesneeded, bytesused;

	bzero(eventmask, sizeof(eventmask));
	register_ext_sta_events(eventmask);
	bcm_mkiovar("event_msgs", eventmask, WL_EVENTING_MASK_LEN, iovbuf, sizeof(iovbuf));
	wl_ioctl(wl, WLC_SET_VAR, iovbuf, sizeof(iovbuf));

	ASSERT(wl);
	sh = &wl->sh;
	ASSERT(sh);
	confighandle = sh->confighandle;

	/* read configuration data from adapter config registry space */
	status = wl_readconfigdata(wl, confighandle, sh->adapterhandle);
	if (status == NDIS_STATUS_SUCCESS) {
		bzero(&configobj, sizeof(NDIS_CONFIGURATION_OBJECT));
		configobj.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
		configobj.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
		configobj.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT);
		configobj.NdisHandle = sh->adapterhandle;
		configobj.Flags = 0;
		status = NdisOpenConfigurationEx(&configobj, &confighandle);
		ASSERT(confighandle);
		if (status == NDIS_STATUS_SUCCESS) {
			wl->oid->sw_phy_state = (mbool)-1;
			/* read reg entries that may override defaults set in wlc_attach() */
			wl_readconfigoverrides((void *) wl, confighandle, sh->id, sh->unit, sh->OS);
			wl_scanoverrides(wl, confighandle, sh->id, sh->unit);
			/* close the registry */
			NdisCloseConfiguration(confighandle);
		}
	}

	wl_iovar_setint(wl, "assoc_recreate", 1);
	wl->pub->_assoc_recreate = 1; /* Host synchronization */

	/* Sync up PHY data with dongle after reload
	 * Fails during DTM Sleep Stress with IO
	 */
	wl_setoid(wl, (NDIS_OID)OID_STA_SET_PHY,
		(PVOID) &wl->oid->phy, (ULONG) sizeof(wl->oid->phy),
		&bytesused, &bytesneeded, 0);

	wl_setoid(wl, (NDIS_OID)OID_STA_SET_PHY_STATE,
		(PVOID)&wl->oid->sw_phy_state, (ULONG)sizeof(wl->oid->sw_phy_state),
		&bytesused, &bytesneeded, 0);
}
#endif /* BCMDHDUSB */

/* snoops or caches oid information */
void
wl_oidinfo_snoop(
	NDIS_HANDLE MiniportAdapterContext,
	NDIS_OID Oid,
	PVOID InfoBuf,
	ULONG InfoBufLen,
	PULONG BytesReadWritten,
	PULONG BytesNeeded)
{
	wl_info_t *wl;
	int len;

	wl = WL_INFO(MiniportAdapterContext);

	if (wl == NULL)
	    return;

	switch (Oid) {
	case OID_DOT11_CURRENT_PHY_ID:
		wl->oid->cur_phy_id = *(ULONG *)InfoBuf;
		break;

	case (WL_OID_BASE+WLC_SET_VAR):
		if (!strncmp((char *)InfoBuf, "msglevel", InfoBufLen)) {
			len = strlen("msglevel") + 1;

			bcopy(((char *)InfoBuf + len), &wl_msg_level, sizeof(int));
		}
		break;
	case OID_DOT11_CONNECT_REQUEST:
		WL_ASSOC(("%s: 0x%x, wl->wlc->assoc_type=%d --> AS_ASSOCIATION(%d)\n", \
					__FUNCTION__, Oid, wl->wlc->assoc_type, AS_ASSOCIATION));
		wl->wlc->assoc_type = AS_ASSOCIATION;
		break;
	default:
		break;
	}
}

static NDIS_STATUS
wl_oid_post_process(NDIS_HANDLE MiniportAdapterContext,
	NDIS_OID Oid,
	PVOID InfoBuf,
	ULONG InBufLen,
	ULONG OutBufLen,
	PULONG BytesWritten,
	PULONG BytesRead,
	PULONG BytesNeeded,
	PNDIS_OID_REQUEST OidRequest,
	NDIS_STATUS prevStatus)
{
	NDIS_STATUS status = prevStatus;
	wl_info_t *wl;

	wl = WL_INFO(MiniportAdapterContext);

	/* InfoBuf is NULL during NDISTest OIDs_ext */
	if ((wl == NULL) || (InfoBuf == NULL))
	    return status;

	switch (Oid) {
	case OID_DOT11_ENUM_BSS_LIST:
		if (prevStatus == NDIS_STATUS_SUCCESS)
		{
			status = wl_dngl_bsslist_reformat(MiniportAdapterContext,
				InfoBuf, OutBufLen, BytesWritten, BytesNeeded);
		} else {
			if (prevStatus == NDIS_STATUS_BUFFER_OVERFLOW)
			{
				ULONG read, needed;

				wl->oid->num_bss = 0;
				wl_queryoid(wl, OID_STA_GET_NUM_BSS, &wl->oid->num_bss,
					sizeof(wl->oid->num_bss), &read, &needed, P2PUNITNUMBER());

				/*
				 * We need it to calculate the extra overhead per BSS entry.
				 */
				*BytesNeeded += wl->oid->num_bss * BSS_ENTRY_STRUCT_ADJ;
			}
		}

		if (status == NDIS_STATUS_BUFFER_OVERFLOW)
		{
			DOT11_BYTE_ARRAY *byte_array;

			byte_array = (DOT11_BYTE_ARRAY *)InfoBuf;
			byte_array->uNumOfBytes = byte_array->uTotalNumOfBytes
				= OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);
			*BytesWritten = 0;
		}

		break;
	case OID_DOT11_STATISTICS: {
		DOT11_STATISTICS *stats;
		DNGL_DOT11_STATISTICS *dngl_stats;
		int i;

		if (prevStatus == NDIS_STATUS_BUFFER_OVERFLOW) {
			/* Intercept the dongle's BytesNeeded and update with
			 * what the Host actually needs since there's a size mismatch
			 * between Host (400) and Dongle (396);
			 * Doing this here so we don't have to change dongle code and leaves
			 * the conversion done exclusively on host side.
			 */
			*BytesNeeded = OFFSETOF(DOT11_STATISTICS, PhyCounters) +
				(wl->oid->phy.num_phy_ids * sizeof(DOT11_PHY_FRAME_STATISTICS));
			break;
		}

		if (OutBufLen < sizeof(DOT11_STATISTICS)) {
			WL_ERROR(("InfoBuf too small for DOT11_STATISTICS=%d\n", OutBufLen));
			break;
		}

		stats = MALLOC(wl->sh.osh, OutBufLen);
		if (stats == NULL)
			break;

		dngl_stats = (DNGL_DOT11_STATISTICS *) InfoBuf;

		bcopy(&dngl_stats->Header, &stats->Header, sizeof(stats->Header));
		bcopy(&dngl_stats->ullFourWayHandshakeFailures, &stats->ullFourWayHandshakeFailures,
			sizeof(stats->ullFourWayHandshakeFailures));
		bcopy(&dngl_stats->ullTKIPCounterMeasuresInvoked,
			&stats->ullTKIPCounterMeasuresInvoked,
			sizeof(stats->ullTKIPCounterMeasuresInvoked));
		bcopy(&dngl_stats->ullReserved, &stats->ullReserved,
			sizeof(stats->ullReserved));
		bcopy(&dngl_stats->MacUcastCounters, &stats->MacUcastCounters,
			sizeof(stats->MacUcastCounters));
		bcopy(&dngl_stats->MacMcastCounters, &stats->MacMcastCounters,
			sizeof(stats->MacMcastCounters));

		for (i = 0; i < (int)wl->oid->phy.num_phy_ids; i++) {
			bcopy(&dngl_stats->PhyCounters[i], &stats->PhyCounters[i],
				sizeof(stats->PhyCounters));
		}

		stats->Header.Size = sizeof(DOT11_STATISTICS);
		*BytesWritten = OutBufLen;

		/* Copy converted struct back */
		bcopy(stats, InfoBuf, OutBufLen);
		MFREE(wl->sh.osh, stats, OutBufLen);
	}
		break;
	case OID_DOT11_ENUM_ASSOCIATION_INFO: {
		DOT11_ASSOCIATION_INFO_LIST *phost_info;
		DOT11_ASSOCIATION_INFO_EX *phost_ai;
		DNGL_PACKED_DOT11_ASSOCIATION_INFO_LIST *pdngl_info;
		DNGL_PACKED_DOT11_ASSOCIATION_INFO_EX *pdngl_ai;
		uint i, j;
		ibss_peer_t *peer;

		if (prevStatus == NDIS_STATUS_BUFFER_OVERFLOW) {
			/* Recalculate size needed for host since the dongle size needed is not
			 * accurate because it's a packed struct
			 */
			*BytesNeeded = OFFSETOF(DOT11_ASSOCIATION_INFO_LIST, dot11AssocInfo) +
				(wl->oid->num_peers * sizeof(DOT11_ASSOCIATION_INFO_EX));
			break;
		} else if (prevStatus != NDIS_STATUS_SUCCESS) {
			WL_ERROR(("OID_DOT11_ENUM_ASSOCIATION_INFO error=0x%x\n", prevStatus));
			break;
		}

		phost_info = MALLOC(wl->sh.osh, OutBufLen);
		if (phost_info == NULL)
			break;
		bzero(phost_info, OutBufLen);

		pdngl_info = (DNGL_PACKED_DOT11_ASSOCIATION_INFO_LIST *)InfoBuf;

		bcopy(&pdngl_info->Header, &phost_info->Header, sizeof(phost_info->Header));
		phost_info->Header.Size = sizeof(DOT11_ASSOCIATION_INFO_LIST);

		bcopy(&pdngl_info->uNumOfEntries, &phost_info->uNumOfEntries,
			sizeof(phost_info->uNumOfEntries));
		bcopy(&pdngl_info->uTotalNumOfEntries, &phost_info->uTotalNumOfEntries,
			sizeof(phost_info->uTotalNumOfEntries));

		if (wl->oid->NDIS_infra == Ndis802_11Infrastructure) {
			pdngl_ai = pdngl_info->dot11AssocInfo;
			phost_ai = phost_info->dot11AssocInfo;
			bcopy(&pdngl_ai->PeerMacAddress, &phost_ai->PeerMacAddress,
				sizeof(phost_ai->PeerMacAddress));
			bcopy(&pdngl_ai->BSSID, &phost_ai->BSSID,
				sizeof(phost_ai->BSSID));
			bcopy(&pdngl_ai->usCapabilityInformation,
				&phost_ai->usCapabilityInformation,
				sizeof(phost_ai->usCapabilityInformation));
			bcopy(&pdngl_ai->usListenInterval, &phost_ai->usListenInterval,
				sizeof(phost_ai->usListenInterval));
			bcopy(pdngl_ai->ucPeerSupportedRates, phost_ai->ucPeerSupportedRates,
				sizeof(phost_ai->ucPeerSupportedRates));
			bcopy(&pdngl_ai->usAssociationID, &phost_ai->usAssociationID,
				sizeof(phost_ai->usAssociationID));
			bcopy(&pdngl_ai->dot11AssociationState, &phost_ai->dot11AssociationState,
				sizeof(phost_ai->dot11AssociationState));
			bcopy(&pdngl_ai->dot11PowerMode, &phost_ai->dot11PowerMode,
				sizeof(phost_ai->dot11PowerMode));
			bcopy(&pdngl_ai->liAssociationUpTime, &phost_ai->liAssociationUpTime,
				sizeof(phost_ai->liAssociationUpTime));
			bcopy(&pdngl_ai->ullNumOfTxPacketSuccesses,
				&phost_ai->ullNumOfTxPacketSuccesses,
				sizeof(phost_ai->ullNumOfTxPacketSuccesses));
			bcopy(&pdngl_ai->ullNumOfTxPacketFailures,
				&phost_ai->ullNumOfTxPacketFailures,
				sizeof(phost_ai->ullNumOfTxPacketFailures));
			bcopy(&pdngl_ai->ullNumOfRxPacketSuccesses,
				&phost_ai->ullNumOfRxPacketSuccesses,
				sizeof(phost_ai->ullNumOfRxPacketSuccesses));
			bcopy(&pdngl_ai->ullNumOfRxPacketFailures,
				&phost_ai->ullNumOfRxPacketFailures,
				sizeof(phost_ai->ullNumOfRxPacketFailures));
		} else {

			ASSERT(wl->oid->num_peers == phost_info->uNumOfEntries);

			for (i = 0; i < wl->oid->num_peers; i++) {
				peer = &wl->oid->peers[i];
				pdngl_ai = &pdngl_info->dot11AssocInfo[i];
				phost_ai = &phost_info->dot11AssocInfo[i];

				bcopy(peer->addr.octet, &phost_ai->PeerMacAddress,
					sizeof(phost_ai->PeerMacAddress));
				bcopy(&pdngl_ai->BSSID, &phost_ai->BSSID,
					sizeof(phost_ai->BSSID));
				bcopy(peer->addr.octet, &phost_ai->usCapabilityInformation,
					sizeof(phost_ai->usCapabilityInformation));
				bcopy(&pdngl_ai->usListenInterval, &phost_ai->usListenInterval,
					sizeof(phost_ai->usListenInterval));

				ASSERT(peer->rateset.count < MAX_NUM_SUPPORTED_RATES_V2);
				for (j = 0; j < peer->rateset.count; j++)
					phost_ai->ucPeerSupportedRates[j] =
					peer->rateset.rates[j] & DOT11_DATA_RATE_INDEX_MASK;

				bcopy(&pdngl_ai->usAssociationID, &phost_ai->usAssociationID,
					sizeof(phost_ai->usAssociationID));
				bcopy(&pdngl_ai->dot11AssociationState,
					&phost_ai->dot11AssociationState,
					sizeof(phost_ai->dot11AssociationState));
				bcopy(&pdngl_ai->dot11PowerMode, &phost_ai->dot11PowerMode,
					sizeof(phost_ai->dot11PowerMode));
				bcopy(&peer->assoc_time, &phost_ai->liAssociationUpTime,
					sizeof(phost_ai->liAssociationUpTime));
				bcopy(&pdngl_ai->ullNumOfTxPacketSuccesses,
					&phost_ai->ullNumOfTxPacketSuccesses,
					sizeof(phost_ai->ullNumOfTxPacketSuccesses));
				bcopy(&pdngl_ai->ullNumOfTxPacketFailures,
					&phost_ai->ullNumOfTxPacketFailures,
					sizeof(phost_ai->ullNumOfTxPacketFailures));
				bcopy(&pdngl_ai->ullNumOfRxPacketSuccesses,
					&phost_ai->ullNumOfRxPacketSuccesses,
					sizeof(phost_ai->ullNumOfRxPacketSuccesses));
				bcopy(&pdngl_ai->ullNumOfRxPacketFailures,
					&phost_ai->ullNumOfRxPacketFailures,
					sizeof(phost_ai->ullNumOfRxPacketFailures));
			}
		}

		/* Copy converted struct back */
		bcopy(phost_info, InfoBuf, OutBufLen);
		*BytesWritten = OutBufLen;

		MFREE(wl->sh.osh, phost_info, OutBufLen);
	}
		break;
	default:
		break;
	}

	return status;
}
#endif /* BCMDONGLEHOST && EXT_STA */

NDIS_STATUS
wl_mqueryinformation(
	NDIS_HANDLE MiniportAdapterContext,
	NDIS_OID Oid,
	PVOID InfoBuf,
	ULONG InfoBufLen,
	PULONG BytesWritten,
	PULONG BytesNeeded
)
{
	wl_info_t *wl;
	ULONG GenericULong;
	LONG GenericLong;
	UCHAR GenericArray[72];
	PVOID MoveSource;
	ULONG MoveBytes;
	NDIS_STATUS status;
	BOOLEAN copy_mem;
	getinformation_t *ginfo = NULL;
#if defined(WDM) && defined(NOT_YET)
	PNDIS_PNP_CAPABILITIES pmcap;
#endif /* defined(WDM) && defined (NOT_YET) */
	int save_bcmerror;
	bool set = FALSE;
	UINT	wlif = 0;

#ifdef TOE
	PNDIS_TASK_OFFLOAD_HEADER   pNdisTaskOffloadHdr;
	PNDIS_TASK_OFFLOAD          pTaskOffload;
	PNDIS_TASK_TCP_IP_CHECKSUM  pTcpIpChecksumTask;
	ULONG                       ulHeadersLen;
	ULONG                       ulMaxOffloadSize;
	UINT                        i;
#endif /* TOE */

	wl = WL_INFO(MiniportAdapterContext);

	if (wl == NULL)
	    return NDIS_STATUS_NOT_SUPPORTED;

	WL_INFORM(("wl%d: wl_mqueryinformation:  oid 0x%x\n", wl->sh.unit, Oid));
#if !defined(NDIS60)
#ifdef P2P
	if (P2PUNITNUMBER()) {
		switch (Oid) {
			case OID_GEN_MEDIA_CONNECT_STATUS:
				GenericULong = (!gP2P.wl_p2p_vif_up) ?
					NdisMediaStateConnected : NdisMediaStateDisconnected;
				NdisMoveMemory(InfoBuf, &GenericULong, sizeof(ULONG));
				*BytesWritten = sizeof(ULONG);
				return NDIS_STATUS_SUCCESS;
				break;
			default:
				gP2P.wl_instance_wl->isVDev = FALSE;
				wlif = P2PUNITNUMBER();
				MiniportAdapterContext = gP2P.wl_instance_wl;
				break;
		}
	} else {
		gP2P.wl_instance_wl->isVDev = FALSE;
	}
#endif /* ifdef P2P */
#endif /* UNDER_CE && !NDIS60 */
	/* initialize defaults to simplify code paths below */
	*BytesNeeded = 0;
	*BytesWritten = 0;
	status = NDIS_STATUS_SUCCESS;
	GenericULong = 0;
	GenericLong = 0;
	MoveSource = (PVOID)&GenericULong;
	MoveBytes = sizeof(GenericULong);
	copy_mem = TRUE;
	save_bcmerror = 1;


#ifndef NDIS60
	ND_LOCK(wl);
#endif /* !NDIS60 */

	if (Oid == OID_BCM_GETINFORMATION) {
		ginfo = (getinformation_t *) InfoBuf;
		/* for "set" oids, check for cookie */
		if (InfoBuf == NULL || InfoBufLen < sizeof(ULONG)) {
			status = NDIS_STATUS_INVALID_LENGTH;
			goto response;
		} else if (ginfo->cookie != OIDENCAP_COOKIE) {
			/* placate WHQL by succeeding req, but do nothing */
			MoveBytes = InfoBufLen;
			copy_mem = FALSE;
			goto response;
		} else {
			/* got cookie; skip past it */
			((char *)InfoBuf) += GETINFORMATION_SIZE;
			InfoBufLen -= GETINFORMATION_SIZE;
			Oid = (NDIS_OID) ginfo->oid;
		}
	} else if (Oid == OID_BCM_SETINFORMATION) {
		setinformation_t *sinfo = (setinformation_t *) InfoBuf;

		/* for "set" oids, check for cookie */
		if (InfoBuf == NULL || InfoBufLen < sizeof(ULONG)) {
			status = NDIS_STATUS_INVALID_LENGTH;
			goto response;
		} else if (sinfo->cookie != OIDENCAP_COOKIE) {
			/* placate WHQL by succeeding req, but do nothing */
			/* SetBytes = InfoBufLen; */
			goto response;
		} else {
			if ((NDIS_OID) sinfo->oid >= OID_DHD_IOCTLS) {
				/* got cookie; skip past it */
				((char *)InfoBuf) += SETINFORMATION_SIZE;
				InfoBufLen -= SETINFORMATION_SIZE;
				Oid = (NDIS_OID) sinfo->oid;
				set = TRUE;
			}
			else if ((NDIS_OID) sinfo->oid == (WL_OID_BASE+WLC_SET_VAR)) {
				/* check for and handle ndis specific iovars */
				((char *)InfoBuf) += SETINFORMATION_SIZE;
				InfoBufLen -= SETINFORMATION_SIZE;
				if (wl_set_ndis_iovar(wl, InfoBuf, InfoBufLen, &status)) {
					MoveBytes = InfoBufLen;
					copy_mem = FALSE;
					goto response;
				}
				else {
					((char *)InfoBuf) -= SETINFORMATION_SIZE;
					InfoBufLen += SETINFORMATION_SIZE;
				}
			}

		}
	}

	WL_OID(("OID: querying %s buflen %d instance %d\n",
		wl_oid2str(Oid), InfoBufLen, P2PUNITNUMBER()));

	if (Oid >= OID_DHD_IOCTLS) {
		status = dhd_bus_ioctl(wl, Oid - OID_DHD_IOCTLS, InfoBuf, InfoBufLen,
			BytesWritten, BytesNeeded, set);

		if (status == NDIS_STATUS_SUCCESS) {
			/* wl_query_oid() already copied */
			MoveBytes = InfoBufLen;
			copy_mem = FALSE;
		} else {
			/* wl_query_oid() already checked */
			MoveBytes = InfoBufLen;
		}
		goto response;
	}

	switch (Oid) {

#ifdef EXT_STA
	case OID_GEN_CURRENT_PACKET_FILTER:
		GenericULong = wl->PacketFilter;
		break;
#endif

	case OID_GEN_SUPPORTED_LIST:
		MoveBytes = sizeof(wl_supported_oids);
		if (InfoBufLen >= MoveBytes) {
			NdisMoveMemory(InfoBuf, wl_supported_oids, sizeof(wl_supported_oids));
		}
		copy_mem = FALSE;
		break;

	case OID_GEN_HARDWARE_STATUS:
		GenericULong = NdisHardwareStatusReady;
		break;

#ifndef EXT_STA
	case OID_GEN_MEDIA_SUPPORTED:
	case OID_GEN_MEDIA_IN_USE:
		GenericULong = NdisMedium802_3;
		break;

#ifdef OID_GEN_PHYSICAL_MEDIUM
	case OID_GEN_PHYSICAL_MEDIUM:
		GenericULong = NdisPhysicalMediumWirelessLan;
		break;
#endif /* OID_GEN_PHYSICAL_MEDIUM */
#else /* EXT_STA */
	case OID_GEN_MEDIA_SUPPORTED:
	case OID_GEN_MEDIA_IN_USE:
		GenericULong = NdisMediumNative802_11;
		break;

#ifdef OID_GEN_PHYSICAL_MEDIUM
	case OID_GEN_PHYSICAL_MEDIUM:
		if (AP_ENAB(wlc))
			GenericULong = NdisPhysicalMediumUnspecified;
		else
			GenericULong = NdisPhysicalMediumNative802_11;
		break;
#endif /* OID_GEN_PHYSICAL_MEDIUM */
#endif /* EXT_STA */

	case OID_GEN_CURRENT_LOOKAHEAD:
	case OID_GEN_MAXIMUM_FRAME_SIZE:
		GenericULong = ETHER_MAX_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN;
		break;

	case OID_GEN_MAXIMUM_TOTAL_SIZE:
		GenericULong = ETHER_MAX_LEN - ETHER_CRC_LEN;
		break;

	case OID_GEN_LINK_SPEED:
		/* 100Mbps - 1000000; 10Mbps - 100000; 24Kbps - 240,
		 * wlc_get_reprate() returns a value in 500s of kbps
		 */
#ifdef DUMMYOID
		/* Conning NDIS here.. as we don't want to get the rates
		 * from dongle for IEEE PS mode and unnecessarily wake up
		 * the dongle.
		 */
		GenericULong = 108; /* 540000/5000 */
#else
		if (wl_ioctl(wl, WLC_GET_RATE, &GenericULong, sizeof(GenericULong)))
			status = NDIS_STATUS_INVALID_DATA;
#endif /* DUMMYOID */
		if (wl->abrateconvert)
			wl_abrate_convert(wl, &GenericULong);
		GenericULong *= 5000;
		WL_OID(("OID: \tOID_GEN_LINK_SPEED returns: %d (%d%s Mbps)\n",
			GenericULong, GenericULong/10000, ((GenericULong % 10000) ? ".5" : "")));

		break;

	case OID_GEN_TRANSMIT_BUFFER_SPACE:
		break;

	case OID_GEN_RECEIVE_BUFFER_SPACE:
		GenericULong = wl->nrxbuf * LBUFSZ;
		break;

	case OID_GEN_TRANSMIT_BLOCK_SIZE:
		break;

	case OID_GEN_MAXIMUM_LOOKAHEAD:
	case OID_GEN_RECEIVE_BLOCK_SIZE:
		GenericULong = LBUFSZ;
		break;

	case OID_GEN_VENDOR_DESCRIPTION:
		/* VEN_DRIVER_DESCRIPTION is defined in vendor.h */
		MoveSource = (PVOID) wl->DriverDesc;
		MoveBytes = strlen(wl->DriverDesc) + 1;
		break;

	case OID_GEN_VENDOR_DRIVER_VERSION:
		GenericULong = EPI_VERSION_NUM;
		break;

	case OID_GEN_DRIVER_VERSION:
		GenericArray[0] = MINOR_NDIS_VERSION;
		GenericArray[1] = MAJOR_NDIS_VERSION;
		MoveSource = (PVOID)GenericArray;
		MoveBytes = 2;
		break;

	case OID_GEN_MAC_OPTIONS:
		GenericULong = NDIS_MAC_OPTION_TRANSFERS_NOT_PEND
		        | NDIS_MAC_OPTION_COPY_LOOKAHEAD_DATA
		        | NDIS_MAC_OPTION_RECEIVE_SERIALIZED
		        | NDIS_MAC_OPTION_NO_LOOPBACK
		        | NDIS_MAC_OPTION_8021P_PRIORITY;
		break;

	case OID_GEN_MEDIA_CONNECT_STATUS:
#ifndef EXT_STA
		if (wl->link) {
			GenericULong = NdisMediaStateConnected;
			WL_OID(("wl%d: OID: \tOID_GEN_MEDIA_CONNECT_STATUS returns Connected\n",
				wl->sh.unit));
		} else {
			GenericULong = NdisMediaStateDisconnected;
			WL_OID(("wl%d: OID: \tOID_GEN_MEDIA_CONNECT_STATUS returns Disconnected\n",
				wl->sh.unit));
		}
#else
		GenericULong = NdisMediaStateConnected;
		WL_OID(("wl%d: OID: \tOID_GEN_MEDIA_CONNECT_STATUS returns Connected\n",
			wl->sh.unit));
#endif /* EXT_STA */
		break;

	case OID_GEN_MAXIMUM_SEND_PACKETS:
		/* Number of packets we can receive per SendPackets() call */
		GenericULong = 8;
		break;

	case OID_GEN_XMIT_OK:
	case OID_GEN_RCV_OK:
	case OID_GEN_XMIT_ERROR:
	case OID_GEN_RCV_ERROR:
		GenericULong = wl_getstat(wl, Oid);
		WL_OID(("OID: \t%s returns: %d\n", wl_oid2str(Oid), GenericULong));
		break;

	case OID_GEN_RCV_NO_BUFFER:
	case OID_802_3_RCV_ERROR_ALIGNMENT:
	case OID_802_3_XMIT_ONE_COLLISION:
	case OID_802_3_XMIT_MORE_COLLISIONS:
		break;

	case OID_802_3_MAXIMUM_LIST_SIZE:
		/* multicast support */
		GenericULong = MAXMULTILIST;
		break;

	case OID_802_3_MAC_OPTIONS:
		MoveBytes = 0;
		status = NDIS_STATUS_NOT_SUPPORTED;
		break;

#ifdef STA
#ifdef EXT_STA
	case OID_DOT11_OPERATION_MODE_CAPABILITY:	/* 0x0D010307 */
	{
		DOT11_OPERATION_MODE_CAPABILITY *cap = (DOT11_OPERATION_MODE_CAPABILITY *)InfoBuf;

		MoveBytes = sizeof(DOT11_OPERATION_MODE_CAPABILITY);
		if (MoveBytes > InfoBufLen)
			break;

		cap->uMajorVersion = NWF_VERSION;
		cap->uMinorVersion = 0;

		cap->uNumOfTXBuffers = wl->ntxbuf;
		cap->uNumOfRXBuffers = wl->nrxbuf;

		cap->uOpModeCapability = DOT11_OPERATION_MODE_EXTENSIBLE_STATION |
		                         DOT11_OPERATION_MODE_NETWORK_MONITOR;

		copy_mem = FALSE;
		break;
	}
#endif /* EXT_STA */
#endif /* STA */

#if defined(WDM)
	case OID_GEN_SUPPORTED_GUIDS:
		/* support an empty list for WHQL? */
		MoveSource = (PUCHAR) &GuidList;
		MoveBytes = sizeof(GuidList);
		break;

	case OID_PNP_CAPABILITIES:
		WL_INFORM(("wl%d: OID_PNP_CAPABILITIES called\n", wl->sh.unit));
		copy_mem = FALSE;
		MoveBytes = sizeof(NDIS_PNP_CAPABILITIES);
		if (InfoBufLen >= MoveBytes) {
			PNDIS_PNP_CAPABILITIES pmcap = (PNDIS_PNP_CAPABILITIES) InfoBuf;
			/* If hardware is capable and sw has not overriden it */
			if (wl->wowl_cap && wl->wowl_cap_override != 0) {
				pmcap->Flags = NDIS_DEVICE_WAKE_UP_ENABLE;
				pmcap->WakeUpCapabilities.MinMagicPacketWakeUp = \
				        (wl->wowl_cap_override & WL_WOWL_MAGIC)? \
				        NdisDeviceStateD3: \
				        NdisDeviceStateUnspecified;
				pmcap->WakeUpCapabilities.MinPatternWakeUp = \
				        (wl->wowl_cap_override & WL_WOWL_NET)? \
				        NdisDeviceStateD3: \
				        NdisDeviceStateUnspecified;
			} else {
				pmcap->WakeUpCapabilities.MinMagicPacketWakeUp = \
							 NdisDeviceStateUnspecified;
				pmcap->WakeUpCapabilities.MinPatternWakeUp = \
						     NdisDeviceStateUnspecified;
				pmcap->WakeUpCapabilities.MinLinkChangeWakeUp = \
							NdisDeviceStateUnspecified;
			}
		}
		break;

	case OID_PNP_QUERY_POWER:
		/* We support PM - must merely return success from this call */
		copy_mem = FALSE;
		break;

	case OID_PNP_ENABLE_WAKE_UP:
		if (wl->wowl_cap)
			GenericULong = wl->wakeup_flags;
		else
			GenericULong = 0;
		break;

#ifdef TOE
	/* Task Offload */
	case OID_TCP_TASK_OFFLOAD:
	{
		WL_INFORM(("wl%d: Query Offloading.\n", wl->sh.unit));

		/*
		 * Calculate the information buffer length we need to write the
		 * offload capabilities
		 */
		MoveBytes = sizeof(NDIS_TASK_OFFLOAD_HEADER) +
			FIELD_OFFSET(NDIS_TASK_OFFLOAD, TaskBuffer) +
			sizeof(NDIS_TASK_TCP_IP_CHECKSUM);

		if (MoveBytes > InfoBufLen)
			break;

		/*
		 * check version and Encapsulation Type.  If no capabilities
		 * supported, OffsetFirstTask should be set to 0.
		 */
		pNdisTaskOffloadHdr = (PNDIS_TASK_OFFLOAD_HEADER)InfoBuf;

		/* Assume the miniport only supports IEEE_802_3_Encapsulation type */
		if (pNdisTaskOffloadHdr->EncapsulationFormat.Encapsulation !=
			IEEE_802_3_Encapsulation) {
			WL_ERROR(("wl%d: Encapsulation  type is not supported.\n", wl->sh.unit));
			pNdisTaskOffloadHdr->OffsetFirstTask = 0;
			status = NDIS_STATUS_NOT_SUPPORTED;
			break;
		}

		/* Assume the miniport only supports task version of NDIS_TASK_OFFLOAD_VERSION */
		if (pNdisTaskOffloadHdr->Size != sizeof(NDIS_TASK_OFFLOAD_HEADER) ||
			pNdisTaskOffloadHdr->Version != NDIS_TASK_OFFLOAD_VERSION) {
			WL_ERROR(("wl%d: Size or Version is not correct.\n", wl->sh.unit));
			pNdisTaskOffloadHdr->OffsetFirstTask = 0;
			status = NDIS_STATUS_NOT_SUPPORTED;
			break;
		}

		/* Currently we support TCP/IP checksum , so set
		 * OffsetFirstTask to indicate the offset of the first
		 * offload task.
		 */
		pNdisTaskOffloadHdr->OffsetFirstTask = pNdisTaskOffloadHdr->Size;

		/* Fill TCP/IP checksum offload structures */
		pTaskOffload = (PNDIS_TASK_OFFLOAD)((PUCHAR)(InfoBuf) +
			pNdisTaskOffloadHdr->Size);
		/* Fill all the offload capabilities the miniport supports. */
		for (i = 0; i < OffloadTasksCount; i++) {
			pTaskOffload->Version = OffloadTasks[i].Version;
			pTaskOffload->Size = OffloadTasks[i].Size;
			pTaskOffload->Task = OffloadTasks[i].Task;
			pTaskOffload->TaskBufferLength = OffloadTasks[i].TaskBufferLength;

			/* Not the last task */
			if (i != OffloadTasksCount - 1)
				pTaskOffload->OffsetNextTask =
				FIELD_OFFSET(NDIS_TASK_OFFLOAD, TaskBuffer) +
				pTaskOffload->TaskBufferLength;
			else
				pTaskOffload->OffsetNextTask = 0;

			/* TCP/IP checksum task offload */
			if (OffloadTasks[i].Task == TcpIpChecksumNdisTask) {
				pTcpIpChecksumTask =
					(PNDIS_TASK_TCP_IP_CHECKSUM) pTaskOffload->TaskBuffer;
				NdisMoveMemory(pTcpIpChecksumTask,
					&TcpIpChecksumTask,
					sizeof(TcpIpChecksumTask));
			}
			/* Points to the next task offload */
			if (i != OffloadTasksCount)
				pTaskOffload = (PNDIS_TASK_OFFLOAD)
					((PUCHAR)pTaskOffload + pTaskOffload->OffsetNextTask);
		}

		WL_INFORM(("wl%d: Offloading is set.\n", wl->sh.unit));
		copy_mem = FALSE;
		break;
	}
#else
	case OID_TCP_TASK_OFFLOAD:
		MoveBytes = 0;
		status = NDIS_STATUS_NOT_SUPPORTED;
		break;
#endif /* TOE */

#endif /* WDM */

	case OID_WL_GETINSTANCE:
		MoveBytes = sizeof(GenericULong);
		/* Always stuff the instance number into the beginning of the buffer. */
		status = wl_queryoid(wl, Oid, InfoBuf, InfoBufLen, BytesWritten,
			BytesNeeded, P2PUNITNUMBER());
		/* Brand string set, but insufficient buffer, must be a legacy app. */
		if ('\0' != wl->Brand[0] && InfoBufLen < sizeof(wl_instance_info_t)) {
			MoveBytes = sizeof(wl_instance_info_t);
			status = NDIS_STATUS_INVALID_LENGTH;
			}
		/* Sufficient buffer; copy in the brand string. OK if it is empty */
		else if (sizeof(wl_instance_info_t) <= InfoBufLen)	{
			MoveBytes = sizeof(wl_instance_info_t);
			strncpy(((wl_instance_info_t*)InfoBuf)->brand, wl->Brand, WL_BRAND_MAX);
			((wl_instance_info_t*)InfoBuf)->brand[WL_BRAND_MAX-1] = 0;
			}
		/* No else. Nothing to do if If 0 == wl->Brand[0] and InfoBufLen ==
		 * sizeof(GenericULong).
		 */
		copy_mem = FALSE;
		break;

#ifndef EXT_STA
	case OID_802_11_BSSID_LIST:
		WL_ISCAN(("OID_802_11_BSSID_LIST\n"));
		if (Oid == OID_802_11_BSSID_LIST) {
			MoveBytes = WLC_IOCTL_MAXLEN;

			/* Scan results are held in 8K buffer, if NDIS sends buffer length 
			   less than 8K all the scan result will not make it up to WZC. So
			   force the NDIS layer to send the buffer with length of at least 8K
			*/
			if (InfoBufLen < WLC_IOCTL_MAXLEN)
				break;
		}
#if defined(WL_IW_USE_ISCAN)
		wl_iscan_get_scan(wl, InfoBuf, InfoBufLen, BytesWritten, BytesNeeded);
		copy_mem = FALSE;
		break;
#endif /* WL_IW_USE_ISCAN */
#endif /* !EXT_STA */

#if defined(EXT_STA)
#if defined(WL_IW_USE_ISCAN)
	case OID_DOT11_ENUM_BSS_LIST:
		if ((status = wl_iscan_get_scan(wl, InfoBuf, InfoBufLen, BytesWritten,
			BytesNeeded)) == NDIS_STATUS_SUCCESS) {
			copy_mem = FALSE;
			MoveBytes = *BytesWritten;
		} else {
			MoveBytes = *BytesNeeded;
		}
		break;
#endif /* WL_IW_USE_ISCAN */

#ifdef SIMPLE_ISCAN
	case OID_DOT11_ENUM_BSS_LIST:
		if ((status = wl_iscan_get_final_result(wl, InfoBuf, InfoBufLen,
			BytesWritten, BytesNeeded)) == NDIS_STATUS_SUCCESS) {
			copy_mem = FALSE;
			MoveBytes = *BytesWritten;
		} else {
			MoveBytes = *BytesNeeded;
		}
		break;
#endif /* SIMPLE_ISCAN */
#endif 

	case (WL_OID_BASE+WLC_GET_VAR):
		/* Intercept and handle ndis specific iovars */
		if (!strncmp((char *)InfoBuf, "wsec_authorization", InfoBufLen)) {
			GenericULong = wl->wsec_authorization_status;
			break;
		}
		else if (!strncmp((char *)InfoBuf, "ndis_unblock_8021x", InfoBufLen)) {
#ifdef BCMSUP_PSK
			GenericULong = wl->unblock_8021x;
#else
			status = NDIS_STATUS_FAILURE;
#endif
			break;
		}
		else if (!strncmp((char *)InfoBuf, "abrateconvert", InfoBufLen)) {
			GenericULong = wl->abrateconvert;
			break;
		}
#ifdef EXT_STA
		else if (!strncmp((char *)InfoBuf, "set_pmk", InfoBufLen)) {
			MoveSource = &wl->psk_info;
			MoveBytes = sizeof(wsec_pmk_t);
			break;
		}
		else if (!strncmp((char *)InfoBuf, "next_wsec", InfoBufLen)) {
			MoveSource = &wl->next_wsec;
			MoveBytes = sizeof(uint32);
			break;
		}
#endif /* EXT_STA */
#ifdef EXT_STA
	case OID_DOT11_ENUM_ASSOCIATION_INFO:
		if (Oid == OID_DOT11_ENUM_ASSOCIATION_INFO) {
		ULONG bytesneeded, bytesused;

		/* Sync num_peers from host to dongle */
		wl_setoid(wl, (NDIS_OID)OID_STA_SET_NUM_PEERS,
			(PVOID)&wl->oid->num_peers, (ULONG)sizeof(wl->oid->num_peers),
			&bytesused, &bytesneeded, 0);
	}
#endif /* !EXT_STA */
	/* else drop down to default */

	default:
		save_bcmerror = 0;
		if (!wl_query_legacy_oid(wl, Oid, InfoBuf, InfoBufLen, BytesWritten, &status)) {
			status = wl_queryoid(wl, Oid, InfoBuf, InfoBufLen, BytesWritten,
				BytesNeeded, wlif);
			/* in case apps use ioctl directly to get link speed */
			if (wl->abrateconvert && Oid == (WL_OID_BASE + WLC_GET_RATE))
				wl_abrate_convert(wl, InfoBuf);
		}

#if defined(EXT_STA)
		status = wl_oid_post_process(MiniportAdapterContext, Oid, InfoBuf, 0,
			InfoBufLen, BytesWritten, NULL, BytesNeeded, NULL, status);
#endif /* BCMDONGLEHOST && EXT_STA */

		if (status == NDIS_STATUS_SUCCESS) {
			/* wl_query_oid() already copied */
			MoveBytes = *BytesWritten;
			copy_mem = FALSE;
		} else {
			/* wl_query_oid() already checked */
			WL_OID(("OID: FAILED querying %s !!!\n", wl_oid2str(Oid)));
			MoveBytes = InfoBufLen;
		}
		break;
	}

response:
	/* account for encapsulation, if any */
	if (ginfo) {
		MoveBytes += GETINFORMATION_SIZE;
		InfoBufLen += GETINFORMATION_SIZE;
	}

#if defined(NDIS60)
	if (Oid != OID_DOT11_ENUM_BSS_LIST) {
#endif /* NDIS60 */
	if ((MoveBytes > InfoBufLen)) {
		/* QUERY_GLOBAL_STATS uses BytesWritten to indicate required buffer length */
		/* TrayApp and wl.exe use QUERY_GLOBAL_STATS so we can't zero BytesWritten */
#if defined(NDIS60)
		*BytesWritten = 0;
#else
		*BytesWritten = MoveBytes;
#endif /* UNDER_CE && NDIS60 */
		*BytesNeeded = MoveBytes;

		status = NDIS_STATUS_INVALID_LENGTH;
	}
#if defined(NDIS60)
	}
#endif /* NDIS60 */

	if (status == NDIS_STATUS_SUCCESS) {
		if (copy_mem)
			NdisMoveMemory(InfoBuf, MoveSource,
				(ginfo ? MoveBytes - GETINFORMATION_SIZE : MoveBytes));
		*BytesWritten = MoveBytes;
	}

#ifndef NDIS60
	ND_UNLOCK(wl);
#endif /* !NDIS60 */

	/* check if the ndis error needs to be mapped to bcmerror */
	if ((save_bcmerror) && (status != NDIS_STATUS_SUCCESS)) {
		int32 bcmerror = ndisstatus2bcmerror(status);
		wl_iovar_setint(wl, "bcmerror", bcmerror);
	}

	return (status);
}

#ifdef EXT_STA
static void
wl_ind_phy_state(wl_info_t *wl, BOOLEAN hw_state, BOOLEAN sw_state)
{
	DOT11_PHY_STATE_PARAMETERS phy_state;

	WL_INFORM(("wl%d: EXT STA indication: PHY STATE CHANGED (%d/%d)\n", wl->sh.unit,
	    hw_state, sw_state));

	phy_state.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	phy_state.Header.Revision = DOT11_PHY_STATE_PARAMETERS_REVISION_1;
	phy_state.Header.Size = sizeof(DOT11_PHY_STATE_PARAMETERS);

	phy_state.uPhyId = DOT11_PHY_ID_ANY;
	phy_state.bHardwarePhyState = hw_state;
	phy_state.bSoftwarePhyState = sw_state;

#ifdef PRE_v1_69_HEADERS
	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_PHY_STATE_CHANGE,
	    &phy_state, sizeof(phy_state));
#else
	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_PHY_STATE_CHANGED,
	    &phy_state, sizeof(phy_state));
#endif /* PRE_v1_69_HEADERS */
}

static void
wl_write_NPS(PVOID Context, NDIS_HANDLE work_item)
{
	wl_info_t *wl = (wl_info_t *)Context;
	NDIS_CONFIGURATION_OBJECT configobj;
	NDIS_HANDLE confighandle;
	NDIS_STATUS status;

	/* free the work item */
	NdisFreeIoWorkItem(work_item);

	/* preserve software phy state in registry (persist across MiniportHalt/reboot) */

	/* open the registry */
	bzero(&configobj, sizeof(NDIS_CONFIGURATION_OBJECT));

	configobj.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
	configobj.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
	configobj.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT);

	configobj.NdisHandle = wl->sh.adapterhandle;
	configobj.Flags = 0;

	status = NdisOpenConfigurationEx(&configobj, &confighandle);
	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: NICPowerState: error opening registry 0x%x\n",
		    wl->sh.unit, status));
	}

	if (confighandle) {
		NDIS_CONFIGURATION_PARAMETER writeparam;
		NDIS_STRING Keyword = NDIS_STRING_CONST("NICPowerState");

		/* save sw_phy_state in registry */
		/* N.B.: Callers of NdisWriteConfiguration() must run at PASSIVE_LEVEL */

		bzero(&writeparam, sizeof(NDIS_CONFIGURATION_PARAMETER));
		writeparam.ParameterType = NdisParameterInteger;
		writeparam.ParameterData.IntegerData = wl->oid->sw_phy_state;

		NdisWriteConfiguration(&status, confighandle, &Keyword, &writeparam);
		if (NDIS_ERROR(status)) {
			WL_ERROR(("wl%d: NICPowerState: error writing registry 0x%x\n",
			    wl->sh.unit, status));
		}

		/* close the registry */
		NdisCloseConfiguration(confighandle);
	}
}

#ifdef NDIS60
static void
wl_ind_link_state(wl_info_t *wl, ULONG link_speed)
{
	NDIS_LINK_STATE sbuf;

	WL_INFORM(("wl%d: EXT STA indication: LINK STATE\n", wl->sh.unit));

	/* report link speed */
	sbuf.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	sbuf.Header.Revision = NDIS_LINK_STATE_REVISION_1;
	sbuf.Header.Size = sizeof(NDIS_LINK_STATE);

	sbuf.MediaConnectState = MediaConnectStateConnected;
	sbuf.MediaDuplexState = MediaDuplexStateFull;

	sbuf.RcvLinkSpeed = link_speed;
	sbuf.XmitLinkSpeed = link_speed;

	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_LINK_STATE,
	    &sbuf, sizeof(sbuf));
}
#endif /* NDIS60 */
#endif /* EXT_STA */



#ifdef P2P
static BOOL
pass2VDev(NDIS_OID Oid,
	PVOID InfoBuf,
	ULONG InfoBufLen)
{
	if ((Oid - WL_OID_BASE) == WLC_SET_WSEC ||
		(Oid - WL_OID_BASE) == WLC_SET_WSEC_PMK ||
		(Oid - WL_OID_BASE) == WLC_SET_WPA_AUTH ||
		(Oid - WL_OID_BASE) == WLC_SET_KEY ||
		(Oid - WL_OID_BASE) == WLC_SET_AUTH ||
		(Oid - WL_OID_BASE) == WLC_SET_KEY_PRIMARY)
	{
		return TRUE;

	} else if ((Oid - WL_OID_BASE) == WLC_SET_VAR) {
		if (!strncmp((char *)InfoBuf, "wpa_auth", InfoBufLen)) {
			return TRUE;
		} else if (!strncmp((char *)InfoBuf, "wsec_key", InfoBufLen)) {
			return TRUE;
		}
	}

	return FALSE;
}
#endif /* P2P */

#ifdef __BBSOFT__
static BOOL
_wl_ssid_is_valid(const NDIS_802_11_SSID *pSSID)
//
//  WZC will set the SSID to a random string of 32 invalid
//  characters in the range 0x01 - 0x1F in order to stop
//  an adapter from associating with any SSID.
//
//  Return false if the SSID is one of these special values
//  being set by WZC.
{
	BOOL isValid = TRUE;
	int i;

	if (pSSID->SsidLength == 32)	 {
		isValid = FALSE;
		for (i = 0; i < 32; i++)
			if (!(0 < pSSID->Ssid[i] && pSSID->Ssid[i] <= 0x1F)) {
				isValid = TRUE;
				break;
			}
	}
	
	return isValid;
}
#endif

NDIS_STATUS
wl_msetinformation(
	NDIS_HANDLE MiniportAdapterContext,
	NDIS_OID Oid,
	PVOID InfoBuf,
	ULONG InfoBufLen,
	PULONG BytesRead,
	PULONG BytesNeeded
)
{
	wl_info_t *wl;
	NDIS_STATUS status;
	ULONG filter;
	ULONG SetBytes;
	int save_bcmerror;
#ifdef EXT_STA
	uint i;
#endif
#ifdef TOE
	PNDIS_TASK_OFFLOAD_HEADER   pNdisTaskOffloadHdr;
	PNDIS_TASK_OFFLOAD          TaskOffload;
	PNDIS_TASK_OFFLOAD          TmpOffload;
	PNDIS_TASK_TCP_IP_CHECKSUM  pTcpIpChecksumTask;
#endif	/* TOE */
	wl = WL_INFO(MiniportAdapterContext);

	if (wl == NULL)
	    return NDIS_STATUS_NOT_SUPPORTED;

#ifdef P2P
	if (P2PUNITNUMBER() && pass2VDev(Oid, InfoBuf, InfoBufLen)) {
		wl = gP2P.wl_instance_wl;
		ASSERT(wl != NULL);
		wl->isVDev = TRUE;
		status = wl_setoid(wl, Oid - WL_OID_BASE, InfoBuf,
			InfoBufLen, BytesRead, BytesNeeded, 1);
		goto response;
	} else {
		wl->isVDev = FALSE;
	}
#endif /* P2P */

	WL_TRACE(("wl%d: wl_msetinformation: oid 0x%x\n", wl->sh.unit, Oid));

	status = NDIS_STATUS_SUCCESS;
	*BytesNeeded = 0;
	SetBytes = sizeof(ULONG);
	save_bcmerror = 1;


#ifndef NDIS60
	ND_LOCK(wl);
#endif /* !NDIS60 */

	if (Oid == OID_BCM_SETINFORMATION) {
		setinformation_t *sinfo = (setinformation_t *) InfoBuf;

		/* for "set" oids, check for cookie */
		if (InfoBuf == NULL || InfoBufLen < sizeof(ULONG)) {
			status = NDIS_STATUS_INVALID_LENGTH;
			goto response;
		} else if (sinfo->cookie != OIDENCAP_COOKIE) {
			/* placate WHQL by succeeding req, but do nothing */
			SetBytes = InfoBufLen;
			goto response;
		} else {
			/* got cookie; skip past it */
			((char *)InfoBuf) += SETINFORMATION_SIZE;
			InfoBufLen -= SETINFORMATION_SIZE;
			Oid = (NDIS_OID) sinfo->oid;
		}
	}

	WL_OID(("OID: setting %s buflen %d instance %d\n",
		wl_oid2str(Oid), InfoBufLen, P2PUNITNUMBER()));

	if (Oid >= OID_DHD_IOCTLS) {
		printf("%s: Oid >= OID_DHD_IOCTLS\n", __FUNCTION__);
		status = dhd_bus_ioctl(wl, Oid - OID_DHD_IOCTLS, InfoBuf, InfoBufLen,
			BytesRead, BytesNeeded, TRUE);

		goto response;
	}

	switch (Oid) {
	case OID_GEN_CURRENT_PACKET_FILTER:
		if (InfoBufLen != sizeof(ULONG)) {
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}
		NdisMoveMemory(&filter, InfoBuf, sizeof(ULONG));
		if (filter & NDIS_PACKET_TYPE_PROMISCUOUS) {
			status = NDIS_STATUS_NOT_SUPPORTED;
			break;
		}

#ifndef EXT_STA
		if (filter & (NDIS_PACKET_TYPE_SOURCE_ROUTING |
			NDIS_PACKET_TYPE_SMT |
			NDIS_PACKET_TYPE_MAC_FRAME |
			NDIS_PACKET_TYPE_FUNCTIONAL |
			NDIS_PACKET_TYPE_ALL_FUNCTIONAL |
			NDIS_PACKET_TYPE_GROUP)) {
			status = NDIS_STATUS_NOT_SUPPORTED;
			break;
		}

#else
		if (filter & ~NDIS_PACKET_TYPE_ALL_802_11_FILTERS) {
			status = NDIS_STATUS_NOT_SUPPORTED;
			break;
		}

		{
		bool sendup_mgmt = (filter & (NDIS_PACKET_TYPE_802_11_DIRECTED_MGMT | \
		NDIS_PACKET_TYPE_802_11_BROADCAST_MGMT | \
		NDIS_PACKET_TYPE_802_11_MULTICAST_MGMT | \
		NDIS_PACKET_TYPE_802_11_ALL_MULTICAST_MGMT | \
		NDIS_PACKET_TYPE_802_11_PROMISCUOUS_MGMT | \
		NDIS_PACKET_TYPE_802_11_RAW_MGMT | \
		NDIS_PACKET_TYPE_802_11_DIRECTED_CTRL | \
		NDIS_PACKET_TYPE_802_11_BROADCAST_CTRL | \
		NDIS_PACKET_TYPE_802_11_PROMISCUOUS_CTRL)) ? 1 : 0;

		wl_iovar_setint(wl, "sendup_mgmt", sendup_mgmt);

		/*
		 * Save the new packet filter value
		 */
		wl->PacketFilter = filter;
		}
#endif /* !EXT_STA */

		/* process PROMISCUOUS */
		if (!AP_ENAB(wl->pub)) {
			int promisc = (filter & NDIS_PACKET_TYPE_PROMISCUOUS) ? 1 : 0;
			wl_ioctl(wl, WLC_SET_PROMISC, &promisc, sizeof(promisc));
		}

		/* update ALL_MULTICAST common code flag */
		wl_iovar_setint(wl, "allmulti", ((filter & NDIS_PACKET_TYPE_ALL_MULTICAST) != 0));

		break;

	case OID_GEN_PROTOCOL_OPTIONS:
	case OID_GEN_CURRENT_LOOKAHEAD:
		/* verify the length */
		if (InfoBufLen != sizeof(ULONG))
			status = NDIS_STATUS_INVALID_LENGTH;
		break;

	case OID_GEN_MACHINE_NAME:
	{
		uint8 name[WLC_IOCTL_SMLEN];
		int len;

		len = wchar2ascii(name, (ushort *)InfoBuf, (ushort)InfoBufLen, sizeof(name));

		/* setting station name to driver */
		wl_iovar_op(wl, "staname", name, len, IOV_SET);
		break;
	}

	case OID_802_3_MULTICAST_LIST:
		if (((InfoBufLen % ETHER_ADDR_LEN) != 0) ||
		    (InfoBufLen > (MAXMULTILIST * ETHER_ADDR_LEN))) {
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}
		else {
			int nmulticast = InfoBufLen / ETHER_ADDR_LEN;
			int buflen = sizeof(struct maclist) + InfoBufLen;
			struct maclist *maclist;

			if ((maclist = MALLOC(wl->sh.osh, buflen)) == NULL) {
				status = NDIS_STATUS_FAILURE;
				break;
			}
			maclist->count = nmulticast;
			bcopy(InfoBuf, &maclist->ea[0], InfoBufLen);
			wl_iovar_op(wl, "mcast_list", maclist, buflen, IOV_SET);
			MFREE(wl->sh.osh, maclist, buflen);
		}
		break;

#ifdef STA
#ifndef EXT_STA
		/* 802.11 OIDs */
	case OID_802_11_TEST: {
		NDIS_802_11_TEST *test = (NDIS_802_11_TEST *) InfoBuf;

		SetBytes = sizeof(NDIS_802_11_TEST);
		if (InfoBufLen >= SetBytes)
			SetBytes = test->Length;
		if (InfoBufLen < SetBytes) {
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		if (test->Type == 1) {
			WL_INFORM(("wl%d: NDIS test 1\n", wl->sh.unit));
			NdisMIndicateStatus(wl->sh.adapterhandle,
				NDIS_STATUS_MEDIA_SPECIFIC_INDICATION, &test->AuthenticationEvent,
				test->Length - OFFSETOF(NDIS_802_11_TEST, AuthenticationEvent));
			NdisMIndicateStatusComplete(wl->sh.adapterhandle);
		} else if (test->Type == 2) {
			WL_INFORM(("wl%d: NDIS test 2\n", wl->sh.unit));
			NdisMIndicateStatus(wl->sh.adapterhandle,
				NDIS_STATUS_MEDIA_SPECIFIC_INDICATION, &test->RssiTrigger,
				sizeof(NDIS_802_11_RSSI));
			NdisMIndicateStatusComplete(wl->sh.adapterhandle);
		} else {
			status = NDIS_STATUS_INVALID_DATA;
			break;
		}

		break;
	}
#else /* EXT_STA */
	case OID_DOT11_NIC_POWER_STATE:			/* 0x0D010311 */
	{
		mbool radio;
		uint radiomaskval;
		ULONG i;
		bool state_change;

#ifdef PRE_v1_68_HEADERS
		/* ExtSTA OP: fail with NDIS_STATUS_INVALID_STATE */
		if (wl->oid->opstateOP) {
			status = NDIS_STATUS_INVALID_STATE;
			break;
		}

		/* ExtSTA INIT: if scan pending, fail with NDIS_STATUS_MEDIA_IN_USE */
		{
			ULONG BytesRead, BytesNeeded;

			wl_queryoid(wl, OID_STA_SCAN_PENDING, &wl->oid->scan_pending, \
					sizeof(wl->oid->scan_pending), &BytesRead, &BytesNeeded);
		}
		if (wl->oid->scan_pending) {
			status = NDIS_STATUS_MEDIA_IN_USE;
			break;
		}
#endif /* PRE_v1_68_HEADERS */

		if (InfoBufLen < sizeof(BOOLEAN)) {
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		wl_ioctl(wl, WLC_GET_RADIO, &i, sizeof(i));
		radio = (mbool)(i & (WL_RADIO_HW_DISABLE | WL_RADIO_SW_DISABLE));
		i = 1 << wl->oid->cur_phy_id;
		state_change = FALSE;

		if (*(BOOLEAN *)InfoBuf) {
			if (!mboolisset(wl->oid->sw_phy_state, i)) {
				state_change = TRUE;
				mboolset(wl->oid->sw_phy_state, \
					((1 << wl->oid->phy.num_phy_ids) - 1));
				wl_ind_phy_state(wl, \
				    !mboolisset(radio, WL_RADIO_HW_DISABLE), TRUE);
			}
		} else {
			if (mboolisset(wl->oid->sw_phy_state, i)) {
				state_change = TRUE;
				mboolclr(wl->oid->sw_phy_state, \
					((1 << wl->oid->phy.num_phy_ids) - 1));
				wl_ind_phy_state(wl,
				    !mboolisset(radio, WL_RADIO_HW_DISABLE), FALSE);
			}
		}

		{
			ULONG bytesneeded, bytesused;

			/* Update sw_phy_state in the dongle */
			wl_setoid(wl, (NDIS_OID)OID_STA_SET_PHY_STATE,
				(PVOID)&wl->oid->sw_phy_state, (ULONG)sizeof(wl->oid->sw_phy_state),
				&bytesused, &bytesneeded, 0);
		}

		/* manage hardware radio state */
		radiomaskval = WL_RADIO_SW_DISABLE << 16;
		if (mboolisset(radio, WL_RADIO_SW_DISABLE) && wl->oid->sw_phy_state) {
			/* turn radio on */
			WL_INFORM(("wl%d: NIC_POWER_STATE: turning radio on\n", wl->sh.unit));
			if (wl_ioctl(wl, WLC_SET_RADIO, &radiomaskval,
				sizeof(radiomaskval)) == -1) {
				WL_ERROR(("wl%d: NIC_POWER_STATE: error turning radio on\n",
				    wl->sh.unit));
			}
		} else if (!mboolisset(radio, WL_RADIO_SW_DISABLE) && !wl->oid->sw_phy_state) {
			/* turn radio off */
			WL_INFORM(("wl%d: NIC_POWER_STATE: turning radio off\n", wl->sh.unit));
			radiomaskval += WL_RADIO_SW_DISABLE;
			if (wl_ioctl(wl, WLC_SET_RADIO, &radiomaskval,
				sizeof(radiomaskval)) == -1) {
				WL_ERROR(("wl%d: NIC_POWER_STATE: error turning radio off\n",
				    wl->sh.unit));
			}
		}

		/* preserve software phy state in registry (persist across MiniportHalt/reboot) */
		if (state_change) {
			NDIS_HANDLE work_item;

			work_item = NdisAllocateIoWorkItem(wl->sh.adapterhandle);
			if (work_item == NULL) {
				WL_ERROR(("wl%d: NIC_POWER_STATE: work item allocation error\n",
				    wl->sh.unit));
				break;
			}

			NdisQueueIoWorkItem(work_item, wl_write_NPS, wl);
		}

		break;
	}

	case OID_DOT11_MULTICAST_LIST:			/* 0x0D010704 */
		if ((InfoBufLen % ETHER_ADDR_LEN) != 0) {
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		if (InfoBufLen > (MAXMULTILIST * ETHER_ADDR_LEN)) {
			status = NDIS_STATUS_MULTICAST_FULL;
			break;
		}

		/* copy the list of multicasts into our private table */
		wl->pub->nmulticast = InfoBufLen / ETHER_ADDR_LEN;
		for (i = 0; i < wl->pub->nmulticast; i++)
			wl->pub->multicast[i] = ((struct ether_addr *)InfoBuf)[i];

		break;
#endif /* !EXT_STA */
#endif /* STA */

#if defined(WDM)
	case OID_PNP_SET_POWER: {
		int devicestate;
		ASSERT(InfoBufLen == sizeof(int));

		devicestate = *((int*) InfoBuf);
#ifdef WL_IW_USE_ISCAN
		wl_iscan_set_device_state(wl, devicestate);
#endif /* WL_IW_USE_ISCAN */
#if defined(__BBSOFT__) && defined(__ZELOS__)
		if (devicestate == NdisDeviceStateD0
				&& g_pGPIOReg) { 
			Set_PinFunction(g_pGPIOReg,  WLAN_WAKEUP);
			Set_PinData(g_pGPIOReg,  WLAN_WAKEUP, TRUE);
		}
#endif
		status = wl_pnp_set_power(wl, devicestate);
		
#if defined(__BBSOFT__) && defined(__ZELOS__)
		if (devicestate == NdisDeviceStateD3
				&& status == NDIS_STATUS_SUCCESS
				&& g_pGPIOReg) {
			Set_PinFunction(g_pGPIOReg,  WLAN_WAKEUP);
			Set_PinData(g_pGPIOReg,  WLAN_WAKEUP, FALSE);
		}
#endif
		break;
	}

	case OID_PNP_ENABLE_WAKE_UP:
		if (InfoBufLen != sizeof(ULONG)) {
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		/* Setup Wakeup flags */
		if (wl->wowl_cap) {
			uint16 wakeup_flags = 0;
			wl->wakeup_flags = (*(ULONG*)InfoBuf);
			WL_ERROR(("\twakeup_flags 0x%x Magic: %d Link: %d Pattern:%d\n",
			          (*(ULONG*)InfoBuf),
			          wl->wakeup_flags & NDIS_PNP_WAKE_UP_MAGIC_PACKET,
			          wl->wakeup_flags & NDIS_PNP_WAKE_UP_LINK_CHANGE,
			          wl->wakeup_flags & NDIS_PNP_WAKE_UP_PATTERN_MATCH));
			if (wl->wakeup_flags & NDIS_PNP_WAKE_UP_MAGIC_PACKET)
				wakeup_flags |= WL_WOWL_MAGIC;
			if (wl->wakeup_flags & NDIS_PNP_WAKE_UP_LINK_CHANGE)
				wakeup_flags |= (WL_WOWL_DIS | WL_WOWL_BCN);
			if (wl->wakeup_flags & NDIS_PNP_WAKE_UP_PATTERN_MATCH)
				wakeup_flags |= WL_WOWL_NET;

			/* Clear the OS flags that override does not want us to set */
			wakeup_flags &= wl->wowl_cap_override;

			/* Set the flags that override wants us to set */
			wakeup_flags |= wl->wowl_cap_override;

			/* Use a different iovar for settings through OS to keep
			 * user-settings and OS settings separate
			 */
			if (wl_iovar_setint(wl, "wowl_os", wakeup_flags) != 0) {
				wl->wakeup_flags = 0;
				status = NDIS_STATUS_INVALID_DATA;
			}
		} else {
			/* do not support any wake up bits */
			if (*(ULONG*)InfoBuf != 0)
				status = NDIS_STATUS_INVALID_DATA;
		}
		break;

	case OID_PNP_REMOVE_WAKE_UP_PATTERN:
		status = NDIS_STATUS_NOT_SUPPORTED;
		break;
	case OID_PNP_ADD_WAKE_UP_PATTERN:
		status = NDIS_STATUS_NOT_SUPPORTED;
		break;

#ifdef TOE
	case OID_TCP_TASK_OFFLOAD:

		WL_INFORM(("wl%d: Set Offloading.\n", wl->sh.unit));

		if (InfoBufLen < sizeof(NDIS_TASK_OFFLOAD_HEADER)) {
			SetBytes = sizeof(NDIS_TASK_OFFLOAD_HEADER);
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		/* Disable all the existing capabilities whenever task offload is updated */
		NdisZeroMemory(&wl->NicTaskOffload, sizeof(NIC_TASK_OFFLOAD));
		NdisZeroMemory(&wl->NicChecksumOffload, sizeof(NIC_CHECKSUM_OFFLOAD));

		*BytesRead = sizeof(NDIS_TASK_OFFLOAD_HEADER);
		/* Assume miniport only supports IEEE_802_3_Encapsulation
		 * Check to make sure that TCP/IP passed down the correct
		 * encapsulation type
		 */
		pNdisTaskOffloadHdr = (PNDIS_TASK_OFFLOAD_HEADER)InfoBuf;
		if (pNdisTaskOffloadHdr->EncapsulationFormat.Encapsulation !=
			IEEE_802_3_Encapsulation) {
			pNdisTaskOffloadHdr->OffsetFirstTask = 0;
			status = NDIS_STATUS_INVALID_DATA;
			break;
		}
		/* No offload task to be set */
		if (pNdisTaskOffloadHdr->OffsetFirstTask == 0) {
			WL_INFORM(("wl%d: No offload task is set\n", wl->sh.unit));
			status = NDIS_STATUS_SUCCESS;
			break;
		}

		/* OffsetFirstTask is not valid */
		if (pNdisTaskOffloadHdr->OffsetFirstTask < pNdisTaskOffloadHdr->Size) {
			pNdisTaskOffloadHdr->OffsetFirstTask = 0;
			status = NDIS_STATUS_FAILURE;
			break;
		}
		/* The length can't hold one task */
		SetBytes = (pNdisTaskOffloadHdr->OffsetFirstTask + sizeof(NDIS_TASK_OFFLOAD));
		if (InfoBufLen < SetBytes) {
			WL_INFORM(("wl%d: response of task offload does not have sufficient "
			           "space for an offload task\n", wl->sh.unit));
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		/* Copy Encapsulation format into adapter, later the
		 * miniport may use it to get Encapsulation header size
		 */
		NdisMoveMemory(&(wl->EncapsulationFormat),
			&(pNdisTaskOffloadHdr->EncapsulationFormat),
			sizeof(NDIS_ENCAPSULATION_FORMAT));

		ASSERT(pNdisTaskOffloadHdr->EncapsulationFormat.Flags.FixedHeaderSize == 1);

		/* Check to make sure we support the task offload requested */
		TaskOffload = (NDIS_TASK_OFFLOAD *)
			((PUCHAR)pNdisTaskOffloadHdr + pNdisTaskOffloadHdr->OffsetFirstTask);

		TmpOffload = TaskOffload;

		/* Check the task in the buffer and enable the offload capabilities */
		while (TmpOffload) {
			*BytesRead += FIELD_OFFSET(NDIS_TASK_OFFLOAD, TaskBuffer);
			switch (TmpOffload->Task)
			{

			case TcpIpChecksumNdisTask:
			/* Invalid information buffer length */
			SetBytes = *BytesRead + sizeof(NDIS_TASK_TCP_IP_CHECKSUM);
			if (InfoBufLen < SetBytes) {
				status = NDIS_STATUS_INVALID_LENGTH;
				break;
			}
			/* Check version */
			for (i = 0; i < OffloadTasksCount; i++) {
				if (OffloadTasks[i].Task == TmpOffload->Task &&
					OffloadTasks[i].Version == TmpOffload->Version)
					break;
			}
			/* Version is mismatched */
			if (i == OffloadTasksCount) {
				status = NDIS_STATUS_NOT_SUPPORTED;
				break;
			}

			/*
			 * This miniport support TCP/IP checksum offload for tx and rx, TCP
			 * and IP checksum with TCP/IP options.
			 * check if the fields in NDIS_TASK_TCP_IP_CHECKSUM is set correctly
			 */
			wl->NicTaskOffload.ChecksumOffload = 1;
			pTcpIpChecksumTask = (PNDIS_TASK_TCP_IP_CHECKSUM) TmpOffload->TaskBuffer;
			if (pTcpIpChecksumTask->V4Transmit.TcpChecksum) {
				/* If miniport doesn't support sending TCP checksum, we can't enable
				 * this capability
				 */
				if (TcpIpChecksumTask.V4Transmit.TcpChecksum == 0) {
					status = NDIS_STATUS_NOT_SUPPORTED;
					break;
				}

				WL_INFORM(("wl%d: Set Sending TCP offloading.\n", wl->sh.unit));
				/* Enable sending TCP checksum */
				wl->NicChecksumOffload.DoXmitTcpChecksum = 1;
			}
			/* left for receive and other IP and UDP checksum offload */
			if (pTcpIpChecksumTask->V4Transmit.IpChecksum)
			{
				/* If the miniport doesn't support sending IP checksum,
				 * we can't enable this capabilities
				 */
				if (TcpIpChecksumTask.V4Transmit.IpChecksum == 0) {
					status = NDIS_STATUS_NOT_SUPPORTED;
					break;
				}

				WL_INFORM(("wl%d: Set Sending IP offloading.\n", wl->sh.unit));
				/* Enable sending IP checksum */
				wl->NicChecksumOffload.DoXmitIpChecksum = 1;
			}
			if (pTcpIpChecksumTask->V4Receive.TcpChecksum) {
				/* If the miniport doesn't support receiving TCP checksum, we can't
				 * enable this capability
				 */
				if (TcpIpChecksumTask.V4Receive.TcpChecksum == 0) {
					status = NDIS_STATUS_NOT_SUPPORTED;
					break;
				}
				WL_INFORM(("wl%d: Set receive TCP offloading.\n", wl->sh.unit));
				/* Enable receiving TCP checksum */
				wl->NicChecksumOffload.DoRcvTcpChecksum = 1;
			}

			if (pTcpIpChecksumTask->V4Receive.IpChecksum) {
			/* If the miniport doesn't support receiving IP checksum, we can't
			 * enable this capability
			 */
				if (TcpIpChecksumTask.V4Receive.IpChecksum == 0) {
					status = NDIS_STATUS_NOT_SUPPORTED;
					break;
				}
				WL_INFORM(("wl%d: Set Receive IP offloading.\n", wl->sh.unit));
				/* Enable receiving IP checksum */
				wl->NicChecksumOffload.DoRcvIpChecksum = 1;
			}

			if (pTcpIpChecksumTask->V4Transmit.UdpChecksum) {
			/* If the miniport doesn't support sending UDP checksum, we can't
			 * enable this capability
			 */
				if (TcpIpChecksumTask.V4Transmit.UdpChecksum == 0) {
					status = NDIS_STATUS_NOT_SUPPORTED;
					break;
				}

				WL_INFORM(("wl%d: Set Sending UDP offloading.\n", wl->sh.unit));
				/* Enable sending UDP checksum */
				wl->NicChecksumOffload.DoXmitUdpChecksum = 1;
			}

			if (pTcpIpChecksumTask->V4Receive.UdpChecksum) {
				/* IF the miniport doesn't support receiving UDP
				* checksum, we can't enable this capability
				*/
				if (TcpIpChecksumTask.V4Receive.UdpChecksum == 0) {
					status = NDIS_STATUS_NOT_SUPPORTED;
					break;
				}
				WL_INFORM(("wl%d: Set receive UDP offloading.\n", wl->sh.unit));
				/* Enable receiving UDP checksum */
				wl->NicChecksumOffload.DoRcvUdpChecksum = 1;
			}
			/* check for V6 setting, because this miniport doesn't support any of
			* offload for V6, so we just return NDIS_STATUS_NOT_SUPPORTED
			* if the protocol tries to set these capabilities
			*/
			if (pTcpIpChecksumTask->V6Transmit.TcpChecksum ||
				pTcpIpChecksumTask->V6Transmit.UdpChecksum ||
				pTcpIpChecksumTask->V6Receive.TcpChecksum ||
				pTcpIpChecksumTask->V6Receive.UdpChecksum) {
				status = NDIS_STATUS_NOT_SUPPORTED;
					break;
				}

			*BytesRead += sizeof(NDIS_TASK_TCP_IP_CHECKSUM);
			break;

			/*
			 * This miniport doesn't implement TSO, IPSec offload,
			 */
			case TcpLargeSendNdisTask:
			default:
			status = NDIS_STATUS_NOT_SUPPORTED;
			break;
			}
			/* Go on to the next offload structure */
			if (TmpOffload->OffsetNextTask)
				TmpOffload = (PNDIS_TASK_OFFLOAD) ((PUCHAR) TmpOffload +
					TmpOffload->OffsetNextTask);
			else
				TmpOffload = NULL;
		}

		break;
#endif /* TOE */
#endif /* defined(WDM) */

#if defined WL_IW_USE_ISCAN
	case OID_802_11_BSSID_LIST_SCAN:
		WL_ISCAN(("OID_802_11_BSSID_LIST_SCAN\n"));
		wl_iscan_set_scan(wl);
		break;

#if defined(EXT_STA)
	case OID_DOT11_FLUSH_BSS_LIST:
		WL_ISCAN(("OID_DOT11_FLUSH_BSS_LIST:\n"));
		wl_iscan_flush_scan(wl);
		break;
#endif 
#endif /* WL_IW_USE_ISCAN */

#if defined(SIMPLE_ISCAN) && defined(EXT_STA)
	case OID_DOT11_FLUSH_BSS_LIST:
		wl->in_scan = FALSE;
		dhd_iscan_free_buf(wl->dhd, 0);
		break;

	case OID_DOT11_SCAN_REQUEST:
		if (wl->in_scan) {
			WL_ERROR(("OID_DOT11_SCAN_REQUEST -> NDIS_STATUS_DOT11_MEDIA_IN_USE\n"));
			status = NDIS_STATUS_DOT11_MEDIA_IN_USE;
			break;
		}

		dhd_iscan_request(wl->dhd, WL_SCAN_ACTION_START);
		wl->in_scan = TRUE;
		break;
#endif 


#if defined(WL_IW_USE_ISCAN) && defined(EXT_STA)
	case OID_DOT11_SCAN_REQUEST:
	{
		DOT11_SCAN_REQUEST_V2 *scanreq;
		DOT11_SSID *ssids;
		DOT11_BSS_TYPE BSSType;
		DOT11_SCAN_TYPE ScanType;

		if (InfoBufLen < sizeof(DOT11_SCAN_REQUEST_V2)) {
			SetBytes = sizeof(DOT11_SCAN_REQUEST_V2);
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		scanreq = (DOT11_SCAN_REQUEST_V2 *)InfoBuf;

		if (scanreq->uNumOfdot11SSIDs < 1) {
			status = NDIS_STATUS_INVALID_DATA;
			break;
		}

		if (InfoBufLen < (OFFSETOF(DOT11_SCAN_REQUEST_V2, ucBuffer) +
		            scanreq->udot11SSIDsOffset +
		            (scanreq->uNumOfdot11SSIDs * sizeof(DOT11_SSID)))) {
			SetBytes = OFFSETOF(DOT11_SCAN_REQUEST_V2, ucBuffer) +
				scanreq->udot11SSIDsOffset +
				(scanreq->uNumOfdot11SSIDs * sizeof(DOT11_SSID));
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}


		if (scanreq->bUseRequestIE
		    /* WM7 TBD&& (wl->scan_type == DOT11_SCANTYPE_ACTIVE )*/) {
			if (InfoBufLen < (OFFSETOF(DOT11_SCAN_REQUEST_V2, ucBuffer) +
			            scanreq->uRequestIDsOffset + scanreq->uNumOfRequestIDs)) {
				SetBytes = OFFSETOF(DOT11_SCAN_REQUEST_V2, ucBuffer) +
					scanreq->uRequestIDsOffset + scanreq->uNumOfRequestIDs;
				status = NDIS_STATUS_INVALID_LENGTH;
				break;
			}
		}


		if (InfoBufLen < (OFFSETOF(DOT11_SCAN_REQUEST_V2, ucBuffer) + scanreq->uIEsOffset +
		            scanreq->uIEsLength)) {
			SetBytes = OFFSETOF(DOT11_SCAN_REQUEST_V2, ucBuffer) +
				scanreq->uIEsOffset + scanreq->uIEsLength;
			status = NDIS_STATUS_INVALID_LENGTH;
			break;
		}

		/* pending scan not complete:  fail with NDIS_STATUS_MEDIA_IN_USE */
		if (wl_iscan_get_scan_status(wl) != ISCAN_STATE_IDLE) {
			WL_INFORM(("SCAN_REQUEST rejected due to scan in progress...\n"));
#ifdef PRE_v1_69_HEADERS
			status = NDIS_STATUS_MEDIA_IN_USE;
#else
			status = NDIS_STATUS_DOT11_MEDIA_IN_USE;
#endif
			break;
		}

		/* cons up scan parms */

		/* validate BSSType and ScanType before caching... */
		if (scanreq->dot11BSSType == dot11_BSS_type_infrastructure) {
			BSSType = DOT11_BSSTYPE_INFRASTRUCTURE;
		} else if (scanreq->dot11BSSType == dot11_BSS_type_independent) {
			BSSType = DOT11_BSSTYPE_INDEPENDENT;
		} else if (scanreq->dot11BSSType == dot11_BSS_type_any) {
			BSSType = DOT11_BSSTYPE_ANY;
		} else {
			status = NDIS_STATUS_INVALID_DATA;
			break;
		}

		if ((scanreq->dot11ScanType & (~dot11_scan_type_forced)) ==
			dot11_scan_type_active) {
			ScanType = DOT11_SCANTYPE_ACTIVE;
		} else if ((scanreq->dot11ScanType & (~dot11_scan_type_forced)) ==
		            dot11_scan_type_passive) {
			ScanType = DOT11_SCANTYPE_PASSIVE;
		} else if ((scanreq->dot11ScanType & (~dot11_scan_type_forced)) ==
			dot11_scan_type_auto) {
			ScanType = -1;
		} else {
			status = NDIS_STATUS_INVALID_DATA;
			break;
		}

/*	WM7: TBD
		bcopy((char *)&scanreq->dot11BSSID, wl->scan_bssid.octet, ETHER_ADDR_LEN);
*/
		/* Don't flush if "forced" is set... */
/* WM7: TBD	wl->scan_no_flush = ((scanreq->dot11ScanType & dot11_scan_type_forced) != 0); */

		/* ExtSTA: ignore scanreq->bRestrictedScan */

		/* Setup scan_ssid array */
		ASSERT(scanreq->uNumOfdot11SSIDs <= MAXSCAN_SSIDS);

		ssids = (DOT11_SSID *)&scanreq->ucBuffer[scanreq->udot11SSIDsOffset];
/* WM7: TBD
		for (i = 0; i < (int)scanreq->uNumOfdot11SSIDs; i++) {
			if (ssids->uSSIDLength)
				bcopy(ssids->ucSSID, wl->scan_ssid[i].SSID, ssids->uSSIDLength);
			wl->scan_ssid[i].SSID_len = ssids->uSSIDLength;
			ssids++;
		}
*/
		ASSERT(!scanreq->bUseRequestIE);



		/* if can't up, we're done (radio must have been disabled...or "wl down") */
		if (!wl->pub->up) {
#ifdef PRE_v1_69_HEADERS
			status = NDIS_STATUS_POWER_STATE_INVALID;
#else
			status = NDIS_STATUS_DOT11_POWER_STATE_INVALID;
#endif
			break;
		}

		wl_iscan_set_scan_type(wl, ScanType);
		wl_iscan_set_bss_type(wl, BSSType);
		wl_iscan_set_scan(wl);
		break;
	}
#endif /* WL_IW_USE_ISCAN && EXT_STA && UNDER_CE */

	case (WL_OID_BASE+WLC_SET_VAR):
		/* Intercept and handle ndis specific iovars */
		if (wl_set_ndis_iovar(wl, InfoBuf, InfoBufLen, &status))
			break;

		/* else drop down to default */

	default:
#ifdef WL_IW_USE_ISCAN
		if (Oid == OID_802_11_SSID) {
			wl_iscan_set_radio_disable(wl, FALSE);
#if 0	//def __BBSOFT__ // Not work module return "set ssid failed"
			{
				int i;
				NDIS_802_11_SSID *ssid = (NDIS_802_11_SSID *)InfoBuf;
			
				if (!_wl_ssid_is_valid(ssid) && wl->probeEnable) {
					if (!wl->probeSSID[wl->probeIndex][0]) wl->probeIndex ^= 1;
					
					if (wl->probeSSID[wl->probeIndex][0]) {			
						memcpy(ssid->Ssid, wl->probeSSID[wl->probeIndex], 32);
						ssid->SsidLength = strlen(ssid->Ssid);
						
						if (!wl->probeSSID[wl->probeIndex][0]) wl->probeIndex ^= 1;
					}
					
					WL_INFORM(("probeEnabled and try to connect[%s]\n", ssid->Ssid));
				}			
			}
#endif
			
#if 0	/* for test only - smlee */
			{
				int z = 0;

				char *str = (char *)InfoBuf;

				printf("Print InfoBuf, bufLen = %d\n", InfoBufLen);
				for(z=0; z<InfoBufLen; z++)
				{
					if(bcm_isalnum(str[z]))
						printf("[%c] ", str[z]);
				}
				printf("\n");
			}
#endif	/* for test only */			
		}
		if (Oid == OID_802_11_DISASSOCIATE) {
			wl_iscan_set_radio_disable(wl, TRUE);
		}
#endif /* WL_IW_USE_ISCAN */

#if defined(SIMPLE_ISCAN) && defined(EXT_STA)
		if (Oid == OID_DOT11_RESET_REQUEST) {
			if (wl->in_scan) {
				/* In case of a pending scan request:
				*  free the scan buffer,
				*  abort the scan (which also sends a failed
				*  scan complete notification to NDIS)
				*/
				WL_INFORM(("OID_DOT11_RESET_REQUEST & scan pending -> \
					reset iscan\n"));
				dhd_iscan_free_buf(wl->dhd, 0);
				dhd_iscan_request(wl->dhd, WL_SCAN_ACTION_ABORT);
				wl->in_scan = FALSE;
			}
		}
#endif 

#if defined(EXT_STA)
		wl_oidinfo_snoop(MiniportAdapterContext, Oid,
			InfoBuf, InfoBufLen, BytesRead, BytesNeeded);
#endif /* BCMDONGLEHOST && EXT_STA */

		save_bcmerror = 0;
		status = wl_setoid(wl, Oid, InfoBuf,
			InfoBufLen, BytesRead, BytesNeeded, P2PUNITNUMBER());
		if (status == NDIS_STATUS_INVALID_LENGTH) {
			/* wl_set_oid() already checked */
			SetBytes = *BytesNeeded;
		}
		break;
	}

response:

#ifndef NDIS60
	ND_UNLOCK(wl);
#endif /* !NDIS60 */

	if (status != NDIS_STATUS_SUCCESS) {
		WL_OID(("OID: FAILED setting %s !!!\n", wl_oid2str(Oid)));
		*BytesRead = 0;
		if (status == NDIS_STATUS_INVALID_LENGTH)
			*BytesNeeded = SetBytes;
	} else
		*BytesRead = InfoBufLen;

	/* check if the ndis error needs to be mapped to bcmerror */
	if ((save_bcmerror) && (status != NDIS_STATUS_SUCCESS)) {
		int32 bcmerror = ndisstatus2bcmerror(status);
		wl_iovar_setint(wl, "bcmerror", bcmerror);
	}

	return (status);
}

#ifdef EXT_STA
void
wl_process_mid_txq(wl_info_t *wl)
{
	ND_PKT *p;

	/* No need to grab the lock here, since its already grabbed 
	 * for us by the timer dispatcher (wl_timer())
	 */
	ND_TXLOCK(wl);
	while ((p = shared_deq(&wl->tx_mid_q))) {
		/* silently toss */
		if (!wl->pub->up || !wl->pub->associated) {
			wl->tx_mid_q_count--;
			WL_ERROR(("%s: pkt tossed\n", __FUNCTION__));
			shared_enq(&wl->sh.txdone, p, FALSE);
			WLCNTINCR(wl->pub->_cnt.txnoassoc);
		} else {
			wl->txenq++;
			shared_enq(&wl->txq, p, FALSE);
		}
	}
	ND_TXUNLOCK(wl);

	if (!wl->pub->up || !wl->pub->associated) {
		WL_ERROR(("%s: Pkt tossed, complete called\n", __FUNCTION__));
		wl_sendcomplete(wl);
	} else {
		ND_TXLOCK(wl);
		wl_process_txq(wl);
		ND_TXUNLOCK(wl);
	}
}

void
wl_tx_convert_d11hdr(wl_info_t *wl, struct lbuf *lb,
	struct ether_header **peh)
{
	struct dot11_header *h;
	struct ether_addr a1, a2, a3;
	struct dot11_llc_snap_header *lsh;
	struct ether_header *eseh;
	uint16 fc, proto, len;


	h = (struct dot11_header *)PKTDATA(wl->sh.osh, lb);
	fc = ltoh16(h->fc);

	ASSERT(FC_TYPE(fc) == FC_TYPE_DATA);

	/*
	 * 802.11 -> 802.3/Ethernet header conversion
	 * eliminate possible overwrite problems
	 */
	bcopy((char *)&h->a1, (char *)&a1, ETHER_ADDR_LEN);
	bcopy((char *)&h->a2, (char *)&a2, ETHER_ADDR_LEN);
	bcopy((char *)&h->a3, (char *)&a3, ETHER_ADDR_LEN);

	/* Locate LLC snap hdr, adjust for qos_control, and get OOB priority */
	lsh = (struct dot11_llc_snap_header *)((uchar *)h + DOT11_A3_HDR_LEN);

	proto = ntoh16(lsh->type);
	if (lsh->dsap == 0xaa && lsh->ssap == 0xaa && lsh->ctl == 0x03 &&
	    lsh->oui[0] == 0 && lsh->oui[1] == 0 &&
	    ((lsh->oui[2] == 0x00 && proto != 0x80f3 && proto != 0x8137) ||
	     (lsh->oui[2] == 0xf8 && (proto == 0x80f3 || proto == 0x8137)))) {
		eseh = (struct ether_header *)
			PKTPULL(wl->sh.osh, lb, DOT11_A3_HDR_LEN + DOT11_LLC_SNAP_HDR_LEN -
				ETHER_HDR_LEN);
	} else {
		eseh = (struct ether_header *)
			PKTPULL(wl->sh.osh, lb, DOT11_A3_HDR_LEN - ETHER_HDR_LEN);
		len = (uint16)(pkttotlen(wl->sh.osh, lb) - ETHER_HDR_LEN);
		ASSERT(len <= ETHER_MAX_DATA);
		eseh->ether_type = hton16(len);
	}

	if ((fc & FC_TODS) && !(fc & FC_FROMDS)) {
		/* infrastructure */
		bcopy((char*)&a3, (char*)&eseh->ether_dhost, ETHER_ADDR_LEN);
	} else {
		/* adhoc */
		ASSERT(!(fc & FC_TODS) && !(fc & FC_FROMDS));
		bcopy((char *)&a1, (char *)&eseh->ether_dhost, ETHER_ADDR_LEN);
	}
	bcopy((char*)&a2, (char*)&eseh->ether_shost, ETHER_ADDR_LEN);

	*peh = eseh;
}

static void
wl_tx_convert_hdr(wl_info_t *wl, struct lbuf *lb,
	struct ether_header **peh)
{
	ND_PKT	*p;
	DOT11_EXTSTA_SEND_CONTEXT *ctxt;
	uint exempt;
	eapol_header_t *epl;

	/* convert 802.11 hdr to 802.3 */
	wl_tx_convert_d11hdr(wl, lb, peh);

	/* process pkt send context */
	p = lb->p;
	ctxt = (DOT11_EXTSTA_SEND_CONTEXT *)
		NET_BUFFER_LIST_INFO(p, MediaSpecificInformation);

	/* N.B.: - Ignore PhyID and SleepValue for now */

#ifndef PRE_v1_68_HEADERS
	/* N.B.: - Ignore MediaSpecificInfo for now...no IHV specific protocol driver */

	/* N.B.: - Ignore SendFlags...none defined yet */
#endif /* PRE_v1_68_HEADERS */

	/* get pkt encryption exemption */
	switch (ctxt->usExemptionActionType) {
	case DOT11_EXEMPT_NO_EXEMPTION:
		exempt = WSEC_EXEMPT_NO;
		break;
	case DOT11_EXEMPT_ON_KEY_MAPPING_KEY_UNAVAILABLE:
		exempt = WSEC_EXEMPT_NO_PAIRWISE;
		break;
	default:
		ASSERT(ctxt->usExemptionActionType == DOT11_EXEMPT_ALWAYS);
		exempt = WSEC_EXEMPT_ALWAYS;
		break;
	}
	epl = (eapol_header_t *)(*peh);
	if (ntoh16(epl->eth.ether_type) == ETHER_TYPE_802_1X && epl->type == EAPOL_KEY) {
		if (exempt == WSEC_EXEMPT_NO_PAIRWISE && !wl->pwk) {
			/* Force M1 ~ M4 to be exempt */
			exempt = WSEC_EXEMPT_ALWAYS;
		}
		WL_INFORM(("%s: Received EAPOL_KEY, exempt %d,length=%d !...\n",
			__FUNCTION__, exempt, BCMSWAP16(epl->length)));
	}

	/* set pkt encryption exemption */
	WLPKTFLAG_EXEMPT_SET(WLPKTTAG(lb), exempt);

	return;
}

#else /* EXT_STA */

static void
wl_process_link_down(wl_info_t *wl)
{
	wl_indicate_link_down(wl);
}

#endif /* EXT_STA */


void
wl_process_txq(wl_info_t *wl)
{
	ND_PKT *p;
	struct lbuf *lb;

	WL_TRACE(("wl%d: wl_process_txq\n", wl->sh.unit));

	/* process ndis packet queue */
	while ((!wl->txqstopped) && (p = shared_deq(&wl->txq))) {
#ifdef EXT_STA
		struct dot11_header *h;
		uint16 fc;
#endif /* EXT_STA */
		struct ether_header *eh;
		int priority;

		/* convert packet to lbuf */
		if (!(lb = PKTFRMNATIVE(wl->sh.osh, p))) {
			shared_enq(&wl->txq, p, TRUE);
			break;
		}

#ifndef EXT_STA
		eh = (struct ether_header *)PKTDATA(wl->sh.osh, lb);
#else /* EXT_STA */
		h = (struct dot11_header *)PKTDATA(wl->sh.osh, lb);
		fc = ltoh16(h->fc);

		if ((fc & FC_KIND_MASK) == FC_DEAUTH) {
			scb_val_t scb_val;
			struct dot11_management_header *mgt_hdr;
			uchar *body;
			uint16 reason;

			mgt_hdr = (struct dot11_management_header *)h;
			body = (uchar *)h + sizeof(struct dot11_management_header);
			reason = ltoh16(*(uint16 *)body);

			bzero(&scb_val, sizeof(scb_val_t));
			bcopy((char *)&mgt_hdr->da, (char *)&scb_val.ea,
			            ETHER_ADDR_LEN);
			scb_val.val = (uint32) reason;
			(void) wl_ioctl_async(wl, WLC_SCB_DEAUTHENTICATE_FOR_REASON,
				NULL, sizeof(scb_val_t));
			PKTFREE(wl->sh.osh, lb, TRUE);
			ND_TXUNLOCK(wl);
			wl_sendcomplete(wl);
			ND_TXLOCK(wl);
			continue;
		}
		/* For Vista convert 802.11 hdr to 802.3 */
		wl_tx_convert_hdr(wl, lb, &eh);
#endif /* !EXT_STA */
#if defined(NDIS60)
		{
			eapol_header_t *eplh = (eapol_header_t *)eh;
			if (ntoh16(eplh->eth.ether_type) == ETHER_TYPE_802_1X &&
				eplh->type == EAPOL_KEY) {
				lb->p = NULL; /* Prevent double complete */
				shared_enq(&wl->sh.txdone, p, FALSE);
				ND_TXUNLOCK(wl);
				wl_sendcomplete(wl);
				ND_TXLOCK(wl);
			}
		}
#endif /* UNDER_CE && NDIS60 */
#ifdef BCMSUP_PSK
		/* block 802.1x from external supplicant */
		if (wl->sup_enable_wpa && !wl->unblock_8021x) {
			eapol_header_t *epl;
			epl = (eapol_header_t *)eh;
			if (ntoh16(epl->eth.ether_type) == ETHER_TYPE_802_1X &&
			    epl->type == EAPOL_START) {
				sup_auth_status_t auth;
				int iovar_int;
				wl_iovar_getint(wl, "sup_auth_status", &iovar_int);
				auth = iovar_int;
				if (auth == WLC_SUP_KEYED) {
					WL_WSEC(("wl%d: wl_process_txq: tossing EAPOL-Start issued"
						" by external supplicant\n", wl->sh.unit));
					shared_enq(&wl->sh.txdone, p, FALSE);
					ND_TXUNLOCK(wl);
					wl_sendcomplete(wl);
					ND_TXLOCK(wl);
					continue;
				}
			}
		}
#endif /* BCMSUP_PSK */

		/* get pkt priority */
		priority = GET_NDIS_PRIORITY(p);

#ifndef EXT_STA
		if (priority == 0)
			pktsetprio(lb, FALSE);
		else
#endif /* !EXT_STA */
		PKTSETPRIO(lb, priority & MAXPRIO);

		wl->txq_count++;
		dhd_start_xmit(wl->dhd, GET_NDIS_P2PUNITNUMBER(p),  lb);
	}
}

void
wl_txflowcontrol(wl_info_t *wl, bool state, int prio)
{
	ND_TXLOCK(wl);

	wl->txqstopped = (state == OFF) ? FALSE : TRUE;
	if (state == OFF)
		wl_process_txq(wl);

	ND_TXUNLOCK(wl);
}


#ifndef NDIS60
static void
wl_msendpackets(
	NDIS_HANDLE MiniportAdapterContext,
	PPNDIS_PACKET PacketArray,
	UINT NumPackets
)
{
	wl_info_t *wl;
	UINT i;

	wl = WL_INFO(MiniportAdapterContext);
	ASSERT(wl);

	WL_TRACE(("wl%d: wl_msendpackets\n", wl->sh.unit));
	ND_LOCK(wl);
	ND_TXLOCK(wl);

	wl->txpacket += NumPackets;

	for (i = 0; i < NumPackets; i++) {
		NDIS_SET_PACKET_STATUS(PacketArray[i], NDIS_STATUS_PENDING);


		wl->txenq++;

		SET_NDIS_P2PUNITNUMBER(PacketArray[i], P2PUNITNUMBER());

		/* enqueue pkt on txq */
		shared_enq(&wl->txq, PacketArray[i], FALSE);
	}

	wl_process_txq(wl);

	ND_TXUNLOCK(wl);
	ND_UNLOCK(wl);
}
#else /* !NDIS60 */
static void
wl_msendpackets(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PNET_BUFFER_LIST NetBufferLists,
	IN NDIS_PORT_NUMBER PortNumber,
	IN ULONG SendFlags
)
{
	wl_info_t *wl;
	NET_BUFFER_LIST *p;

	wl = WL_INFO(MiniportAdapterContext);

	WL_TRACE(("wl%d: wl_msendpackets\n", wl->sh.unit));

#if defined(BCMDHDUSB)
	if (wl->devhalted == TRUE) {
		WL_ERROR(("wl_msendpacket: Dev is halted\n"));
		return;
	}
#endif /* BCMDONGLEHOST && BCMDHDUSB */

	ND_TXLOCK(wl);
	for (p = NetBufferLists; p != NULL; p = NET_BUFFER_LIST_NEXT_NBL(p)) {

		wl->tx_mid_q_count++;
		wl->txpacket++;
		/* enqueue pkt on txq */
		shared_enq(&wl->tx_mid_q, p, FALSE);
	}
	wl_add_timer(wl, wl->process_mid_txq_timer, 0, FALSE);

	ND_TXUNLOCK(wl);
}


static void
wl_mcancelsend(
	IN NDIS_HANDLE MiniportAdapterContext,
	IN PVOID CancelId
)
{
}

static void
wl_mposttxds(
	IN PDEVICE_OBJECT pDO,
	IN PVOID Reserved,
	IN PSCATTER_GATHER_LIST pSGL,
	IN PVOID Context
)
{
}
#endif /* !NDIS60 */

void
wl_sendcomplete(wl_info_t *wl)
{
#ifndef NDIS60
	PNDIS_PACKET p;

	/*
	 * As you can see, great pains are made to meet the requirement
	 * that NdisMSendComplete() be called only from a Dpc/Timer routine
	 * with no private locks held.
	 */
	while (p = shared_deq(&wl->sh.txdone)) {
		wl->txcomplete++;
		/* sendcomplete stack packets */
		ND_UNLOCK(wl);
		NdisMSendComplete(wl->sh.adapterhandle, p, NDIS_STATUS_SUCCESS);
		ND_LOCK(wl);
	}
#else
	PNET_BUFFER_LIST p, head, prev;

	/* 
	 * TODO - This may need to be removed due to possible deadlock
	 * issues seen on XP.
	 */
	ND_TXLOCK(wl);

	head = NULL;
	prev = NULL;
	while (p = shared_deq(&wl->sh.txdone)) {
		wl->txcomplete++;

		/* mark packet status */
		NET_BUFFER_LIST_STATUS(p) = NDIS_STATUS_SUCCESS;

		/* relink NetBufferLists list before completing */
		if (!head)
			head = p;
		NET_BUFFER_LIST_NEXT_NBL(p) = NULL;
		if (prev)
			NET_BUFFER_LIST_NEXT_NBL(prev) = p;
		prev = p;
	}

	/* 
	 * TODO - This may need to be removed due to possible deadlock
	 * issues seen on XP.
	 */
	ND_TXUNLOCK(wl);

	if (head) {
		ND_UNLOCK(wl);
		NdisMSendNetBufferListsComplete(wl->sh.adapterhandle, head, 0);
		ND_LOCK(wl);
	}
#endif /* NDIS60 */
}

#ifndef NDIS60
static void
wl_mrxreturn(
	NDIS_HANDLE MiniportAdapterContext,
	PNDIS_PACKET p
)
{
	wl_info_t *wl;

	wl = WL_INFO(MiniportAdapterContext);
	ASSERT(wl);

	WL_TRACE(("wl%d: wl_mrxreturn\n", wl->sh.unit));

	/* free it regardless of packet status */
	ND_LOCK(wl);
	ND_RXLOCK(wl);
	wl->rxreturn++;
	shared_free_ndispacket(&wl->sh, p);
	ND_RXUNLOCK(wl);

#ifdef EXT_STA
	if (wl->pended_oid &&
	    ((wl->rxind - wl->rxreturn) == 0)) {
		uint complete = 1;

		wl_ioctl(wl, WLC_SOFT_RESET, &complete, sizeof(complete));
	}
#endif /* EXT_STA */
	ND_UNLOCK(wl);
}
#else /* !NDIS60 */
static void
wl_mrxreturn(
	NDIS_HANDLE MiniportAdapterContext,
	PNET_BUFFER_LIST p,
	ULONG ReturnFlags
)
{
	wl_info_t *wl;
	wl = WL_INFO(MiniportAdapterContext);

	WL_TRACE(("wl%d: wl_mrxreturn\n", wl->sh.unit));

	ND_LOCK(wl);
	while (p) {
		PNET_BUFFER_LIST next;
		next = NET_BUFFER_LIST_NEXT_NBL(p);

		ND_RXLOCK(wl);
		/* free it regardless of packet status */
		wl->rxreturn++;
		shared_free_ndispacket(&wl->sh, p);
		ND_RXUNLOCK(wl);

		p = next;
	}

#ifdef EXT_STA
	if (wl->pended_oid &&
	    ((wl->rxind - wl->rxreturn) == 0)) {
		uint complete = 1;
		WL_ERROR(("%s: wl%d: wl_mrxreturn WLC_SOFT_RESET!!\n",
			__FUNCTION__, wl->sh.unit));
		wl_ioctl(wl, WLC_SOFT_RESET, &complete, sizeof(complete));
	}
#endif /* EXT_STA */

	ND_UNLOCK(wl);
}
#endif /* NDIS60 */

#ifndef EXT_STA
void
wl_indicate_link_up(wl_info_t *wl)
{
	WL_ERROR(("wl%d: link up\n", wl->sh.unit));

	wl->link = TRUE;
	NdisMIndicateStatus(wl->sh.adapterhandle, NDIS_STATUS_MEDIA_CONNECT,
		NULL, 0);
	NdisMIndicateStatusComplete(wl->sh.adapterhandle);
}

void
wl_indicate_link_down(wl_info_t *wl)
{
	WL_ERROR(("wl%d: indicate link down\n", wl->sh.unit));

	wl->link = FALSE;
	ND_UNLOCK(wl);
	NdisMIndicateStatus(wl->sh.adapterhandle, NDIS_STATUS_MEDIA_DISCONNECT, NULL, 0);
	ND_LOCK(wl);
	NdisMIndicateStatusComplete(wl->sh.adapterhandle);
}
#endif /* !EXT_STA */

static void
wl_dev_gone(void *wlh)
{
	wl_info_t *wl = (wl_info_t*)(wlh);

	/* this callback can be invoked in any thread context, needs lock/unlock */
	ND_LOCK(wl);
#ifndef EXT_STA
	wl_indicate_link_down(wl);
#endif /* !EXT_STA */
	bcmsdh_device_remove(wl->sh.sdh);
	wl_down(wl);
	ND_UNLOCK(wl);
}

#if defined(EXT_STA) || defined(DHD_NDIS_OID)
/* return true if the rateset contains an OFDM rate */
bool
wlc_rateset_isofdm(uint count, uint8 *rates)
{
	int i;
	for (i = count - 1; i >= 0; i--)
		if (IS_OFDM(rates[i]))
			return (TRUE);
	return (FALSE);
}

/* Adapted from wlc.c wlc_bss2wl_bss(); for dongle, we need to do the opposite */
static int
wl_bss2wlc_bss(wl_info_t *wl, wl_bss_info_t *bi, wlc_bss_info_t *to_bi, int to_bi_len)
{
	wlc_bss_info_t wlc_bi;
	uint8 *ie;
	struct dot11_bcn_prb *fixed;

	/* Sanity check */
	if ((bi->ie_length != 0) && (bi->length != (bi->ie_offset + ROUNDUP(bi->ie_length, 4)))) {
		WL_ERROR(("bi->length=%d != off(%d)+ie_len(%d)\n", bi->length,
			bi->ie_offset, bi->ie_length));
		return -1;
	}

	if (to_bi->bcn_prb != NULL) {
		WL_TRACE(("Freeing previous association BSS beacon probe = %d\n", \
					to_bi->bcn_prb_len));
		MFREE(wl->sh.osh, to_bi->bcn_prb, to_bi->bcn_prb_len);
		to_bi->bcn_prb = NULL;
		to_bi->bcn_prb_len = 0;
	}

	to_bi->bcn_prb_len = bi->ie_length + DOT11_BCN_PRB_LEN;
	to_bi->bcn_prb = MALLOC(wl->sh.osh, to_bi->bcn_prb_len);

	if (!to_bi->bcn_prb) {
		WL_ERROR(("to_bi->bcn_prb is NULL!\n"));
		return -1;
	}

	fixed = (struct dot11_bcn_prb *) to_bi->bcn_prb;
	if (bi->ie_length) {
		ie = (uint8 *)bi + bi->ie_offset;
		bcopy(ie, (uint8 *)to_bi->bcn_prb + DOT11_BCN_PRB_LEN, bi->ie_length);
	} else {
		WL_ERROR(("BSS has no IEs\n"));
	}

	WL_TRACE(("bi->length=%d ie_off=%d ie_len=%d\n", bi->length, bi->ie_offset, bi->ie_length));

	bzero(&wlc_bi, sizeof(wlc_bss_info_t));

	wlc_bi.bcn_prb_len      = to_bi->bcn_prb_len;
	wlc_bi.bcn_prb          = to_bi->bcn_prb;
	wlc_bi.beacon_period	= bi->beacon_period;
	wlc_bi.capability	= bi->capability;
	wlc_bi.chanspec		= bi->chanspec;
	wlc_bi.atim_window	= bi->atim_window;
	wlc_bi.dtim_period	= bi->dtim_period;
	wlc_bi.RSSI		= bi->RSSI;
	wlc_bi.phy_noise	= bi->phy_noise;

	bcopy(bi->BSSID.octet, wlc_bi.BSSID.octet, ETHER_ADDR_LEN);
	wlc_bi.SSID_len	= bi->SSID_len;
	bcopy(bi->SSID, wlc_bi.SSID, wlc_bi.SSID_len);

	if (bi->flags & WL_BSS_FLAGS_FROM_BEACON)
		wlc_bi.flags |= WLC_BSS_BEACON;

	/* limit rates to destination rate array size */
	wlc_bi.rateset.count = MIN(bi->rateset.count, sizeof(wlc_bi.rateset.rates));
	bcopy(bi->rateset.rates, wlc_bi.rateset.rates, wlc_bi.rateset.count);

	if ((bi->capability & DOT11_CAP_ESS) && !(bi->capability & DOT11_CAP_IBSS))
		wlc_bi.infra = 1;
	wlc_bi.capability = bi->capability;
	if (fixed) {
		fixed->capability = wlc_bi.capability;
		fixed->beacon_interval = wlc_bi.beacon_period;
		if (to_bi->bcn_prb_len >= DOT11_BCN_PRB_LEN) {
			wl_ioctl(wl, WLC_GET_BSS_BCN_TS, fixed->timestamp,
				sizeof(fixed->timestamp));
		}
	}


	/* copy fixed portion of the bss info structure to user buffer */
	bcopy(&wlc_bi, to_bi, sizeof(wlc_bss_info_t));

	return 0;
}

/*
 * BCMDONGLEHOST port
 *
 * NOTE: In dongle mode, current BSS info is kept in dongle
 */
wlc_bss_info_t *
wl_get_current_bss(wl_info_t *wl)
{
	wlc_bss_info_t *wlc_bss = NULL;
	wl_bss_info_t *wl_bss;
	char *ioctlbuf;
	int i;
#ifndef DHD_NDIS_OID
	ULONG bytesneeded, bytesused;
#endif /* !DHD_NDIS_OID */
	if (wl->bss_good == TRUE) {
		WL_ERROR(("Using cached current_bss!\n"));
		wlc_bss = &wl->current_bss;
		return wlc_bss;
	}

	ioctlbuf = MALLOC(wl->sh.osh, PAGE_SIZE);
	if (!ioctlbuf) {
		WL_ERROR(("wl_get_current_bss: MALLOC failed\n"));
		return NULL;
	}
	bzero(ioctlbuf, PAGE_SIZE);
	*(uint32*)ioctlbuf = hton32(PAGE_SIZE);
	if (wl_ioctl(wl, WLC_GET_BSS_INFO, ioctlbuf, PAGE_SIZE) == 0) {

		wl_bss = (wl_bss_info_t *)(ioctlbuf + 4);
		wlc_bss = &wl->current_bss;

		if (wl_bss2wlc_bss(wl, wl_bss, wlc_bss, 0) < 0) {
			WL_ERROR(("wl_bss2wlc_bss failed! \n"));
			wlc_bss = NULL;
			goto error;
		}

		wl_ioctl(wl, WLC_GET_BSS_WPA_RSN, &wlc_bss->wpa, sizeof(wlc_bss->wpa));
#ifdef BCMWPA2
		wl_ioctl(wl, WLC_GET_BSS_WPA2_RSN, &wlc_bss->wpa2, sizeof(wlc_bss->wpa2));
#endif

		/* Update phy channel with current BSS and sync with dongle
		 * OID_DOT11_CURRENT_CHANNEL now returns correct value
		 */
		for (i = 0; i < (int)wl->oid->phy.num_phy_ids; i++) {
			wl->oid->phy.phy_channel[i] = CHSPEC_CHANNEL(wlc_bss->chanspec);
		}
#ifndef DHD_NDIS_OID
		wl_setoid(wl, (NDIS_OID)OID_STA_SET_PHY,
			(PVOID) &wl->oid->phy, (ULONG) sizeof(wl->oid->phy),
			&bytesused, &bytesneeded, 0);
#endif /* !DHD_NDIS_OID */
	}
	else
		WL_ERROR(("WLC_GET_BSS_INFO ioctl failed \n"));

error:
	MFREE(wl->sh.osh, ioctlbuf, PAGE_SIZE);

	return wlc_bss;
}

/*
 * wl_get_bandstate() retrieves the bandstate table and
 * returns the current active bandunit.
 */
uint
wl_get_bandstate(wl_info_t *wl)
{
	uint i, bandunit = 0;

	wl_ioctl(wl, WLC_GET_BANDSTATES, &wl->wlc->bandstate, sizeof(wl->wlc->bandstate));

	for (i = 0; i < MAXBANDS; i++)
		wl->wlc->bandstate[i].pi = NULL;	/* the *pi is not valid in driver */

	wl_iovar_getint(wl, "bandunit", &bandunit);

	if (bandunit > MAXBANDS)
		bandunit = 0;

	return bandunit;
}
#endif /*  (BCMDONGLEHOST && EXT_STA) || DHD_NDIS_OID */

#ifdef EXT_STA
static void
wl_ind_assoc_completion(wl_info_t *wl, wl_event_msg_t *e, bool auth, bool reassoc,
	bool no_ndis)
{
	DOT11_ASSOCIATION_COMPLETION_PARAMETERS *StatusBuffer;
	wl_assoc_info_t assoc_info;
	int err;
	wlc_bss_info_t *cur_bss = NULL;
	struct dot11_assoc_req *assoc_req = NULL;
	struct dot11_assoc_resp *assoc_resp = NULL;
	uint assoc_req_len, assoc_resp_len;
	NDIS_802_11_NETWORK_TYPE net_type;
	ULONG offset, stbuf_size;
	uint wsec;
	if (!no_ndis) {
		/* ASSERT(wl->blocks & IND_BLOCK_ASSOC); */
		if (!(wl->blocks & IND_BLOCK_ASSOC)) {
			WL_INFORM(("%s: IND_BLOCK_ASSOC error blocks 0x%x\n", __FUNCTION__,
				wl->blocks));
		}
		WL_INFORM(("wl%d: EXT STA indication: ASSOCIATION COMPLETION : auth 0x%x, "
			"reassoc 0x%x, event: status 0x%x auth_type 0x%x ind_blocks 0x%x\n",
			wl->sh.unit, auth, reassoc, e->status, e->auth_type, wl->blocks));
	}

	{
		uint req_ies_len = 0, resp_ies_len = 0;

		/* The size of wl_bss_info_t should be big enough for all */
		cur_bss = wl_get_current_bss(wl);
		if (!cur_bss) {
			WL_ERROR(("ERROR: error getting cur_bss\n"));
			return;
		}

		bzero(&assoc_info, sizeof(wl_assoc_info_t));
		err = wl_iovar_op(wl, "assoc_info", &assoc_info, sizeof(wl_assoc_info_t), IOV_GET);
		if (err) {
			WL_ERROR(("ERROR: error getting assoc_info \n"));
			return;
		}

		assoc_req = NULL;
		assoc_resp = NULL;
		assoc_req_len = assoc_info.req_len;
		assoc_resp_len = assoc_info.resp_len;

		if (assoc_req_len) {
			assoc_req = MALLOC(wl->sh.osh, assoc_req_len);
			if (assoc_req == NULL) {
				WL_ERROR(("ERROR: assoc_req malloc failed\n"));
				return;
			}

			bcopy(&assoc_info.req, assoc_req, sizeof(struct dot11_assoc_req));

			req_ies_len = assoc_info.req_len - sizeof(struct dot11_assoc_req);
			if (assoc_info.flags & WLC_ASSOC_REQ_IS_REASSOC) {
				req_ies_len -= ETHER_ADDR_LEN;
			}

			if (reassoc)
				err = wl_iovar_op(wl, "assoc_req_ies",
					((unsigned char*)assoc_req)+10, req_ies_len, IOV_GET);
			else
			err = wl_iovar_op(wl, "assoc_req_ies", assoc_req+1, req_ies_len, IOV_GET);
			if (err) {
				WL_ERROR(("ERROR: error getting assoc_req_ies\n"));
				return;
			}
		}

		/* get the association resp IE's if there are any */
		if (assoc_resp_len) {
			assoc_resp = MALLOC(wl->sh.osh, assoc_resp_len);

			if (assoc_resp == NULL) {
				WL_ERROR(("ERROR: assoc_resp malloc failed\n"));
				if (assoc_req)
					MFREE(wl->sh.osh, assoc_req, assoc_req_len);
				return;
			}

			bcopy(&assoc_info.resp, assoc_resp, sizeof(struct dot11_assoc_resp));

			resp_ies_len = assoc_info.resp_len - sizeof(struct dot11_assoc_resp);
			err = wl_iovar_op(wl, "assoc_resp_ies", assoc_resp+1, \
					resp_ies_len, IOV_GET);
			if (err) {
				WL_ERROR(("ERROR: error getting assoc_resp_ies\n"));
				if (assoc_req)
					MFREE(wl->sh.osh, assoc_req, assoc_req_len);
				if (assoc_resp)
					MFREE(wl->sh.osh, assoc_resp, assoc_resp_len);
				return;
			}
		}
	}
	offset = sizeof(DOT11_ASSOCIATION_COMPLETION_PARAMETERS) + 0x40;
	if (e->auth_type == DOT11_BSSTYPE_INFRASTRUCTURE && !auth) {
		if (assoc_req)
			offset += assoc_req_len;
		if (e->status != WLC_E_STATUS_TIMEOUT && assoc_resp)
			offset += assoc_resp_len;
	}
	if (e->status == WLC_E_STATUS_SUCCESS)
		offset += cur_bss->bcn_prb_len;
	if (e->status == WLC_E_STATUS_SUCCESS && !auth)
		offset += sizeof(ULONG);

	stbuf_size = offset;
	StatusBuffer = MALLOC(wl->sh.osh, offset);
	ASSERT(StatusBuffer);
	if (!StatusBuffer) {
		WL_ERROR(("wl%d: wl_ind_assoc_completion: out of memory, alloc failed\n",
			wl->sh.unit));
		return;
	}

	bzero(StatusBuffer, stbuf_size);
	StatusBuffer->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	StatusBuffer->Header.Revision = DOT11_ASSOCIATION_COMPLETION_PARAMETERS_REVISION_1;
	StatusBuffer->Header.Size = sizeof(DOT11_ASSOCIATION_COMPLETION_PARAMETERS);
	bcopy(e->addr.octet, StatusBuffer->MacAddr, ETHER_ADDR_LEN);
	switch (e->status) {
	case WLC_E_STATUS_SUCCESS:
		ASSERT(!auth);
		StatusBuffer->uStatus = DOT11_ASSOC_STATUS_SUCCESS;
		WL_INFORM(("%s: ASSOC SUCCESS %d\n", __FUNCTION__, e->status));
		/* cache association time... */
		NdisGetCurrentSystemTime(&wl->oid->assoc_time);
		break;
	case WLC_E_STATUS_FAIL:
		/* auth/(re)assoc failure should always have a status code */
		WL_INFORM(("%s: ASSOC FAIL %d\n", __FUNCTION__, e->status));
		ASSERT(e->reason);
#ifdef PRE_v1_68_HEADERS
		StatusBuffer->uStatus = DOT11_ASSOC_STATUS_ASSOCIATION_RESPONSE_START |
			((uint16)e->reason);
#else
		StatusBuffer->uStatus = DOT11_ASSOC_STATUS_ASSOCIATION_RESPONSE |
			((uint16)e->reason);
#endif /* PRE_v1_68_HEADERS */
		break;
	case WLC_E_STATUS_NO_ACK:
	case WLC_E_STATUS_TIMEOUT:
		StatusBuffer->uStatus = DOT11_ASSOC_STATUS_UNREACHABLE;
		WL_INFORM(("%s: ASSOC TIMEOUT %d\n", __FUNCTION__, e->status));
		break;
	case WLC_E_STATUS_ABORT:
		StatusBuffer->uStatus = DOT11_ASSOC_STATUS_CANCELLED;
		WL_INFORM(("%s: ASSOC ABORT %d\n", __FUNCTION__, e->status));
		break;
	default:
		/* unexpected status */
		WL_INFORM(("wl%d: EXT STA indication: ASSOCIATION COMPLETION "
		    ": unexpected status 0x%x\n", wl->sh.unit, e->status));
		StatusBuffer->uStatus = DOT11_ASSOC_STATUS_FAILURE;
		break;
	}

	offset = sizeof(DOT11_ASSOCIATION_COMPLETION_PARAMETERS);
	/* N.B.: bss_type passed through auth_type event field */
	if (e->auth_type == DOT11_BSSTYPE_INFRASTRUCTURE && !auth) {
		if (reassoc) {
			StatusBuffer->bReAssocReq = TRUE;
			StatusBuffer->bReAssocResp = TRUE;
		} else {
			StatusBuffer->bReAssocReq = FALSE;
			StatusBuffer->bReAssocResp = FALSE;
		}

		if (assoc_req) {
			StatusBuffer->uAssocReqOffset = offset;
			StatusBuffer->uAssocReqSize = assoc_req_len;
			bcopy(assoc_req, &((char *)StatusBuffer)[offset],
			      StatusBuffer->uAssocReqSize);
		} else {
			StatusBuffer->uAssocReqOffset = 0;
			StatusBuffer->uAssocReqSize = 0;
		}
		offset += StatusBuffer->uAssocReqSize;
		if (e->status != WLC_E_STATUS_TIMEOUT && assoc_resp) {
			StatusBuffer->uAssocRespOffset = offset;
			StatusBuffer->uAssocRespSize = assoc_resp_len;
			bcopy(assoc_resp, &((char *)StatusBuffer)[offset],
			      StatusBuffer->uAssocRespSize);
		} else {
			StatusBuffer->uAssocRespOffset = 0;
			StatusBuffer->uAssocRespSize = 0;
		}
		offset += StatusBuffer->uAssocRespSize;
	} else {
		StatusBuffer->bReAssocReq = FALSE;
		StatusBuffer->uAssocReqOffset = 0;
		StatusBuffer->uAssocReqSize = 0;

		StatusBuffer->bReAssocResp = FALSE;
		StatusBuffer->uAssocRespOffset = 0;
		StatusBuffer->uAssocRespSize = 0;
	}

	StatusBuffer->uIHVDataOffset = 0;
	StatusBuffer->uIHVDataSize = 0;

	if (e->status == WLC_E_STATUS_SUCCESS) {
		StatusBuffer->uBeaconOffset = offset;
		StatusBuffer->uBeaconSize = cur_bss->bcn_prb_len;
		bcopy(cur_bss->bcn_prb, &((char *)StatusBuffer)[offset], StatusBuffer->uBeaconSize);
		offset += StatusBuffer->uBeaconSize;
	}

	if (e->status == WLC_E_STATUS_SUCCESS && !auth) {

		StatusBuffer->AuthAlgo = wl->oid->NDIS_auth;

		(void)wl_ioctl(wl, WLC_GET_WSEC, &wsec, sizeof(wsec));
		if (WSEC_AES_ENABLED(wsec)) {
			StatusBuffer->UnicastCipher = DOT11_CIPHER_ALGO_CCMP;
		} else if (WSEC_TKIP_ENABLED(wsec)) {
			StatusBuffer->UnicastCipher = DOT11_CIPHER_ALGO_TKIP;
		} else if (WSEC_WEP_ENABLED(wsec)) {
			StatusBuffer->UnicastCipher = DOT11_CIPHER_ALGO_WEP;
		} else {
			StatusBuffer->UnicastCipher = DOT11_CIPHER_ALGO_NONE;
		}

		/* No mixed mode support in IBSS mode */
		if ((wl->oid->NDIS_auth == DOT11_AUTH_ALGO_80211_OPEN) ||
		    (wl->oid->NDIS_auth == DOT11_AUTH_ALGO_80211_SHARED_KEY) ||
		    (wl->oid->NDIS_infra == Ndis802_11IBSS)) {
			StatusBuffer->MulticastCipher = StatusBuffer->UnicastCipher;
		} else {
			uint8 mcast = 0;
			if ((wl->oid->NDIS_auth == DOT11_AUTH_ALGO_WPA) ||
			    (wl->oid->NDIS_auth == DOT11_AUTH_ALGO_WPA_PSK)) {
				mcast = cur_bss->wpa.multicast;
			} else {
				ASSERT((wl->oid->NDIS_auth == DOT11_AUTH_ALGO_RSNA) ||
				       (wl->oid->NDIS_auth == DOT11_AUTH_ALGO_RSNA_PSK));
				mcast = cur_bss->wpa2.multicast;
			}
			switch (mcast) {
			case WPA_CIPHER_AES_CCM:
				StatusBuffer->MulticastCipher = DOT11_CIPHER_ALGO_CCMP;
				break;
			case WPA_CIPHER_TKIP:
				StatusBuffer->MulticastCipher = DOT11_CIPHER_ALGO_TKIP;
				break;
			case WPA_CIPHER_WEP_104:
				StatusBuffer->MulticastCipher = DOT11_CIPHER_ALGO_WEP104;
				break;
			case WPA_CIPHER_WEP_40:
				StatusBuffer->MulticastCipher = DOT11_CIPHER_ALGO_WEP40;
				break;
			default:
				WL_ERROR(("unsupported mcast cipher...%d\n", mcast));
				ASSERT(0);
			}
		}
		if ((int)((char *)StatusBuffer + offset) % sizeof(ULONG)) {
			offset += sizeof(ULONG) -
				(int)((char *)StatusBuffer + offset) % sizeof(ULONG);
		}
		StatusBuffer->uActivePhyListOffset = offset;
		StatusBuffer->uActivePhyListSize = sizeof(ULONG);
		if (CHSPEC_IS2G(cur_bss->chanspec))
#ifdef WL11N
			if (cur_bss->flags & WLC_BSS_HT)
				net_type = Ndis802_11FH;
			else
#endif /* WL11N */
			if (wlc_rateset_isofdm(cur_bss->rateset.count, cur_bss->rateset.rates))
				net_type = Ndis802_11OFDM24;
			else
				net_type = Ndis802_11DS;
		else
#ifdef WL11N
			if (cur_bss->flags & WLC_BSS_HT)
				net_type = Ndis802_11NetworkTypeMax;
			else
#endif /* WL11N */
			net_type = Ndis802_11OFDM5;
		*(ULONG *)(&((char *)StatusBuffer)[offset]) = wl->oid->active_phy_id =
			wl_NTIU2phy_id(wl->oid, net_type);
		offset += StatusBuffer->uActivePhyListSize;

		StatusBuffer->bFourAddressSupported = FALSE;

			StatusBuffer->bPortAuthorized = FALSE;

#ifndef PRE_v1_68_HEADERS
		if (WME_ENAB(wl->pub))
			    StatusBuffer->ucActiveQoSProtocol = DOT11_QOS_PROTOCOL_FLAG_WMM;
#endif /* PRE_v1_68_HEADERS */

		StatusBuffer->DSInfo = DOT11_DS_UNKNOWN;

		StatusBuffer->uEncapTableOffset = 0;
		StatusBuffer->uEncapTableSize = 0;
	}
	else {
		WL_ERROR(("NOT DOING AUTH STUFF!\n"));
	}

	if (e->status == WLC_E_STATUS_SUCCESS)
		wl->pub->associated = 1;
	if (!no_ndis) {
		shared_indicate_status(wl->sh.adapterhandle, \
			NDIS_STATUS_DOT11_ASSOCIATION_COMPLETION, \
			StatusBuffer, offset);
		wl->blocks &= ~IND_BLOCK_ASSOC;
	}
	if (e->auth_type == DOT11_BSSTYPE_INDEPENDENT) {
		uint i;

		for (i = 0; i < wl->oid->num_peers; i++)
			if (!bcmp(wl->oid->peers[i].addr.octet, e->addr.octet, ETHER_ADDR_LEN))
				break;

			if (i == wl->oid->num_peers && wl->oid->num_peers < MAXBSS) {
				bcopy(e->addr.octet, wl->oid->peers[i].addr.octet, ETHER_ADDR_LEN);
				wl->oid->num_peers++;
			}
		WL_ASSOC(("%s: wl->oid->num_peers=%d\n", \
					__FUNCTION__, wl->oid->num_peers));
	}

	MFREE(wl->sh.osh, StatusBuffer, stbuf_size);
	if (assoc_req)
		MFREE(wl->sh.osh, assoc_req, assoc_req_len);
	if (assoc_resp)
		MFREE(wl->sh.osh, assoc_resp, assoc_resp_len);
}

static void
wl_ind_roam_complete(wl_info_t *wl, uint status, bool no_ndis)
{
	DOT11_ROAMING_COMPLETION_PARAMETERS StatusBuffer;

	if (!no_ndis) {
		/* WM7: removed assert for now */
		/* ASSERT(wl->blocks & IND_BLOCK_ROAM); */
		if (!(wl->blocks & IND_BLOCK_ROAM)) {
			WL_INFORM(("%s: IND_BLOCK_ROAM error blocks 0x%x\n",
				__FUNCTION__, wl->blocks));
		}
		WL_INFORM(("wl%d: EXT STA indication: ROAMING COMPLETION\n", wl->sh.unit));
	} else
		WL_INFORM(("wl%d: EXT STA CCX indication: ROAMING COMPLETION\n", wl->sh.unit));

	StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	StatusBuffer.Header.Revision = DOT11_ROAMING_COMPLETION_PARAMETERS_REVISION_1;
	StatusBuffer.Header.Size = sizeof(DOT11_ROAMING_COMPLETION_PARAMETERS);
	if (status == WLC_E_STATUS_SUCCESS) {
		StatusBuffer.uStatus = DOT11_ASSOC_STATUS_SUCCESS;
		wl->link_report = 1;
	} else if (status == WLC_E_STATUS_ABORT)
		StatusBuffer.uStatus = DOT11_ASSOC_STATUS_CANCELLED;
	else
		/* WLC_E_STATUS_NO_NETWORKS, WLC_E_STATUS_FAIL */
		StatusBuffer.uStatus = DOT11_ASSOC_STATUS_CANDIDATE_LIST_EXHAUSTED;

	if (!no_ndis) {
		shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_ROAMING_COMPLETION,
			&StatusBuffer, sizeof(StatusBuffer));
		wl->blocks &= ~IND_BLOCK_ROAM;
	}
}

static void
wl_ind_connect_complete(wl_info_t *wl, uint status)
{
	DOT11_CONNECTION_COMPLETION_PARAMETERS StatusBuffer;

	/* WM7: removed assert for now */
	/* ASSERT(wl->blocks & IND_BLOCK_CONNECT); */
	if (!(wl->blocks & IND_BLOCK_CONNECT)) {
		WL_INFORM(("%s: IND_BLOCK_CONNECT error blocks 0x%x\n", __FUNCTION__, wl->blocks));
	}

	WL_INFORM(("wl%d: EXT STA indication: CONNECTION COMPLETION\n", wl->sh.unit));

	StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	StatusBuffer.Header.Revision = DOT11_CONNECTION_COMPLETION_PARAMETERS_REVISION_1;
	StatusBuffer.Header.Size = sizeof(DOT11_CONNECTION_COMPLETION_PARAMETERS);
	if (status == WLC_E_STATUS_SUCCESS) {
		StatusBuffer.uStatus = DOT11_ASSOC_STATUS_SUCCESS;
		wl->link_report = 1;
	} else if (status == WLC_E_STATUS_ABORT)
		StatusBuffer.uStatus = DOT11_ASSOC_STATUS_CANCELLED;
	else
		/* WLC_E_STATUS_NO_NETWORKS, WLC_E_STATUS_FAIL */
		StatusBuffer.uStatus = DOT11_ASSOC_STATUS_CANDIDATE_LIST_EXHAUSTED;

	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_CONNECTION_COMPLETION,
	    &StatusBuffer, sizeof(StatusBuffer));
	wl->blocks &= ~IND_BLOCK_CONNECT;

	/* After connection complete, do not use cached current_bss */
	wl->bss_good = FALSE;
}

static void
wl_close_ind_blocks(wl_info_t *wl)
{
	/* ASSERT(wl->blocks == 0); */
	if (wl->blocks) {
		WL_INFORM(("%s: Indication blocks still open %d\n", __FUNCTION__, wl->blocks));
	}
	if (wl->blocks & IND_BLOCK_ASSOC) {
		wl->blocks &= ~IND_BLOCK_ASSOC;
	}
	if (wl->blocks & IND_BLOCK_ROAM) {
		wl_ind_roam_complete(wl, WLC_E_STATUS_FAIL, FALSE);
	}
	if (wl->blocks & IND_BLOCK_CONNECT) {
		wl_ind_connect_complete(wl, WLC_E_STATUS_FAIL);
	}
}

static void
wl_ind_connection_start(wl_info_t *wl, wl_event_msg_t *e)
{
	DOT11_CONNECTION_START_PARAMETERS StatusBuffer;
	ULONG BytesRead, BytesNeeded;

	wl_queryoid(wl, OID_STA_DESIRED_SSID, &wl->oid->desired_ssid, \
		sizeof(wl->oid->desired_ssid), &BytesRead, &BytesNeeded, 0);


	wl_close_ind_blocks(wl);

	WL_INFORM(("wl%d: EXT STA indication: CONNECTION START\n", wl->sh.unit));

	bzero(&StatusBuffer, sizeof(DOT11_CONNECTION_START_PARAMETERS));
	StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	StatusBuffer.Header.Revision = DOT11_CONNECTION_START_PARAMETERS_REVISION_1;
	StatusBuffer.Header.Size = sizeof(DOT11_CONNECTION_START_PARAMETERS);

	/* N.B.: bss_type passed through auth_type event field */
	if (e->auth_type == DOT11_BSSTYPE_INFRASTRUCTURE) {
		StatusBuffer.BSSType = dot11_BSS_type_infrastructure;
	} else {
		StatusBuffer.BSSType = dot11_BSS_type_independent;

		bcopy(e->addr.octet, StatusBuffer.AdhocBSSID, ETHER_ADDR_LEN);

		{
			ULONG BytesRead, BytesNeeded;

			bzero(wl->oid->desired_ssid.SSID, \
				sizeof(wl->oid->desired_ssid.SSID));
			wl_queryoid(wl, OID_STA_DESIRED_SSID, \
				&wl->oid->desired_ssid, \
				sizeof(wl->oid->desired_ssid), \
				&BytesRead, &BytesNeeded, 0);
			WL_ERROR(("Desired SSID = %s, len=%d\n", \
					wl->oid->desired_ssid.SSID, \
					wl->oid->desired_ssid.SSID_len));
		}

		if (wl->oid->desired_ssid.SSID_len != DOT11_INVALID_SSID_LEN) {
			StatusBuffer.AdhocSSID.uSSIDLength = wl->oid->desired_ssid.SSID_len;
			bcopy(wl->oid->desired_ssid.SSID, StatusBuffer.AdhocSSID.ucSSID,
				wl->oid->desired_ssid.SSID_len);
		}
		else
			StatusBuffer.AdhocSSID.uSSIDLength = 0;

	}

	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_CONNECTION_START,
	    &StatusBuffer, sizeof(StatusBuffer));

	wl->blocks |= IND_BLOCK_CONNECT;
}

void
wl_ind_scan_confirm(wl_info_t *wl, NDIS_STATUS StatusBuffer)
{
	WL_INFORM(("wl%d: EXT STA indication: SCAN CONFIRM\n", wl->sh.unit));

	wl->in_scan = FALSE;
	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_SCAN_CONFIRM,
		&StatusBuffer, sizeof(StatusBuffer));
}

uint
wl_Rssi2LQ(int Rssi)
{
	uint lq;

	/* Range 60db Rssi from -90db to -30db needs 
	 * to be mapped into 0 - 100 Link quality
	 * First lowest 5db i.e -90db to -85db is mapped 20-40 -- 20
	 * Next lowest  7db i.e -85db to -78db is mapped 40-60 -- 20
	 * Next lowest 12db i.e -78db to -66db is mapped 60-75 -- 15
	 * Next lowest 15db i.e -66db to -51db is mapped 75-90 -- 15
	 * Next lowest 21db i.e -51db to -30db is mapped 90-100 -- 10
	 */

	if (Rssi <= -91)
		lq = 0;

	else if (Rssi >= -90 && Rssi < -85)
		lq = 20 + (((Rssi - (-90)) * 20) / 5);

	else if (Rssi >= -85 && Rssi < -78)
		lq = 40 + (((Rssi - (-85)) * 20) / 7);

	else if (Rssi >= -78 && Rssi < -66)
		lq = 60 + (((Rssi - (-78)) * 15) / 12);

	else if (Rssi >= -66 && Rssi < -51)
		lq = 75 + (((Rssi - (-66)) * 15) / 15);

	else if (Rssi >= -51 && Rssi < -30)
		lq = 90 + (((Rssi - (-51)) * 10) / 21);

	else if (Rssi >= -30)
		lq = 100;

	WL_TRACE(("RSSI = %d, lq = %d\n", Rssi, lq));

	return lq;
}

ULONG
wl_NTIU2phy_id(wl_oid_t *wl, NDIS_802_11_NETWORK_TYPE NetworkTypeInUse)
{
	uint i;
	uint desired_phy_type;
	uint band = WLC_BAND_2G;

	if (NetworkTypeInUse == Ndis802_11OFDM24) {
		desired_phy_type = PHY_TYPE_G;
	} else if (NetworkTypeInUse == Ndis802_11OFDM5) {
		desired_phy_type = PHY_TYPE_A;
		band = WLC_BAND_5G;
#ifdef WL11N
	} else if (NetworkTypeInUse == Ndis802_11FH) {
		desired_phy_type = PHY_TYPE_N;
	} else if (NetworkTypeInUse == Ndis802_11NetworkTypeMax) {
		desired_phy_type = PHY_TYPE_N;
		band = WLC_BAND_5G;
#endif /* WL11N */
	} else {
		ASSERT(NetworkTypeInUse == Ndis802_11DS);
		desired_phy_type = PHY_TYPE_B;
	}

	for (i = 0; i < wl->phy.num_phy_ids; i++) {
		if ((wl->phy.phy_id2type[i] == desired_phy_type) &&
		    ((desired_phy_type != PHY_TYPE_N) ||
		     (wl->phy.nphy_band[i] == band)))
			return i;
	}

	ASSERT(wl->phy.num_phy_ids);
	return 0;
}
#endif /* EXT_STA */

/*
 * Port specific event handler
 *
 * NOTE: In dongle mode need to explicitly register for events we are interested.
 * See code in wl_minit above.
 * Event has to be translated n2h prior to this function call
 */
void
wl_ndisevent(wl_info_t *wl, wl_event_msg_t *e, void *data)
{
	uint event_type, flags;
	event_type = e->event_type;
	flags = e->flags;
#if defined(BCMDHDUSB) && defined(EXT_STA)
	if (wl->devhalted == TRUE) {
		WL_ERROR(("wl_ndisevent (%d): Dev is halted\n", event_type));
		return;
	}
#endif /* BCMDHDUSB && EXT_STA */

	if ((event_type != WLC_E_TRACE) && (event_type != WLC_E_RADIO) && \
	(event_type != WLC_E_NDIS_LINK) && (event_type != WLC_E_PROBREQ_MSG)) {
		WL_TRACE(("wl_ndisevent got event=%d\n", event_type));
	}

	switch (event_type) {
#ifndef EXT_STA
	case WLC_E_NDIS_LINK:
		if (flags&WLC_EVENT_MSG_LINK) {
#ifdef __BBSOFT__
			if (wl->dbgBSSEvent)
				WL_PRINT(("BSS_EVENT : BSSID %02x:%02x:%02x:%02x:%02x:%02x\n",
						e->addr.octet[0], e->addr.octet[1], e->addr.octet[2],
						e->addr.octet[3], e->addr.octet[4], e->addr.octet[5]));
#endif

			wl_link_up(wl);
		} else
			wl_link_down(wl);
		break;
	case WLC_E_MIC_ERROR:
		wl_mic_error(wl, &e->addr,
			flags&WLC_EVENT_MSG_GROUP,
			flags&WLC_EVENT_MSG_FLUSHTXQ);
		break;
#ifdef BCMWPA2
	case WLC_E_PMKID_CACHE: {
		uint len;
		pmkid_cand_list_t* pmkid_list = (pmkid_cand_list_t*)data;
		struct {
			NDIS_802_11_STATUS_INDICATION Status;
			NDIS_802_11_PMKID_CANDIDATE_LIST pmkid_list;
			PMKID_CANDIDATE foo[MAXPMKID - 1];
		} pmkidreq;

		if (pmkid_list && pmkid_list->npmkid_cand) {
			/* fill in candidate buffer */
			len = wl_pmkid_cache_ind(pmkid_list, (void*)&pmkidreq);
			NdisMIndicateStatus(wl->sh.adapterhandle,
				NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
				(void*)&pmkidreq, len);
			NdisMIndicateStatusComplete(wl->sh.adapterhandle);
		}
		break;
	}
#endif /* BCMWPA2 */
#ifdef WLBTAMP_HIGH_ONLY
	case WLC_E_SET_SSID:
			wl->pub->associated = (e->status == WLC_E_STATUS_SUCCESS);
		break;
	case WLC_E_DISASSOC:
	case WLC_E_DEAUTH_IND:
	case WLC_E_DISASSOC_IND:
			wl->pub->associated = 0;
		break;
#endif /* WLBTAMP_HIGH_ONLY */

#else /* !EXT_STA */

	case WLC_E_NDIS_LINK:
		/* - periodically report link speed and link quality */

		if (--wl->link_report == 0) {
			DOT11_LINK_QUALITY_PARAMETERS *qbuf;
			DOT11_LINK_QUALITY_ENTRY *qlist;
			uint qbuf_sz;
			scb_val_t scb_val;
			int rssi;
#ifdef NDIS60
			ULONG link_speed;

			if (wl_ioctl(wl, WLC_GET_RATE, &link_speed, sizeof(link_speed)))
				link_speed = 108;
			if (wl->abrateconvert)
				wl_abrate_convert(wl, &link_speed);
			link_speed *= 500000;

			wl_ind_link_state(wl, link_speed);
#endif /* NDIS60 */

			/* report link quality */
			qbuf_sz = sizeof(DOT11_LINK_QUALITY_PARAMETERS) +
				sizeof(DOT11_LINK_QUALITY_ENTRY);
			qbuf = MALLOC(wl->sh.osh, qbuf_sz);
			if (qbuf == NULL) {
				WL_ERROR(("wl%d: failed to alloc qbuf for link quality"
					" indication\n", wl->sh.unit));
				break;
			}

			qbuf->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
			qbuf->Header.Revision = DOT11_LINK_QUALITY_PARAMETERS_REVISION_1;
			qbuf->Header.Size = sizeof(DOT11_LINK_QUALITY_PARAMETERS);

			qbuf->uLinkQualityListSize = 1;
			qbuf->uLinkQualityListOffset = sizeof(DOT11_LINK_QUALITY_PARAMETERS);

			qlist = (DOT11_LINK_QUALITY_ENTRY *)(((uchar *)qbuf) +
			    qbuf->uLinkQualityListOffset);
			bzero(qlist->PeerMacAddr, ETHER_ADDR_LEN);
			bzero(&scb_val, sizeof(scb_val_t));
			wl_ioctl(wl, WLC_GET_RSSI, &scb_val, sizeof(scb_val_t));
			rssi = scb_val.val;
			qlist->ucLinkQuality = (uchar)wl_Rssi2LQ(rssi);

			shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_LINK_QUALITY,
				qbuf, qbuf_sz);
			MFREE(wl->sh.osh, qbuf, qbuf_sz);

			wl->link_report = SW_TIMER_LINK_REPORT;
		}
		break;

	case WLC_E_MIC_ERROR:
		/* N.B.: key index passed through auth_type event field */
		wl_mic_error(wl, &e->addr, e->auth_type,
		    ((flags & WLC_EVENT_MSG_GROUP) != 0),
		    ((flags & WLC_EVENT_MSG_FLUSHTXQ) != 0));
		break;

#ifdef BCMWPA2
	case WLC_E_PMKID_CACHE: {
		uint len;
		pmkid_cand_list_t* pmkid_list = (pmkid_cand_list_t*)data;
		DOT11_PMKID_CANDIDATE_LIST_PARAMETERS *pmkidreq;

		if (pmkid_list && pmkid_list->npmkid_cand) {
			uint num_cand = ntoh32(pmkid_list->npmkid_cand);

			len = sizeof(DOT11_PMKID_CANDIDATE_LIST_PARAMETERS) +
				(num_cand * sizeof(DOT11_BSSID_CANDIDATE));
			pmkidreq = MALLOC(wl->sh.osh, len);
			if (pmkidreq == NULL) {
			    WL_ERROR(("wl%d: PMKID candidate list alloc failed\n", wl->sh.unit));
				break;
			}

			/* fill in candidate buffer */
			(void)wl_pmkid_cache_ind(pmkid_list, (void*)pmkidreq);

			shared_indicate_status(wl->sh.adapterhandle,
				NDIS_STATUS_DOT11_PMKID_CANDIDATE_LIST, pmkidreq, len);
			MFREE(wl->sh.osh, pmkidreq, len);
		}
		break;
	}
#endif /* BCMWPA2 */

#ifndef NDIS60
	case WLC_E_RESET_COMPLETE:
		NdisMSetInformationComplete(wl->sh.adapterhandle, NDIS_STATUS_SUCCESS);
		break;
#else /* NDIS60 */
	case WLC_E_RESET_COMPLETE: {
		DOT11_STATUS_INDICATION *StatusBuffer;


		StatusBuffer = (DOT11_STATUS_INDICATION *)
		                wl->pended_oid->DATA.METHOD_INFORMATION.InformationBuffer;

		StatusBuffer->uStatusType = DOT11_STATUS_RESET_CONFIRM;
		StatusBuffer->ndisStatus = NDIS_STATUS_SUCCESS;

		NdisMOidRequestComplete(wl->sh.adapterhandle, wl->pended_oid, NDIS_STATUS_SUCCESS);
		wl->pended_oid = NULL;
		break;
	}
#endif /* NDIS60 */

	case WLC_E_JOIN:
		/* Infrastructure join processed immediately after association completion */
		/* N.B.: bss_type passed through auth_type event field */
		if (e->auth_type != DOT11_BSSTYPE_INDEPENDENT)
			break;
		/* FALL THROUGH */
	case WLC_E_START:
		wl_ind_connection_start(wl, e);
		break;

	case WLC_E_JOIN_START:
		/* IBSS join/start processed above, not here */
		/* N.B.: bss_type passed through auth_type event field */
		if (e->auth_type == DOT11_BSSTYPE_INDEPENDENT)
			break;

		wl_ind_connection_start(wl, e);
		break;

	case WLC_E_REASSOC:
		if (e->status == WLC_E_STATUS_UNSOLICITED) {
			WL_INFORM(("wl%d: %s: ignoring WLC_E_STATUS_UNSOLICITED event 0x%x\n",
			    wl->sh.unit, __FUNCTION__, e->event_type));
			break;
		}

		/* N.B.: assoc_state check passed through reason event field */
		if (e->status != WLC_E_STATUS_ABORT || e->reason != 0) {
			/* 
			 * NEED to indicate successful roam completion here...
			 * WLC_E_ROAM is fine for failure case, but isn't indicated
			 * in successful case until after beacon is received, creating a
			 * window in which a deauth might be received resulting in a roam
			 * start before the roam completion
			 */
			wl_ind_assoc_completion(wl, e, FALSE, TRUE, FALSE);

			wl_ind_roam_complete(wl, e->status, FALSE);
		}
		break;

	case WLC_E_ROAM:
		/*
		 * Sync up the assoc_type with dongle.
		 */
		WL_ASSOC(("%s: event_type=0x%x, assoc_type=%d --> AS_IDLE(%d)\n", \
					__FUNCTION__, event_type, wl->wlc->assoc_type, AS_IDLE));
		wl->wlc->assoc_type = AS_IDLE;
		break;
	case WLC_E_ROAM_START:
		/*
		 * Sync up the assoc_type with dongle.
		 */
		WL_ASSOC(("%s: event_type=0x%x, assoc_type=%d --> AS_ROAM(%d)\n", \
					__FUNCTION__, event_type, wl->wlc->assoc_type, AS_ROAM));
		wl->wlc->assoc_type = AS_ROAM;
		break;

	case WLC_E_ASSOC_START:
		/* indicate roam start if roam */
		if (wl->wlc->assoc_type != AS_ASSOCIATION) {
			DOT11_ROAMING_START_PARAMETERS StatusBuffer;
			uint reason;

			if (event_type == WLC_E_ASSOC_START)
			{
				wl_close_ind_blocks(wl);
			}

			WL_INFORM(("wl%d: EXT STA indication: %s\n", wl->sh.unit, \
				(event_type == WLC_E_ASSOC_START ? \
				 "ROAMING START" : "CCX ROAMING START")));

			bzero(&StatusBuffer, sizeof(DOT11_ROAMING_START_PARAMETERS));
			StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
			StatusBuffer.Header.Revision = DOT11_ROAMING_START_PARAMETERS_REVISION_1;
			StatusBuffer.Header.Size = sizeof(DOT11_ROAMING_START_PARAMETERS);

			if (e->auth_type != DOT11_BSSTYPE_INFRASTRUCTURE) {
			}

			/* on fast roam failure, preserve previous roam reason */
		{
			ULONG BytesRead, BytesNeeded;

			wl_queryoid(wl, OID_STA_GET_ROAM_REASON, \
					&wl->oid->prev_roam_reason, \
					sizeof(wl->oid->prev_roam_reason), \
					&BytesRead, &BytesNeeded, 0);
		}
			if (e->reason == WLC_E_REASON_FAST_ROAM_FAILED)
				reason = wl->oid->prev_roam_reason;
			else
				reason = e->reason;
		if (reason != wl->oid->prev_roam_reason) {
			ULONG BytesWritten, BytesNeeded;

			wl_setoid(wl, OID_STA_SET_ROAM_REASON, &reason, sizeof(reason), \
					&BytesWritten, &BytesNeeded, 0);
		}
			wl->oid->prev_roam_reason = reason;

			if (reason == WLC_E_REASON_LOW_RSSI)
				StatusBuffer.uRoamingReason = \
						  DOT11_ASSOC_STATUS_ROAMING_BETTER_AP_FOUND;
			else
				/* DEAUTH, DISASSOC, BCNS_LOST, FAST_ROAM_FAILED */
				StatusBuffer.uRoamingReason = \
						  DOT11_ASSOC_STATUS_ROAMING_ASSOCIATION_LOST;

			{
				shared_indicate_status(wl->sh.adapterhandle, \
						NDIS_STATUS_DOT11_ROAMING_START, \
						&StatusBuffer, sizeof(StatusBuffer));

				wl->blocks |= IND_BLOCK_ROAM;
			}


			/* 
			 * The following code is removed to comply with NDIS stack
			 * Whenever the 802.11 station implicitly disassociates from
			 * an AP or peer station, the miniport driver must NOT make the
			 * NDIS_STATUS_DOT11_DISASSOCIATION indication.
			 */

			/*
			{
				DOT11_DISASSOCIATION_PARAMETERS StatusBufferDis;

				bzero(&StatusBufferDis, sizeof(DOT11_DISASSOCIATION_PARAMETERS));

				StatusBufferDis.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
				StatusBufferDis.Header.Revision =
					DOT11_DISASSOCIATION_PARAMETERS_REVISION_1;
				StatusBufferDis.Header.Size =
					sizeof(DOT11_DISASSOCIATION_PARAMETERS);

				StatusBufferDis.uReason = DOT11_ASSOC_STATUS_UNREACHABLE;

				bcopy(wl->wlc->assoc_bssid.octet, &StatusBufferDis.MacAddr,
					ETHER_ADDR_LEN);

				shared_indicate_status(wl->sh.adapterhandle,
					NDIS_STATUS_DOT11_DISASSOCIATION,
					&StatusBufferDis, sizeof(StatusBufferDis));
			}
			*/
		}
		bcopy(e->addr.octet, wl->wlc->assoc_bssid.octet, ETHER_ADDR_LEN);

		/* IBSS "association" event below... */
		/* N.B.: bss_type passed through auth_type event field */
		if (e->auth_type == DOT11_BSSTYPE_INDEPENDENT)
			break;

		/* FALL THROUGH */
	case WLC_E_IBSS_ASSOC: {
		DOT11_ASSOCIATION_START_PARAMETERS StatusBuffer;
		ibss_peer_t *peer;
		/* ASSERT(!(wl->blocks & IND_BLOCK_ASSOC)); */
		if (!(wl->blocks & IND_BLOCK_ASSOC)) {
			WL_INFORM(("%s: IND_BLOCK_ASSOC error blocks 0x%x\n",
				__FUNCTION__, wl->blocks));
		}

		if (wl->wlc->assoc_type != AS_ROAM && wl->wlc->assoc_type != AS_RECREATE) {
			WL_ERROR(("%s: blocks %d event_type %d\n",
				__FUNCTION__, wl->blocks, e->event_type));
			/*
			ASSERT((wl->blocks & IND_BLOCK_CONNECT) || \
					(e->event_type == WLC_E_IBSS_ASSOC));
			*/
			if (!((wl->blocks & IND_BLOCK_CONNECT) ||
				(e->event_type == WLC_E_IBSS_ASSOC))) {
				WL_INFORM(("%s: IND_BLOCK_CONNECT error blocks 0x%x\n",
					__FUNCTION__, wl->blocks));
			}
		}

		WL_INFORM(("wl%d: EXT STA indication: %s\n", wl->sh.unit, "ASSOCIATION START"));

		bzero(&StatusBuffer, sizeof(DOT11_ASSOCIATION_START_PARAMETERS));

		StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
		StatusBuffer.Header.Revision = DOT11_ASSOCIATION_START_PARAMETERS_REVISION_1;
		StatusBuffer.Header.Size = sizeof(DOT11_ASSOCIATION_START_PARAMETERS);

		bcopy(e->addr.octet, StatusBuffer.MacAddr, ETHER_ADDR_LEN);
		/* N.B.: bss_type passed through auth_type event field */
		if (e->auth_type == DOT11_BSSTYPE_INFRASTRUCTURE) {
			StatusBuffer.SSID.uSSIDLength = wl->oid->desired_ssid.SSID_len;
			bcopy(wl->oid->desired_ssid.SSID, StatusBuffer.SSID.ucSSID,
			      StatusBuffer.SSID.uSSIDLength);
		}

		/* StatusBuffer.uIHVData{Offset,Size} zeroed above... */

		shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_ASSOCIATION_START,
		    &StatusBuffer, sizeof(StatusBuffer));

		wl->blocks |= IND_BLOCK_ASSOC;

		if (e->event_type == WLC_E_ASSOC_START)
			break;

		ASSERT(e->auth_type == DOT11_BSSTYPE_INDEPENDENT);

		/* Synthesize IBSS "association" completion */
		wl_ind_assoc_completion(wl, e, FALSE, FALSE, FALSE);

		/* cache relevant information */
		if (wl->oid->num_peers == MAXBSS) {
			WL_ERROR(("wl%d: IBSS peer table overflow\n", wl->sh.unit));
			break;
		}
#if defined(EXT_STA)
		if (wl->oid->num_peers > 0)
			peer = &wl->oid->peers[wl->oid->num_peers-1];
		else
			peer = &wl->oid->peers[0];
#else
		peer = &wl->oid->peers[wl->oid->num_peers++];
#endif

		bcopy(e->addr.octet, peer->addr.octet, ETHER_ADDR_LEN);
#ifndef EXT_STA
		peer->cap = wl->pub->current_bss.bcn_prb->capability;
#else
		{
			wlc_bss_info_t *cur_bss;

			cur_bss = wl_get_current_bss(wl);
			if (!cur_bss) {
				break;
			}
			if (cur_bss->bcn_prb)
				peer->cap = cur_bss->bcn_prb->capability;
			else
				ASSERT(cur_bss->bcn_prb);

			bcopy((char *)&cur_bss->rateset, &peer->rateset, sizeof(wlc_rateset_t));
		}
#endif /* EXT_STA */
		peer->assoc_time = wl->oid->assoc_time;

#ifdef BCMDBG
		if (WL_INFORM_ON()) {
			char eabuf[ETHER_ADDR_STR_LEN];
			uint i;

			WL_INFORM(("wl%d: IBSS peer table - num_peers = %d\n",
			    wl->sh.unit, wl->oid->num_peers));
			for (i = 0; i < wl->oid->num_peers; i++)
				WL_INFORM(("wl%d: peers[%d] = %s\n", wl->sh.unit, i,
				    bcm_ether_ntoa(&wl->oid->peers[i].addr, eabuf)));
		}
#endif /* BCMDBG */
		break;
	}

	case WLC_E_AUTH:
		if ((e->status == WLC_E_STATUS_NO_ACK) ||
		    (e->status == WLC_E_STATUS_TIMEOUT) ||
			(e->status == WLC_E_STATUS_FAIL)) {
			wl_ind_assoc_completion(wl, e, TRUE, FALSE, FALSE);
			if (wl->wlc->assoc_type != AS_ASSOCIATION)
				wl_ind_roam_complete(wl, WLC_E_STATUS_FAIL, FALSE);
		}
		break;

	case WLC_E_ASSOC:
		if (e->status == WLC_E_STATUS_UNSOLICITED) {
			WL_INFORM(("wl%d: %s: ignoring WLC_E_STATUS_UNSOLICITED event 0x%x\n",
			    wl->sh.unit, __FUNCTION__, e->event_type));
			break;
		}

		/* N.B.: assoc_state check passed through reason event field */
		if (e->status != WLC_E_STATUS_ABORT || e->reason != 0)
			wl_ind_assoc_completion(wl, e, FALSE, FALSE, FALSE);
		/* 
		 * NEED to indicate successful connect or roam completion here...
		 * WLC_E_SET_SSID is fine for failure case, but isn't indicated
		 * in successful case until after beacon is received, creating a
		 * window in which a deauth might be received resulting in a roam
		 * start before the connect completion
		 *
		 * Fall through to WLC_E_SET_SSID in successful case!
		 */

		/*
		if (e->status != WLC_E_STATUS_SUCCESS)
			break;
		*/

		/* FALL THROUGH */
		/* Remove fall-through. The dongle has not completed the assoc yet,
		 * doing this will result in 
		 * failure for any following requests from NDIS before dongle sent SET_SSID.
		 */
		break;

	case WLC_E_SET_SSID:
		if (wl->blocks & IND_BLOCK_CONNECT)
			wl_ind_connect_complete(wl, e->status);

		WL_ASSOC(("%s: wl->wlc->assoc_type=%d --> AS_IDLE(%d)\n", \
					__FUNCTION__, wl->wlc->assoc_type, AS_IDLE));
		wl->wlc->assoc_type = AS_IDLE;

		/* Need to synthesize an IBSS connection start here */
		if (e->auth_type == DOT11_BSSTYPE_INDEPENDENT &&
		    e->status != WLC_E_STATUS_SUCCESS)
			wl_ind_connection_start(wl, e);

		break;

	case WLC_E_DISASSOC:
	case WLC_E_DEAUTH_IND:
	case WLC_E_DISASSOC_IND: {
		DOT11_DISASSOCIATION_PARAMETERS StatusBuffer;
		bool leaving_ibss = FALSE;
		uint i;

		WL_INFORM(("%s: EXT STA indication: DISASSOCIATION(%d), reason=0x%x\n", \
					__FUNCTION__, event_type, e->reason));

		bzero(&StatusBuffer, sizeof(DOT11_DISASSOCIATION_PARAMETERS));

		StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
		StatusBuffer.Header.Revision = DOT11_DISASSOCIATION_PARAMETERS_REVISION_1;
		StatusBuffer.Header.Size = sizeof(DOT11_DISASSOCIATION_PARAMETERS);

		if (e->auth_type == DOT11_BSSTYPE_INDEPENDENT &&
		    e->reason == DOT11_RC_DISASSOC_LEAVING) {
			leaving_ibss = TRUE;
			bcopy(&ether_bcast, &StatusBuffer.MacAddr, ETHER_ADDR_LEN);
		} else {
			bcopy(e->addr.octet, &StatusBuffer.MacAddr, ETHER_ADDR_LEN);
		}
		if (e->event_type == WLC_E_DEAUTH_IND) {
			StatusBuffer.uReason = DOT11_ASSOC_STATUS_PEER_DEAUTHENTICATED |
				((uint16)e->reason);
		} else if (e->event_type == WLC_E_DISASSOC_IND) {
			StatusBuffer.uReason = DOT11_ASSOC_STATUS_PEER_DISASSOCIATED |
				((uint16)e->reason);
		} else if (e->event_type == WLC_E_DISASSOC) {
			{
				ULONG BytesRead, BytesNeeded;

				/* int_disassoc may no longer be valid when reading from dongle
				 * so check for reason DOT11_RC_DISASSOC_LEAVING to set
				 * uReason which must not be zero for WHQL/DTM
				 */
				wl_queryoid(wl, OID_STA_INT_DISASSOC, &wl->oid->int_disassoc,
					sizeof(wl->oid->int_disassoc), &BytesRead, &BytesNeeded, 0);
			}
			if (
			e->reason == DOT11_RC_DISASSOC_LEAVING ||
			wl->oid->int_disassoc) {

				StatusBuffer.uReason = DOT11_ASSOC_STATUS_DISASSOCIATED_BY_OS;
			} else if (e->reason == DOT11_RC_INACTIVITY) {
				StatusBuffer.uReason = DOT11_ASSOC_STATUS_UNREACHABLE;
			} else {
				/* 
				 * N.B.: I'm not sure this is really a valid inference, but it'll
				 *       do for now.  If we are disassociating and the hw radio
				 *       switch is off, that's probably why we're leaving...
				 */
				wl_ioctl(wl, WLC_GET_RADIO, &i, sizeof(i));
				if (mboolisset((mbool)i,
				    (WL_RADIO_HW_DISABLE | WL_RADIO_SW_DISABLE))) {
					StatusBuffer.uReason = DOT11_ASSOC_STATUS_RADIO_OFF;
				}
			}
		}

		/* StatusBuffer.uIHVData{Offset,Size} zeroed above */

		shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_DISASSOCIATION,
		    &StatusBuffer, sizeof(StatusBuffer));

		/* Set the event to wake up wl_pnp_set_power_normal */
		wl_pnp_set_wifioffevent(wl);

		wl->pub->associated = 0;
		wl->blocks &= ~IND_BLOCK_CONNECT;
		wl->bss_good = FALSE;
#if defined(EXT_STA)
		wl->pwk = 0;    /* Reset pairwise key flag */
#endif /* EXT_STA */

		/* N.B.: bss_type passed through auth_type event field */
		if (e->auth_type == DOT11_BSSTYPE_INDEPENDENT) {
			if (leaving_ibss) {
				wl->oid->num_peers = 0;
			} else {
				for (i = 0; i < wl->oid->num_peers; i++)
					if (!bcmp(wl->oid->peers[i].addr.octet, e->addr.octet,
					    ETHER_ADDR_LEN))
						break;
				ASSERT(i != wl->oid->num_peers);
				for (i = i + 1; i < wl->oid->num_peers; i++)
					bcopy(&wl->oid->peers[i], &wl->oid->peers[i - 1],
					      sizeof(ibss_peer_t));
				wl->oid->num_peers--;
			}
#ifdef BCMDBG
			if (WL_INFORM_ON()) {
				char eabuf[ETHER_ADDR_STR_LEN];

				WL_INFORM(("wl%d: IBSS peer table - num_peers = %d\n",
				    wl->sh.unit, wl->oid->num_peers));
				for (i = 0; i < wl->oid->num_peers; i++)
					WL_INFORM(("wl%d: peers[%d] = %s\n", wl->sh.unit, i,
					    bcm_ether_ntoa(&wl->oid->peers[i].addr, eabuf)));
			}
#endif /* BCMDBG */
		}

		break;
	}

	case WLC_E_RADIO:
	{
		uint i;
		BOOLEAN hw_state;

		wl_ioctl(wl, WLC_GET_RADIO, &i, sizeof(i));
		if (wl->last_phyind == (mbool)(i & WL_RADIO_HW_DISABLE))
			break;

		WL_INFORM(("wl%d: HARDWARE PHY STATE CHANGED\n", wl->sh.unit));

		wl->last_phyind = (mbool)(i & WL_RADIO_HW_DISABLE);
		hw_state = (wl->last_phyind == 0);

		wl_ind_phy_state(wl, hw_state, mboolisset(wl->oid->sw_phy_state, 1));

		break;
	}

#ifndef SIMPLE_ISCAN
	case WLC_E_SCAN_CONFIRM_IND:
		wl_ind_scan_confirm(wl, NDIS_STATUS_SUCCESS);
		break;
#endif /* ! SIMPLE_ISCAN */
#endif /* EXT_STA */

#ifdef WL_IW_USE_ISCAN
	case WLC_E_SCAN_COMPLETE:
		wl_e_scan_complete(wl);
		break;
#endif /* WL_IW_USE_ISCAN */

#ifdef SIMPLE_ISCAN
	case WLC_E_SCAN_COMPLETE:
		dhd_iscan_notify();
		break;
#endif /* SIMPLE_ISCAN */

#ifdef WLBTAMP
#ifndef WLBTAMP_HIGH_ONLY
	case WLC_E_BTA_HCI_EVENT:
#ifdef BCMDBG
		if (DHD_BTA_ON())
			dhd_bta_hcidump_evt(dhd_bus_pub(wl->dhd), (amp_hci_event_t *)data);
#endif
#if defined(WLBTWUSB) || defined(WLBTCEAMP11)
		BtKernForwardEvent(wl, (PVOID)data, (ULONG)e->datalen);
#endif
		break;
#endif /* !WLBTAMP_HIGH_ONLY */
#endif /* WLBTAMP */

	default:
		break;
	}
}


/* set named driver variable to int value and return error indication */
int
wl_iovar_setint(wl_info_t *wl, const char *name, int val)
{
	char buf[WLC_IOCTL_SMLEN];
	uint len;

	len = bcm_mkiovar((char *)name, (char *)(&val), sizeof(val), buf, sizeof(buf));
	ASSERT(len);

	return (wl_ioctl(wl, WLC_SET_VAR, buf, len));
}

int
wl_iovar_getint(wl_info_t *wl, const char *name, int *val)
{
	char buf[WLC_IOCTL_SMLEN];
	int error;
	uint len;
	uint data_null;

	len = bcm_mkiovar((char *)name, (char *)(&data_null), 0, buf, sizeof(buf));
	ASSERT(len);
	error = wl_ioctl(wl, WLC_GET_VAR, buf, len);

	*val = *(int *)buf;

	return (error);
}

/* wlif is placeholder, real NDIS adapter handler comes from wl->sh.adapterhandle */
void
wl_sendup(wl_info_t *wl, struct wl_if *wlif, void *pkt, int numpkt)
{
	NDIS_STATUS * status = NULL;
	ND_PKT *p;
	void *pnext;
	int k, pkts_to_indicate;
	int ifidx;

#ifdef NDIS60
	uint ReceiveFlags = NDIS_RECEIVE_FLAGS_DISPATCH_LEVEL;
	ND_PKT *head, *prev, *next;
#else /* NDIS60 */
	PNDIS_PACKET *pktarray = NULL;
#endif /* NDIS60 */

#ifdef EXT_STA
	DOT11_EXTSTA_RECV_CONTEXT ctxt;
	uint32	priority;
	struct lbuf *lb = (struct lbuf *)pkt;
	int i;
	rx_ctxt_t *wlc_ctxt;
#endif /* EXT_STA */

#ifndef NDIS60
	pktarray = MALLOC(wl->sh.osh, numpkt * sizeof(PNDIS_PACKET));

	if (!pktarray) {
		WL_ERROR(("wl%d: wl_sendup can't allocate pkt array of %d\n", wl->sh.unit, numpkt));
		goto error;
	}

	status = MALLOC(wl->sh.osh, numpkt * sizeof(NDIS_STATUS));

	if (!status) {
		WL_ERROR(("wl%d: wl_sendup can't allocate status array of %d\n",
		          wl->sh.unit, numpkt));
		goto error;
	}
#else /* !NDIS60 */
	head = NULL;
	prev = NULL;
#endif /* !NDIS60 */

	for (k = 0, pkts_to_indicate = 0; pkt && k < numpkt; k++, pkt = pnext) {

		pnext = PKTNEXT(wl->sh.osh, pkt);
		PKTSETNEXT(wl->sh.osh, pkt, NULL);

		ifidx =  ((struct lbuf *)pkt)->ifidx;
#ifdef EXT_STA
		/* The receive context structure was appended to the packet buffer. */
		lb = (struct lbuf *)pkt;
		wlc_ctxt = (rx_ctxt_t *)&((lb->data)[lb->len]);

		/* wmm priority is delivered to Vista in an OOB call */
		priority = PKTPRIO(lb);

		/*
		 * Prepend an 802.11 header if there is none ... this will be
		 * the case if the frame came out of an amsdu or cram packet.
		 */
		if (wlc_ctxt->prepend_dot11_hdr) {
			struct ether_header *eh; /* ptr to 802.3 or ether hdr in incoming pkt */
			struct dot11_llc_snap_header *lsh;
			struct ether_header *neh; /* ptr to 802.3 or ether hdr in reformatted pkt */
			struct dot11_header *phdr;

			/*
			 * If we discover an ethernet header, replace it by an
			 * 802.3 hdr + SNAP header.
			 */
			eh = (struct ether_header *)PKTDATA(wl->sh.osh, pkt);
			if (ntoh16(eh->ether_type) > ETHER_MAX_LEN) {
				int body_len = PKTLEN(wl->sh.osh, pkt) - ETHER_HDR_LEN;
				neh = (struct ether_header *)
				        PKTPUSH(wl->sh.osh, pkt, DOT11_LLC_SNAP_HDR_LEN);

				/* Construct 802.3 header (not really needed because it's
				 * going to be overwritten by the 802.11 header.
				 */
				bcopy((char *)eh->ether_dhost,
				      (char *)neh->ether_dhost, ETHER_ADDR_LEN);
				bcopy((char *)eh->ether_shost,
				      (char *)neh->ether_shost, ETHER_ADDR_LEN);
				neh->ether_type = hton16(body_len+DOT11_LLC_SNAP_HDR_LEN);

				/* Construct LLC SNAP header */
				lsh = (struct dot11_llc_snap_header *)((char *)neh + ETHER_HDR_LEN);
				lsh->dsap = 0xaa;
				lsh->ssap = 0xaa;
				lsh->ctl = 0x03;
				lsh->oui[0] = 0;
				lsh->oui[1] = 0;
				lsh->oui[2] = 0;
				/*
				 * The snap type code is already in place, inherited from the
				 * ethernet header that is now overlaid.
				 */
			} else {
				neh = (struct ether_header *)PKTDATA(wl->sh.osh, pkt);
			}

			/* Force the 802.11 a3 address to be the ethernet source address */
			bcopy((char *)neh->ether_shost,
			      (char *)&wlc_ctxt->dot11_hdr.a3, ETHER_ADDR_LEN);

			/*
			 * Replace the 802.3 header, if present, by an 802.11 header
			 * The original 802.11 header was appended to the frame along
			 * with the receive data needed by microsoft.
			 */
			phdr = (struct dot11_header *)
				PKTPUSH(wl->sh.osh, pkt, DOT11_A3_HDR_LEN - ETHER_HDR_LEN);
			bcopy((char *)&wlc_ctxt->dot11_hdr, (char *)phdr, DOT11_A3_HDR_LEN);

			/*
			 * Clear all frame control bits except version, type,
			 * data subtype & from-ds
			 */
			phdr->fc = htol16(ltoh16(phdr->fc) & (FC_FROMDS | FC_TYPE_MASK |
			                 (FC_SUBTYPE_MASK & ~QOS_AMSDU_MASK) | FC_PVER_MASK));
		}
#endif /* EXT_STA */


		ND_RXLOCK(wl);

		if (!(p = PKTTONATIVE(wl->sh.osh, pkt))) {
			WL_ERROR(("wl%d: rx: PKTTONATIVE failed \n", wl->sh.unit));
			wl->rxnondisbuf++;
			/* Stop processing any more packets and deliver the packets
			 * which has been successfully converted into NDIS stack
			 */
			ND_RXUNLOCK(wl);
			break;
		}

		ND_RXUNLOCK(wl);

#ifndef NDIS60
		if (wl->sh.rxfree.count > NRXBUFLOW) {
			status[pkts_to_indicate] = NDIS_STATUS_SUCCESS;
		} else {
			status[pkts_to_indicate] = NDIS_STATUS_RESOURCES;
			WL_INFORM(("wl%d: wl_sendup: Low on pkt resources to receive\n", \
						wl->sh.unit));
		}
		pktarray[pkts_to_indicate] = p;
		NDIS_SET_PACKET_STATUS(p, status[pkts_to_indicate]);
#else /* !NDIS60 */
		if (wl->sh.rxfree.count <= NRXBUFLOW) {
			ReceiveFlags |= NDIS_RECEIVE_FLAGS_RESOURCES;
			WL_INFORM(("wl%d: wl_sendup: Low on pkt resources to receive\n", \
						wl->sh.unit));
		}
		/* Chain/link the packet togather before delivery to NDIS */
		if (!head)
			head = p;
		NET_BUFFER_LIST_NEXT_NBL(p) = NULL;
		if (prev)
			NET_BUFFER_LIST_NEXT_NBL(prev) = p;
		prev = p;
#endif /* !NDIS60 */

#ifdef EXT_STA
		ctxt.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
		ctxt.Header.Revision = DOT11_EXTSTA_RECV_CONTEXT_REVISION_1;
		ctxt.Header.Size = sizeof(DOT11_EXTSTA_RECV_CONTEXT);

		ctxt.uReceiveFlags = 0;

		if (WME_ENAB(wl->pub)) {
			SET_NDIS_PRIORITY(p, priority);
		}

		for (i = 0; i < (int)wl->oid->phy.num_phy_ids; i++) {
			if (
				((wl->oid->phy.phy_id2type[i] == wlc_ctxt->phy_type) &&
				((wlc_ctxt->phy_type != PHY_TYPE_N))))
					break;
			if (
				((wlc_ctxt->phy_type == PHY_TYPE_LP)))
					break;
		}
		if (i == wl->oid->phy.num_phy_ids) {
			WL_ERROR(("wl_sendup error: phytype=%d not found\n",
				wlc_ctxt->phy_type));
		}
		ASSERT(i != wl->oid->phy.num_phy_ids);
		ctxt.uPhyId = i;

		ctxt.usNumberOfMPDUsReceived = wlc_ctxt->numfrags;
		ctxt.lRSSI = wlc_ctxt->rssi;
		ctxt.ucDataRate = wlc_ctxt->rate & DOT11_DATA_RATE_INDEX_MASK;

		/* N.B.: Ignore TimeStamp until network monitor mode supported... */

#ifndef NDIS60
		NDIS_SET_PACKET_MEDIA_SPECIFIC_INFO(p, &ctxt, sizeof(ctxt));
#else
		NET_BUFFER_LIST_INFO(p, MediaSpecificInformation) = &ctxt;
#endif /* !NDIS60 */

#endif	/* EXT_STA */

		/* Increment the pkt to indicate into NDIS stack */
		pkts_to_indicate++;
	}

	if (pkts_to_indicate != numpkt)
		WL_INFORM(("wl%d: wl_sendup: Mismatch in paket queued and sentup " \
					"into NDIS stack pkts_to_indicate = %d, numpkt = %d\n", \
					wl->sh.unit, pkts_to_indicate, numpkt));

	/* send packet up */
	wl->rxind += pkts_to_indicate;

	if (pkts_to_indicate) {
		ND_UNLOCK(wl);
#ifndef NDIS60

#ifdef P2P
		/* P2P - decide which interface to Indicate packet chain */
		/* P2P code insure packet chains will not have multiple interfaces */
		/* so we can safely assume pkts will come grouped together for each Interface */

		if (ifidx && gP2P.p2p_vip_AdapterHandle) {
			NdisMIndicateReceivePacket(gP2P.p2p_vip_AdapterHandle,
				pktarray, pkts_to_indicate);
		} else
#endif /* P2P */
			NdisMIndicateReceivePacket(wl->sh.adapterhandle,
				pktarray, pkts_to_indicate);
#else
		NdisMIndicateReceiveNetBufferLists(wl->sh.adapterhandle, head, \
				0, pkts_to_indicate, ReceiveFlags);
		p = head;
#endif /* !NDIS60 */
		ND_LOCK(wl);
	}

	/* Check for the ownership of the packet indicated
	 * up to NDIS. If it is not owned by NDIS due to
	 * NDIS_STATUS_RESOURCES/NDIS_RECEIVE_FLAGS_RESOURCES,
	 * then free those right away
	 */
	for (k = 0; k < pkts_to_indicate; k++) {
#ifndef NDIS60
		if (status[k] == NDIS_STATUS_RESOURCES) {
			ND_RXLOCK(wl);
			wl->rxreturn++;
			shared_free_ndispacket(&wl->sh, pktarray[k]);
			ND_RXUNLOCK(wl);
		}
#else /* !NDIS60 */
		/* In NDIS6 NDIS_RECEIVE_FLAGS_RESOURCES apply to the entire link */
		if (p && (ReceiveFlags & NDIS_RECEIVE_FLAGS_RESOURCES)) {
			next = NET_BUFFER_LIST_NEXT_NBL(p);
			ND_RXLOCK(wl);
			wl->rxreturn++;
			shared_free_ndispacket(&wl->sh, p);
			p = next;
			ND_RXUNLOCK(wl);
		}
#endif /* !NDIS60 */
	}

#ifndef NDIS60
error:
	if (status)
		MFREE(wl->sh.osh, status, numpkt * sizeof(NDIS_STATUS));

	if (pktarray)
		MFREE(wl->sh.osh, pktarray, numpkt * sizeof(PNDIS_PACKET));
#endif /* !NDIS60 */
}

static NDIS_STATUS
wl_readconfigdata(
	IN wl_info_t *wl,
	IN NDIS_HANDLE ConfigurationHandle,
	IN NDIS_HANDLE MiniportAdapterHandle)
{
	NDIS_STATUS status;
	NDIS_STATUS ret = NDIS_STATUS_SUCCESS;
	NDIS_HANDLE confighandle = NULL;
#ifdef NDIS60
	NDIS_CONFIGURATION_OBJECT configobj;
#endif /* NDIS60 */
	PNDIS_CONFIGURATION_PARAMETER param;
	NDIS_STRING Environment = NDIS_STRING_CONST("Environment");
	NDIS_STRING NdisVersion = NDIS_STRING_CONST("NdisVersion");
	NDIS_STRING BusType = NDIS_STRING_CONST("BusType");

	/* open the registry */
#ifndef NDIS60
	NdisOpenConfiguration(&status, &confighandle, ConfigurationHandle);
#else
	bzero(&configobj, sizeof(NDIS_CONFIGURATION_OBJECT));

	configobj.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
	configobj.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
	configobj.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT);

	configobj.NdisHandle = MiniportAdapterHandle;
	configobj.Flags = 0;

	status = NdisOpenConfigurationEx(&configobj, &confighandle);
#endif /* !NDIS60 */
	if (NDIS_ERROR(status)) {
		WL_ERROR(("wl%d: wl_minit: NdisOpenConfiguration error 0x%x\n",
			wl->sh.unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle,
			NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER, 1, 19);
		ret = status;
		goto error;
	}
	ASSERT(confighandle);

	/* read drive strength to be used if it exists */
	if ((param = wl_readparam(nic_configs, "DriveStrength", confighandle, wl, NULL))) {
		wl->sh.DrvStrength = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: DrvStrength set to %d from the registry\n", wl->sh.unit,
			wl->sh.DrvStrength));
	} else {
		wl->sh.DrvStrength = 6;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"DriveStrength\") error\n",
			wl->sh.unit));
	}

	/* read drive Watchdog priority to be used if it exists */
	if ((param = wl_readparam(nic_configs, "WatchdogPriority", confighandle, wl, NULL))) {
		wl->sh.WatchdogPriority = param->ParameterData.IntegerData;
		wl->sh.wd_prio = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: WatchdogPriority set to %d from the registry\n", wl->sh.unit,
			wl->sh.WatchdogPriority));
	} else {
		wl->sh.WatchdogPriority = 251;
		wl->sh.wd_prio = 251;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"WatchdogPriority\") error\n",
			wl->sh.unit));
	}

	/* read RXThreadPriority if it exists */
	if ((param = wl_readparam(nic_configs, "RXThreadPriority", confighandle, wl, NULL))) {
		wl->sh.RXThreadPriority = param->ParameterData.IntegerData;
		wl->sh.sendup_prio = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: RXThreadPriority set to %d from the registry\n", wl->sh.unit,
			wl->sh.RXThreadPriority));
	} else {
		wl->sh.RXThreadPriority = 101;
		wl->sh.sendup_prio = 101;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"RXThreadPriority\") error\n",
			wl->sh.unit));
	}

	/* read EventPriority if it exists */
	if ((param = wl_readparam(nic_configs, "EventPriority", confighandle, wl, NULL))) {
		wl->sh.EventPriority = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: EvenPriority set to %d from the registry\n", wl->sh.unit,
			wl->sh.EventPriority));
	} else {
		wl->sh.EventPriority = 132;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"EventPriority\") error\n",
			wl->sh.unit));
	}

	/* read OidPriority if it exists */
	if ((param = wl_readparam(nic_configs, "OidPriority", confighandle, wl, NULL))) {
		wl->sh.OidPriority = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: OidPriority set to %d from the registry\n", wl->sh.unit,
			wl->sh.OidPriority));
	} else {
		wl->sh.OidPriority = 0; /* Default to off */
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"OidPriority\") error\n",
			wl->sh.unit));
	}


	/* read driver mode poll/interrupt to be used if it exists */
	if ((param = wl_readparam(nic_configs, "EnablePoll", confighandle, wl, NULL))) {
		wl->sh.EnablePoll = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: EnablePoll set to %d from the registry\n", wl->sh.unit,
			wl->sh.EnablePoll));
	} else {
		wl->sh.EnablePoll = 0;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"EnablePoll\") error\n",
			wl->sh.unit));
	}

	/* read DPCPriority if it exists */
	if ((param = wl_readparam(nic_configs, "DPCPriority", confighandle, wl, NULL))) {
		wl->sh.DPCPriority = param->ParameterData.IntegerData;
		wl->sh.dpc_prio = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: DPCPriority set to %d from the registry\n", wl->sh.unit,
			wl->sh.DPCPriority));
	} else {
		wl->sh.DPCPriority = 251;
		wl->sh.dpc_prio = 251;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"DPCPriority\") error\n",
			wl->sh.unit));
	}

	/* read RxDPCPriority if it exists */
	if ((param = wl_readparam(nic_configs, "RxDPCPriority", confighandle, wl, NULL))) {
		wl->sh.RxDPCPriority = param->ParameterData.IntegerData;
		wl->sh.rxdpc_prio = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: RxDPCPriority set to %d from the registry\n", wl->sh.unit,
			wl->sh.RxDPCPriority));
	} else {
		wl->sh.RxDPCPriority = 251;
		wl->sh.rxdpc_prio = 251;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"RxDPCPriority\") error\n",
			wl->sh.unit));
	}

	/* read RxflowMode if it exists */
	if ((param = wl_readparam(nic_configs, "RxflowMode", confighandle, wl, NULL))) {
		wl->sh.rxflowMode = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: RxflowMode set to %d from the registry\n", wl->sh.unit,
			wl->sh.rxflowMode));
	} else {
		wl->sh.rxflowMode = RX_FLOW_CONTROL_OFF;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"RxflowMode\") error\n",
			wl->sh.unit));
	}

	/* read RxflowHi if it exists */
	if ((param = wl_readparam(nic_configs, "RxflowHi", confighandle, wl, NULL))) {
		wl->sh.rxflowHi = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: RxflowMode set to %d from the registry\n", wl->sh.unit,
			wl->sh.rxflowHi));
	} else {
		wl->sh.rxflowHi = RX_FLOW_CTL_HIGH;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"RxflowHi\") error\n",
			wl->sh.unit));
	}

	/* read RxflowLow if it exists */
	if ((param = wl_readparam(nic_configs, "RxflowLow", confighandle, wl, NULL))) {
		wl->sh.rxflowLow = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: RxflowMode set to %d from the registry\n", wl->sh.unit,
			wl->sh.rxflowLow));
	} else {
		wl->sh.rxflowLow = RX_FLOW_CTL_LOW;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"RxflowLow\") error\n",
			wl->sh.unit));
	}

	/* read RxflowRxDPCPriority if it exists */
	if ((param = wl_readparam(nic_configs, "RxflowRxDPCPriority", confighandle, wl, NULL))) {
		wl->sh.RxflowRxDPCPriority = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: RxflowRxDPCPriority set to %d from the registry\n", wl->sh.unit,
			wl->sh.RxflowRxDPCPriority));
	} else {
		wl->sh.RxflowRxDPCPriority = 252;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"RxflowRxDPCPriority\") error\n",
			wl->sh.unit));
	}

	/* read RxflowRXThreadPriority if it exists */
	if ((param = wl_readparam(nic_configs, "RxflowRXThreadPriority", confighandle, wl, NULL))) {
		wl->sh.RxflowRXThreadPriority = param->ParameterData.IntegerData;

		WL_INFORM(("wl%d: RxflowRXThreadPriority set to %d from the registry\n",
			wl->sh.unit, wl->sh.RxflowRXThreadPriority));
	} else {
		wl->sh.RxflowRXThreadPriority = 101;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"RxflowRXThreadPriority\") error\n",
			wl->sh.unit));
	}

	/* read dongle image path if it exists */
	if ((param = wl_readparam(nic_configs, "DongleImagePath", confighandle, wl, NULL))) {

		wchar2ascii(wl->sh.dongleImagePath, param->ParameterData.StringData.Buffer,
			param->ParameterData.StringData.Length,
			sizeof(wl->sh.dongleImagePath));

		WL_INFORM(("wl%d: Dongle ImagePath set to %s from the registry\n", wl->sh.unit,
			wl->sh.dongleImagePath));
	} else {
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"DongleImagePath\") error\n",
			wl->sh.unit));
	}

	/* read srom image path if it exists */
	if ((param = wl_readparam(nic_configs, "SROMImagePath", confighandle, wl, NULL))) {

		wchar2ascii(wl->sh.sromImagePath, param->ParameterData.StringData.Buffer,
			param->ParameterData.StringData.Length,
			sizeof(wl->sh.sromImagePath));

		WL_INFORM(("wl%d: SRPOM ImagePath set to %s from the registry\n", wl->sh.unit,
			wl->sh.sromImagePath));
	} else {
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"SROMImagePath\") error\n", wl->sh.unit));
	}

	/* In case of dongle contorl for when wl up should happen. This is to allow
	   the some of setting like wme, country code etc. to be performed before
	   the wl up in case of reclaim image. Mainly used for testing and controlled
	   via "AllowIntfUp" registry entry
	*/
	if ((param = wl_readparam(nic_configs, "AllowIntfUp", confighandle, wl, NULL))) {

		wl->AllowIntfUp = param->ParameterData.IntegerData ? TRUE : FALSE;

		WL_INFORM(("wl%d: AllowIntfUp set to %d from the registry\n", wl->sh.unit,
			wl->AllowIntfUp));
	} else {
		wl->AllowIntfUp = TRUE;
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"AllowIntfUp\") error\n", wl->sh.unit));
	}

#if defined(BCMDHDUSB)
	if ((param = wl_readparam(nic_configs, "DnglResetAfterDisable", confighandle, wl, NULL)))
		wl->dnglResetAfterDisable = param->ParameterData.IntegerData ? TRUE : FALSE;
	else
		wl->dnglResetAfterDisable = FALSE;

	g_dnglResetAfterDisable = wl->dnglResetAfterDisable;
#endif /* BCMDONGLEHOST && BCMDHDUSB */

	/* read Environment - must be first thing read from registry */
	NdisReadConfiguration(&status, &param, confighandle, &Environment,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS) {
		wl->sh.OS = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
		WL_INFORM(("wl%d: Environment %d\n", wl->sh.unit, (int)wl->sh.OS));
	} else {
		WL_ERROR(("wl%d: wl_minit: NdisReadConfiguration(\"Environment\") error 0x%x\n",
			wl->sh.unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle,
			NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER, 1, 21);
		ret = status;
		goto error;
	}

#if defined(BCMDBG)
	/* read MsgLevel if it exists */
	if ((param = wl_readparam(nic_configs, "MsgLevel", confighandle, wl, NULL))) {
		/* Conversion would not have been done if bcm_strtoul() returned ULONG_MAX */
		if (param->ParameterType  == NdisParameterString &&
		    wcslen(param->ParameterData.StringData.Buffer) > 8) {
			ULONG strsize = param->ParameterData.StringData.Length / sizeof(WCHAR) + 1;
			char* msglevel = MALLOC(wl->sh.osh, strsize);
			uint sz_msglvl_high;

			if (!msglevel) {
				WL_ERROR(("wl_readconfigdata: out of memory, malloced %d bytes\n",
				MALLOCED(wl->sh.osh)));
				return NDIS_STATUS_RESOURCES;
			}
			wchar2ascii(msglevel, param->ParameterData.StringData.Buffer,
				param->ParameterData.StringData.Length, strsize);

			if (strlen(msglevel) > 8) {
				sz_msglvl_high = strlen(msglevel)-8;
				wl_msg_level = bcm_strtoul(msglevel+sz_msglvl_high, NULL, 16);
				msglevel[sz_msglvl_high] = '\0';
				wl_msg_level2 = bcm_strtoul(msglevel, NULL, 16);
			}

		MFREE(wl->sh.osh, msglevel, strsize);

		} else if (param->ParameterType  == NdisParameterInteger) {
			wl_msg_level = param->ParameterData.IntegerData;
			WL_INFORM(("wl%d: wl_msg_level set to 0x%x from the registry\n", \
						wl->sh.unit, wl_msg_level));
		} else {
			WL_INFORM(("wl%d: Invalid msglevel string in registry", wl->sh.unit));
			wl_msg_level = 1;
		}
	} else
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"MsgLevel\") error\n", wl->sh.unit));
#endif /* defined (BCMDBG) */

	/* read NdisVersion */
	NdisReadConfiguration(&status, &param, confighandle, &NdisVersion, NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS) {
		wl->ndisver = param->ParameterData.IntegerData;
		WL_INFORM(("wl%d: NdisVersion 0x%x\n", wl->sh.unit, wl->ndisver));
	} else {
		WL_ERROR(("wl%d: wl_minit: NdisReadConfiguration(\"NdisVersion\") error 0x%x\n",
			wl->sh.unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle,
			NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER, 1, 20);
		ret = status;
		goto error;
	}

	/* read BusType */
	NdisReadConfiguration(&status, &param, confighandle, &BusType,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS) {
		wl->BusType = (NDIS_INTERFACE_TYPE)param->ParameterData.IntegerData;
		WL_INFORM(("wl%d: BusType %d\n", wl->sh.unit, (int)wl->BusType));
	} else {
		WL_ERROR(("wl%d: wl_minit: NdisReadConfiguration(\"BusType\") error 0x%x\n",
			wl->sh.unit, status));
		NdisWriteErrorLogEntry(MiniportAdapterHandle,
			NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER, 1, 22);
		ret = status;
		goto error;
	}

	wl->BusType = NdisInterfaceSDIO;

	/* read ForcePIO if it's there */
	if ((param = wl_readparam(nic_configs, "ForcePIO", confighandle, wl, NULL))) {
		wl->piomode = (param->ParameterData.IntegerData != 0);
		WL_INFORM(("wl%d: piomode %d\n", wl->sh.unit, wl->piomode));
	} else {
		WL_INFORM(("wl%d: wl_minit: wl_readparam(\"ForcePIO\") error; assuming DMA\n",
			wl->sh.unit));
		wl->piomode = FALSE;
	}

	/* Try to read AdapterDesc (for those who want a custom description) */
	if ((param = wl_readparam(nic_configs, "AdapterDesc", confighandle, wl, NULL))) {
		/* All platforms handle an ascii char string */
		if (((char *)param->ParameterData.StringData.Buffer)[1] == '\0') {
			/* it's WCHAR--copy into ascii buffer */
			wchar2ascii(wl->DriverDesc, param->ParameterData.StringData.Buffer,
				param->ParameterData.StringData.Length, sizeof(wl->DriverDesc));
		} else
			strncpy(wl->DriverDesc, (char *) param->ParameterData.StringData.Buffer,
				MAX_DRV_DESC-1);
	} else {
		/* read DriverDesc */
		if ((param = wl_readparam(nic_configs, "DriverDesc", confighandle, wl, NULL))) {
			/* All platforms handle an ascii char string */
			if (((char *)param->ParameterData.StringData.Buffer)[1] == '\0') {
				/* it's WCHAR--copy into ascii buffer */
				wchar2ascii(wl->DriverDesc, param->ParameterData.StringData.Buffer,
					param->ParameterData.StringData.Length,
					sizeof(wl->DriverDesc));
			} else
				strncpy(wl->DriverDesc,
					(char *) param->ParameterData.StringData.Buffer,
					MAX_DRV_DESC-1);
		} else {
			*((uint16 *)wl->DriverDesc) = 0;
			WL_ERROR(("wl%d: wl_minit: wl_readparam(\"DriverDesc\") error\n",
				wl->sh.unit));
		}
	}

	/* Try to read name of the SDIO host controller being used 
	for GPIO control of 4325 chip reset pins
	*/
	if ((param = wl_readparam(nic_configs, "SDHostCtrl", confighandle, wl, NULL))) {
		/* All platforms handle an ascii char string */
		if (((char *)param->ParameterData.StringData.Buffer)[1] == '\0') {
			/* it's WCHAR--copy into ascii buffer */
			memcpy(wl->sdHostCtrlName, param->ParameterData.StringData.Buffer,
				param->ParameterData.StringData.Length + 2);
		}
	} else {
#if 1	/* smlee : HSMMC = 'HSC' */
		memcpy(wl->sdHostCtrlName, L"HSC1:", (strlen("HSC1:") + 1) * 2);
#else
		memcpy(wl->sdHostCtrlName, L"SHC1:", (strlen("SHC1:") + 1) * 2);
#endif
	}


	if ((param = wl_readparam(nic_configs, "Brand", confighandle, wl, NULL))) {
		/* All platforms handle an ascii char string */
		if (((char *)param->ParameterData.StringData.Buffer)[1] == '\0') {
			/* it's WCHAR--copy into ascii buffer */
			wchar2ascii(wl->Brand, param->ParameterData.StringData.Buffer,
				param->ParameterData.StringData.Length, sizeof(wl->Brand));
		} else {
			strncpy(wl->Brand, (char *) param->ParameterData.StringData.Buffer,
				WL_BRAND_MAX);
			wl->Brand[WL_BRAND_MAX-1] = 0;
		}
	} else {
		*((uint16 *)wl->Brand) = 0;
	}

#ifdef EXT_STA

	if ((param = wl_readparam(nic_configs, "*PriorityVLANTag", confighandle, wl, NULL))) {
		wl->priority_vlan_tag = param->ParameterData.IntegerData;
	} else {
		wl->priority_vlan_tag = 0;
	}
#endif /* EXT_STA */

	/* Store the wakeup capabilities override from registry if hardware is capable */
	if ((param = wl_readparam(nic_configs, "WakeupCapabilities", confighandle, wl, NULL)))
		wl->wowl_cap_override = (int)param->ParameterData.IntegerData;
	else
		wl->wowl_cap_override = -1;

	/* See if broadcast key rotation is to be enabled */
	if ((param = wl_readparam(nic_configs, "WowlKeyRot", confighandle, wl, NULL)))
		wl->wowl_key_rot = (int)param->ParameterData.IntegerData;
	else
		wl->wowl_key_rot = 0;

error:
	/* close the registry */
	if (confighandle)
		NdisCloseConfiguration(confighandle);

	return ret;
}

PNDIS_CONFIGURATION_PARAMETER
wl_readparam_ndis(
	ndis_config_t *ndis_config,
	const char *str,
	NDIS_HANDLE confighandle,
	wl_info_t *wl,
	int *index
)
{
	NDIS_STATUS status = NDIS_STATUS_FAILURE;
	static NDIS_CONFIGURATION_PARAMETER readparam;
	PNDIS_CONFIGURATION_PARAMETER local_param;
	UNICODE_STRING ndis_str;
	ANSI_STRING ansi_str;
	USHORT uni_buflen;

	ASSERT(ndis_config);

	bzero(&ndis_str, sizeof(ndis_str));
	bzero(&ansi_str, sizeof(ansi_str));

	ndis_config = wl_findconfig(str, ndis_config, index);
	if (!ndis_config)
		goto done;

	/* create the counted ansi string for the call to NdisAnsiStringToUnicodeString */
	NdisInitAnsiString(&ansi_str, str);

	/* create the counted unicode string for the call to NdisAnsiStringToUnicodeString */
	uni_buflen = 2 * ansi_str.MaximumLength;
	ndis_str.MaximumLength = uni_buflen;
#ifndef NDIS60
	status = NdisAllocateMemoryWithTag(&ndis_str.Buffer, uni_buflen, MEMORY_TAG);
#else
	ndis_str.Buffer = NdisAllocateMemoryWithTagPriority(wl->sh.adapterhandle, uni_buflen,
	    MEMORY_TAG, NormalPoolPriority);
	if (!ndis_str.Buffer)
		status = NDIS_STATUS_RESOURCES;
	else
		status = NDIS_STATUS_SUCCESS;
#endif /* !NDIS60 */
	if (status != NDIS_STATUS_SUCCESS)
		goto done;

	status = NdisAnsiStringToUnicodeString(&ndis_str, &ansi_str);
	if (status != NDIS_STATUS_SUCCESS)
		goto done;

	if (ndis_config->type == NdisParameterInteger) {
		CHAR *str = NULL;
		ULONG strsize;
		ULONG result;
		/* use bcm_strtoul to interpret strings with 0x prefixes as numbers. Stock Windows
		 * NdisReadConfiguration conversion of string to integer only does decimal
		 * conversion.
		 * Also, Win9x will not interpret strings of negative numbers. In particular, "-1"
		 * in the Registry will come back as 0 if NdisReadConfiguration() is asked to return
		 * the value as an integer.
		 */
		NdisReadConfiguration(&status, &local_param, confighandle, &ndis_str,
		                      NdisParameterString);

		if (NDIS_ERROR(status)) {
			/* not found or error */
		} else if (local_param->ParameterType == NdisParameterInteger) {
			/* if the reg data is DWORD, NdisReadConfiguration seems to return no error,
			 * but does not return a string. Use as is.
			 */
			ndis_config->param = local_param;
		} else {
			strsize = local_param->ParameterData.StringData.Length / sizeof(WCHAR) + 1;
			str = MALLOC(wl->sh.osh, strsize);
			if (!str) {
				WL_ERROR(("wl_readparam: out of memory, malloced %d bytes\n",
					MALLOCED(wl->sh.osh)));
				status = NDIS_STATUS_RESOURCES;
				goto done;
			}

			if (local_param->ParameterData.StringData.Length >= 2 &&
			    ((char *)local_param->ParameterData.StringData.Buffer)[1] == '\0')
				/* it's WCHAR -- copy into ascii buffer */
				wchar2ascii(str, local_param->ParameterData.StringData.Buffer,
					local_param->ParameterData.StringData.Length, strsize);
			else
				strcpy(str, (char*)local_param->ParameterData.StringData.Buffer);

			/* don't clobber NDIS' memory; point param to driver local copy */
			result =  bcm_strtoul(str, NULL, 0);
			/* Are we trying to in a 64-bit integer? */
			if (result != (unsigned long) -1 /* ULONG_MAX */ || !strcmp("-1", str)) {
				readparam.ParameterData.IntegerData = result;
				readparam.ParameterType = NdisParameterInteger;
				ndis_config->param = &readparam;
			}
			else {
				WL_ERROR(("wl%d: wl_readparam: Overflow detected in integer "
				          "conversion; preserving as string\n", wl->sh.unit));
				ndis_config->param = local_param;
			}
			MFREE(wl->sh.osh, str, strsize);
		}
	} else
		NdisReadConfiguration(&status, &ndis_config->param, confighandle, &ndis_str,
			ndis_config->type);

	if (status == NDIS_STATUS_SUCCESS &&
		ndis_config->type == NdisParameterInteger &&
		(ndis_config->flags & SKIP_MINUS_1) &&
		ndis_config->param->ParameterData.IntegerData == -1)
		    status = NDIS_STATUS_FILE_NOT_FOUND;

done:
	if (ndis_str.Buffer)
		NdisFreeMemory(ndis_str.Buffer, uni_buflen, 0);

	return (NDIS_ERROR(status)? NULL : ndis_config->param);
}

#define READ_PARAM_STRING_COUNT 2048

PNDIS_CONFIGURATION_PARAMETER
wl_readparam_native(
	ndis_config_t *ndis_config,
	const char *dstr,
	NDIS_HANDLE confighandle,
	wl_info_t *wl,
	int *index
)
{
	NDIS_STATUS status = NDIS_STATUS_FAILURE;
	static NDIS_CONFIGURATION_PARAMETER readparam;
	static char strbuf[READ_PARAM_STRING_COUNT];
	PNDIS_CONFIGURATION_PARAMETER local_param;
	uint32 dwType, dwWord, dwSize;
	ULONG strsize;
	ULONG result;
	CHAR *str = NULL;

	TCHAR TStr[1024];
	uint32 ret;
	ASSERT(ndis_config);

	local_param = &readparam;
	readparam.ParameterData.StringData.Buffer = (LPWSTR)strbuf;

	ndis_config = wl_findconfig(dstr, ndis_config, index);
	if (!ndis_config)
		goto done;

	/* Convert ascii string to TCHAT type for RegQueryValueEx API */
	MultiByteToWideChar(CP_ACP, 0, dstr, -1, TStr, 256);

	/* Find out what is the original registry entry type & size of the entry */
	dwSize = 1;
	ret = RegQueryValueEx(confighandle, TStr, NULL, &dwType, (LPBYTE)&dwWord, &dwSize);
	if (ret == ERROR_MORE_DATA) {

		if (dwType == REG_DWORD) {
			if (RegQueryValueEx(confighandle, TStr, NULL, NULL,
				(LPBYTE)&readparam.ParameterData.IntegerData, &dwSize)
				!= ERROR_SUCCESS) {
				WL_ERROR(("wl%d: wl_readparam: failed RegQueryValueEx " \
					"while reading IntergerType, registry string = %s\n",
					wl->sh.unit, dstr));
				goto done;
			}
			readparam.ParameterType = NdisParameterInteger;

		} else if (dwType == REG_SZ) {
			local_param->ParameterData.StringData.Length = (USHORT)dwSize;

			if (dwSize >= READ_PARAM_STRING_COUNT) {
				WL_ERROR(("wl%d: wl_readparam: failed string data " \
					"is more than %d bytes, registry string = %s\n",
					wl->sh.unit, READ_PARAM_STRING_COUNT, dstr));
				goto done;
			}

			if (RegQueryValueEx(confighandle, TStr, NULL, NULL,
				(LPBYTE)readparam.ParameterData.StringData.Buffer, &dwSize)
				!= ERROR_SUCCESS) {

				WL_ERROR(("wl%d: wl_readparam: failed RegQueryValueEx " \
					"while reading StringType, registry string = %s\n",
					wl->sh.unit, dstr));
				goto done;
			}
			readparam.ParameterType = NdisParameterString;

		} else {
			WL_ERROR(("wl%d: wl_readparam: failed RegQueryValueEx " \
				"while reading unknown type %d, registry string = %s\n",
				wl->sh.unit, dwType, dstr));
			goto done;
		}
	} else {
		if (ret != ERROR_FILE_NOT_FOUND) {
			WL_ERROR(("wl%d: wl_readparam: failed RegQueryValueEx " \
			"while finding registry type & size, registry string = %s\n",
			wl->sh.unit, dstr));
		}
		goto done;
	}

	status = NDIS_STATUS_SUCCESS;

	if (ndis_config->type == NdisParameterInteger && dwType == REG_SZ) {
		strsize = local_param->ParameterData.StringData.Length / sizeof(WCHAR) + 1;
		str = (CHAR *)MALLOC(wl->sh.osh, strsize);
		if (!str) {
			WL_ERROR(("wl_readparam: out of memory, malloced %d bytes\n",
				MALLOCED(wl->sh.osh)));
			status = NDIS_STATUS_RESOURCES;
			goto done;
		}

		if (local_param->ParameterData.StringData.Length >= 2 &&
			((char *)local_param->ParameterData.StringData.Buffer)[1] == '\0')
			/* it's WCHAR -- copy into ascii buffer */
			wchar2ascii(str, local_param->ParameterData.StringData.Buffer,
				local_param->ParameterData.StringData.Length, strsize);
		else
			strcpy(str, (char*)local_param->ParameterData.StringData.Buffer);

		/* don't clobber NDIS' memory; point param to driver local copy */
		result =  bcm_strtoul(str, NULL, 0);

		/* Are we trying to in a 64-bit integer? */
		if (result != (unsigned long) -1 /* ULONG_MAX */ || !strcmp("-1", str)) {
			readparam.ParameterData.IntegerData = result;
			readparam.ParameterType = NdisParameterInteger;
		}
		else {
			WL_ERROR(("wl%d: wl_readparam: Overflow detected in integer "
				      "conversion; preserving as string\n", wl->sh.unit));
			ndis_config->param = local_param;
			status = NDIS_STATUS_FAILURE;
			goto done;
		}
	}

	ndis_config->param = &readparam;

	if (status == NDIS_STATUS_SUCCESS &&
		ndis_config->type == NdisParameterInteger &&
		(ndis_config->flags & SKIP_MINUS_1) &&
		ndis_config->param->ParameterData.IntegerData == -1)
		    status = NDIS_STATUS_FILE_NOT_FOUND;
done:
	if (str)
		MFREE(wl->sh.osh, str, strsize);

	return (NDIS_ERROR(status)? NULL : ndis_config->param);
}

PNDIS_CONFIGURATION_PARAMETER
wl_readparam(
	ndis_config_t *ndis_config,
	const char *str,
	NDIS_HANDLE confighandle,
	wl_info_t *wl,
	int *index
)
{
	PNDIS_CONFIGURATION_PARAMETER retparam = NULL;

	/*
	"wl_readparam" function get called during "wl_minit" under the NDIS
	 context where one is allowed make call into "NdisOpenConfiguration",
	"NdisReadConfiguration" via NDIS API. But when "wl_readparam" is called
	not in the NDIS context i.e. during power on/off we need to use native
	registry API's to access the registry values
	*/

	if (wl->context == REG_ACCESS_INIT)
		retparam = wl_readparam_ndis(ndis_config, str, confighandle, wl, index);
	else
		retparam = wl_readparam_native(ndis_config, str, confighandle, wl, index);

	return retparam;
}

#ifndef EXT_STA
static void
wl_mic_error(wl_info_t *wl, struct ether_addr *ea, bool group, bool flush_txq)
{
	struct {
		NDIS_802_11_STATUS_INDICATION Status;
		NDIS_802_11_AUTHENTICATION_REQUEST Request[1];
	} AuthenticationEvent;

	WL_WSEC(("wl%d: mic error using %s key\n", wl->sh.unit,
		(group) ? "group" : "pairwise"));

	/* flush txq to speed up tx of 2nd MIC failure indication */
	if (flush_txq)
		wl_flushtxq(wl);


	AuthenticationEvent.Status.StatusType =
		Ndis802_11StatusType_Authentication;
	bcopy(ea, &AuthenticationEvent.Request[0].Bssid, ETHER_ADDR_LEN);

	if (group)
		AuthenticationEvent.Request[0].Flags =
			NDIS_802_11_AUTH_REQUEST_GROUP_ERROR;
	else
		AuthenticationEvent.Request[0].Flags =
			NDIS_802_11_AUTH_REQUEST_PAIRWISE_ERROR;
	AuthenticationEvent.Request[0].Length =
		sizeof(AuthenticationEvent.Request[0]);

	NdisMIndicateStatus(wl->sh.adapterhandle,
		NDIS_STATUS_MEDIA_SPECIFIC_INDICATION,
		&AuthenticationEvent, sizeof(AuthenticationEvent));
	NdisMIndicateStatusComplete(wl->sh.adapterhandle);
}
#else
static void
wl_mic_error(wl_info_t *wl, struct ether_addr *ea, int key_index, bool group, bool flush_txq)
{
	DOT11_TKIPMIC_FAILURE_PARAMETERS AuthenticationEvent;

	WL_WSEC(("wl%d: mic error using %s key\n", wl->sh.unit,
		(group) ? "group" : "pairwise"));

	/* flush txq to speed up tx of 2nd MIC failure indication */
	if (flush_txq) {
		ND_PKT *p;

		ND_TXLOCK(wl);
		while (p = shared_deq(&wl->txq))
			shared_enq(&wl->sh.txdone, p, FALSE);
		ND_TXUNLOCK(wl);
	}

	AuthenticationEvent.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	AuthenticationEvent.Header.Revision = DOT11_TKIPMIC_FAILURE_PARAMETERS_REVISION_1;
	AuthenticationEvent.Header.Size = sizeof(DOT11_TKIPMIC_FAILURE_PARAMETERS);

	AuthenticationEvent.bDefaultKeyFailure = (group ? TRUE : FALSE);
	AuthenticationEvent.uKeyIndex = (group ? key_index : 0);
	bcopy(ea, &AuthenticationEvent.PeerMac, ETHER_ADDR_LEN);

	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_TKIPMIC_FAILURE,
	    &AuthenticationEvent, sizeof(AuthenticationEvent));
}
#endif /* EXT_STA */

#if NDIS_WDM
#endif /* NDIS_WDM */

#if defined(NDIS51)
void
wl_PnP_event(
	IN  NDIS_HANDLE             MiniportAdapterContext,
	IN  NDIS_DEVICE_PNP_EVENT   PnPEvent,
	IN  PVOID                   InformationBuffer,
	IN  ULONG                   InformationBufferLength
)
{

	wl_info_t *wl = WL_INFO(MiniportAdapterContext);
	WL_ERROR(("Entered wl_PnP_event\n"));
	ND_LOCK(wl);

	switch (PnPEvent)
	{
	case NdisDevicePnPEventQueryRemoved:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventQueryRemoved\n"));
		break;

	case NdisDevicePnPEventRemoved:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventRemoved\n"));
		break;

	case NdisDevicePnPEventSurpriseRemoved:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventSurpriseRemoved\n"));
		wl_down(wl);
		break;

	case NdisDevicePnPEventQueryStopped:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventQueryStopped\n"));
		break;

	case NdisDevicePnPEventStopped:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventStopped\n"));
		break;

	case NdisDevicePnPEventPowerProfileChanged:
		WL_ERROR(("PnPEventNotify: NdisDevicePnPEventPowerProfileChanged\n"));
		wl->acpwr = (*((ULONG*)InformationBuffer) != NdisPowerProfileBattery);
		WL_ERROR(("Power profile %d\n", *((ULONG *)InformationBuffer)));
		break;

	default:
		WL_ERROR(("PnPEventNotify: unknown PnP event %x \n", PnPEvent));
		break;
	}

	ND_UNLOCK(wl);
}

#endif /* defined (NDIS51) */

void
wl_flushtxq(wl_info_t *wl)
{
	ND_PKT* p;

	ND_TXLOCK(wl);
	while (p = shared_deq(&wl->txq))
		shared_enq(&wl->sh.txdone, p, FALSE);
	ND_TXUNLOCK(wl);
}

#ifdef BCMWPA2
/* fill in the pmkidreq_t struct with info from the pmkid_cand_list_t */
static uint
wl_pmkid_cache_ind(pmkid_cand_list_t *pmkid_list, void *cand_buffer)
{
	uint i, len;
	typedef struct {
		NDIS_802_11_STATUS_INDICATION Status;
		NDIS_802_11_PMKID_CANDIDATE_LIST pmkid_list;
	} pmkidreq_t;
	pmkidreq_t *pmkidreq = (pmkidreq_t *)cand_buffer;

	bzero(pmkidreq, sizeof(pmkidreq_t));

	pmkidreq->Status.StatusType = Ndis802_11StatusType_PMKID_CandidateList;
	pmkidreq->pmkid_list.Version = 1;
	pmkidreq->pmkid_list.NumCandidates = ntoh32(pmkid_list->npmkid_cand);

	for (i = 0; i < pmkidreq->pmkid_list.NumCandidates; i++) {
		PMKID_CANDIDATE *cand = &pmkidreq->pmkid_list.CandidateList[i];
		uint j = pmkidreq->pmkid_list.NumCandidates - 1 - i;

		bzero(cand, sizeof(PMKID_CANDIDATE));
		bcopy(pmkid_list->pmkid_cand[j].BSSID.octet, cand->BSSID, ETHER_ADDR_LEN);
		if (pmkid_list->pmkid_cand[j].preauth)
			cand->Flags |= NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED;
		else
			cand->Flags &= ~NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED;

#ifdef BCMDBG
		if (WL_WSEC_ON()) {
			char eabuf[ETHER_ADDR_STR_LEN];

			WL_WSEC(("wl_pmkid_cache_ind: PMKID req: %s\n",
				bcm_ether_ntoa((struct ether_addr *)cand->BSSID, eabuf)));
		}
#endif /* BCMDBG */
	}

	len = sizeof(NDIS_802_11_STATUS_INDICATION) +
		sizeof(NDIS_802_11_PMKID_CANDIDATE_LIST) +
		((pmkidreq->pmkid_list.NumCandidates - 1) * sizeof(PMKID_CANDIDATE));
	return (len);
}
#endif /* BCMWPA2 */

/* keep backward compatibility with legacy brcm UIs */
bool
wl_query_legacy_oid(wl_info_t *wl, NDIS_OID Oid, PVOID InfoBuf, ULONG InfoBufLen,
	PULONG BytesWritten, NDIS_STATUS *status)
{
	ULONG BytesNeeded;

	/* WLC_GET_LOCAL used to need to be supported and be called
	 * before most other ioctls
	 */
	if (Oid == (WL_OID_BASE + 73)) {
		if (InfoBufLen >= sizeof(ULONG)) {
			/* Just return WorldWide(0) */
			*(ULONG*)InfoBuf = 0;
			*BytesWritten = sizeof(ULONG);
			*status = NDIS_STATUS_SUCCESS;
		} else {
			*BytesWritten = 0;
			*status = NDIS_STATUS_INVALID_LENGTH;
		}
		wl->legacy_brcm_apps = TRUE;
		return TRUE;
	}

	/* WLC_GET_COUNTRY buffer used to be 3-byte long and the ioctl is called
	 * before most other ioctls
	 */
	if (Oid == (WL_OID_BASE + WLC_GET_COUNTRY) && InfoBufLen == 3) {
		uint8 buf[8];
		*status = wl_queryoid(wl, Oid, buf, sizeof(buf), BytesWritten,
			&BytesNeeded, 0);
		if (*status == NDIS_STATUS_SUCCESS) {
			buf[2] = 0;
			bcopy(buf, InfoBuf, 3);
			*BytesWritten = 3;
		}
		wl->legacy_brcm_apps = TRUE;
		return TRUE;
	}

	/* WLC_SCAN_RESULTS used to have version 107 of wl_bss_info_t */
	if (wl->legacy_brcm_apps && Oid == (WL_OID_BASE + WLC_SCAN_RESULTS)) {
		*status = wl_queryoid(wl, Oid, InfoBuf, InfoBufLen, BytesWritten,
			&BytesNeeded, 0);
		if (*status == NDIS_STATUS_SUCCESS) {
			uint32 i, delta_len;
			wl_scan_results_t *list = (wl_scan_results_t *)InfoBuf;
			wl_bss_info_t *src = &list->bss_info[0];
			wl_bss_info_107_t *dst = (wl_bss_info_107_t *)&list->bss_info[0];
			/* if it is legacy version of wl_bss_info, xfer not needed */
			if (list->version == LEGACY_WL_BSS_INFO_VERSION)
				return TRUE;
			delta_len = sizeof(wl_bss_info_t) - sizeof(wl_bss_info_107_t);
			/* convert ver 108 of wl_bss_info_t into ver 107 of wl_bss_info_t */
			for (i = 0; i < list->count; i++) {
				dst->version = LEGACY_WL_BSS_INFO_VERSION;
				dst->length = src->length - delta_len;
				bcopy(&src->BSSID, &dst->BSSID,
					(int)((uint8*)&src->chanspec - (uint8*)&src->BSSID));
				dst->channel = CHSPEC_CHANNEL(src->chanspec);
				bcopy(&src->atim_window, &dst->atim_window,
					(int)((uint8*)&src->ie_length - (uint8*)&src->atim_window));
				dst->ie_length = src->ie_length;
				bcopy((src + 1), (dst + 1), src->ie_length);
				src = (wl_bss_info_t *)((uint8 *)src + dst->length + delta_len);
				dst = (wl_bss_info_107_t *)((uint8 *)dst + dst->length);
			}
			list->version = LEGACY_WL_BSS_INFO_VERSION;
			list->buflen -= (delta_len * list->count);
			*BytesWritten -= (delta_len * list->count);
		}
		return TRUE;
	}

	return FALSE;
}

#if defined(EXT_STA) && defined(BCMDBG)
void
fake_ibss(wlc_info_t *wlc, struct ether_addr* addr)
{
	DOT11_ASSOCIATION_START_PARAMETERS StatusBuffer;
	wl_info_t *wl = wlc->wl;
	char perm[32];
	ibss_peer_t *peer;
	wlc_event_t event;
	wlc_event_t *e = &event;
	wlc_bss_info_t *cur_bss = NULL;

	bcm_ether_ntoa(addr, perm);
	WL_ASSOC(("%s: Incoming addr is %s\n", __FUNCTION__, perm));

	/* Simulate this call:
	wlc_mac_event(wlc, WLC_E_IBSS_ASSOC, &scb->ea, 0, 0, DOT11_BSSTYPE_INDEPENDENT, 0, 0);
	*/
	bzero(&event, sizeof(wlc_event_t));
	bcopy(addr->octet, e->event.addr.octet, ETHER_ADDR_LEN);
	e->addr = &(e->event.addr);
	e->event.event_type = WLC_E_IBSS_ASSOC;
	e->event.status = (uint)0;
	e->event.reason = (uint)0;
	e->event.auth_type = DOT11_BSSTYPE_INDEPENDENT;
	e->event.datalen = 0;

	/* Stolen from wl_event(): WLC_E_IBSS_ASSOC */
	if (wl->blocks & IND_BLOCK_ASSOC) {
		WL_ERROR(("%s: IND_BLOCK_ASSOC: Don't know what to do. Bail.\n", __FUNCTION__));
		return;
	}

	bzero(&StatusBuffer, sizeof(DOT11_ASSOCIATION_START_PARAMETERS));
	StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	StatusBuffer.Header.Revision = DOT11_ASSOCIATION_START_PARAMETERS_REVISION_1;
	StatusBuffer.Header.Size = sizeof(DOT11_ASSOCIATION_START_PARAMETERS);
	bcopy(addr->octet, StatusBuffer.MacAddr, ETHER_ADDR_LEN);

	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_ASSOCIATION_START,
	    &StatusBuffer, sizeof(StatusBuffer));
	wl->blocks |= IND_BLOCK_ASSOC;

	/* Synthesize IBSS "association" completion */
	wl_ind_assoc_completion(wl, &e->event, FALSE, FALSE, FALSE);

	/* cache relevant information */
	if (wl->oid->num_peers == MAXBSS) {
		WL_ERROR(("wl%d: IBSS peer table overflow\n", wl->sh.unit));
		return;
	}
	cur_bss = wl_get_current_bss(wl);
	if (!cur_bss) {
		return;
	}
	peer = &wl->oid->peers[wl->oid->num_peers++];
	bcopy(e->addr->octet, peer->addr.octet, ETHER_ADDR_LEN);
	peer->cap = cur_bss->bcn_prb->capability;
	bcopy((char *)&cur_bss->rateset, (void *)&peer->rateset, sizeof(wlc_rateset_t));
	peer->assoc_time = wl->oid->assoc_time;
	return;
}
#endif /* EXT_STA && BCMDBG */


/* Report 125Mbps if in afterburner mode */
static void
wl_abrate_convert(wl_info_t *wl, ULONG *link_speed)
{
	int afterburner;
	wl_iovar_getint(wl, "afterburner", &afterburner);
	if (afterburner)
		/* fixed to 125 Mbps */
		*link_speed = 125 * 2;
}

/* handle ndis specific iovars */
static bool
wl_set_ndis_iovar(wl_info_t *wl, PVOID InfoBuf, ULONG InfoBufLen, NDIS_STATUS *status)
{
	uint len;

	if (!strncmp((char *)InfoBuf, "ndis_unblock_8021x", InfoBufLen)) {
#ifdef BCMSUP_PSK
		len = strlen("ndis_unblock_8021x") + 1;
		if (InfoBufLen < (len + sizeof(int)))
			*status = NDIS_STATUS_INVALID_LENGTH;
		else
			bcopy(((char *)InfoBuf + len), &wl->unblock_8021x, sizeof(int));
#else
		*status = NDIS_STATUS_FAILURE;
#endif
		return TRUE;
	}
	else if (!strncmp((char *)InfoBuf, "wsec_authorization", InfoBufLen)) {
		len = strlen("wsec_authorization") + 1;
		if (InfoBufLen < (len + sizeof(int)))
			*status = NDIS_STATUS_INVALID_LENGTH;
		else
			bcopy(((char *)InfoBuf + len), &wl->wsec_authorization_status,
			      sizeof(int));
		return TRUE;
	}
	else if (!strncmp((char *)InfoBuf, "abrateconvert", InfoBufLen)) {

		len = strlen("abrateconvert") + 1;
		if (InfoBufLen < (len + sizeof(int)))
			*status = NDIS_STATUS_INVALID_LENGTH;
		else
			bcopy(((char *)InfoBuf + len), &wl->abrateconvert, sizeof(int));
		return TRUE;
	}
#ifdef BCMSUP_PSK
	else if (!strncmp((char *)InfoBuf, "sup_wpa", InfoBufLen)) {
		/* just cache the value here but still pass it down */
		len = strlen("sup_wpa") + 1;
		if (InfoBufLen >= (len + sizeof(int)))
			bcopy(((char *)InfoBuf + len), &wl->sup_enable_wpa, sizeof(int));
		return FALSE;
	}
#endif

#ifdef EXT_STA
	else if (!strncmp((char *)InfoBuf, "set_pmk", InfoBufLen)) {
		len = strlen("set_pmk") + 1;
		if (InfoBufLen < (len + 2*sizeof(ushort) +  WSEC_MIN_PSK_LEN))
			*status = NDIS_STATUS_INVALID_LENGTH;
		else {
			bcopy(((char *)InfoBuf + len), &wl->psk_info, sizeof(wl->psk_info));
			return TRUE;
		}
	}
	else if (!strncmp((char *)InfoBuf, "next_wsec", InfoBufLen)) {
		len = strlen("next_wsec") + 1;
		if (InfoBufLen < (len + sizeof(int32)))
			*status = NDIS_STATUS_INVALID_LENGTH;
		else {
			bcopy(((char *)InfoBuf + len), &wl->next_wsec, sizeof(wl->next_wsec));
			return TRUE;
		}
	}
#endif /* EXT_STA */

	return FALSE;
}


NDIS_STATUS
wl_shared_pnp_set_power(wl_info_t *wl, int devicestate, int changestate)
{
	NDIS_STATUS status = NDIS_STATUS_FAILURE;
	/* doesn't work on XP dongle */
#if defined(EXT_STA)
	int assoc_recreate;
	status = NDIS_STATUS_SUCCESS;
	switch (devicestate) {
	case NdisDeviceStateD1:
	case NdisDeviceStateD2:
	case NdisDeviceStateD3: /* sleep */
		/* if we have the association recreation feature enbled,
		 * preserve the STAs bsscfg while we down the driver for
		 * system suspend.
		 */
		wl_iovar_getint(wl, "assoc_recreate", &assoc_recreate);
		WL_ERROR(("wl%d: OID_PNP_SET_POWER D3 (shared handler)\n", wl->sh.unit));

		if (ASSOC_RECREATE_ENAB(wl->pub) && wl->pub->associated) {
			WL_ERROR(("wl%d: %s: Preserving association during D-state change\n",
			          wl->sh.unit, __FUNCTION__));
			wl_iovar_setint(wl, "preserve_assoc", 1);
		}
		if (changestate)
			wl_down(wl);

		wl->pub->hw_up = FALSE;
		break;

	case NdisDeviceStateD0: /* wake */
#ifdef EXT_STA
		/* if we are coming up with an assoc recreate, set the flag
		 * to suppress SSIDs in initial roam scans for security reasons.
		 */
		WL_ERROR(("wl%d: OID_PNP_SET_POWER D0 (shared handler)\n", wl->sh.unit));
		if (ASSOC_RECREATE_ENAB(wl->pub) && wl->pub->associated)
			wl_iovar_setint(wl, "scan_suppress_ssid",
			                (int)(!wl->oid->hidden_network_enabled));
#endif

		/* Reset calibration results to uninitialized */
		if (ASSOC_RECREATE_ENAB(wl->pub)) {
			int val = 1;
			WL_ERROR(("wl%d: %s: Restarting prior association on resume\n",
			          wl->sh.unit, __FUNCTION__));
			wl_ioctl(wl, WLC_RESTART, &val, sizeof(val));
		}

		/* Since the power setting of the device has changed
		 * initialize the hardware first
		 */
		wl_up(wl);
		break;
	default:
		ASSERT(0); /* invalid ndis power state */
	}
#endif 
	return status;
}


#if defined(WL_IW_USE_ISCAN) || defined(SIMPLE_ISCAN)
/* These chan_info[] & lookup routines replicate those from wlc_phy.c because of BMAC split */
static struct chan_info {
	uint16	chan;	/* channel number */
	uint16	freq;	/* in MHz */
} chan_info[] = {
	/* 11b/11g */
/* 0 */		{1,	2412},
/* 1 */		{2,	2417},
/* 2 */		{3,	2422},
/* 3 */		{4,	2427},
/* 4 */		{5,	2432},
/* 5 */		{6,	2437},
/* 6 */		{7,	2442},
/* 7 */		{8,	2447},
/* 8 */		{9,	2452},
/* 9 */		{10,	2457},
/* 10 */	{11,	2462},
/* 11 */	{12,	2467},
/* 12 */	{13,	2472},
/* 13 */	{14,	2484},

#ifdef BAND5G
/* 11a japan high */
/* 14 */	{34,	5170},
/* 15 */	{38,	5190},
/* 16 */	{42,	5210},
/* 17 */	{46,	5230},

/* 11a usa low */
/* 18 */	{36,	5180},
/* 19 */	{40,	5200},
/* 20 */	{44,	5220},
/* 21 */	{48,	5240},
/* 22 */	{52,	5260},
/* 23 */	{56,	5280},
/* 24 */	{60,	5300},
/* 25 */	{64,	5320},

/* 11a Europe */
/* 26 */	{100,	5500},
/* 27 */	{104,	5520},
/* 28 */	{108,	5540},
/* 29 */	{112,	5560},
/* 30 */	{116,	5580},
/* 31 */	{120,	5600},
/* 32 */	{124,	5620},
/* 33 */	{128,	5640},
/* 34 */	{132,	5660},
/* 35 */	{136,	5680},
/* 36 */	{140,	5700},

/* 11a usa high, ref5 only */
/* 37 */	{149,	5745},
/* 38 */	{153,	5765},
/* 39 */	{157,	5785},
/* 40 */	{161,	5805},
/* 41 */	{165,	5825},

/* 11a japan */
/* 42 */	{184,	4920},
/* 43 */	{188,	4940},
/* 44 */	{192,	4960},
/* 45 */	{196,	4980},
/* 46 */	{200,	5000},
/* 47 */	{204,	5020},
/* 48 */	{208,	5040},
/* 49 */	{212,	5060},
/* 50 */	{216,	5080}
#endif /* BAND5G */
};

/*
 * Converts channel frequency to channel number.
 * Returns 0 if the frequency does not match any channel definition.
 */
uint
wl_freq2channel(uint freq)
{
	uint i;

	for (i = 0; i < ARRAYSIZE(chan_info); i++) {
		if (chan_info[i].freq == freq)
			return (chan_info[i].chan);
	}
	WL_ERROR(("wlc_freq2channel: Cannot find freq %d\n", freq));
	return (0);
}

/*
 * Converts channel number to channel frequency.
 * Returns 0 if the channel is out of range.
 * Also used by some code in wlc_iw.c
 */
uint
wl_channel2freq(uint channel)
{
	uint i;

	for (i = 0; i < ARRAYSIZE(chan_info); i++)
		if (chan_info[i].chan == channel)
			return (chan_info[i].freq);
	return (0);
}

/* rate defs copied from wlc_rate.h */

#define	WLC_RATE_FLAG	0x80	/* Rate flag: basic or ofdm */

/* rate spec bit fields */
#define RSPEC_RATE_MASK		0x0000007F     	/* Either 500Kbps units or MIMO MCS idx */
#define RSPEC_MIMORATE		0x08000000	/* mimo MCS is stored in RSPEC_RATE_MASK */
#define RSPEC_BW_MASK		0x00000700     	/* mimo bw mask */
#define RSPEC_BW_SHIFT		8		/* mimo bw shift */
#define RSPEC_STF_MASK		0x00003800     	/* mimo Space/Time/Frequency mode mask */
#define RSPEC_STF_SHIFT		11		/* mimo Space/Time/Frequency mode shift */
#define RSPEC_CT_MASK		0x0000C000     	/* mimo coding type mask */
#define RSPEC_CT_SHIFT		14		/* mimo coding type shift */
#define RSPEC_STC_MASK		0x00300000	/* mimo num STC streams per PLCP defn. */
#define RSPEC_STC_SHIFT		20		/* mimo num STC streams per PLCP defn. */
#define RSPEC_ADC_CODING	0x00400000	/* mimo bit indicates adv coding in use */
#define RSPEC_SHORT_GI		0x00800000	/* mimo bit indicates short GI in use */
#define RSPEC_OVERRIDE		0x80000000	/* bit indicates override both rate & mode */

/* Rate info table; takes a legacy rate or ratespec_t */
#ifdef WL11N
#define	IS_MCS(r)     	(r & RSPEC_MIMORATE)
#else /* WL11N */
#define	IS_MCS(r)     	0
#endif /* WL11N */
#define	IS_OFDM(r)     	(!IS_MCS(r) && (rate_info[(r) & RSPEC_RATE_MASK] & WLC_RATE_FLAG))
/* return true if the rateset contains an OFDM rate */
bool
wl_rateset_isofdm(uint count, uint8 *rates)
{
	int i;

	for (i = count - 1; i >= 0; i--)
		if (IS_OFDM(rates[i]))
			return (TRUE);
	return (FALSE);
}
#endif /* defined(WL_IW_USE_ISCAN) || defined(SIMPLE_ISCAN) */

#if defined(WL_IW_USE_ISCAN)

/* *************  ISCAN IMPLEMENTATION  **************** */

/* Helper private functions */

static int
wl_iovar_setbuf(
	wl_info_t *wl,
	char *iovar,
	void *param,
	int paramlen,
	void *bufptr,
	int buflen)
{
	int iolen;

	iolen = bcm_mkiovar(iovar, param, paramlen, bufptr, buflen);
	ASSERT(iolen);

	return (wl_ioctl(wl, WLC_SET_VAR, bufptr, iolen));
}

static int
wl_iovar_getbuf(
	wl_info_t *wl,
	char *iovar,
	void *param,
	int paramlen,
	void *bufptr,
	int buflen)
{
	int iolen;

	iolen = bcm_mkiovar(iovar, param, paramlen, bufptr, buflen);
	ASSERT(iolen);

	return (wl_ioctl(wl, WLC_GET_VAR, bufptr, buflen));
}

void
wl_bss_wlc2ndis(wl_bss_info_t *bi, PNDIS_WLAN_BSSID_EX to)
{
	uint num_rates;

	bzero(to, OFFSETOF(NDIS_WLAN_BSSID_EX, IEs));

	/* add fixed fields */
	to->Length = OFFSETOF(NDIS_WLAN_BSSID_EX, IEs);
	bcopy(bi->BSSID.octet, to->MacAddress, ETHER_ADDR_LEN);
	to->Ssid.SsidLength = bi->SSID_len;
	bcopy(bi->SSID, to->Ssid.Ssid, bi->SSID_len);
	to->Privacy = ((bi->capability & DOT11_CAP_PRIVACY) ? 1 : 0);
	to->Rssi = bi->RSSI;
//printf("AP = %s, RSSI = %d\n",to->Ssid.Ssid, to->Rssi); // kdongsuk.
	to->Configuration.Length = sizeof(NDIS_802_11_CONFIGURATION);
	to->Configuration.BeaconPeriod = bi->beacon_period;
	to->Configuration.ATIMWindow = bi->atim_window;
	to->Configuration.DSConfig = wl_channel2freq(CHSPEC_CHANNEL(bi->chanspec)) * 1000;

	if (CHSPEC_IS2G(bi->chanspec))
#ifdef EXT_STA
#ifdef WL11N
		if (bi->flags & WLC_BSS_HT)
			to->NetworkTypeInUse = Ndis802_11FH;
		else
#endif /* WL11N */
#endif /* EXT_STA */
		if (wl_rateset_isofdm(bi->rateset.count, bi->rateset.rates))
			to->NetworkTypeInUse = Ndis802_11OFDM24;
		else
			to->NetworkTypeInUse = Ndis802_11DS;
	else
#ifdef EXT_STA
#ifdef WL11N
		if (bi->flags & WLC_BSS_HT)
			to->NetworkTypeInUse = Ndis802_11NetworkTypeMax;
		else
#endif /* WL11N */
#endif /* EXT_STA */
		to->NetworkTypeInUse = Ndis802_11OFDM5;
	to->Configuration.FHConfig.Length = sizeof(NDIS_802_11_CONFIGURATION_FH);
	to->InfrastructureMode = dtoh16(bi->capability) & DOT11_CAP_ESS ?
		Ndis802_11Infrastructure : Ndis802_11IBSS;

	/* limit the rates to the room in the destination NDIS_802_11_RATES_EX array */
	num_rates = bi->rateset.count;
	if (num_rates > sizeof(NDIS_802_11_RATES_EX))
		num_rates = sizeof(NDIS_802_11_RATES_EX);
	bcopy(bi->rateset.rates, to->SupportedRates, num_rates);
}


static int
wl_valid_IE_len(const UCHAR* var_IE, ULONG len)
{
	int remaining_bytes = len;
	bcm_tlv_t* IE = (bcm_tlv_t*) var_IE;
	int ret;

	if (!bcm_valid_tlv(IE, remaining_bytes))
		return 0;

	while ((IE = bcm_next_tlv(IE, &remaining_bytes)) != NULL)
		;

	ret = len - remaining_bytes;

	if (remaining_bytes) {
		WL_TRACE(("wl: Malformed TLV detected!\n"));
	}

	return ret;
}


/* Pads existing zeroes onto the buffer to make the list of IEs valid, */
/* or truncates if the last IE has no length allocated */
static int
wl_padded_IE_len(const UCHAR* IEs, ULONG len)
{
	bcm_tlv_t* last_tlv;
	int padded_len, valid_len = wl_valid_IE_len(IEs, len);

	/* Pad scenarios - there's an extra tag/value or extra tag/value and an invalid length */
	if (len - valid_len >= 2) {
		last_tlv = (bcm_tlv_t*) (IEs + valid_len); /* start of the last tag */
		padded_len = valid_len + last_tlv->len + TLV_HDR_LEN;
	}
	else
		padded_len = valid_len;

	return padded_len;
}

/* 

IScan private functions (implementation)

*/

static void
wl_iscan_lock_data(wl_info_t *wl)
{
#if defined(NDIS60)
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	NdisAcquireSpinLock(&(iscan->SpinLock));
#endif
}

static void
wl_iscan_unlock_data(wl_info_t *wl)
{
#if defined(NDIS60)
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	NdisReleaseSpinLock(&(iscan->SpinLock));
#endif
}


static int
wl_iscan_prep(wl_info_t *wl, wl_scan_params_t *params, wlc_ssid_t *ssid)
{
	int err = 0;
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;

	memcpy(&params->bssid, &ether_bcast, ETHER_ADDR_LEN);
#if defined(NDIS60)
	params->bss_type = iscan->BSSType;
	params->scan_type = iscan->ScanType;
#else
	params->bss_type = DOT11_BSSTYPE_ANY;
#ifdef CUSTOMER_HW_PT
	params->scan_type = 0;
#else
	params->scan_type = -1;
#endif
#endif
	params->nprobes = -1;
	params->active_time = -1;
	params->passive_time = -1;
	params->home_time = -1;
	params->channel_num = 0;

	params->nprobes = htod32(params->nprobes);
	params->active_time = htod32(params->active_time);
	params->passive_time = htod32(params->passive_time);
	params->home_time = htod32(params->home_time);
	if (ssid && ssid->SSID_len)
		memcpy(&params->ssid, ssid, sizeof(wlc_ssid_t));

	return err;
}


static void wl_send_scan_complete(iscan_info_t *iscan)
{
}


static void
wl_iscan_timer(
	IN PVOID systemspecific1,
	IN NDIS_HANDLE context,
	IN PVOID systemspecific2,
	IN PVOID systemspecific3
)
{
	struct wl_timer *t;
	wl_info_t *wl;

	t = (struct wl_timer *)context;
	wl = t->wl;
	ND_LOCK(wl);

	if (wl->callbacks)
		wl->callbacks--;

	if (t->set && !wl->timer_stop) {
		if (!t->periodic)
			t->set = FALSE;

		(*t->fn)(t->arg);
	}

	ND_UNLOCK(wl);
}


struct wl_timer *
wl_iscan_init_timer(wl_info_t *wl, struct wl_timer *t, void (*fn)(void* arg), void* arg)
{
	bzero(t, sizeof(struct wl_timer));
	t->wl = wl;
	t->fn = fn;
	t->arg = arg;

	NdisMInitializeTimer(&t->ndis_timer, wl->sh.adapterhandle,
		wl_iscan_timer, (PVOID)t);

	return t;
}

void
wl_iscan_add_timer(struct wl_timer *t, uint ms, int periodic)
{
	ASSERT(t->fn);

	WL_ISCAN(("%s ms = %d:\n", __FUNCTION__, ms));
	/* add same timer twice is not supported */

	/* do not check driver "up" for timer to be added when driver is "down" */
	t->periodic = periodic;
	if (!t->set) {
		t->set = TRUE;
		if (periodic)
			NdisMSetPeriodicTimer(&t->ndis_timer, ms);
		else
			NdisMSetTimer(&t->ndis_timer, ms);
	}
}

bool
wl_iscan_del_timer(struct wl_timer *t)
{
	bool canceled;

	WL_ISCAN(("%s:\n", __FUNCTION__));
	ASSERT(t->fn);

	canceled = TRUE;

	if (t->set) {
		NdisMCancelTimer(&t->ndis_timer, &canceled);
		t->set = FALSE;
		t->periodic = FALSE;
	}
	return (canceled);
}

void
wl_iscan_free_timer(wl_info_t *wlh, struct wl_timer *timer)
{
	/* NOOP for windows. */
}

static uint32
wl_iscan_get(wl_info_t *wl)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan_buf_t * buf;
	iscan_buf_t * ptr;
	wl_iscan_results_t * list_buf;
	wl_iscan_results_t list;
	wl_scan_results_t *results;
	uint32 status;
	shared_info_t *sh = &wl->sh;
	WL_ISCAN(("ISCAN GET\n"));
	wl_iscan_lock_data(wl);

	/* buffers are allocated on demand */
	if (iscan->list_cur) {
		buf = iscan->list_cur;
		iscan->list_cur = buf->next;
	}
	else {
		buf = MALLOC(sh->osh, sizeof(iscan_buf_t));
		if (!buf)
			return WL_SCAN_RESULTS_ABORTED;
		buf->next = NULL;
		if (!iscan->list_hdr)
			iscan->list_hdr = buf;
		else {
			ptr = iscan->list_hdr;
			while (ptr->next) {
				ptr = ptr->next;
			}
			ptr->next = buf;
		}
	}
	memset(buf->iscan_buf, 0, WLC_IW_ISCAN_MAXLEN);
	list_buf = (wl_iscan_results_t*)buf->iscan_buf;
	results = &list_buf->results;
	results->buflen = WL_ISCAN_RESULTS_FIXED_SIZE;
	results->version = 0;
	results->count = 0;

	memset(&list, 0, sizeof(list));
	list.results.buflen = htod32(WLC_IW_ISCAN_MAXLEN);
	wl_iovar_getbuf(
		wl,
		"iscanresults",
		&list,
		WL_ISCAN_RESULTS_FIXED_SIZE,
		buf->iscan_buf,
		WLC_IW_ISCAN_MAXLEN);
	results->buflen = dtoh32(results->buflen);
	results->version = dtoh32(results->version);
	results->count = dtoh32(results->count);
	WL_TRACE(("results->count = %d\n", results->count));

	WL_TRACE(("results->buflen = %d\n", results->buflen));
	status = dtoh32(list_buf->status);
	wl_iscan_unlock_data(wl);
	return status;
}

#ifdef __BBSOFT__
static int
_wl_iscan_get_channel_list(wl_info_t *wl, wl_scan_params_t *params)
{
	int i;
	
	for (i = 0; i < wl->channel_num; i++) {
		params->channel_list[i] = htodchanspec(wl->channel_list[i]);
	}
	
	return wl->channel_num;
}
#endif

static int
wl_iscan_request(wl_info_t *wl, uint16 action)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	int params_size = (WL_SCAN_PARAMS_FIXED_SIZE + OFFSETOF(wl_iscan_params_t, params));
	wl_iscan_params_t *params;
	int err = 0;
	wlc_ssid_t req_ssid;
	LARGE_INTEGER	start;
	shared_info_t *sh = &wl->sh;
	NdisGetCurrentSystemTime(&start);
	WL_ISCAN(("ISCAN REQUEST\n"));

#ifdef __BBSOFT__
	if (wl->channel_num)
		params_size += (wl->channel_num * sizeof(uint16));  
#endif

	bzero(&req_ssid, sizeof(req_ssid));
	req_ssid.SSID_len = 0;

	if (req_ssid.SSID_len) {
		params_size += sizeof(wlc_ssid_t);
	}
	params = (wl_iscan_params_t*)MALLOC(wl->sh.osh, params_size);
	if (params == NULL) {
		return NDIS_STATUS_RESOURCES;
	}
	memset(params, 0, params_size);
	ASSERT(params_size < WLC_IOCTL_SMLEN);

	err = wl_iscan_prep(wl, &params->params, &req_ssid);

#ifdef __BBSOFT__
	params->params.channel_num = htod32(_wl_iscan_get_channel_list(wl, &params->params));
#endif

	if (!err) {
		params->version = htod32(ISCAN_REQ_VERSION);
		params->action = htod16(action);
		params->scan_duration = htod16(0);

		/* params_size += OFFSETOF(wl_iscan_params_t, params); */
		err = wl_iovar_setbuf(wl, "iscan", params, params_size,
			iscan->ioctlbuf, WLC_IOCTL_SMLEN);
	}
	MFREE(wl->sh.osh, params, params_size);

	return err;
}


static void
wl_iscan_read(wl_info_t *wl)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	uint32 status = WL_SCAN_RESULTS_PARTIAL;

	WL_ISCAN(("%s:\n", __FUNCTION__));
	if (iscan->timer_on) {
		wl_iscan_del_timer(iscan->timer);
		iscan->timer_on = 0;
	}

	if (iscan->iscan_state != ISCAN_STATE_SCANING)
		return;

	status = wl_iscan_get(wl);

	switch (status) {
		case WL_SCAN_RESULTS_PARTIAL:
			WL_ISCAN(("iscanresults incomplete\n"));
			/* make sure our buffer size is enough before going next round */
			wl_iscan_request(wl, WL_SCAN_ACTION_CONTINUE);
			/* Reschedule the timer */
			wl_iscan_add_timer(iscan->timer, iscan->timer_ms, FALSE);
			iscan->timer_on = 1;
			break;
		case WL_SCAN_RESULTS_SUCCESS:
			WL_ISCAN(("iscanresults complete\n"));
			iscan->iscan_state = ISCAN_STATE_IDLE;
			wl_send_scan_complete(iscan);
#if defined(NDIS60)
			WL_ISCAN(("iscanresults complete -> Notify NDIS\n"));
			wl_ind_scan_confirm(wl, NDIS_STATUS_SUCCESS);
#endif
			break;
		case WL_SCAN_RESULTS_PENDING:
			WL_ISCAN(("iscanresults pending\n"));
			/* Reschedule the timer */
			wl_iscan_add_timer(iscan->timer, iscan->timer_ms, FALSE);
			iscan->timer_on = 1;
			break;
		case WL_SCAN_RESULTS_ABORTED:
			WL_ISCAN(("iscanresults aborted\n"));
			iscan->iscan_state = ISCAN_STATE_IDLE;
			wl_iscan_set_scan(wl);
			break;
		default:
			WL_ISCAN(("iscanresults returned unknown status %d\n", status));
#if defined(NDIS60)
			/* Unknown state, try reseting iscan */
			wl_iscan_flush_scan(wl);
#endif
			/* iscan->iscan_state = ISCAN_STATE_IDLE; */
			break;
	 }
}

static void wl_set_event_mask(wl_info_t *wl)
{
	char eventmask[WL_EVENTING_MASK_LEN];
	char iovbuf[WL_EVENTING_MASK_LEN + 12];	/* Room for "event_msgs" + '\0' + bitvec */

	WL_ISCAN(("wl_set_event_mask\n"));
	wl_iovar_getbuf(wl, "event_msgs", "", 0, iovbuf, sizeof(iovbuf));
	bcopy(iovbuf, eventmask, WL_EVENTING_MASK_LEN);
	setbit(eventmask, WLC_E_SCAN_COMPLETE);
	wl_iovar_setbuf(wl, "event_msgs", eventmask, WL_EVENTING_MASK_LEN,
		iovbuf, sizeof(iovbuf));

#ifdef DHD_DEBUG
	wl_iovar_getbuf(wl, "event_msgs", "", 0, iovbuf, sizeof(iovbuf));
#endif /* DHD_DEBUG */
}

VOID
wl_iscan_workitem
#if !defined(NDIS60) || !defined(NDIS60)
	(PNDIS_WORK_ITEM WorkItem, PVOID Context)
#elif defined(NDIS60)
	(PVOID Context, NDIS_HANDLE  NdisIoWorkItemHandle)
#else /* !NDIS60 */
	(WDFWORKITEM  WorkItem)
#endif /* NDIS60 */
{
	wl_info_t * wl = (wl_info_t *)Context;

	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	WL_ISCAN(("%s:\n", __FUNCTION__));
	if (iscan->dev_state == NdisDeviceStateD0) {
		if (iscan->iscan_state == ISCAN_STATE_SCANING) {
			wl_iscan_read(wl);
		} else {
			wl_iscan_set_scan(wl);
		}
	}
}

void wl_iscan_timerfn(wl_info_t *wl)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	if (!iscan)
		return;
	iscan->timer_on = 0;
	if (iscan->iscan_state != ISCAN_STATE_IDLE) {
		WL_ISCAN(("timer trigger\n"));
		/*
		* Workitem used to post read requests.
		*/
#if defined(NDIS60)
		NdisQueueIoWorkItem(iscan->IScanWorkItem, wl_iscan_workitem, (PVOID)wl);
#else /* NDIS60 */
		NdisScheduleWorkItem(&iscan->IScanWorkItem);
#endif /* NDIS60 */
	}
}

/* 

IScan public interface (API)

*/

static int
wl_iscan_get_scan_status(wl_info_t *wl)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	return iscan->iscan_state;
}

static void
wl_iscan_set_device_state(wl_info_t *wl, int dev_state)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan->dev_state = dev_state;
}

static void
wl_iscan_set_radio_disable(wl_info_t *wl, bool status)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan->NDISradio_disabled = status;
}

#if defined(NDIS60)
static void
wl_iscan_flush_scan(wl_info_t *wl)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan_buf_t  *buf;

	if (iscan->timer_on) {
		wl_iscan_del_timer(iscan->timer);
		iscan->timer_on = 0;
	}

	if (iscan->iscan_state != ISCAN_STATE_IDLE) {
		wl_iscan_request(wl, WL_SCAN_ACTION_ABORT);
	}

	wl_iscan_lock_data(wl);
	if (wl->sh.osh) {
		while (iscan->list_hdr) {
			buf = iscan->list_hdr->next;
			MFREE(wl->sh.osh, iscan->list_hdr, sizeof(iscan_buf_t));
			iscan->list_hdr = buf;
		}
	}
	iscan->list_cur = NULL;
	iscan->list_hdr = NULL;
	wl_iscan_unlock_data(wl);

}

static void
wl_iscan_set_scan_type(wl_info_t *wl, DOT11_SCAN_TYPE ScanType)
{

	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan->ScanType = ScanType;

}

static void
wl_iscan_set_bss_type(wl_info_t *wl, DOT11_BSS_TYPE BSSType)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan->BSSType = BSSType;
}
#endif 

static int
wl_iscan_set_scan(wl_info_t *wl)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;

	WL_ISCAN(("%s:\n", __FUNCTION__));

	if (iscan->dev_state != NdisDeviceStateD0) {
		WL_ISCAN(("%s: Dongle is off  -> return error\n", __FUNCTION__));
		return -1;
	}

	if (iscan->iscan_state != ISCAN_STATE_IDLE) {
		WL_ISCAN(("%s: iScan already started -> ignore start request\n", __FUNCTION__));
		return 0;
	}

	if (iscan->timer_on) {
		wl_iscan_del_timer(iscan->timer);
		iscan->timer_on = 0;
	}

	if (iscan->iscan_state == ISCAN_STATE_IDLE) {
		iscan->timer_retry_cnt = ISCAN_RETRY_CNT;
		wl_set_event_mask(wl);
	}
	if (wl_iscan_request(wl, WL_SCAN_ACTION_START) == 0) {
		WL_ISCAN(("wl_iscan_set_scan: successful\n"));
		wl_iscan_lock_data(wl);
		iscan->list_cur = iscan->list_hdr;
		wl_iscan_unlock_data(wl);
		iscan->iscan_state = ISCAN_STATE_SCANING;
		wl_iscan_add_timer(iscan->timer, iscan->timer_ms, FALSE);
		iscan->timer_on = 1;
		return -1;
	} else if (iscan->timer_retry_cnt > 0) {
		WL_ISCAN(("wl_iscan_set_scan: failed. Retry pending...\n"));
		iscan->timer_retry_cnt--;
		iscan->iscan_state = ISCAN_STATE_PENDING;
		wl_iscan_add_timer(iscan->timer, iscan->timer_retry_ms, FALSE);
		iscan->timer_on = 1;
	} else {
		WL_ISCAN(("wl_iscan_set_scan: failed. Abort\n"));
		iscan->iscan_state = ISCAN_STATE_IDLE;
	}

	return 0;
}


void wl_e_scan_complete(wl_info_t *wl)
{
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	WL_ISCAN(("%s:\n", __FUNCTION__));
	if (iscan->iscan_state != ISCAN_STATE_IDLE) {
		if (iscan->timer_on) {
			wl_iscan_del_timer(iscan->timer);
			iscan->timer_on = 0;
		}
#if defined(NDIS60)
		NdisQueueIoWorkItem(iscan->IScanWorkItem, wl_iscan_workitem, (PVOID)wl);
#else /* NDIS60 */
		NdisScheduleWorkItem(&iscan->IScanWorkItem);
#endif /* NDIS60 */
	}
}

#if defined(EXT_STA)

#define MAX_APs	9999
static int
wl_iscan_get_scan(
	wl_info_t * wl,
	PVOID InfoBuf,
	ULONG InfoBufLen,
	PULONG BytesWritten,
	PULONG BytesNeeded
)
{
	DOT11_BYTE_ARRAY *byte_array = NULL;
	DOT11_BSS_ENTRY UNALIGNED  *Bss;
	uint var_IE_off = sizeof(NDIS_802_11_FIXED_IEs);
	ULONG MoveBytes;
	LONG BytesLeft;
	wl_scan_results_t *list;
	wl_bss_info_t *bi = NULL;
	uint ii;
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan_buf_t * p_buf;
	uint32  counter = 0;
	uint max_ap = 0;
	ULONG timeStamp;

	WL_ISCAN(("%s buflen_from_user %d:\n", __FUNCTION__, InfoBufLen));

	if (!InfoBuf) {
		WL_TRACE(("%s: SIOCGIWSCAN GET bad parameter\n", __FUNCTION__));
		return NDIS_STATUS_BUFFER_OVERFLOW;
	}
	byte_array = (DOT11_BYTE_ARRAY *)InfoBuf;

	/*
	 * Windows HCT requires that the NumberofItems returned, must
	 * be 0, if OID_802_11_BSSID_LIST is called after the radio
	 * has been disabled by OID_802_11_DISASSOCIATE
	 *
	 * Note: The bssid list is not cleared by OID_802_11_DISASSOCIATE.
	 *       So if OID_802_11_SSID is called to enable the radio, followed
	 *       immediately by a call to OID_802_11_BSSID_LIST, the previous
	 *       scan list will be returned.
	 */
	if (iscan->NDISradio_disabled) {
		WL_ISCAN(("%s: Radio is disabled -> Don't return scan results\n"));
		MoveBytes = sizeof(DOT11_BYTE_ARRAY);
		if (MoveBytes <= InfoBufLen) {
			byte_array->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
			byte_array->Header.Revision = DOT11_BSS_ENTRY_BYTE_ARRAY_REVISION_1;
			byte_array->Header.Size = sizeof(DOT11_BYTE_ARRAY);
			byte_array->uNumOfBytes = 0;
			*BytesWritten = MoveBytes;
		}
		return NDIS_STATUS_SUCCESS;
	}

	/* Check for scan in progress */
	if (iscan->iscan_state != ISCAN_STATE_IDLE) {
		WL_ISCAN(("%s: SIOCGIWSCAN GET still scanning, state = %d\n",
			__FUNCTION__, iscan->iscan_state));
		MoveBytes = sizeof(DOT11_BYTE_ARRAY);
		if (MoveBytes <= InfoBufLen) {
			byte_array->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
			byte_array->Header.Revision = DOT11_BSS_ENTRY_BYTE_ARRAY_REVISION_1;
			byte_array->Header.Size = sizeof(DOT11_BYTE_ARRAY);
			byte_array->uNumOfBytes = 0;
			*BytesWritten = MoveBytes;
		}
		return NDIS_STATUS_SUCCESS;
	}
	/* is there enough room for the byte array header? */
	MoveBytes = 0;

	wl_iscan_lock_data(wl);

	if (MoveBytes <= InfoBufLen) {
		byte_array->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
		byte_array->Header.Revision = DOT11_BSS_ENTRY_BYTE_ARRAY_REVISION_1;
		byte_array->Header.Size = sizeof(DOT11_BYTE_ARRAY);
		byte_array->uNumOfBytes = 0;

		/* is there enough room for the scan results? */
		p_buf = iscan->list_hdr;
		while (p_buf != iscan->list_cur && max_ap < MAX_APs) {
			list = &((wl_iscan_results_t*)p_buf->iscan_buf)->results;
			bi = NULL;
			for (ii = 0; ii < list->count && max_ap < MAX_APs; ii++) {
			    max_ap++;
			    bi = bi ? (wl_bss_info_t *)((uintptr)bi + dtoh32(bi->length))
				: list->bss_info;
			    MoveBytes += (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer) +	bi->ie_length);
			}
			p_buf = p_buf->next;
		} /* while (p_buf) */

		byte_array->uTotalNumOfBytes = MoveBytes;
		byte_array->uNumOfBytes = byte_array->uTotalNumOfBytes;
		MoveBytes += OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);
	}

	if (MoveBytes > InfoBufLen) {
		WL_ISCAN(("%s: ERROR: Bytes needed for broadcast results: %d (> than %d)\n",
		__FUNCTION__, byte_array->uTotalNumOfBytes, InfoBufLen));
		wl_iscan_unlock_data(wl);
		*BytesNeeded = MoveBytes;
		return NDIS_STATUS_BUFFER_OVERFLOW;
	}

	BytesLeft = MoveBytes - OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);
	max_ap = 0;
	p_buf = iscan->list_hdr;

	/* Get scan results */
	Bss = (DOT11_BSS_ENTRY *)byte_array->ucBuffer;
	while (p_buf != iscan->list_cur && max_ap < MAX_APs) {
	    list = &((wl_iscan_results_t*)p_buf->iscan_buf)->results;

	    counter += list->count;

	    if (list->version != WL_BSS_INFO_VERSION) {
			WL_ERROR(("list->version %d != WL_BSS_INFO_VERSION\n", list->version));
	    }

	    bi = NULL;
	    for (ii = 0; ii < list->count && max_ap < MAX_APs; ii++) {
			max_ap++;
			bi = bi ? (wl_bss_info_t *)((uintptr)bi + dtoh32(bi->length))
			: list->bss_info;

			/* Go from WL to Native WiFi format */
			{
				NDIS_802_11_FIXED_IEs *fixed_IEs = \
					(NDIS_802_11_FIXED_IEs *) ((uint8 *)bi) \
					+ sizeof(wl_bss_info_t) + var_IE_off;
				NDIS_802_11_NETWORK_TYPE NetworkTypeInUse;
			if (CHSPEC_IS2G(bi->chanspec))
#ifdef EXT_STA
#ifdef WL11N
					if (bi->flags & WLC_BSS_HT)
						NetworkTypeInUse = Ndis802_11FH;
					else
#endif /* WL11N */
#endif /* EXT_STA */
					if (wl_rateset_isofdm(bi->rateset.count, bi->rateset.rates))
						NetworkTypeInUse = Ndis802_11OFDM24;
					else
						NetworkTypeInUse = Ndis802_11DS;
			else
#ifdef EXT_STA
#ifdef WL11N
					if (bi->flags & WLC_BSS_HT)
						NetworkTypeInUse = Ndis802_11NetworkTypeMax;
					else
#endif /* WL11N */
#endif /* EXT_STA */
						NetworkTypeInUse = Ndis802_11OFDM5;

				Bss->uPhyId = wl_NTIU2phy_id(wl->oid, NetworkTypeInUse);
				Bss->PhySpecificInfo.uChCenterFrequency =
				wl_channel2freq(CHSPEC_CHANNEL(bi->chanspec));
				bcopy(bi->BSSID.octet, (char *)&Bss->dot11BSSID, ETHER_ADDR_LEN);
				Bss->dot11BSSType = dtoh16(bi->capability) & DOT11_CAP_ESS ?
				dot11_BSS_type_infrastructure : dot11_BSS_type_independent;
				Bss->lRSSI = bi->RSSI;
				Bss->uLinkQuality = wl_Rssi2LQ(bi->RSSI);

				Bss->bInRegDomain = TRUE;

				Bss->usBeaconPeriod = bi->beacon_period;
				NdisGetCurrentSystemTime((PLARGE_INTEGER)&timeStamp);
				Bss->ullTimestamp = *(ULONGLONG *)fixed_IEs->Timestamp;
				Bss->ullHostTimestamp = timeStamp;
				Bss->usCapabilityInformation = bi->capability;

				Bss->uBufferLength = bi->ie_length;
				if (Bss->uBufferLength) {
					bcopy(((uint8 *)bi) + sizeof(wl_bss_info_t),
					Bss->ucBuffer, Bss->uBufferLength);
				}
			}
			BytesLeft -= (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer) + Bss->uBufferLength);
			/* ASSERT(BytesLeft >= 0); */
			Bss = (DOT11_BSS_ENTRY UNALIGNED *)&Bss->ucBuffer[Bss->uBufferLength];

	    }
	    p_buf = p_buf->next;
	} /* while (p_buf) */

	WL_INFO(("Total number of AP's found  = %d\n", max_ap));

	wl_iscan_unlock_data(wl);

	/* ASSERT(BytesLeft == 0); */
	*BytesWritten = MoveBytes;
	WL_ISCAN(("%s returning %d (%d) bytes of scan info to ndis (APs=%d)\n",
	__FUNCTION__, MoveBytes, byte_array->uNumOfBytes, counter));
	return NDIS_STATUS_SUCCESS;
}

#else /* defined(EXT_STA) && defined(UNDER_CE) */

#ifdef __BBSOFT__
static void
wl_bss_print(wl_bss_info_t *bi)
{
	char ssid[33];
	memcpy(ssid, bi->SSID, bi->SSID_len);
	ssid[bi->SSID_len] = '\0';
	
	WL_PRINT(("SCAN RESULT : SSID[%32s], BSSID[%02x:%02x:%02x:%02x:%02x:%02x]\n",
			ssid,
			bi->BSSID.octet[0], bi->BSSID.octet[1], bi->BSSID.octet[2],
			bi->BSSID.octet[3], bi->BSSID.octet[4], bi->BSSID.octet[5]));
}
#endif

static int
wl_iscan_get_scan(
	wl_info_t * wl,
	PVOID InfoBuf,
	ULONG InfoBufLen,
	PULONG BytesWritten,
	PULONG BytesNeeded
)
{
	ULONG Length = 0;
	ULONG IELength;
	ULONG MoveBytes;
	NDIS_802_11_BSSID_LIST_EX *NdisList = (NDIS_802_11_BSSID_LIST_EX *) InfoBuf;
	NDIS_WLAN_BSSID_EX *Bssid;

	wl_scan_results_t *list;
	wl_bss_info_t *bi = NULL;
	uint ii;
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;
	iscan_buf_t * p_buf;
	uint32  counter = 0;

	WL_ISCAN(("%s buflen_from_user %d:\n", __FUNCTION__, InfoBufLen));
	if (!InfoBuf) {
		WL_TRACE(("%s: SIOCGIWSCAN GET bad parameter\n", __FUNCTION__));
		return 0;
	}
	Bssid = NdisList->Bssid;
	NdisList->NumberOfItems = 0;

	/*
	 * Windows HCT requires that the NumberofItems returned, must
	 * be 0, if OID_802_11_BSSID_LIST is called after the radio
	 * has been disabled by OID_802_11_DISASSOCIATE
	 *
	 * Note: The bssid list is not cleared by OID_802_11_DISASSOCIATE.
	 *       So if OID_802_11_SSID is called to enable the radio, followed
	 *       immediately by a call to OID_802_11_BSSID_LIST, the previous
	 *       scan list will be returned.
	 */
	if (iscan->NDISradio_disabled) {
		MoveBytes = sizeof(NDIS_802_11_BSSID_LIST_EX);
		if (MoveBytes <= InfoBufLen)
			((NDIS_802_11_BSSID_LIST_EX *) InfoBuf)->NumberOfItems = 0;
		return 0;
	}

	/* Check for scan in progress */
	if (iscan->iscan_state != ISCAN_STATE_IDLE) {
		WL_ISCAN(("%s: SIOCGIWSCAN GET still scanning, state = %d\n",
			__FUNCTION__, iscan->iscan_state));
		return 0;
	}
	/* calculate total request bytes */
	wl_iscan_lock_data(wl);
	MoveBytes = sizeof(ULONG);
	p_buf = iscan->list_hdr;
	while (p_buf != iscan->list_cur) {
	    list = &((wl_iscan_results_t*)p_buf->iscan_buf)->results;
	    bi = NULL;
	    for (ii = 0; ii < list->count; ii++) {
		bi = bi ? (wl_bss_info_t *)((uintptr)bi + dtoh32(bi->length)) : list->bss_info;
		IELength = bi->ie_length;
		MoveBytes += ROUNDUP(OFFSETOF(NDIS_WLAN_BSSID_EX, IEs) + IELength, 4);
	    }
	    p_buf = p_buf->next;
	} /* while (p_buf) */

	WL_ISCAN(("%s: SIOCGIWSCAN GET broadcast results\n", __FUNCTION__));
	Length += sizeof(ULONG);

	p_buf = iscan->list_hdr;
	/* Get scan results */
	while (p_buf != iscan->list_cur) {
	    list = &((wl_iscan_results_t*)p_buf->iscan_buf)->results;

	    counter += list->count;

	    if (list->version != WL_BSS_INFO_VERSION) {
		WL_ERROR(("list->version %d != WL_BSS_INFO_VERSION\n", list->version));
	    }

	    bi = NULL;
	    for (ii = 0; ii < list->count; ii++) {
		bi = bi ? (wl_bss_info_t *)((uintptr)bi + dtoh32(bi->length)) : list->bss_info;

		if (Length + 256 + sizeof(NDIS_WLAN_BSSID_EX) + bi->ie_length +
			sizeof(NDIS_802_11_FIXED_IEs) > InfoBufLen) {
			WL_ISCAN(("%s NDIS buf too small: need %d bytes, got %d bytes (APs=%d)\n",
			__FUNCTION__, Length, InfoBufLen, counter));
			wl_iscan_unlock_data(wl);
		return 0;
		}

#ifdef __BBSOFT__
		if (wl->dbgBSSList)
			wl_bss_print(bi);
		
		if (wl->apEnable) {
			int i;

			for (i = 0; i < 2; i++) {
				if (!memcmp(wl->apSSID[i], bi->SSID, 32))
					break;
			}

			if (i >= 2) continue;
		}
#endif

		NdisList->NumberOfItems++;

		wl_bss_wlc2ndis(bi, Bssid);

		/* fixup the total structure length */
		if (bi->ie_length) {
			bcopy(((uint8 *)bi) + sizeof(wl_bss_info_t),
			(char *) Bssid->IEs + sizeof(NDIS_802_11_FIXED_IEs),
				bi->ie_length);
			IELength = bi->ie_length + sizeof(NDIS_802_11_FIXED_IEs);
		}
		else {
			IELength = 0;
		}
		Bssid->Length = ROUNDUP(OFFSETOF(NDIS_WLAN_BSSID_EX, IEs) + IELength, 4);
		Length += Bssid->Length;

		/* add fixed and tagged parameters */
		Bssid->IELength = IELength;
		Bssid = (NDIS_WLAN_BSSID_EX *) ((int8*) Bssid + Bssid->Length);
	    }
	    p_buf = p_buf->next;
	} /* while (p_buf) */
	wl_iscan_unlock_data(wl);

	WL_ISCAN(("%s returning %d bytes of scan info to ndis (APs=%d)\n",
	__FUNCTION__, Length, counter));

	return 0;
}
#endif 

int wl_iscan_attach(wl_info_t *wl)
{
	iscan_info_t *iscan = NULL;

	if (!wl)
		return 0;

	iscan = MALLOC(wl->sh.osh, sizeof(iscan_info_t));
	if (!iscan) {
		WL_ISCAN(("%s MALLOC failed\n", __FUNCTION__));
		return NDIS_STATUS_FAILURE;
	}
	memset(iscan, 0, sizeof(iscan_info_t));
	/* we only care about main interface so save a global here */
	wl->iscan = (void *)iscan;
	iscan->iscan_state = ISCAN_STATE_IDLE;

	iscan->timer = &iscan->iscan_timer;
#if 0 // kdongsuk. for DOTEL
	iscan->timer_ms = 3000;
	iscan->timer_retry_ms = 250; // kdongsuk. for DOTEL

#else
	iscan->timer_ms = 3000;
	iscan->timer_retry_ms = 500;
#endif
	iscan->timer_retry_cnt = ISCAN_RETRY_CNT ;
	iscan->dev_state = NdisDeviceStateD0;
	wl_iscan_init_timer(wl, iscan->timer, wl_iscan_timerfn, (PVOID)wl);
#if defined(NDIS60)
	iscan->IScanWorkItem = NdisAllocateIoWorkItem(wl_adapter_handle);
	if (iscan->IScanWorkItem == NULL) {
		WL_ERROR(("NdisAllocateIoWorkItem() failed! wl_adapter_handle = 0x%x\n",
		wl_adapter_handle));
		return -1;
	}
	NdisAllocateSpinLock(&(iscan->SpinLock));
#else /* NDIS60 */
	NdisInitializeWorkItem(&iscan->IScanWorkItem,
		wl_iscan_workitem,
		(PVOID)wl);
#endif /* NDIS60 */

	WL_ISCAN(("%s iscan->iscan_state = %d\n", __FUNCTION__, iscan->iscan_state));
	return 0;
}

void wl_iscan_detach(wl_info_t *wl)
{
	iscan_buf_t  *buf;
	iscan_info_t *iscan = (iscan_info_t *)wl->iscan;

	if (!iscan)
		return;

	if (iscan->timer_on) {
		wl_iscan_del_timer(iscan->timer);
		iscan->timer_on = 0;
	}

	wl_iscan_lock_data(wl);
	if (wl->sh.osh) {
		while (iscan->list_hdr) {
			buf = iscan->list_hdr->next;
			MFREE(wl->sh.osh, iscan->list_hdr, sizeof(iscan_buf_t));
			iscan->list_hdr = buf;
		}
		MFREE(wl->sh.osh, iscan, sizeof(iscan_info_t));
	}
	wl_iscan_unlock_data(wl);

#if defined(NDIS60)
	NdisFreeSpinLock(&(iscan->SpinLock));
#endif
}
#endif /* WL_IW_USE_ISCAN */

#ifdef SIMPLE_ISCAN

int
wl_iscan_calculate_final_result_size(void)
{
	int size = 0, i, totatAp = 0;
	iscan_buf_t *iscan_cur;
	wl_iscan_results_t *list;
	wl_scan_results_t *results;
	wl_bss_info_t UNALIGNED *bi;

	iscan_cur = dhd_iscan_result_buf();

	while (iscan_cur) {
		list = (wl_iscan_results_t *)iscan_cur->iscan_buf;
		if (!list)
			break;

		results = (wl_scan_results_t *)&list->results;
		if (!results)
			break;

		if (results->version != WL_BSS_INFO_VERSION) {
			WL_ERROR(("%s: results->version %d != WL_BSS_INFO_VERSION\n", \
			__FUNCTION__, results->version));
			goto fail;
		}

		bi = results->bss_info;
		for (i = 0; i < results->count; i++) {
			if (!bi)
				break;
			size += (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer) +	bi->ie_length);
			bi = (wl_bss_info_t *)((uintptr)bi + dtoh32(bi->length));
			totatAp++;
		}
		iscan_cur = iscan_cur->next;
	}
	size += OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);

fail:
	WL_ISCAN(("%s: Size required for OID_DOT11_ENUM_BSS_LIST = %d, \
		total AP's found = %d\n", __FUNCTION__, size, totatAp));
	return size;
}

int
wl_iscan_get_final_result(
	wl_info_t * wl,
	PVOID InfoBuf,
	ULONG InfoBufLen,
	PULONG BytesWritten,
	PULONG BytesNeeded
)
{
	int status = NDIS_STATUS_FAILURE;
	int i, BytesLeft;
	int MoveBytes;
	DOT11_BYTE_ARRAY *byte_array = NULL;
	DOT11_BSS_ENTRY UNALIGNED  *Bss;
	iscan_buf_t *iscan_cur;
	wl_iscan_results_t *list;
	wl_scan_results_t *results;
	wl_bss_info_t UNALIGNED *bi;
	uint var_IE_off = sizeof(NDIS_802_11_FIXED_IEs);
	ULONG timeStamp;

	dhd_iscan_lock();

	if (!InfoBuf) {
		*BytesNeeded = 0;
		*BytesWritten = 0;
		goto fail;
	}
	byte_array = (DOT11_BYTE_ARRAY *)InfoBuf;
	memset(InfoBuf, 0, InfoBufLen);

	byte_array->Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	byte_array->Header.Revision = DOT11_BSS_ENTRY_BYTE_ARRAY_REVISION_1;
	byte_array->Header.Size = sizeof(DOT11_BYTE_ARRAY);

	MoveBytes = wl_iscan_calculate_final_result_size();

	byte_array->uNumOfBytes = MoveBytes - OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);
	byte_array->uTotalNumOfBytes = byte_array->uNumOfBytes;

	if (MoveBytes > InfoBufLen) {
		WL_ISCAN(("%s Input buffer length supplied for OID_DOT11_ENUM_BSS_LIST \
			request = %d < needed = %d\n", __FUNCTION__, InfoBufLen, MoveBytes));
		byte_array->uTotalNumOfBytes = byte_array->uNumOfBytes = 0;
		*BytesNeeded = MoveBytes;
		*BytesWritten = OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);
		status = NDIS_STATUS_BUFFER_OVERFLOW;
		goto fail;
	}

	/* Convert raw wl_bss_info_t into DOT11_BSS_ENTRY entries */
	Bss = (DOT11_BSS_ENTRY *)byte_array->ucBuffer;
	iscan_cur = dhd_iscan_result_buf();
	BytesLeft = MoveBytes - OFFSETOF(DOT11_BYTE_ARRAY, ucBuffer);

	while (iscan_cur) {
		list = (wl_iscan_results_t *)iscan_cur->iscan_buf;
		if (!list)
			break;

		results = (wl_scan_results_t *)&list->results;
		if (!results)
			break;

		if (results->version != WL_BSS_INFO_VERSION) {
			WL_ERROR(("%s: results->version %d != WL_BSS_INFO_VERSION\n",
			__FUNCTION__, results->version));
			break;
		}
		/* For every partial result loop through all the BSS reported */
		bi = results->bss_info;
		for (i = 0; i < results->count; i++) {
			if (!bi)
				break;

			if (BytesLeft < (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer) + bi->ie_length))
				break;
			{
				NDIS_802_11_FIXED_IEs *fixed_IEs = \
				(NDIS_802_11_FIXED_IEs *)((uint8 *)bi) \
				+ sizeof(wl_bss_info_t) + var_IE_off;
				NDIS_802_11_NETWORK_TYPE NetworkTypeInUse;

				if (CHSPEC_IS2G(bi->chanspec)) {
#ifdef EXT_STA
#ifdef WL11N
					if (bi->flags & WLC_BSS_HT)
						NetworkTypeInUse = Ndis802_11FH;
					else
#endif /* WL11N */
#endif /* EXT_STA */
					if (wl_rateset_isofdm(bi->rateset.count, bi->rateset.rates))
						NetworkTypeInUse = Ndis802_11OFDM24;
					else
						NetworkTypeInUse = Ndis802_11DS;
				} else {
#ifdef EXT_STA
#ifdef WL11N
					if (bi->flags & WLC_BSS_HT)
						NetworkTypeInUse = Ndis802_11NetworkTypeMax;
					else
#endif /* WL11N */
#endif /* EXT_STA */
						NetworkTypeInUse = Ndis802_11OFDM5;
				}
				Bss->uPhyId = wl_NTIU2phy_id(wl->oid, NetworkTypeInUse);
				Bss->PhySpecificInfo.uChCenterFrequency =
				wl_channel2freq(CHSPEC_CHANNEL(bi->chanspec));
				bcopy(bi->BSSID.octet, (char *)&Bss->dot11BSSID, ETHER_ADDR_LEN);
				Bss->dot11BSSType = dtoh16(bi->capability) & DOT11_CAP_ESS ?
				dot11_BSS_type_infrastructure : dot11_BSS_type_independent;
				Bss->lRSSI = bi->RSSI;
				Bss->uLinkQuality = wl_Rssi2LQ(bi->RSSI);

				Bss->bInRegDomain = TRUE;

				Bss->usBeaconPeriod = bi->beacon_period;
				NdisGetCurrentSystemTime((PLARGE_INTEGER)&timeStamp);
				Bss->ullTimestamp = *(ULONGLONG *)fixed_IEs->Timestamp;
				Bss->ullHostTimestamp = timeStamp;
				Bss->usCapabilityInformation = bi->capability;

				Bss->uBufferLength = bi->ie_length;
				if (Bss->uBufferLength) {
					bcopy(((uint8 *)bi) + sizeof(wl_bss_info_t),
					Bss->ucBuffer, Bss->uBufferLength);
				}
			}
			BytesLeft -= (OFFSETOF(DOT11_BSS_ENTRY, ucBuffer) + Bss->uBufferLength);
			Bss = (DOT11_BSS_ENTRY UNALIGNED *)&Bss->ucBuffer[Bss->uBufferLength];
			bi = (wl_bss_info_t *)((uintptr)bi + dtoh32(bi->length));
		}
		iscan_cur = iscan_cur->next;
	}
	*BytesNeeded = 0;
	*BytesWritten = MoveBytes;
	status = NDIS_STATUS_SUCCESS;

fail:
	dhd_iscan_unlock();
	return status;
}
#endif /* SIMPLE_ISCAN */

#if defined(NDIS60)
/* copy rateset src to dst as-is (no masking or sorting) */
void
wlc_rateset_copy(const wlc_rateset_t *src, wlc_rateset_t *dst)
{
	bcopy(src, dst, sizeof(wlc_rateset_t));
}

#ifdef WL11N
void
wlc_rateset_mcs_clear(wlc_rateset_t *rateset)
{
	uint i;
	for (i = 0; i < MCSSET_LEN; i++)
		rateset->mcs[i] = 0;
}
#endif /* WL11N */
/*
 * Copy and selectively filter one rateset to another.
 * 'basic_only' means only copy basic rates.
 * 'cck_only' means only copy cck (11b) rates.
 * 'xmask' is the copy mask (typically 0x7f or 0xff).
 */
void
wlc_rateset_filter(wlc_rateset_t *src, wlc_rateset_t *dst, bool basic_only, bool cck_only,
	uint xmask, bool mcsallow)
{
	uint i;
	uint r;
	uint count;

	count = 0;
	for (i = 0; i < src->count; i++) {
		r = src->rates[i];
		if (basic_only && !(r & WLC_RATE_FLAG))
			continue;
		if (cck_only && IS_OFDM((r & RATE_MASK)))
			continue;
		dst->rates[count++] = r & xmask;
	}
	dst->count = count;
	dst->htphy_membership = src->htphy_membership;

#ifdef WL11N
	if (mcsallow && !cck_only)
		bcopy(&src->mcs[0], &dst->mcs[0], MCSSET_LEN);
	else
		wlc_rateset_mcs_clear(dst);
#endif /* WL11N */
}

/* check if rateset is valid.
 * if check_brate is true, rateset without a basic rate is considered NOT valid.
 */
bool
wlc_rateset_valid(wlc_rateset_t *rs, bool check_brate)
{
	uint idx;

	if (!rs->count)
		return FALSE;

	if (!check_brate)
		return TRUE;

	/* error if no basic rates */
	for (idx = 0; idx < rs->count; idx++) {
		if (rs->rates[idx] & WLC_RATE_FLAG)
			return TRUE;
	}
	return FALSE;
}

/* filter based on hardware rateset, and sort filtered rateset with basic bit(s) preserved,
 * and check if resulting rateset is valid.
*/
bool
wlc_rate_hwrs_filter_sort_validate(wlc_rateset_t *rs,
const wlc_rateset_t *hw_rs, bool check_brate)
{
	uint8 rateset[WLC_MAXRATE+1];
	uint8 r;
	uint count;
	uint i;

	bzero(rateset, sizeof(rateset));
	count = rs->count;

	for (i = 0; i < count; i++) {
		/* mask off "basic rate" bit, WLC_RATE_FLAG */
		r = (int)rs->rates[i] & RATE_MASK;
		if ((r > WLC_MAXRATE) || (rate_info[r] == 0)) {
			WL_INFORM(("wlc_rate_hwrs_filter_sort: bad rate in rate set 0x%x\n", r));
			continue;
		}
		rateset[r] = rs->rates[i]; /* preserve basic bit! */
	}

	/* fill out the rates in order, looking at only supported rates */
	count = 0;
	for (i = 0; i < hw_rs->count; i++) {
		r = hw_rs->rates[i] & RATE_MASK;
		ASSERT(r <= WLC_MAXRATE);
		if (rateset[r])
			rs->rates[count++] = rateset[r];
	}

	rs->count = count;

#ifdef WL11N
		/* only set the mcs rate bit if the equivalent hw mcs bit is set */
		for (i = 0; i < MCSSET_LEN; i++)
			rs->mcs[i] = (rs->mcs[i] & hw_rs->mcs[i]);

#endif /* WL11N */

	if (wlc_rateset_valid(rs, check_brate))
		return TRUE;
	else
		return FALSE;
}

/* select rateset for a given phy_type and bandtype and filter it, sort it
 * and fill rs_tgt with result
 */
void
wlc_rateset_default(wlc_rateset_t *rs_tgt, const wlc_rateset_t *rs_hw, uint phy_type,
	int bandtype, bool cck_only, uint rate_mask, bool mcsallow, uint8 bw)
{
	const wlc_rateset_t *rs_dflt;
	wlc_rateset_t rs_sel;

	if ((phy_type == PHY_TYPE_LP) ||
		(phy_type == PHY_TYPE_N) ||
		(phy_type == PHY_TYPE_SSN)) {
		if (bandtype == WLC_BAND_5G)
			rs_dflt = (bw == WLC_20_MHZ ?
			           &ofdm_mimo_rates : &ofdm_40bw_mimo_rates);
		else
			rs_dflt = (bw == WLC_20_MHZ ?
			           &cck_ofdm_mimo_rates : &cck_ofdm_40bw_mimo_rates);
	} else if (phy_type == PHY_TYPE_A) {
		rs_dflt = &ofdm_rates;
	} else if (phy_type == PHY_TYPE_G) {
		rs_dflt = &cck_ofdm_rates;
	} else {
		ASSERT(0); /* should not happen */
		rs_dflt = &cck_rates; /* force cck */
	}

	/* if hw rateset is not supplied, assign selected rateset to it */
	if (!rs_hw)
		rs_hw = rs_dflt;

	wlc_rateset_copy(rs_dflt, &rs_sel);
#ifdef WL11N
	/* single stream PHY 20MHz only, only rates mcs0-7 are valid */
	if (mcsallow) {
		int i;
		if ((phy_type == PHY_TYPE_SSN) && (bw == WLC_20_MHZ)) {
			for (i = 1; i < MCSSET_LEN; i++)
				rs_sel.mcs[i] = 0x00;
		}
	}
#endif /* WL11N */
	wlc_rateset_filter(&rs_sel, rs_tgt, FALSE, cck_only, rate_mask, mcsallow);
	wlc_rate_hwrs_filter_sort_validate(rs_tgt, rs_hw, FALSE);
}
#endif /* NDIS60 && UNDER_CE */
