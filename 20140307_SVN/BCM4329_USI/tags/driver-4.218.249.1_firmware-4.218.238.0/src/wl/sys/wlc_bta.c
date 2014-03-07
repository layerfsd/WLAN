/*
 * BTA (BlueTooth Alternate Mac and Phy module aka BT-AMP)
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
 * $Id: wlc_bta.c,v 1.2.12.1.16.11 2009/09/10 03:48:47 Exp $
 */
#ifndef WLBTAMP
#error "WLBTAMP is not defined"
#endif	/* WLBTAMP */

#include <wlc_cfg.h>
#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <siutils.h>
#include <bcmendian.h>
#include <proto/802.11.h>
#include <wlioctl.h>
#include <bcmwpa.h>
#include <bcmwifi.h>
#include <sbhndpio.h>
#include <sbhnddma.h>
#include <hnddma.h>
#include <d11.h>
#include <wlc_rate.h>
#include <wlc_pub.h>
#include <wlc_key.h>
#include <wlc_channel.h>
#include <wlc_bsscfg.h>
#include <wlc.h>
#include <wlc_scb.h>
#include <wlc_frmutil.h>
#ifdef BCMSUP_PSK
#include <proto/eapol.h>
#include <wlc_sup.h>
#endif
#ifdef BCMAUTH_PSK
#include <wlc_auth.h>
#endif
#include <proto/bt_amp_hci.h>
#include <proto/802.11_bta.h>
#include <wl_export.h>
#include <wlc_bta.h>
#ifdef APCS
#include <wlc_apcs.h>
#include <wlc_scan.h>
#endif
#ifdef WLBTAMP_HIGH_ONLY
#include <dhd_dbg.h>
#include <dhd_bta.h>
#endif

#ifdef WLBTWUSB
extern VOID BtKernForwardEvent(void *Context, PVOID buffer, ULONG len);
extern VOID BtKernForwardData(void *Context, PVOID buffer, ULONG len);
#endif

/* iovar table */
enum {
	IOV_BT_AMP,		/* enable/disable BT-AMP */
	IOV_BT_AMP_FLAGS,	/* test parameters */
	IOV_HCI_CMD,		/* HCI command */
	IOV_HCI_ACL_DATA,	/* HCI data packet */
	IOV_CHAN,		/* specify channel */
	IOV_BTAMP_ROLE,		/* BTAMP role */
	IOV_BTAMP_KEY,		/* BTAMP key */
	IOV_BTAMP_RPC,		/* BTAMP RPC commands */
};

static const char BTAMP_RPC[] = "bta_rpc";

static const bcm_iovar_t bta_iovars[] = {
	{"btamp", IOV_BT_AMP, IOVF_SET_DOWN, IOVT_BOOL, 0},
	{"btamp_flags", IOV_BT_AMP_FLAGS, 0, IOVT_UINT8, 0},
	{"HCI_cmd", IOV_HCI_CMD, 0, IOVT_BUFFER, 0},
	{"HCI_ACL_data", IOV_HCI_ACL_DATA, 0, IOVT_BUFFER, 0},
	{"btamp_chan", IOV_CHAN, 0, IOVT_UINT8, 0},
	{"btamp_role", IOV_BTAMP_ROLE, 0, IOVT_UINT8, 0},
	{"btamp_key", IOV_BTAMP_KEY, 0, IOVT_BUFFER, 0},
#ifdef WLBTAMP_LOW_ONLY
	{BTAMP_RPC, IOV_BTAMP_RPC, 0, IOVT_BUFFER, 0},
#endif
	{NULL, 0, 0, 0, 0}
};

#define WLC_MAXBTA		4
#define BTA_MAXLOGLINKS		(WLC_MAXBTA * 4)

/* maximum PDU allowed (1492 octets) */
#define BTA_MTU	(ETHER_MAX_DATA - DOT11_LLC_SNAP_HDR_LEN)

/* maximum number of data blocks allowed (no validation yet) */
#define BTA_MAXDATABLKS		48

/* bsscfg flags */
#define BTA_BSSCFG_FLAGS	(WLC_BSSCFG_NOBCMC | \
				 WLC_BSSCFG_NOVIF)
#ifdef USE_PRIMARY_BSSCFG
#define PRIMARY_BSSCFG_FLAGS	(WLC_BSSCFG_BTA | \
				 WLC_BSSCFG_NOBCMC | \
				 WLC_BSSCFG_NOVIF | \
				 WLC_BSSCFG_NOROAM)
#endif

/* invalid flush timeout value */
#define BTA_INVFTO		0xffffffff

/* BTA module specific state */
#define BTA_FLAGS_NO_ASSOC		0x01	/* acceptor doesn't have to associate w/ creator */
#define BTA_FLAGS_NO_SECURITY		0x02	/* no 4-way handshake */
#define BTA_FLAGS_NO_QOS		0x04	/* no QoS processing */
#define BTA_FLAGS_ET_RX			0x08	/* preserve Ethernet header */

#define BTA_FLAGS_QTP_AR		0x10	/* send activity report spec'd in Qual Test Plan */

#define BTA_FLAGS_NO_CATO		0x80	/* no link accept timeout (for debugging) */

/* scb cubby structure */
typedef struct scb_bta {
	uint8 plh;
	uint8 plidx;
} scb_bta_t;

#define SCB_BTA(bta, scb)	(scb_bta_t *)SCB_CUBBY((scb), (bta)->scb_handle)

/* local prototypes */
static void scb_bta_init(void *context, struct scb *scb);
static void scb_bta_deinit(void *context, struct scb *scb);

static int wlc_bta_doiovar(void *context, const bcm_iovar_t *vi, uint32 actionid,
	const char *name, void *p, uint plen, void *a, int alen, int vsize,
	struct wlc_if *wlcif);

#ifdef WLBTAMP_LOW
static int wlc_bta_dodisconnect(bta_info_t *bta, int plidx);
#endif

static void wlc_bta_doconnect_error(bta_info_t *bta, int plidx, uint8 status);

static void wlc_bta_phy_link_complete(bta_info_t *bta, struct scb *scb, uint8 status);
static void wlc_bta_phy_link_complete_event(bta_info_t *bta, uint8 *buf, uint buflen,
	bool disconnect);

static void wlc_bta_tx_hcidata_complete_event(bta_info_t *bta, uint16 llh, bool flush);

static void wlc_bta_disconnect_logical_link(bta_info_t *bta, uint16 llh);

static uint8 wlc_bta_valid_logical_link(bta_info_t *bta, uint16 llh);

static void wlc_bta_flush_hcidata_occurred_event(bta_info_t *bta, uint16 llh);

static amp_hci_event_t *wlc_bta_alloc_hcievent(bta_info_t *bta, uint8 ecode, uint8 plen);
static void wlc_bta_free_hcievent(bta_info_t *bta, amp_hci_event_t *evt);

static void wlc_bta_send_link_supervision_pkt(bta_info_t *bta, bta_pl_t *pl, bool request);
static void wlc_bta_send_activity_report_pkt(bta_info_t *bta, bta_pl_t *phy_link);
static void wlc_bta_send_hcidata_pkt(bta_info_t *bta, uint16 llh, void *p);

static void wlc_bta_evtfwd_upd(bta_info_t *bta);
static void wlc_bta_flags_upd(bta_info_t *bta);

#ifdef WLBTAMP_LOW
static void wlc_bta_send_activity_report_pkt(bta_info_t *bta, bta_pl_t *phy_link);
static void wlc_bta_phy_link_complete(bta_info_t *bta, struct scb *scb, uint8 status);
extern bool wlc_bta_send_proc(bta_info_t *bta, void *p, wlc_if_t **wlcif);
static int wlc_bta_doiovar(void *context, const bcm_iovar_t *vi, uint32 actionid,
	const char *name, void *p, uint plen, void *a, int alen, int vsize,
	struct wlc_if *wlcif);
static int wlc_bta_watchdog(void *context);
static int wlc_bta_down(void *context);
static uint8 wlc_bta_join(wlc_info_t *wlc, wlc_bsscfg_t *bsscfg,
	uint8 *SSID, int SSID_len, uint8 *bssid, chanspec_t chanspec);
#endif

#ifdef WLBTAMP_HIGH
static uint8 wlc_bta_build_AMP_ASSOC(bta_info_t *bta, chanspec_t chanspec,
	uint8 **AMP_ASSOC, uint16 *AMP_ASSOC_len);
static uint8 wlc_bta_create_local_AMP_ASSOC(bta_info_t *bta,
	chanspec_t chanspec, uint8 **AMP_ASSOC, uint16 *AMP_ASSOC_len);
static uint8 wlc_bta_cs(bta_info_t *bta, int plidx, uint8 *pref_chan, 
                          uint pref_chan_len, chanspec_t *chanspec_sel);
#ifdef WLBTAMP_HIGH_ONLY
uint8 hci_cmd_buf[BTA_HCI_CMD_MAX_LEN];
uint hci_cmd_len = 0;
#endif
#endif

static uint8 *wlc_bta_parse_tlvs(void *buf, int buflen, uint key);
static void wlc_bta_add_hdrs(bta_pl_t *pl, void *p, uint16 type);

static void wlc_bta_default(bta_info_t *bta);

#if defined(BCMDBG) && !defined(WLBTAMP_HIGH_ONLY)
static int wlc_bta_dump(bta_info_t *bta, struct bcmstrbuf *b);
#endif

extern void wlc_bta_vendor_specific_event(bta_info_t *bta, uint8 *data, uint8 len);

/* default local AMP_ASSOC */
static const uint8 default_local[] = {
	0x04, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x05, 0x05, 0x00, 0x01, 0x0f, 0x00, 0x10, 0x09,
	/* BTA_TYPE_MAC_ADDRESS and BTA_TYPE_ID_PREFERRED_CHANNELS are here... */
};

/* AMP SSID format */
static const char *amp_ssid_format = "AMP-%02x-%02x-%02x-%02x-%02x-%02x";

bta_info_t *
BCMATTACHFN(wlc_bta_attach)(wlc_info_t *wlc)
{
	bta_info_t *bta;


	WL_TRACE(("wl%d: wlc_bta_attach: wlc->%x\n", wlc->pub.unit, wlc));

	/* sanity checks */
	ASSERT(WLC_MAXBTA > 0);

	if ((bta = (bta_info_t *)MALLOC(wlc->pub.osh, sizeof(bta_info_t))) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_attach: OOM, malloced %d bytes\n",
		          wlc->pub.unit, MALLOCED(wlc->pub.osh)));
		return NULL;
	}

	bzero((char *)bta, sizeof(bta_info_t));

	bta->wlc = wlc;

#ifdef WLBTAMP_LOW
	/* reserve cubby in the scb container for per-scb private data */
	bta->scb_handle = wlc_scb_cubby_reserve(wlc, sizeof(scb_bta_t),
		scb_bta_init, scb_bta_deinit, NULL, (void *)bta);

	if (bta->scb_handle < 0) {
		WL_ERROR(("wl%d: wlc_scb_cubby_reserve() failed\n", wlc->pub.unit));
		goto fail;
	}

	/* register module */
	if (wlc_module_register(&wlc->pub, bta_iovars, "bta", bta,
	                        wlc_bta_doiovar, wlc_bta_watchdog, wlc_bta_down)) {
		WL_ERROR(("wl%d: bta wlc_module_register() failed\n", wlc->pub.unit));
		goto fail;
	}
#endif


	wlc_bta_default(bta);

	/* temporary inits */
	wlc->pub._bta = TRUE;

	/* initial flags */
	/* bta->flags |= BTA_FLAGS_NO_SECURITY; */

	/* update event forwarding mask */
	wlc_bta_evtfwd_upd(bta);

	/* update stuff depending on flags */
	wlc_bta_flags_upd(bta);

#if defined(BCMDBG) && defined(WLBTAMP_LOW)
	wlc_dump_register(&wlc->pub, "bta", (dump_fn_t)wlc_bta_dump, (void *)bta);
#endif

#ifdef WLBTAMP_HIGH_ONLY
	wlc->bta = bta;

	/*
	 * We need to collect some default info from dongle
	 */
	wl_iovar_op(wlc->wl, "country", wlc->country_abbrev, WLC_CNTRY_BUF_SZ, IOV_GET);
	wl_iovar_op(wlc->wl, "cur_etheraddr", &wlc->pub.cur_etheraddr, ETHER_ADDR_LEN, IOV_GET);
	wl_iovar_op(wlc->wl, "chanspec", &bta->chanspec_sel, sizeof(chanspec_t), IOV_GET);
#endif

	return bta;

#ifdef WLBTAMP_LOW
fail:
#endif
	MFREE(wlc->pub.osh, bta, sizeof(bta_info_t));
	return NULL;
}

void
BCMATTACHFN(wlc_bta_detach)(bta_info_t *bta)
{
	wlc_info_t *wlc = bta->wlc;
	int i;

	ASSERT(bta && bta->wlc);
	if (wlc->bta == NULL)
		return;

#ifdef WLBTAMP_LOW
	wlc_module_unregister(&wlc->pub, "bta", bta);
#endif
	for (i = 0; i < WLC_MAXBTA; i++) {
		bta_pl_t *phy_link = &bta->pl[i];

		if (phy_link->local)
			MFREE(wlc->pub.osh, phy_link->local, phy_link->llen);
		if (phy_link->remote)
			MFREE(wlc->pub.osh, phy_link->remote, phy_link->rlen);
	}
	if (bta->local)
		MFREE(wlc->pub.osh, bta->local, bta->llen);
#ifdef WLBTAMP_LOW
	if (bta->cse_timer != NULL)
		wl_free_timer(wlc->wl, bta->cse_timer);
#endif
	if (bta->cs != NULL)
		MFREE(wlc->pub.osh, bta->cs, sizeof(bta_cs_t));
	MFREE(wlc->pub.osh, bta, sizeof(bta_info_t));

	wlc->bta = NULL;
}


#ifdef WLBTAMP_HIGH_ONLY
static char *
wlc_bta_mkiovar(wlc_info_t *wlc, const char *cmd, char **p, int *len)
{
	char *buf;

	*len += strlen(cmd) + 1;
	if ((buf = (char *)MALLOC(wlc->pub.osh, *len)) == NULL)
		WL_ERROR(("%s: OOM\n", __FUNCTION__));

	bzero(buf, *len);
	strcpy(buf, cmd);
	*p = &buf[strlen(cmd)+1];

	return buf;
}

void
wlc_get_valid_chanspecs(wlc_info_t *wlc, wl_uint32_list_t *list, bool bw20, bool band2G, char *abbrev)
{
	int len = sizeof(chanspec_t) + WLC_CNTRY_BUF_SZ
		+ sizeof(uint32) * (MAXCHANNEL+1);
	char *buf, *p;
	wl_uint32_list_t *pl;

	buf = wlc_bta_mkiovar(wlc, "chanspecs", &p, &len);
	*(chanspec_t *)p = (band2G? WL_CHANSPEC_BAND_2G : 0)
				| (bw20? WL_CHANSPEC_BW_20 : 0);
	strncpy(p+sizeof(chanspec_t), abbrev, WLC_CNTRY_BUF_SZ);
	p[sizeof(chanspec_t)+WLC_CNTRY_BUF_SZ-1] = '\0';
	wl_ioctl(wlc->wl, WLC_GET_VAR, buf, len);
	pl = (wl_uint32_list_t *)buf;
	if (pl->count > 0)
		memcpy(list, pl, sizeof(pl->count)+sizeof(pl->element[0])*pl->count);
	else
		WL_ERROR(("%s: returned 0\n", __FUNCTION__));

	MFREE(wlc->pub.osh, buf, len);
}

static void
wlc_bta_set_ca_ts(bta_info_t *bta, int plidx)
{
	wlc_bta_set_ca_ts_t *rc;
	int len = sizeof(*rc);
	char *buf;

	buf = wlc_bta_mkiovar(bta->wlc, BTAMP_RPC, (char **)&rc, &len);
	rc->result = BTAMP_RPC_SET_CA_TS;
	rc->plidx = plidx;
	wl_ioctl(bta->wlc->wl, WLC_GET_VAR, buf, len);
	MFREE(bta->wlc->pub.osh, buf, len);
}

bool
wlc_valid_chanspec(wlc_info_t *wlc, chanspec_t chspec, bool dualband)
{
	wlc_bta_valid_chanspec_t *rc;
	int len = sizeof(*rc);
	char *buf;
	bool result;

	buf = wlc_bta_mkiovar(wlc, BTAMP_RPC, (char **)&rc, &len);
	rc->result = BTAMP_RPC_VALID_CHANSPEC;
	rc->chspec = chspec;
	rc->dualband = dualband;
	wl_ioctl(wlc->wl, WLC_GET_VAR, buf, len);
	result = (bool)rc->result;
	MFREE(wlc->pub.osh, buf, len);

	return result;
}

uint8
wlc_get_regclass(wlc_info_t *wlc, chanspec_t chanspec)
{
	wlc_bta_get_regclass_t *rc;
	int len = sizeof(*rc);
	char *buf;
	uint8 result;

	buf = wlc_bta_mkiovar(wlc, BTAMP_RPC, (char **)&rc, &len);
	rc->result = BTAMP_RPC_GET_REGCLASS;

	rc->chspec = chanspec;
	wl_ioctl(wlc->wl, WLC_GET_VAR, buf, len);
	result = (uint8)rc->result;
	MFREE(wlc->pub.osh, buf, len);

	return result;
}

static void
wlc_bta_sync(bta_info_t *bta, bta_info_t *rbta, int plidx, int cmd, int result)
{
	switch (cmd) {
	case BTAMP_RPC_BTA_CONNECT:
		if (result == HCI_SUCCESS)
		{
			if (!(bta->pl[plidx].flags & BTA_PL_CREATOR))
			{
				bta->pl[plidx].flags |= BTA_PL_COMPLETE;
			}
		}
		break;
	case BTAMP_RPC_BTA_DISCONNECT:
		if (result == HCI_SUCCESS)
		{
			uint16 llidx;
			bta_pl_t *phy_link;

			phy_link = &bta->pl[plidx];
			/* free local and remote AMP_ASSOCs */
			if (phy_link->local)
				MFREE(bta->wlc->pub.osh, phy_link->local, phy_link->llen);
			if (phy_link->remote)
				MFREE(bta->wlc->pub.osh, phy_link->remote, phy_link->rlen);
			/* clear the rest of the physical link */
			bzero(phy_link, sizeof(bta_pl_t));
			for (llidx = 0; llidx < BTA_MAXLOGLINKS; llidx ++)
				if (bta->ll[llidx].plh == bta->plh[plidx])
					wlc_bta_disconnect_logical_link(bta, llidx);
			bta->plh[plidx] = 0;
			if (--bta->numpls < 0)
				bta->numpls = 0;
		}
		break;
	default:
		break;
	}
}

/* make sure AMP_ASSOC/AMP_ASSOC_len are not NULL */
/*
 * wlc_bta_doconnect() calls
 * wlc_bsscfg_get_free_idx()
 * wlc_bsscfg_alloc()
 * wlc_bsscfg_bta_init()
 * wlc_bsscfg_enable()
 * wlc_set()
 * wlc_bta_activate()
 * wlc_bsscfg_primary()
 * wlc_bsscfg_reinit_ext()
 * wlc_txc_upd()
 * wlc_scblookupband()
 * wlc_scb_setstatebit()
 * wlc_bta_join_complete() -> wlc_bta_phy_link_complete() -> wlc_set_sup()
 * wlc_auth_set_ea()								-> wlc_find_sup_auth_ies()
 * wlc_auth_set_pmk()
 * wlc_sup_set_ea()
 * wlc_sup_set_pmk()
 * wl_init_timer()
 * wl_add_timer()
 * wlc_bta_cse_timer()
 * wlc_bta_join() -> wlc_join()
 */
uint8
wlc_bta_doconnect(bta_info_t *bta, int plidx, chanspec_t chanspec)
{
	char *buf, *pbuf;
	wlc_bta_connect_t *p;
	uint len, plen;
	uint8 result;
	bta_pl_t *phy_link;
	wlc_info_t *wlc = bta->wlc;

	phy_link = &bta->pl[plidx];
	/* if needed, update local AMP_ASSOC */
	if (phy_link->llen == 0)
	{
		uint8 status;

		status = wlc_bta_create_local_AMP_ASSOC(bta, chanspec, &phy_link->local, &phy_link->llen);
		if (status != HCI_SUCCESS)
		{
			WL_ERROR(("wl%d: wlc_bta_doconnect: fail to build local AMP_ASSOC for "
					  "physical link %d\n", wlc->pub.unit, bta->plh[plidx]));
			return status;
		}
	}

	plen = len = sizeof(*p) +
				 bta->pl[plidx].rlen +
				 bta->pl[plidx].llen;

	pbuf = wlc_bta_mkiovar(bta->wlc, BTAMP_RPC, (char **)&p, &plen);
	buf = (char *)p;
	p->result = BTAMP_RPC_BTA_CONNECT;
	p->len = len;
	p->plidx = plidx;
	p->chanspec = chanspec;
#if defined(BCMDBG)
	bta->flags |= BTA_FLAGS_NO_CATO | BTA_FLAGS_NO_SECURITY;
#endif
	p->flags = bta->flags;
	p->ca_to = bta->ca_to;
	memcpy((char *)&p->pl, (char *)&bta->pl[plidx], sizeof(bta_pl_t));
	p->plh = bta->plh[plidx];
	if (p->pl.rlen > 0)
		memcpy(&buf[len -= p->pl.rlen], bta->pl[plidx].remote, p->pl.rlen);
	if (p->pl.llen > 0)
		memcpy(&buf[len -= p->pl.llen], bta->pl[plidx].local, p->pl.llen);

	wl_ioctl(bta->wlc->wl, WLC_GET_VAR, pbuf, plen);
	result = (uint8)p->result;
	wlc_bta_sync(bta, &p->bta, plidx, BTAMP_RPC_BTA_CONNECT, p->result);
	MFREE(bta->wlc->pub.osh, pbuf, plen);

	return result;
}

/*
 * wlc_bta_disconnect_physical_link() calls
 * wlc_bta_enable_btcx() -> wlc_enable_btc_ps_protection()
 * wlc_scbfree()
 * wlc_assoc_abort()
 * wlc_bsscfg_disable()
 * wlc_bsscfg_reinit_ext()
 * wlc_bsscfg_free()
 * wl_del_timer()
 * wl_free_timer()
 * wlc_bta_deactivate()
 */
int
wlc_bta_disconnect_physical_link(bta_info_t *bta, int plidx)
{
	char *buf;
	wlc_bta_disconnect_t *p;
	uint len;
	int result;

	len = sizeof(*p);

	buf = wlc_bta_mkiovar(bta->wlc, BTAMP_RPC, (char **)&p, &len);
	p->result = BTAMP_RPC_BTA_DISCONNECT;
	p->plidx = plidx;
	wl_ioctl(bta->wlc->wl, WLC_GET_VAR, buf, len);
	result = (int)p->result;
	wlc_bta_sync(bta, &p->bta, plidx, BTAMP_RPC_BTA_DISCONNECT, result);
	MFREE(bta->wlc->pub.osh, buf, len);

	return result;
}

uint8
wlc_bta_phy_link_rssi(bta_info_t *bta, int plidx)
{
	wlc_bta_phy_link_rssi_t *rc;
	int len = sizeof(*rc);
	char *buf;
	uint8 result;

	buf = wlc_bta_mkiovar(bta->wlc, BTAMP_RPC, (char **)&rc, &len);
	rc->result = BTAMP_RPC_PHY_LINK_RSSI;
	rc->plidx = plidx;
	wl_ioctl(bta->wlc->wl, WLC_GET_VAR, buf, len);
	result = (uint8)rc->result;
	MFREE(bta->wlc->pub.osh, buf, len);

	return result;
}

int
wlc_cs_scan_start(wlc_info_t *wlc, wl_uint32_list_t *request, bool bw40, bool active, void (*cb)(void *arg, int status), void *arg)
{
	char *buf, *pbuf;
	wlc_bta_cs_scan_start_t *p;
	int len, plen, result;
	int plidx=0;
	bta_pl_t *phy_link;

	if (request->count <= 0)
		return -1;

	if (wlc->bta->cs)
		plidx = wlc->bta->cs->plidx;
	phy_link = &wlc->bta->pl[plidx];

	/* if creator, update local AMP_ASSOC */
	if (wlc->bta->pl[plidx].flags & BTA_PL_CREATOR) {
		uint8 status;

		status = wlc_bta_create_local_AMP_ASSOC(wlc->bta, wlc->bta->chanspec_sel, &phy_link->local, &phy_link->llen);
		if (status != HCI_SUCCESS) {
			WL_ERROR(("%s: wlc_bta_doconnect: fail to build local AMP_ASSOC for "
					  "physical link %d\n", __FUNCTION__, wlc->bta->plh[plidx]));
			return -1;
		}
	}

	plen = len = sizeof(*p) + sizeof(request->count) +
				 sizeof(request->element[0]) * request->count +
				 wlc->bta->pl[plidx].rlen +
				 wlc->bta->pl[plidx].llen;

	pbuf = wlc_bta_mkiovar(wlc, BTAMP_RPC, (char **)&p, &plen);
	buf = (char *)p;
	p->result = BTAMP_RPC_CS_SCAN_START;
	p->bw40 = bw40;
	p->active = active;
	p->plidx = plidx;
	p->plh = wlc->bta->plh[plidx];
	p->len = len;
#if defined(BCMDBG)
	wlc->bta->flags |= BTA_FLAGS_NO_CATO | BTA_FLAGS_NO_SECURITY;
#endif
	p->flags = wlc->bta->flags;
	p->ca_to = wlc->bta->ca_to;
	memcpy((char *)&p->pl, (char *)phy_link, sizeof(bta_pl_t));
	p->plh = wlc->bta->plh[plidx];
	if (p->pl.rlen > 0)
		memcpy(&buf[len -= p->pl.rlen], wlc->bta->pl[plidx].remote, p->pl.rlen);
	if (p->pl.llen > 0)
		memcpy(&buf[len -= p->pl.llen], wlc->bta->pl[plidx].local, p->pl.llen);
	memcpy(&p->list, request, len - sizeof(wlc_bta_cs_scan_start_t));
	wl_ioctl(wlc->wl, WLC_GET_VAR, pbuf, plen);
	result = p->result;
	MFREE(wlc->pub.osh, pbuf, plen);
	if (wlc->bta->cs)
	{
		MFREE(wlc->pub.osh, wlc->bta->cs, sizeof(bta_cs_t));
		wlc->bta->cs = NULL;
	}

	return result;
}

static int
wlc_bta_update_logical_link(bta_info_t *bta, int plidx, int llidx)
{
	wlc_bta_update_llink_t *rc;
	int len = sizeof(*rc);
	char *buf;
	int result;

	buf = wlc_bta_mkiovar(bta->wlc, BTAMP_RPC, (char **)&rc, &len);
	rc->result = BTAMP_RPC_UPDATE_LLINK;
	rc->plidx = plidx;
	rc->llidx = llidx;
	memcpy(&rc->plink, &bta->pl[plidx], sizeof(rc->plink));
	memcpy(&rc->llink, &bta->ll[llidx], sizeof(rc->llink));
	wl_ioctl(bta->wlc->wl, WLC_GET_VAR, buf, len);
	result = (int)rc->result;
	MFREE(bta->wlc->pub.osh, buf, len);

	return result;
}

#else	/* !WLBTAMP_HIGH_ONLY */

static void
wlc_bta_set_ca_ts(bta_info_t *bta, int plidx)
{
	bta->pl[plidx].ca_ts = bta->wlc->pub.now;
}

#ifdef WLBTAMP_LOW_ONLY
static int
wlc_bta_dorpc(bta_info_t *bta, int32 cmd, void *pval, int len)
{
	wlc_info_t *wlc = bta->wlc;
	int err = BCME_OK;

	switch (cmd) {
	case BTAMP_RPC_SET_CA_TS:
		{
			wlc_bta_set_ca_ts_t *p = (wlc_bta_set_ca_ts_t *)pval;

			wlc_bta_set_ca_ts(bta, p->plidx);
			p->result = BCME_OK;
		}
		break;
	case BTAMP_RPC_VALID_CHANSPEC:
		{
			wlc_bta_valid_chanspec_t *p = (wlc_bta_valid_chanspec_t *)pval;

			p->result = (uint32)wlc_valid_chanspec(wlc,
				(chanspec_t)p->chspec, (bool)p->dualband);
		}
		break;
	case BTAMP_RPC_GET_REGCLASS:
		{
			wlc_bta_get_regclass_t *p = (wlc_bta_get_regclass_t *)pval;

			p->result = (uint32)wlc_get_regclass(wlc, (chanspec_t)p->chspec);
		}
		break;
	case BTAMP_RPC_PHY_LINK_RSSI:
		{
			wlc_bta_phy_link_rssi_t *p = (wlc_bta_phy_link_rssi_t *)pval;

			p->result = (uint32)wlc_bta_phy_link_rssi(bta, (int)p->plidx);
		}
		break;
	case BTAMP_RPC_CS_SCAN_START:
		{
			extern void wlc_bta_cs_complete(void *arg, int st);
			wlc_bta_cs_scan_start_t *p = (wlc_bta_cs_scan_start_t *)pval;
			bta_pl_t *pl = &bta->pl[p->plidx];
			int len;

			if (pl->remote != NULL)
			{
				MFREE(wlc->pub.osh, pl->remote, pl->rlen);
				pl->remote = NULL;
			}
			if (pl->local != NULL)
			{
				MFREE(wlc->pub.osh, pl->local, pl->llen);
				pl->local = NULL;
			}
			memcpy((char *)pl, (char *)&p->pl, sizeof(bta_pl_t));
			bta->flags = p->flags | BTA_FLAGS_ET_RX;
			bta->ca_to = p->ca_to;
			bta->plh[p->plidx] = p->plh;
			if (pl->rlen && (pl->remote = (uint8 *)MALLOC(wlc->pub.osh, pl->rlen)) == NULL)
			{
				p->result = BCME_NORESOURCE;
				WL_ERROR(("wl%d: %s(line %d): OOM(%d bytes)\n", wlc->pub.unit, __FUNCTION__, __LINE__, pl->rlen));
				break;
			}
			if (pl->llen && (pl->local = (uint8 *)MALLOC(wlc->pub.osh, pl->llen)) == NULL)
			{
				MFREE(wlc->pub.osh, pl->remote, p->pl.rlen);
				pl->remote = NULL;
				p->result = BCME_NORESOURCE;
				WL_ERROR(("wl%d: %s(line %d): OOM(%d bytes)\n", wlc->pub.unit, __FUNCTION__, __LINE__, pl->llen));
				break;
			}
			len = p->len - pl->rlen;
			memcpy(pl->remote, &(((char *)p)[len]), pl->rlen);
			len -= pl->llen;
			memcpy(pl->local, &(((char *)p)[len]), pl->llen);

			/* allocate a parameter block to pass to complete callback */
			bta->cs = MALLOC(wlc->pub.osh, sizeof(bta_cs_t));
			if (bta->cs == NULL) {
				WL_ERROR(("wl%d: %s(line %d): OOM(%d bytes)\n", wlc->pub.unit, __FUNCTION__, __LINE__, sizeof(bta_cs_t)));
				p->result = BCME_NORESOURCE;
				break;
			}
			bta->cs->plidx = p->plidx;
			p->result = (int32)wlc_cs_scan_start(wlc, &p->list, p->bw40, p->active, wlc_bta_cs_complete, bta);
			memcpy((char *)&p->bta, (char *)bta, sizeof(bta_info_t));
			p->chanspec_sel = (uint32)wlc_bta_chan_select(bta);
		}
		break;
	case BTAMP_RPC_BTA_CONNECT:
		{
			wlc_bta_connect_t *p = (wlc_bta_connect_t *)pval;
			bta_pl_t *pl = &bta->pl[p->plidx];

			if (pl->remote != NULL)
				MFREE(wlc->pub.osh, pl->remote, pl->rlen);
			if (pl->local != NULL)
				MFREE(wlc->pub.osh, pl->local, pl->llen);
			memcpy((char *)pl, (char *)&p->pl, sizeof(bta_pl_t));
			bta->flags = p->flags | BTA_FLAGS_ET_RX;
			bta->ca_to = p->ca_to;
			bta->plh[p->plidx] = p->plh;
			if ((pl->remote = (uint8 *)MALLOC(wlc->pub.osh, p->pl.rlen)) == NULL)
			{
				p->result = BCME_NORESOURCE;
				break;
			}
			if ((pl->local = (uint8 *)MALLOC(wlc->pub.osh, p->pl.llen)) == NULL)
			{
				MFREE(wlc->pub.osh, bta->pl[p->plidx].remote, p->pl.rlen);
				bta->pl[p->plidx].remote = NULL;
				p->result = BCME_NORESOURCE;
				break;
			}
			len = p->len - p->pl.rlen;
			memcpy(pl->remote, &(((char *)p)[len]), p->pl.rlen);
			len -= p->pl.llen;
			memcpy(pl->local, &(((char *)p)[len]), p->pl.llen);
			p->result = wlc_bta_doconnect(bta, p->plidx, p->chanspec);
			memcpy((char *)&p->bta, (char *)bta, sizeof(bta_info_t));
		}
		break;
	case BTAMP_RPC_BTA_DISCONNECT:
		{
			wlc_bta_disconnect_t *p = (wlc_bta_disconnect_t *)pval;

			p->result = wlc_bta_disconnect_physical_link(bta, p->plidx);
			memcpy((char *)&p->bta, (char *)bta, sizeof(bta_info_t));
		}
		break;
	case BTAMP_RPC_UPDATE_LLINK:
		{
			wlc_bta_update_llink_t *p = (wlc_bta_update_llink_t *)pval;

			memcpy(&bta->ll[p->llidx], &p->llink, sizeof(p->llink));
			bta->plh[p->plidx] = p->llink.plh;
			bta->pl[p->plidx].allocbw = p->plink.allocbw;
			p->result = BCME_OK;
		}
		break;
	default:
		err = BCME_UNSUPPORTED;
		break;
	}

	return err;
}
#endif /* WLBTAMP_LOW_ONLY */

/* handle BTA related iovars */
static int
wlc_bta_doiovar(void *context, const bcm_iovar_t *vi, uint32 actionid, const char *name,
	void *p, uint plen, void *a, int alen, int vsize, struct wlc_if *wlcif)
{
	bta_info_t *bta = (bta_info_t *)context;
	wlc_info_t *wlc = bta->wlc;
	int32 int_val = 0;
	bool bool_val;
	int err = 0;

	ASSERT(bta == wlc->bta);

#ifdef WLBTAMP_LOW
	/*
	 * Leave the validation to dongle
	 */
	if ((err = wlc_iovar_check(&wlc->pub, vi, a, alen, IOV_ISSET(actionid))) != 0)
		return err;
#endif

	if (plen >= (int)sizeof(int_val))
		bcopy(p, &int_val, sizeof(int_val));

	bool_val = (int_val != 0) ? TRUE : FALSE;

	switch (actionid) {
	case IOV_SVAL(IOV_HCI_CMD): {
		amp_hci_cmd_t *cmd = (amp_hci_cmd_t *)a;

		/* sanity check: command preamble present */
		if (alen < (int)HCI_CMD_PREAMBLE_SIZE)
			return BCME_BUFTOOSHORT;

		/* sanity check: command parameters are present */
		if (alen < (int)(HCI_CMD_PREAMBLE_SIZE + cmd->plen))
			return BCME_BUFTOOSHORT;

		wlc_bta_docmd(bta, (uint8 *)cmd, alen);
		break;
	}

	case IOV_SVAL(IOV_HCI_ACL_DATA): {
		amp_hci_ACL_data_t *ACL_data = (amp_hci_ACL_data_t *)a;
		uint16 dlen;

		/* sanity check: HCI header present */
		if (alen < (int)HCI_ACL_DATA_PREAMBLE_SIZE)
			return BCME_BUFTOOSHORT;

		/* sanity check: ACL data is present */
		dlen = ltoh16(ACL_data->dlen);
		if (alen < (int)(HCI_ACL_DATA_PREAMBLE_SIZE + dlen))
			return BCME_BUFTOOSHORT;

		wlc_bta_tx_hcidata(bta, (uint8 *)ACL_data, alen);
		break;
	}

	case IOV_GVAL(IOV_BT_AMP):
		*((uint32 *)a) = BTA_ENAB(&wlc->pub);
		break;

	case IOV_SVAL(IOV_BT_AMP):
		wlc->pub._bta = bool_val;
		break;

	case IOV_GVAL(IOV_BT_AMP_FLAGS):
		*((uint32 *)a) = (uint32)bta->flags;
		break;

	case IOV_SVAL(IOV_BT_AMP_FLAGS): {
		uint8 flags = (uint8)int_val;

		/* use of security and QoS require association */
		if ((flags & BTA_FLAGS_NO_ASSOC) &&
		    (!(flags & BTA_FLAGS_NO_SECURITY) || !(flags & BTA_FLAGS_NO_QOS))) {
			WL_ERROR(("wl%d: use of security and QoS require association\n",
			          wlc->pub.unit));
			err = BCME_EPERM;
		} else {
			bta->flags = flags;
#if defined(WLBTAMP_HIGH)
			bta->flags |= BTA_FLAGS_ET_RX;
#endif
		}

		/* update event forwarding */
		wlc_bta_evtfwd_upd(bta);
		/* update stuff depending on flags */
		wlc_bta_flags_upd(bta);
		break;
	}

	case IOV_SVAL(IOV_CHAN):
		bta->chanspec_user = CH20MHZ_CHSPEC((uint8)int_val);
		break;

	case IOV_GVAL(IOV_BTAMP_ROLE):
		{
			uint32 val;

			val = (uint32)(bta->flags & BTA_PL_CREATOR);
			WL_BTA(("wl%d: return role as %s(%x)\n",
				wlc->pub.unit, val?"Creator":"Acceptor", val));
			memcpy(a, &val, sizeof(val));
		}
		break;
	case IOV_GVAL(IOV_BTAMP_KEY):
		{
		struct scb *scb = NULL;
		scb_bta_t *scb_bta;
		uint8 band;

		/*
		 * We use the current chanspec to lookup the associated SCB
		 */
		band = CHSPEC_WLCBANDUNIT(wlc->pub.current_bss.chanspec);
		/* find or create BT-AMP peer scb */
		if (!(scb = wlc_scblookupband(wlc, (struct ether_addr *)p, band))) {
			WL_BTA(("wl%d: no SCB @ %08x\n", wlc->pub.unit, *(uint32 *)p));
			err = HCI_ERR_UNSUPPORTED_VALUE;
			break;
		}
		/* fixup scb cubby */
		scb_bta = SCB_BTA(bta, scb);

		WL_BTA(("wl%d: return key for plidx %d\n", wlc->pub.unit, scb_bta->plidx));
		memcpy(a, bta->pl[scb_bta->plidx].link_key, sizeof(bta->pl[0].link_key));
		}
		break;
#ifdef WLBTAMP_LOW_ONLY
	case IOV_GVAL(IOV_BTAMP_RPC):
	case IOV_SVAL(IOV_BTAMP_RPC):
		err = wlc_bta_dorpc(bta, int_val, p, plen);
		break;
#endif /* WLBTAMP_LOW_ONLY */

	default:
		err = BCME_UNSUPPORTED;
	}

	return err;
}

static int
wlc_bta_watchdog(void *context)
{
	bta_info_t *bta = (bta_info_t *)context;
	wlc_info_t *wlc = bta->wlc;
	uint now;
	int plidx;

	now = wlc->pub.now;

	/* For each physical link... */
	for (plidx = 0; plidx < WLC_MAXBTA; plidx++) {
		bta_pl_t *phy_link;
		uint8 plh;

		if (bta->plh[plidx] == 0)
			continue;

		plh = bta->plh[plidx];

		phy_link = &bta->pl[plidx];

		if (phy_link->flags & BTA_PL_COMPLETE) {
			uint time = (now - phy_link->used) * 1600;

			/* if completed, check link supervision timeout */
			if (time > (uint)phy_link->ls_to) {
				dis_phy_link_evt_parms_t evt_parms;

				WL_BTA(("wl%d: link supervision timeout for plh 0x%x\n",
					wlc->pub.unit, plh));

				/* kill the physical link */
				wlc_bta_disconnect_physical_link(bta, plidx);

				/* generate Disconnect Physical Link Complete event */
				evt_parms.status = HCI_SUCCESS;
				evt_parms.plh = plh;
				evt_parms.reason = HCI_ERR_CONNECTION_TIMEOUT;
				wlc_bta_phy_link_complete_event(bta, (uint8 *)&evt_parms,
				                                sizeof(evt_parms), TRUE);
			}
			/* if completed and close to link supervision timeout, probe */
			else if (time > ((uint)phy_link->ls_to >> 1)) {
				/* send link supervision request */
				wlc_bta_send_link_supervision_pkt(bta, phy_link, TRUE);
			}
		} else {

			/* if pending, check connection accept timeout */
			if (!(bta->flags & BTA_FLAGS_NO_CATO) &&
			    ((now - phy_link->ca_ts) * 1600) > bta->ca_to) {
				phy_link_evt_parms_t evt_parms;

				WL_BTA(("wl%d: connection accept timeout for plh 0x%x, now=%d, ca_ts=%d, ca_to=%d\n", wlc->pub.unit, plh, now, phy_link->ca_ts, bta->ca_to));

				/* kill the physical link */
				wlc_bta_disconnect_physical_link(bta, plidx);

				/* generate Physical Link Complete event */
				evt_parms.status = HCI_ERR_CONNECTION_ACCEPT_TIMEOUT;
				evt_parms.plh = plh;
				wlc_bta_phy_link_complete_event(bta, (uint8 *)&evt_parms,
				                                sizeof(evt_parms), FALSE);
			}
		}
	}

	return 0;
}

static int
wlc_bta_down(void *context)
{
	bta_info_t *bta = (bta_info_t *)context;
	int plidx;
	int callbacks = 0;

	for (plidx = 0; plidx < WLC_MAXBTA; plidx ++) {
		if (bta->plh[plidx] == 0)
			continue;
		WL_BTA(("wl%d: wlc_bta_down: disconnec physical link %d\n",
		        bta->wlc->pub.unit, bta->plh[plidx]));
		callbacks += wlc_bta_disconnect_physical_link(bta, plidx);
	}

	return callbacks;
}

static void
scb_bta_init(void *context, struct scb *scb)
{
}

static void
scb_bta_deinit(void *context, struct scb *scb)
{
	bta_info_t *bta = (bta_info_t *)context;
	scb_bta_t *scb_bta;

	scb_bta = SCB_BTA(bta, scb);
	bzero(scb_bta, sizeof(scb_bta_t));
}

uint8
wlc_bta_phy_link_rssi(bta_info_t *bta, int plidx)
{
	bta_pl_t *pl = &bta->pl[plidx];
	int rssi = WLC_RSSI_MINVAL;
	uint8 rval = 0;

	if (pl->scb != NULL)
		rssi = wlc_scb_rssi(pl->scb);

	if (rssi != WLC_RSSI_MINVAL) {
		if (rssi < -90)
			rval = 0;
		else if (rssi < -70)
			rval = (rssi + 90) * 2;
		else if (rssi < -68)
			rval = ((rssi + 70) * 10) + 40;
		else if (rssi < -58)
			rval = ((rssi + 68) * 2) + 60;
		else if (rssi < -30)
			rval = (((rssi + 58) * 5) / 7) + 80;
		else
			rval = 100;
		rval += 150;
	}
	return rval;
}

/* call it from external entry points, for example, PAL and watchdog */
int
wlc_bta_disconnect_physical_link(bta_info_t *bta, int plidx)
{
	uint16 llidx;

	/* disconnect any still-connected logical links */
	for (llidx = 0; llidx < BTA_MAXLOGLINKS; llidx ++) {
		if (bta->ll[llidx].plh == bta->plh[plidx]) {
			wlc_bta_disconnect_logical_link(bta, llidx);
		}
	}

#ifdef APCS
	/* abort ongoing Channel Select process */
	if (bta->cs != NULL) {
		wlc_scan_abort(bta->wlc->scan, WLC_E_STATUS_CS_ABORT);
		/* wlc_bta_cs_complete() will take care the rest of disconnect */
		return 0;
	}
#endif

	/* do the rest of disconnecting physical link */
	return wlc_bta_dodisconnect(bta, plidx);
}
#endif /* !WLBTAMP_HIGH_ONLY */

static void
wlc_bta_default(bta_info_t *bta)
{
	/* set default connection accept timeout */
	bta->ca_to = 0x1f40;

	/* set default logical link accept timeout */
	bta->lla_to = 0x1f40;
}

/* AMP HCI ACL data packet processing */
#if defined(BCMDBG) || defined(WLMSG_BTA)
static void
wlc_bta_hcidump_ACL_data(bta_info_t *bta, amp_hci_ACL_data_t *ACL_data, bool tx)
{
	wlc_info_t *wlc = bta->wlc;
	uint16 handle = ltoh16(ACL_data->handle);
	uint16 dlen = ltoh16(ACL_data->dlen);

	WL_BTA(("wl%d: %s ACL data: handle 0x%04x flags 0x%02x dlen %d\n", wlc->pub.unit,
		tx ? "<" : ">", HCI_ACL_DATA_HANDLE(handle), HCI_ACL_DATA_FLAGS(handle), dlen));
	prhex(NULL, ACL_data->data, dlen);
	WL_BTA(("\n"));
}
#endif /* BCMDBG || WLMSG_BTA */

#ifdef WLBTAMP_LOW
static void
wlc_bta_send_link_supervision_pkt(bta_info_t *bta, bta_pl_t *phy_link, bool request)
{
	wlc_info_t *wlc = bta->wlc;
	void *pkt;

	/* get an OSL packet w/ appropriate headroom */
	if (!(pkt = PKTGET(wlc->pub.osh, TXOFF + RFC1042_HDR_LEN, TRUE))) {
		WL_ERROR(("wl%d: wlc_bta_send_link_supervision_pkt: pktget error for len %d\n",
		          wlc->pub.unit, 0));
		return;
	}

	/* headers only and no payload */
	PKTPULL(wlc->pub.osh, pkt, TXOFF + RFC1042_HDR_LEN);

	/* add Ethernet header with 802.11 PAL LLC/SNAP header */
	wlc_bta_add_hdrs(phy_link, pkt, request ?
	                 BTA_PROT_LINK_SUPERVISION_REQUEST :
	                 BTA_PROT_LINK_SUPERVISION_REPLY);

	/* RTS/CTS protection */
	if (phy_link->flags & BTA_PL_USE_RTS)
		WLPKTTAG(pkt)->flags |= WLF_USERTS;

	/* submit to wl tx path */
	wlc_sendpkt(wlc, pkt, SCB_WDS(phy_link->scb));
}

/* Return TRUE if the packet must be freed by the caller otherwise allow the
 * packet to go up to the stack
 */
bool
wlc_bta_recv_proc(bta_info_t *bta, struct wlc_frminfo *f, struct scb *scb)
{
	wlc_info_t *wlc = bta->wlc;
	uint16 type = ntoh16(((struct dot11_llc_snap_header *)&f->eh[1])->type);
	uint16 len = ntoh16(f->eh->ether_type);
	amp_hci_ACL_data_t *HCI_hdr = NULL;
	bool toss = FALSE;
	bta_pl_t *phy_link;
	scb_bta_t *scb_bta;
	bool update = TRUE;

	ASSERT(scb != NULL);

	scb_bta = SCB_BTA(bta, scb);
	phy_link = &bta->pl[scb_bta->plidx];

	switch (type) {
	case BTA_PROT_L2CAP: {

		uint8 *src = (uint8 *)f->eh;
		uint16 handle, dlen;
		int i;

		/* insert HCI ACL data header between RFC1042 header and HCI ACL data */
		/* copy Ethernet/LLC/SNAP header back to preserve HCI ACL data header */
		for (i = 0; i < RFC1042_HDR_LEN; i++)
			src[i - HCI_ACL_DATA_PREAMBLE_SIZE] = src[i];

		/* update frame info fields */
		PKTPUSH(wlc->pub.osh, f->p, HCI_ACL_DATA_PREAMBLE_SIZE);
#if defined(EXT_STA) && defined(NOT_YET)
		f->h = (struct dot11_header *)
			((uintptr)f->h - HCI_ACL_DATA_PREAMBLE_SIZE);
#else
		f->eh = (struct ether_header *)
			((uintptr)f->eh - HCI_ACL_DATA_PREAMBLE_SIZE);
#endif
		f->pbody -= HCI_ACL_DATA_PREAMBLE_SIZE;

		/* update packet headers (with insertion of HCI ACL data header) */
		f->eh->ether_type = hton16(len + HCI_ACL_DATA_PREAMBLE_SIZE);
		handle = HCI_ACL_DATA_BC_FLAGS | HCI_ACL_DATA_PB_FLAGS | scb_bta->plh;
		dlen = PKTLEN(wlc->pub.osh, f->p) - RFC1042_HDR_LEN - HCI_ACL_DATA_PREAMBLE_SIZE;
		HCI_hdr = (amp_hci_ACL_data_t *)((uintptr)f->eh + RFC1042_HDR_LEN);
		HCI_hdr->handle = htol16(handle);
		HCI_hdr->dlen = htol16(dlen);

		/* Forward HCI ACL data only */
		if (!(bta->flags & BTA_FLAGS_ET_RX)) {
#ifdef WLBTWUSB
			/* Forward data to BTKRNL */
			WL_BTA(("wl%d: forwarding rx HCI data frame to BTKRNL\n", wlc->pub.unit));
			BtKernForwardData(wlc->wl, HCI_hdr, dlen + HCI_ACL_DATA_PREAMBLE_SIZE);

#endif
			toss = TRUE;
		}
		/* Forward HCI ACL data along with the Ethernet/LLC/header */
		/* else {} */

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON())
			wlc_bta_hcidump_ACL_data(bta, HCI_hdr, FALSE);
#endif

		break;
	}

	case BTA_PROT_ACTIVITY_REPORT: {

		amp_hci_activity_report_t *activity_report_body;

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			WL_BTA(("wl%d: received Activity Report on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, f->p), PKTLEN(wlc->pub.osh, f->p));
			WL_BTA(("\n"));
		}
#endif

		/* strip Ethernet/LLC/SNAP header */
		PKTPULL(wlc->pub.osh, f->p, RFC1042_HDR_LEN);

		/* 
		 * process activity report:
		 * if ScheduleKnown is set to 0, enable RTS
		 * else if NumReports is NOT 0, enable RTS or schedule
		 * else disable RTS
		 */
		activity_report_body = (amp_hci_activity_report_t *)PKTDATA(wlc->pub.osh, f->p);
		if (!(activity_report_body->ScheduleKnown & HCI_AR_SCHEDULE_KNOWN)) {
			/* enable RTS */
			phy_link->flags |= BTA_PL_USE_RTS;
		} else if (activity_report_body->NumReports != 0) {
#ifdef NOT_YET
#else
			/* ...or enable RTS */
			phy_link->flags |= BTA_PL_USE_RTS;
#endif
		} else {
			/* disable RTS */
			phy_link->flags &= ~BTA_PL_USE_RTS;
		}

		WL_BTA(("wl%d: turn %s RTS/CTS on physical link %d\n", wlc->pub.unit,
		        (phy_link->flags & BTA_PL_USE_RTS) ? "on" : "off",
		        bta->plh[scb_bta->plidx]));

		toss = TRUE;
		break;
	}

	case BTA_PROT_SECURITY: {

#if defined(BCMAUTH_PSK) || defined(BCMSUP_PSK)
		wlc_bsscfg_t *bsscfg = SCB_BSSCFG(scb);
#endif
		bool sendup = TRUE;

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			WL_BTA(("wl%d: received BT-SIG 802.1x frame on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, f->p), PKTLEN(wlc->pub.osh, f->p));
			WL_BTA(("\n"));
		}
#endif

		if (bta->flags & BTA_FLAGS_NO_SECURITY) {
			toss = TRUE;
			break;
		}

		/* call in-driver supplicant or authenticator */
		if (phy_link->flags & BTA_PL_CREATOR) {
#ifdef BCMAUTH_PSK
			if (bsscfg->authenticator != NULL)
				sendup = FALSE;
#endif
		}
		else {
#ifdef BCMSUP_PSK
			if (bsscfg->sup != NULL)
				sendup = FALSE;
#endif
		}
		/* forward to in-driver supplicant or authenticator */
		if (!sendup) {
			PKTPULL(wlc->pub.osh, f->p, DOT11_LLC_SNAP_HDR_LEN);
			f->eh = (struct ether_header *)PKTDATA(wlc->pub.osh, f->p);
			f->eh->ether_type = HTON16(ETHER_TYPE_802_1X);
			bcopy(f->sa, f->eh->ether_shost, ETHER_ADDR_LEN);
			bcopy(f->da, f->eh->ether_dhost, ETHER_ADDR_LEN);
		}

		/* mark packet as 802.1x (since ether_type will be hard to detect) */
		WLPKTTAG(f->p)->flags |= WLF_8021X;

		/* don't update the tick count as the link is not complete yet */
		update = FALSE;

		/* continue the rx processing... */
		break;
	}

	case BTA_PROT_LINK_SUPERVISION_REQUEST:

		/* received link supervision request... */

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			WL_BTA(("wl%d: received Link Supervision Request on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, f->p), PKTLEN(wlc->pub.osh, f->p));
			WL_BTA(("\n"));
		}
#endif

		/* send link supervision reply */
		wlc_bta_send_link_supervision_pkt(bta, phy_link, FALSE);

		toss = TRUE;
		break;

	case BTA_PROT_LINK_SUPERVISION_REPLY:

		/* received link supervision reply... */

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			WL_BTA(("wl%d: received Link Supervision Reply on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, f->p), PKTLEN(wlc->pub.osh, f->p));
			WL_BTA(("\n"));
		}
#endif

		toss = TRUE;
		break;

	default:

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			WL_BTA(("wl%d: wlc_bta_recv_proc: unrecognized protocol: 0x%04x\n",
			        wlc->pub.unit, type));
			prhex(NULL, PKTDATA(wlc->pub.osh, f->p), PKTLEN(wlc->pub.osh, f->p));
			WL_BTA(("\n"));
		}
#endif

		update = FALSE;
		toss = TRUE;
		break;
	}

	/* update phy_link's last activity time */
	if (update)
		phy_link->used = wlc->pub.now;

	return toss;
}

void
wlc_bta_vendor_specific_event(bta_info_t *bta, uint8 *data, uint8 len)
{
	amp_hci_event_t *evt;
	vendor_specific_evt_parms_t *parms;

	/* generate vendor-specific possibly lost packet event */
	if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Vendor_Specific,
	                sizeof(vendor_specific_evt_parms_t) - 1 + DOT11_OUI_LEN + len)) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_tx_hcidata_complete: failed to create "
		          "vendor-specific event\n", bta->wlc->pub.unit));
		return;
	}

	parms = (vendor_specific_evt_parms_t *)evt->parms;
	parms->len = DOT11_OUI_LEN + len;
	bcopy(BRCM_PROP_OUI, &parms->parms[0], DOT11_OUI_LEN);
	bcopy(data, &parms->parms[DOT11_OUI_LEN], len);

	wlc_bta_doevent(bta, evt);

	wlc_bta_free_hcievent(bta, evt);
}

#if defined(BCMDBG) || defined(BCMDBG_ERR)
static void
wlc_bta_tx_hcidata_lost_event(bta_info_t *bta, uint16 llh)
{
	uint8 data[] = {1}; /* possibly lost packet */

	wlc_bta_vendor_specific_event(bta, data, sizeof(data));
}
#endif /* BCMDBG || BCMDBG_ERR */

/* tx complete (success or failure) callback */
void
wlc_bta_tx_hcidata_complete(bta_info_t *bta, void *p, uint txstatus)
{
	uint16 llh;

	/* generate Number of Completed Data Blocks event is the main goal,
	 * generate other events as they fit.
	 */

	ASSERT(WLPKTFLAG_BTA_HCI_ACL(WLPKTTAG(p)));

	llh = WLPKTTAG(p)->shared.bta.llh;

	/* frame is xmit'd and ack'd */
	if (txstatus & TX_STATUS_ACK_RCV)
		goto doevent;

	/* frame isn't xmit'd or ack'd */
	switch (txstatus & TX_STATUS_SUPR_MASK) {
	case TX_STATUS_SUPR_PMQ:
		/* frame is suppressed due to PM mode transition to ON, do nothing */
		WL_BTA(("wl%d: frame is suppressed due to PM transition, do nothing\n",
		        bta->wlc->pub.unit));
		return;

	case TX_STATUS_SUPR_EXPTIME:
		/* frame is suppressed due to lifetime expiration */
		wlc_bta_flush_hcidata_occurred_event(bta, llh);
		break;

	case 0:
		/* frame isn't suppressed */
#if defined(BCMDBG) || defined(BCMDBG_ERR)
		{
		uint tx_frame_count = (txstatus & TX_STATUS_FRM_RTX_MASK) >>
		        TX_STATUS_FRM_RTX_SHIFT;

		if (WLPKTTAG(p)->flags & WLF_USERTS) {
			uint tx_rts_count = (txstatus & TX_STATUS_RTS_RTX_MASK) >>
			        TX_STATUS_RTS_RTX_SHIFT;

			WL_ERROR(("wl%d: wlc_bta_tx_hcidata_complete: "
			          "RTS retries %u frame retries %u, "
			          "frame possibly lost on logical link %u\n",
			          bta->wlc->pub.unit, tx_rts_count, tx_frame_count, llh));
		}
		else {
			WL_ERROR(("wl%d: wlc_bta_tx_hcidata_complete: "
			          "frame retries %u, "
			          "frame possibly lost on logical link %u\n",
			          bta->wlc->pub.unit, tx_frame_count, llh));
		}

		wlc_bta_tx_hcidata_lost_event(bta, llh);
		}
#endif /* BCMDBG || BCMDBG_ERR */
		break;

	default:
		break;
	}

doevent:
	wlc_bta_tx_hcidata_complete_event(bta, llh, FALSE);

	/* clear the flag to avoid wlc_bta_tx_hcidata_lost() gets called */
	WLPKTTAG(p)->flags &= ~WLF_BTA_HCI_ACL_TYPE;
}

/* tx lost callback (packet is freed before going through txstatus processing due to fail
 * to go in to txq or flush or whatever reason...)
 */
void
wlc_bta_tx_hcidata_lost(bta_info_t *bta, void *p)
{
	uint16 llh;

	ASSERT(WLPKTFLAG_BTA_HCI_ACL(WLPKTTAG(p)));

	llh = WLPKTTAG(p)->shared.bta.llh;

#if defined(BCMDBG) || defined(BCMDBG_ERR)
	wlc_bta_tx_hcidata_lost_event(bta, llh);
#endif

	wlc_bta_tx_hcidata_complete_event(bta, llh, FALSE);
}
#endif /* WLBTAMP_LOW */

static amp_hci_event_t *
wlc_bta_alloc_hcievent(bta_info_t *bta, uint8 ecode, uint8 plen)
{
	uint evt_size = HCI_EVT_PREAMBLE_SIZE + plen;
	amp_hci_event_t *evt;

	if ((evt = (amp_hci_event_t *)MALLOC(bta->wlc->pub.osh, evt_size)) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_alloc_hcievent: out of memory, malloced %d bytes\n",
		          bta->wlc->pub.unit, MALLOCED(bta->wlc->pub.osh)));
		return NULL;
	}
	bzero(evt, evt_size);

	evt->ecode = ecode;
	evt->plen = plen;

	return evt;
}

static void
wlc_bta_free_hcievent(bta_info_t *bta, amp_hci_event_t *evt)
{
	uint evt_size;

	evt_size = HCI_EVT_PREAMBLE_SIZE + evt->plen;
	MFREE(bta->wlc->pub.osh, evt, evt_size);
}

/* defer or generate Number of Completed Data Blocks events */
#define BTA_MAXDATABLKS_DLY	(BTA_MAXDATABLKS / 2)

static void
wlc_bta_tx_hcidata_complete_event(bta_info_t *bta, uint16 llh, bool flush)
{
	bool generate = FALSE;
	bool invllh = FALSE;

	if (flush) {
		if (bta->ll[llh].datablks_complete > 0) {
			generate = TRUE;
			goto gen_evt;
		}
		return;
	}

	/* llh may be invalid at this point if it was wrong when passed in or
	 * if the logical link has been disconnected.
	 */

	ASSERT(bta->datablks_pending >= 1);
	bta->datablks_pending --;

	bta->datablks_complete ++;

	if (wlc_bta_valid_logical_link(bta, llh) == HCI_SUCCESS)
		bta->ll[llh].datablks_complete ++;
	else {
		generate = TRUE;
		invllh = TRUE;
	}

	/* defer the event as much as possible to save some resources */
	if (!generate &&
	    (bta->datablks_pending == 0 ||
	     bta->datablks_complete >= BTA_MAXDATABLKS_DLY))
		generate = TRUE;

gen_evt:
	/* generate Number of Completed Data Blocks event */
	if (generate || flush) {
		num_completed_data_blocks_evt_parms_t *parms;
		amp_hci_event_t *evt;
		uint16 llidx;
		uint8 lls;

		/* collapse all individual blocks completes into a single event... */

		for (llidx = 0, lls = 0; llidx < BTA_MAXLOGLINKS; llidx ++) {
			if (bta->ll[llidx].plh != 0 &&
			    bta->ll[llidx].datablks_complete > 0)
				lls ++;
		}
		if (invllh)
			lls ++;

		if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Number_of_Completed_Data_Blocks,
		                sizeof(num_completed_data_blocks_evt_parms_t) +
		                sizeof(data_blocks_t) * (lls - 1))) == NULL) {
			WL_ERROR(("wl%d: wlc_bta_tx_hcidata_complete_event: failed to create "
			          "event\n", bta->wlc->pub.unit));
			return;
		}

		parms = (num_completed_data_blocks_evt_parms_t *)evt->parms;
		htol16_ua_store(BTA_MAXDATABLKS, (uint8 *)&parms->num_blocks);
		parms->num_handles = lls;
		for (llidx = 0, lls = 0; llidx < BTA_MAXLOGLINKS; llidx ++) {
			if (bta->ll[llidx].plh != 0 &&
			    bta->ll[llidx].datablks_complete > 0) {
				htol16_ua_store(llidx, (uint8 *)&parms->completed[lls].handle);
				htol16_ua_store(1, (uint8 *)&parms->completed[lls].pkts);
				htol16_ua_store(bta->ll[llidx].datablks_complete,
				                (uint8 *)&parms->completed[lls].blocks);
				lls ++;
			}
		}
		if (invllh) {
			htol16_ua_store(llh, (uint8 *)&parms->completed[lls].handle);
			htol16_ua_store(1, (uint8 *)&parms->completed[lls].pkts);
			htol16_ua_store(1, (uint8 *)&parms->completed[lls].blocks);
		}

		wlc_bta_doevent(bta, evt);

		wlc_bta_free_hcievent(bta, evt);

		/* reset counters */
		for (llidx = 0; llidx < BTA_MAXLOGLINKS; llidx ++) {
			if (bta->ll[llidx].plh != 0 &&
			    bta->ll[llidx].datablks_complete > 0) {
				ASSERT(bta->datablks_complete >= bta->ll[llidx].datablks_complete);
				bta->datablks_complete -= bta->ll[llidx].datablks_complete;
				bta->ll[llidx].datablks_complete = 0;
			}
		}
		if (invllh) {
			ASSERT(bta->datablks_complete >= 1);
			bta->datablks_complete -= 1;
		}
		ASSERT(bta->datablks_complete == 0);
	}
}

/* Add Ethernet and 802.11 PAL LLC/SNAP headers */
static void
wlc_bta_add_hdrs(bta_pl_t *pl, void *p, uint16 type)
{
	struct ether_header *eh;
	struct dot11_llc_snap_header *lsh;
	uint16 len;
	wlc_bsscfg_t *bsscfg;
	struct scb *scb;

	scb = pl->scb;
	ASSERT(scb != NULL);

	bsscfg = SCB_BSSCFG(scb);
	ASSERT(bsscfg != NULL);

	len = (uint16)PKTLEN(bsscfg->wlc->pub.osh, p);

	PKTPUSH(bsscfg->wlc->pub.osh, p, RFC1042_HDR_LEN);

	eh = (struct ether_header *)PKTDATA(bsscfg->wlc->pub.osh, p);
	bcopy(&scb->ea, eh->ether_dhost, ETHER_ADDR_LEN);
	bcopy(&bsscfg->cur_etheraddr, eh->ether_shost, ETHER_ADDR_LEN);
	eh->ether_type = hton16(DOT11_LLC_SNAP_HDR_LEN + len);

	lsh = (struct dot11_llc_snap_header *)&eh[1];
	bcopy(BT_SIG_SNAP_MPROT, lsh, DOT11_LLC_SNAP_HDR_LEN - 2);
	lsh->type = hton16(type);
}

/* Transmit a HCI ACL data packet encapsulated in BT-SIG frame.
 * Input format:
 *	hci-acl-data header
 *	hci-acl-data
 * Output format:
 *	ether_header
 *	bt-sig-llc-snap header
 *	hci-acl-data header
 *	hci-acl-data
 * Note: The packet will go through the send path during which the hci-acl-data header
 *       will be removed by wlc_bta_send_proc() before transmission.
 */
static void
wlc_bta_send_hcidata_pkt(bta_info_t *bta, uint16 llh, void *p)
{
	wlc_info_t *wlc = bta->wlc;
	bta_ll_t *ll;
	bta_pl_t *pl;

	ll = &bta->ll[llh];
	pl = &bta->pl[ll->plidx];

	/* add Ethernet header with 802.11 PAL LLC/SNAP header */
	wlc_bta_add_hdrs(pl, p, BTA_PROT_L2CAP);

	/* submit to wl tx path */
	wlc_sendpkt(wlc, p, SCB_WDS(pl->scb));
}

#ifdef WLBTAMP_LOW
/* Process the BT-SIG packet being transmitted.
 *   Input format:
 *     ether header
 *     bt-sig-llc-snap header
 *     payload data
 * HCI ACL data payload: Remove HCI ACL data header before going through 802.11 processing.
 *   Input format:
 *     hci-acl-data header
 *     hci-acl-data
 *   Output format:
 *     hci-acl-data
 * Type 0 (undefined, used to encapsulate HCI cmd when DA is locally admin'd address) payload:
 * Route the HCI cmd to ioctl handler.
 *   The packet will not be transmitted.
 * Security data payload:
 *   No processing, transmit as is.
 * Activity report payload:
 *   No processing, transmit as is.
 * Link Supervision Request payload:
 *   No processing, transmit as is.
 * Link Supervision Reply payload:
 *   No processing, transmit as is.
 * Other payloads:
 *   The packet will not be transmitted.
 *
 * Update the wlcif pointer for HCI ACL data and Security packets as well since
 * these two types of frames could come from outside the wlc_bta.c module and the
 * wlcif pointer may not be correct.
 *
 * Make sure the routine is called at the very beginning of the tx processing path.
 *
 * Return TRUE to indicate the frame has been consumed and should not be transmitted
 * hence free the packet upon return from this function.
 */
bool
wlc_bta_send_proc(bta_info_t *bta, void *p, wlc_if_t **wlcif)
{
	wlc_info_t *wlc = bta->wlc;
	struct ether_header *eh;
	struct dot11_llc_snap_header *lsh;
	bta_pl_t *pl;
	bool toss = FALSE;
	uint16 type;
	struct scb *scb;
	scb_bta_t *scb_bta;

	ASSERT(PKTLEN(wlc->pub.osh, p) >= RFC1042_HDR_LEN);

	eh = (struct ether_header *)PKTDATA(wlc->pub.osh, p);
	lsh = (struct dot11_llc_snap_header *)&eh[1];

	type = ntoh16(lsh->type);

	/* HCI cmd */
	if (type == 0 && ETHER_IS_LOCALADDR(eh->ether_dhost)) {

		wlc_bta_docmd(bta, (uint8 *)&lsh[1], PKTLEN(wlc->pub.osh, p) - RFC1042_HDR_LEN);

		return TRUE;
	}

	switch (type) {
	case BTA_PROT_L2CAP: {

		amp_hci_ACL_data_t *ACL_data = (amp_hci_ACL_data_t *)&lsh[1];
		uint16 handle = ltoh16(ACL_data->handle);
		uint16 llh = HCI_ACL_DATA_HANDLE(handle);
		bta_ll_t *ll;

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON())
			wlc_bta_hcidump_ACL_data(bta, ACL_data, TRUE);
#endif

		/* mark the packet early so that no matter what happens later the packet
		 * type is known.
		 */
		WLPKTTAG(p)->flags |= WLF_BTA_HCI_ACL_TYPE;
		WLPKTTAG(p)->shared.bta.llh = llh;

		/* optimize the data completion event generation */
		bta->datablks_pending ++;

		/* get logical link */
		if (wlc_bta_valid_logical_link(bta, llh) != HCI_SUCCESS) {
			WL_INFORM(("wl%d: dropping packet sent on invalid logical link %u\n",
			           wlc->pub.unit, llh));
			toss = TRUE;
			break;
		}
		ll = &bta->ll[llh];

		/* drop packet if txflow spec has service type = no_traffic */
		if (ll->prio == PRIO_8021D_NONE) {
			WL_INFORM(("wl%d: dropping packet sent on no traffic logical link %u\n",
			           wlc->pub.unit, llh));
			toss = TRUE;
			break;
		}

		/* remove HCL ACL data header and fix up Ethernet headers */
		PKTPULL(wlc->pub.osh, p, RFC1042_HDR_LEN + HCI_ACL_DATA_PREAMBLE_SIZE);
		pl = &bta->pl[ll->plidx];
		wlc_bta_add_hdrs(pl, p, BTA_PROT_L2CAP);

		/* RTS/CTS protection */
		if (pl->flags & BTA_PL_USE_RTS)
			WLPKTTAG(p)->flags |= WLF_USERTS;

		/* enable short range mode */
		if (pl->short_range != 0)
			WLPKTTAG(p)->flags |= WLF_BTA_SRM;

		/* mark packet priority */
		PKTSETPRIO(p, ll->prio);

		/* set packet lifetime */
		if (ll->fto != BTA_INVFTO)
			wlc_lifetime_set(wlc, p, ll->fto);

		/* route the frame to the wlcif representing the phy link */
		*wlcif = SCB_WDS(pl->scb);

		/* continue wl tx processing... */
		break;
	}

	case BTA_PROT_ACTIVITY_REPORT:

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			scb = wlc_scbfindband(wlc, (struct ether_addr *)eh->ether_dhost,
				CHSPEC_WLCBANDUNIT(wlc->pub.current_bss.chanspec));
			scb_bta = SCB_BTA(bta, scb);
			WL_BTA(("wl%d: sending Activity Report on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, p), PKTLEN(wlc->pub.osh, p));
			WL_BTA(("\n"));
		}
#endif

		/* continue wl tx processing... */
		break;

	case BTA_PROT_SECURITY:

		/* find the scb from DA */
		scb = wlc_scbfindband(wlc, (struct ether_addr *)eh->ether_dhost,
			CHSPEC_WLCBANDUNIT(wlc->pub.current_bss.chanspec));
		if (scb == NULL) {
#if defined(BCMDBG)
			char eabuf[ETHER_ADDR_STR_LEN];
			WL_ERROR(("wl%d: wlc_bta_send_proc: no scb for %s\n", wlc->pub.unit,
			          bcm_ether_ntoa((struct ether_addr *)eh->ether_dhost, eabuf)));
#endif
			toss = TRUE;
			break;
		}

		scb_bta = SCB_BTA(bta, scb);
		pl = &bta->pl[scb_bta->plidx];

		/* RTS/CTS protection */
		if (pl->flags & BTA_PL_USE_RTS)
			WLPKTTAG(p)->flags |= WLF_USERTS;

		/* mark packet as 802.1x (since ether_type will be hard to detect) */
		WLPKTTAG(p)->flags |= WLF_8021X;

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			WL_BTA(("wl%d: sending BT-SIG 802.1x frame on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, p), PKTLEN(wlc->pub.osh, p));
			WL_BTA(("\n"));
		}
#endif

		/* route the frame to the wlcif representing the phy link */
		*wlcif = SCB_WDS(pl->scb);

		/* continue wl tx processing... */
		break;

	case BTA_PROT_LINK_SUPERVISION_REQUEST:

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			scb = wlc_scbfindband(wlc, (struct ether_addr *)eh->ether_dhost,
				CHSPEC_WLCBANDUNIT(wlc->pub.current_bss.chanspec));
			scb_bta = SCB_BTA(bta, scb);
			WL_BTA(("wl%d: sending Link Supervision Request on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, p), PKTLEN(wlc->pub.osh, p));
			WL_BTA(("\n"));
		}
#endif

		/* continue wl tx processing... */
		break;

	case BTA_PROT_LINK_SUPERVISION_REPLY:

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			scb = wlc_scbfindband(wlc, (struct ether_addr *)eh->ether_dhost,
				CHSPEC_WLCBANDUNIT(wlc->pub.current_bss.chanspec));
			scb_bta = SCB_BTA(bta, scb);
			WL_BTA(("wl%d: sending Link Supervision Reply on physical link 0x%x\n",
			        wlc->pub.unit, bta->plh[scb_bta->plidx]));
			prhex(NULL, PKTDATA(wlc->pub.osh, p), PKTLEN(wlc->pub.osh, p));
			WL_BTA(("\n"));
		}
#endif

		/* continue wl tx processing... */
		break;

	default:

#if defined(BCMDBG) || defined(WLMSG_BTA)
		if (WL_BTA_ON()) {
			WL_BTA(("wl%d: wlc_bta_send_proc: unrecognized protocol: 0x%04x\n",
			        wlc->pub.unit, type));
			prhex(NULL, PKTDATA(wlc->pub.osh, p), PKTLEN(wlc->pub.osh, p));
			WL_BTA(("\n"));
		}
#endif

		toss = TRUE;
		break;
	}

	/* toss! */
	return toss;
}

void
wlc_bta_tx_hcidata(bta_info_t *bta, uint8 *data_buf, uint data_len)
{
	amp_hci_ACL_data_t *ACL_data = (amp_hci_ACL_data_t *)data_buf;
	uint16 handle = ltoh16(ACL_data->handle);
	uint16 dlen = ltoh16(ACL_data->dlen);
	wlc_info_t *wlc = bta->wlc;
	uint16 llh;
	void *pkt;
	void *buf;

	/* get logical link */
	llh = HCI_ACL_DATA_HANDLE(handle);
	if (wlc_bta_valid_logical_link(bta, llh) != HCI_SUCCESS) {
		WL_BTA(("wl%d: dropping packet sent on invalid logical link %u\n",
		        wlc->pub.unit, llh));
		return;
	}

	/* get an OSL packet w/ appropriate headroom */
	if (!(pkt = PKTGET(wlc->pub.osh,
		TXOFF + RFC1042_HDR_LEN + HCI_ACL_DATA_PREAMBLE_SIZE + dlen, TRUE))) {
		WL_ERROR(("wl%d: wlc_bta_tx_hcidata: pktget error for len %d\n",
		          wlc->pub.unit, dlen));
		return;
	}

	/* copy HCI ACL data header and ACL data at this offset */
	buf = PKTPULL(wlc->pub.osh, pkt, TXOFF + RFC1042_HDR_LEN);

	/* copy in HCI ACL data header and ACL data */
	bcopy((uint8 *)ACL_data, buf, HCI_ACL_DATA_PREAMBLE_SIZE + dlen);

	/* send packet */
	wlc_bta_send_hcidata_pkt(bta, llh, pkt);
}
#endif /* WLBTAMP_LOW */

/* AMP HCI event processing */
#if defined(BCMDBG) || defined(WLMSG_BTA)
static const struct {
	uint8 evtval;
	char *evtstr;
} evt_map[] = {
	{ HCI_Command_Complete, "Command Complete" },
	{ HCI_Command_Status, "Command Status" },
	{ HCI_Flush_Occurred, "Flush Occurred" },
	{ HCI_Enhanced_Flush_Complete, "Enhanced Flush Complete" },
	{ HCI_Physical_Link_Complete, "Physical Link Complete" },
	{ HCI_Channel_Select, "Channel Select" },
	{ HCI_Disconnect_Physical_Link_Complete, "Disconnect Physical Link Complete" },
	{ HCI_Logical_Link_Complete, "Logical Link Complete" },
	{ HCI_Disconnect_Logical_Link_Complete, "Disconnect Logical Link Complete" },
	{ HCI_Number_of_Completed_Data_Blocks, "Number of Completed Data Blocks" },
	{ HCI_Short_Range_Mode_Change_Complete, "Short Range Mode Change Complete" },
	{ HCI_Vendor_Specific, "Vendor Specific" }
};

static char *
evt2str(uint8 evt, char *buf)
{
	uint i;

	sprintf(buf, "Unknown");
	for (i = 0; i < ARRAYSIZE(evt_map); i++) {
		if (evt == evt_map[i].evtval) {
			sprintf(buf, evt_map[i].evtstr);
		}
	}

	return buf;
}

static void
wlc_bta_hcidump_evt(bta_info_t *bta, amp_hci_event_t *event)
{
	wlc_info_t *wlc = bta->wlc;
	char buf[34];

	WL_BTA(("wl%d: > HCI Event: %s(0x%x) plen %d\n", wlc->pub.unit,
		evt2str(event->ecode, buf), event->ecode, event->plen));
	prhex(NULL, event->parms, event->plen);
	WL_BTA(("\n"));
}
#endif /* BCMDBG || WLMSG_BTA */

void
wlc_bta_doevent(bta_info_t *bta, amp_hci_event_t *event)
{
	uint event_size = HCI_EVT_PREAMBLE_SIZE + event->plen;

#if defined(BCMDBG) || defined(WLMSG_BTA)
	if (WL_BTA_ON())
		wlc_bta_hcidump_evt(bta, event);
#endif

	if (!(bta->flags & BTA_FLAGS_ET_RX)) {
#ifdef WLBTAMP_HIGH_ONLY
		/*
		 * Handle it locally in DHD
		 */
		dhd_bta_doevt((struct dhd_pub *)bta, event, event_size);
		/*
		 * The event->plen could be adjusted in the dhd_bta_doevt().
		 */
		event_size = HCI_EVT_PREAMBLE_SIZE + event->plen;
#endif	/* WLBTAMP_HIGH_ONLY */
#ifdef WLBTWUSB
		/* Forward data to BTKRNL */
		WL_BTA(("wl%d: forwarding HCI event to BTKRNL\n", bta->wlc->pub.unit));
		BtKernForwardEvent(bta->wlc->wl, (PVOID)event, (ULONG)event_size);
#endif
	}
	else {
#ifdef WLBTAMP_LOW
		/* Forward event to upper stack */
		wlc_mac_event(bta->wlc, WLC_E_BTA_HCI_EVENT, NULL, 0, 0, 0, event, event_size);
#endif
	}
}

static void
wlc_bta_cmd_status_event(bta_info_t *bta, uint16 op, uint8 status)
{
	amp_hci_event_t *evt;
	cmd_status_parms_t *parms;

	/* generate Command Status event */
	if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Command_Status,
	                sizeof(cmd_status_parms_t))) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_cmd_status_event: failed to create event\n",
		          bta->wlc->pub.unit));
		return;
	}

	parms = (cmd_status_parms_t *)evt->parms;
	parms->status = status;
	parms->cmdpkts = 1;
	htol16_ua_store(op, (uint8 *)&parms->opcode);

	wlc_bta_doevent(bta, evt);

	wlc_bta_free_hcievent(bta, evt);
}

static void
wlc_bta_phy_link_complete_event(bta_info_t *bta, uint8 *buf, uint buflen, bool disconnect)
{
	amp_hci_event_t *evt;

	/* generate Disconnect Physical Link Complete/Physical Link Complete event */
	if ((evt = wlc_bta_alloc_hcievent(bta, disconnect ?
	                        HCI_Disconnect_Physical_Link_Complete :
	                        HCI_Physical_Link_Complete,
	                (uint8)buflen)) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_phy_link_complete_event: failed to create event\n",
		          bta->wlc->pub.unit));
		return;
	}

	bcopy(buf, evt->parms, buflen);

	wlc_bta_doevent(bta, evt);

	wlc_bta_free_hcievent(bta, evt);
}

static void
wlc_bta_cmd_complete_event(bta_info_t *bta, uint16 op, uint8 *buf, uint buflen)
{
	amp_hci_event_t *evt;
	cmd_complete_parms_t *parms;

	/* generate Command Complete event */
	if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Command_Complete,
	                (uint8)(OFFSETOF(cmd_complete_parms_t, parms) + buflen))) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_cmd_complete_event: failed to create event\n",
		          bta->wlc->pub.unit));
		return;
	}

	parms = (cmd_complete_parms_t *)evt->parms;
	parms->cmdpkts = 1;
	htol16_ua_store(op, (uint8 *)&parms->opcode);
	bcopy(buf, parms->parms, buflen);

	wlc_bta_doevent(bta, evt);

	wlc_bta_free_hcievent(bta, evt);
}

/* AMP HCI command processing */
#if defined(BCMDBG) || defined(WLMSG_BTA)
static const struct {
	uint16 opval;
	char *opstr;
} op_map[] = {
	{ HCI_Read_Logical_Link_Accept_Timeout, "Read Logical Link Accept Timeout" },
	{ HCI_Write_Logical_Link_Accept_Timeout, "Write Logical Link Accept Timeout" },
	{ HCI_Read_Buffer_Size, "Read Buffer Size" },
	{ HCI_Read_Data_Block_Size, "Read Data Block Size" },
	{ HCI_Reset, "Reset" },
	{ HCI_Enhanced_Flush, "Enhanced Flush" },
	{ HCI_Read_Best_Effort_Flush_Timeout, "Read Best Effort Flush Timeout" },
	{ HCI_Write_Best_Effort_Flush_Timeout, "Write Best Effort Flush Timeout" },
	{ HCI_Read_Connection_Accept_Timeout, "Read Connection Accept Timeout" },
	{ HCI_Write_Connection_Accept_Timeout, "Write Connection Accept Timeout" },
	{ HCI_Read_Link_Supervision_Timeout, "Read Link Supervision Timeout" },
	{ HCI_Write_Link_Supervision_Timeout, "Write Link Supervision Timeout" },
	{ HCI_Read_Link_Quality, "Read Link Quality" },
	{ HCI_Read_Local_AMP_Info, "Read Local AMP Info" },
	{ HCI_Read_Local_AMP_ASSOC, "Read Local AMP ASSOC" },
	{ HCI_Write_Remote_AMP_ASSOC, "Write Remote AMP ASSOC" },
	{ HCI_Create_Physical_Link, "Create Physical Link" },
	{ HCI_Accept_Physical_Link_Request, "Accept Physical Link Request" },
	{ HCI_Disconnect_Physical_Link, "Disconnect Physical Link" },
	{ HCI_Create_Logical_Link, "Create Logical Link" },
	{ HCI_Accept_Logical_Link, "Accept Logical Link" },
	{ HCI_Disconnect_Logical_Link, "Disconnect Logical Link" },
	{ HCI_Logical_Link_Cancel, "Logical Link Cancel" },
	{ HCI_Short_Range_Mode, "Short Range Mode" }
};

static char *
op2str(uint16 op, char *buf)
{
	uint i;

	sprintf(buf, "Unknown");
	for (i = 0; i < ARRAYSIZE(op_map); i++) {
		if (op == op_map[i].opval) {
			sprintf(buf, op_map[i].opstr);
		}
	}

	return buf;
}

static void
wlc_bta_hcidump_cmd(bta_info_t *bta, amp_hci_cmd_t *cmd)
{
	wlc_info_t *wlc = bta->wlc;
	uint16 op = ltoh16_ua((uint8 *)&cmd->opcode);
	char buf[40];

	WL_BTA(("wl%d: < HCI Command: %s(0x%x|0x%x) plen %d\n", wlc->pub.unit,
		op2str(op, buf), HCI_CMD_OGF(op), HCI_CMD_OCF(op),
		cmd->plen));
	prhex(NULL, cmd->parms, cmd->plen);
	WL_BTA(("\n"));
}
#endif /* BCMDBG || WLMSG_BTA */

#ifdef WLBTAMP_HIGH
static uint8
wlc_bta_connect_phy_link(bta_info_t *bta, phy_link_cmd_parms_t *parms, uint8 plen, bool creator)
{
	bta_pl_t *phy_link;
	int plidx;

	/* sanity check: all parameters present */
	if (plen < (OFFSETOF(phy_link_cmd_parms_t, key) + parms->key_length))
		return HCI_ERR_UNIT_KEY_USED;

	/* sanity check: key not too long */
	if (parms->key_length > 32)
		return HCI_ERR_UNSUPPORTED_VALUE;

	/* sanity check: physical link handle cannot be zero */
	if (parms->plh == 0)
		return HCI_ERR_UNSUPPORTED_VALUE;

	/* sanity check: physical link already exists */
	for (plidx = 0; plidx < WLC_MAXBTA; plidx++) {
		if (bta->plh[plidx] == parms->plh)
			break;
	}
	if (plidx < WLC_MAXBTA)
		return HCI_ERR_CONNECTION_EXISTS;

	/* allocate physical link */
	for (plidx = 0; plidx < WLC_MAXBTA; plidx++) {
		if (bta->plh[plidx] == 0)
			break;
	}
	if (plidx == WLC_MAXBTA)
		return HCI_ERR_MAX_NUM_OF_CONNECTIONS;

	/* update physical link handle */
	bta->plh[plidx] = parms->plh;

	phy_link = &bta->pl[plidx];

	bzero(phy_link, sizeof(bta_pl_t));

	/* set role */
	if (creator)
		phy_link->flags |= BTA_PL_CREATOR;

	/* set link key */
	phy_link->lk_type_len |= (((parms->key_type & BTA_PLK_TYPE_MASK) - 3) <<
		BTA_PLK_TYPE_SHIFT);
	phy_link->lk_type_len |= (parms->key_length & BTA_PLK_LENGTH_MASK);
	bcopy(parms->key, phy_link->link_key, parms->key_length);

	/* timestamp connection attempt */
	wlc_bta_set_ca_ts(bta, plidx);

	/* set default link supervision timeout */
	phy_link->ls_to = 0x3e80;

	return HCI_SUCCESS;
}
#endif /* WLBTAMP_HIGH */

/*
 * Traverse a string of 1-byte tag/2-byte length/variable-length value
 * triples, returning a pointer to the substring whose first element
 * matches tag
 */
static uint8 *
wlc_bta_parse_tlvs(void *buf, int buflen, uint key)
{
	uint8 *elt;
	int totlen;

	elt = (uint8 *)buf;
	totlen = buflen;

	/* find tagged parameter */
	while (totlen >= 3) {
		int len = ltoh16_ua(&elt[1]);

		/* validate remaining totlen */
		if ((elt[0] == key) && (totlen >= (len + 3)))
			return (elt);

		elt = &elt[len + 3];
		totlen -= (len + 3);
	}

	return NULL;
}

#ifdef WLBTAMP_LOW
static void
wlc_bta_activate(bta_info_t *bta)
{
	wlc_info_t *wlc = bta->wlc;
#if WLBTAMP_D11RETRY > 7
	int SRL, LRL;
#endif
	int infra;
	int PM;

#if WLBTAMP_D11RETRY > 7
	bta->SRL = (uint8)wlc->SRL;
	SRL = WLBTAMP_D11RETRY;
	wlc_ioctl(wlc, WLC_SET_SRL, &SRL, sizeof(SRL), NULL);
	bta->LRL = (uint8)wlc->LRL;
	LRL = WLBTAMP_D11RETRY / 2;
	wlc_ioctl(wlc, WLC_SET_LRL, &LRL, sizeof(LRL), NULL);
#endif

	bta->infra = wlc->default_bss.infra;
	infra = 1;
	wlc_ioctl(wlc, WLC_SET_INFRA, &infra, sizeof(infra), NULL);

	bta->PM = wlc->PM;
	PM = PM_OFF;
	wlc_ioctl(wlc, WLC_SET_PM, &PM, sizeof(PM), NULL);

	/* save global states */
	bta->_apsta = wlc->pub._apsta;
	wlc->pub._apsta = TRUE;
	bta->_ap = wlc->pub._ap;
	wlc->pub._ap = TRUE;
}

static void
wlc_bta_deactivate(bta_info_t *bta)
{
	wlc_info_t *wlc = bta->wlc;
#if WLBTAMP_D11RETRY > 7
	int SRL, LRL;
#endif
	int infra;
	int PM;

	/* restore global states */
	wlc->pub._ap = bta->_ap;
	wlc->pub._apsta = bta->_apsta;

	PM = bta->PM;
	wlc_ioctl(wlc, WLC_SET_PM, &PM, sizeof(PM), NULL);

	infra = bta->infra;
	wlc_ioctl(wlc, WLC_SET_INFRA, &infra, sizeof(infra), NULL);

#if WLBTAMP_D11RETRY > 7
	SRL = bta->SRL;
	wlc_ioctl(wlc, WLC_SET_SRL, &SRL, sizeof(SRL), NULL);
	LRL = bta->LRL;
	wlc_ioctl(wlc, WLC_SET_LRL, &LRL, sizeof(LRL), NULL);
#endif
}

static void
wlc_bta_enable_btcx(bta_info_t *bta, bta_pl_t *pl, bool enable)
{
	struct scb *scb = pl->scb;

	/* STA: PS; AP: CTS */
	wlc_enable_btc_ps_protection(bta->wlc, SCB_BSSCFG(scb), enable);
}

static int
wlc_bta_dodisconnect(bta_info_t *bta, int plidx)
{
	wlc_info_t *wlc = bta->wlc;
	bta_pl_t *phy_link;
	struct scb *scb;
	wlc_bsscfg_t *bsscfg;
	int callbacks = 0;

	/* clear physical link handle */
	bta->plh[plidx] = 0;

	/* tear down the supporting state structs */
	phy_link = &bta->pl[plidx];

	if ((scb = phy_link->scb) != NULL) {

		bsscfg = SCB_BSSCFG(scb);
		ASSERT(bsscfg != NULL);

		wlc_bta_enable_btcx(bta, phy_link, FALSE);

		/* force to free the scb */
		scb->permanent = FALSE;
		/* free the scb for creator, the container bsscfg will be freed only
		 * when there is no other physical links existing.
		 */
		if (phy_link->flags & BTA_PL_CREATOR) {
			struct maclist *maclist;
			uint len;
			uint i;

			len = sizeof(*maclist) + (MAXMACLIST - 1) * sizeof(struct ether_addr);
			maclist = MALLOC(wlc->pub.osh, len);
			if (maclist != NULL) {
				wlc_ioctl(wlc, WLC_GET_MACLIST, maclist, len, &bsscfg->wlcif);
				for (i = 0; i < maclist->count; i ++) {
					if (bcmp(&scb->ea, &maclist->ea[i], ETHER_ADDR_LEN) != 0)
						continue;
					bcopy(&maclist->ea[i + 1], &maclist->ea[i],
					      (maclist->count - i - 1) * ETHER_ADDR_LEN);
					maclist->count -= 1;
					break;
				}
				wlc_ioctl(wlc, WLC_SET_MACLIST, maclist, len, &bsscfg->wlcif);
				MFREE(wlc->pub.osh, maclist, len);
			}
			else {
				WL_ERROR(("wl%d: wlc_bta_dodisconnect: "
				          "failed to allocate maclist\n", wlc->pub.unit));
			}
			wlc_scbfree(wlc, scb);
		}
		/* free the bsscfg for acceptor as well */
		else {
#ifdef USE_PRIMARY_BSSCFG
			wlc_assoc_abort(wlc);
			if (bsscfg->enable)
				wlc_bsscfg_disable(wlc, bsscfg);
			bsscfg->flags &= ~PRIMARY_BSSCFG_FLAGS;
			wlc_bsscfg_reinit_ext(wlc, bsscfg, FALSE);
			/* restore join/assoc retry */
			wlc->assoc_retry_max = bta->assoc_retry_max;
			/* restore security settings */
			bsscfg->WPA_auth = bta->WPA_auth;
			bsscfg->wsec = bta->wsec;
			bsscfg->wsec_restrict = bta->wsec_restrict;
			bsscfg->eap_restrict = bta->eap_restrict;
			if (!(bta->flags & BTA_FLAGS_NO_SECURITY)) {
				wlc->sup_enable_wpa = bta->sup_enable_wpa;
			}
#else /* USE_PRIMARY_BSSCFG */
			if (bsscfg->enable)
				wlc_bsscfg_disable(wlc, bsscfg);
			wlc_bsscfg_free(wlc, bsscfg);
#endif /* !USE_PRIMARY_BSSCFG */
		}

		bta->numpls --;
	}

	/* free local and remote AMP_ASSOCs */
	if (phy_link->local)
		MFREE(wlc->pub.osh, phy_link->local, phy_link->llen);
	if (phy_link->remote)
		MFREE(wlc->pub.osh, phy_link->remote, phy_link->rlen);

	/* clear the rest of the physical link */
	bzero(phy_link, sizeof(bta_pl_t));

	/* Other clean ups when there are no physical links in the system */

	if (bta->numpls > 0)
		return 0;

	/* tear down creators' container/beaconing bsscfg */
	if ((bsscfg = bta->bsscfg) != NULL) {
		if (bsscfg->enable)
			wlc_bsscfg_disable(wlc, bsscfg);
		wlc_bsscfg_free(wlc, bsscfg);
		bta->bsscfg = NULL;
	}

	/* delete any Channel Select event delay timer */
	if (bta->cse_timer != NULL) {
		if (!wl_del_timer(wlc->wl, bta->cse_timer))
			callbacks ++;
		else {
			wl_free_timer(wlc->wl, bta->cse_timer);
			bta->cse_timer = NULL;
		}
	}

	/* restore global states */
	wlc_bta_deactivate(bta);

	bta->chanspec_sel = 0;

	return callbacks;
}
#endif /* WLBTAMP_LOW */

void
wlc_bta_scb_cleanup(bta_info_t *bta, struct scb *scb)
{
	WL_BTA(("wl%d: wlc_bta_scb_cleanup:\n", bta->wlc->pub.unit));

}

#ifdef WLBTAMP_HIGH
uint8
wlc_bta_pllookup(bta_info_t *bta, uint8 plh, int *plidx)
{
	int index;

	for (index = 0; index < WLC_MAXBTA; index++) {
		if (bta->plh[index] == plh) {
			*plidx = index;
			return HCI_SUCCESS;
		}
	}

	return HCI_ERR_NO_CONNECTION;
}
#endif /* WLBTAMP_HIGH */

bool
wlc_bta_qos(bta_info_t *bta)
{
	return ((bta->flags & BTA_FLAGS_NO_QOS) ? FALSE : TRUE);
}

bool
wlc_bta_active(bta_info_t *bta)
{
	return bta->numpls > 0;
}

#ifdef WLBTAMP_LOW
static void
wlc_bta_phy_link_complete(bta_info_t *bta, struct scb *scb, uint8 status)
{
	phy_link_evt_parms_t evt_parms;
	scb_bta_t *scb_bta;
	bta_pl_t *pl;

	scb_bta = SCB_BTA(bta, scb);
	pl = &bta->pl[scb_bta->plidx];

	/* we are no longer in connecting process */
	pl->flags &= ~BTA_PL_CONN;

	/* generate Physical Link Complete event */
	evt_parms.status = status;
	evt_parms.plh = scb_bta->plh;
	wlc_bta_phy_link_complete_event(bta, (uint8 *)&evt_parms, sizeof(evt_parms), FALSE);

	/* mark physical link as having been completed (regardless 'status') */
	pl->flags |= BTA_PL_COMPLETE;

	if (status == HCI_SUCCESS) {
		/* enable RTS/CTS protection */
		pl->flags |= BTA_PL_USE_RTS;

		/* initialize link supervision timer */
		pl->used = bta->wlc->pub.now;

		/* turn on BT coex protection */
		wlc_bta_enable_btcx(bta, pl, TRUE);

		wlc_bta_send_activity_report_pkt(bta, pl);
	}
}

static uint8
wlc_bta_join(wlc_info_t *wlc, wlc_bsscfg_t *bsscfg,
	uint8 *SSID, int SSID_len, uint8 *bssid, chanspec_t chanspec)
{
	wl_assoc_params_t assoc_params;

	bcopy(bssid, (void *)&assoc_params.bssid, ETHER_ADDR_LEN);
	assoc_params.chanspec_list[0] = chanspec;
	assoc_params.chanspec_num = 1;

	wlc_join(wlc, bsscfg, SSID, SSID_len, &assoc_params, sizeof(assoc_params));

	return 0;
}

void
wlc_bta_join_complete(bta_info_t *bta, struct scb *scb, uint8 status)
{
	wlc_info_t *wlc = bta->wlc;
	wlc_bsscfg_t *bsscfg;

	ASSERT(scb != NULL);

	bsscfg = SCB_BSSCFG(scb);
	ASSERT(bsscfg != NULL);

	if (status == 0) {
#ifdef WDS
		/* configure BT-AMP peer for WDS */
		wlc_wds_create(wlc, scb, WDS_INFRA_BSS);
#endif /* WDS */
	}

	if (status != 0 || (bta->flags & BTA_FLAGS_NO_SECURITY)) {
		if (status == 0)
			status = HCI_SUCCESS;
		else {
			WL_ERROR(("wl%d: wlc_bta_join_complete: failed with error %d\n",
			          wlc->pub.unit, status));
			status = HCI_ERR_UNSPECIFIED;
		}
		wlc_bta_phy_link_complete(bta, scb, status);
	} else {
		scb_bta_t *scb_bta;
		int plidx;

		scb_bta = SCB_BTA(bta, scb);
		ASSERT(scb_bta != NULL);

		plidx = scb_bta->plidx;

		if (!(bta->pl[plidx].flags & BTA_PL_CREATOR)) {
#if defined(BCMSUP_PSK)
			uint auth_ies_len = BCN_TMPL_LEN;
			bool stat = FALSE;

			/* mutually derive keys */
			if (bsscfg->sup != NULL) {
				uint8 *auth_ies;
				uint8 *sup_ies;
				uint sup_ies_len;

				/* prep supplicant */
				wlc_find_sup_auth_ies(bsscfg->sup, &sup_ies, &sup_ies_len,
				                      &auth_ies, &auth_ies_len);
				stat = wlc_set_sup(bsscfg->sup, SUP_WPAPSK,
				                   sup_ies, sup_ies_len, auth_ies, auth_ies_len);
			}
			if (!stat) {
				WL_ERROR(("wl%d: wlc_bta_join_complete: 4-way handshake "
				          "config problem\n", wlc->pub.unit));
			}
#endif /* BCMSUP_PSK */
		}
	}
}

void
wlc_bta_chan_notify(bta_info_t *bta, chanspec_t chanspec)
{
	WL_BTA(("wl%d: chanspec changed from 0x%04x to 0x%04x\n", bta->wlc->pub.unit,
	        bta->chanspec_sel, chanspec));

}
#endif /* WLBTAMP_LOW */

chanspec_t
wlc_bta_chan_select(bta_info_t *bta)
{

	/* use the same channel as existing creators and acceptors */
	if (wlc_bta_active(bta))
		return bta->chanspec_sel;
	/* use the same channel as existing connections */
	else if (bta->wlc->pub.associated)
		return bta->wlc->pub.current_bss.chanspec;
	/* use user specified channel */
	else if (bta->chanspec_user != 0)
		return bta->chanspec_user;

	return 0;
}

#ifdef WLBTAMP_LOW
static void
wlc_bta_send_activity_report_pkt(bta_info_t *bta, bta_pl_t *phy_link)
{
	wlc_info_t *wlc = bta->wlc;
	void *pkt;
	uint len;
	amp_hci_activity_report_t *activity_report;

	/* compute size of activity report triples */
	len = OFFSETOF(amp_hci_activity_report_t, data);
	if (bta->flags & BTA_FLAGS_QTP_AR)
		len += sizeof(amp_hci_activity_report_triple_t);

	/* get an OSL packet w/ appropriate headroom */
	if (!(pkt = PKTGET(wlc->pub.osh, TXOFF + RFC1042_HDR_LEN + len, TRUE))) {
		WL_ERROR(("wl%d: wlc_bta_send_activity_report_pkt: pktget error for len %d\n",
		          wlc->pub.unit, len));
		return;
	}

	/* add payload at this location */
	PKTPULL(wlc->pub.osh, pkt, TXOFF + RFC1042_HDR_LEN);

	/* fill in header and activity reports */
	activity_report = (amp_hci_activity_report_t *)PKTDATA(wlc->pub.osh, pkt);
	activity_report->ScheduleKnown = HCI_AR_SCHEDULE_KNOWN;
	if (bta->flags & BTA_FLAGS_QTP_AR) {
		amp_hci_activity_report_triple_t *activity_report_triple;
		uint32 tsf_l, tsf_h;

		activity_report->NumReports = 1;

		activity_report_triple = (amp_hci_activity_report_triple_t *)activity_report->data;
		activity_report_triple->Duration = 20000;
		activity_report_triple->Periodicity = 50000;
		wlc_read_tsf(wlc, &tsf_l, &tsf_h);
		activity_report_triple->StartTime = tsf_l;
	} else {
		activity_report->NumReports = 0;
	}

	/* add Ethernet header with 802.11 PAL LLC/SNAP header */
	wlc_bta_add_hdrs(phy_link, pkt, BTA_PROT_ACTIVITY_REPORT);

	/* RTS/CTS protection */
	if (phy_link->flags & BTA_PL_USE_RTS)
		WLPKTTAG(pkt)->flags |= WLF_USERTS;

	/* submit to wl tx path */
	wlc_sendpkt(wlc, pkt, SCB_WDS(phy_link->scb));
}

void
wlc_bta_AKM_complete(bta_info_t *bta, struct scb *scb)
{
	ASSERT(scb != NULL);

	/* generate Physical Link Complete event */
	wlc_bta_phy_link_complete(bta, scb, HCI_SUCCESS);
}
#endif /* WLBTAMP_LOW */

static void
wlc_bta_disconnect_logical_link(bta_info_t *bta, uint16 llh)
{
	bta_ll_t *ll;

	/* generate Number of Completed Data Blocks event if necessary */
	wlc_bta_tx_hcidata_complete_event(bta, llh, TRUE);


	ll = &bta->ll[llh];

	/* not much to do here... */
	bzero(ll, sizeof(bta_ll_t));

}

static uint8
wlc_bta_valid_logical_link(bta_info_t *bta, uint16 llh)
{
	/* sanity check logical link handle */
	if (llh >= BTA_MAXLOGLINKS)
		return HCI_ERR_NO_CONNECTION;

	/* make sure logical link has been allocated */
	if (bta->ll[llh].plh == 0)
		return HCI_ERR_NO_CONNECTION;

	return HCI_SUCCESS;
}

#ifdef WLBTAMP_LOW
static void
wlc_bta_cse_timer(void *arg)
{
	bta_info_t *bta = (bta_info_t *)arg;
	int plidx;

	for (plidx = 0; plidx < WLC_MAXBTA; plidx ++) {
		if (bta->plh[plidx] == 0)
			continue;
		if (bta->pl[plidx].scb == NULL)
			continue;
		if (bta->pl[plidx].flags & BTA_PL_CSE_PEND) {
#ifdef WLBTAMP_LOW_ONLY
			chanspec_select_evt_t chsp_evt;
			chanspec_t chanspec;
#else
			amp_hci_event_t evt;
#endif

			bta->pl[plidx].flags &= ~BTA_PL_CSE_PEND;

#ifdef WLBTAMP_LOW_ONLY
			chsp_evt.evt.ecode = HCI_Channel_Select;
			chsp_evt.evt.plen = 1;
			chsp_evt.evt.parms[0] = bta->plh[plidx];
			/*
			 * For Split BTAMP, we need to deliver additional info
			 *    to the HIGH driver to sync up the state.
			 * The final HCI event length is adjusted back in DHD
			 *    after consuming the info.
			 * Piggybacking the plidx, selected chanspec, and numpls ....
			 */
			chsp_evt.evt.plen += sizeof(chsp_evt) - sizeof(amp_hci_event_t);
			chsp_evt.plidx = plidx;
			chsp_evt.numpls = bta->numpls;
			chanspec = wlc_bta_chan_select(bta);
			htol16_ua_store(chanspec, (uint8 *)&chsp_evt.chanspec);
			wlc_bta_doevent(bta, (amp_hci_event_t *)&chsp_evt);
#else
			evt.ecode = HCI_Channel_Select;
			evt.plen = 1;
			evt.parms[0] = bta->plh[plidx];
			wlc_bta_doevent(bta, &evt);
#endif
		}
	}

	wl_free_timer(bta->wlc->wl, bta->cse_timer);
	bta->cse_timer = NULL;
}
#endif /* WLBTAMP_LOW */

static void
wlc_bta_flush_hcidata_occurred_event(bta_info_t *bta, uint16 llh)
{
	amp_hci_event_t *evt;
	flush_occurred_evt_parms_t *flush_parms;

	/* generate Flush Occurred event */
	if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Flush_Occurred,
	                sizeof(flush_occurred_evt_parms_t))) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_flush_hcidata_occurred_event: failed to create event\n",
		          bta->wlc->pub.unit));
		return;
	}

	flush_parms = (flush_occurred_evt_parms_t *)evt->parms;
	htol16_ua_store(llh, (uint8 *)&flush_parms->handle);

	wlc_bta_doevent(bta, evt);

	wlc_bta_free_hcievent(bta, evt);
}

#ifdef WLBTAMP_LOW
static void
wlc_bta_flush_hcidata_complete_event(bta_info_t *bta, uint16 llh)
{
	amp_hci_event_t *evt;
	eflush_complete_evt_parms_t *eflush_parms;

	/* generate Enhanced Flush Complete event */
	if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Enhanced_Flush_Complete,
	                sizeof(eflush_complete_evt_parms_t))) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_flush_hcidata_complete_event: failed to create event\n",
		          bta->wlc->pub.unit));
		return;
	}

	eflush_parms = (eflush_complete_evt_parms_t *)evt->parms;
	htol16_ua_store(llh, (uint8 *)&eflush_parms->handle);

	wlc_bta_doevent(bta, evt);

	wlc_bta_free_hcievent(bta, evt);
}
#endif /* WLBTAMP_HIGH */

#ifdef WLBTAMP_LOW
void
wlc_bta_flush_hcidata(bta_info_t *bta, uint16 llh)
{
	int prec;
	struct pktq *q = &bta->wlc->txq;

	/* Walk through the txq and toss all HCI ACL data packets */
	PKTQ_PREC_ITER(q, prec) {
		void *head_pkt = NULL;

		while (pktq_ppeek(q, prec) != head_pkt) {
			void *pkt = pktq_pdeq(q, prec);
			wlc_pkttag_t *pkttag = WLPKTTAG(pkt);

			if (WLPKTFLAG_BTA_HCI_ACL(pkttag) &&
			    (pkttag->shared.bta.llh == llh)) {
				PKTFREE(bta->wlc->pub.osh, pkt, TRUE);
				continue;
			}

			if (head_pkt == NULL)
				head_pkt = pkt;
			pktq_penq(q, prec, pkt);
		}
	}
}

uint8
wlc_bta_doconnect(bta_info_t *bta, int plidx, chanspec_t chanspec)
{
	struct scb *scb = NULL;
	scb_bta_t *scb_bta;
	int idx, macmode;
	wlc_bsscfg_t *bsscfg = NULL;
	struct ether_addr *ea;
	uint8 *mac;
	wsec_pmk_t pmk;
	bool defer_cse = FALSE;
	bta_pl_t *phy_link;
	wlc_info_t *wlc = bta->wlc;
	uint8 plh;
	uint8 chan;
	uint8 band;
	uint8 status=HCI_SUCCESS;

	phy_link = &bta->pl[plidx];
	plh = bta->plh[plidx];

#if defined(WLBTAMP_HIGH) && defined(WLBTAMP_LOW)
	/* if creator, update local AMP_ASSOC */
	if (phy_link->flags & BTA_PL_CREATOR) {
		status = wlc_bta_create_local_AMP_ASSOC(bta, chanspec, &phy_link->local, &phy_link->llen);
		if (status != HCI_SUCCESS) {
			WL_ERROR(("wl%d: wlc_bta_doconnect: fail to build local AMP_ASSOC for "
					  "physical link %d\n", wlc->pub.unit, plh));
			goto done;
		}
	}
#endif


	chan = CHSPEC_CHANNEL(chanspec);
	band = CHANNEL_BANDUNIT(wlc, chan);

	/* prepare a "global" AP bsscfg for:
	 * - beaconing (per device, for creator as well as acceptor)
	 * - creators scbs' container
	 */
	if (bta->bsscfg == NULL) {

		/* allocate bsscfg */
		idx = wlc_bsscfg_get_free_idx(wlc);
		if (idx == -1) {
			WL_ERROR(("wl%d: no free index for bsscfg\n", wlc->pub.unit));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}

		bsscfg = wlc_bsscfg_alloc(wlc, idx, BTA_BSSCFG_FLAGS);
		if (bsscfg == NULL) {
			WL_ERROR(("wl%d: Cannot create bsscfg\n", wlc->pub.unit));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}
		else if (wlc_bsscfg_bta_init(wlc, bsscfg, TRUE)) {
			WL_ERROR(("wl%d: Cannot init bsscfg\n", wlc->pub.unit));
			wlc_bsscfg_free(wlc, bsscfg);
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}

		bta->bsscfg = bsscfg;

		/* set SSID */
		ea = &bsscfg->cur_etheraddr;

		bsscfg->SSID_len = (uint8)sprintf((char *)bsscfg->SSID, amp_ssid_format,
		                                  ea->octet[0], ea->octet[1], ea->octet[2],
		                                  ea->octet[3], ea->octet[4], ea->octet[5]);

		/* set the channel (set again if already set) */
		wlc_set(wlc, WLC_SET_CHANNEL, chan);

		/* restrict associations */
		macmode = WLC_MACMODE_ALLOW;
		wlc_ioctl(wlc, WLC_SET_MACMODE, &macmode, sizeof(macmode), &bsscfg->wlcif);

		/* reset glabal states. */
		wlc_bta_activate(bta);

		/* remember the channel, when it changes later
		 * for whatever reason for example non-AMP STA roam,
		 * compare it to determine if all physical links
		 * should be torn down...
		 */
		bta->chanspec_sel = chanspec;

		/* configure security settings... */
		if (!(bta->flags & BTA_FLAGS_NO_SECURITY)) {
			/* ...for association */
			bsscfg->WPA_auth = WPA2_AUTH_PSK;
			bsscfg->wsec = AES_ENABLED;
			bsscfg->wsec_restrict = TRUE;
			bsscfg->eap_restrict = TRUE;
		}

		/* ready, steady, go */
		if (wlc_bsscfg_enable(wlc, bsscfg)) {
			WL_ERROR(("wl%d: Cannot enable bsscfg\n", wlc->pub.unit));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}

		/* defer Channel Select event until we are capable of
		 * responding to probe request.
		 */
		if (phy_link->flags & BTA_PL_CREATOR)
			defer_cse = TRUE;
	}
	else {
		if (bta->chanspec_sel != chanspec) {
			WL_ERROR(("wl%d: a different chanspec 0x%04x is selected "
			          "(we are currently on 0x%04x)\n", wlc->pub.unit,
			          chanspec, bta->chanspec_sel));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}
	}

	mac = wlc_bta_parse_tlvs(phy_link->remote, phy_link->rlen, BTA_TYPE_ID_MAC_ADDRESS);
	ASSERT(mac != NULL);

	/* use the "global" AP bsscfg for all creators */
	if (phy_link->flags & BTA_PL_CREATOR) {
		struct maclist *maclist;
		uint16 len;

		bsscfg = bta->bsscfg;

		/* restrict associations */
		/* configure BT-AMP peer for association */
		len = sizeof(*maclist) + (MAXMACLIST - 1) * sizeof(struct ether_addr);
		maclist = MALLOC(wlc->pub.osh, len);
		if (maclist == NULL) {
			WL_ERROR(("wl%d: wlc_bta_docmd: failed to allocate "
			          "maclist\n", wlc->pub.unit));
			status = HCI_ERR_MEMORY_FULL;
			goto done;
		}
		wlc_ioctl(wlc, WLC_GET_MACLIST, maclist, len, &bsscfg->wlcif);
		bcopy(&mac[3], &maclist->ea[maclist->count], ETHER_ADDR_LEN);
		maclist->count += 1;
		wlc_ioctl(wlc, WLC_SET_MACLIST, maclist, len, &bsscfg->wlcif);
		MFREE(wlc->pub.osh, maclist, len);
	}
	/* use a separate STA bsscfg for acceptor */
	else {
#ifdef USE_PRIMARY_BSSCFG
		bsscfg = wlc_bsscfg_primary(wlc);
		if (bsscfg->enable) {
			WL_ERROR(("wl%d: primary bsscfg is in use\n", wlc->pub.unit));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}
		wlc_bsscfg_reinit_ext(wlc, bsscfg, FALSE);
		bsscfg->flags |= PRIMARY_BSSCFG_FLAGS;
		bta->assoc_retry_max = wlc->assoc_retry_max;
		wlc->assoc_retry_max = 16;
		/* save security settings */
		bta->WPA_auth = bsscfg->WPA_auth;
		bta->wsec = bsscfg->wsec;
		bta->wsec_restrict = bsscfg->wsec_restrict;
		bta->eap_restrict = bsscfg->eap_restrict;
		/* use in-driver wpa-psk supplicant... */
		if (!(bta->flags & BTA_FLAGS_NO_SECURITY)) {
			bta->sup_enable_wpa = wlc->sup_enable_wpa;
			wlc->sup_enable_wpa = TRUE;
		}
		/* reset security settings... */
		else {
			bsscfg->WPA_auth = 0;
			bsscfg->wsec = 0;
			bsscfg->wsec_restrict = FALSE;
			bsscfg->eap_restrict = FALSE;
		}
#else /* !USE_PRIMARY_BSSCFG */
		/* allocate bsscfg */
		idx = wlc_bsscfg_get_free_idx(wlc);
		if (idx == -1) {
			WL_ERROR(("wl%d: no free index for bsscfg\n", wlc->pub.unit));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}

		bsscfg = wlc_bsscfg_alloc(wlc, idx, BTA_BSSCFG_FLAGS);
		if (bsscfg == NULL) {
			WL_ERROR(("wl%d: Cannot create bsscfg\n", wlc->pub.unit));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}
		else if (wlc_bsscfg_bta_init(wlc, bsscfg, FALSE)) {
			WL_ERROR(("wl%d: Cannot init bsscfg\n", wlc->pub.unit));
			wlc_bsscfg_free(wlc, bsscfg);
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}
#endif /* !USE_PRIMARY_BSSCFG */

		/* configure security settings... */
		if (!(bta->flags & BTA_FLAGS_NO_SECURITY)) {
			/* ...for association */
			bsscfg->WPA_auth = WPA2_AUTH_PSK;
			bsscfg->wsec = AES_ENABLED;
			bsscfg->wsec_restrict = TRUE;
			bsscfg->eap_restrict = TRUE;
		}
	}

	/* update txc to sync up with wsec and eap_restrict changes */
	wlc_txc_upd(wlc);

	/* find or create BT-AMP peer scb */
	if (!(scb = wlc_scblookupband(wlc, (struct ether_addr *)&mac[3], band))) {
		WL_ERROR(("wl%d: no scb for BT-AMP peer\n", wlc->pub.unit));
		status = HCI_ERR_UNSPECIFIED;
		goto done;
	}
	scb->bsscfg = bsscfg;

	/* fixup scb cubby */
	scb_bta = SCB_BTA(bta, scb);
	scb_bta->plidx = (uint8)plidx;
	scb_bta->plh = plh;

	/* fix association state */
	if (bta->flags & BTA_FLAGS_NO_ASSOC) {
		wlc_scb_setstatebit(scb, AUTHENTICATED);
		wlc_scb_setstatebit(scb, ASSOCIATED);
	}

	/* cacbe scb in physical link struct */
	phy_link->scb = scb;
	bta->numpls ++;

	/* we are done! */
	if (bta->flags & BTA_FLAGS_NO_ASSOC) {
		wlc_bta_join_complete(bta, scb, 0);
	}


	/* configure security settings... */
	if (!(bta->flags & BTA_FLAGS_NO_SECURITY)) {

		/* ...for authenticated key management */
		bzero(&pmk, sizeof(wsec_pmk_t));
		pmk.key_len = phy_link->lk_type_len & BTA_PLK_LENGTH_MASK;
		bcopy(phy_link->link_key, pmk.key, pmk.key_len);

		if (phy_link->flags & BTA_PL_CREATOR) {
#ifdef BCMAUTH_PSK
			WL_BTA(("wl%d: setting up as authenticator\n", wlc->pub.unit));

			if (bsscfg->authenticator == NULL) {
				WL_ERROR(("wl%d: authenticator not attached\n", wlc->pub.unit));
				status = HCI_ERR_UNSPECIFIED;
				goto done;
			}

			wlc_auth_set_pmk(bsscfg->authenticator, &pmk);
#endif /* BCMAUTH_PSK */
		} else {
#ifdef BCMSUP_PSK
			WL_BTA(("wl%d: setting up as supplicant\n", wlc->pub.unit));

			if (bsscfg->sup == NULL) {
				WL_ERROR(("wl%d: supplicant not attached\n", wlc->pub.unit));
				status = HCI_ERR_UNSPECIFIED;
				goto done;
			}

			bsscfg->sup_type = SUP_WPAPSK;
			wlc_sup_set_ea(bsscfg->sup, (struct ether_addr *)&mac[3]);
			wlc_sup_set_pmk(bsscfg->sup, &pmk, FALSE);
#endif /* BCMSUP_PSK */
		}
	}

	/* announce creator up running */
	if (phy_link->flags & BTA_PL_CREATOR) {
		/* generate Channel Select event */
		/* mark the Channel Select event pending on this physical link */
		phy_link->flags |= BTA_PL_CSE_PEND;

		/* we are told to defer */
		if (defer_cse) {
			ASSERT(bta->cse_timer == NULL);
			if ((bta->cse_timer =
			     wl_init_timer(wlc->wl, wlc_bta_cse_timer, bta, "bta_cse")) == NULL) {
				WL_ERROR(("wl%d: wlc_bta_doconnect: fail to create cse timer\n",
				          wlc->pub.unit));
				status = HCI_ERR_UNSPECIFIED;
				goto done;
			}
			wl_add_timer(wlc->wl, bta->cse_timer, 40, FALSE);
		}
		/* we are not told to defer and there is no pending event */
		else if (bta->cse_timer == NULL)
			wlc_bta_cse_timer(bta);
		/* we are not told to defer but there are pending event(s) */
		/* else {} */

	}
	/* connect acceptor to creator */
	else if (!(bta->flags & BTA_FLAGS_NO_ASSOC)) {
		wlc_ssid_t ssid;

		ea = (struct ether_addr *)&mac[3];

		ssid.SSID_len = sprintf((char *)ssid.SSID, amp_ssid_format,
		                        ea->octet[0], ea->octet[1], ea->octet[2],
		                        ea->octet[3], ea->octet[4], ea->octet[5]);

		if (wlc_bta_join(wlc, bsscfg, ssid.SSID, ssid.SSID_len, ea->octet, chanspec)) {
			WL_ERROR(("wl%d: wlc_bta_doconnect: fail to join %s\n",
			          wlc->pub.unit, ssid.SSID));
			status = HCI_ERR_UNSPECIFIED;
			goto done;
		}
	}

done:
	return status;
}
#endif /* WLBTAMP_LOW */

static void
wlc_bta_doconnect_error(bta_info_t *bta, int plidx, uint8 status)
{
	phy_link_evt_parms_t evt_parms;
	uint8 plh;

	plh = bta->plh[plidx];

	WL_ERROR(("wl%d: wlc_bta_doconnect_error: physical link %d error %d\n",
	          bta->wlc->pub.unit, plh, status));

	/* generate Physical Link Complete event */
	evt_parms.status = status;
	evt_parms.plh = plh;
	wlc_bta_phy_link_complete_event(bta, (uint8 *)&evt_parms, sizeof(evt_parms), FALSE);

	/* perform the rest of disconnecting physical link */
#ifdef WLBTAMP_HIGH_ONLY
	wlc_bta_disconnect_physical_link(bta, plidx);
#else
	wlc_bta_dodisconnect(bta, plidx);
#endif
}

/* Channel Selection */

#ifdef APCS
/* Channel Select complete callback */
void
wlc_bta_cs_complete(void *arg, int st)
{
	bta_info_t *bta = (bta_info_t *)arg;
	int plidx = bta->cs->plidx;
	uint8 status;


	MFREE(bta->wlc->pub.osh, bta->cs, sizeof(bta_cs_t));
	bta->cs = NULL;

	if (st == WLC_E_STATUS_SUCCESS) {
		status = wlc_bta_doconnect(bta, plidx, bta->wlc->chanspec_selected);
		if (status == HCI_SUCCESS)
			return;
		WL_ERROR(("wl%d: wlc_bta_cs_complete: wlc_bta_doconnect() failed %d\n",
		          bta->wlc->pub.unit, status));
	}
	else if (st == WLC_E_STATUS_CS_ABORT) {
#ifdef WLBTAMP_HIGH_ONLY
		wlc_bta_disconnect_physical_link(bta, plidx);
#else
		wlc_bta_dodisconnect(bta, plidx);
#endif
		return;
	}
	else {
		WL_ERROR(("wl%d: wlc_bta_cs_complete: select channel failed %d\n",
		          bta->wlc->pub.unit, st));
		status = HCI_ERR_UNSPECIFIED;
	}

	wlc_bta_doconnect_error(bta, plidx, status);
}
#endif /* APCS */

/* select a channel or kick off channel selection process */
#define WLC_CHAN_BAND(ch)	((ch) <= WLC_MAX_2G_CHANNEL ? WLC_BAND_2G : WLC_BAND_5G)

#ifdef WLBTAMP_HIGH
static uint8
wlc_bta_cs(bta_info_t *bta, int plidx, uint8 *pref_chan, uint pref_chan_len,
	chanspec_t *chanspec_sel)
{
	wlc_info_t *wlc = bta->wlc;
	uint8 *triplet;
	uint8 *country;
	uint8 pref_rc[MAXRCLISTSIZE];
	uint8 pref_rcs;
	uint8 i;
	chanspec_t chanspec;
	int band, orig_band;
	uint8 cur_pref_rc = 0;
	int8 cur_pref_rc_idx = -1;
	uint8 chan[MAXCHANNEL];
	uint8 chans;
	wl_uint32_list_t *req;
	uint req_len;
	uint8 status;
	bool non_country = FALSE;
	int ret;

	/* the pre_chan must include country string and 1 channel specification */
	if (pref_chan_len < 6 || pref_chan_len % 3 != 0) {
		WL_ERROR(("wl%d: wlc_bta_cs: malformed Preferred Channel List\n",
		          wlc->pub.unit));
		return HCI_ERR_UNSPECIFIED;
	}

	/* apply the country string, assuming the first 2 letters are same as ours */
	country = pref_chan;
#ifdef WLBTAMP_HIGH_ONLY
	ret = wl_ioctl(wlc->wl, WLC_SET_COUNTRY, country, 2);
#else
	ret = wlc_ioctl(wlc, WLC_SET_COUNTRY, country, 2, NULL);
#endif
	if (ret != BCME_OK) {
		WL_ERROR(("wl%d: wlc_bta_cs: unsupported Country String %c%c%c\n",
		          wlc->pub.unit, country[0], country[1], country[2]));
		return HCI_ERR_UNSPECIFIED;
	}

	non_country = wlc->country_abbrev[0] == 'X' && wlc->country_abbrev[1] == 'X';

	/* Get channels both listed by regulatory classes and explicit channels
	 * in the Preferred Channel List and supported by the driver supports
	 */

	/* 1. get all channels the driver supports */
#ifdef WLBTAMP_HIGH_ONLY
	wl_ioctl(wlc->wl, WLC_GET_BAND, (char *)&orig_band, sizeof(orig_band));
#else
	wlc_ioctl(wlc, WLC_GET_BAND, &orig_band, sizeof(orig_band), NULL);
#endif
	band = WLC_BAND_AUTO;
#ifdef WLBTAMP_HIGH_ONLY
	wl_ioctl(wlc->wl, WLC_SET_BAND, (char *)&band, sizeof(band));
#else
	wlc_ioctl(wlc, WLC_SET_BAND, &band, sizeof(band), NULL);
#endif
	for (chans = 0, i = 0; i < MAXCHANNEL; i ++) {
		if (VALID_CHANSPEC_DB(wlc, CH20MHZ_CHSPEC(i)))
			chan[chans ++] = i;
	}

#ifdef WLBTAMP_HIGH_ONLY
	wl_ioctl(wlc->wl, WLC_SET_BAND, (char *)&orig_band, sizeof(orig_band));
#else
	wlc_ioctl(wlc, WLC_SET_BAND, &orig_band, sizeof(orig_band), NULL);
#endif

	/* 2. build preferred channel list based on the Preferred Channel List */
	req_len = OFFSETOF(wl_uint32_list_t, element) + sizeof(req->element[0]) * chans;
	if ((req = MALLOC(wlc->pub.osh, req_len)) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_cs: unable to allocate Channel Select "
		          "channel list\n", wlc->pub.unit));
		return HCI_ERR_MEMORY_FULL;
	}
	req->count = 0;
	bzero(pref_rc, sizeof(pref_rc));
	pref_rcs = 0;
	for (triplet = pref_chan + 3; triplet < pref_chan + pref_chan_len; triplet += 3) {
		/* regulatory extension identifier */
		if (triplet[0] == 201) {
			if (pref_rcs >= sizeof(pref_rc)) {
				WL_ERROR(("wl%d: wlc_bta_cs: Preferred Channel List error, "
				          "too many regulatory classes\n", wlc->pub.unit));
				status = HCI_ERR_UNSPECIFIED;
				goto exit;
			}
			/* copy channels of the last regulatory class if it hasn't
			 * been invalidated by any explicit channels.
			 */
			if (cur_pref_rc_idx >= 0 && pref_rc[cur_pref_rc_idx] != 0) {
				for (i = 0; i < chans; i ++) {
					if ((non_country && chan[i] >= 1 && chan[i] <= 11) ||
					    wlc_get_regclass(wlc, CH20MHZ_CHSPEC(chan[i])) ==
					    cur_pref_rc)
						req->element[req->count++] = chan[i];
				}
			}
			/* save the current regulatory class */
			cur_pref_rc = pref_rc[pref_rcs] = triplet[1];
			cur_pref_rc_idx = (int8)pref_rcs;
			pref_rcs ++;
		}
		/* explicit channel number */
		else if (triplet[0] < MAXCHANNEL) {
			if (cur_pref_rc_idx < 0) {
				WL_ERROR(("wl%d: wlc_bta_cs: Preferred Channel List error, "
				          "channel %u present without a regulatory class\n",
				          wlc->pub.unit, triplet[0]));
				status = HCI_ERR_UNSPECIFIED;
				goto exit;
			}
			/* copy specific channel(s) */
			for (i = 0; i < triplet[1]; i ++) {
				uint8 pref_chan = triplet[0] + i;
				if ((non_country && pref_chan >= 1 && pref_chan <= 11) ||
				    wlc_get_regclass(wlc, CH20MHZ_CHSPEC(pref_chan)) == cur_pref_rc)
					req->element[req->count++] = pref_chan;
			}
			/* invalidate the current regulatory class */
			pref_rc[cur_pref_rc_idx] = 0;
		}
	}
	/* copy channels of the last regulatory class if it hasn't
	 * been invalidated by any explicit channels.
	 */
	if (cur_pref_rc_idx >= 0 && pref_rc[cur_pref_rc_idx] != 0) {
		for (i = 0; i < chans; i ++) {
			if ((non_country && chan[i] >= 1 && chan[i] <= 11) ||
			    wlc_get_regclass(wlc, CH20MHZ_CHSPEC(chan[i])) == cur_pref_rc)
				req->element[req->count++] = chan[i];
		}
	}
	/* there must be at least one channel in the preferred channel list */
	if (req->count == 0) {
		WL_ERROR(("wl%d: wlc_bta_cs: no channel specified in Preferred Channel List\n",
		          wlc->pub.unit));
		status = HCI_ERR_UNSPECIFIED;
		goto exit;
	}

	chanspec = wlc_bta_chan_select(bta);

	/* we are connected to someone so honor the channel we are on when
	 * it is also in the acceptor's Preferred Chennel
	 */
	if (chanspec != 0) {
		for (i = 0; i < req->count; i ++) {
			if (chanspec == CH20MHZ_CHSPEC(req->element[i])) {
				*chanspec_sel = chanspec;
				status = HCI_SUCCESS;
				goto exit;
			}
		}
		WL_ERROR(("wl%d: wlc_bta_cs: chanspec 0x%04x is not in Preferred Channel List\n",
		          wlc->pub.unit, chanspec));
		status = HCI_ERR_UNSPECIFIED;
		goto exit;
	}

	/* we are not connected so go by the acceptor's Preferred Channel List */
	band = WLC_CHAN_BAND(req->element[0]);
	for (i = 1; i < req->count; i ++) {
		if (band != WLC_CHAN_BAND(req->element[i]))
			break;
	}
	req->count = i;
	if (req->count == 1) {
		*chanspec_sel = CH20MHZ_CHSPEC(req->element[0]);
		status = HCI_SUCCESS;
		goto exit;
	}

#ifdef APCS
	/* Channel Select (to select a channel in which
	 * there is the least number of BSS(es) running...
	 */

	if (bta->cs != NULL) {
		WL_ERROR(("wl%d: wlc_bta_cs: Channel Select in progress\n", wlc->pub.unit));
		status = HCI_ERR_UNSPECIFIED;
		goto exit;
	}

	/* allocate memory to hold necessary parameters for the channel select callback. */
	if ((bta->cs = MALLOC(wlc->pub.osh, sizeof(bta_cs_t))) == NULL) {
		WL_ERROR(("wl%d: wlc_bta_cs: unable to allocate Channel Select "
		          "info block\n", wlc->pub.unit));
		status = HCI_ERR_MEMORY_FULL;
		goto exit;
	}
	bta->cs->plidx = plidx;

	/* kick off Channel Select process */
	if (wlc_cs_scan_start(wlc, req, FALSE, TRUE, wlc_bta_cs_complete, bta) != BCME_OK) {
		WL_ERROR(("wl%d: wlc_bta_cs: unable to start Channel Select process\n",
		          wlc->pub.unit));
		status = HCI_ERR_UNSPECIFIED;
		goto exit;
	}

#else
	*chanspec_sel = CH20MHZ_CHSPEC(req->element[0]);
#endif /* APCS */
	status = HCI_SUCCESS;

exit:
	MFREE(wlc->pub.osh, req, req_len);
	return status;
}

static uint8
wlc_bta_get_rclist_20MHz(bta_info_t *bta, uint8 rclist[], uint *count, uint8 band)
{
	wlc_info_t *wlc = bta->wlc;
	wl_uint32_list_t *chspec_list;
	uint chspec_list_len;
	uint idx, i, j;
	uint8 rc;

	chspec_list_len = OFFSETOF(wl_uint32_list_t, element) +
	        sizeof(chspec_list->element[0]) * MAXCHANNEL;
	if ((chspec_list = MALLOC(bta->wlc->pub.osh, chspec_list_len)) == NULL) {
		WL_ERROR(("wl%d: failed to allocate Local AMP ASSOC\n", wlc->pub.unit));
		return HCI_ERR_MEMORY_FULL;
	}
	chspec_list->count = 0;

	wlc_get_valid_chanspecs(wlc, chspec_list, TRUE, band == WLC_BAND_2G, wlc->country_abbrev);

	for (i = 0, idx = 0; i < chspec_list->count; i ++) {
		/* skip all duplicates */
		if (chspec_list->element[i] == 0)
			continue;

		rc = wlc_get_regclass(wlc, (chanspec_t)chspec_list->element[i]);
		if (rc == 0) {
			WL_ERROR(("wl%d: failed to find regulatory class for chanspec 0x%04x\n",
			          wlc->pub.unit, chspec_list->element[i]));
			continue;
		}
		rclist[idx++] = rc;

		/* remove all duplicates */
		for (j = i + 1; j < chspec_list->count; j ++) {
			if (wlc_get_regclass(wlc, (chanspec_t)chspec_list->element[j]) == rc)
				chspec_list->element[j] = 0;
		}
	}

	MFREE(wlc->pub.osh, chspec_list, chspec_list_len);

	*count = idx;

	return HCI_SUCCESS;
}

/* make sure AMP_ASSOC/AMP_ASSOC_len are not NULL */
static uint8
wlc_bta_build_AMP_ASSOC(bta_info_t *bta, chanspec_t chanspec,
	uint8 **AMP_ASSOC, uint16 *AMP_ASSOC_len)
{
	wlc_info_t *wlc = bta->wlc;
	uint8 *local;
	uint16 local_len;
	uint8 *mac;
	uint8 rclist[MAXRCLISTSIZE];
	uint8 rcs;
	uint8 *pref_chan;
	uint16 pref_chan_len;
	uint8 i;
	bool non_country = FALSE;

	ASSERT(AMP_ASSOC != NULL);
	ASSERT(AMP_ASSOC_len != NULL);

	non_country = wlc->country_abbrev[0] == 'X' && wlc->country_abbrev[1] == 'X';

	/* local AMP_ASSOC length is default_local + MAC Address + Preferred Channel List */
	if (chanspec != 0) {
		if (non_country)
			rclist[0] = 254;
		else if ((rclist[0] = wlc_get_regclass(wlc, chanspec)) == 0) {
			WL_ERROR(("wl%d: failed to find regulatory class for chanspec 0x%04x\n",
			          wlc->pub.unit, chanspec));
			return HCI_ERR_UNSPECIFIED;
		}
		rcs = 1;
		/* Country String + Reg Class + Chan Spec */
		pref_chan_len = 3 + 3 + 3;
	}
	else {
		if (non_country) {
			rclist[0] = 254;
			rcs = 1;
		}
		else {
			uint8 status;
			uint lsize;
			rcs = 0;
			if ((status = wlc_bta_get_rclist_20MHz(bta, &rclist[rcs], &lsize,
			                WLC_BAND_5G)) != HCI_SUCCESS)
				return status;
			rcs += (uint8)lsize;
			if ((status = wlc_bta_get_rclist_20MHz(bta, &rclist[rcs], &lsize,
			                WLC_BAND_2G)) != HCI_SUCCESS)
				return status;
			rcs += (uint8)lsize;
			if (rcs == 0) {
				WL_ERROR(("wl%d: failed to find regulatory class(es)\n",
				          wlc->pub.unit));
				return HCI_ERR_UNSPECIFIED;
			}
		}
		/* Country String + Reg Classes... */
		pref_chan_len = 3 + rcs * 3;
	}
	local_len = sizeof(default_local) + 3 + ETHER_ADDR_LEN + 3 + pref_chan_len;

	if ((local = MALLOC(wlc->pub.osh, local_len)) == NULL) {
		WL_ERROR(("wl%d: failed to allocate Local AMP ASSOC\n", wlc->pub.unit));
		return HCI_ERR_MEMORY_FULL;
	}

	/* initialize local AMP_ASSOC */
	/* default_local:
	 * 0x04 0x04 0x00 0x01 0x00 0x00 0x00
	 * 0x05 0x05 0x00 0x00 0x0f 0x00 0x10 0x09
	 */
	bcopy(default_local, local, sizeof(default_local));
	/* mac address:
	 * 0x01 0x06 0x00 <MAC Address>
	 */
	mac = local + sizeof(default_local);
	mac[0] = BTA_TYPE_ID_MAC_ADDRESS;
	htol16_ua_store(ETHER_ADDR_LEN, &mac[1]);
	bcopy(&wlc->pub.cur_etheraddr, &mac[3], ETHER_ADDR_LEN);
	/* preferred channel list:
	 * 0x02 (tag)
	 * 0xXX 0xXX (length)
	 * 0xXX 0xXX 0xXX (country string)
	 * ... (regulatory class(es))
	 */
	pref_chan = local + sizeof(default_local) + 3 + ETHER_ADDR_LEN;
	pref_chan[0] = BTA_TYPE_ID_PREFERRED_CHANNELS;
	htol16_ua_store(pref_chan_len, &pref_chan[1]);
	pref_chan[3] = wlc->country_abbrev[0];
	pref_chan[4] = wlc->country_abbrev[1];
	/* our country abbrev is only 2 char long, extend it 'X' (for 'X''X') or
	 * ' ' (for others in all environments)
	 */
	pref_chan[5] = non_country ? 'X' : ' ';
	if (chanspec != 0) {
		pref_chan[6] = 201;
		pref_chan[7] = rclist[0];
		pref_chan[8] = 0;
		pref_chan[9] = CHSPEC_CHANNEL(chanspec);
		pref_chan[10] = 1;
		pref_chan[11] = 20;
	}
	else {
		for (i = 0; i < rcs; i ++) {
			pref_chan[6 + i * 3] = 201;
			pref_chan[7 + i * 3] = rclist[i];
			pref_chan[8 + i * 3] = 0;
		}
	}

	*AMP_ASSOC = local;
	*AMP_ASSOC_len = local_len;

	return HCI_SUCCESS;
}

/* make sure AMP_ASSOC/AMP_ASSOC_len are not NULL */
static uint8
wlc_bta_create_local_AMP_ASSOC(bta_info_t *bta, chanspec_t chanspec,
	uint8 **AMP_ASSOC, uint16 *AMP_ASSOC_len)
{
	wlc_info_t *wlc = bta->wlc;
	uint8 status;

	ASSERT(AMP_ASSOC && AMP_ASSOC_len);

	if (*AMP_ASSOC != NULL) {
		MFREE(wlc->pub.osh, *AMP_ASSOC, *AMP_ASSOC_len);
		*AMP_ASSOC = NULL;
	}

	status = wlc_bta_build_AMP_ASSOC(bta, chanspec, AMP_ASSOC, AMP_ASSOC_len);
	if (status != HCI_SUCCESS)
		WL_ERROR(("wl%d: %s: fail to build local AMP_ASSOC\n",
			  wlc->pub.unit, __FUNCTION__));

	return status;
}

#endif /* WLBTAMP_HIGH */

#if defined(BCMDBG) || defined(WLMSG_BTA)
/* #define VAL_HCI_CMD_LEN */
#endif

#ifdef VAL_HCI_CMD_LEN
/* cmd parms minimum length (for those we care) */
static const struct {
	uint16 op_code;
	uint8 min_plen;
} bta_cmd_min_plen[] = {
	{HCI_Read_Local_AMP_ASSOC, OFFSETOF(read_local_cmd_parms_t, max_remote)},
	{HCI_Write_Remote_AMP_ASSOC, OFFSETOF(write_remote_cmd_parms_t, frag)},
	{HCI_Read_Link_Supervision_Timeout, OFFSETOF(ls_to_cmd_parms_t, timeout)},
	{HCI_Write_Link_Supervision_Timeout, sizeof(ls_to_cmd_parms_t)},
	{HCI_Read_Best_Effort_Flush_Timeout, OFFSETOF(befto_cmd_parms_t, befto)},
	{HCI_Write_Best_Effort_Flush_Timeout, sizeof(befto_cmd_parms_t)},
	{HCI_Create_Physical_Link, sizeof(phy_link_cmd_parms_t)},
	{HCI_Accept_Physical_Link_Request, sizeof(phy_link_cmd_parms_t)},
	{HCI_Disconnect_Physical_Link, sizeof(dis_phy_link_cmd_parms_t)},
	{HCI_Accept_Logical_Link, sizeof(log_link_cmd_parms_t)},
	{HCI_Create_Logical_Link, sizeof(log_link_cmd_parms_t)},
	{HCI_Disconnect_Logical_Link, 2},
	{HCI_Logical_Link_Cancel, sizeof(log_link_cancel_cmd_parms_t)},
	{HCI_Short_Range_Mode, sizeof(srm_cmd_parms_t)},
	{HCI_Write_Connection_Accept_Timeout, 2},
	{HCI_Write_Logical_Link_Accept_Timeout, 2},
	{HCI_Read_Link_Quality, 1},
	{HCI_Enhanced_Flush, 2}
};
#endif /* VAL_HCI_CMD_LEN */

int
wlc_bta_docmd(bta_info_t *bta, uint8 *cmd_buf, uint cmd_len)
{
	amp_hci_cmd_t *cmd = (amp_hci_cmd_t *)cmd_buf;
#if defined(WLBTAMP_HIGH) || defined(VAL_HCI_CMD_LEN)
	wlc_info_t *wlc = bta->wlc;
#endif
	uint16 op = 0; /* HCI_NOP */
	uint8 status = HCI_SUCCESS;
#ifdef VAL_HCI_CMD_LEN
	uint8 plen = 0;
	uint8 i;
#endif

#ifdef VAL_HCI_CMD_LEN
	/* validate cmd buf length */
	if (cmd_len < HCI_CMD_PREAMBLE_SIZE) {
		WL_ERROR(("wl%d: wlc_bta_docmd: command buffer is too short\n",
		          wlc->pub.unit));
		status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
		goto fmt_err;
	}
#endif

	op = ltoh16_ua((uint8 *)&cmd->opcode);

#ifdef VAL_HCI_CMD_LEN
	plen = cmd->plen;

	/* validate cmd buf parm length */
	if (cmd_len < HCI_CMD_PREAMBLE_SIZE + plen) {
		WL_ERROR(("wl%d: wlc_bta_docmd: command buffer length and parms length mismatch\n",
		          wlc->pub.unit));
		status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
		goto fmt_err;
	}
#endif

#if defined(BCMDBG) || defined(WLMSG_BTA)
	if (WL_BTA_ON())
		wlc_bta_hcidump_cmd(bta, cmd);
#endif

#ifdef VAL_HCI_CMD_LEN
	/* validate minimum cmd parm length */
	for (i = 0; i < ARRAYSIZE(bta_cmd_min_plen); i ++) {
		if (bta_cmd_min_plen[i].op_code == op &&
		    bta_cmd_min_plen[i].min_plen > plen) {
			WL_ERROR(("wl%d: wlc_bta_docmd: command parms is too short\n",
			          wlc->pub.unit));
			status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
			goto fmt_err;
		}
	}

	status = HCI_SUCCESS;

fmt_err:
	/* generate Command Status event */
	if (status != HCI_SUCCESS) {
		wlc_bta_cmd_status_event(bta, op, status);
		return;
	}
#endif /* VAL_HCI_CMD_LEN */

	switch (op) {
#ifdef WLBTAMP_HIGH
	case HCI_Read_Local_AMP_Info: {
		read_local_info_evt_parms_t evt_parms;

		/* generate the "global" default AMP_Info */
		evt_parms.status = HCI_SUCCESS;
		evt_parms.AMP_status = 0x1;
		htol32_ua_store(30000, (uint8 *)&evt_parms.bandwidth);
		htol32_ua_store(30000, (uint8 *)&evt_parms.gbandwidth);
		htol32_ua_store(28, (uint8 *)&evt_parms.latency);
		htol32_ua_store(BTA_MTU, (uint8 *)&evt_parms.PDU_size);
		evt_parms.ctrl_type = 0x1;
		htol16_ua_store(wlc_bta_qos(bta), (uint8 *)&evt_parms.PAL_cap);
		htol16_ua_store(672, (uint8 *)&evt_parms.AMP_ASSOC_len);
		htol32_ua_store(BTA_INVFTO, (uint8 *)&evt_parms.max_flush_timeout);
		htol32_ua_store(BTA_INVFTO, (uint8 *)&evt_parms.be_flush_timeout);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));
		break;
	}

	case HCI_Read_Local_AMP_ASSOC: {
		read_local_cmd_parms_t *parms = (read_local_cmd_parms_t *)cmd->parms;
		read_local_evt_parms_t *evt_ptr = NULL;
		read_local_evt_parms_t evt_parms;
		uint evt_size = 0;
		uint16 offset;
		uint8 *local = NULL;
		uint16 len = 0;
		uint16 flen = 0;	/* len of frag field */

		offset = ltoh16_ua(parms->offset);

		/* read global AMP_ASSOC (for AMP_Get_Info_Response) */
		if (parms->plh == 0) {
			if (offset == 0) {
				status = wlc_bta_create_local_AMP_ASSOC(bta,
					wlc_bta_chan_select(bta), &bta->local, &bta->llen);
			}
			else if (bta->local == NULL) {
				WL_ERROR(("wl%d: wlc_bta_docmd: local AMP_ASSOC on "
				          "physical link %d hasn't been initialized\n",
				          wlc->pub.unit, parms->plh));
				status = HCI_ERR_UNSPECIFIED;
			}
			else
				status = HCI_SUCCESS;
			if (status == HCI_SUCCESS) {
				if (offset >= bta->llen)
				{
					status = HCI_ERR_PARAM_OUT_OF_RANGE;
				} else {
					local = bta->local;
					len = bta->llen;
				}
			}
		}
		/* read specific physical links AMP_ASSOC */
		else {
			int plidx=0;

			status = wlc_bta_pllookup(bta, parms->plh, &plidx);
			if (status == HCI_SUCCESS) {
				if (offset >= bta->pl[plidx].llen)
				{
					status = HCI_ERR_PARAM_OUT_OF_RANGE;
				} else if (bta->pl[plidx].local == NULL) {
					WL_ERROR(("wl%d: wlc_bta_docmd: local AMP_ASSOC on "
					          "physical link %d hasn't been initialized\n",
					          wlc->pub.unit, parms->plh));
					status = HCI_ERR_UNSPECIFIED;
				}
				else {
					local = bta->pl[plidx].local;
					len = bta->pl[plidx].llen;
				}
			}
		}

		/* generate Command Complete event with local AMP_ASSOC */
		if (status == HCI_SUCCESS) {

			flen = len - offset;

			/* limit the frag field to 246 octets */
			if (flen > 246)
				flen = 246;

			/* allocate memory for local AMP_ASSOC data */
			evt_size = OFFSETOF(read_local_evt_parms_t, frag) + flen;
			if ((evt_ptr = MALLOC(wlc->pub.osh, evt_size)) == NULL) {
				WL_ERROR(("wl%d: wlc_bta_docmd: failed to create "
				          "Read Local AMP ASSOC event\n", wlc->pub.unit));
				status = HCI_ERR_MEMORY_FULL;
			}
		}
		/* use minimum event on the stack to communicate error status */
		if (status != HCI_SUCCESS) {
			flen = 0;
			evt_size = OFFSETOF(read_local_evt_parms_t, frag);
			evt_ptr = &evt_parms;
		}
		evt_ptr->status = status;
		evt_ptr->plh = parms->plh;
		htol16_ua_store(flen, (uint8 *)&evt_ptr->len);
		/* start reading at offset Length So Far parameter */
		if (status == HCI_SUCCESS)
			bcopy(&local[offset], evt_ptr->frag, flen);
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)evt_ptr, evt_size);

		if (status == HCI_SUCCESS)
			MFREE(wlc->pub.osh, evt_ptr, evt_size);

		break;
	}

	case HCI_Write_Remote_AMP_ASSOC: {
		write_remote_cmd_parms_t *parms = (write_remote_cmd_parms_t *)cmd->parms;
		write_remote_evt_parms_t evt_parms;
		bta_pl_t *phy_link = NULL;
		uint16 offset;
		uint16 fraglen = 0;
		int plidx=0;
		uint8 *remote, *mac, *pref_chan = NULL, *ver_ie;
		uint16 len = 0;

		offset = ltoh16_ua(parms->offset);

		/* find target physical link */
		status = wlc_bta_pllookup(bta, parms->plh, &plidx);

		/* accumulate remote AMP_ASSOC */
		do {
			if (status != HCI_SUCCESS) {
				WL_ERROR(("wl%d: wlc_bta_docmd: wlc_bta_pllookup failed\n",
				          wlc->pub.unit));
				break;
			}

			phy_link = &bta->pl[plidx];

			if (phy_link->flags & BTA_PL_CONN) {
				WL_ERROR(("wl%d: physical link %d is already in "
				          "connecting/connected state\n",
				          wlc->pub.unit, parms->plh));
				status = HCI_ERR_UNSPECIFIED;
				break;
			}
			phy_link->flags |= BTA_PL_CONN;

			/* allocate remote AMP_ASSOC if necessary */
			if (offset == 0) {
				if (phy_link->remote != NULL) {
					MFREE(wlc->pub.osh, phy_link->remote, phy_link->rlen);
					phy_link->remote = NULL;
				}
				len = phy_link->rlen = ltoh16_ua(parms->len);
				remote = phy_link->remote = MALLOC(wlc->pub.osh, len);
				if (remote == NULL) {
					WL_ERROR(("wl%d: wlc_bta_docmd: failed to allocate "
					          "Remote AMP ASSOC\n", wlc->pub.unit));
					status = HCI_ERR_MEMORY_FULL;
					break;
				}
			}
			else {
				remote = phy_link->remote;
				len = phy_link->rlen;
			}

			/* write frag starting at offset */
			fraglen = cmd->plen - OFFSETOF(write_remote_cmd_parms_t, frag);
			if (len < offset + fraglen) {
				WL_ERROR(("wl%d: wlc_bta_docmd: too much data\n",
				          wlc->pub.unit));
				status = HCI_ERR_UNSPECIFIED;
				break;
			}
			bcopy(parms->frag, &remote[offset], fraglen);

			/* remote AMP_ASSOC is partial... */
			if (len > offset + fraglen) {
				WL_BTA(("wl%d: wlc_bta_docmd: more data to come...\n",
				        wlc->pub.unit));
				break;
			}


			/* MAC address */
			mac = wlc_bta_parse_tlvs(remote, len, BTA_TYPE_ID_MAC_ADDRESS);
			if (!mac || (mac[1] != ETHER_ADDR_LEN) || (mac[2] != 0)) {
				WL_ERROR(("wl%d: missing MAC address or invalid MAC address "
				          "length in remote AMP_ASSOC\n", wlc->pub.unit));
				status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
				break;
			}

			/* Preferred Channel List */
			pref_chan = wlc_bta_parse_tlvs(remote, len, BTA_TYPE_ID_PREFERRED_CHANNELS);
			if (pref_chan == NULL) {
				WL_ERROR(("wl%d: no preferred channel list in "
					  "remote AMP_ASSOC\n", wlc->pub.unit));
				status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
				break;
			}

			/* PAL version */
			ver_ie = wlc_bta_parse_tlvs(remote, len, BTA_TYPE_ID_VERSION);
			if (!ver_ie || (ver_ie[1] != 5) || (ver_ie[2] != 0)) {
				WL_ERROR(("wl%d: missing PAL version or invalid PAL version length "
					  "in remote AMP_ASSOC\n", wlc->pub.unit));
				status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
				break;
			}
		} while (FALSE);

		/* generate Command Complete event */
		evt_parms.status = status;
		evt_parms.plh = parms->plh;
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));

		/* we now have the complete remote AMP_ASSOC */
		if ((status == HCI_SUCCESS) && (len == offset + fraglen)) {
			chanspec_t chanspec = 0;

			/* select a channel or start a channel selection process */
			status = wlc_bta_cs(bta, plidx, &pref_chan[3], ltoh16_ua(&pref_chan[1]), &chanspec);

			/* non 0 chanspec indicates the channel is selected so go start it */
			if (status == HCI_SUCCESS && chanspec != 0)
			{
				status = wlc_bta_doconnect(bta, plidx, chanspec);
			}
			/* 0 chanspec indicates the channel selection process in ongoing
			 * and we will start the connection once the prcess is finished.
			 */
			if (status == HCI_SUCCESS)
				break;

			/* channel select or start connetion failed */
			wlc_bta_doconnect_error(bta, plidx, status);
		}

		break;
	}

	case HCI_Read_Link_Supervision_Timeout: {
		ls_to_cmd_parms_t *cmdparms = (ls_to_cmd_parms_t *)cmd->parms;
		read_ls_to_evt_parms_t evt_parms;
		uint8 cmdplh;
		int plidx=0;

		do {
			/* sanity check: physical link handle cannot be zero */
			cmdplh = cmdparms->handle.amp.plh;
			evt_parms.handle.amp.plh = cmdplh;
			if (cmdplh == 0) {
				status = HCI_ERR_UNSUPPORTED_VALUE;
				break;
			}

			/* find target physical link */
			status = wlc_bta_pllookup(bta, cmdplh, &plidx);
			if (status != HCI_SUCCESS)
				break;

			htol16_ua_store(bta->pl[plidx].ls_to, (uint8 *)&evt_parms.timeout);

		} while (FALSE);

		/* generate Command Complete event */
		evt_parms.status = status;
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));
		break;
	}

	case HCI_Write_Link_Supervision_Timeout: {
		ls_to_cmd_parms_t *cmdparms = (ls_to_cmd_parms_t *)cmd->parms;
		uint8 cmdplh;
		int plidx=0;

		do {
			/* sanity check: physical link handle cannot be zero */
			cmdplh = cmdparms->handle.amp.plh;
			if (cmdplh == 0) {
				status = HCI_ERR_UNSUPPORTED_VALUE;
				break;
			}

			/* find target physical link */
			status = wlc_bta_pllookup(bta, cmdplh, &plidx);
			if (status != HCI_SUCCESS)
				break;

			/* set timeout */
			bta->pl[plidx].ls_to = ltoh16_ua(cmdparms->timeout);

		} while (FALSE);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&status, sizeof(status));
		break;
	}

	case HCI_Read_Connection_Accept_Timeout: {
		read_lla_ca_to_evt_parms_t evt_parms;

		evt_parms.status = HCI_SUCCESS;
		htol16_ua_store(bta->ca_to, (uint8 *)&evt_parms.timeout);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));
		break;
	}

	case HCI_Write_Connection_Accept_Timeout: {
		uint16 timeout;

		/* check range */
		timeout = ltoh16_ua(cmd->parms);
		if (timeout >= 1 && timeout <= 0xb540) {
			bta->ca_to = timeout;
			status = HCI_SUCCESS;
		} else {
			status = HCI_ERR_PARAM_OUT_OF_RANGE;
		}

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&status, sizeof(status));
		break;
	}

	case HCI_Read_Link_Quality: {
		read_linkq_evt_parms_t evt_parms;
		uint8 plh;
		int plidx=0;

		do {
			/* sanity check: physical link handle cannot be zero */
			plh = cmd->parms[0];
			evt_parms.handle.amp.plh = plh;
			if (plh == 0) {
				status = HCI_ERR_UNSUPPORTED_VALUE;
				break;
			}

			/* find target physical link */
			status = wlc_bta_pllookup(bta, plh, &plidx);
			if (status != HCI_SUCCESS)
				break;

			evt_parms.link_quality = wlc_bta_phy_link_rssi(bta, plidx);

		} while (FALSE);

		/* generate Command Complete event */
		evt_parms.status = status;
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));
		break;
	}

	case HCI_Read_Best_Effort_Flush_Timeout: {
		uint16 llh = ltoh16_ua(cmd->parms);
		befto_evt_parms_t evt_parms;

		do {
			status = wlc_bta_valid_logical_link(bta, llh);
			if (status != HCI_SUCCESS)
				break;


			htol32_ua_store(bta->ll[llh].fto, (uint8 *)&evt_parms.befto);

		} while (FALSE);

		/* generate Command Complete event */
		evt_parms.status = status;
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));

		break;
	}

	case HCI_Write_Best_Effort_Flush_Timeout: {
		befto_cmd_parms_t *cmd_parms = (befto_cmd_parms_t *)cmd->parms;
		uint16 llh = ltoh16_ua(cmd_parms->llh);

		do {
			status = wlc_bta_valid_logical_link(bta, llh);
			if (status != HCI_SUCCESS)
				break;


			bta->ll[llh].fto = ltoh32_ua(cmd_parms->befto);
		} while (FALSE);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&status, sizeof(status));

		break;
	}

	case HCI_Short_Range_Mode: {
		srm_cmd_parms_t *cmd_parms = (srm_cmd_parms_t *)cmd->parms;
		srm_evt_parms_t *evt_parms;
		amp_hci_event_t *evt;
		int plidx;

		do {
			/* find target physical link */
			status = wlc_bta_pllookup(bta, cmd_parms->plh, &plidx);
			if (status != HCI_SUCCESS)
				break;

			/* sanity check short range mode setting */
			if (cmd_parms->srm > 1) {
				status = HCI_ERR_UNSUPPORTED_VALUE;
				break;
			}

			/* apply short range mode setting */
			bta->pl[plidx].short_range = cmd_parms->srm;

		} while (FALSE);

		/* generate Command Status event */
		wlc_bta_cmd_status_event(bta, op, status);
		if (status != HCI_SUCCESS)
			break;

		/* generate Short Range Mode Change Complete event */
		if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Short_Range_Mode_Change_Complete,
		                sizeof(srm_evt_parms_t))) == NULL) {
			WL_ERROR(("wl%d: wlc_bta_docmd: failed to create Short Range Mode event\n",
			          wlc->pub.unit));
			break;
		}

		evt_parms = (srm_evt_parms_t *)evt->parms;
		evt_parms->status = status;
		evt_parms->plh = cmd_parms->plh;
		evt_parms->srm = cmd_parms->srm;

		wlc_bta_doevent(bta, evt);

		wlc_bta_free_hcievent(bta, evt);
		break;
	}

	case HCI_Create_Physical_Link:
		status = wlc_bta_connect_phy_link(bta,
		        (phy_link_cmd_parms_t *)cmd->parms, cmd->plen, TRUE);
		wlc_bta_cmd_status_event(bta, op, status);
		break;

	case HCI_Accept_Physical_Link_Request:
		status = wlc_bta_connect_phy_link(bta,
		        (phy_link_cmd_parms_t *)cmd->parms, cmd->plen, FALSE);
		wlc_bta_cmd_status_event(bta, op, status);
		break;

	case HCI_Disconnect_Physical_Link: {
		dis_phy_link_cmd_parms_t *parms = (dis_phy_link_cmd_parms_t *)cmd->parms;
		dis_phy_link_evt_parms_t evt_parms;
		bool completed;
		int plidx = 0;

		do {
			/* find target physical link */
			status = wlc_bta_pllookup(bta, parms->plh, &plidx);

		} while (FALSE);

		/* generate Command Status event */
		wlc_bta_cmd_status_event(bta, op, status);
		if (status != HCI_SUCCESS)
			break;

		completed = (bta->pl[plidx].flags & BTA_PL_COMPLETE) ? TRUE : FALSE;

		/* tear down the physical link */
		wlc_bta_disconnect_physical_link(bta, plidx);

		/* generate Disconnect Physical Link Complete event */
		evt_parms.status = status;
		evt_parms.plh = parms->plh;
		evt_parms.reason = parms->reason;
		wlc_bta_phy_link_complete_event(bta, (uint8 *)&evt_parms, sizeof(evt_parms), TRUE);

		if (!completed) {
			phy_link_evt_parms_t evt_parms;

			/* generate Physical Link Complete event with error code 0x2 */
			evt_parms.status = HCI_ERR_NO_CONNECTION;
			evt_parms.plh = parms->plh;
			wlc_bta_phy_link_complete_event(bta, (uint8 *)&evt_parms, sizeof(evt_parms),
			                                FALSE);
		}
		break;
	}

	case HCI_Accept_Logical_Link:
	case HCI_Create_Logical_Link: {
		log_link_cmd_parms_t *cmdparms = (log_link_cmd_parms_t *)cmd->parms;
		amp_hci_event_t *evt;
		log_link_evt_parms_t *evtparms;
		uint16 llidx = 0;
		int plidx=0;
		uint32 sdu_ia_time;

		do {
			ext_flow_spec_t *txflow, *rxflow;
			bta_pl_t *phy_link;

WL_BTA(("wl%d: Create logical link for plh %d plidx %d\n", wlc->pub.unit, cmdparms->plh, plidx));

			/* validate physical link */
			status = wlc_bta_pllookup(bta, cmdparms->plh, &plidx);
			if (status != HCI_SUCCESS)
				break;

			if (!(bta->pl[plidx].flags & BTA_PL_COMPLETE)) {
				status = HCI_ERR_NO_CONNECTION;
				break;
			}

			/* allocate logical link */
			for (llidx = 0; llidx < BTA_MAXLOGLINKS; llidx++) {
				if (bta->ll[llidx].plh == 0) {
					status = HCI_SUCCESS;
					break;
				}
			}
			if (llidx == BTA_MAXLOGLINKS) {
				status = HCI_ERR_MAX_NUM_OF_CONNECTIONS;
				break;
			}

			bzero(&bta->ll[llidx], sizeof(bta_ll_t));

			/* update UP */
			txflow = (ext_flow_spec_t *)cmdparms->txflow;
			rxflow = (ext_flow_spec_t *)cmdparms->rxflow;

			if (txflow->service_type == EFS_SVCTYPE_GUARANTEED &&
			    rxflow->service_type == EFS_SVCTYPE_GUARANTEED) {
				if (bta->flags & BTA_FLAGS_NO_QOS) {
					WL_BTA(("wl%d: No QoS: rejecting guaranteed logical link\n",
						wlc->pub.unit));

					status = HCI_ERR_QOS_REJECTED;
					break;
				} else {
					/* map to a non-Best Effort UP (Video for now) */
					bta->ll[llidx].prio = PRIO_8021D_VI;
					bta->ll[llidx].fto = ltoh32_ua(txflow->flush_timeout);
				}
			} else if (txflow->service_type == EFS_SVCTYPE_NO_TRAFFIC) {
				bta->ll[llidx].prio = PRIO_8021D_NONE;
			} else {
				/* Best Effort */
				bta->ll[llidx].prio = PRIO_8021D_BE;
				bta->ll[llidx].fto = BTA_INVFTO;
			}

			/* cache remote MAC address (for use as DA) */
			phy_link = &bta->pl[plidx];

			/* validate bandwidth request */
			sdu_ia_time = ltoh32_ua(txflow->sdu_ia_time);
			if (sdu_ia_time != 0xFFFFFFFF) {
				uint16 max_sdu;
				uint16 reqbw;

				/* 
				 * SDU inter-arrival time is in us, so bw is Mbps
				 * Multiply by 1000 to get to Kbps
				 */
				max_sdu = ltoh16_ua(txflow->max_sdu);
				ASSERT(sdu_ia_time != 0);
				reqbw = ((max_sdu * 8) * 1000)/sdu_ia_time;
				if ((phy_link->allocbw + reqbw) > 30000) {
					WL_BTA(("wl%d: Max bandwidth exceeded: rejecting "
					        "guaranteed logical link \n", wlc->pub.unit));

					status = HCI_ERR_QOS_REJECTED;
					break;
				} else {
					phy_link->allocbw += reqbw;
				}
			}

			/* cache physical link handle and tx flow spec identifier */
			bta->ll[llidx].plidx = (uint8)plidx;
			bta->ll[llidx].plh = bta->plh[plidx];
			bta->ll[llidx].tx_fs_ID = txflow->id;

			WL_BTA(("wl%d: logical link allocated: llh = %d prio = %d\n",
			        wlc->pub.unit, llidx, bta->ll[llidx].prio));

#ifdef WLBTAMP_HIGH_ONLY
			if (status == HCI_SUCCESS)
				wlc_bta_update_logical_link(bta, plidx, llidx);
#endif
		} while (FALSE);

		/* generate Command Status event */
		wlc_bta_cmd_status_event(bta, op, status);
		if (status != HCI_SUCCESS)
			break;

		/* generate Logical Link Complete event */
		if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Logical_Link_Complete,
		                sizeof(log_link_evt_parms_t))) == NULL) {
			WL_ERROR(("wl%d: wlc_bta_docmd: failed to create Logical Link Complete "
			          "event\n", wlc->pub.unit));
			break;
		}

		evtparms = (log_link_evt_parms_t *)evt->parms;
		evtparms->status = status;
		htol16_ua_store(llidx, (uint8 *)&evtparms->llh);
		evtparms->u.plh = bta->plh[plidx];

		wlc_bta_doevent(bta, evt);

		wlc_bta_free_hcievent(bta, evt);
		break;
	}

	case HCI_Disconnect_Logical_Link: {
		uint16 llh = ltoh16_ua(cmd->parms);
		amp_hci_event_t *evt;
		log_link_evt_parms_t *evtparms;

		do {
			status = wlc_bta_valid_logical_link(bta, llh);

		} while (FALSE);

		/* generate Command Status event */
		wlc_bta_cmd_status_event(bta, op, status);
		if (status != HCI_SUCCESS)
			break;

		/* disconnect logical link */
		wlc_bta_disconnect_logical_link(bta, llh);

		/* generate Disconnect Logical Link event */
		if ((evt = wlc_bta_alloc_hcievent(bta, HCI_Disconnect_Logical_Link_Complete,
		                sizeof(log_link_evt_parms_t))) == NULL) {
			WL_ERROR(("wl%d: wlc_bta_docmd: failed to create Disconnect Logical Link "
			          "event\n", bta->wlc->pub.unit));
			break;
		}

		evtparms = (log_link_evt_parms_t *)evt->parms;
		evtparms->status = HCI_SUCCESS;
		htol16_ua_store(llh, (uint8 *)&evtparms->llh);
		evtparms->u.reason = HCI_SUCCESS;

		wlc_bta_doevent(bta, evt);

		wlc_bta_free_hcievent(bta, evt);

		break;
	}

	case HCI_Logical_Link_Cancel: {
		log_link_cancel_cmd_parms_t *parms = (log_link_cancel_cmd_parms_t *)cmd->parms;
		uint16 llidx;

		do {
			/* sanity check: physical link handle cannot be zero */
			if (parms->plh == 0) {
				status = HCI_ERR_UNSUPPORTED_VALUE;
				break;
			}

			status = HCI_ERR_NO_CONNECTION;

			/* find logical link with desired tx flow spec ID */
			for (llidx = 0; llidx < BTA_MAXLOGLINKS; llidx++) {
				bta_ll_t *ll = &bta->ll[llidx];

				if ((ll->plh == parms->plh) &&
				    (ll->tx_fs_ID == parms->tx_fs_ID)) {
					status = HCI_ERR_CONNECTION_EXISTS;
					break;
				}
			}

		} while (FALSE);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&status, sizeof(status));
		break;
	}

	case HCI_Reset: {
		int plidx;

		status = HCI_SUCCESS;

		/* disconnect any physical links */
		for (plidx = 0; plidx < WLC_MAXBTA; plidx++) {
			if (bta->plh[plidx] == 0)
				continue;

			wlc_bta_disconnect_physical_link(bta, plidx);
		}

		/* restore parameters to their default values */
		wlc_bta_default(bta);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&status, sizeof(status));
		break;
	}

	case HCI_Read_Logical_Link_Accept_Timeout: {
		read_lla_ca_to_evt_parms_t evt_parms;

		evt_parms.status = HCI_SUCCESS;
		htol16_ua_store(bta->lla_to, (uint8 *)&evt_parms.timeout);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));
		break;
	}

	case HCI_Write_Logical_Link_Accept_Timeout: {
		uint16 timeout = ltoh16_ua(cmd->parms);

		/* check range */
		if (timeout >= 1 && timeout <= 0xb540) {
			bta->lla_to = timeout;
			status = HCI_SUCCESS;
		} else {
			status = HCI_ERR_PARAM_OUT_OF_RANGE;
		}

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&status, sizeof(status));
		break;
	}

	case HCI_Read_Buffer_Size: {
		uint8 rbs_pars[8];


		rbs_pars[0] = HCI_SUCCESS; /* status */
		htol16_ua_store(BTA_MTU, &rbs_pars[1]); /* HC ACL Data Packet Length */
		rbs_pars[3] = 0;  /* HC Synchronous Data Packet Length */
		htol16_ua_store(8, &rbs_pars[4]); /* HC Total Num ACL Data Packets */
		htol16_ua_store(0, &rbs_pars[6]); /* HC Total Num Synchronous Data Packets */

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, rbs_pars, sizeof(rbs_pars));
		break;
	}

	case HCI_Read_Data_Block_Size: {
		read_data_block_size_evt_parms_t evt_parms;


		evt_parms.status = HCI_SUCCESS;
		htol16_ua_store(BTA_MTU, (uint8 *)&evt_parms.ACL_pkt_len);
		htol16_ua_store(BTA_MTU, (uint8 *)&evt_parms.data_block_len);
		htol16_ua_store(BTA_MAXDATABLKS, (uint8 *)&evt_parms.data_block_num);

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&evt_parms, sizeof(evt_parms));
		break;
	}
#endif /* WLBTAMP_HIGH */

#ifdef WLBTAMP_LOW
	case HCI_Enhanced_Flush: {
		eflush_cmd_parms_t *cmdparms = (eflush_cmd_parms_t *)cmd->parms;
		uint16 llh = ltoh16_ua(cmdparms->llh);

		do {
			status = wlc_bta_valid_logical_link(bta, llh);

		} while (FALSE);

		/* generate Command Status event */
		wlc_bta_cmd_status_event(bta, op, status);
		if (status != HCI_SUCCESS)
			break;

		wlc_bta_flush_hcidata(bta, llh);

		/* generate Enhanced Flush Complete event */
		wlc_bta_flush_hcidata_complete_event(bta, llh);
		break;
	}
#endif /* WLBTAMP_LOW */

	default:
		WL_ERROR(("wl%d: < HCI Command: Unsupported(0x%x|0x%x)\n",
			bta->wlc->pub.unit, HCI_CMD_OGF(op), HCI_CMD_OCF(op)));

		status = HCI_ERR_ILLEGAL_COMMAND;

		/* generate Command Complete event */
		wlc_bta_cmd_complete_event(bta, op, (uint8 *)&status, sizeof(status));

		break;
	}

	return (int)status;
}

#ifdef WLBTAMP_HIGH_ONLY
int
_wlc_bta_docmd(bta_info_t *bta, uint8 *cmd_buf, uint cmd_len)
{
	amp_hci_cmd_t *cmd = (amp_hci_cmd_t *)cmd_buf;
	wlc_info_t *wlc = bta->wlc;
	uint16 op = 0; /* HCI_NOP */
	int status = HCI_SUCCESS;
#ifdef VAL_HCI_CMD_LEN
	uint8 plen = 0;
	uint8 i;
#endif

#ifdef VAL_HCI_CMD_LEN
	/* validate cmd buf length */
	if (cmd_len < HCI_CMD_PREAMBLE_SIZE) {
		WL_ERROR(("wl%d: _wlc_bta_docmd: command buffer is too short\n",
		          wlc->pub.unit));
		status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
		goto fmt_err;
	}
#endif

	op = ltoh16_ua((uint8 *)&cmd->opcode);

#ifdef VAL_HCI_CMD_LEN
	plen = cmd->plen;

	/* validate cmd buf parm length */
	if (cmd_len < HCI_CMD_PREAMBLE_SIZE + plen) {
		WL_ERROR(("wl%d: _wlc_bta_docmd: command buffer length and parms length mismatch\n",
		          wlc->pub.unit));
		status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
		goto fmt_err;
	}
#endif

#if defined(BCMDBG) || defined(WLMSG_BTA)
	if (WL_BTA_ON())
		wlc_bta_hcidump_cmd(bta, cmd);
#endif

#ifdef VAL_HCI_CMD_LEN
	/* validate minimum cmd parm length */
	for (i = 0; i < ARRAYSIZE(bta_cmd_min_plen); i ++) {
		if (bta_cmd_min_plen[i].op_code == op &&
		    bta_cmd_min_plen[i].min_plen > plen) {
			WL_ERROR(("wl%d: _wlc_bta_docmd: command parms is too short\n",
			          wlc->pub.unit));
			status = HCI_ERR_ILLEGAL_PARAMETER_FMT;
			goto fmt_err;
		}
	}

fmt_err:
	/* generate Command Status event */
	if (status != HCI_SUCCESS) {
		wlc_bta_cmd_status_event(bta, op, status);
		return;
	}
#endif /* VAL_HCI_CMD_LEN */

	switch (op) {
	case HCI_Read_Local_AMP_Info:
	case HCI_Read_Data_Block_Size:
	case HCI_Read_Connection_Accept_Timeout:
	case HCI_Write_Connection_Accept_Timeout:
	case HCI_Read_Logical_Link_Accept_Timeout:
	case HCI_Write_Logical_Link_Accept_Timeout:
	case HCI_Read_Link_Supervision_Timeout:
	case HCI_Write_Link_Supervision_Timeout:
	case HCI_Read_Buffer_Size:
	case HCI_Logical_Link_Cancel:
	case HCI_Read_Best_Effort_Flush_Timeout:
	case HCI_Write_Best_Effort_Flush_Timeout:
		status = wlc_bta_docmd(bta, cmd_buf, cmd_len);
		break;
	case HCI_Write_Remote_AMP_ASSOC:
	case HCI_Accept_Logical_Link:
	case HCI_Create_Logical_Link:
	case HCI_Create_Physical_Link:
	case HCI_Accept_Physical_Link_Request:
	case HCI_Read_Local_AMP_ASSOC:
	case HCI_Read_Link_Quality:
	case HCI_Short_Range_Mode:
	case HCI_Disconnect_Physical_Link:
	case HCI_Disconnect_Logical_Link:
	case HCI_Reset:
		status = wl_bta_cmd_async(wlc->wl, cmd_buf, cmd_len);
		break;
	default:
		WL_TRACE(("%s:HIGH Driver:Unsupported HCI command(0x%x|0x%x)\n",
		          __FUNCTION__, HCI_CMD_OGF(op), HCI_CMD_OCF(op)));

		status = BCME_UNFINISHED;
		break;
	}

	return status;
}

void
wlc_bta_cs_cb(bta_info_t *bta, uint8 plidx, chanspec_t chsp, uint8 numpls)
{
	/* if creator, update local AMP_ASSOC */
	if (bta->chanspec_sel != chsp) {
		bta_pl_t *phy_link;
		uint8 status;

		phy_link = &bta->pl[plidx];
		status = wlc_bta_create_local_AMP_ASSOC(bta, chsp, &phy_link->local, &phy_link->llen);
		if (status != HCI_SUCCESS) {
			WL_ERROR(("%s: wlc_bta_doconnect: fail to build local AMP_ASSOC for "
					  "physical link %d\n", __FUNCTION__, bta->plh[plidx]));
		}
	}

	bta->chanspec_sel = chsp;
}
#endif	/* BWLBTAMP_HIGH_ONLY */

static void
wlc_bta_evtfwd_upd(bta_info_t *bta)
{
#ifdef WLBTAMP_LOW
	/* turn on/off event forwarding */
	if (bta->wlc->eventq != NULL)
		wlc_eventq_set_ind(bta->wlc->eventq, WLC_E_BTA_HCI_EVENT,
		                   (bta->flags & BTA_FLAGS_ET_RX) != 0);
#endif
}

static void
wlc_bta_flags_upd(bta_info_t *bta)
{
}

#if defined(BCMDBG) && !defined(WLBTAMP_HIGH_ONLY)
static int
wlc_bta_dump(bta_info_t *bta, struct bcmstrbuf *b)
{
	int pl, ll;

	bcm_bprintf(b, "numpls: %u lla_to: %d ca_to: %d chanspec_sel: 0x%04x\n",
	            bta->numpls, bta->lla_to, bta->ca_to, bta->chanspec_sel);
	bcm_bprintf(b, "pending: %u complete: %u\n",
	            bta->datablks_pending, bta->datablks_complete);
	for (pl = 0; pl < WLC_MAXBTA; pl ++) {
		if (bta->plh[pl] == 0)
			continue;
		bcm_bprintf(b, "plh: %u plidx: %d\n",
		            bta->plh[pl], pl);
		bcm_bprintf(b, "scb: %p flags: 0x%02x ca_ts: %u short_range: %d\n",
		            bta->pl[pl].scb, bta->pl[pl].flags, bta->pl[pl].ca_ts,
		            bta->pl[pl].short_range);
		bcm_bprintf(b, "ls_to: %d used: %d\n",
		            bta->pl[pl].ls_to, bta->pl[pl].used);
	}
	for (ll = 0; ll < BTA_MAXLOGLINKS; ll ++) {
		if (bta->ll[ll].plh == 0)
			continue;
		bcm_bprintf(b, "llh: %u plh: %d plidx: %d\n",
		            ll, bta->ll[ll].plh, bta->ll[ll].plidx);
		bcm_bprintf(b, "prio: %u complete: %u\n",
		            bta->ll[ll].prio, bta->ll[ll].datablks_complete);
	}

	return 0;
}
#endif 
