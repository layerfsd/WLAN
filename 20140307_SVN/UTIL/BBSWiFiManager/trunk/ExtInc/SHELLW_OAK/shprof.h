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

#ifndef __SHPROF_H__
#define __SHPROF_H__


#ifndef NOINSTRUMENT


// Profiling class
#ifdef __cplusplus
class CShellProfile
{
private:
    LARGE_INTEGER _liStart;
    int _iTag;

public:
    CShellProfile(int iTag);
    ~CShellProfile();

    void WaitForWindow(HWND hwnd);
};
#endif


#ifdef __cplusplus
extern "C" {
#endif


// Application constants
#define PROF_NOTSET             0xFF
#define PROF_RESOLUTION         0
#define PROF_CALENDAR           1
#define PROF_CONTACTS           2
#define PROF_NOTES              3
#define PROF_PIE                4
#define PROF_TASKS              5
#define PROF_SETTINGS           6
#define PROF_PROGRAMS           7
#define PROF_INBOX              8
#define PROF_CALSWITCH          9
#define PROF_CALNEW             10
#define PROF_CALNOTIFICATION    11
#define PROF_CALNEWSESSION      12
#define PROF_CALAGENDASTART     13
#define PROF_CONTACTSWITCH      14
#define PROF_CONTACTNEW         15
#define PROF_TASKSSWITCH        16
#define PROF_TASKSNEW           17
#define PROF_NOTESSWITCH        18
#define PROF_INBOXSWITCH        19
#define PROF_INBOXEXECUTE       20
#define PROF_PIESWITCH          21
#define PROF_SETTINGSSWITCH     22
#define PROF_PROGRAMSSWITCH     23
#define PROF_TODAYSWITCH        24
#define PROF_CPLBUTTONS         25
#define PROF_CPLINPUT           26
#define PROF_CPLMENUS           27
#define PROF_CPLOWNER           28
#define PROF_CPLPASSWORD        29
#define PROF_CPLSOUND           30
#define PROF_CPLTODAY           31
#define PROF_CPLABOUT           32
#define PROF_CPLALIGN           33
#define PROF_CPLBACKLIGT        34
#define PROF_CPLCLOCK           35
#define PROF_CPLCONTRAST        36
#define PROF_CPLMEMORY          37
#define PROF_CPLPOWER           38
#define PROF_CPLREGION          39
#define PROF_CPLREMOVE          40
#define PROF_CPLMODEM           41
#define PROF_CPLPC              42
#define PROF_NOTESBOOTCASE      43
#define PROF_PXL                44
#define PROF_PWORD              45
#define PROF_PWORDBOOTCASE      46
#define PROF_PWORDSWITCH        47
#define PROF_FINDSEARCH         48

#define PROF_CARDADDRNEW        49
#define PROF_CARDADDROPEN       50
#define PROF_CARDCALNEW         51
#define PROF_CARDCALOPEN        52
#define PROF_CARDTASKNEW        53
#define PROF_CARDTASKOPEN       54
#define PROF_CARDINBOXNEW       55
#define PROF_CARDINBOXOPEN      56

#define PROF_CALWEEKVIEW        57
#define PROF_CALDAYVIEW         58
#define PROF_CALMONTHVIEW       59
#define PROF_CALAGENDAVIEW      60
#define PROF_CALYEARVIEW        61

#define PROF_SWITCHINKMODE      62
#define PROF_RECORDDELAY        63
#define PROF_DROPSTARTMENU      64

#define PROF_USER1              95
#define PROF_USER2              96
#define PROF_USER3              97
#define PROF_USER4              98
#define PROF_USER5              99


// Start Modes.  These are mutually exclusive.  Increment by one when adding.
#define PROF_OLDSTART       0x00000000
#define PROF_TODAY          0x00001000
#define PROF_HARDWARE       0x00002000
#define PROF_EXPLORER       0x00003000
#define PROF_AUTOMATIC      0x00004000

// New Messages.
// used for profiling
#define PWM_STOP                    (WM_USER + 10)
#define PWM_SETAPPINDEX             (WM_USER + 11)


//
// Shell profiling APIs
//
void SHStartProfile(DWORD dwStartedBits);
void SHSignalDone(HWND hwndUpdate, UINT uid);
void SHStartIfNeeded();
void SHClearStartedBit();
void SHStartAndBlock(DWORD dwBits);

void *SHNewProfileObj(int iTag);
void SHEndProfileObj(void *p);


#ifdef __cplusplus
}
#endif

#endif // NOINSTRUMENT


#endif // __SHPROF_H__
