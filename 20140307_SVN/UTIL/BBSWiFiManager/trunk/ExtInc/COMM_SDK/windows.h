//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++


Module Name: windows.h

Purpose: Master include file for Windows applications.

--*/

#ifndef __WINDOWS__
#define __WINDOWS__

#if defined (_MSC_VER)
#pragma warning(disable:4116)       // TYPE_ALIGNMENT generates this - move it
                                    // outside the warning push/pop scope.
#endif

#ifndef RC_INVOKED
#pragma warning(disable:4514)
#ifndef __WINDOWS_DONT_DISABLE_PRAGMA_PACK_WARNING__
#pragma warning(disable:4103)
#endif
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4001)
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#endif /* RC_INVOKED */

#include <windef.h>
#include <types.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>
#include <winreg.h>
#include <shellapi.h>
#if !defined(WINCEMACRO) && !defined(WIN32_NO_OLE)
#include <ole2.h>
#endif

#include <imm.h>

#include <tchar.h>
#include <excpt.h>

#ifndef RC_INVOKED
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4001)
#pragma warning(default:4201)
#pragma warning(default:4214)
/* Leave 4514 disabled.  It's an unneeded warning anyway. */
#endif
#endif /* RC_INVOKED */

// On CE force the functions to be linked instead of inline and
// turn off auto deprecate
#ifdef UNDER_CE
#define STRSAFE_LIB
#define STRSAFE_NO_DEPRECATE
#endif
#include <strsafe.h>

#endif /* __WINDOWS__ */

