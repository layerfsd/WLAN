//------------------------------------------------------------------------------
// <copyright file="wlan_recv_beacon.c" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// IEEE 802.11 input handling.
//
// Author(s): ="Atheros"
//==============================================================================

#include "a_config.h"
#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include <wmi.h>
#include <ieee80211.h>
#include <wlan_api.h>

#define IEEE80211_VERIFY_LENGTH(_len, _minlen) do {         \
    if ((_len) < (_minlen)) {                   \
        return A_EINVAL;                         \
    }                               \
} while (0)

#define IEEE80211_VERIFY_ELEMENT(__elem, __maxlen) do {         \
    if ((__elem) == NULL) {                     \
        return A_EINVAL;                         \
    }                               \
    if ((__elem)[1] > (__maxlen)) {                 \
        return A_EINVAL;                         \
    }                               \
} while (0)


/* unaligned little endian access */
#define LE_READ_2(p)                            \
    ((A_UINT16)                            \
     ((((A_UINT8 *)(p))[0]      ) | (((A_UINT8 *)(p))[1] <<  8)))

#define LE_READ_4(p)                            \
    ((A_UINT32)                            \
     ((((A_UINT8 *)(p))[0]      ) | (((A_UINT8 *)(p))[1] <<  8) | \
      (((A_UINT8 *)(p))[2] << 16) | (((A_UINT8 *)(p))[3] << 24)))


static int __inline
iswpaoui(const A_UINT8 *frm)
{
    return frm[1] > 3 && LE_READ_4(frm+2) == ((WPA_OUI_TYPE<<24)|WPA_OUI);
}

static int __inline
iswmmoui(const A_UINT8 *frm)
{
    return frm[1] > 3 && LE_READ_4(frm+2) == ((WMM_OUI_TYPE<<24)|WMM_OUI);
}

/* unused functions for now */
#if 0
static int __inline
iswmmparam(const A_UINT8 *frm)
{
    return frm[1] > 5 && frm[6] == WMM_PARAM_OUI_SUBTYPE;
}

static int __inline
iswmminfo(const A_UINT8 *frm)
{
    return frm[1] > 5 && frm[6] == WMM_INFO_OUI_SUBTYPE;
}
#endif

static int __inline
isatherosoui(const A_UINT8 *frm)
{
    return frm[1] > 3 && LE_READ_4(frm+2) == ((ATH_OUI_TYPE<<24)|ATH_OUI);
}

static int __inline
iswscoui(const A_UINT8 *frm)
{
    return frm[1] > 3 && LE_READ_4(frm+2) == ((0x04<<24)|WPA_OUI);
}


/* Fix Bug 82893 - don't show results from entities with PHY different
 *                 than this radios present setting
 */
A_STATUS
check_phy_rates(A_UINT8 *rates_ie, A_UINT8 phy_mode)
{
    A_UINT8 length = rates_ie[1];
    A_UINT8 index;
    A_UINT8 rate;
    if (length)
    {
	switch (phy_mode)
	{
	    case WMI_11B_MODE:
	    for (index = 0; index < length; index++)
	    {
		rate = 0x7f & rates_ie[2+index];
		if (!((rate==2) || (rate == 4) || (rate == 11) || (rate==22)))	/* fail for anything but 'b' rates */
		{
		    return A_EINVAL;
		}
	    }
	    break;
	    
            case WMI_11A_MODE:			/* filter out 'n' rates */
	    case WMI_11GONLY_MODE:		/* filter out 'b' and 'n' rates */
	    for (index = 0; index < length; index++)
	    {
		rate = 0x7f & rates_ie[2+index];
		if (!((rate==12)||(rate==18)||(rate==24)||(rate==36)||(rate==48)||(rate==72)||(rate==96)||(rate==108)))
		{
		    return A_EINVAL;
		}
	    }
	    break;
	}
    }
    return A_OK;
}
A_STATUS
wlan_parse_beacon(A_UINT8 *buf, int framelen, struct ieee80211_common_ie *cie, A_UINT8 phy_mode)	/* Bug 82893 */
{
    A_UINT8 *frm, *efrm;
    A_UINT8 elemid_ssid = FALSE;

    frm = buf;
    efrm = (A_UINT8 *) (frm + framelen);

    /*
     * beacon/probe response frame format
     *  [8] time stamp
     *  [2] beacon interval
     *  [2] capability information
     *  [tlv] ssid
     *  [tlv] supported rates
     *  [tlv] country information
     *  [tlv] parameter set (FH/DS)
     *  [tlv] erp information
     *  [tlv] extended supported rates
     *  [tlv] WMM
     *  [tlv] WPA or RSN
     *  [tlv] Atheros Advanced Capabilities
     */
    IEEE80211_VERIFY_LENGTH(efrm - frm, 12);
    A_MEMZERO(cie, sizeof(*cie));

    cie->ie_tstamp = frm; frm += 8;
    cie->ie_beaconInt = A_LE2CPU16(*(A_UINT16 *)frm);  frm += 2;
    cie->ie_capInfo = A_LE2CPU16(*(A_UINT16 *)frm);  frm += 2;
    cie->ie_chan = 0;

    while (frm < efrm) {
        switch (*frm) {
        case IEEE80211_ELEMID_SSID:
            if (!elemid_ssid) {
                cie->ie_ssid = frm;
                elemid_ssid = TRUE;
            }
            break;
        case IEEE80211_ELEMID_RATES:
            cie->ie_rates = frm;
	    if (A_OK != check_phy_rates(cie->ie_rates, phy_mode))	 /* Fix Bug 82893 */
		return A_EINVAL;
            break;
        case IEEE80211_ELEMID_COUNTRY:
            cie->ie_country = frm;
            break;
        case IEEE80211_ELEMID_FHPARMS:
            break;
        case IEEE80211_ELEMID_DSPARMS:
            cie->ie_chan = frm[2];
            break;
        case IEEE80211_ELEMID_TIM:
            cie->ie_tim = frm;
            break;
        case IEEE80211_ELEMID_IBSSPARMS:
            break;
        case IEEE80211_ELEMID_XRATES:
            cie->ie_xrates = frm;
	    if (A_OK != check_phy_rates(cie->ie_xrates, phy_mode))	/* Fix Bug 82893 */
		return A_EINVAL;
            break;
        case IEEE80211_ELEMID_ERP:
            if (frm[1] != 1) {
                //A_PRINTF("Discarding ERP Element - Bad Len\n");
                return A_EINVAL;
            }
            cie->ie_erp = frm[2];
            break;
        case IEEE80211_ELEMID_RSN:
            cie->ie_rsn = frm;
            break;
#ifdef WAPI_ENABLE
        case IEEE80211_ELEMID_WAPI:
            cie->ie_wapi = frm;
            break;
#endif /* WAPI_ENABLE */
        case IEEE80211_ELEMID_VENDOR:
            if (iswpaoui(frm)) {
                cie->ie_wpa = frm;
            } else if (iswmmoui(frm)) {
                cie->ie_wmm = frm;
            } else if (isatherosoui(frm)) {
                cie->ie_ath = frm;
            } else if(iswscoui(frm)) {
                cie->ie_wsc = frm;
            }
            break;
        default:
            break;
        }
        frm += frm[1] + 2;
    }
    IEEE80211_VERIFY_ELEMENT(cie->ie_rates, IEEE80211_RATE_MAXSIZE);
    IEEE80211_VERIFY_ELEMENT(cie->ie_ssid, IEEE80211_NWID_LEN);

    return A_OK;
}
