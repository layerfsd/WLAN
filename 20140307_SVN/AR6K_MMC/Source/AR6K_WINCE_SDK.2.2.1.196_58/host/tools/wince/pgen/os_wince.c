//------------------------------------------------------------------------------
// <copyright file="os_wince.c" company="Atheros">
//    Copyright (c) 2007 Atheros Corporation.  All rights reserved.
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
#include "os_wince.h"

struct timerhandle g_timer;

void usleep(int usec)
{
	Sleep(usec/1000);
}

void sleep(int sec)
{
	Sleep(sec*1000);
}

void bzero(char *ptr, int size)
{
	memset(ptr, 0, size);
}

void perror(char *msg)
{
	printf("ERROR: %s : %d\n", msg, GetLastError());
}

void gettimeofday(struct timeval *t, int dummmy)
{
	DWORD time = GetTickCount();
	t->tv_sec = time/1000;
	t->tv_usec = (time%1000)*1000;
}

void CALLBACK TimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	g_timer.id = 0;
	if(g_timer.enable)
	{
		g_timer.fn();
	}
}

int	setitimer(int dummy, struct itimerval *t, struct itimerval *t1)
{
	DWORD	timeMilliSec;

	if(g_timer.id)
	{
		UINT ret = timeKillEvent(g_timer.id);
		//if(ret != TIMERR_NOERROR)
		//{
		//	perror("setitimer failed 1");
		//}
	}

	if( (t->it_value.tv_sec==0) && (t->it_value.tv_usec==0) )
		return 0;

	timeMilliSec = (t->it_value.tv_sec*1000)+(t->it_value.tv_usec/1000);

	g_timer.id = timeSetEvent(timeMilliSec, 1, TimerProc, 0, TIME_ONESHOT|TIME_CALLBACK_FUNCTION);
	if(g_timer.id == 0)
	{
		perror("setitimer failed 2");
	}
	return 0;
}


int signal(int dummy, sighandler fn)
{
	if(fn == SIG_IGN)
	{
		g_timer.enable = 0;
	}
	else
	{
		g_timer.fn = fn;
		g_timer.enable = 1;
	}
	return 0;
}

int system(TCHAR *arg)
{
	if(!CreateProcess(L"wmiconfig.exe", arg, NULL, NULL,
				FALSE, 0, NULL, NULL, NULL, NULL ))
	{
		perror("Failed to run wmiconfig");
		return -1;
	}

	return 0;
}
