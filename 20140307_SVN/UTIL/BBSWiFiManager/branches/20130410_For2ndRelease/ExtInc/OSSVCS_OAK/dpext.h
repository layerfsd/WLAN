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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) Microsoft Corporation

--*/

#ifndef _DPEXTENS_H_
#define _DPEXTENS_H_

// Dial Parser Extensibility response recommendations
#define DPEF_ALLOW_DIAL_REQUEST 0x01
#define DPEF_ABORT_DIAL_REQUEST 0x02
#define DPEF_REQUEST_NOT_SECURE 0x04

// Phone Accumulator Extensibility flags
#define PHAEF_DEFAULT       0x00000001  // Drawing dialing string
#define PHAEF_DTMF          0x00000002  // Drawing DTMF string

/****************************************************************************

   NOTES:

      The Dial Parser offers the ability to extend its functionality
      by means of a DLL which implements three functions as described
      below.

      An OEM implementor should include an entry in the registry, in
      the appropriate .reg file, which points to the name of the DLL
      which exports these functions.  For example:

      [HKEY_LOCAL_MACHINE\Software\Microsoft\DialParser\]
         "DLL-OEM"="DialParsExtOEM.dll"

      (The name of the DLL can be anything, but the registry key must
      be as shown.)

****************************************************************************

   FUNCTION: IsImmediateDialRequestRequired

   SIGNATURE:
      BOOL IsImmediateDialRequestRequired(LPCTSTR lpszDialString);

   PURPOSE:
      Give Dial Parser Extensibility a chance to decide whether
      the framework should regard the digits collected so far as
      an actionable dial string.

   PARAMETERS:
      lpszDialString - The digits collected so far

   RETURNS:
      TRUE if lpszDialString is a complete dial request, meaning
           that no additional digits need to be collected and
           the Talk button does not need to be pressed in order
           for a meaningful dial action to take place
      FALSE otherwise

****************************************************************************

   FUNCTION: PreprocessDialRequest

   SIGNATURE:
      DWORD PreprocessDialRequest(HWND hWnd, LPCTSTR lpszDialString, BOOL fSecure);

   PURPOSE:
      Give Dial Parser Extensibility a chance to intercept the
      Dial Request.  If the DLL chooses not to, the framework
      will send the Dial Request to RIL.

      The implementor can take whatever action is appropriate,
      including nothing at all, before returning.  The implementor
      may wish to use hWnd to display a MessageBox, or may wish to
      use tapiRequestMakeCall to issue a call to a number other
      than the one that was entered, etc.

      Typically, if the DLL takes action on its own, it will
      return DPEF_ABORT_DIAL_REQUEST, and if it does not take
      action on its own, it will return DPEF_ALLOW_DIAL_REQUEST.
      However, this is a recommendation, not a requirement.

   PARAMETERS:
      hWnd           - A window handle, typically used as a
                       parent window for MessageBoxes
      lpszDialString - The digits collected so far
      fSecure        - Whether the string represents a secure
                       call request

   RETURNS:
      DPEF_ALLOW_DIAL_REQUEST - the framework should do whatever
                                it would normally do
      DPEF_ABORT_DIAL_REQUEST - the framework should assume that
                                the dial request has been handled
                                and stop processing it further

****************************************************************************

FUNCTION: PreprocessDialRequestEx

   SIGNATURE:
      DWORD PreprocessDialRequestEx(HWND hWnd, LPCTSTR lpszDialString, BOOL fSecure, const PHONEMAKECALLINFO *pphMakeCallInfo);

   PURPOSE:
      This is the Extended version of PreprocessDialRequest.
      If this version is not present in the extensibility dll,
      the framework defaults to PreprocessDialRequest.
      
      Give Dial Parser Extensibility a chance to intercept the
      Dial Request.  If the DLL chooses not to, the framework
      will send the Dial Request to RIL.

      The implementor can take whatever action is appropriate,
      including nothing at all, before returning.  The implementor
      may wish to use hWnd to display a MessageBox, or may wish to
      use tapiRequestMakeCall to issue a call to a number other
      than the one that was entered, or respond accordingly to the
      PHMAKECALLINFO struct passed in, etc.

      Typically, if the DLL takes action on its own, it will
      return DPEF_ABORT_DIAL_REQUEST, and if it does not take
      action on its own, it will return DPEF_ALLOW_DIAL_REQUEST.
      However, this is a recommendation, not a requirement.

   PARAMETERS:
      hWnd           - A window handle, typically used as a
                       parent window for MessageBoxes
      lpszDialString - The digits collected so far  
      fSecure        - Whether the string represents a secure
                       call request
      pphMakeCallInfo- Pointer to PHMAKECALLINFO struct associated with lpszDialString.
                       NULL if there is no struct associated with it.

   RETURNS:
      DPEF_ALLOW_DIAL_REQUEST - the framework should do whatever
                                it would normally do
      DPEF_ABORT_DIAL_REQUEST - the framework should assume that
                                the dial request has been handled
                                and stop processing it further

****************************************************************************

   FUNCTION: PHExtFormatPhoneNumber

   SIGNATURE: BOOL PHExtFormatPhoneNumber(
                   LPCTSTR lpszCurrentCountryCode,
                   LPCTSTR lpszInput,
                   LPTSTR  lpszOut,
                   DWORD   cchOut);

   PURPOSE: Depending on where a device is primarily used the OEM may choose
            to customize the phone number formatting for a particular region. 
            This function will be called each time the user adds a new number
            to the dial string. The OEM can choose to handle the dial string
            or choose to allow Windows Mobile to proceed with default formatting.
            NOTE: As this function is called each time the user adds or removes
            a number from the dial string the OEM needs to take care to make sure
            this function is performant. 

   PARAMETERS:
            lpszCurrentCountryCode  - The Country Code for the current locale
            lpszInput               - The raw dial string
            lpszOut                 - A buffer to place the formatted dial string
            cchOut                  - The size of the buffer (in chars) pointed to by lpszOut

   RETURNS: If the function returns TRUE the OEM is assumed to have handled
            the phone number formatting and no additional formatting will be
            performed. If the function returns FALSE Windows Mobile will proceed
            with normal phone number formatting.

****************************************************************************

   FUNCTION: PhExtOnDrawAccumulatorText

   SIGNATURE: BOOL PhExtOnDrawAccumulatorText(
                   HDC hdc,
                   const RECT *prc,
                   LPCTSTR pszInput,
                   DWORD dwFlags);

   PURPOSE: This function will be called each time the accumulator text is going
            to be drawn. The OEM can choose to handle the drawing of the text or
            choose to allow Windows Mobile to proceed with default drawing.

   PARAMETERS:
            hdc         - Handle to the Device Context
            prc         - Pointer to the bounding rectangle to draw accumulator text to
            lpszInput   - The accumulator string to draw
            dwFlags     - PHAEF_DEFAULT:    when the string to draw is a dialing string
                          PHAEF_DTMF:       when the string to draw is a DTMF string      

   RETURNS: If the function returns S_OK the OEM is assumed to have handled
            the drawing of the accumulator text and no additional drawing will
            be done.

***************************************************************************/                                

#define PHExtFormatPhoneNumber_FunctionName     TEXT("PhExtFormatPhoneNumber")
#define PHExtOnDrawAccumulatorText_FunctionName TEXT("PhExtOnDrawAccumulatorText")

typedef BOOL (*PhExtFormatPhoneNumber_Function) 
             (LPCTSTR lpszCurrentCountryCode,
              LPCTSTR lpszInput,
              LPTSTR  lpszOut,
              DWORD   cchOut);

typedef HRESULT (*PhExtOnDrawAccumulatorText_Function)
                (HDC hdc,
                 const LPRECT prc,
                 LPCTSTR pszInput,
                 DWORD dwFlags);

#endif // _DPEXTENS_H_
