/*
 * Misc utility routines used by kernel or app-level.
 * Contents are wifi-specific, used by any kernel or app-level
 * software that might want wifi things as it grows.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: bcmwifi.c,v 1.18.24.2.4.1 2009/09/25 00:32:01 Exp $
 */

#include <typedefs.h>

#ifdef BCMDRIVER
#include <osl.h>
#include <bcmutils.h>
#define strtoul(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))
#define tolower(c) (bcm_isupper((c)) ? ((c) + 'a' - 'A') : (c))
#else
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#endif /* BCMDRIVER */
#include <bcmwifi.h>

#if defined(WIN32) && (defined(BCMDLL) || defined(WLMDLL))
#include <bcmstdlib.h> 	/* For wl/exe/GNUmakefile.brcm_wlu and GNUmakefile.wlm_dll */
#endif

/* Chanspec ASCII representation:
 * <channel><band><bandwidth><ctl-sideband>
 *   digit   [AB]     [N]        [UL]
 *
 * <channel>: channel number of the 10MHz or 20MHz channel,
 *	or control sideband channel of 40MHz channel.
 * <band>: A for 5GHz, B for 2.4GHz
 * <bandwidth>: N for 10MHz, nothing for 20MHz or 40MHz
 *	(ctl-sideband spec implies 40MHz)
 * <ctl-sideband>: U for upper, L for lower
 *
 * <band> may be omitted on input, and will be assumed to be
 * 2.4GHz if channel number <= 14.
 *
 * Examples:
 *	8  ->  2.4GHz channel 8, 20MHz
 *	8b ->  2.4GHz channel 8, 20MHz
 *	8l ->  2.4GHz channel 8, 40MHz, lower ctl sideband
 *	8a ->  5GHz channel 8 (low 5 GHz band), 20MHz
 *	36 ->  5GHz channel 36, 20MHz
 *	36l -> 5GHz channel 36, 40MHz, lower ctl sideband
 *	40u -> 5GHz channel 40, 40MHz, upper ctl sideband
 *	180n -> channel 180, 10MHz
 */


/* given a chanspec and a string buffer, format the chanspec as a
 * string, and return the original pointer a.
 * Min buffer length must be CHANSPEC_STR_LEN.
 * On error return NULL
 */
char *
wf_chspec_ntoa(chanspec_t chspec, char *buf)
{
	const char *band, *bw, *sb;
	uint channel;

	band = "";
	bw = "";
	sb = "";
	channel = CHSPEC_CHANNEL(chspec);
	/* check for non-default band spec */
	if ((CHSPEC_IS2G(chspec) && channel > CH_MAX_2G_CHANNEL) ||
	    (CHSPEC_IS5G(chspec) && channel <= CH_MAX_2G_CHANNEL))
		band = (CHSPEC_IS2G(chspec)) ? "b" : "a";
	if (CHSPEC_IS40(chspec)) {
		if (CHSPEC_SB_UPPER(chspec)) {
			sb = "u";
			channel += CH_10MHZ_APART;
		} else {
			sb = "l";
			channel -= CH_10MHZ_APART;
		}
	} else if (CHSPEC_IS10(chspec)) {
		bw = "n";
	}

	/* Outputs a max of 6 chars including '\0'  */
	_snprintf(buf, 6, "%d%s%s%s", channel, band, bw, sb);
	return (buf);
}

/* given a chanspec string, convert to a chanspec.
 * On error return 0
 */
chanspec_t
wf_chspec_aton(char *a)
{
	char *endp = NULL;
	uint channel, band, bw, ctl_sb;
	char c;

	channel = strtoul(a, &endp, 10);

	/* check for no digits parsed */
	if (endp == a)
		return 0;

	if (channel > MAXCHANNEL)
		return 0;

	band = ((channel <= CH_MAX_2G_CHANNEL) ? WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G);
	bw = WL_CHANSPEC_BW_20;
	ctl_sb = WL_CHANSPEC_CTL_SB_NONE;

	a = endp;

	c = tolower(a[0]);
	if (c == '\0')
		goto done;

	/* parse the optional ['A' | 'B'] band spec */
	if (c == 'a' || c == 'b') {
		band = (c == 'a') ? WL_CHANSPEC_BAND_5G : WL_CHANSPEC_BAND_2G;
		a++;
		c = tolower(a[0]);
		if (c == '\0')
			goto done;
	}

	/* parse bandwidth 'N' (10MHz) or 40MHz ctl sideband ['L' | 'U'] */
	if (c == 'n') {
		bw = WL_CHANSPEC_BW_10;
	} else if (c == 'l') {
		bw = WL_CHANSPEC_BW_40;
		ctl_sb = WL_CHANSPEC_CTL_SB_LOWER;
		/* adjust channel to center of 40MHz band */
		if (channel <= (MAXCHANNEL - CH_20MHZ_APART))
			channel += CH_10MHZ_APART;
		else
			return 0;
	} else if (c == 'u') {
		bw = WL_CHANSPEC_BW_40;
		ctl_sb = WL_CHANSPEC_CTL_SB_UPPER;
		/* adjust channel to center of 40MHz band */
		if (channel > CH_20MHZ_APART)
			channel -= CH_10MHZ_APART;
		else
			return 0;
	} else {
		return 0;
	}

done:
	return (channel | band | bw | ctl_sb);
}

/*
 * Return the channel number for a given frequency and base frequency.
 * The returned channel number is relative to the given base frequency.
 * If the given base frequency is zero, a base frequency of 5 GHz is assumed for
 * frequencies from 5 - 6 GHz, and 2.407 GHz is assumed for 2.4 - 2.5 GHz.
 *
 * Frequency is specified in MHz.
 * The base frequency is specified as (start_factor * 500 kHz).
 * Constants WF_CHAN_FACTOR_2_4_G, WF_CHAN_FACTOR_5_G are defined for
 * 2.4 GHz and 5 GHz bands.
 *
 * The returned channel will be in the range [1, 14] in the 2.4 GHz band
 * and [0, 200] otherwise.
 * -1 is returned if the start_factor is WF_CHAN_FACTOR_2_4_G and the
 * frequency is not a 2.4 GHz channel, or if the frequency is not and even
 * multiple of 5 MHz from the base frequency to the base plus 1 GHz.
 *
 * Reference 802.11 REVma, section 17.3.8.3, and 802.11B section 18.4.6.2
 */
int
wf_mhz2channel(uint freq, uint start_factor)
{
	int ch = -1;
	uint base;
	int offset;

	/* take the default channel start frequency */
	if (start_factor == 0) {
		if (freq >= 2400 && freq <= 2500)
			start_factor = WF_CHAN_FACTOR_2_4_G;
		else if (freq >= 5000 && freq <= 6000)
			start_factor = WF_CHAN_FACTOR_5_G;
	}

	if (freq == 2484 && start_factor == WF_CHAN_FACTOR_2_4_G)
		return 14;

	base = start_factor / 2;

	/* check that the frequency is in 1GHz range of the base */
	if ((freq < base) || (freq > base + 1000))
		return -1;

	offset = freq - base;
	ch = offset / 5;

	/* check that frequency is a 5MHz multiple from the base */
	if (offset != (ch * 5))
		return -1;

	/* restricted channel range check for 2.4G */
	if (start_factor == WF_CHAN_FACTOR_2_4_G && (ch < 1 || ch > 13))
		return -1;

	return ch;
}

/*
 * Return the center frequency in MHz of the given channel and base frequency.
 * The channel number is interpreted relative to the given base frequency.
 *
 * The valid channel range is [1, 14] in the 2.4 GHz band and [0, 200] otherwise.
 * The base frequency is specified as (start_factor * 500 kHz).
 * Constants WF_CHAN_FACTOR_2_4_G, WF_CHAN_FACTOR_5_G are defined for
 * 2.4 GHz and 5 GHz bands.
 * The channel range of [1, 14] is only checked for a start_factor of
 * WF_CHAN_FACTOR_2_4_G (4814).
 * Odd start_factors produce channels on .5 MHz boundaries, in which case
 * the answer is rounded down to an integral MHz.
 * -1 is returned for an out of range channel.
 *
 * Reference 802.11 REVma, section 17.3.8.3, and 802.11B section 18.4.6.2
 */
int
wf_channel2mhz(uint ch, uint start_factor)
{
	int freq;

	if ((start_factor == WF_CHAN_FACTOR_2_4_G && (ch < 1 || ch > 14)) ||
	    (ch <= 200))
		freq = -1;
	if ((start_factor == WF_CHAN_FACTOR_2_4_G) && (ch == 14))
		freq = 2484;
	else
		freq = ch * 5 + start_factor / 2;

	return freq;
}
