//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
#ifndef _DBGUTIL_H_
#define _DBGUTIL_H_
//***   dbgutil.h -- general debug helper utilities
//

#define EXPECT      ASSERT      // non-fatal ASSERT (unexpected, but o.k.)

#if 0 
//  extracted from /nt/private/shell/inc/debug.h
//  EVAL
//  RIP
//  THR(FAILED), TBOOL(0), TINT(-1), TPTR(0)
#ifdef DEBUG
#define EVAL(e)     ((e) || (XxxFailed(), 0))
...
#else
#define EVAL(e)     (e)
...
#endif
#endif

#ifdef DEBUG
#define DBEXEC(f, e)    ((f) ? ((e), 1) : 0)
#else
#define DBEXEC(f, e)    /*NOTHING*/
#endif

#define FASSERT_TESTED  1       // 0:brkpt on untested code, 1:don't
#define FASSERT_COMPAT  1       // 0:brkpt on incompatible code, 1:don't
#define FASSERT_NNYI    1       // 0:brkpt on NYI code, 1:don't

//***   ZONE_* -- standard zones {
#ifdef DEBUGZONE
// standard zones
#define ZONE_TRUE       1               // always on
#define ZONE_TRACE      DEBUGZONE(0)
#define ZONE_WARNING    DEBUGZONE(1)
#define ZONE_ERROR      DEBUGZONE(2)
#define ZONE_VERBOSE    DEBUGZONE(3)    // usage: ZONE_XXX && ZONE_VERBOSE
// custom zones start here...
// define your zones privately as DEBUGZONE(_ZONE_CUSTOM0 + xxx)
#define _ZONE_CUSTOM0   4               // 1st index for custom zones

#define ZONE_STRINGS(z0,z1,z2,z3,z4,z5,z6,z7,z8,z9,z10,z11,z12,z13,z14,z15) \
    L"Trace", L"Warning", L"Error", L"Verbose", \
    z0, z1, z2, z3, \
    z4, z5, z6, z7, \
    z8, z9, z10, z11, \
    /* z12, z13, z14, z15 */

#define ZONE_MASK(dwMask)   ((dwMask) << _ZONE_CUSTOM0)

#endif
// }

//***   SIG_* -- signatures {
// DESCRIPTION
//  add a SIG_DECL to front of your class, set it in ctor/init/dtor,
// and ASSERT_SIG on it in you top-level mfuncs.
// NOTES
//  n.b. class will change size btwn debug/retail!
#define SIG_CTORED(i)       (i - 1)
#define SIG_INITED(i)       (i)
#define SIG_DTORED(i)       (i + 1)

#define SIG_NOSIG           0

#if defined(DEBUG) && !SIG_NOSIG
#define SIG_CTOR(dwSig)     if (1) { _dwSig = SIG_CTORED(dwSig); } else 0
#define SIG_DTOR(dwSig)     if (1) { _dwSig = SIG_DTORED(dwSig); } else 0
#define SIG_INIT(dwSig)     if (1) { _dwSig = SIG_INITED(dwSig); } else 0
#define SIG_ASSERT(dwSig)   ASSERT(_dwSig == SIG_INITED(dwSig))
#else
#define SIG_CTOR(dwSig)
#define SIG_DTOR(dwSig)
#define SIG_INIT(dwSig)
#define SIG_ASSERT(dwSig)   /*NOTHING*/
#endif

#if !SIG_NOSIG
#define SIG_DECL            DWORD _dwSig
#define SIG_GET()           _dwSig
#else
#define SIG_DECL            /*NOTHING*/
#define SIG_GET()           *** error ***
#endif

// some standard signatures...
#define SIG_CCONTEXTMENU    ('cmnu')    // mnemonic 4-byte signature
// }

#endif // _DBGUTIL_H_
