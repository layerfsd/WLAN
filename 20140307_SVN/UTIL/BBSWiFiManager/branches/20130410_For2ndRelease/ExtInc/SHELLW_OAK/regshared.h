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
//**********************************************************
//
// Registry strings shared among various components.
//
//*********************************************************/

#define REG_szPhoneSettingsRegKey                           TEXT("ControlPanel\\Phone")
#define REG_szPhoneSettings_Flags2                          TEXT("Flags2")
// It is assumed that these flags are set very rarely, either at
// boot time or when the device is configured by a network
// operator. No end user action changes these flags.
// If this word is not present, all bits are assumed to be not set.

    #define PSF2_SUPERMATCH                      0x0002  // smart-filter all props (vs. just smart)
    #define PSF2_UILR                            0x0004  // dialer: show UI for L/R discoverability
    #define PSF2_ENDSUSPENDABLE                  0x0008  // callpol: end suspend-resumable calls on VK_TEND
    #define PSF2_LOG_SR_CALLS                    0x0010  // call timers: log suspend resume calls
    #define PSF2_HIDE_PIN2                       0x0020  // set high to hide change PIN2 UI
    #define PSF2_DONTRESTORELINE                 0x0040  // Return to line that we dialed from
    #define PSF2_SOFTKEY1ISHOLD                  0x0080  // SK1 offers hold not mute during call.
                                                         // Set bit 0x80 to 1 to make SK1 hold during a call.
                                                         // If bit 0x80 is zero or the word is not present,
                                                         // SK1 will be mute during a call.

    #define PSF2_NOFORMATNUMBER                 0x0100  // don't pretty-print
    #define PSF2_FILTERSTARPOUND                0x0200  // filter */#
// coming soon
//  #define PSF2_FILTERALPHA                    0x0400  // smart-filter a-z (vs. just 0-9)

// in HKEY_CURRENT_USER under REG_szPhoneSettingsRegKey
#define REG_szPhoneSettings_DialerLineSwitchOff             TEXT("DialerLineSwitchOff")
                                                         // Set to zero to turn off line switching from the dialer.
#define REG_szPhoneSettings_DialerLineSwitchUsesStar        TEXT("DialerLineSwitchUsesStar")
                                                         // Set to 1 to make the dialer uses the * key not the # to switch line.

#define REG_szPhoneSettings_SuppressDurationViewAllCalls    TEXT("SuppressDurationViewAllCalls")
#define REG_szPhoneSettings_DisplayDialedNumberTime         TEXT("DisplayDialedNumTime")
#define REG_szPhoneSettings_SendCallID                      TEXT("SendCallID")
#define REG_szPhoneSettings_DefaultAddressID                TEXT("DefaultAddressID")
#define REG_szPhoneSettings_CallHistoryMax                  TEXT("CallHistoryMax")
#define REG_szPhoneSettings_AnyKey                          TEXT("AnyKey")
#define REG_szPhoneSettings_AutoAnswer                      TEXT("AutoAns")
#define REG_szPhoneSettings_Volume                          TEXT("Vol")
#define REG_szPhoneSettings_VoicePrivacyNotification        TEXT("VoicePrivacyNotification")
    #define VPN_DEFAULTVOICEPRIVACY                               1
#define REG_szPhoneSettings_Features                        TEXT("Features")
#define REG_szPhoneSettings_NoKeypadPlus                    TEXT("NoKeypadPlus")     // Allow the OEM to prevent a '+' from showing up on the keypad or being translated from press-n-hold '0'
#define REG_szPhoneSettings_SupressUSSDUI                   TEXT("SuppressUSSD")
#define REG_szPhoneSettings_IgnoreUSSDExclusions            TEXT("USSDExclude")
#define REG_szPhoneSettings_ShortStringExclusionList        TEXT("ShortStringExclusionList")
#define REG_szPhoneSettings_ShowSIM                         TEXT("ShowSIM")
#define REG_szPhoneSettings_SortSIMLast                     TEXT("SortSIMContactsLast")
#define REG_szPhoneSettings_MinFixedDialingAlphaTagLen      TEXT("MinFixedDialingAlphaTagLen")
#define REG_szPhoneSettings_MaxFavoriteContacts             TEXT("MaxFavContacts")
#define REG_szPhoneSettings_AnalogRoamText                  TEXT("s1")
#define REG_szPhoneSettings_DigitalRoamText                 TEXT("s2")

#define REG_szPhoneSettings_AutoPrefix                      TEXT("AutoPrefix")
#define REG_szPhoneSettings_InternationalCode               TEXT("InternationalCode")

#define REG_szPhoneSettings_SynchronousMuteCheck            TEXT("syncmute")
#define REG_szPhoneSettings_SimSecShowBranding              TEXT("SimSecBrand")
    #define SIMSEC_BRAND_VALIDSIM                           (0)
    #define SIMSEC_BRAND_NEVER                              (1)
    #define SIMSEC_BRAND_ALWAYS                             (2)
#define REG_szPhoneSettings_SimSecAllowStarPound            TEXT("SimSecAllowStarPound")

#define REG_szPhoneSettings_SimSecPasswdDescriptionTextColor  TEXT("SimSecPasswdDescriptionTextColor")
#define REG_szPhoneSettings_SimSecKeypadActionButtonTextColor TEXT("SimSecKeypadActionButtonTextColor")
#define REG_szPhoneSettings_SimSecKeypadButtonTextColor       TEXT("SimSecKeypadButtonTextColor")
#define REG_szPhoneSettings_SimSecPasswdDescriptionStylefont  TEXT("SimSecPasswdDescriptionStylefont")
#define REG_szPhoneSettings_SimSecKeypadActionButtonStylefont TEXT("SimSecKeypadActionButtonStylefont")
#define REG_szPhoneSettings_SimSecKeypadButtonNumberStylefont TEXT("SimSecKeypadButtonNumberStylefont")
#define REG_szPhoneSettings_SimSecKeypadButtonLetterStylefont TEXT("SimSecKeypadButtonLetterStylefont")

#define REG_szPhoneSettings_NoCallHandleTimeout             TEXT("nocallhandletimeout")
#define REG_szPhoneSettings_PhoneWatsonDumpFlags            TEXT("watsondumpflags")
#define REG_szPhoneSettings_Dbgbreaknocallhandletimeout     TEXT("dbgbreaknocallhandletimeout")
#define REG_szPhoneSettings_SmartDialFilterMaxWait          TEXT("sdMaxWait")
#define REG_szPhoneSettings_TrackEndedCallTimeout           TEXT("TrackEndedCallTimeout")
#define REG_szPhoneSettings_TrackAnsweredCallTimeout        TEXT("TrackAnsweredCallTimeout")

#define REG_szPhoneSettings_ShowSimVoiceMailBubble          TEXT("ShowSimVoiceMailBubble")
#define REG_szPhoneSettings_EnablePushToTalkSupport         TEXT("EnablePushToTalkSupport")

// no longer used (formerly had some cprog OEM bits UI_FEATURE_ROAM_TEXT/etc
// that were moved to wpcpriv.h as SHDFL_PHONE_SUPPORTROAMTEXT/etc
// #define REG_szPhoneSettings_Flags                           TEXT("Flags")
#define REG_szPhoneSettings_CallIDMatch                     TEXT("CallIDMatch")
#define REG_szPhoneSettings_dwVMailCPL                      TEXT("VMailCPL")
#define REG_szPhoneSettings_Multiline                       TEXT("ML")
#define REG_szPhoneSettings_MultilineFeatures               TEXT("MLFeatures")

#define REG_szPhoneSettings_PhoneAutomation                 TEXT("PhoneAutomationBuild")

#define REG_szPhoneSettings_DisableDialpad                  TEXT("DisDial")
    #define DISABLEDIALPAD_PORTRAIT                         0x01
    #define DISABLEDIALPAD_LANDSCAPE                        0x02
    #define DISABLEDIALPAD_SQUARE                           0x04
    #define DISABLEDIALPAD_ANY                              (DISABLEDIALPAD_PORTRAIT | DISABLEDIALPAD_LANDSCAPE | DISABLEDIALPAD_SQUARE)
    #define REPLACEDIALPAD_PORTRAIT                         0x10
    #define REPLACEDIALPAD_LANDSCAPE                        0x20
    #define REPLACEDIALPAD_SQUARE                           0x40
    #define REPLACEDIALPAD_ANY                              (REPLACEDIALPAD_PORTRAIT | REPLACEDIALPAD_LANDSCAPE | REPLACEDIALPAD_SQUARE)

#define REG_szPhoneSettings_DisableMatchesAccum             TEXT("NoMatchesAccum")
#define REG_szPhoneSettings_FullScreenSDOnLaunch            TEXT("KeypadStateOnLaunch")
#define REG_szPhoneSettings_FullScreenSDOrientations        TEXT("KeypadStateOrientations")

#define REG_szPhoneSettings_DialpadDLL                      TEXT("DialpadDLL")
#define REG_SZPhoneSettings_WndProcExport                   TEXT("DialpadWndPrc")

// Control whether SIP should be available in full screen SmartDial's menu bar
// If the regkey is not present or set to 1 then the SIP will be present in the menu bar
// If the regkey is set to 0 then the SIP will not be present in the menu bar
#define REG_szPhoneSettings_SIP                             TEXT("SIP")

// Control whether "Go to Contacts" should be displayed in SmartDial's SK2 menu
// If the regkey is set to 1 then this menu item will be present
// If the regkey is not present or set to 0 then the menuitem will not be present
#define REG_szPhoneSettings_GoToContacts                    TEXT("GoToContacts")

// Network settings
#define REG_szNetworkSettings                               TEXT("ControlPanel\\Network")
// Control whether or not read-only networks are viewable by the user.
#define REG_szNetworkSettingsViewCMNetworks                 TEXT("ViewCMNetworks")

// Control whether to make voice mail notification iconic (no bubble) when user presses Dismiss, or just hide the bubble (PPC only)
#define REG_szPhoneSettings_VmailHideOnDismiss              TEXT("VmailHideOnDismiss")

#define REG_szTapiDialRegKey                                TEXT("ControlPanel\\Dial")
#define REG_szTapiDial_CurrentLoc                           TEXT("CurrentLoc")
#define REG_szTapiDial_HighLocID                            TEXT("HighLocID")
#define REG_szTapiDialLocationsRegKey                       TEXT("ControlPanel\\Dial\\Locations")
//a word on Tapi dial locations:
// Each lives as a numeric sub key, as a MULTI_SZ in the following format:
// LocationName, LocalFmt, LdFmt, IntntlFmt, AreaCode, CwCode, CountryCode, & Options

#define REG_szPhoneNetSettingsRegKey                        TEXT("ControlPanel\\PhoneNetSelMode")
#define REG_szPhoneNetSettings_Option                       TEXT("Option")
#define REG_szPhoneNetSettings_ManualNet                    TEXT("ManualNet")
#define REG_szPhoneNetSettings_ManualNetAcT                 TEXT("ManualNetAcT")
#define REG_szPhoneNetSettings_NumRetries                   TEXT("NumRetries")
#define REG_szPhoneNetSettings_RetryDuration                TEXT("RetryDuration")
#define REG_szPhoneNetSettings_Timeout                      TEXT("Timeout")

#define REG_szHomeSettingsRegKey                            TEXT("ControlPanel\\Home")
#define REG_szHomeSettings_Scheme                           TEXT("Scheme")
#define REG_szColorSchemeSettings_Scheme                    TEXT("ColorScheme")
#define REG_szBgImageSettings_Scheme                        TEXT("BgImage")
#define REG_szBgImageNameSettings_Scheme                    TEXT("CurBgImageName")
#define REG_szHomeSettings_IntermediateFileCurrent          TEXT("HIFFileCurrent")
#define REG_szHomeSettings_IntermediateFileNew              TEXT("HIFFileNew")

#define REG_szPowerSettingsRegKey                           TEXT("ControlPanel\\Power")
#define REG_szPowerSettings_DisplayTimeout                  TEXT("Display")
#define REG_szPowerSettings_DisplayTimeoutLocked            TEXT("DisplayLocked")
#define REG_szPowerSettings_DisplayTimeoutSaved             TEXT("DisplaySaved")

#define REG_szShellSettings                                 TEXT("Software\\Microsoft\\Shell")
#define REG_szHasKeyboard                                   TEXT("HasKeyboard")
#define REG_szClearHasKeyboard                              TEXT("ClearHasKeyboard")

#define REG_szNotificationSettings                          TEXT("ControlPanel\\Notifications")
#define REG_szNotificationSettings_AlertInterrupt           TEXT("AlertInterrupt")
    #define ALERTINTERRUPT_ENABLED                          (1)

#define REG_szNotificationSettingsRegKey                    TEXT("ControlPanel\\Notifications")
#define REG_szNotificationSettings_HangupStopOnlyRinger     TEXT("HangupStopOnlyPhoneRinger")
    #define HANGUPSTOPONLYRINGER_ENABLED                    (1)

// under HKEY_LOCAL_MACHINE
#define REG_szSecurityContactsKey                           TEXT("Security\\Contacts")
#define REG_szSecurityContacts_RefreshFrequencyDuringImport TEXT("RefreshFrequencyDuringImport")

#define REG_szSecurityContactsStateKey                      TEXT("Security\\Contacts\\State")
#define REG_szSecurityContactsState_SIMImportStatus         TEXT("SIMImportStatus")
    #define SIMIMPORTSTATUS_NOTRUNNING                      (0)
    #define SIMIMPORTSTATUS_IMPORT                          (1)

#define REG_szSecurityPhoneSettingKey                       TEXT("Security\\Phone")
#define REG_szSecurityPhoneSetting_SpeedDialTalk            TEXT("SpeedDialOnTalk")
#define REG_szShowRoamingIconsInCallHistory                 TEXT("ShowRoamingIconsInCallHistory")

#define REG_szAlphaDialingEnabled                           TEXT("AlphaDialingEnabled")
#define REG_szAlphaDialingTranslations                      TEXT("AlphaDialingTranslations")
#define REG_szDefaultInputMode                              TEXT("DefaultInputMode")

// Controlling the handling of special characters in dial strings.  See phcanvas.h for more details.
#define REG_szSendCodesToRilDial                            TEXT("SendCodesToRilDial")
    #define SENDCODES_PAUSE                                 0x01
    #define SENDCODES_CLIR                                  0x02

#define REG_szSecurityVoIPPhoneSettingKey                   TEXT("Security\\Phone\\VoIP")
#define REG_szSecurityVoIPPhoneSetting_CheckDateTimeTimeout TEXT("CheckDateTimeTimeout")
#define REG_szSecurityVoIPPhoneSetting_OverrideNetworkTimeout TEXT("OverrideNetworkTimeout")
#define REG_szSecurityVoIPPhoneSetting_WaitAfterSendDTMFTimeout TEXT("WaitAfterSendDTMFTimeout")
#define REG_szSecurityVoIPPhoneSetting_IsSIMRequired        TEXT("RequireSIM")
#define REG_szSecurityVoIPPhoneSetting_Regex                TEXT("Regex")
#define REG_szSecurityVoIPPhoneSetting_RegexSMSBlockedNumberKey TEXT("Security\\Phone\\VoIP\\SMSBlockedNumber")
#define REG_szSecurityVoIPPhoneSetting_RegexVoIPPhoneNumberKey TEXT("Security\\Phone\\VoIP\\VoIPPhoneNumber")
#define REG_szSecurityVoIPPhoneSetting_DisableSettingsUI    TEXT("DisableSettingsUI")
#define REG_szSecurityVoIPPhoneSetting_VoIPPhoneCanvas      TEXT("VoIPPhoneCanvasIsAvailable")
#define REG_szSecurityVoIPPhoneSetting_VoIPPhoneOverride    TEXT("VoIPPhoneOverride")
#define REG_szSecurityVoIPPhoneSetting_HandsetModeMaxVolume TEXT("HandsetModeMaxVolume")
#define REG_szSecurityVoIPPhoneSetting_SpeakerModeMaxVolume TEXT("SpeakerModeMaxVolume")
#define REG_szSecurityVoIPPhoneSetting_ProvisioningKey0     TEXT("Security\\Phone\\VoIP\\Provisioning0")
#define REG_szSecurityVoIPPhoneSetting_ProvisioningKey1     TEXT("Security\\Phone\\VoIP\\Provisioning1")

#define REG_szMatchKey                                      TEXT("Security\\Phone\\PhoneKeyMatch")
#define REG_szKeyboardMappings                              TEXT("Security\\Phone\\PhoneKeyMatch\\KeyboardMappings")
#define REG_szOnScreenKeypadMappings                        TEXT("Security\\Phone\\PhoneKeyMatch\\OnScreenKeypadMappings")
#define REG_szAltLangProcessing                             TEXT("AlternateLanguageProcessing")
#define REG_szSmartFilterFlags                              TEXT("SmartFilterFlags")
#define REG_szMatchWildCard                                 TEXT("Wildcard")
#define REG_szSendAllKeysToDialer                           TEXT("SendAllKeysToDialer")

#define REG_szColorRegKey                                   TEXT("SOFTWARE\\Microsoft\\Color")
#define REG_szColor_Default                                 TEXT("DefSHColor")
#define REG_szColorBaseHue                                  TEXT("BaseHue")
#define REG_szThemeColors                                   TEXT("ThemeColors")
#define REG_szThemeValues                                   TEXT("ThemeValues")

#define REG_SystemGWERegKey                                 TEXT("SYSTEM\\GWE")
#define REG_szSysColor_Default                              TEXT("DefSysColor")

#define REG_szMetricRegKey                                  TEXT("SOFTWARE\\Microsoft\\Metric")
#define REG_szMetric_Default                                TEXT("DefSHMetric")

#define REG_szShellStartupRegKey                            TEXT("Software\\Microsoft\\Shell\\StartUp")
#define REG_szShellStartup_AppLaunchDelay                   TEXT("AppLaunchDelay")

#define REG_szStartRegKey                                   TEXT("Software\\Microsoft\\Shell\\StartMenu")
#define REG_szStart_Order                                   TEXT("Order")
#define REG_szStart_GridView                                TEXT("GridView")
#define REG_szStart_Command                                 TEXT("Command")

#define REG_szFolderOrderRegKey                             TEXT("Software\\Microsoft\\Shell\\FolderOrder")
#define REG_szFolderOrder_Order                             TEXT("Order")

#define REG_szAccessibilitySettingsRegKey                   TEXT("ControlPanel\\Accessibility")

#define REG_szSpeechRegKey                                  TEXT("Software\\Microsoft\\Speech")
#define REG_szSpeech_Engine                                 TEXT("Engine")
#define REG_szSpeech_PlayDelay                              TEXT("PlayDly")
#define REG_szSpeech_SRDelay                                TEXT("SRDly")
#define REG_szSpeech_BeepDelay                              TEXT("BeepDly")
#define REG_szSpeech_LastTag                                TEXT("LastTag")

#define REG_hkeySANBase                                     HKEY_LOCAL_MACHINE
#define REG_szSANBase                                       TEXT("Software\\Microsoft\\Shell\\Rai\\")

#define REG_szIFFSRegKey                                    TEXT("Software\\Microsoft\\IFFS")
#define REG_szIFFS_MajorVersion                             TEXT("MajorV")
#define REG_szIFFS_MinorVersion                             TEXT("MinorV")

#define REG_szVMailRegKey                                   TEXT("Software\\Microsoft\\Vmail")
#define REG_szVMailFlags                                    TEXT("VMailFlags")
#define REG_szVMailHidePluginWhenCountUnknown               TEXT("HidePluginWhenVMailCountUnknown")

#define REG_szIgnoredExtModemsKey                           TEXT("Software\\Microsoft\\Shell\\IgnoredExtModems")

#define REG_szEmergencyRegKey                               TEXT("Software\\Microsoft\\Shell\\HomePlugins")
#define REG_szExplicitEmerSup                               TEXT("ExplicitEmergencySupport")

#define REG_szStylusRegKey                                  TEXT("ControlPanel\\Stylus")
#define REG_szText_FontSizeArray                            TEXT("FontArray")
#define REG_szText_CurFont                                  TEXT("CurFont")


#define REG_szBetaKey                                       TEXT("Software\\Microsoft\\Shell\\DeviceBeta")
#define REG_szBeta_About                                    TEXT("About")
#define REG_szBeta_Today                                    TEXT("Today")

#define REG_szVersionKey                                    TEXT("System\\Versions")
#define REG_szAkuVersion                                    TEXT("Aku")

#define REG_szBluetoothSettingsKey                          TEXT("Software\\Microsoft\\Bluetooth\\Settings")
#define REG_szBluetoothIgnoreExternal                       TEXT("IgnoreExternal")

#define REG_szBthBondKey                                    TEXT("Software\\Microsoft\\Bthbond")

#define REG_szBthHandsfreeOnKey                             TEXT("Software\\Microsoft\\Bluetooth\\HandsfreeProfileOn")


#define REG_szLucyRegKey                                    TEXT("Software\\Microsoft\\Lucy")
#define REG_szLucyTestMode                                  TEXT("TestMode")

#define REG_szDataListRegKey                                TEXT("Software\\Microsoft\\DataList")
#define REG_szDataList_SupportGetItemText                   TEXT("SupportGetItemText")
#define REG_szDataList_SupportGetItemOid                    TEXT("SupportGetItemOid")

#define REG_szSpeedDialOverride                             TEXT("Software\\Microsoft\\Phone\\SpeedDial\\SpeedDialOverride")
#define REG_szSpeedDialOverrideDLL                          TEXT("DLLName")

#define REG_szRegPathToday                                    TEXT("Software\\Microsoft\\Today")
#define REG_szRegValueSkin                                    TEXT("Skin")

#define REG_szVoiceRegistryKey                              TEXT("Software\\Microsoft\\Voice")
#define REG_szRegAllowInCallRecord                          TEXT("AllowInCallRecording")          // Do we want In Call Recording

#define REG_szPHCanvasSkinning                              TEXT("Security\\Phone\\Skin")
#define REG_szPHCanvasEnable                                TEXT("Enabled")
#define REG_szPHCanvasOEMDLL                                TEXT("ext")

#define REG_szPHRingtoneSearch                              TEXT("RingtoneSearch")
#define REG_szPHSoundSearch                                 TEXT("SoundSearch")

#define REG_hkeySndValidBase                                HKEY_CURRENT_USER
#define REG_szSndValidSoundsKey                             TEXT("ControlPanel\\Sounds")
#define REG_szSndValidRingtones                             TEXT("Ringtones")
#define REG_szSndValidSounds                                TEXT("AllSounds")

#define REG_hkeyRingtonesCustomBase                         HKEY_LOCAL_MACHINE
#define REG_szRingtonesCustomKey                            TEXT("ControlPanel\\Sounds\\CustomSounds\\Ringtones")
#define REG_szRingtonesCustom                               TEXT("Directory")

#define CLOCK_REG_ENTRY                                     TEXT("Software\\Microsoft\\Clock")

#define REG_hkeySmsUiHive                                   HKEY_CURRENT_USER
#define REG_szSmsUiKey                                      TEXT("Software\\Microsoft\\SMS")
#define REG_szDisableSmsUiValue                             TEXT("DisableSmsUi")
#define REG_szEnableSms2UiValue                             TEXT("EnableRadioAsSms2Ui")

#define REG_hkeyPhoneLock                                   HKEY_LOCAL_MACHINE
#define REG_szPhoneLockKey                                  TEXT("Security\\Phone")
#define REG_szPhoneLock                                     TEXT("L")
#define REG_szPhoneLockConfirm                              TEXT("KeyLockConfirm")
#define REG_szPhoneLockConfirmPrompt                        TEXT("KeyLockConfirmPrompt")
#define REG_szPhoneKeyLockOnly                              TEXT("KeyLockOnly")

// Registry settings for determining whether to allow SMS service to send to email addresses
#define REG_szRegKeySettings                                TEXT("SYSTEM\\Inbox\\Settings")
#define REG_szRegAllowSMStoSMTPAddress                      TEXT("AllowSMStoSMTPAddress")
#define REG_szRegSMStoSMTPShortCode                         TEXT("SMStoSMTPShortCode")
#define REG_szRegAllowSMStoSMTPConcatenation                TEXT("AllowSMStoSMTPConcatenation")

// Registry setting to override VCard support over SMS
#define REG_szEnableVcardUI                                 TEXT("EnableVcardUI")

//Registry settings to stop importing FDN and SDN entries to contacts DB
#define REG_szRegKeySettingsPIM                            TEXT("Software\\Microsoft\\PIM")
#define REG_szRegNotImportFDN                              TEXT("DoNotImportFDN")
#define REG_szRegNotImportSDN                              TEXT("DoNotImportSDN")

// Registry setting to determine if cumulative call timers are displayed
#define REG_szDisableCCTimersPath                          TEXT("Security\\Phone")
#define REG_szDisableCCTimers                               TEXT("DisableCCTimers")

// Registry settings to determine the maximum number of recipients
// in a SMS message, uses the same path above
#define REG_szLimitSMSRecipients                      TEXT("LimitSMSRecipients")

// Registry setting for determining whether to allow Email service to send to sms addresses
#define REG_szRegAllowSMTPtoSMSAddress                      TEXT("AllowSMTPtoSMSAddress")

// Registry setting for Dial Parser extension
#define REG_hkeyDialParser                                  HKEY_LOCAL_MACHINE
#define REG_szRegDialParserKey                              TEXT("Software\\Microsoft\\DialParser")
#define REG_szRegDialParserOEMDll                           TEXT("DLL-OEM")

// Registry setting for specific Rocker functionality within apps.
#define REG_szShellKey                                      TEXT("Software\\Microsoft\\Shell")
#define REG_szRockerSupport                                 TEXT("RockerSupport")
#define ROCKER_STARTMENU_ENABLED                            0x1
#define ROCKER_OUTLOOK_ENABLED                              0x2

//Registry settings for SQM/CEIP
#define REG_szSQMRegKey                                     TEXT("System\\SQM")
#define REG_szSQMControlStatus                              TEXT("Enabled")
#define REG_szSQMUIStatus                                   TEXT("EnableUI")
#define REG_szSQMUIPromptDelayMin                           TEXT("PromptDelayMin")
#define REG_szSQMCPLRegKey                                  TEXT("ControlPanel\\Customer Feedback")
#define REG_szSQMCPLRegValue                                TEXT("Redirect")

// Registry settings for ignoring AutoFormating of particular strings
#define REG_szIgnoreAutoFormatStringsKey                    TEXT("SYSTEM\\Shell")
#define REG_szIgnoreAutoFormatStrings                       TEXT("IgnoreAutoFormatStrings")

// Registry settings for USIM call control dialog policy
#define REG_szSIMModifiedRegKey                             TEXT("ControlPanel\\Phone")
#define REG_szPhoneSettings_DisableCallModifiedDialogs      TEXT("DisableCallModifiedDialogs")
#define DISABLE_CALLMODDIALOG_VOICE_TO_VOICE        0x01
#define DISABLE_CALLMODDIALOG_VOICE_TO_USSD         0x02
#define DISABLE_CALLMODDIALOG_USSD_TO_USSD          0x04
#define DISABLE_CALLMODDIALOG_USSD_TO_VOICE         0x08

#define REG_szSyncRegKey                                    TEXT("Security\\Sync")
#define REG_szSyncRapiMinTimeout                            TEXT("RapiMinTimeout")

// Clock
#define REG_szRegAlarmSnoozeKey                             TEXT("System\\Shell\\Clock\\Snooze")

// this file must end in a newline because it is included from resource files

