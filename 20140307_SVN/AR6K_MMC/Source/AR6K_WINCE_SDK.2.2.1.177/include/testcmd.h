//------------------------------------------------------------------------------
// <copyright file="testcmd.h" company="Atheros">
//    Copyright (c) 2004-2005 Atheros Corporation.  All rights reserved.
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
// Author(s): ="Atheros"
//==============================================================================

#ifndef  TESTCMD_H_
#define  TESTCMD_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ZEROES_PATTERN = 0,
    ONES_PATTERN,
    REPEATING_10,
    PN7_PATTERN,
    PN9_PATTERN,
    PN15_PATTERN
}TX_DATA_PATTERN;

/* Continous tx
   mode : TCMD_CONT_TX_OFF - Disabling continous tx
          TCMD_CONT_TX_SINE - Enable continuous unmodulated tx
          TCMD_CONT_TX_FRAME- Enable continuous modulated tx
   freq : Channel freq in Mhz. (e.g 2412 for channel 1 in 11 g)
dataRate: 0 - 1 Mbps
          1 - 2 Mbps
          2 - 5.5 Mbps
          3 - 11 Mbps
          4 - 6 Mbps
          5 - 9 Mbps
          6 - 12 Mbps
          7 - 18 Mbps
          8 - 24 Mbps
          9 - 36 Mbps
         10 - 28 Mbps
         11 - 54 Mbps
  txPwr: Tx power in dBm[5 -11] for unmod Tx, [5-14] for mod Tx
antenna:  1 - one antenna
          2 - two antenna
Note : Enable/disable continuous tx test cmd works only when target is awake.
*/

typedef enum {
    TCMD_CONT_TX_OFF = 0,
    TCMD_CONT_TX_SINE,
    TCMD_CONT_TX_FRAME,
    TCMD_CONT_TX_TX99,
    TCMD_CONT_TX_TX100
} TCMD_CONT_TX_MODE;

typedef PREPACK struct {
    A_UINT32                testCmdId;
    A_UINT32                mode;
    A_UINT32                freq;
    A_UINT32                dataRate;
    A_INT32                 txPwr;
    A_UINT32                antenna;
    A_UINT32                enANI;
    A_UINT32                scramblerOff;
    A_UINT32                aifsn;
    A_UINT16                pktSz;
    A_UINT16                txPattern;
} POSTPACK TCMD_CONT_TX;

#define TCMD_TXPATTERN_ZERONE                 0x1
#define TCMD_TXPATTERN_ZERONE_DIS_SCRAMBLE    0x2

/* Continuous Rx
 act: TCMD_CONT_RX_PROMIS - promiscuous mode (accept all incoming frames)
      TCMD_CONT_RX_FILTER - filter mode (accept only frames with dest
                                             address equal specified
                                             mac address (set via act =3)
      TCMD_CONT_RX_REPORT  off mode  (disable cont rx mode and get the
                                          report from the last cont
                                          Rx test)

     TCMD_CONT_RX_SETMAC - set MacAddr mode (sets the MAC address for the
                                                 target. This Overrides
                                                 the default MAC address.)

*/
typedef enum {
    TCMD_CONT_RX_PROMIS =0,
    TCMD_CONT_RX_FILTER,
    TCMD_CONT_RX_REPORT,
    TCMD_CONT_RX_SETMAC,
    TCMD_CONT_RX_SET_ANT_SWITCH_TABLE
} TCMD_CONT_RX_ACT;

typedef PREPACK struct {
    A_UINT32         testCmdId;
    A_UINT32        act;
    A_UINT32        enANI;
    PREPACK union {
        struct PREPACK TCMD_CONT_RX_PARA {
            A_UINT32    freq;
            A_UINT32    antenna;
        } POSTPACK para;
        struct PREPACK TCMD_CONT_RX_REPORT {
            A_UINT32    totalPkt;
            A_INT32    rssiInDBm;
	    A_UINT32 crcErrPkt;
	    A_UINT32 secErrPkt;
	    A_UINT32 noiseFloor;
        } POSTPACK report;
        struct PREPACK TCMD_CONT_RX_MAC {
            A_UCHAR    addr[ATH_MAC_LEN];
        } POSTPACK mac;
        struct PREPACK TCMD_CONT_RX_ANT_SWITCH_TABLE {
            A_UINT32                antswitch1;
            A_UINT32                antswitch2;
        }POSTPACK antswitchtable;
    } POSTPACK u;
} POSTPACK TCMD_CONT_RX;

/* Force sleep/wake  test cmd
 mode: TCMD_PM_WAKEUP - Wakeup the target
       TCMD_PM_SLEEP - Force the target to sleep.
 */
typedef enum {
    TCMD_PM_WAKEUP = 1, /* be consistent with target */
    TCMD_PM_SLEEP
} TCMD_PM_MODE;

typedef PREPACK struct {
	A_UINT32  testCmdId;
    A_UINT32  mode;
} POSTPACK TCMD_PM;

typedef enum {
    TCMD_CONT_TX_ID,
    TCMD_CONT_RX_ID,
    TCMD_PM_ID,
    TCMD_GET_MAC_ID
} TCMD_ID;


typedef PREPACK struct {
    A_UINT32         testCmdId;
    A_UCHAR    addr[ATH_MAC_LEN];
} POSTPACK TCMD_GET_MAC;

typedef PREPACK union {
          TCMD_CONT_TX contTx;
          TCMD_CONT_RX contRx;
          TCMD_PM pm;
	      TCMD_GET_MAC getMac;
} POSTPACK TEST_CMD;

#ifdef __cplusplus
}
#endif

#endif /* TESTCMD_H_ */
