/*
 * NDIS-specific Windows registry configuration override
 * code for Broadcom 802.11abg Networking Device Driver
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wl_ndconfig.c,v 1.98.8.2.8.9 2010/02/09 13:23:30 Exp $
 */

#include <typedefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <siutils.h>
#include <wlioctl.h>
#include <proto/802.11.h>
#if defined(EXT_STA) || defined(DHD_NDIS_OID) || defined(WLBTAMP_HIGH_ONLY)
#include <wlc_cfg.h>
#endif /* EXT_STA || DHD_NDIS_OID || WLBTAMP_HIGH_ONLY */
#include <sbhndpio.h>
#include <sbhnddma.h>
#if defined(EXT_STA) || defined(DHD_NDIS_OID) || defined(WLBTAMP_HIGH_ONLY)
#include <hnddma.h>
#include <d11.h>
#include <wlc_rate.h>
#include <wlc_key.h>
#include <wlc_channel.h>
#include <wlc_pub.h>
#include <wlc_bsscfg.h>
#include <wlc.h>
#include <wlc_scan.h>
#include <wl_oid.h>
#endif /* EXT_STA || DHD_NDIS_OID || WLBTAMP_HIGH_ONLY */

#include <wl_dbg.h>

#include <wl_ndconfig.h>

/* This file is shared between the Windows NIC driver, Windows DHD
 * and the "retail" dongle builds.
 * It is not a miniport driver when it is part of the dongle build
 * and directly calls the wlc common code.
 */
#ifdef NDIS_MINIPORT_DRIVER
#include <wl_ndis.h>
#else
#define wl_ioctl(wl, cmd, buf, len) wlc_ioctl(wl, cmd, buf, len, NULL)
#define wl_iovar_setint(wl, name, arg) wlc_iovar_setint(wl, name, arg)
#define wl_iovar_op(wl, name, buf, len, set) wlc_iovar_op(wl, name, NULL, 0, buf, len, set, NULL)
#define wl_setoid(wl, oid, buf, len, bytes_read, bytes_needed, ifnet) \
	wl_set_oid(wl, oid, buf, len, bytes_read, bytes_needed)
#endif

ndis_config_t ndis_configs[] = {
	{ "Rate", NdisParameterInteger, 0, 0, NULL },
	{ "RateA", NdisParameterInteger, 0, 0, NULL },
	{ "featureflag", NdisParameterHexInteger, 0, 0, NULL},
	{ "Afterburner", NdisParameterInteger, 0, 0, NULL },
	{ "IBSSAllowed", NdisParameterInteger, 0, 0, NULL },
	{ "IBSSMode", NdisParameterInteger, 0, 0, NULL },
	{ "IBSSGProtection", NdisParameterInteger, WLC_SET_PROTECTION_CONTROL, 0, NULL },
	{ "LegacyMode", NdisParameterInteger, WLC_SET_GMODE, 0, NULL },
	{ "LegacyProbe", NdisParameterInteger, 0, 0, NULL },
	{ "band", NdisParameterInteger, WLC_SET_BAND, 0, NULL },
	{ "BTCoexist", NdisParameterInteger, 0, 0, NULL },
	{ "BTC_stuck_war", NdisParameterInteger, 0, 0, NULL },
	{ "ledbh0", NdisParameterInteger, WLC_SET_LED, SKIP_MINUS_1, NULL },
	{ "ledbh1", NdisParameterInteger, WLC_SET_LED, SKIP_MINUS_1, NULL },
	{ "ledbh2", NdisParameterInteger, WLC_SET_LED, SKIP_MINUS_1, NULL },
	{ "ledbh3", NdisParameterInteger, WLC_SET_LED, SKIP_MINUS_1, NULL },
	{ "ledblinkslow", NdisParameterInteger, 0, SKIP_MINUS_1, NULL },
	{ "ledblinkmed", NdisParameterInteger, 0, SKIP_MINUS_1, NULL },
	{ "ledblinkfast", NdisParameterInteger, 0, SKIP_MINUS_1, NULL },
	{ "Country", NdisParameterString, WLC_SET_COUNTRY, 0, NULL },
	{ "NetworkAddress", NdisParameterString, 0, 0, NULL },
	{ "WME", NdisParameterInteger, 0, 0, NULL },
	{ "WME_qosinfo", NdisParameterInteger, 0, 0, NULL },
#ifndef EXT_STA
	{ "vlan_mode", NdisParameterInteger, 0, 0, NULL },
#endif /* EXT_STA */
	{ "DriverDesc", NdisParameterString, 0, 0, NULL },
	{ "AdapterDesc", NdisParameterString, 0, 0, NULL },
	{ "WPA", NdisParameterInteger, 0, 0, NULL },
	{ "MsgLevel", NdisParameterInteger, 0, 0, NULL },
	{ "scan_channel_time", NdisParameterInteger, WLC_SET_SCAN_CHANNEL_TIME, SKIP_MINUS_1,
	NULL },
	{ "scan_unassoc_time", NdisParameterInteger, WLC_SET_SCAN_UNASSOC_TIME, SKIP_MINUS_1,
	NULL },
	{ "scan_home_time", NdisParameterInteger, WLC_SET_SCAN_HOME_TIME, SKIP_MINUS_1, NULL },
	{ "scan_passive_time", NdisParameterInteger, WLC_SET_SCAN_PASSIVE_TIME, SKIP_MINUS_1,
	NULL },
	{ "scan_passes", NdisParameterInteger, WLC_SET_SCAN_NPROBES, SKIP_MINUS_1, NULL },
	{ "Chanspec", NdisParameterString, 0, 0, NULL },
#ifndef __BBSOFT__
	{ "PowerSaveMode", NdisParameterInteger, WLC_SET_PM, SIMPLE_CONFIG, NULL },
#else //__BBSOFT__
	{ "powerSaveMode", NdisParameterInteger, WLC_SET_PM, SIMPLE_CONFIG, NULL },
#endif //__BBSOFT__
	{ "PLCPHeader", NdisParameterInteger, WLC_SET_PLCPHDR, SIMPLE_CONFIG, NULL },
	{ "antdiv", NdisParameterInteger, WLC_SET_ANTDIV, SIMPLE_CONFIG | SKIP_MINUS_1, NULL },
	{ "frag", NdisParameterInteger, 0, 0, NULL },
	{ "rts", NdisParameterInteger, 0, 0, NULL },
	{ "PwrOut", NdisParameterInteger, WLC_SET_PWROUT_PERCENTAGE, SIMPLE_CONFIG, NULL },
	{ "BandPref", NdisParameterInteger, WLC_SET_ASSOC_PREFER, SIMPLE_CONFIG, NULL },
	{ "AssocRoamPref", NdisParameterInteger, 0, 0, NULL },
	{ "RoamTrigger", NdisParameterInteger, 0, 0, NULL },
	{ "RoamDelta", NdisParameterInteger, 0, 0, NULL },

	{ "RoamScanPrd", NdisParameterInteger, WLC_SET_ROAM_SCAN_PERIOD, 0, NULL },
	{ "ListenInt", NdisParameterInteger, 0, 0, NULL },
	{ "WmeApsd", NdisParameterInteger, 0, 0, NULL },
	{ "ApsdTriggerPrd", NdisParameterInteger, 0, 0, NULL },
	{ "ApsdAutoTrigger", NdisParameterInteger, 0, 0, NULL },

	{ "RoamScanFreq", NdisParameterInteger, 0, 0, NULL }, /* Partial scan frequency */
	/* Full roam scan interval when using cached channel roaming */
	{ "FullRoamScanInterval", NdisParameterInteger, 0, 0, NULL },
	{ "FrameBursting", NdisParameterInteger, WLC_SET_FAKEFRAG, SIMPLE_CONFIG, NULL },
	{ "BadFramePreempt", NdisParameterInteger, WLC_SET_BAD_FRAME_PREEMPT, SIMPLE_CONFIG, NULL },
	{ "Interference_Mode", NdisParameterInteger, WLC_SET_INTERFERENCE_MODE, SIMPLE_CONFIG |
	SKIP_MINUS_1, NULL },
	{ "11HNetworks", NdisParameterInteger, WLC_SET_SPECT_MANAGMENT, SIMPLE_CONFIG |
	SKIP_MINUS_1, NULL },
#ifndef EXT_STA
	{ "RadioState", NdisParameterInteger, 0, 0, NULL },
#else /* EXT_STA */
	{ "NICPowerState", NdisParameterInteger, 0, 0, NULL },
#endif /* EXT_STA */
	{ "MPC", NdisParameterInteger, 0, 0, NULL },
	{ "ApCompatMode", NdisParameterInteger, 0, 0, NULL },
	{ "IBSSLink", NdisParameterInteger, 0, 0, NULL },
#ifdef BCMRECLAIM
	{ "Reclaim", NdisParameterInteger, 0, 0, NULL },
#endif /* BCMRECLAIM */
	{ "leddc", NdisParameterInteger, 0, 0, NULL },
	{ "EFCEnable", NdisParameterInteger, 0, 0, NULL },
	{ "BandwidthCap", NdisParameterInteger, 0, 0, NULL },
	{ "mimo_antsel", NdisParameterInteger, 0, SKIP_MINUS_1, NULL },
	{ "AbRateConvert", NdisParameterInteger, 0, SKIP_MINUS_1, NULL },
	{ "assoc_recreate", NdisParameterInteger, 0, SIMPLE_IOVAR_CONFIG | SKIP_MINUS_1, NULL },
	{ "11NPreamble", NdisParameterInteger, 0, 0, NULL },
	{ "Intolerant", NdisParameterInteger, 0, 0, NULL },
	{ "OBSSCoex", NdisParameterInteger, 0, 0, NULL },
#ifdef WLDPT
	{ "DPTEnable", NdisParameterInteger, 0, 0, NULL },
#endif /* WLDPT */
	{ "AdvPsPoll", NdisParameterInteger, 0, 0, NULL },
	{ "ht_wsec_restrict", NdisParameterInteger, 0, 0, NULL },
#ifdef __BBSOFT__
	{ "RoamThreshold", NdisParameterInteger, 0, 0, NULL },
	{ "RoamFloorThre", NdisParameterInteger, 0, 0, NULL },

	{ "dwellTime", NdisParameterInteger, WLC_SET_SCAN_UNASSOC_TIME, SKIP_MINUS_1,	NULL },
	{ "channelList", NdisParameterString, 0, 0, NULL },

	{ "dbgBSSList", NdisParameterInteger, 0, 0, NULL },
	{ "dbgBSSEvent", NdisParameterInteger, 0, 0, NULL },
	
	{ "apEnable", NdisParameterInteger, 0, 0, NULL },
	{ "apSSID", NdisParameterString, 0, 0, NULL },
	{ "apSSIDa", NdisParameterString, 0, 0, NULL },

	{ "probeEnable", NdisParameterInteger, 0, 0, NULL },
	{ "probeSSID", NdisParameterString, 0, 0, NULL },
	{ "probeSSIDa", NdisParameterString, 0, 0, NULL },
#endif //__BBSOFT__
	{ NULL, 0, 0, 0, NULL }
};

void
wl_readconfigoverrides(
	void *wl,
	NDIS_HANDLE confighandle,
	char *id,
	uint unit,
	uint OS
)
{
	int i;
	PNDIS_CONFIGURATION_PARAMETER param;
	int index;
	char ledbh[6];

	WL_TRACE(("wl_readconfigoverrides\n"));

#if defined(BCMDBG)
	/* read MsgLevel if it exists (redundant in wl_ndis.c but handy for other ports) */
	/* from wl_ndis.c. They will only be copied into the driver below if 32 bits or less. */
	if ((param = wl_readparam(ndis_configs, "MsgLevel", confighandle, wl, NULL))) {
		if (param->ParameterType != NdisParameterString) {
		       wl_msg_level = param->ParameterData.IntegerData;
		       WL_INFORM(("%s%d: wl_msg_level set to 0x%x from the registry\n", id, unit,
		                  wl_msg_level));
		}
	} else {
		WL_INFORM(("%s%d: wl_minit: wl_readparam(\"MsgLevel\") error\n", id, unit));
	}
#endif /* defined (BCMDBG) */


	/* read country code */
	if ((param = wl_readparam(ndis_configs, "Country", confighandle, wl, &index))) {
		/* null string means using default SROM country setting */
		if (param->ParameterData.StringData.Length) {
			char country_code[20];
			/* copy two character country code */
			if (((char *)param->ParameterData.StringData.Buffer)[1] == '\0')
				/* it's WCHAR--copy into ascii buffer */
				wchar2ascii(country_code, (ushort *) (param->ParameterData.
					StringData.Buffer), param->ParameterData.StringData.Length,
					sizeof(country_code));
			else
				strcpy(country_code, (char*)param->ParameterData.StringData.Buffer);
			wl_ioctl(wl, ndis_configs[index].cmd, (void *)country_code,
				strlen(country_code));
		}
	}

	/* read network address */
	if ((param = wl_readparam(ndis_configs, "NetworkAddress", confighandle, wl, NULL)) &&
		param->ParameterData.StringData.Length) {
		char addr[ETHER_ADDR_LEN*2 + 1];

		*addr = '\0';
		if (((char *)param->ParameterData.StringData.Buffer)[1] == '\0') {
			if (param->ParameterData.StringData.Length == ETHER_ADDR_LEN * 2 *
				sizeof(WCHAR))
				/* it's WCHAR--copy into ascii buffer */
				wchar2ascii(addr, (ushort *) (param->ParameterData.StringData.
					Buffer), param->ParameterData.StringData.Length,
					sizeof(addr));
		} else if (param->ParameterData.StringData.Length == ETHER_ADDR_LEN*2)
				strcpy(addr, (char*)param->ParameterData.StringData.Buffer);
		/* convert MAC addr string of form "XXXXXXXXXXXX" */
		if (*addr) {
			char addrbyte[3];
			unsigned char etheraddr[ETHER_ADDR_LEN];

			addrbyte[2] = '\0';
			for (i = 0; i < ETHER_ADDR_LEN; ++i) {
				bcopy(addr + (i*2), addrbyte, 2);
				etheraddr[i] = (uint8) bcm_strtoul(addrbyte, NULL, 16);
			}
			if (wl_iovar_op(wl, "cur_etheraddr", etheraddr,
			                sizeof(etheraddr), IOV_SET))
				WL_ERROR(("%s%d: wl_readconfigoverrides: error setting MAC addr"
					" override; buf: %s, len %d\n", id, unit,
					(char*)param->ParameterData.StringData.Buffer,
					param->ParameterData.StringData.Length));
		} else
			WL_ERROR(("%s%d: wl_readconfigoverrides: bad MAC addr override; buf: %s,"
				" len %d\n", id, unit,
				(char*)param->ParameterData.StringData.Buffer,
				param->ParameterData.StringData.Length));
	}

	/* read and set flags to enable various sw features (like AB) */
	if ((param = wl_readparam(ndis_configs, "featureflag", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "wlfeatureflag", param->ParameterData.IntegerData))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting wlfeatureflag\n",
				id, unit));
	}

#ifdef STA
	/* IBSS Lock Out */
	if ((param = wl_readparam(ndis_configs, "IBSSAllowed", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "ibss_allowed", param->ParameterData.IntegerData))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting ibss_allowed\n",
				id, unit));
	}
#endif /* STA */

#ifdef STA
	/* G Protection control for IBSS */
	if ((param = wl_readparam(ndis_configs, "IBSSGProtection", confighandle, wl, &index))) {
		int g_protection_ctl = -1;
		bool invalid = FALSE;

		if (param->ParameterData.IntegerData == 0) {
			g_protection_ctl = WLC_PROTECTION_CTL_OFF;
		} else if (param->ParameterData.IntegerData == 1) {
			g_protection_ctl = WLC_PROTECTION_CTL_LOCAL;
		} else if (param->ParameterData.IntegerData == 2) {
			g_protection_ctl = WLC_PROTECTION_CTL_OVERLAP;
		} else {
			WL_ERROR(("%s%d: wl_readconfigoverrides: invalid IBSSGProtection value:"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
				invalid = TRUE;
		}
		if (!invalid && wl_ioctl(wl, ndis_configs[index].cmd, &g_protection_ctl,
		                sizeof(g_protection_ctl)) == -1) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: error setting "
				"WLC_SET_GMODE_PROTECTION_CONTROL to: %d\n", id, unit,
				g_protection_ctl));
		}
	}

	/* Chanspec for IBSS */
	if ((param = wl_readparam(ndis_configs, "Chanspec", confighandle, wl, NULL))) {
		if (param->ParameterData.StringData.Length) {
			chanspec_t chspec = 0;
		        char chspec_str[20];
			/* copy two character chanspec code */
			if (((char *)param->ParameterData.StringData.Buffer)[1] == '\0')
				/* it's WCHAR--copy into ascii buffer */
				wchar2ascii(chspec_str, (ushort *) (param->ParameterData.
					StringData.Buffer), param->ParameterData.StringData.Length,
					sizeof(chspec_str));
			else
				strcpy(chspec_str, (char*)param->ParameterData.StringData.Buffer);
			if ((chspec = wf_chspec_aton(chspec_str)) == 0)
				WL_ERROR(("%s%d: wl_readconfigoverrides: Error converting"
					  " chanspec %s\n", id, unit, chspec_str));
			if (chspec && wl_iovar_setint(wl, "chanspec", (int)chspec))
				WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting"
					  " chanspec 0x%x\n", id, unit, chspec));
		}
	}

#endif /* STA */

	if ((param = wl_readparam(ndis_configs, "IBSSMode", confighandle, wl, NULL))) {
		int gmode;
		int nmode;

		switch (param->ParameterData.IntegerData) {
		case 0:		/* 802.11a/b */
			gmode = GMODE_LEGACY_B;
			nmode = OFF;
			break;
		case 1:		/* 802.11a/b/g LRS */
			/* LRS (Limited Rate Set) Mode for 11b compatibility */
			gmode = GMODE_LRS;
			nmode = OFF;
			break;
		case 2:		/* 802.11a/b/g Auto */
			gmode = GMODE_AUTO;
			nmode = OFF;
			break;
		case 3:		/* 802.11a/g Only */
			gmode = GMODE_ONLY;
			nmode = OFF;
			break;
		case 4:		/* 802.11a/b/g Performance */
			gmode = GMODE_PERFORMANCE;
			nmode = OFF;
			break;
		case 5:		/* 802.11a/b/g/n Auto */
			gmode = GMODE_AUTO;
			nmode = ON;
			break;
		default:
			gmode = -1;
			nmode = -2;
			WL_ERROR(("%s%d: wl_readconfigoverrides: "
			          "invalid IBSS Mode value: %lu\n",
			          id, unit, param->ParameterData.IntegerData));
			break;
		}

		/* save our gmode and nmode */
		wl_iovar_setint(wl, "ibss_gmode", gmode);
		wl_iovar_setint(wl, "ibss_nmode", nmode);

	}

	/* Legacy modes for STA */
	if ((param = wl_readparam(ndis_configs, "LegacyMode", confighandle, wl, &index))) {
		int gmode;
		int nmode;

		/* Defaults if an illegal value is given */
		gmode = GMODE_AUTO;
		nmode = OFF;

		/* 802.11b/g (and 802.11a if dual band) */
		if (param->ParameterData.IntegerData == 0) {
			gmode = GMODE_AUTO;
			nmode = OFF;
		}
		/* 802.11b only (and 802.11a if dual band) */
		else if (param->ParameterData.IntegerData == 1) {
			gmode = GMODE_LEGACY_B;
			nmode = OFF;
		}
		/* 802.11b/g/n (and 802.11a if dual band) */
		else if (param->ParameterData.IntegerData == 2) {
			gmode = GMODE_AUTO;
			nmode = AUTO;
		}

		if (wl_iovar_setint(wl, "nmode", nmode))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting"
			          " Nmode to %d for BSS Mode value: %d\n",
			          id, unit, param->ParameterData.IntegerData,
			          nmode));

		if (wl_ioctl(wl, ndis_configs[index].cmd, &gmode, sizeof(gmode)) == -1) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting"
			          " Gmode to %d for BSS Mode value: %d\n",
			          id, unit, param->ParameterData.IntegerData,
			          gmode));
		}
	}
	/* Should we use CCK rates only in probe requests for G modes? */
	if ((param = wl_readparam(ndis_configs, "LegacyProbe", confighandle, wl, &index)) &&
	    param->ParameterData.IntegerData == 0) {
	    wl_iovar_setint(wl, "legacy_probe", (int)param->ParameterData.IntegerData);
	}

	/* multiband: restrict the band (default=auto, a, b) */
	if ((param = wl_readparam(ndis_configs, "band", confighandle, wl, &index))) {
		int band = param->ParameterData.IntegerData;
		if (wl_ioctl(wl, ndis_configs[index].cmd, &band, sizeof(band)) != 0) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting band value: %d\n",
				id, unit, band));
		}
	}

	/* Read A band rate override. Does not require band locking */
	if ((param = wl_readparam(ndis_configs, "RateA", confighandle, wl, NULL))) {
		if (param->ParameterData.IntegerData != 0 &&
		    wl_iovar_setint(wl, "a_rate", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting A band rate override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	/* Read B band rate override. Does not require band locking */
	if ((param = wl_readparam(ndis_configs, "Rate", confighandle, wl, NULL))) {
		if (param->ParameterData.IntegerData != 0 &&
		    wl_iovar_setint(wl, "bg_rate", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting B band rate override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	/* fragthreshold override. */
	if ((param = wl_readparam(ndis_configs, "frag", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "fragthresh", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting fragthresh override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	/* rtsthreshold override. */
	if ((param = wl_readparam(ndis_configs, "rts", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "rtsthresh", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting rtsthresh override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	/* Allow BlueTooth coexistence (default=no) */
	if ((param = wl_readparam(ndis_configs, "BTCoexist", confighandle, wl, NULL)) &&
		(param->ParameterData.IntegerData == 1)) {
		if (wl_iovar_setint(wl, "btc_mode", WL_BTC_ENABLE))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting btc mode"
				" %lu\n", id, unit));
		else {
			WL_INFORM(("BT Coexistance enabled\n"));
			if ((param = wl_readparam(ndis_configs, "BTC_stuck_war", confighandle,
				wl, NULL)) &&
			    (param->ParameterData.IntegerData == 1)) {
				WL_INFORM(("BTC stuck WAR enabled\n"));
				wl_iovar_setint(wl, "btc_stuck_war", 1);
			}
		}

	}

	/* ledbh 0-3 */
	for (i = 0; i < 4; i++) {
		sprintf(ledbh, "ledbh%d", i);
		if ((param = wl_readparam(ndis_configs, ledbh, confighandle, wl, &index))) {
			wl_led_info_t ledi;
			ledi.index = i;
			ledi.behavior = (param->ParameterData.IntegerData & WL_LED_BEH_MASK) %
				WL_LED_NUMBEHAVIOR;
			ledi.activehi = (param->ParameterData.IntegerData & WL_LED_AL_MASK)? FALSE :
				TRUE;
			wl_ioctl(wl, ndis_configs[index].cmd, &ledi, sizeof(ledi));
		}
	}

	/* Read LED blink rate overrides */
	if ((param = wl_readparam(ndis_configs, "ledblinkslow", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "led_blinkslow",
			(uint32)(param->ParameterData.IntegerData)))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting blinkslow override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
	}

	if ((param = wl_readparam(ndis_configs, "ledblinkmed", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "led_blinkmed",
			(uint32)(param->ParameterData.IntegerData)))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting blinkmed override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
	}

	if ((param = wl_readparam(ndis_configs, "ledblinkfast", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "led_blinkfast",
			(uint32)(param->ParameterData.IntegerData)))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting blinkfast override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
	}

	/* Read WME settings */
	if ((param = wl_readparam(ndis_configs, "WME", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "wme", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting WME override %lu\n",
				id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "WME_qosinfo", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "wme_qosinfo",
		                    (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: "
			          "Error setting WME_qosinfo override %lu\n",
			          id, unit, param->ParameterData.IntegerData));
		}
	}

	/* Afterburner gmode.  Set after WME option since WME has priority */
	if ((param = wl_readparam(ndis_configs, "Afterburner", confighandle, wl, NULL)) &&
	    param->ParameterData.IntegerData == 1) {
		if (wl_iovar_setint(wl, "afterburner_override", AUTO))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error enabling Afterburner\n",
				id, unit));
	}

#ifndef EXT_STA
	/* Read VLAN Mode */
	if ((param = wl_readparam(ndis_configs, "vlan_mode", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "vlan_mode", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: "
			          "Error setting vlan_mode override %lu\n",
			          id, unit, param->ParameterData.IntegerData));
		}
	}
#endif /* !EXT_STA */


#ifndef EXT_STA
	if ((param = wl_readparam(ndis_configs, "RadioState", confighandle, wl, NULL))) {
		uint radiomaskval = WL_RADIO_SW_DISABLE << 16;

		radiomaskval += (param->ParameterData.IntegerData == 0) ? 0 : WL_RADIO_SW_DISABLE;
		if (wl_ioctl(wl, WLC_SET_RADIO, &radiomaskval, sizeof(radiomaskval)) == -1) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting RadioState override"
				" %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}
#else /* EXT_STA */
	/* retrieve cached value of wl->oid->sw_phy_state from registry */
	if ((param = wl_readparam(ndis_configs, "NICPowerState", confighandle, wl, NULL))) {
		((wl_info_t*)wl)->oid->sw_phy_state = param->ParameterData.IntegerData;
		if (((wl_info_t*)wl)->oid->sw_phy_state == 0) {
			uint radiomaskval = (WL_RADIO_SW_DISABLE << 16) + WL_RADIO_SW_DISABLE;

			WL_INFORM(("%s%d: wl_readconfigoverrides: NICPowerState: turning radio "
				   "off\n", id, unit));
			if (wl_ioctl(wl, WLC_SET_RADIO, &radiomaskval, sizeof(radiomaskval)) ==
				-1) {
				WL_ERROR(("%s%d: wl_readconfigoverrides: NICPowerState: "
					  "Error turning radio off\n", id, unit));
			}
		}
	}
#endif /* EXT_STA */
	if ((param = wl_readparam(ndis_configs, "MPC", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "mpc", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting MPC override %lu\n",
				id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "AssocRoamPref", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "assocroam", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting AssocRoamPref"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "RoamTrigger", confighandle, wl, NULL))) {
		/* use driver default for 0(default) */
		if (param->ParameterData.IntegerData != 0) {
			int roamtrigger[2];
			roamtrigger[0] = param->ParameterData.IntegerData;
			roamtrigger[1] = WLC_BAND_ALL;
			if (wl_ioctl(wl, WLC_SET_ROAM_TRIGGER, (void *)roamtrigger,
				sizeof(roamtrigger)) == -1) {
				WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting RoamTrigger"
					" override %lu\n", id, unit,
					param->ParameterData.IntegerData));
			}
		}
	}

	if ((param = wl_readparam(ndis_configs, "RoamDelta", confighandle, wl, NULL))) {
		/* use driver default for 1(moderate), convert 0(aggressive) and 2(conservative) */
		if (param->ParameterData.IntegerData != 1 &&
		    ((param->ParameterData.IntegerData == 0) ||
		     (param->ParameterData.IntegerData == 2))) {
			int roam_delta[2];
			roam_delta[0] = (param->ParameterData.IntegerData == 0) ? 10 : 30;
			roam_delta[1] = WLC_BAND_ALL;
			if (wl_ioctl(wl, WLC_SET_ROAM_DELTA, (void *)roam_delta,
			    sizeof(roam_delta)) == -1) {
				WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting RoamDelta"
					" override %lu\n", id, unit,
					param->ParameterData.IntegerData));
			}
		}
	}

#ifdef __BBSOFT__
	{
		int roam_threshold = 0;

		if ((param = wl_readparam(ndis_configs, "RoamThreshold", confighandle, wl, NULL))) {
			/* use driver default for 0(default) */
			if (param->ParameterData.IntegerData != 0) {
				int roamtrigger[2];
				if (param->ParameterData.IntegerData > 0)
					roamtrigger[0] = 0 - param->ParameterData.IntegerData;
				else
					roamtrigger[0] = param->ParameterData.IntegerData;
	
				roamtrigger[1] = WLC_BAND_ALL;
				if (wl_ioctl(wl, WLC_SET_ROAM_TRIGGER, (void *)roamtrigger,
					sizeof(roamtrigger)) == -1) {
					WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting RoamThreshold"
						" override %lu\n", id, unit,
						roamtrigger[0]));
				} else {
					roam_threshold = 0 - roamtrigger[0];
				}
			}
		}
	
		if ((param = wl_readparam(ndis_configs, "RoamFloorThre", confighandle, wl, NULL))) {
			if (param->ParameterData.IntegerData <= roam_threshold) {
				int roam_delta[2];
				roam_delta[0] = roam_threshold - param->ParameterData.IntegerData;
				roam_delta[1] = WLC_BAND_ALL;
				if (wl_ioctl(wl, WLC_SET_ROAM_DELTA, (void *)roam_delta,
					sizeof(roam_delta)) == -1) {
					WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting RoamFloorThre"
						" override %lu\n", id, unit,
						roam_delta[0]));
				}
			}
		}
	}
#endif //__BBSOFT__

	/* modify the roam scan period in beacon intervals. (default=10) */
	if ((param = wl_readparam(ndis_configs, "RoamScanPrd", confighandle, wl, &index))) {
		int prd = param->ParameterData.IntegerData;
		if (wl_ioctl(wl, ndis_configs[index].cmd, &prd, sizeof(prd)) != 0) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting scan period"
				" value: %d\n", id, unit, prd));
		}
	}

	if ((param = wl_readparam(ndis_configs, "ListenInt", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "assoc_listen", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting ListenInt"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "WmeApsd", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "wme_apsd", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting WmeApsd"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "ApsdTriggerPrd", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "wme_apsd_trigger",
			(int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting ApsdTriggerPrd"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "ApsdAutoTrigger", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "wme_auto_trigger",
			(int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting ApsdAutoTrigger"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	/* This registry key is used to disable cached roam scanning. The 'fullroamperiod'
	 * iovar defaults to WLC_FULLROAM_PERIOD seconds, but this wlc member is overloaded to be
	 * the roam scan interval when caching is disabled, which was hardcoded to be ten seconds.
	 * So if we disable cached roaming via the registry, make sure that we also set
	 * the roam period to be ten seconds
	*/

	if ((param = wl_readparam(ndis_configs, "RoamScanFreq", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "roamperiod", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting RoamScanFreq"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
		if (param->ParameterData.IntegerData == 0) {
			if (wl_iovar_setint(wl, "fullroamperiod", 10)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting RoamScanFreq"
				" override to 10sec\n", id, unit));
			}
		}
	}

	if ((param = wl_readparam(ndis_configs, "FullRoamScanInterval", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "fullroamperiod", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting FullRoamScanInterval"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "ApCompatMode", confighandle, wl, NULL))) {
		/*
		 * freqtrack_override values : 0 (Auto), 1 (On), 2 (Off).
		 * Note: Currently the value 2 (Off) cannot be set in the Advanced Tab.
		 */
		if (wl_iovar_setint(wl, "freqtrack", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting AP Compatibility"
				" Mode override %lu\n", id, unit,
				param->ParameterData.IntegerData));
		}
	}

#ifndef EXT_STA
	/* LegacyLink behavior for IBSS: 0 - Legacy => TRUE, 1 - Default => FALSE */
	if ((param = wl_readparam(ndis_configs, "IBSSLink", confighandle, wl, NULL))) {
		ulong legacy_link, bytes_needed, bytes_read;
		if (param->ParameterData.IntegerData == 0)
			legacy_link = TRUE;
		else if (param->ParameterData.IntegerData == 1)
			legacy_link = FALSE;
		else WL_ERROR(("%s%d: wl_readconfigoverrides: invalid IBSSLink value: %lu\n",
			id, unit, param->ParameterData.IntegerData));
		wl_setoid(wl, OID_LEGACY_LINK_BEHAVIOR, &legacy_link,
		          sizeof(legacy_link), &bytes_read, &bytes_needed, 0);
	}
#endif /* EXT_STA */

	/* LED powersave option */
	if ((param = wl_readparam(ndis_configs, "leddc", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "leddc", (int)param->ParameterData.IntegerData))
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error on set leddc 0x%x\n",
			          id, unit, (int)param->ParameterData.IntegerData));
	}


	if ((param = wl_readparam(ndis_configs, "BandwidthCap", confighandle, wl, NULL))) {
		int bwcap = 1;
		bool invalid = FALSE;
		if (param->ParameterData.IntegerData == 0)
			bwcap = WLC_N_BW_20ALL;
		else if (param->ParameterData.IntegerData == 1)
			bwcap = WLC_N_BW_40ALL;
		else if (param->ParameterData.IntegerData == 2)
			bwcap = WLC_N_BW_20IN2G_40IN5G;
		else {
			WL_ERROR(("%s%d: wl_readconfigoverrides: invalid BandwidthCap value:"
				  " %lu\n", id, unit, param->ParameterData.IntegerData));
			invalid = TRUE;
		}
		if (!invalid && wl_iovar_setint(wl, "mimo_bw_cap", bwcap)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting Bandwidth Capability"
				" override %lu\n", id, unit, bwcap));
		}
	}

	if ((param = wl_readparam(ndis_configs, "mimo_antsel", confighandle, wl, NULL))) {
		int err;
		uint8 antsel = 0;
		bool invalid = FALSE;
		if (param->ParameterData.IntegerData == 0) {
			antsel = 0;
		} else {
			WL_ERROR(("%s%d: wl_readconfigoverrides: invalid mimo_antsel value:"
				  " %lu\n", id, unit, param->ParameterData.IntegerData));
			invalid = TRUE;
		}
		if (!invalid && (err = wl_iovar_setint(wl, "nphy_antsel_override", antsel))) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting antsel override "
				"err code %d\n", id, unit, err));
		}
	}

	if ((param = wl_readparam(ndis_configs, "AbRateConvert", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "abrateconvert", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting AbRateConvert"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "11NPreamble", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "mimo_preamble", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting 11NPreamble"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}
	if ((param = wl_readparam(ndis_configs, "Intolerant", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "intol40", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting 40 intolerant"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "OBSSCoex", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "obss_coex", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting OBSS Coex"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}
#ifdef WLDPT
	if ((param = wl_readparam(ndis_configs, "DPTEnable", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "dpt", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting dpt"
				" to %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}
#endif /* WLDPT */

	if ((param = wl_readparam(ndis_configs, "AdvPsPoll", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "adv_ps_poll", (int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting AdvPsPoll"
				" override %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

	if ((param = wl_readparam(ndis_configs, "ht_wsec_restrict", confighandle, wl, NULL))) {
		if (wl_iovar_setint(wl, "ht_wsec_restrict",
			(int)param->ParameterData.IntegerData)) {
			WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting ht_wsec_restrict"
				" to %lu\n", id, unit, param->ParameterData.IntegerData));
		}
	}

#ifdef __BBSOFT__
	if ((param = wl_readparam(ndis_configs, "dbgBSSList", confighandle, wl, NULL))) {
		((wl_info_t *)wl)->dbgBSSList = (int)param->ParameterData.IntegerData;
	}

	if ((param = wl_readparam(ndis_configs, "dbgBSSEvent", confighandle, wl, NULL))) {
		((wl_info_t *)wl)->dbgBSSEvent= (int)param->ParameterData.IntegerData;
	}

	if ((param = wl_readparam(ndis_configs, "apEnable", confighandle, wl, NULL))) {
		((wl_info_t *)wl)->apEnable = (int)param->ParameterData.IntegerData;
	}

	if (((wl_info_t *)wl)->apEnable) {
		if ((param = wl_readparam(ndis_configs, "apSSID", confighandle, wl, NULL))) {
			wchar2ascii(((wl_info_t *)wl)->apSSID[0], (ushort *) (param->ParameterData.StringData.
				Buffer), param->ParameterData.StringData.Length,
				sizeof(((wl_info_t *)wl)->apSSID[0]));
		}
		if ((param = wl_readparam(ndis_configs, "apSSIDa", confighandle, wl, NULL))) {
			wchar2ascii(((wl_info_t *)wl)->apSSID[1], (ushort *) (param->ParameterData.StringData.
				Buffer), param->ParameterData.StringData.Length,
				sizeof(((wl_info_t *)wl)->apSSID[1]));
		}
	}
	
	if ((param = wl_readparam(ndis_configs, "probeEnable", confighandle, wl, NULL))) {
		((wl_info_t *)wl)->probeEnable = (int)param->ParameterData.IntegerData;
	}

	if (((wl_info_t *)wl)->probeEnable) {
		if ((param = wl_readparam(ndis_configs, "probeSSID", confighandle, wl, NULL))) {
			wchar2ascii(((wl_info_t *)wl)->probeSSID[0], (ushort *) (param->ParameterData.StringData.
				Buffer), param->ParameterData.StringData.Length,
				sizeof(((wl_info_t *)wl)->probeSSID[0]));
		}
		if ((param = wl_readparam(ndis_configs, "probeSSIDa", confighandle, wl, NULL))) {
			wchar2ascii(((wl_info_t *)wl)->probeSSID[1], (ushort *) (param->ParameterData.StringData.
				Buffer), param->ParameterData.StringData.Length,
				sizeof(((wl_info_t *)wl)->probeSSID[1]));
		}
	}
#endif //__BBSOFT__

	/* process simple configs in a loop */
	for (i = 0; ndis_configs[i].str; ++i) {
		if ((ndis_configs[i].flags & (SIMPLE_CONFIG | SIMPLE_IOVAR_CONFIG)) &&
			(param = wl_readparam(ndis_configs, ndis_configs[i].str, confighandle, wl,
			NULL))) {
			int set_err;
			int int_val = param->ParameterData.IntegerData;

			if (ndis_configs[i].flags & SIMPLE_CONFIG)
				set_err = wl_ioctl(wl, ndis_configs[i].cmd, &int_val,
					sizeof(int_val));
			else
				set_err = wl_iovar_setint(wl, ndis_configs[i].str, int_val);

			if (set_err != 0) {
				WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting override"
					" \"%s\" value %lu\n",
					id, unit, ndis_configs[i].str, int_val));
			}
		}
	}
}

#ifdef __BBSOFT__
static int
_wl_parse_channel_list(char* list_str, uint16* channel_list, int channel_num)
{
	int num;
	int val;
	char* str;
	char* endptr = NULL;

	if (list_str == NULL)
		return 0;

	str = list_str;
	num = 0;
	while (*str != '\0') {
		val = (int)strtol(str, &endptr, 0);
		if (endptr == str) {
			WL_ERROR(("could not parse channel number starting at"
				" substring \"%s\" in list:\n%s\n",
				str, list_str));
			return 0;
		}
		str = endptr + strspn(endptr, " ,");

		if (num == channel_num) {
			WL_ERROR(("too many channels (more than %d) in channel list:\n%s\n",
				channel_num, list_str));
			return 0;
		}

		if (val > 200) {
			val = (int)wl_freq2channel((uint)val);
			if (val == 0) continue;
		}

		channel_list[num++] = (uint16)val;
		WL_TRACE(("SCAN CHANNEL[%d] : %d\n", num, val));
	}
	
	return num;
}
#endif

void
wl_scanoverrides(
	void *wl,
	NDIS_HANDLE confighandle,
	char *id,
	uint unit
)
{
	int i;
	const char *scan_configs[] = {
		"scan_channel_time",
		"scan_unassoc_time",
		"scan_home_time",
		"scan_passive_time",
		"scan_passes"
	};
	bool scan_time_override = FALSE;
	PNDIS_CONFIGURATION_PARAMETER param;
	int index;

	WL_TRACE(("wl_scanoverrides\n"));
	for (i = 0; i < sizeof(scan_configs)/sizeof(char*); i++) {
		if ((param = wl_readparam(ndis_configs, scan_configs[i], confighandle,
		   wl, &index))) {
			if (wl_ioctl(wl, ndis_configs[index].cmd,
			             &param->ParameterData.IntegerData,
			             sizeof(param->ParameterData.IntegerData)) != 0) {
				WL_ERROR(("%s%d: wl_readconfigoverrides: Error setting override"
					" \"%s\" value %lu\n",
					id, unit, ndis_configs[index].str,
					param->ParameterData.IntegerData));
			} else if (ndis_configs[index].cmd == WLC_SET_SCAN_UNASSOC_TIME ||
				ndis_configs[index].cmd == WLC_SET_SCAN_NPROBES) {
				/* note that there was a registry override of scan unassociated
				 * timing
				 */
				scan_time_override = TRUE;
			}
		}
	}

#ifdef __BBSOFT__
		if ((param = wl_readparam(ndis_configs, "dwellTime", confighandle,
				wl, &index))) {
			if (wl_ioctl(wl, ndis_configs[index].cmd,
						 &param->ParameterData.IntegerData,
						 sizeof(param->ParameterData.IntegerData)) != 0) {
				WL_ERROR(("%s%s: wl_readconfigoverrides: Error setting dwellTime\n",
						id, unit));
			} else {
				scan_time_override = TRUE;
			}
		}
		
		if ((param = wl_readparam(ndis_configs, "channelList", confighandle,
				wl, &index))) {
			char channels[100];
			
			wchar2ascii(channels, param->ParameterData.StringData.Buffer,
				param->ParameterData.StringData.Length,
				sizeof(channels));

			((wl_info_t *)wl)->channel_num = _wl_parse_channel_list(channels,
					((wl_info_t *)wl)->channel_list, WL_NUMCHANNELS);
		}
#endif
	
	wl_iovar_setint(wl, "scantime_override", scan_time_override);
	return;
}

ndis_config_t *
wl_findconfig(const char *name, ndis_config_t *table, int *index)
{
	ndis_config_t *ndis_config;
	int i;

	if (table)
		ndis_config = table;
	else
		ndis_config = ndis_configs;

	for (i = 0; ndis_config->str; ++i) {
		if (strcmp(ndis_config->str, name) == 0)
			break;
		++ndis_config;
	}

	if (index)
		*index = i;

	/* add the registry entry to the table for lookup */
	if (ndis_config->str)
		return ndis_config;
	else
		return NULL;
}

#ifndef NDIS_MINIPORT_DRIVER

PNDIS_CONFIGURATION_PARAMETER
wl_readparam(
	ndis_config_t *ndis_config,
	const char *str,
	NDIS_HANDLE confighandle,
	void *wl,
	int *index
)
{
	ASSERT(ndis_config);

	ndis_config = wl_findconfig(str, ndis_config, index);

	if (!ndis_config || !ndis_config->param)
		return NULL;

	/* most config entries are strings - convert to ints where appropriate */
	if (ndis_config->type == NdisParameterInteger &&
		ndis_config->param->ParameterData.StringData.Buffer)
		ndis_config->param->ParameterData.IntegerData =
			bcm_strtoul(ndis_config->param->ParameterData.StringData.Buffer, NULL, 0);

	if (ndis_config->type == NdisParameterInteger &&
		(ndis_config->flags & SKIP_MINUS_1) &&
		ndis_config->param->ParameterData.IntegerData == -1)
		return NULL;

	return ndis_config->param;
}

#endif /* NDIS_MINIPORT_DRIVER */
