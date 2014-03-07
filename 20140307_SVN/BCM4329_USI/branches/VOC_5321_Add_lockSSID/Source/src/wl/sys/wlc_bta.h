/*
 * BTA (BlueTooth Alternate Mac and Phy module aka BT-AMP)
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wlc_bta.h,v 1.1.62.5 2009/07/09 18:15:18 Exp $
*/

#ifndef _wlc_bta_h_
#define _wlc_bta_h_

#ifdef WLBTAMP
struct wl_info;
struct amp_hci_event;
#ifdef WLBTAMP_HIGH
struct bta_info;
struct wlc_info;
struct scb;
struct wlc_frminfo;
struct wlc_if;
#ifdef WLBTAMP_HIGH_ONLY
struct dhd_info;
extern int wl_ioctl(struct wl_info *wl, uint cmd, void *buf, uint len);
extern int wl_iovar_getint(struct wl_info *wl, const char *name, int *val);
extern int wl_iovar_op(struct wl_info *wl, const char *name, void *buf, uint len, bool set);
extern int wl_bta_cmd_async(struct wl_info *wl, void *buf, uint32 bufLen);
extern int wl_bta_evt_async(struct wl_info *wl, void *buf, uint32 bufLen);
extern int _wlc_bta_docmd(struct bta_info *bta, uint8 *cmd, uint len);
extern void wlc_bta_cs_cb(struct bta_info *bta, uint8 plidx, chanspec_t chsp, uint8 numpls);
#endif
#endif /* BTAMP_HIGH */
#ifdef WLBTAMP_LOW
extern void wlc_bta_scan_abort(struct wlc_info *wlc, int status);
extern int wlc_cs_scan_start(struct wlc_info *wlc, wl_uint32_list_t *request, \
	bool bw40, bool active, void (*cb)(void *arg, int status), void *arg);
extern uint8 wlc_bta_phy_link_rssi(struct bta_info *bta, int plidx);
extern void wlc_bta_cs_complete(void *arg, int st);
extern void wlc_bta_flush_hcidata(struct bta_info *bta, uint16 llh);
extern uint8 wlc_bta_doconnect(struct bta_info *bta, int plidx, chanspec_t chanspec);
extern int wlc_bta_disconnect_physical_link(struct bta_info *bta, int plidx);
#endif /* WLBTAMP_LOW */

#define BTA_HCI_CMD_MAX_LEN (HCI_CMD_PREAMBLE_SIZE + HCI_CMD_DATA_SIZE)

#define USE_PRIMARY_BSSCFG	1

/* maximum MAC retries */
#ifndef WLBTAMP_D11RETRY
#define WLBTAMP_D11RETRY	15
#endif

/* BT-AMP logical link */
typedef struct bta_ll {
	uint8 plh;			/* associated physical link handle */
	uint8 plidx;			/* physical link states struct index in bta_info_t */
	uint8 prio;
	uint8 tx_fs_ID;			/* tx flow spec ID */
	uint32 fto;			/* flush timeout */
	uint8 datablks_complete;	/* number of data blocks completed */
} bta_ll_t;

/* BT-AMP physical link */
typedef struct bta_pl {
	uint8 *local;			/* local AMP_ASSOC */
	uint8 *remote;			/* remote AMP_ASSOC */
	uint16 llen;			/* local AMP_ASSOC length */
	uint16 rlen;			/* remote AMP_ASSOC length */
	uint ca_ts;				/* connection attempt timestamp */
	void *scb;			/* Keep the data size the same between high and low */
	uint8 link_key[32];
	uint8 lk_type_len;		/* link_key_type | link_key_length */
	uint8 flags;			/* see BTA_PL_XXXX */
	uint16 ls_to;			/* link supervision timeout */
	uint16 allocbw;			/* guaranteed bandwidth allocated for this physical link */
	uint8 short_range;		/* short range mode */
	uint used;
} bta_pl_t;

/* link_key type and length */
#define	BTA_PLK_LENGTH_MASK	0x3F
#define	BTA_PLK_TYPE_MASK	0xC0
#define BTA_PLK_TYPE_SHIFT	6

/* physical link flags */
#define	BTA_PL_CREATOR		0x01	/* creator */
#define BTA_PL_COMPLETE		0x02	/* link completed */
#define BTA_PL_USE_RTS		0x04	/* RTS/CTS protection */
#define BTA_PL_CSE_PEND		0x10	/* Channel Select event pending */
#define BTA_PL_CONN		0x20	/* connecting */

/* Channel Select info */
typedef struct bta_cs {
	/* the index of the physical link that initiated the process */
	int plidx;
} bta_cs_t;

#define WLC_MAXBTA		4
#define BTA_MAXLOGLINKS		(WLC_MAXBTA * 4)

/* BTA module info */
struct bta_info {
#ifdef WLBTAMP_HIGH_ONLY
	struct dhd_info *dhd;	/* pointer to main dhd structure */
#endif
	struct wlc_info *wlc;	/* pointer to main wlc structure */
	int scb_handle;			/* scb cubby handle to retrieve data from scb */
	bta_ll_t ll[BTA_MAXLOGLINKS];	/* array of logical links (indexed by handle) */
	bta_pl_t pl[WLC_MAXBTA];	/* array of physical links */
	uint8 plh[WLC_MAXBTA];		/* physical link handles */
	uint8 *local;			/* default local AMP_ASSOC */
	uint16 llen;			/* length of default local AMP_ASSOC */
	uint16 lla_to;			/* logical link accept timeout */
	uint16 ca_to;			/* connection accept timeout */
	uint8 flags;			/* see BTA_FLAGS_XXXX */
	uint8 numpls;			/* number of active physical links */
	void *bsscfg;		/* Keep the data size the same between high and low */
	struct wl_timer *cse_timer;	/* deferred Channel Select event timer */
	bta_cs_t *cs;				/* Channel Select control block */
	chanspec_t chanspec_sel;	/* auto selected channel */
	chanspec_t chanspec_user;	/* user specified channel */
	uint8 datablks_pending;		/* number of date blocks being xmitted */
	uint8 datablks_complete;	/* number of data blocks completed */

	/* global states to save and restore */
	bool _apsta;
	bool _ap;
	uint8 PM;
	uint8 infra;
#if WLBTAMP_D11RETRY > 7
	uint8 SRL;
	uint8 LRL;
#endif
#ifdef USE_PRIMARY_BSSCFG
	/* primary bsscfg states to save and restore */
	uint8 assoc_retry_max;
	bool sup_enable_wpa;
	bool wsec_restrict;
	bool eap_restrict;
	uint16 WPA_auth;
	uint32 wsec;
#endif
};

extern struct bta_info *wlc_bta_attach(struct wlc_info *wlc);
extern void wlc_bta_detach(struct bta_info *bta);

extern bool wlc_bta_qos(struct bta_info *bta);
extern bool wlc_bta_active(struct bta_info *bta);

extern void wlc_bta_join_complete(struct bta_info *bta, struct scb *scb, uint8 status);
extern void wlc_bta_AKM_complete(struct bta_info *bta, struct scb *scb);

extern chanspec_t wlc_bta_chan_select(struct bta_info *bta);
extern void wlc_bta_chan_notify(struct bta_info *bta, chanspec_t chanspec);

extern bool wlc_bta_recv_proc(struct bta_info *bta, struct wlc_frminfo *f, struct scb *scb);
extern bool wlc_bta_send_proc(struct bta_info *bta, void *p, struct wlc_if **wlcif);

extern void wlc_bta_tx_hcidata_complete(struct bta_info *bta, void *p, uint status);
extern void wlc_bta_tx_hcidata_lost(struct bta_info *bta, void *p);

extern void wlc_bta_tx_hcidata(struct bta_info *bta, uint8 *data, uint len);
extern int wlc_bta_docmd(struct bta_info *bta, uint8 *cmd, uint len);
extern void wlc_bta_doevent(struct bta_info *bta, struct amp_hci_event *event);

extern uint8 wlc_bta_pllookup(struct bta_info *bta, uint8 plh, int *plidx);

extern void wlc_bta_scb_cleanup(struct bta_info *bta, struct scb *scb);

#if defined(WLBTAMP_LOW_ONLY) || defined(WLBTAMP_HIGH_ONLY)
/* rpc functions */
enum {
	BTAMP_RPC_SET_CA_TS = 1,
	BTAMP_RPC_VALID_CHANSPEC,
	BTAMP_RPC_GET_REGCLASS,
	BTAMP_RPC_PHY_LINK_RSSI,
	BTAMP_RPC_CS_SCAN_START,
	BTAMP_RPC_BTA_CONNECT,
	BTAMP_RPC_BTA_DISCONNECT,
	BTAMP_RPC_UPDATE_LLINK
};

typedef struct wlc_bta_set_ca_ts {
	uint32	result;
	uint32	plidx;
} wlc_bta_set_ca_ts_t;

typedef struct wlc_bta_valid_chaspec {
	uint32	result;
	uint32	chspec;
	uint32	dualband;
} wlc_bta_valid_chanspec_t;

typedef struct wlc_bta_get_regclass {
	uint32	result;
	uint32	chspec;
} wlc_bta_get_regclass_t;

typedef struct wlc_bta_phy_link_rssi {
	uint32	result;
	uint32	plidx;
} wlc_bta_phy_link_rssi_t;

typedef struct wlc_bta_cs_scan_start {
	int32  result;
	uint32 bw40;
	uint32 active;
	uint32 plidx;
	uint32 chanspec_sel;
	uint32 plh;
	uint32 len;
	uint32 ca_to;
	uint32 flags;
	bta_pl_t pl;
	struct bta_info bta;
	wl_uint32_list_t list;
} wlc_bta_cs_scan_start_t;

typedef struct wlc_bta_connect {
	uint32 result;
	uint32 plidx;
	uint32 chanspec;
	uint32 plh;
	uint32 len;
	uint32 ca_to;
	uint32 flags;
	bta_pl_t pl;
	struct bta_info bta;
} wlc_bta_connect_t;

typedef struct wlc_bta_disconnect {
	int32 result;
	uint32 plidx;
	struct bta_info bta;
} wlc_bta_disconnect_t;

typedef struct wlc_bta_update_llink {
	int32 result;
	int32 plidx;
	int32 llidx;
	bta_pl_t plink;
	bta_ll_t llink;
} wlc_bta_update_llink_t;

/*
 * This HCI event is defined to carry additional info to high driver
 */
#include <proto/bt_amp_hci.h>
typedef struct chanspec_select_evt {
	amp_hci_event_t evt;
	uint8 plidx;
	uint8 numpls;
	uint16 chanspec;
} chanspec_select_evt_t;

#endif /* WLBTAMP_LOW_ONLY || WLBTAMP_HIGH_ONLY */

typedef struct wlc_flush_hcidata {
	uint32 llh;
} wlc_flush_hcidata_t;

#else	/* stubs */

#define wlc_bta_attach(a) (struct bta_info *)0x0dadbeef
#define	wlc_bta_detach(a) do {} while (0)

#endif /* WLBTAMP */

#endif /* _wlc_bta_h_ */
