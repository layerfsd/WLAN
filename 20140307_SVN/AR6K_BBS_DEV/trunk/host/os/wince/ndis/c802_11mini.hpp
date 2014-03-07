//------------------------------------------------------------------------------
// <copyright file="c802_11mini.hpp" company="Atheros and Microsoft">
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
// NDIS 802.11 Miniport class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#ifndef _C80211MINI_H_
#define _C80211MINI_H_

#include <ndis.h>
#include "athdrv.h"

#define DEFAULT_802_11_MAX_MULTICAST_ADDRESSES   32

class C802_11Miniport : public C802_3Miniport
{
protected:
	BYTE  m_MulticastAddresses[DEFAULT_802_11_MAX_MULTICAST_ADDRESSES][ETHERNET_MAC_ADDRESS_LENGTH];
	ULONG m_numMulticastAddresses;

	// m_bRSSITriggerSet is false initially, becomes true after a Set Request
	// to OID_802_11_RSSI_TRIGGER occurs.
	boolean m_bRSSITriggerSet;

	boolean m_bInfrastructureModeSet;

public:
	C802_11Miniport();
    virtual ~C802_11Miniport();

	virtual NDIS_STATUS           QueryInformation(IN NDIS_OID Oid, OUT PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbWritten, OUT PULONG pcbNeeded);
	virtual NDIS_STATUS           SetInformation(  IN NDIS_OID Oid, IN  PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbRead, OUT PULONG pcbNeeded);

	virtual NDIS_PHYSICAL_MEDIUM  GetGenPhysicalMedium();

	virtual PNDIS_OID             GetSupportedOidList(OUT PNDIS_OID pOidBuffer, IN OUT PULONG pMaxOidsToAddToBuffer, IN OUT PULONG pTotalNumberOfSupportedOids);
	boolean                       SSIDIsValid(const NDIS_802_11_SSID *pSSID);
#ifdef DEBUG
	void                          Get80211OidTypeAndData(OUT char    *szBuffer, IN NDIS_OID Oid, IN PBYTE    Data);
#endif

	// OID_802_3 Query Handlers
	virtual void                  Get802_3MulticastList(OUT PBYTE *pMulticastAddreses, OUT PULONG pNumMulticastAddresses);
	virtual ULONG                 Get802_3MaxMulticastListSize();

	// OID_802_3 Set Handlers
	virtual NDIS_STATUS           Set802_3MulticastList(IN PBYTE pMulticastAddreses, IN ULONG NumMulticastAddresses);

	// OID_802_11 Query Handlers
	virtual NDIS_STATUS           Get802_11AssociatedAPBSSID(OUT NDIS_802_11_MAC_ADDRESS *) = NULL;
	virtual void                  Get802_11SSID(OUT NDIS_802_11_SSID *) = NULL;
	virtual void                  Get802_11NetworkTypesSupported(OUT NDIS_802_11_NETWORK_TYPE **, OUT PULONG pNumTypes) = NULL;
	virtual void                  Get802_11NetworkTypeInUse(OUT PNDIS_802_11_NETWORK_TYPE) = NULL;
	virtual NDIS_STATUS           Get802_11TxPowerLevel(OUT NDIS_802_11_TX_POWER_LEVEL *) = NULL;
	virtual NDIS_STATUS           Get802_11RSSI(OUT NDIS_802_11_RSSI *) = NULL;
	virtual NDIS_STATUS           Get802_11RSSITrigger(OUT NDIS_802_11_RSSI *) = NULL;
	virtual void                  Get802_11InfrastructureMode(OUT NDIS_802_11_NETWORK_INFRASTRUCTURE *) = NULL;
	virtual NDIS_STATUS           Get802_11FragmentationThreshold(OUT NDIS_802_11_FRAGMENTATION_THRESHOLD *pThreshold) = NULL;
	virtual NDIS_STATUS           Get802_11RTSThreshold(OUT NDIS_802_11_RTS_THRESHOLD *pThreshold) = NULL;
	virtual NDIS_STATUS           Get802_11NumberOfAntennas(OUT ULONG *pNumberOfAntennas) = NULL;
	virtual NDIS_STATUS           Get802_11RxAntennaSelected(OUT ULONG *pAntenna) = NULL;
	virtual NDIS_STATUS           Get802_11TxAntennaSelected(OUT ULONG *pAntenna) = NULL;
	virtual void                  Get802_11SupportedRates(OUT BYTE *Rates) = NULL;
	virtual NDIS_STATUS           Get802_11DesiredRates(OUT NDIS_802_11_RATES *Rates) = NULL;
	virtual void                  Get802_11Configuration(OUT PNDIS_802_11_CONFIGURATION pConfiguration) = NULL;
	virtual NDIS_STATUS           Get802_11Statistics(OUT PNDIS_802_11_STATISTICS pStatistics) = NULL;
	virtual NDIS_STATUS           Get802_11PowerMode(OUT PNDIS_802_11_POWER_MODE pPowerMode) = NULL;
	virtual void                  Get802_11BSSIDList(OUT PNDIS_802_11_BSSID_LIST_EX pList, ULONG cbBuffer, PULONG pcbNeeded) = NULL;
	virtual void                  Get802_11AuthenticationMode(OUT NDIS_802_11_AUTHENTICATION_MODE *pMode) = NULL;
	virtual NDIS_STATUS           Get802_11PrivacyFilter(OUT PNDIS_802_11_PRIVACY_FILTER pFilter) = NULL;
	virtual void                  Get802_11EncryptionStatus(OUT PNDIS_802_11_ENCRYPTION_STATUS pStatus) = NULL;
	virtual NDIS_STATUS           Get802_11AssociationInformation(OUT PNDIS_802_11_ASSOCIATION_INFORMATION pInfo, ULONG cbBuffer, PULONG pcbNeeded) = NULL;
#ifdef SUPPORT_WPA2
	virtual NDIS_STATUS           Get802_11PMKID(OUT NDIS_802_11_PMKID *pPMKID, IN  ULONG cbBuffer, OUT ULONG *pcbNeeded) = NULL;
	virtual NDIS_STATUS           Get802_11Capability(OUT NDIS_802_11_CAPABILITY *pCap, IN ULONG cbBuffer, OUT ULONG *pcbNeeded) = NULL;
#endif

	// OID_802_11 Set Handlers
	virtual NDIS_STATUS           Set802_11AssociatedAPBSSID(IN NDIS_802_11_MAC_ADDRESS *) = NULL;
	virtual NDIS_STATUS           Set802_11SSID(IN NDIS_802_11_SSID *) = NULL;
	virtual NDIS_STATUS           Set802_11NetworkTypeInUse(IN NDIS_802_11_NETWORK_TYPE) = NULL;
	virtual NDIS_STATUS           Set802_11TxPowerLevel(NDIS_802_11_TX_POWER_LEVEL) = NULL;
	virtual NDIS_STATUS           Set802_11RSSITrigger(NDIS_802_11_RSSI) = NULL;
	virtual NDIS_STATUS           Set802_11InfrastructureMode(NDIS_802_11_NETWORK_INFRASTRUCTURE) = NULL;
	virtual NDIS_STATUS           Set802_11FragmentationThreshold(NDIS_802_11_FRAGMENTATION_THRESHOLD) = NULL;
	virtual NDIS_STATUS           Set802_11RTSThreshold(NDIS_802_11_RTS_THRESHOLD) = NULL;
	virtual NDIS_STATUS           Set802_11RxAntennaSelected(ULONG Antenna) = NULL;
	virtual NDIS_STATUS           Set802_11TxAntennaSelected(ULONG Antenna) = NULL;
	virtual NDIS_STATUS           Set802_11DesiredRates(NDIS_802_11_RATES *Rates) = NULL;
	virtual NDIS_STATUS           Set802_11Configuration(IN NDIS_802_11_CONFIGURATION *pConfiguration) = NULL;
	virtual NDIS_STATUS           Set802_11Disassociate() = NULL;
	virtual NDIS_STATUS           Set802_11PowerMode(IN NDIS_802_11_POWER_MODE PowerMode) = NULL;
	virtual NDIS_STATUS           Set802_11BSSIDListScan() = NULL;
	virtual NDIS_STATUS           Set802_11PrivacyFilter(IN NDIS_802_11_PRIVACY_FILTER Filter) = NULL;
	virtual NDIS_STATUS           Set802_11ReloadDefaults(IN NDIS_802_11_RELOAD_DEFAULTS) = NULL;
	virtual NDIS_STATUS           Set802_11AuthenticationMode(IN NDIS_802_11_AUTHENTICATION_MODE Mode) = NULL;
	virtual NDIS_STATUS           Set802_11EncryptionStatus(IN NDIS_802_11_ENCRYPTION_STATUS EncryptionStatus) = NULL;
	virtual NDIS_STATUS           Set802_11AddWEP(IN NDIS_802_11_WEP *pWEP) = NULL;
	virtual NDIS_STATUS           Set802_11RemoveWEP(IN NDIS_802_11_KEY_INDEX) = NULL;
	virtual NDIS_STATUS           Set802_11AddKey(IN NDIS_802_11_KEY *) = NULL;
	virtual NDIS_STATUS           Set802_11RemoveKey(IN NDIS_802_11_REMOVE_KEY *) = NULL;
	virtual NDIS_STATUS           Set802_11AssociationInformation(IN NDIS_802_11_ASSOCIATION_INFORMATION *pInfo, ULONG cbInfo) = NULL;
#ifdef SUPPORT_WPA2
	virtual NDIS_STATUS           Set802_11PMKID(OUT NDIS_802_11_PMKID* pPMKID, IN ULONG cbBuffer) = NULL;
#endif
	virtual NDIS_STATUS           Set802_11Test(IN NDIS_802_11_TEST *, IN ULONG cbBuffer) = NULL;
};

#endif
