/*
 * Common definitions for NDIS interface
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wl_ndis.h,v 1.47.4.2.20.35 2010/06/11 19:24:49 Exp $
 */

#ifndef _wl_ndis_h_
#define _wl_ndis_h_


#include <wl_toe.h>
#include <wl_arpoe.h>

#ifdef WLBTAMP
#if defined(WLBTWUSB)
/* Interface with Bluetooth */
#include <bt_int.h>
#elif defined(WLBTCEAMP11)
#include <btceamp11.h>
#endif
#endif /* WLBTAMP */
#if defined(NDIS60) || (defined(EXT_STA) || defined(DHD_NDIS_OID))
#include <siutils.h>
#include <wlc_cfg.h>
#include <wlc_pub.h>
#endif /* UNDER_CE && NDIS60 */
/* required access macros */
#if defined(NDIS50)
#define	MAJOR_NDIS_VERSION	5 /* Major version number for 5.0 */
#define	MINOR_NDIS_VERSION	0 /* Minor version number for 5.0 */
#elif defined(NDIS51)
#define	MAJOR_NDIS_VERSION	5 /* Major version number for 5.1 */
#define	MINOR_NDIS_VERSION	1 /* Minor version number for 5.1 */
#elif defined(NDIS60)
#define	MAJOR_NDIS_VERSION	6 /* Major version number for 6.0 */
#define	MINOR_NDIS_VERSION	0 /* Minor version number for 6.0 */
#else
#error "unsupported version of NDIS"
#endif /* defined(NDIS50) */

#ifdef EXT_STA
#define	NWF_VERSION		2 /* v1.68: 1 if <= v1.44, 2 if >= 1.50 */
#endif /* EXT_STA */

#define REG_ACCESS_INIT 0
#define REG_ACCESS_POWER_ON_OFF 1

#define MAX_DRV_DESC	128 /* Max. string length for Driver Description */

enum {
	PNP_NORMAL, PNP_RESET
};

/* private tunables */
#define	NTXBUF		256		/* # local transmit buffers */
#define	NRXBUF		512		/* # local receive buffers */
#define	NRXBUFLOW	32		/* # rx bufs when we start whining */

#ifndef MAX_TIMERS
#ifndef WLC_MAXDPT
#ifdef WLDPT
#define WLC_MAXDPT	4		/* Max # of DPT links */
#else
#define WLC_MAXDPT	0		/* Max # of DPT links */
#endif /* DPT */
#endif /* WLC_MAXDPT */
#define	MAX_TIMERS	(27 + (2 * WLC_MAXDPT))		/* max # wl timers */
#endif /* MAX_TIMERS */


#if defined(P2P) && !defined(NDIS60)
#define WL_INFO(devicestate)	(gP2P.wl_instance_wl? gP2P.wl_instance_wl : devicestate)
#else
#define WL_INFO(devicestate)	(wl_info_t*) (devicestate)
#endif


struct wl_timer
{
	struct wl_info		*wl;
	NDIS_MINIPORT_TIMER	ndis_timer;
	void			(*fn)(void*);
	void*			arg; /* arg to *fn */
	int			set;
	int			periodic;
#ifdef BCMDBG
	char*			name;		/* Desription of the timer */
#endif /* BCMDBG */
};

/*
 * Principle (per-DeviceObject) software data structure.
 *
 * Start with the wlc_info_t structure of OS-independent state.
 *
 * There are two simple static lists of free lbufs, one for transmit
 * and the other for receive.  The lists are carved from cached shared memory pages.
 * The free lists of lbufs obtain good memory utilization by allocating and freeing
 * whole pages, exclusively, splitting these into 2Kbyte buffers
 * initializing NTXBUF/NRXBUF at initialization time.
 *
 * Almost all of the transmit and receive work is done by the wlc
 * os-independent "common" code:
 *	wlc_attach/detach()
 *	wlc_up/down()
 * 	wlc_reset()/init()
 *	wlc_send()
 *	wlc_isr()
 *	wlc_dpc()
 *	wlc_ioctl()
 *
 * Interrupts:
 * Transmit, Receive, and Error notification interrupts are enabled.
 * The ISR routine clears non-fatal interrupts and atomically passes
 * all set interrupt status bits to the DPC for further processing.
 * The DPC processes transmit, receive, and error interrupts.
 *
 * Locking:
 * A single private spinlock is acquired at the "top" of driver entry points
 * and released at the "bottom" of these entry points.
 * Thus there may be at most one thread actively transmitting or receiving
 * packets within the driver.  Since the ISR does not need to acquire
 * the private spinlock, it can execute concurrently with a driver thread.
 */

typedef struct wl_info {
	shared_info_t	sh;		/* shared NDIS-specific data */
	struct wl_oid	*oid;		/* OID handler state */
	struct wlc_pub	*pub;		/* pointer to public wlc state */
	struct wlc_info *wlc;		/* pointer to private common os-independent data */
	void *dhd;

	bool AllowIntfUp; /* In case of dongle contorl for when wl up should
					     happen. This is to allow the some of setting
						 like wme, country code etc. to be performed before
						 the wl up in case of reclaim image. Mainly used for
						 testing and controlled via "AllowIntfUp" registry
						 entry
					    */

	uint		ndisver;	/* Ndis version from NdisReadConfiguration */
	NDIS_INTERFACE_TYPE BusType;	/* Bus type from the registry */
	bool		piomode;	/* Set from ForcePIO key from the registry */

#ifndef NDIS60
	NDIS_MINIPORT_INTERRUPT intr;	/* interrupt object */
	uint		intrvector;	/* interrupt vector */
	uint		intrlevel;	/* interrupt level */
#else
	NDIS_HANDLE	intr;		/* interrupt object */
#endif /* NDIS60 */

	bool		txqstopped;	/* wlc txq is full */
	bool		dpclive;	/* dpc is scheduled or running */
	bool		intr_registered;	/* TRUE if interrupt is registered */

	bool		link;		/* link status */
	bool		down_pending;	/* down pending */
	bool		linkdown_pending;	/* link down indication pending */

	NDIS_SPIN_LOCK	tx_lock;	/* tx lock */
	NDIS_SPIN_LOCK	lock;		/* perimeter driver lock */
	NDIS_SPIN_LOCK	dpclock;	/* dpc lock */
	NDIS_SPIN_LOCK	dhdlock;	/* dhd lock */
	NDIS_SPIN_LOCK	dhd_tx_queue_lock;	/* tx queue lock */
	NDIS_SPIN_LOCK	dhd_rx_queue_lock;	/* rx queue lock */
	NDIS_SPIN_LOCK	dhd_evq_lock;	/* rx queue lock */
	NDIS_SPIN_LOCK	rx_lock;
	volatile uint	callbacks;	/* # outstanding callback functions */

	struct npq	txq;		/* first stage transmit queue (ND_PKT) */
#ifdef EXT_STA
	struct npq	tx_mid_q;	/* middle stage transmit queue (ND_PKT) */
#endif /* EXT_STA */

	int		ntimers;
#if !!defined(EXT_STA) || defined(WLBTAMP_HIGH_ONLY)
	struct wl_timer	timer[MAX_TIMERS];	/* timers for wlc  */
#endif /* !(BCMDONGLEHOST && !EXT_STA) */
	bool		timer_stop;	/* bypass timer callbacks */

	bool		scan_overrides;	/* true if any of the scan times have been overridden by
					 * Reg entries
					 */
	bool context;	 /* Context in which the registry needs to be handled during power on/off */

#ifndef NDIS60
	bool		shutdownhandler; /* shutdown handler registered */
#endif /* NDIS60 */

	void		*regsva;	/* pointer to core registers */
	uint		regsmaplen;	/* # bytes mapped chip registers */

#ifndef NDIS60
	bool		gotmapregs;	/* map registers have been allocated */
#endif /* NDIS60 */
	bool		gotspinlocks;	/* spin locks have been initialized */
#ifdef NDIS60
	NDIS_OID_REQUEST	*pended_oid;	/* pended OID awaiting asynch completion */
#endif /* NDIS60 */

	CHAR		DriverDesc[MAX_DRV_DESC]; /* counted string for Driver Description */

	CHAR		Brand[WL_BRAND_MAX];

	/* transmit stat counters */
	uint32	txenq;		/* total tx stack packets enqueued */
	uint32	txpacket;	/* total tx stack packets */
	uint32	txcomplete;	/* total tx stack packets completed */
	uint32	prev_txcomplete;	/* snap shot of total tx stack packets completed */

	NDIS_PHYSICAL_ADDRESS bar0;
	uint32	txq_count;			/* Keep track wl timer txq progress */
	uint32	tx_mid_q_count;	/* Keep track wl timer txq progress */
#if NDIS_WDM
#endif /* NDIS_WDM */

#ifdef STA
	/* for auto connect */
	bool			auto_connect;	/* flag for auto connection */
	NDIS_802_11_SSID	auto_ssid;	/* ssid */
	ULONG			auto_auth;	/* authentication mode */
	ULONG			auto_infra;	/* infrastructure mode */
	wl_wsec_key_t		auto_wep_key;	/* WEP info */
#ifdef BCMSUP_PSK
	bool			sup_enable_wpa;	/* in-driver sup enabled? */
	bool			unblock_8021x;	/* un-block 802.1x forced */
#endif /* BCMSUP_PSK */
#ifdef EXT_STA
	uint32		pwk;
	uint32		blocks;		/* open indication block bitmap */
	ULONG		link_report;	/* period link speed and link quality report timer */
	mbool		last_phyind;	/* last reported hardware phy state */
	uint32	priority_vlan_tag;	/* registry value: PriorityVLANTag */
#endif /* EXT_STA */
#endif /* STA */

	uint32		wsec_authorization_status; /* opaque wsec authorization status */

	uint32		ntxbuf;		/* number of tx buffers */
	uint32		nrxbuf;		/* number of rx buffers */
	uint32		rxind;		/* total rx packets indicated up to stack */
	uint32		rxreturn;	/* total rx packets returned from stack */
	uint32		rxnondisbuf;	/* number of NDIS buffer allocation failures */

	void		*pch;		/* pc device handle */

	bool		legacy_brcm_apps;	/* flag for legacy apps */
	bool		abrateconvert;	/* afterburner bit rate fixed to 125Mbps in report */
#ifdef EXT_STA
	struct wl_timer		*process_mid_txq_timer;	/* timers for wlc  */
#endif /* EXT_STA */
#if defined(WL_IW_USE_ISCAN) || defined(SIMPLE_ISCAN)
	struct wl_timer	*iscan_timer;	/* timers for iscan  */
	void		*iscan;	/* data for iscan  */
#endif /* WL_IW_USE_ISCAN */
	wl_toe_info_t	*toei;		/* pointer to toe specific information */
	wl_arp_info_t	*arpi;		/* pointer to arp agent offload info */
	ULONG		wakeup_flags;	/* wakeup flags as set by NDIS */
	bool		acpwr;		/* computer on AC or not */
	bool		wowl_cap;	/* capability cached for convenience */
	int		wowl_cap_override; /* Override by registry */
	int		wowl_key_rot;	/* Enable broadcast key rotation */
#ifdef TOE
	NIC_TASK_OFFLOAD	NicTaskOffload;
	NIC_CHECKSUM_OFFLOAD	NicChecksumOffload;
	bool			OffloadEnable;
	NDIS_ENCAPSULATION_FORMAT	EncapsulationFormat;
#endif /* TOE */

	uint32		tx_no_progress;		/* No forward Tx movement */
	uint32		tx_hang_detected;	/* total tx hang condition detected */

#if defined(EXT_STA) || defined(DHD_NDIS_OID)
	wlc_bss_info_t	current_bss;
	int		bss_good;
	bool	devhalted;
#endif /* EXT_STA */

#if defined(BCMDHDUSB)
	bool	dnglResetAfterDisable;
#endif /* BCMDHDUSB */
#ifdef EXT_STA
	ULONG		PacketFilter;		/* 802.11 packet filter */

	/* The following variable is to used only by wl.exe  to store the PMK between */
	/* subsequent invocations of the utility, and are not used by Native WiFi */
	wsec_pmk_t psk_info;
	int32 next_wsec;
#endif

	TCHAR sdHostCtrlName[25];
	bool in_scan;
	int PNPMethod;

#ifdef WLBTAMP
#if defined(WLBTWUSB) || defined(WLBTCEAMP11)
	/* Bluetooth context */
	DEVICE_EXTENSION bt_ext;
#endif
#endif /* WLBTAMP */
#ifdef P2P
	BOOL isVDev;
#endif  /* P2P */

#ifdef __BBSOFT__
	BOOL apEnable;
	CHAR apSSID[2][33];
	
	BOOL probeEnable;
	CHAR probeSSID[2][33];
	int probeIndex;
	
	int channel_num;
	uint16 channel_list[WL_NUMCHANNELS];
	
	BOOL dbgBSSList;
	BOOL dbgBSSEvent;
#endif //__BBSOFT__

} wl_info_t;


#if defined(EXT_STA) || defined(WLBTAMP_HIGH_ONLY)
#ifndef WLBTAMP_HIGH_ONLY
#pragma pack(push, 1)
typedef struct DNGL_PACKED_DOT11_BSS_ENTRY {
	ULONG uPhyId;
	DOT11_BSS_ENTRY_PHY_SPECIFIC_INFO PhySpecificInfo;
	DOT11_MAC_ADDRESS dot11BSSID;
	DOT11_BSS_TYPE dot11BSSType;
	LONG lRSSI;
	ULONG uLinkQuality;
	BOOLEAN bInRegDomain;
	USHORT usBeaconPeriod;
	ULONGLONG ullTimestamp;
	ULONGLONG ullHostTimestamp;
	USHORT usCapabilityInformation;
	ULONG uBufferLength;
	UCHAR ucBuffer[1];
} DNGL_PACKED_DOT11_BSS_ENTRY, * PDNGL_PACKED_DOT11_BSS_ENTRY;

typedef struct DNGL_DOT11_STATISTICS {
	NDIS_OBJECT_HEADER Header;
	ULONGLONG ullFourWayHandshakeFailures;
	ULONGLONG ullTKIPCounterMeasuresInvoked;
	ULONGLONG ullReserved;

	DOT11_MAC_FRAME_STATISTICS MacUcastCounters;
	DOT11_MAC_FRAME_STATISTICS MacMcastCounters;
	DOT11_PHY_FRAME_STATISTICS PhyCounters[1];
} DNGL_DOT11_STATISTICS, * PDNGL_DOT11_STATISTICS;

typedef struct _DNGL_PACKED_DOT11_ASSOCIATION_INFO_EX {
	DOT11_MAC_ADDRESS PeerMacAddress;
	DOT11_MAC_ADDRESS BSSID;
	USHORT usCapabilityInformation;
	USHORT usListenInterval;
	UCHAR ucPeerSupportedRates[MAX_NUM_SUPPORTED_RATES_V2];
	USHORT usAssociationID;
	DOT11_ASSOCIATION_STATE dot11AssociationState;
	DOT11_POWER_MODE dot11PowerMode;
	LARGE_INTEGER liAssociationUpTime;
	ULONGLONG ullNumOfTxPacketSuccesses;
	ULONGLONG ullNumOfTxPacketFailures;
	ULONGLONG ullNumOfRxPacketSuccesses;
	ULONGLONG ullNumOfRxPacketFailures;
} DNGL_PACKED_DOT11_ASSOCIATION_INFO_EX, *PDNGL_PACKED_DOT11_ASSOCIATION_INFO_EX;

typedef struct _DNGL_PACKED_DOT11_ASSOCIATION_INFO_LIST {
	#define DOT11_ASSOCIATION_INFO_LIST_REVISION_1  1
	NDIS_OBJECT_HEADER Header;
	ULONG uNumOfEntries;
	ULONG uTotalNumOfEntries;
	DNGL_PACKED_DOT11_ASSOCIATION_INFO_EX dot11AssocInfo[1];
} DNGL_PACKED_DOT11_ASSOCIATION_INFO_LIST, *PDNGL_PACKED_DOT11_ASSOCIATION_INFO_LIST;
#pragma pack(pop)

/* EXT_STA events and OIDs are contending for the bus simultaneously.
 * Need to serialize OIDs and events by handling both asynchronously.
 * Under Vista/EXT_STA, rx pkts need to be serialized with events as well.
 *
 * This is common data structure to serialize OIDs, events, and rx pkts.
 */
typedef struct _ndis_oidreq {
	PNDIS_OID_REQUEST pendingOidReq;
	NDIS_OID oid;
	NDIS_REQUEST_TYPE reqType;
	PVOID infoBuf;
	ULONG infoBufLen;
	ULONG outBufLen;
	PULONG bytesRead;
	PULONG bytesWritten;
	PULONG bytesNeeded;
} ndis_oidreq_t;

typedef struct _rxpkt_request {
	void *pkt;
	void *rcb;
} rxpkt_req_t;

typedef struct _ioctl_request {
	ULONG cmd;
	PVOID infoBuf;
	ULONG infoBufLen;
	BOOLEAN freeInfoBuf;
} ioctl_req_t;
#endif	/* !WLBTAMP_HIGH_ONLY */

#ifdef WLBTAMP_HIGH_ONLY
typedef struct _bta_request {
	PVOID infoBuf;
	ULONG infoBufLen;
	BOOLEAN freeInfoBuf;
} bta_req_t;
#endif

typedef void (*async_req_cb_t)(struct _async_req *req, void *arg);

typedef struct _async_req
{
	LIST_ENTRY list; /* This must be the first entry */
	async_req_cb_t cb;
	void *arg;
	int ifidx;

	union {
#ifndef WLBTAMP_HIGH_ONLY
		ndis_oidreq_t oidreq;
		rxpkt_req_t rxpkt;
		ioctl_req_t ioctlreq;
#else
		bta_req_t	btareq;
#endif
	} parms;
} async_req_t;

extern int dhd_async_req_schedule(wl_info_t *wl, async_req_t *req);
#endif /* EXT_STA */

#define WL_HEADROOM_RSVD 90		/* Headroom reserved in PKTFRMNATIVE */
#ifndef AP_ENAB
#define AP_ENAB(wl) (0)
#endif

#ifdef STA
#define wl_auto_connect(a) do {} while (0)
#ifndef NDIS60
#define wl_read_auto_config(a, b, c) do {} while (0)
#else
#define wl_read_auto_config(a) do {} while (0)
#endif /* NDIS60 */
#endif  /* STA */

#if defined(P2P) && !defined(NDIS60)

#define P2PCOOKIE	0xc00ceeee

#undef WL_INFO
#define WL_INFO(devicestate)	(gP2P.wl_instance_wl? gP2P.wl_instance_wl : devicestate)
#define P2PUNITNUMBER()		((MiniportAdapterContext == ((NDIS_HANDLE)P2PCOOKIE)) ? 1 : 0)
#define P2PSAVESTATUS(s)		(p2psavestatus(s))

#define GET_NDIS_P2PUNITNUMBER(p)	(p->MiniportReserved[4])
#define SET_NDIS_P2PUNITNUMBER(p, n)	(p->MiniportReserved[4]  = n)


struct p2p_state {
	NTSTATUS	wl_instance_rval;	/* last return value for wl_minit */
	wl_info_t	*wl_instance_wl;	/* pointer to primary instance (BCMSDDHD1) */
	BOOL		wl_p2p_vif_up;
	NDIS_HANDLE	p2p_vip_AdapterHandle;
	int		wl_instance_i;
};

extern struct p2p_state gP2P;

#define P2PHANDLE() (gP2P.p2p_vip_AdapterHandle)
#define WL_ISVDEV(wl) (wl->isVDev)
#define WL_HANDLE(wl) (wl->isVDev ? P2PHANDLE() : wl->sh.adapterhandle)

#else
#define P2PHANDLE()                      (NULL)
#define WL_ISVDEV(wl)                    (FALSE)
#define WL_HANDLE(wl)                    (wl->sh.adapterhandle)

#define P2PSAVESTATUS(S)		(S)
#define P2PUNITNUMBER()			(0)
#define GET_NDIS_P2PUNITNUMBER(p)	(0)
#define SET_NDIS_P2PUNITNUMBER(p, n)

#endif /* P2P */

extern int dhd_probe(shared_info_t *sh, void * wl, void* bar, void** regsva, uint irq, void ** dhd);
extern void dhd_remove(osl_t *osh, void *instance, shared_info_t *sh);
extern int dhd_start_xmit(void *bus, int ifidx, void *pktbuf);
extern int dhd_bus_ioctl(wl_info_t *wl, int oid, char *buf, uint buflen,
	PULONG bytes_read, PULONG bytes_needed, bool set);
#if defined(NDIS51) || defined(NDIS60)
extern void dhd_pnp_surp_remove(void *instance);
#endif /* NDIS51 || NDIS60 */
extern int wl_up(wl_info_t *wl);
extern void wl_down(wl_info_t *wl);
extern int wl_reboot(wl_info_t *wl);


#ifdef _WIN32
extern void sdstd_isr(PBOOLEAN InterruptRecognized, PBOOLEAN QueueMiniportHandleInterrupt,
	NDIS_HANDLE MiniportAdapterContext);
extern void sdstd_dpc(NDIS_HANDLE MiniportAdapterContext);
#endif /* _WIN32 */
extern NDIS_STATUS wl_mqueryinformation(NDIS_HANDLE MiniportAdapterContext, NDIS_OID Oid,
	PVOID InfoBuf, ULONG InfoBufLen, PULONG BytesWritten, PULONG BytesNeeded);
extern NDIS_STATUS wl_msetinformation(NDIS_HANDLE MiniportAdapterContext, NDIS_OID Oid,
	PVOID InfoBuf, ULONG InfoBufLen, PULONG BytesRead, PULONG BytesNeeded);
#ifdef NDIS60
extern NDIS_STATUS wl_method_oid(NDIS_HANDLE MiniportAdapterContext, NDIS_OID Oid,
	PVOID InfoBuf, ULONG InBufLen, ULONG OutBufLen, PULONG BytesWritten,
	PULONG BytesRead, PULONG BytesNeeded, PNDIS_OID_REQUEST OidRequest);
#endif /* NDIS60 */

#ifdef EXT_STA
extern void wl_process_mid_txq(wl_info_t *wl);
extern void wl_tx_convert_d11hdr(wl_info_t *wl, struct lbuf *lb, struct ether_header **peh);
#endif /* EXT_STA */
extern void wl_process_txq(wl_info_t *wl);
extern void wl_sendcomplete(wl_info_t *wl);
#ifndef EXT_STA
extern void wl_indicate_link_up(wl_info_t *wl);
extern void wl_indicate_link_down(wl_info_t *wl);
#endif /* !EXT_STA */
extern void wl_ndisevent(wl_info_t *wl, wl_event_msg_t *e, void *data);
extern void wl_flushtxq(wl_info_t *wl);
extern int wl_iovar_setint(wl_info_t *wl, const char *name, int val);
extern int wl_iovar_getint(wl_info_t *wl, const char *name, int *val);

extern void wl_sendup(wl_info_t *wl, struct wl_if *wlif, void *pkt, int numpkt);
extern void wl_txflowcontrol(wl_info_t *wl, bool state, int prio);

/* APIs exported by NIC/Dongle specific modules */

extern void wl_shutdown(wl_info_t *wl);
extern uint32 wl_getstat(wl_info_t *wl, NDIS_OID oid);

/* dhd/nhd-specific pnp power routines */
extern void wl_pnp_set_wifioffevent(wl_info_t *wl);
extern NDIS_STATUS wl_pnp_set_power(wl_info_t *wl, int devicestate);

/* shared pnp code, for assoc_recreate */
extern NDIS_STATUS wl_shared_pnp_set_power(wl_info_t *wl, int devicestate, int changestate);

#ifndef EXT_STA
extern void wl_link_up(wl_info_t *wl);
extern void wl_link_down(wl_info_t *wl);
#endif /* EXT_STA */
extern int wl_ioctl(wl_info_t *wl, uint cmd, void *buf, uint len);
extern NDIS_STATUS wl_queryoid(wl_info_t *wl, NDIS_OID Oid, PVOID InfoBuf, ULONG InfoBufLen,
	PULONG BytesWritten, PULONG BytesNeeded, int ifnet);
extern NDIS_STATUS wl_setoid(wl_info_t *wl, NDIS_OID Oid, PVOID InfoBuf, ULONG InfoBufLen,
	PULONG BytesRead, PULONG BytesNeeded, int ifnet);
extern int wl_iovar_op(wl_info_t *wl, const char *name, void *buf, uint len, bool set);
extern bool wl_query_legacy_oid(wl_info_t *wl, NDIS_OID Oid, PVOID InfoBuf, ULONG InfoBufLen,
	PULONG BytesWritten, NDIS_STATUS *status);

extern void *dhd_bus_pub(struct dhd_bus *bus);

#ifdef DHD_NDIS_OID
wlc_bss_info_t * wl_get_current_bss(wl_info_t *wl);
NDIS_STATUS wl_band_state_update(wl_info_t *wl);
bool wlc_rateset_isofdm(uint count, uint8 *rates);
#endif /* DHD_NDIS_OID */

#if defined(WL_IW_USE_ISCAN) || defined(SIMPLE_ISCAN)
#define ISCAN_RETRY_CNT   5
#define ISCAN_STATE_IDLE   0
#define ISCAN_STATE_SCANING 1
#define ISCAN_STATE_PENDING 2

/* the buf lengh can be WLC_IOCTL_MAXLEN (8K) to reduce iteration */
#define WLC_IW_ISCAN_MAXLEN   2048
typedef struct iscan_buf {
	struct iscan_buf * next;
#if defined(EXT_STA) && defined(SIMPLE_ISCAN)
	long	time;
#endif
	char   iscan_buf[WLC_IW_ISCAN_MAXLEN];
} iscan_buf_t;

typedef struct iscan_info {
#if defined(NDIS60)
	NDIS_HANDLE IScanWorkItem;
#else
	NDIS_WORK_ITEM IScanWorkItem;
#endif /* NDIS60 && !UNDER_CE */

	int    iscan_state;
	uint32 timer_on;
	struct wl_timer 	iscan_timer;	/* XP DTM WAR  */
	struct wl_timer *	timer;	/* XP DTM WAR  */
	uint32 timer_ms;
	uint32 timer_retry_ms;
	uint32 timer_retry_cnt;
	bool   NDISradio_disabled;
	iscan_buf_t * list_hdr;
	iscan_buf_t * list_cur;
	int	dev_state;
	char ioctlbuf[WLC_IOCTL_SMLEN];
#if defined(NDIS60)
	DOT11_BSS_TYPE BSSType;
	DOT11_SCAN_TYPE ScanType;
	NDIS_SPIN_LOCK SpinLock;
#endif
} iscan_info_t;
bool wl_iscan_del_timer(struct wl_timer *t);
struct wl_timer * wl_init_timer(wl_info_t *wl, void (*fn)(void* arg), void* arg, const char *name);
void wl_add_timer(wl_info_t *wl, struct wl_timer *t, uint ms, int periodic);
bool wl_del_timer(wl_info_t *wl, struct wl_timer *t);
#endif /* WL_IW_USE_ISCAN */
#endif /* _wl_ndis_h_ */
