/*
 * Structure used by apps whose drivers access SDIO drivers.
 * Pulled out separately so dhdu and wlu can both use it.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: sdiovar.h,v 13.5.14.2.16.2 2009/12/08 22:34:21 Exp $
 */

#ifndef _sdiovar_h_
#define _sdiovar_h_

#include <typedefs.h>

/* require default structure packing */
#define BWL_DEFAULT_PACKING
#include <packed_section_start.h>

typedef struct sdreg {
	int func;
	int offset;
	int value;
} sdreg_t;

/* Common msglevel constants */
#define SDH_ERROR_VAL		0x0001	/* Error */
#define SDH_TRACE_VAL		0x0002	/* Trace */
#define SDH_INFO_VAL		0x0004	/* Info */
#define SDH_DEBUG_VAL		0x0008	/* Debug */
#define SDH_DATA_VAL		0x0010	/* Data */
#define SDH_CTRL_VAL		0x0020	/* Control Regs */
#define SDH_LOG_VAL		0x0040	/* Enable bcmlog */
#define SDH_DMA_VAL		0x0080	/* DMA */

#define NUM_PREV_TRANSACTIONS	16


#include <packed_section_end.h>

#endif /* _sdiovar_h_ */
