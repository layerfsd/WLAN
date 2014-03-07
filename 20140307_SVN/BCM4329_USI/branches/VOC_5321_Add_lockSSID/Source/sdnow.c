
/*
 * sdnow.c: SDIO bus driver abstraction layer
 *	provides the bcmsdh API for a set of sdio host controllers, 
 *	encapsulates the knowledge of BRCM sdio device core
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 * $Id: sdnow.c,v 1.56.8.4.6.15 2010/05/02 06:44:24 Exp $
 */
#include <typedefs.h>
#include <bcmdevs.h>
#include <bcmendian.h>
#include <bcmutils.h>
#include <hndsoc.h>
#include <osl.h>

#include <sdio.h>
#include <sbsdio.h>
#include <bcmsdh.h>
#include <dhd_dbg.h>
#include <wlioctl.h>
#include <wl_ndis.h>

BOOL			g_IsDriverLoaded = FALSE;

#include <bldver.h>
#include <sdiovar.h>

#include <SDCardDDK.h>

/* FILE-CSTYLED */

/* debug/trace */
#ifdef	BCMDBG
#define	SD_ERROR(args)	printf args
#else
#define	SD_ERROR(args)
#endif
#if defined(EXT_STA)
#include <ndis.h>
#endif /* UNDER_CE && EXT_STA */
struct _DBGPARAM dpCurSettings = {0};

#define SD_NOW_TAG 'SDIO'
#define NUMBER_OF_TAGS 4
#define NUMBER_OF_ASYNC_REQUESTS	32

#define BLOCK_TRANSFER_LENGTH_FUNC_1		(64)
#define BLOCK_TRANSFER_LENGTH_FUNC_2		(512)
#define BYTEMODE_MAX_LENGTH 	(64)	

#define AcquireRemovalLock(sdi) EnterCriticalSection(&(sdi)->RemovalLock)
#define ReleaseRemovalLock(sdi) LeaveCriticalSection(&(sdi)->RemovalLock)

#define NDIS_SUCCESS(Status) ((NDIS_STATUS)(Status) == NDIS_STATUS_SUCCESS)

#define SDNOW_MAX_ACTIVE_REG_PATH 256

#define IOCTL_WLAN_ON	CTL_CODE(FILE_DEVICE_HAL, 4001, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WLAN_OFF	CTL_CODE(FILE_DEVICE_HAL, 4002, METHOD_BUFFERED, FILE_ANY_ACCESS)

struct bcmsdh_info {
	NDIS_HANDLE		ConfigurationHandle;    /* NDIS configuration handle */
	SD_DEVICE_HANDLE	hDevice;		/* SD device handle */
	SD_DEVICE_HANDLE	hDevicefun2;
	WCHAR			ActivePath[SDNOW_MAX_ACTIVE_REG_PATH];	/* adapter regpath */
	
	UCHAR			Function;		/* total I/O function number */
	USHORT			RCA;			/* relative card address */
	UCHAR			device_code;		/* devicecode */
	UCHAR			card_cap;		/* capability */
	DWORD			cis_pointer;		/* cis address */
	DWORD			csa_pointer;		/* csa address */

	BOOL			InterruptEnabled;	/* interrupt connnected flag */
	ULONG			InterruptCount;		/* interrupt count */
	BOOL			CardEjected;		/* card eject flag */
	BOOL			intrh_valid;		/* client driver interrupt handler valid */
	bcmsdh_cb_fn_t		intr_handler;		/* registered interrupt handler */
	VOID			*intrh_argh;		/* argument to call interrupt handler */
	
	bcmsdh_cb_fn_t		dev_remove_handler;	/* registered device remove handler */
	VOID			*dev_remove_argh;	/* argument to call device remove handler */
	BOOL			devremoveh_valid;	/* client driver device remove handler valid */
	uint32			vendevid;
	uint32			sdclockrate;
	uint32			sdbuswidth;
	uint32			sdforceHS;
	uint32			sdfastpath;
	uint32			sdsoftblock;
	osl_t			*osh;
	uint32			sbwad;				/* back plane address window */
	wl_info_t       *wl;

	/* below are for Async requeset and its callback */
	CRITICAL_SECTION    IoLock;                 // interlock
	CRITICAL_SECTION    RemovalLock;            // removal lock
	HANDLE              hCompletionEvent;       // async I/O completion event
	ULONG               AsyncErrors;            // error count for Async cmd
	LONG                RequestCount;           // running request count
	LONG                RequestCompleteCount;   // running completed request count
	PSD_BUS_REQUEST     pRequests[NUMBER_OF_ASYNC_REQUESTS];  // array to hold requests  
	DWORD               CurrentRequestBufferIndex;  // buffer index for current transfer
	DWORD               NextReadAddress;            // next read address

	HANDLE				oobIntrEvent;
	HANDLE				hIST;
	DWORD				dwSysIntr;
	int					ISTPriority;

	BOOL				regfail;
};

/* for those calls passing no sdi handler */
bcmsdh_info_t * backupsdi = NULL;

static NDIS_STATUS sdnow_get_device_handler(bcmsdh_info_t *sdi, osl_t *osh);
static NDIS_STATUS sdnow_init_adapter(bcmsdh_info_t *sdi, osl_t *osh);
static SD_API_STATUS sdnow_setup_card(bcmsdh_info_t *sdi, osl_t *osh);
static VOID sdnow_cleanup(bcmsdh_info_t * sdi);
static VOID sdnow_slot_event_callback(SD_DEVICE_HANDLE hDevice, PVOID pContext, SD_SLOT_EVENT_TYPE SlotEventType, PVOID pData, DWORD DataLength);

static uint sdnow_cmd52_rw_byte(bcmsdh_info_t * sdi, ULONG rw, UCHAR func_num, ULONG offset, UCHAR *onebyte);
static uint sdnow_cmd52_rw_buf (bcmsdh_info_t * sdi, ULONG rw, UCHAR func_num, ULONG start_addr, PUCHAR p_buffer, ULONG len); 
static uint sdnow_cmd53_rw_reg (bcmsdh_info_t * sdi, ULONG rw, UCHAR func_num, ULONG offset, UCHAR *buf, ULONG size);
static uint sdnow_cmd53_rw_buf (bcmsdh_info_t * sdi, ULONG rw, UCHAR func_num, ULONG start_addr, UCHAR inc_fix, PUCHAR p_buffer, ULONG len);

static SD_API_STATUS sdnow_intr_callback(SD_DEVICE_HANDLE hDevice, PVOID pContext);
static int sdnow_intr_disable(bcmsdh_info_t * sdi);
static int sdnow_intr_enable(bcmsdh_info_t * sdi);

#if 1 // Chris.jeong.
void sdnow_init_Interrupt(bcmsdh_info_t *sdi);
#endif

/* IOVar table */
enum {
	IOV_BLOCKSIZE = 1,
	IOV_DEVREG
};

const bcm_iovar_t sdioh_iovars[] = {
	{"sd_blocksize", IOV_BLOCKSIZE, 0,	IOVT_UINT32,	0 }, /* ((fn << 16) | size) */
	{"sd_devreg", IOV_DEVREG,	0,	IOVT_BUFFER,	sizeof(sdreg_t)	},
	{NULL, 0, 0, 0, 0 }
};

void bcmsdh_device_remove(void * sdh)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	ASSERT(sdi);

	if (!sdi->dwSysIntr)
		SDIODisconnectInterrupt(sdi->hDevice);
}

/*** implementation of exported APIs for bcmsdh.h */

/* 
 * bcmsdh_attach - initialize SDIO bus and local structure, 
 *  INPUT: config handler, 
 *  RETURN: TRUE on success, FALSE otherwise 
 */
bcmsdh_info_t *
bcmsdh_attach(osl_t *osh, void *cfghdl, void **regsva, uint irq)
{
	NDIS_STATUS status;
	bcmsdh_info_t *sdi = NULL;
	NDIS_HANDLE confighandle = NULL;
	NDIS_STRING SDClock = NDIS_STRING_CONST("SDClock");
	NDIS_STRING SDBusWidth = NDIS_STRING_CONST("SDBusWidth");
	NDIS_STRING SDForceHighSpeed = NDIS_STRING_CONST("SDForceHighSpeed");
	NDIS_STRING SDFastPath = NDIS_STRING_CONST("SDFastPath");
	NDIS_STRING SDSoftBlock = NDIS_STRING_CONST("SDSoftBlock");
	NDIS_STRING EnableOOBInterrupt = NDIS_STRING_CONST("EnableOOBInterrupt");
	NDIS_STRING OOBIstPriority = NDIS_STRING_CONST("OOBIstPriority");
	PNDIS_CONFIGURATION_PARAMETER param;
#if defined(NDIS60)
	NDIS_CONFIGURATION_OBJECT configObj;
#endif /* NDIS60 */
	SD_ERROR(("bcmsdh_attach: %s\n", __FUNCTION__));
	
	if ((sdi = (bcmsdh_info_t *)MALLOC(osh, sizeof(bcmsdh_info_t))) == NULL) {
		SD_ERROR(("bcmsdh_attach: out of memory, malloced %d bytes\n", MALLOCED(osh)));
		goto err;
	}

	/* save the handler locally */
	backupsdi = sdi;
	bzero((char *)sdi, sizeof(bcmsdh_info_t));

	/* other card initialization */
	InitializeCriticalSection(&sdi->IoLock);	/* win32 API */
	InitializeCriticalSection(&sdi->RemovalLock);	/* win32 API */

	sdi->ConfigurationHandle = cfghdl;
	/* initialize regbase */
	sdi->intrh_valid = FALSE;
	sdi->devremoveh_valid = FALSE;
	sdi->osh = osh;
	sdi->wl = (wl_info_t *)irq;
#if defined(NDIS60)
	SD_ERROR(("bcmsdh_attach: %s, calling NdisOpenConfigurationEx() now ....\n", __FUNCTION__));

	bzero(&configObj, sizeof(NDIS_CONFIGURATION_OBJECT));		
	configObj.Flags = 0;
	configObj.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
	configObj.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT); 
	configObj.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
	configObj.NdisHandle = sdi->ConfigurationHandle;
	status = NdisOpenConfigurationEx(&configObj, &confighandle);

	SD_ERROR(("bcmsdh_attach: %s, out of NdisOpenConfigurationEx() now ....\n", __FUNCTION__));
#else

	/* open the registry */
	NdisOpenConfiguration(&status, &confighandle, sdi->ConfigurationHandle);
#endif /* NDIS60 */
	if (NDIS_ERROR(status)) {
		SD_ERROR(("bcmsdh_attach: failed to open registry\n"));
		goto err;
	}
	ASSERT(confighandle);

	/* read SDClock - from registry */
	NdisReadConfiguration(&status, &param, confighandle, &SDClock,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS) {
		sdi->sdclockrate = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
		/* Range check */
		if (sdi->sdclockrate <= 0 || sdi->sdclockrate > 25000000)
			sdi->sdclockrate = 0;
	}else
		SD_ERROR(("bcmsdh_attach: failed to read SDClock registry entry\n"));

	/* read SDBusWidth - from registry */
	NdisReadConfiguration(&status, &param, confighandle, &SDBusWidth,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS) {
		sdi->sdbuswidth = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
		/* Range check */
		if (sdi->sdbuswidth != 1 && sdi->sdbuswidth != 4)
			sdi->sdbuswidth = 0;
	}
	else
		SD_ERROR(("bcmsdh_attach: failed to read SDBusWidth registry entry\n"));

	/* read SDForceHighSpeed - from registry */
	NdisReadConfiguration(&status, &param, confighandle, &SDForceHighSpeed,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS)
		sdi->sdforceHS = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
	else
		SD_ERROR(("bcmsdh_attach: failed to read SDForceHighSpeed registry entry\n"));

	/* read SDFastPath - from registry */
	sdi->sdfastpath = 1;	/* By default trun on Fastpath */
	NdisReadConfiguration(&status, &param, confighandle, &SDFastPath,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS)
		sdi->sdfastpath = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
	else
		SD_ERROR(("bcmsdh_attach: failed to read SDFastPath registry entry\n"));

	/* read SDSoftBlock - from registry */
	sdi->sdsoftblock = 0;	/* By default turn off Softblock */
	NdisReadConfiguration(&status, &param, confighandle, &SDSoftBlock,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS)
		sdi->sdsoftblock = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
	else
		SD_ERROR(("bcmsdh_attach: failed to read SDSoftBlock registry entry\n"));

	/* read OOB interrupt setting - from registry */
	sdi->dwSysIntr = 0;	/* By default turn off Softblock */
	NdisReadConfiguration(&status, &param, confighandle, &EnableOOBInterrupt,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS)
		sdi->dwSysIntr = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
	else
		SD_ERROR(("bcmsdh_attach: failed to read EnableOOBInterrupt registry entry\n"));

	/* read OOB IST priority - from registry */
	NdisReadConfiguration(&status, &param, confighandle, &OOBIstPriority,
		NdisParameterInteger);
	if (status == NDIS_STATUS_SUCCESS)
		sdi->ISTPriority = (NDIS_ENVIRONMENT_TYPE)param->ParameterData.IntegerData;
	else
		SD_ERROR(("bcmsdh_attach: failed to read OOBIstPriority registry entry\n"));

	if (confighandle)
		NdisCloseConfiguration(confighandle);

	/* get the device handler from ActivePath registry */
	status = sdnow_get_device_handler(sdi, osh);
	if (!NDIS_SUCCESS(status)) {
		SD_ERROR(("BCMSD: No device Handle!\n"));
		goto err; 
	}

	/* initialize the card and register interrupt handler */
	status = sdnow_init_adapter(sdi, osh);
        if (!NDIS_SUCCESS(status)) {
		SD_ERROR(("BCMSD: BCMSD: InitializeAdapter failed (0x%08X)\n", status));
                
		/* free our adapter */
		goto err;
	}
	
	sdnow_intr_disable(sdi);	/* disable interrupt */

	*regsva = (void *)SI_ENUM_BASE;
	sdi->sbwad = SI_ENUM_BASE;
	SD_ERROR(("BCMSD: Initialized sucessfully !\n"));

	return sdi;

err:
	bcmsdh_detach(osh, sdi);
	*regsva = 0;

	return NULL;
}

int 
bcmsdh_detach(osl_t *osh, bcmsdh_info_t *sdi)
{
	SD_ERROR(("wl: bcmsdh_detach, sdi = 0x%x \n", sdi));

	if (sdi) {
		sdnow_cleanup(sdi);
		MFREE(osh, sdi, sizeof(bcmsdh_info_t));
		sdi = NULL;
	}
	backupsdi = NULL;

	return 0;
}

bool 
bcmsdh_intr_query(void *sdh)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	ASSERT(sdi);
	return (sdi->InterruptEnabled == TRUE);
}

int
bcmsdh_intr_enable(void *sdh)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;

	ASSERT(sdi);

	EnterCriticalSection(&sdi->IoLock);

	if (sdnow_intr_enable(sdi)) {
		LeaveCriticalSection(&sdi->IoLock);
		return BCME_ERROR;
	}

	LeaveCriticalSection(&sdi->IoLock);

	return 0;
}

int
bcmsdh_intr_disable(void *sdh)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;

	ASSERT(sdi);

	EnterCriticalSection(&sdi->IoLock);

	if (sdnow_intr_disable(sdi)) {
		LeaveCriticalSection(&sdi->IoLock);
		return BCME_ERROR;
	}

	LeaveCriticalSection(&sdi->IoLock);
	return 0;
}

int 
bcmsdh_intr_reg(void *sdh, bcmsdh_cb_fn_t fn, void *argh)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	ASSERT(sdi);
	sdi->intr_handler = fn;
	sdi->intrh_argh = argh;
	sdi->intrh_valid = TRUE;
	return 0;
}

static void 
intr_dummy(void *dum)
{
	return;
}

int 
bcmsdh_intr_dereg(void *sdh)
{	
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	ASSERT(sdi);
	sdi->intrh_valid = FALSE;
	sdi->intr_handler = intr_dummy;
	sdi->intrh_argh = NULL;
	return 0;
}

int
bcmsdh_devremove_reg(void *sdh, bcmsdh_cb_fn_t fn, void *argh)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	ASSERT(sdi);
	sdi->dev_remove_handler = fn;
	sdi->dev_remove_argh = argh;
	sdi->devremoveh_valid = TRUE;
	return 0;
}

#if defined(DHD_DEBUG) || defined(BCMDBG)
bool
bcmsdh_intr_pending(void *sdh) {
	return FALSE;
}
#endif

int
bcmsdh_cis_read(void *sdh, uint fnc_num, uint8 *cis, uint32 length)
{
	uint32 offset;
	uint status ;
	
	EnterCriticalSection(&backupsdi->IoLock);

	if (fnc_num == SDIO_FUNC_0) {
		offset = SBSDIO_CIS_BASE_COMMON;
	} else if (fnc_num == SDIO_FUNC_1) {
		offset = backupsdi->cis_pointer;
	}

	status = sdnow_cmd52_rw_buf(backupsdi, SD_IO_OP_READ, SDIO_FUNC_0, offset, cis, length);

	LeaveCriticalSection(&backupsdi->IoLock);

	return (status ? BCME_ERROR : 0);
}

/* read/write config register in particular function */
uint8 
bcmsdh_cfg_read(void *sdh, uint fuc, uint32 addr, int *err)
{
	uint8 data = 0xff;
	uint status;
	
	ASSERT(fuc <= SBSDIO_NUM_FUNCTION);

	EnterCriticalSection(&backupsdi->IoLock);

	status = sdnow_cmd52_rw_byte(backupsdi, SD_IO_OP_READ, fuc, addr, &data);
 	if (err)
		*err = (status ? BCME_SDIO_ERROR : 0);

	LeaveCriticalSection(&backupsdi->IoLock);

	return data;
}

/* Dummy call.  Will be required for gSPI bus. */
uint32
bcmsdh_cfg_read_word(void *sdh, uint fnc_num, uint32 addr, int *err)
{
	return 0;
}


void
bcmsdh_cfg_write(void *sdh, uint fuc, uint32 addr, uint8 data, int *err)
{
	uint status;

	ASSERT(fuc <= SBSDIO_NUM_FUNCTION);

	EnterCriticalSection(&backupsdi->IoLock);
#if defined(NDIS60)
	/* OSL_DELAY(50); */
#endif /* NDIS60 */
	if ((fuc == 0) && (addr >= SBSDIO_CIS_BASE_COMMON)) {
		SD_ERROR(("Write abort: sdio func0 CIS is write-protected\n"));
		if (err)
			*err = BCME_ERROR;

		LeaveCriticalSection(&backupsdi->IoLock);
		return;
	}

	status = sdnow_cmd52_rw_byte(backupsdi, SD_IO_OP_WRITE, fuc, addr, &data);
	if (err)
		*err = (status ? BCME_SDIO_ERROR : 0);

	LeaveCriticalSection(&backupsdi->IoLock);
}


static int
bcmsdhsdio_set_sbaddr_window(void *sdh, uint32 address)
{
	int err = 0;
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	bcmsdh_cfg_write(sdh, SDIO_FUNC_1, SBSDIO_FUNC1_SBADDRLOW,
		(address >> 8) & SBSDIO_SBADDRLOW_MASK, &err);
	if (!err)
		bcmsdh_cfg_write(sdh, SDIO_FUNC_1, SBSDIO_FUNC1_SBADDRMID,
			(address >> 16) & SBSDIO_SBADDRMID_MASK, &err);
	if (!err)
		bcmsdh_cfg_write(sdh, SDIO_FUNC_1, SBSDIO_FUNC1_SBADDRHIGH,
			(address >> 24) & SBSDIO_SBADDRHIGH_MASK, &err);

	return err;
}

/* access register in function 1 for 1/2/4 bytes */
uint32 
bcmsdh_reg_read(void *sdh, uint32 addr, uint size)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	uint8 buf[4] = {0};
	uint32 res = 0;
	uint fuc = SDIO_FUNC_1;	/* chip registers are through function 1 */
	uint status = 0;
	uint bar0 = (addr & (~SBSDIO_SB_OFT_ADDR_MASK));
	int err = 0;
	
	ASSERT(size == 2 || size == 4);
	
	if (!sdi)
		sdi = backupsdi;

	if (bar0 != sdi->sbwad) {
		/* Need to rebase back plane address window */
		err = bcmsdhsdio_set_sbaddr_window(sdh, bar0);
		if (err)
			return 0xFFFFFFFF;
		sdi->sbwad = bar0;
	}
	addr &= SBSDIO_SB_OFT_ADDR_MASK;

	EnterCriticalSection(&backupsdi->IoLock);

	backupsdi->regfail = FALSE;

	if ( size == 4 ) {
		addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;
	}
	status = sdnow_cmd53_rw_reg(backupsdi, SD_IO_OP_READ, fuc, addr, (UCHAR *)&res, size);
	if (status != 0)
		goto fail;

	LeaveCriticalSection(&backupsdi->IoLock);
	return res; /* good read */

 fail:
	backupsdi->regfail = TRUE;
	if (size == 2) {
		LeaveCriticalSection(&backupsdi->IoLock);
		return 0xffff;
	} else {
		LeaveCriticalSection(&backupsdi->IoLock);
		return 0xffffffff;
	}
}

uint32 
bcmsdh_reg_write(void *sdh, uint32 addr, uint size, uint32 data)
{
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	uint fuc = SDIO_FUNC_1;	/* chip registers are through function 1 */
	uint status = 0;
	int err = 0;
	uint bar0;

	ASSERT(size == 2 || size == 4);

	if (!sdi)
		sdi = backupsdi;

	bar0 = addr & ~SBSDIO_SB_OFT_ADDR_MASK; 	
	if (bar0 != sdi->sbwad) {
		/* Need to rebase back plane address window */
		err = bcmsdhsdio_set_sbaddr_window(sdh, bar0);
		if (err)
			return 0xFFFFFFFF;
		sdi->sbwad = bar0;
	}
	addr &= SBSDIO_SB_OFT_ADDR_MASK;

	EnterCriticalSection(&backupsdi->IoLock);

	backupsdi->regfail = FALSE;

	if ( size == 4 ) {
		addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;
	}

	status = sdnow_cmd53_rw_reg(backupsdi, SD_IO_OP_WRITE, fuc, addr, (UCHAR *)&data, size);
	if (status != 0)
		goto fail;

	LeaveCriticalSection(&backupsdi->IoLock);
	return 0;	/* good write */

fail:
	backupsdi->regfail = TRUE;
	if (size == 2) {
		LeaveCriticalSection(&backupsdi->IoLock);
		return 0xffff;
	} else {
		LeaveCriticalSection(&backupsdi->IoLock);
		return 0xffffffff;
	}
}

int
bcmsdh_recv_buf(void *sdh, uint32 addr, uint fn, uint flags,
                uint8 *buf, uint nbytes, void *pkt, bcmsdh_cmplt_fn_t complete, void *handle)
{
	uint status = 0;
	uchar incr_fix;
	uint32 i, j, bar0;
	uint32 num_transfer, blocksize;
	uint width;
	SD_DEVICE_HANDLE hDevice;
	int err = 0;
	
	SD_API_STATUS          sdstatus;
	DWORD                  argument;
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;

	/* Async not supported */
	ASSERT(!(flags & SDIO_REQ_ASYNC));

	if (flags & SDIO_REQ_ASYNC)
		return BCME_UNSUPPORTED;

	ASSERT(sdi);

	if (sdi->CardEjected == TRUE)
		return BCME_ERROR;

	bar0 = addr & ~SBSDIO_SB_OFT_ADDR_MASK;
	if (bar0 != sdi->sbwad) {
		err = bcmsdhsdio_set_sbaddr_window(sdh, bar0);
		if (err)
			return err;
		sdi->sbwad = bar0;
	}
	addr &= SBSDIO_SB_OFT_ADDR_MASK;

	EnterCriticalSection(&sdi->IoLock);

	incr_fix = (flags & SDIO_REQ_FIXED) ? SD_IO_FIXED_ADDRESS : SD_IO_INCREMENT_ADDRESS;
	width = (flags & SDIO_REQ_4BYTE) ? 4 : 2;
	if ( width == 4 ) {
		addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;
	}
	if (fn == SDIO_FUNC_2 && sdi->hDevicefun2) {
		blocksize = BLOCK_TRANSFER_LENGTH_FUNC_2;
		hDevice = sdi->hDevicefun2;
	}
	else
	{
		blocksize = BLOCK_TRANSFER_LENGTH_FUNC_1;
		hDevice = sdi->hDevice;
	}
	num_transfer = nbytes / blocksize;
	
	if (num_transfer > 0) {
		ASSERT(flags & SDIO_REQ_FIXED);
		argument = BUILD_IO_RW_EXTENDED_ARG(SD_IO_OP_READ, 
			    SD_IO_BLOCK_MODE,	
			    fn, 
			    addr,
			    incr_fix,
			    num_transfer);
		sdstatus = SDSynchronousBusRequest(hDevice, 
				SD_CMD_IO_RW_EXTENDED,
				argument,
				SD_READ, 
				ResponseR5,
				NULL, 		/* response holder */
				num_transfer,		
				blocksize,	/* data size */
				buf,
				0);			/* no flags */
		if (!SD_API_SUCCESS(sdstatus)) {
			SD_ERROR(("Block mode read failed error code 0x%X\n",sdstatus));
			LeaveCriticalSection(&sdi->IoLock);
			return BCME_SDIO_ERROR;
		}

		buf += (blocksize * num_transfer);	
	} 
	
	/* if there are more bytes left, get them */
	i = nbytes % blocksize; 
	if (i) {
		j = (i % 2) ? (i + 1) : i;
		argument = BUILD_IO_RW_EXTENDED_ARG(SD_IO_OP_READ, 
			SD_IO_BYTE_MODE,	/* always byte mode */
			fn, 
			addr,
			incr_fix,		/* fix address or increment */
			j);			/* for 512 bytes, set to 0 */
		sdstatus = SDSynchronousBusRequest(hDevice, 
			SD_CMD_IO_RW_EXTENDED,
			argument,
			SD_READ, 
			ResponseR5,
			NULL, 			/* response holder */
			1,			/* NumBlock, always 1 for byte mode */
			j,			/* data size */
			buf,
			0);			/* no flags */
		if (!SD_API_SUCCESS(sdstatus)) {
			LeaveCriticalSection(&sdi->IoLock);
			return BCME_SDIO_ERROR;
		}
	}
	
	LeaveCriticalSection(&sdi->IoLock);
	return 0;
}

int
bcmsdh_send_buf(void *sdh, uint32 addr, uint fn, uint flags,
                uint8 *buf, uint nbytes, void *pkt, bcmsdh_cmplt_fn_t complete, void *handle)
{
	uint status = 0;
	uchar incr_fix;
	uint32 i, bar0;
	uint32 num_transfer, blocksize;
	uint width;
	SD_DEVICE_HANDLE hDevice;
	int err = 0;

	SD_API_STATUS		   sdstatus;
	DWORD				   argument;
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;

	/* Async not supported */
	ASSERT(!(flags & SDIO_REQ_ASYNC));
	if (flags & SDIO_REQ_ASYNC)
		return BCME_UNSUPPORTED;

	ASSERT(sdi);

	if (sdi->CardEjected == TRUE)
		return BCME_ERROR;

	bar0 = addr & ~SBSDIO_SB_OFT_ADDR_MASK;
	if (bar0 != sdi->sbwad) {
		err = bcmsdhsdio_set_sbaddr_window(sdh, bar0);
		if (err)
			return err;
		sdi->sbwad = bar0;
	}
	addr &= SBSDIO_SB_OFT_ADDR_MASK;

	EnterCriticalSection(&sdi->IoLock);

	width = (flags & SDIO_REQ_4BYTE) ? 4 :2;
	if ( width == 4 ) {
		addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;
	}
	incr_fix = (flags & SDIO_REQ_FIXED) ? SD_IO_FIXED_ADDRESS : SD_IO_INCREMENT_ADDRESS;
	if (fn == SDIO_FUNC_2 && sdi->hDevicefun2) {
		blocksize = BLOCK_TRANSFER_LENGTH_FUNC_2;
		hDevice = sdi->hDevicefun2;
	}
	else
	{
		blocksize = BLOCK_TRANSFER_LENGTH_FUNC_1;
		hDevice = sdi->hDevice;
	}
	num_transfer = nbytes / blocksize;
	
	if (num_transfer > 0) {
		ASSERT(flags & SDIO_REQ_FIXED);
		argument = BUILD_IO_RW_EXTENDED_ARG(SD_IO_OP_WRITE, 
			    SD_IO_BLOCK_MODE,	
			    fn, 
			    addr,
			    incr_fix,	
			    num_transfer);
		sdstatus = SDSynchronousBusRequest(hDevice, 
			SD_CMD_IO_RW_EXTENDED,
			argument,
			SD_WRITE, 
			ResponseR5,
			NULL,	 
			num_transfer,
			blocksize,	
			buf,
			0);
		if (!SD_API_SUCCESS(sdstatus)) {
			SD_ERROR(("bcmsdh_send_buf block write failed error 0x%X\n",sdstatus));
			LeaveCriticalSection(&sdi->IoLock);
			return BCME_SDIO_ERROR;
		}
		else
		buf += (blocksize * num_transfer);
	}
	
	/* if there are more bytes left, send it */
	i = nbytes % blocksize;
	if (i) {
		argument = BUILD_IO_RW_EXTENDED_ARG(SD_IO_OP_WRITE, 
				SD_IO_BYTE_MODE,	
				fn, 
				addr,
				incr_fix,	
				i); 
		sdstatus = SDSynchronousBusRequest(hDevice, 
				SD_CMD_IO_RW_EXTENDED,
				argument,
				SD_WRITE,
				ResponseR5,
				NULL,	/* response holder */
				1,		
				i,	/* data size */
				buf,
				0);	/* no flags */
		if (!SD_API_SUCCESS(sdstatus)) {
			LeaveCriticalSection(&sdi->IoLock);
			return BCME_SDIO_ERROR;
		}
	}

	LeaveCriticalSection(&sdi->IoLock);
	return 0;
}

int
bcmsdh_rwdata(void *sdh, uint rw, uint32 addr, uint8 *buf, uint nbytes)
{
	uint status = 0;
	uchar incr_fix;
	uint32 i;
	uint32 num_transfer, blocksize;
	uint width;
	SD_DEVICE_HANDLE hDevice;
	
	SD_API_STATUS		   sdstatus;
	DWORD				   argument;
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;

	if (!sdi)
		sdi = backupsdi;

	ASSERT(sdi);

	if (sdi->CardEjected == TRUE)
		return BCME_ERROR;

	EnterCriticalSection(&sdi->IoLock);

	incr_fix = SD_IO_INCREMENT_ADDRESS;
	width = 4;
	addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;
	blocksize = BLOCK_TRANSFER_LENGTH_FUNC_1;
	num_transfer = nbytes / blocksize;
	hDevice = sdi->hDevice;
	
	if (num_transfer > 0) {
		argument = BUILD_IO_RW_EXTENDED_ARG(rw ? SD_IO_OP_WRITE : SD_IO_OP_READ, 
				SD_IO_BLOCK_MODE,	
				SDIO_FUNC_1, 
				addr,
				incr_fix,
				num_transfer);
		sdstatus = SDSynchronousBusRequest(hDevice, 
				SD_CMD_IO_RW_EXTENDED,
				argument,
				rw ? SD_WRITE : SD_READ, 
				ResponseR5,
				NULL,		/* response holder */
				num_transfer,		
				blocksize,	/* data size */
				buf,
				0); 		/* no flags */
		if (!SD_API_SUCCESS(sdstatus)) {
			SD_ERROR(("Block mode read failed error code 0x%X\n",sdstatus));
			LeaveCriticalSection(&sdi->IoLock);
			return BCME_SDIO_ERROR;
		}

		buf += (blocksize * num_transfer);	
	} 
	
	/* if there are more bytes left, get them */
	i = nbytes % blocksize; 
	addr += nbytes - i;
	if (i) {
		argument = BUILD_IO_RW_EXTENDED_ARG(rw ? SD_IO_OP_WRITE : SD_IO_OP_READ, 
			SD_IO_BYTE_MODE,	/* always byte mode */
			SDIO_FUNC_1, 
			addr,
			incr_fix,		/* fix address or increment */
			i); 		/* for 512 bytes, set to 0 */
		sdstatus = SDSynchronousBusRequest(hDevice, 
			SD_CMD_IO_RW_EXTENDED,
			argument,
			rw ? SD_WRITE : SD_READ, 
			ResponseR5,
			NULL,			/* response holder */
			1,			/* NumBlock, always 1 for byte mode */
			i,			/* data size */
			buf,
			0); 		/* no flags */
		if (!SD_API_SUCCESS(sdstatus)) {
			LeaveCriticalSection(&sdi->IoLock);
			SD_ERROR(("SDSynchronousBusRequest error code 0x%X\n",sdstatus));
			return BCME_SDIO_ERROR;
		}
	}

	LeaveCriticalSection(&sdi->IoLock);

	return 0;
}

int
bcmsdh_iovar_op(void *sdh, const char *name,
                    void *params, int plen, void *arg, int len, bool set)
{ 
	uint32 size = 64;
	const bcm_iovar_t *vi = NULL;
	int bcmerror = 0;
	int val_size;
	int32 int_val = 0;
	uint32 actionid;

	/* Get must have return space; Set does not take qualifiers */
	ASSERT(set || (arg && len));
	ASSERT(!set || (!params && !plen));

	if ((vi = bcm_iovar_lookup(sdioh_iovars, name)) == NULL) {
		bcmerror = BCME_UNSUPPORTED;
		goto exit;
	}

	if ((bcmerror = bcm_iovar_lencheck(vi, arg, len, set)) != 0)
	{
		goto exit;
	}

	/* Set up params so get and set can share the convenience variables */
	if (params == NULL) {
		params = arg;
		plen = len;
	}

	if (vi->type == IOVT_VOID)
		val_size = 0;
	else if (vi->type == IOVT_BUFFER)
		val_size = len;
	else
		val_size = sizeof(int);

	if (plen >= (int)sizeof(int_val))
		bcopy(params, &int_val, sizeof(int_val));

	actionid = set ? IOV_SVAL(vi->varid) : IOV_GVAL(vi->varid);
	switch (actionid) {
	case IOV_GVAL(IOV_BLOCKSIZE):
	{
		SD_HOST_BLOCK_CAPABILITY blockCapability;
		SD_API_STATUS            apiStatus;
		USHORT                   blocks;
		USHORT                   blockLength;
		bcmsdh_info_t	*sdi = (bcmsdh_info_t *)sdh;

		blockLength = (USHORT)size;
		blocks = (USHORT)(0x20000/blockLength);

		blockCapability.ReadBlockSize = blockLength;
		blockCapability.WriteBlockSize = blockLength;
		blockCapability.ReadBlocks  = blocks;
		blockCapability.WriteBlocks = blocks;

		apiStatus = SDCardInfoQuery(sdi->hDevice,
									SD_INFO_HOST_BLOCK_CAPABILITY,
									&blockCapability,
									sizeof(blockCapability));

		if (!SD_API_SUCCESS(apiStatus)) {
				bcmerror = BCME_SDIO_ERROR;
		}
		else {
			int_val = (int)blockCapability.ReadBlockSize;
		}
	}
	if (!bcmerror)
		bcopy(&int_val, arg, val_size);
		break;

	case IOV_GVAL(IOV_DEVREG):
	{
		sdreg_t *sd_regs = (sdreg_t *)params;

        int_val = (int32)bcmsdh_cfg_read(sdh, sd_regs->func, 
			                             SI_ENUM_BASE + sd_regs->offset, &bcmerror);
        if (!bcmerror)
            bcopy(&int_val, arg, sizeof(int32));
        break;
    }

	case IOV_SVAL(IOV_DEVREG):  /* CMD52 8-bit interface */
	{
		int err = 0;
		sdreg_t *sd_regs = (sdreg_t *)params;

		bcmsdh_cfg_write(sdh, sd_regs->func, SI_ENUM_BASE + sd_regs->offset, 
			             sd_regs->value, &err);
		if (0 != err) {
			bcmerror = BCME_SDIO_ERROR;
			break;
		}
		break; 
	}

	default:
		bcmerror = BCME_UNSUPPORTED;
		break;
	}
exit:
	return bcmerror;
}

int
bcmsdh_query_device(void *sdh)
{
	bcmsdh_info_t *bcmsdh = (bcmsdh_info_t *)sdh;
	bcmsdh->vendevid = (VENDOR_BROADCOM << 16) | 0; 

	return (bcmsdh->vendevid);
}


uint
bcmsdh_query_iofnum(void *sdh)
{
	return 1;
}

int
bcmsdh_reset(bcmsdh_info_t *sdh)
{
	return 1;
}

bool
bcmsdh_regfail(void *sdh)
{
	ASSERT(backupsdi);
	return backupsdi->regfail;
}

/* Function to propogate gSPI specific device-status bits to dhd. */
uint32
bcmsdh_get_dstatus(void *sdh)
{
	return 0;
}

void
bcmsdh_chipinfo(void *sdh, uint32 chip, uint32 chiprev)
{
	return;
}

void
bcmsdh_dwordmode(void *sdh, bool set)
{
	return;
}

int
bcmsdh_abort(void *sdh, uint fn)
{
	SD_API_STATUS		   sdstatus;
	DWORD				   argument;
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	SD_DEVICE_HANDLE hDevice;

	if (!sdi)
		sdi = backupsdi;

	ASSERT(sdi);

	EnterCriticalSection(&sdi->IoLock);

	if (fn == SDIO_FUNC_2 && sdi->hDevicefun2)
		hDevice = sdi->hDevicefun2;
	else
		hDevice = sdi->hDevice;

	argument = BUILD_IO_RW_DIRECT_ARG( SD_IO_OP_WRITE,
		SD_IO_RW_NORMAL,	/* Normal */
		SDIO_FUNC_0,
		SD_IO_REG_IO_ABORT, 	/* address	*/
		fn);		/* data */

	sdstatus = SDSynchronousBusRequest(hDevice, 
		SD_IO_RW_DIRECT,
		argument,
		SD_COMMAND, 
		ResponseR5,
		NULL,			/* response holder */
		0,			/* NumBlock, always 1 for byte mode */
		0,			/* data size */
		0,
		0);

	if (!SD_API_SUCCESS(sdstatus)) {
		LeaveCriticalSection(&sdi->IoLock);
		return BCME_SDIO_ERROR;
	}

	LeaveCriticalSection(&sdi->IoLock);
	return 0;
}

bool
bcmsdh_reenumerate(void *sdh)
{
	SD_API_STATUS sdstatus;
	bool retcode = FALSE;
	bcmsdh_info_t *sdi = (bcmsdh_info_t *)sdh;
	SD_COMMAND_RESPONSE response;
	uint status;
	uint8 data;

	if (!sdi) {
		SD_ERROR(("Backup sdi handle is used, backupsdi = 0x%x\n", backupsdi));
		sdi = backupsdi;
	}

	ASSERT(sdi);

	EnterCriticalSection(&sdi->IoLock);

	sdstatus = SDSynchronousBusRequest(sdi->hDevice, SD_CMD_GO_IDLE_STATE, 0x00000000, SD_COMMAND, NoResponse, NULL, 0, 0, NULL, (DWORD)SD_SLOTRESET_REQUEST);
	if (!SD_API_SUCCESS(sdstatus)) {
		SD_ERROR(("CMD 0 failed, during re-enumeration\n"));
		goto error;
	}


	sdstatus = SDSynchronousBusRequest(sdi->hDevice, SD_CMD_IO_OP_COND, 0x00000000, SD_COMMAND, ResponseR4, &response, 0, 0, NULL, (DWORD)0);
	if (!SD_API_SUCCESS(sdstatus)) {
		SD_ERROR(("CMD 5, instance 0 failed, during re-enumeration\n"));
		goto error;
	}

	Sleep(100);

	sdstatus = SDSynchronousBusRequest(sdi->hDevice, SD_CMD_IO_OP_COND, 0x00040000, SD_COMMAND, ResponseR4, &response, 0, 0, NULL, (DWORD)0);
	if (!SD_API_SUCCESS(sdstatus)) {
		SD_ERROR(("CMD 5, instance 1 failed, during re-enumeration\n"));
		goto error;
	}

	sdstatus = SDSynchronousBusRequest(sdi->hDevice, SD_CMD_MMC_SET_RCA, 0x00000000, SD_COMMAND, ResponseR1, &response, 0, 0, NULL, (DWORD)0);
	if (!SD_API_SUCCESS(sdstatus)) {
		SD_ERROR(("CMD 3, instance 1 failed, during re-enumeration\n"));
		goto error;
	}

	sdstatus = SDSynchronousBusRequest(sdi->hDevice, SD_CMD_SELECT_DESELECT_CARD, 0x00010000, SD_COMMAND, ResponseR1b, &response, 0, 0, NULL, (DWORD)0);
	if (!SD_API_SUCCESS(sdstatus)) {
		SD_ERROR(("CMD 7, instance 1 failed, during re-enumeration\n"));
		goto error;
	}

	/* Disable card detect at CCCR register 0x7 */
	/* Enable 4bit mode CCCR register 0x7 */
	data = (BUS_CARD_DETECT_DIS | BUS_SD_DATA_WIDTH_4BIT);
	status = sdnow_cmd52_rw_byte(backupsdi, SD_IO_OP_WRITE, 0, SDIOD_CCCR_BICTRL, &data);
	if (status == BCME_SDIO_ERROR) {
		SD_ERROR(("CCCR reg 0x7 write failed for disable card detect, during re-enumeration\n"));
		SD_ERROR(("CCCR reg 0x7 write failed for 4bit enable, during re-enumeration\n"));
		goto error;
	}

	/* Re initilize the card */
	status = sdnow_setup_card(sdi, sdi->osh);
	if (!SD_API_SUCCESS(status)) {
		SD_ERROR(("BCMSD: Failed to setup card\n", status));
		goto error;
	}

	/* Disable the interrupt on the safer side */
	sdnow_intr_disable(sdi);

	retcode = TRUE;

error:
	LeaveCriticalSection(&sdi->IoLock);
	return retcode;
}

/* Return current fun-1 sb window address */
uint32
bcmsdh_cur_sbwad(void *sdh)
{
	bcmsdh_info_t *bcmsdh = (bcmsdh_info_t *)sdh;
	return bcmsdh->sbwad;
}

/*** sdnow functions */

/*
//  sdnow_slot_event_callback - slot event callback for fast-path events
//  Input:  hDevice - device handle
//          pContext - device specific context that was registered
//          SlotEventType - slot event type
//          pData - Slot event data (can be NULL)
//          DataLength - length of slot event data (can be 0)
//  Notes:  
//
//      If this callback is registered the client driver can be notified of
//      slot events (such as device removal) using a fast path mechanism.  This
//      is useful if a driver must be notified of device removal 
//      before its XXX_Deinit is called.  
//
//      This callback can be called at a high thread priority and should only
//      set flags or set events.  This callback must not perform any
//      bus requests or call any apis that can perform bus requests.
*/
static VOID 
sdnow_slot_event_callback(SD_DEVICE_HANDLE    hDevice,
				PVOID               pContext,                      
				SD_SLOT_EVENT_TYPE  SlotEventType,         
				PVOID               pData,                      
				DWORD               DataLength)
{
	bcmsdh_info_t * sdi;
	SD_ERROR(("sdnow_slot_event_callback reason: %d \n", SlotEventType));
	switch (SlotEventType) {
		case SDCardEjected :
			sdi = (bcmsdh_info_t *)pContext;
			if (!sdi)
				break;
			sdi->CardEjected = TRUE;
			/* call registered device remove handler */
			if (sdi->devremoveh_valid)
				(sdi->dev_remove_handler)(sdi->dev_remove_argh);

			break;
	}
}

/*
//  sdnow_intr_callback - interrupt callback
//  Input:  hDevice - handle to SD device
//          pContext - context that was registered with the device
//  Notes: 
//          The ISR callback attempts to read a register.  The interrupt
//          does not need to be cleared in hardware. 
*/
static SD_API_STATUS 
sdnow_intr_callback(SD_DEVICE_HANDLE hDevice, PVOID pContext)
{
	bcmsdh_info_t *	sdi = (bcmsdh_info_t *)pContext;
	SD_API_STATUS	status;                     
	ULONG		offset = OFFSETOF(sdio_regs_t, intr_status);

	if (!sdi->intr_handler) {
		/* interrupt already deregistered, return without calling handler */
		status = SD_API_STATUS_UNSUCCESSFUL;
	} else {
		sdi->InterruptCount++;
		(sdi->intr_handler)(sdi->intrh_argh);
		status = SD_API_STATUS_SUCCESS;
	}

	return status;
}

static DWORD WINAPI
sdh_oobIST(LPVOID lpParameter)
{
	bcmsdh_info_t * sdi = (bcmsdh_info_t *)lpParameter;
	DWORD dwWait;

	CeSetThreadPriority(sdi->hIST, sdi->ISTPriority);
	SD_ERROR(("%s: IST is launched with priority = %d\n", __FUNCTION__, sdi->ISTPriority));

	if (sdi) {
		while(1) {
			dwWait = WaitForSingleObject(sdi->oobIntrEvent, INFINITE);

            if (dwWait == WAIT_OBJECT_0) {
				sdnow_intr_callback(sdi->hDevice, sdi);
                InterruptDone(sdi->dwSysIntr);
            }
		}
	}else
		SD_ERROR(("%s: sdi is null\n", __FUNCTION__));

	return 0;
}

int
sdh_register_oob_interrupt(bcmsdh_info_t * sdi)
{
	int ret = -1;

	printf("%s : Called\n", __FUNCTION__);
    // Create interrupt event
    sdi->oobIntrEvent = CreateEvent(0, FALSE, FALSE, NULL);
    if (sdi->oobIntrEvent == NULL) {
		SD_ERROR(("%s: failed to create interrupt event\n", __FUNCTION__));
        goto fail;
    }

	// Establish interrupt with event
	if ((InterruptInitialize(sdi->dwSysIntr, sdi->oobIntrEvent, NULL, 0) == FALSE)){
		SD_ERROR(("%s: failed InterruptInitialize on SysIntr = %d\n", __FUNCTION__, sdi->dwSysIntr));
        goto fail;
    }

    sdi->hIST = CreateThread(NULL, 0, sdh_oobIST, sdi, 0, NULL);
    if (sdi->hIST == NULL) {
		SD_ERROR(("%s: failed to create IST\n", __FUNCTION__));
        goto fail;
    }
	ret = 0;

fail:
	return ret;
}

static int
sdnow_intr_disable(bcmsdh_info_t * sdi)
{
	int errcode;

	bcmsdh_cfg_write(sdi, SDIO_FUNC_0, SDIOD_CCCR_INTEN, 0, &errcode);
	if (0 != errcode) {
		SD_ERROR(("BCMSD: interrupt disable failed\n"));
		return BCME_SDIO_ERROR;
	}

	sdi->InterruptEnabled = FALSE;
	return 0;
}

static int
sdnow_intr_enable(bcmsdh_info_t * sdi)
{
	int errcode;

	bcmsdh_cfg_write(sdi, SDIO_FUNC_0, SDIOD_CCCR_INTEN, 7, &errcode);

	if (0 != errcode)  {
		SD_ERROR(("BCMSD: interrupt enable failed\n"));
		return BCME_SDIO_ERROR;
	}

	sdi->InterruptEnabled = TRUE;
	return 0;
}

static SD_API_STATUS 
sdnow_setup_card(bcmsdh_info_t * sdi, osl_t *osh)
{
	SD_IO_FUNCTION_ENABLE_INFO  functionEnable;
	SD_API_STATUS               status;
	SD_CARD_INTERFACE           cardInterface;
	SDIO_CARD_INFO              sdioInfo;
	DWORD                       blockLength;
	sdi->InterruptCount = 0;

	/* query the RCA */
	status = SDCardInfoQuery(sdi->hDevice, SD_INFO_REGISTER_RCA, &sdi->RCA, sizeof(sdi->RCA));
	if (!SD_API_SUCCESS(status)) {
		SD_ERROR(("BCMSD: Failed to query RCA ! 0x%08X  \n",status));
		return status;
	}
	SD_ERROR(("BCMSD: RCA: 0x%04X \n", sdi->RCA));

	/* query the card interface */
	status = SDCardInfoQuery(sdi->hDevice, SD_INFO_CARD_INTERFACE, &cardInterface, sizeof(cardInterface));
	if (!SD_API_SUCCESS(status)) {
		SD_ERROR(("BCMSD: Failed to query interface ! 0x%08X  \n",status));
		return status;
	}

	if (cardInterface.ClockRate == 0) {
		SD_ERROR(("BCMSD: Device interface rate is zero! \n"));
		return SD_API_STATUS_UNSUCCESSFUL;
	}

	SD_ERROR(("BCMSD: Interface Clock : %d Hz WriteProtect %d\n",  cardInterface.ClockRate,cardInterface.WriteProtected));
	
	if (cardInterface.InterfaceMode == SD_INTERFACE_SD_MMC_1BIT) {
		SD_ERROR(("BCMSD: 1 Bit interface mode \n"));
	} else if (cardInterface.InterfaceMode == SD_INTERFACE_SD_4BIT) {
		SD_ERROR(("BCMSD: 4 bit interface mode \n"));
	} else {
		SD_ERROR(("BCMSD: Unknown interface mode! %d \n", cardInterface.InterfaceMode));
		return SD_API_STATUS_UNSUCCESSFUL;
	}

	/* query the SDIO information */
	status = SDCardInfoQuery(sdi->hDevice, SD_INFO_SDIO, &sdioInfo, sizeof(sdioInfo));
	if (!SD_API_SUCCESS(status)) {
		SD_ERROR(("BCMSD: Failed to query SDIO info ! 0x%08X  \n",status));
		return status;
	}

	if (osh) {
		if (sdioInfo.FunctionNumber != 1) {
			SD_ERROR(("BCMSD: Function number %d is incorrect! \n", sdioInfo.FunctionNumber));
			return SD_API_STATUS_UNSUCCESSFUL;
		}

		/* save off function number */
		sdi->Function = sdioInfo.FunctionNumber;
		
		SD_ERROR(("BCMSD: Function: %d \n",  sdi->Function)); 
		SD_ERROR(("BCMSD: Device Code: %d \n",	sdioInfo.DeviceCode)); 
		SD_ERROR(("BCMSD: CISPointer: 0x%08X \n",  sdioInfo.CISPointer)); 
		SD_ERROR(("BCMSD: CSAPointer: 0x%08X \n",  sdioInfo.CSAPointer)); 
		SD_ERROR(("BCMSD: CardCaps: 0x%02X \n",  sdioInfo.CardCapability)); 
		if (sdioInfo.CISPointer == 0) {
			SD_ERROR(("BCMSD: SDIO information is incorrect \n"));
			return SD_API_STATUS_UNSUCCESSFUL;
		}

		sdi->device_code = sdioInfo.DeviceCode;
		sdi->card_cap = sdioInfo.CardCapability;
		sdi->cis_pointer = sdioInfo.CISPointer;
		sdi->csa_pointer = sdioInfo.CSAPointer;

		/* Set the clock rate and bus width based on registry entry */
		if (sdi->sdclockrate)
			cardInterface.ClockRate = sdi->sdclockrate;
		if (sdi->sdbuswidth)
			cardInterface.InterfaceMode = sdi->sdbuswidth == 4  ?  SD_INTERFACE_SD_4BIT : SD_INTERFACE_SD_MMC_1BIT;

		status = SDSetCardFeature(sdi->hDevice, SD_SET_CARD_INTERFACE, &cardInterface, sizeof(cardInterface));
		if (!SD_API_SUCCESS(status)) {
			SD_ERROR(("BCMSD: Failed to set the clock rate to = %d Hz ! 0x%08X  \n", cardInterface.ClockRate, status));
			SD_ERROR(("BCMSD: Failed to set the bus width to = %d ! 0x%08X  \n", sdi->sdbuswidth, status));
		}

		/* enable the I/O function, wait 10 ms inbetween */
		functionEnable.Interval = 10;		//org 100
		functionEnable.ReadyRetryCount = 10;		//org 99
		status = SDSetCardFeature(sdi->hDevice, SD_IO_FUNCTION_ENABLE, &functionEnable, sizeof(functionEnable));
		if (!SD_API_SUCCESS(status)) {
			SD_ERROR(("BCMSD: Failed to enable Function ! 0x%08X  \n",status));
		}
	}

	/* Check for the availability of FastPath and act accordingly based on registry setting */
	status = SDSetCardFeature(sdi->hDevice, SD_IS_FAST_PATH_AVAILABLE, 0, 0);
	if (SD_API_SUCCESS(status)){
		SD_ERROR(("BCMSD: Fast path Supported\n"));

		if (sdi->sdfastpath) {
			status = SDSetCardFeature(sdi->hDevice, SD_FAST_PATH_ENABLE, 0, 0);
			if (!SD_API_SUCCESS(status))
				SD_ERROR(("BCMSD: Failed to enable Fast path ! 0x%08X  \n",status));
			else
				SD_ERROR(("BCMSD: Fast path enabled\n"));
		} else {
			status = SDSetCardFeature(sdi->hDevice, SD_FAST_PATH_DISABLE, 0, 0);
			if (!SD_API_SUCCESS(status))
				SD_ERROR(("BCMSD: Failed to disable Fast path ! 0x%08X  \n",status));
			else
				SD_ERROR(("BCMSD: Fast path disabled\n"));
		}
	} else
		SD_ERROR(("BCMSD: Fast path not Supported ! 0x%08X  \n",status));

   /* enable soft block */
	status = SDSetCardFeature(sdi->hDevice, SD_IS_SOFT_BLOCK_AVAILABLE, 0, 0);
	if (SD_API_SUCCESS(status)){
		SD_ERROR(("BCMSD: Soft Block Supported\n"));

		if (sdi->sdsoftblock) {
			status = SDSetCardFeature(sdi->hDevice, SD_SOFT_BLOCK_FORCE_UTILIZATION, 0, 0);
			if (!SD_API_SUCCESS(status))
				SD_ERROR(("BCMSD: Failed to enable soft block ! 0x%08X  \n",status));
			else
				SD_ERROR(("BCMSD: Soft block enabled\n"));
		} else {
			status = SDSetCardFeature(sdi->hDevice, SD_SOFT_BLOCK_DEFAULT_UTILIZATON, 0, 0);
			if (!SD_API_SUCCESS(status))
				SD_ERROR(("BCMSD: Failed to disable Soft block ! 0x%08X  \n",status));
			else
				SD_ERROR(("BCMSD: Soft block disabled\n"));
		}
	} else
		SD_ERROR(("BCMSD: Soft block not Supported ! 0x%08X  \n",status));	


	/* Enable High speed on device */
	if (sdi->sdforceHS)
		bcmsdh_cfg_write(sdi, SDIO_FUNC_0, SDIOD_CCCR_SPEED_CONTROL, 0x2, NULL);
	/* Set the block size for F2 function */
	bcmsdh_cfg_write(sdi, SDIO_FUNC_0,
	                 SDIOD_FBR_BASE(SDIO_FUNC_2) + SDIOD_FBR_BLKSIZE_0,
	                 BLOCK_TRANSFER_LENGTH_FUNC_1, NULL);
	bcmsdh_cfg_write(sdi, SDIO_FUNC_0,
	                 SDIOD_FBR_BASE(SDIO_FUNC_2) + SDIOD_FBR_BLKSIZE_1,
	                 0, NULL);
	bcmsdh_cfg_write(sdi, SDIO_FUNC_0, SDIOD_CCCR_IOEN, SDIO_FUNC_ENABLE_1, NULL);

	if (sdioInfo.FunctionNumber == 1) 
		blockLength = BLOCK_TRANSFER_LENGTH_FUNC_1;
	else if (sdioInfo.FunctionNumber == 2) 
		blockLength = BLOCK_TRANSFER_LENGTH_FUNC_2;
	else {
		SD_ERROR(("BCMSD: Invalide function no = %d\n", sdioInfo.FunctionNumber));
		return SD_API_STATUS_UNSUCCESSFUL;
	}

	/* set the block length for this function */
	status = SDSetCardFeature(sdi->hDevice, SD_IO_FUNCTION_SET_BLOCK_SIZE, &blockLength, sizeof(blockLength));
	if (!SD_API_SUCCESS(status)) {
		SD_ERROR(("BCMSD: Failed to set Block Length ! 0x%08X  \n", status));
		return status;
	}
	SD_ERROR(("BCMSD: Block Size set to %d bytes \n",  blockLength));

	return SD_API_STATUS_SUCCESS;
}

/*  device cleanup in case of init failure */
static VOID 
sdnow_cleanup(bcmsdh_info_t  *sdi) 
{
	SDCARD_CLIENT_REGISTRATION_INFO clientInfo;
	SD_API_STATUS					sdStatus;

	SD_ERROR(("wl: sdnow_cleaup\n"));

	if (sdi) {
		if (sdi->hDevice) {

			memset(&clientInfo, 0, sizeof(clientInfo));
			sdStatus = SDRegisterClient(sdi->hDevice, sdi, &clientInfo);
			if (!SD_API_SUCCESS(sdStatus))
				SD_ERROR(("BCMSD: Failed to register client : 0x%08X \n", sdStatus));

			if (!sdi->dwSysIntr)
				SDIODisconnectInterrupt(sdi->hDevice);
			SD_ERROR(("BCMSD: Disconnect interrupt \n"));
		}
		sdi->InterruptEnabled = FALSE;

		if (sdi->hCompletionEvent)
			CloseHandle(sdi->hCompletionEvent);
		
		if (sdi->hDevice) {
			DeleteCriticalSection(&sdi->RemovalLock);	/* win32 API */
			DeleteCriticalSection(&sdi->IoLock);	/* win32 API */
		}
	}
}

/*
// sdnow_get_device_handler - Get the SD Device handle
// Inputs:  
// Outputs:
// returns: NDIS_SUCCESS if successful
// Notes:
//          The bus driver loads NDIS and stores the SD Device Handle context
//          in NDIS's device active key.  This function scans the NDIS 
//          configuration for the ActivePath to feed into the
//          SDGetDeviceHandle API.
*/
static NDIS_STATUS 
sdnow_get_device_handler(bcmsdh_info_t * sdi, osl_t *osh)
{
	NDIS_STATUS status;
	NDIS_HANDLE configHandle;
	NDIS_STRING activePathKey = NDIS_STRING_CONST("ActivePath");
	PNDIS_CONFIGURATION_PARAMETER pConfigParm;	
#if defined(NDIS60)
	NDIS_CONFIGURATION_OBJECT configObj;
#endif /* NDIS60 */
	if (osh) {
#if defined(NDIS60)
		SD_ERROR(("sdnow_get_device_handler: %s, calling NdisOpenConfigurationEx() 2 now ....\n", __FUNCTION__));

		bzero(&configObj, sizeof(NDIS_CONFIGURATION_OBJECT));		
		configObj.Flags = 0;
		configObj.Header.Revision = NDIS_CONFIGURATION_OBJECT_REVISION_1;
		configObj.Header.Size = sizeof(NDIS_CONFIGURATION_OBJECT); 
		configObj.Header.Type = NDIS_OBJECT_TYPE_CONFIGURATION_OBJECT;
		configObj.NdisHandle = sdi->ConfigurationHandle;
		status = NdisOpenConfigurationEx(&configObj, &configHandle);

		SD_ERROR(("sdnow_get_device_handler: %s, out of NdisOpenConfigurationEx() 2 now ....\n", __FUNCTION__));
#else
		/* open a handle to our configuration in the registry */
		NdisOpenConfiguration(&status, &configHandle, sdi->ConfigurationHandle);
#endif /* NDIS60 */
		if (!NDIS_SUCCESS(status)) {
			SD_ERROR(("BCMSD: NdisOpenConfiguration failed (0x%08X)\n", status));
			return status;
		}

		/* read the ActivePath key set by the NDIS loader driver */
		NdisReadConfiguration(&status, &pConfigParm, configHandle, &activePathKey, NdisParameterString);

		if (!NDIS_SUCCESS(status)) {
			SD_ERROR(("BCMSD: Failed to get active path key (0x%08X)\n", status));
			/* close our registry configuration */
			NdisCloseConfiguration(configHandle);
			return status;
		}

		if (NdisParameterString != pConfigParm->ParameterType) {
			SD_ERROR(("BCMSD: PARAMETER TYPE NOT STRING !!!\n")); 
			/* close our registry configuration */
			NdisCloseConfiguration(configHandle);
			return status;
		}

		if (pConfigParm->ParameterData.StringData.Length > SDNOW_MAX_ACTIVE_REG_PATH) {
			SD_ERROR(("BCMSD: Active path too long!\n"));
			return NDIS_STATUS_FAILURE;
		}

		/* copy the counted string over */
		memcpy(sdi->ActivePath, pConfigParm->ParameterData.StringData.Buffer, pConfigParm->ParameterData.StringData.Length);

		NdisCloseConfiguration(configHandle);

		SD_ERROR(("BCMSD: Active Path Retrieved: %s \n", sdi->ActivePath));

		/* now get the device handle */
		sdi->hDevice = SDGetDeviceHandle((DWORD)sdi->ActivePath, NULL);

		SD_ERROR(("BCMSD: DeviceHandler: 0x%x\n", sdi->hDevice));
	}
	else
	{
		/* now get the device handle */
		sdi->hDevicefun2 = SDGetDeviceHandle((DWORD)sdi->ActivePath, NULL);

		SD_ERROR(("BCMSD: DeviceHandler: 0x%x\n", sdi->hDevicefun2));
	}
	
	return NDIS_STATUS_SUCCESS;
}

static NDIS_STATUS 
sdnow_init_adapter(bcmsdh_info_t *sdi, osl_t *osh)
{
	NDIS_STATUS                     NdisStatus;
	SDCARD_CLIENT_REGISTRATION_INFO clientInfo;
	SD_API_STATUS                   sdStatus;
	SD_DEVICE_HANDLE				savehDevice;

	SD_ERROR(("BCMSD: sdnow_init_adapter\n"));

	NdisStatus = NDIS_STATUS_FAILURE;

	if (!osh) {
		savehDevice = sdi->hDevice;
		sdi->hDevice = sdi->hDevicefun2;
	}

	if (sdi->hDevice == NULL) {
		SD_ERROR(("hDevice is NULL !\n"));
		goto err;
	}
		
	memset(&clientInfo, 0, sizeof(clientInfo));
	/* set client options and register as a client device */
	_tcscpy(clientInfo.ClientName, TEXT("BCMSD 802.11"));
	/* prohibit bus driver does retries in the case of sync cmd failure */
	/* clientInfo.ClientFlags = SD_CLIENT_HANDLES_RETRY; */
	/* set the event callback */
	if (osh)
		clientInfo.pSlotEventCallBack = sdnow_slot_event_callback;
	sdStatus = SDRegisterClient(sdi->hDevice, sdi, &clientInfo);
	if (!SD_API_SUCCESS(sdStatus)) {
		SD_ERROR(("BCMSD: Failed to register client : 0x%08X \n", sdStatus));
		goto err;
	}

	if (osh) {
		sdi->hCompletionEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	/* win32 API */
		if (NULL == sdi->hCompletionEvent) {
			SD_ERROR(("Failed to create async event \n"));
			sdnow_cleanup(sdi);
			goto err;
		}
	}
	
	sdStatus = sdnow_setup_card(sdi, osh);
	if (!SD_API_SUCCESS(sdStatus)) {
		SD_ERROR(("BCMSD: Failed to setup card\n", sdStatus));
		sdnow_cleanup(sdi);
		goto err;
	}
			
	/* connect the interrupt callback */
	/* The bus driver will enable the interrupt for the function in the
		card's CCCR area prior to returning from this function. 
		
		The interrupt callback is called whenever the device function is 
		interrupting. The bus driver will determine the interrupting function,
		disable the interrupt on the card and call the callback.  Upon return 
		from the callback the bus driver will reenable the interrupt in the
		card's CCCR.
	*/
	/* In SDIO Now! Driver model, Interrupts are handled by host controller driver. 
	Client driver or bus driver do not call InterruptInitialize() to associate one 
	SYSINTR with a event that they want. It is host controller driver who handle 
	interrupts from SD hardware. HC driver may use poll or interrupt to handler 
	SDIO Interrupt, depends on implementations of different host controller driver. 
	(until now, all HC driver use interrupt to handle SDIO Interrupt.)
	
	When a SDIO Interrupt is generated by SDIO card:
		* SDIO card generated SDIO interrupt, DAT1 will be low under Interrupt
		Timing defined in SDIO Spec.
		* Host controller hardware detected SDIO interrupt.
		* Host controller hardware generate Interrupt to OS.
		* OS OAL responses with SYSINTR_xxx.  (It was defined in platform code
		and can not change.)
		* OS set event to wake up IST of host controller driver. (The event is
		set by InterruptInitialize() call in Host controller driver init routine.)
		* Host controller driver IST is waked up and found it is SDIO Interrupt
		* Host controller driver tell bus driver SDIO card is interruptting.
		* Bus driver call Client driver's call back ISR.
		* Client driver ISR send CMD52 to SDIO card to read interrupt status
		register, check if there is interrupt pending, (and send CMD52 again to clear
		interrupt status). SDIO Interrupt is level triggered, so it will
		maintain low level until the source is cleared.
		* Client driver does something else. like SetEvent() in ISR to wake up 
		other blocked thread dpc, which is waiting for events forever.
                * Bus driver's thread priority is 100, modified through registry setting.
	*/

	if (osh) {
		if (sdi->dwSysIntr) {
			sdh_register_oob_interrupt(sdi);
		}else{
			sdStatus = SDIOConnectInterrupt(sdi->hDevice, (PSD_INTERRUPT_CALLBACK)sdnow_intr_callback);
			if (!SD_API_SUCCESS(sdStatus)) {
				SD_ERROR(("BCMSD: Failed to connect interrupt: 0x%08X \n", sdStatus));
				sdnow_cleanup(sdi);
				goto err;
			}
		}
		sdi->InterruptEnabled = TRUE;
	}
	else {
		printf("not found osh...\n");
	}
	NdisStatus = NDIS_STATUS_SUCCESS;

err:
	if (!osh) {
		sdi->hDevice = savehDevice;
	}

	SD_ERROR(("BCMSD : SDIO Card is ready \n"));
	return NdisStatus;
}

/*
//  sdnow_query_block_length - query hardware support for block length
//  Input:  BlockLength - desired block length
//          pClientInfo - this device instance 
//  Return: TRUE if block length supported
//  Notes:  This is limited to query host capability, not target
*/
static bool 
sdnow_query_block_length(bcmsdh_info_t * sdi, uint BlockLength, uint DataLength)
{
    SD_HOST_BLOCK_CAPABILITY blockCapability;
    SD_API_STATUS            apiStatus;
    USHORT                   blocks;
    USHORT                   blockLength;

    blockLength = (USHORT)BlockLength;
    blocks = (USHORT)(DataLength/BlockLength);

    blockCapability.ReadBlockSize = blockLength;
    blockCapability.WriteBlockSize = blockLength;
    blockCapability.ReadBlocks  = blocks;
    blockCapability.WriteBlocks = blocks;

    apiStatus = SDCardInfoQuery(sdi->hDevice,
                                SD_INFO_HOST_BLOCK_CAPABILITY,
                                &blockCapability,
                                sizeof(blockCapability));

    if (!SD_API_SUCCESS(apiStatus)) {
	SD_ERROR(("blocklength %d not supported\n", blockLength));
	return FALSE;
    }

	/* compare original parameters to see if the host controller driver altered it */
    if( (blockCapability.ReadBlockSize != blockLength) ||
        (blockCapability.WriteBlockSize != blockLength) ||
        (blockCapability.ReadBlocks != blocks) ||
        (blockCapability.WriteBlocks != blocks) ) {

         SD_ERROR(("BCMSDIO: Desired Block Capabilities: "));
         SD_ERROR(("Block Length: %d  ", blockLength));
         SD_ERROR(("Blocks : %d  \n", blocks));
         SD_ERROR(("BCMSDIO: Actual Block Capabilities: "));
         SD_ERROR(("Read Block Length: %d , Read Blocks: %d ", 
                blockCapability.ReadBlockSize, 
                blockCapability.ReadBlocks));
         SD_ERROR(("Write Block Length: %d , Write Blocks: %d ", 
                blockCapability.WriteBlockSize, 
                blockCapability.WriteBlocks));
	/* host cannot support these parameters */
        return FALSE;
    }

    return TRUE;
}

/* sync cmd52 1 byte read/write */
static uint 
sdnow_cmd52_rw_byte(bcmsdh_info_t * sdi, ULONG rw, 
			UCHAR func_num, ULONG offset, UCHAR *onebyte) 
{
	
	SD_API_STATUS		status;
	SD_IO_TRANSFER_TYPE	transferType;
	ULONG			num_bytes = 1;

	ASSERT(sdi);

	if (sdi->CardEjected == TRUE)
	{
		printf("%s: CardEjected\n", __FUNCTION__);
		return BCME_ERROR;
	}

	if (rw == SD_IO_OP_WRITE)
		transferType = SD_IO_WRITE;
	else
		transferType = SD_IO_READ;

	/* r/w multiple register(one byte each) */
	status = SDReadWriteRegistersDirect(sdi->hDevice,
						transferType,
						func_num,
						offset,
						SD_IO_RW_NORMAL,
						onebyte,
						num_bytes);
	if (!SD_API_SUCCESS(status)) {
		SD_ERROR(("sdnow_cmd52_rw_byte - func %d addr 0x%x failed 0x%08X \n", 
			  func_num, offset, status));
		return BCME_SDIO_ERROR;
	}

	return 0;
}

/* sync cmd52 N bytes read/write */
static uint 
sdnow_cmd52_rw_buf(bcmsdh_info_t * sdi, ULONG rw, 
			UCHAR func_num, ULONG start_addr, PUCHAR p_buffer, ULONG len) 
{
	
	SD_API_STATUS		status;
	SD_IO_TRANSFER_TYPE	transferType;

	ASSERT(sdi);

	if (sdi->CardEjected == TRUE)
		return BCME_ERROR;

	if (len == 0)
		return BCME_ERROR;

	if (rw == SD_IO_OP_WRITE)
		transferType = SD_IO_WRITE;
	else
		transferType = SD_IO_READ;

	/* r/w multiple register(one byte each) */
	status = SDReadWriteRegistersDirect(sdi->hDevice,
						transferType,
						func_num,
						start_addr,
						SD_IO_RW_NORMAL,
						p_buffer,
						len);
	if (!SD_API_SUCCESS(status)) {
		SD_ERROR(("sdnow_cmd52_rw_buf - failed 0x%08X \n", status));
		return BCME_SDIO_ERROR;
	}

	return 0;
}

/* sync cmd53 1/2/4 bytes read/write */
static uint 
sdnow_cmd53_rw_reg( bcmsdh_info_t * sdi, ULONG rw, 
			UCHAR func_num, ULONG offset, UCHAR *buf, ULONG size) 
{
	SD_API_STATUS          sdstatus;
	SD_TRANSFER_CLASS      transferClass;
	DWORD                  argument;
	ULONG                  bytesToTransfer;

	ASSERT(sdi);

	if ((size != 1) && (size != 2) && (size != 4))
		return BCME_ERROR;

	if (sdi->CardEjected == TRUE)
		return BCME_ERROR;

	bytesToTransfer = size;

	if (rw == SD_IO_OP_WRITE)
		transferClass = SD_WRITE;
	else
		transferClass = SD_READ;

	argument = BUILD_IO_RW_EXTENDED_ARG(rw, 
		SD_IO_BYTE_MODE,	/* always byte mode */
       		func_num, 
               	offset,
               	/* SD_IO_FIXED_ADDRESS,  */
               	SD_IO_INCREMENT_ADDRESS, 
               	bytesToTransfer);
	sdstatus = SDSynchronousBusRequest(sdi->hDevice, 
			SD_CMD_IO_RW_EXTENDED,
			argument,
			transferClass, 
			ResponseR5,
			NULL, 		/* response holder */
			1,		/* NumBlock, always 1 for byte mode */
			bytesToTransfer, 	/* data size */
			buf,
			0);		/* no flags */
	if (!SD_API_SUCCESS(sdstatus)) {
		SD_ERROR(("BCMSD: r/w %d func %d reg 0x%x size %d failed, reason: 0x%08X !!!\n", 
			  rw, func_num, offset, size, sdstatus));
		return BCME_SDIO_ERROR;
	}

	return 0;
}

/* sync cmd53 N bytes read/write
 */
static uint 
sdnow_cmd53_rw_buf(bcmsdh_info_t * sdi, ULONG rw, 
		UCHAR func_num, ULONG start_addr, UCHAR inc_fix, 
		PUCHAR p_buffer, ULONG len) 
{
	SD_API_STATUS          status;
	SD_TRANSFER_CLASS      transferClass;
	DWORD                  argument;

	ASSERT(sdi);
	
	if (sdi->CardEjected == TRUE)
		return BCME_ERROR;

	if (rw == SD_IO_OP_WRITE)
		transferClass = SD_WRITE;
	else
		transferClass = SD_READ;
	argument = BUILD_IO_RW_EXTENDED_ARG(rw, 
		SD_IO_BYTE_MODE,	/* always byte mode */
		func_num, 
		start_addr,
		inc_fix,	/* fix address or increment */
		len);		/* for 512 bytes, set to 0 */
	
	status = SDSynchronousBusRequest(sdi->hDevice, 
		SD_CMD_IO_RW_EXTENDED,
		argument,
		transferClass, 
		ResponseR5,
		NULL,			/* response holder */
		1,			/* NumBlock, always 1 for byte mode */
		len,			/* data size */
		p_buffer,
		0);			/* no flags */
	if (!SD_API_SUCCESS(status))
		return BCME_SDIO_ERROR;
	
	return 0;
}



/************************** NDIS Driver Loader **************************/
/* NDIS does not use Windows CE's ActivateDevice() interface.
 * Hence, this NDIS Driver Loader is created using stream driver interface. 
 * NDL_Init will call NdisRegisterAdapter which creates the actual NDIS instance.
 * i.e. it is a stub driver that acts as a proxy to load the NDIS miniport.  
 * This NDIS Driver Loader can only run when the SDIO bus driver loads this client driver.
 */

#define BCMSD_REG_PATH	TEXT("\\Comm\\BCMSD")

#define MAX_MINIPORT_NAME_PATH  256
#define MAX_NUMBER_OF_ADAPTERS  8
#define LOADER_INSTANCE_KEY TEXT("Instance")

    // miniport instance information
typedef struct _MINIPORT_INSTANCE_INFO {
    WCHAR  MiniportName[MAX_MINIPORT_NAME_PATH];
    WCHAR  MiniportInstance[MAX_MINIPORT_NAME_PATH];
    WCHAR  RegPath[MAX_MINIPORT_NAME_PATH];
    WCHAR  ActiveKeyPath[MAX_MINIPORT_NAME_PATH];
    ULONG  InstanceNumber;
}MINIPORT_INSTANCE_INFO, *PMINIPORT_INSTANCE_INFO;

BOOL AllocatedInstance[MAX_NUMBER_OF_ADAPTERS];
CRITICAL_SECTION	LoaderCriticalSection;

/*
//  LoaderEntry - init loader
//  Input:  hInstance - the instance that is attaching
//          Reason - the reason for attaching
//          pReserved - not much
//  Output: 
//  Return: Always returns TRUE
//  Notes:  this is only used to initialize the zones
*/
BOOL 
LoaderEntry(HINSTANCE  hInstance, ULONG Reason, LPVOID     pReserved)
{
    if ( Reason == DLL_PROCESS_ATTACH ) {
        InitializeCriticalSection(&LoaderCriticalSection);
        memset(&AllocatedInstance,0, sizeof(AllocatedInstance));
    }

    if ( Reason == DLL_PROCESS_DETACH ) {
        DeleteCriticalSection(&LoaderCriticalSection);
    }
    
    return(TRUE);
}

/*
//  DllMain - the main dll entry point
//  Input:  hInstance - the instance that is attaching
//          Reason - the reason for attaching
//          pReserved - not much
//  Output:
//  Return: TRUE
//  Notes:  this is only used to initialize the zones
*/
BOOL WINAPI
DllMain(HINSTANCE  hInstance, ULONG Reason, LPVOID pReserved)
{
	if ( Reason == DLL_PROCESS_ATTACH ) {
		SD_DEBUG_ZONE_REGISTER(hInstance, BCMSD_REG_PATH);
		SDInitializeMemoryTagging(2, TEXT("BCMSDIODriver"));
		SD_ERROR(("BCMSDLDR: DllMain, Reason: attach\n"));
#if (CE_MAJOR_VER >= 0x0005)
		SDInitializeCardLib();
#endif /* (CE_MAJOR_VER >= 0x0005) */
	}

	if ( Reason == DLL_PROCESS_DETACH ) {
#if (CE_MAJOR_VER >= 0x0005)
		SDDeinitializeCardLib();
#endif /* (CE_MAJOR_VER >= 0x0005) */
		SD_ERROR(("BCMSDLDR: DllMain, Reason: detach\n"));
		SDCheckMemoryTags(NULL);
		SDDeleteMemoryTagging();
	}

	return LoaderEntry(hInstance,Reason, pReserved);
}

/*
//  LoadMiniport - load the miniport for this instance
//  Input:  pInstance - information for this instance
//  Output: 
//  Return: TRUE if successful
//  Notes:  
*/
static BOOL 
LoadMiniport(PMINIPORT_INSTANCE_INFO pInstance)
{
    HKEY     hKey;              // registry key
    DWORD    win32Status;       // status  
    DWORD    dataSize;          // data size for query
    WCHAR    stringBuff[128];   // string buffer
    WCHAR    instanceKey[32];   // instance name
    WCHAR    instanceNumber[10];    // instance number
    WCHAR    *token;                // tokenizer
    NDIS_STATUS NdisStatus;         // ndis status

    SD_ERROR(("%s: Enter\n", __FUNCTION__));

   // smlee : pInstance->RegPath = "\Drivers\SDCARD\ClientDrivers\Custom\MANF-02D0-CARDID-4329-FUNC-1"
   // open the registry path for this instance
    if ((win32Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                    pInstance->RegPath,
                                    0,
                                    KEY_ALL_ACCESS,
                                    &hKey)) != ERROR_SUCCESS) {
        SD_ERROR(("BCMSDLDR:Failed to open path %s; %d \n", pInstance->RegPath, win32Status));
        return FALSE;
    }

    dataSize = sizeof(stringBuff);

        // build up the instance key
    wcscpy(instanceKey,LOADER_INSTANCE_KEY);
    _ultow(pInstance->InstanceNumber, instanceNumber, 10);
    wcscat(instanceKey, instanceNumber);

    // smlee : "stringBuff" is null at this point
    // retrieve the real reg path to the device parameters
    if (RegQueryValueEx(hKey, 
                        instanceKey, 
                        0, 
                        NULL, 
                        (PUCHAR)stringBuff, 
                        &dataSize) != ERROR_SUCCESS) {
        SD_ERROR(("BCMSDLDR: Failed to get the instance key : %d \n", instanceKey));
        RegCloseKey(hKey);
        return FALSE;
    }

    // don't need this anymore
    RegCloseKey(hKey);

    // smlee : stringBuff = "BCMSDDHD:BCMSDDHD1"
    SD_ERROR(("BCMSDLDR: Tokenizing instance information: %s \n", stringBuff));

        // extract the miniport name and instance name, in the form of "<Miniport Name>:<Miniport Instance>
    token = wcstok(stringBuff, TEXT(":"));

    if (token != NULL) {

        wcscpy(pInstance->MiniportName, token);
        
            // search for the next one
        token = wcstok( NULL, TEXT(":"));

        if (token != NULL) {
            wcscpy(pInstance->MiniportInstance, token);
        } else {
             SD_ERROR(("BCMSDLDR: Failed to get miniport instance \n"));
             return FALSE;
        }

    } else {
        SD_ERROR(("BCMSDLDR: Failed to get miniport name \n"));
        return FALSE;
    }
   
        // build up the miniport instance path in order to stick in the "ActivePath" key
    wcscpy(stringBuff,TEXT("\\Comm\\"));
    wcscat(stringBuff,pInstance->MiniportInstance);
    wcscat(stringBuff,TEXT("\\Parms"));

    // smlee : stringBuff = "\Comm\BCMSDDHD1\Parms"
    SD_ERROR(("BCMSDLDR: Miniport instance path %s\n", stringBuff));

    /* stringBuff == \Comm\BCMSDDHD1\Parms */
   
    if ((win32Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                    stringBuff,
                                    0,
                                    KEY_ALL_ACCESS,
                                    &hKey)) != ERROR_SUCCESS) {
	SD_ERROR(("BCMSDLDR:Failed to open path %s; %d \n", stringBuff, win32Status));
	return FALSE;
    }

    SD_ERROR(("BCMSDLDR: delete ActivePath first\n"));
    RegDeleteValue(hKey, TEXT("ActivePath")); 

    // smlee : pInstance->ActiveKeyPath = "Drivers\Active\71"
    SD_ERROR(("BCMSDLDR: Storing ActiveKey Path %s\n", pInstance->ActiveKeyPath));
	/* save the active ActivePath in the registry path for the miniport.  
	The miniport portion will look up this key
        e.g. \HKLM\Comm\BCMSDDHD1\Parms\ActivePath = Drivers\Active\26
        */
	
    if (RegSetValueEx(hKey, 
                      TEXT("ActivePath"), 
                      0, 
                      REG_SZ, 
                      (PUCHAR)pInstance->ActiveKeyPath, 
                      ((sizeof(WCHAR))*(wcslen(pInstance->ActiveKeyPath) + 1))) != ERROR_SUCCESS) { 
        SD_ERROR(("BCMSDLDR: Failed to set ActiveKey path \n"));
        RegCloseKey(hKey);
        return FALSE;
    }
    RegCloseKey(hKey);

    // smlee : pInstance->ActiveKeyPath = "Drivers\Active\71"

    // smlee : pInstance->MiniportName = "BCMSDDHD"
    /* register the adapter, invoke NDIS driverentry --> */
    NdisRegisterAdapter(&NdisStatus,
                        pInstance->MiniportName,
                        pInstance->MiniportInstance);

	/* make sure the key is deleted last after miniport initialization */
    if ((win32Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				stringBuff,
				0,
				KEY_ALL_ACCESS,
				&hKey)) == ERROR_SUCCESS) {
	/* smlee : pInstance->ActiveKeyPath = Drivers\Active\71 */
	SD_ERROR(("BCMSDLDR: Remove ActivePath last %s\n", pInstance->ActiveKeyPath));
	RegDeleteValue(hKey, TEXT("ActivePath")); 
	RegCloseKey(hKey);
    }

    if (!NDIS_SUCCESS(NdisStatus))
	return FALSE;

#ifdef P2P
        /* build up the miniport instance path for 2nd instance in order to stick in the "ActivePath" key */
    wcscpy(stringBuff,TEXT("\\Comm\\"));
    wcscat(stringBuff,TEXT("BCMSDDHD2"));
    wcscat(stringBuff,TEXT("\\Parms"));

    SD_ERROR(("BCMSDLDR: Miniport instance path %s\n", stringBuff));

	/* stringBuff == \Comm\BCMSDDHD2\Parms */
   
    if ((win32Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                    stringBuff,
                                    0,
                                    KEY_ALL_ACCESS,
                                    &hKey)) != ERROR_SUCCESS) {
	SD_ERROR(("BCMSDLDR:Failed to open path %s; %d \n", stringBuff, win32Status));
	return FALSE;
    }

	SD_ERROR(("BCMSDLDR: delete ActivePath first\n"));
    RegDeleteValue(hKey, TEXT("ActivePath")); 

    SD_ERROR(("BCMSDLDR: Storing ActiveKey Path %s\n", pInstance->ActiveKeyPath));

	/* save the active ActivePath in the registry path for the miniport.  
	The miniport portion will look up this key */
	
    if (RegSetValueEx(hKey, 
                      TEXT("ActivePath"), 
                      0, 
                      REG_SZ, 
                      (PUCHAR)pInstance->ActiveKeyPath, 
                      ((sizeof(WCHAR))*(wcslen(pInstance->ActiveKeyPath) + 1))) != ERROR_SUCCESS) { 
        SD_ERROR(("BCMSDLDR: Failed to set ActiveKey path \n"));
        RegCloseKey(hKey);
        return FALSE;
    }

	/* register the adapter, invoke NDIS driverentry --> */

	NdisRegisterAdapter(&NdisStatus,
                        pInstance->MiniportName,
                        TEXT("BCMSDDHD2"));


	RegCloseKey(hKey);

	/* make sure the key is deleted last after miniport initialization */
    if ((win32Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				stringBuff,
				0,
				KEY_ALL_ACCESS,
				&hKey)) == ERROR_SUCCESS) {
	SD_ERROR(("BCMSDLDR: Remove ActivePath last %s\n", pInstance->ActiveKeyPath));
	RegDeleteValue(hKey, TEXT("ActivePath")); 
	RegCloseKey(hKey);
    }


    if (!NDIS_SUCCESS(NdisStatus))
	return FALSE;
#endif /* P2P */

    return TRUE;
}

/*
//  UnloadMiniport - unload the miniport
//  Input:   pInstance - the instance to unload
//  Output: 
//  Return: 
//  Notes:  
*/
static VOID 
UnloadMiniport(PMINIPORT_INSTANCE_INFO pInstance)
{
    NDIS_STATUS NdisStatus;

    SD_ERROR(("BCMSDLDR: Unloading Miniport Instance %s \n", pInstance->MiniportInstance));
   
    NdisDeregisterAdapter(&NdisStatus, pInstance->MiniportInstance);

    SD_ERROR(("BCMSDLDR: Miniport Unloaded 0x%08X \n", NdisStatus));
}

/*
//  NDL_Deinit - the deinit entry point for this driver
//  Input:  hDeviceContext - the context returned from NDL_Init
//  Output: 
//  Returns: always returns TRUE.  
//  Notes:  
*/
__declspec(dllexport)
BOOL 
NDL_Deinit(DWORD hDeviceContext)
{
    PMINIPORT_INSTANCE_INFO pInstance;     /* device instance */

    SD_ERROR(("BCMSDLDR: +NDL_Deinit\n"));

    pInstance = (PMINIPORT_INSTANCE_INFO)hDeviceContext;

    UnloadMiniport(pInstance);

    EnterCriticalSection(&LoaderCriticalSection);

	/* free our instance number */
    AllocatedInstance[pInstance->InstanceNumber] = FALSE;
    
    LeaveCriticalSection(&LoaderCriticalSection);

    LocalFree(pInstance);

    SD_ERROR(("BCMSDLDR: -NDL_Deinit\n"));

	g_IsDriverLoaded = FALSE;

    return TRUE;
}

void LaunchNDIS(LPVOID lpInput)
{
	PMINIPORT_INSTANCE_INFO pInstance = (PMINIPORT_INSTANCE_INFO)lpInput;

	if (!LoadMiniport(pInstance))
		SD_ERROR(("BCMSDLDR: -LaunchNDIS failed !\n"));
}

/*
//  NDL_Init - the init entry point
//  Input:  dwContext - the context for this init
//  Output: 
//  Return: returns a non-zero context
//  Notes:  
*/
__declspec(dllexport) 
DWORD 
NDL_Init(DWORD dwContext)
{
    PMINIPORT_INSTANCE_INFO         pInstance;      /* this instance of the device */
    ULONG                           ii;
	NDIS_STATUS						status;

    SD_ERROR(("BCMSDLDR: +NDL_Init -- g_IsDriverLoaded = %d\n", g_IsDriverLoaded));

    pInstance = (PMINIPORT_INSTANCE_INFO)
        LocalAlloc(LPTR, sizeof(MINIPORT_INSTANCE_INFO));

    if (pInstance == NULL) {
        SD_ERROR(("BCMSDLDR: Failed to allocate device info \n"));
        SD_ERROR(("BCMSDLDR: -NDL_Init\n"));
        return 0;
    }

   /* smlee : dwContext = "Drivers\Active\71" */

	/* on CE, the dwContext is a pointer to a string to the "Active" registry path
		we pass this to the NDIS driver */
    wcscpy(pInstance->ActiveKeyPath, (PWCHAR)dwContext);

   /* smlee : pInstance->ActiveKeyPath = "Drivers\Active\71", which includes following values
        Hnd : 0xXXXXXXX
        Name : "NDL1:"
        Key : "\Drivers\SDCARD\ClientDrivers\Custom\MANF-02D0-CARDID-4329-FUNC-1"
        BusParent : 0xXXXXXXX
        InterfaceType : 0
        BusName : SDBus_0_0_1
        ClientInfo : ie. 0x27900 */
 
    if (SDGetRegPathFromInitContext((PWCHAR)dwContext, 
                                    pInstance->RegPath, 
                                    sizeof(pInstance->RegPath)) != ERROR_SUCCESS) {
        SD_ERROR(("BCMSDLDR: Failed to get reg path \n"));
        LocalFree(pInstance);
        return 0;
    }

    EnterCriticalSection(&LoaderCriticalSection);

	/* walk through the array and find an open instance number, mark it used */
    for (ii = 0; ii < MAX_NUMBER_OF_ADAPTERS; ii++) {
        if (AllocatedInstance[ii] == FALSE) {
            AllocatedInstance[ii] = TRUE;
            pInstance->InstanceNumber = ii;
            break;
        }
    }

	if (g_IsDriverLoaded == FALSE) {
		g_IsDriverLoaded = TRUE;
		/*
		 * In case of dongle DHD driver, counts on device interrupt being in place to 
		 * complete the initiliztion successfully. The SDIO stack doesn't fully enable
		 * the device interrupt unless NDL_Init call returns back to SDIO stack. So
		 * we defer the initilization of device by launching thread and performing 
		 * required device initilation of DHD drive
		 */
		printf("%s: CreateThread[LanunchNDIS]\n", __FUNCTION__);
		CreateThread(NULL,		/* security atributes */
						0,		/* initial stack size */
						(LPTHREAD_START_ROUTINE)LaunchNDIS,	/* Main() function */
						pInstance, /* arg to reader thread */
						0,		/* creation flags */
						0); /* returned thread id */
	}
	else
	{
		/*
		 * Since our 4328 dongle device had two functions and we decided to 
		 * register both the functions in registry to make use of different
		 * block size for each of the function independently. This driver 
		 * entry will be called twice back to back. This dealy will allows us
		 * to finish initilizing the first function completly, before we procced 
		 * with the next function which counts on first function initilized properly
		 */
		Sleep(500);
		
		if (backupsdi) {
			wcscpy(backupsdi->ActivePath, (PWCHAR)dwContext);

			/* get the device handler from ActivePath registry */
			status = sdnow_get_device_handler(backupsdi, 0);
			if (!NDIS_SUCCESS(status)) {
				SD_ERROR(("BCMSD: No device Handle!\n"));

				return 0;
			}

			/* initialize the card and register interrupt handler */
			status = sdnow_init_adapter(backupsdi, 0);
			if (!NDIS_SUCCESS(status)) {
				SD_ERROR(("BCMSD: BCMSD: InitializeAdapter failed (0x%08X)\n", status));
                
				return 0;
			}
		}
	}

    LeaveCriticalSection(&LoaderCriticalSection);

    if (ii >= MAX_NUMBER_OF_ADAPTERS) {
         SD_ERROR(("BCMSDLDR: Max instances exceeded \n"));
         LocalFree(pInstance);
         return 0;
    }

    SD_ERROR(("BCMSDLDR: -NDL_Init succeeded\n"));

    return (DWORD)pInstance;
}


/*
//  NDL_IOControl - the I/O control entry point 
//  Input:  Handle - the context returned from NDL_Open
//          IoctlCode - the ioctl code
//          pInBuf - the input buffer from the user
//          InBufSize - the length of the input buffer
//          pOutBuf - the output buffer from the user
//          InBufSize - the length of the output buffer
//          pBytesReturned - the size of the transfer
//  Output: 
//  Return:  TRUE if Ioctl was handled
//  Notes:   Not used
*/
__declspec(dllexport)
BOOL 
NDL_IOControl(DWORD   Handle, DWORD   IoctlCode, PBYTE   pInBuf, DWORD   InBufSize,
                   PBYTE   pOutBuf, DWORD   OutBufSize, PDWORD  pBytesReturned)
{
	BOOL ret = FALSE;

	if (IoctlCode == IOCTL_WLAN_ON) {
		wl_pnp_set_power(backupsdi->wl, NdisDeviceStateD0);
		ret = TRUE;
	}else if (IoctlCode == IOCTL_WLAN_OFF) {
		wl_pnp_set_power(backupsdi->wl, NdisDeviceStateD3);
		ret = TRUE;
	}

    return ret;
}

__declspec(dllexport)
DWORD 
NDL_Open(DWORD    hDeviceContext, DWORD    AccessCode, DWORD    ShareMode)
{
    return hDeviceContext;
}

__declspec(dllexport)
void 
NDL_PowerDown(DWORD    hDeviceContext)
{
    return;
}

__declspec(dllexport)
void 
NDL_PowerUp(DWORD  hDeviceContext)
{
    return;
}

__declspec(dllexport)
DWORD 
NDL_Read(DWORD    hOpenContext, LPVOID   pBuffer, DWORD    Count)
{
    return 0;
}

__declspec(dllexport)
DWORD 
NDL_Seek(DWORD    hOpenContext, long     Amount, DWORD    Type)
{   
    return 0;
}

__declspec(dllexport)
DWORD 
NDL_Write(DWORD   hOpenContext, LPCVOID pBuffer, DWORD   Count)
{
    return 0;
}

__declspec(dllexport)
BOOL 
NDL_Close(DWORD hOpenContext)
{
    return TRUE;
}

#if 1 // Chris.Jeong 
void sdnow_init_Interrupt(bcmsdh_info_t *sdi)
{
           SD_API_STATUS                   sdStatus;
//         SDCARD_CLIENT_REGISTRATION_INFO clientInfo;


//         memset(&clientInfo, 0, sizeof(clientInfo));
//         /* set client options and register as a client device */
//         _tcscpy(clientInfo.ClientName, TEXT("BCMSD 802.11"));

//         clientInfo.pSlotEventCallBack = sdnow_slot_event_callback;

//         sdStatus = SDRegisterClient(sdi->hDevice, sdi, &clientInfo);
//         if (!SD_API_SUCCESS(sdStatus)) {
//                   SD_ERROR(("BCMSD: Failed to register client : 0x%08X \n", sdStatus));
//                   printf(("BCMSD: Failed to register client : 0x%08X \n", sdStatus));
//         }
//

           if (sdi->dwSysIntr) {
                     sdh_register_oob_interrupt(sdi);
           }else{
                     SDIODisconnectInterrupt(sdi->hDevice);
                     Sleep(100);
                     sdStatus = SDIOConnectInterrupt(sdi->hDevice, (PSD_INTERRUPT_CALLBACK)sdnow_intr_callback);
                     if (!SD_API_SUCCESS(sdStatus)) {
                                SD_ERROR(("BCMSD: Failed to connect interrupt: 0x%08X \n", sdStatus));
                                sdnow_cleanup(sdi);
                                return;
                     }
           }
           sdi->InterruptEnabled = TRUE;

}

#endif

