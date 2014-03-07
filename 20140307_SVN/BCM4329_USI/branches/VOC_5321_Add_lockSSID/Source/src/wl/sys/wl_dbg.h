/*
 * Minimal debug/trace/assert driver definitions for
 * Broadcom 802.11 Networking Adapter.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wl_dbg.h,v 1.96.4.1.22.3 2009/02/06 23:18:53 Exp $
 */

#ifndef _wl_dbg_h_
#define _wl_dbg_h_

/* wl_msg_level is a bit vector with defs in wlioctl.h */
extern uint32 wl_msg_level;
extern uint32 wl_msg_level2;

#define WL_PRINT(args)		printf args

#ifdef BCMDBG

/* To disable a message completely ... until you need it again */
#define	WL_NONE(args)		do {if (wl_msg_level & 0) WL_PRINT(args);} while (0)

#define	WL_ERROR(args)		do {if (wl_msg_level & WL_ERROR_VAL) WL_PRINT(args);} while (0)
#define	WL_TRACE(args)		do {if (wl_msg_level & WL_TRACE_VAL) WL_PRINT(args);} while (0)
#define WL_PRHDRS(i, p, f, t, r, l) 	do { \
						if (wl_msg_level & WL_PRHDRS_VAL) \
							wlc_print_hdrs(i, p, f, t, r, l); \
					} while (0)
#define	WL_PRPKT(m, b, n)	do {if (wl_msg_level & WL_PRPKT_VAL) prhex(m, b, n);} while (0)
#define	WL_INFORM(args)		do {if (wl_msg_level & WL_INFORM_VAL) WL_PRINT(args);} while (0)
#define	WL_TMP(args)		do {if (wl_msg_level & WL_TMP_VAL) WL_PRINT(args);} while (0)
#define	WL_OID(args)		do {if (wl_msg_level & WL_OID_VAL) WL_PRINT(args);} while (0)
#define	WL_RATE(args)		do {if (wl_msg_level & WL_RATE_VAL) WL_PRINT(args);} while (0)
#define WL_ASSOC(args)		do {if (wl_msg_level & WL_ASSOC_VAL) WL_PRINT(args);} while (0)
#define	WL_PRUSR(m, b, n)	do {if (wl_msg_level & WL_PRUSR_VAL) prhex(m, b, n);} while (0)
#define WL_PS(args)		do {if (wl_msg_level & WL_PS_VAL) WL_PRINT(args);} while (0)
#define WL_TXPWR(args)		do {if (wl_msg_level & WL_TXPWR_VAL) WL_PRINT(args);} while (0)
#define WL_PORT(args)		do {if (wl_msg_level & WL_PORT_VAL) WL_PRINT(args);} while (0)
#define WL_DUAL(args)		do {if (wl_msg_level & WL_DUAL_VAL) WL_PRINT(args);} while (0)
#define WL_WSEC(args)		do {if (wl_msg_level & WL_WSEC_VAL) WL_PRINT(args);} while (0)
#define WL_WSEC_DUMP(args)	do {if (wl_msg_level & WL_WSEC_DUMP_VAL) WL_PRINT(args);} while (0)
#define WL_NRSSI(args)		do {if (wl_msg_level & WL_NRSSI_VAL) WL_PRINT(args);} while (0)
#define WL_LOFT(args)		do {if (wl_msg_level & WL_LOFT_VAL) WL_PRINT(args);} while (0)
#define WL_REGULATORY(args)	do {if (wl_msg_level & WL_REGULATORY_VAL) WL_PRINT(args);} while (0)
#define WL_PHYCAL(args)		do {if (wl_msg_level & WL_PHYCAL_VAL) WL_PRINT(args);} while (0)
#define WL_RADAR(args)		do {if (wl_msg_level & WL_RADAR_VAL) WL_PRINT(args);} while (0)
#define WL_MPC(args)		do {if (wl_msg_level & WL_MPC_VAL) WL_PRINT(args);} while (0)
#define WL_APSTA(args)		do {if (wl_msg_level & WL_APSTA_VAL) WL_PRINT(args);} while (0)
#define WL_DFS(args)		do {if (wl_msg_level & WL_DFS_VAL) WL_PRINT(args);} while (0)
#define WL_BA(args)		do {if (wl_msg_level & WL_BA_VAL) WL_PRINT(args);} while (0)
#define WL_MBSS(args)		do {if (wl_msg_level & WL_MBSS_VAL) WL_PRINT(args);} while (0)
#define	WL_PHYDBG(args)		do {if (wl_msg_level & 0) WL_PRINT(args);} while (0)
#define WL_CAC(args)		do {if (wl_msg_level & WL_CAC_VAL) WL_PRINT(args);} while (0)
#define WL_AMSDU(args)		do {if (wl_msg_level & WL_AMSDU_VAL) WL_PRINT(args);} while (0)
#define WL_AMPDU(args)		do {if (wl_msg_level & WL_AMPDU_VAL) WL_PRINT(args);} while (0)
#define WL_FFPLD(args)		do {if (wl_msg_level & WL_FFPLD_VAL) WL_PRINT(args);} while (0)
/* wl_msg_level is full. Use wl_msg_level_2 now */
#define WL_DPT(args)		do {if (wl_msg_level2 & WL_DPT_VAL) WL_PRINT(args);} while (0)
#define WL_ASSOC_OR_DPT(args)	do {if (WL_ASSOC_ON() || WL_DPT_ON()) WL_PRINT(args);} while (0)
#define WL_WOWL(args)		do {if (wl_msg_level2 & WL_WOWL_VAL) WL_PRINT(args);} while (0)
#define WL_SCAN(args)           do {if (wl_msg_level2 & WL_SCAN_VAL) WL_PRINT(args);} while (0)
#define	WL_COEX(args)		do {if (wl_msg_level2 & WL_COEX_VAL) WL_PRINT(args);} while (0)
#define WL_RTDC(w,s,i,j)	do {if (wl_msg_level2 & WL_RTDC_VAL) wlc_log(w,s,i,j);} while (0)
#define WL_RTDC2(w,s,i,j)	do {if (wl_msg_level2 & 0) wlc_log(w,s,i,j);} while (0)
#define WL_BTA(args)		do {if (wl_msg_level2 & WL_BTA_VAL) WL_PRINT(args);} while (0)

#define WL_ERROR_ON()		(wl_msg_level & WL_ERROR_VAL)
#define WL_TRACE_ON()		(wl_msg_level & WL_TRACE_VAL)
#define WL_PRHDRS_ON()		(wl_msg_level & WL_PRHDRS_VAL)
#define WL_PRPKT_ON()		(wl_msg_level & WL_PRPKT_VAL)
#define WL_INFORM_ON()		(wl_msg_level & WL_INFORM_VAL)
#define WL_TMP_ON()		(wl_msg_level & WL_TMP_VAL)
#define WL_OID_ON()		(wl_msg_level & WL_OID_VAL)
#define WL_RATE_ON()		(wl_msg_level & WL_RATE_VAL)
#define WL_ASSOC_ON()		(wl_msg_level & WL_ASSOC_VAL)
#define WL_PORT_ON()		(wl_msg_level & WL_PORT_VAL)
#define WL_WSEC_ON()		(wl_msg_level & WL_WSEC_VAL)
#define WL_WSEC_DUMP_ON()	(wl_msg_level & WL_WSEC_DUMP_VAL)
#define WL_MPC_ON()		(wl_msg_level & WL_MPC_VAL)
#define WL_REGULATORY_ON()	(wl_msg_level & WL_REGULATORY_VAL)
#define WL_PHYCAL_ON()		(wl_msg_level & WL_PHYCAL_VAL)
#define WL_APSTA_ON()		(wl_msg_level & WL_APSTA_VAL)
#define WL_DFS_ON()		(wl_msg_level & WL_DFS_VAL)
#define WL_BA_ON()		(wl_msg_level & WL_BA_VAL)
#define WL_MBSS_ON()		(wl_msg_level & WL_MBSS_VAL)
#define WL_AMPDU_ON()		(wl_msg_level & WL_AMPDU_VAL)
#define WL_DPT_ON()		(wl_msg_level2 & WL_DPT_VAL)
#define WL_SCAN_ON()		(wl_msg_level2 & WL_SCAN_VAL)
#define WL_BTA_ON()		(wl_msg_level2 & WL_BTA_VAL)

/* Extra message control for APSTA debugging */
#define WL_APSTA_UPDN_VAL	0x00000001 /* Config up/down related  */
#define WL_APSTA_BCN_VAL	0x00000002 /* Calls to beacon update  */
#define WL_APSTA_TX_VAL		0x00000004 /* Transmit data path */
#define WL_APSTA_RX_VAL		0x00000008 /* Receive data path  */
#define WL_APSTA_TSF_VAL	0x00000010 /* TSF-related items  */
#define WL_APSTA_BSSID_VAL	0x00000020 /* Calls to set bssid */

extern uint32 wl_apsta_dbg;

#define WL_APSTA_UPDN(args) do {if (wl_apsta_dbg & WL_APSTA_UPDN_VAL) {WL_APSTA(args);}} while (0)
#define WL_APSTA_BCN(args) do {if (wl_apsta_dbg & WL_APSTA_BCN_VAL) {WL_APSTA(args);}} while (0)
#define WL_APSTA_TX(args) do {if (wl_apsta_dbg & WL_APSTA_TX_VAL) {WL_APSTA(args);}} while (0)
#define WL_APSTA_RX(args) do {if (wl_apsta_dbg & WL_APSTA_RX_VAL) {WL_APSTA(args);}} while (0)
#define WL_APSTA_TSF(args) do {if (wl_apsta_dbg & WL_APSTA_TSF_VAL) {WL_APSTA(args);}} while (0)
#define WL_APSTA_BSSID(args) do {if (wl_apsta_dbg & WL_APSTA_BSSID_VAL) {WL_APSTA(args);}} while (0)

#else	/* BCMDBG */

/* To disable a message completely ... until you need it again */
#define	WL_NONE(args)

#ifdef BCMDBG_ERR
#define	WL_ERROR(args)		WL_PRINT(args)
#else
#define	WL_ERROR(args)
#endif /* BCMDBG_ERR */
#define	WL_TRACE(args)
#ifdef WLMSG_PRHDRS
#define WL_PRHDRS(i, p, f, t, r, l)	wlc_print_hdrs(i, p, f, t, r, l)
#else
#define	WL_PRHDRS(i, p, f, t, r, l)
#endif
#ifdef WLMSG_PRPKT
#define	WL_PRPKT(m, b, n)	prhex(m, b, n)
#else
#define	WL_PRPKT(m, b, n)
#endif
#ifdef WLMSG_INFORM
#define	WL_INFORM(args)		WL_PRINT(args)
#else
#define	WL_INFORM(args)
#endif
#define	WL_TMP(args)
#define WL_OID(args)
#define	WL_RATE(args)
#ifdef WLMSG_ASSOC
#define	WL_ASSOC(args)		WL_PRINT(args)
#else
#define	WL_ASSOC(args)
#endif
#define	WL_PRUSR(m, b, n)
#ifdef WLMSG_PS
#define WL_PS(args)		WL_PRINT(args)
#else
#define WL_PS(args)
#endif
#define WL_TXPWR(args)
#define WL_PORT(args)
#define WL_DUAL(args)
#ifdef WLMSG_WSEC
#define WL_WSEC(args)		WL_PRINT(args)
#define WL_WSEC_DUMP(args)	WL_PRINT(args)
#else
#define WL_WSEC(args)
#define WL_WSEC_DUMP(args)
#endif
#define WL_NRSSI(args)
#define WL_LOFT(args)
#define WL_REGULATORY(args)
#define WL_PHYCAL(args)
#define WL_RADAR(args)
#ifdef WLMSG_MPC
#define WL_MPC(args)		WL_PRINT(args)
#else
#define WL_MPC(args)
#endif
#define WL_APSTA(args)
#define WL_APSTA_UPDN(args)
#define WL_APSTA_BCN(args)
#define WL_APSTA_TX(args)
#define WL_APSTA_RX(args)
#define WL_APSTA_TSF(args)
#define WL_APSTA_BSSID(args)
#define WL_BA(args)
#define WL_MBSS(args)
#define	WL_PHYDBG(args)
#define	WL_CAC(args)
#define WL_AMSDU(args)
#define WL_AMPDU(args)
#define WL_FFPLD(args)
#define WL_DFS(args)
#define WL_WOWL(args)
#define WL_DPT(args)
#define WL_ASSOC_OR_DPT(args)
#ifdef WLMSG_SCAN
#define WL_SCAN(args)		WL_PRINT(args)
#else
#define WL_SCAN(args)
#endif
#define	WL_COEX(args)
#define WL_RTDC(w, s, i, j)
#define WL_RTDC2(w, s, i, j)
#ifdef WLMSG_BTA
#define WL_BTA(args)		printf args
#else
#define WL_BTA(args)
#endif

#ifdef BCMDBG_ERR
#define WL_ERROR_ON()		1
#else
#define WL_ERROR_ON()		0
#endif
#define WL_TRACE_ON()		0
#ifdef WLMSG_PRHDRS
#define WL_PRHDRS_ON()		1
#else
#define WL_PRHDRS_ON()		0
#endif
#ifdef WLMSG_PRPKT
#define WL_PRPKT_ON()		1
#else
#define WL_PRPKT_ON()		0
#endif
#ifdef WLMSG_INFORM
#define WL_INFORM_ON()		1
#else
#define WL_INFORM_ON()		0
#endif
#define WL_OID_ON()		0
#define WL_TMP_ON()		0
#define WL_RATE_ON()		0
#ifdef WLMSG_ASSOC
#define WL_ASSOC_ON()		1
#else
#define WL_ASSOC_ON()		0
#endif
#define WL_PORT_ON()		0
#ifdef WLMSG_WSEC
#define WL_WSEC_ON()		1
#define WL_WSEC_DUMP_ON()	1
#else
#define WL_WSEC_ON()		0
#define WL_WSEC_DUMP_ON()	0
#endif
#ifdef WLMSG_MPC
#define WL_MPC_ON()		1
#else
#define WL_MPC_ON()		0
#endif
#define WL_REGULATORY_ON()	0
#define WL_PHYCAL_ON()		0
#define WL_APSTA_ON()		0
#define WL_BA_ON()		0
#define WL_MBSS_ON()		0
#define WL_DFS_ON()		0
#define WL_DPT_ON()		0
#ifdef WLMSG_SCAN
#define WL_SCAN_ON()            1
#else
#define WL_SCAN_ON()            0
#endif
#ifdef WLMSG_BTA
#define WL_BTA_ON()		1
#else
#define WL_BTA_ON()		0
#endif
#endif /* BCMDBG */

#endif /* _wl_dbg_h_ */
