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

// This is a named event an application can wait on to 
// be notified when data protected by SDP is avaialble for use. 
// Until it is set, all access to sensitive data will fail.
//
// If an OpenEvent on this named event  fails, SDP is not supported in your build of the OS.
// which implies SDP data is available.
#pragma once 
#define SDP_DATA_AVAILABLE_EVENT_NAME L"SDP_DATA_AVAILABLE"
