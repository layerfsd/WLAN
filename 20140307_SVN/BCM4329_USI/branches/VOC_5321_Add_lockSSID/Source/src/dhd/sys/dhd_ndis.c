/*
 * Broadcom Dongle Host Driver (DHD), NDIS-specific network interface
 * Basically selected code segments from usb-cdc.c and usb-rndis.c
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: dhd_ndis.c,v 1.35.4.6.2.9.4.98 2010/06/11 19:24:48 Exp $
 */
#if  defined(EXT_STA) || defined(DHD_NDIS_OID) || defined(WLBTAMP_HIGH_ONLY)
#include <wlc_cfg.h>
#endif /* EXT_STA || DHD_NDIS_OID || WLBTAMP_HIGH_ONLY */

#include <typedefs.h>
#include <osl.h>

#include <epivers.h>
#include <bcmutils.h>

#include <wlioctl.h>

#include <bcmendian.h>

#include <proto/ethernet.h>
#include <dngl_stats.h>
#include <dhd.h>
#include <dhd_bus.h>
#include <dhd_proto.h>
#include <dhd_dbg.h>
#include <siutils.h>
#include <bcmendian.h>
#include <sbconfig.h>
#include <sbpcmcia.h>
#include <proto/802.11.h>
#include <proto/802.1d.h>
#include <proto/bcmip.h>
#include <sbhndpio.h>
#include <sbhnddma.h>
/* Not needed for dongle/XP and dongle/CE */
#if defined(EXT_STA) || defined(DHD_NDIS_OID)
#include <hnddma.h>
#include <d11.h>
#include <wlc_rate.h>
#include <wlc_key.h>
#include <wlc_channel.h>
#include <wlc_pub.h>
#include <wlc_bsscfg.h>
#include <wlc_pio.h>
#include <wlc.h>
#include <wlc_phy.h>
#include <wlc_ethereal.h>
#include <wlc_wowl.h>
#include <wl_oid.h>
#include <wl_export.h>
#else
#ifdef WLBTAMP_HIGH_ONLY
#include <wlc_pub.h>
#endif
#endif /* EXT_STA || DHD_NDIS_OID */

#if defined(NDIS60)
#include <oidencap.h>
#include <ndis.h>
#endif /* NDIS60 */
#include <wl_ndis.h>
#include <wl_ndconfig.h>
#include <ndiserrmap.h>
#include <proto/802.1d.h>

#include <halether.h>
#include <bldver.h>
#if !defined(NDIS60)
#include <MsgQueueUtil.h>
#endif /* !NDIS60 */


#ifdef WLBTAMP
#include <proto/bt_amp_hci.h>
#include <proto/802.11_bta.h>
#include <dhd_bta.h>
#if defined(WLBTWUSB)
#include <bt_int.h>
#elif defined(WLBTCEAMP11)
#include <btceamp11.h>
#endif
#ifdef WLBTAMP_HIGH_ONLY
#include <wlc_bta.h>
#endif
#endif /* WLBTAMP */
#if defined(NDIS60)
#define QLEN		256	/* bulk rx and tx queue lengths */
#else
#define QLEN		128	/* bulk rx and tx queue lengths */
#endif /* UNDER_CE && NDIS60 */
#define PRIOMASK	7
#define CE_THREAD_PRIORITY_MAX 	255
#define CE_THREAD_PRIORITY_MIN 	0

#if defined(NDIS60) || defined(WLBTAMP_HIGH_ONLY)
typedef enum _workitem_state {
	WORKITEM_STATE_FREE = 0,
	WORKITEM_STATE_BUSY = 1
} workitem_state;
#endif /* NDIS60 */

#define IFNAMSIZ	16

/* Interface control information */
typedef struct dhd_if {
	struct dhd_info *info;			/* back pointer to dhd_info */
	/* OS/stack specifics */
	int 			idx;			/* iface idx in dongle */
	int 			state;			/* interface state */
	char			name[IFNAMSIZ+1]; /*  interface name */
	uint8		mac_addr[ETHER_ADDR_LEN];	/* assigned MAC address */
	void			*msgque;			/* CE Msgqueue */
	uint8			bssidx;			/* bsscfg index for users */
} dhd_if_t;

#if !defined(NDIS60)
/* These are "well known" queue names shared with user applications */
typedef struct dhd_sendup {
	void *msgqueue_handle;		/* msg queue handle */
	char *msgqueue_name;		/* msg queue name */
} dhd_sendup_t;
dhd_sendup_t dhd_sendup_info[2] =
{{ 0, BCMSDD_PRI_QUEUE},
{ 0, BCMSDD_VIF_QUEUE}};

#endif /* UNDER_CE && !NDIS60 */

/* Local private structure (extension of pub) */
typedef struct dhd_info {
	dhd_pub_t pub;

	/* OS/stack specifics */
	CRITICAL_SECTION proto_sem;
	HANDLE ioctl_resp_wait;
	/*
	NDIS_EVENT chip_active_wait;
	*/
	NDIS_EVENT wifioff_event;
	NDIS_EVENT sendup_event;
	HANDLE sendup_handle;		/* sendup thread handle */
	uint sendup_thread_id;		/* sendup thread id */
	struct pktq	rxq;
	dhd_if_t *iflist[DHD_MAX_IFS];

	NDIS_MINIPORT_TIMER h_wd_timer;
	bool wd_timer_valid;

	/* holds the pointer to wl structure created in wl_ndis.c */
	void * wl;

#if defined(NDIS60) || defined(WLBTAMP_HIGH_ONLY)
	/* To handle asynchronous request */
	workitem_state asyncReqWorkItemState;
	NDIS_WORK_ITEM asyncReqWorkItem;
	NDIS_SPIN_LOCK asyncReqLock;
	LIST_ENTRY asyncReqList;
#endif /* NDIS60 */
	NDIS_EVENT host_event;
	/* NDIS_EVENT ctrlpkt_event; */
	HANDLE event_process_handle;		/* sendup thread handle */
	uint event_process_thread_id;		/* sendup thread id */
	struct pktq	eventq;
} dhd_info_t;

extern void sdstd_status_intr_enable(void *sdh, bool enable);
extern void dhdsdio_isr(void *arg);
extern int bcmsdh_probe(shared_info_t *sh, void* bar, void** regsva, uint irq,
                        void ** dhd, void * wl);
extern void bcmsdh_remove(osl_t *osh, void *instance, shared_info_t *sh);
extern bool bcmsdh_reenumerate(void *sdh);

extern void wl_process_mid_txq(wl_info_t *wl);

#if defined(EXT_STA) || defined(WLBTAMP_HIGH_ONLY)
static void wl_timer(PVOID systemspecific1, NDIS_HANDLE context,
        PVOID systemspecific2, PVOID systemspecific3);
static NDIS_STATUS wl_ndis_reassoc(wl_info_t *wl);
extern void register_ext_sta_events(char *eventmask);
#endif

static int dhd_open(dhd_info_t *dhd, wl_info_t *wl);
static void dhd_dpc(void *data);
extern void shared_dpc_reschedule(PVOID systemspecific1,
	NDIS_HANDLE context, PVOID systemspecific2, PVOID systemspecific3);
/* IOCTL response timeout */
static void dhd_sendup_flush(dhd_pub_t *dhdp);

unsigned long dhd_ioctl_timeout_msec = IOCTL_RESP_TIMEOUT;
unsigned long dhd_chipactive_timeout_msec = IOCTL_CHIP_ACTIVE_TIMEOUT;

/* Watchdog interval */
uint dhd_watchdog_ms = 250;

#ifdef DHD_DEBUG
/* Console poll interval */
uint dhd_console_ms = 0;
#endif /* DHD_DEBUG */

/* The following are specific to the SDIO dongle */

/* Idle timeout for backplane clock */
int dhd_idletime = DHD_IDLETIME_TICKS;

/* Use polling */
uint dhd_poll = FALSE;

/* Use interrupts */
uint dhd_intr = TRUE;

/* ARP offload agent mode */
uint dhd_arp_mode = 0;

/* ARP offload enable */
uint dhd_arp_enable = FALSE;

/* Pkt filte enable control */
uint dhd_pkt_filter_enable = FALSE;

/*  Pkt filter init setup */
uint dhd_pkt_filter_init = 1;

/* Pkt filter mode control */
uint dhd_master_mode = TRUE;

/* Contorl fw roaming. 0 enables roam, 1 disables roam */
uint dhd_roam = 0;

/* Control radio state */
uint dhd_radio_up = 0;

/* SDIO Drive Strength (in milliamps) */
uint dhd_sdiod_drive_strength = 6;

uint wl_msg_level = DHD_ERROR_VAL;
uint wl_msg_level2 = 0;

/* Tx/Rx bounds */
extern uint dhd_txbound;
extern uint dhd_rxbound;


#ifdef SDTEST
/* Echo packet generator (pkts/s) */
uint dhd_pktgen = 0;

/* Echo packet len (0 => sawtooth, max 2040) */
uint dhd_pktgen_len = 0;
#endif

/* Version string to report */
#ifdef BCMDBG
#define DHD_COMPILED "\nCompiled in " SRCBASE
#else
#define DHD_COMPILED
#endif

#define dhd_bus_dhd(bus) (dhd_info_t *)dhd_bus_pub(bus)


void
dhd_isr(bool * InterruptRecognized, bool * QueueMiniportHandleInterrupt, void *arg)
{
	/* Dynamically adjust current priority and restore */
#if !defined(NDIS60)
	int ori_prio;
#endif /* !NDIS60 */
	dhd_info_t *dhd_info = dhd_bus_dhd(arg);
	wl_info_t *wl = dhd_info->wl;
	/*
	 * Since dhd_dpc is called in the context of ISR
	 * we don't need to schedule DPC in case of CE
	 */
	*QueueMiniportHandleInterrupt = FALSE;

	*InterruptRecognized = TRUE;

	dhdsdio_isr(arg);

	/*
	 * Call the DPC within the ISR in case of CE to prevent
	 * MSFT SDIO stack from doing few extra SDIO access to
	 * read interrupt status and enable/disable interrupts
	 * and also to avoid the scheduling delay
	 */
#if !defined(NDIS60)
	ori_prio = CeGetThreadPriority(GetCurrentThread());
	CeSetThreadPriority(GetCurrentThread(), wl->sh.rxdpc_prio);
#endif /* !NDIS60 */
	dhd_dpc(dhd_info);
#if !defined(NDIS60)
	CeSetThreadPriority(GetCurrentThread(), ori_prio);
#endif /* !NDIS60 */
}

uint
dhd_rxflow_mode(dhd_pub_t *dhdp, uint mode, bool set)
{
	dhd_info_t *dhd_info = (dhd_info_t *) dhdp;
	wl_info_t *wl = (wl_info_t *)dhd_info->wl;
	if (set)
		wl->sh.rxflowMode = mode;
	return wl->sh.rxflowMode;
}

void
dhd_rx_flow(void *dhdp, bool enable)
{
	dhd_info_t *dhd_info = (dhd_info_t *) dhdp;
	wl_info_t *wl = (wl_info_t *)dhd_info->wl;

	if (enable) {
		dhd_thread_priority(dhdp, DHD_RXDPC,
			&wl->sh.RxflowRxDPCPriority, TRUE);
		dhd_thread_priority(dhdp, DHD_SENDUP,
			&wl->sh.RxflowRXThreadPriority, TRUE);
	} else {
		dhd_thread_priority(dhdp, DHD_RXDPC,
			&wl->sh.RxDPCPriority, TRUE);
		dhd_thread_priority(dhdp, DHD_SENDUP,
			&wl->sh.RXThreadPriority, TRUE);
	}
}

void
dhd_sendup_indicate(dhd_pub_t *dhdp)
{
	dhd_info_t *dhd_info = (dhd_info_t *) dhdp;
	/* Signal the sendup thread */
	NdisSetEvent(&dhd_info->sendup_event);
}

static void
dhd_sendup(void *h)
{
	dhd_info_t * dhd = (dhd_info_t *)h;
	wl_info_t * wl = (wl_info_t *)dhd->wl;

	/* Read the priority from registry */
	CeSetThreadPriority(GetCurrentThread(), wl->sh.sendup_prio);
	DHD_ERROR(("RX thread priority = %d\n", wl->sh.sendup_prio));

	while (TRUE) {
		NdisWaitEvent(&dhd->sendup_event, 0);	/* wait forever */
		NdisResetEvent(&dhd->sendup_event);		/* reset the event */
		DHD_TRACE(("%s: Enter\n", __FUNCTION__));

		dhd_sendup_flush((dhd_pub_t *)h);
	}
}

/* Use lock if the caller not already using lock */
static void
dhd_sendup_flush(dhd_pub_t *dhdp)
{
	dhd_info_t *dhd_info = (dhd_info_t *) dhdp;
	uint prec_out, num;
	void *pkt, *start_pkt, *pnext;

	num = prec_out = 0;
	pkt = start_pkt = pnext = NULL;

	dhd_os_sdlock_sndup_rxq(dhdp);
	start_pkt = pkt = pktq_deq(&dhd_info->rxq, &prec_out);

	while (pkt) {
		pnext = pktq_deq(&dhd_info->rxq, &prec_out);
		num++;

		if (!pnext)
			break;

		PKTSETNEXT(wl->sh.osh, pkt, pnext);
		pkt = pnext;
	}
	dhd_os_sdunlock_sndup_rxq(dhdp);
#if defined(P2P) && !defined(NDIS60)
		{
			void *a_start_pkt = NULL;
			void *a_pcur = NULL;
			int a_num = 0;
			void *plast = NULL;

			int total_num = num; /* debug */
			void *old_start_pkt = start_pkt; /* debug */

			pkt = start_pkt;

			/* Make a pass thru packet chain and pull out any packets for the */
			/* virtual interface. Place them on a alternate list and indicate them */
			/* separate from the primary interface. */
			while (pkt) {
				pnext = PKTNEXT(wl->sh.osh, pkt);

				/* Check if for the virtual interface */
				if (((struct lbuf *)pkt)->ifidx) {

					/* unlink from old list */

					if (plast) {
						PKTSETNEXT(wl->sh.osh,
							plast,  PKTNEXT(wl->sh.osh, pkt));
					}

					PKTSETNEXT(wl->sh.osh, pkt,  NULL);

					if (start_pkt == pkt)
						start_pkt = pnext;

					/* link into new list */

					if (a_start_pkt) {
						PKTSETNEXT(wl->sh.osh, a_pcur,  pkt);
						a_pcur = pkt;
					} else {
						a_start_pkt = a_pcur = pkt;
					}
					ASSERT(num);
					num--;
					a_num++;
				}

				plast = pkt;
				pkt = pnext;
			}

			ASSERT(a_num + num == total_num);
			ASSERT(a_start_pkt == old_start_pkt || start_pkt == old_start_pkt);

			ASSERT(!total_num || a_start_pkt != start_pkt);
			ASSERT(!total_num || a_start_pkt || start_pkt);

			if (a_num)
				wl_sendup(dhd_info->wl, (void *) 1, a_start_pkt, a_num);

}
#endif /* P2P */
	if (num)
		wl_sendup(dhd_info->wl, NULL, start_pkt, num);
}

uint32
dhd_rx_flow_hilimit(dhd_pub_t *dhdp, uint32 val)
{
	dhd_info_t *dhd = (dhd_info_t *) dhdp;
	wl_info_t *wl = dhd->wl;

	if (val)
		wl->sh.rxflowHi = val;

	return wl->sh.rxflowHi;
}

uint32
dhd_rx_flow_lowlimit(dhd_pub_t *dhdp, uint32 val)
{
	dhd_info_t *dhd = (dhd_info_t *) dhdp;
	wl_info_t *wl = dhd->wl;

	if (val)
		wl->sh.rxflowLow = val;

	return wl->sh.rxflowLow;
}

void
dhd_thread_priority(dhd_pub_t *dhdp, uint thread, uint *prio, bool set)
{
	dhd_info_t *dhd = (dhd_info_t *) dhdp;
	wl_info_t *wl = dhd->wl;
	if (set) {
		if ((*prio >= CE_THREAD_PRIORITY_MIN) && (*prio <= CE_THREAD_PRIORITY_MAX)) {
			switch (thread) {
				case (DHD_DPC):
					wl->sh.dpc_prio = *prio;
					CeSetThreadPriority(wl->sh.dpc_handle,
						wl->sh.dpc_prio);
					break;
				case (DHD_RXDPC):
					wl->sh.rxdpc_prio = *prio;
					break;
				case (DHD_SENDUP):
					wl->sh.sendup_prio = *prio;
					CeSetThreadPriority(dhd->sendup_handle,
						wl->sh.sendup_prio);
					break;
				case (DHD_WATCHDOG):
					wl->sh.wd_prio = *prio;
					break;
			}
		}
	} else {
		switch (thread) {
			case (DHD_DPC):
				*prio = wl->sh.dpc_prio;
				break;
			case (DHD_RXDPC):
				*prio = wl->sh.rxdpc_prio;
				break;
			case (DHD_SENDUP):
				*prio  = wl->sh.sendup_prio;
				break;
			case (DHD_WATCHDOG):
				*prio = wl->sh.wd_prio;
				break;
		}
	}
}

void
dhd_thread_quantum(dhd_pub_t *dhdp, uint thread, uint *tq, bool set)
{
	dhd_info_t *dhd = (dhd_info_t *) dhdp;
	wl_info_t *wl = dhd->wl;
	if (set) {
		switch (thread) {
			case (DHD_DPC):
				if (!CeSetThreadQuantum(wl->sh.dpc_handle, (DWORD)*tq))
					DHD_ERROR(("%s: set dpc failed\n", __FUNCTION__));
				break;
			case (DHD_SENDUP):
				if (!CeSetThreadQuantum(dhd->sendup_handle, (DWORD)*tq))
					DHD_ERROR(("%s: set sendup failed\n", __FUNCTION__));
				break;
		}
	} else {
		switch (thread) {
			case (DHD_DPC):
				*tq = (uint) CeGetThreadQuantum(wl->sh.dpc_handle);
				break;
			case (DHD_SENDUP):
				*tq = (uint) CeGetThreadQuantum(dhd->sendup_handle);
				break;
		}
	}
}

static void
dhd_event_process(void *h)
{
	dhd_info_t * dhd = (dhd_info_t *)h;
	void *data, *pkt = 0;
	uint prec_out, num = 0;
	wl_info_t * wl = (wl_info_t *)dhd->wl;

	wl_event_msg_t event;
	dhd_pub_t *dhdp;
	int ifidx;
	int bcmerror;
	struct ether_header *eh;
	dhdp = (dhd_pub_t *)h;

	/* Read the priority from registry */
	CeSetThreadPriority(GetCurrentThread(), wl->sh.EventPriority);
	DHD_TRACE(("dhd_event_process thread priority = %d\n",
		CeGetThreadPriority(GetCurrentThread())));

	while (TRUE) {
		DHD_TRACE(("%s - Waiting\n", __FUNCTION__));

		NdisWaitEvent(&dhd->host_event, 0);	/* wait forever */
		NdisResetEvent(&dhd->host_event);		/* reset the event */

		DHD_TRACE(("%s - Sending up into the NDIS stack\n", __FUNCTION__));

		num = 0;
		do {
			dhd_os_sdlock((dhd_pub_t *)dhd);
			DHD_TRACE(("%s: DEQ eventq %d\n",
				__FUNCTION__, pktq_mlen(&dhd->eventq, 8)));
			pkt = pktq_deq(&dhd->eventq, &prec_out);
			dhd_os_sdunlock((dhd_pub_t *)dhd);

			if (pkt) {
				PKTSETNEXT(wl->sh.osh, pkt, NULL);
				eh = (struct ether_header *)PKTDATA(dhdp->osh, pkt);
				if (eh) {
					bcmerror = wl_host_event(dhd, &ifidx,
						(uint8 *)eh, &event, &data);
					wl_event_to_host_order(&event);
					if (bcmerror == BCME_OK) {
						wl_ndisevent(wl, &event, (void *)data);
					}
				}
			} else
				break;

			ND_RXLOCK(wl);
			PKTFREE(dhdp->osh, pkt, FALSE);
			ND_RXUNLOCK(wl);
		} while (TRUE);
	}
}


int
dhd_probe(shared_info_t *sh, void * wl, void* bar, void** regsva, uint irq, void ** dhd)
{
	NDIS_STATUS status;
	int ret = -1;
	void * bus;
	dhd_info_t *dhd_info;

	if (NULL == sh)
		goto err;

	/* Assign drive strength read from registry before calling bcmsdh_probe */
	if (sh->DrvStrength)
		dhd_sdiod_drive_strength = sh->DrvStrength;

	dhd_poll = sh->EnablePoll;
	dhd_intr = !dhd_poll;
	DHD_ERROR(("Driver is opertating in %s mode\n", dhd_intr ? "Interrupt" : "Poll"));

	ret = dhd_bus_register();
	if (-1 == ret)
		goto err;

	ret = bcmsdh_probe(sh, bar, regsva, irq, dhd, wl);
	if (-1 == ret)
		goto err;

	bus = *dhd;
	dhd_info = dhd_bus_dhd(bus);
	dhd_info->wl = wl;
#ifndef NDIS60
	/* register our ISR */
	status = shared_interrupt_register(0, 0,
		0, 0,
		0,
		0,
		0,
		sh, dhd_isr, bus, dhd_dpc, dhd_info);

	if (NDIS_ERROR(status)) {
		DHD_ERROR(("wl%d: NdisMRegisterInterrupt error 0x%x\n", sh->unit, status));
		goto err;
	}
#else /* !NDIS60 */
	status = shared_interrupt_register(0, sh, dhd_isr, bus, dhd_dpc, dhd_info);
	if (NDIS_ERROR(status)) {
		DHD_ERROR(("wl%d: NdisMRegisterInterrupt error 0x%x\n", sh->unit, status));
		goto err;
	}

#endif /* !NDIS60 */
	/* Event for signaling the completion of turning off WiFi */
	NdisInitializeEvent(&dhd_info->wifioff_event);
	NdisResetEvent(&dhd_info->wifioff_event);

	NdisInitializeEvent(&dhd_info->sendup_event);
	NdisResetEvent(&dhd_info->sendup_event);
	pktq_init(&dhd_info->rxq, (PRIOMASK+1), QLEN);

	dhd_info->sendup_handle = CreateThread(NULL,		/* security atributes */
		0,		/* initial stack size */
		(LPTHREAD_START_ROUTINE)dhd_sendup,	/* Main() function */
		dhd_info,		/* arg to reader thread */
		0,		/* creation flags */
		&dhd_info->sendup_thread_id); /* returned thread id */
	if (!dhd_info->sendup_handle)
		return NDIS_STATUS_FAILURE;
#if defined(EXT_STA)
	pktq_init(&dhd_info->eventq, (PRIOMASK+1), QLEN);
	NdisInitializeEvent(&dhd_info->host_event);
	NdisResetEvent(&dhd_info->host_event);
	/* NdisInitializeEvent(&dhd_info->ctrlpkt_event);
	NdisResetEvent(&dhd_info->ctrlpkt_event);
	*/

	dhd_info->event_process_handle = CreateThread(NULL,		/* security atributes */
			0,		/* initial stack size */
			(LPTHREAD_START_ROUTINE)dhd_event_process, /* Main() function */
			dhd_info,		/* arg to reader thread */
			0,		/* creation flags */
			&dhd_info->event_process_thread_id); /* returned thread id */
	if (!dhd_info->event_process_handle)
		return NDIS_STATUS_FAILURE;
	DHD_ERROR(("event thread id 0x%x", dhd_info->event_process_handle));
#endif /* EXT_STA */
	sh->attached = TRUE;
	sh->dhd = dhd_info;
	if (dhd_open(dhd_info, (wl_info_t *)wl)) {
		DHD_ERROR(("%s: dhd_open failed\n", __FUNCTION__));
		ret = -1;
		goto err;
	}

err:
	return ret;
}

void
dhd_remove(osl_t *osh, void *instance, shared_info_t *sh)
{
#if !defined(NDIS60)
	shared_interrupt_deregister(sh->intr, sh);
#endif /* NDIS60 */

	bcmsdh_remove(osh, instance, sh);

	dhd_bus_unregister();
}

#if defined(NDIS51) || defined(NDIS60)
void
dhd_pnp_surp_remove(void *instance)
{
#if defined(BCMDHDUSB)
	dhdusb_pnp_surp_remove(instance);
#endif /* BCMDHDUSB */
}

void
dhd_pnp_postsleep(void *instance)
{
#if defined(BCMDHDUSB)
	dhdusb_pnp_postsleep(instance);
#endif /* BCMDHDUSB */
}
#endif /* NDIS51 || NDIS60  */

char *
dhd_ifname(dhd_pub_t *dhdp, int ifidx)
{
	dhd_info_t *dhd = (dhd_info_t *)dhdp;

	return "<no ifname>";
}

int
dhd_sendpkt(dhd_pub_t *dhdp, int ifidx, void *pktbuf)
{
	dhd_info_t *dhd = (dhd_info_t *)dhdp;
	int ret = 0;
	osl_t *osh = dhdp->osh;
	wl_info_t * wl = dhd->wl;

	DHD_TRACE(("%s: Enter\n", __FUNCTION__));

	/* Reject if down */
	if (!dhdp->up || (dhdp->busstate == DHD_BUS_DOWN)) {
		/* This can happen after PnP surprise remove event */
		dhd_txcomplete(dhdp, pktbuf, FALSE);
		PKTFREE(osh, pktbuf, TRUE);
		ret = -BCME_NOTUP;
		goto exit;
	}

	/* If the protocol uses a data header, apply it */
	dhd_prot_hdrpush(dhdp, ifidx, pktbuf);

	/* Use bus module to send data frame */
	ret = dhd_bus_txdata(dhdp->bus, pktbuf);

#if !defined(NDIS60)
	/* Since the TX is queued and really transmitted from DPC,
	   schedule DPC manually to start sending the packets
	*/
	shared_dpc_schedule(&wl->sh);
#endif /* !NDIS60 || UNDER_CE */

exit:
	if (ret)
		dhdp->dstats.tx_dropped++;
	else
		dhdp->tx_packets++;

	/* Return ok: we always eat the packet */
	return ret;
}

int
dhd_start_xmit(void *bus, int ifidx, void *pkt)
{
	dhd_pub_t *dhdp = dhd_bus_pub(bus);

	DHD_TRACE(("%s: Enter\n", __FUNCTION__));

	return dhd_sendpkt(dhdp, ifidx, pkt);
}

void
dhd_txflowcontrol(dhd_pub_t *dhdp, int ifidx, bool state)
{
	dhd_info_t * dhd_info;

	DHD_TRACE(("Flow control changed = %d for idx %d\n", state, ifidx));

	dhd_info =  (dhd_info_t *)dhdp;

	dhdp->txoff = state;

	wl_txflowcontrol((wl_info_t *)dhd_info->wl, state, ALLPRIO);
}

void
dhd_event(struct dhd_info *dhd, char *evpkt, int evlen, int ifidx)
{
	dhd_if_t *ifp;

	DHD_TRACE(("dhd_event: idx %d\n", ifidx));


	if (ifidx != 0 && !(ifp = dhd->iflist[ifidx]))
		return;


	dhd_vif_sendup(dhd, ifidx, evpkt, evlen);

}

void
dhd_vif_sendup(struct dhd_info *dhd, int ifidx, uchar *cp, int len)
{
#if !defined(NDIS60)
	DHD_TRACE(("dhd_vif_sendup: sending up %d bytes on ifidx %d\n", len, ifidx));

	/* "Well Known" queue names */
	if (ifidx == 0) {
		MsgQueueSend((HANDLE)dhd_sendup_info[0].msgqueue_handle, (LPVOID) cp, len);
	} else {
		/* Need to add info for disambiguation when we have multiple vifs */
		MsgQueueSend((HANDLE)dhd_sendup_info[1].msgqueue_handle, (LPVOID) cp, len);
	}
#endif /* UNDER_CE && !NDIS60 */
}

void
dhd_rx_frame(dhd_pub_t *dhdp, int ifidx, void *pktbuf, int numpkt)
{
	struct ether_header *eh;
	uint16 type;
#if !defined(EXT_STA)
	wl_event_msg_t event;
	void *data;
#endif
	void *pnext;
	wl_info_t *wl;
	uint len;
	int i;
	dhd_info_t * dhd_info;
#if !defined(EXT_STA)
	int bcmerror;
#endif /* !(EXT_STA && UNDER_CE) */
#ifdef DHD_NDIS_OID
	wlc_event_t wlc_e;
#endif /* DHD_NDIS_OID */
	dhd_info =  (dhd_info_t *)dhdp;
	wl = dhd_info->wl;

	DHD_TRACE(("%s: Enter\n", __FUNCTION__));

	for (i = 0; pktbuf && i < numpkt; i++, pktbuf = pnext) {
		pnext = PKTNEXT(dhdp->osh, pktbuf);
		PKTSETNEXT(dhdp->osh, pktbuf, NULL);

		len = PKTLEN(dhdp->osh, pktbuf);

		eh = (struct ether_header *)PKTDATA(dhdp->osh, pktbuf);
		type  = ntoh16(eh->ether_type);

		/* Get the protocol, maintain skb around eth_type_trans() */
		/* Process special event packets and then discard them */
		if (type == ETHER_TYPE_BRCM) {
#if defined(EXT_STA)
			/* Place the received packet into queue for later delivery to NDIS stack */
			/* Priority is not used, choose a fixed value so packets stay in order */
			if (ifidx == 0) {
				if (!pktq_full(&dhd_info->eventq)) {
					DHD_TRACE(("%s: ENQ eventq %d\n",
						__FUNCTION__, pktq_mlen(&dhd_info->eventq, 8)));
					pktq_penq(&dhd_info->eventq, PRIOMASK, pktbuf);
					NdisSetEvent(&dhd_info->host_event);
					continue;
				} else {
					DHD_ERROR(("%s: out of event queue\n", __FUNCTION__));
					ND_RXLOCK(wl);
					PKTFREE(dhdp->osh, pktbuf, FALSE);
					ND_RXUNLOCK(wl);
					break;
				}
			} else {
				DHD_ERROR(("%s: wrong ifidx %d!\n", __FUNCTION__,
					ifidx));
				return;
			}
#else /* EXT_STA && UNDER_CE */
			bcmerror = wl_host_event(dhd_info, &ifidx, (uint8 *)eh, &event, &data);
			wl_event_to_host_order(&event);
			if (bcmerror == BCME_OK) {
#ifdef WLBTAMP
				if (event.event_type == WLC_E_BTA_HCI_EVENT) {
#ifdef WLBTAMP_HIGH_ONLY
					wl_bta_evt_async(wl, data, event.datalen);
#else
					dhd_bta_doevt(dhdp, data, event.datalen);
#endif /* WLBTAMP_HIGH_ONLY */
				}
#endif /* WLBTAMP */
#ifdef DHD_NDIS_OID
				memcpy(&wlc_e.event, &event, sizeof(wl_event_msg_t));
				wl_oid_event(wl->oid, &wlc_e);
				memcpy(&event, &wlc_e.event, sizeof(wl_event_msg_t));
#endif /* DHD_NDIS_OID */
#ifdef P2P
				if (ifidx == 1) {
					wl->isVDev = TRUE;
					wl_ndisevent(wl, &event, (void *)data);
					wl->isVDev = FALSE;
				} else if (ifidx == 0)
#endif /* P2P */
				wl_ndisevent(wl, &event, (void *)data);
				ND_RXLOCK(wl);
				PKTFREE(dhdp->osh, pktbuf, FALSE);
				ND_RXUNLOCK(wl);
				continue;
			}
			else {
				DHD_ERROR(("%s: wl_host_event returned error %d\n", bcmerror));
			}
#endif /* EXT_STA && UNDER_CE */
		}

		dhdp->dstats.rx_bytes += len;
		dhdp->rx_packets++; /* Local count */

#ifdef EXT_STA
#define DNGL_RXCTXT_SIZE	45
		/*
		* sizeof(rx_ctxt_t), 48, is larger on the dongle than Vista (45)
		* Fixed for now and find cleaner solution later
		*/
		PKTSETLEN(dhdp->osh, pktbuf, len - DNGL_RXCTXT_SIZE);
#endif /* EXT_STA */

#ifdef WLBTAMP
		/* Forward HCI ACL data to BT stack */
		if ((type < ETHER_TYPE_MIN) && (len >= RFC1042_HDR_LEN)) {
			struct dot11_llc_snap_header *lsh =
			        (struct dot11_llc_snap_header *)&eh[1];

			if (bcmp(lsh, BT_SIG_SNAP_MPROT, DOT11_LLC_SNAP_HDR_LEN - 2) == 0) {
				if (ntoh16(lsh->type) == BTA_PROT_L2CAP) {
					amp_hci_ACL_data_t *ACL_data =
						(amp_hci_ACL_data_t *)&lsh[1];
#ifdef BCMDBG
					if (DHD_BTA_ON())
						dhd_bta_hcidump_ACL_data(dhdp, ACL_data, FALSE);
#endif
#if defined(WLBTWUSB) || defined(WLBTCEAMP11)
					BtKernForwardData(wl, (PVOID)ACL_data,
					                  (ULONG)len - RFC1042_HDR_LEN);
#endif
					ND_RXLOCK(wl);
					PKTFREE(dhdp->osh, pktbuf, FALSE);
					ND_RXUNLOCK(wl);
					continue;
				} else if (ntoh16(lsh->type) == BTA_PROT_SECURITY) {
					dhd_vif_sendup(dhd_info, ifidx, (uchar *) eh, len);
				}
			}
		}
#endif /* WLBTAMP */

		/* Place the received packet into queue for later delivery to NDIS stack */
		/* Priority is not used, choose a fixed value so packets stay in order */

		dhd_os_sdlock_sndup_rxq((dhd_pub_t *)dhd_info);
		((struct lbuf *)pktbuf)->ifidx = ifidx;
		if (!pktq_full(&dhd_info->rxq)) {
			pktq_penq(&dhd_info->rxq, PRIOMASK, pktbuf);
		} else {
			DHD_ERROR(("%s: out of rxq queue\n", __FUNCTION__));
			dhdp->dstats.rx_dropped++;
			ND_RXLOCK(wl);
			PKTFREE(dhdp->osh, pktbuf, FALSE);
			ND_RXUNLOCK(wl);
			dhd_os_sdunlock_sndup_rxq((dhd_pub_t *)dhd_info);
			break;
		}
		dhd_os_sdunlock_sndup_rxq((dhd_pub_t *)dhd_info);
	}
}

/* send up locally generated event */
void
dhd_sendup_event(dhd_pub_t *dhdp, wl_event_msg_t *event, void *data)
{
	dhd_info_t *dhd_info = (dhd_info_t *)dhdp;
	wl_info_t *wl = dhd_info->wl;

	wl_event_to_host_order(event);
	/*
	/* send the event up for processing */
	wl_ndisevent(wl, event, data);
}

void
dhd_txcomplete(dhd_pub_t *dhdp, void *txp, bool success)
{
	int ifidx;
#ifdef WLBTAMP
	struct ether_header *eh;
	uint16 type;
	uint len;
#endif

	dhd_prot_hdrpull(dhdp, &ifidx, txp);

#ifdef WLBTAMP
	/* Crack open the packet and check to see if it is BT HCI ACL data packet.
	 * If yes generate packet completion event.
	 */
	len = PKTLEN(dhdp->osh, txp);

	eh = (struct ether_header *)PKTDATA(dhdp->osh, txp);
	type  = ntoh16(eh->ether_type);

	/* Generate ACL data tx completion event locally to avoid SDIO bus transaction */
	if ((type < ETHER_TYPE_MIN) && (len >= RFC1042_HDR_LEN)) {
		struct dot11_llc_snap_header *lsh = (struct dot11_llc_snap_header *)&eh[1];

		if (bcmp(lsh, BT_SIG_SNAP_MPROT, DOT11_LLC_SNAP_HDR_LEN - 2) == 0 &&
		    ntoh16(lsh->type) == BTA_PROT_L2CAP) {

			dhd_bta_tx_hcidata_complete(dhdp, txp, success);
		}
	}
#endif /* WLBTAMP */
}

void dhd_watchdog(PVOID systemspecific1, NDIS_HANDLE context,
	PVOID systemspecific2, PVOID systemspecific3)
{
	dhd_info_t *dhd;
	wl_info_t * wl;

	if (context) {
		dhd = (dhd_info_t *)context;
		ASSERT(context);

		wl = (wl_info_t *)dhd->wl;
#if defined(NDIS60)
		if (wl->tx_mid_q_count != wl->txq_count) {
			DHD_INFO(("%s: kick midq timer, tx_mid_q_count %d txq_count %d\n",
				__FUNCTION__, wl->tx_mid_q_count, wl->txq_count));

			wl_add_timer(wl, wl->process_mid_txq_timer, 0, FALSE);
		}
#endif /* UNDER_CE && NDIS60 */

		if (dhd_bus_watchdog(&dhd->pub)) {
#ifdef DHD_DEBUG
			dhd->pub.wd_dpc_sched++;
#endif /* DHD_DEBUG */
			shared_dpc_reschedule(NULL, (NDIS_HANDLE)&wl->sh, NULL, NULL);
		}
		/* Count the tick for reference */
		dhd->pub.tickcnt++;
	}
}

int
dhd_ioctl_entry(dhd_info_t * dhd, int ifidx, char *pInBuf)
{
	dhd_ioctl_t * ioc;
	int bcmerror = 0;
	int len = 0;
	void * buf = NULL;
	wl_info_t *wl;
#if (CE_MAJOR_VER >= 0x0006)
#if !defined(EXT_STA)
	HRESULT status;
#endif
#endif /* CE_MAJOR_VER >= 0x0006 */

	wl = (wl_info_t *)dhd->wl;

	ioc = (dhd_ioctl_t *)pInBuf;

	if (ioc->buf) {
		len = MIN(ioc->len, DHD_IOCTL_MAXLEN);
#if (CE_MAJOR_VER <= 0x0005)
		/* When CETK 5.0 attempts unbinding adapter using ProtocolUnbindAdapter,
		* it tries to clear the multicast list (OID_802_3_MULTICAST_LIST)
		* in the miniport driver. It is observed that during this time,
		* MapCallerPtr returns a non-null, but incorrectly mapped pointer,
		* using which causes a crash in dhd. However, any number of usage of the same
		* OID (OID_802_3_MULTICAST_LIST) prior to this point succeeds without any problem.
		*
		* In case of OID_802_3_MULTICAST_LIST, an internal buffer (created in wl_iovar_op)
		* is used instead of external input buffer (received in wl_msetinformation).
		* Hence we can avoid MapCallerPtr call during this point, preventing the crash.
		* But we need to figure out the exact cause of MapCallerPtr behavior
		* during ProtocolUnbindAdapter under CE 5.0.
		*/
		if (GetCallerProcess() != GetOwnerProcess())
			buf = MapCallerPtr(ioc->buf, ioc->len);
		else
			buf = ioc->buf;
#else /* CE_MAJOR_VER <= 0x0005 */
#if !defined(NDIS60)
		status = CeOpenCallerBuffer(&buf, ioc->buf, ioc->len, ARG_IO_PTR, FALSE);
		if (status != S_OK) {
			DHD_ERROR(("BCMSDLDR: CeOpenCallerBuffer failed status = %d\n", status));
			bcmerror = -BCME_BADADDR;
			goto err;
		}
#else
	 /* CE6.0 driver running in kernel mode does not need to map buffer */
	buf = ioc->buf;
#endif /* NDIS60 */
#endif /* CE_MAJOR_VER <= 0x0005 */

		if (!buf) {
			DHD_ERROR(("BCMSDLDR: Error mapping the ioctl buffer\n"));
			bcmerror = -BCME_BADADDR;
			goto err;
		}
	}

	/* check for local dhd ioctl and handle it */
	if (ioc->driver == DHD_IOCTL_MAGIC) {
		bcmerror = dhd_ioctl(&dhd->pub, ioc, buf, len);
		if (bcmerror)
			dhd->pub.bcmerror = bcmerror;
		goto err;
	}

	/* send to dongle (must be up, and wl) */
	if ((dhd->pub.busstate != DHD_BUS_DATA)) {
		bcmerror = -BCME_NOTUP;
		goto err;
	}

	if (!dhd->pub.iswl) {
		bcmerror = -BCME_NOTUP;
		goto err;
	}

#if defined(EXT_STA)
	switch (ioc->cmd) {
	case OID_DOT11_CIPHER_KEY_MAPPING_KEY:
		/* Serialize M4 send and set key OID to prevent M4 encryption */
		wl_process_mid_txq(wl);
		break;
	default:
		break;
	}
#endif	/* BCMDONGLEHOST */
#ifndef P2P
	bcmerror = dhd_prot_ioctl(&dhd->pub, ifidx, (wl_ioctl_t *)ioc, buf, len);
#else
	bcmerror = dhd_prot_ioctl(&dhd->pub, wl->isVDev ? 1:0, (wl_ioctl_t *)ioc, buf, len);
#endif /* P2P */
	if (bcmerror) {
		dhd->pub.bcmerror = bcmerror;
		goto err;
	}
#if defined(EXT_STA)
	if (ioc->set) {

		switch (ioc->cmd) {
		case OID_DOT11_ENABLED_AUTHENTICATION_ALGORITHM:
			wl->oid->NDIS_auth = ((DOT11_AUTH_ALGORITHM_LIST *)buf)->AlgorithmIds[0];
			break;
		case OID_DOT11_DESIRED_BSS_TYPE:
			wl->oid->NDIS_infra =
				(*(DOT11_BSS_TYPE *)buf == dot11_BSS_type_infrastructure)?
				Ndis802_11Infrastructure : Ndis802_11IBSS;
			break;
		case OID_DOT11_RESET_REQUEST:
			wl->oid->NDIS_infra = Ndis802_11Infrastructure;
			break;

		case OID_DOT11_CIPHER_KEY_MAPPING_KEY:
			wl->pwk = 1;
			break;

		default:
			break;
		}
	}
#endif	/* BCMDONGLEHOST */

err:
	if (buf) {
#if (CE_MAJOR_VER <= 0x0005)
		if (GetCallerProcess() != GetOwnerProcess())
			UnMapPtr(buf);
#else /* CE_MAJOR_VER <= 0x0005 */
#if !defined(NDIS60)
		CeCloseCallerBuffer(buf, ioc->buf, ioc->len, ARG_IO_PTR);
#endif
#endif /* CE_MAJOR_VER <= 0x0005 */
	}

	if (bcmerror > BCME_LAST)
		return OSL_ERROR(bcmerror);
	else {
		/* Must be NDIS errors so pass to host */
		return bcmerror;
	}
}

static int
dhd_stop(dhd_info_t *dhd)
{
	bool canceled;

	DHD_TRACE(("%s: Enter\n", __FUNCTION__));

	/* Set state and stop OS transmissions */
	dhd->pub.up = 0;

	/* Stop the protocol module */
	dhd_prot_stop(&dhd->pub);

	/* Stop the bus module */
	dhd_bus_stop(dhd->pub.bus, TRUE);

	if (dhd->wd_timer_valid == TRUE)
		NdisMCancelTimer(&dhd->h_wd_timer, &canceled);

	return 0;
}

static char dongleimagepathkey[] = "XXXXDongleImagePath";
static char sromimagepathkey[] = "XXXXSROMImagePath";
static char pktfilterkey[] = "PKTFilterXX";
static ndis_config_t dhd_configs[] = {
	{ dongleimagepathkey, NdisParameterString, 0, 0, NULL },
	{ sromimagepathkey, NdisParameterString, 0, 0, NULL },
	{ pktfilterkey, NdisParameterString, 0, 0, NULL },
	{ "DHDMsgLevel", NdisParameterInteger, 0, 0, NULL },
	{ "WLMsgLevel", NdisParameterInteger, 0, 0, NULL },
	{ "DongleImagePath", NdisParameterString, 0, 0, NULL },
	{ "SROMImagePath", NdisParameterString, 0, 0, NULL },
	{ "EnableAOE", NdisParameterInteger, 0, 0, NULL },
	{ "AOEMode", NdisParameterInteger, 0, 0, NULL },
	{ "EnablePKTFilter", NdisParameterInteger, 0, 0, NULL },
	{ "PKTFilterMode", NdisParameterInteger, 0, 0, NULL },
	{ "EnableOOBInterrupt", NdisParameterInteger, 0, 0, NULL },
	{ "DongleConsoleMs", NdisParameterInteger, 0, 0, NULL },
	{ "DbgOutput", NdisParameterInteger, 0, 0, NULL },
	{ "PNPMethod", NdisParameterInteger, 0, 0, NULL },
	{ "WatchdogPeriod", NdisParameterInteger, 0, 0, NULL },
	{ "DPCPriority", NdisParameterInteger, 0, 0, NULL },
	{ "RXThreadPriority", NdisParameterInteger, 0, 0, NULL },
	{ "OidPriority", NdisParameterInteger, 0, 0, NULL },
	{ "EventPriority", NdisParameterInteger, 0, 0, NULL },
#if defined(SIMPLE_ISCAN)
	{ "MaxScanResultAge", NdisParameterInteger, 0, 0, NULL },
#endif
	{ NULL, 0, 0, 0, NULL }
};

static int
dhd_open(dhd_info_t *dhd, wl_info_t *wl)
{
	int ret = -1;
	shared_info_t *sh = &wl->sh;
	char iovbuf[WL_EVENTING_MASK_LEN + 12];	/* Room for "event_msgs" + '\0' + bitvec */
	NDIS_HANDLE confighandle = NULL;
	NDIS_STATUS status;
	PNDIS_CONFIGURATION_PARAMETER param;
#ifdef NDIS60
	NDIS_CONFIGURATION_OBJECT configobj;
#endif /* NDIS60 */
	uint chip;
	int i = 0;
	dhd_pub_t *dhdp = dhd_bus_pub(wl->dhd);

	DHD_TRACE(("%s: Enter\n", __FUNCTION__));

	/* firmware and nvram files paths. chip specific paths take precedence */
	chip = dhd_bus_chip(dhd->pub.bus);

	/* open the registry */
#ifndef NDIS60
	NdisOpenConfiguration(&status, &confighandle, sh->confighandle);
#else
	bzero(&configobj, sizeof(NDIS_CONFIGURATION_OBJECT));

	configobj.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
	configobj.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
	configobj.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT);

	configobj.NdisHandle = sh->adapterhandle;
	configobj.Flags = 0;

	status = NdisOpenConfigurationEx(&configobj, &confighandle);
#endif /* !NDIS60 */
	if (NDIS_ERROR(status)) {
		DHD_ERROR(("dhd_open: NdisOpenConfiguration error 0x%x\n", status));
		NdisWriteErrorLogEntry(sh->adapterhandle,
		                       NDIS_ERROR_CODE_MISSING_CONFIGURATION_PARAMETER, 1, 24);
		return -1;
	}
	ASSERT(confighandle);

	/* Debug messages in DHD_xx debug macros */
	if ((param = wl_readparam(dhd_configs, "DHDMsgLevel", confighandle, wl, NULL))) {
		dhd_msg_level = param->ParameterData.IntegerData;
		printf("dhd_msg_level set to %u\n", dhd_msg_level);
	}

	/* Debug messages in WL_xx debug macros (wl_ndis.c) */
	if ((param = wl_readparam(dhd_configs, "WLMsgLevel", confighandle, wl, NULL))) {
		wl_msg_level = param->ParameterData.IntegerData;
		printf("wl_msg_level set to %u\n", wl_msg_level);
	}

	/* read dongle image path if it exists */
	snprintf(dongleimagepathkey, sizeof(dongleimagepathkey), "%04XDongleImagePath", chip);
	if ((param = wl_readparam(dhd_configs, dongleimagepathkey, confighandle, wl, NULL)) ||
	    (param = wl_readparam(dhd_configs, "DongleImagePath", confighandle, wl, NULL))) {

		wchar2ascii(fw_path, param->ParameterData.StringData.Buffer,
			param->ParameterData.StringData.Length,
			sizeof(fw_path));
	}
	DHD_INFO(("firmware path set to %s\n", fw_path));

	/* read srom image path if it exists */
	snprintf(sromimagepathkey, sizeof(sromimagepathkey), "%04XSROMImagePath", chip);
	if ((param = wl_readparam(dhd_configs, sromimagepathkey, confighandle, wl, NULL)) ||
	    (param = wl_readparam(dhd_configs, "SROMImagePath", confighandle, wl, NULL))) {

		wchar2ascii(nv_path, param->ParameterData.StringData.Buffer,
			param->ParameterData.StringData.Length,
			sizeof(nv_path));
	}
	DHD_INFO(("nvram path set to %s\n", nv_path));

	/* EnableArpOffload allows to control the arp offload feature in the firmware
	*/
	if ((param = wl_readparam(dhd_configs, "EnableAOE", confighandle, wl, NULL))) {
		dhd_arp_enable = param->ParameterData.IntegerData ? TRUE : FALSE;
	}
	DHD_INFO(("EnableArpOffload set to %d from the registry\n", dhd_arp_enable));

	/* Controls ArpOffload mode 
	*/
	if ((param = wl_readparam(dhd_configs, "AOEMode", confighandle, wl, NULL))) {
		dhd_arp_mode = param->ParameterData.IntegerData;
	}
	DHD_INFO(("ArpOffload mode set to 0x%x from the registry\n", dhd_arp_mode));

	/* Controls Pkt filter enable/disable
	*/
	if ((param = wl_readparam(dhd_configs, "EnablePKTFilter", confighandle, wl, NULL))) {
		dhd_pkt_filter_enable = param->ParameterData.IntegerData;
	}
	DHD_INFO(("EnablePktfilter set to 0x%x from the registry\n", dhd_pkt_filter_enable));

	/* Controls Pkt filter mode
	*/
	if ((param = wl_readparam(dhd_configs, "PKTFilterMode", confighandle, wl, NULL))) {
		dhd_master_mode = param->ParameterData.IntegerData;
	}
	DHD_INFO(("EnablePktfilter mode set to 0x%d from the registry\n", dhd_master_mode));

	if (dhd_pkt_filter_enable) {
		do
		{
			/* read pkt filter defination */
			snprintf(pktfilterkey, sizeof(pktfilterkey), "PKTFilter%02d", i);
			if ((param = wl_readparam(dhd_configs, pktfilterkey,
				confighandle, wl, NULL))) {

				if (!(dhd->pub.pktfilter[i] = MALLOC(dhd->pub.osh,
					param->ParameterData.StringData.Length + 1))) {
					DHD_ERROR(("%s: kmalloc failed\n", __FUNCTION__));
					break;
				}

				wchar2ascii(dhd->pub.pktfilter[i],
					param->ParameterData.StringData.Buffer,
					param->ParameterData.StringData.Length,
					param->ParameterData.StringData.Length);
			} else {
				break;
			}
			DHD_INFO(("%s is set to %s\n", pktfilterkey, dhd->pub.pktfilter[i]));
		} while (++i);

		dhd->pub.pktfilter_count = i;
		dhd_pkt_filter_enable = i ? 1 : 0;
	}
#if defined(DHD_DEBUG)
	/* Dongle console message poll interval */
	if ((param = wl_readparam(dhd_configs, "DongleConsoleMs", confighandle, wl, NULL))) {
		 dhd_console_ms = param->ParameterData.IntegerData;
	}
	DHD_INFO(("DongleConsoleMs set to 0x%x ms from the registry\n", dhd_console_ms));
#endif /* DHD_DEBUG */

	/* Debug message output destination */
	if ((param = wl_readparam(dhd_configs, "DbgOutput", confighandle, wl, NULL))) {
		if (param->ParameterData.IntegerData < DBG_OUTPUT_MAX) {
			 g_dbgOutputIndex = param->ParameterData.IntegerData;
		}
	}
	DHD_INFO(("DbgOutput set to %d from the registry\n", g_dbgOutputIndex));

	/* PNP handling option */
	if ((param = wl_readparam(dhd_configs, "PNPMethod", confighandle, wl, NULL))) {
		wl->PNPMethod = param->ParameterData.IntegerData;
	} else {
		wl->PNPMethod = PNP_NORMAL;
	}
	DHD_INFO(("PNPMethod set to %d from the registry\n", wl->PNPMethod));

	/* WatchdogPeriod handling option */
	if ((param = wl_readparam(dhd_configs, "WatchdogPeriod", confighandle, wl, NULL))) {
		dhd_watchdog_ms = param->ParameterData.IntegerData;
	}
	DHD_INFO(("WatchdogPeriod set to %d from the registry\n", dhd_watchdog_ms));

	/* OidPriority handling option */
	if ((param = wl_readparam(dhd_configs, "OidPriority", confighandle, wl, NULL))) {
		wl->sh.OidPriority = param->ParameterData.IntegerData;
	} else {
		wl->sh.OidPriority = 0;
	}
	DHD_INFO(("OidPriority set to %d from the registry\n", wl->sh.OidPriority));

	/* EventPriority handling option */
	if ((param = wl_readparam(dhd_configs, "EventPriority", confighandle, wl, NULL))) {
		wl->sh.EventPriority = param->ParameterData.IntegerData;
	} else {
		wl->sh.EventPriority = 132;
	}
	DHD_INFO(("EventPriority set to %d from the registry\n", wl->sh.EventPriority));

	if ((param = wl_readparam(dhd_configs, "DPCPriority", confighandle, wl, NULL))) {
		wl->sh.DPCPriority = param->ParameterData.IntegerData;
		wl->sh.dpc_prio = param->ParameterData.IntegerData;
	} else {
		wl->sh.DPCPriority = 251;
		wl->sh.dpc_prio = 251;
	}
	DHD_INFO(("DPCPriority set to %d from the registry\n", wl->sh.DPCPriority));

	if ((param = wl_readparam(dhd_configs, "RXThreadPriority", confighandle, wl, NULL))) {
		wl->sh.RXThreadPriority = param->ParameterData.IntegerData;
		wl->sh.sendup_prio = param->ParameterData.IntegerData;
	} else {
		wl->sh.RXThreadPriority = 135;
		wl->sh.sendup_prio = 135;
	}
	DHD_INFO(("RXThreadPriority set to %d from the registry\n", wl->sh.RXThreadPriority));


#if defined(EXT_STA) && defined(SIMPLE_ISCAN)
	/* See if we should remove a BSS entry from scan list if entry is too old */
	if ((param = wl_readparam(dhd_configs, "MaxScanResultAge", confighandle, wl, NULL)))
		dhdp->max_scan_result_age = (int)param->ParameterData.IntegerData;
	else
		dhdp->max_scan_result_age = 0;

	DHD_INFO(("dhdp->max_scan_result_age set to %d from the registry\n",
		dhdp->max_scan_result_age));
#endif 

	/* close the registry */
	NdisCloseConfiguration(confighandle);

	/* download image and nvram to the dongle */
	if (!(dhd_bus_download_firmware(dhd->pub.bus, dhd->pub.osh,
	                                fw_path, nv_path))) {
		DHD_ERROR(("%s: dhdsdio_probe_download failed. firmware = %s nvram = %s\n",
		           __FUNCTION__, fw_path, nv_path));
		return -1;
	}


	dhd_os_wd_timer(dhd, dhd_watchdog_ms);

	/* Bring up the bus */
	if ((ret = dhd_bus_init(&dhd->pub, TRUE)) != 0)
		return ret;

	/* If bus is not ready, can't come up */
	if (dhd->pub.busstate != DHD_BUS_DATA) {
		return -1;
	}

	/* Need to explicity register for events in dongle mode */
	/* by default want to see events MIC_ERROR, NDIS_LINK and PMKID_CACHE */
	bcm_mkiovar("event_msgs", NULL, 0, iovbuf, sizeof(iovbuf));
	wl_ioctl(wl, WLC_GET_VAR, iovbuf, sizeof(iovbuf));
	bcopy(iovbuf, dhd->pub.eventmask, WL_EVENTING_MASK_LEN);

#if defined(NDIS60)
	bzero(dhd->pub.eventmask, sizeof(dhd->pub.eventmask));
#endif /* UNDER_CE && NDIS60 */

#if defined(EXT_STA)
	register_ext_sta_events(dhd->pub.eventmask);
#else
	setbit(dhd->pub.eventmask, WLC_E_MIC_ERROR);
	setbit(dhd->pub.eventmask, WLC_E_NDIS_LINK);
	setbit(dhd->pub.eventmask, WLC_E_PMKID_CACHE);
#ifdef DHD_NDIS_OID
	setbit(dhd->pub.eventmask, WLC_E_QUIET_START);
	setbit(dhd->pub.eventmask, WLC_E_QUIET_END);
	setbit(dhd->pub.eventmask, WLC_E_BEACON_RX);
	setbit(dhd->pub.eventmask, WLC_E_SET_SSID);
	setbit(dhd->pub.eventmask, WLC_E_LINK);
#endif /* DHD_NDIS_OID */
#endif /* EXT_STA */
#ifdef WLBTAMP_HIGH_ONLY
	register_bta_events(dhd->pub.eventmask);
#endif /* WLBTAMP_HIGH_ONLY */

	/* Bus is ready, do any protocol initialization */
	dhd_prot_init(&dhd->pub);
	DHD_ERROR(("Dongles MAC address = %02X:%02X:%02X:%02X:%02X:%02X\n",
		dhd->pub.mac.octet[0], dhd->pub.mac.octet[1], dhd->pub.mac.octet[2],
		dhd->pub.mac.octet[3], dhd->pub.mac.octet[4], dhd->pub.mac.octet[5]));

	/* Allow transmit calls */
	dhd->pub.up = 1;

#ifdef WLBTAMP_HIGH_ONLY
	dhd->pub.bta = wlc_bta_attach(((struct wl_info *)dhd->wl)->wlc);
	if (dhd->pub.bta == NULL)
		return -ENOMEM;
#endif

#if !defined(NDIS60)
	/* create the well known message queues */
	DHD_TRACE(("dhd_open: creating queue %s\n", dhd_sendup_info[0].msgqueue_name));
	dhd_sendup_info[0].msgqueue_handle = (void *)MsgQueueCreate(
			dhd_sendup_info[0].msgqueue_name, (bool) FALSE);
	DHD_TRACE(("dhd_open: creating queue %s\n", dhd_sendup_info[1].msgqueue_name));
	dhd_sendup_info[1].msgqueue_handle = (void *)MsgQueueCreate(
			dhd_sendup_info[1].msgqueue_name, (bool) FALSE);
#endif /* !NDIS60 */

	return 0;
}

osl_t *
dhd_osl_attach(void *pdev, uint bustype)
{
	return 0;
}

void
dhd_osl_detach(osl_t *osh)
{
	return;
}

/*
 * Generalized timeout mechanism.  Uses spin sleep with exponential back-off.  Usage:
 *
 *      dhd_timeout_start(&tmo, usec);
 *      while (!dhd_timeout_expired(&tmo))
 *              if (poll_something())
 *                      break;
 *      if (dhd_timeout_expired(&tmo))
 *              fatal();
 */

void
dhd_timeout_start(dhd_timeout_t *tmo, uint usec)
{
	tmo->limit = usec;
	tmo->increment = 0;
	tmo->elapsed = 0;
	tmo->tick = 10000;
}

int
dhd_timeout_expired(dhd_timeout_t *tmo)
{
	/* Does nothing the first call */
	if (tmo->increment == 0) {
		tmo->increment = 1;
		return 0;
	}

	if (tmo->elapsed >= tmo->limit)
		return 1;

	/* Add the delay that's about to take place */
	tmo->elapsed += tmo->increment;


	OSL_DELAY(tmo->increment);
	tmo->increment *= 2;
	if (tmo->increment > tmo->tick)
		tmo->increment = tmo->tick;

	return 0;
}

int
dhd_ifname2idx(dhd_info_t *dhd, char *name)
{
	int i = DHD_MAX_IFS;

	ASSERT(dhd);

	if (name == NULL || *name == '\0')
		return 0;

	while (--i > 0)
		if (dhd->iflist[i] && !strncmp(dhd->iflist[i]->name, name, IFNAMSIZ))
				break;

	DHD_TRACE(("%s: return idx %d for \"%s\"\n", __FUNCTION__, i, name));

	return i;	/* default - the primary interface */
}

int
dhd_add_if(dhd_info_t *dhd, int ifidx, void *handle, char *name,
	uint8 *mac_addr, uint32 flags, uint8 bssidx)
{
	dhd_if_t *ifp;

	DHD_TRACE(("%s: idx %d, handle->%p\n", __FUNCTION__, ifidx, handle));

	ASSERT(dhd && (ifidx < DHD_MAX_IFS));

	ifp = dhd->iflist[ifidx];
	if (!ifp && !(ifp = MALLOC(dhd->pub.osh, sizeof(dhd_if_t)))) {
		DHD_ERROR(("%s: OOM - dhd_if_t\n", __FUNCTION__));
		return -ENOMEM;
	}

	memset(ifp, 0, sizeof(dhd_if_t));
	ifp->info = dhd;
	dhd->iflist[ifidx] = ifp;
	strncpy(ifp->name, name, IFNAMSIZ);
	ifp->name[IFNAMSIZ] = '\0';
	if (mac_addr != NULL)
		memcpy(&ifp->mac_addr, mac_addr, ETHER_ADDR_LEN);

	ifp->state = WLC_E_IF_ADD;
	ifp->idx = ifidx;
	ifp->bssidx = bssidx;

	return 0;
}

uint8 *
dhd_bssidx2bssid(dhd_pub_t *dhdp, int idx)
{
	int i;
	dhd_info_t *dhd = (dhd_info_t *)dhdp;

	ASSERT(dhd);
	for (i = 0; i < DHD_MAX_IFS; i++)
	if (dhd->iflist[i] && dhd->iflist[i]->bssidx == idx)
		return dhd->iflist[i]->mac_addr;

	return NULL;
}


void
dhd_del_if(dhd_info_t *dhd, int ifidx)
{
	dhd_if_t *ifp;

	DHD_TRACE(("%s: idx %d\n", __FUNCTION__, ifidx));

	ASSERT(dhd && ifidx && (ifidx < DHD_MAX_IFS));
	ifp = dhd->iflist[ifidx];
	if (!ifp) {
		DHD_ERROR(("%s: Null interface\n", __FUNCTION__));
		return;
	}

	ifp->state = WLC_E_IF_DEL;
	ifp->idx = ifidx;
	return;
}

dhd_pub_t *
dhd_attach(osl_t *osh, struct dhd_bus *bus, uint bus_hdrlen)
{
	dhd_info_t *dhd = NULL;

	DHD_TRACE(("%s: Enter\n", __FUNCTION__));

	if (!(dhd = MALLOC(osh, sizeof(dhd_info_t)))) {
		DHD_ERROR(("malloc of dhd_info_t failed!\n"));
		goto fail;
	}

	memset(dhd, 0, sizeof(dhd_info_t));
	dhd->pub.osh = osh;

	/* Link to bus module */
	dhd->pub.bus = bus;
	dhd->pub.hdrlen = bus_hdrlen;

	InitializeCriticalSection(&dhd->proto_sem);

	dhd->ioctl_resp_wait = CreateSemaphore(0, 0, 1, 0);
	if (!dhd->ioctl_resp_wait) {
		DHD_ERROR(("CreateSemaphore failed\n"));
		goto fail;
	}

	/* Attach and link in the protocol */
	if (dhd_prot_attach(&dhd->pub) != 0) {
		DHD_ERROR(("dhd_prot_attach failed\n"));
		goto fail;
	}

	dhd->pub.rxsz = ETHER_MTU + 14 + dhd->pub.hdrlen;

	return (dhd_pub_t *)dhd;

fail:
	dhd_detach((dhd_pub_t *)dhd);

	return NULL;
}
int
dhd_bus_start(dhd_pub_t *dhdp)
{
	/* Nothing to do */
	return 0;
}

int
dhd_net_attach(dhd_pub_t *dhdp, int ifidx)
{
	/* Nothing to do */
	return 0;
}

void
dhd_detach(dhd_pub_t *dhdp)
{
	dhd_info_t *dhd = (dhd_info_t *)dhdp;

	DHD_TRACE(("%s: Enter\n", __FUNCTION__));

	if (dhd) {

		dhd_stop(dhd);

		if (dhd->pub.prot)
			dhd_prot_detach(&dhd->pub);

		if (dhd->ioctl_resp_wait)
			CloseHandle(dhd->ioctl_resp_wait);

		DeleteCriticalSection(&dhd->proto_sem);

		if (dhd->sendup_handle)
			CloseHandle(dhd->sendup_handle);

		NdisFreeEvent(&dhd->sendup_event);
		NdisFreeEvent(&dhd->wifioff_event);
#if defined(EXT_STA)
		if (dhd->event_process_handle)
			CloseHandle(dhd->event_process_handle);
		NdisFreeEvent(&dhd->host_event);
#endif /* EXT_STA */


#ifdef WLBTAMP_HIGH_ONLY
		if (dhd->pub.bta)
		{
			wlc_bta_detach(dhd->pub.bta);
			dhd->pub.bta = NULL;
		}
#endif
		if (dhd->pub.osh)
			MFREE(dhd->pub.osh, dhd, sizeof(dhd_info_t));
	}
}

#if defined(BCMDHDUSB)
bool
dhd_reload_image(void *data)
{
	dhd_info_t * dhd;

	dhd = *(dhd_info_t **)data;
	ASSERT(dhd != NULL);

	return dhdusb_reload_image((void *)&dhd->pub);
}
#endif

#ifdef DHD_NDIS_OID
NDIS_STATUS
dhd_oid_attach(wl_info_t *wl)
{
	NDIS_STATUS status;

	status = NdisAllocateMemoryWithTag(&wl->wlc, sizeof(wlc_info_t), MEMORY_TAG);
	if (!wl->wlc || status != NDIS_STATUS_SUCCESS) {
		WL_ERROR(("%s: wl->wlc NdisAllocateMemoryWithTag error %x\n",
			__FUNCTION__, status));
		return status;
	} else {
		bzero(wl->wlc, sizeof(wlc_info_t));
		wl->pub = (wlc_pub_t *)wl->wlc;
		wl->wlc->wl = wl;
	}
	wl->wlc->pub.osh = wl->sh.osh;
	wl->oid = wl_oid_attach(wl->wlc);
	if (!wl->oid) {
		return NDIS_STATUS_FAILURE;
	}
	wl->oid->scan_request_pending = FALSE;
	wl_iovar_op(wl, "perm_etheraddr", &wl->wlc->perm_etheraddr, ETHER_ADDR_LEN, IOV_GET);
	wl_iovar_op(wl, "cur_etheraddr", &wl->pub->cur_etheraddr, ETHER_ADDR_LEN, IOV_GET);
	wl_iovar_op(wl, "default_bss_info", &wl->wlc->default_bss, sizeof(wlc_bss_info_t), IOV_GET);
	return status;
}

void
dhd_oid_detach(wl_info_t *wl)
{
	uint i = 0;

	if (wl->wlc) {
		for (i = 0; i < NBANDS_PUB(wl->pub); i++) {
			if (wl->wlc->bandstate[i].pi) {
				NdisFreeMemory(wl->wlc->bandstate[i].pi,
					sizeof(*wl->wlc->bandstate[i].pi), 0);
			}
		}
		NdisFreeMemory(wl->wlc, sizeof(wlc_info_t), 0);
	}
	if (wl->oid) {
		wl_oid_detach(wl->oid);
	}
}

NDIS_STATUS
wl_queryoid(wl_info_t *wl, NDIS_OID oid, PVOID buf, ULONG buflen,
	PULONG bytes_written, PULONG bytes_needed, int ifidx)
{
	return wl_query_oid(wl->oid, oid, buf, buflen, bytes_written, bytes_needed);
}

NDIS_STATUS
wl_setoid(wl_info_t *wl, NDIS_OID oid, PVOID buf, ULONG buflen,
	PULONG bytes_read, PULONG bytes_needed, int ifidx)
{
	return wl_set_oid(wl->oid, oid, buf, buflen, bytes_read, bytes_needed);
}

void
dhd_iovar_post_process(wl_info_t *wl, const char *name, void *buf, uint len, bool set)
{
	/* Only cache after oid attach */
	if (wl->wlc && wl->oid) {
		if (!strcmp(name, "ibss_allowed")) {
			wl->wlc->ibss_allowed = *(bool *)buf;
		}
		if (!strcmp(name, "cur_ssid")) {
			ssid_info_t * ssid_info = (ssid_info_t *)buf;
			wl->pub->current_bss.SSID_len = ssid_info->ssid_len;
			bcopy(ssid_info->ssid, wl->pub->current_bss.SSID,
				wl->pub->current_bss.SSID_len);
		}
	}
}

void
dhd_ioctl_post_process(wl_info_t *wl, uint cmd, char *buf, uint buflen)
{
	/* Only cache after oid attach */
	if (wl->wlc && wl->oid) {
		switch (cmd) {
			case WLC_SET_INFRA:{
				int infra = *(int *)buf;
				if (!(AP_ENAB(&wl->wlc->pub) && infra == 0)) {
					wl->wlc->default_bss.infra = infra?1:0;
				}

				break;
				}
			default:
				break;
		}
	}
}

#else
NDIS_STATUS
wl_queryoid(wl_info_t *wl, NDIS_OID oid, PVOID buf, ULONG buflen,
	PULONG bytes_written, PULONG bytes_needed, int ifidx)
{
	dhd_ioctl_t local_ioc;
	int status;

	memset(&local_ioc, 0, sizeof(local_ioc));
	local_ioc.cmd = oid;
	local_ioc.buf = buf;
	local_ioc.len = buflen;

	status = dhd_ioctl_entry(dhd_bus_dhd(wl->dhd), ifidx, (char *)&local_ioc);
	if (status != 0) {
#if defined(EXT_STA)
		/* OID_GEN_STATISTICS (0x20106) is not supported in dongle */
		if (OID_GEN_STATISTICS != oid)
			DHD_TRACE(("wl_queryoid failed 0x%x oid=0x%x, needed=%d\n",
				status, oid, local_ioc.needed));
		else {
			DHD_TRACE(("wl_queryoid: unsupported oid=0x%x\n", oid));
		}
#endif

		*bytes_written = 0;
		*bytes_needed = local_ioc.needed;
		return status;
}
	if (local_ioc.used > buflen)
		*bytes_written = 0;
	else
		*bytes_written = local_ioc.used;
	*bytes_needed = local_ioc.needed;

	if (status != 0)
		DHD_TRACE(("%s: oid 0x%x status %d failed\n", __FUNCTION__, oid, status));

	return status;
}

NDIS_STATUS
wl_setoid(wl_info_t *wl, NDIS_OID oid, PVOID buf, ULONG buflen,
	PULONG bytes_read, PULONG bytes_needed, int ifidx)
{
	dhd_ioctl_t local_ioc;
	int status;

	memset(&local_ioc, 0, sizeof(local_ioc));
	local_ioc.cmd = oid;
	local_ioc.buf = buf;
	local_ioc.len = buflen;
	local_ioc.set = TRUE;

	status = dhd_ioctl_entry(dhd_bus_dhd(wl->dhd), ifidx, (char *)&local_ioc);
	if (status != 0) {
		DHD_ERROR(("wl_setoid failed = 0x%x oid=0x%x!\n", status, oid));
		local_ioc.len = 0;
	}

	*bytes_read = local_ioc.len;
	*bytes_needed = local_ioc.needed;

	if (status != 0)
		DHD_TRACE(("%s: oid 0x%x failed\n", __FUNCTION__, oid));

	return status;
}
#endif /* DHD_NDIS_OID */

int
wl_iovar_op(wl_info_t *wl, const char *name, void *buf, uint len, bool set)
{
	dhd_ioctl_t local_ioc;
	int iovar_len, tot_len, status;
	char *maxbuf;

	iovar_len = strlen(name) + 1;
	tot_len = iovar_len + len;
	if ((maxbuf = MALLOC(wl->sh.osh, tot_len)) == NULL) {
		DHD_ERROR(("%s: malloc of size %d failed!\n", __FUNCTION__, tot_len));
		return NDIS_STATUS_FAILURE;
	}

	strcpy(maxbuf, name);
	memcpy(&maxbuf[iovar_len], buf, len);

	memset(&local_ioc, 0, sizeof(local_ioc));
	local_ioc.cmd = set ? WLC_SET_VAR : WLC_GET_VAR;
	local_ioc.buf = maxbuf;
	local_ioc.len = tot_len;
	local_ioc.set = set;

	/* XXXX Hack should go away when we fix the dhd_WAR44971 for CE */
	if (!strcmp(name, "war44971")) {
		local_ioc.driver = DHD_IOCTL_MAGIC;
		local_ioc.cmd = set ? DHD_SET_VAR : DHD_GET_VAR;
	}

	if (!strcmp(name, "devreset")) {
		local_ioc.driver = DHD_IOCTL_MAGIC;
		local_ioc.cmd = set ? DHD_SET_VAR : DHD_GET_VAR;
		DHD_ERROR(("%s: Calling WLANOFF \n", __FUNCTION__));
	}
	status = dhd_ioctl_entry(dhd_bus_dhd(wl->dhd), 0, (char *)&local_ioc);

	if (!set && (status == 0)) {
		bcopy(maxbuf, buf, len);
	}
#ifdef DHD_NDIS_OID
	if (status == 0) {
		dhd_iovar_post_process(wl, name, buf, len, set);
	}
#endif /* DHD_NDIS_OID */
	MFREE(wl->sh.osh, maxbuf, tot_len);
	return status;
}

int
wl_ioctl(wl_info_t *wl, uint cmd, char *buf, uint buflen)
{
	dhd_ioctl_t local_ioc;
	int status;

	memset(&local_ioc, 0, sizeof(local_ioc));
	local_ioc.cmd = cmd;
	local_ioc.buf = buf;
	local_ioc.len = buflen;
	if (cmd == WLC_SET_VAR)
		local_ioc.set = TRUE;

	status = dhd_ioctl_entry(dhd_bus_dhd(wl->dhd), 0, (char *)&local_ioc);
#ifdef DHD_NDIS_OID
	if (status == 0) {
		dhd_ioctl_post_process(wl, cmd, buf, buflen);
	}
#endif /* DHD_NDIS_OID */
	return status;
}

int
dhd_bus_ioctl(wl_info_t *wl, int oid, char *buf, uint buflen,
	PULONG bytes_read, PULONG bytes_needed, bool set)
{
	dhd_ioctl_t local_ioc;
	int ret;

	memset(&local_ioc, 0, sizeof(local_ioc));
	local_ioc.cmd = oid;
	local_ioc.buf = buf;
	local_ioc.len = buflen;
	local_ioc.set = set;
	local_ioc.driver = DHD_IOCTL_MAGIC;

	ret = dhd_ioctl_entry(dhd_bus_dhd(wl->dhd), 0, (char *)&local_ioc);

	*bytes_read = local_ioc.len;
	*bytes_needed = local_ioc.needed;

	return ret;
}

static void
dhd_dpc(void *arg)
{
	dhd_info_t * dhd = (dhd_info_t *)arg;
	wl_info_t * wl = dhd->wl;

	if (dhd_bus_dpc(dhd->pub.bus))
		shared_dpc_schedule(&wl->sh);

	wl_sendcomplete(wl);
}

void
dhd_sched_dpc(dhd_pub_t *dhdp)
{
#if defined(NDIS60)
	/* XXXX ???
	 * Scheduling the DPC here is very sensitive
	 * to the thread priority of DPC thread. If the
	 * priority is high enough then receive doesn't work
	 * properly
	 */
	dhd_info_t *dhd = (dhd_info_t *)dhdp;
	wl_info_t * wl = dhd->wl;

	/* For CE back round thread schedules the DPC */
	shared_dpc_schedule(&wl->sh);
#endif 
}

uint
wl_getstat(wl_info_t *wl, NDIS_OID Oid)
{
	dhd_pub_t *dhdp = dhd_bus_pub(wl->dhd);

	if (dhdp->up) {
		/* Use the protocol to get dongle stats */
		dhd_prot_dstats(dhdp);
	}

	switch (Oid) {
	case OID_GEN_XMIT_OK:
		return dhdp->dstats.tx_packets;

	case OID_GEN_RCV_OK:
		return dhdp->dstats.rx_packets;

	case OID_GEN_XMIT_ERROR:
		return dhdp->dstats.tx_errors;

	case OID_GEN_RCV_ERROR:
		return dhdp->dstats.rx_errors;

	default:
		return 0;
	}
}

void
wl_link_up(wl_info_t *wl)
{
#ifndef EXT_STA
	wl_indicate_link_up(wl);
#endif /* EXT_STA */
}

void
wl_link_down(wl_info_t *wl)
{
#ifndef EXT_STA
	wl_indicate_link_down(wl);
#endif /* EXT_STA */
}

int
wl_up(wl_info_t *wl)
{
	/* In case of dongle control for when wl up should happen. This allows
	   some settings like wme, country code etc. to be performed before the
	   wl up in case of a reclaim image. Mainly used for testing, controlled
	   via "AllowIntfUp" registry entry
	*/
	if (TRUE == wl->AllowIntfUp) {
		uint up = 1;
		int err;
#if defined(BCMDHDUSB)
		dhd_info_t *dhd = *(dhd_info_t **)wl->dhd;

		dhdusb_ctlreq_waitcomplete(dhd->pub.bus);
#endif /* BCMDHDUSB */
		err = wl_ioctl(wl, WLC_UP, (char *)&up, sizeof(up));
#if defined(BCMDHDUSB)
		if (err) {
			dhdusb_ctlreq_waitcomplete(dhd->pub.bus);
			err = wl_ioctl(wl, WLC_UP, (char *)&up, sizeof(up));
		}
#endif /* BCMDHDUSB */
		return err;
	}

	return 0;
}

void
wl_down(wl_info_t *wl)
{
	uint down = 1;
	wl_ioctl(wl, WLC_DOWN, (char *)&down, sizeof(down));
	wl_flushtxq(wl);
	wl_sendcomplete(wl);
}

int
wl_reboot(wl_info_t *wl)
{
	return wl_ioctl(wl, WLC_REBOOT, NULL, 0);
}

/*
 * called by NDIS during restart/shutdown/crash, driver maybe in any state
 * need to put chip in known state and other hardware in clean state if possible
 */
void
wl_shutdown(wl_info_t *wl)
{
}
bool
wl_pnp_reinit_ndis(wl_info_t *wl)
{
	/* Need to explicity register for events in dongle mode */
	/* by default want to see events MIC_ERROR, NDIS_LINK and PMKID_CACHE */
	HKEY confighandle = NULL;
	int infra;
	NDIS_STATUS status;
	shared_info_t *sh = &wl->sh;
	bool ret = FALSE;
#if !defined(WL_IW_USE_ISCAN)
	ulong bytes_needed, bytes_read;
#endif /* !WL_IW_USE_ISCAN */

	/* avoid premature wlc_up() in wl_readconfigoverrides() */
	wl_iovar_setint(wl, "down_override", TRUE);

	/* open the registry */
	if ((status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"\\Comm\\BCMSDDHD1\\Parms",
		0, KEY_ALL_ACCESS, &confighandle)) != ERROR_SUCCESS) {
		DHD_ERROR(("BCMSDLDR:Failed to open path %s; %d \n",
			"\\Comm\\BCMSDDHD1\\Parms", status));
		goto error;
	}

	wl->context = REG_ACCESS_POWER_ON_OFF;
	/* read reg entries that may override defaults set in wlc_attach() */
	wl_readconfigoverrides((void *) wl, confighandle, sh->id, sh->unit, sh->OS);
	wl_scanoverrides(wl, confighandle, sh->id, sh->unit);
#if !defined(NDIS60)
	/* read the autoconnect configuration params */
	wl_read_auto_config(wl, confighandle, sh->adapterhandle);
#else
	wl_read_auto_config(wl);
#endif /* !NDIS60 */
	RegCloseKey(confighandle);

	ND_LOCK(wl);

	/*
	   Initialize chip. This could be dummy call in case of dongle,
	   if "AllowIntfUp" registry entry is set to '1'
	*/
	if (wl_up(wl)) {
		ND_UNLOCK(wl);
		DHD_ERROR(("wl%d: %s: wl_up error 0x%x\n", wl->sh.unit, __FUNCTION__, status));
		goto error;
	}

	ND_UNLOCK(wl);

	/* permits up onwards */
	wl_iovar_setint(wl, "down_override", FALSE);
	ret = TRUE;

error:
	return ret;
}

#define IOCTL_GET_WLAN_PM_STS	CTL_CODE(FILE_DEVICE_HAL, 5005, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_WLAN_PM_STS	CTL_CODE(FILE_DEVICE_HAL, 5006, METHOD_BUFFERED, FILE_ANY_ACCESS)

#if defined(NDIS60)
NDIS_STATUS
wl_pnp_set_power_reset(wl_info_t *wl, int devicestate)
{
	BOOL ret = FALSE;
	DWORD outBuf;
	DWORD byteReturned;
	dhd_info_t * dhd;
	DWORD inBuf = 1;
	int wlanoff_val = 0;
	bool canceled;
	char buf[20] = "disassoc";
	uint glom_mode = 0;

	/* Platform specific */
	HANDLE h = CreateFile(wl->sdHostCtrlName, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	dhd = dhd_bus_dhd(wl->dhd);

	switch (devicestate)
	{
	case NdisDeviceStateD0:
		DHD_ERROR(("**** BCMSDDHD off==>on ****\n"));

		/* Platform specific */
		inBuf = 1;
		if (h != INVALID_HANDLE_VALUE) {
			ret = DeviceIoControl(h,
				IOCTL_SET_WLAN_PM_STS,
				&inBuf,
				sizeof(DWORD),
				&outBuf,
				sizeof(DWORD),
				&byteReturned,
				NULL);
			if (!ret)
				DHD_ERROR(("wl%d: %s ERROR : IOCTL_GET_WLAN_STATUS return FALSE\n",
				wl->sh.unit, __FUNCTION__));
		}

		/* May need some delay based on customer platform */
		Sleep(200);

		if (bcmsdh_reenumerate(NULL) == FALSE)
			DHD_ERROR(("wl%d: Failed re-enumeration\n", wl->sh.unit, __FUNCTION__));

		wlanoff_val = 0;
		wl_iovar_op(wl, "devreset", &wlanoff_val, sizeof(wlanoff_val), IOV_SET);

		dhd_os_wd_timer(dhd, dhd_watchdog_ms);

		/* Re initilize NDIS related setting from registry and other IOCTLS */
		if (wl_pnp_reinit_ndis(wl) == FALSE)
			DHD_ERROR(("wl%d: Failed re-initilize NDIS\n", wl->sh.unit, __FUNCTION__));

#if defined(WL_IW_USE_ISCAN)
		/* Reinitialize iscan state */
		((iscan_info_t *)wl->iscan)->timer_on = 0;
		((iscan_info_t *)wl->iscan)->iscan_state = ISCAN_STATE_IDLE;
		((iscan_info_t *)wl->iscan)->timer_retry_cnt = ISCAN_RETRY_CNT;
#endif /* WL_IW_USE_ISCAN */
#ifdef WLBTAMP
#ifdef WLBTCEAMP11
		BtKernStart(wl);
#endif
#endif /* WLBTAMP */
		break;

	case NdisDeviceStateD3:
		DHD_ERROR(("**** BCMSDDHD on==>off ****\n"));

#ifdef WLBTAMP
#ifdef WLBTCEAMP11
		BtKernStop(wl);
#endif
#endif /* WLBTAMP */
		/* Proactively disassoc */
		wl_ioctl(wl, WLC_DISASSOC, 0, 0);

#if defined(WL_IW_USE_ISCAN)
		if (((iscan_info_t *)wl->iscan)->timer_on) {
			wl_iscan_del_timer(((iscan_info_t *)wl->iscan)->timer);
			((iscan_info_t *)wl->iscan)->timer_on = 0;
			((iscan_info_t *)wl->iscan)->iscan_state = ISCAN_STATE_IDLE;
		}
#endif /* WL_IW_USE_ISCAN */
		/* May need sleep to quench stray timer, even though we killed the timer */
		Sleep(1000);

		/* Make sure we kill the timer after some sleep so that
		 * watchdog timer will get chance to trun off HT back
		 * plane clock to save power while Wi-Fi is turned of
		 */
		dhd_os_wd_timer(dhd, 0);
		dhd_bus_watchdog(&dhd->pub);

		wlanoff_val = 1;
		wl_iovar_op(wl, "devreset", &wlanoff_val, sizeof(wlanoff_val), IOV_SET);

		/* Platform specific */
		if (h != INVALID_HANDLE_VALUE) {
			inBuf = 0;
			ret = DeviceIoControl(h,
				IOCTL_SET_WLAN_PM_STS,
				&inBuf,
				sizeof(DWORD),
				&outBuf,
				sizeof(DWORD),
				&byteReturned,
				NULL);
			if (!ret)
				DHD_ERROR(("wl%d: %s ERROR : IOCTL_GET_WLAN_STATUS return FALSE\n",
					wl->sh.unit, __FUNCTION__));
		}
		break;
	}

	if (h)
		CloseHandle(h);

	return NDIS_STATUS_SUCCESS;
}

void
wl_pnp_set_wifioffevent(wl_info_t *wl)
{
	dhd_info_t * dhd;

	dhd = dhd_bus_dhd(wl->dhd);
	NdisSetEvent(&dhd->wifioff_event);
}

NDIS_STATUS
wl_pnp_set_power_normal(wl_info_t *wl, int devicestate)
{
	dhd_info_t * dhd;
	bool canceled;

	dhd = dhd_bus_dhd(wl->dhd);

	switch (devicestate)
	{
	case NdisDeviceStateD0:
		DHD_ERROR(("**** BCMSDDHD off==>on ****\n"));

		dhd_os_wd_timer(dhd, dhd_watchdog_ms);

		break;

	case NdisDeviceStateD3:
		DHD_ERROR(("**** BCMSDDHD on==>off ****\n"));

		/* Don't be misled by old WLC_E_DISASSOC messages */
		NdisResetEvent(&dhd->wifioff_event);

		/* Proactively disassoc */
		wl_ioctl(wl, WLC_DISASSOC, 0, 0);

		/* Wait for the completion of disassoc, time out - 1000ms */
		NdisWaitEvent(&dhd->wifioff_event, 1000);
		NdisResetEvent(&dhd->wifioff_event);

		/* Make sure we kill the timer after some sleep so that
		 * watchdog timer will get chance to trun off HT back
		 * plane clock to save power while Wi-Fi is turned of
		 */
		dhd_os_wd_timer(dhd, 0);
		dhd_bus_watchdog(&dhd->pub);

		break;
	}
	return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
wl_pnp_set_power(wl_info_t *wl, int devicestate)
{
	if (wl->PNPMethod == PNP_NORMAL)
		return wl_pnp_set_power_normal(wl, devicestate);
	else
		return wl_pnp_set_power_reset(wl, devicestate);

}

#else
NDIS_STATUS
wl_pnp_set_power(wl_info_t *wl, int devicestate)
{
	BOOL ret = FALSE;
	DWORD outBuf;
	DWORD byteReturned;
	dhd_info_t * dhd;
	DWORD inBuf = 1;
	int wlanoff_val = 0;
	bool canceled;
	char buf[20] = "disassoc";
	uint glom_mode = 0;

	/* Platform specific */
	HANDLE h = CreateFile(wl->sdHostCtrlName, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	dhd = dhd_bus_dhd(wl->dhd);

	switch (devicestate)
	{
	case NdisDeviceStateD0:
		DHD_ERROR(("**** BCMSDDHD off==>on ****\n"));

		/* Platform specific */
		inBuf = 1;
		if (h != INVALID_HANDLE_VALUE) {
			ret = DeviceIoControl(h,
				IOCTL_SET_WLAN_PM_STS,
				&inBuf,
				sizeof(DWORD),
				&outBuf,
				sizeof(DWORD),
				&byteReturned,
				NULL);
			if (!ret)
				DHD_ERROR(("wl%d: %s ERROR : IOCTL_GET_WLAN_STATUS return FALSE\n",
				wl->sh.unit, __FUNCTION__));
		}

		/* May need some delay based on customer platform */
		Sleep(200);

		if (bcmsdh_reenumerate(NULL) == FALSE)
			DHD_ERROR(("wl%d: Failed re-enumeration\n", wl->sh.unit, __FUNCTION__));

		wlanoff_val = 0;
		wl_iovar_op(wl, "devreset", &wlanoff_val, sizeof(wlanoff_val), IOV_SET);

		/* Turn off glom for CE */
		wl_iovar_op(wl, "bus:txglom", &glom_mode, sizeof(glom_mode), IOV_SET);

		dhd_os_wd_timer(dhd, dhd_watchdog_ms);

		/* Re initilize NDIS related setting from registry and other IOCTLS */
		if (wl_pnp_reinit_ndis(wl) == FALSE) {
			DHD_ERROR(("wl%d: Failed re-initilize NDIS\n", wl->sh.unit, __FUNCTION__));
		}
#if defined(WL_IW_USE_ISCAN)
		/* Reinitialize iscan state */
		((iscan_info_t *)wl->iscan)->timer_on = 0;
		((iscan_info_t *)wl->iscan)->iscan_state = ISCAN_STATE_IDLE;
		((iscan_info_t *)wl->iscan)->timer_retry_cnt = ISCAN_RETRY_CNT;
#endif /* WL_IW_USE_ISCAN */
#ifdef WLBTAMP
#ifdef WLBTCEAMP11
		BtKernStart(wl);
#endif
#endif /* WLBTAMP */
		break;

	case NdisDeviceStateD3:
		DHD_ERROR(("**** BCMSDDHD on==>off ****\n"));

#ifdef WLBTAMP
#ifdef WLBTCEAMP11
		BtKernStop(wl);
#endif
#endif /* WLBTAMP */
		/* Proactively disassoc */
		wl_ioctl(wl, WLC_DISASSOC, 0, 0);

		/* Indicate link down */
		wl_link_down(wl);

		if (dhd->wd_timer_valid == TRUE) {
			NdisMCancelTimer(&dhd->h_wd_timer, &canceled);
			dhd->wd_timer_valid = FALSE;
		}
#if defined(WL_IW_USE_ISCAN)
		if (((iscan_info_t *)wl->iscan)->timer_on) {
			wl_iscan_del_timer(((iscan_info_t *)wl->iscan)->timer);
			((iscan_info_t *)wl->iscan)->timer_on = 0;
			((iscan_info_t *)wl->iscan)->iscan_state = ISCAN_STATE_IDLE;
		}
#endif /* WL_IW_USE_ISCAN */
		/* May need sleep to quench stray timer, even though we killed the timer */
		Sleep(1000);

		wlanoff_val = 1;
		wl_iovar_op(wl, "devreset", &wlanoff_val, sizeof(wlanoff_val), IOV_SET);

		/* Platform specific */
		if (h != INVALID_HANDLE_VALUE) {
			inBuf = 0;
			ret = DeviceIoControl(h,
				IOCTL_SET_WLAN_PM_STS,
				&inBuf,
				sizeof(DWORD),
				&outBuf,
				sizeof(DWORD),
				&byteReturned,
				NULL);
			if (!ret)
				DHD_ERROR(("wl%d: %s ERROR : IOCTL_GET_WLAN_STATUS return FALSE\n",
					wl->sh.unit, __FUNCTION__));
		}

		break;
	}

	if (h)
		CloseHandle(h);

	return NDIS_STATUS_SUCCESS;
}
#endif /* NDIS60 */

#if defined(EXT_STA) || defined(WLBTAMP_HIGH_ONLY)
struct wl_timer *
wl_init_timer(wl_info_t *wl, void (*fn)(void* arg), void* arg, const char *name)
{
	struct wl_timer *t;

	ASSERT(wl->ntimers < MAX_TIMERS);

	t = &wl->timer[wl->ntimers++];

	bzero(t, sizeof(struct wl_timer));
	t->wl = wl;
	t->fn = fn;
	t->arg = arg;


	NdisMInitializeTimer(&t->ndis_timer, wl->sh.adapterhandle,
		wl_timer, (PVOID)t);

	return t;
}

void
wl_add_timer(wl_info_t *wl, struct wl_timer *t, uint ms, int periodic)
{
	ASSERT(t >= &wl->timer[0]);
	ASSERT(t < &wl->timer[MAX_TIMERS]);

	ASSERT(t->fn);

	/* add same timer twice is not supported */

	/* do not check driver "up" for timer to be added when driver is "down" */
	t->periodic = periodic;
	if (!t->set) {
		t->set = TRUE;
		if (periodic) {
			NdisMSetPeriodicTimer(&t->ndis_timer, ms);
		} else {
			NdisMSetTimer(&t->ndis_timer, ms);
		}
	} else {
		WL_TRACE(("wl%d: wl_add_timer() no timer added! Already set!\n", wl->sh.unit));
	}
}

bool
wl_del_timer(wl_info_t *wl, struct wl_timer *t)
{
	bool canceled;

	ASSERT(t >= &wl->timer[0]);
	ASSERT(t < &wl->timer[MAX_TIMERS]);

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
wl_free_timer(wl_info_t *wlh, struct wl_timer *timer)
{
	/* NOOP for windows. */
}

static void
wl_timer(
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
	} else {
		DHD_TRACE(("%s: wl_timer not calling fnc: %X (t->set = %u  wl->timer_stop = %u)\n",
			__FUNCTION__, t->fn, t->set, wl->timer_stop));
	}

	ND_UNLOCK(wl);
}
#endif /* EXT_STA */


/*
 * OS specific functions required to implement DHD driver in OS independent way
 */
int
dhd_os_proto_block(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = (wl_info_t *)dhd->wl;

	if (dhd) {
		EnterCriticalSection(&dhd->proto_sem);
	}

	return 1;
}

#ifdef EXT_STA
static NDIS_STATUS
wl_ndis_reassoc(wl_info_t *wl)
{
	DOT11_ROAMING_START_PARAMETERS StatusBuffer;
	ASSERT(wl);

	if (!wl->pub->associated)
		return NDIS_STATUS_SUCCESS;

	WL_ASSOC(("%s: Faking NDIS_STATUS_DOT11_ROAMING_START to restore wlc state to the dongle\n",
	          __FUNCTION__));

	bzero(&StatusBuffer, sizeof(DOT11_ROAMING_START_PARAMETERS));
	StatusBuffer.Header.Type = NDIS_OBJECT_TYPE_DEFAULT;
	StatusBuffer.Header.Revision = DOT11_ROAMING_START_PARAMETERS_REVISION_1;
	StatusBuffer.Header.Size = sizeof(DOT11_ROAMING_START_PARAMETERS);
	StatusBuffer.uRoamingReason =  DOT11_ASSOC_STATUS_ROAMING_ASSOCIATION_LOST;

	shared_indicate_status(wl->sh.adapterhandle, NDIS_STATUS_DOT11_ROAMING_START,
		&StatusBuffer, sizeof(StatusBuffer));

	return NDIS_STATUS_SUCCESS;
}
#endif /* EXT_STA */

int
dhd_os_proto_unblock(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t *wl = (wl_info_t *)dhd->wl;

	if (dhd) {
		LeaveCriticalSection(&dhd->proto_sem);
	}

	return 1;
}

unsigned int
dhd_os_get_ioctl_resp_timeout(void)
{
	return ((unsigned int)dhd_ioctl_timeout_msec);
}

void
dhd_os_set_ioctl_resp_timeout(unsigned int timeout_msec)
{
	dhd_ioctl_timeout_msec = (unsigned long)timeout_msec;
}

int
dhd_os_ioctl_resp_wait(dhd_pub_t * pub, uint * condition, bool * pending)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	int ret;

	* pending = FALSE;

	if (dhd) {
		ret = WaitForSingleObject(dhd->ioctl_resp_wait, dhd_ioctl_timeout_msec);
		if (ret == WAIT_TIMEOUT)
			return 0;
		else
			return 1;
	}

	return -1;
}

int
dhd_os_ioctl_resp_wake(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;

	if (dhd) {
		ReleaseSemaphore(dhd->ioctl_resp_wait, 1, 0);
		return 0;
	}

	return -1;
}

void
dhd_os_wd_timer(void *dhdh, uint wdtick)
{
	bool canceled;
	dhd_info_t *dhd = dhdh;
	static uint save_dhd_watchdog_ms = 0;
	static bool first = TRUE;

	/* don't start the wd until fw is loaded */
	if (dhd->pub.busstate == DHD_BUS_DOWN)
		return;

	if (first) {
		NdisMInitializeTimer(&dhd->h_wd_timer, ((wl_info_t *)dhd->wl)->sh.adapterhandle,
			dhd_watchdog, (PVOID)dhd);
		first = FALSE;
	}

	/* Totally stop the timer */
	if (!wdtick && dhd->wd_timer_valid == TRUE) {
		NdisMCancelTimer(&dhd->h_wd_timer, &canceled);

		dhd->wd_timer_valid = FALSE;
		save_dhd_watchdog_ms = wdtick;
		return;
	}

	if (wdtick) {
		dhd_watchdog_ms = (uint)wdtick;
		if (save_dhd_watchdog_ms != dhd_watchdog_ms) {

			if (dhd->wd_timer_valid == TRUE)
				/* Stop timer and restart at new value */
				NdisMCancelTimer(&dhd->h_wd_timer, &canceled);

			/* Create timer again when watchdog period is
			   dynamically changed or in the first instance
			*/
			NdisMSetTimer(&dhd->h_wd_timer, wdtick);
		} else {
			/* Re arm the timer, at last watchdog period */
			NdisMSetTimer(&dhd->h_wd_timer, wdtick);
		}

		dhd->wd_timer_valid = TRUE;
		save_dhd_watchdog_ms = wdtick;
	}
}

void *
dhd_os_open_image(char * filename)
{
	FILE * fr = NULL;

	fr = fopen(filename, "rb");
	if (fr == NULL) {
		DHD_ERROR(("%s: Can't open image file %s, reason: %d\n", __FUNCTION__, filename,
			GetLastError()));
	}

	return fr;
}

int
dhd_os_get_image_block(char * buf, int len, void * image)
{
	int ret = 0;

	FILE * fr = (FILE *)image;

	if (fr)
		ret = fread(buf, sizeof(uint8), len, fr);

	return ret;

}

void
dhd_os_close_image(void * image)
{
	FILE * fr = (FILE *)image;

	if (fr)
		fclose(fr);
}


void
dhd_os_sdlock(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = (wl_info_t *)dhd->wl;

	if (wl) {
		NdisAcquireSpinLock(&wl->dhdlock);
	}
}

void
dhd_os_sdunlock(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = (wl_info_t *)dhd->wl;

	if (wl) {
		NdisReleaseSpinLock(&wl->dhdlock);
	}
}

void
dhd_os_sdlock_txq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = (wl_info_t *)dhd->wl;

	if (wl)
		NdisAcquireSpinLock(&wl->dhd_tx_queue_lock);
}

void
dhd_os_sdunlock_txq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = (wl_info_t *)dhd->wl;

	if (wl)
		NdisReleaseSpinLock(&wl->dhd_tx_queue_lock);
}

void
dhd_os_sdlock_rxq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	ND_RXLOCK(wl);
}

void
dhd_os_sdunlock_rxq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	ND_RXUNLOCK(wl);
}

void
dhd_os_sdtxlock(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	ND_TXLOCK(wl);
}

void
dhd_os_sdtxunlock(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	ND_TXUNLOCK(wl);
}

void
dhd_os_sdlock_sndup_rxq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	if (wl)
		NdisAcquireSpinLock(&wl->dhd_rx_queue_lock);
}

void
dhd_os_sdunlock_sndup_rxq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	if (wl)
		NdisReleaseSpinLock(&wl->dhd_rx_queue_lock);
}
void
dhd_os_sdlock_eventq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	if (wl)
		NdisAcquireSpinLock(&wl->dhd_evq_lock);
}

void
dhd_os_sdunlock_eventq(dhd_pub_t * pub)
{
	dhd_info_t * dhd = (dhd_info_t *)pub;
	wl_info_t * wl = dhd->wl;

	if (wl)
		NdisReleaseSpinLock(&wl->dhd_evq_lock);
}
void
dhd_wait_for_event(dhd_pub_t *pub, bool *lockvar)
{
	return;
}

void
dhd_wait_event_wakeup(dhd_pub_t*pub)
{
	return;
}

void
dhd_ind_scan_confirm(void *h, bool status)
{
	dhd_info_t *dhd_info = dhd_bus_dhd(h);
#if defined(EXT_STA)
	wl_ind_scan_confirm(dhd_info->wl, status ? NDIS_STATUS_SUCCESS : NDIS_STATUS_REQUEST_ABORTED);
#endif /* EXT_STA */
}

void
dhd_init_timer(void *h, PNDIS_TIMER_FUNCTION fn, void* arg, NDIS_MINIPORT_TIMER *pndis_timer)
{
	dhd_info_t *dhd_info = dhd_bus_dhd(h);
	wl_info_t *wl = (wl_info_t *)dhd_info->wl;

	NdisMInitializeTimer(pndis_timer, wl->sh.adapterhandle,	fn, arg);
}

void
dhd_release_timer(void *h, NDIS_MINIPORT_TIMER *pndis_timer)
{
	/* TBD */
}

int
dhd_del_timer(void * dhdp, NDIS_MINIPORT_TIMER *pndis_timer)
{
	bool canceled = TRUE;

	NdisMCancelTimer(pndis_timer, &canceled);
	return 0;
}

int
dhd_add_timer(void * dhdp, NDIS_MINIPORT_TIMER *pndis_timer, int delay)
{
	NdisMSetTimer(pndis_timer, delay);

	return 0;
}

int
dhd_wl_ioctl(void *h, uint cmd, char *buf, uint buflen)
{
	dhd_info_t *dhd_info = dhd_bus_dhd(h);

	return wl_ioctl(dhd_info->wl, cmd, buf, buflen);
}

#ifdef DHD_DEBUG
int
write_to_file(dhd_pub_t *dhd, uint8 *buf, int size)
{
	int ret = 0;
	FILE *pFile;

	/* Open physical file to write */
	pFile = fopen("mem_dump", "wb");
	if (pFile == NULL) {
		printf("%s: Open file error\n", __FUNCTION__);
		ret = -1;
		goto exit;
	}

	/* Write buf to physical file */
	fwrite(buf, sizeof(*buf), size, pFile);

exit:
	/* Handle buf free before exit */
	if (buf) {
		MFREE(dhd->osh, buf, size);
	}
	if (pFile) {
		fclose(pFile);
	}
	return ret;
}
#endif /* DHD_DEBUG */
