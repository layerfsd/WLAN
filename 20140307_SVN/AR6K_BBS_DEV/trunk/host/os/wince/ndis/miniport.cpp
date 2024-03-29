//------------------------------------------------------------------------------
// <copyright file="miniport.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// NDIS miniport entry points
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================

//#ifndef DEBUG
//#define DEBUG
//#endif

#include <windows.h>
#include <ndis.h>


#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
//#include "osapi.h"
#include "athdefs.h"
#include "athtypes_wince.h"
#include "htc_internal.h"
#include "htc.h"
#include "hif.h"
#include "wmi.h"
#include "wlan_api.h"
#include "car6k.hpp"
#ifdef NDIS_BUS_DRIVER
#include "bus_driver.h"
#endif
#include "platform.h"

#define _P	L"\r[AR6K: MiniPort] "L


enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0,
}MiniPortDebug;

#if 0

void
DbgPrintf(A_INT32 mask, A_CHAR * format, ...)
{
    va_list argList;    /* holds the arguement list passed in */
    A_CHAR    buffer[1024];
    wchar_t wBuf[1024];

#if 1
	if (mask) {
		/* get the argument list */
		va_start(argList, "[AR6k]");
	
		/*
		 * using vprintf to perform the printing it is the same is printf, only
		 * it takes a va_list or arguements
		 */
		
		vsprintf(buffer+2, "[AR6k]", argList);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wBuf, 256 );
		OutputDebugString(wBuf);
	}
	
#endif

	if (mask) {
		/* get the argument list */
		va_start(argList, format);

		/*
		 * using vprintf to perform the printing it is the same is printf, only
		 * it takes a va_list or arguements
		 */
		vsprintf(buffer, format, argList);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wBuf, 256 );
		OutputDebugString(wBuf);
	}
	return;
}

#endif

extern "C" {
#ifdef DEBUG
unsigned int txcreditsavailable[HTC_MAILBOX_NUM_MAX] = {0};
unsigned int txcreditsconsumed[HTC_MAILBOX_NUM_MAX] = {0};
unsigned int txcreditintrenable[HTC_MAILBOX_NUM_MAX] = {0};
unsigned int txcreditintrenableaggregate[HTC_MAILBOX_NUM_MAX] = {0};
#endif
unsigned int tx_attempt[HTC_MAILBOX_NUM_MAX] = {0};
unsigned int tx_post[HTC_MAILBOX_NUM_MAX] = {0};
unsigned int tx_complete[HTC_MAILBOX_NUM_MAX] = {0};
}

// Handle to the Driver which is set when NDIS loads us.
NDIS_HANDLE g_hMPDriverHandle;
LONG        g_MPAdapterCount;
static BYTE ndisDriverMinorVersion;


#pragma NDIS_INIT_FUNCTION(DriverEntry)

#define MP_CLASS                            CAR6KMini
#define MP_CHECK_FOR_HANG_INTERVAL_SECONDS	0 // Use default interval



extern  
NDIS_STATUS
MPInitialize(
    OUT PNDIS_STATUS OpenErrorStatus,
    OUT PUINT        SelectedMediumIndex,
    IN  PNDIS_MEDIUM MediumArray,
    IN  UINT         MediumArraySize,
    IN  NDIS_HANDLE  MiniportAdapterHandle,
    IN  NDIS_HANDLE  WrapperConfigurationContext
    )

/*++

Routine Description:

    MiniportInitialize starts an adapter and registers resources with the
    wrapper.

Arguments:

    OpenErrorStatus - Extra status bytes for opening token ring adapters.

    SelectedMediumIndex - Index of the media type chosen by the driver.

    MediumArray - Array of media types for the driver to chose from.

    MediumArraySize - Number of entries in the array.

    MiniportAdapterHandle - Handle for passing to the wrapper when
       referring to this adapter.

    WrapperConfigurationContext - A handle to pass to NdisOpenConfiguration.

Return Value:

    NDIS_STATUS_SUCCESS
    NDIS_STATUS_PENDING

--*/

{
	MP_CLASS    *Adapter;
    NDIS_HANDLE ConfigHandle = NULL;
    ULONG       i;
	NDIS_STATUS	Status;
	NDIS_MEDIUM SupportedMedium;
#ifdef NDIS_BUS_DRIVER
    UINT    SysIntr;
    UINT    BusType;
    NDIS_STRING  SysIntrStr = NDIS_STRING_CONST("SysIntr");
    NDIS_STRING  BusTypeStr = NDIS_STRING_CONST("BusType");
    PNDIS_CONFIGURATION_PARAMETER ParameterValue;
#endif

	Adapter = new MP_CLASS;
	if (NULL == Adapter)
	{
		Status = NDIS_STATUS_RESOURCES;
		goto done;
	}

	// Store the ndis versions
	Adapter->m_NdisDriverMajorVersion = 5;
	Adapter->m_NdisDriverMinorVersion = ndisDriverMinorVersion;

	// Search for the medium type the miniport supports (802.3) in the given array.
	//
	Status = NDIS_STATUS_UNSUPPORTED_MEDIA;
	SupportedMedium = Adapter->GetSupportedNdisMediumType();
	for (i = 0; i < MediumArraySize; i++)
	{
		if (MediumArray[i] == SupportedMedium)
		{
			*SelectedMediumIndex = i;
			Status = NDIS_STATUS_SUCCESS;
			break;
		}
	}
	if (Status != NDIS_STATUS_SUCCESS)
		// Protocol doesn't support our media type
		goto done;

	//
	// Open the configuration space.
	//
	NdisOpenConfiguration(&Status, &ConfigHandle, WrapperConfigurationContext);
	//NDIS_DEBUG_PRINTF(1,(TEXT("AR6K: MPInitialize... ################\r\n")));

	if (Status != NDIS_STATUS_SUCCESS)
	{
		NDIS_DEBUG_PRINTF(DBG_ERR, "AR6K: ERROR - NdisOpenConfiguration failed 0x%x\r\n", Status);
		goto done;
	}

	plat_init();

	//
	// Inform the wrapper of the physical attributes of this adapter.
	//
#ifdef NDIS_BUS_DRIVER
    NdisReadConfiguration(
			&Status,
			&ParameterValue,
			ConfigHandle,
			&BusTypeStr,
			NdisParameterHexInteger
			);

	 if (Status != NDIS_STATUS_SUCCESS) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "AR6K: ERROR - NdisReadConfiguration failed 0x%x \r\n", Status);
		goto done;
     }

    BusType = ParameterValue->ParameterData.IntegerData;
    
    NdisReadConfiguration(
         &Status,
         &ParameterValue,
         ConfigHandle,
         &SysIntrStr,
         NdisParameterHexInteger);
        
	 if (Status != NDIS_STATUS_SUCCESS) {
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - NdisReadConfiguration failed 0x%x \r\n", Status);
		goto done;
     }
        
     SysIntr = ParameterValue->ParameterData.IntegerData; 

	 NdisMSetAttributesEx(MiniportAdapterHandle, 
			                (NDIS_HANDLE)Adapter, 
							MP_CHECK_FOR_HANG_INTERVAL_SECONDS, 
			                NDIS_ATTRIBUTE_DESERIALIZE | NDIS_ATTRIBUTE_NO_HALT_ON_SUSPEND, 
							(NDIS_INTERFACE_TYPE)BusType);

	 Status = busDriverInit(MiniportAdapterHandle, 
    					    WrapperConfigurationContext,
							SysIntr,
							&Adapter->m_pBusDriverHandle);
    
	if (Status != NDIS_STATUS_SUCCESS) {
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "AR6K: ERROR - busDriverInit failed 0x%x \r\n", Status);
		goto done;
    }
#else
	NdisMSetAttributesEx(MiniportAdapterHandle, 
			                (NDIS_HANDLE)Adapter, 
							MP_CHECK_FOR_HANG_INTERVAL_SECONDS, 
			                NDIS_ATTRIBUTE_DESERIALIZE | NDIS_ATTRIBUTE_NO_HALT_ON_SUSPEND, 
							NdisInterfaceInternal);
#endif

	Status = Adapter->Initialize(MiniportAdapterHandle, ConfigHandle);

done:
	if (ConfigHandle)
		NdisCloseConfiguration(ConfigHandle);

	if (Status != NDIS_STATUS_SUCCESS)
	{
		if (Adapter)
			delete Adapter;
	}
	else
	{
		NdisInterlockedIncrement(&g_MPAdapterCount);
	}


	NDIS_DEBUG_PRINTF(1, "AR6K: MPInitialize complete Status=%x \r\n", Status);


	return Status;
}

extern 
VOID
MPHalt(
    IN NDIS_HANDLE MiniportAdapterContext
    )

/*++

Routine Description:

    MiniportHalt removes an adapter that was previously initialized
	via MiniportInitialize.

Arguments:

    MiniportAdapterContext - The context value that the Miniport returned
        from MiniportInitialize.

Return Value:

    None.

--*/

{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;
#ifdef NDIS_BUS_DRIVER
	MP_CLASS  *Ar6kAdapter = (MP_CLASS *)MiniportAdapterContext;
#endif
    
	Adapter->Halt();
    
#ifdef NDIS_BUS_DRIVER
	if (Ar6kAdapter->m_pBusDriverHandle) {
        busDriverShutdown(Ar6kAdapter->m_pBusDriverHandle);
    }
#endif

    plat_exit();

	NdisInterlockedDecrement(&g_MPAdapterCount);

	delete Adapter;
}

NDIS_STATUS
MPReset(
    OUT PBOOLEAN pAddressingReset,
    IN NDIS_HANDLE MiniportAdapterContext
    )
/*++

Routine Description:

    The MinportReset request instructs the Miniport to issue a hardware reset
    to the network adapter.  The driver also resets its software state.  See
    the description of NdisMReset for a detailed description of this request.

Arguments:

    AddressingReset - Does the adapter need the addressing information reloaded.

    MiniportAdapterContext - Pointer to the adapter structure.

Return Value:

    The function value is the status of the operation.

--*/

{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;

	return Adapter->Reset(pAddressingReset);
}


NDIS_STATUS
MPQueryInformation(
    IN NDIS_HANDLE MiniportAdapterContext,
    IN NDIS_OID Oid,
    IN PVOID    Buffer,
    IN ULONG    cbBuffer,
    OUT PULONG  pcbWritten,
    OUT PULONG  pcbNeeded
)

/*++

Routine Description:

    The MiniportQueryInformation process a Query request for
    NDIS_OIDs that are specific about the Driver.

Arguments:

    MiniportAdapterContext - a pointer to the adapter.

    Oid - the NDIS_OID to process.

    InformationBuffer -  a pointer into the
    NdisRequest->InformationBuffer into which store the result of the query.

    InformationBufferLength - a pointer to the number of bytes left in the
    InformationBuffer.

    BytesWritten - a pointer to the number of bytes written into the
    InformationBuffer.

    BytesNeeded - If there is not enough room in the information buffer
    then this will contain the number of bytes needed to complete the
    request.

Return Value:

    The function value is the status of the operation.

--*/
{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;
	NDIS_STATUS Status = 0 ;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "\r AR6K: NDIS QueryInformation OID=%x \r\n", Oid);

	Status = Adapter->QueryInformation(Oid, Buffer, cbBuffer, pcbWritten, pcbNeeded);
	
    return Status;
}

extern
NDIS_STATUS
MPSetInformation(
    IN NDIS_HANDLE MiniportAdapterContext,
    IN NDIS_OID Oid,
    IN PVOID    Buffer,
    IN ULONG    cbBuffer,
    OUT PULONG  pcbRead,
    OUT PULONG  pcbNeeded
    )

/*++

Routine Description:

    MiniportSetInformation handles a set operation for a
    single OID.

Arguments:

    MiniportAdapterContext - Context registered with the wrapper, really
        a pointer to the adapter.

    Oid - The OID of the set.

    InformationBuffer - Holds the data to be set.

    InformationBufferLength - The length of InformationBuffer.

    BytesRead - If the call is successful, returns the number
        of bytes read from InformationBuffer.

    BytesNeeded - If there is not enough data in InformationBuffer
        to satisfy the OID, returns the amount of storage needed.

Return Value:

    NDIS_STATUS_SUCCESS
    NDIS_STATUS_PENDING
    NDIS_STATUS_INVALID_LENGTH
    NDIS_STATUS_INVALID_OID

--*/
{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;
	NDIS_STATUS Status;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "\r AR6K: NDIS SetInformation OID=%x \r\n", Oid);

	Status = Adapter->SetInformation(Oid, Buffer, cbBuffer, pcbRead, pcbNeeded);

	NDIS_DEBUG_PRINTF(DBG_TRACE, "\r AR6K: NDIS SetInformation Exit\r\n");

    return Status;
}

VOID
MPSendPackets(
    IN NDIS_HANDLE      MiniportAdapterContext,
    IN PPNDIS_PACKET    PacketArray,
    IN UINT             NumberOfPackets
    )
//
//  MiniportSendPackets transfers some number of packets, specified as an array of packet pointers, over the network.
//
{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;

	Adapter->SendPackets(PacketArray, NumberOfPackets);

}

VOID
MPReturnPacket(
    IN NDIS_HANDLE      MiniportAdapterContext,
    IN PNDIS_PACKET     Packet)
//
//  MiniportReturnPacket is called by NDIS to return control of a packet that the miniport driver
//  gave ownership of to NDIS by calling NdisMIndicateReceivePacket.
//
{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;

	Adapter->ReturnPacket(Packet);
}

#ifdef NDIS_BUS_DRIVER

extern VOID
MPISR (
    OUT PBOOLEAN InterruptRecognized,
	OUT PBOOLEAN QueueMiniportHandleInterrupt,
	IN  NDIS_HANDLE MiniportAdapterContext)
{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;
	MP_CLASS  *Ar6kAdapter = (MP_CLASS *)MiniportAdapterContext;
	BOOL		callDsr;

	NDIS_DEBUG_PRINTF(DBG_TRACE, "%s() +Enter \r\n", __FUCNTION__);
	busDriverIsr(Ar6kAdapter->m_pBusDriverHandle, &callDsr);
	*InterruptRecognized = TRUE;
	*QueueMiniportHandleInterrupt = callDsr;

	return;
}

extern  VOID
MPHandleInterrupt (
    IN NDIS_HANDLE MiniportAdapterContext)
{
	CMiniport *Adapter = (CMiniport *)MiniportAdapterContext;
	MP_CLASS  *Ar6kAdapter = (MP_CLASS *)MiniportAdapterContext;

	NDIS_DEBUG_PRINTF(DBG_TRACE, " %s() +Enter \r\n",__FUNCTION__);
	busDriverDsr(Ar6kAdapter->m_pBusDriverHandle);
	return;
}
#endif

extern "C" NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
    )

/*++

Routine Description:

    This is the primary initialization routine for the driver.
    It is responsible for the initializing the wrapper and registering
    the Miniport driver.

Arguments:

    DriverObject - Pointer to driver object created by the system.

    RegistryPath - Path to the parameters for this driver in the registry.

Return Value:

    The status of the operation.

--*/

{
	NDIS_STATUS Status;
	NDIS40_MINIPORT_CHARACTERISTICS MiniChar40;
	NDIS50_MINIPORT_CHARACTERISTICS MiniChar50;
#ifdef NDIS51_MINIPORT
	NDIS51_MINIPORT_CHARACTERISTICS MiniChar51;
#endif
	NDIS_HANDLE NdisWrapperHandle;
	
	NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: DriverEntry \r\n");

	//
	// Initialize the wrapper.
	//
	NdisMInitializeWrapper(&NdisWrapperHandle, DriverObject, RegistryPath, NULL);

	// Initially there are no adapters
	g_MPAdapterCount = 0;

	//
	// Initialize the Miniport characteristics for the call to
	// NdisMRegisterMiniport.
	//
	memset(&MiniChar40, 0, sizeof(MiniChar40));
	memset(&MiniChar50, 0, sizeof(MiniChar50));
#ifdef NDIS51_MINIPORT
	memset(&MiniChar51, 0, sizeof(MiniChar51));
#endif

	MiniChar40.Ndis30Chars.MajorNdisVersion = 5;
	MiniChar40.Ndis30Chars.HaltHandler = MPHalt;
	MiniChar40.Ndis30Chars.InitializeHandler = MPInitialize;
#ifdef NDIS_BUS_DRIVER
	MiniChar40.Ndis30Chars.HandleInterruptHandler = MPHandleInterrupt;
	MiniChar40.Ndis30Chars.ISRHandler = MPISR;
#endif
	MiniChar40.Ndis30Chars.QueryInformationHandler = MPQueryInformation;
	MiniChar40.Ndis30Chars.ResetHandler = MPReset;
	MiniChar40.Ndis30Chars.SetInformationHandler = MPSetInformation;

	MiniChar40.ReturnPacketHandler = MPReturnPacket;
	MiniChar40.SendPacketsHandler = MPSendPackets;

	MiniChar50.Ndis40Chars = MiniChar40;
#ifdef NDIS51_MINIPORT
	MiniChar40.Ndis30Chars.MinorNdisVersion = 1;
	MiniChar51.Ndis50Chars = MiniChar50;
	Status = NdisMRegisterMiniport(NdisWrapperHandle, (PNDIS_MINIPORT_CHARACTERISTICS)&MiniChar51, sizeof(MiniChar51));
	if (Status == NDIS_STATUS_SUCCESS) {
		ndisDriverMinorVersion = 1;
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: Successfully registered as 5.1 miniport  \r\n");
		return Status;
	}
#endif
    
	MiniChar40.Ndis30Chars.MinorNdisVersion = 0;
	Status = NdisMRegisterMiniport(NdisWrapperHandle, (PNDIS_MINIPORT_CHARACTERISTICS)&MiniChar50, sizeof(MiniChar50));
	if (Status != NDIS_STATUS_SUCCESS) {
		NDIS_DEBUG_PRINTF(DBG_ERR, "AR6K: ERROR - NdisMRegisterMiniport failed error = %x \r\n", Status);
		NdisTerminateWrapper (NdisWrapperHandle, NULL);
	} else {
		ndisDriverMinorVersion = 0;
		NDIS_DEBUG_PRINTF(DBG_TRACE, "AR6K: Successfully registered as 5.0 miniport  \r\n");
	}

	return Status;
}

