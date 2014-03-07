
/*
 *
 * Copyright (c) 2004-2008 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
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
 *
 */

#include "system.h"
#include "AR6001/addrs.h"
#include "AR6001/hw/mbox_host_reg.h"
#include "AR6001/hw/rtc_reg.h"
#include "AR6001/hw/mc_reg.h"
#include "AR6001/hw/mbox_reg.h"
#include "targaddrs.h"
#include "u_hif.h"
#include "u_bmi.h"

#include <wmi.h>
#include <strsafe.h>

#define SAFE_STR_CPY(d,s,c)  StringCbCopyA((d),(c),(s))
//#define SAFE_STR_CPY(d,s,c)  strncpy((d),(s),(c))
#define SAFE_STR_CAT(d,s,c)  StringCchCatA((d),(c),(s))
//#define SAFE_STR_CAT(d,s,c)  strncat((d),(s),(c))


#define USE_4BYTE_REGISTER_ACCESS
//#define VERIFY_TRANSFER

#define AR6002_VERSION_REV2     0x20000188

#define HOST_INTEREST_ITEM_ADDRESS(target, item)    \
(((target) == TARGET_TYPE_AR6001) ?     \
   AR6001_HOST_INTEREST_ITEM_ADDRESS(item) :    \
   AR6002_HOST_INTEREST_ITEM_ADDRESS(item))

static A_STATUS VerifyTransfer(HIF_DEVICE *hifDevice, A_UCHAR *pOrigBuffer, A_UINT32 TargetAddress, A_UINT32 Length);

static A_CHAR GetHexNibble(A_UINT8 Nibble)
{
    if (Nibble <= 9) {
        return (A_CHAR)('0' + Nibble);
    }

    Nibble -= 10;
    return (A_CHAR)('A' + Nibble);
}

static void AddHexNibbleString(A_CHAR *pBuffer, int MaxLength, A_UINT8 Value)
{
    A_CHAR num[3];

    A_MEMZERO(num, sizeof(num));
    num[0] = GetHexNibble(Value & 0xF);
    num[1] = GetHexNibble((Value >> 4) & 0xF);

    if (MaxLength) {
        SAFE_STR_CPY(pBuffer,num,MaxLength);
    }
}

void DebugDumpBytes(A_UCHAR *buffer, A_UINT16 length, char *pDescription)
{
    A_CHAR stream[60];
    A_UINT32 i;
    A_UINT16 offset, count, linenum;

    AR_DEBUG_PRINTF(ATH_DEBUG_ANY, ("<---------Dumping %d Bytes : %s ------>\n", length, pDescription));

    count = 0;
    offset = 0;
    linenum = 0;
    for(i = 0; i < length; i++) {
        //sprintf(stream + offset, "%2.2X ", buffer[i]);
        AddHexNibbleString(stream + offset, (60 - offset), buffer[i]);
        SAFE_STR_CAT(stream + offset + 2," ", (58 - offset));
        count ++;
        offset += 3;

        if(count == 16) {
            count = 0;
            offset = 0;
            AR_DEBUG_PRINTF(ATH_DEBUG_ANY, ("[%X]: %s\n", linenum, stream));
            A_MEMZERO(stream, 60);
            linenum += 16;
        }
    }

    if(offset != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ANY, ("[%X]: %s\n", linenum, stream));
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_ANY, ("<------------------------------------------------->\n"));

}


/* Compile the 4BYTE version of the window register setup routine,
 * This mitigates host interconnect issues with non-4byte aligned bus requests, some
 * interconnects use bus adapters that impose strict limitations.
 * Since diag window access is not intended for performance critical operations, the 4byte mode should
 * be satisfactory even though it generates 4X the bus activity. */

#ifdef USE_4BYTE_REGISTER_ACCESS

    /* set the window address register (using 4-byte register access ). */
A_STATUS ar6000_SetAddressWindowRegister(HIF_DEVICE *hifDevice, A_UINT32 RegisterAddr, A_UINT32 Address)
{
    A_STATUS status;
    A_UINT8 addrValue[4];
    int i;

        /* write bytes 1,2,3 of the register to set the upper address bytes, the LSB is written
         * last to initiate the access cycle */

    for (i = 1; i <= 3; i++) {
            /* fill the buffer with the address byte value we want to hit 4 times*/
        addrValue[0] = ((A_UINT8 *)&Address)[i];
        addrValue[1] = addrValue[0];
        addrValue[2] = addrValue[0];
        addrValue[3] = addrValue[0];

            /* hit each byte of the register address with a 4-byte write operation to the same address,
             * this is a harmless operation */
        status = HIFReadWrite(hifDevice,
                              RegisterAddr+i,
                              addrValue,
                              4,
                              HIF_WR_SYNC_BYTE_FIX,
                              NULL);
        if (status != A_OK) {
            break;
        }
    }

    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Cannot write initial bytes of 0x%x to window reg: 0x%X \n",
             RegisterAddr, Address));
        return status;
    }

        /* write the address register again, this time write the whole 4-byte value.
         * The effect here is that the LSB write causes the cycle to start, the extra
         * 3 byte write to bytes 1,2,3 has no effect since we are writing the same values again */
    status = HIFReadWrite(hifDevice,
                          RegisterAddr,
                          (A_UCHAR *)(&Address),
                          4,
                          HIF_WR_SYNC_BYTE_INC,
                          NULL);

    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Cannot write 0x%x to window reg: 0x%X \n",
            RegisterAddr, Address));
        return status;
    }

    return A_OK;



}


#else

    /* set the window address register */
A_STATUS ar6000_SetAddressWindowRegister(HIF_DEVICE *hifDevice, A_UINT32 RegisterAddr, A_UINT32 Address)
{
    A_STATUS status;

        /* write bytes 1,2,3 of the register to set the upper address bytes, the LSB is written
         * last to initiate the access cycle */
    status = HIFReadWrite(hifDevice,
                          RegisterAddr+1,  /* write upper 3 bytes */
                          ((A_UCHAR *)(&Address))+1,
                          sizeof(A_UINT32)-1,
                          HIF_WR_SYNC_BYTE_INC,
                          NULL);

    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Cannot write initial bytes of 0x%x to window reg: 0x%X \n",
             RegisterAddr, Address));
        return status;
    }

        /* write the LSB of the register, this initiates the operation */
    status = HIFReadWrite(hifDevice,
                          RegisterAddr,
                          (A_UCHAR *)(&Address),
                          sizeof(A_UINT8),
                          HIF_WR_SYNC_BYTE_INC,
                          NULL);

    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Cannot write 0x%x to window reg: 0x%X \n",
            RegisterAddr, Address));
        return status;
    }

    return A_OK;
}

#endif

/*
 * Read from the AR6000 through its diagnostic window.
 * No cooperation from the Target is required for this.
 */
A_STATUS
ar6000_ReadRegDiag(HIF_DEVICE *hifDevice, A_UINT32 *address, A_UINT32 *data)
{
    A_STATUS status;

        /* set window register to start read cycle */
    status = ar6000_SetAddressWindowRegister(hifDevice,
                                             WINDOW_READ_ADDR_ADDRESS,
                                             *address);

    if (status != A_OK) {
        return status;
    }

        /* read the data */
    status = HIFReadWrite(hifDevice,
                          WINDOW_DATA_ADDRESS,
                          (A_UCHAR *)data,
                          sizeof(A_UINT32),
                          HIF_RD_SYNC_BYTE_INC,
                          NULL);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Cannot read from WINDOW_DATA_ADDRESS\n"));
        return status;
    }

    return status;
}


/*
 * Write to the AR6000 through its diagnostic window.
 * No cooperation from the Target is required for this.
 */
A_STATUS
ar6000_WriteRegDiag(HIF_DEVICE *hifDevice, A_UINT32 *address, A_UINT32 *data)
{
    A_STATUS status;

        /* set write data */
    status = HIFReadWrite(hifDevice,
                          WINDOW_DATA_ADDRESS,
                          (A_UCHAR *)data,
                          sizeof(A_UINT32),
                          HIF_WR_SYNC_BYTE_INC,
                          NULL);
    if (status != A_OK) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Cannot write 0x%x to WINDOW_DATA_ADDRESS\n", *data));
        return status;
    }

        /* set window register, which starts the write cycle */
    return ar6000_SetAddressWindowRegister(hifDevice,
                                           WINDOW_WRITE_ADDR_ADDRESS,
                                           *address);
    }

A_STATUS
ar6000_ReadDataDiag(HIF_DEVICE *hifDevice, A_UINT32 address,
                    A_UCHAR *data, A_UINT32 length)
{
    A_UINT32 count;
    A_STATUS status = A_OK;

    for (count = 0; count < length; count += 4, address += 4) {
        if ((status = ar6000_ReadRegDiag(hifDevice, &address,
                                         (A_UINT32 *)&data[count])) != A_OK)
        {
            break;
        }
    }

    return status;
}

A_STATUS
ar6000_WriteDataDiag(HIF_DEVICE *hifDevice, A_UINT32 address,
                    A_UCHAR *data, A_UINT32 length)
{
    A_UINT32 count;
    A_STATUS status = A_OK;

    for (count = 0; count < length; count += 4, address += 4) {
        if ((status = ar6000_WriteRegDiag(hifDevice, &address,
                                         (A_UINT32 *)&data[count])) != A_OK)
        {
            break;
        }
    }

    return status;
}

A_STATUS
ar6000_reset_device_skipflash(HIF_DEVICE *hifDevice)
{
    int i;
    struct forceROM_s {
        A_UINT32 addr;
        A_UINT32 data;
    };
    struct forceROM_s *ForceROM;
    int szForceROM;
    A_UINT32 instruction;

    static struct forceROM_s ForceROM_REV2[] = {
        /* NB: This works for old REV2 ROM (old). */
        {0x00001ff0, 0x175b0027}, /* jump instruction at 0xa0001ff0 */
        {0x00001ff4, 0x00000000}, /* nop instruction at 0xa0001ff4 */

        {MC_REMAP_TARGET_ADDRESS, 0x00001ff0}, /* remap to 0xa0001ff0 */
        {MC_REMAP_COMPARE_ADDRESS, 0x01000040},/* ...from 0xbfc00040 */
        {MC_REMAP_SIZE_ADDRESS, 0x00000000},   /* ...1 cache line */
        {MC_REMAP_VALID_ADDRESS, 0x00000001},  /* ...remap is valid */

        {LOCAL_COUNT_ADDRESS+0x10, 0}, /* clear BMI credit counter */

#define WARM_RESET_WAR 1
#if WARM_RESET_WAR
        /*
         * Need to do 2 consecutive warm resets.  Current belief is
         * that if the first warm reset occurs while there are outstanding
         * reads (e.g. to flash) then instruction/data loads in startup
         * code -- which executes just after the reset -- may return
         * bogus results.  The second warm reset occurs quickly enough
         * so that we're likely to still be in ROM code (i.e. not
         * accessing flash) when it occurs, and everything works well.
         * TBDXXX: Hardware Eng to verify that a warm reset with pending
         * reads to flash is problematic.
         */
        {RESET_CONTROL_ADDRESS, RESET_CONTROL_WARM_RST_MASK},
#endif /* WARM_RESET_WAR */
        {RESET_CONTROL_ADDRESS, RESET_CONTROL_WARM_RST_MASK},
    };

    static struct forceROM_s ForceROM_NEW[] = {
        /* NB: This works for AR6000 ROM REV3 and beyond.  */
        {LOCAL_SCRATCH_ADDRESS, AR6K_OPTION_IGNORE_FLASH},
        {LOCAL_COUNT_ADDRESS+0x10, 0}, /* clear BMI credit counter */
        {RESET_CONTROL_ADDRESS, RESET_CONTROL_WARM_RST_MASK},
    };

    /*
     * Examine a semi-arbitrary instruction that's different
     * in REV2 and other revisions.
     * NB: If a Host port does not require simultaneous support
     * for multiple revisions of Target ROM, this code can be elided.
     */
    (void)ar6000_ReadDataDiag(hifDevice, 0x01000040,
                              (A_UCHAR *)&instruction, 4);

    AR_DEBUG_PRINTF(ATH_LOG_ERR, ("instruction=0x%x\n", instruction));

    if (instruction == 0x3c1aa200) {
        /* It's an old ROM */
        ForceROM = ForceROM_REV2;
        szForceROM = sizeof(ForceROM_REV2)/sizeof(*ForceROM);
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Using OLD method\n"));
    } else {
        ForceROM = ForceROM_NEW;
        szForceROM = sizeof(ForceROM_NEW)/sizeof(*ForceROM);
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Using NEW method\n"));
    }

    AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Force Target to execute from ROM....\n"));
    for (i = 0; i < szForceROM; i++)
    {
        if (ar6000_WriteRegDiag(hifDevice,
                                &ForceROM[i].addr,
                                &ForceROM[i].data) != A_OK)
        {
            AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Cannot force Target to execute ROM!\n"));
            return A_ERROR;
        }
    }

    /*
     * Delay to allow dragon to enter BMI phase.
     * This covers 1 second that AR6001 firmware
     * allows for any low frequency crystal to
     * settle before it calibrates the refclk,
     * plus a bit of time to complete initialization.
     */
    A_MDELAY(1050);

    return A_OK;
}

/* reset device */
A_STATUS ar6000_reset_device(HIF_DEVICE *hifDevice, A_UINT32 TargetType)
{
    A_STATUS status = A_OK;
    A_UINT32 address;
    A_UINT32 data;

    do {

        // address = RESET_CONTROL_ADDRESS;
        data = RESET_CONTROL_COLD_RST_MASK;

          /* Hardcode the address of RESET_CONTROL_ADDRESS based on the target type */
        if (TargetType == TARGET_TYPE_AR6001) {
            address = 0x0C000000;
        } else {
            if (TargetType == TARGET_TYPE_AR6002) {
                address = 0x00004000;
            } else {
                A_ASSERT(0);
            }
        }

        status = ar6000_WriteRegDiag(hifDevice, &address, &data);

        if (A_FAILED(status)) {
            break;
        }

        /*
         * Read back the RESET CAUSE register to ensure that the cold reset
         * went through.
         */
        A_MDELAY(2000); /* 2 second delay to allow things to settle down */


        // address = RESET_CAUSE_ADDRESS;
        /* Hardcode the address of RESET_CAUSE_ADDRESS based on the target type */
        if (TargetType == TARGET_TYPE_AR6001) {
            address = 0x0C0000CC;
        } else {
            if (TargetType == TARGET_TYPE_AR6002) {
                address = 0x000040C0;
            } else {
                A_ASSERT(0);
            }
        }

        data = 0;
        status = ar6000_ReadRegDiag(hifDevice, &address, &data);

        if (A_FAILED(status)) {
            break;
        }

        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Reset Cause readback: 0x%X \n",data));
        data &= RESET_CAUSE_LAST_MASK;
        if (data != 2) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Unable to cold reset the target \n"));
        }

    } while (FALSE);

    if (A_FAILED(status)) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Failed to reset target \n"));
    }

    return A_OK;
}

#define REG_DUMP_COUNT_AR6001   38  /* WORDs, derived from AR6001_regdump.h */
#define REG_DUMP_COUNT_AR6002   32  /* WORDs, derived from AR6002_regdump.h */


#if REG_DUMP_COUNT_AR6001 <= REG_DUMP_COUNT_AR6002
#define REGISTER_DUMP_LEN_MAX  REG_DUMP_COUNT_AR6002
#else
#define REGISTER_DUMP_LEN_MAX  REG_DUMP_COUNT_AR6001
#endif

void ar6000_dump_target_assert_info(HIF_DEVICE *hifDevice, A_UINT32 TargetType)
{
    A_UINT32 address;
    A_UINT32 regDumpArea = 0;
    A_STATUS status;
    A_UINT32 regDumpValues[REGISTER_DUMP_LEN_MAX];
    A_UINT32 regDumpCount = 0;
    A_UINT32 i;

    do {

            /* the reg dump pointer is copied to the host interest area */
        address = HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_failure_state);

        if (TargetType == TARGET_TYPE_AR6001) {
                /* for AR6001, this is a fixed location because the ptr is actually stuck in cache,
                 * this may be fixed in later firmware versions */
            address = 0x18a0;
            regDumpCount = REG_DUMP_COUNT_AR6001;

        } else  if (TargetType == TARGET_TYPE_AR6002) {

            regDumpCount = REG_DUMP_COUNT_AR6002;

        } else {
            A_ASSERT(0);
        }

            /* read RAM location through diagnostic window */
        status = ar6000_ReadRegDiag(hifDevice, &address, &regDumpArea);

        if (A_FAILED(status)) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("AR6K: Failed to get ptr to register dump area \n"));
            break;
        }

        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("AR6K: Location of register dump data: 0x%X \n",regDumpArea));

        if (regDumpArea == 0) {
                /* no reg dump */
            break;
        }

        if (TargetType == TARGET_TYPE_AR6001) {
            regDumpArea &= 0x0FFFFFFF;  /* convert to physical address in target memory */
        }

            /* fetch register dump data */
        status = ar6000_ReadDataDiag(hifDevice,
                                     regDumpArea,
                                     (A_UCHAR *)&regDumpValues[0],
                                     regDumpCount * (sizeof(A_UINT32)));

        if (A_FAILED(status)) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("AR6K: Failed to get register dump \n"));
            break;
        }

        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("AR6K: Register Dump: \n"));

        for (i = 0; i < regDumpCount; i++) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,(" %d :  0x%X \n",i, regDumpValues[i]));
        }

    } while (FALSE);

}

/* set HTC/Mbox operational parameters, this can only be called when the target is in the
 * BMI phase */
A_STATUS ar6000_set_htc_params(HIF_DEVICE *hifDevice,
                               A_UINT32    TargetType,
                               A_UINT32    MboxIsrYieldValue,
                               A_UINT8     HtcControlBuffers)
{
    A_STATUS status;
    A_UINT32 blocksizes[HTC_MAILBOX_NUM_MAX];
    A_UINT32 value;

    do {
            /* get the block sizes */
        status = HIFConfigureDevice(hifDevice, HIF_DEVICE_GET_MBOX_BLOCK_SIZE,
                                    blocksizes, sizeof(blocksizes));

        if (A_FAILED(status)) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("Failed to get block size info from HIF layer...\n"));
            break;
        }
            /* note: we actually get the block size for mailbox 1, for SDIO the block
             * size on mailbox 0 is artificially set to 1 */
            /* must be a power of 2 */
        A_ASSERT((blocksizes[1] & (blocksizes[1] - 1)) == 0);

        if (HtcControlBuffers != 0) {
                /* set override for number of control buffers to use */
            blocksizes[1] |=  ((A_UINT32)HtcControlBuffers) << 16;
        }

            /* set the host interest area for the block size */
        status = BMIWriteMemory(hifDevice,
                                HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_mbox_io_block_sz),
                                (A_UCHAR *)&blocksizes[1],
                                4);

        if (A_FAILED(status)) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("BMIWriteMemory for IO block size failed \n"));
            break;
        }

        AR_DEBUG_PRINTF(ATH_LOG_INF,("Block Size Set: %d (target address:0x%X)\n",
                blocksizes[1], HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_mbox_io_block_sz)));

        if (MboxIsrYieldValue != 0) {
                /* set the host interest area for the mbox ISR yield limit */
            status = BMIWriteMemory(hifDevice,
                                    HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_mbox_isr_yield_limit),
                                    (A_UCHAR *)&MboxIsrYieldValue,
                                    4);

            if (A_FAILED(status)) {
                AR_DEBUG_PRINTF(ATH_LOG_ERR,("BMIWriteMemory for yield limit failed \n"));
                break;
            }
        }


        value = HTC_PROTOCOL_VERSION;
        status = BMIWriteMemory (hifDevice,
                                 HOST_INTEREST_ITEM_ADDRESS (TargetType, hi_app_host_interest),
                                 (A_UCHAR *)&value,
                                 4);

        if (A_FAILED(status)) {
            break;
        }


    } while (FALSE);

    return status;
}


static A_STATUS prepare_ar6002(HIF_DEVICE *hifDevice, A_UINT32 TargetVersion)
{
    A_STATUS status = A_OK;

    do {

        /* we need to disable
         * sleep as soon as possible as the on-board crystal can be wildly off causing our
         * sleep activation timer to fire off early */
        A_UINT32 value;

            /* read host interest area for system sleep setting */
        status = BMIReadMemory(hifDevice,
                               HOST_INTEREST_ITEM_ADDRESS(TARGET_TYPE_AR6002, hi_system_sleep_setting),
                               (A_UCHAR *)&value,
                               4);

        if (A_FAILED(status)) {
            break;
        }

            /* force the setting to disable sleep, this prevents our sleep activation timer
             * from having any effect, the host must reset the sleep setting back to zero after it
             * has completed all initialization */
        value |= 0x1;

        status = BMIWriteMemory(hifDevice,
                                HOST_INTEREST_ITEM_ADDRESS(TARGET_TYPE_AR6002, hi_system_sleep_setting),
                                (A_UCHAR *)&value,
                                4);

        if (A_FAILED(status)) {
            break;
        }

        /* also disable sleep immediately by programming the sleep register directly */

        status = BMIReadSOCRegister(hifDevice,0x40c4,&value);

        if (A_FAILED(status)) {
            break;
        }

        value |= 0x1;

        status = BMIWriteSOCRegister(hifDevice,0x40c4,value);

        if (A_FAILED(status)) {
            break;
        }

   } while (FALSE);

   return status;
}

/* this function assumes the caller has already initialized the BMI APIs */
A_STATUS ar6000_prepare_target(HIF_DEVICE *hifDevice,
                               A_UINT32    TargetType,
                               A_UINT32    TargetVersion)
{
    if (TargetType == TARGET_TYPE_AR6002) {
            /* do any preparations for AR6002 devices */
        return prepare_ar6002(hifDevice,TargetVersion);
    }

    return A_OK;
}

/* initialize target console prints */
A_STATUS ar6000_enable_target_console(HIF_DEVICE *hifDevice,
                                      A_UINT32    TargetType)
{
   A_UINT32 param = 1;

   return BMIWriteMemory(hifDevice,
                         HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_serial_enable),
                         (A_UCHAR *)&param,
                         4);

}


#define A_ROUND_UP(x, y)             ((((x) + ((y) - 1)) / (y)) * (y))

#define AR6002_INIPATCH_REV2_ADDRESS  0x52db64
#define AR6002_ATHWLAN_REV2_ADDRESS   0x502400


A_STATUS ar6000_set_wmi_protocol_ver(HIF_DEVICE *hifDevice, A_UINT32 TargetType, A_UINT32 WmiProtocolVersion)
{
    A_UINT32 address, data;
    A_STATUS status;
    struct   host_app_area_s host_app_area;

    A_MEMZERO(&host_app_area, sizeof(host_app_area));

    do {
            /* Fetch the address of the host_app_area_s instance in the host interest area */
        address = HOST_INTEREST_ITEM_ADDRESS(TargetType, hi_app_host_interest);

        status = ar6000_ReadRegDiag(hifDevice, &address, &data);

        if (A_FAILED(status)) {
           break;
        }

        address = data;

        status = ar6000_ReadDataDiag(hifDevice,
                                     address,
                                     (A_UCHAR *)&host_app_area,
                                     sizeof(struct host_app_area_s));

        if (A_FAILED(status)) {
           break;
        }

        host_app_area.wmi_protocol_ver = WmiProtocolVersion;

        status = ar6000_WriteDataDiag (hifDevice,
                                       address,
                                       (A_UCHAR *)&host_app_area,
                                       sizeof(struct host_app_area_s));

    } while (FALSE);

    return status;
}

A_STATUS ar6000_configure_clock (HIF_DEVICE *hifDevice,
                                 A_UINT32    TargetType,
                                 A_UINT32    TargetVersion,
                                 A_UINT32    clkFreq)
{
    A_UINT32 ext_clk_detected   = 0;
    A_STATUS status             = A_OK;

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

#define MAX_BUF (8*1024)

static A_UCHAR g_BinBuffer[MAX_BUF];

static A_STATUS Download_binary (HIF_DEVICE *hifDevice,
                                 A_UINT32   address,
                                 A_UCHAR    *pDataBuffer,
                                 A_UINT32   DataLength,
                                 A_BOOL     bCompressed)
{
    A_STATUS  status = A_OK;
    A_UCHAR   *buffer = g_BinBuffer;
    A_UINT32  length = 0;
    A_UINT32  next_address=0;
    A_UINT32  bytes_left;

    do {

        if (bCompressed)
        {
            status = BMILZStreamStart (hifDevice, address);
            if (status != A_OK)
            {
                break;
            }
        }

        bytes_left = A_ROUND_UP(DataLength, 4);

        next_address = address;

        while (bytes_left)
        {
            length  = min(bytes_left , MAX_BUF);

            if (bCompressed)
            {
                 //
                 // 0 pad last word of data to avoid messy uncompression
                 //
                 ((A_UINT32 *)buffer)[((length-1)/4)] = 0;
            }

            memcpy(buffer, pDataBuffer, length);

            if (bCompressed)
            {
                status = BMILZData(hifDevice, buffer, length);
            }
            else
            {
                status = BMIWriteMemory(hifDevice, next_address, buffer, length);
            }

            if (FAILED(status)) {
                break;
            }

#ifdef VERIFY_TRANSFER
            status = VerifyTransfer(hifDevice, pDataBuffer, next_address, length);
            if (FAILED(status)) {
                break;
            }
#endif

            pDataBuffer += length;
            next_address += length;
            bytes_left -= length;

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

    return status;
}

A_STATUS DownloadFirmware(HIF_DEVICE *hifDevice,
                          A_UINT32   TargetType,
                          A_UINT32   TargetVersion,
                          A_UCHAR    *pAthwlan,
                          A_UINT32   AthWlanLength,
                          A_BOOL     AthWlanCompressed,
                          A_UCHAR    *pDataPatch,
                          A_UINT32   DataPatchLength)
{
    A_UINT32 athWlanAddress, dataPatchAddress;
    A_STATUS status = A_OK;

    if (TargetType != TARGET_TYPE_AR6002) {
        return A_ERROR;
    }

    if (TargetVersion == AR6002_VERSION_REV2) {
        athWlanAddress = AR6002_ATHWLAN_REV2_ADDRESS;
        dataPatchAddress = AR6002_INIPATCH_REV2_ADDRESS;
    }

    do {

        status = Download_binary(hifDevice,
                                 athWlanAddress,
                                 pAthwlan,
                                 AthWlanLength,
                                 AthWlanCompressed);

        if (A_FAILED(status)) {
            break;
        }

        if (pDataPatch == NULL) {
            break;
        }

            /* data patch is optional */
        status = Download_binary(hifDevice,
                                 dataPatchAddress,
                                 pDataPatch,
                                 DataPatchLength,
                                 FALSE);

        if (A_FAILED(status)) {
            break;
        }

        status = BMIWriteMemory(hifDevice,
                                AR6002_HOST_INTEREST_ITEM_ADDRESS(hi_dset_list_head),
                                (A_UCHAR *)&dataPatchAddress,
                                4);

        if (A_FAILED(status)) {
            break;
        }


    } while (FALSE);


    return status;
}

#ifdef VERIFY_TRANSFER

static A_STATUS VerifyTransfer(HIF_DEVICE *hifDevice, A_UCHAR *pOrigBuffer, A_UINT32 TargetAddress, A_UINT32 Length)
{
    A_STATUS       status = A_OK;
    static A_UCHAR buffer[1024];
    A_UCHAR        *pDataPtr = pOrigBuffer;
    A_UINT32       address = TargetAddress;
    A_UINT32       thisLength;
    A_UINT32       totalLength = Length;
    A_UINT32       i;
    int            errors = 0;
    int            printErr;

    while (totalLength) {

        thisLength = min(1024, totalLength);

        memset(buffer,0xCE,thisLength);

        status = BMIReadMemory(hifDevice,address,buffer,thisLength);

        if (A_FAILED(status)) {
            break;
        }

        printErr = 10;

        for (i = 0; i < thisLength; i++) {
            if (pDataPtr[i] != buffer[i]) {
                errors++;
                if (printErr) {
                    printErr--;
                    DBG_LOG_PRINT(DBG_ZONE_ERR,
                        ("** DATA MISMATCH offset:0x%X was:0x%X, should be 0x%X \n",
                            (address + i), buffer[i], pDataPtr[i]));
                }
            }
        }

        if (errors) {
            DebugDumpBytes(pDataPtr, thisLength, "GOOD Buffer");
            DebugDumpBytes(buffer, thisLength, "BAD Buffer");
            break;
        }

        totalLength -= thisLength;
        address += thisLength;
        pDataPtr += thisLength;
    }

    DBG_LOG_PRINT(DBG_ZONE_ERR,
                        ("**Buffer MisMatch Errors: %d \n",errors));

    if (errors) {
        status = A_ERROR;
    }
    return status;
}

#endif
