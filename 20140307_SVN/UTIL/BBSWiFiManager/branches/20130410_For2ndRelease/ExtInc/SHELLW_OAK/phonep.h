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
// phonep.h
//
// Phone API as exposed by aygshell.dll.
// Some functions are thunks that call into the CProg.exe PSL.
//
// Author: JasonFul 10/18/00

#pragma once 

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <olectl.h>

typedef struct 
{
    BOOL  fInCall;                // does a call currently exist?
    TCHAR szDisplayName [80];    // other party's name, if known
    TCHAR szDisplayNumber [64]; // other party's phone number
    TCHAR szDisplayCompanyName [80];  // other party's company name, if known
    TCHAR szDisplayMethod[10]; // work/work2/home/home2/etc indicator
    SYSTEMTIME stStartTime;        // start time of the call
    DWORD dwFlags;              // call state flags
    CEOID oidClog;              // sqloid (to associating notes to calls)    
    void* hCall;                // CallPolicy call handle
    CEOID oidsqlContact;        // sqloid for the contact.
    CEOID oidClogPim;           // pimoid for the call history item
} PHONECALLPROPS, *PPHONECALLPROPERTIES;


// PhoneGetCallProperties
//
// Retrieves information about the current phone call.  If there is no call,
// and dwFlags is  is true, the last call will be returned.  Otherwise,
// pcallprops->fInCall will be FALSE and the values of the other fields are
// undefined.
HRESULT PhoneGetCallProperties (DWORD dwFlags, PHONECALLPROPS *pcallprops); 

#define PGCP_CURRENTCALL       0
#define PGCP_LASTCONNECTEDCALL 1

// AssociateNoteWithCall
//
// Associates a file with a call log.
HRESULT AssociateNoteWithCall (LPTSTR szFileName);


// PhoneGetCallPropertyBag
//
// Retrieves information about the current phone call, as a property bag.
// If there is no call, this function will gracefully fail and *pppropbag 
// will be NULL.
// If there is a call,the Phone's property bag will be returned.
//
// The property set exposed by the Phone is:
//
// property name:   type:      value:
// --------------   -----      ------
// PropSet          VT_BSTR    "Phone"
// Name             VT_BSTR    the name of the other person on the call, if known. 
//                              A null string otherwise.
// Number           VT_BSTR    the phone number of the other person on the call
// CompanyName      VT_BSTR    the company name of the other person on the call, if known. 
//                              A null string otherwise.
// Year             VT_I2      the year of the beginning time of the call
// Month            VT_I2      the month of the beginning time of the call
// Day              VT_I2      the day of the beginning time of the call
// Hour             VT_I2      the hour of the beginning time of the call  
// Minute           VT_I2      the minute of the beginning time of the call
// Second           VT_I2      the second of the beginning time of the call
// DayOfWeek        VT_I2      the day of the week of the beginning time of the call
//
// This function is most often used to respond to the NMN_GETPROPERTYBAG 
// notification.

HRESULT PhoneGetCallPropertyBag (DWORD dwFlags, IPropertyBag **pppropbag);



// PhoneSpeedDialOverride
//
// Allows the OEM the ability to override the execution
// of a speed dial.

typedef struct tagSPEEDDIALOVERRIDE
{
    
    DWORD       cbSize;             // Set to the size of the structure.
    DWORD       dwFlags;            // Flags. See note below.
    CEBLOB      blobOverrideData;   // Contains the data for a speed dial source. See note below.

} SPEEDDIALOVERRIDE, *PSPEEDDIALOVERRIDE;

// Speed dial override data
//
// One of these structures will be contained in the blobOverrideData member and will
// corrospond to the speed dial source flag. Currently only the contacts source is implemented.

typedef struct tagSPEEDDIALOVERRIDEDATABASE
{

    DWORD       cbSize;         // Set to the size of the structure.
    HANDLE      hDatabase;      // The HANDLE to the database that the speed dial points to.
    CEOID       oidRec;         // The CEOID of the record that the speed dial points to.
    CEPROPID    propidRec;      // The CEPROPID of the property that the speed dial points to.

} SPEEDDIALOVERRIDEDATABASE, *PSPEEDDIALOVERRIDEDATABASE;

// Flags
//
// The flags field will contain one speed dial action
// and one speed dial source.

// Speed dial action

#define SPEED_DIAL_KEYPRESS     0x1
#define SPEED_DIAL_VOICE        0x2

// Speed dial source

#define SPEED_DIAL_CONTACTS     0x10

// The OEM uses the following registry key if they decide to implement
// the speed dial override.
//
// [HKLM\Software\Microsoft\Phone\SpeedDial\SpeedDialOverride]
// "DLLName"    = <string> (This is the OEM DLL that the speed dial override procedure will be loaded from)

// The OEM speed dial override procedure must have the following form:
// HRESULT PhoneSpeedDialOverride(const SPEEDDIALOVERRIDE * psdOverride);

// The procedure will be loaded and run using the following:

#define SPEEDDIALOVERRIDEFUNCNAME TEXT("PhoneSpeedDialOverride")
typedef HRESULT (*SPEEDDIALOVERRIDEFUNC) (const SPEEDDIALOVERRIDE * psdOverride);

// Every time a speed dial is initiated on the device the speed dial override
// procedure will be called if it has been provided by the OEM. If the OEM
// wishes to let the Windows Mobile software handle the speed dial normally
// the speed dial override function should return S_OK. If the OEM wishes to
// handle the speed dial in a proprietary manner the speed dial override
// function should return E_ABORT and the Windows Mobile software will not
// process the speed dial. 


// LoadLangMap
//
// Load language specific mapping data and override current language specific mappings.
// Usable by trusted callers only.
//
// Parameters:
//  szMappings
//   [in opt] String containing the colon-delimited list of mappings.
//            See FESmartDialLayout.xls for more information.
//
// Return Value:
//  (HANDLE) Handle to LangMap identifier for use with UnloadLangMap.
//  INVALID_HANDLE_VALUE in case of failure.
//
// Remarks:
//  This API is only useful on Chinese, Japanese, and Korean devices
//  and only affects matching scenarios for characters in the language of the device.
//  Language specific mapping data is restored on reboot or after calling UnloadLangMap.
//  Be sure to call UnloadLangMap if the device for this mapping data is disconnected.
//  Set pszMappings to NULL to disable alternate character matching for
//  characters in the language of the device.
HANDLE LoadLangMap(__in_opt const TCHAR *pszMappings);

// UnloadLangMap
//
// Unload language specific mapping data and restore default language specific mappings.
// Usable by trusted callers only.
//
// Parameters:
//  hLangMap
//   [in] HANDLE to LangMap identifier returned by LoadLangMap.
//
// Return Value:
//  Standard HRESULT.
//
HRESULT UnloadLangMap(HANDLE hLangMap);


#ifdef __cplusplus
}
#endif
