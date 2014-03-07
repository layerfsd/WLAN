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

Module Name:  

audioddi.h

--*/
#ifdef __cplusplus
extern "C" {
#endif



//
// PFN_AUDIO_CALLBACK - Pointer to callback function, to let windows interface
//                      know sound is completed playing.
//
typedef VOID (*PFN_AUDIO_CALLBACK)(PBYTE);


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
BOOL 
AudioDllEntry(
   HANDLE hinstDll, 
   DWORD fdwReason, 
   LPVOID lpvReserved
   );



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
MMRESULT APIENTRY 
AudioPlaySound(
   PBYTE pMemFile,
   UINT  wFlags,
   PFN_AUDIO_CALLBACK pfnCallback
   );


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
MMRESULT APIENTRY 
AudioWaveOutGetVolume (
   UINT IDDevice, 
   LPDWORD lpdwVolume
   );



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
MMRESULT APIENTRY 
AudioWaveOutSetVolume (
   UINT IDDevice, 
   DWORD dwVolume
   );



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
VOID 
AudioPowerHandler (
   BOOL
   );


//
// These are only "public" when building the audio driver as a .lib to 
// be staticly linked with the event manager. Otherwise we are building
// the driver as a DLL and these functions are called by the DLL entry function.
//
#ifdef BUILD_AS_LIB
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
VOID 
AudioDriverInitialize (
   BOOL
   );


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
VOID 
AudioDriverDeinitialize (
   BOOL
   );
#endif   // building as .lib



#ifdef __cplusplus
}
#endif


