//------------------------------------------------------------------------------
// <copyright file="hif.c" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
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
// This file contains the routines handling the interaction with the SDIO driver
//
// Author(s): ="Atheros"
//==============================================================================
#include "ar6000_sdio.h"
#include "a_debug.h"
#include "hif.h"
#include "hif_internal.h"
#include "htc_packet.h"
#include "bmi_msg.h"
#include "htc_api.h"
#include "common_drv.h"
#include "targaddrs.h"
#include "AR6002\hw\mbox_host_reg.h"


/**** Description of Compile time flags:
 *
 * HIF_SYNC - set this flag to operate all async bus requests in synchronous fashion.  This is useful
 * when dealing with the high capacity bus driver during early bringup where SD bus errors should not
 * be silently ignored (see SD_HIGH_CAPACITY_BUSDRIVER_SUPPORT flag below).
 *
 * SD_HIGH_CAPACITY_BUSDRIVER_SUPPORT - set this flag if using the high capacity bus driver.  This
 * flag disables request status checking in the async I/O completion handler because by design
 * the pRequest is no longer a pointer to the struct (now an opaque handle). This has the consequnce of
 * silently ignoring bus errors.  During early bring up using HIF_SYNC can detect these errors.
 * This flag also creates a deferred ASYNC I/O completion thread to work around a known deadlock
 * in the high capacity driver (see description below).  This setting automatically enables the
 * HIF_DEFER_HIF_ASYNC_COMPLETION flag.
 *
 * HIF_SDBUS_WM7 - use for the WM7 SDBUS variant. (HIF_SYNC should not be needed in this case).
 *
 * HIF_SINGLE_BLOCK_WAR - some host controllers have issues with multi-block CMD53 transfers.  This macro
 * enables the conversion of multi-block requests to single block (atomic) operations. The single block
 * commands use CMD53 in byte mode where each bus request transfers a maximum of 512 bytes. The macro
 * enables conversion for both READ and WRITE, however alternative macros are provided to selectively
 * enable read or write if the issues only affects one direction.
 *
 * HIF_SDIO_LARGE_BLOCK_MODE - this setting is used in conjunction with HIF_SINGLE_BLOCK_WAR. To reclaim
 * lost performance due to the multi-block to single block conversion, a workaround is applied to set
 * the SDIO block size to a maximum expected HTC frame size (1664 for Olca).  READ or WRITE operations
 * of maximum size use a single block.  This requires the host controller to support the large block
 * size.  Smaller HTC frames are issuesd using the single block WAR (1-3 CMD53-byte mode operations).
 *
 * HIF_SDIO_BYPASS - this setting requires the host controller driver to implement the Atheros
 * proprietary bypass mode.  The bypass mode provides direct access from the HIF layer to the host
 * controller driver. The client driver is enumerated normally by the SDIO bus driver.  Once control
 * transfers to HIF, HIF will issue a command to switch the host controller into bypass mode.
 * In this mode, the host controller operates under the control of the client driver.
 * A complete description is provided in ar6k_sdio_bypass.h.
 *
 * HIF_USE_SOFTBLOCK - this is an experimental mode to use the soft-block transfer feature in the
 * high-capacity bus driver. This would have removed the need for locking mailbox accesses and allowed
 * the bus driver to perform the multi-block to single-block conversion.  However
 * it was discovered that the soft-block feature in the bus driver has a bug preventing proper
 * operation.  This feature should not be used until the soft-block feature has been fixed in the
 * bus driver.
 *
 * HIF_HEARTBEAT - this setting is for debugging only.  A heartbeat thread can be created to issue
 * a software break after a fixed period of time.  This is useful to debug system hang issues which
 * can prevent the KITL debugger from normally breaking from Platform builder.
 *
 * HIF_SDIO_DISABLE_BMI_SLEEP - (AR6002-only) A work around for slow loading device drivers and 
 * slow enumerating hosts. The AR6002 device can go to sleep immediately due to an uncalibrated low
 * frequency clock.  This can cause a problem if the LF clock is off by a 
 * considerable amount.  It may sleep early and cause synchronization issues
 * between SDIO and sleep.  Typically this is not a problem if the host can (in a timely manner)
 * load the AR6K driver and allow the driver to complete initialization to
 * disable sleep and switch to an external crystal or properly calibrate the internal
 * one. On host where the driver may be loaded over kitl (and debugger loads symbols) the
 * load process can take 10 or more seconds causing the window
 * to be missed.  On those systems this forced sleep disable can be used.
 * 
 *
 */

/*  HOST PLATFORM SLOT POWER CONTROL
 *
 *  Under Windows CE/Mobile, the network interface driver cannot be unloaded when the NIC is requested
 *  to be powered off.  This creates a problem because of the dependency on the SDIO bus driver to
 *  re-enumerate a powered-off card. This action can only occur on a removal/reinsert event which forces
 *  the client driver unloading process.  A work around to this issue is provided at the HIF layer
 *  but requires cooperation from the underlying driver that controls slot power
 * (typically the host controller driver).
 *
 *  The HIF layer can issue a customer device I/O control to a streams driver that implements the WiFi
 *  module ON/OFF logic.  The power state change starts at the NIC layer and flows down to the HIF
 *  layer. The HIF layer requests slot power removal and handles all SDIO interface re-initialization
 *  requirements when power is restored.
 *
 *  The host platform must define the following keys
 *
 *  [HKEY_LOCAL_MACHINE\Comm\AR6K_SD1\Parms\HIF\SDIO]
 *       "SlotPowerDevName"="<stream device name>"
 *       "WifiModuleOffIoctl"=dword:<private ioctl code>
 *       "WifiModuleOnIoctl"=dword:<private ioctl code>
 *
 *  The driver that handles these IOCTLs controls power cycling and any necessary module reset
 *  sequencing.
 *
 *
 */

#ifdef BLUEBIRD_WINMOBILE
A_BOOL g_HifDone = FALSE;
#endif

//buildercheck
A_UINT32 sdio1bitmode = 0;	//builderorg = 0
A_UINT32 sdiobusspeedlow = 0;  //builderorg = 0

#define SLOTPOWER_DEV_NAME_REG_VAL_KEY  TEXT("SlotPowerDevName")
#define SLOTPOWER_OFF_IOCTL_REG_VAL_KEY TEXT("WifiModuleOffIoctl")
#define SLOTPOWER_ON_IOCTL_REG_VAL_KEY  TEXT("WifiModuleOnIoctl")
static void SetupSlotPowerControl(HIF_DEVICE *device);
static void CleanupSlotPowerControl(HIF_DEVICE *device);
static BOOL PowerUpDownSlot(HIF_DEVICE *device, A_BOOL PowerUp);

#ifdef HIF_SDIO_BYPASS

A_BOOL g_BypassModeActive = FALSE;

#define ISSUE_ASYNC_BUS_REQUEST(_status,h,cmd,arg,tC,RespT,bC,bL,buffer,cb,ctxt,pBusReq,flags) \
    if (g_BypassModeActive) {                                                                 \
        (_status) = ASyncRequestBypass(cmd,arg,tC,RespT,bC,bL,buffer,cb,ctxt);                 \
    } else {                                                                                  \
        (_status) = SDBusRequest(h,cmd,arg,tC,RespT,bC,bL,buffer,cb,ctxt,pBusReq,flags);       \
    }

#define ISSUE_SYNC_BUS_REQUEST(_status,h,cmd,arg,tC,RespT,pResp,bC,bL,buffer,flags)       \
    if (g_BypassModeActive) {                                                            \
        (_status) = SynchRequestBypass(cmd,arg,tC,RespT,pResp,bC,bL,buffer);             \
    } else {                                                                             \
        (_status) = SDSynchronousBusRequest(h,cmd,arg,tC,RespT,pResp,bC,bL,buffer,flags); \
    }

#else

#define ISSUE_ASYNC_BUS_REQUEST(_status,h,cmd,arg,tC,RespT,bC,bL,buffer,cb,ctxt,pBusReq,flags) \
    (_status) = SDBusRequest(h,cmd,arg,tC,RespT,bC,bL,buffer,cb,ctxt,pBusReq,flags)

#define ISSUE_SYNC_BUS_REQUEST(_status,h,cmd,arg,tC,RespT,pResp,bC,bL,buffer,flags)  \
    (_status) = SDSynchronousBusRequest(h,cmd,arg,tC,RespT,pResp,bC,bL,buffer,flags)

#endif


#ifdef HIF_PROFILE_IO_PROCESSING

typedef struct _HIF_STATS {
    LONG   HIFSyncRef;
    LONG   HIFAsyncRef;
    LONG   InAsyncCount;
}HIF_STATS;

HIF_STATS g_HIFStats = {0,0,0};

#define HIF_INC_SYNC_REF() InterlockedIncrement(&g_HIFStats.HIFSyncRef)
#define HIF_DEC_SYNC_REF() InterlockedDecrement(&g_HIFStats.HIFSyncRef)
#define HIF_INC_ASYNC_REF() InterlockedIncrement(&g_HIFStats.HIFAsyncRef)
#define HIF_DEC_ASYNC_REF() InterlockedDecrement(&g_HIFStats.HIFAsyncRef)
#define HIF_INC_ASYNC_CB_REF() InterlockedIncrement(&g_HIFStats.InAsyncCount)
#define HIF_DEC_ASYNC_CB_REF() InterlockedDecrement(&g_HIFStats.InAsyncCount)

#else

#define HIF_INC_SYNC_REF()
#define HIF_DEC_SYNC_REF()
#define HIF_INC_ASYNC_REF()
#define HIF_DEC_ASYNC_REF()
#define HIF_INC_ASYNC_CB_REF()
#define HIF_DEC_ASYNC_CB_REF()

#endif

#ifdef SD_HIGH_CAPACITY_BUSDRIVER_SUPPORT

//#define HIF_USE_SOFTBLOCK

#ifndef HIF_SYNC
    /* the high capacity busdriver holds the slot's bus request lock when it calls our ASYNC completion routine.
     * This can result in a deadlock because of our LOCK_IO_SEQUENCE() for the reduce block mode
     * work-around.  The I/O lock makes reads/writes to the mailbox registers atomic.  However this
     * can potentially deadlock the async completion routine.  This compilation flag defers the ASYNC completion routine
     * callback to a thread
     * ALSO... there is also another potential deadlock if the async routine submits a new
     * request while some other thread is blocked on a synchronous bus request call (i.e. CMD52 from
     * bus-driver's interrupt processing thread).  The bus driver holds onto the slot request
     * lock while it waits for the synchronous wait object to signal.  If this pending request is
     * behind the newly completed async request, this is a deadlock situation.
     */
#define HIF_DEFER_HIF_ASYNC_COMPLETION

static void ShutdownIOCompletionThread();
static void SetupIOCompletionThread();
static void QueueIOPacket(HTC_PACKET *pPacket);

#endif
#endif

int g_SoftBlockAvail = 0;

#ifdef HIF_HEARTBEAT
void ShutdownHeartBeatThread();
void SetupHeartBeatThread();
#endif

static void DoCMD53Test(HIF_DEVICE *device);
static void DisableBMISleep(HIF_DEVICE *device);
static A_STATUS PowerChangeNotify(HIF_DEVICE *device, HIF_DEVICE_POWER_CHANGE_TYPE PowerChange);
static A_STATUS ReinitSDIO(HIF_DEVICE *device);

#ifdef HIF_SYNC

    /*  force all ASYNC I/O to be synchronous */
#define IS_ASYNC_IO_FORCED_SYNC(request)  TRUE

#else

    /* force all read ops to SYNC mode
       write operations will still be async mode... */
#define IS_ASYNC_IO_FORCED_SYNC(request)  (!((request) & HIF_WRITE))

#endif

static char *GetBusErrorString(SD_API_STATUS Status)
{

    switch (Status) {

        case SD_API_STATUS_DATA_ERROR:
            return "DATA CRC ERROR";

        case SD_API_STATUS_CRC_ERROR:
            return "RESPONSE CRC ERROR";

        case SD_API_STATUS_RESPONSE_TIMEOUT:
            return "RESPONSE TIMEOUT";

        case SD_API_STATUS_DATA_TIMEOUT:
            return "DATA TIMEOUT";

        default:
            break;
    }

    return " ";
}

static TCHAR *GetBusErrorString_T(SD_API_STATUS Status)
{

    switch (Status) {

        case SD_API_STATUS_DATA_ERROR:
            return TEXT("DATA CRC ERROR");

        case SD_API_STATUS_CRC_ERROR:
            return TEXT("RESPONSE CRC ERROR");

        case SD_API_STATUS_RESPONSE_TIMEOUT:
            return TEXT("RESPONSE TIMEOUT");

        case SD_API_STATUS_DATA_TIMEOUT:
            return TEXT("DATA TIMEOUT");

        default:
            break;
    }

    return TEXT(" ");
}

#ifdef SD_HIGH_CAPACITY_BUSDRIVER_SUPPORT

#ifdef HIF_SDIO_BYPASS

#define GET_SD_BUS_REQUEST_COMPLETION_STATUS(_sdstat, pRequest) \
    if (g_BypassModeActive) {                                   \
        (_sdstat) = (SD_API_STATUS)pRequest;                    \
    } else if (g_pBusRequestStatusAPI != NULL) {                \
        (_sdstat) = g_pBusRequestStatusAPI(pRequest);           \
    } else {                                                    \
        (_sdstat) = SD_API_STATUS_SUCCESS;                      \
    }

#else

#define GET_SD_BUS_REQUEST_COMPLETION_STATUS(_sdstat, pRequest) \
    if (g_pBusRequestStatusAPI != NULL) {                       \
        (_sdstat) = g_pBusRequestStatusAPI(pRequest);           \
    } else {                                                    \
        (_sdstat) = SD_API_STATUS_SUCCESS;                      \
    }

#endif  // HIF_SDIO_BYPASS

typedef SD_API_STATUS (*BUSREQUEST_STATUS_API)(void *);

BUSREQUEST_STATUS_API g_pBusRequestStatusAPI = NULL;

static GetBusRequestStatusFunc()
{
    HINSTANCE   hDLL = NULL;

    do {
           /* try to load the bus driver DLL */
        hDLL = LoadDriver(TEXT("sdbus.dll"));

        if (NULL == hDLL) {
            break;
        }
            /* get the request status access function, requires
             * special bus driver */
        g_pBusRequestStatusAPI = (BUSREQUEST_STATUS_API)
                            GetProcAddress(hDLL, TEXT("SDBusRequestStatus__X"));

        if (NULL == g_pBusRequestStatusAPI) {
            /* not available */
            break;
        }

        RETAILMSG(1,
            (TEXT(" SDBusRequestStatus__X API found, ptr:0x%X \r\n"),
              g_pBusRequestStatusAPI ));

    } while (FALSE);

    if (hDLL != NULL) {
        FreeLibrary(hDLL);
    }
}
#else // SD_HIGH_CAPACITY_BUSDRIVER_SUPPORT

#ifdef HIF_SDIO_BYPASS
#ifdef HIF_SDBUS_WM7
#define GET_SD_BUS_REQUEST_COMPLETION_STATUS(_sdstat, pRequest)\
    {\
        SD_COMMAND_RESPONSE _sdCmdResp;\
        _sdstat = SdBusRequestResponse(pRequest, &_sdCmdResp);\
    }
#else // HIF_SDBUS_WM7
#define GET_SD_BUS_REQUEST_COMPLETION_STATUS(_sdstat, pRequest) \
    if (g_BypassModeActive) {                                   \
        (_sdstat) = (SD_API_STATUS)pRequest;                    \
    } else {                                                    \
        (_sdstat) = (pRequest)->Status;                         \
    }
#endif // HIF_SDBUS_WM7
#else // HIF_SDIO_BYPASS

#ifdef HIF_SDBUS_WM7
#define GET_SD_BUS_REQUEST_COMPLETION_STATUS(_sdstat, pRequest)\
    {\
        SD_COMMAND_RESPONSE _sdCmdResp;\
        _sdstat = SdBusRequestResponse(pRequest, &_sdCmdResp);\
    }
#else // HIF_SDBUS_WM7

    /* get it directly from the request */
#define GET_SD_BUS_REQUEST_COMPLETION_STATUS(_sdstat, pRequest) (_sdstat) =(pRequest)->Status;

#endif // HIF_SDBUS_WM7
#endif // HIF_SDIO_BYPASS

#endif // SD_HIGH_CAPACITY_BUSDRIVER_SUPPORT

/* ------ Functions ------ */
A_STATUS
HIFInit(OSDRV_CALLBACKS *callbacks)
{
    SD_API_STATUS sdStatus;

    /* Store the callback and event handlers */
    osdrvCallbacks = *callbacks;   

    /* Register with bus driver core */
    sdFunction.pName      = "sdio_wlan";
    sdFunction.pProbe     = hifDeviceInserted;
    sdFunction.pRemove    = hifDeviceRemoved;

    sdStatus = SDIORegisterFunction(&sdFunction);
    //-builder AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));
    if( !SD_API_SUCCESS(sdStatus) )
	{
	RETAIL_DEBUG_PRINTF(1, (TEXT("builder:ar6k2:SDIO bus not prepared!!!\r\n")));
	return A_ERROR;
	}

#ifdef SD_HIGH_CAPACITY_BUSDRIVER_SUPPORT
    GetBusRequestStatusFunc();
#endif

#ifdef HIF_DEFER_HIF_ASYNC_COMPLETION
        /* setup I/O completion thread */
    SetupIOCompletionThread();
#endif

#ifdef HIF_HEARTBEAT
    SetupHeartBeatThread();
#endif

    return A_OK;
}

#ifdef HIF_SINGLE_BLOCK_WAR

    /* single block WAR work around for broken host controllers */
#ifdef HIF_SDIO_LARGE_BLOCK_MODE

#ifdef HTC_EPPING_TEST
    /* ep ping test cannot handle this block size */
#define HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE  1536
#else
    /* when running normal WLAN, the largest data frame is
     * 1664 bytes */
#define HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE  1664
#endif //HTC_EPPING_TEST

    /* reads and writes to the MBOX address range requires synchronization
     * because small transaction will use the single block WAR */
#define REQUEST_REQUIRES_LOCK(request,isMbox) (!g_SoftBlockAvail && isMbox)
    /* reads and writes use reduced I/O when transfering less than the
     * block mode size */
#define USE_REDUCED_IO(transferClass,blockcount) (!g_SoftBlockAvail && (blockcount) > 1)

#else

    /* operations to the MBOX address range requires synchronization */
#define REQUEST_REQUIRES_LOCK(request,isMbox) (!g_SoftBlockAvail && (isMbox))
    /* Alternative macro forms :
     *
     * only WRITE requests use single block WAR :
     * #define REQUEST_REQUIRES_LOCK(request,isMbox) (!g_SoftBlockAvail && (request & HIF_WRITE) && isMbox)
     *
     * only READ requests use single block WAR :
     * #define REQUEST_REQUIRES_LOCK(request,isMbox) (!g_SoftBlockAvail && !(request & HIF_WRITE) && isMbox)
     *
     */

    /*  operations use the reduced I/O (single blocks) */
#define USE_REDUCED_IO(transferClass,blockcount) (!g_SoftBlockAvail && ((blockcount) > 1))
    /* Alternative macro forms :
     *
     * only WRITE requests use single block WAR :
     * #define USE_REDUCED_IO(transferClass,blockcount) (!g_SoftBlockAvail && (transferClass == SD_WRITE) && ((blockcount) > 1))
     *
     * only READ requests use single block WAR :
     * #define USE_REDUCED_IO(transferClass,blockcount) (!g_SoftBlockAvail && (transferClass == SD_READ) && ((blockcount) > 1))
     *
     */

#endif //HIF_SDIO_LARGE_BLOCK_MODE

/* when using the single block work-around the write and/or read sequences has to be
    setup atomically , these lock macros are used when the work around is applied. */

#define LOCK_IO_SEQUENCE(request,device,isMbox)                         \
    if (REQUEST_REQUIRES_LOCK(request,isMbox)) {     \
        if ((request) & HIF_WRITE)                                      \
            EnterCriticalSection(&(device)->WriteLock);                 \
        else                                                            \
            EnterCriticalSection(&(device)->ReadLock);                  \
    }

#define UNLOCK_IO_SEQUENCE(request,device,isMbox) \
    if (REQUEST_REQUIRES_LOCK(request,isMbox))  {    \
        if ((request) & HIF_WRITE)                                      \
            LeaveCriticalSection(&(device)->WriteLock);                 \
        else                                                            \
            LeaveCriticalSection(&(device)->ReadLock);                  \
    }


#define CHECK_REDUCED_IO_REQUEST(tranferClass, blockcount) \
    (USE_REDUCED_IO(transferClass,blockcount))

#else

#define LOCK_IO_SEQUENCE(request,device,ismbox)

#define UNLOCK_IO_SEQUENCE(request,device,ismbox)

#define CHECK_REDUCED_IO_REQUEST(tranferClass, blockcount) (FALSE)

#endif //HIF_SINGLE_BLOCK_WAR


#define REDUCE_BLOCK_MODE_FUNC TransferSingleBlockMode

static void AsyncCMD53TransferHandler(SD_DEVICE_HANDLE  hDevice,
                                      PSD_BUS_REQUEST   pRequest,
                                      PVOID             notUsed,
                                      DWORD             dwParam)
{
    A_STATUS status = A_OK;
    SD_API_STATUS sdStatus;

    GET_SD_BUS_REQUEST_COMPLETION_STATUS(sdStatus, pRequest);

    if (SD_API_SUCCESS(sdStatus)) {
        status = A_OK;
    } else {
        status = A_ERROR;
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("  Async Bus Request Failed: 0x%x (%s) \n",
                sdStatus, GetBusErrorString(sdStatus)));
#ifndef DEBUG
        RETAILMSG(1, (TEXT("   Async Bus Request Failed: 0x%x (%s) \n"),
                 sdStatus, GetBusErrorString_T(sdStatus)));
	logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k: Async Bus Request Failed: 0x%x (%s) \n",
		sdStatus, GetBusErrorString_T(sdStatus));
	logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k: may be SDIO interface down or module hang up\n" );
#endif

    }

#ifdef HIF_SDIO_BYPASS
    if (g_BypassModeActive) {
        /* in bypass mode, no need to free the request */
    } else
#endif
    {
        SDFreeBusRequest(pRequest);
    }
    return;
}


#define MAX_BYTES_PER_TRANS_BYTE_MODE 512

static SD_API_STATUS TransferSingleBlockMode(SD_DEVICE_HANDLE hSDIODevice,
                                             A_UINT32         Request,
                                             A_UINT32         Address,
                                             A_UCHAR          *pBuffer,
                                             A_UINT32         Length,
                                             VOID             *Context)
{
    SD_COMMAND_RESPONSE response;
    DWORD               dwArg;
    A_UINT32            curLength;
    int                 currentTransfer, totalTransfers;
    SD_API_STATUS       sdStatus = SD_API_STATUS_SUCCESS;
    BOOL                doTransferAsync = TRUE;
    BOOL                lastTransfer = FALSE;

    totalTransfers = Length/MAX_BYTES_PER_TRANS_BYTE_MODE;

    if (Length % MAX_BYTES_PER_TRANS_BYTE_MODE) {
        totalTransfers++;
    }

    currentTransfer = 0;

    while (Length) {

        curLength = min(Length, MAX_BYTES_PER_TRANS_BYTE_MODE);

        dwArg = BUILD_IO_RW_EXTENDED_ARG (((Request & HIF_WRITE) ?
                                            SD_IO_OP_WRITE : SD_IO_OP_READ),
                                          SD_IO_BYTE_MODE,
                                          1, /* funcNo */
                                          Address,
                                          ((Request & HIF_INCREMENTAL_ADDRESS) ?
                                                SD_IO_INCREMENT_ADDRESS : SD_IO_FIXED_ADDRESS),
                                          ((curLength >= 512) ?  0 : curLength));

        if (currentTransfer < (totalTransfers - 1)) {
                /* not the last */
            lastTransfer = FALSE;
                /* always queue up transfers except the last transfer ,
                   this will improve performance by overlapping each of the previous requests */
            doTransferAsync = TRUE;
        } else {
                /* the last transfer */
            lastTransfer = TRUE;

            if (Request & HIF_SYNCHRONOUS) {
                   /* last one is synchronously sent, since the caller wanted SYNCH operation */
                doTransferAsync = FALSE;
            } else {
                /* async transfer */
#ifdef HIF_SYNC
                    /* force last transfer to be a SYNC operation to fake synchronous behavior */
                doTransferAsync = FALSE;
#else
                    /* last transfer can be sent asynchronously */
                doTransferAsync = TRUE;
#endif
            }
        }

        if (doTransferAsync) {
            PSD_BUS_REQUEST     busRequest;

                /* pipeline some of the transfers asynchronously */
            ISSUE_ASYNC_BUS_REQUEST(sdStatus,
                                    hSDIODevice,
                                    SD_IO_RW_EXTENDED,
                                    dwArg,
                                    (Request & HIF_WRITE) ? SD_WRITE : SD_READ,
                                    ResponseR5,
                                    1, /* 1 block */
                                    curLength,
                                    pBuffer,
                                    lastTransfer ? hifRWCompletionHandler : AsyncCMD53TransferHandler,
                                    (DWORD)Context,
                                    &busRequest,
                                    0);


        } else {
                /* do this synchronously */
            ISSUE_SYNC_BUS_REQUEST(sdStatus,
                                   hSDIODevice,
                                   SD_IO_RW_EXTENDED,
                                   dwArg,
                                   (Request & HIF_WRITE) ? SD_WRITE : SD_READ,
                                   ResponseR5,
                                   &response,
                                   1, /* 1 block */
                                   curLength,
                                   pBuffer,
                                   0);

        }

        if (!SD_API_SUCCESS(sdStatus)) {
            break;
        }

        currentTransfer++;
        pBuffer += curLength;
        Length -= curLength;
        if (Request & HIF_INCREMENTAL_ADDRESS) {
            Address += curLength;
        }

    }

    return sdStatus;
}


A_STATUS
HIFReadWrite(HIF_DEVICE  *device,
             A_UINT32     address,
             A_UCHAR     *buffer,
             A_UINT32     length,
             A_UINT32     request,
             void        *context)
{
    A_UINT8             rw          = 0;
    A_UINT8             mode        = SD_IO_BYTE_MODE;
    A_UINT8             opcode      = SD_IO_FIXED_ADDRESS;
    A_UINT32            blockLen    = 0;
    A_UINT32            blockCount  = 0;
    A_UINT32            count       = 0;
    A_STATUS            status      = A_OK;
    DWORD               dwArg       = 0;
    A_UCHAR             command     = SD_IO_RW_NORMAL;

    SD_API_STATUS       sdStatus = SD_API_STATUS_SUCCESS;
    SD_COMMAND_RESPONSE response;
    SD_TRANSFER_CLASS   transferClass = SD_READ;
    A_BOOL              isMbox = FALSE;

#ifdef HIF_SDIO_LARGE_BLOCK_MODE
    A_UINT32            regAddress = address;
#endif
#ifdef BLUEBIRD_WINMOBILE
	g_HifDone = FALSE;
#endif

    AR_DEBUG_PRINTF(ATH_LOG_TRC, ("HIFReadWrite:Enter\n"));
    AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Address 0x%x\n", address));

    if (request & HIF_SYNCHRONOUS) {
        HIF_INC_SYNC_REF();
    } else {
        HIF_INC_ASYNC_REF();
    }

	if ((request & HIF_BLOCK_BASIS)	&&
       !(request & HIF_EXTENDED_IO))
    {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Block mode not allowed for this type of command\n"));
        return A_ERROR;
    }

    if (request & HIF_BLOCK_BASIS)
    {
        mode = SD_IO_BLOCK_MODE;
        blockLen = HIF_MBOX_BLOCK_SIZE;
        blockCount = length / HIF_MBOX_BLOCK_SIZE;
        count = blockCount;
        AR_DEBUG_PRINTF(ATH_LOG_TRC,
                        ("Block mode (BlockLen: %d, BlockCount: %d)\n",
                          blockLen, blockCount));
    }
    else if (request & HIF_BYTE_BASIS)
    {
        mode = SD_IO_BYTE_MODE;
        blockLen = length;
        blockCount = 1;
        count = blockLen;
        AR_DEBUG_PRINTF(ATH_LOG_TRC,
                        ("Byte mode (BlockLen: %d, BlockCount: %d)\n",
                          blockLen, blockCount));

    }

    if (request & HIF_FIXED_ADDRESS)
    {
        opcode = SD_IO_FIXED_ADDRESS;
        AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Fixed       "));
    }
    else if (request & HIF_INCREMENTAL_ADDRESS)
    {
        opcode = SD_IO_INCREMENT_ADDRESS;
        AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Incremental "));
    }

    if (request & HIF_WRITE)
    {
        transferClass = SD_WRITE;
        rw = SD_IO_OP_WRITE;
        if ((address >= HIF_MBOX_START_ADDR(0)) &&
            (address <= HIF_MBOX_END_ADDR(3)))
        {
            /* Mailbox write. Adjust the address so that the last byte
               falls on the EOM address */
            address = address + HIF_MBOX_WIDTH - length;
            isMbox = TRUE;
        }
        AR_DEBUG_PRINTF(ATH_LOG_TRC, ("[Write]"));
    }
    else
    {
        transferClass = SD_READ;
        rw = SD_IO_OP_READ;
        AR_DEBUG_PRINTF(ATH_LOG_TRC, ("[Read ]"));
        if ((address >= HIF_MBOX_START_ADDR(0)) &&
            (address <= HIF_MBOX_END_ADDR(3))) {
            isMbox = TRUE;
        }
    }

#ifdef HIF_SDIO_LARGE_BLOCK_MODE
        /* filter all block mode requests */
    if ((request & HIF_BLOCK_BASIS)) {
        if (length <= MAX_BYTES_PER_TRANS_BYTE_MODE) {
                /* this can be done in byte mode */
                /* force to byte mode */
            mode = SD_IO_BYTE_MODE;
            blockLen = length;
            blockCount = 1;
            count = (length == MAX_BYTES_PER_TRANS_BYTE_MODE) ? 0 : length;

        } else if (length >= WLAN_MAX_SEND_FRAME) {

                /* Special case to check for large-single block operations:
                 *
                 * -- for WRITE operations, if the length is greater than WLAN_MAX_SEND_FRAME,
                 *    we add additional padding to round it up to the
                 *    programmed block size (HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE) so we can
                 *    transfer in a single block
                 *
                 * -- for READ operations the length must be exactly equal to the
                 *    large block size (HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE).
                 *
                 * The rationale for this:
                 *    To minimize changes in the firmware, the firmware credit size is
                 *    actually at least HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE bytes each,
                 *    however we keep I/O block size padding to a minimum value (HIF_MBOX_BLOCK_SIZE).
                 *    We convert any WLAN_MAX_SEND_FRAME SDIO writes to a single MAX block write.
                 *    The receive direction uses (max) HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE byte
                 *    packets because we optimized the target receive buffers to provide 1 maximum 802.3
                 *    frame + lookahead and credit reports. This pushes the largest recv packet to
                 *    HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE bytes.
                 *    Thus, in choosing the common block size to use, we are using the recv side.
                 *
                 * */

            if ((request & HIF_WRITE) ||
                (!(request & HIF_WRITE) && (length == HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE))) {

                A_ASSERT(length <= HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE);
                length = HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE;
                blockLen = HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE;
                blockCount = 1;
                count = 1;
                if (request & HIF_WRITE) {
                        /* since we increased the actual length we need to re-adjust the
                         * address so the last byte falls in the right place */
                    address = regAddress + HIF_MBOX_WIDTH - length;
                }
            } else {
                /* empty case... fall through, it'll get picked up by our reduced block mode transfer */
                A_ASSERT(blockCount > 1);
            }

        } else {
                /* if it falls through here, there BETTER be multiblock transfers
                 * these will get sent out using the  reduced single block transfer mode since the
                 * SDIO card is currently using a large value for the block size */
            A_ASSERT(blockCount > 1);
        }

#ifdef HIF_USE_SOFTBLOCK
        if ((blockCount > 1) && g_SoftBlockAvail) {
            /* catch multi-block operations that will use "soft block"
             * Soft block has one disadvantage that it will use the block size as
             * the size of each single block transfer.
             * Here we check to see if the total amount of data is a multiple of 512
             * or 256 bytes and let softblock transfer using the larger block size */
            if ((length % 512) == 0) {
                blockLen = 512;
                blockCount = length / 512;
                count = blockCount;
            } else if ((length % 256) == 0) {
                blockLen = 256;
                blockCount = length / 256;
                count = blockCount;
            }
        }
#endif

    }
#endif

    if (request & HIF_EXTENDED_IO)
    {
        dwArg = BUILD_IO_RW_EXTENDED_ARG (rw, mode, funcNo,
                                          address, opcode, count);
        command = SD_IO_RW_EXTENDED;

    }
    else if (request & HIF_BASIC_IO)
    {
        dwArg = BUILD_IO_RW_DIRECT_ARG(rw, SD_IO_RW_NORMAL,
                                       funcNo, address, 0);
        command = SD_IO_RW_NORMAL;

    }

    /*AR_DEBUG_PRINTF(ATH_LOG_ERR,
                    ("HIF (%s) Address:0x%X, (BlockLen: %d, BlockCount: %d)\n",
                    (request & HIF_WRITE) ? "WRITE" : "READ",
                    address,
                    blockLen,
                    blockCount));*/

    if (request & HIF_SYNCHRONOUS) {
        AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Synchronous\n"));

        LOCK_IO_SEQUENCE(request,device,isMbox);

        if (CHECK_REDUCED_IO_REQUEST(transferClass,blockCount)) {

            /* force write operations to use single block because of host controller
             * multiblock issues */
            sdStatus = REDUCE_BLOCK_MODE_FUNC(device->handle,
                                              request,
                                              address,
                                              buffer,
                                              length,
                                              context);

        } else  {

            ISSUE_SYNC_BUS_REQUEST(sdStatus,device->handle, command, dwArg,
                        transferClass, ResponseR5, &response, blockCount,
                        blockLen, buffer, 0);
        }

        UNLOCK_IO_SEQUENCE(request,device,isMbox);

        HIF_DEC_SYNC_REF();

    } else {

        /* ASYNC operation */

        if (!IS_ASYNC_IO_FORCED_SYNC(request)) {

                /* normal asynchronous operation ... */
            AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Asynchronous\n"));

            LOCK_IO_SEQUENCE(request,device,isMbox);

            if (CHECK_REDUCED_IO_REQUEST(transferClass,blockCount)) {

                /* force operations to use single block because of host controller
                 * multiblock issues */
                sdStatus = REDUCE_BLOCK_MODE_FUNC(device->handle,
                                                  request,
                                                  address,
                                                  buffer,
                                                  length,
                                                  context);

            } else  {
                PSD_BUS_REQUEST     busRequest;

                ISSUE_ASYNC_BUS_REQUEST(sdStatus,device->handle, command, dwArg, transferClass,
                            ResponseR5, blockCount, blockLen, buffer,
                            hifRWCompletionHandler, (DWORD) context, &busRequest, 0);

            }

            UNLOCK_IO_SEQUENCE(request,device,isMbox);

        } else {

            /* FAKE ASYNC operation */

            /*  ASYNC operation forced to use synchronous mode, for a number of reasons:
                    1). to get SD bus status errors when using SDBUS2 (during bringup)
                    2). get better performance on downlink   */

            AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Synchronous (fake Async)\n"));

            LOCK_IO_SEQUENCE(request,device,isMbox);

            if (CHECK_REDUCED_IO_REQUEST(transferClass,blockCount)) {

                sdStatus = REDUCE_BLOCK_MODE_FUNC(device->handle,
                                                  request | HIF_SYNCHRONOUS, /* force synchronous here, we need to
                                                     guarantee that the last transfer is synchronous */
                                                  address,
                                                  buffer,
                                                  length,
                                                  context);
            } else {

                    /* fake async using synchronous operation */
                ISSUE_SYNC_BUS_REQUEST(sdStatus, device->handle, command, dwArg,
                            transferClass, ResponseR5, &response, blockCount,
                            blockLen, buffer, 0);

            }

            if (!SD_API_SUCCESS(sdStatus))
            {
                AR_DEBUG_PRINTF(ATH_LOG_ERR, ("SDBusRequest (fake Async) failed 0x%x \r\n", sdStatus));
                status = A_ERROR;
            }

            UNLOCK_IO_SEQUENCE(request,device,isMbox);

            HIF_DEC_ASYNC_REF();
            HIF_INC_ASYNC_CB_REF();
            device->htcCallbacks.rwCompletionHandler(context, status);
            HIF_DEC_ASYNC_CB_REF();
        }

    }

    if (!SD_API_SUCCESS(sdStatus)) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("SDBusRequest (%s) failed 0x%x\n",
            (request & HIF_SYNCHRONOUS) ? "SYNC" : "ASYNC", sdStatus));
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("    ....(%s) address:0x%4.4X blocks:%d, blksize:%d 0x%x (%s) \n",
                (transferClass == SD_READ) ? "READ" : "WRITE",
                address,
                blockCount, blockLen,
                sdStatus, GetBusErrorString(sdStatus)));

#ifndef DEBUG
        RETAILMSG(1, (TEXT("SDBusRequest (%s) failed 0x%x\n"),
            (request & HIF_SYNCHRONOUS) ? TEXT("SYNC") : TEXT("ASYNC"), sdStatus));
        RETAILMSG(1, (TEXT("    ....(%s) address:0x%4.4X blocks:%d, blksize:%d 0x%x (%s) \n"),
                (transferClass == SD_READ) ? TEXT("READ") : TEXT("WRITE"),
                address,
                blockCount, blockLen,
                sdStatus, GetBusErrorString_T(sdStatus)));
	logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k: SDBusRequest (%s) failed 0x%x\n",
            (request & HIF_SYNCHRONOUS) ? TEXT("SYNC") : TEXT("ASYNC"), sdStatus);
        logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k:     ....(%s) address:0x%4.4X blocks:%d, blksize:%d 0x%x (%s) \n",
                (transferClass == SD_READ) ? TEXT("READ") : TEXT("WRITE"),
                address,
                blockCount, blockLen,
                sdStatus, GetBusErrorString_T(sdStatus));
	logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k: may be SDIO interface down or SDIO power off or module hang up\n" );

#endif
        status = A_ERROR;
    }

#ifdef BLUEBIRD_WINMOBILE
	g_HifDone = TRUE;
#endif

    return status;

}

void
HIFShutDownDevice(HIF_DEVICE *device)
{
    SD_API_STATUS       sdStatus;
    UCHAR               buffer;
    SD_DEVICE_HANDLE    *hDevice = NULL;

	RETAIL_DEBUG_PRINTF(1, (TEXT("builder:ar6k2:++>HIFShutDownDevice()\r\n")));
	logPrintf( (debugFileLog && debugErrorLog), "builder:ar6k2:++>HIFShutDownDevice()\r\n" );

#ifdef HIF_HEARTBEAT
    ShutdownHeartBeatThread();
#endif

#ifdef HIF_DEFER_HIF_ASYNC_COMPLETION
    ShutdownIOCompletionThread();
#endif

    do
    {
        if (device == NULL)
        {
            /* Unregister with bus driver core */
            Sleep(100);
            sdStatus = SDIOUnregisterFunction(&sdFunction);
            break;
        }

        device->Shutdown = TRUE;

        /* Disable the card */
        SDIODisconnectInterrupt(device->handle);

        sdStatus = SDSetCardFeature(device->handle, SD_IO_FUNCTION_DISABLE,
                                      NULL, 0);

        AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));

        /* Perform a soft I/O reset */
        buffer = 1 << 3;
        sdStatus = SDReadWriteRegistersDirect(device->handle, SD_IO_WRITE, 0,
                        SD_IO_REG_IO_ABORT, 1, &buffer, 1);

        AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));

#ifdef CEPC_ELLEN
            /* wake IRQ thread to exit */
        NdisSetEvent(&device->hifIRQEvent);
#endif

        //
        // assume that, the valid handle hasgot and removed it from array for
        // multiple/recursive call
        //
        if (SD_API_STATUS_SUCCESS == sdStatus)
        {
            delHifDevice (hDevice);
        }


        AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));


    }while (FALSE);


    return;
}


void
hifRWCompletionHandler(SD_DEVICE_HANDLE  hDevice,
                       PSD_BUS_REQUEST   pRequest,
                       PVOID             notUsed,
                       DWORD             dwParam)
{
    A_STATUS status = A_OK;
    SD_API_STATUS  sdStatus;
    PVOID    htcContext;
    HIF_DEVICE *device;
    
    device = getHifDevice(hDevice);

    GET_SD_BUS_REQUEST_COMPLETION_STATUS(sdStatus,pRequest);

    if (SD_API_SUCCESS(sdStatus)) {
        status = A_OK;
    } else {
        status = A_ERROR;
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("  Async Bus Request Failed: 0x%x (%s) \n",
                sdStatus, GetBusErrorString(sdStatus)));
#ifndef DEBUG
        RETAILMSG(1, (TEXT("   Async Bus Request Failed: 0x%x (%s) \n"),
                 sdStatus, GetBusErrorString_T(sdStatus)));
	logPrintf( (debugFileLog && debugErrorLog), "   Async Bus Request Failed: 0x%x (%s) \n",
                 sdStatus, GetBusErrorString_T(sdStatus));
#endif
    }

    htcContext = (void *)dwParam;

    HIF_DEC_ASYNC_REF();

#ifdef HIF_DEFER_HIF_ASYNC_COMPLETION

#ifdef HIF_SDIO_BYPASS
    if (g_BypassModeActive) {
        /* in bypass mode no need to queue the packet, just call directly */
        HIF_INC_ASYNC_CB_REF();
        device->htcCallbacks.rwCompletionHandler(htcContext, status);
        HIF_DEC_ASYNC_CB_REF();
    } else
#endif
    {
        /****** WARNING... this only works because the HTC layer uses the HTC_PACKET structure for
         * all ASYNC I/O, we can re-use this structure to manage the request ********/
        HTC_PACKET *pPacket = (HTC_PACKET *)htcContext;
            /* save status */
        pPacket->Status = status;
            /* defer these to the completion thread to avoid a deadlock */
        QueueIOPacket(pPacket);
    }
#else

    HIF_INC_ASYNC_CB_REF();
    device->htcCallbacks.rwCompletionHandler(htcContext, status);
    HIF_DEC_ASYNC_CB_REF();

#endif

    AR_DEBUG_ASSERT(status == A_OK);

#ifdef HIF_SDIO_BYPASS
    if (g_BypassModeActive) {
        /* in bypass mode the request is already freed */
    } else
#endif
    {
        SDFreeBusRequest(pRequest);
    }
    return;
}

#ifdef CEPC_ELLEN

DWORD
hifIRQThread(LPVOID Context)
{
    SD_DEVICE_HANDLE hDevice = Context;
    A_STATUS         status;
    HIF_DEVICE      *device;

    if (hDevice == NULL) {
        return SD_API_STATUS_UNSUCCESSFUL;
    }

    device = getHifDevice(hDevice);

    while (1) {
        NdisWaitEvent(&device->hifIRQEvent, 0);
        if (device->Shutdown) {
            break;
        }
        NdisResetEvent(&device->hifIRQEvent);
        status = device->htcCallbacks.dsrHandler(device->htcCallbacks.context);
        AR_DEBUG_ASSERT(status == A_OK);
    } // while
    return SD_API_STATUS_SUCCESS;
}

#endif

static SD_API_STATUS
hifIRQHandler(SD_DEVICE_HANDLE hDevice, PVOID notUsed)
{
    A_STATUS         status = A_OK;

    AR_DEBUG_PRINTF(ATH_LOG_TRC, ("hifIRQHandler : Enter\n"));

#ifndef CEPC_ELLEN

    status = getHifDevice(hDevice)->htcCallbacks.dsrHandler((getHifDevice(hDevice))->htcCallbacks.context);
    AR_DEBUG_ASSERT(status == A_OK);
#else
    NdisSetEvent(&(getHifDevice(hDevice))->hifIRQEvent);
#endif

    if (A_FAILED(status)) {
        A_UINT32 regValue = 0;
        /* a failure in the dsr handler could leave a pending interrupt
         * running rampant, the SDIO bus driver on CE does not seem to care
         * that we return failure from this handler.  So we disable all interrupts
         * by clearing the interrupt enables */
        HIFReadWrite(getHifDevice(hDevice),
                     0x418,
                     (A_UINT8*)&regValue,
                     4,
                     HIF_WR_SYNC_BYTE_INC,
                     NULL);
    }

    AR_DEBUG_PRINTF(ATH_LOG_TRC, ("hifIRQHandler : Exit\n"));

    return (A_FAILED(status) ? SD_API_STATUS_UNSUCCESSFUL : SD_API_STATUS_SUCCESS);
}


static BOOL SetupSDIOInterface(HIF_DEVICE *device)
{
    SD_API_STATUS               sdStatus;
    SDIO_CARD_INFO              sdioInfo;
    SD_HOST_BLOCK_CAPABILITY    blockCap;
    SD_CARD_RCA                 cardRCA;
    A_UCHAR                     rgucTuple[SD_CISTPLE_MAX_BODY_SIZE];
    PSD_CISTPL_FUNCE_FUNCTION   pFunce = (PSD_CISTPL_FUNCE_FUNCTION) rgucTuple;
    A_UINT32                    ulLength = 0;
    A_UCHAR                     ucRegVal;
    A_BOOL                      blockMode;
    SD_CARD_INTERFACE           ci;
    SD_IO_FUNCTION_ENABLE_INFO  fData;
    DWORD                       bData;
    BOOL                        doInterfaceChange = FALSE;
    SD_DEVICE_HANDLE            handle;

    handle = device->handle;

    /* Enable SDIO [dragon] function */
    fData.Interval = DEFAULT_SDIO_FUNCTION_RETRY_TIMEOUT;
    fData.ReadyRetryCount = DEFAULT_SDIO_FUNCTION_RETRIES;

    sdStatus = SDSetCardFeature (handle, SD_IO_FUNCTION_ENABLE,
                                  &fData, sizeof(fData));
    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }

    /*
     * Issue commands to get the manufacturer ID and stuff and compare it
     * against the rev Id derived from the ID registered during the
     * initialization process. Report the device only in the case there
     * is a match.
     */

    sdStatus = SDCardInfoQuery(handle, SD_INFO_SDIO,
                            &sdioInfo, sizeof(sdioInfo));
    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }
    funcNo = sdioInfo.FunctionNumber;

    sdStatus = SDCardInfoQuery(handle, SD_INFO_REGISTER_RCA,
                                &cardRCA, sizeof(cardRCA));
    AR_DEBUG_PRINTF(ATH_LOG_INF, ("Card RCA  is 0x%x\n", cardRCA));

    /* Configure the SDIO Bus Width */
    memset(&ci, 0, sizeof(ci));

        /* get current interface settings */
    sdStatus = SDCardInfoQuery(handle, SD_INFO_CARD_INTERFACE,
                                &ci, sizeof(ci));

    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }

    //DebugBreak();

#ifdef HIF_SDIO_1BIT
        /* force to 1 bit mode */
    sdio1bitmode = 1;
#endif

    if (sdio1bitmode && (ci.InterfaceMode != SD_INTERFACE_SD_MMC_1BIT)) {
            /* force to 1 bit mode */
        ci.InterfaceMode = SD_INTERFACE_SD_MMC_1BIT;
        doInterfaceChange = TRUE;
    }

        /* check for forced low speed operation */
    if (sdiobusspeedlow) {
            /* only set the lower of our current rate and our desired reduced rate,
             * otherwise we run at a clock rate that the bus driver setup for us */
        ci.ClockRate = min(ci.ClockRate, SDIO_CLOCK_FREQUENCY_REDUCED);
        doInterfaceChange = TRUE;
    }

    if (doInterfaceChange) {
        sdStatus = SDSetCardFeature(handle, SD_SET_CARD_INTERFACE,
                                    &ci, sizeof(ci));

        if (!SD_API_SUCCESS(sdStatus)) {
            return FALSE;
        }
    }

    AR_DEBUG_PRINTF(ATH_LOG_INF, ("** SDIO Interface : %s : %d (%s)\r\n",
        doInterfaceChange ? "Overridden to" : "Set by busdriver",
        ci.ClockRate,
        (ci.InterfaceMode == SD_INTERFACE_SD_MMC_1BIT) ?
        "1-bit" : "4-bit"));

   RETAIL_DEBUG_PRINTF( 1, (TEXT("** SDIO Interface : %S : %d (%S)\r\n"),
        doInterfaceChange ? "Overridden to" : "Set by busdriver",
        ci.ClockRate,
        (ci.InterfaceMode == SD_INTERFACE_SD_MMC_1BIT) ?
        "1-bit" : "4-bit"));

        /* save card interface mode to restore later */
    A_MEMCPY(&device->CardInterface, &ci, sizeof(ci));

#ifdef HIF_SINGLE_BLOCK_WAR

#ifdef HIF_SDIO_LARGE_BLOCK_MODE
        /* mixed block reduced mode */
    AR_DEBUG_PRINTF(ATH_LOG_ERR,("** SDIO large block work-around (single block , block size: %d) \r\n",
           HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE));
#else
        /* single block reduced mode */
    AR_DEBUG_PRINTF(ATH_LOG_INF,("** SDIO reduced block work-around (single block (SDIO byte-mode), write blen:%d) \r\n",
           MAX_BYTES_PER_TRANS_BYTE_MODE));
#endif
#endif

#ifdef HIF_DEFER_HIF_ASYNC_COMPLETION
    AR_DEBUG_PRINTF(ATH_LOG_INF,("** SDIO ASYNC DEFERRED COMPLETIONS \r\n"));
#endif

#ifdef HIF_SYNC
    AR_DEBUG_PRINTF(ATH_LOG_INF,("** SDIO SYNC I/O MODE \r\n"));
#else
    AR_DEBUG_PRINTF(ATH_LOG_INF,("** SDIO ASYNC I/O MODE \r\n"));
#endif

    /* Check if the target supports block mode */
    sdStatus = SDReadWriteRegistersDirect(handle, SD_IO_READ,
                    0, 0x08, FALSE, &ucRegVal, 1);
    if (!SD_API_SUCCESS(sdStatus)) {
        return FALSE;
    }
    blockMode = (ucRegVal & 0x2) >> 1; // SMB is bit 1
    if (!blockMode) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR, ("Function does not support block mode\n"));
        return FALSE;
    } else {
        AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Function supports block mode\n"));

        blockCap.ReadBlocks = blockCap.WriteBlocks = 8;
        blockCap.ReadBlockSize = blockCap.WriteBlockSize = HIF_MBOX_BLOCK_SIZE;

#ifdef HIF_SINGLE_BLOCK_WAR
#ifdef HIF_SDIO_LARGE_BLOCK_MODE

            /* using single block with a large block size */
        blockCap.ReadBlockSize = blockCap.WriteBlockSize =
                HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE;

#endif
#endif

        sdStatus = SDCardInfoQuery(handle, SD_INFO_HOST_BLOCK_CAPABILITY,
                        &blockCap, sizeof(blockCap));

        if (blockCap.ReadBlockSize < blockCap.WriteBlockSize) {
            maxBlockSize = blockCap.ReadBlockSize;
        } else {
            maxBlockSize = blockCap.WriteBlockSize;
        }
        if (blockCap.ReadBlocks < blockCap.WriteBlocks) {
            maxBlocks = blockCap.ReadBlocks;
        } else {
            maxBlocks = blockCap.WriteBlocks;
        }

        sdStatus = SDGetTuple(handle, SD_CISTPL_FUNCE, NULL, &ulLength, FALSE);
        if ((!SD_API_SUCCESS(sdStatus)) || (ulLength > sizeof(rgucTuple)) ) {
            return FALSE;
        }
        sdStatus = SDGetTuple(handle, SD_CISTPL_FUNCE, rgucTuple, &ulLength, FALSE);
        if ((!SD_API_SUCCESS(sdStatus)) ||
            (pFunce->bType != SD_CISTPL_FUNCE_FUNCTION_TYPE) ) {
            return FALSE;
        }

        if (maxBlockSize > pFunce->wMaxBlkSize) {
            maxBlockSize = pFunce->wMaxBlkSize;
        }

        bData = (DWORD)maxBlockSize;
        sdStatus = SDSetCardFeature(handle,
            SD_IO_FUNCTION_SET_BLOCK_SIZE,
            &bData, sizeof(bData));
    }

    AR_DEBUG_PRINTF(ATH_LOG_ERR,
                   ("Bytes Per Block: %d bytes, Block Count:%d \n",
                     maxBlockSize, maxBlocks));

#ifdef HIF_SINGLE_BLOCK_WAR
#ifdef HIF_SDIO_LARGE_BLOCK_MODE
    if (maxBlockSize < HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE) {
        AR_DEBUG_PRINTF(ATH_LOG_ERR,
                   ("!!! Cannot support this host controller in large block mode : Bytes Per Block: %d bytes (%d required)\n",
                     maxBlockSize, HIF_SDIO_MAX_BYTES_LARGE_BLOCK_MODE));
        return FALSE;
    }

#endif
#endif

#ifdef HIF_USE_SOFTBLOCK

#ifndef SD_SOFT_BLOCK_FORCE_UTILIZATION
#define SD_SOFT_BLOCK_FORCE_UTILIZATION 7
#endif

    /* in 4-bit mode attempt to enable soft block mode, so that the bus driver
     * performs multi-block transfers as a series of single block tranfers */
    sdStatus = SDSetCardFeature (handle,
                                 SD_SOFT_BLOCK_FORCE_UTILIZATION,
                                 NULL,
                                  0);

    if (!SD_API_SUCCESS(sdStatus)) {
        AR_DEBUG_PRINTF(ATH_LOG_INF,("** failed to enable soft block mode! (0x%X) \r\n",
           sdStatus);
    } else {
        g_SoftBlockAvail = TRUE;
    }


    if (g_SoftBlockAvail) {
        AR_DEBUG_PRINTF(ATH_LOG_INF,("** Soft block workaround enabled \r\n"));
    }

#endif

    return TRUE;
}



BOOL
hifDeviceInserted(SD_DEVICE_HANDLE *handle)
{
    HIF_DEVICE  *device;
#ifdef CEPC_ELLEN
    HANDLE       hThread;
#endif //CEPC

    device = addHifDevice(handle);
    AR_DEBUG_PRINTF(ATH_LOG_TRC, ("hifDeviceInserted: Enter\n"));

#ifdef HIF_SINGLE_BLOCK_WAR
    InitializeCriticalSection(&device->WriteLock);
    InitializeCriticalSection(&device->ReadLock);
#endif

    SetupSlotPowerControl(device);

    if (!SetupSDIOInterface(device)) {
        return FALSE;
    }

#ifdef HIF_SDIO_DISABLE_BMI_SLEEP
        /* workaround for slow enumerating hosts */
    DisableBMISleep(device);
    
#endif

#ifdef CEPC_ELLEN

    /* TODO: on CEPC, the PCI standard host driver on occasion fails to detect a pending interrupt (DAT1=low)
     * a work around for this is to process our interrupt in thread outside of the busdriver's
     * interrupt handling thread.  This forces the interrupt to be ack'd while we process it.
     */

    NdisInitializeEvent(&device->hifIRQEvent);
    hThread = CreateThread(NULL, 0,
        hifIRQThread, (LPVOID)handle, 0, NULL);
    CeSetThreadPriority(hThread, 200);
    CloseHandle(hThread);
#endif //CEPC

#ifdef HIF_SDIO_BYPASS

        /* activate bypass mode if it is available */
    g_BypassModeActive = ActivateSDIOStackBypassMode();

    if (g_BypassModeActive) {
        RETAILMSG(1,(TEXT("** SDIO Bypass Mode is now active \r\n")));
        AR_DEBUG_PRINTF(ATH_LOG_INF,("** SDIO Bypass Mode is now active \r\n"));
    }

#endif

        /* Inform OS layer */
    if ((osdrvCallbacks.deviceInsertedHandler(osdrvCallbacks.context,device)) != A_OK)
    {
        AR_DEBUG_PRINTF(ATH_LOG_TRC, ("Device rejected\n"));
        return FALSE;
    }

    return TRUE;
}

void
HIFAckInterrupt(HIF_DEVICE *device)
{

#ifdef HIF_SDIO_BYPASS
    if (g_BypassModeActive) {
        AckInterruptBypass();
    }
#endif

    return;
}

void
HIFUnMaskInterrupt(HIF_DEVICE *device)
{
    SD_API_STATUS sdStatus;

#ifdef HIF_SDIO_BYPASS
    if (g_BypassModeActive) {
        sdStatus = InterruptEnableDisableBypass(TRUE, hifIRQHandler);
    } else
#endif
    {
            /* Register the IRQ Handler */
       sdStatus = SDIOConnectInterrupt(device->handle, hifIRQHandler);
    }
    AR_DEBUG_ASSERT(SD_API_SUCCESS(sdStatus));
    return;
}

void HIFMaskInterrupt(HIF_DEVICE *device)
{
#ifdef HIF_SDIO_BYPASS
    if (g_BypassModeActive) {
        InterruptEnableDisableBypass(FALSE, NULL);
    } else
#endif
    {
        SDIODisconnectInterrupt(device->handle);
    }
    return;
}

void
hifDeviceRemoved(SD_DEVICE_HANDLE *handle)
{
    A_STATUS    status;
    HIF_DEVICE *device;

    device = getHifDevice(handle);
    if (device->claimedContext != NULL) {
        status = osdrvCallbacks.deviceRemovedHandler(device->claimedContext, device);
    }
    AR_DEBUG_ASSERT(status == A_OK);
#ifdef HIF_SINGLE_BLOCK_WAR
    DeleteCriticalSection(&device->WriteLock);
    DeleteCriticalSection(&device->ReadLock);
#endif
    CleanupSlotPowerControl(device);
    return;
}

HIF_DEVICE *
addHifDevice(SD_DEVICE_HANDLE *handle)
{
    hifDevice[0].handle = handle;
    return &hifDevice[0];
}

HIF_DEVICE *
getHifDevice(SD_DEVICE_HANDLE *handle)
{
    return &hifDevice[0];
}

void
delHifDevice(SD_DEVICE_HANDLE *handle)
{
    hifDevice[0].handle = NULL;
}

A_STATUS
HIFConfigureDevice(HIF_DEVICE *device, HIF_DEVICE_CONFIG_OPCODE opcode,
                   void *config, A_UINT32 configLen)
{
    A_UINT32 count;
    A_STATUS status = A_OK;

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

        case HIF_DEVICE_GET_IRQ_PROC_MODE:
            // TODO for now lets do sync-only (it's simplier), to opimize we should implement ASYNC
            // processing in our get pending events function
            *((HIF_DEVICE_IRQ_PROCESSING_MODE *)config) = HIF_DEVICE_IRQ_SYNC_ONLY;
            break;

        case HIF_DEVICE_POWER_STATE_CHANGE:
            status = PowerChangeNotify(device, *(HIF_DEVICE_POWER_CHANGE_TYPE *)config);
            break;

        default:
            return A_ERROR;
    }

    return status;
}

#ifdef HIF_DEFER_HIF_ASYNC_COMPLETION

/* the following thread processes ASYNC completions in a separate thread context to avoid
 * a deadlock when using the high-capacity bus driver */

static BOOL   g_ExitIOThread = FALSE;
static HANDLE g_hWakeIOThreadEvent = NULL;
static HANDLE g_hIOThread = NULL;
static HTC_PACKET_QUEUE g_AsyncIOQueue;
static CRITICAL_SECTION g_QueueLock;
static BOOL             g_IOInit = FALSE;

static void QueueIOPacket(HTC_PACKET *pPacket)
{
    EnterCriticalSection(&g_QueueLock);
    HTC_PACKET_ENQUEUE(&g_AsyncIOQueue,pPacket);
    LeaveCriticalSection(&g_QueueLock);
    SetEvent(g_hWakeIOThreadEvent);
}

static DWORD IOCompletionThreadFunc(LPVOID pContext)
{
    HTC_PACKET *pPacket;
    HIF_DEVICE *device = &hifDevice[0];
    
    CeSetThreadPriority(GetCurrentThread(),100);

    while (!g_ExitIOThread) {

        WaitForSingleObject(g_hWakeIOThreadEvent, INFINITE);

            /* unload the queue */
        while (1) {
            EnterCriticalSection(&g_QueueLock);
            pPacket = HTC_PACKET_DEQUEUE(&g_AsyncIOQueue);
            LeaveCriticalSection(&g_QueueLock);

            if (pPacket != NULL) {
                    /* complete this packet */
                HIF_INC_ASYNC_CB_REF();
                device->htcCallbacks.rwCompletionHandler(pPacket, pPacket->Status);
                HIF_DEC_ASYNC_CB_REF();
            } else {
                break;
            }

        }
    }

    return 0;
}

static void ShutdownIOCompletionThread()
{
    if (g_IOInit) {

        g_IOInit = FALSE;
        g_ExitIOThread = TRUE;

        if (g_hWakeIOThreadEvent != NULL) {
            SetEvent(g_hWakeIOThreadEvent);
        }

        if (g_hIOThread != NULL) {
            WaitForSingleObject(g_hIOThread, INFINITE);
            CloseHandle(g_hIOThread);
            g_hIOThread = NULL;
        }

        if (g_hWakeIOThreadEvent != NULL) {
            CloseHandle(g_hWakeIOThreadEvent);
            g_hWakeIOThreadEvent = NULL;
        }

        DeleteCriticalSection(&g_QueueLock);

    }
}

static void SetupIOCompletionThread()
{
    DWORD threadId;

    g_ExitIOThread = FALSE;

    if (!g_IOInit) {

        g_IOInit = TRUE;

        InitializeCriticalSection(&g_QueueLock);

        INIT_HTC_PACKET_QUEUE(&g_AsyncIOQueue);

        g_hWakeIOThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

        if (NULL == g_hWakeIOThreadEvent) {
            A_ASSERT(FALSE);
            return;
        }

        g_hIOThread = CreateThread(NULL,
                                   0,
                                   IOCompletionThreadFunc,
                                   NULL,
                                   0,
                                   &threadId);

        if (NULL == g_hIOThread) {
            ShutdownIOCompletionThread();
        }
    }
}

#endif



#ifdef HIF_HEARTBEAT

static BOOL   g_ExitThread = FALSE;
static HANDLE g_hExitEvent = NULL;
static HANDLE g_hThread = NULL;

A_BOOL g_doBreak = FALSE;

static DWORD HeartBeatThreadFunc(LPVOID pContext)
{
    int    count = 0;

    CeSetThreadPriority(GetCurrentThread(),10);

    RETAILMSG(1,
        (TEXT("HeartBeat Starting: priority: %d \r\n"),
            CeGetThreadPriority(GetCurrentThread())));

    while (!g_ExitThread) {
        WaitForSingleObject(g_hExitEvent, 5000);
        RETAILMSG(1, (TEXT("HB:%d  \r\n"),count));
        if (((count % 6) == 0) && g_doBreak) {
            DebugBreak();
        }
        count++;
    }

    RETAILMSG(1,
        (TEXT("HeartBeat thread exiting...(%d) \r\n"),
            CeGetThreadPriority(GetCurrentThread())));
    return 0;
}

void SetupHeartBeatThread()
{
    DWORD threadId;

    g_ExitThread = FALSE;

    if (g_hThread == NULL) {

        g_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

        if (NULL == g_hExitEvent) {
            return;
        }

        g_hThread = CreateThread(NULL,
                                 0,
                                 HeartBeatThreadFunc,
                                 NULL,
                                 0,
                                 &threadId);
    }


}

void ShutdownHeartBeatThread()
{
    g_ExitThread = TRUE;

    if (g_hExitEvent != NULL) {
        SetEvent(g_hExitEvent);
    }

    if (g_hThread != NULL) {
        WaitForSingleObject(g_hThread, INFINITE);
        CloseHandle(g_hThread);
        g_hThread = NULL;
    }

    if (g_hExitEvent != NULL) {
        CloseHandle(g_hExitEvent);
        g_hExitEvent = NULL;
    }

}
#endif

#ifdef HIF_SDIO_DISABLE_BMI_SLEEP

static void DisableBMISleep(HIF_DEVICE *device)
{
    A_UINT32 data;
    A_UINT32 address;
    A_STATUS status;
    SD_API_STATUS sdStatus;
    
    RETAILMSG(1,(TEXT("Disabling BMI sleep....\r\n")));    
    
    do {
        
            /* disable the function which puts the chip in HOST_OFF again */
        sdStatus = SDSetCardFeature(device->handle,
                                    SD_IO_FUNCTION_DISABLE,
                                    NULL,
                                    0);

        if (!SD_API_SUCCESS(sdStatus)) {
            break;    
        }

        {
            A_UINT8  buffer;
                /* Perform a I/O reset, this causes the SDIO interface
                 * to be in the un-enumerated state */
            buffer = 1 << 3;
            sdStatus = SDReadWriteRegistersDirect(device->handle,
                                                  SD_IO_WRITE,
                                                  0,
                                                  SD_IO_REG_IO_ABORT,
                                                  0,
                                                  &buffer,
                                                  1);

            A_ASSERT(SD_API_SUCCESS(sdStatus));
        }
        
        if (!SD_API_SUCCESS(sdStatus)) {
            break;    
        }
        
            /* re-init SDIO as if the card was re-inserted */
        status = ReinitSDIO(device);
        
        if (A_SUCCESS(status)) {
            RETAILMSG(1,(TEXT("SDIO reinit success \r\n")));    
        } else {
            break;    
        }
                          
        data = AR6K_OPTION_SLEEP_DISABLE;
        
            /* disable sleep option */
        status = HIFReadWrite(device,
                              SCRATCH_ADDRESS,
                             (A_UINT8 *)&data,
                              4,
                              HIF_WR_SYNC_BYTE_INC,
                              NULL);
                             
        if (A_FAILED(status)) {
            break;
        }
        
            /* disable sleep through the DIAG window */
            
        address = 0x40C4;
        
        status = ar6000_ReadRegDiag(device, &address, &data);
        
        if (A_FAILED(status)) {
            break;
        }
   
        data |= 0x1;
        
        status = ar6000_WriteRegDiag(device, &address, &data);        
        
        if (A_FAILED(status)) {
            break;
        }
            
        RETAILMSG(1,(TEXT("Disable BMI sleep successful. \r\n")));
            
    } while (FALSE);         
}

#endif

static void DoCMD53Test(HIF_DEVICE *device)
{
    A_STATUS status = A_OK;
    A_UINT32 data = 0;
    int      count = 0;
    A_UINT8  regTable[16];

    //DebugBreak();

    status = HIFReadWrite(device,
                          0x400,
                          regTable,
                          16,
                          HIF_RD_SYNC_BYTE_INC,
                          NULL);

    if (A_FAILED(status)) {
        RETAILMSG(1,(TEXT("Reg Table Read FAILLLLLLED  \r\n")));
    }

#if 0
    while (count < 1000) {

        /*status = HIFReadWrite(device,
                              0x460,
                              (A_UINT8 *)&data,
                              4,
                              HIF_WR_SYNC_BYTE_FIX,
                              NULL); */

        status = HIFReadWrite(device,
                              0x460,
                              (A_UINT8 *)&data,
                              4,
                              HIF_RD_SYNC_BYTE_FIX,
                              NULL);

        //if (A_FAILED(status)) {
        //    break;
        //}

        count++;
    }
#endif


}


#define SDIO_ENABLE_REG         0x02
#define SDIO_READY_REG          0x03
#define SDIO_BUS_IF_REG         0x07
#define CARD_DETECT_DISABLE     0x80
#define SD_SDIO_R4_GET_OCR(pR) ((A_UINT32)((pR)[1])        |  \
                          (((A_UINT32)(pR)[2]) << 8)  |  \
                          (((A_UINT32)(pR)[3]) << 16))

#define SD_SDIO_R4_IS_CARD_READY(pR)       (((pR)[4] & 0x80) == 0x80)
#define SD_SDIO_R6_GET_RCA(pR)             ((A_UINT16)((pR)[3]) | ((A_UINT16)((pR)[4]) << 8))

static A_BOOL IssueSDCommand(SD_DEVICE_HANDLE     hDevice,
                             A_UINT8              Cmd,
                             A_UINT32             Argument,
                             SD_RESPONSE_TYPE     ResponseType,
                             SD_COMMAND_RESPONSE  *pResponse)
{
    SD_API_STATUS sdStatus;

    sdStatus = SDSynchronousBusRequest(hDevice,
                                       Cmd,
                                       Argument,
                                       SD_COMMAND,
                                       ResponseType,
                                       pResponse,
                                       0,
                                       0,
                                       NULL,
                                       0);

    return SD_API_SUCCESS(sdStatus) ? TRUE : FALSE;
}

static A_BOOL IssueCMD52(SD_DEVICE_HANDLE hDevice,
                         A_UINT32         Address,
                         A_UCHAR          Func,
                         A_BOOL           Write,
                         A_UINT8          *pData)
{
   SD_API_STATUS sdStatus;

   sdStatus = SDReadWriteRegistersDirect(hDevice,
                                         Write ? SD_IO_WRITE : SD_IO_READ,
                                         Func,
                                         Address,
                                         0,
                                         pData,
                                         1);

    return TRUE;
}



static A_STATUS ReinitSDIO(HIF_DEVICE *device)
{
    A_STATUS            status = A_OK;
    SD_COMMAND_RESPONSE response;
    A_UINT32            ocr;
    int                 cardReadyRetry = 10;
    A_UINT16            rca;
    A_UINT8             cmd52Data;

    do {

        if (!IssueSDCommand(device->handle,0,0,NoResponse,NULL)) {
            break;
        }

#ifdef BLUEBIRD_WINMOBILE
		Sleep(3);
#else
        Sleep(30);
#endif

        AR_DEBUG_PRINTF(ATH_LOG_INF,("Issuing first CMD5 ... \n"));

            /* issue CMD5, ARG=0 */
        if (!IssueSDCommand(device->handle,5,0,ResponseR4,&response)) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("CMD5 FAILED, NO card detected \n"));
            break;
        }

        ocr = SD_SDIO_R4_GET_OCR(response.ResponseBuffer);

        AR_DEBUG_PRINTF(ATH_LOG_INF,("CMD5 success, OCR = 0x%X \n",ocr));

            /* issue CMD5, ARG=0x00100000 repeatedly until card is ready */
        while (cardReadyRetry) {
            if (!IssueSDCommand(device->handle,
                                5,
                                0x00100000, /* use 3.0 volts, although this really is a don't care */
                                ResponseR4,
                                &response)) {
                AR_DEBUG_PRINTF(ATH_LOG_ERR,("CMD5 Failed \n"));
                cardReadyRetry = 0;
                break;
            }

            if (SD_SDIO_R4_IS_CARD_READY(response.ResponseBuffer)) {
                AR_DEBUG_PRINTF(ATH_LOG_INF,("SDIO Card is Ready! \n"));
                break;
            }
            cardReadyRetry--;
            Sleep(10);
        }

        if (cardReadyRetry == 0) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("CMD5 Ready polling Expired \r\n"));
            break;
        }

        AR_DEBUG_PRINTF(ATH_LOG_INF,("Issuing CMD3 ... \n"));

             /* issue CMD3 */
        if (!IssueSDCommand(device->handle,3,0,ResponseR6,&response)) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("CMD3 FAILED \n"));
            break;
        }

        rca = SD_SDIO_R6_GET_RCA(response.ResponseBuffer);

        AR_DEBUG_PRINTF(ATH_LOG_INF,("Card RCA : 0x%X \n",rca));

        AR_DEBUG_PRINTF(ATH_LOG_INF,("Issuing CMD7 ... \n"));

              /* issue CMD7 to select card */
        if (!IssueSDCommand(device->handle, 7,(((A_UINT32)rca) << 16),ResponseR1b,&response)) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("CMD7 FAILED \n"));
            break;
        }

        AR_DEBUG_PRINTF(ATH_LOG_INF,("Card is now selected! \n"));

            /* disable card detect resistor and restore operational bus width */
        cmd52Data = CARD_DETECT_DISABLE;
        if (device->CardInterface.InterfaceMode == SD_INTERFACE_SD_4BIT) {
                /* enable 4-bit mode */
            cmd52Data |= 0x2;
        }

        if (!IssueCMD52(device->handle,
                        SDIO_BUS_IF_REG,
                        0,
                        TRUE,
                        &cmd52Data)) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("Failed to set bus mode register \r\n"));
            break;
        }

            /* complete interface setup */
        if (!SetupSDIOInterface(device)) {
            status = A_ERROR;
        }

    } while (FALSE);


    return status;
}

#define POWER_OFF 1
#define POWER_ON  0


static A_STATUS PowerChangeNotify(HIF_DEVICE *device, HIF_DEVICE_POWER_CHANGE_TYPE PowerChange)
{
    A_STATUS      status = A_OK;
    SD_API_STATUS sdStatus = SD_API_STATUS_SUCCESS;

RETAILMSG(1, (TEXT("builder:ar6k2:%i PowerChangeNotify() start\r\n"), GetTickCount()));

    switch (PowerChange) {

        case HIF_DEVICE_POWER_UP:
#if 0	//buildertest:power lock		
            if (InterlockedExchange(&device->PowerStateOff, POWER_ON) != POWER_OFF) {
                    /* shouldn't be calling this if we are not powered off */
                A_ASSERT(FALSE);
                break;
            }
#endif			
RETAILMSG(1, (TEXT("builder:ar6k2:%i PowerChangeNotify() 0\r\n"), GetTickCount()));
            if (device->SlotPowerRemoved) {
                    /* power was removed from a previous power down attempt */
                device->SlotPowerRemoved = FALSE;
                    /* try powering up */
                PowerUpDownSlot(device, TRUE);
            }
RETAILMSG(1, (TEXT("builder:ar6k2:%i PowerChangeNotify() 1\r\n"), GetTickCount()));
                /* Reinit SDIO since HIF_DEVICE_POWER_DOWN or HIF_DEVICE_POWER_OFF
                 * would have reset the SDIO interface */
            status = ReinitSDIO(device);

#ifdef HIF_SDIO_BYPASS
            if (A_SUCCESS(status)) {
                    /* re-activate */
                g_BypassModeActive = ActivateSDIOStackBypassMode();
            }
#endif
            break;

        case HIF_DEVICE_POWER_DOWN:
        case HIF_DEVICE_POWER_CUT:
#if 0	//buildertest:power lock
                /* set powered off flag, but check previous value */
            if (InterlockedExchange(&device->PowerStateOff, POWER_OFF) != POWER_ON) {
                    /* shouldn't be calling this if we are powered off already  */
                A_ASSERT(FALSE);
                break;
            }
#endif			

#ifdef HIF_SDIO_BYPASS
            g_BypassModeActive = FALSE;
                /* de-activeate bypass mode  */
            DeactivateSDIOStackBypassMode();
#endif

                /* disable I/O function and reset SDIO controller to minimize power */
                sdStatus = SDSetCardFeature(device->handle,
                                            SD_IO_FUNCTION_DISABLE,
                                            NULL,
                                            0);

                A_ASSERT(SD_API_SUCCESS(sdStatus));

                {
                    A_UINT8  buffer;
                        /* Perform a I/O reset, this causes the SDIO interface
                         * to be in the un-enumerated state */
                    buffer = 1 << 3;
                    sdStatus = SDReadWriteRegistersDirect(device->handle,
                                                          SD_IO_WRITE,
                                                          0,
                                                          SD_IO_REG_IO_ABORT,
                                                          0,
                                                          &buffer,
                                                          1);

                    A_ASSERT(SD_API_SUCCESS(sdStatus));
                }

            if (PowerChange == HIF_DEVICE_POWER_CUT) {
                    /* caller wants the device to be completely off */
                device->SlotPowerRemoved = PowerUpDownSlot (device, FALSE);
            }

            break;

        default:
            A_ASSERT(FALSE);
            return A_ERROR;
    }

RETAILMSG(1, (TEXT("builder:ar6k2:%i PowerChangeNotify() end\r\n"), GetTickCount()));
    return status;
}

#define MAX_POWER_CONTROL_DEV_NAME 16

    /* get optional slot power control information from the registry */
static void SetupSlotPowerControl(HIF_DEVICE *device)
{
    TCHAR   deviceName[MAX_POWER_CONTROL_DEV_NAME + 1];

    if (NULL == device->hSlotPowerDevice) {
            /* first time through */
        device->hSlotPowerDevice = INVALID_HANDLE_VALUE;
    }

    if (device->hSlotPowerDevice != INVALID_HANDLE_VALUE) {
        return;
    }

    A_MEMZERO(deviceName,sizeof(deviceName));

    do {

            /* get the device name */
        if (!GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                                HIF_SDIO_PARAMS_REG_PATH,
                                SLOTPOWER_DEV_NAME_REG_VAL_KEY,
                                (PUCHAR)deviceName,
                                MAX_POWER_CONTROL_DEV_NAME * (sizeof(TCHAR)))) {
            break;
        }

        DEBUGMSG(1,(TEXT(" Slot power control device name : %s \r\n"), deviceName));

        device->hSlotPowerDevice = CreateFile(deviceName,
                                              GENERIC_READ | GENERIC_WRITE,
                                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                                              NULL,
                                              OPEN_EXISTING,
                                              FILE_ATTRIBUTE_NORMAL,
                                              0);

        if (INVALID_HANDLE_VALUE == device->hSlotPowerDevice) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("Failed to open slot power control device (%d) \n",
                GetLastError()));
            break;
        }

            /* get IOCTL codes for ON/OFF */
        GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                            HIF_SDIO_PARAMS_REG_PATH,
                            SLOTPOWER_OFF_IOCTL_REG_VAL_KEY,
                            (PUCHAR)&device->SlotPowerOffIoctl,
                            sizeof(device->SlotPowerOffIoctl));

        GetRegistryKeyValue(HKEY_LOCAL_MACHINE,
                            HIF_SDIO_PARAMS_REG_PATH,
                            SLOTPOWER_ON_IOCTL_REG_VAL_KEY,
                            (PUCHAR)&device->SlotPowerOnIoctl,
                            sizeof(device->SlotPowerOnIoctl));

        AR_DEBUG_PRINTF(ATH_LOG_INF,(" Slot power IOCTL codes on:%d off:%d \r\n", device->SlotPowerOnIoctl,
                device->SlotPowerOffIoctl));

    } while (FALSE);

}

static void CleanupSlotPowerControl(HIF_DEVICE *device)
{
    if (INVALID_HANDLE_VALUE == device->hSlotPowerDevice) {
        return;
    }

    CloseHandle(device->hSlotPowerDevice);
    device->hSlotPowerDevice = INVALID_HANDLE_VALUE;
}

static BOOL PowerUpDownSlot(HIF_DEVICE *device, A_BOOL PowerUp)
{
    DWORD returnBytes;
    BOOL  retVal = FALSE;

    if (device->hSlotPowerDevice != INVALID_HANDLE_VALUE) {
        if (!DeviceIoControl(device->hSlotPowerDevice,
                             PowerUp ? device->SlotPowerOnIoctl : device->SlotPowerOffIoctl,
                             NULL,
                             0,
                             NULL,
                             0,
                             &returnBytes,
                             NULL)) {
            AR_DEBUG_PRINTF(ATH_LOG_ERR,("Failed to power %s slot \n", PowerUp ? "UP" : "DOWN"));
        } else {
            retVal = TRUE;
        }
    }

    return retVal;
}

void HIFClaimDevice(HIF_DEVICE  *device, void *context)
{
    device->claimedContext = context;   
}

void HIFReleaseDevice(HIF_DEVICE  *device)
{
    device->claimedContext = NULL;    
}

A_STATUS HIFAttachHTC(HIF_DEVICE *device, HTC_CALLBACKS *callbacks)
{
    if (device->htcCallbacks.context != NULL) {
            /* already in use! */
        return A_ERROR;    
    }
    device->htcCallbacks = *callbacks; 
    return A_OK;
}

void HIFDetachHTC(HIF_DEVICE *device)
{
    A_MEMZERO(&device->htcCallbacks,sizeof(device->htcCallbacks));
}

