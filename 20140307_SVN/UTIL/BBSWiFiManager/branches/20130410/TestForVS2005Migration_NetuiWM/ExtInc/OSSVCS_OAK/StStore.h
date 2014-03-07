//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/*****************************************************************************
    Copyright (c) Microsoft Corporation. All rights reserved.

    File: StStore.h

    Public interface to the status store

*/
#pragma once

#ifdef __cplusplus
extern "C"{
#endif

// Macros that define the layout of status data IDs.  Basicaly the same as CEPROPIDs
#define SDID_TYPE_MASK      0x0000FFFF
#define SDID_TYPE_SHIFT     0
#define SDID_INDEX_MASK     0xFFFF0000
#define SDID_INDEX_SHIFT    16
#define MAKE_SDID(type, index) ((((type) << SDID_TYPE_SHIFT) & SDID_TYPE_MASK) | (((index) << SDID_INDEX_SHIFT) & SDID_INDEX_MASK))
#define GET_SDID_TYPE(sdid) (((sdid) & SDID_TYPE_MASK) >> SDID_TYPE_SHIFT)
#define GET_SDID_INDEX(sdid) (((sdid) & SDID_INDEX_MASK) >> SDID_INDEX_SHIFT)
typedef DWORD StatusDataID;

//
// Build the list of status data ids
//

// First build up the indices
#define SDID_INFO(id, type, flags, subkey, valuename) autoNumber ## id,
enum enAutoNumber
{
    #include "sdids.h"
};

// Then the actual IDs.
#define SDID_INFO(id, type, flags, subkey, valuename) const StatusDataID id = MAKE_SDID(type, autoNumber ## id);
#include "sdids.h"

// SIM Support Capabilities Field (SDID_SIMSUPPORT_CAPS)
#define SIMSUPPORT_NONE                    (0x00000000)
#define SIMSUPPORT_BASIC                   (0x00000001)

// Phone multiline support capabilities field (SDID_MULTILINE_CAPS)
#define MULTILINE_NONE                     (0x00000000)
#define MULTILINE_BASIC                    (0x00000001)

// Backup battery macros (SDID_BACKUP_BATTERY)
#define BACKUP_BATTERY_FLAGS(l)        (LOWORD(l))
#define BACKUP_BATTERY_STRENGTH(l)     (HIWORD(l))

// Battery macros (SDID_BATTERY)
#define BATTERY_FLAGS(l)                (LOWORD(l))
#define BATTERY_STRENGTH(l)             (HIWORD(l))

// Battery status flags (SDID_BATTERY, SDID_BACKUP_BATTERY)
#define BSF_NO_BATTERY                  0x0001      // No battery.
#define BSF_CHARGING                    0x0002      // Battery is charging.
#define BSF_LOW                         0x0004      // Battery is low.
#define BSF_CRITICAL_LOW                0x0008      // Battery is critically low.

// Battery strength ranges (SDID_BATTERY, SDID_BACKUP_BATTERY)
#define BATTERY_LEVEL_VERY_LOW   0          // 0-20
#define BATTERY_LEVEL_LOW        21         // 21-40
#define BATTERY_LEVEL_MED        41         // 41-60
#define BATTERY_LEVEL_HI         61         // 61-80
#define BATTERY_LEVEL_VERY_HI    81         // 81-100

// Lock status flags (SDID_LOCK)
#define LSF_DEVICE_LOCKED       0x00000001  // Device locked
#define LSF_KEY_LOCKED          0x00000002  // Key locked
#define LSF_SIM_LOCKED          0x00000004  // SIM locked

// Phone status flags (SDID_PHONE_STATUS_FLAGS)
#define PSF_SIM_FULL            0x00000001  // SIM full.
#define PSF_NO_SIM              0x00000002  // No SIM.
#define PSF_INVALID_SIM         0x00000004  // Invalid SIM.
#define PSF_BLOCKED_SIM         0x00000008  // Blocked SIM.
#define PSF_RADIO_OFF           0x00000010  // Radio on/off.
#define PSF_RADIO_PRESENT       0x00000020  // Radio plugged in
#define PSF_RINGER_OFF          0x00000040  // Ringer on/off.
#define PSF_LINE_1              0x00000080  // Selected line 1.
#define PSF_LINE_2              0x00000100  // Selected line 2.
#define PSF_REGISTERED_ROAM     0x00000200  // Roaming service.
#define PSF_CALL_FORWARDING_LINE1 0x00000400  // Call forwarding on line 1.
#define PSF_MISSED_CALL         0x00000800  // Missed call.
#define PSF_DATA_CALL           0x00001000  // Active data call.
#define PSF_CALL_BARRING        0x00002000  // Call barring.
#define PSF_CALL_ON_HOLD        0x00004000  // Call on hold.
#define PSF_CONFERENCE          0x00008000  // Conference.
#define PSF_INCOMING_CALL       0x00010000  // Incoming call.
#define PSF_CALL_CALLING        0x00020000  // Attempting to connect
#define PSF_THIRD_PARTY_CALL    0x00040000  // Active third-party call (may be data or voice)
#define PSF_ANALOG              0x00080000  // Analog/digital.
#define PSF_GPRS                0x00100000  // GPRS coverage
#define PSF_UNREGISTERED        0x00200000  // No service.
#define PSF_REGISTERING         0x00400000  // Searching for service.
#define PSF_REGISTERED_HOME     0x00800000  // Home service.
#define PSF_REGISTRATION_DENIED 0x01000000  // Service registration denied
#define PSF_GPRS_CONNECTION     0x02000000  // Call being made with GPRS connection.
#define PSF_REGISTRATION_REGISTERING_AFTER_DENIED 0x04000000 // Radio is registering after having been denied on a specific network
#define PSF_VOICE_PRIVACY_DROP  0x08000000  // Voice privacy is off
#define PSF_1XRTT               0x10000000  // 1xRTT coverage
#define PSF_CALL_TALKING        0x20000000  // Call talking
#define PSF_CALL_FORWARDING_LINE2 0x40000000  // Call forwarding on line 2.
#define PSF_LOCATION            0x80000000 // Location Based Services On / Off

// Phone status flags extended (SDID_PHONE_STATUS_FLAGS_EX)
#define EPSF_VIBRATE_ON         0x00000001  // Vibrate on/off.
#define EPSF_LINE_INTERNET      0x00000002  // Selected Internet line.
#define EPSF_CALL_TRANSFERRING  0x00000004  // Call is transferring
#define EPSF_RADIO_EMERGENCY    0x00000008  // Radio is available but only for emergency calls

// VoIP status flags (SDID_VOIP_STATUS_FLAGS)

//Enabled Status (off is non-zero)
#define VSF_OFF                  0x00000001 //off should have some value (so it can be detected...)
#define VSF_ON                   0x00000002
#define VSF_MISSING_REQUIRED_SIM 0x00000004 //operator requires a valid SIM
#define VSF_SERVICE_BLOCKED      0x00000008 //operator has blocked service
#define VSF_ENABLED_BITMASK     (VSF_OFF | VSF_ON | VSF_MISSING_REQUIRED_SIM | VSF_SERVICE_BLOCKED)

//Registration status (mutually exclusive...)
#define VSF_NO_SIP_SETTINGS     0x00000010        // No SIP registration settings.
#define VSF_UNREGISTERED        0x00000020        // No VoIP service.
#define VSF_REGISTERING         0x00000040        // Searching for service.
#define VSF_REGISTERED          0x00000080        // VoIP service.
#define VSF_REGISTRATION_ERROR  0x00000100        // Some error ocurred during registration
#define VSF_REGISTRATION_ERROR_NO_CONNECTION 0x00000200 // Registration error due to unavailable connection
#define VSF_REGISTRATION_STATUS_BITMASK (VSF_NO_SIP_SETTINGS | VSF_UNREGISTERED | VSF_REGISTERING | VSF_REGISTERED | VSF_REGISTRATION_ERROR | VSF_REGISTRATION_ERROR_NO_CONNECTION)

//voicemail status (mutually exclusive...)
#define VSF_NO_VOICEMAIL_SETTINGS  0x00000400 // No voicemail subscription settings
#define VSF_VOICEMAIL_UNSUBSCRIBED 0x00000800  // No voicemail service.
#define VSF_VOICEMAIL_SUBSCRIBED   0x00001000  // Voicemail service.
#define VSF_VOICEMAIL_SUBSCRIPTION_ERROR 0x00002000 // Some error ocurred during subscription
#define VSF_VOICEMAIL_STATUS_BITMASK (VSF_NO_VOICEMAIL_SETTINGS | VSF_VOICEMAIL_UNSUBSCRIBED | VSF_VOICEMAIL_SUBSCRIBED | VSF_VOICEMAIL_SUBSCRIPTION_ERROR)

// Phone Network Preferences (SDID_PHONE_NETWORK_PREFERENCES)
typedef enum tagPHONE_NETWORK_PREFERENCES
{
    PNP_VOIP_OFF                            = 0x00000000,
    PNP_VOIP_WHENEVER_AVAILABLE             = 0x00000001,
    PNP_VOIP_EXCEPT_NIGHTS_AND_WEEKENDS     = 0x00000002,
    PNP_VOIP_ONLY_IF_CELL_IS_NOT_AVAILABLE  = 0x00000003,
    PNP_LAST                                = PNP_VOIP_ONLY_IF_CELL_IS_NOT_AVAILABLE,
} PHONE_NETWORK_PREFERENCES;

//incoming voice call barring values
#define  LKGSETTING_CALLBARRING_DISABLED                0x0
#define  LKGSETTING_CALLBARRING_ENABLED                 0x1
#define  LKGSETTING_CALLBARRING_ENABLED_WHEN_ROAMING    0x2

#define MAX_FORWARD_NUMBER_LEN  256
typedef struct tagLKG_VOICEFORWARDING_DATA
{
    BOOL    fForwardAlways; //Always forward - enabled?
    TCHAR   ForwardAlwaysNumber[MAX_FORWARD_NUMBER_LEN];

    BOOL    fForwardOnBusy; //Forward on busy - enabled?
    TCHAR   ForwardOnBusyNumber[MAX_FORWARD_NUMBER_LEN];

    BOOL    fForwardOnTimeout; //Forward on timeout - enabled?
    UINT    TimeoutInSeconds;
    TCHAR   ForwardOnTimeoutNumber[MAX_FORWARD_NUMBER_LEN];
} LKG_VOICEFORWARDING_DATA;


// Synchronize states (SDID_SYNCHRONIZE)
#define SSF_INSYNC              0x00000001  // Synchronizing

// Radio ready states (SDID_RADIO_READY_STATE)
#define RRSF_READYSTATE_INITIALIZED  0x00000001  // The Radio has been initialized (but may not be ready)
#define RRSF_READYSTATE_SIM          0x00000002  // The Radio is ready for SIM Access
#define RRSF_READYSTATE_SMS          0x00000004  // The Radio is ready for SMS messages
#define RRSF_READYSTATE_UNLOCKED     0x00000008  // The SIM has been unlocked
#define RRSF_READYSTATE_SIM_PB       0x00000010  // The SIM PB has been fully copied to volatile memory and is ready for access 
#define RRSF_READYSTATE_ALL          (RRSF_READYSTATE_INITIALIZED | RRSF_READYSTATE_SIM | RRSF_READYSTATE_SMS | RRSF_READYSTATE_UNLOCKED | RRSF_READYSTATE_SIM_PB)


// Notify states (SDID_NOTIFY)
// #define NSF_NEW_VOICE_MSG        0x00000001  // New voice message.
#define NSF_NEW_SMS                 0x00000002  // New SMS.
#define NSF_NEW_EMAIL               0x00000004  // New E-mail.
#define NSF_NOTIFY_DIALOG_ACTIVE    0x00000008  // Set if the notification dialog is active. Unset if it is dismissed.
#define NSF_NEW_IM                  0x00000010  // New IM message
#define NSF_NEW_VM_LINE1            0x00000020  // New voice message on Line 1 of 2 (on a SIM with CPHS/ALS).
#define NSF_NEW_VM_LINE2            0x00000040  // New voice message on Line 2 of 2 (on a SIM with CPHS/ALS).
#define NSF_INCOMING_BEAM           0x00000080  // Currently beaming data
#define NSF_WIFI_CONNECTED          0x00000100  // Connected to a wifi network, not included in NSF_ALL since NSF_ALL includes all radio related flags only
#define NSF_NEW_VM_VOIP             0x00000200  // New voice message on VoIP line
#define NSF_NOTIFY_CLOCKALARM_ACTIVE 0x00000400  // Set if the Clock Alarm is active. Unset if it is dismissed.
#define NSF_NOTIFY_APPOINTMENT_ACTIVE 0x00000800  // Set if the Appointment is active. Unset if it is dismissed.
#define NSF_RADIO_ALL               (NSF_NEW_VM_LINE1 | NSF_NEW_VM_LINE2)
#define NSF_RESERVED1               0x00010000  // pseudo-bit, *never* write it to statstor, but reserve it for app internal use
#define NSF_RESERVED2               0x00020000  // ditto

// Input status flags (SDID_INPUT_STATE)
#define ISF_SPELL               0x00000001  // Spell (Multipress) mode.
#define ISF_AMBIG               0x00000002  // Ambig. (T9) mode.
#define ISF_NUMBERS             0x00000004  // Numbers mode.
#define ISF_CAPS                0x00000008  // Caps mode.
#define ISF_CAPSLOCK            0x00000018  // CapsLock mode.
#define ISF_CUSTOM              0x00000020  // OEM IME provides icon
#define ISF_INPUT_ALL           (ISF_SPELL | ISF_AMBIG | ISF_NUMBERS | ISF_CAPS | ISF_CAPSLOCK | ISF_CUSTOM)

// Bluetooth status flags (SDID_BLUETOOTH)
#define BTSF_POWER_ON           0x00000001  // power/connectability is on
#define BTSF_DISCOVERABLE       0x00000002  // discoverability is on
#define BTSF_A2DP_CONNECTED     0x00000004  // A2DP is currently connected
#define BTSF_HARDWARE_PRESENT   0x00000008  // Bluetooth hardware is present
#define BTSF_HANDSFREE_CONTROL  0x00000010  // handsfree device is under SCO control
#define BTSF_HANDSFREE_AUDIO    0x00000020  // handsfree device is under control and audio link open

// Wi-Fi status flags (SDID_WIFI)
#define WSF_HARDWARE_PRESENT    0x00000001  // Wi-Fi hardware is present
#define WSF_POWER_ON            0x00000002  // Wi-Fi is powered on
#define WSF_NETWORKS_AVAILABLE  0x00000004  // Wi-Fi networks are available
#define WSF_CONNECTING          0x00000008  // Wi-Fi is connecting
#define WSF_CONNECTED           0x00000010  // Wi-Fi is connected

// SHNotification flags (SDID_SHNOTIFICATION)
#define SNF_OEM_NOTIF1          0x00000001  // Change of CLSID_SHNAPI_OemNotif1
#define SNF_OEM_NOTIF2          0x00000002  // Change of CLSID_SHNAPI_OemNotif2
#define SNF_OEM_NOTIF3          0x00000004  // Change of CLSID_SHNAPI_OemNotif3
#define SNF_OEM_NOTIF4          0x00000008  // Change of CLSID_SHNAPI_OemNotif4
#define SNF_OEM_NOTIF5          0x00000010  // Change of CLSID_SHNAPI_OemNotif5
#define SNF_OEM_NOTIF6          0x00000020  // Change of CLSID_SHNAPI_OemNotif6

// All PSF_* flags should be bucketed below, mainly so that they're updated
// properly when the radio turns off or is removed. (SDID_PHONE_STATUS_FLAGS)
#define PSF_SIM      (PSF_SIM_FULL | PSF_NO_SIM | PSF_INVALID_SIM | PSF_BLOCKED_SIM)

#define PSF_PHONECALLSTATE (PSF_DATA_CALL | PSF_CALL_ON_HOLD | PSF_CONFERENCE |\
    PSF_INCOMING_CALL | PSF_CALL_CALLING | PSF_THIRD_PARTY_CALL | PSF_GPRS_CONNECTION | PSF_CALL_TALKING)

#define PSF_PHONE   (PSF_RINGER_OFF | PSF_REGISTERED_ROAM | PSF_LINE_1 | \
    PSF_LINE_2 | PSF_CALL_FORWARDING_LINE1 | PSF_CALL_FORWARDING_LINE2 | \
    PSF_MISSED_CALL | PSF_ANALOG | PSF_RADIO_OFF | PSF_UNREGISTERED | \
    PSF_CALL_BARRING | PSF_REGISTERING | PSF_REGISTRATION_DENIED | \
    PSF_REGISTRATION_REGISTERING_AFTER_DENIED | PSF_REGISTERED_HOME | \
    PSF_RADIO_PRESENT | PSF_GPRS | PSF_VOICE_PRIVACY_DROP | PSF_1XRTT | PSF_LOCATION)

// (SDID_SIGNAL_STRENGTH)
#define SIGNAL_LEVEL_VERY_LOW   10
#define SIGNAL_LEVEL_LOW        30
#define SIGNAL_LEVEL_MED        60
#define SIGNAL_LEVEL_HI         90
#define SIGNAL_LEVEL_VERY_HI    100

// Connection system available flags (SDID_CELLULAR_SYSTEM_AVAILABLE)
#define CSAF_GPRS           0x00000001  // GPRS available
#define CSAF_1XRTT          0x00000002  // 1XRTT available
#define CSAF_1XEVDO         0x00000004  // 1XEVDO available
#define CSAF_EDGE           0x00000008  // EDGE available
#define CSAF_UMTS           0x00000010  // UMTS available
#define CSAF_1XEVDV         0x00000020  // EVDV available
#define CSAF_HSDPA          0x00000040  // HSDPA available

// Connection system _connected_ flags (SDID_CELLULAR_SYSTEM_CONNECTED)
#define CSCF_NONE           0x00000000  // No connection established
#define CSCF_GPRS           0x00000001  // GPRS connected
#define CSCF_1XRTT          0x00000002  // 1XRTT connected
#define CSCF_1XEVDO         0x00000004  // 1XEVDO connected
#define CSCF_EDGE           0x00000008  // EDGE connected
#define CSCF_UMTS           0x00000010  // UMTS connected
#define CSCF_1XEVDV         0x00000020  // EVDV connected
#define CSCF_HSDPA          0x00000040  // HSDPA connected
#define CSCF_CSD            0x80000000  // CSD connected

// InternetSharing Status Flags (SDID_INTERNETSHARING)
#define ISSF_PROCESS_RUNNING  0x00000001 // Instance of InternetSharing is currently running
#define ISSF_ENABLED          0x00000002 // InternetSharing data session is currently enabled.  This means InternetSharing is either connecting or connected.
#define ISSF_DATA_CONNECTED   0x00000004 // IS is connected with a data call
#define ISSF_HOST_CONNECTED   0x00000008 // IS is connected with the user's pc
#define ISSF_HOST_USB         0x00000010 // Host is connected over USB
#define ISSF_HOST_BLUETOOTH   0x00000020 // Host is connected over BT

// Flags for AddCallbackIDs
#define CCIF_OPERATION_MASK 0x0000000F
#define CCIF_ADD            0x00000001 // Add the ids to any current callback ids for us
#define CCIF_RESET          0x00000002 // Remove any current IDs that we are monitoring and replace them with the ones passed in.

// flags for VoIP Settings (SDID_VOIP_UPDATED_SETTINGS)
#define  VUS_NO_UPDATE           0x00000000   // There is no update to any VoIP Setting
#define  VUS_ACTIVE_REG_SET      0x00000001   // Indicates where the active reg key is 
#define  VUS_SIP_SETTINGS        0x00000002   // SIP settings 
#define  VUS_VOICEMAIL_SETTINGS  0x00000004   // Voicemail server
#define  VUS_VOICEMAIL_NUMBER    0x00000008   // Voicemail number
#define  VUS_BACKUP_SIP_SETTINGS 0x00000010   // Backup SIP settings
#define  VUS_DIALPLAN            0x00000020   // Dial plan
#define  VUS_ALL_SETTINGS (VUS_SIP_SETTINGS | VUS_VOICEMAIL_SETTINGS | VUS_VOICEMAIL_NUMBER | VUS_BACKUP_SIP_SETTINGS | VUS_DIALPLAN)

// Values for Mobile VPN connection status (SDID_MOBILEVPN_STATUS)
enum MOBILEVPN_STATUS
{
    MOBILEVPN_DISABLED         = 0x00000001,  // VPN disabled
    MOBILEVPN_DISCONNECTED     = 0x00000002,  // VPN disconnected
    MOBILEVPN_CONNECTING       = 0x00000003,  // VPN connecting
    MOBILEVPN_CONNECTED        = 0x00000004,  // VPN connected
    MOBILEVPN_DISCONNECTING    = 0x00000005,  // VPN disconnecting
};



typedef HRESULT (*PFN_ONSTATUSDATACHANGE)(UINT cids, const StatusDataID* rgids, LPARAM lParam);

EXTERN_C const IID IID_IStatStore;

// New code that uses StatStore should use GetStatStore and IStatStore
interface DECLSPEC_UUID("628B618D-F57A-4b8c-80F3-2F6FA7500979") IStatStore :
    public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetInfo(UINT cids, const StatusDataID* rgids, const CEPROPVAL* pVals) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUINT(StatusDataID id, BOOL fDelete, UINT uVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetString(StatusDataID id, BOOL fDelete, const TCHAR* pszVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInfo(UINT cids, const StatusDataID* rgids, CEPROPVAL** ppVals) = 0;
    virtual HRESULT STDMETHODCALLTYPE UpdateUINT(StatusDataID id, BOOL fDelete, UINT uVal, UINT uMask) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetCallback(HWND hwndParent, PFN_ONSTATUSDATACHANGE pfn, LPARAM lParam) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveCallback(HWND hwndParent) = 0;
    virtual HRESULT STDMETHODCALLTYPE ClearAll() = 0;
    virtual HRESULT STDMETHODCALLTYPE ChangeCallbackIDs(DWORD dwFlags, UINT cids, const StatusDataID* rgids) = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterForNotifications(HWND hwndParent, PFN_ONSTATUSDATACHANGE pfn, LPARAM lParam, UINT cids, const StatusDataID* rgids) = 0;
};
HRESULT GetStatStore(IStatStore** ppbs);

#ifdef __cplusplus
}
#endif


