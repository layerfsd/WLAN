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

Component Name:

gweadd.h

Abstract:  

This header file declares the function Initialize_AdditionalComponents().
If an additional components (beyond those that come with Windows CE) is added
to GWE via the cesysgen.bat file, and if that component needs to do some
initialization at boot time, then that components can implement this function.
GWE will call this function at boot time.
++*/

#ifdef __cplusplus
extern "C"
#endif
void Initialize_AdditionalComponents (void);
