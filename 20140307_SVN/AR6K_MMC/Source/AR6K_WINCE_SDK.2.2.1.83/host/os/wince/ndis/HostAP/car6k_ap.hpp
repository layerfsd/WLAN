//------------------------------------------------------------------------------
// <copyright file="car6k_ap.hpp" company="Atheros">
//    Copyright (c) 2008-2009 Atheros Corporation.  All rights reserved.
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
//
// Author(s): ="Atheros"
//==============================================================================

#define ATH_PWR_MAX_QUE_LEN 32

#define WPA_HNDSHK_MSG_1          1
#define WPA_HNDSHK_MSG_2          2
#define WPA_HNDSHK_MSG_3          3
#define WPA_HNDSHK_MSG_4          4
#define WPA_HNDSHK_MSG_G1         5
#define WPA_HNDSHK_MSG_G2         6
#define MAX_KEY_LEN_BITS          128
#define MAX_KEY_LEN_BYTES         (MAX_KEY_LEN_BITS / 8)
#define ELE_UCSE_SIZE             32
#define ELE_AUTHSE_SIZE           32
#define AES_KEY_SIZE_BYTES        16
#define TKIP_KEY_SIZE_BYTES       32
#define CFG_MAX_PASSPHRASE        64
#define MAX_HNDSHK_PCK_SIZE       512
#define ATH_WPA_IE_LEN            0x16
#define ATH_WPA2_IE_LEN           0x14
#define INFO_ELEMENT_SIZE(e)      (((e).length > 0) ? (e).length + 2 : 0)
#define ELE_VENDOR_PRIVATE        221     /* Vendor private space; must demux OUI */
#define KEYDATA_GRPKEY            1
#define A_ROUNDUP(x, y)           ((((x) + ((y) - 1)) / (y)) * (y))
#define A_ROUNDUP_PAD(x, y)       (A_ROUNDUP(x, y) - (x))
#define A_SHA_DIGEST_LEN          20
const A_UINT8 ouiWPA2[3]       =  {0x00, 0x0F, 0xAC};
#define MIN_WPA2IE_ELELEN     \
            (FIELD_OFFSET(WPA2_IE, pwCiphers) - 2 +   \
                                sizeof(A_SUITE_SEL) + \
             sizeof(A_UINT16) + sizeof(A_SUITE_SEL))

typedef struct wlanWPAie {
    A_UINT8    elementID;
    A_UINT8    length;
    A_UINT8    oui[3];
    A_UINT8    ouiType;
    A_UINT16   version;
    A_UINT32   mCipher[1];   /* group key selection */
    A_UINT16   uCipherCount; /* count of pairwise key ciphers */
    A_UINT32   uCiphers[ELE_UCSE_SIZE/4];  /* ciphers 1-6 array of 4 octets */
    A_UINT16   authSelectorCount;          /* authentication selector count */
    A_UINT32   authSelectors[ELE_AUTHSE_SIZE/4];/* selectors 4*n octets */
    A_UINT16   caps;  /* Capabilities */
}  WPA_IE;

#define MIN_WPAIE_ELELEN                                         \
                     (FIELD_OFFSET(WPA_IE, uCiphers) - 2 +       \
                     (1*sizeof(A_UINT32)) + sizeof(A_UINT16) +   \
                     (1*sizeof(A_UINT32)))

typedef struct suiteSel {
#ifdef BIG_ENDIAN
    A_UINT32    suite:8,
                oui  :24;
#else
    A_UINT32    oui  :24,
                suite:8;
#endif
} A_SUITE_SEL;

typedef struct wpa2Ie {
    A_UINT8             elementID;  /* Must be ELE_WPA2 */
    A_UINT8             length;
    A_UINT16            version;
    A_SUITE_SEL         gCipher;   /* group cipher suite */
    A_UINT16            pwCount; /* pairwise cipher count */
    A_UINT8             pwCiphers[166];
    /* enough for 4 uciphers, 1 AKMcnt, 4 AKMs, 1 caps, 1 PMKIDcnt, and 8 PMKIDs */
    /*   octets: 16           2        16       2       2             128 */
} WPA2_IE;















