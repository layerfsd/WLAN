//------------------------------------------------------------------------------
// <copyright file="c802_11mini.cpp" company="Atheros and Microsoft">
//    Copyright (c) 2004-2007 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
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
#include <windows.h>
#include <ndis.h>

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "a_debug.h"

C802_11Miniport::C802_11Miniport()
{
    m_numMulticastAddresses  = 0;
    m_bRSSITriggerSet        = FALSE;
    m_ForceDisable           = FALSE;
}

C802_11Miniport::~C802_11Miniport()
{
}

boolean
C802_11Miniport::SSIDIsValid(
    const NDIS_802_11_SSID *pSSID)
//
//  WZC will set the SSID to a random string of 32 invalid
//  characters in the range 0x01 - 0x1F in order to stop
//  an adapter from associating with any SSID.
//
//  Return FALSE if the SSID is one of these special values
//  being set by WZC.
{
    boolean isValid = TRUE;
    UINT    i;

    if (pSSID->SsidLength == 32)
    {
        isValid = FALSE;
        for (i = 0; i < 32; i++)
            if (!(0 < pSSID->Ssid[i] && pSSID->Ssid[i] <= 0x1F))
            {
                isValid = TRUE;
                break;
            }
    }
    return isValid;
}

#ifdef DEBUG

static const char *
GetInfrastructureModeString(
    NDIS_802_11_NETWORK_INFRASTRUCTURE Mode)
{
    switch (Mode)
    {
    case Ndis802_11IBSS:              return "Ndis802_11IBSS";
    case Ndis802_11Infrastructure:    return "Ndis802_11Infrastructure";
    case Ndis802_11AutoUnknown:       return "Ndis802_11AutoUnknown";
    default: return "???";
    }
}


static const char *
GetAuthenticationModeString(
    NDIS_802_11_AUTHENTICATION_MODE Mode)
{
    switch (Mode)
    {
    case Ndis802_11AuthModeOpen:        return "Ndis802_11AuthModeOpen";
    case Ndis802_11AuthModeShared:      return "Ndis802_11AuthModeShared";
    case Ndis802_11AuthModeAutoSwitch:  return "Ndis802_11AuthModeAutoSwitch";
    case Ndis802_11AuthModeWPA:         return "Ndis802_11AuthModeWPA";
    case Ndis802_11AuthModeWPAPSK:      return "Ndis802_11AuthModeWPAPSK";
    case Ndis802_11AuthModeWPANone:     return "Ndis802_11AuthModeWPANone";
    case Ndis802_11AuthModeWPA2:        return "Ndis802_11AuthModeWPA2";
    case Ndis802_11AuthModeWPA2PSK:     return "Ndis802_11AuthModeWPA2PSK";
    default: return "???";
    }
}

static const char *
GetEncryptionStatusString(
    NDIS_802_11_ENCRYPTION_STATUS Status)
{
    switch (Status)
    {
    case Ndis802_11WEPEnabled:          return "Ndis802_11WEPEnabled";
    case Ndis802_11EncryptionDisabled:  return "Ndis802_11EncryptionDisabled";
    case Ndis802_11WEPKeyAbsent:        return "Ndis802_11WEPKeyAbsent";
    case Ndis802_11WEPNotSupported:     return "Ndis802_11WEPNotSupported";
    case Ndis802_11Encryption2Enabled:  return "Ndis802_11Encryption2Enabled";
    case Ndis802_11Encryption2KeyAbsent: return "Ndis802_11Encryption2KeyAbsent";
    case Ndis802_11Encryption3Enabled:  return "Ndis802_11Encryption3Enabled";
    case Ndis802_11Encryption3KeyAbsent: return "Ndis802_11Encryption3KeyAbsent";
    default: return "???";
    }
}

void
C802_11Miniport::Get80211OidTypeAndData(
    OUT char    *szBuffer,
    IN NDIS_OID Oid,
    IN PBYTE    Data)
{
    switch(Oid)
    {
    case OID_802_11_BSSID:

        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_BSSID %02X%02X%02X%02X%02X%02X", Data[0], Data[1], Data[2], Data[3], Data[4], Data[5]);
        break;

    case OID_802_11_SSID:
        {
        NDIS_802_11_SSID *SSID = (NDIS_802_11_SSID *)Data;

        if (SSIDIsValid(SSID))
            StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_SSID Len=%u '%.32s'", SSID->SsidLength, SSID->Ssid);
        else
            StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_SSID Len=%u INVALID SSID String (non printable ASCII)", SSID->SsidLength);
        }
        break;

    case OID_802_11_NETWORK_TYPES_SUPPORTED:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_NETWORK_TYPES_SUPPORTED");
        break;

    case OID_802_11_NETWORK_TYPE_IN_USE:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_NETWORK_TYPE_IN_USE");
        break;

    case OID_802_11_TX_POWER_LEVEL:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_TX_POWER_LEVEL");
        break;

    case OID_802_11_RSSI:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_RSSI");
        break;

    case OID_802_11_RSSI_TRIGGER:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_RSSI_TRIGGER");
        break;

    case OID_802_11_INFRASTRUCTURE_MODE:
        {
            NDIS_802_11_NETWORK_INFRASTRUCTURE Mode = *(NDIS_802_11_NETWORK_INFRASTRUCTURE *)Data;

            StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_INFRASTRUCTURE_MODE %hs", GetInfrastructureModeString(Mode));
        }
        break;

    case OID_802_11_FRAGMENTATION_THRESHOLD:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_FRAGMENTATION_THRESHOLD");
        break;

    case OID_802_11_RTS_THRESHOLD:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_RTS_THRESHOLD");
        break;

    case OID_802_11_NUMBER_OF_ANTENNAS:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_NUMBER_OF_ANTENNAS");
        break;

    case OID_802_11_RX_ANTENNA_SELECTED:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_RX_ANTENNA_SELECTED");
        break;

    case OID_802_11_TX_ANTENNA_SELECTED:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_TX_ANTENNA_SELECTED");
        break;

    case OID_802_11_SUPPORTED_RATES:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_SUPPORTED_RATES");
        break;

    case OID_802_11_DESIRED_RATES:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_DESIRED_RATES");
        break;

    case OID_802_11_CONFIGURATION:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_CONFIGURATION");
        break;

    case OID_802_11_DISASSOCIATE:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_DISASSOCIATE");
        break;

    case OID_802_11_STATISTICS:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_STATISTICS");
        break;

    case OID_802_11_POWER_MODE:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_POWER_MODE");
        break;

    case OID_802_11_BSSID_LIST_SCAN:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_BSSID_LIST_SCAN");
        break;

    case OID_802_11_BSSID_LIST:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_BSSID_LIST");
        break;

    case OID_802_11_PRIVACY_FILTER:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_PRIVACY_FILTER");
        break;

    case OID_802_11_RELOAD_DEFAULTS:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_RELOAD_DEFAULTS");
        break;

    case OID_802_11_AUTHENTICATION_MODE:
        {

        NDIS_802_11_AUTHENTICATION_MODE Mode = *(NDIS_802_11_AUTHENTICATION_MODE *)Data;
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_AUTHENTICATION_MODE %hs", GetAuthenticationModeString(Mode));
        }
        break;

    case OID_802_11_ENCRYPTION_STATUS:
        {
        NDIS_802_11_ENCRYPTION_STATUS EncryptionStatus = *(NDIS_802_11_ENCRYPTION_STATUS *)Data;

        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_ENCRYPTION_STATUS %hs", GetEncryptionStatusString(EncryptionStatus));
        }
        break;

    case OID_802_11_ADD_WEP:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_ADD_WEP");
        break;

    case OID_802_11_REMOVE_WEP:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_REMOVE_WEP");
        break;

    case OID_802_11_ADD_KEY:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_ADD_KEY");
        break;

    case OID_802_11_REMOVE_KEY:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_REMOVE_KEY");
        break;

    case OID_802_11_ASSOCIATION_INFORMATION:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_ASSOCIATION_INFORMATION");
        break;

    case OID_802_11_TEST:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_TEST");
        break;

#ifdef SUPPORT_WPA2
    case OID_802_11_CAPABILITY:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_CAPABILITY");
        break;

    case OID_802_11_PMKID:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "OID_802_11_PMKID");
        break;
#endif

    default:
        StringCchPrintfA(szBuffer, STRSAFE_MAX_CCH, "???");
        break;
    }
}
#endif // DEBUG

static MPOidInfo g_802_11SupportedQueryOids[] =
{
    { OID_802_11_BSSID,                    sizeof(NDIS_802_11_MAC_ADDRESS) },
    { OID_802_11_SSID,                     sizeof(NDIS_802_11_SSID) },
    { OID_802_11_NETWORK_TYPES_SUPPORTED,  sizeof(ULONG) },
    { OID_802_11_NETWORK_TYPE_IN_USE,      sizeof(NDIS_802_11_NETWORK_TYPE) },
    { OID_802_11_TX_POWER_LEVEL,           sizeof(NDIS_802_11_TX_POWER_LEVEL) },
    { OID_802_11_RSSI,                     sizeof(NDIS_802_11_RSSI) },
    { OID_802_11_RSSI_TRIGGER,             sizeof(NDIS_802_11_RSSI) },
    { OID_802_11_INFRASTRUCTURE_MODE,      sizeof(NDIS_802_11_NETWORK_INFRASTRUCTURE) },
    { OID_802_11_FRAGMENTATION_THRESHOLD,  sizeof(NDIS_802_11_FRAGMENTATION_THRESHOLD) },
    { OID_802_11_RTS_THRESHOLD,            sizeof(NDIS_802_11_RTS_THRESHOLD) },
    { OID_802_11_NUMBER_OF_ANTENNAS,       sizeof(ULONG) },
    { OID_802_11_RX_ANTENNA_SELECTED,      sizeof(NDIS_802_11_ANTENNA) },
    { OID_802_11_TX_ANTENNA_SELECTED,      sizeof(NDIS_802_11_ANTENNA) },
    { OID_802_11_SUPPORTED_RATES,          sizeof(NDIS_802_11_RATES) },
    { OID_802_11_DESIRED_RATES,            sizeof(NDIS_802_11_RATES) },
    { OID_802_11_CONFIGURATION,            sizeof(NDIS_802_11_CONFIGURATION) },
    { OID_802_11_STATISTICS,               sizeof(NDIS_802_11_STATISTICS) },
    { OID_802_11_POWER_MODE,               sizeof(NDIS_802_11_POWER_MODE) },
    { OID_802_11_BSSID_LIST,               sizeof(ULONG) },
    { OID_802_11_PRIVACY_FILTER,           sizeof(NDIS_802_11_PRIVACY_FILTER) },
    { OID_802_11_AUTHENTICATION_MODE,      sizeof(NDIS_802_11_AUTHENTICATION_MODE) },
    { OID_802_11_ENCRYPTION_STATUS,        sizeof(NDIS_802_11_ENCRYPTION_STATUS) },
    { OID_802_11_ASSOCIATION_INFORMATION,  sizeof(NDIS_802_11_ASSOCIATION_INFORMATION) },
#ifdef SUPPORT_WPA2
    { OID_802_11_CAPABILITY,               sizeof(NDIS_802_11_CAPABILITY) },
    { OID_802_11_PMKID,                    sizeof(NDIS_802_11_PMKID) },
//  { OID_802_11_MEDIA_STREAM_MODE,        sizeof(ULONG) },
#endif
#ifdef OS_ROAM_MANAGEMENT
    {OID_802_11_NIC_ROAM_CONTROL,          sizeof(NDIS_802_11_NIC_ROAM_CONTROL)},
    {OID_802_11_CHANNEL_LIST,              sizeof(NDIS_802_11_CHANNEL_LIST)},
    {OID_802_11_BSSID_RATE_RSSI_FILTER,    sizeof(NDIS_802_11_BSSID_RATE_RSSI_FILTER)},
    {OID_802_11_BSSID_RATE_RSSI_INFO,      sizeof(NDIS_802_11_BSSID_RATE_RSSI_INFO)},
#endif


    { 0,                             0 }
};

NDIS_STATUS
C802_11Miniport::QueryInformation(
    IN  NDIS_OID Oid,
    OUT PVOID    Buffer,
    IN  ULONG    cbBuffer,
    OUT PULONG   pcbWritten,
    OUT PULONG   pcbNeeded)
//
// QueryInformation is a required function that returns information
// about the capabilities and staus of the driver and/or its NIC.
//
{
    NDIS_STATUS                 Status = NDIS_STATUS_SUCCESS;

    // Check that OID is supported and buffer size is reasonable.
    Status = CheckOidRequest(&g_802_11SupportedQueryOids[0], Oid, cbBuffer, pcbNeeded);
    if (NDIS_STATUS_INVALID_OID == Status)
    {
        // OID is not a supported 802_11 OID. Check to see if it is an 802_3 OID.
        Status = C802_3Miniport::QueryInformation(Oid, Buffer, cbBuffer, pcbWritten, pcbNeeded);
        goto done;
    }
    if (NDIS_STATUS_SUCCESS != Status)
        goto done;

    switch (Oid)
    {
        //
        // 802_11 OIDS
        //
        case OID_802_11_BSSID:
            // When queried, this OID requests that the miniport driver return the MAC address
            // of the associated AP. If the device is operating in ad hoc mode,
            // the miniport driver returns the IBSS MAC address. The miniport driver
            // must return an error code of NDIS_STATUS_ADAPTER_NOT_READY if the device
            // is neither operating in ad hoc mode nor associated with an AP.
            Status = Get802_11AssociatedAPBSSID((NDIS_802_11_MAC_ADDRESS *)Buffer);
            break;

        case OID_802_11_SSID:
            // When queried, this OID requests that the miniport driver return the SSID
            // with which the device is associated. The miniport driver returns zero for
            // the SsidLength member if the device is not associated with an SSID.
            Get802_11SSID((NDIS_802_11_SSID *)Buffer);
            break;

        case OID_802_11_NETWORK_TYPES_SUPPORTED:
            // When queried, the OID_802_11_NETWORK_TYPES_SUPPORTED OID requests that
            // the miniport driver return an array of all the network types that the
            // underlying NIC's physical layer (PHY) supports. If the NIC supports
            // multiple 802.11 radios, the driver returns all applicable network types.
            {
                NDIS_802_11_NETWORK_TYPE *pTypes;
                ULONG                     numTypes;

                Get802_11NetworkTypesSupported(&pTypes, &numTypes);

                NDIS_802_11_NETWORK_TYPE_LIST *pNetTypeList = (NDIS_802_11_NETWORK_TYPE_LIST *)Buffer;
                *pcbNeeded = offsetof(NDIS_802_11_NETWORK_TYPE_LIST, NetworkType[0])
                           + numTypes * sizeof(NDIS_802_11_NETWORK_TYPE);
                pNetTypeList->NumberOfItems = numTypes;
                if (*pcbNeeded <= cbBuffer)
                    memcpy((PBYTE) Buffer + offsetof(NDIS_802_11_NETWORK_TYPE_LIST, NetworkType[0]), pTypes, numTypes * sizeof(NDIS_802_11_NETWORK_TYPE));
            }
            break;

        case OID_802_11_NETWORK_TYPE_IN_USE:
            // Return the network type that the underlying NIC's physical layer (PHY) will use:
            //   Ndis802_11FH       - Indicates the physical layer for the frequency-hopping spread-spectrum radio.
            //   Ndis802_11DS       - Indicates the physical layer for the direct-sequence spread-spectrum radio.
            //   Ndis802_11OFDM5    - Indicates the physical layer for 5-GHz OFDM radios.
            //   Ndis802_11OFDM24   - Indicates the physical layer for 2.4-GHz OFDM radios.
            //   Ndis802_11Automode - Indicates that the NIC will operate on all supported and enabled physical layers.
            Get802_11NetworkTypeInUse((PNDIS_802_11_NETWORK_TYPE)Buffer);
            break;

        case OID_802_11_TX_POWER_LEVEL:
            // When queried, this OID requests that the miniport driver return the transmit
            // power level, in mW, of the NIC.
            Status = Get802_11TxPowerLevel((NDIS_802_11_TX_POWER_LEVEL *)Buffer);
            break;

        case OID_802_11_RSSI:
            // When queried, the OID_802_11_RSSI OID requests that the miniport driver
            // return the current value of the received signal strength indication (RSSI).
            Status = Get802_11RSSI((NDIS_802_11_RSSI *)Buffer);
            break;

        case OID_802_11_RSSI_TRIGGER:
            // When queried, this OID requests that the miniport driver return the current
            // RSSI trigger value. The driver must return NDIS_STATUS_ADAPTER_NOT_READY if
            // an RSSI trigger has not been previously set.
            if (FALSE == m_bRSSITriggerSet)
                Status = NDIS_STATUS_ADAPTER_NOT_READY;
            else
                Status = Get802_11RSSITrigger((NDIS_802_11_RSSI *)Buffer);
            break;

        case OID_802_11_INFRASTRUCTURE_MODE:
            Get802_11InfrastructureMode((PNDIS_802_11_NETWORK_INFRASTRUCTURE)Buffer);
            break;

        case OID_802_11_FRAGMENTATION_THRESHOLD:
            // Return the fragmentation threshold setting.
            Status = Get802_11FragmentationThreshold((NDIS_802_11_FRAGMENTATION_THRESHOLD *)Buffer);
            break;

        case OID_802_11_RTS_THRESHOLD:
            // When queried, this OID requests that the miniport driver return the
            // request to send (RTS) threshold setting.
            Status = Get802_11RTSThreshold((NDIS_802_11_RTS_THRESHOLD *)Buffer);
            break;

        case OID_802_11_NUMBER_OF_ANTENNAS:
            // When queried, the OID_802_11_NUMBER_OF_ANTENNAS OID requests that
            // the miniport driver return the number of antennas on the underlying
            // NIC's radio. The number of antennas is returned in a value of type ULONG.
            Status = Get802_11NumberOfAntennas((ULONG *)Buffer);
            break;

        case OID_802_11_RX_ANTENNA_SELECTED:
            // When queried, this OID requests that the miniport driver return the
            // antenna number that identifies the currently selected receive antenna.
            Status = Get802_11RxAntennaSelected((NDIS_802_11_ANTENNA *)Buffer);
            break;

        case OID_802_11_TX_ANTENNA_SELECTED:
            // When queried, this OID requests that the miniport driver return the
            // antenna number that identifies the currently selected transmit antenna.
            Status = Get802_11TxAntennaSelected((NDIS_802_11_ANTENNA *)Buffer);
            break;

        case OID_802_11_SUPPORTED_RATES:
            // When queried, the OID_802_11_SUPPORTED_RATES OID requests that
            // the miniport driver return the underlying NIC's data rate set,
            // which includes the data rates that the NIC's radio supports.
            Get802_11SupportedRates((BYTE *)Buffer);
            break;

        case OID_802_11_DESIRED_RATES:
            // Return the NIC's set of data transmission rates
            Status = Get802_11DesiredRates((NDIS_802_11_RATES *)Buffer);
            break;

        case OID_802_11_CONFIGURATION:
            // Return the NIC's current radio configuration
            Get802_11Configuration((PNDIS_802_11_CONFIGURATION)Buffer);
            break;

        case OID_802_11_STATISTICS:
            // Return the current statistics for the IEEE 802.11 interface
            Status = Get802_11Statistics((PNDIS_802_11_STATISTICS)Buffer);
            break;

        case OID_802_11_POWER_MODE:
            // Return the current power mode of the NIC
            Status = Get802_11PowerMode((PNDIS_802_11_POWER_MODE)Buffer);
            break;

        case OID_802_11_BSSID_LIST:
            // Return a list containing all of the detected BSSIDs and their attributes.
            // This list contains all the BSSIDs that the device detected during its most
            // recent scan of potential BSSs. The miniport driver must respond to this OID
            // as quickly as possible.
            //
            // If this OID is queried without a preceding OID_802_11_BSSID_LIST_SCAN
            // set request, and the device is active, the miniport driver must return
            // a list of BSSIDs that are valid to join based on the device's current
            // configuration.
            //
            // If this OID is queried 6 seconds or longer after OID_802_11_BSSID_LIST_SCAN
            // is set, the list of BSSIDs must also contain all of the BSSIDs found during
            // the most recent network scan.
            Get802_11BSSIDList((PNDIS_802_11_BSSID_LIST_EX)Buffer, cbBuffer, pcbNeeded);
            break;

        case OID_802_11_AUTHENTICATION_MODE:
            // Return the IEEE 802.11 Authentication Mode
            Get802_11AuthenticationMode((NDIS_802_11_AUTHENTICATION_MODE *)Buffer);
            break;

        case OID_802_11_PRIVACY_FILTER:
            // Return the IEEE 802.1X privacy filter mode
            Status = Get802_11PrivacyFilter((PNDIS_802_11_PRIVACY_FILTER)Buffer);
            break;

        case OID_802_11_ENCRYPTION_STATUS:
            // Return the current encryption mode.
            // In response, the miniport driver can indicate which encryption mode is
            // enabled or disabled, that the transmit key is absent, or that encryption
            // is not supported.
            Get802_11EncryptionStatus((PNDIS_802_11_ENCRYPTION_STATUS)Buffer);
            break;

        case OID_802_11_ASSOCIATION_INFORMATION:
            // return the IEs that were used in the last association or reassociation
            // request to an access point. The driver also returns the IEs in the last
            // association or reassociation response from the access point.
            Status = Get802_11AssociationInformation((PNDIS_802_11_ASSOCIATION_INFORMATION)Buffer, cbBuffer, pcbNeeded);
            break;

#ifdef SUPPORT_WPA2
        case OID_802_11_PMKID:
            Status = Get802_11PMKID((NDIS_802_11_PMKID *)Buffer, cbBuffer, pcbNeeded);
            break;

        case OID_802_11_CAPABILITY:
            Status = Get802_11Capability((NDIS_802_11_CAPABILITY *)Buffer, cbBuffer, pcbNeeded);
            break;
#endif

#ifdef OS_ROAM_MANAGEMENT
        case OID_802_11_NIC_ROAM_CONTROL:
            Status = Get802_11RoamControl((PNDIS_802_11_NIC_ROAM_CONTROL)Buffer);
            break;

        case OID_802_11_CHANNEL_LIST:
            Status = Get802_11ChannelList((PNDIS_802_11_CHANNEL_LIST)Buffer, cbBuffer, pcbNeeded);
            break;

        case OID_802_11_BSSID_RATE_RSSI_FILTER:
            Status = Get802_11RateRssiFilter((PNDIS_802_11_BSSID_RATE_RSSI_FILTER)Buffer);
            break;

        case OID_802_11_BSSID_RATE_RSSI_INFO:
            Status = Get802_11RateRssiInfo((PNDIS_802_11_BSSID_RATE_RSSI_INFO)Buffer);
            break;
#endif //OS_ROAM_MANAGEMENT


        default:
            Status = NDIS_STATUS_INVALID_OID;
            break;
    }

#ifdef DEBUG
    char szBuffer[256];

    Get80211OidTypeAndData(szBuffer, Oid, (PBYTE)Buffer);
    NDIS_DEBUG_PRINTF(ATH_LOG_INF,"AR6K: Query Status=%x for %hs\n", Status, szBuffer);
#endif

    if (NDIS_STATUS_SUCCESS == Status)
    {
        if (*pcbNeeded > cbBuffer)
            Status = NDIS_STATUS_INVALID_LENGTH;
        else
            *pcbWritten = *pcbNeeded;
    }

done:
    return Status;
}

static MPOidInfo g_802_11SupportedSetOids[] =
{
    { OID_802_11_BSSID,                    sizeof(NDIS_802_11_MAC_ADDRESS) },
    { OID_802_11_SSID,                     sizeof(NDIS_802_11_SSID) },
    { OID_802_11_NETWORK_TYPE_IN_USE,      sizeof(NDIS_802_11_NETWORK_TYPE) },
    { OID_802_11_TX_POWER_LEVEL,           sizeof(NDIS_802_11_TX_POWER_LEVEL) },
    { OID_802_11_RSSI_TRIGGER,             sizeof(NDIS_802_11_RSSI) },
    { OID_802_11_INFRASTRUCTURE_MODE,      sizeof(NDIS_802_11_NETWORK_INFRASTRUCTURE) },
    { OID_802_11_FRAGMENTATION_THRESHOLD,  sizeof(NDIS_802_11_FRAGMENTATION_THRESHOLD) },
    { OID_802_11_RTS_THRESHOLD,            sizeof(NDIS_802_11_RTS_THRESHOLD) },
    { OID_802_11_RX_ANTENNA_SELECTED,      sizeof(NDIS_802_11_ANTENNA) },
    { OID_802_11_TX_ANTENNA_SELECTED,      sizeof(NDIS_802_11_ANTENNA) },
    { OID_802_11_DESIRED_RATES,            sizeof(NDIS_802_11_RATES) },
    { OID_802_11_CONFIGURATION,            sizeof(NDIS_802_11_CONFIGURATION) },
    { OID_802_11_DISASSOCIATE,             0 },
    { OID_802_11_POWER_MODE,               sizeof(NDIS_802_11_POWER_MODE) },
    { OID_802_11_BSSID_LIST_SCAN,          0 },
    { OID_802_11_PRIVACY_FILTER,           sizeof(NDIS_802_11_PRIVACY_FILTER) },
    { OID_802_11_RELOAD_DEFAULTS,          sizeof(NDIS_802_11_RELOAD_DEFAULTS) },
    { OID_802_11_AUTHENTICATION_MODE,      sizeof(NDIS_802_11_AUTHENTICATION_MODE) },
    { OID_802_11_ENCRYPTION_STATUS,        sizeof(NDIS_802_11_ENCRYPTION_STATUS) },
    { OID_802_11_ADD_WEP,                  offsetof(NDIS_802_11_WEP, KeyMaterial[0]) },
    { OID_802_11_REMOVE_WEP,               sizeof(NDIS_802_11_KEY_INDEX) },
    { OID_802_11_ADD_KEY,                  offsetof(NDIS_802_11_KEY, KeyMaterial[0]) },
    { OID_802_11_REMOVE_KEY,               sizeof(NDIS_802_11_REMOVE_KEY) },
    { OID_802_11_ASSOCIATION_INFORMATION,  sizeof(NDIS_802_11_ASSOCIATION_INFORMATION) },
    { OID_802_11_TEST,                     sizeof(NDIS_802_11_TEST) },
#ifdef SUPPORT_WPA2
    { OID_802_11_PMKID,                    offsetof(NDIS_802_11_PMKID, BSSIDInfo) },
//  { OID_802_11_MEDIA_STREAM_MODE,        sizeof(ULONG) },
#endif
#ifdef OS_ROAM_MANAGEMENT
    {OID_802_11_NIC_ROAM_CONTROL,          sizeof(NDIS_802_11_NIC_ROAM_CONTROL)},
    {OID_802_11_BSSID_CHANNEL_SCAN,        sizeof(NDIS_802_11_BSSID_CHANNEL_SCAN)},
    {OID_802_11_BSSID_RATE_RSSI_FILTER,    sizeof(NDIS_802_11_BSSID_RATE_RSSI_FILTER)},
#endif

    { ATH_OID_802_11_SSID,                 sizeof(NDIS_802_11_SSID) },
    { ATH_OID_802_11_DISASSOCIATE,         0 },
    { 0,                                   0 }
};

NDIS_STATUS
C802_11Miniport::SetInformation(
    IN  NDIS_OID Oid,
    OUT PVOID    Buffer,
    IN  ULONG    cbBuffer,
    OUT PULONG   pcbWritten,
    OUT PULONG   pcbNeeded)
{
    NDIS_STATUS                 Status = NDIS_STATUS_SUCCESS;

    // Check that OID is supported and buffer size is reasonable.

    Status = CheckOidRequest(&g_802_11SupportedSetOids[0], Oid, cbBuffer, pcbNeeded);

    if (NDIS_STATUS_INVALID_OID == Status)
    {
        // OID is not a supported 802_11 OID. Check to see if it is an 802_3 OID.
        Status = C802_3Miniport::SetInformation(Oid, Buffer, cbBuffer, pcbWritten, pcbNeeded);
        goto done;
    }
    if (NDIS_STATUS_SUCCESS != Status)
        goto done;

    switch (Oid)
    {
        //
        // 802_11 OIDS
        //
        case OID_802_11_BSSID:
            Status = Set802_11AssociatedAPBSSID((NDIS_802_11_MAC_ADDRESS *)Buffer);
            break;

        case OID_802_11_SSID:
            Status = Set802_11SSID((NDIS_802_11_SSID *)Buffer);
            break;

        case ATH_OID_802_11_SSID:
            m_ForceDisable = FALSE;
            Status = Set802_11SSID((NDIS_802_11_SSID *)Buffer);
            break;

        case OID_802_11_NETWORK_TYPE_IN_USE:
            Status = Set802_11NetworkTypeInUse(*(NDIS_802_11_NETWORK_TYPE *)Buffer);
            break;

        case OID_802_11_TX_POWER_LEVEL:
            Status = Set802_11TxPowerLevel(*(NDIS_802_11_TX_POWER_LEVEL *)Buffer);
            break;

        case OID_802_11_RSSI_TRIGGER:
            Status = Set802_11RSSITrigger(*(NDIS_802_11_RSSI *)Buffer);
            break;

        case OID_802_11_INFRASTRUCTURE_MODE:
            Status = Set802_11InfrastructureMode(*(NDIS_802_11_NETWORK_INFRASTRUCTURE *)Buffer);
            break;

        case OID_802_11_FRAGMENTATION_THRESHOLD:
            Status = Set802_11FragmentationThreshold(*(NDIS_802_11_FRAGMENTATION_THRESHOLD *)Buffer);
            break;

        case OID_802_11_RTS_THRESHOLD:
            Status = Set802_11RTSThreshold(*(NDIS_802_11_RTS_THRESHOLD *)Buffer);
            break;

        case OID_802_11_RX_ANTENNA_SELECTED:
            Status = Set802_11RxAntennaSelected(*(ULONG *)Buffer);
            break;

        case OID_802_11_TX_ANTENNA_SELECTED:
            Status = Set802_11TxAntennaSelected(*(ULONG *)Buffer);
            break;

        case OID_802_11_DESIRED_RATES:
            Status = Set802_11DesiredRates((NDIS_802_11_RATES *)Buffer);
            break;

        case OID_802_11_CONFIGURATION:
            Status = Set802_11Configuration((NDIS_802_11_CONFIGURATION *)Buffer);
            break;

        case OID_802_11_DISASSOCIATE:
            Status = Set802_11Disassociate();
            break;

        case ATH_OID_802_11_DISASSOCIATE:
            m_ForceDisable = TRUE;
            Status = Set802_11Disassociate();
            break;

        case OID_802_11_POWER_MODE:
            Status = Set802_11PowerMode(*(NDIS_802_11_POWER_MODE *)Buffer);
            break;

        case OID_802_11_BSSID_LIST_SCAN:
            Status = Set802_11BSSIDListScan();
            break;

        case OID_802_11_PRIVACY_FILTER:
            Status = Set802_11PrivacyFilter(*(NDIS_802_11_PRIVACY_FILTER *)Buffer);
            break;

        case OID_802_11_RELOAD_DEFAULTS:
            Status = Set802_11ReloadDefaults(*(NDIS_802_11_RELOAD_DEFAULTS *)Buffer);
            break;

        case OID_802_11_AUTHENTICATION_MODE:
            Status = Set802_11AuthenticationMode(*(NDIS_802_11_AUTHENTICATION_MODE *)Buffer);
            break;

        case OID_802_11_ENCRYPTION_STATUS:
            Status = Set802_11EncryptionStatus(*(NDIS_802_11_ENCRYPTION_STATUS *)Buffer);
            break;

        case OID_802_11_ADD_WEP:
            Status = Set802_11AddWEP((NDIS_802_11_WEP *)Buffer);
            break;

        case OID_802_11_REMOVE_WEP:
            Status = Set802_11RemoveWEP(*(NDIS_802_11_KEY_INDEX *)Buffer);
            break;

        case OID_802_11_ADD_KEY:
            Status = Set802_11AddKey((NDIS_802_11_KEY *)Buffer, FALSE);
            break;

        case OID_802_11_REMOVE_KEY:
            Status = Set802_11RemoveKey((NDIS_802_11_REMOVE_KEY *)Buffer);
            break;

        case OID_802_11_ASSOCIATION_INFORMATION:
            Status = Set802_11AssociationInformation((NDIS_802_11_ASSOCIATION_INFORMATION *)Buffer, cbBuffer);
            break;

#ifdef SUPPORT_WPA2
        case OID_802_11_PMKID:
            Status = Set802_11PMKID((NDIS_802_11_PMKID*)Buffer, cbBuffer);
            break;
#endif

        case OID_802_11_TEST:
            Status = Set802_11Test((NDIS_802_11_TEST *)Buffer,cbBuffer);
            break;

#ifdef OS_ROAM_MANAGEMENT
        case OID_802_11_NIC_ROAM_CONTROL:
            Status = Set802_11RoamControl((PNDIS_802_11_NIC_ROAM_CONTROL)Buffer);
            break;

        case OID_802_11_BSSID_CHANNEL_SCAN:
            Status = Set802_11ChannelScan((PNDIS_802_11_BSSID_CHANNEL_SCAN)Buffer);
            break;

        case OID_802_11_BSSID_RATE_RSSI_FILTER:
            Status = Set802_11RateRssiFilter((PNDIS_802_11_BSSID_RATE_RSSI_FILTER)Buffer);
            break;
#endif //OS_ROAM_MANAGEMENT


        default:
            ASSERT(FALSE);
            Status = NDIS_STATUS_INVALID_OID;
            break;
    }

#ifdef DEBUG
    char szBuffer[256];

    Get80211OidTypeAndData(szBuffer, Oid, (PBYTE)Buffer);
    NDIS_DEBUG_PRINTF(ATH_LOG_INF, "AR6K: Set Status=%x for %hs\n", Status, szBuffer);
#endif

done:
    return Status;
}

PNDIS_OID
C802_11Miniport::GetSupportedOidList(
    OUT     PNDIS_OID  pOidBuffer,
    IN  OUT PULONG     pMaxOidsToAddToBuffer,
    IN  OUT PULONG     pTotalNumberOfSupportedOids)
//
//  Add the OIDs supported by the driver to pOidBuffer, but
//  do not add more than MaxOidsToAddToBuffer.
//
//  TotalNumberOfSupportedOids is set to the number of Oids
//  that the driver supports.
//
//  Returns a pointer to the next unused slot in pOidBuffer
//  after the oids have been added.
//
{
    PNDIS_OID   p802_11OidListStart;
    ULONG       OidListCount = 0;

    // First fill in base class supported OIDs
    p802_11OidListStart = C802_3Miniport::GetSupportedOidList(pOidBuffer, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

    // Now append 802.11 OIDs
    AddOidsToList(&g_802_11SupportedQueryOids[0], p802_11OidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);
    AddOidsToList(&g_802_11SupportedSetOids[0],   p802_11OidListStart, &OidListCount, pMaxOidsToAddToBuffer, pTotalNumberOfSupportedOids);

    return p802_11OidListStart + OidListCount;
}

void
C802_11Miniport::Get802_3MulticastList(
    OUT PBYTE  *pMulticastAddresses,
    OUT PULONG  pNumMulticastAddresses)
// The multicast address list on the NIC enabled for packet reception.
{
    *pMulticastAddresses = &m_MulticastAddresses[0][0];
    *pNumMulticastAddresses = m_numMulticastAddresses;
}

ULONG
C802_11Miniport::Get802_3MaxMulticastListSize()
// The maximum number of multicast addresses the NIC driver can manage.
{
    return DEFAULT_802_11_MAX_MULTICAST_ADDRESSES;
}

NDIS_STATUS
C802_11Miniport::Set802_3MulticastList(
    IN PBYTE pMulticastAddreses,
    IN ULONG NumMulticastAddresses)
//
//  Set the multicast address list. This function just saves the settings
//  for use in software filtering. If hardware filtering is supported then
//  the device specific subclass method should handle hardware configuration.
//
{
    NDIS_STATUS Status = NDIS_STATUS_SUCCESS;

    if (NumMulticastAddresses > DEFAULT_802_11_MAX_MULTICAST_ADDRESSES)
    {
        Status = NDIS_STATUS_INVALID_LENGTH;
    }
    else
    {
        memcpy(&m_MulticastAddresses[0][0], pMulticastAddreses, NumMulticastAddresses * ETHERNET_MAC_ADDRESS_LENGTH);
        m_numMulticastAddresses = NumMulticastAddresses;
    }

    return Status;
}


