/**
	
*/
#include <windows.h>
#include <ndis.h>

#include <athdefs.h>
#include "a_types.h"

#include "ar6k_drv.h"
#include "wmi_api.h"
#include "a_osapi.h"

#include "ndisnetbuf.h"
#include "ar6000_api.h"

#define _P	L"\r[AR6K_DRV] "L

typedef enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0
}Ar6kDebug;

int debugdriver = 0;

static NDIS_SPIN_LOCK m_Lock;

static int refClock = 26000000; 
int regCode = 0x0; 

int bmienable = 1;
int work_mode = 0;
int chan_num = 0;
int txpwr = 0;

unsigned int bypasswmi = 0;


static unsigned int processDot11Hdr = 0;


static unsigned int enableuartprint = 0;
unsigned int enabletimerwar = 0;
unsigned int fwmode = 1;
unsigned int mbox_yield_limit = 99;


unsigned int wlanNodeCaching = 1;
unsigned int logWmiRawMsgs = 0;



static int reduce_credit_dribble = 1 + HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_ONE_HALF;


#define AR6000_SPIN_LOCK()		NdisAcquireSpinLock(&m_Lock)
#define AR6000_SPIN_UNLOCK()	NdisReleaseSpinLock(&m_Lock)


static struct ar_cookie s_ar_cookie_mem[MAX_COOKIE_NUM];


#define HOST_INTEREST_ITEM_ADDRESS(ar, item) \
        (((ar)->arTargetType == TARGET_TYPE_AR6001) ? AR6001_HOST_INTEREST_ITEM_ADDRESS(item) : \
        (((ar)->arTargetType == TARGET_TYPE_AR6002) ? AR6002_HOST_INTEREST_ITEM_ADDRESS(item) : \
        (((ar)->arTargetType == TARGET_TYPE_AR6003) ? AR6003_HOST_INTEREST_ITEM_ADDRESS(item) : 0)))
        
#if 0

#define AR6000_SPIN_LOCK(lock, param)   do {                            \
    if (irqs_disabled()) {                                              \
        AR_DEBUG_PRINTF("IRQs disabled:AR6000_LOCK\n");                 \
    }                                                                   \
    spin_lock_bh(lock);                                                 \
} while (0)

#define AR6000_SPIN_UNLOCK(lock, param) do {                            \
    if (irqs_disabled()) {                                              \
        AR_DEBUG_PRINTF("IRQs disabled: AR6000_UNLOCK\n");              \
    }                                                                   \
    spin_unlock_bh(lock);                                               \
} while (0)
#endif


extern void eeprom_ar6000_transfer(HIF_DEVICE *device, char *fake_file, char *p_mac, int regcode, char *c_mac);


int ar6000_init(AR_SOFTC_T *ar);

static void ar6000_tx_complete(void *Context, HTC_PACKET *pPacket);
static void ar6000_rx(void *Context, HTC_PACKET *pPacket);
//static void ar6000_rx_refill(void *Context, HTC_ENDPOINT_ID Endpoint);
static HTC_SEND_FULL_ACTION ar6000_tx_queue_full(void *Context, HTC_PACKET *pPacket);
static void ar6000_cookie_init(AR_SOFTC_T *ar);
static void ar6000_cookie_cleanup(AR_SOFTC_T *ar);
void ar6000_free_cookie(AR_SOFTC_T *ar, struct ar_cookie * cookie);
static void deliver_frames_to_nw_stack(AR_SOFTC_T *ar, struct ndis_mini_buf_t *skb);
static struct ar_cookie *ar6000_alloc_cookie(AR_SOFTC_T *ar);
static int checkforDHCPPacket(struct ndis_mini_buf_ *skb) ;
static void ar6000_detect_error(void *ptr);



static A_STATUS
ar6000_configure_target(AR_SOFTC_T *ar)
{
	A_UINT32 param;

	RETAILMSG(DBG_TRACE, (_P"ar6000_configure_target() + Enter \r\n"));
	if (enableuartprint) 
	{
		param = 1;
		if (BMIWriteMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_serial_enable),
							(A_UCHAR *)&param,
							4)!= A_OK)
		{
			RETAILMSG(DBG_ERR, (_P"BMIWriteMemory for enableuartprint failed \n"));
			return A_ERROR;
        }
		
		RETAILMSG(DBG_TRACE, (_P"Serial console prints enabled\n"));
    }

	/* Tell target which HTC version it is used*/
	param = HTC_PROTOCOL_VERSION;
	if (BMIWriteMemory(ar->arHifDevice,
						HOST_INTEREST_ITEM_ADDRESS(ar, hi_app_host_interest),
						(A_UCHAR *)&param,
						4)!= A_OK)
	{
		RETAILMSG(DBG_ERR, (_P"BMIWriteMemory for htc version failed \n"));
		return A_ERROR;
	}

#ifdef CONFIG_HOST_TCMD_SUPPORT
	if(testmode) 
		ar->arTargetMode = AR6000_TCMD_MODE;
	else 
		ar->arTargetMode = AR6000_WLAN_MODE;
#endif
	if (enabletimerwar) 
	{
		A_UINT32 param;

		if (BMIReadMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
							(A_UCHAR *)&param,
							4)!= A_OK)
		{
			RETAILMSG(DBG_ERR, (_P"BMIReadMemory for enabletimerwar failed \n"));
			return A_ERROR;
		}

		param |= HI_OPTION_TIMER_WAR;

		if (BMIWriteMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
							(A_UCHAR *)&param,
							4) != A_OK)
		{
			RETAILMSG(DBG_ERR, (_P"BMIWriteMemory for enabletimerwar failed \n"));
			return A_ERROR;
		}
		
		RETAILMSG(DBG_TRACE,(_P"Timer WAR enabled\n"));
		
	}

	/* set the firmware mode to STA/IBSS/AP */
	{
		A_UINT32 param;

		if (BMIReadMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
							(A_UCHAR *)&param,
							4)!= A_OK)
		{
			RETAILMSG(DBG_ERR, (_P"BMIReadMemory for setting fwmode failed \n"));
			return A_ERROR;
		}

		param |= (fwmode << HI_OPTION_FW_MODE_SHIFT);

		if (BMIWriteMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
							(A_UCHAR *)&param,
							4) != A_OK)
		{
			RETAILMSG(DBG_ERR, (_P"BMIWriteMemory for setting fwmode failed \n"));
			return A_ERROR;
		}
		
		RETAILMSG(DBG_TRACE,(_P"Firmware mode set\n"));
		
	}
	
	if (processDot11Hdr) 
	{
		A_UINT32 param;

		if (BMIReadMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
							(A_UCHAR *)&param,
							4)!= A_OK)
		{
			RETAILMSG(DBG_ERR, (_P"BMIReadMemory for processDot11Hdr failed \n"));
			return A_ERROR;
		}

		param |= HI_OPTION_RELAY_DOT11_HDR;

		if (BMIWriteMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_option_flag),
							(A_UCHAR *)&param,
							4) != A_OK)
		{
			RETAILMSG(DBG_ERR, (_P"BMIWriteMemory for processDot11Hdr failed \n"));
			return A_ERROR;
		}
		
		RETAILMSG(DBG_TRACE,(_P"processDot11Hdr enabled\n"));
		
	}


	/* No need to reserve RAM space for patch as olca/dragon is flash based
	*/
	if (ar->arTargetType == TARGET_TYPE_AR6001) 
	{
		param = 0;
		if(BMIWriteMemory(ar->arHifDevice,
							HOST_INTEREST_ITEM_ADDRESS(ar, hi_end_RAM_reserve_sz),
							(A_UCHAR *)&param,
							4) != A_OK)
		{
			RETAILMSG(DBG_ERR,(_P"BMIWriteMemory for hi_end_RAM_reserve_sz failed \n"));
			return A_ERROR;
		}
	}


	/* since BMIInit is called in the driver layer, we have to set the block
	* size here for the target */

	if (A_FAILED(ar6000_set_htc_params(ar->arHifDevice,
										ar->arTargetType,
										mbox_yield_limit,
										0 /* use default number of control buffers */
										))) 
	{
		return A_ERROR;
	}


	RETAILMSG(DBG_TRACE, (_P"ar6000_configure_target() - Exit \r\n"));
	return A_OK;
	
}



static void ar6000_target_failure(void *Instance, A_STATUS Status)
{
	RETAILMSG(DBG_TRACE, (_P"ar6000_target_failure() --> not ready ~~ must have something to do !!!!! \r\n"));
#if 0 //bluebird
    AR_SOFTC_T *ar = (AR_SOFTC_T *)Instance;
    WMI_TARGET_ERROR_REPORT_EVENT errEvent;
    static A_BOOL sip = FALSE;

    if (Status != A_OK) {

        printk(KERN_ERR "ar6000_target_failure: target asserted \n");

        if (timer_pending(&ar->arHBChallengeResp.timer)) {
            A_UNTIMEOUT(&ar->arHBChallengeResp.timer);
        }

        /* try dumping target assertion information (if any) */
        ar6000_dump_target_assert_info(ar->arHifDevice,ar->arTargetType);

        /*
         * Fetch the logs from the target via the diagnostic
         * window.
         */
        ar6000_dbglog_get_debug_logs(ar);

        /* Report the error only once */
        if (!sip) {
            sip = TRUE;
            errEvent.errorVal = WMI_TARGET_COM_ERR |
                                WMI_TARGET_FATAL_ERR;
            ar6000_send_event_to_app(ar, WMI_ERROR_REPORT_EVENTID,
                                     (A_UINT8 *)&errEvent,
                                     sizeof(WMI_TARGET_ERROR_REPORT_EVENT));
        }
    }
#endif //bluebird	
}


extern char data_patch_hw[1336];
extern char athwlan2_firmware[98324];



#define MAX_BUF (8*1024)
#define A_ROUND_UP(x, y)  ((((x) + ((y) - 1)) / (y)) * (y))




static int ar6000_readwrite_file(const A_CHAR *filename, A_CHAR *rbuf, const A_CHAR *wbuf, size_t length)
{

	HANDLE hfile;
	int r;

	hfile=CreateFile(L"athwlan.bin.z77",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	RETAILMSG(DBG_TRACE, (_P" CreateFile = %d \r\n", hfile));
	r = ReadFile(hfile,rbuf,length,NULL,NULL);

	return 0;

}




static void firmware_transfer(HIF_DEVICE *device, int file_id, A_UINT32 address, A_BOOL isCompressed)
{
	int ret;
	char * fw_buf = NULL, *src;
//	char tp_buf[1024 * 100];
		
	size_t length, length1, remaining, bufsize, bmisize;

	RETAILMSG(DBG_TRACE,(_P" firmware_transfer(file_id = %d, address = 0x%08x, isCompressed = %d ) \r\n", file_id, address, isCompressed));

	//ar6000_readwrite_file(NULL, tp_buf, NULL, 98304);
		
	if(file_id == 1)		// ar6000 firmware 
		length = 94567;
	else if(file_id == 2)	//ar6000 patch file
		length = 1340;

	bufsize = (length + PAGE_SIZE) & (~(PAGE_SIZE-1));

	RETAILMSG(DBG_TRACE,(_P" firmware_transfer(): length = %d, bufsize = %d \r\n", length, bufsize));

	bmisize = A_ROUND_UP(length, 4);
	bufsize = max(bmisize, bufsize);
	fw_buf = (char*)malloc(bufsize);

	if(fw_buf == NULL)
	{
		RETAILMSG(1,(_P" can't get fw_buff memory alloc !!! \r\n"));
		return ;
	}

	if(file_id == 1)
		memcpy(fw_buf, athwlan2_firmware, 98324);
	else if(file_id == 2)
		memcpy(fw_buf, data_patch_hw, 1336);

	
	if ( isCompressed) 
	{
		ret = BMILZStreamStart(device, address);
		
		if (ret != A_OK) 
		{
			RETAILMSG(1,(_P"firmware_transfer(): BMILZStreamStart failed, ret=%d \r\n", ret));
			goto Transfer_DONE;
		}
	}	

	remaining = bufsize;
	src = fw_buf;
	while (remaining>0) 
	{
		length = (remaining > MAX_BUF)? MAX_BUF : remaining;
		length1 = A_ROUND_UP(length, 4);
		
		if (isCompressed) 
		{
			RETAILMSG(1,(_P"firmware_transfer(): BMILZData: len=%d, remaining=%d \r\n",length1, remaining-length));
			ret = BMILZData(device, (A_UCHAR *)src, length1);
			
			if (ret != A_OK) 
			{
				RETAILMSG(1,(L"firmware_transfer(): BMILZData failed, ret=%d \r\n", ret));
				goto Transfer_DONE;
			}
		} 
		else 
		{
		
			ret = BMIWriteMemory(device, address, (A_UCHAR*)src, length1);
			
			if (ret != A_OK) 
			{
				RETAILMSG(1, (L"firmware_transfer(): BMIWriteMemory failed, ret=%d \r\n",ret));
				goto Transfer_DONE;
			}
		}

		remaining -= length;
		address += length;
		src += length;
	}
Transfer_DONE:
	if (fw_buf) 
	{
		free(fw_buf);
	}

	RETAILMSG(1,(L"Firmware transfer complete !! \r\n"));

	return;
	
}


static A_STATUS
ar6000_download_image(AR_SOFTC_T *ar)
{

    A_STATUS ret = A_OK;

    if (ar->bIsDestroyProgress) 
	{
//        up(&ar->arSem);
            return A_ERROR;
	}
	
/* ATHENV */


#if 1 //bluebired defined(ANDROID_ENV)
    //if (tgt_fw != NULL) 
	{
		A_UINT32 value, old_options, old_sleep;
		A_UCHAR	mac_addr[6];
		/* temporarily disable system sleep */
		value = 0;
		BMIReadSOCRegister(ar->arHifDevice, 0x180c0, &value);
		old_options = value;
		value |= 0x08;
		BMIWriteSOCRegister(ar->arHifDevice, 0x180c0, value);
		value = 0;
		BMIReadSOCRegister(ar->arHifDevice, 0x40c4, &value);
		old_sleep = value;
		value |= 0x01;
		BMIWriteSOCRegister(ar->arHifDevice, 0x40c4, value);
		RETAILMSG(DBG_TRACE, (_P"old options [%d] old sleep [%d]\n", old_options, old_sleep));

		/* run at 40/44MHz by default */
		value = 0;
		BMIWriteSOCRegister(ar->arHifDevice, 0x4020, value);

		/* Set hi_refclk_hz */
		RETAILMSG(DBG_TRACE, (_P"Set hi_refclk_hz : Ref Clock=%d\n", refClock));
		BMIWriteSOCRegister(ar->arHifDevice, 0x500478, refClock);

		/* use internal clock? */
		BMIReadSOCRegister(ar->arHifDevice, 0x50047c, &value);
		if (value == 0) 
		{
			if(debugdriver) 
				RETAILMSG(DBG_TRACE, (_P"use internal clock\n"));
		   
			value = 0x100000;
			BMIWriteSOCRegister(ar->arHifDevice, 0x40e0, value);
		}


		/* eeprom */
		/*
		* Change to use the mechanism as Olca 2.1 : eeprom -> Host -> FW
		* With this mechanism, though the data needs to move to the host side first.
		* But we can change the eeprom data at the driver side
		*/
        
		/* The way Olca 2.1 used */
		AR_DEBUG_PRINTF("AR6000: eeprom transfer by HOST\n");

		eeprom_ar6000_transfer(ar->arHifDevice, NULL, NULL, 0, mac_addr);

		RETAILMSG(DBG_TRACE, (_P"MAC_ADDRESS --> %02x:%02x:%02x:%02x:%02x:%02x \r\n", 
							mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]));
#if 0	//bluebird
        if (IS_MAC_NULL(ar->arNetDev->dev_addr)) {
            if(debugdriver) printk("Cannot get eeprom data\n");
            ret = A_ERROR;
            goto err_exit;
        }
#endif  //bluebird

		RETAILMSG(DBG_TRACE, (_P"AR6000: BMISetAppStart\n"));
		BMISetAppStart(ar->arHifDevice, 0x9140f0);

		/* enable HI_OPTION_TIMER_WAR */
		RETAILMSG(DBG_TRACE, (_P"AR6000: enable HI_OPTION_TIMER_WAR\n"));
        value = 0;
		BMIReadSOCRegister(ar->arHifDevice, 0x500410, &value);
		value |= 0x01;
		BMIWriteSOCRegister(ar->arHifDevice, 0x500410, value);

		/* fw */
		RETAILMSG(DBG_TRACE, (_P"AR6000: firmware_transfer\n"));
#if 0 //bluebird		
		if ((tgt_fw[strlen(tgt_fw) - 3] == 'z')
			&& (tgt_fw[strlen(tgt_fw) - 2] == '7')
			&& (tgt_fw[strlen(tgt_fw) - 1] == '7')) 
#endif //bluebird			
		{
			firmware_transfer(ar->arHifDevice, 1, 0x502070, TRUE);
		}
     


		/* WLAN patch DataSets */
		firmware_transfer(ar->arHifDevice, 2, 0x52d6c4, FALSE);
		BMIWriteSOCRegister(ar->arHifDevice, 0x500418, 0x52d6c4);

		/* restore system sleep */
		BMIWriteSOCRegister(ar->arHifDevice, 0x40c4, old_sleep);
		ret = BMIWriteSOCRegister(ar->arHifDevice, 0x180c0, old_options);
		if (ret!=A_OK) 
		{
		    /* check if the card has any problem before we call ar6000_init */
			RETAILMSG(DBG_ERR, (_P" the card has any problem before we call ar6000_init \r\n"));
		    goto err_exit;
		}

		if (work_mode == 2) 
		{
			/* art mode */
			BMIWriteSOCRegister(ar->arHifDevice, 0x500478, refClock);
			BMIWriteSOCRegister(ar->arHifDevice, 0x500458, 0x1);
			Sleep(1000);
		} 
		else 
		{
			/* normal WIFI or TCMD mode, done */
			
			ret = ar6000_init(ar);
			
			if (ret!= A_OK) 
			{
				if(debugdriver) 
					RETAILMSG(DBG_TRACE, (_P"%s: ar6000_init failed, ret=%d\n", __FUNCTION__, ret));
				goto err_exit;
			}

			if ((work_mode == 0) && (ret == A_OK)) 
			{
				RETAILMSG(DBG_TRACE, (_P" chan_num = %d \r\n", chan_num));
				/* configure channel number? */
				if ((chan_num == 11) || (chan_num == 13) || (chan_num == 14)) 
				{
					int i;
					A_UINT16 chan_list[16] = {0};

					RETAILMSG(1,(_P"AR6000: configure channel number [0x%x]\n", chan_num));
					
					for (i = 0; i < chan_num; i++) 
					{
						/* channel 14 */
						if (i == 14)
							chan_list[i] = 2484;
						
						/* channel 1~13 */
						chan_list[i] = (2407 + ((i + 1) * 5));
                    }
					
					if (wmi_set_channelParams_cmd(ar->arWmi, 1, WMI_11G_MODE, chan_num, chan_list) != A_OK) 
					{
						if(debugdriver) 
							RETAILMSG(DBG_TRACE, (_P"Fialed to configure channel number\n"));
					}
                }
/* ATHENV V10.1 +++
 */
				if (txpwr > 0 ) 
				{
					if ( wmi_set_txPwr_cmd(ar->arWmi, txpwr) != A_OK )  
					{
						if(debugdriver) 
							RETAILMSG(DBG_TRACE, (_P"Failed to set txpwr : %d\n", txpwr));
					} 
					else 
					{
						if(debugdriver) 
							RETAILMSG(DBG_TRACE, (_P"AR6K: set txpwr : %d successfully\n", txpwr));
					}
				}
				
				{
					WMI_SET_ROAM_CTRL_CMD cmd;
					A_MEMZERO(&cmd, sizeof(cmd));
					cmd.roamCtrlType = WMI_SET_LOWRSSI_SCAN_PARAMS;
					wmi_set_roam_ctrl_cmd(ar->arWmi, &cmd, sizeof(cmd));
				}



/* ATHENV V10.1 ---
 */

				/* start a scan immediately if it's normal WIFI mode */
				if (wmi_bssfilter_cmd(ar->arWmi, ALL_BSS_FILTER, 0) != A_OK)
					RETAILMSG(DBG_TRACE, (_P"Fialed to set filter\n"));
				
				if (wmi_startscan_cmd(ar->arWmi, WMI_LONG_SCAN, FALSE, FALSE, 0, 0, 0, NULL) != A_OK)
					RETAILMSG(DBG_TRACE, (_P"Failed to send scan cmd\n"));
				/* configure disconnect timeout value */
				if (wmi_disctimeout_cmd(ar->arWmi, 3) != A_OK)
					RETAILMSG(DBG_TRACE, (_P"Failed to disctimeout cmd\n"));				
			
			}
		}
	}
#endif /* ANDROID_ENV */
/* ATHENV */


err_exit:

    return ret;

}




/* connect to a service */
static A_STATUS ar6000_connectservice(AR_SOFTC_T               *ar,
                                      HTC_SERVICE_CONNECT_REQ  *pConnect,
                                      char                     *pDesc)
{

	A_STATUS                 status;
	HTC_SERVICE_CONNECT_RESP response;

	do 
	{

		A_MEMZERO(&response,sizeof(response));

		status = HTCConnectService(ar->arHtcTarget, pConnect, &response);

		if (A_FAILED(status)) 
		{
			RETAILMSG(DBG_ERR,(_P" Failed to connect to %s service status:%d \n",pDesc, status));
		    break;
		}

		RETAILMSG(DBG_TRACE, (_P" Service ID = %d, Endpoint = %d \r\n", pConnect->ServiceID,response.Endpoint));
		switch (pConnect->ServiceID) 
		{
			case WMI_CONTROL_SVC :
				if (ar->arWmiEnabled) 
				{
					/* set control endpoint for WMI use */
					wmi_set_control_ep(ar->arWmi, response.Endpoint);
				}
				
				/* save EP for fast lookup */
				ar->arControlEp = response.Endpoint;
				break;
				 
			case WMI_DATA_BE_SVC :
				arSetAc2EndpointIDMap(ar, WMM_AC_BE, response.Endpoint);
				break;
				 
			case WMI_DATA_BK_SVC :
				arSetAc2EndpointIDMap(ar, WMM_AC_BK, response.Endpoint);
				break;
				 
			case WMI_DATA_VI_SVC :
				arSetAc2EndpointIDMap(ar, WMM_AC_VI, response.Endpoint);
				break;
				  
			case WMI_DATA_VO_SVC :
				arSetAc2EndpointIDMap(ar, WMM_AC_VO, response.Endpoint);
				break;
				 
			default:
                RETAILMSG(DBG_ERR,(_P"ServiceID not mapped %d\n", pConnect->ServiceID));
                status = A_EINVAL;
            	break;
        }

    } while (FALSE);

    return status;

}




/* This function does one time initialization for the lifetime of the device */
int ar6000_init(AR_SOFTC_T *ar)
{
 //   AR_SOFTC_T *ar;
    A_STATUS    status;
//    A_INT32     timeleft;

    /* Do we need to finish the BMI phase */
	/* Start AR6002 Firmware */
	if(BMIDone(ar->arHifDevice) != A_OK)
	{
		RETAILMSG(DBG_TRACE, (_P" Failed BMIDone(ar->arHifDevice) !! \r\n"));
		return -1;
	}

	if (!bypasswmi)
	{
#if 0 /* TBDXXX */
        if (ar->arVersion.host_ver != ar->arVersion.target_ver) {
            A_PRINTF("WARNING: Host version 0x%x does not match Target "
                    " version 0x%x!\n",
                    ar->arVersion.host_ver, ar->arVersion.target_ver);
        }
#endif

		/* Indicate that WMI is enabled (although not ready yet) */
		ar->arWmiEnabled = TRUE;
		if ((ar->arWmi = wmi_init((void *) ar)) == NULL)
		{
			RETAILMSG(DBG_ERR, (_P"ar6000_init(): Failed to initialize WMI.\n"));
			return(-1);
		}
		ar6000_Set_WMI_Handle((void *) ar, ar->arWmi);
		RETAILMSG(DBG_TRACE, (_P"ar6000_init(): Got WMI @ 0x%08x.\n",(unsigned int) ar->arWmi));
	}

	do 
	{
		HTC_SERVICE_CONNECT_REQ connect;

		/* the reason we have to wait for the target here is that the driver layer
		* has to init BMI in order to set the host block size,
		*/
		status = HTCWaitTarget(ar->arHtcTarget);

		if (A_FAILED(status)) 
		{
			RETAILMSG(DBG_TRACE, (_P" Failed HTCWaitTarget() \r\n"));
			break;
		}

		A_MEMZERO(&connect,sizeof(connect));
		
		/* meta data is unused for now */
		connect.pMetaData = NULL;
		connect.MetaDataLength = 0;
		
		/* these fields are the same for all service endpoints */
		connect.EpCallbacks.pContext = ar;
		connect.EpCallbacks.EpTxComplete = ar6k_tx_complete;//ar6000_tx_complete;
		connect.EpCallbacks.EpRecv = ar6k_rx;//ar6k_rx;
		connect.EpCallbacks.EpRecvRefill = ar6000_rx_refill;
		connect.EpCallbacks.EpSendFull = ar6000_tx_queue_full;
		
		/* set the max queue depth so that our ar6000_tx_queue_full handler gets called.
		* Linux has the peculiarity of not providing flow control between the
		* NIC and the network stack. There is no API to indicate that a TX packet
		* was sent which could provide some back pressure to the network stack.
		* Under linux you would have to wait till the network stack consumed all sk_buffs
		* before any back-flow kicked in. Which isn't very friendly.
		* So we have to manage this ourselves */
		connect.MaxSendQueueDepth = 32;

		/* connect to control service */
		connect.ServiceID = WMI_CONTROL_SVC;
		status = ar6000_connectservice(ar, &connect, "WMI CONTROL");
		
		if (A_FAILED(status)) 
		{
			RETAILMSG(DBG_TRACE, (_P"ar6000_connectservice() Break 1\r\n"));
			break;
		}

		/* for the remaining data services set the connection flag to reduce dribbling,
		* if configured to do so */
		if (reduce_credit_dribble) 
		{
			connect.ConnectionFlags |= HTC_CONNECT_FLAGS_REDUCE_CREDIT_DRIBBLE;
			/* the credit dribble trigger threshold is (reduce_credit_dribble - 1) for a value
			* of 0-3 */
			connect.ConnectionFlags &= ~HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK;
			connect.ConnectionFlags |= ((A_UINT16)reduce_credit_dribble - 1) & HTC_CONNECT_FLAGS_THRESHOLD_LEVEL_MASK;
        }
		
		/* connect to best-effort service */
		connect.ServiceID = WMI_DATA_BE_SVC;

        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI DATA BE");
        if (A_FAILED(status)) 
		{
            break;
        }

            /* connect to back-ground
             * map this to WMI LOW_PRI */
        connect.ServiceID = WMI_DATA_BK_SVC;
        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI DATA BK");
        if (A_FAILED(status)) {
            break;
        }

            /* connect to Video service, map this to
             * to HI PRI */
        connect.ServiceID = WMI_DATA_VI_SVC;
        status = ar6000_connectservice(ar,
                                       &connect,
                                       "WMI DATA VI");
		if (A_FAILED(status)) 
		{
		    break;
		}

		    /* connect to VO service, this is currently not
		* mapped to a WMI priority stream due to historical reasons.
		* WMI originally defined 3 priorities over 3 mailboxes
		* We can change this when WMI is reworked so that priorities are not
		* dependent on mailboxes */
		connect.ServiceID = WMI_DATA_VO_SVC;
		status = ar6000_connectservice(ar,
		                               &connect,
		                               "WMI DATA VO");
		if (A_FAILED(status)) {
		    break;
		}

		A_ASSERT(arAc2EndpointID(ar,WMM_AC_BE) != 0);
		A_ASSERT(arAc2EndpointID(ar,WMM_AC_BK) != 0);
		A_ASSERT(arAc2EndpointID(ar,WMM_AC_VI) != 0);
		A_ASSERT(arAc2EndpointID(ar,WMM_AC_VO) != 0);

		    /* setup access class priority mappings */
		ar->arAcStreamPriMap[WMM_AC_BK] = 0; /* lowest  */
		ar->arAcStreamPriMap[WMM_AC_BE] = 1; /*         */
		ar->arAcStreamPriMap[WMM_AC_VI] = 2; /*         */
		ar->arAcStreamPriMap[WMM_AC_VO] = 3; /* highest */

    } while (FALSE);

	if (A_FAILED(status)) 
	{
		RETAILMSG(DBG_ERR, (_P"ar6000_init() Exti 1\r\n"));
		return (-1);
	}

    /*
     * give our connected endpoints some buffers
     */

    ar6000_rx_refill(ar, ar->arControlEp);
    ar6000_rx_refill(ar, arAc2EndpointID(ar,WMM_AC_BE));
	
    /*
     * We will post the receive buffers only for SPE or endpoint ping testing so we are
     * making it conditional on the 'bypasswmi' flag.
     */
    if (bypasswmi) {
        ar6000_rx_refill(ar,arAc2EndpointID(ar,WMM_AC_BK));
        ar6000_rx_refill(ar,arAc2EndpointID(ar,WMM_AC_VI));
        ar6000_rx_refill(ar,arAc2EndpointID(ar,WMM_AC_VO));
    }

        /* setup credit distribution */
    ar6000_setup_credit_dist(ar->arHtcTarget, &ar->arCreditStateInfo);

    /* Since cookies are used for HTC transports, they should be */
    /* initialized prior to enabling HTC.                        */
    ar6000_cookie_init(ar);


    /* start HTC */
    status = HTCStart(ar->arHtcTarget);
	
    if (status != A_OK) {
        if (ar->arWmiEnabled == TRUE) {
            wmi_shutdown(ar->arWmi);
            ar->arWmiEnabled = FALSE;
            ar->arWmi = NULL;
        }
		RETAILMSG(DBG_TRACE, (_P"HTC Start ERR !!!\n"));
        ar6000_cookie_cleanup(ar);
        return -1;
    }

    if (!bypasswmi) {
        
#if 0 //bluebird		

		/* Wait for Wmi event to be ready */

        timeleft = wait_event_interruptible_timeout(arEvent, (ar->arWmiReady == TRUE), wmitimeout * HZ);

        if(!timeleft || signal_pending(current))
        {
            RETAILMSG(DBG_ERR,(_P"WMI is not ready or wait was interrupted\n"));
            return -EIO;
        }
		
#endif //bluebird		
		Sleep(1000);
		RETAILMSG(DBG_TRACE,(_P"SKIP writing wmi protocal version !!!\n"));
	
    }

    ar->arNumDataEndPts = 1;

    if (bypasswmi) 
	{
#if 0 //bluebird	
		/* for tests like endpoint ping, the MAC address needs to be non-zero otherwise
             * the data path through a raw socket is disabled */
        dev->dev_addr[0] = 0x00;
        dev->dev_addr[1] = 0x01;
        dev->dev_addr[2] = 0x02;
        dev->dev_addr[3] = 0xAA;
        dev->dev_addr[4] = 0xBB;
        dev->dev_addr[5] = 0xCC;
#endif //bluebird
    }

	
#if 1
	/* Add by MMC ÇãÁø¿µ,  don't go to sleep mode*/
	//builder:change default config
	wmi_set_nodeage(ar->arWmi, 13000);
	
	if( wmi_scanparams_cmd(ar->arWmi, 0, 0, 5, 100, 110, 0, 1, DEFAULT_SCAN_CTRL_FLAGS, 0, 0) != A_OK )
		RETAILMSG(DBG_TRACE, (_P"AR6K2:ar6000_init(): Scan parameters setting failed\r\n"));
	
	if( wmi_disctimeout_cmd(ar->arWmi, 5) != A_OK )
		RETAILMSG(DBG_TRACE, (_P"AR6K2:ar6000_init(): Disconnect timeout setting failed\r\n" ));
		
	if( wmi_bmisstime_cmd(ar->arWmi, 0, 10) != A_OK )
		RETAILMSG(DBG_TRACE, (_P"AR6K2:ar6000_init(): Beacon miss timeout setting failed\r\n" ));
			
	if( wmi_powermode_cmd(ar->arWmi, MAX_PERF_POWER) != A_OK )
		RETAILMSG(DBG_TRACE, (_P"AR6K2:ar6000_init(): Power mode setting failed\r\n" ));
	
	if( wmi_pmparams_cmd(ar->arWmi, 100, 0, 2) != A_OK )
		RETAILMSG(DBG_TRACE, (_P"AR6K2:ar6000_init(): Power management setting failed\r\n" ));
	
	if( wmi_listeninterval_cmd(ar->arWmi, 0, MIN_LISTEN_BEACONS) != A_OK )
		RETAILMSG(DBG_TRACE, (_P"AR6K2:ar6000_init(): Listen interval setting failed\r\n" ));
#endif
    return(0);
}


/*
 * HTC Event handlers
 */
A_STATUS
ar6000_avail_ev(void *context, void *hif_handle)
{

	AR_SOFTC_T *ar;
	HTC_INIT_INFO  htcInfo;

	RETAILMSG(DBG_ERR, (_P" %s() : Enter + \r\n",__func__)); 

	// Spin Lock Init
	NdisAllocateSpinLock(&m_Lock);

	ar = A_MALLOC(sizeof(AR_SOFTC_T));
	memset(ar,0,sizeof(AR_SOFTC_T));
	ar->arHifDevice = hif_handle;
	ar->arWlanState = WLAN_ENABLED;

	A_INIT_TIMER((A_TIMER *)&ar->arHBChallengeResp.timer, ar6000_detect_error, ar);
	ar->arHBChallengeResp.seqNum = 0;
	ar->arHBChallengeResp.outstanding = FALSE;
	ar->arHBChallengeResp.missCnt = 0;
	ar->arHBChallengeResp.frequency = AR6000_HB_CHALLENGE_RESP_FREQ_DEFAULT;
	ar->arHBChallengeResp.missThres = AR6000_HB_CHALLENGE_RESP_MISS_THRES_DEFAULT;


//	A_INIT_TIMER(&ar->disconnect_timer, disconnect_timer_handler, dev);
//	A_INIT_TIMER(&ar->rxPriorityTimer, rx_priority_timer_handler, dev);


	BMIInit();
	{
		struct bmi_target_info targ_info;

		if (BMIGetTargetInfo((HIF_DEVICE *)hif_handle, &targ_info) != A_OK) {
			RETAILMSG(DBG_ERR, (_P"BMIGetTargetInfo Fail !!\n"));
			return A_ERROR;
        }
		
		ar->arVersion.target_ver = targ_info.target_ver;
		ar->arTargetType = targ_info.target_type;
		ar->hCar6k = context;

		/* do any target-specific preparation that can be done through BMI */
		if (ar6000_prepare_target((HIF_DEVICE *)hif_handle,
								targ_info.target_type,
								targ_info.target_ver) != A_OK) 
		{
			AR_DEBUG_PRINTF("ar6000_prepare_target Fail !!\n");
			return A_ERROR;
		}		
	}


	if (ar6000_configure_target(ar) != A_OK) 
	{
		AR_DEBUG_PRINTF("ar6000_configure_target !!\n");
		return A_ERROR;
	}


	A_MEMZERO(&htcInfo,sizeof(htcInfo));
	htcInfo.pContext = ar;
	htcInfo.TargetFailure = ar6000_target_failure;

	ar->arHtcTarget = HTCCreate(ar->arHifDevice,&htcInfo);

	if (ar->arHtcTarget == NULL) 
	{
		RETAILMSG(DBG_ERR, (_P"ar->arHtcTarget fail !! \r\n"));
		return A_ERROR;
    }

	HIFClaimDevice(ar->arHifDevice, ar);


	if (ar6000_download_image(ar)!=A_OK) 
	{
		RETAILMSG(DBG_ERR, (_P"ar6000_download_image fail & Skip ar6000_destroy !!! \r\n"));
        //ar6000_destroy(dev, 0);
        return A_ERROR;
    }

	RETAILMSG(DBG_TRACE, (_P"ar6000_avail_ev() Exit - \r\n")); 

	return A_OK;
}


A_STATUS
ar6000_unavail_ev(void *context, void *hif_handle)
{
	
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    AR_SOFTC_T *ar = (AR_SOFTC_T *)context;

        /* NULL out it's entry in the global list */
    ar6000_devices[ar->arDeviceIndex] = NULL;
    ar6000_destroy(ar->arNetDev, 1);
#endif
    return A_ERROR;
}






/*
 * Receive event handler.  This is called by HTC when a packet is received
 */
int pktcount;

static void
ar6000_rx(void *Context, HTC_PACKET *pPacket)
{

#if 1 //bluebird

	AR_SOFTC_T *ar = (AR_SOFTC_T *)Context;
#if 0
	struct sk_buff *skb = (struct sk_buff *)pPacket->pPktContext;
#else
	struct ndis_mini_buf_t * skb = (struct ndis_mini_buf_t *)pPacket->pPktContext;
#endif
//	int minHdrLen;
	A_STATUS        status = pPacket->Status;
	HTC_ENDPOINT_ID   ept = pPacket->Endpoint;

	A_ASSERT((status != A_OK) ||
			(pPacket->pBuffer == (A_NETBUF_DATA(skb) + HTC_HEADER_LEN)));

	RETAILMSG(0, (_P"ar6000_rx ar=0x%x eid=%d, skb=0x%x, data=0x%x, len=0x%x \r\n",
					(A_UINT32)ar, ept, (A_UINT32)skb, (A_UINT32)pPacket->pBuffer,
					pPacket->ActualLength));

	if (status != A_OK) 
	{
		RETAILMSG(DBG_ERR, (_P"ERR\n"));
	} 
	else 
	{
		RETAILMSG(0, (_P"OK\n"));
	}
    

	/* take lock to protect buffer counts
	* and adaptive power throughput state */
	//AR6000_SPIN_LOCK(&ar->arLock, 0);
	AR6000_SPIN_LOCK();

	ar->arRxBuffers[ept]--;

	if (A_SUCCESS(status)) 
	{
#if 0	
		AR6000_STAT_INC(ar, rx_packets);
		ar->arNetStats.rx_bytes += pPacket->ActualLength;
#ifdef ADAPTIVE_POWER_THROUGHPUT_CONTROL
		aptcTR.bytesReceived += a_netbuf_to_len(skb);
		applyAPTCHeuristics(ar);
#endif /* ADAPTIVE_POWER_THROUGHPUT_CONTROL */
#endif

		A_NETBUF_PUT(skb, pPacket->ActualLength +  HTC_HEADER_LEN);
		A_NETBUF_PULL(skb, HTC_HEADER_LEN);

#ifdef DEBUG
		if (debugdriver >= 2) {
			ar6000_dump_skb(skb);
		}
#endif /* DEBUG */


	}

	//AR6000_SPIN_UNLOCK(&ar->arLock, 0);
	AR6000_SPIN_UNLOCK();

//	skb->dev = ar->arNetDev;

	if (status != A_OK) 
	{
		//AR6000_STAT_INC(ar, rx_errors);
		A_NETBUF_FREE(skb);
	} 
	else if (ar->arWmiEnabled == TRUE) 
	{
		if (ept == ar->arControlEp) 
		{
			/*
			* this is a wmi control msg
			*/
			wmi_control_rx(ar->arWmi, skb, ar->arPhyCapability);  /* Bug 82893 */
		} 
		else 
		{
			/*
			* this is a wmi data packet
			*/
             /* NWF
			 */
			 RETAILMSG(DBG_TRACE, (_P" %s() -- Get a Data ########\r\n",__func__));
#if 0 //bluebird --> data packet.
			if (processDot11Hdr) 
				minHdrLen = sizeof(WMI_DATA_HDR) + sizeof(struct ieee80211_frame) + sizeof(ATH_LLC_SNAP_HDR);
			else 
				minHdrLen = sizeof (WMI_DATA_HDR) + sizeof(ATH_MAC_HDR) + sizeof(ATH_LLC_SNAP_HDR);
			

			/* In the case of AP mode we may receive NULL data frames
			* that do not have LLC hdr. They are 16 bytes in size.
			* Allow these frames in the AP mode.
			*/
			if (ar->arNetworkType != AP_NETWORK && ((pPacket->ActualLength < minHdrLen) ||
				(pPacket->ActualLength > AR6000_BUFFER_SIZE)))
			{
				/*
				* packet is too short or too long
				*/
				AR_DEBUG_PRINTF("TOO SHORT or TOO LONG\n");
				AR6000_STAT_INC(ar, rx_errors);
				AR6000_STAT_INC(ar, rx_length_errors);
				A_NETBUF_FREE(skb);
			} 
			else 
			{

				/* Get the Power save state of the STA */
				if (ar->arNetworkType == AP_NETWORK) 
				{
					sta_t *conn = NULL;
					A_UINT8 psState=0,prevPsState;
					ATH_MAC_HDR *datap=NULL;

					psState = (((WMI_DATA_HDR *)A_NETBUF_DATA(skb))->info
								>> WMI_DATA_HDR_PS_SHIFT) & WMI_DATA_HDR_PS_MASK;
					datap = (ATH_MAC_HDR *)(A_NETBUF_DATA(skb)+sizeof(WMI_DATA_HDR));
					conn = ieee80211_find_conn(ar, datap->srcMac);


					if (conn) 
					{
						/* if there is a change in PS state of the STA,
						* take appropriate steps.
						* 1. If Sleep-->Awake, flush the psq for the STA
						*    Clear the PVB for the STA.
						* 2. If Awake-->Sleep, Starting queueing frames
						* the STA.
						*/
						prevPsState = STA_IS_PWR_SLEEP(conn);
						
						if (psState)	STA_SET_PWR_SLEEP(conn);
						else 			STA_CLR_PWR_SLEEP(conn);
							

						if (prevPsState ^ STA_IS_PWR_SLEEP(conn)) 
						{

							if (!STA_IS_PWR_SLEEP(conn)) 
							{

								A_MUTEX_LOCK(&conn->psqLock);
								while (!A_NETBUF_QUEUE_EMPTY(&conn->psq)) 
								{
									struct sk_buff *skb=NULL;

									skb = A_NETBUF_DEQUEUE(&conn->psq);
									A_MUTEX_UNLOCK(&conn->psqLock);
									ar6000_data_tx(skb,ar->arNetDev);
									A_MUTEX_LOCK(&conn->psqLock);
								}
								A_MUTEX_UNLOCK(&conn->psqLock);
									
								/* Clear the PVB for this STA */
								wmi_set_pvb_cmd(ar->arWmi, conn->aid, 0);
                               }
						}
					} 
					else 
					{
						/* This frame is from a STA that is not associated*/
						A_ASSERT(FALSE);
					}

					/* Drop NULL data frames here */
					if((pPacket->ActualLength < minHdrLen) ||
                                (pPacket->ActualLength > AR6000_BUFFER_SIZE)) 
					{
						A_NETBUF_FREE(skb);
						goto refill;
					}
				}

				wmi_data_hdr_remove(ar->arWmi, skb);
				/* NWF: print the 802.11 hdr bytes */
				if(processDot11Hdr) {
					wmi_dot11_hdr_remove(ar->arWmi,skb);
				} else {
					wmi_dot3_2_dix(ar->arWmi, skb);
				}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
				/*
				* extra push and memcpy, for eth_type_trans() of 2.4 kernel
				* will pull out hard_header_len bytes of the skb.
				*/
				A_NETBUF_PUSH(skb, sizeof(WMI_DATA_HDR) + sizeof(ATH_LLC_SNAP_HDR) + HTC_HEADER_LEN);
				A_MEMCPY(A_NETBUF_DATA(skb), A_NETBUF_DATA(skb) + sizeof(WMI_DATA_HDR) +
							sizeof(ATH_LLC_SNAP_HDR) + HTC_HEADER_LEN, sizeof(ATH_MAC_HDR));
#endif
				if ((ar->arNetDev->flags & IFF_UP) == IFF_UP) 
				{	
					if (ar->arNetworkType == AP_NETWORK) 
					{
						struct sk_buff *skb1 = NULL;
						ATH_MAC_HDR *datap;

						datap = (ATH_MAC_HDR *)A_NETBUF_DATA(skb);
							
						if (IEEE80211_IS_MULTICAST(datap->dstMac)) 
						{
							/* Bcast/Mcast frames should be sent to the OS
							* stack as well as on the air.
							*/
							skb1 = skb_copy(skb,GFP_ATOMIC);
						} 
						else 
						{
							/* Search for a connected STA with dstMac as
							* the Mac address. If found send the frame to
							* it on the air else send the frame up the
							* stack
							*/
								
							sta_t *conn = NULL;
							conn = ieee80211_find_conn(ar, datap->dstMac);

							if (conn && ar->intra_bss) 
							{
								skb1 = skb;
								skb = NULL;
							} 
							else if(conn && !ar->intra_bss) 
							{
								A_NETBUF_FREE(skb);
								skb = NULL;
							}
						}// (IEEE80211_IS_MULTICAST(datap->dstMac))
							
						if (skb1) 
						{
							ar6000_data_tx(skb1, ar->arNetDev);
						}
					}
				}
#ifndef ANDROID_ENV
				if((ar->arNetworkType != AP_NETWORK) && (skb) && (skb->len == ATH_DHCP_PKT_SIZE)) 
#else
				/* Packet length is not guaranteed for a DHCP packet. */
				/* We need to parse                                   */
				if((ar->arNetworkType != AP_NETWORK) && skb) 
#endif
				{ 
					if( checkforDHCPPacket(skb) == ATH_DHCP_ACK) 
					{

						/* When the device resumes from suspend mode make sure target is ready 
						* Fix for EV#7280 */ 
						if (ar->arWmiReady == TRUE) 
						{
							A_UNTIMEOUT(&dhcp_timer);
							wmi_set_bt_status_cmd(ar->arWmi, BT_STREAM_ALL, BT_STATUS_START); 
							AR_DEBUG_PRINTF("DHCP ACK received start A2DP \n"); 
						}
					}
				}
				deliver_frames_to_nw_stack(ar, skb);
			}
#endif	// data packet			
		}
    } 
	else 
	{
        deliver_frames_to_nw_stack(ar, skb);
    }
	
//refill:
    if (status != A_ECANCELED) {
        /*
         * HTC provides A_ECANCELED status when it doesn't want to be refilled
         * (probably due to a shutdown)
         */
        ar6000_rx_refill(Context, ept);
    }
#else
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#endif //bluebird	
}


A_UINT32 wmiSendCmdNum = 0;


A_STATUS
ar6k_control_tx(void *devt, void *osbuf, HTC_ENDPOINT_ID eid)
{
#if 1
	AR_SOFTC_T       *ar = (AR_SOFTC_T *)devt;
	A_STATUS         status = A_OK;
	struct ar_cookie *cookie = NULL;
	A_UINT32 i;
#ifdef CONFIG_PM
	if (ar->arWowState) 
	{
		A_NETBUF_FREE(osbuf);
		if(debugdriver) printk("Wow enabled. Not allow to send tx control\n");
		return A_EACCES;
	}
#endif 
	/* CONFIG_PM */
	/* take lock to protect ar6000_alloc_cookie() */

	RETAILMSG(0, (_P"%s() + Enter, status = %d \r\n", __func__, status));

	AR6000_SPIN_LOCK();

	do 
	{

		RETAILMSG(0, (_P"ar_contrstatus = ol_tx: skb=0x%x, len=0x%x eid =%d\n",
                         (A_UINT32)osbuf, A_NETBUF_LEN(osbuf), eid));

		if (ar->arWMIControlEpFull && (eid == ar->arControlEp)) 
		{
			/* control endpoint is full, don't allocate resources, we
			* are just going to drop this packet */
			cookie = NULL;
			RETAILMSG(DBG_TRACE, (_P" WMI Control EP full, dropping packet : 0x%X, len:%d \n",
							(A_UINT32)osbuf, A_NETBUF_LEN(osbuf)));
        } 
		else 
		{
			cookie = ar6000_alloc_cookie(ar);
        }

		if (cookie == NULL) 
		{
			status = A_NO_MEMORY;
			RETAILMSG(DBG_ERR, (_P" %s() : No Memory !! \r\n",__func__));
			break;
        }

		if(logWmiRawMsgs) 
		{
			RETAILMSG(DBG_TRACE, (_P"WMI cmd send, msgNo %d :", wmiSendCmdNum));
			
			for(i = 0; i < a_netbuf_to_len(osbuf); i++)
					RETAILMSG(DBG_TRACE, (L"%x ", ((A_UINT8 *)a_netbuf_to_data(osbuf))[i]));
			
            RETAILMSG(DBG_TRACE, (L" \r\n"));
		}

		wmiSendCmdNum++;

    } while (FALSE);

	if (cookie != NULL) 
	{
		/* got a structure to send it out on */
		ar->arTxPending[eid]++;

		if (eid != ar->arControlEp) 
		{
			ar->arTotalTxDataPending++;
		}
	}

	AR6000_SPIN_UNLOCK();

	if (cookie != NULL) 
	{
		cookie->arc_bp[0] = (A_UINT32)osbuf;
		cookie->arc_bp[1] = 0;
		SET_HTC_PACKET_INFO_TX(&cookie->HtcPkt, cookie, A_NETBUF_DATA(osbuf), A_NETBUF_LEN(osbuf), eid, AR6K_CONTROL_PKT_TAG);
		
		/* this interface is asynchronous, if there is an error, cleanup will happen in the
		 * TX completion callback */
		HTCSendPkt(ar->arHtcTarget, &cookie->HtcPkt);
		status = A_OK;
	}

	if (status != A_OK) 
	{
		A_NETBUF_FREE(osbuf);
	}

	RETAILMSG(0, (_P"%s() - Exit, status = %d \r\n", __func__, status));
	
	return status;
#else
	RETAILMSG(DBG_TRACE, (_P"%s() is not ready yet ~ \r\n", __func__));
	return  A_ERROR;
#endif	//bluebird 
}





static int
ar6000_data_tx(struct ndis_mini_buf_ *skb, void *devt)
{
#define AC_NOT_MAPPED   99
	AR_SOFTC_T        *ar = (AR_SOFTC_T *)devt;
	A_UINT8            ac = AC_NOT_MAPPED;
	HTC_ENDPOINT_ID    eid = ENDPOINT_UNUSED;
	A_UINT32          mapNo = 0;
//	int               len;
	struct ar_cookie *cookie;
	A_BOOL            checkAdHocPsMapping = FALSE,bMoreData = FALSE;
#ifdef CONFIG_PM
    if (ar->arWowState) 
	{
        A_NETBUF_FREE(skb);
        if(debugdriver) 
			printk("Wow enabled. Not allow to send tx data\n");
        return 0;
    }
#endif /* CONFIG_PM */


#ifndef ANDROID_ENV
	if( (ar->arNetworkType != AP_NETWORK) && (skb) && 
		(skb->buf_len == ATH_DHCP_PKT_SIZE) && (!ar->bIsDestroyProgress)) 
#else
	/* Packet length is not guaranteed for a DHCP packet. We need to parse */
	if((ar->arNetworkType != AP_NETWORK) && skb && !ar->bIsDestroyProgress) 
#endif
	{ 

	#if 0 //bluebird
		
		if( checkforDHCPPacket(skb) == ATH_DHCP_DISCOVER && !timer_pending(&dhcp_timer)) 
		{

			/* When the device resumes from suspend mode make sure target is ready 
			* Fix for EV#7280 */ 
			if (ar->arWmiReady == TRUE) 
			{
				wmi_set_bt_status_cmd(ar->arWmi, BT_STREAM_ALL, BT_STATUS_STOP); 
				A_UNTIMEOUT(&dhcp_timer);
				A_TIMEOUT_MS(&dhcp_timer, A_DHCP_TIMER_INTERVAL, 0);
				AR_DEBUG_PRINTF("DHCP DISCOVER stop A2DP \n"); 
			} 
		}
	#else
		RETAILMSG(1, (_P" %s() , Need create DHCP Timer !! \r\n", __func__));
	#endif
	}

    RETAILMSG(DBG_TRACE,(_P"ar6000_data_tx start - skb=0x%x, data=0x%x, len=0x%x\n",
							(A_UINT32)skb, (A_UINT32)A_NETBUF_DATA(skb), A_NETBUF_LEN(skb)));

	/* If target is not associated */
	if( (!ar->arConnected && !bypasswmi)
#ifdef CONFIG_HOST_TCMD_SUPPORT
	/* TCMD doesnt support any data, free the buf and return */
	|| (ar->arTargetMode == AR6000_TCMD_MODE)
#endif
    										) 
	{
		A_NETBUF_FREE(skb);
		return 0;
	}

	do 
	{

		if (ar->arWmiReady == FALSE && bypasswmi == 0) 
		{
			break;
		}

#ifdef BLOCK_TX_PATH_FLAG
		if (blocktx) 
            break;
#endif /* BLOCK_TX_PATH_FLAG */

		/* AP mode Power save processing */
		/* If the dst STA is in sleep state, queue the pkt in its PS queue */
		if (ar->arNetworkType == AP_NETWORK) 
		{
			ATH_MAC_HDR *datap = (ATH_MAC_HDR *)A_NETBUF_DATA(skb);
			sta_t *conn = NULL;


			RETAILMSG(1, (_P" This is a ## AP_NETWORK ## !! need modify ~ \r\n"));
		#if 0 //bluebird

			/* If the dstMac is a Multicast address & atleast one of the
			* associated STA is in PS mode, then queue the pkt to the
			* mcastq
			*/			
			if (IEEE80211_IS_MULTICAST(datap->dstMac)) 
			{
				A_UINT8 ctr=0;
				A_BOOL qMcast=FALSE;

				for (ctr=0; ctr<AP_MAX_NUM_STA; ctr++) 
				{
					if (STA_IS_PWR_SLEEP((&ar->sta_list[ctr]))) 
					{
						qMcast = TRUE;
					}
				}
				
				if(qMcast) 
				{
					/* If this transmit is not because of a Dtim Expiry q it */
					if (ar->DTIMExpired == FALSE) 
					{
						A_BOOL isMcastqEmpty = FALSE;

						A_MUTEX_LOCK(&ar->mcastpsqLock);
						isMcastqEmpty = A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq);
						A_NETBUF_ENQUEUE(&ar->mcastpsq, skb);
						A_MUTEX_UNLOCK(&ar->mcastpsqLock);

						/* If this is the first Mcast pkt getting queued
						* indicate to the target to set the BitmapControl LSB
						* of the TIM IE.
						*/
						
						if (isMcastqEmpty) 
						{
							wmi_set_pvb_cmd(ar->arWmi, MCAST_AID, 1);
						}
						return 0;
						
					} 
					else 
					{
						/* This transmit is because of Dtim expiry. Determine if
						* MoreData bit has to be set.
						*/
						A_MUTEX_LOCK(&ar->mcastpsqLock);
						
						if(!A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq)) 
						{
							bMoreData = TRUE;
						}
						A_MUTEX_UNLOCK(&ar->mcastpsqLock);
					}
				}
			}
			else 
			{
				conn = ieee80211_find_conn(ar, datap->dstMac);
				if (conn) 
				{
					if (STA_IS_PWR_SLEEP(conn)) 
					{
						/* If this transmit is not because of a PsPoll q it*/
						if (!STA_IS_PS_POLLED(conn)) 
						{
							A_BOOL isPsqEmpty = FALSE;
							/* Queue the frames if the STA is sleeping */
							A_MUTEX_LOCK(&conn->psqLock);
							isPsqEmpty = A_NETBUF_QUEUE_EMPTY(&conn->psq);
							A_NETBUF_ENQUEUE(&conn->psq, skb);
							A_MUTEX_UNLOCK(&conn->psqLock);

							/* If this is the first pkt getting queued
							* for this STA, update the PVB for this STA
							*/
							if (isPsqEmpty) 
							{
								wmi_set_pvb_cmd(ar->arWmi, conn->aid, 1);
							}

							return 0;
						}
						else 
						{
							/* This tx is because of a PsPoll. Determine if
							* MoreData bit has to be set
							*/
							A_MUTEX_LOCK(&conn->psqLock);
							if (!A_NETBUF_QUEUE_EMPTY(&conn->psq)) 
							{
								bMoreData = TRUE;
							}
							A_MUTEX_UNLOCK(&conn->psqLock);
						}
					}
				}
				else 
				{
					/* non existent STA. drop the frame */
					A_NETBUF_FREE(skb);
					return 0;
				}
			}
		#endif	
		}

		if (ar->arWmiEnabled) 
		{

	#if 0	//bluebird	
			A_UINT32 hard_header_len ;
			
			hard_header_len = ETH_HLEN + sizeof(ATH_LLC_SNAP_HDR) + sizeof(WMI_DATA_HDR) + HTC_HEADER_LEN;
			
			if (A_NETBUF_HEADROOM(skb) < dev->hard_header_len) 
			{
				struct sk_buff  *newbuf;

				/*
				* We really should have gotten enough headroom but sometimes
				* we still get packets with not enough headroom.  Copy the packet.
				*/

				len = A_NETBUF_LEN(skb);
				newbuf = A_NETBUF_ALLOC(len);
				if (newbuf == NULL) {
				    break;
				}
				A_NETBUF_PUT(newbuf, len);
				A_MEMCPY(A_NETBUF_DATA(newbuf), A_NETBUF_DATA(skb), len);
				A_NETBUF_FREE(skb);
				skb = newbuf;
				/* fall through and assemble header */
			}
#endif
			if (processDot11Hdr) 
			{
				if (wmi_dot11_hdr_add(ar->arWmi,skb,ar->arNetworkType) != A_OK) 
				{
					AR_DEBUG_PRINTF("ar6000_data_tx-wmi_dot11_hdr_add failed\n");
					break;
				}
			} 
			else 
			{
				if (wmi_dix_2_dot3(ar->arWmi, skb) != A_OK) 
				{
					AR_DEBUG_PRINTF("ar6000_data_tx - wmi_dix_2_dot3 failed\n");
					break;
				}
			}

			if (wmi_data_hdr_add(ar->arWmi, skb, DATA_MSGTYPE, bMoreData) != A_OK) 
			{
				AR_DEBUG_PRINTF("ar6000_data_tx - wmi_data_hdr_add failed\n");
				break;
			}

			if ((ar->arNetworkType == ADHOC_NETWORK) &&
				ar->arIbssPsEnable && ar->arConnected) 
			{
				/* flag to check adhoc mapping once we take the lock below: */
				checkAdHocPsMapping = TRUE;
			}
			else 
			{
				/* get the stream mapping */
				ac  =  wmi_implicit_create_pstream(ar->arWmi, skb, 0, ar->arWmmEnabled);
			}

		} 
		else 
		{
		#if 0 //bluebird
			struct iphdr    *ipHdr;
			/*
			* the endpoint is directly based on the TOS field in the IP
			* header **** only for testing ******
			*/
			
			ipHdr = (struct iphdr *)(A_NETBUF_DATA(skb) + sizeof(ATH_MAC_HDR));
			/* here we map the TOS field to an access class, this is for
			* the endpointping test application.  The application uses 0,1,2,3
			* for the TOS field to emulate writing to mailboxes.  The number is
			* used to map directly to an access class */
			ac = (ipHdr->tos >> 1) & 0x3;
		 #endif	
		}

	} while (FALSE);

	/* did we succeed ? */
	if ((ac == AC_NOT_MAPPED) && !checkAdHocPsMapping) 
	{
		/* cleanup and exit */
		A_NETBUF_FREE(skb);
		//AR6000_STAT_INC(ar, tx_dropped);
		//AR6000_STAT_INC(ar, tx_aborted_errors);
		return 0;
	}

	cookie = NULL;

	/* take the lock to protect driver data */
	AR6000_SPIN_LOCK();

	do 
	{

		if (checkAdHocPsMapping) 
		{
			#if 0 //bluebird
			eid = ar6000_ibss_map_epid(skb, dev, &mapNo);
			#endif 
		}
		else 
		{
			eid = arAc2EndpointID (ar, ac);
		}
		
		/* validate that the endpoint is connected */
		if (eid == 0 || eid == ENDPOINT_UNUSED ) 
		{
			AR_DEBUG_PRINTF(" eid %d is NOT mapped!\n", eid);
			break;
		}
		
		/* allocate resource for this packet */
		cookie = ar6000_alloc_cookie(ar);

		if (cookie != NULL) 
		{
			/* update counts while the lock is held */
			ar->arTxPending[eid]++;
			ar->arTotalTxDataPending++;
		}

	} while (FALSE);

	AR6000_SPIN_UNLOCK();

	if (cookie != NULL) 
	{
		cookie->arc_bp[0] = (A_UINT32)skb;
		cookie->arc_bp[1] = mapNo;
		SET_HTC_PACKET_INFO_TX(&cookie->HtcPkt,
								cookie,
								A_NETBUF_DATA(skb),
								A_NETBUF_LEN(skb),
								eid,
								AR6K_DATA_PKT_TAG);

		/* HTC interface is asynchronous, if this fails, cleanup will happen in
		* the ar6000_tx_complete callback */
		HTCSendPkt(ar->arHtcTarget, &cookie->HtcPkt);
	}
	else
	{
		/* no packet to send, cleanup */
		A_NETBUF_FREE(skb);
		//AR6000_STAT_INC(ar, tx_dropped);
		//AR6000_STAT_INC(ar, tx_aborted_errors);
    }

    return 0;
}



#if 0
static HTC_SEND_FULL_ACTION ar6000_tx_queue_full(void *Context, HTC_PACKET *pPacket)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    AR_SOFTC_T     *ar = (AR_SOFTC_T *)Context;
    HTC_SEND_FULL_ACTION    action = HTC_SEND_FULL_KEEP;
    A_BOOL                  stopNet = FALSE;
    HTC_ENDPOINT_ID         Endpoint = HTC_GET_ENDPOINT_FROM_PKT(pPacket);

    do {

        if (bypasswmi) {
            /* for endpointping testing no other checks need to be made
             * we can however still allow the network to stop */
            stopNet = TRUE;
            break;
        }

        if (Endpoint == ar->arControlEp) {
                /* under normal WMI if this is getting full, then something is running rampant
                 * the host should not be exhausting the WMI queue with too many commands
                 * the only exception to this is during testing using endpointping */
            AR6000_SPIN_LOCK(&ar->arLock, 0);
                /* set flag to handle subsequent messages */
            ar->arWMIControlEpFull = TRUE;
            AR6000_SPIN_UNLOCK(&ar->arLock, 0);
            AR_DEBUG_PRINTF("WMI Control Endpoint is FULL!!! \n");
                /* no need to stop the network */
            stopNet = FALSE;
            break;
        }

        /* if we get here, we are dealing with data endpoints getting full */

        if (HTC_GET_TAG_FROM_PKT(pPacket) == AR6K_CONTROL_PKT_TAG) {
            /* don't drop control packets issued on ANY data endpoint */
            break;
        }

        if (ar->arNetworkType == ADHOC_NETWORK) {
            /* in adhoc mode, we cannot differentiate traffic priorities so there is no need to
             * continue, however we should stop the network */
            stopNet = TRUE;
            break;
        }

        if ((ar->arAcStreamPriMap[arEndpoint2Ac(ar,Endpoint)] < ar->arHiAcStreamActivePri) || (ar->arAcStreamPriMap[arEndpoint2Ac(ar,Endpoint)] < ar->arHiRXAcStreamActivePri)) {
                /* this stream's priority is less than the highest active priority, we
                 * give preference to the highest priority stream by directing
                 * HTC to drop the packet that overflowed */
            action = HTC_SEND_FULL_DROP;
                /* since we are dropping packets, no need to stop the network */
            stopNet = FALSE;
            break;
        }

    } while (FALSE);

    if (stopNet) {
        AR6000_SPIN_LOCK(&ar->arLock, 0);
        ar->arNetQueueStopped = TRUE;
        AR6000_SPIN_UNLOCK(&ar->arLock, 0);
        /* one of the data endpoints queues is getting full..need to stop network stack
         * the queue will resume in ar6000_tx_complete() */
        netif_stop_queue(ar->arNetDev);
    }

    return action;
#else
	return 1;
#endif	//bluebird
}
#endif



static void
ar6000_tx_complete(void *Context, HTC_PACKET *pPacket)
{
#if 0 //bluebird

	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#else

	AR_SOFTC_T     *ar = (AR_SOFTC_T *)Context;
	void           *cookie = (void *)pPacket->pPktContext;
#if 0 //bluebird
	struct sk_buff *skb = NULL;
#else
	struct ndis_mini_buf_t * skb = NULL; // 
#endif
	A_UINT32        mapNo = 0;
	A_STATUS        status;
	struct ar_cookie * ar_cookie;
	HTC_ENDPOINT_ID   eid;
	A_BOOL          wakeEvent = FALSE;

	status = pPacket->Status;
	ar_cookie = (struct ar_cookie *)cookie;
#if 0	
	skb = (struct sk_buff *)ar_cookie->arc_bp[0];
#else
	skb = (struct ndis_mini_buf_t *)ar_cookie->arc_bp[0];
#endif

	eid = pPacket->Endpoint ;
	mapNo = ar_cookie->arc_bp[1];

	A_ASSERT(skb);
	A_ASSERT(pPacket->pBuffer == A_NETBUF_DATA(skb));

	if (A_SUCCESS(status)) {
	    A_ASSERT(pPacket->ActualLength == A_NETBUF_LEN(skb));
	}

	RETAILMSG(DBG_TRACE,(_P"ar6000_tx_complete skb=0x%x data=0x%x len=0x%x eid=%d ",
							(A_UINT32)skb, (A_UINT32)pPacket->pBuffer,
							pPacket->ActualLength,
							eid));

	/* lock the driver as we update internal state */
	AR6000_SPIN_LOCK();

	ar->arTxPending[eid]--;

	if ((eid  != ar->arControlEp) || bypasswmi) 
	{
		ar->arTotalTxDataPending--;
	}

	if (eid == ar->arControlEp)
	{
		if (ar->arWMIControlEpFull) 
		{
			/* since this packet completed, the WMI EP is no longer full */
			ar->arWMIControlEpFull = FALSE;
		}

		if (ar->arTxPending[eid] == 0) 
		{
			wakeEvent = TRUE;
		}
	}

	if (A_FAILED(status)) 
	{
		//AR6000_STAT_INC(ar, tx_errors);
		if (status != A_NO_RESOURCE) 
		{
			RETAILMSG(DBG_TRACE,(_P"%s() -TX ERROR, status: 0x%x\n", __func__, status));
		}
	} 
	else 
	{
		RETAILMSG(DBG_TRACE, (_P"%s() : Must be need to Modify \r\n", __func__));
		// AR6000_STAT_INC(ar, tx_packets);
		// ar->arNetStats.tx_bytes += A_NETBUF_LEN(skb);
	}

	/* TODO this needs to be looked at
 	*/
	if ((ar->arNetworkType == ADHOC_NETWORK) && ar->arIbssPsEnable
		&& (eid != ar->arControlEp) && mapNo)
	{
		mapNo --;
		ar->arNodeMap[mapNo].txPending --;

		if (!ar->arNodeMap[mapNo].txPending && (mapNo == (ar->arNodeNum - 1))) 
		{
			A_UINT32 i;
			for (i = ar->arNodeNum; i > 0; i --) 
			{
				if (!ar->arNodeMap[i - 1].txPending) 
				{
					A_MEMZERO(&ar->arNodeMap[i - 1], sizeof(struct ar_node_mapping));
					ar->arNodeNum --;
				} 
				else 
				{
					break;
				}
			}
		}
	}

	/* Freeing a cookie should not be contingent on either of */
	/* these flags, just if we have a cookie or not.           */
	/* Can we even get here without a cookie? Fix later.       */
	if (ar->arWmiReady == TRUE || (bypasswmi))
	{
		ar6000_free_cookie(ar, cookie);
	}

	if (ar->arNetQueueStopped) 
	{
		ar->arNetQueueStopped = FALSE;
	}

	AR6000_SPIN_UNLOCK();

    /* lock is released, we can freely call other kernel APIs */

	A_NETBUF_FREE(skb);

	if ((ar->arConnected == TRUE) || (bypasswmi)) 
	{
		if (status != A_ECANCELED) 
		{
			/* don't wake the queue if we are flushing, other wise it will just
			* keep queueing packets, which will keep failing */
		#if 0	
			netif_wake_queue(ar->arNetDev);
		#else
			RETAILMSG(1, (_P"%s() : Must be need to modify \r\n", __func__));
		#endif
		}
	}

	if (wakeEvent) 
	{
	#if 0
		wake_up(&arEvent);
	#else
		RETAILMSG(1, (_P"%s() : Must be need to modify \r\n", __func__));
	#endif 
	}
#endif //bluebird
}




static void
deliver_frames_to_nw_stack(AR_SOFTC_T *ar, struct ndis_mini_buf_t *skb)
{
#if 0 //bluebird
    A_UINT8                 rxPriority;
    if(skb) {
        if ((skb->dev->flags & IFF_UP) == IFF_UP) {
#ifdef ANDROID_ENV
            android_ar6k_check_wow_status((AR_SOFTC_T*)netdev_priv(skb->dev), skb, FALSE);
#endif
            skb->protocol = eth_type_trans(skb, skb->dev);

        rxPriority = wmi_get_traffic_class(skb, 0, ar->arWmmEnabled);
        if ((rxPriority > WMM_AC_BK) && ((rxPriority > ar->arHiAcStreamActivePri) || (((rxPriority > ar->arHiAcStreamActivePri)) && (rxPriority > ar->arHiRXAcStreamActivePri))))
        {
            ar->arHiRXAcStreamActivePri = rxPriority;
            A_UNTIMEOUT(&ar->rxPriorityTimer);				
            A_TIMEOUT_MS(&ar->rxPriorityTimer, 20, 0);			
        }

        /*
         * If this routine is called on a ISR (Hard IRQ) or DSR (Soft IRQ)
         * or tasklet use the netif_rx to deliver the packet to the stack
         * netif_rx will queue the packet onto the receive queue and mark
         * the softirq thread has a pending action to complete. Kernel will 
         * schedule the softIrq kernel thread after processing the DSR.
         *
         * If this routine is called on a process context, use netif_rx_ni
         * which will schedle the softIrq kernel thread after queuing the packet.
         */
            if (in_interrupt()) {
                netif_rx(skb);
            } else {
                netif_rx_ni(skb);
            }
        } else {
            A_NETBUF_FREE(skb);
        }
    }
#endif //bluebird	
}




#if 0
void
ar6000_rx_refill(void *Context, HTC_ENDPOINT_ID Endpoint)
{


#if 1 //bluebird
	AR_SOFTC_T  *ar = (AR_SOFTC_T *)Context;

	int         RxBuffers;
	int         buffersToRefill;
	HTC_PACKET  *pPacket;


	buffersToRefill = (int)AR6000_MAX_RX_BUFFERS - (int)ar->arRxBuffers[Endpoint];

	if (buffersToRefill <= 0) 
	{
		/* fast return, nothing to fill */
		RETAILMSG(DBG_TRACE, (_P" %s() : buffersToRefill <= 0 !! \r\n", __func__));	
		return;
	}

	RETAILMSG(0,(_P"%s(): providing htc with %d buffers at eid=%d\n",__func__, buffersToRefill, Endpoint));

	for (RxBuffers = 0; RxBuffers < buffersToRefill; RxBuffers++) 
	{
		void        *osBuf;
		
		osBuf = A_NETBUF_ALLOC(AR6000_BUFFER_SIZE);

		if (NULL == osBuf) 
		{
			break;
		}
		
		/*	the HTC packet wrapper is at the head of the reserved area
		* 	linxue --> in the skb, wince --> in ndis_mini_buf_t  */
		
		pPacket = 	(HTC_PACKET *)A_NETBUF_HEAD(osBuf);
		/* set re-fill info */
		SET_HTC_PACKET_INFO_RX_REFILL(pPacket,osBuf,A_NETBUF_DATA(osBuf),AR6000_BUFFER_SIZE,Endpoint);		

		
		/* add this packet */
		HTCAddReceivePkt(ar->arHtcTarget, pPacket);
    }

	/* update count */
	AR6000_SPIN_LOCK();
	ar->arRxBuffers[Endpoint] += RxBuffers;
	AR6000_SPIN_UNLOCK();
#else
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#endif //bluebird	
}

#endif


#if 0
A_UINT32
a_copy_to_user(void *to, const void *from, A_UINT32 n)
{
    return(copy_to_user(to, from, n));
}
#endif



A_UINT32
a_copy_from_user(void *to, const void *from, A_UINT32 n)
{
#if 0 //bluebird
    return(copy_from_user(to, from, n));
#else
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
	return -1;
#endif //bluebird
}



void
ar6000_bssInfo_event_rx(AR_SOFTC_T *ar, A_UINT8 *datap, int len)
{

    struct ndis_mini_buf_t *skb;
    WMI_BSS_INFO_HDR *bih = (WMI_BSS_INFO_HDR *)datap;


    if (!ar->arMgmtFilter) {
        return;
    }
    if (((ar->arMgmtFilter & IEEE80211_FILTER_TYPE_BEACON) &&
        (bih->frameType != BEACON_FTYPE))  ||
        ((ar->arMgmtFilter & IEEE80211_FILTER_TYPE_PROBE_RESP) &&
        (bih->frameType != PROBERESP_FTYPE)))
    {
        return;
    }

    if ((skb = A_NETBUF_ALLOC_RAW(len)) != NULL) 
	{
#if 0
        A_NETBUF_PUT(skb, len);
        A_MEMCPY(A_NETBUF_DATA(skb), datap, len);
        skb->dev = ar->arNetDev;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
        A_MEMCPY(skb_mac_header(skb), A_NETBUF_DATA(skb), 6);
#else
        skb->mac.raw = A_NETBUF_DATA(skb);
#endif
        skb->ip_summed = CHECKSUM_NONE;
        skb->pkt_type = PACKET_OTHERHOST;
        skb->protocol = __constant_htons(0x0019);
        netif_rx(skb);
#else
		RETAILMSG(1, (_P" %s() is not ready yet  ########### !! \r\n", __func__));
#endif		
    }

}




A_STATUS
ar6000_get_driver_cfg(struct net_device *dev,
                        A_UINT16 cfgParam,
                        void *result)
{

    A_STATUS    ret = 0;


#if 1 //bluebird
    switch(cfgParam)
    {
        case AR6000_DRIVER_CFG_GET_WLANNODECACHING:
           *((A_UINT32 *)result) = wlanNodeCaching;
           break;
        case AR6000_DRIVER_CFG_LOG_RAW_WMI_MSGS:
           *((A_UINT32 *)result) = logWmiRawMsgs;
            break;
        default:
           ret = A_ERROR;
           break;
    }
#else
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#endif //bluebird

    return ret;
}



void
ar6000_pmkid_list_event(void *devt, A_UINT8 numPMKID, WMI_PMKID *pmkidList,
                        A_UINT8 *bssidList)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    A_UINT8 i, j;

    A_PRINTF("Number of Cached PMKIDs is %d\n", numPMKID);

    for (i = 0; i < numPMKID; i++) {
        A_PRINTF("\nBSSID %d ", i);
            for (j = 0; j < ATH_MAC_LEN; j++) {
                A_PRINTF("%2.2x", bssidList[j]);
            }
        bssidList += (ATH_MAC_LEN + WMI_PMKID_LEN);
        A_PRINTF("\nPMKID %d ", i);
            for (j = 0; j < WMI_PMKID_LEN; j++) {
                A_PRINTF("%2.2x", pmkidList->pmkid[j]);
            }
        pmkidList = (WMI_PMKID *)((A_UINT8 *)pmkidList + ATH_MAC_LEN +
                                  WMI_PMKID_LEN);
    }
#endif //bluebird	
}



void ar6000_pspoll_event(AR_SOFTC_T *ar,A_UINT8 aid)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    sta_t *conn=NULL;
    A_BOOL isPsqEmpty = FALSE;

    conn = ieee80211_find_conn_for_aid(ar, aid);

    /* If the PS q for this STA is not empty, dequeue and send a pkt from
     * the head of the q. Also update the More data bit in the WMI_DATA_HDR
     * if there are more pkts for this STA in the PS q. If there are no more
     * pkts for this STA, update the PVB for this STA.
     */
    A_MUTEX_LOCK(&conn->psqLock);
    isPsqEmpty  = A_NETBUF_QUEUE_EMPTY(&conn->psq);
    A_MUTEX_UNLOCK(&conn->psqLock);

    if (isPsqEmpty) {
        /* TODO:No buffered pkts for this STA. Send out a NULL data frame */
    } else {
        struct sk_buff *skb = NULL;

        A_MUTEX_LOCK(&conn->psqLock);
        skb = A_NETBUF_DEQUEUE(&conn->psq);
        A_MUTEX_UNLOCK(&conn->psqLock);
        /* Set the STA flag to PSPolled, so that the frame will go out */
        STA_SET_PS_POLLED(conn);
        ar6000_data_tx(skb, ar->arNetDev);
        STA_CLR_PS_POLLED(conn);

        /* Clear the PVB for this STA if the queue has become empty */
        A_MUTEX_LOCK(&conn->psqLock);
        isPsqEmpty  = A_NETBUF_QUEUE_EMPTY(&conn->psq);
        A_MUTEX_UNLOCK(&conn->psqLock);

        if (isPsqEmpty) {
            wmi_set_pvb_cmd(ar->arWmi, conn->aid, 0);
        }
    }
#endif //bluebird
}



void ar6000_dtimexpiry_event(AR_SOFTC_T *ar)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    A_BOOL isMcastQueued = FALSE;
    struct sk_buff *skb = NULL;

    /* If there are no associated STAs, ignore the DTIM expiry event.
     * There can be potential race conditions where the last associated
     * STA may disconnect & before the host could clear the 'Indicate DTIM'
     * request to the firmware, the firmware would have just indicated a DTIM
     * expiry event. The race is between 'clear DTIM expiry cmd' going
     * from the host to the firmware & the DTIM expiry event happening from
     * the firmware to the host.
     */
    if (ar->sta_list_index == 0) {
        return;
    }

    A_MUTEX_LOCK(&ar->mcastpsqLock);
    isMcastQueued = A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq);
    A_MUTEX_UNLOCK(&ar->mcastpsqLock);

    A_ASSERT(isMcastQueued == FALSE);

    /* Flush the mcast psq to the target */
    /* Set the STA flag to DTIMExpired, so that the frame will go out */
    ar->DTIMExpired = TRUE;

    A_MUTEX_LOCK(&ar->mcastpsqLock);
    while (!A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq)) {
        skb = A_NETBUF_DEQUEUE(&ar->mcastpsq);
        A_MUTEX_UNLOCK(&ar->mcastpsqLock);

        ar6000_data_tx(skb, ar->arNetDev);

        A_MUTEX_LOCK(&ar->mcastpsqLock);
    }
    A_MUTEX_UNLOCK(&ar->mcastpsqLock);

    /* Reset the DTIMExpired flag back to 0 */
    ar->DTIMExpired = FALSE;

    /* Clear the LSB of the BitMapCtl field of the TIM IE */
    wmi_set_pvb_cmd(ar->arWmi, MCAST_AID, 0);
	
#endif

}



void
ar6000_wow_list_event(struct ar6_softc *ar, A_UINT8 num_filters, WMI_GET_WOW_LIST_REPLY *wow_reply)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    A_UINT8 i,j;

    /*Each event now contains exactly one filter, see bug 26613*/
    A_PRINTF("WOW pattern %d of %d patterns\n", wow_reply->this_filter_num,                 wow_reply->num_filters);
    A_PRINTF("wow mode = %s host mode = %s\n",
            (wow_reply->wow_mode == 0? "disabled":"enabled"),
            (wow_reply->host_mode == 1 ? "awake":"asleep"));


    /*If there are no patterns, the reply will only contain generic
      WoW information. Pattern information will exist only if there are
      patterns present. Bug 26716*/

   /* If this event contains pattern information, display it*/
    if (wow_reply->this_filter_num) {
        i=0;
        A_PRINTF("id=%d size=%d offset=%d\n",
                    wow_reply->wow_filters[i].wow_filter_id,
                    wow_reply->wow_filters[i].wow_filter_size,
                    wow_reply->wow_filters[i].wow_filter_offset);
       A_PRINTF("wow pattern = ");
       for (j=0; j< wow_reply->wow_filters[i].wow_filter_size; j++) {
             A_PRINTF("%2.2x",wow_reply->wow_filters[i].wow_filter_pattern[j]);
        }

        A_PRINTF("\nwow mask = ");
        for (j=0; j< wow_reply->wow_filters[i].wow_filter_size; j++) {
            A_PRINTF("%2.2x",wow_reply->wow_filters[i].wow_filter_mask[j]);
        }
        A_PRINTF("\n");
    }
#endif //bluebird	
}




HTC_ENDPOINT_ID
ar6000_ac2_endpoint_id ( void * devt, A_UINT8 ac)
{
	
#if 1 //bluebird
    AR_SOFTC_T *ar = (AR_SOFTC_T *) devt;
    return(arAc2EndpointID(ar, ac));
#else
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
	return -1;
#endif
}


void
ar6000_dbglog_init_done(AR_SOFTC_T *ar)
{

    ar->dbglog_init_done = TRUE;

}



void
ar6000_dbglog_event(AR_SOFTC_T *ar, A_UINT32 dropped,
                    A_INT8 *buffer, A_UINT32 length)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
#ifdef REPORT_DEBUG_LOGS_TO_APP
    #define MAX_WIRELESS_EVENT_SIZE 252
    /*
     * Break it up into chunks of MAX_WIRELESS_EVENT_SIZE bytes of messages.
     * There seems to be a limitation on the length of message that could be
     * transmitted to the user app via this mechanism.
     */
    A_UINT32 send, sent;

    sent = 0;
    send = dbglog_get_debug_fragment(&buffer[sent], length - sent,
                                     MAX_WIRELESS_EVENT_SIZE);
    while (send) {
        ar6000_send_event_to_app(ar, WMIX_DBGLOG_EVENTID, &buffer[sent], send);
        sent += send;
        send = dbglog_get_debug_fragment(&buffer[sent], length - sent,
                                         MAX_WIRELESS_EVENT_SIZE);
    }
#else
    AR_DEBUG_PRINTF("Dropped logs: 0x%x\nDebug info length: %d\n",
                    dropped, length);

    /* Interpret the debug logs */
    dbglog_parse_debug_logs(buffer, length);
#endif /* REPORT_DEBUG_LOGS_TO_APP */
#endif //bluebird
}



/* Init cookie queue */
static void
ar6000_cookie_init(AR_SOFTC_T *ar)
{

	A_UINT32    i;

	RETAILMSG(DBG_TRACE, (_P" %s() + Enter \r\n", __func__));

	ar->arCookieList = NULL;
	A_MEMZERO(s_ar_cookie_mem, sizeof(s_ar_cookie_mem));

	for (i = 0; i < MAX_COOKIE_NUM; i++) 
	{
		ar6000_free_cookie(ar, &s_ar_cookie_mem[i]);
	}
	
}


/* cleanup cookie queue */
static void
ar6000_cookie_cleanup(AR_SOFTC_T *ar)
{
    /* It is gone .... */
    ar->arCookieList = NULL;
}


/* Init cookie queue */
void
ar6000_free_cookie(AR_SOFTC_T *ar, struct ar_cookie * cookie)
{
	/* Insert first */
	A_ASSERT(ar != NULL);
	A_ASSERT(cookie != NULL);
	cookie->arc_list_next = ar->arCookieList;
	ar->arCookieList = cookie;
}


/* cleanup cookie queue */
static struct ar_cookie *
ar6000_alloc_cookie(AR_SOFTC_T  *ar)
{
    struct ar_cookie   *cookie;

    cookie = ar->arCookieList;
    if(cookie != NULL)
    {
        ar->arCookieList = cookie->arc_list_next;
    }

    return cookie;
}



void
ar6000_snrThresholdEvent_rx(void *devt, WMI_SNR_THRESHOLD_VAL newThreshold, A_UINT8 snr)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    WMI_SNR_THRESHOLD_EVENT event;
    AR_SOFTC_T *ar = (AR_SOFTC_T *)devt;

    event.range = newThreshold;
    event.snr = snr;

    ar6000_send_event_to_app(ar, WMI_SNR_THRESHOLD_EVENTID, (A_UINT8 *)&event,
                             sizeof(WMI_SNR_THRESHOLD_EVENT));
#endif //bluebird	
}



void
ar6000_lqThresholdEvent_rx(void *devt, WMI_LQ_THRESHOLD_VAL newThreshold, A_UINT8 lq)
{
#if 0 //bluebird
    AR_DEBUG2_PRINTF("lq threshold range %d, lq %d\n", newThreshold, lq);
#endif //bluebird
}



void
ar6000_tx_retry_err_event(void *devt)
{
#if 0 //bluebird
    AR_DEBUG2_PRINTF("Tx retries reach maximum!\n");
#endif //bluebird
}



void
ar6000_hbChallengeResp_event(AR_SOFTC_T *ar, A_UINT32 cookie, A_UINT32 source)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0	//bluebird
    if (source == APP_HB_CHALLENGE) {
        /* Report it to the app in case it wants a positive acknowledgement */
        ar6000_send_event_to_app(ar, WMIX_HB_CHALLENGE_RESP_EVENTID,
                                 (A_UINT8 *)&cookie, sizeof(cookie));
    } else {
        /* This would ignore the replys that come in after their due time */
        if (cookie == ar->arHBChallengeResp.seqNum) {
            ar->arHBChallengeResp.outstanding = FALSE;
        }
    }
#endif //bluebird	
}


void
ar6000_channel_change_event(AR_SOFTC_T *ar, A_UINT16 oldChannel,
                            A_UINT16 newChannel)
{

//    A_PRINTF("Channel Change notification\nOld Channel: %d, New Channel: %d\n",
//             oldChannel, newChannel);
}



void
ar6000_keepalive_rx(void *devt, A_UINT8 configured)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0 //bluebird
    AR_SOFTC_T *ar = (AR_SOFTC_T *)devt;

    ar->arKeepaliveConfigured = configured;
    wake_up(&arEvent);
#endif //bluebird	
}



void
ar6000_ratemask_rx(void *devt, A_UINT16 ratemask)
{
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#if 0	//bluebird
    AR_SOFTC_T *ar = (AR_SOFTC_T *)devt;

    ar->arRateMask = ratemask;
    wake_up(&arEvent);
#endif //bluebird	
}



/* indicate tx activity or inactivity on a WMI stream */
void ar6000_indicate_tx_activity(void *devt, A_UINT8 TrafficClass, A_BOOL Active)
{

	
#if 1	//bluebird
    AR_SOFTC_T  *ar = (AR_SOFTC_T *)devt;
    HTC_ENDPOINT_ID eid ;
    int i;

	RETAILMSG(DBG_TRACE, (_P" %s() + Enter \r\n", __func__));
		
    if (ar->arWmiEnabled) {
        eid = arAc2EndpointID(ar, TrafficClass);

        AR6000_SPIN_LOCK();

        ar->arAcStreamActive[TrafficClass] = Active;

        if (Active) {
            /* when a stream goes active, keep track of the active stream with the highest priority */

            if (ar->arAcStreamPriMap[TrafficClass] > ar->arHiAcStreamActivePri) {
                    /* set the new highest active priority */
                ar->arHiAcStreamActivePri = ar->arAcStreamPriMap[TrafficClass];
            }

        } else {
            /* when a stream goes inactive, we may have to search for the next active stream
             * that is the highest priority */

            if (ar->arHiAcStreamActivePri == ar->arAcStreamPriMap[TrafficClass]) {

                /* the highest priority stream just went inactive */

                    /* reset and search for the "next" highest "active" priority stream */
                ar->arHiAcStreamActivePri = 0;
                for (i = 0; i < WMM_NUM_AC; i++) {
                    if (ar->arAcStreamActive[i]) {
                        if (ar->arAcStreamPriMap[i] > ar->arHiAcStreamActivePri) {
                            /* set the new highest active priority */
                            ar->arHiAcStreamActivePri = ar->arAcStreamPriMap[i];
                        }
                    }
                }
            }
        }

        AR6000_SPIN_UNLOCK();

    } else {
            /* for mbox ping testing, the traffic class is mapped directly as a stream ID,
             * see handling of AR6000_XIOCTL_TRAFFIC_ACTIVITY_CHANGE in ioctl.c */
        eid = (HTC_ENDPOINT_ID)TrafficClass;
    }

        /* notify HTC, this may cause credit distribution changes */

    HTCIndicateActivityChange(ar->arHtcTarget,
                              eid,
                              Active);
#else
	RETAILMSG(DBG_TRACE, (_P" %s() is not ready yet~ \r\n", __func__));
#endif	//bluebird
}


void ar6000_peer_event(
    void *context,
    A_UINT8 eventCode,
    A_UINT8 *macAddr)
{
    A_UINT8 pos=0;

    //for (pos=0;pos<6;pos++)
    //    printk("%02x: ",*(macAddr+pos));
    //printk("\n");
}


static int checkforDHCPPacket(struct ndis_mini_buf_ *skb) 
{
   DHCP_PACKET *dhcpPacket;

	skb->buf_dat = NULL;
	
   dhcpPacket = (DHCP_PACKET*)skb->buf_dat;
    if((dhcpPacket->dhcpOptions[0] == ATH_DHCP_OPCODE_MSG_TYPE) && (dhcpPacket->dhcpOptions[1] == ATH_DHCP_MSG_TYPE_LEN)) {
       if(dhcpPacket->dhcpOptions[2] == ATH_DHCP_DISCOVER) {
          return ATH_DHCP_DISCOVER;
       }
       else if (dhcpPacket->dhcpOptions[2] == ATH_DHCP_REQUEST){
          return ATH_DHCP_REQUEST;
       }
       else if (dhcpPacket->dhcpOptions[2] == ATH_DHCP_ACK) {
          return ATH_DHCP_ACK;
       } 
    }
    return ATH_DHCP_INVALID_MSG;
}


static void ar6000_detect_error(void *ptr)
{

    AR_SOFTC_T *ar = (AR_SOFTC_T *)ptr;
    WMI_TARGET_ERROR_REPORT_EVENT errEvent;

    AR6000_SPIN_LOCK();

    if (ar->arHBChallengeResp.outstanding) {
        ar->arHBChallengeResp.missCnt++;
    } else {
        ar->arHBChallengeResp.missCnt = 0;
    }

    if (ar->arHBChallengeResp.missCnt > ar->arHBChallengeResp.missThres) {
        /* Send Error Detect event to the application layer and do not reschedule the error detection module timer */
        ar->arHBChallengeResp.missCnt = 0;
        ar->arHBChallengeResp.seqNum = 0;
        errEvent.errorVal = WMI_TARGET_COM_ERR | WMI_TARGET_FATAL_ERR;
        AR6000_SPIN_UNLOCK();
#if 0		
        ar6000_send_event_to_app(ar, WMI_ERROR_REPORT_EVENTID,
                                 (A_UINT8 *)&errEvent,
                                 sizeof(WMI_TARGET_ERROR_REPORT_EVENT));
#endif
        return;
    }

    /* Generate the sequence number for the next challenge */
    ar->arHBChallengeResp.seqNum++;
    ar->arHBChallengeResp.outstanding = TRUE;

    AR6000_SPIN_UNLOCK();

    /* Send the challenge on the control channel */
    if (wmi_get_challenge_resp_cmd(ar->arWmi, ar->arHBChallengeResp.seqNum, DRV_HB_CHALLENGE) != A_OK) {
        AR_DEBUG_PRINTF("Unable to send heart beat challenge\n");
    }


    /* Reschedule the timer for the next challenge */
    A_TIMEOUT_MS(&ar->arHBChallengeResp.timer, ar->arHBChallengeResp.frequency * 1000, 0);
}




