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
Copyright (c) 1995-1999  Microsoft Corporation

--*/

#ifndef _DIALACT_H_
#define _DIALACT_H_

#define MAXPARAMS               4

// Macros to determine easily which type of code this is
#define ISDIALCODE(x)       ((x) & 0x00010000)
#define ISDTMFCODE(x)       ((x) & 0x00020000)
#define ISINCALLCODE(x)     ((x) & 0x00040000)
#define ISSUPSVCCODE(x)     ((x) & 0x00080000)
#define ISUSSDCODE(x)       ((x) & 0x00100000)

#define MAKEDIALCODE(x)     ((x) | 0x00010000)
#define MAKEDTMFCODE(x)     ((x) | 0x00020000)
#define MAKEINCALLCODE(x)   ((x) | 0x00040000)
#define MAKESUPSVCCODE(x)   ((x) | 0x00080000)
#define MAKEUSSDCODE(x)     ((x) | 0x00100000)

// Possible codes, note that we have replaced leading 0s with F (since codes only use decimal digits).
// Also, hex digits A-E are used for codes that cannot be directly entered by the user but rather are
// inferred, for example DTMF tones (which are separated by a p)

// Dialing codes
#define REGULARCALL                     0xA
#define DIALCODE_REGULARCALL            MAKEDIALCODE(REGULARCALL)
#define ASISCALL                        0xC
#define DIALCODE_ASISCALL               MAKEDIALCODE(ASISCALL)

// DTMF codes
#define DTMFSENDONCONNECT               0xD
#define DTMFCODE_SENDONCONNECT          MAKEDTMFCODE(DTMFSENDONCONNECT)
#define DTMFAFTERPAUSE                  0xE
#define DTMFCODE_AFTERPAUSE             MAKEDTMFCODE(DTMFAFTERPAUSE)
#define DTMFAFTERTALK                   0X1A
#define DTMFCODE_AFTERTALK              MAKEDTMFCODE(DTMFAFTERTALK)

// SIM Phone book entry
#define SIMPHONEBOOK                    0x1B
#define SIMCODE_SIMPHONEBOOK            MAKESUPSVCCODE(SIMPHONEBOOK)

// IMEI
#define IMEI                            0xF6
#define SIMCODE_DISPLAYIMEI             MAKESUPSVCCODE(IMEI)

// PINs
#define CHANGEPIN                       0xF4
#define SIMCODE_CHANGEPIN               MAKESUPSVCCODE(CHANGEPIN)
#define CHANGEPIN2                      0xF42
#define SIMCODE_CHANGEPIN2              MAKESUPSVCCODE(CHANGEPIN2)
#define UNBLOCKPIN                      0xF5
#define SIMCODE_UNBLOCKPIN              MAKESUPSVCCODE(UNBLOCKPIN)
#define UNBLOCKPIN2                     0xF52
#define SIMCODE_UNBLOCKPIN2             MAKESUPSVCCODE(UNBLOCKPIN2)

// Change SS Password
#define SSCHANGEPASSWORD                0xF3
#define SUPSERVCODE_CHANGEPASSWORD      MAKESUPSVCCODE(SSCHANGEPASSWORD)

// eMLPP (02.67)
#define EMLPPBASE                       0x75
#define EMLPPCODE_BASE                  MAKESUPSVCCODE(EMLPPBASE)
#define EMLPPLEVEL0                     0x750
#define EMLPPCODE_LEVEL0                MAKESUPSVCCODE(EMLPPLEVEL0)
#define EMLPPLEVEL1                     0x751
#define EMLPPCODE_LEVEL1                MAKESUPSVCCODE(EMLPPLEVEL1)
#define EMLPPLEVEL2                     0x752
#define EMLPPCODE_LEVEL2                MAKESUPSVCCODE(EMLPPLEVEL2)
#define EMLPPLEVEL3                     0x753
#define EMLPPCODE_LEVEL3                MAKESUPSVCCODE(EMLPPLEVEL3)
#define EMLPPLEVEL4                     0x754
#define EMLPPCODE_LEVEL4                MAKESUPSVCCODE(EMLPPLEVEL4)

// Call Deflection (02.72)
#define CALLDEFLECT                     0x66
#define CALLCODE_CALLDEFLECT            MAKESUPSVCCODE(CALLDEFLECT)

// Call ID codes (02.81)
#define CALLIDCLIP                      0x30
#define CALLIDCODE_CALLINGPRESENT       MAKESUPSVCCODE(CALLIDCLIP)
#define CALLIDCLIR                      0x31
#define CALLIDCODE_CALLINGRESTRICT      MAKESUPSVCCODE(CALLIDCLIR)
#define CALLIDCOLP                      0x76
#define CALLIDCODE_CONNECTEDPRESENT     MAKESUPSVCCODE(CALLIDCOLP)
#define CALLIDCOLR                      0x77
#define CALLIDCODE_CONNECTEDRESTRICT    MAKESUPSVCCODE(CALLIDCOLR)

// Call forwarding codes (02.82)
#define FWDUNCONDITIONAL                0x21
#define FWDCODE_UNCONDITIONAL           MAKESUPSVCCODE(FWDUNCONDITIONAL)
#define FWDBUSY                         0x67
#define FWDCODE_BUSY                    MAKESUPSVCCODE(FWDBUSY)
#define FWDNOREPLY                      0x61
#define FWDCODE_NOREPLY                 MAKESUPSVCCODE(FWDNOREPLY)
#define FWDNOTREACHABLE                 0x62
#define FWDCODE_NOTREACHABLE            MAKESUPSVCCODE(FWDNOTREACHABLE)
#define FWDALL                          0xFF2
#define FWDCODE_ALL                     MAKESUPSVCCODE(FWDALL)
#define FWDALLCONDITIONAL               0xFF4
#define FWDCODE_ALLCONDITIONAL          MAKESUPSVCCODE(FWDALLCONDITIONAL)

// Call waiting (02.83)
#define CALLWAITING                     0x43
#define CALLCODE_CALLWAITING            MAKESUPSVCCODE(CALLWAITING)

// User-User Service (02.87)
#define UUSALL                          0x360
#define UUSCODE_ALL                     MAKESUPSVCCODE(UUSALL)
#define UUSSERVICE1                     0x361
#define UUSCODE_SERVICE1                MAKESUPSVCCODE(UUSSERVICE1)
#define UUSSERVICE2                     0x362
#define UUSCODE_SERVICE2                MAKESUPSVCCODE(UUSSERVICE2)
#define UUSSERVICE3                     0x363
#define UUSCODE_SERVICE3                MAKESUPSVCCODE(UUSSERVICE3)

// Call barring codes (02.88)
#define BARROUT                         0x33
#define BARRCODE_OUT                    MAKESUPSVCCODE(BARROUT)
#define BARROUTINTL                     0x331
#define BARRCODE_OUTINTL                MAKESUPSVCCODE(BARROUTINTL)
#define BARROUTINTLEXTOHOME             0x332
#define BARRCODE_OUTINTLEXTOHOME        MAKESUPSVCCODE(BARROUTINTLEXTOHOME)
#define BARRIN                          0x35
#define BARRCODE_IN                     MAKESUPSVCCODE(BARRIN)
#define BARRINROAM                      0x351
#define BARRCODE_INROAM                 MAKESUPSVCCODE(BARRINROAM)
#define BARRALL                         0x330
#define BARRCODE_ALL                    MAKESUPSVCCODE(BARRALL)
#define BARRALLOUT                      0x333
#define BARRCODE_ALLOUT                 MAKESUPSVCCODE(BARRALLOUT)
#define BARRALLIN                       0x353
#define BARRCODE_ALLIN                  MAKESUPSVCCODE(BARRALLIN)

// ECT (02.91)
#define CALLTRANSFER                    0x96
#define CALLCODE_TRANSFER               MAKESUPSVCCODE(CALLTRANSFER)

// CCBS (02.93)
#define CALLCOMPLETEBUSY                0x37
#define CALLCODE_COMPLETEBUSY           MAKESUPSVCCODE(CALLCOMPLETEBUSY)

// PNP codes (02.95)
#define PNP0                            0xf70
#define PNPCODE_PNP0                    MAKESUPSVCCODE(PNP0)
#define PNP1                            0xf71
#define PNPCODE_PNP1                    MAKESUPSVCCODE(PNP1)
#define PNP2                            0xf72
#define PNPCODE_PNP2                    MAKESUPSVCCODE(PNP2)
#define PNP3                            0xf73
#define PNPCODE_PNP3                    MAKESUPSVCCODE(PNP3)
#define PNP4                            0xf74
#define PNPCODE_PNP4                    MAKESUPSVCCODE(PNP4)
#define PNP5                            0xf75
#define PNPCODE_PNP5                    MAKESUPSVCCODE(PNP5)
#define PNP6                            0xf76
#define PNPCODE_PNP6                    MAKESUPSVCCODE(PNP6)
#define PNP7                            0xf77
#define PNPCODE_PNP7                    MAKESUPSVCCODE(PNP7)
#define PNP8                            0xf78
#define PNPCODE_PNP8                    MAKESUPSVCCODE(PNP8)
#define PNP9                            0xf79
#define PNPCODE_PNP9                    MAKESUPSVCCODE(PNP9)

// CNAP (02.96)
#define CALLCNAP                        0x300
#define CALLCODE_CALLINGNAMEPRESENT     MAKESUPSVCCODE(CALLCNAP)

// MSP (02.97)
#define MSP1                            0x591
#define MSPCODE_MSP1                    MAKESUPSVCCODE(MSP1)
#define MSP2                            0x592
#define MSPCODE_MSP2                    MAKESUPSVCCODE(MSP2)
#define MSP3                            0x593
#define MSPCODE_MSP3                    MAKESUPSVCCODE(MSP3)
#define MSP4                            0x594
#define MSPCODE_MSP4                    MAKESUPSVCCODE(MSP4)

// Pound string -- something that we don't understand that ends in pound
#define POUNDSTRING                     0x1C
#define POUNDCODE_POUNDSTRING           MAKEUSSDCODE(POUNDSTRING)

// An invalid code that doesn't even end in pound -- try your best
#define INVALIDSTRING                   0x1D
#define INVALIDCODE_INVALIDSTRING       MAKEUSSDCODE(INVALIDSTRING)

// Read in network settings (from Tapi CSP)
#define CURRENTNETWORK                  0x1E
#define NETCODE_CURRENTNETWORK          MAKESUPSVCCODE(CURRENTNETWORK)
#define PREFERREDNETWORK                0x2A
#define NETCODE_PREFERREDNETWORK        MAKESUPSVCCODE(PREFERREDNETWORK)
#define NETWORKLIST                     0x2B
#define NETCODE_NETWORKLIST             MAKESUPSVCCODE(NETWORKLIST)
#define ALLNETWORKSLIST                 0x3E
#define NETCODE_ALLNETWORKSLIST         MAKESUPSVCCODE(ALLNETWORKSLIST)

//read in equipment info
#define MANUFACTURER                   0x2C
#define EQUIPCODE_MANUFACTURER         MAKESUPSVCCODE(MANUFACTURER)
#define MODEL                          0x2D
#define EQUIPCODE_MODEL                MAKESUPSVCCODE(MODEL)
#define REVISION                       0x2E
#define EQUIPCODE_REVISION             MAKESUPSVCCODE(REVISION)

// Show event log
#define SHOWLOG                        0x3A
#define SHOWCODE_SHOWLOG               MAKESUPSVCCODE(SHOWLOG)

// Run diagnose application
#define DIAGINFO                       0x3B
#define SHOWCODE_DIAGINFO              MAKESUPSVCCODE(DIAGINFO)

// Run diagnose application
#define OEMDIAG                        0x3C
#define SHOWCODE_OEMDIAG               MAKESUPSVCCODE(OEMDIAG)

#define ISDIAGNOSTICCODE(x) ((SHOWCODE_DIAGINFO == x) || (SHOWCODE_SHOWLOG == x) || (SHOWCODE_OEMDIAG == x)) 

// switch current outgoing line
#define CURRENTLINE                    0x3D
#define LINECODE_CURRENTLINE           MAKESUPSVCCODE(CURRENTLINE)

// get RIL version
#define RILVERSION                     0x3F
#define RIL_VERSION                    MAKESUPSVCCODE(RILVERSION)

// Possible actions
typedef enum tagenumAction
{
    actNone, actActivation, actDeactivation, actInterrogation, actRegistration, actErasure
} enumAction;

typedef struct tagDIALACTION
{
    DWORD dwFlags;
    DWORD dwCode;
    DWORD dwAddressId;
    enumAction eaAction;
    
    // Parameter listing
    DWORD dwNumParams;
    TCHAR *rgpwszParams[MAXPARAMS];

    // The original string
    TCHAR *pwszOriginal;
} DIALACTION, *LPDIALACTION;

// Flags for dwFlags
#define DIALACTION_FLAGS_COMMANDCOMPLETED       0x01
#define DIALACTION_FLAGS_HIDEINPUT              0x02
#define DIALACTION_FLAGS_PARAMPASTPOUND         0x04

#endif _DIALACT_H_
