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

#ifndef  __SIM_SECURITY_H__
#define  __SIM_SECURITY_H__

#ifdef __cplusplus
extern "C" {
#endif

enum SIM_SECURITY_TASK
{
    SIMSEC_TASK_ANY = 0,
    SIMSEC_TASK_UNLOCK,     // Unlock the phone
    SIMSEC_TASK_CHANGE,    // Change  pin code
    SIMSEC_TASK_ENABLE,    // Enable  pin code protection
    SIMSEC_TASK_DISABLE,   // Disable pin code protection
    SIMSEC_TASK_REQUEST,  // request pin from user, but don't verify with SIM
    SIMSEC_TASK_CHECK,
};

enum SIMPIN_TYPE
{
    SIMPIN_SIM,
    SIMPIN_SIM2,
};

// GSM spec 2.17 sez passwords are min 4, max 8 digits, PUKs are
// always 8 digits
const INT MINLENGTH_SIMPASSWORD = 4;
const INT MINLENGTH_SIMPUKWORD = 8;
const INT MAXLENGTH_SIMPASSWORD = 8;

// style flags
#define SIMSEC_STYLE_EMERGENCY_CALL         0x00000001
#define SIMSEC_STYLE_AUTO_CLOSE             0x00000002
#define SIMSEC_STYLE_SHOW_ACCEPT            0x00000004
#define SIMSEC_STYLE_CANNOT_CANCEL          0x00000008
#define SIMSEC_STYLE_BRANDED                0x00000010		// Only supported on smartphone
#define SIMSEC_STYLE_BACKGROUNDIMAGE        0x00000020		

// fake facility so we can use this UI to get the call barring
// password (use this only with SIMSEC_TASK_REQUEST)
#define SIM_FAKEFACILITY_CALLBARRING  (0x1 << (SIM_NUMLOCKFACILITIES + 1))

HRESULT SIMSecurityUI(HWND hwndParent, enum SIM_SECURITY_TASK nTask, DWORD dwLockFacility, DWORD dwStyle, void *pVoid);
HRESULT SIMSecurityUnlockPhone(HWND hwndParent, DWORD dwStyle);
HRESULT SIMSecurityChangePin(HWND hwndParent, DWORD dwLockFacility, DWORD dwStyle);
HRESULT SIMSecurityEnablePin(HWND hwndParent, DWORD dwLockFacility, DWORD dwStyle, BOOL fEnable);
// pszPinOutBuff must be at least MAXLENGTH_SIMPASSWORD + 1
HRESULT SIMSecurityRequestPin(HWND hwndParent, DWORD dwLockFacility, DWORD dwStyle, TCHAR* pszPinOutBuff);

#ifdef __cplusplus
}
#endif

#endif //__SIM_SECURITY_H__
