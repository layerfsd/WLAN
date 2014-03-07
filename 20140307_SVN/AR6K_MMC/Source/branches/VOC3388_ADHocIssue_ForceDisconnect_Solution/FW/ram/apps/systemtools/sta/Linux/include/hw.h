/* hw.h - macros and definitions for linux environment hardware access */

/* Copyright (c) 2001 Atheros Communications, Inc., All Rights Reserved */

#ifndef __INChwh
#define __INChwh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* includes */
#include "wlantype.h"
#include "dk_client.h"

#define DRV_MAJOR_VERSION	1
#define DRV_MINOR_VERSION	2
#define F2_PCI_BAR0_REG 0x10

#ifndef NULL
#define NULL    0
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif


/* defines */
#define A_MEM_ZERO(addr, len) memset(addr, 0, len)

#define F2_VENDOR_ID			0x168C		/* vendor ID for our device */
#define MAX_REG_OFFSET			0xfffc	    /* maximum platform register offset */
#define MAX_CFG_OFFSET          256         /* maximum locations for PCI config space per device */
#define MAX_MEMREAD_BYTES       2048        /* maximum of 2k location per OSmemRead action */

/* PCI Config space mapping */
#define F2_PCI_CMD				0x04		/* address of F2 PCI config command reg */
#define F2_PCI_CACHELINESIZE    0x0C        /* address of F2 PCI cache line size value */
#define F2_PCI_LATENCYTIMER     0x0D        /* address of F2 PCI Latency Timer value */
#define F2_PCI_BAR				0x10		/* address of F2 PCI config BAR register */
#define F2_PCI_INTLINE          0x3C        /* address of F2 PCI Interrupt Line reg */
/* PCI Config space bitmaps */
#define MEM_ACCESS_ENABLE		0x002       /* bit mask to enable mem access for PCI */
#define MASTER_ENABLE           0x004       /* bit mask to enable bus mastering for PCI */
#define MEM_WRITE_INVALIDATE    0x010       /* bit mask to enable write and invalidate combos for PCI */
#define SYSTEMERROR_ENABLE      0x100		/* bit mask to enable system error */

#define WLAN_MAX_DEV	8		/* Number of maximum supported devices */
#define MAX_BARS    6

/* WLAN_DRIVER_INFO structure will hold the driver global information.
 *
 */
typedef struct mdk_wlanDrvInfo
{
	A_UINT32           devCount;                     /* No. of currently connected devices */
	struct mdk_wlanDevInfo *pDevInfoArray[WLAN_MAX_DEV]; /* Array of devinfo pointers */
} MDK_WLAN_DRV_INFO;

/*
 * MDK_WLAN_DEV_INFO structure will hold all kinds of device related information.
 * It will hold OS specific information about the device and a device number.
 * Most of the code doesn't need to know what's inside that structure.
 * The fields are very likely to change.
 */

typedef	struct	mdk_wlanDevInfo
{
	struct dkDevInfo *pdkInfo;    /* pointer to structure containing info for dk */
	A_UINT32	handle;
	A_UCHAR	   *pbuffMapBytes;      /* holds bit maps for descriptors allocated */
    A_UINT16   *pnumBuffs;          /* holds number of buffers allocated by each index */
} MDK_WLAN_DEV_INFO;


// extern variable declarations
extern MDK_WLAN_DRV_INFO	globDrvInfo; 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INChwh */
