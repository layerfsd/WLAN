/*
 * Broadcom Event  protocol definitions
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 *
 * Dependencies: proto/bcmeth.h
 *
 * $Id: bcmevent.h,v 9.34.4.1.20.16 2009/09/25 23:52:38 Exp $
 *
 */

/*
 * Broadcom Ethernet Events protocol defines
 *
 */

#ifndef _BCMEVENT_H_
#define _BCMEVENT_H_

#ifndef _TYPEDEFS_H_
#include <typedefs.h>
#endif

/* This marks the start of a packed structure section. */
#include <packed_section_start.h>

#define BCM_EVENT_MSG_VERSION		1	/* wl_event_msg_t struct version */
#define BCM_MSG_IFNAME_MAX		16	/* max length of interface name */

/* flags */
#define WLC_EVENT_MSG_LINK		0x01	/* link is up */
#define WLC_EVENT_MSG_FLUSHTXQ		0x02	/* flush tx queue on MIC error */
#define WLC_EVENT_MSG_GROUP		0x04	/* group MIC error */

/* these fields are stored in network order */
typedef BWL_PRE_PACKED_STRUCT struct
{
	uint16	version;
	uint16	flags;			/* see flags below */
	uint32	event_type;		/* Message (see below) */
	uint32	status;			/* Status code (see below) */
	uint32	reason;			/* Reason code (if applicable) */
	uint32	auth_type;		/* WLC_E_AUTH */
	uint32	datalen;		/* data buf */
	struct ether_addr	addr;	/* Station address (if applicable) */
	char	ifname[BCM_MSG_IFNAME_MAX]; /* name of the packet incoming interface */
} BWL_POST_PACKED_STRUCT wl_event_msg_t;

/* used by driver msgs */
typedef BWL_PRE_PACKED_STRUCT struct bcm_event {
	struct ether_header eth;
	bcmeth_hdr_t		bcm_hdr;
	wl_event_msg_t		event;
	/* data portion follows */
} BWL_POST_PACKED_STRUCT bcm_event_t;

#define BCM_MSG_LEN	(sizeof(bcm_event_t) - sizeof(bcmeth_hdr_t) - sizeof(struct ether_header))

/* Event messages */
#define WLC_E_SET_SSID		0	/* indicates status of set SSID */
#define WLC_E_JOIN		1	/* differentiates join IBSS from found (WLC_E_START) IBSS */
#define WLC_E_START		2	/* STA founded an IBSS or AP started a BSS */
#define WLC_E_AUTH		3	/* 802.11 AUTH request */
#define WLC_E_AUTH_IND		4	/* 802.11 AUTH indication */
#define WLC_E_DEAUTH		5	/* 802.11 DEAUTH request */
#define WLC_E_DEAUTH_IND	6	/* 802.11 DEAUTH indication */
#define WLC_E_ASSOC		7	/* 802.11 ASSOC request */
#define WLC_E_ASSOC_IND		8	/* 802.11 ASSOC indication */
#define WLC_E_REASSOC		9	/* 802.11 REASSOC request */
#define WLC_E_REASSOC_IND	10	/* 802.11 REASSOC indication */
#define WLC_E_DISASSOC		11	/* 802.11 DISASSOC request */
#define WLC_E_DISASSOC_IND	12	/* 802.11 DISASSOC indication */
#define WLC_E_QUIET_START	13	/* 802.11h Quiet period started */
#define WLC_E_QUIET_END		14	/* 802.11h Quiet period ended */
#define WLC_E_BEACON_RX		15	/* BEACONS received/lost indication */
#define WLC_E_LINK		16	/* generic link indication */
#define WLC_E_MIC_ERROR		17	/* TKIP MIC error occurred */
#define WLC_E_NDIS_LINK		18	/* NDIS style link indication */
#define WLC_E_ROAM		19	/* roam attempt occurred: indicate status & reason */
#define WLC_E_TXFAIL		20	/* change in dot11FailedCount (txfail) */
#define WLC_E_PMKID_CACHE	21	/* WPA2 pmkid cache indication */
#define WLC_E_RETROGRADE_TSF	22	/* current AP's TSF value went backward */
#define WLC_E_PRUNE		23	/* AP was pruned from join list for reason */
#define WLC_E_AUTOAUTH		24	/* report AutoAuth table entry match for join attempt */
#define WLC_E_EAPOL_MSG		25	/* Event encapsulating an EAPOL message */
#define WLC_E_SCAN_COMPLETE	26	/* Scan results are ready or scan was aborted */
#define WLC_E_ADDTS_IND		27	/* indicate to host addts fail/success */
#define WLC_E_DELTS_IND		28	/* indicate to host delts fail/success */
#define WLC_E_BCNSENT_IND	29	/* indicate to host of beacon transmit */
#define WLC_E_BCNRX_MSG		30	/* Send the received beacon up to the host */
#define WLC_E_BCNLOST_MSG	31	/* indicate to host loss of beacon */
#define WLC_E_ROAM_PREP		32	/* before attempting to roam */
#define WLC_E_PFN_NET_FOUND	33	/* PFN network found event */
#define WLC_E_PFN_NET_LOST	34	/* PFN network lost event */
#define WLC_E_RESET_COMPLETE	35
#define WLC_E_JOIN_START	36
#define WLC_E_ROAM_START	37
#define WLC_E_ASSOC_START	38
#define WLC_E_IBSS_ASSOC	39
#define WLC_E_RADIO		40
#define WLC_E_PSM_WATCHDOG	41	/* PSM microcode watchdog fired */
#define WLC_E_PROBREQ_MSG       44      /* probe request received */
#define WLC_E_SCAN_CONFIRM_IND  45
#define WLC_E_PSK_SUP	46	/* WPA Handshake fail */
#define WLC_E_COUNTRY_CODE_CHANGED 47
#define	WLC_E_EXCEEDED_MEDIUM_TIME 48	/* WMMAC excedded medium time */
#define WLC_E_ICV_ERROR		49	/* WEP ICV error occurred */
#define WLC_E_UNICAST_DECODE_ERROR 50 /* Unsupported unicast encrypted frame */
#define WLC_E_MULTICAST_DECODE_ERROR 51 /* Unsupported multicast encrypted frame */
#define WLC_E_TRACE 52
#ifdef WLBTAMP
#define WLC_E_BTA_HCI_EVENT	53	/* BT-AMP HCI event */
#endif
#define WLC_E_IF		54	/* I/F change (for dongle host notification) */
#define WLC_E_RSSI		56	/* indicate RSSI change based on configured levels */
#define WLC_E_PFN_SCAN_COMPLETE	57	/* PFN completed scan of network list */
#define WLC_E_ACTION_FRAME      58      /* Action frame reception */
#define WLC_E_ACTION_FRAME_COMPLETE 59  /* Action frame Tx complete */

#define WLC_E_ESCAN_RESULT 69               /* escan result event */
#define WLC_E_WAKE_EVENT	70	/* Wake Event timer fired, used for wake WLAN test mode */
#define WLC_E_LAST		71	/* highest val + 1 for range checking */
	/* Reminder: when adding events and changing the value of WLC_E_LAST, also
	 * update the 2 switch statements in wlc.c that end in a check for the
	 * numerical value of WLC_E_LAST.
	 */


/* Event status codes */
#define WLC_E_STATUS_SUCCESS		0	/* operation was successful */
#define WLC_E_STATUS_FAIL		1	/* operation failed */
#define WLC_E_STATUS_TIMEOUT		2	/* operation timed out */
#define WLC_E_STATUS_NO_NETWORKS	3	/* failed due to no matching network found */
#define WLC_E_STATUS_ABORT		4	/* operation was aborted */
#define WLC_E_STATUS_NO_ACK		5	/* protocol failure: packet not ack'd */
#define WLC_E_STATUS_UNSOLICITED	6	/* AUTH or ASSOC packet was unsolicited */
#define WLC_E_STATUS_ATTEMPT		7	/* attempt to assoc to an auto auth configuration */
#define WLC_E_STATUS_PARTIAL		8	/* scan results are incomplete */
#define WLC_E_STATUS_NEWSCAN	9	/* scan aborted by another scan */
#define WLC_E_STATUS_NEWASSOC	10	/* scan aborted due to assoc in progress */
#define WLC_E_STATUS_11HQUIET	11	/* 802.11h quiet period started */
#define WLC_E_STATUS_SUPPRESS	12	/* user disabled scanning (WLC_SET_SCANSUPPRESS) */
#define WLC_E_STATUS_NOCHANS	13	/* no allowable channels to scan */
#define WLC_E_STATUS_CCXFASTRM	14	/* scan aborted due to CCX fast roam */
#define WLC_E_STATUS_CS_ABORT	15	/* abort channel select */

/* roam reason codes */
#define WLC_E_REASON_INITIAL_ASSOC	0	/* initial assoc */
#define WLC_E_REASON_LOW_RSSI		1	/* roamed due to low RSSI */
#define WLC_E_REASON_DEAUTH		2	/* roamed due to DEAUTH indication */
#define WLC_E_REASON_DISASSOC		3	/* roamed due to DISASSOC indication */
#define WLC_E_REASON_BCNS_LOST		4	/* roamed due to lost beacons */
#define WLC_E_REASON_FAST_ROAM_FAILED	5	/* roamed due to fast roam failure */
#define WLC_E_REASON_DIRECTED_ROAM	6	/* roamed due to request by AP */
#define WLC_E_REASON_TSPEC_REJECTED	7	/* roamed due to TSPEC rejection */
#define WLC_E_REASON_BETTER_AP		8	/* roamed due to finding better AP */

/* prune reason codes */
#define WLC_E_PRUNE_ENCR_MISMATCH	1	/* encryption mismatch */
#define WLC_E_PRUNE_BCAST_BSSID		2	/* AP uses a broadcast BSSID */
#define WLC_E_PRUNE_MAC_DENY		3	/* STA's MAC addr is in AP's MAC deny list */
#define WLC_E_PRUNE_MAC_NA		4	/* STA's MAC addr is not in AP's MAC allow list */
#define WLC_E_PRUNE_REG_PASSV		5	/* AP not allowed due to regulatory restriction */
#define WLC_E_PRUNE_SPCT_MGMT		6	/* AP does not support STA locale spectrum mgmt */
#define WLC_E_PRUNE_RADAR		7	/* AP is on a radar channel of STA locale */
#define WLC_E_RSN_MISMATCH		8	/* STA does not support AP's RSN */
#define WLC_E_PRUNE_NO_COMMON_RATES	9	/* No rates in common with AP */
#define WLC_E_PRUNE_BASIC_RATES		10	/* STA does not support all basic rates of BSS */
#define WLC_E_PRUNE_CIPHER_NA		12	/* BSS's cipher not supported */
#define WLC_E_PRUNE_KNOWN_STA		13	/* AP is already known to us as a STA */
#define WLC_E_PRUNE_WDS_PEER		15	/* AP is already known to us as a WDS peer */
#define WLC_E_PRUNE_QBSS_LOAD		16	/* QBSS LOAD - AAC is too low */
#define WLC_E_PRUNE_HOME_AP		17	/* prune home AP */

/* WPA failure reason codes carried in the WLC_E_PSK_SUP event */
#define WLC_E_SUP_OTHER				0	/* Other reason */
#define WLC_E_SUP_DECRYPT_KEY_DATA	1	/* Decryption of key data failed */
#define WLC_E_SUP_BAD_UCAST_WEP128	2	/* Illegal use of ucast WEP128 */
#define WLC_E_SUP_BAD_UCAST_WEP40	3	/* Illegal use of ucast WEP40 */
#define WLC_E_SUP_UNSUP_KEY_LEN		4	/* Unsupported key length */
#define WLC_E_SUP_PW_KEY_CIPHER		5	/* Unicast cipher mismatch in pairwise key */
#define WLC_E_SUP_MSG3_TOO_MANY_IE	6	/* WPA IE contains > 1 RSN IE in key msg 3 */
#define WLC_E_SUP_MSG3_IE_MISMATCH	7	/* WPA IE mismatch in key message 3 */
#define WLC_E_SUP_NO_INSTALL_FLAG	8	/* INSTALL flag unset in 4-way msg */
#define WLC_E_SUP_MSG3_NO_GTK		9	/* encapsulated GTK missing from msg 3 */
#define WLC_E_SUP_GRP_KEY_CIPHER	10	/* Multicast cipher mismatch in group key */
#define WLC_E_SUP_GRP_MSG1_NO_GTK	11	/* encapsulated GTK missing from group msg 1 */
#define WLC_E_SUP_GTK_DECRYPT_FAIL	12	/* GTK decrypt failure */
#define WLC_E_SUP_SEND_FAIL			13	/* message send failure */
#define WLC_E_SUP_DEAUTH			14	/* received FC_DEAUTH */
#define WLC_E_SUP_WPA_PSK_TMO       15  /* WPA PSK 4-way handshake timeout */

/* I/F change code in WLC_E_IF event */
#define WLC_E_IF_ADD		1	/* add */
#define WLC_E_IF_DEL		2	/* delete */

/* This marks the end of a packed structure section. */
#include <packed_section_end.h>

#endif /* _BCMEVENT_H_ */
