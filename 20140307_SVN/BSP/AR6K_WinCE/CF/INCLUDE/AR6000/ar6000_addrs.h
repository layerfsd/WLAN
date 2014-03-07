//------------------------------------------------------------------------------
// <copyright file="AR6000_addrs.h" company="Atheros">
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
// AR6000 special addresses
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef __AR6000_ADDRS_H__
#define __AR6000_ADDRS_H__

/* 
 * Special AR6000 Addresses that may be needed by special
 * applications (e.g. ART) on the Host as well as Target.
 * Note that these are all uncached addresses.
 */

#define AR6000_RAM_START 0xa0000000
#define AR6000_RAM_ADDR(byte_offset) (AR6000_RAM_START+(byte_offset))
#define TARG_RAM_ADDRS(byte_offset) AR6000_RAM_ADDR(byte_offset)

#define AR6000_FLASH_START 0xa2000000
#define AR6000_FLASH_ADDR(byte_offset) (AR6000_FLASH_START+(byte_offset))
#define TARG_FLASH_ADDRS(byte_offset) AR6000_FLASH_ADDR(byte_offset)

#define AR6000_ROM_START 0xa1000000
#define AR6000_ROM_ADDR(byte_offset) (AR6000_ROM_START+(byte_offset))
#define TARG_ROM_ADDRS(byte_offset) AR6000_ROM_ADDR(byte_offset)

/* Size of Board Data, in bytes */
#define AR6000_BOARD_DATA_SZ 512

/*
 * Calibration data and other Board Data is located in flash in the
 * last 512 bytes of the first Half Megabyte.  The API A_BOARD_DATA_ADDR()
 * is the proper way to get a read pointer to the data -- it returns a RAM
 * version of the source Board Data which may have come directly from the
 * flash copy or from elsewhere.
 *
 * (NB: Flash address of Board Data is 0xa207fe00)
 */
#define AR6000_BOARD_DATA_OFFSET (0x80000 - AR6000_BOARD_DATA_SZ)
#define AR6000_BOARD_DATA_ADDR TARG_FLASH_ADDRS(AR6000_BOARD_DATA_OFFSET)

/*
 * Pointer to index of DataSets in Flash,
 * located at flash offset 512KB-512-4,
 * which is the word just before the Board Data.
 */
#define AR6000_FLASH_DATASET_INDEX_OFFSET \
    (AR6000_BOARD_DATA_OFFSET - sizeof(void *))
#define FLASH_DATASET_INDEX_OFFSET AR6000_FLASH_DATASET_INDEX_OFFSET
#define FLASH_DATASET_INDEX_ADDR TARG_FLASH_ADDRS(FLASH_DATASET_INDEX_OFFSET)

#endif /* __AR6000_ADDRS_H__ */
