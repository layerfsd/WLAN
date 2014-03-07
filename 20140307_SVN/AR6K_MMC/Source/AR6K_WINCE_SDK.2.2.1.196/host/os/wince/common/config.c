//------------------------------------------------------------------------------
// <copyright file="config.c" company="Atheros">
//    Copyright (c) 2008 Atheros Corporation.  All rights reserved.
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
// Firmware config
//
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include "a_config.h"
#include "athdefs.h"
#include "a_types.h"
#include "targaddrs.h"

#include "AR6002/hw/mbox_host_reg.h"
#include "AR6002/hw/apb_map.h"
#include "AR6002/hw/si_reg.h"
#include "AR6002/hw/gpio_reg.h"
#include "AR6002/hw/rtc_reg.h"
#include "AR6002/hw/mbox_reg.h"
#include "AR6002/hw/vmc_reg.h"
#include "AR6002/addrs.h"

#include "hif.h"
#include "htc_api.h"
#include "common_drv.h"
#include "bmi.h"
#include "wince_common_drv.h"
#include "config.h"
#include "wmi.h"
#include "ar6xapi_wince.h"
#include "dbglog_api.h"
#include "debug_wince.h"
#include "athdrv_wince.h"

#ifdef EMBEDDED_AR6K_FIRMWARE

#include "athwlan_h.bin"
#include "datapatch_h.bin"

#ifdef AR6K_ROMPATCH_APPLY
#include "rompatch_h.bin"
#endif

#ifdef CONFIG_HOST_TCMD_SUPPORT
#include "athtcmd_h.bin"
#endif

#ifdef WINCE_ART
#include "art_h.bin"
#endif

#endif


#define A_ROUND_UP(x, y)             ((((x) + ((y) - 1)) / (y)) * (y))
#define AR6002_INIPATCH_REV2_ADDRESS  0x52d6c4
#define AR6002_ATHWLAN_REV2_ADDRESS   0x502070
#define RPDF_CMD_INSTALL              0x1
#define RPDF_CMD_INST_ACT             0x2
#define AR6002_REV2_MAX_PATCHES       32
#define AR6K_MAX_PATCHES              AR6002_REV2_MAX_PATCHES

#define INIPATCH_REV2_STR            L"\\data.patch.hw2_0.bin"
#define WLAN_PATCH_REV2_STR          L"\\athwlan2_0.rpdf"

    /* for now only support REV 2.0's compressed image, if using rev 1.0, the old method should be
     * used */
#define MAX_FIRMWARE_CACHE_SIZE  (92*1024)
#define MAX_DATAPATCH_CACHE_SIZE (2*1024)

typedef struct _AR6K_BIN_CACHE_INFO {
    A_UINT8     *pData;         /* pointer to allocated cache buffer or fimrware blob buffer */
    A_UINT32    ActualLength;   /* actual length of data stored in cache buffer */
    A_UINT32    MaxLength;      /* max length of cache buffer */
    A_BOOL      Valid;          /* cached data is valid */
    A_BOOL      Static;         /* buffer is not writeable */
} AR6K_BIN_CACHE_INFO;

#define DECLARE_AR6K_FIRMWARE_BIN_CACHE(pFirm,FirmSize,MaxSize,Valid,Static)   \
    static AR6K_BIN_CACHE_INFO  g_FirmwareCache = {(pFirm) ,                   \
                                                   (FirmSize),                 \
                                                   (MaxSize),                  \
                                                   (Valid),                    \
                                                   (Static),                   \
                                                   }                           \

#define DECLARE_AR6K_TCMD_BIN_CACHE(pFirm,FirmSize,MaxSize,Valid,Static)   \
    static AR6K_BIN_CACHE_INFO  g_TcmdCache = {(pFirm) ,                   \
                                                   (FirmSize),                 \
                                                   (MaxSize),                  \
                                                   (Valid),                    \
                                                   (Static),                   \
                                                   }                           \

#define DECLARE_AR6K_ART_BIN_CACHE(pFirm,FirmSize,MaxSize,Valid,Static)   \
    static AR6K_BIN_CACHE_INFO  g_ArtCache = {(pFirm) ,                   \
                                                   (FirmSize),                 \
                                                   (MaxSize),                  \
                                                   (Valid),                    \
                                                   (Static),                   \
                                                   }                           \

#define DECLARE_AR6K_DATAPATCH_BIN_CACHE(pDataPatch,PatchSize,MaxSize,Valid,Static)  \
    static AR6K_BIN_CACHE_INFO  g_DataPatchCache = {(pDataPatch),                    \
                                                    (PatchSize),                     \
                                                    (MaxSize),                       \
                                                    (Valid),                         \
                                                    (Static),                        \
                                                    }

#define DECLARE_AR6K_ROMPATCH_BIN_CACHE(pDataPatch,PatchSize,MaxSize,Valid,Static)  \
    static AR6K_BIN_CACHE_INFO  g_RomPatchCache =  {(pDataPatch),                    \
                                                    (PatchSize),                     \
                                                    (MaxSize),                       \
                                                    (Valid),                         \
                                                    (Static),                        \
                                                    }

static A_UINT32 InstalledPatches[AR6K_MAX_PATCHES];
static A_UINT32 NumPatches = 0;

#ifdef AR6K_FIRMWARE_CACHE

/* firmware caching into host RAM (dynamic)
 * Situations where this is useful:
 *   - On power resume, filesystem access may not be possible
 *   - On power resume, the load process will be slightly faster */

static A_UINT8  g_FirmwareBuffer[MAX_FIRMWARE_CACHE_SIZE];
static A_UINT8  g_DataPatchBuffer[MAX_DATAPATCH_CACHE_SIZE];

DECLARE_AR6K_FIRMWARE_BIN_CACHE(g_FirmwareBuffer,
                                0,
                                sizeof(g_FirmwareBuffer),
                                FALSE, /* is not valid, forces re-fill */
                                FALSE  /* can be updated on the fly */
                                );

DECLARE_AR6K_DATAPATCH_BIN_CACHE(g_DataPatchBuffer,
                                 0,
                                 sizeof(g_DataPatchBuffer),
                                 FALSE, /* is not valid, forces re-fill */
                                 FALSE  /* can be updated on the fly */
                                 );

DECLARE_AR6K_ROMPATCH_BIN_CACHE(NULL,
                                 0,
                                 0,
                                 FALSE, /* is not valid, forces re-fill */
                                 TRUE  /* not writeable after first fill */
                                 );

#elif EMBEDDED_AR6K_FIRMWARE

/* Situations where this is useful:
 *   - Customers who want 1 driver with all integrated firmware
 *   - Systems where updating the firmware binaries is not very easy */

DECLARE_AR6K_FIRMWARE_BIN_CACHE((A_UINT8*)athwlan,
                                sizeof(athwlan),
                                sizeof(athwlan),
                                TRUE, /* valid */
                                TRUE  /* static, not writeable */
                                );

DECLARE_AR6K_DATAPATCH_BIN_CACHE((A_UINT8*)datapatch,
                                 sizeof(datapatch),
                                 sizeof(datapatch),
                                 TRUE, /* valid */
                                 TRUE  /* static, not writeable */
                                 );

#ifdef AR6K_ROMPATCH_APPLY
DECLARE_AR6K_ROMPATCH_BIN_CACHE(rompatch,
                                sizeof(rompatch),
                                sizeof(rompatch),
                                TRUE, /* valid */
                                TRUE  /* static, not writeable */
                                );
#endif

#ifdef CONFIG_HOST_TCMD_SUPPORT
DECLARE_AR6K_TCMD_BIN_CACHE((A_UINT8*)athtcmd,
                                sizeof(athtcmd),
                                sizeof(athtcmd),
                                TRUE, /* valid */
                                TRUE  /* static, not writeable */
                                );
#endif

#ifdef WINCE_ART
DECLARE_AR6K_ART_BIN_CACHE(art,
                                sizeof(art),
                                sizeof(art),
                                TRUE, /* valid */
                                TRUE  /* static, not writeable */
                                );
#endif

#else

    /* firmware caching not used */
DECLARE_AR6K_FIRMWARE_BIN_CACHE(NULL, 0, 0, FALSE, FALSE);
DECLARE_AR6K_DATAPATCH_BIN_CACHE(NULL, 0, 0, FALSE, FALSE);
DECLARE_AR6K_ROMPATCH_BIN_CACHE(NULL, 0, 0, FALSE, FALSE);
DECLARE_AR6K_TCMD_BIN_CACHE(NULL, 0, 0, FALSE, FALSE);
DECLARE_AR6K_ART_BIN_CACHE(NULL, 0, 0, FALSE, FALSE);

#endif

#define GET_FIRMWARE_CACHE_INFO()  &g_FirmwareCache
#define GET_DATAPATCH_CACHE_INFO() &g_DataPatchCache
#define GET_ROMPATCH_CACHE_INFO() &g_RomPatchCache
#define GET_TCMD_CACHE_INFO()  &g_TcmdCache
#define GET_ART_CACHE_INFO()  &g_ArtCache

static void CopyChunkToBinCache(AR6K_BIN_CACHE_INFO *pCache,
                                A_UINT8             *pChunk,
                                A_UINT32            Length)
{
    A_ASSERT(!pCache->Static);

    if ((pCache->ActualLength + Length) > Length &&
        (pCache->ActualLength + Length) < pCache->MaxLength) {
        A_MEMCPY(&pCache->pData[pCache->ActualLength],pChunk,Length);
        pCache->ActualLength += Length;
    } else {
        A_ASSERT(FALSE);
    }
}

A_UINT32
dbglog_get_debug_hdr_ptr(A_UINT32 tgtType,
                         HIF_DEVICE *device)
{
    A_UINT32 param   = 0;
    A_UINT32 address = 0;
    A_STATUS status  = A_OK;

    address = HOST_INTEREST_ITEM_ADDRESS(tgtType, hi_dbglog_hdr);

    if ((status = ar6000_ReadDataDiag(device, address, (A_UCHAR *)&param, 4)) != A_OK)
    {
        param = 0;
    }

    return param;
}


A_STATUS early_init_ar6000(HIF_DEVICE *hifDevice,
                      A_UINT32    TargetType,
                      A_UINT32    TargetVersion)
{
    A_STATUS status   = A_OK;

    status = ar6000_prepare_target(hifDevice,
                                  TargetType,
                                  TargetVersion);

    return status;
}

static A_STATUS
ar6000_configure_clock (HIF_DEVICE *hifDevice,
                        A_UINT32    TargetType,
                        A_UINT32    TargetVersion,
                        A_UINT32    clkFreq)
{
    A_UINT32 ext_clk_detected = 0;
    A_STATUS status = A_OK;

    do
    {
        if (TargetType == TARGET_TYPE_AR6001)
        {
            status = BMIWriteSOCRegister(hifDevice, 0xAC000020, 0x203);
            if (status != A_OK)
            {
                break;
            }

            status = BMIWriteSOCRegister (hifDevice, 0xAC000024, 0x203);
            if (status != A_OK)
            {
                break;
            }
        }

        if (TargetType == TARGET_TYPE_AR6002)
        {
            if (TargetVersion == AR6002_VERSION_REV2)
            {
                status = BMIReadMemory(hifDevice,
                         HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_ext_clk_detected),
                         (A_UCHAR *)&ext_clk_detected,
                         4);
                if (status != A_OK)
                {
                    break;
                }

#ifdef FORCE_INTERNAL_CLOCK
                status = BMIWriteSOCRegister (hifDevice, 0x40E0, 0x10000);
                if (status != A_OK)
                {
                    break;
                }
                status = BMIWriteSOCRegister (hifDevice, 0x4028, 0x0);
                if (status != A_OK)
                {
                    break;
                }
#else
                // LPO_CAL.Enable for internal clock
                if (ext_clk_detected == 0x0)
                {
                    status = BMIWriteSOCRegister (hifDevice, 0x40E0, 0x10000);
                    if (status != A_OK)
                    {
                        break;
                    }
                }
#endif
                // Run at 40/44 MHz clock
                status = BMIWriteSOCRegister (hifDevice, 0x4020, 0x0);
                if (status != A_OK)
                {
                    break;
                }
            }

            // Write refclk
            status = BMIWriteMemory(hifDevice,
                                    HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_refclk_hz),
                                    (A_UCHAR *)&clkFreq,
                                    4);
            if (status != A_OK)
            {
                break;
            }
        }
    } while (FALSE);

    return status;
}


A_STATUS download_binary (HIF_DEVICE *hifDevice,
                          A_UINT32 address,
                          wchar_t *fileName,
                          wchar_t *fileRoot,
                          A_BOOL  bCompressed,
                          AR6K_BIN_CACHE_INFO *pBinCache)
{
    A_STATUS status = A_OK;
    A_UCHAR  *buffer = NULL;
    A_UINT32 length = 0;
    A_UINT32 fileSize = 0;
    A_UINT32 next_address=0;
    A_INT32 file_left = 0;
    size_t nSize;
    wchar_t filePath[128];
    HANDLE fd = NULL;
    BY_HANDLE_FILE_INFORMATION finfo;
    A_BOOL  fillCache = FALSE;

    A_MEMZERO(&finfo, sizeof(finfo));
    if ( wcslen(fileName) + wcslen(fileRoot) > 127 )
    {
         ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: File Name Very Long :: ====>");
         return A_ERROR;
    }

    wcscpy(filePath,fileRoot);
    wcscat(filePath,fileName);


    ATHR_DISPLAY_MSG (L"file %s \n",filePath);
    do
    {
        if (pBinCache->Valid) {

                /* binary cache is valid */
            if (bCompressed) {
                status = BMILZStreamStart (hifDevice, address);
                if (A_FAILED(status)) {
                    break;
                }
            }

            if (bCompressed) {
                A_UINT32  lastWord = 0;
                A_UINT32  lastWordOffset = pBinCache->ActualLength & ~0x3;
                A_UINT32  unalignedBytes = pBinCache->ActualLength & 0x3;

                if (unalignedBytes) {
                        /* copy the last word into a zero padded buffer */
                    A_MEMCPY(&lastWord,
                             &pBinCache->pData[lastWordOffset],
                             unalignedBytes);
                }

                status = BMILZData(hifDevice,
                                   pBinCache->pData,
                                   lastWordOffset);

                if (A_FAILED(status)) {
                    break;
                }

                if (unalignedBytes) {
                    status = BMILZData(hifDevice,
                                       (A_UINT8 *)&lastWord,
                                       4);
                }

            } else {
                status = BMIWriteMemory(hifDevice,
                                        address,
                                        pBinCache->pData,
                                        A_ROUND_UP(pBinCache->ActualLength,4));
            }

            if (bCompressed && A_SUCCESS(status)) {
                //
                // Close compressed stream and open a new (fake) one.  This serves mainly to flush Target caches.
                //
                status = BMILZStreamStart (hifDevice, 0x00);
                if (A_FAILED(status)) {
                    break;
                }
            }

                /* all done */
            break;
        }

        //Determine the length of the file
        if ( (fd = CreateFile (filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
        {
            status = A_ERROR;
            ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: File Not Found :: ====> %s", filePath);
            break;
        }

        if (!GetFileInformationByHandle(fd, &finfo))
        {
            ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: File Size Error :: ====> %s", filePath);
            status = A_ERROR;
            break;
        }
        fileSize = finfo.nFileSizeLow;
        file_left = fileSize;

        if ((pBinCache->pData != NULL) && (!pBinCache->Static)) {
            /* binary caching is supported */
            A_ASSERT(!pBinCache->Valid);

            if (fileSize <= pBinCache->MaxLength) {
                    /* size if good, flag to cache this binary when read from the filesystem */
                fillCache = TRUE;
                pBinCache->ActualLength = 0; /* reset */
            } else {
                    /* cache is not big enough to hold data */
                A_ASSERT(FALSE);
                ATHR_DISPLAY_MSG (L"AR6K::WARNING!!!! :: File :%s (%d bytes) too big for cache (max=%d)",
                         filePath, fileSize, pBinCache->MaxLength);
            }
        }

        //
        // zero data buffer and init length
        //
        buffer = (A_UCHAR *)A_MALLOC(MAX_BUF);
        if (NULL == buffer)
        {
            status = A_ERROR;
            break;
        }

        if (bCompressed)
        {
            status = BMILZStreamStart (hifDevice, address);
            if (status != A_OK)
            {
                break;
            }
        }


        while (file_left)
        {
            length  = (file_left < (MAX_BUF)) ? file_left : MAX_BUF;
            if (bCompressed)
            {
                //
                // 0 pad last word of data to avoid messy uncompression
                //
                ((A_UINT32 *)buffer)[((length-1)/4)] = 0;
            }

            if (!ReadFile( fd, buffer, length, &nSize, NULL) || nSize != length)
            {
                ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: ReadFile Error :: ====>");
                status = A_ERROR;
                break;
            }
            next_address = address + fileSize - file_left;

            //
            // We round up the requested length because some SDIO Host controllers can't handle other lengths.
            // This generally isn't a problem for users, but it's something to be aware of.
            //
            length = A_ROUND_UP(length, 4);

            if (bCompressed)
            {
                status = BMILZData(hifDevice, buffer, length);
            }
            else
            {
                status = BMIWriteMemory(hifDevice, next_address, buffer, length);
            }

            if (status != A_OK)
            {
               break;
            }

            if (fillCache) {
                CopyChunkToBinCache(pBinCache, buffer, length);
            }

            if (file_left >= MAX_BUF)
            {
                file_left = file_left - MAX_BUF;
            }
            else
            {
                file_left = 0;
            }

        };

        if (status != A_OK)
        {
            break;
        }

        if (fillCache) {
                /* cache was filled, mark it valid */
            pBinCache->Valid = TRUE;
        }

        if (bCompressed)
        {
            //
            // Close compressed stream and open a new (fake) one.  This serves mainly to flush Target caches.
            //
            status = BMILZStreamStart (hifDevice, 0x00);
            if (status != A_OK)
            {
                break;
            }
        }

    } while (FALSE);

    if (buffer)
    {
        free (buffer);
        buffer = NULL;
    }
    if (fd)
    {
        CloseHandle (fd);
    }

    return status;
}

A_STATUS apply_patch (HIF_DEVICE *hifDevice,
                      A_UINT32 TargetVersion,
                      wchar_t *patchFile,
                      wchar_t *fileRoot,
                      AR6K_BIN_CACHE_INFO *pRomCache)
{
    A_STATUS status = A_OK;
    HANDLE fd = INVALID_HANDLE_VALUE;
    wchar_t filePath[128];
    A_UINT32 ret = 0;
    A_UINT32 id = 0;
    A_UINT32 tpid = 0;
    A_UINT32 chkSum = 0;
    A_UINT32 count = 0;
    A_UINT32 max = 0;
    A_UINT32 cmd = 0;
    A_UINT32 romAddr = 0;
    A_UINT32 ramAddr = 0;
    A_UINT32 reMapSz = 0;
    A_UCHAR  *buffer = NULL;
    A_UINT32 length = 0;
    A_UINT32 i = 0;
    BY_HANDLE_FILE_INFORMATION finfo;
    A_UINT32 fileSize = 0;

    do
    {
        if (pRomCache->Valid) {
            buffer = pRomCache->pData;
            fileSize = pRomCache->ActualLength;
        } else {
            if ( wcslen(patchFile) + wcslen(fileRoot) > 127 )
            {
                 ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: File Name Very Long :: ====>");
                return A_ERROR;
            }

            wcscpy(filePath,fileRoot);
            wcscat(filePath,patchFile);

            ATHR_DISPLAY_MSG (L"file %s \n",filePath);
            //Read the patch distribution file
            if ( (fd = CreateFile (filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
            {
                ATHR_DISPLAY_MSG (L"CAR6K:: No Patch File :: ====> %s", filePath);
                break;
            }

            if (!GetFileInformationByHandle(fd, &finfo))
            {
                ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: File Size Error :: ====> %s", filePath);
                status = A_ERROR;
                break;
            }
            fileSize = finfo.nFileSizeLow;
            if ((buffer=A_MALLOC(fileSize)) == NULL)
            {
                ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Unable allocated memory :: ====> %d",fileSize);
                status = A_ERROR;
                break;
            }
            if ( !ReadFile(fd, buffer, fileSize, &ret, NULL) || (ret != fileSize))
            {
                ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Unable to read rpdf ID from :: ====> %s",filePath);
                status = A_ERROR;
                break;
            }
            if (pRomCache->Static)
            {
                pRomCache->Valid = TRUE;
                pRomCache->pData = buffer;
                pRomCache->ActualLength = fileSize;
                pRomCache->MaxLength = fileSize;
            }
        }
        id = *((A_UINT32 *)buffer);
        buffer += sizeof(id);

        chkSum = *((A_UINT32 *)buffer);
        buffer += sizeof(chkSum);

        count = *((A_UINT32 *)buffer);
        buffer += sizeof(count);

        if ( TargetVersion == AR6002_VERSION_REV2 )
        {
             max = AR6002_REV2_MAX_PATCHES;
        }

        if (count > max)
        {
            ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Maximum patches supported :: %d ====> %d",max,count);
            status = A_ERROR;
            break;
        }

        for (; count; count--)
        {
            cmd = 0;
            romAddr = 0;
            ramAddr = 0;
            reMapSz = 0;
            length = 0;
            tpid = 0;

            cmd = *((A_UINT32 *)buffer);
            buffer += sizeof(cmd);

            if ( cmd != RPDF_CMD_INSTALL && cmd != RPDF_CMD_INST_ACT )
            {
                ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Unknown command in patch specification :: ====> %d",cmd);
                status = A_ERROR;
                break;
            }

            reMapSz= *((A_UINT32 *)buffer);
            buffer += sizeof(reMapSz);

            if ( reMapSz == 0 || reMapSz & (reMapSz-1) )
            {
                ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Invalid remap size :: ====> 0x%x",reMapSz);
                status = A_ERROR;
                break;
            }

            romAddr= *((A_UINT32 *)buffer);
            buffer += sizeof(romAddr);

            ramAddr= *((A_UINT32 *)buffer);
            buffer += sizeof(ramAddr);

            length= *((A_UINT32 *)buffer);
            buffer += sizeof(length);

            if (length > 0)
            {
                status = BMIWriteMemory(hifDevice, ramAddr, buffer, length);
                if (status != A_OK)
                {
                    ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Unable write patch to target :: ====> ");
                    break;
                }

                buffer += length;
            }

            if (cmd == RPDF_CMD_INST_ACT || cmd == RPDF_CMD_INSTALL)
            {
                status = BMIrompatchInstall(hifDevice, romAddr, ramAddr, reMapSz, 0, &tpid);
                if (status != A_OK)
                {
                    ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Unable write patch registers in target :: ====> ");
                    break;
                }

                InstalledPatches[NumPatches++] = tpid;
            }

            if (cmd == RPDF_CMD_INST_ACT && NumPatches)
            {
                status = BMIrompatchActivate(hifDevice, NumPatches, InstalledPatches);
                if (status != A_OK)
                {
                    ATHR_DISPLAY_MSG (L"CAR6K::WARNING!!!! :: Unable activate patch in target :: ====> ");
                    break;
                }
            }
        }
    } while (FALSE);

    if ((!pRomCache->Valid) && (buffer))
    {
       A_FREE(buffer);
    }

    if (fd)
    {
       CloseHandle (fd);
    }
    NumPatches = 0;

    return status;
}


A_STATUS ar6002_download_image(HIF_DEVICE *hifDevice,
                               A_UINT32    TargetVersion,
                               wchar_t    *fileName,
                               wchar_t    *fileRoot,
                               A_BOOL      bCompressed,
                               A_BOOL      isColdBoot)
{
    A_UINT32 address    = 0;
    A_STATUS status     = A_OK;
    A_UINT32 ini_patch_address = 0;

    do
    {
        address = AR6002_ATHWLAN_REV2_ADDRESS;

        if (isColdBoot) {
#ifdef EMBEDDED_AR6K_FIRMWARE
        if (!wcscmp(fileName, TCMD_BINARY_REV2_STR))
        {

#ifdef CONFIG_HOST_TCMD_SUPPORT
            status = download_binary (hifDevice,
                                      address,
                                      fileName,
                                      fileRoot,
                                      bCompressed,
                                      GET_TCMD_CACHE_INFO());
#endif
        }
        else if (!wcscmp(fileName, ART_BINARY_REV2_STR))
        {
#ifdef WINCE_ART
            status = download_binary (hifDevice,
                                      address,
                                      fileName,
                                      fileRoot,
                                      bCompressed,
                                      GET_ART_CACHE_INFO());
#endif
        }
        else
        {
            status = download_binary (hifDevice,
                                      address,
                                      fileName,
                                      fileRoot,
                                      bCompressed,
                                      GET_FIRMWARE_CACHE_INFO());
        }
#else
            status = download_binary (hifDevice,
                                      address,
                                      fileName,
                                      fileRoot,
                                      bCompressed,
                                      GET_FIRMWARE_CACHE_INFO());
#endif


            if (status != A_OK)
            {
                break;
            }
#ifdef EMBEDDED_AR6K_FIRMWARE
#ifdef AR6K_ROMPATCH_APPLY
            status = apply_patch (hifDevice,
                                  TargetVersion,
                                  WLAN_PATCH_REV2_STR,
                                  fileRoot,
                                  GET_ROMPATCH_CACHE_INFO());
            if (status != A_OK)
            {
                break;
            }
#endif
#else

            status = apply_patch (hifDevice,
                                  TargetVersion,
                                  WLAN_PATCH_REV2_STR,
                                  fileRoot,
                                  GET_ROMPATCH_CACHE_INFO());
            if (status != A_OK)
            {
                break;
            }
#endif

            // Download INI Patch
            address = AR6002_INIPATCH_REV2_ADDRESS;
            status = download_binary(hifDevice,
                                 address,
                                 INIPATCH_REV2_STR,
                                 fileRoot,
                                 FALSE,
                                 GET_DATAPATCH_CACHE_INFO());

            if (status != A_OK)
            {
                break;
            }
        }

        address = AR6002_INIPATCH_REV2_ADDRESS;

        status = BMIWriteMemory(hifDevice,
                                AR6002_HOST_INTEREST_ITEM_ADDRESS(hi_dset_list_head),
                                (A_UCHAR *)&address,
                                4);
        if (status != A_OK)
        {
            break;
        }

    } while (FALSE);

    return status;
}

/* Modules for firmware download */
A_STATUS configure_ar6000(HIF_DEVICE *hifDevice,
                           A_UINT32    TargetType,
                           A_UINT32    TargetVersion,
                           A_BOOL      enableUART,
                           A_BOOL      timerWAR,
                           A_UINT32    clkFreq,
                           wchar_t     *fileName,
                           wchar_t     *fileRoot,
                           A_BOOL      bCompressed,
                           A_BOOL      isColdBoot,
                           wchar_t     *eepromFile)
{
    A_STATUS status   = A_OK;
    A_UINT32 value    = 0;
    A_UINT32 oldSleep = 0;

    /* do any target-specific preparation that can be done through BMI */
    do
    {
        if (enableUART)
        {
            A_UINT32 uartparam;
            uartparam = 1;
            status = BMIWriteMemory(hifDevice,
                                HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_serial_enable),
                                (A_UCHAR *)&uartparam,
                                4);
            if (status != A_OK)
            {
                break;
            }
        }

        value = HTC_PROTOCOL_VERSION;
        status = BMIWriteMemory (hifDevice,
                                 HOST_INTEREST_ITEM_ADDRESS (TargetType, hi_app_host_interest),
                                (A_UCHAR *)&value,
                                4);
        if (status != A_OK)
        {
            break;
        }

        /* Selectively enable/disable the WAR for Timer Hang Symptom in the
         * firmware by setting a bit in the AR6K Scratch register. The firmware will
         * read this during init and appropriately enable/disable the WAR.
         * This setting will be retained in the firmware till target reset.
         */
        if (timerWAR)
        {
            A_UINT32 timerparam;

            status = BMIReadMemory(hifDevice,
                                   HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_option_flag),
                                   (A_UCHAR *)&timerparam,
                                   4);
            if (status != A_OK)
            {
                break;
            }
            timerparam |= HI_OPTION_TIMER_WAR;

            status = BMIWriteMemory(hifDevice,
                                    HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_option_flag),
                                    (A_UCHAR *)&timerparam,
                                    4);
            if (status != A_OK)
            {
                break;
            }
        }

        if (TargetType == TARGET_TYPE_AR6001)
        {
#ifdef FLASH_18V
            // Change the flash access time for 1.8V flash to 150ns
            status = BMIWriteSOCRegister (hifDevice,  0xac004004,  0x920100d1);
            if (status != A_OK)
            {
                break;
            }
#endif

        }
         /* set the firmware mode to AP */

        {
            A_UINT32 param;
            A_UINT32 fwmode = HI_OPTION_FW_MODE_BSS_STA;

            if (BMIReadMemory(hifDevice,
                HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_option_flag),
                (A_UCHAR *)&param,4)!= A_OK)
            {
                return -1;
            }

            param |= (fwmode << HI_OPTION_FW_MODE_SHIFT);

            if (BMIWriteMemory(hifDevice,
                HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_option_flag),
                (A_UCHAR *)&param,4) != A_OK)
            {
                return -1;
            }
        }

        if (TargetType == TARGET_TYPE_AR6002)
        {
            // Store the value of sleep
            oldSleep = 0x0;
            status = BMIReadSOCRegister (hifDevice, 0x40C4, &oldSleep);
            if (status != A_OK)
            {
                break;
            }

            // disable sleep
            status = BMIWriteSOCRegister (hifDevice, 0x40C4, oldSleep |  0x1);
            if (status != A_OK)
            {
                break;
            }

            status = ar6002_download_image(hifDevice, TargetVersion, fileName, fileRoot, bCompressed,isColdBoot);
            if (status != A_OK)
            {
               break;
            }

            // Restore the value of sleep
            status = BMIWriteSOCRegister (hifDevice, 0x40C4, oldSleep);
            if (status != A_OK)
            {
                break;
            }

            if ((TargetVersion == AR6002_VERSION_REV2) && (AR6002_REV2_APP_START_OVERRIDE != 0)) {
                    /* override default app start address known to ROM  */
                status = BMISetAppStart(hifDevice, AR6002_REV2_APP_START_OVERRIDE);
                if (status != A_OK) {
                    break;
                }
            }

        }

        status = ar6000_configure_clock (hifDevice, TargetType, TargetVersion, clkFreq);
        if (status != A_OK)
        {
            break;
        }

        status = eeprom_ar6000_transfer (hifDevice, TargetType, fileRoot, eepromFile);
        if (status != A_OK)
        {
            break;
        }

    } while (FALSE);

    return status;
}

void
config_exit()
{
#ifdef AR6K_FIRMWARE_CACHE
    if (g_RomPatchCache.pData)
    {
       A_FREE(g_RomPatchCache.pData);
    }
#endif
    return;
}
