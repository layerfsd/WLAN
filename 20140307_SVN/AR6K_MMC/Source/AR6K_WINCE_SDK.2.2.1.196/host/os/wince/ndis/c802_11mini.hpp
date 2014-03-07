//------------------------------------------------------------------------------
// <copyright file="c802_11mini.hpp" company="Atheros and Microsoft">
//    Copyright (c) 2004-2008 Microsoft Corporation.  All rights reserved.
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
// NDIS 802.11 Miniport class.
//
// Author(s): ="Atheros and Microsoft"
//==============================================================================
#ifndef _C80211MINI_H_
#define _C80211MINI_H_

#include <ndis.h>
#include <a_drv.h>
#include <a_types.h>
#include <a_osapi.h>
#include <wmi.h>


#include "wince_common_drv.h"

class C802_11Miniport : public C802_3Miniport
{
protected:
    BYTE  m_MulticastAddresses[DEFAULT_802_11_MAX_MULTICAST_ADDRESSES][ETHERNET_MAC_ADDRESS_LENGTH];
    ULONG m_numMulticastAddresses;

    // m_bRSSITriggerSet is false initially, becomes true after a Set Request
    // to OID_802_11_RSSI_TRIGGER occurs.
    boolean m_bRSSITriggerSet;

public:
    C802_11Miniport();
    virtual ~C802_11Miniport();

    boolean m_ForceDisable;
    WLAN_STA_CONFIG m_Config;
    virtual NDIS_STATUS           QueryInformation(IN NDIS_OID Oid, OUT PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbWritten, OUT PULONG pcbNeeded);
    virtual NDIS_STATUS           SetInformation(  IN NDIS_OID Oid, IN  PVOID Buffer, IN ULONG cbBuffer, OUT PULONG pcbRead, OUT PULONG pcbNeeded);
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

#ifdef OS_ROAM_MANAGEMENT
    virtual NDIS_STATUS           Get802_11RoamControl(OUT PNDIS_802_11_NIC_ROAM_CONTROL pCtl) = NULL;
    virtual NDIS_STATUS           Get802_11ChannelList(OUT PNDIS_802_11_CHANNEL_LIST pList, IN ULONG cbList, OUT ULONG *pcbNeeded) = NULL;
    virtual NDIS_STATUS           Get802_11RateRssiFilter(OUT PNDIS_802_11_BSSID_RATE_RSSI_FILTER pFilter) = NULL;
    virtual NDIS_STATUS           Get802_11RateRssiInfo(OUT PNDIS_802_11_BSSID_RATE_RSSI_INFO pInfo) = NULL;
#endif


    // OID_802_11 Set Handlers
    virtual NDIS_STATUS           Set802_11AssociatedAPBSSID(IN NDIS_802_11_MAC_ADDRESS *) = NULL;
    virtual NDIS_STATUS           Set802_11SSID(IN NDIS_802_11_SSID *) = NULL;
    virtual NDIS_STATUS           Set802_11ScanParams(IN WMI_SCAN_PARAMS_CMD *) = NULL;
    virtual NDIS_STATUS           Set802_11ChannelParams(IN WMI_CHANNEL_PARAMS_CMD  *) = NULL;
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
    virtual NDIS_STATUS           Set802_11AddKey(IN NDIS_802_11_KEY *, IN BOOL) = NULL;
    virtual NDIS_STATUS           Set802_11RemoveKey(IN NDIS_802_11_REMOVE_KEY *) = NULL;
    virtual NDIS_STATUS           Set802_11AssociationInformation(IN NDIS_802_11_ASSOCIATION_INFORMATION *pInfo, ULONG cbInfo) = NULL;
#ifdef SUPPORT_WPA2
    virtual NDIS_STATUS           Set802_11PMKID(OUT NDIS_802_11_PMKID* pPMKID, IN ULONG cbBuffer) = NULL;
#endif
    virtual NDIS_STATUS           Set802_11Test(IN NDIS_802_11_TEST *, IN ULONG cbBuffer) = NULL;

#ifdef OS_ROAM_MANAGEMENT
    virtual NDIS_STATUS           Set802_11RoamControl(IN PNDIS_802_11_NIC_ROAM_CONTROL pCtl) = NULL;
    virtual NDIS_STATUS           Set802_11RateRssiFilter(IN PNDIS_802_11_BSSID_RATE_RSSI_FILTER pFilter) = NULL;
    virtual NDIS_STATUS           Set802_11ChannelScan(IN PNDIS_802_11_BSSID_CHANNEL_SCAN pChInfo) = NULL;
#endif

};

#endif
