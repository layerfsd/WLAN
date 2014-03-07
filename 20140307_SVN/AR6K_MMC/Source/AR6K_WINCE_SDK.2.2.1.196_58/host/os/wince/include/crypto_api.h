//------------------------------------------------------------------------------
// <copyright file="crypto_api.h" company="Atheros">
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



#ifdef __cplusplus
extern "C" {
#endif

void hmac_sha1 (unsigned char *text, int text_len,
               unsigned char *key, int key_len,
               unsigned char *digest);

void hmac_md5 (unsigned char *text, int text_len, unsigned char *key,
               int  key_len, unsigned char * digest);

A_STATUS PasswordHash (char *password, unsigned char *ssid,
                       int ssidlength, unsigned char *output);

void crypto_keyCounterIncr (A_UINT8 m_keyCounter[]);

void crypto_keyCounterInit (A_UINT8 *bssid, A_UINT8 m_keyCounter[]);

void crypto_GMKGenerate (A_UINT8 *bssid, A_UINT8  m_keyCounter[],
                         A_UINT16 *GMKLength, A_UINT8  GMK[]);

void crypto_GNonceCalc (A_UINT8 *bssid, A_UINT8 m_keyCounter[], A_UINT8 GNonce[]);

void crypto_GTKCalc (A_UINT8 Type, A_UINT8 *bssid, A_UINT16 keyIndex, A_UINT8  m_keyCounter[],
                     A_UINT8 GNonce[], A_UINT16 *GTKLength, A_UINT8 GTK[][32],
                     A_UINT8 GMK[], A_UINT16 *GMKLength);

void crypto_ANonceCalc (STATION *station, A_UINT8 m_keyCounter[]);

void crypto_PTKGenerate (STATION *station, A_UINT8 type, BYTE m_CurrentAddress[],
                         A_UINT8 key[]);

void crypto_sendEapolKeyMsg (A_UINT8 descType, A_BOOL secure, A_BOOL mic,
                            A_BOOL ack, A_BOOL tx, A_UINT32 index, A_UINT8 isPW,
                            A_BOOL doEncrypt, A_UINT64 keyRSC, const A_UINT8 nonce[],
                            const A_UINT32 keyLength, A_UINT32 keyDataLen,
                            const A_UINT8 *keyData, const A_UINT32 keyBufferLength,
                            A_UINT8  *bssid, BYTE  m_CurrentAddress[],
                            A_UINT8  m_keyCounter[], ULONGLONG *replayCtr,
                            STATION *station, A_UINT8 buff[], A_UINT16 *length);

int crypto_wpa2GTKInfoElementCreate (A_UINT8 *buf, int len, A_UINT8 GTK[][32],
                                     A_UINT8 m_current_grp_index);

A_STATUS crypto_wpaMsgVerify (STATION *station, EAPOL_KEY_WPA *ekw, WPA_MSGNUM msgNum,
                              A_BOOL m_isSecure, A_UINT8 m_crypt_type);

void crypto_statetrans (STATION* station, AP_MODE_CMD m_apCmd, A_UINT8 m_key_size,
                        A_UINT8 GTK[][32], ULONGLONG *replayCtr,
                        PVOID m_pWMI, A_UINT8 m_current_grp_index, A_UINT8 m_AP_conn_sta);
#ifdef __cplusplus
 }
#endif















