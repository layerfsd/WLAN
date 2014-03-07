/*++
Copyright (c) 2008 Atheros Communications Inc.
All rights reserved.


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

 Wifi Test definitions


--*/

#ifndef WIFITEST_H_
#define WIFITEST_H_

#define MTE_WIFI_STATUS_SUCCESS          0x0
#define MTE_WIFI_STATUS_SDIO_INIT_FAILED 0x1
#define MTE_WIFI_STATUS_SETUP_FAILED     0x2
#define MTE_WIFI_STATUS_BOOT_FAILED      0x3
#define MTE_WIFI_STATUS_ASSOC_FAILED     0x4
#define MTE_WIFI_STATUS_DHCP_FAILED      0x5
#define MTE_WIFI_STATUS_WMI_FAILED       0x6

#define MTE_WIFI_STATUS_FAILED           0x10

typedef struct _MTE_WIFI_TEST_STATUS {
    A_UINT32    StatusCode;
    A_UINT32    RSSI;
} MTE_WIFI_TEST_STATUS;


#define MAX_WIFI_CHANNELS      14

A_BOOL RunAR6kTest(A_UINT32             Channel, 
                   A_CHAR               *pSSID, 
                   MTE_WIFI_TEST_STATUS *pTestStatus);


#endif
