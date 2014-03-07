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
// Registry strings shared among various cellcore components.
//
//*********************************************************/
#define REG_szPhoneSettingsRegKey                           TEXT("ControlPanel\\Phone") // HKEY_CURRENT_USER
#define REG_szPhoneSettingsSecureRegKey                     TEXT("Security\\Phone")     // HKEY_LOCAL_MACHINE
#define REG_szPhoneSettings_Features                        TEXT("Features")
#define REG_szPhoneSettings_Flags2                          TEXT("Flags2")
#define REG_szPhoneSettings_DefaultAddressID                TEXT("DefaultAddressID")
#define REG_szPhoneSettings_Multiline                       TEXT("ML")
#define REG_szPhoneSettings_MultilineFeatures               TEXT("MLFeatures")

#define PSF2_LOG_SR_CALLS   0x0010 // call timers: log suspend resume calls

#define REG_szPhoneNetSettingsRegKey                        TEXT("ControlPanel\\PhoneNetSelMode")
#define REG_szPhoneNetSettings_Option                       TEXT("Option")
#define REG_szPhoneNetSettings_ManualNet                    TEXT("ManualNet")
#define REG_szPhoneNetSettings_ManualNetAcT                 TEXT("ManualNetAcT")
