/* hw.c - access hardware for dk_client */

/* Copyright (c) 2001 Atheros Communications, Inc., All Rights Reserved */

/*
DESCRIPTION
This module contains the functions to access the hardware for the dk_client
The calls made by this module assumes linux dk driver interface. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <assert.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>


#include "dk_common.h"
#include "hw.h"
#include "dk_mem.h"
#include "ar5210reg.h"
#include "hwext.h"
#include "dk_client.h"
#include "dk_ioctl.h"

#include "linux_ansi.h"

#ifdef PERL_CORE
#include "dk_master.h"
#endif

#define NETWORK_CLASS   0x2
#define SIMPLE_COMM_CLASS       0x7



/////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
A_UINT16    driverOpen();
void freeDevInfo(MDK_WLAN_DEV_INFO *pdevInfo);
MDK_WLAN_DEV_INFO *createDevInfo(void);
void 		sig_handler(int arg);

/////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
volatile A_BOOL inSignalHandler = FALSE;
MDK_WLAN_DRV_INFO	globDrvInfo;				
A_UINT16 minorVersion;
A_UINT16 majorVersion;

/**************************************************************************
* envInit - performs any intialization needed by the environment
*
* For the windows NT hardware environment, need to open the driver and
* perform any other initialization required by it
*
* RETURNS: A_OK if it works, A_ERROR if not
*/
A_STATUS envInit
(
    A_BOOL debugMode
)
{
    A_UINT16 i;

    // quiet our optional printfs...
    dk_quiet(debugMode ? 0 : 1);

    // open a handle to the driver
    if ( !driverOpen() )
        return A_ERROR;

	ansi_init();

    globDrvInfo.devCount = 0;

    // set all the devInfo pointers to null
    for(i = 0; i < WLAN_MAX_DEV; i++)
        globDrvInfo.pDevInfoArray[i] = NULL;

    // register our handler for CTRL-C, etc.
	signal(SIGINT,sig_handler);
}

void envCleanup
(
    A_BOOL closeDriver
)
{
    A_UINT16 i;
    MDK_WLAN_DEV_INFO *pdevInfo;

    // cleanup all the devInfo structures
    for ( i = 0; i < WLAN_MAX_DEV; i++ ) {
		pdevInfo = globDrvInfo.pDevInfoArray[i];
        if ( pdevInfo ) {
			deviceCleanup(i);
        }
    }
    
    globDrvInfo.devCount = 0;

    // close the handle to the driver
    if ( closeDriver ) {
    }

}


void sig_handler
(
 	int arg
) 
{
		uiPrintf("Received SIGINT !! cleanup and close down ! \n");
#ifdef PERL_CORE
		dkPerlCleanup();
#else
		envCleanup(TRUE);
#endif
		exit(0);
}

/**************************************************************************
* driverOpen - Only needed in NT_HW environment to open comms with driver
*
* Called within the NT_HW environment. Opens and gets a handle to the 
* driver for doing the hardware access. 
*
* RETURNS: 1 if OK and driver open, 0 if not
*/

A_UINT16 driverOpen()
{
    return 1;
}

/**************************************************************************
* deviceInit - performs any initialization needed for a device
*
* Perform the initialization needed for a device.  This includes creating a 
* devInfo structure and initializing its contents
*
* RETURNS: A_OK if successful, A_ERROR if not
*/
A_STATUS deviceInit
(
    A_UINT16 devIndex, /* index of globalDrvInfo which to add device to */                     
    A_UINT16 device_fn
)
{
    MDK_WLAN_DEV_INFO *pdevInfo;
    A_UINT32      regValue;
	A_UINT32      mapAddress;
	A_INT32 	  handle;
	A_UINT32 	  *vir_addr;
	struct client_info cliInfo;
	char 		dev_name[16];
	A_UINT32        NumBuffBlocks;
	A_UINT32        NumBuffMapBytes;
    A_UINT32    version, iIndex;


   	if (globDrvInfo.pDevInfoArray[devIndex]) {
		uiPrintf("Error: Device already in use \n");
       	return A_ERROR;
   	}
 
    	pdevInfo = createDevInfo();
	if (pdevInfo == NULL) {
        uiPrintf("Error: Unable to create dev info structure !\n");
        return(A_ERROR);
	}
	
	/* open the device based on the device index */
	if (devIndex > (WLAN_MAX_DEV-1)) {
		freeDevInfo(pdevInfo);
        uiPrintf("Error: Only 4 devices/functions supported !\n");
        return(A_ERROR);
	}
	
	if (device_fn == WMAC_FUNCTION) {
	   strcpy(dev_name,"/dev/dk");
	   dev_name[7]='0'+devIndex;
	   dev_name[8]='\0';
	}
	else {
	   strcpy(dev_name,"/dev/dk_uart");
	   dev_name[12]='0'+ (devIndex - (device_fn * MDK_MAX_NUM_DEVICES));
	   dev_name[13]='\0';
	}
	uiPrintf("Open dev %s\n", dev_name);
	handle = open(dev_name,O_RDWR);
	if (handle <  0) {
		freeDevInfo(pdevInfo);
        uiPrintf("Error: Unable to open the device !\n");
        return(A_ERROR);
	}

	if (ioctl(handle,DK_IOCTL_GET_VERSION,&version) < 0) {
		close(handle);
		freeDevInfo(pdevInfo);
        uiPrintf("Error: get version ioctl failed !\n");
        return(A_ERROR);
	}

    minorVersion = version & 0xffff;
    majorVersion = (version>>16) & 0xffff;
	if (majorVersion != DRV_MAJOR_VERSION) {
		close(handle);
		freeDevInfo(pdevInfo);
		uiPrintf("Error: Driver (%d) and application (%d) version mismatch::version=%x \n",  majorVersion, DRV_MAJOR_VERSION, version);
        return(A_ERROR);
	}

	if (ioctl(handle,DK_IOCTL_GET_CLIENT_INFO,&cliInfo) < 0) {
		close(handle);
		freeDevInfo(pdevInfo);
        uiPrintf("Error: get version ioctl failed !\n");
        return(A_ERROR);
    }

	pdevInfo->handle = handle;

	pdevInfo->pdkInfo->f2Mapped = 1;
	pdevInfo->pdkInfo->devMapped = 1;
	pdevInfo->pdkInfo->haveEvent = 0;
	pdevInfo->pdkInfo->devIndex = devIndex;

	pdevInfo->pdkInfo->f2MapAddress = cliInfo.reg_phy_addr;
	pdevInfo->pdkInfo->regMapRange = cliInfo.reg_range;
	pdevInfo->pdkInfo->memPhyAddr = cliInfo.mem_phy_addr;
    pdevInfo->pdkInfo->numBars  = cliInfo.numBars;
    pdevInfo->pdkInfo->device_class  = cliInfo.device_class;


    if (minorVersion >= 2) {
       for(iIndex=0;iIndex<cliInfo.numBars;iIndex++) {
	     pdevInfo->pdkInfo->aregPhyAddr[iIndex] = (A_UINT32)cliInfo.areg_phy_addr[iIndex];
	     vir_addr = (A_UINT32 *)mmap((char *)0,cliInfo.areg_range[iIndex],PROT_READ|PROT_WRITE,MAP_SHARED,handle,cliInfo.areg_phy_addr[iIndex]);
	     if (vir_addr == NULL) {
		   close(handle);
		   freeDevInfo(pdevInfo);
		   uiPrintf("Error: Cannot map the device registers in user address space \n");
		   return A_ERROR;
			
	     }
        pdevInfo->pdkInfo->aregRange[iIndex] = cliInfo.areg_range[iIndex];
	     pdevInfo->pdkInfo->aregVirAddr[iIndex] = (A_UINT32)vir_addr;
       } // end of fo4
	   pdevInfo->pdkInfo->regVirAddr = pdevInfo->pdkInfo->aregVirAddr[0];
    }
    else {
        pdevInfo->pdkInfo->aregPhyAddr[0] = cliInfo.reg_phy_addr;
	     vir_addr = (A_UINT32 *)mmap((char *)0,cliInfo.reg_range,PROT_READ|PROT_WRITE,MAP_SHARED,handle,cliInfo.reg_phy_addr);
	     if (vir_addr == NULL) {
		   close(handle);
		   freeDevInfo(pdevInfo);
		   uiPrintf("Error: Cannot map the device registers in user address space \n");
		   return A_ERROR;
			
	     }
        pdevInfo->pdkInfo->aregVirAddr[0] = (A_UINT32)  vir_addr;
        pdevInfo->pdkInfo->aregRange[0] = cliInfo.reg_range;
	    pdevInfo->pdkInfo->regVirAddr = pdevInfo->pdkInfo->aregVirAddr[0];
    }

	vir_addr = (A_UINT32 *)mmap((char *)0,cliInfo.mem_size,PROT_READ|PROT_WRITE,MAP_SHARED,handle,cliInfo.mem_phy_addr);
	if (vir_addr == NULL) {
		munmap((void *)pdevInfo->pdkInfo->regVirAddr, pdevInfo->pdkInfo->regMapRange);
		close(handle);
		freeDevInfo(pdevInfo);
		uiPrintf("Error: Cannot map memory in user address space \n");
		return A_ERROR;
	}
	pdevInfo->pdkInfo->memVirAddr = (A_UINT32)vir_addr;
	pdevInfo->pdkInfo->memSize = cliInfo.mem_size;
    
    q_uiPrintf("+ Allocated memory\n");
    q_uiPrintf("+ VirtAddress = %08x\n", (A_UINT32)(pdevInfo->pdkInfo->memVirAddr));
    q_uiPrintf("+ PhysAddress = %08x\n", (A_UINT32)(pdevInfo->pdkInfo->memPhyAddr));

	NumBuffBlocks	= pdevInfo->pdkInfo->memSize / BUFF_BLOCK_SIZE;
	NumBuffMapBytes = NumBuffBlocks / 8;

	/* initialize the map bytes for tracking memory management: calloc will init to 0 as well*/
    pdevInfo->pbuffMapBytes = (A_UCHAR *) calloc(NumBuffMapBytes, sizeof(A_UCHAR));
    if(!pdevInfo->pbuffMapBytes) {
		munmap((void *)pdevInfo->pdkInfo->memVirAddr, pdevInfo->pdkInfo->memSize);
		munmap((void *)pdevInfo->pdkInfo->regVirAddr, pdevInfo->pdkInfo->regMapRange);
		close(handle);
       	freeDevInfo(pdevInfo);
        uiPrintf("Error: Unable to allocate buffMapBytes struct!\n");
        return A_ERROR;
    }

    pdevInfo->pnumBuffs = (A_UINT16 *) calloc(NumBuffBlocks, sizeof(A_UINT16));
    if(!pdevInfo->pnumBuffs) {
		A_FREE(pdevInfo->pbuffMapBytes);
		munmap((void *)pdevInfo->pdkInfo->memVirAddr, pdevInfo->pdkInfo->memSize);
		munmap((void *)pdevInfo->pdkInfo->regVirAddr, pdevInfo->pdkInfo->regMapRange);
		close(handle);
       	freeDevInfo(pdevInfo);
        uiPrintf("Error: Unable to allocate numBuffs struct!\n");
        return A_ERROR;
    }

	globDrvInfo.pDevInfoArray[devIndex] = pdevInfo;
    globDrvInfo.devCount++;
    
  	// Setup memory window, bus mastering, & SERR
    regValue = hwCfgRead32(devIndex, F2_PCI_CMD);
    regValue |= (MEM_ACCESS_ENABLE | MASTER_ENABLE | SYSTEMERROR_ENABLE); 
	regValue &= ~MEM_WRITE_INVALIDATE; // Disable write & invalidate for our device
    hwCfgWrite32(devIndex, F2_PCI_CMD, regValue);

    regValue = hwCfgRead32(devIndex, F2_PCI_CACHELINESIZE);
    regValue = (regValue & 0xffff) | (0x40 << 8) | 0x08;
	hwCfgWrite32(devIndex, F2_PCI_CACHELINESIZE, regValue);
            
    
    return A_OK;
}

/**************************************************************************
* createDevInfo - allocates memory needed for devInfo
*
* Performs most of the mallocs needed to create the devInfo struct, including
* MDK_WLAN_DEV_INFO, DK_DEV_INFO and DK_KERNEL_INFO.  Performs any alignment
* needed on the DK_KERNEL_INFO structure so that it does not straddle a
* page boundary
* RETURNS: A pointer to devInfo if successful, null if an error
*/
MDK_WLAN_DEV_INFO *createDevInfo
(
	void
)
{
    MDK_WLAN_DEV_INFO *pdevInfo = NULL;

    pdevInfo = (MDK_WLAN_DEV_INFO *) A_MALLOC(sizeof(MDK_WLAN_DEV_INFO));
    if(!pdevInfo) {                                            
        uiPrintf("Error: Unable to allocate MDK_WLAN_DEV_INFO struct!\n");
        return(NULL);
    }

    pdevInfo->pdkInfo = (DK_DEV_INFO *) A_MALLOC(sizeof(DK_DEV_INFO));
    if(!pdevInfo->pdkInfo) {
        A_FREE(pdevInfo);
        uiPrintf("Error: Unable to allocate DK_DEV_INFO struct!\n");
        return (NULL);
    }

	//zero out the pdkInfo struct
    A_MEM_ZERO(pdevInfo->pdkInfo, sizeof(DK_DEV_INFO));

	return pdevInfo;
}

/**************************************************************************
* freeDevInfo - frees memory held by devInfo
*
* RETURNS: N/A
*/
void freeDevInfo
(
	MDK_WLAN_DEV_INFO *pdevInfo
)
{
    A_FREE(pdevInfo->pdkInfo);
    A_FREE(pdevInfo);
    
    return;
}

/**************************************************************************
* deviceCleanup - performs any memory cleanup needed for a device
*
* Perform any cleanup needed for a device.  This includes deleting any 
* memory allocated by a device, and unregistering the card with the driver
*
* RETURNS: 1 if successful, 0 if not
*/
void deviceCleanup
(
	A_UINT16 devIndex
)
{
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

    if (pdevInfo->pdkInfo->device_class == NETWORK_CLASS)  {
				
	// disable the interrupts from the hardware
    hwMemWrite32(devIndex, pdevInfo->pdkInfo->f2MapAddress + F2_IER, F2_IER_DISABLE);

    // place the hardware into reset to quiesce all transfers
    hwMemWrite32(devIndex, pdevInfo->pdkInfo->f2MapAddress + F2_RC, (F2_RC_PCI | F2_RC_F2 | F2_RC_D2 | 
        F2_RC_DMA | F2_RC_PCU) );

	munmap((void *)pdevInfo->pdkInfo->regVirAddr, pdevInfo->pdkInfo->regMapRange);
	munmap((void *)pdevInfo->pdkInfo->memVirAddr, pdevInfo->pdkInfo->memSize);

    }

	close(pdevInfo->handle);
	
	/* free the DK_DEV_INFO struct */
    A_FREE(pdevInfo->pbuffMapBytes);
    A_FREE(pdevInfo->pnumBuffs);
	freeDevInfo(pdevInfo);
	globDrvInfo.pDevInfoArray[devIndex] = NULL;        

	globDrvInfo.devCount--;
}

 

/**************************************************************************
* checkRegSpace - Check to see if an address sits in the setup register space
* 
* This internal routine checks to see if an address lies in the register space
*
* RETURNS: A_OK to signify a valid address or A_ENOENT
*/
A_STATUS checkRegSpace
(
	MDK_WLAN_DEV_INFO *pdevInfo,
	A_UINT32      address
)
{
	
	if((address >= pdevInfo->pdkInfo->f2MapAddress) && 
		(address < pdevInfo->pdkInfo->f2MapAddress + pdevInfo->pdkInfo->regMapRange)) 
		return A_OK;
	else 
		return A_ENOENT;
}
 
/**************************************************************************
* checkMemSpace - Check to see if an address sits in the setup physical memory space
* 
* This internal routine checks to see if an address lies in the physical memory space
*
* RETURNS: A_OK to signify a valid address or A_ENOENT
*/
A_STATUS checkMemSpace
(
	MDK_WLAN_DEV_INFO *pdevInfo,
	A_UINT32      address
)
{
	if((address >= (A_UINT32)pdevInfo->pdkInfo->memPhyAddr) &&
            (address < (A_UINT32)((A_UCHAR *)(pdevInfo->pdkInfo->memPhyAddr) + pdevInfo->pdkInfo->memSize)))
		return A_OK;
	else 
		return A_ENOENT;
}


/**************************************************************************
* hwCfgRead8 - read an 8 bit configuration value
*
* This routine will call into the driver to activate a 8 bit PCI configuration 
* read cycle.
*
* RETURNS: the value read
*/
A_UINT8 hwCfgRead8
(
	A_UINT16 devIndex,
	A_UINT32 address                    /* the address to read from */
)
{
	struct cfg_op co;
	A_UINT32 data;
	A_UINT8 out;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

	co.offset=address;
	co.size = 1;
	co.value = 0;

	if (ioctl(pdevInfo->handle,DK_IOCTL_CFG_READ,&co) < 0) {
		uiPrintf("Error: PCI Config read failed \n");
		data = 0xdeadbeef;
	} else { 
		data = co.value;
	} 
	out = (A_UINT8)(data & 0x000000ff);

	return out;
}

/**************************************************************************
* hwCfgRead16 - read a 16 bit value
*
* This routine will call into the driver to activate a 16 bit PCI configuration 
* read cycle.
*
* RETURNS: the value read
*/
A_UINT16 hwCfgRead16
(
	A_UINT16 devIndex,
	A_UINT32 address                    /* the address to read from */
)
{
	struct cfg_op co;
	A_UINT32 data;
	A_UINT16 out;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

	address = address & 0xfffffffe;
	co.offset=address;
	co.size = 2;
	co.value = 0;

	if (ioctl(pdevInfo->handle,DK_IOCTL_CFG_READ,&co) < 0) {
		uiPrintf("Error: PCI Config read failed \n");
		data = 0xdeadbeef;
	} else { 
		data = co.value;
	} 
	out = (A_UINT16)(data & 0x0000ffff);

	return out;
}

/**************************************************************************
* hwCfgRead32 - read a 32 bit value
*
* This routine will call into the driver to activate a 32 bit PCI configuration 
* read cycle.
*
* RETURNS: the value read
*/
A_UINT32 hwCfgRead32
(
	A_UINT16 devIndex,
	A_UINT32 address                    /* the address to read from */
)
{
	struct cfg_op co;
	A_UINT32 data;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

	address = address & 0xfffffffc;
	co.offset=address;
	co.size = 4;
	co.value = 0;

	if (ioctl(pdevInfo->handle,DK_IOCTL_CFG_READ,&co) < 0) {
		uiPrintf("Error: PCI Config read failed \n");
		data = 0xdeadbeef;
	} else { 
		data = co.value;
	} 

	return data;
}



/**************************************************************************
* hwCfgWrite8 - write an 8 bit value
*
* This routine will call into the simulation environment to activate an
* 8 bit PCI configuration write cycle
*
* RETURNS: N/A
*/
void hwCfgWrite8
(
	A_UINT16 devIndex,
	A_UINT32 address,                    /* the address to write */
	A_UINT8  value                        /* value to write */
)
{
	struct cfg_op co;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
	
	co.offset=address;
	co.size = 1;
	co.value = value;

	if (ioctl(pdevInfo->handle,DK_IOCTL_CFG_WRITE,&co) < 0) {
		uiPrintf("Error: PCI Config write failed \n");
	} 
	
	return;
}

/**************************************************************************
* hwCfgWrite16 - write a 16 bit value
*
* This routine will call into the simulation environment to activate a
* 16 bit PCI configuration write cycle
*
* RETURNS: N/A
*/
void hwCfgWrite16
(
 	A_UINT16 devIndex,
	A_UINT32 address,                    /* the address to write */
	A_UINT16  value                        /* value to write */
)
{
	struct cfg_op co;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
	
	co.offset=address;
	co.size = 2;
	co.value = value;

	if (ioctl(pdevInfo->handle,DK_IOCTL_CFG_WRITE,&co) < 0) {
		uiPrintf("Error: PCI Config write failed \n");
	} 

	return;	
}

/**************************************************************************
* hwCfgWrite32 - write a 32 bit value
*
* This routine will call into the simulation environment to activate a
* 32 bit PCI configuration write cycle
*
* RETURNS: N/A
*/
void hwCfgWrite32
(
 	A_UINT16 devIndex,
	A_UINT32 address,                    /* the address to write */
	A_UINT32 value                        /* value to write */
)
{
	struct cfg_op co;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
	
	co.offset=address;
	co.size = 4;
	co.value = value;

	if (ioctl(pdevInfo->handle,DK_IOCTL_CFG_WRITE,&co) < 0) {
		uiPrintf("Error: PCI Config write failed \n");
	}

	return;
}


/**************************************************************************
* hwMemRead8 - read an 8 bit value
*
* This routine will call into the simulation environment to activate an
* 8 bit PCI memory read cycle, value read is returned to caller
*
* RETURNS: the value read
*/
A_UINT8 hwMemRead8
(
	A_UINT16 devIndex,
	A_UINT32 address                    /* the address to read from */
)
{
	A_UINT8 *pMem;
	A_UINT16 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
    
    // check within the register regions 
	if (A_OK == checkRegSpace(pdevInfo, address))
	{
		pMem = (A_UINT8 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (address - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
		return (*pMem);
	}

	//check within memory allocation
	if(A_OK == checkMemSpace(pdevInfo, address))
	{
		pMem = (A_UINT8 *) (pdevInfo->pdkInfo->memVirAddr + 
						  (address - pdevInfo->pdkInfo->memPhyAddr));
		return(*pMem);
	}
	uiPrintf("ERROR: hwMemRead8 could not access hardware address: %08lx\n", address);
    return (0xdb);
    
}

/**************************************************************************
* hwMemRead16 - read a 16 bit value
*
* This routine will call into the simulation environment to activate a
* 16 bit PCI memory read cycle, value read is returned to caller
*
* RETURNS: the value read
*/
A_UINT16 hwMemRead16
(
	A_UINT16 devIndex,
	A_UINT32 address                    /* the address to read from */
)
{
	A_UINT16 *pMem;
    A_UINT16 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
    
    // check within the register regions 
	if (A_OK == checkRegSpace(pdevInfo, address))
	{
		pMem = (A_UINT16 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (address - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
		return (*pMem);
	}

	//check within memory allocation
	if(A_OK == checkMemSpace(pdevInfo, address))
	{
		pMem = (A_UINT16 *) (pdevInfo->pdkInfo->memVirAddr + 
						  (address - pdevInfo->pdkInfo->memPhyAddr));
		return(*pMem);
	}
 	uiPrintf("ERROR: hwMemRead16 could not access hardware address: %08lx\n", address);
    return (0xdead);
}

/**************************************************************************
* hwMemRead32 - read an 32 bit value
*
* This routine will call into the simulation environment to activate a
* 32 bit PCI memory read cycle, value read is returned to caller
*
* RETURNS: the value read
*/
A_UINT32 hwMemRead32
(
	A_UINT16 devIndex,
	A_UINT32 address                    /* the address to read from */
)
{
    A_UINT32 *pMem;
    A_UINT16 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
    
	// check within the register regions 
	if (A_OK == checkRegSpace(pdevInfo, address))
	{
		pMem = (A_UINT32 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (address - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
		return(*pMem);
	}

	//check within memory allocation
	if(A_OK == checkMemSpace(pdevInfo, address))
	{
		pMem = (A_UINT32 *) (pdevInfo->pdkInfo->memVirAddr + 
						  (address - pdevInfo->pdkInfo->memPhyAddr));
		return(*pMem);
	}
 	uiPrintf("ERROR: hwMemRead32 could not access hardware address: %08lx\n", address);
    return (0xdeadbeef);
}

/**************************************************************************
* hwMemWrite8 - write an 8 bit value
*
* This routine will call into the simulation environment to activate an
* 8 bit PCI memory write cycle
*
* RETURNS: N/A
*/
void hwMemWrite8
(
	A_UINT16 devIndex,
	A_UINT32 address,                    /* the address to write */
	A_UINT8  value                        /* value to write */
)
{
    A_UINT8 *pMem;
    A_UINT16 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
    
	// check within the register regions 
	if (A_OK == checkRegSpace(pdevInfo, address))
	{
		pMem = (A_UINT8 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (address - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
		*pMem = value;
		return;
	}

	// check within our malloc area
	if(A_OK == checkMemSpace(pdevInfo, address))
	{
		pMem = (A_UINT8 *) (pdevInfo->pdkInfo->memVirAddr + 
					  (address - pdevInfo->pdkInfo->memPhyAddr));
		*pMem = value;
		return;
	}
   	uiPrintf("ERROR: hwMemWrite8 could not access hardware address: %08lx\n", address);

}

/**************************************************************************
* hwMemWrite16 - write a 16 bit value
*
* This routine will call into the simulation environment to activate a
* 16 bit PCI memory write cycle
*
* RETURNS: N/A
*/
void hwMemWrite16
(
	A_UINT16 devIndex,
	A_UINT32 address,                    /* the address to write */
	A_UINT16  value                        /* value to write */
)
{
    A_UINT16 *pMem;
    A_UINT16 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
	// check within the register regions 
	if (A_OK == checkRegSpace(pdevInfo, address))
	{
		pMem = (A_UINT16 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (address - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
		*pMem = value;
		return;
	}

	// check within our malloc area
	if(A_OK == checkMemSpace(pdevInfo, address))
	{
		pMem = (A_UINT16 *) (pdevInfo->pdkInfo->memVirAddr + 
					  (address - pdevInfo->pdkInfo->memPhyAddr));
		*pMem = value;
		return;
	}
   	uiPrintf("ERROR: hwMemWrite16 could not access hardware address: %08lx\n", address);
}



/**************************************************************************
* hwMemWrite32 - write a 32 bit value
*
* This routine will call into the simulation environment to activate a
* 32 bit PCI memory write cycle
*
* RETURNS: N/A
*/
void hwMemWrite32
(
	A_UINT16 devIndex,
	A_UINT32 address,                    /* the address to write */
	A_UINT32  value                        /* value to write */
)
{
    A_UINT32 *pMem;
    A_UINT16 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

	// check within the register regions 
	if (A_OK == checkRegSpace(pdevInfo, address))
	{
		pMem = (A_UINT32 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (address - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
		*pMem = value;
		return;
	}

	// check within our malloc area
	if(A_OK == checkMemSpace(pdevInfo, address))
	{
		pMem = (A_UINT32 *) (pdevInfo->pdkInfo->memVirAddr + 
					  (address - pdevInfo->pdkInfo->memPhyAddr));
		*pMem = value;
		return;
	}
	uiPrintf("ERROR: hwMemWrite32 could not access hardware address: %08lx\n", address);
}

/**************************************************************************
* hwCreateEvent - Handle event creation within windows environment
*
* Create an event within windows environment
*
*
* RETURNS: 0 on success, -1 on error
*/

A_INT16 hwCreateEvent
(
	A_UINT16 devIndex,
	PIPE_CMD *pCmd
)
{
	struct event_op event;
	MDK_WLAN_DEV_INFO    *pdevInfo;

		pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

		event.valid = 1;
		event.param[0] = pCmd->CMD_U.CREATE_EVENT_CMD.type;
		event.param[1] = pCmd->CMD_U.CREATE_EVENT_CMD.persistent;
		event.param[2] = pCmd->CMD_U.CREATE_EVENT_CMD.param1;
		event.param[3] = pCmd->CMD_U.CREATE_EVENT_CMD.param2;
		event.param[4] = pCmd->CMD_U.CREATE_EVENT_CMD.param3;
		event.param[5] = (pCmd->CMD_U.CREATE_EVENT_CMD.eventHandle.f2Handle << 16) | pCmd->CMD_U.CREATE_EVENT_CMD.eventHandle.eventID;

		if (ioctl(pdevInfo->handle,DK_IOCTL_CREATE_EVENT,&event) < 0) {
			uiPrintf("Error:Create Event failed \n");
			return -1;
		}

	   return(0);
}

A_UINT16 getNextEvent
(
	MDK_WLAN_DEV_INFO *pdevInfo,
	EVENT_STRUCT *pEvent
)
{
	struct event_op event;
	A_INT32 i;

		event.valid = 0;
		pEvent->type = 0;

		if (ioctl(pdevInfo->handle,DK_IOCTL_GET_NEXT_EVENT,&event) < 0) {
			uiPrintf("Error:Get next Event failed \n");
			return (A_UINT16)-1;
		}

		if (!event.valid) {
			return 0;
		}

		pEvent->type = event.param[0];
		pEvent->persistent = event.param[1];
		pEvent->param1 = event.param[2];
		pEvent->param2 = event.param[3];
		pEvent->param3 = event.param[4];
		pEvent->eventHandle.eventID = event.param[5] & 0xffff;
		pEvent->eventHandle.f2Handle = (event.param[5] >> 16 ) & 0xffff;
		pEvent->result = event.param[6];
#ifdef MAUI
		for (i=0;i<5;i++) {
			pEvent->additionalParams[i] = event.param[7+i];
		}
#endif
		
   		 return(1);
}

A_INT16 hwGetNextEvent
(
	A_UINT16 devIndex,
	void *pBuf
)
{
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
	return getNextEvent(pdevInfo, (EVENT_STRUCT *)pBuf);
}




/**************************************************************************
* hwMemWriteBlock -  Write a block of memory within the simulation environment
*
* Write a block of memory within the simulation environment
*
*
* RETURNS: 0 on success, -1 on error
*/
A_INT16 hwMemWriteBlock
(
	A_UINT16 devIndex,
	A_UCHAR    *pBuffer,
	A_UINT32 length,
	A_UINT32 *pPhysAddr
)
{
    A_UCHAR *pMem;                /* virtual pointer to area to be written */
    A_UINT16 i;
    A_UINT32 startPhysAddr;        /* physical address of start of device memory block,
                                   for easier readability */
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];
    
    if(*pPhysAddr == 0)
    {
        return(-1);
    }

    /* first need to check that the phys address is within the allocated memory block.
       Need to make sure that the begin size and endsize match.  Will check all the 
       devices.  Only checking the memory block, will not allow registers to be accessed
       this way
     */
    
	//check start and end addresswithin memory allocation
	startPhysAddr = pdevInfo->pdkInfo->memPhyAddr;
	if((*pPhysAddr >= startPhysAddr) &&
		(*pPhysAddr <= (startPhysAddr + pdevInfo->pdkInfo->memSize)) &&
		((*pPhysAddr + length) >= startPhysAddr) &&
		((*pPhysAddr + length) <= (startPhysAddr + pdevInfo->pdkInfo->memSize))
	) { 
			/* address is within range, so can do the write */
            
			/* get the virtual pointer to start and read */
			pMem = (A_UINT8 *) (pdevInfo->pdkInfo->memVirAddr + (*pPhysAddr - pdevInfo->pdkInfo->memPhyAddr));
            memcpy(pMem, pBuffer, length);
			return(0);
     }
	// check within the register regions
    startPhysAddr = pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select];
	if ((*pPhysAddr >= startPhysAddr) &&
        (*pPhysAddr < startPhysAddr + pdevInfo->pdkInfo->aregRange[pdevInfo->pdkInfo->bar_select]) &&
        ((*pPhysAddr + length) >= startPhysAddr) &&
        ((*pPhysAddr + length) <= (startPhysAddr + pdevInfo->pdkInfo->aregRange[pdevInfo->pdkInfo->bar_select]))) {
			pMem = (A_UINT8 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (*pPhysAddr - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
            memcpy(pMem, pBuffer, length);
            return(0);
	}
    /* if got to here, then address is bad */
    uiPrintf("Warning: Address is not within legal memory range, nothing written\n");
    return(-1);
    }

/**************************************************************************
* hwMemReadBlock - Read a block of memory within the simulation environment
*
* Read a block of memory within the simulation environment
*
*
* RETURNS: 0 on success, -1 on error
*/
A_INT16 hwMemReadBlock
(
	A_UINT16 devIndex,
	A_UCHAR    *pBuffer,
	A_UINT32 physAddr,
	A_UINT32 length
)
{
    A_UCHAR *pMem;                /* virtual pointer to area to be written */
    A_UINT16 i;
    A_UINT32 startPhysAddr;        /* physical address of start of device memory block,
                                   for easier readability */
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];


    		//check start and end addresswithin memory allocation
			startPhysAddr = pdevInfo->pdkInfo->memPhyAddr;
			if((physAddr >= startPhysAddr) &&
				(physAddr <= (startPhysAddr + pdevInfo->pdkInfo->memSize)) &&
				((physAddr + length) >= startPhysAddr) &&
				((physAddr + length) <= (startPhysAddr + pdevInfo->pdkInfo->memSize))
				) { 
				/* address is within range, so can do the read */
				/* get the virtual pointer to start and read */
				pMem = (A_UINT8 *) (pdevInfo->pdkInfo->memVirAddr + (physAddr - pdevInfo->pdkInfo->memPhyAddr));
				memcpy(pBuffer, pMem, length);
				return(0);
			}
			startPhysAddr = pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select];
			if ((physAddr >= startPhysAddr) &&
                (physAddr < startPhysAddr + pdevInfo->pdkInfo->aregRange[pdevInfo->pdkInfo->bar_select]) &&
                ((physAddr + length) >= startPhysAddr) &&
                ((physAddr + length) <= (startPhysAddr + pdevInfo->pdkInfo->aregRange[pdevInfo->pdkInfo->bar_select]))) {
				pMem = (A_UINT8 *) (pdevInfo->pdkInfo->aregVirAddr[pdevInfo->pdkInfo->bar_select] + (physAddr - pdevInfo->pdkInfo->aregPhyAddr[pdevInfo->pdkInfo->bar_select]));
     		// check within the register regions
                memcpy(pBuffer, pMem, length);
                return(0);
            }

    /* if got to here, then address is bad */
    uiPrintf("Warning: Address is not within legal memory range, nothing read\n");
    return(-1);
}







/**************************************************************************
* hwGetPhysMem - get a block of physically contiguous memory
*
* This routine gets physically contiguous driver-level memory
*
* RETURNS: The physical address of the memory allocated
*/
/* #####Note may replace this with the function later that returns */
/* both physical and virtual memory of buffer. */
void *hwGetPhysMem
(
	A_UINT16 devIndex,
	A_UINT32 memSize,		             /* number of bytes to allocate */
	A_UINT32 *physAddress
)
{
    A_UCHAR *virtAddress;
    A_UINT32 offset;        /* offset from start of where descriptor is */
    A_UINT16 numBlocks;     /* num blocks need to allocate */
    A_UINT16 index;         /* index of first block to allocate */
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];


	if((memSize/BUFF_BLOCK_SIZE + 1) >= (1 << 8 * sizeof(A_UINT16))) {
 		uiPrintf("WARNING: Physical memory of size %ld out of range - returning NULL address\n", memSize); 
        return(NULL);
	}

    /* calculate how many blocks of memory are needed and round up result */
    numBlocks = (A_UINT16) (memSize/BUFF_BLOCK_SIZE + ((memSize % BUFF_BLOCK_SIZE) || 0));
    
    if(memGetIndexForBlock2(pdevInfo, pdevInfo->pbuffMapBytes, numBlocks, &index) != A_OK) {
		uiPrintf("WARNING: Failed to allocate physical memory of size %ld - returning NULL address\n", memSize); 
        return(NULL);
	}

    /* got an index, now calculate addresses */
    offset = index * BUFF_BLOCK_SIZE;
    virtAddress = (A_UCHAR *)(pdevInfo->pdkInfo->memVirAddr + offset);
    *physAddress = (A_UINT32)(pdevInfo->pdkInfo->memPhyAddr + offset);

    /* zero out memory */
	memset(virtAddress, 0, memSize);
    return(virtAddress);    
}

/**************************************************************************
* hwFreeAll - Environment specific code for Command to free all the 
*             currently allocated memory
*
* This routine calls to the hardware abstraction layer, to free all of the
* currently allocated memory.  This will include all descriptors and packet
* data as well as any memory allocated with the alloc command.
*
*
* RETURNS: N/A
*/
void hwFreeAll
(
	A_UINT16 devIndex
)
{
	A_UINT32        NumBuffBlocks;
	A_UINT32        NumBuffMapBytes;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];


    if(pdevInfo) {
		NumBuffBlocks	= pdevInfo->pdkInfo->memSize / BUFF_BLOCK_SIZE;
		NumBuffMapBytes = NumBuffBlocks / 8;

		/* clear the memory allocated by clearing all the map bytes */
		memset( pdevInfo->pbuffMapBytes, 0, NumBuffMapBytes * sizeof(A_UCHAR) );
		memset( pdevInfo->pnumBuffs, 0, NumBuffBlocks * sizeof(A_UINT16) );
	}

    return;
}


/**************************************************************************
* hwEnableFeature - Handle feature enable within windows environment
*
* Enable ISR features within windows environment
*
*
* RETURNS: 0 on success, -1 on error
*/

A_INT16 hwEnableFeature
(
	A_UINT16 devIndex,
	PIPE_CMD *pCmd
)
{
	uiPrintf("hwEnableFeature not implemented for linux dk \n");
	return(0);
}

/**************************************************************************
* hwDisableFeature - Handle feature disable within windows environment
*
* Disble ISR features within windows environment
*
*
* RETURNS: 0 on success, -1 on error
*/

A_INT16 hwDisableFeature
(
	A_UINT16 devIndex,
	PIPE_CMD *pCmd
)
{
	uiPrintf("hwDiableFeature not implemented for linux dk \n");
	return(0);
}

/**************************************************************************
* hwGetStats - Get stats
*
* call into kernel plugin to get the stats copied into user supplied 
* buffer
*
*
* RETURNS: 0 on success, -1 on error
*/

A_INT16 hwGetStats
(
	A_UINT16 devIndex,
	A_UINT32 clearOnRead,
	A_UCHAR  *pBuffer,
	A_BOOL	 rxStats
)
{
	uiPrintf("hwGetStats not implemented for linux dk \n");
	return(0);
}

/**************************************************************************
* hwGetSingleStat - Get single stat
*
* call into kernel plugin to get the stats copied into user supplied 
* buffer
*
*
* RETURNS: 0 on success, -1 on error
*/

A_INT16 hwGetSingleStat
(
	A_UINT16 devIndex,
	A_UINT32 statID,
	A_UINT32 clearOnRead,
	A_UCHAR  *pBuffer,
	A_BOOL	 rxStats
)
{
	uiPrintf("hwGetSingleStat not implemented for linux dk \n");
	return(0);
}


/**************************************************************************
* hwRemapHardware - Remap the hardware to a new address
*
* Remap the hardware to a new address
*
*
* RETURNS: 0 on success, -1 on error
*/
A_INT16 hwRemapHardware
(
	A_UINT16 devIndex,
	A_UINT32 mapAddress
)
{
	uiPrintf("hwReMapHardware not implemented for linux dk \n");
    return(0);
}

/* hwTramWriteBlock -  Write trace ram
*
* Write a block of trace ram
*
*
* RETURNS: 0 on success, -1 on error
*/
A_INT16 hwTramWriteBlock
(
	A_UINT16 devIndex,
	A_UCHAR    *pBuffer,
	A_UINT32 length,
	A_UINT32 physAddr
)
{
	A_UINT32 numDWords = length / 4;
	A_UINT32 *pData = (A_UINT32 *)pBuffer;
	A_UINT32 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

	
	for(i = 0; i < numDWords; i++) {
		//write the address
		hwMemWrite32(devIndex, pdevInfo->pdkInfo->aregPhyAddr[0] + 0x18, physAddr);

		//write the value
		hwMemWrite32(devIndex, pdevInfo->pdkInfo->aregPhyAddr[0] + 0x1c, *pData);

		pData++;
		physAddr++;
	}
	return 0;
}

/**************************************************************************
* hwTramReadBlock - Read a block of trace ram
*
* Read a block of traceram
*
*
* RETURNS: 0 on success, -1 on error
*/
A_INT16 hwTramReadBlock
(
	A_UINT16 devIndex,
	A_UCHAR    *pBuffer,
	A_UINT32 physAddr,
	A_UINT32 length
)
{
	A_UINT32 numDWords = length / 4;
	A_UINT32 *pData = (A_UINT32 *)pBuffer;
	A_UINT32 i;
	MDK_WLAN_DEV_INFO    *pdevInfo;

	pdevInfo = globDrvInfo.pDevInfoArray[devIndex];

	
	for(i = 0; i < numDWords; i++) {
		//write the address
		hwMemWrite32(devIndex, pdevInfo->pdkInfo->aregPhyAddr[0] + 0x18, physAddr);

		//read the value
		*pData = hwMemRead32(devIndex, pdevInfo->pdkInfo->aregPhyAddr[0] + 0x1c);

		pData++;
		physAddr++;
	}
	return 0;

}

