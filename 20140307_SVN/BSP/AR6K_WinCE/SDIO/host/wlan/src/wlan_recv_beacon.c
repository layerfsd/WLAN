//------------------------------------------------------------------------------
// <copyright file="wlan_recv_beacon.c" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// 
//
// Author(s): ="Atheros"
//=============================================================================

#include "athdefs.h"
#include "athtypes.h"
#include "osapi.h"
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

static int __inline
isatherosoui(const A_UINT8 *frm)
{
    return frm[1] > 3 && LE_READ_4(frm+2) == ((ATH_OUI_TYPE<<24)|ATH_OUI);
}

A_STATUS
wlan_parse_beacon(A_UINT8 *buf, A_INT32 framelen, struct ieee80211_common_ie *cie)
{
    A_UINT8 *frm, *efrm;

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
            cie->ie_ssid = frm;
            break;
        case IEEE80211_ELEMID_RATES:
            cie->ie_rates = frm;
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
            break;
        case IEEE80211_ELEMID_ERP:
            if (frm[1] != 1) {
                WLAN_DEBUG_PRINTF(ATH_LOG_ERR, "Discarding ERP Element - Bad Len\n");
                return A_EINVAL;
            }
            cie->ie_erp = frm[2];
            break;
        case IEEE80211_ELEMID_RSN:
            cie->ie_wpa2 = frm;
            break;
        case IEEE80211_ELEMID_VENDOR:
            if (iswpaoui(frm)) {
                cie->ie_wpa = frm;
            } else if (iswmmoui(frm)) {
                cie->ie_wmm = frm;
            } else if (isatherosoui(frm)) {
                cie->ie_ath = frm;
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
