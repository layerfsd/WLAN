//------------------------------------------------------------------------------
// <copyright file="ctendpack.h" company="Atheros">
//    Copyright (c) 2007-2008 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#ifdef VXWORKS
#endif /* VXWORKS */

#if defined(LINUX) || defined(__linux__)
#endif /* LINUX */

#ifdef QNX
#endif /* QNX */

#ifdef INTEGRITY
#include "integrity/ctendpack_integrity.h"
#endif /* INTEGRITY */

#ifdef NUCLEUS
#endif /* NUCLEUS */

#ifdef UNDER_CE
#include "wince/ctendpack_wince.h"
#endif /* WINCE */

