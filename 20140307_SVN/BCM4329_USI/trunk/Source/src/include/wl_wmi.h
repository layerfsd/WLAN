/*
 * Defines WMI GUIDs that a host program would issue to the driver under
 * Windows. A GUID sent by host application arrives at the driver in the
 * form of the associated OID.
 *
 * Copyright 2003, Broadcom Corporation.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: wl_wmi.h,v 13.6 2006/02/22 20:33:01 Exp $
 */

#ifndef _wl_wmi_h_
#define _wl_wmi_h_

#define GUID_BCM_SET {0x8d04c330, 0x8985, 0x11d2, {0x8b, 0x4b, 0x00, 0x40, 0x05, 0xa3, 0x02, 0x1c}}

/*
 * WMI support
 * For validation purposes, all "set" or "no-data" OIDS have
 * an additional argument, the EPICTRL_COOKIE.
 */

static NDIS_GUID GuidList[] = {
	{GUID_BCM_SET, {OID_BCM_SETINFORMATION}, -1, (fNDIS_GUID_TO_OID)},
};

#endif /* _wl_wmi_h_ */
