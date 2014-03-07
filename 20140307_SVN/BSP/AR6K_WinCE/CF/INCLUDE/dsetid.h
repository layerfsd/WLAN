//------------------------------------------------------------------------------
// <copyright file="dsetid.h" company="Atheros">
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
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// DSETs definitions
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef __DSETID_H__
#define __DSETID_H__

/* Well-known DataSet IDs */
#define DSETID_UNUSED                     0x00000000
#define DSETID_BOARD_DATA                 0x00000001 /* Cal and board data */
#define DSETID_REGDB                      0x00000002 /* Regulatory Database */
#define DSETID_POWER_CONTROL              0x00000003 /* TX Pwr Lim & Ant Gain */
#define DSETID_USER_CONFIG                0x00000004 /* User Configuration */
#define DESTID_ANALOG_CONTROL_DATA_START  0x00000005
#define DESTID_ANALOG_CONTROL_DATA_END    0x00000025
/* Get DSETID for different crystal speed*/
#define ANALOG_CONTROL_DATA_DSETID(refclk) (DESTID_ANALOG_CONTROL_DATA_START + refclk)
#define DSETID_STARTUP_PATCH              0x00000026
#define DSETID_GPIO_CONFIG_PATCH          0x00000027

#define DSETID_VENDOR_START               0x00010000 /* Vendor-defined DataSets */

#define DSETID_INDEX_END                  0xfffffffe /* Reserved to indicate the
                                                        end of a memory-based
                                                        DataSet Index */
#define DSETID_INDEX_FREE                 0xffffffff /* An unused index entry */

/* An item in a memory (e.g. RAM/ROM/flash) DataSet Index. */
typedef struct dset_index_item_s {
    A_UINT32 id;      /* DataSet ID (as above) */
    A_UINT32 offset;  /* Relative to start of memory type */
    A_UINT32 size;    /* size in bytes of the DataSet */
    A_UINT32 version; /* DataSet-specific version */
} dset_index_item_t;

/* 
 * PATCH DataSet format:
 * A list of patches, terminated by a patch with
 * address=PATCH_END.
 *
 * This allows for patches to be stored in flash.
 */
struct patch_s {
    A_UINT32 *address;
    A_UINT32  data;
};

/*
 * Skip some patches.  Can be used to erase a single patch in a
 * patch DataSet without having to re-write the DataSet.  May
 * also be used to embed information for use by subsequent
 * patch code.  The "data" in a PATCH_SKIP tells how many
 * bytes of length "patch_s" to skip.
 */
#define PATCH_SKIP      ((A_UINT32 *)0x00000000)

/*
 * Execute code at the address specified by "data".
 * The address of the patch structure is passed as
 * the one parameter.
 */
#define PATCH_CODE_ABS  ((A_UINT32 *)0x00000001)

/*
 * Same as PATCH_CODE_ABS, but treat "data" as an
 * offset from the start of the patch word.
 */
#define PATCH_CODE_REL  ((A_UINT32 *)0x00000002)

/* Mark the end of this patch DataSet. */
#define PATCH_END       ((A_UINT32 *)0xffffffff)

#endif /* __DSETID_H__ */
