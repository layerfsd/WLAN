//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Module Name:    FMD.H

Abstract:       FLASH Media Driver Interface for Windows CE 
  
Notes:          The following module defines the required entry points for
                creating a new FLASH Media Driver (FMD) for Windows CE.  The 
                FMD is called by the FLASH Abstraction Layer (FAL) and is 
                responsible for directly writing to the underlying FLASH hardware
                (i.e. NAND/NOR chip).  In turn, the FAL handles all of the necessary
                wear-leveling of the device.  Practically, this means that the FMD
                is NEVER asked to read/write an invalid page or block.

                The FMD is responsible for:
                    * reading from the FLASH media
                    * writing to the FLASH media
                    * erasing FLASH media blocks
                    * performing error correcting-codes (ECC) on data stored on the 
                      FLASH media (if necessary)

Environment:    As noted, this media driver works on behalf of the FAL to directly
                access the underlying FLASH hardware.  Consquently, this module 
                needs to be linked with FAL.LIB to produce the device driver 
                named FLASHDRV.DLL.

-----------------------------------------------------------------------------*/

#ifndef _FMD_H_
#define _FMD_H_ 

#include <windows.h>
#include <diskio.h>
#include <bldver.h>
#include <pcireg.h>
#include "fls.h"

// FMD IOCTL definitions.
//
#define IOCTL_FMD_SET_XIPMODE	        IOCTL_DISK_USER(0)
#define IOCTL_FMD_LOCK_BLOCKS	        IOCTL_DISK_USER(1)
#define IOCTL_FMD_UNLOCK_BLOCKS	        IOCTL_DISK_USER(2)
#define IOCTL_FMD_GET_INTERFACE         IOCTL_DISK_USER(3)
#define IOCTL_FMD_GET_XIPMODE	        IOCTL_DISK_USER(4)
#define IOCTL_FMD_READ_RESERVED         IOCTL_DISK_USER(5)
#define IOCTL_FMD_WRITE_RESERVED        IOCTL_DISK_USER(6)
#define IOCTL_FMD_GET_RESERVED_TABLE    IOCTL_DISK_USER(7)
#define IOCTL_FMD_SET_REGION_TABLE      IOCTL_DISK_USER(8)
#define IOCTL_FMD_SET_SECTORSIZE        IOCTL_DISK_USER(9)
#define IOCTL_FMD_RAW_WRITE_BLOCKS      IOCTL_DISK_USER(10)
#define IOCTL_FMD_GET_RAW_BLOCK_SIZE    IOCTL_DISK_USER(11)
#define IOCTL_FMD_GET_INFO              IOCTL_DISK_USER(12)

// FMD block status definitions.
#define BLOCK_STATUS_UNKNOWN	0x01
#define BLOCK_STATUS_BAD		0x02
#define BLOCK_STATUS_READONLY	0x04
#define BLOCK_STATUS_RESERVED 0x08
#define BLOCK_STATUS_XIP            0x10
// FMD OEM reserved area bitfield.
#define OEM_BLOCK_RESERVED		0x01
#define OEM_BLOCK_READONLY		0x02

#ifdef __cplusplus
extern "C" {
#endif 

#define INVALID_BLOCK_ID        0xFFFFFFFF
#define INVALID_SECTOR_ADDR     0xFFFFFFFF

#define MIN(a, b) (a < b ? a : b) 

//--------------------------- Structure Definitions -----------------------------

typedef enum  _FLASH_TYPE { NAND, NOR } FLASH_TYPE;

typedef DWORD  SECTOR_ADDR;
typedef PDWORD PSECTOR_ADDR;

typedef DWORD  BLOCK_ID;
typedef PDWORD PBLOCK_ID;

typedef struct _FlashInfo
{
    FLASH_TYPE  flashType;
    DWORD       dwNumBlocks;
    DWORD       dwBytesPerBlock;
    WORD        wSectorsPerBlock;
    WORD        wDataBytesPerSector;

}FlashInfo, *PFlashInfo;

typedef struct _FlashInfoEx
{
    DWORD       cbSize;
    FLASH_TYPE  flashType;
    DWORD       dwNumBlocks;
    DWORD       dwDataBytesPerSector;
    DWORD       dwNumRegions;
    FlashRegion  region[1];

}FlashInfoEx, *PFlashInfoEx;


typedef struct _SectorInfo
{
    DWORD dwReserved1;              // Reserved - used by FAL
    BYTE  bOEMReserved;             // For use by OEM
    BYTE  bBadBlock;	            // Indicates if block is BAD
    WORD  wReserved2;               // Reserved - used by FAL
    
}SectorInfo, *PSectorInfo;

typedef struct _BlockLockInfo
{
    BLOCK_ID StartBlock;
    ULONG    NumBlocks;
}BlockLockInfo, *PBlockLockInfo;


typedef struct _ReservedReq
{
    CHAR szName[RESERVED_NAME_LEN];
    DWORD dwStart;
    DWORD dwLen;
    LPBYTE pBuffer;
}ReservedReq, *PReservedReq;

typedef struct _RawWriteBlocksReq
{
    DWORD dwStartBlock;             // Starting block to write to
    DWORD dwNumBlocks;              // Number of blocks to write    
    LPBYTE pBuffer;                 // Buffer to write
    DWORD cbBuffer;                 // Size of buffer in bytes
}RawWriteBlocksReq, *PRawWriteBlocksReq;

typedef struct _FMDInfo
{
    DWORD       cbSize;
    FLASH_TYPE  flashType;
    DWORD       dwBaseAddress;
    DWORD       dwNumRegions;
    DWORD       dwNumReserved;

}FMDInfo, *PFMDInfo;

typedef PVOID  (*PFN_INIT)(LPCTSTR lpActiveReg, PPCI_REG_INFO pRegIn, PPCI_REG_INFO pRegOut);
typedef BOOL  (*PFN_DEINIT)(PVOID);
typedef BOOL  (*PFN_GETINFO)(PFlashInfo pFlashInfo);
typedef BOOL  (*PFN_GETINFOEX)(PFlashInfoEx pFlashInfo, PDWORD pdwNumRegions);
typedef DWORD (*PFN_GETBLOCKSTATUS)(BLOCK_ID blockID);
typedef BOOL (*PFN_SETBLOCKSTATUS)(BLOCK_ID blockID, DWORD dwStatus);
typedef BOOL  (*PFN_READSECTOR)(SECTOR_ADDR startSectorAddr, LPBYTE pSectorBuff, PSectorInfo pSectorInfoBuff, DWORD dwNumSectors);
typedef BOOL  (*PFN_WRITESECTOR)(SECTOR_ADDR startSectorAddr, LPBYTE pSectorBuff, PSectorInfo pSectorInfoBuff, DWORD dwNumSectors);
typedef BOOL  (*PFN_ERASEBLOCK)(BLOCK_ID blockID);
typedef VOID  (*PFN_POWERUP)(VOID);
typedef VOID  (*PFN_POWERDOWN)(VOID);
typedef VOID (*PFN_GETPHYSSECTORADDR)(DWORD dwSector, PSECTOR_ADDR pStartSectorAddr);
typedef BOOL (*PFN_OEMIOCONTROL)(DWORD dwIoControlCode, PBYTE pInBuf, DWORD nInBufSize, PBYTE pOutBuf, DWORD nOutBufSize, PDWORD pBytesReturned);


typedef struct _FMDInterface
{
    DWORD cbSize;
    PFN_INIT pInit;
    PFN_DEINIT pDeInit;
    PFN_GETINFO pGetInfo;
    PFN_GETBLOCKSTATUS pGetBlockStatus;
    PFN_SETBLOCKSTATUS pSetBlockStatus;
    PFN_READSECTOR pReadSector;
    PFN_WRITESECTOR pWriteSector;
    PFN_ERASEBLOCK pEraseBlock;
    PFN_POWERUP pPowerUp;
    PFN_POWERDOWN pPowerDown;
    PFN_GETPHYSSECTORADDR pGetPhysSectorAddr;
    PFN_GETINFOEX pGetInfoEx;
    PFN_OEMIOCONTROL pOEMIoControl;
    
} FMDInterface, *PFMDInterface;


//------------------------------- Public Interface (used by the FAL) ------------------------------
PVOID  FMD_Init(LPCTSTR lpActiveReg, PPCI_REG_INFO pRegIn, PPCI_REG_INFO pRegOut);
BOOL  FMD_Deinit(PVOID);
BOOL  FMD_GetInfo(PFlashInfo pFlashInfo);
BOOL  FMD_GetInfoEx(PFlashInfoEx pFlashInfo, PDWORD pdwNumRegions);
DWORD FMD_GetBlockStatus(BLOCK_ID blockID);
BOOL FMD_SetBlockStatus(BLOCK_ID blockID, DWORD dwStatus);

BOOL  FMD_ReadSector (SECTOR_ADDR startSectorAddr, LPBYTE pSectorBuff, PSectorInfo pSectorInfoBuff, DWORD dwNumSectors);
BOOL  FMD_WriteSector(SECTOR_ADDR startSectorAddr, LPBYTE pSectorBuff, PSectorInfo pSectorInfoBuff, DWORD dwNumSectors);

BOOL  FMD_EraseBlock(BLOCK_ID blockID);

VOID  FMD_PowerUp(VOID);
VOID  FMD_PowerDown(VOID);

BOOL  FMD_OEMIoControl(DWORD dwIoControlCode, PBYTE pInBuf, DWORD nInBufSize, 
                       PBYTE pOutBuf, DWORD nOutBufSize, PDWORD pBytesReturned);

//-------------------- FMD Hook Functions exported by an FMD hook library --------------------------
PVOID FMDHOOK_HookInterface(PFMDInterface pInterface);
VOID  FMDHOOK_UnhookInterface(PVOID pContext, PFMDInterface pInterface);

//---------------------------------------- Helper Functions ----------------------------------------

#ifdef __cplusplus
}
#endif

#endif _FMD_H_
