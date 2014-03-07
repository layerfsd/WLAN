/*
 * HND SOCRAM TCAM software interface.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: hndtcam.h,v 1.2.24.2 2009/01/27 04:09:40 Exp $
 */
#ifndef _hndtcam_h_
#define _hndtcam_h_

/*
 * 0 - 1
 * 1 - 2 Consecutive locations are patched
 * 2 - 4 Consecutive locations are patched
 * 3 - 8 Consecutive locations are patched
 * 4 - 16 Consecutive locations are patched
 * Define default to patch 2 locations
 */

#ifdef  PATCHCOUNT
#define SRPC_PATCHCOUNT PATCHCOUNT
#else
#define PATCHCOUNT 0
#define SRPC_PATCHCOUNT PATCHCOUNT
#endif

/* N Consecutive location to patch */
#define SRPC_PATCHNLOC (1 << (SRPC_PATCHCOUNT))

/* patch values and address structure */
typedef struct patchaddrvalue {
	uint32	addr;
	uint32	value;
} patchaddrvalue_t;

extern void hnd_patch_init(void *srp);
extern void hnd_tcam_write(void *srp, uint16 index, uint32 data);
extern void hnd_tcam_read(void *srp, uint16 index, uint32 *content);
void * hnd_tcam_init(void *srp, uint no_addrs);
extern void hnd_tcam_disablepatch(void *srp);
extern void hnd_tcam_enablepatch(void *srp);
extern void hnd_tcam_load(void *srp, const  patchaddrvalue_t *patchtbl);

#endif /* _hndtcam_h_ */
