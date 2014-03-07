/* hwext.h - external declarations environment hardware access */

/* Copyright (c) 2000 Atheros Communications, Inc., All Rights Reserved */

#ifndef __INChwexth
#define __INChwexth
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#include "hw.h"
#include "perlarry.h"
#include "event.h"
#include "dk_cmds.h"

extern MDK_WLAN_DRV_INFO	globDrvInfo;	            /* Global driver data structure */

/////////////////////////////////////
// function declarations

A_UINT8  hwMemRead8 (A_UINT16 devIndex, A_UINT32 memAddress);
A_UINT16 hwMemRead16(A_UINT16 devIndex, A_UINT32 memAddress);
A_UINT32 hwMemRead32(A_UINT16 devIndex, A_UINT32 memAddress);

void hwMemWrite8(A_UINT16 devIndex, A_UINT32 memAddress, A_UINT8 writeValue);
void hwMemWrite16(A_UINT16 devIndex, A_UINT32 memAddress, A_UINT16 writeValue);
void hwMemWrite32(A_UINT16 devIndex, A_UINT32 memAddress, A_UINT32 writeValue);

A_UINT8  hwCfgRead8 (A_UINT16 devIndex, A_UINT32 address);
A_UINT16 hwCfgRead16(A_UINT16 devIndex, A_UINT32 address);
A_UINT32 hwCfgRead32(A_UINT16 devIndex, A_UINT32 address);

void hwCfgWrite8(A_UINT16 devIndex, A_UINT32 address, A_UINT8 writeValue);
void hwCfgWrite16(A_UINT16 devIndex, A_UINT32 address, A_UINT16 writeValue);
void hwCfgWrite32(A_UINT16 devIndex, A_UINT32 address, A_UINT32 writeValue);

A_STATUS    envInit(A_BOOL debugMode);
void        envCleanup(A_BOOL closeDriver);
void*       hwGetPhysMem(A_UINT16 devIndex, A_UINT32 memSize, A_UINT32 *physAddress);

/**************************************************************************
* deviceInit - performs any initialization needed for a device
*
* Perform the initialization needed for a device.  This includes creating a 
* devInfo structure and initializing its contents
*
* RETURNS: 1 if successful, 0 if not
*/
A_STATUS deviceInit
    (
    A_UINT16 devIndex,         /* index of globalDrvInfo which to add device to */
    A_UINT16 device_fn
    );

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
    );


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
    );


/**************************************************************************
* hwCreateEvent - Handle event creation 
*
* Create an event 
*
*
* RETURNS: 0 on success, -1 on error
*/
A_INT16 hwCreateEvent
(
	A_UINT16 devIndex,
	PIPE_CMD *pCmd
);

/**************************************************************************
* hwGetNextEvent - Get next event
*
* Call into the driver to get the next event, copy into called supplied buffer
*
*
*/

A_INT16 hwGetNextEvent
(
	A_UINT16 devIndex,
	void *pBuf
);

/**************************************************************************
* hwMemWriteBlock - Read a block of memory within the simulation environment
*
* Read a block of memory within the simulation environment
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
);

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
);

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
);

/**************************************************************************
* hwEnableFeature - Enable features within the ISR
*
* Enable the features within the ISR 
*
* RETURNS: 
*/
A_INT16 hwEnableFeature
(
	A_UINT16 devIndex,
	PIPE_CMD *pCmd
);

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
);


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
);

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
);

A_INT16 hwTramReadBlock
(
	A_UINT16 devIndex,
	A_UCHAR    *pBuffer,
	A_UINT32 physAddr,
	A_UINT32 length
);

A_INT16 hwTramWriteBlock
(
	A_UINT16 devIndex,
	A_UCHAR    *pBuffer,
	A_UINT32 length,
	A_UINT32 pPhysAddr
);

A_UINT16 hwGetBarSelect(A_UINT16 index);
A_UINT16 hwSetBarSelect(A_UINT16 index, A_UINT16 bs);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INChwexth */
