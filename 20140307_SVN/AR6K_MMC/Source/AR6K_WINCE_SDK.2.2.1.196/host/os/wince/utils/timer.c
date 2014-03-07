//------------------------------------------------------------------------------
// <copyright file="timer.c" company="Atheros">
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
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
#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include "a_debug.h"
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

    if ((pTimer) && (pTimer->callback)) {
        pTimer->callback(pTimer->arg);
    }
    return;
}

A_STATUS
ar6000_delete_timer (A_TIMER *timer)
{
    timer->callback = NULL;
    timer->arg = NULL;

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
                         (TIME_ONESHOT |
                         TIME_CALLBACK_FUNCTION));
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
                   A_ATH_TIMER arg)
{
	if (timer)
	{
		timer->callback = proc_timer;
	}

    if (timer)
    {
		timer->arg = arg;
	}

    return A_OK;
}

A_UINT32
ar6000_ms_tickget()
{
    A_UINT32 upTime;

    NdisGetSystemUpTime(&upTime);

    return upTime;
}
