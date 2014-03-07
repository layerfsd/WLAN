//------------------------------------------------------------------------------
// <copyright file="timer.c" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// Timer functions
//
// Author(s): ="Atheros"
//==============================================================================
#include "athdefs.h"
#include "athtypes.h"
#include "osapi.h"
#include <windows.h>
#include <mmsystem.h>
#include <ndis.h>

/*
 * This function will call the Timer Function registered
 * by the other modules through A_INIT_TIMER
 */

static void
ar6000_wrapper_timeout (unsigned int timerID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	A_TIMER  *pTimer = (A_TIMER *)dwUser;

	if (pTimer == NULL) {
		return;
	}
	pTimer->callback(pTimer->arg);
	return;
}

A_STATUS
ar6000_delete_timer (A_TIMER *timer)
{
	return A_OK;
}

void
ar6000_set_timer (A_TIMER *timer, unsigned long timeout, A_INT32 periodic)
{
	if (periodic) {
		NDIS_DEBUG_PRINTF(ATH_LOG_ERR, "Periodic Timer not implemented\n");
		return;
	}
    timer->timerId = timeSetEvent(
                         timeout,
                         0,
                         ar6000_wrapper_timeout,
                         (DWORD)timer,
                         TIME_ONESHOT | TIME_CALLBACK_FUNCTION);
	return;
}

A_STATUS
ar6000_cancel_timer (A_TIMER *timer)
{
    MMRESULT ret;

    ret = timeKillEvent(timer->timerId);

    if (ret != TIMERR_NOERROR) {
        return A_ERROR;
    }

	return A_OK;
}

A_STATUS
ar6000_init_timer (A_TIMER *timer, A_TIMER_FN proc_timer, 
				   PVOID arg)
{
	timer->callback = proc_timer;
	timer->arg = arg;

	return A_OK;
}

A_UINT32
ar6000_ms_tickget()
{
    A_UINT32 upTime;

    NdisGetSystemUpTime(&upTime);

    return upTime;
}
