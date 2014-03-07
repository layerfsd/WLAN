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
#include "athdefs.h"
#include "a_types.h"
#include "targaddrs.h"
#include "AR6002/hw/si_reg.h"
#include "AR6002/hw/gpio_reg.h"
#include "AR6002/hw/rtc_reg.h"
#include "u_hif.h"
#include "u_bmi.h"

#define HOST_INTEREST_ITEM_ADDRESS(target, item)    \
(((target) == TARGET_TYPE_AR6001) ?     \
   AR6001_HOST_INTEREST_ITEM_ADDRESS(item) :    \
   AR6002_HOST_INTEREST_ITEM_ADDRESS(item))
   
#define MAX_EEPROM_SZ                     768
#define EEPROM_WAIT_LIMIT             4

static void fetch_8bytes (HIF_DEVICE *device, int offset, A_UINT32 *data, A_UINT32 TargetType);
static void readboard_data (HIF_DEVICE *device, A_UINT32 address, A_UINT32 *pvalue);
static void writeboard_data (HIF_DEVICE *device, A_UINT32 address, A_UINT8 *data, A_UINT32 sz);

#define AR6000_RTC_BASE_ADDRESS(x)        (((x) == TARGET_TYPE_AR6001) ?0x0c000000:0x00004000)
#define AR6000_SLEEP_OFFSET(x)            (((x) == TARGET_TYPE_AR6001) ?0x000000d0:0x000000c4)
#define AR6000_CLOCK_CONTROL_OFFSET(x)    (((x) == TARGET_TYPE_AR6001) ?0x0000002c:0x00000028)
#define AR6000_GPIO_BASE_ADDRESS(x)       (((x) == TARGET_TYPE_AR6001) ?0x0c010000:0x00014000)
#define AR6000_SI_BASE_ADDRESS(x)         (((x) == TARGET_TYPE_AR6001) ?0x0c00c000:0x00010000)

A_STATUS
eeprom_ar6000_transfer (HIF_DEVICE *device, A_UINT32 TargetType)
{
    A_UINT32        regval = 0;
    A_UINT32        sys_sleep_reg;
    A_UINT32        board_data_addr = 0;
    A_UCHAR         eeprom_data[MAX_EEPROM_SZ];
    A_UINT32        i = 0;
    A_UINT32        eeprom_size;

    //
    //disable system sleep temporarily
    //
    BMIReadSOCRegister (device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                AR6000_SLEEP_OFFSET(TargetType),
                                &sys_sleep_reg);

    BMIWriteSOCRegister (device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                 AR6000_SLEEP_OFFSET(TargetType),
                                 SYSTEM_SLEEP_DISABLE_SET(1));


    BMIReadSOCRegister (device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                AR6000_CLOCK_CONTROL_OFFSET(TargetType),
                                &regval);
    regval &= ~CLOCK_CONTROL_SI0_CLK_MASK;
    BMIWriteSOCRegister (device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                 AR6000_CLOCK_CONTROL_OFFSET(TargetType),
                                 regval);


    BMIReadSOCRegister (device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                RESET_CONTROL_OFFSET,
                                &regval);
    regval &= ~RESET_CONTROL_SI0_RST_MASK;
    BMIWriteSOCRegister (device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                 RESET_CONTROL_OFFSET,
                                 regval);


    BMIReadSOCRegister (device, AR6000_GPIO_BASE_ADDRESS(TargetType)+
                                GPIO_PIN0_OFFSET,
                                &regval);
    regval &= ~GPIO_PIN0_CONFIG_MASK;
    BMIWriteSOCRegister (device, AR6000_GPIO_BASE_ADDRESS(TargetType)+
                                 GPIO_PIN0_OFFSET,
                                 regval);


    BMIReadSOCRegister (device, AR6000_GPIO_BASE_ADDRESS(TargetType)+
                                GPIO_PIN1_OFFSET,
                                &regval);
    regval &= ~GPIO_PIN1_CONFIG_MASK;
    BMIWriteSOCRegister (device, AR6000_GPIO_BASE_ADDRESS(TargetType)+
                                 GPIO_PIN1_OFFSET,
                                 regval);

    /* SI_CONFIG = 0x500a5; */
    regval =    SI_CONFIG_BIDIR_OD_DATA_SET(1)  |
                SI_CONFIG_I2C_SET(1)            |
                SI_CONFIG_POS_SAMPLE_SET(1)     |
                SI_CONFIG_INACTIVE_CLK_SET(1)   |
                SI_CONFIG_INACTIVE_DATA_SET(1)  |
                SI_CONFIG_DIVIDER_SET(6);

    BMIWriteSOCRegister (device, AR6000_SI_BASE_ADDRESS(TargetType)+
                                 SI_CONFIG_OFFSET,
                                 regval);

    if (TargetType == TARGET_TYPE_AR6001) 
    {
        eeprom_size = 512;
    }
    else 
    {
        eeprom_size = 768;
    }

    for (i = 0; i < eeprom_size; i += 8)
    {
        fetch_8bytes (device, i, (A_UINT32*)(&eeprom_data[i]), TargetType);
    }

    /* Determine where in Target RAM to write Board Data */
    readboard_data (device, HOST_INTEREST_ITEM_ADDRESS(TargetType,
                            hi_board_data),
                        &board_data_addr);

    if (board_data_addr == 0)
    {
        return A_ERROR;
    }

    /* Write EEPROM data to Target RAM */
    writeboard_data (device, board_data_addr, ((A_UINT8 *)eeprom_data), eeprom_size);

    /* Record the fact that Board Data IS initialized */
    {
       A_UINT32 one = 1;
       writeboard_data (device, HOST_INTEREST_ITEM_ADDRESS(TargetType,
                                hi_board_data_initialized),
                                (A_UINT8 *)&one, sizeof(A_UINT32));
    }

    // Disable SI
    BMIWriteSOCRegister(device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                RESET_CONTROL_OFFSET,
                                RESET_CONTROL_SI0_RST_MASK);
    BMIReadSOCRegister(device,  AR6000_RTC_BASE_ADDRESS(TargetType)+
                                AR6000_CLOCK_CONTROL_OFFSET(TargetType),
                                &regval);
    regval |= CLOCK_CONTROL_SI0_CLK_MASK;
    BMIWriteSOCRegister(device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                AR6000_CLOCK_CONTROL_OFFSET(TargetType),
                                regval);//Gate SI0 clock

    // Enable System Sleep setting
    BMIWriteSOCRegister (device, AR6000_RTC_BASE_ADDRESS(TargetType)+
                                 AR6000_SLEEP_OFFSET(TargetType),
                                 sys_sleep_reg);

    return A_OK;
}

/*
 * Check whether or not an EEPROM request that was started
 * earlier has completed yet.
 */
A_BOOL
request_in_progress (HIF_DEVICE *device, A_UINT32 TargetType)
{
    A_UINT32 regval;

    /* Wait for DONE_INT in SI_CS */
    BMIReadSOCRegister (device, AR6000_SI_BASE_ADDRESS(TargetType)+
                                SI_CS_OFFSET,
                                &regval);

    if (regval & SI_CS_DONE_ERR_MASK) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR,("Err:EEPROM signaled ERROR (0x%x)\n", regval));
    }

    return (!(regval & SI_CS_DONE_INT_MASK));
}


/*
 * Tell the Target to start an 8-byte read from EEPROM,
 * putting the results in Target RX_DATA registers.
 */
void
request_8byte_read (HIF_DEVICE *device,
                    int offset,
                    A_UINT32 TargetType)
{
    A_UINT32 regval;

    /* SI_TX_DATA0 = read from offset */
    regval =    (0xa1<<16)              |
                ((offset & 0xff)<<8)    |
                (0xa0 | ((offset & 0xff00)>>7));

    BMIWriteSOCRegister (device, AR6000_SI_BASE_ADDRESS(TargetType)+
                                 SI_TX_DATA0_OFFSET,
                                 regval);

    regval =    SI_CS_START_SET(1)      |
                SI_CS_RX_CNT_SET(8)     |
                SI_CS_TX_CNT_SET(3);

    BMIWriteSOCRegister (device, AR6000_SI_BASE_ADDRESS(TargetType)+
                                 SI_CS_OFFSET,
                                 regval);


}

/*
 * Wait for a previously started command to complete.
 * Timeout if the command is takes "too long".
 */
void
wait_for_eeprom_completion (HIF_DEVICE *device, A_UINT32 TargetType)
{
    int i = 0;

    while (request_in_progress(device, TargetType)) {
        if (i++ == EEPROM_WAIT_LIMIT)
        {
            i = 0;
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("Err:LEEPROM not responding\n"));
        }
    }
}

/*
 * Extract the results of a completed EEPROM Read request
 * and return them to the caller.
 */
void
read_8byte_results (HIF_DEVICE *device,
                    A_UINT32 *data,
                    A_UINT32 TargetType)
{
    /* Read SI_RX_DATA0 and SI_RX_DATA1 */
    BMIReadSOCRegister (device, AR6000_SI_BASE_ADDRESS(TargetType)+
                                SI_RX_DATA0_OFFSET,
                                &data[0]);
    BMIReadSOCRegister (device, AR6000_SI_BASE_ADDRESS(TargetType)+
                                SI_RX_DATA1_OFFSET,
                                &data[1]);
}

/*
 * High-level function which starts an 8-byte read,
 * waits for it to complete, and returns the result.
 */
static void
fetch_8bytes (HIF_DEVICE *device,
              int offset,
              A_UINT32 *data,
              A_UINT32 TargetType)
{
    request_8byte_read (device, offset, TargetType);
    wait_for_eeprom_completion (device, TargetType);
    read_8byte_results (device, data, TargetType);

    /* Clear any pending intr */
    BMIWriteSOCRegister (device, AR6000_SI_BASE_ADDRESS(TargetType)+
                                 SI_CS_OFFSET,
                                 SI_CS_DONE_INT_MASK);
}


static void
readboard_data (HIF_DEVICE *device,
                A_UINT32 address,
                A_UINT32 *pvalue)
{
    A_UCHAR    buffer[8] = {0};
    A_UINT32 dwAddress = 0;
    A_STATUS status;

    memset(buffer, 0, sizeof(buffer));

    status = BMIReadMemory (device, address, (A_UCHAR *)(&dwAddress), sizeof(A_UINT32));

    if ( status != A_OK)
    {
        *pvalue = (A_UINT32)NULL;
    }
    else
    {
        *pvalue = dwAddress;
    }

}

static void
writeboard_data (HIF_DEVICE *device,
                 A_UINT32 address,
                 A_UINT8 *data,
                 A_UINT32 sz)
{
   int chunk_sz;
   A_UCHAR buffer[BMI_DATASZ_MAX+4];
   A_STATUS status;

   memset(buffer, 0, sizeof(buffer));

   while (sz) {

      chunk_sz = (BMI_DATASZ_MAX > sz) ? sz : BMI_DATASZ_MAX;

      memcpy (buffer, data, chunk_sz);

      status = BMIWriteMemory (device, address, (A_UCHAR *)(&buffer[0]), chunk_sz);

      if (A_OK != status)
      {
          break;
      }

      sz -= chunk_sz;
      data += chunk_sz;
      address += chunk_sz;
   }
}


