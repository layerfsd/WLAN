/*++
Copyright (c) 2004-2008 Atheros Communications Inc.
All rights reserved.


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


 Simplified, single threaded (micro) implementation of HIF.
 
 This HIF layer implements the interface to the bootloader's
 SD API support.


--*/

#include "system.h"
#include "u_hif.h"
#include "sdapi.h"

#define HIF_MBOX_BLOCK_SIZE                128
#define HIF_MBOX_BASE_ADDR                 0x800
#define HIF_MBOX_WIDTH                     0x800
#define HIF_MBOX0_BLOCK_SIZE               1
#define HIF_MBOX1_BLOCK_SIZE               HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX2_BLOCK_SIZE               HIF_MBOX_BLOCK_SIZE
#define HIF_MBOX3_BLOCK_SIZE               HIF_MBOX_BLOCK_SIZE

#define HIF_MBOX_START_ADDR(mbox)                        \
    HIF_MBOX_BASE_ADDR + mbox * HIF_MBOX_WIDTH

#define HIF_MBOX_END_ADDR(mbox)                          \
    HIF_MBOX_START_ADDR(mbox) + HIF_MBOX_WIDTH - 1
    

/* I/O Enable  */
#define SDIO_ENABLE_REG         0x02
/* I/O Ready */
#define SDIO_READY_REG          0x03
/* Bus Interface */
#define SDIO_BUS_IF_REG         0x07
#define CARD_DETECT_DISABLE     0x80

#define SDIO_SET_CMD52_ARG(arg,rw,func,raw,address,writedata) \
    (arg) = (((rw) & 1) << 31)           | \
            (((func) & 0x7) << 28)       | \
            (((raw) & 1) << 27)          | \
            (1 << 26)                    | \
            (((address) & 0x1FFFF) << 9) | \
            (1 << 8)                     | \
            ((writedata) & 0xFF)

#define CMD53_BLOCK_BASIS   1 
#define CMD53_BYTE_BASIS    0
#define CMD53_FIXED_ADDRESS 0
#define CMD53_INCR_ADDRESS  1
#define SDIO_SET_CMD53_ARG(arg,rw,func,mode,opcode,address,bytes_blocks) \
    (arg) = (((rw) & 1) << 31)                  | \
            (((func) & 0x7) << 28)              | \
            (((mode) & 1) << 27)                | \
            (((opcode) & 1) << 26)              | \
            (((address) & 0x1FFFF) << 9)        | \
            ((bytes_blocks) & 0x1FF)
            
#define SD_SDIO_R5_READ_DATA_OFFSET    1
#define SD_R5_GET_READ_DATA(pR)  (pR)[SD_SDIO_R5_READ_DATA_OFFSET]


#define SD_SDIO_R4_GET_OCR(pR) ((A_UINT32)((pR)[1])        |  \
                          (((A_UINT32)(pR)[2]) << 8)  |  \
                          (((A_UINT32)(pR)[3]) << 16))

#define SD_SDIO_R4_IS_CARD_READY(pR)       (((pR)[4] & 0x80) == 0x80)
#define SD_SDIO_R6_GET_RCA(pR)             ((A_UINT16)((pR)[3]) | ((A_UINT16)((pR)[4]) << 8)) 


#define CMD52_WRITE TRUE
#define CMD52_READ  FALSE

static A_BOOL IssueCMD52(A_UINT32 Address,
                         A_UCHAR  Func,
                         A_BOOL   Write,
                         A_UINT8  *pData)
{
    SD_COMMAND_RESPONSE response;
    A_UINT32            arg;
    
    SDIO_SET_CMD52_ARG(arg,
                       Write ? 1 : 0,
                       Func,
                       0,
                       Address,
                       Write ? *pData : 0);
                       
    if (!SDIO_IssueCommand(52,arg, ResponseR5, &response)) {
        DBG_LOG_PRINT(DBG_ZONE_ERR,("CMD52 FAILED, Address:0x%X Func:%d  (%s) \r\n",
            Address,Func, Write ? "WRITE" : "READ" ));  
        return FALSE;    
    }
    
    if (!Write) {
        *pData = SD_R5_GET_READ_DATA(response.ResponseBuffer);   
    }
    
    return TRUE;
}

#define CMD53_WRITE TRUE
#define CMD53_READ  FALSE


HIF_DEVICE  *HIFGetDevice(void)
{
        /* just return something non-NULL */
    return (HIF_DEVICE *)0x1;   
}
                     
A_STATUS HIFInit(void)
{
    A_STATUS            success = A_ERROR;
    SD_COMMAND_RESPONSE response;
    DWORD               ocr;
    int                 cardReadyRetry = 10;
    A_UINT16            rca;
    A_UINT8             cmd52Data;
    
    DBG_LOG_PRINT(DBG_ZONE_HIF,("+HIFInit \r\n")); 
     
    
    do {
        
          
        if (!SDIO_SetupBus()) {
            break;    
        }
        
        SDIO_SetBusRate(25000000);  /* 25Mhz bus */
        
        DBG_LOG_PRINT(DBG_ZONE_HIF,("Clock set to 25Mhz, Issuing CMD0 ... \r\n"));  
        
        if (!SDIO_IssueCommand(0,0,NoResponse,NULL)) {
            break;    
        }
               
        DBG_LOG_PRINT(DBG_ZONE_HIF,("Issuing first CMD5 ... \r\n"));  
        
            /* issue CMD5, ARG=0 */
        if (!SDIO_IssueCommand(5,0,ResponseR4,&response)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("CMD5 FAILED, NO card detected \r\n"));  
            break;    
        }
        
        ocr = SD_SDIO_R4_GET_OCR(response.ResponseBuffer);
        
        DBG_LOG_PRINT(DBG_ZONE_HIF,("CMD5 success, OCR = 0x%X \r\n",ocr));  
        
            /* issue CMD5, ARG=0x00100000 repeatedly until card is ready */
        while (cardReadyRetry) {
            if (!SDIO_IssueCommand(5,
                                0x00100000, /* use 3.0 volts, although this really is a don't care */
                                ResponseR4,
                                &response)) {
                DBG_LOG_PRINT(DBG_ZONE_ERR,("CMD5 Failed \r\n"));  
                cardReadyRetry = 0;
                break;
            }            
            
            if (SD_SDIO_R4_IS_CARD_READY(response.ResponseBuffer)) {
                DBG_LOG_PRINT(DBG_ZONE_HIF,("SDIO Card is Ready! \r\n"));  
                break;    
            }
            cardReadyRetry--;    
            A_MDELAY(10);
        }
        
        if (cardReadyRetry == 0) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("CMD5 Ready polling Expired \r\n"));  
            break;    
        }
         
        DBG_LOG_PRINT(DBG_ZONE_HIF,("Issuing CMD3 ... \r\n")); 
        
             /* issue CMD3 */
        if (!SDIO_IssueCommand(3,0,ResponseR6,&response)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("CMD3 FAILED \r\n"));  
            break;    
        }
        
        rca = SD_SDIO_R6_GET_RCA(response.ResponseBuffer);
        
        DBG_LOG_PRINT(DBG_ZONE_HIF,("Card RCA : 0x%X \r\n",rca)); 
        
        DBG_LOG_PRINT(DBG_ZONE_HIF,("Issuing CMD7 ... \r\n")); 
         
              /* issue CMD7 to select card */
        if (!SDIO_IssueCommand(7,(((A_UINT32)rca) << 16),ResponseR1b,&response)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("CMD7 FAILED \r\n"));  
            break;    
        }
         
        DBG_LOG_PRINT(DBG_ZONE_HIF,("Card is now selected! \r\n")); 
                     
            /* disable card detect resistor */           
        cmd52Data = CARD_DETECT_DISABLE;            
        if (!IssueCMD52(SDIO_BUS_IF_REG,
                        0,
                        CMD52_WRITE,
                        &cmd52Data)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("Failed to disable CD resistor \r\n"));   
            break;                  
        }     
            /* enable 4-bit mode */
        cmd52Data = CARD_DETECT_DISABLE | 0x2;            
        if (!IssueCMD52(SDIO_BUS_IF_REG,
                        0,
                        CMD52_WRITE,
                        &cmd52Data)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("Failed to set 4 bit mode \r\n"));   
            break;                  
        }

 
            /* enable 4-bit at the SDHC */
        if (!SDIO_SetBusWidth(SD_BUS_4BIT)) {
            break;    
        }
               
        DBG_LOG_PRINT(DBG_ZONE_HIF,("Enabling I/O Function 1 .. \r\n")); 
         
            /* enable function 1 */
        cmd52Data =0x2;             
        if (!IssueCMD52(SDIO_ENABLE_REG,
                        0,
                        CMD52_WRITE,
                        &cmd52Data)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("Failed to write to I/O func enable register \r\n"));                     
            break;
        }
        
        cardReadyRetry = 10;
        
        while (cardReadyRetry) {
            
            if (!IssueCMD52(SDIO_READY_REG,
                            0,
                            CMD52_READ,
                            &cmd52Data)) {
                DBG_LOG_PRINT(DBG_ZONE_ERR,("Failed to read function I/O ready register \r\n"));                     
                cardReadyRetry = 0;
                break;
            }
                        
            if (cmd52Data & 0x2) {
                DBG_LOG_PRINT(DBG_ZONE_HIF,("I/O Function 1 is ready! \r\n")); 
                break;    
            }         
                
            cardReadyRetry--;    
            A_MDELAY(10);         
        } 
        
        if (cardReadyRetry == 0) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("Function I/O Ready polling Expired \r\n"));  
            break;    
        }
                
        cmd52Data = 128;
        if (!IssueCMD52(0x110,
                        0,
                        CMD52_WRITE,
                        &cmd52Data)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("Failed to write I/O block size LOW \r\n"));   
            break;
        }
        
        cmd52Data = 0;        
        if (!IssueCMD52(0x111,
                        0,
                        CMD52_WRITE,
                        &cmd52Data)) {
            DBG_LOG_PRINT(DBG_ZONE_ERR,("Failed to write I/O block size HIGH \r\n"));   
            break;
        }          
        
        
        DBG_LOG_PRINT(DBG_ZONE_HIF,("SDIO Init Complete \r\n"));        
                           
        success = A_OK;
        
    } while (FALSE);
    
    
    DBG_LOG_PRINT(DBG_ZONE_HIF,("-HIFInit \r\n")); 
    
    return success;
}

#define MAX_BYTES_PER_TRANS_BYTE_MODE 512
            
A_STATUS HIFReadWrite(HIF_DEVICE    *device,
                      A_UINT32       address, 
                      A_UCHAR       *buffer, 
                      A_UINT32       length, 
                      A_UINT32       request,
                      void          *Context)
{
    A_UINT32      arg;
    A_UINT32      blockLen = 0;
    A_UINT32      blockCount = 0;
    A_UINT32      count = 0;
    A_UINT32      mode = 0;
    A_UINT32      opcode = 0;
    A_UINT32      rw = 0;

    DBG_LOG_PRINT(DBG_ZONE_HIF_TRC,   ("+HIF\n")); 
    if (request & HIF_BLOCK_BASIS) {

#ifdef HIF_REDUCED_BLOCK_WRITES_4BIT   
        if (request & HIF_WRITE) {   
            if (length <= MAX_BYTES_PER_TRANS_BYTE_MODE) {
                    /* this can be done in byte mode */
                    /* force to byte mode */
                mode = CMD53_BYTE_BASIS;
                blockLen = length;
                blockCount = 1;
                count = (length == MAX_BYTES_PER_TRANS_BYTE_MODE) ? 0 : length;        
            } else {
                /* because of mult-block write issues, we just fail these */
                A_ASSERT(FALSE);
                return A_ERROR;    
            }
        } else 
#endif   
        {   
            mode = CMD53_BLOCK_BASIS;
            blockLen = HIF_MBOX_BLOCK_SIZE;
            blockCount = length / HIF_MBOX_BLOCK_SIZE;
            count = blockCount; 
        }
                          
    } else if (request & HIF_BYTE_BASIS) {
        mode = CMD53_BYTE_BASIS;
        blockLen = length;
        blockCount = 1;
        count = blockLen;
    }
        /* only synchronous supported!! */
    A_ASSERT((request & HIF_SYNCHRONOUS));
    
    if (request & HIF_FIXED_ADDRESS) {
        opcode = CMD53_FIXED_ADDRESS;
    } else if (request & HIF_INCREMENTAL_ADDRESS){   
        opcode = CMD53_INCR_ADDRESS;
    }

    if (request & HIF_WRITE) {
        rw = 1;
        if ((address >= HIF_MBOX_START_ADDR(0)) &&
            (address <= HIF_MBOX_END_ADDR(3))) {
            /* Mailbox write. Adjust the address so that the last byte
               falls on the EOM address */
            address = address + HIF_MBOX_WIDTH - length;
        }
    } else {
        rw = 0;
    }
    
    DBG_LOG_PRINT(DBG_ZONE_HIF_TRC,
                    ("HIF (%s) Address:0x%X, (BlockLen: %d, BlockCount: %d) \r\n",
                    (request & HIF_WRITE) ? "WRITE" : "READ",
                    address,
                    blockLen, 
                    blockCount));
                    
    SDIO_SET_CMD53_ARG(arg,
                       rw,
                       1, /* always function 1 */
                       mode,
                       opcode,                       
                       address,
                       count);
    if (!SDIO_IssueCMD53(rw ? CMD53_WRITE : CMD53_READ,
                         arg,
                         blockCount,
                         blockLen,
                         buffer)) {
         DBG_LOG_PRINT(DBG_ZONE_ERR,
                    ("*FAILED* HIF (%s) Address:0x%X, (BlockLen: %d, BlockCount: %d) \r\n",
                    (request & HIF_WRITE) ? "WRITE" : "READ",
                    address,
                    blockLen, 
                    blockCount));
             DBG_LOG_PRINT(DBG_ZONE_HIF_TRC,   ("error-HIF\n")); 
        return A_ERROR;                    
    }                  
        DBG_LOG_PRINT(DBG_ZONE_HIF_TRC,   ("-HIF\n")); 
    return A_OK;    
}


void HIFShutDown()
{
    
    
}

A_STATUS HIFConfigureDevice(HIF_DEVICE    *device,
                            HIF_DEVICE_CONFIG_OPCODE opcode, 
                            void           *config, 
                            A_UINT32       configLen)
{
    A_UINT32 count;

    switch(opcode) {
        case HIF_DEVICE_GET_MBOX_BLOCK_SIZE:
            ((A_UINT32 *)config)[0] = HIF_MBOX0_BLOCK_SIZE;
            ((A_UINT32 *)config)[1] = HIF_MBOX1_BLOCK_SIZE;
            ((A_UINT32 *)config)[2] = HIF_MBOX2_BLOCK_SIZE;
            ((A_UINT32 *)config)[3] = HIF_MBOX3_BLOCK_SIZE;
            break;

        case HIF_DEVICE_GET_MBOX_ADDR:
            for (count = 0; count < 4; count ++) {
                ((A_UINT32 *)config)[count] = HIF_MBOX_START_ADDR(count);
            }
            break;
            
        default:
            return A_ERROR;
    }
    return A_OK; 
}
