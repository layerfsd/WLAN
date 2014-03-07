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
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of your
// Microsoft Mobile Device 2003 Software Pre-Release Development Agreement
// or other applicable Microsoft license agreement for the software.
// If you did not accept the terms of such a license, you are not
// authorized to use this source code.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

-----------------------------------------------------------------------------

@doc EXTERNAL

@module SIMTKIT.H - SIM Toolkit API |

-----------------------------------------------------------------------------


Notes:


--*/

#pragma once


#include <windows.h>
#include "simmgr.h"

// Errors
#define FACILITY_SIMTOOLKIT                         0x400

#define SIM_E_TOOLKITNOTSUPPORTED                   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_SIMTOOLKIT, 0x90)

//
// Notification codes for toolkit functions -- these match 13.4 in GSM 11.14
//

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Toolkit Notify | Different kinds of proactive SIM commands
//
// @comm None
//
// -----------------------------------------------------------------------------

#define SIM_NOTIFY_REFRESH              (0x01)       // @constdefine TBD
#define SIM_NOTIFY_MORETIME             (0x02)       // @constdefine TBD
#define SIM_NOTIFY_POLLINTERVAL         (0x03)       // @constdefine TBD
#define SIM_NOTIFY_POLLINGOFF           (0x04)       // @constdefine TBD
#define SIM_NOTIFY_EVENTLIST            (0x05)       // @constdefine TBD
#define SIM_NOTIFY_SETUPCALL            (0x10)       // @constdefine TBD
#define SIM_NOTIFY_SENDSS               (0x11)       // @constdefine TBD
#define SIM_NOTIFY_SENDUSSD             (0x12)       // @constdefine TBD
#define SIM_NOTIFY_SENDSMS              (0x13)       // @constdefine TBD
#define SIM_NOTIFY_SENDDTMF             (0x14)       // @constdefine TBD
#define SIM_NOTIFY_LAUNCHBROWSER        (0x15)       // @constdefine TBD
#define SIM_NOTIFY_PLAYTONE             (0x20)       // @constdefine TBD
#define SIM_NOTIFY_DISPLAYTEXT          (0x21)       // @constdefine TBD
#define SIM_NOTIFY_GETINKEY             (0x22)       // @constdefine TBD
#define SIM_NOTIFY_GETINPUT             (0x23)       // @constdefine TBD
#define SIM_NOTIFY_SELECTITEM           (0x24)       // @constdefine TBD
#define SIM_NOTIFY_SETUPMENU            (0x25)       // @constdefine TBD
#define SIM_NOTIFY_LOCALINFO            (0x26)       // @constdefine TBD
#define SIM_NOTIFY_SETUPIDLEMODETEXT    (0x28)       // @constdefine TBD
#define SIM_NOTIFY_RUNATCOMMAND         (0x34)       // @constdefine TBD
#define SIM_NOTIFY_LANGUAGENOTIFICATION (0x35)       // @constdefine TBD
#define SIM_NOTIFY_OPENCHANNEL          (0x40)       // @constdefine TBD
#define SIM_NOTIFY_CLOSECHANNEL         (0x41)       // @constdefine TBD
#define SIM_NOTIFY_RECEIVEDATA          (0x42)       // @constdefine TBD
#define SIM_NOTIFY_SENDDATA             (0x43)       // @constdefine TBD
#define SIM_NOTIFY_CHANNELSTATUS        (0x44)       // @constdefine TBD

#define SIM_NOTIFY_ENDSESSION           (0x200)      // @constdefine The Toolkit session is over
#define SIM_NOTIFY_CALLSETUP            (0x201)      // @constdefine Notification that call setup was successful
#define SIM_NOTIFY_SETUPMENU_REPLY      (0x202)      // @constdefine Menu item selection reply
#define SIM_NOTIFY_EVENTDOWNLOAD        (0x203)      // @constdefine Requested event occurred.


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Max Length | Maximum length constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define MAXLEN_AREAID           (4)          // @constdefine 4
#define MAXLEN_CELLID           (4)          // @constdefine 4
#define MAXLEN_OPERATOR_NUMERIC (5)          // @constdefine 5
#define MAXLEN_IMEI             (15)         // @constdefine 15
#define MAXLEN_NMR              (16)         // @constdefine 16
#define MAXLEN_BCCH             (16)         // @constdefine 16
#define MAXNUM_BCCH             (3)          // @constdefine 3
#define MAXLEN_LANGUAGE         (3)          // @constdefine 3

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMTEXT | SIMTEXT structure constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIMTEXT_HIGHPRIORITY        (0x001)  // @constdefine The text message is high priority
#define SIMTEXT_DELAYCLEAR          (0x002)  // @constdefine The message should disappear on its own
#define SIMTEXT_KEYPADDIGITS        (0x004)  // @constdefine User input should only consist of keypad digits
#define SIMTEXT_NOECHO              (0x008)  // @constdefine The SIM UI should not echo entered characters
#define SIMTEXT_HELPINFO            (0x010)  // @constdefine Context sensitive help is supported
#define SIMTEXT_GSMDEFAULTALPHABET  (0x020)  // @constdefine User input should only consist of characters in the GSM default alphabet
#define SIMTEXT_PACKEDRESPONSE      (0x040)  // @constdefine The response string needs to be packed
#define SIMTEXT_IMMEDIATERESPONSE   (0x080)  // @constdefine Immediate response required
#define SIMTEXT_YESNO               (0x100)  // @constdefine Yes/No response for Get Inkey requested

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMMENU | SIMMENU structure constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIMMENU_HELPINFO        (0x10) // @constdefine Context sensitive help is supported

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMSMS | SIMSMS structure constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIMSMS_PACKINGREQUIRED  (0x01) // @constdefine Text must be packed before sending

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMCALL | SIMCALL structure constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIMCALL_IDLE            (0x01) // @constdefine Place call only if phone is idle
#define SIMCALL_HOLDCALLS       (0x02) // @constdefine Place call holding all other calls
#define SIMCALL_DISCONNECTCALLS (0x04) // @constdefine Place call disconnecting all other calls
#define SIMCALL_REDIAL          (0x80) // @constdefine Attempt redial

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMLOCALINFO | SIMLOCALINFO structure constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIMLOCALINFO_LOCATION            (0x01) // @constdefine MCC, MNC, LAC, CellID
#define SIMLOCALINFO_IMEI                (0x02) // @constdefine International Mobile Equipment Identity
#define SIMLOCALINFO_NMR                 (0x04) // @constdefine Network Measurement Results

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMTONE | SIMTONE structure constants
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIMTONE_DIAL              (0x01) // @constdefine Play an OK tone
#define SIMTONE_BUSY              (0x02) // @constdefine Called party is busy
#define SIMTONE_CONGESTION        (0x03) // @constdefine Congestion
#define SIMTONE_SERVICE           (0x04) // @constdefine Radio service available
#define SIMTONE_NOSERVICE         (0x05) // @constdefine No radio service available
#define SIMTONE_SPECIAL           (0x06) // @constdefine Special information
#define SIMTONE_CALLWAIT          (0x07) // @constdefine Call waiting tone
#define SIMTONE_RING              (0x08) // @constdefine Ring tone
#define SIMTONE_DEFAULT           (0x10) // @constdefine Play the default tone
#define SIMTONE_SUCCESS           (0x11) // @constdefine Play an "ok" tone
#define SIMTONE_ERROR             (0x12) // @constdefine Play an "error" tone
#define SIMTONE_DEFAULTDURATION   (0xffffffff) // @constdefine Play for the default duration

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMSENDDATA | SIMSENDDATA structure constants
//
// -----------------------------------------------------------------------------
#define SIMSENDDATA_STOREDATA          (0x01) // @constdefine Store data in Tx buffer
#define SIMSENDDATA_SENDIMMEDIATE      (0x02) // @constdefine Send data immediately

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMSENDDATA | SIMSENDDATA structure constants
//
// -----------------------------------------------------------------------------
#define SIMOPENCHANNEL_ONDEMAND        (0x00) // @constdefine OnDemand link establishment
#define SIMOPENCHANNEL_IMMEDIATE       (0x01) // @constdefine Immediate link establishment
#define SIMOPENCHANNEL_AUTORECONNECT   (0x02) // @constdefine Automatic reconnection
#define SIMOPENCHANNEL_DEFAULTDURATION SIMTONE_DEFAULTDURATION // @constdefine Default for duration values

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants SIMRESPONSE | SIMRESPONSE structure constants
//
// @comm Note that these constants correspond to similar SIMTEXT_ constants
//
// -----------------------------------------------------------------------------
#define SIMRESPONSE_KEYPADDIGITS        (0x04) // @constdefine Text should only consist of keypad digits
#define SIMRESPONSE_GSMDEFAULTALPHABET  (0x20) // @constdefine Text should be encoded in GSM default alphabet (as opposed to UCS2)
#define SIMRESPONSE_PACKEDRESPONSE      (0x40) // @constdefine Text string needs to be packed

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTEXT
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMTEXT_FLAGS                     (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMTEXT_MINRESPONSE               (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMTEXT_MAXRESPONSE               (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMTEXT_TEXTSIZE                  (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMTEXT_TEXTOFFSET                (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMTEXT_DEFAULTTEXTSIZE           (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMTEXT_DEFAULTTEXTOFFSET         (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMTEXT_ICONID                    (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMTEXT_ICONQUALIFIER             (0x00000100)     // @paramdefine
#define SIM_PARAM_SIMTEXT_ALL                       (0x000001ff)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMMENUITEM
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMMENUITEM_IDENTIFIER            (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMMENUITEM_FLAGS                 (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMMENUITEM_NEXTACTION            (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMMENUITEM_TEXTSIZE              (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMMENUITEM_TEXTOFFSET            (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMMENUITEM_ICONID                (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMMENUITEM_ICONQUALIFIER         (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMMENUITEM_ALL                   (0x0000007f)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMMENU
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMMENU_FLAGS                     (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMMENU_DEFAULTITEM               (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMMENU_TEXTSIZE                  (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMMENU_TEXTOFFSET                (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMMENU_MENUITEMCOUNT             (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMMENU_MENUITEMOFFSET            (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMMENU_MENUITEMSIZE              (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMMENU_ICONID                    (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMMENU_ICONQUALIFIER             (0x00000100)     // @paramdefine
#define SIM_PARAM_SIMMENU_ALL                       (0x000001ff)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMSMS
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMSMS_FLAGS                      (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMSMS_ADDRESSTYPE                (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMSMS_NUMPLAN                    (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMSMS_TEXTSIZE                   (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMSMS_TEXTOFFSET                 (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMSMS_ADDRESSSIZE                (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMSMS_ADDRESSOFFSET              (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMSMS_TPDUSIZE                   (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMSMS_TPDUOFFSET                 (0x00000100)     // @paramdefine
#define SIM_PARAM_SIMSMS_ICONID                     (0x00000200)     // @paramdefine
#define SIM_PARAM_SIMSMS_ICONQUALIFIER              (0x00000400)     // @paramdefine
#define SIM_PARAM_SIMSMS_ALL                        (0x000007ff)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMUSSD
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMUSSD_ADDRESSTYPE               (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMUSSD_NUMPLAN                   (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMUSSD_USSDSIZE                  (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMUSSD_USSDOFFSET                (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMUSSD_TEXTSIZE                  (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMUSSD_TEXTOFFSET                (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMUSSD_ICONID                    (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMUSSD_ICONQUALIFIER             (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMUSSD_ALL                       (0x000000ff)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMCALL
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMCALL_FLAGS                     (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMCALL_REDIALDURATION            (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMCALL_TEXTSIZE                  (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMCALL_TEXTOFFSET                (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMCALL_ADDRESSSIZE               (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMCALL_ADDRESSOFFSET             (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMCALL_SUBADDRESSSIZE            (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMCALL_SUBADDRESSOFFSET          (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMCALL_ADDRESSTYPE               (0x00000100)     // @paramdefine
#define SIM_PARAM_SIMCALL_NUMPLAN                   (0x00000200)     // @paramdefine
#define SIM_PARAM_SIMCALL_ICONID                    (0x00000400)     // @paramdefine
#define SIM_PARAM_SIMCALL_ICONQUALIFIER             (0x00000800)     // @paramdefine
#define SIM_PARAM_SIMCALL_CALLSETUPTEXTSIZE         (0x00001000)     // @paramdefine
#define SIM_PARAM_SIMCALL_CALLSETUPTEXTOFFSET       (0x00002000)     // @paramdefine
#define SIM_PARAM_SIMCALL_CALLSETUPICONID           (0x00004000)     // @paramdefine
#define SIM_PARAM_SIMCALL_CALLSETUPICONQUALIFIER    (0x00008000)     // @paramdefine
#define SIM_PARAM_SIMCALL_ALL                       (0x0000ffff)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTONE
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMTONE_TONE                      (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMTONE_DURATION                  (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMTONE_TEXTSIZE                  (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMTONE_TEXTOFFSET                (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMTONE_ICONID                    (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMTONE_ICONQUALIFIER             (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMTONE_ALL                       (0x0000003f)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMEVENTLIST
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMEVENTLIST_EVENTSSIZE           (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMEVENTLIST_EVENTSOFFSET         (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMEVENTLIST_ALL                  (0x00000003)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMRESPONSE
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMRESPONSE_RESPONSE              (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_ADDITIONALINFO        (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_POLLINTERVAL          (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_TEXT                  (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_ITEM                  (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_LOCALINFO             (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_FLAGS                 (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_CHANNELID             (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_CHANNELDATASIZE       (0x00000100)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_CHANNELDATA           (0x00000200)     // @paramdefine
#define SIM_PARAM_SIMRESPONSE_CHANNELSTATUS         (SIM_PARAM_SIMRESPONSE_CHANNELDATASIZE\
                                                    | SIM_PARAM_SIMRESPONSE_CHANNELDATA)  // @paramdefine
#define SIM_PARAM_SIMRESPONSE_CHANNELDATALENGTH     (0x00000400)  // @paramdefine
#define SIM_PARAM_SIMRESPONSE_BUFFERSIZE            (0x00000800)  // @paramdefine
#define SIM_PARAM_SIMRESPONSE_BEARERTYPE            (0x00001000)  // @paramdefine
#define SIM_PARAM_SIMRESPONSE_BEARERDESC            (0x00002000)  // @paramdefine
#define SIM_PARAM_SIMRESPONSE_ALL                   (0x00003fff)  // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMLOCALINFO
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMLOCALINFO_NUMNAME              (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMLOCALINFO_LAC                  (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMLOCALINFO_CELLID               (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMLOCALINFO_IMEI                 (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMLOCALINFO_NMR                  (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMLOCALINFO_BCCH                 (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMLOCALINFO_NUMBCCH              (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMLOCALINFO_ALL                  (0x0000007f)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMLAUNCHBROWSER
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMLAUNCHBROWSER_FLAGS               (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_BROWSERID           (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_URLSIZE             (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_URLOFFSET           (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_BEARERSIZE          (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_BEAREROFFSET        (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_GATEWAYSIZE         (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_GATEWAYOFFSET       (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_TEXTSIZE            (0x00000100)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_TEXTOFFSET          (0x00000200)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_PROVISIONFILECOUNT  (0x00000400)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_PROVISIONFILESIZE   (0x00000800)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_PROVISIONFILEOFFSET (0x00001000)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_ICONID              (0x00002000)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_ICONQUALIFIER       (0x00004000)     // @paramdefine
#define SIM_PARAM_SIMLAUNCHBROWSER_ALL                 (0x00007FFF)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMFILEREFERENCE
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMFILEREFERENCE_FILESIZE         (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMFILEREFERENCE_FILEOFFSET       (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMFILEREFERENCE_ALL              (0x00000003)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMSENDDTMF
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMSENDDTMF_TEXTSIZE              (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMSENDDTMF_TEXTOFFSET            (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMSENDDTMF_DTMFSIZE              (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMSENDDTMF_DTMFOFFSET            (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMSENDDTMF_ICONID                (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMSENDDTMF_ICONQUALIFIER         (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMSENDDTMF_ALL                   (0x0000003F)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMOPENCHANNEL
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMOPENCHANNEL_FLAGS              (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_BEARERDESCTYPE     (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_BUFFERSIZE         (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ADDRESSTYPE        (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_NUMPLAN            (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_DURATION1          (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_DURATION2          (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_LOCALADDRTYPE      (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_DESTADDRTYPE       (0x00000100)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_PROTOCOLTYPE       (0x00000200)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_PORTNUM            (0x00000400)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_TEXTSIZE           (0x00000800)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_TEXTOFFSET         (0x00001000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_BEARERDESCSIZE     (0x00002000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_BEARERDESCOFFSET   (0x00004000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ADDRESSSIZE        (0x00008000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ADDRESSOFFSET      (0x00010000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_SUBADDRESSSIZE     (0x00020000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_SUBADDRESSOFFSET   (0x00040000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_LOCALADDRSIZE      (0x00080000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_LOCALADDROFFSET    (0x00100000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_DESTADDRSIZE       (0x00200000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_DESTADDROFFSET     (0x00400000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_LOGINSIZE          (0x00800000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_LOGINOFFSET        (0x01000000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_PASSWORDSIZE       (0x02000000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_PASSWORDOFFSET     (0x04000000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ACCESSNAMESIZE     (0x08000000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ACCESSNAMEOFFSET   (0x10000000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ICONID             (0x20000000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ICONQUALIFIER      (0x40000000)     // @paramdefine
#define SIM_PARAM_SIMOPENCHANNEL_ALL                (0x7fffffff)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMCLOSECHANNEL
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMCLOSECHANNEL_TEXTSIZE          (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMCLOSECHANNEL_TEXTOFFSET        (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMCLOSECHANNEL_CHANNELID         (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMCLOSECHANNEL_ICONID            (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMCLOSECHANNEL_ICONQUALIFIER     (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMCLOSECHANNEL_ALL               (0x0000001F)     // @paramdefine


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMSENDDATA
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMSENDDATA_FLAGS                 (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_TEXTSIZE              (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_TEXTOFFSET            (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_CHANNELDATASIZE       (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_CHANNELDATAOFFSET     (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_CHANNELID             (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_ICONID                (0x00000040)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_ICONQUALIFIER         (0x00000080)     // @paramdefine
#define SIM_PARAM_SIMSENDDATA_ALL                   (0x000000FF)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMRECEIVEDATA
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMRECEIVEDATA_CHANNELDATALENGTH  (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMRECEIVEDATA_TEXTSIZE           (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMRECEIVEDATA_TEXTOFFSET         (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMRECEIVEDATA_CHANNELID          (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMRECEIVEDATA_ICONID             (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMRECEIVEDATA_ICONQUALIFIER      (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMRECEIVEDATA_ALL                (0x0000003F)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMLANGUAGE
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMLANGUAGE_LANGUAGE              (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMLANGUAGE_ALL                   (0x00000001)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMRUNATCMD
//
// @comm None
//
// -----------------------------------------------------------------------------
#define SIM_PARAM_SIMRUNATCMD_CMDSIZE               (0x00000001)     // @paramdefine
#define SIM_PARAM_SIMRUNATCMD_CMDOFFSET             (0x00000002)     // @paramdefine
#define SIM_PARAM_SIMRUNATCMD_TEXTSIZE              (0x00000004)     // @paramdefine
#define SIM_PARAM_SIMRUNATCMD_TEXTOFFSET            (0x00000008)     // @paramdefine
#define SIM_PARAM_SIMRUNATCMD_ICONID                (0x00000010)     // @paramdefine
#define SIM_PARAM_SIMRUNATCMD_ICONQUALIFIER         (0x00000020)     // @paramdefine
#define SIM_PARAM_SIMRUNATCMD_ALL                   (0x0000003F)     // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTKITRSPITEM
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_ITEM_ID       (0x00000001) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_ITEM_ALL      (0x00000001) // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTKITRSPTEXT
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_TEXT_DCS        (0x00000001) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_TEXT_TEXTOFFSET (0x00000002) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_TEXT_TEXTSIZE   (0x00000004) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_TEXT_YESNO      (0x00000008) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_TEXT_ALL        (0x0000000F) // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTKITRSPPOLLINGINTERVAL
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_POLL_UNIT     (0x00000001) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_POLL_INTERVAL (0x00000002) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_POLL_ALL      (0x00000003) // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTKITRSPCLOSECHANNEL
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_CLOSECHANNEL_ID  (0x00000001) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_CLOSECHANNEL_ALL (0x00000001) // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTKITRSPRECEIVEDATA
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_RECEIVEDATA_ID                    (0x00000001) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_RECEIVEDATA_DATASIZE              (0x00000002) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_RECEIVEDATA_DATAOFFSET            (0x00000004) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_RECEIVEDATA_CHANNELDATALENGTH     (0x00000008) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_RECEIVEDATA_ALL                   (0x0000000f) 


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params SIMTKITRSPSENDDATA
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_SENDDATA_ID         (0x00000001) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_SENDDATA_DATALENGTH (0x00000002) // @paramdefine
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_SENDDATA_ALL        (0x00000003) // @paramdefine

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Response | Responses to proactive SIM commands
//
// @comm These match the definition of GSM 11.14, section 12.12
//
// -----------------------------------------------------------------------------
#define SIM_RESPONSE_OK                             (0x00000000)     // @constdefine A-OK
#define SIM_RESPONSE_OK_PARTIAL                     (0x00000001)     // @constdefine Command completed only partially
#define SIM_RESPONSE_OK_MISSINGINFO                 (0x00000002)     // @constdefine Command completed, some data missing
#define SIM_RESPONSE_OK_REFRESH                     (0x00000003)     // @constdefine Command completed with file refresh
#define SIM_RESPONSE_OK_MODIFIEDPARAMETERS          (0x00000007)     // @constdefine Command completed using modified parameters
#define SIM_RESPONSE_OK_ENDSESSION                  (0x00000010)     // @constdefine Command completed, user has terminated the session
#define SIM_RESPONSE_OK_BACK                        (0x00000011)     // @constdefine Command completed, user has navigated back
#define SIM_RESPONSE_OK_NORESPONSE                  (0x00000012)     // @constdefine Command completed, user did not respond
#define SIM_RESPONSE_OK_HELP                        (0x00000013)     // @constdefine Command completed, user requested help
#define SIM_RESPONSE_ERR_MOBILEUNABLE               (0x00000020)     // @constdefine Command error, mobile is busy and cannot perform requested action
#define SIM_RESPONSE_ERR_NETWORKUNABLE              (0x00000021)     // @constdefine Command error, network not available
#define SIM_RESPONSE_ERR_CALLSETUPREJECTED          (0x00000022)     // @constdefine Command error, user rejected call setup
#define SIM_RESPONSE_ERR_USERREJECTED               (0x00000022)     // @constdefine Command error, user rejected
#define SIM_RESPONSE_ERR_BROWSERGENERIC             (0x00000026)     // @constdefine Command error, launch browser generic error
#define SIM_RESPONSE_ERR_MOBILEINCAPABLE            (0x00000030)     // @constdefine Command error, requested action beyond mobile's capabilities
#define SIM_RESPONSE_ERR_COMMANDTYPE                (0x00000031)     // @constdefine Command error, problem with command type
#define SIM_RESPONSE_ERR_COMMANDDATA                (0x00000032)     // @constdefine Command error, problem with command data
#define SIM_RESPONSE_ERR_COMMANDNUMBER              (0x00000033)     // @constdefine Command error, problem with command number
#define SIM_RESPONSE_ERR_SS                         (0x00000034)     // @constdefine Command error, problem sending USSD
#define SIM_RESPONSE_ERR_SMS                        (0x00000035)     // @constdefine Command error, problem sending SMS
#define SIM_RESPONSE_ERR_VALUESMISSING              (0x00000036)     // @constdefine Command error, some values are missing
#define SIM_RESPONSE_ERR_BEARERINDEPENDENTPROTOCOLERROR        (0x0000003A) // @constdefine Command error, when bearer error occurs

// Additional Information with responses
#define SIM_ADDINFO_ERR_NETWORKUNABLE_NOSPECIFICREASON                  (0x00000000) // @constdefine Network error, no specific information can be given

#define SIM_ADDINFO_ERR_BIPERROR_NOSPECIFICREASON                       (0x00000000) // @constdefine Bearer error, no specific information can be given
#define SIM_ADDINFO_ERR_BIPERROR_NOCHANNELAVAILABLE                     (0x00000001) // @constdefine Bearer error, no channel available
#define SIM_ADDINFO_ERR_BIPERROR_CHANNELCLOSED                          (0x00000002) // @constdefine Bearer error, Channel already closed
#define SIM_ADDINFO_ERR_BIPERROR_CHANNELIDINVALID                       (0x00000003) // @constdefine Bearer error, Channel ID is invalid
#define SIM_ADDINFO_ERR_BIPERROR_REQUESTEDBUFFERNOTAVAILABLE            (0x00000004) // @constdefine Bearer error, Requested Buffer size not available
#define SIM_ADDINFO_ERR_BIPERROR_SECURITYERROR                          (0x00000005) // @constdefine Bearer error, Security error
#define SIM_ADDINFO_ERR_BIPERROR_REQUESTEDSIMMEINTERFACENOTAVAILABLE    (0x00000006) // @constdefine Bearer error

#define SIM_ADDINFO_ERR_MEUNABLE_NOSPECIFICREASON                       (0x00000000) // @constdefine MEUnable error, no specific information can be given
#define SIM_ADDINFO_ERR_MEUNABLE_SCREENISBUSY                           (0x00000001) // @constdefine MEUnable error, screen Busy
#define SIM_ADDINFO_ERR_MEUNABLE_BUSYONCALL                             (0x00000002) // @constdefine MEUnable error, Busy on Call
#define SIM_ADDINFO_ERR_MEUNABLE_BUSYONSSTRANSACTION                    (0x00000003) // @constdefine MEUnable error, Busy on SSTransaction
#define SIM_ADDINFO_ERR_MEUNABLE_NOSERVICE                              (0x00000004) // @constdefine MEUnable error, No service available
#define SIM_ADDINFO_ERR_MEUNABLE_ACCESSCONTROLCLASSBAR                  (0x00000005) // @constdefine MEUnable error, access control class bar
#define SIM_ADDINFO_ERR_MEUNABLE_RADIONOTGRANTED                        (0x00000006) // @constdefine MEUnable error, radio not granted
#define SIM_ADDINFO_ERR_MEUNABLE_NOTINSPEECHCALL                        (0x00000007) // @constdefine MEUnable error, not in speech call
#define SIM_ADDINFO_ERR_MEUNABLE_BUSYONUSSDTRANSACTION                  (0x00000008) // @constdefine MEUnable error, busy on USSD transaction
#define SIM_ADDINFO_ERR_MEUNABLE_BUSYONSENDSTMF                         (0x00000009) // @constdefine MEUnable error, busy on sending STMF


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Unavailable | Detailed reason for SIM_RESPONSE_ERR_MOBILEUNABLE
//
// @comm For SIM_RESPONSE_ERR_MOBILEUNABLE, GSM 11.14, 12.12.2
//
// -----------------------------------------------------------------------------
#define SIM_RESPONSE_ERR_MOBILEUNABLE_NOCAUSE       (0x00000000)     // @constdefine No cause given
#define SIM_RESPONSE_ERR_MOBILEUNABLE_SCREENBUSY    (0x00000001)     // @constdefine Screen is busy
#define SIM_RESPONSE_ERR_MOBILEUNABLE_BUSYONCALL    (0x00000002)     // @constdefine Phone is in a call
#define SIM_RESPONSE_ERR_MOBILEUNABLE_BUSYONSS      (0x00000003)     // @constdefine Phone is sending SS
#define SIM_RESPONSE_ERR_MOBILEUNABLE_NOSERVICE     (0x00000004)     // @constdefine No service available
#define SIM_RESPONSE_ERR_MOBILEUNABLE_ACCESSBAR     (0x00000005)     // @constdefine TBD
#define SIM_RESPONSE_ERR_MOBILEUNABLE_NORADIO       (0x00000006)     // @constdefine TBD
#define SIM_RESPONSE_ERR_MOBILEUNABLE_NOTINCALL     (0x00000007)     // @constdefine Phone is not in a call
#define SIM_RESPONSE_ERR_MOBILEUNABLE_BUSYONUSSD    (0x00000008)     // @constdefine Phone is sending USSD

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Unavailable | Detailed reason for Launch Browser Reponse
//
// -----------------------------------------------------------------------------
#define SIM_RESPONSE_ERR_LAUNCHBROWSER_BEARERUNAVAIL  (0x00000001)   // @constdefine Bearer Unavailable
#define SIM_RESPONSE_ERR_LAUNCHBROWSER_BROWSERUNAVAIL (0x00000002)   // @constdefine Browser Unavailable
#define SIM_RESPONSE_ERR_LAUNCHBROWSER_PROVISIONERROR (0x00000003)   // @constdefine Terminal unable to read the provisioning data.

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Unavailable | Detailed reason for support of toolkit functions
//
// -----------------------------------------------------------------------------
#define SIM_MEIMPLEMENTS                            (0x00000001)     // @constdefine The ME must implement this notification
#define SIM_RADIOIMPLEMENTS_NONOTIFICATION          (0x00000002)     // @constdefine The radio will implement and not give a notification to the ME
#define SIM_RADIOIMPLEMENTS_NOTIFICATION            (0x00000003)     // @constdefine The radio will implement and give a notification to the ME that it was done
#define SIM_RADIOIMPLEMENTS_REQUESTMEINPUT          (0x00000004)     // @constdefine The radio will implement, but requests information from the ME first

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Unavailable | Text encoding flags
//
// -----------------------------------------------------------------------------
#define SIMTKIT_TEXT_ENCODING_PACKED                (0x0)
#define SIMTKIT_TEXT_ENCODING_UNPACKED              (0x4)
#define SIMTKIT_TEXT_ENCODING_UCS2                  (0x8)

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Unavailable | Polling interval unit durations.
//
// -----------------------------------------------------------------------------
#define SIMTKIT_POLLINTERVAL_UNIT_DURATIONMINUTES   (0x0)
#define SIMTKIT_POLLINTERVAL_UNIT_DURATIONSECONDS   (0x01)
#define SIMTKIT_POLLINTERVAL_UNIT_DURATIONTENTHS    (0x02)


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @constants Unavailable | Event Download events.
//
// -----------------------------------------------------------------------------
#define SIMTKIT_EVENT_MTCALL                        (0x0)
#define SIMTKIT_EVENT_CALLCONNECTED                 (0x01)
#define SIMTKIT_EVENT_CALLDISCONNECTED              (0x02)
#define SIMTKIT_EVENT_LOCATIONSTATUS                (0x03)
#define SIMTKIT_EVENT_USERACTIVITY                  (0x04)
#define SIMTKIT_EVENT_IDLESCREEN                    (0x05)
#define SIMTKIT_EVENT_CARDREADERSTATUS              (0x06)
#define SIMTKIT_EVENT_LANGUAGESELECTION             (0x07)
#define SIMTKIT_EVENT_BROWSERTERMINATION            (0x08)
#define SIMTKIT_EVENT_DATAAVAILABLE                 (0x09)
#define SIMTKIT_EVENT_CHANNELSTATUS                 (0x0A)
#define SIMTKIT_EVENT_ACCESSTECHNOLOGYCHANGE        (0x0B)
#define SIMTKIT_EVENT_DISPLAYPARAMETERS             (0x0C)
#define SIMTKIT_EVENT_LOCALCONNECTION               (0x0D)

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTEXT | Used for SIM_NOTIFY_DISPLAYTEXT, SIM_GETINPUT, and
//         SIM_GETINKEY, SIM_SETUPIDLEMODETEXT messages
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtext_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFlags;                      // @field Contains SIMTEXT_* flags
    DWORD dwMinResponse;                // @field Minimum response length
    DWORD dwMaxResponse;                // @field Maximum response length
    DWORD dwTextSize;                   // @field Size of text message in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwDefaultTextSize;            // @field Size of default text message in bytes
    DWORD dwDefaultTextOffset;          // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMTEXT, FAR *LPSIMTEXT;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMMENUITEM | Used in the SIMMENU structure
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simmenuitem_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwIdentifier;                 // @field Identifies each item on the menu
    DWORD dwFlags;                      // @field Contains SIMMENU_* flags
    DWORD dwNextAction;                 // @field Contains SIM_NOTIFY_* flags indicating what
                                        //        the next proactive command could be
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMMENUITEM, FAR *LPSIMMENUITEM;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMMENU | Used for SIM_NOTIFY_SETUPMENU and SIM_NOTIFY_SELECTITEM
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simmenu_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFlags;                      // @field Contains SIMMENU_* flags
    DWORD dwDefaultItem;                // @field Index of default item
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwMenuItemCount;              // @field Number of menu items
    DWORD dwMenuItemSize;               // @field Size of each SIMMENUITEM structure in bytes
    DWORD dwMenuItemOffset;             // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMMENU, FAR *LPSIMMENU;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMSMS | Used for SIM_NOTIFY_SENDSMS
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simsms_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFlags;                      // @field Contains SIMMENU_* flags
    DWORD dwAddressType;                // @field Contains SIMADDRTYPE_* flags
    DWORD dwNumPlan;                    // @field Contains SIMNUMPLAN_* flags
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwAddressSize;                // @field Size of address in bytes
    DWORD dwAddressOffset;              // @field Offset from start of this structure in bytes
    DWORD dwTPDUSize;                   // @field Size of SMS TPDU
    DWORD dwTPDUOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMSMS, FAR *LPSIMSMS;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMUSSD | Used for SIM_NOTIFY_SENDSS
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simussd_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwAddressType;                // @field Contains SIMADDRTYPE_* flags
    DWORD dwNumPlan;                    // @field Contains SIMNUMPLAN_* flags
    DWORD dwUSSDSize;                   // @field Size of USSD
    DWORD dwUSSDOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMUSSD, FAR *LPSIMUSSD;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMCALL | Used for SIM_NOTIFY_SETUPCALL
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simcall_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFlags;                      // @field Contains SIMCALL_* flags
    DWORD dwRedialDuration;             // @field Number of seconds to attempt redialing
    DWORD dwAddressType;                // @field Contains SIMADDRTYPE_* flags
    DWORD dwNumPlan;                    // @field Contains SIMNUMPLAN_* flags
    DWORD dwTextSize;                   // @field Size of User Confirmation text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwAddressSize;                // @field Size of address in bytes
    DWORD dwAddressOffset;              // @field Offset from start of this structure in bytes
    DWORD dwSubAddrSize;                // @field Size of subaddress in bytes
    DWORD dwSubAddrOffset;              // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for User Confirmation icon
    DWORD dwIconQualifier;              // @field Qualifier designating how the User Confirmation icon should be used.
    DWORD dwCallSetupTextSize;          // @field Size of associated text in bytes
    DWORD dwCallSetupTextOffset;        // @field Offset from start of this structure in bytes
    DWORD dwCallSetupIconIdentifier;    // @field Identifier for the Call Setup Phase icon
    DWORD dwCallSetupIconQualifier;     // @field Qualifier designating how the Call Setup Phase icon should be used.
} SIMCALL, FAR *LPSIMCALL;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTONE | Used for SIM_NOTIFY_PLAYTONE
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtone_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwTone;                       // @field Contains SIMTONE_* tone types
    DWORD dwDuration;                   // @field Duration in milliseconds
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMTONE, FAR *LPSIMTONE;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMLOCALINFO | Used for SIM_NOTIFY_LOCALINFO
//
// @comm Note that we don't support NMR and BCCH -- they're included for completeness
//
// -----------------------------------------------------------------------------
typedef struct simlocalinfo_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwLAC;                        // @field Location Area Code
    DWORD dwCellId;                     // @field Cellular ID
    TCHAR lpszNumName[MAXLEN_OPERATOR_NUMERIC]; // @field Numeric operator name
    TCHAR lpszIMEI[MAXLEN_IMEI];        // @field IMEI of the phone
    BYTE rgbNMR[MAXLEN_NMR];            // @field Network Measurement Results
    BYTE rgbBCCH[MAXLEN_BCCH * MAXNUM_BCCH];    // @field BCCH - 16 bytes per sub-list
    DWORD dwNumBCCH;                    // @field Number of BCCH sub-lists, 1-3
} SIMLOCALINFO, *LPSIMLOCALINFO;


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct BEARERDESC | Used for Bearer description in response to Open Channel command
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct bearerdesc_tag
{     
    BYTE bPrecedence;
    BYTE bDelay;
    BYTE bReliability;
    BYTE bPeak;
    BYTE bMean;
    BYTE bPDPType;
    SHORT iReserved;
} BEARERDESC, *LPBEARERDESC;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMRESPONSE | Used for SimSendResponse function
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simresponse_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwResponse;                   // @field Response
    DWORD dwAdditionalInfo;             // @field Additional response data
    DWORD dwPollInterval;               // @field Polling interval
    LPTSTR lpszText;                    // @field Response text
    DWORD dwFlags;                      // @field Indicates how the text should be formatted
    DWORD dwItem;                       // @field Selected item
    LPSIMLOCALINFO lpLocalInfo;         // @field Local info
    DWORD  dwChannelId;                 // @field Channel Id.
    DWORD  dwChannelDataSize;           // @field Size of channel data
    LPBYTE lpChannelData;               // @field Channel data

    DWORD  dwChannelDataLength;         // @field Channel data length (empty space of Tx Buffer)
    DWORD  dwBufferSize;                // @field Open Channel Buffer Size 
    DWORD  dwBearerType;                // @field Bearer type
    LPBEARERDESC lpBearerDesc;          // @field Bearer Description
} SIMRESPONSE, *LPSIMRESPONSE;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMEVENTLIST | Used for SIM_NOTIFY_EVENTLIST
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simeventlist_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwEventsSize;                 // @field Size of associated event list in bytes
    DWORD dwEventsOffset;               // @field Offset from start of this structure in bytes
} SIMEVENTLIST, FAR *LPSIMEVENTLIST;


// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMFILEREFERENCE | Used to contain a SIM file reference.
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simfilereference_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFileSize;                   // @field Size of associated file reference in bytes
    DWORD dwFileOffset;                 // @field Offset from start of this structure in bytes
} SIMFILEREFERENCE, FAR *LPSIMFILEREFERENCE;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMSENDDTMF | Used for SIM_NOTIFY_SENDDTMF
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simsenddtmf_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwDTMFSize;                   // @field Size of associated DTMF String in bytes
    DWORD dwDTMFOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMSENDDTMF, FAR *LPSIMSENDDTMF;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMLAUNCHBROWSER | Used for SIM_NOTIFY_LAUNCHBROWSER
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simlaunchbrowser_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFlags;                      // @field Command Qualifier details
    DWORD dwBrowserId;                  // @field Browser Identity
    DWORD dwURLSize;                    // @field Size of associated URL in bytes
    DWORD dwURLOffset;                  // @field Offset from start of this structure in bytes
    DWORD dwBearerSize;                 // @field Size of associated Bearer list in bytes
    DWORD dwBearerOffset;               // @field Offset from start of this structure in bytes
    DWORD dwGatewaySize;                // @field Size of associated Gateway in bytes
    DWORD dwGatewayOffset;              // @field Offset from start of this structure in bytes
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwProvisionFileCount;         // @field Number of Provisioning files
    DWORD dwProvisionFileSize;          // @field Size of each SIMFILEREFERENCE structure in bytes
    DWORD dwProvisionFileOffset;        // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMLAUNCHBROWSER, FAR *LPSIMLAUNCHBROWSER;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMOPENCHANNEL | Used for SIM_NOTIFY_OPENCHANNEL
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simopenchannel_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFlags;                      // @field Indicates valid parameters
    DWORD dwBearerDescType;             // @field Bearer type
    DWORD dwBufferSize;                 // @field Channel buffer size
    DWORD dwAddressType;                // @field Contains SIMADDRTYPE_* flags
    DWORD dwNumPlan;                    // @field Contains SIMNUMPLAN_* flags
    DWORD dwDuration1;                  // @field Duration 1, reconnect tries
    DWORD dwDuration2;                  // @field Duration 2, Inactivity timeout
    DWORD dwLocalAddrType;              // @field Local address type
    DWORD dwDestAddrType;               // @field Destination address type
    DWORD dwProtocolType;               // @field Transport Protocol type.
    DWORD dwPortNum;                    // @field Transport Port number
    DWORD dwTextSize;                   // @field Size of associated text (alphaID) in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwBearerDescSize;             // @field Size of Bearer description parameters in bytes
    DWORD dwBearerDescOffset;           // @field Offset from start of this structure in bytes
    DWORD dwAddressSize;                // @field Size of CS dialing number in bytes
    DWORD dwAddressOffset;              // @field Offset from start of this structure in bytes
    DWORD dwSubAddressSize;             // @field Size of subaddress in bytes
    DWORD dwSubAddressOffset;           // @field Offset from start of this structure in bytes
    DWORD dwLocalAddrSize;              // @field Size of local IP address in bytes
    DWORD dwLocalAddrOffset;            // @field Offset from start of this structure in bytes
    DWORD dwDestAddrSize;               // @field Size of destination IP address in bytes
    DWORD dwDestAddrOffset;             // @field Offset from start of this structure in bytes
    DWORD dwLoginSize;                  // @field Size of user login in bytes
    DWORD dwLoginOffset;                // @field Offset from start of this structure in bytes
    DWORD dwPasswordSize;               // @field Size of user password in bytes
    DWORD dwPasswordOffset;             // @field Offset from start of this structure in bytes
    DWORD dwAccessNameSize;             // @field Size of access point name in bytes
    DWORD dwAccessNameOffset;           // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMOPENCHANNEL, FAR *LPSIMOPENCHANNEL;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMCLOSECHANNEL | Used for SIM_NOTIFY_CLOSECHANNEL
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simclosechannel_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwChannelId;                  // @field Channel Id command is for
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMCLOSECHANNEL, FAR *LPSIMCLOSECHANNEL;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMRECEIVEDATA | Used for SIM_NOTIFY_RECEIVEDATA
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simreceivedata_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwChannelId;                  // @field Channel Id command is for
    DWORD dwChannelDataLength;          // @field Number of bytes requested.
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMRECEIVEDATA, FAR *LPSIMRECEIVEDATA;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMSENDDATA | Used for SIM_NOTIFY_SENDDATA
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simsenddata_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwFlags;                      // @field Command Qualifier details
    DWORD dwChannelId;                  // @field Channel Id command is for
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwChannelDataSize;            // @field Size of associated data in bytes
    DWORD dwChannelDataOffset;          // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMSENDDATA, FAR *LPSIMSENDDATA;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMLANGUAGE | Used for SIM_NOTIFY_LANGUAGENOTIFICATION
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simlanguage_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    TCHAR lpszLanguage[MAXLEN_LANGUAGE];// @field Language code
} SIMLANGUAGE, FAR *LPSIMLANGUAGE;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMRUNATCMD | Used for SIM_NOTIFY_RUNATCOMMAND
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simrunatcmd_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwCmdSize;                    // @field Size of AT Command text in bytes
    DWORD dwCmdOffset;                  // @field Offset from start of this structure in bytes
    DWORD dwTextSize;                   // @field Size of associated text in bytes
    DWORD dwTextOffset;                 // @field Offset from start of this structure in bytes
    DWORD dwIconIdentifier;             // @field Identifier for Icon to use.
    DWORD dwIconQualifier;              // @field Qualifier designating how the icon should be used.
} SIMRUNATCMD, FAR *LPSIMRUNATCMD;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTOOLKITCAPS | Used for SimToolkitGetDevCaps
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtoolkitcaps_tag {
    DWORD dwRefresh;                    // @constdefine TBD
    DWORD dwMoreTime;                   // @constdefine TBD
    DWORD dwPollInterval;               // @constdefine TBD
    DWORD dwPollingOff;                 // @constdefine TBD
    DWORD dwSetUpCall;                  // @constdefine TBD
    DWORD dwSendSS;                     // @constdefine TBD
    DWORD dwSendUSSD;                   // @constdefine TBD
    DWORD dwSendSMS;                    // @constdefine TBD
    DWORD dwPlayTone;                   // @constdefine TBD
    DWORD dwDisplayText;                // @constdefine TBD
    DWORD dwGetInkey;                   // @constdefine TBD
    DWORD dwSetupIdleModeText;          // @constdefine TBD
    DWORD dwGetInput;                   // @constdefine TBD
    DWORD dwSelectItem;                 // @constdefine TBD
    DWORD dwSetupMenu;                  // @constdefine TBD
    DWORD dwLocalInfo;                  // @constdefine TBD
    DWORD dwSetupEventList;             // @constdefine TBD
    DWORD dwSendDTMF;                   // @constdefine TBD
    DWORD dwLaunchBrowser;              // @constdefine TBD
    DWORD dwOpenChannel;                // @constdefine TBD
    DWORD dwCloseChannel;               // @constdefine TBD
    DWORD dwReceiveData;                // @constdefine TBD
    DWORD dwSendData;                   // @constdefine TBD
    DWORD dwRunAtCmd;                   // @constdefine TBD
    DWORD dwTimerManagement;            // @constdefine TBD 
    DWORD dwEventMTCall;                // @constdefine TBD
    DWORD dwEventCallConnected;         // @constdefine TBD
    DWORD dwEventCallDisconnected;      // @constdefine TBD
    DWORD dwEventLocationStatus;        // @constdefine TBD
    DWORD dwEventUserActivity;          // @constdefine TBD
    DWORD dwEventIdleScreen;            // @constdefine TBD
    DWORD dwEventLanguageSelection;     // @constdefine TBD
    DWORD dwEventBrowserTermination;    // @constdefine TBD
    DWORD dwEventDataAvailable;         // @constdefine TBD
    DWORD dwEventChannelStatus;         // @constdefine TBD
    DWORD dwEventDisplayChange;         // @constdefine TBD

    DWORD dwGetChannelStatus;           // @constdefine for getchannelstatus command in BIP
} SIMTOOLKITCAPS, *LPSIMTOOLKITCAPS;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPITEM | Detailed information used for Select Item command
//                          responses.
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrspitem_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwId;                         // @field Selected Item Identifier
} SIMTKITRSPITEM, *LPSIMTKITRSPITEM;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPTEXT | Detailed information used for Get Input and Get Inkey
//                          command responses.
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrsptext_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwDCS;                        // @field Indicates requested data encoding
                                        // scheme for text.
    DWORD dwYesNo;                      // @field Yes/No response
    DWORD dwTextOffset;                 // @field Offset from structure for text.
    DWORD dwTextSize;                   // @field Size of text.
} SIMTKITRSPTEXT, FAR *LPSIMTKITRSPTEXT;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPPOLLINTERVAL | Detailed information used for Poll Interval
//                                  command responses.
//
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrsppollinterval_tag {
    DWORD cbSize;               // @field Structure size in bytes
    DWORD dwParams;             // @field Indicates valid parameters
    DWORD dwUnit;               // @field Time Unit. i.e. Minutes, seconds, 1/10s etc.
    DWORD dwInterval;           // @field Interval, number of units.
} SIMTKITRSPPOLLINTERVAL, FAR *LPSIMTKITRSPPOLLINTERVAL;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPCLOSECHANNEL | Detailed information used for Close Channel
//                                  command responses.
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrspclosechannel_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwChannelId;                  // @field Channel Identifier
} SIMTKITRSPCLOSECHANNEL, FAR *LPSIMTKITRSPCLOSECHANNEL;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPRECEIVEDATA | Detailed information used for Receive Data
//                                 command responses.
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrspreceivedata_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwChannelId;                  // @field Channel Identifier
    DWORD dwDataSize;                   // @field Channel Data size
    DWORD dwDataOffset;                 // @field Channel data
    DWORD dwChannelDataLength;          // @field Channel Data remaining in Rx buffer
} SIMTKITRSPRECEIVEDATA, FAR *LPSIMTKITRSPRECEIVEDATA;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPSENDDATA | Detailed information used for Send Data
//                              command responses.
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrspsenddata_tag {
    DWORD cbSize;                       // @field Structure size in bytes
    DWORD dwParams;                     // @field Indicates valid parameters
    DWORD dwChannelId;                  // @field Channel Identifier
    DWORD dwDataLength;                 // @field Channel Data length
} SIMTKITRSPSENDDATA, FAR *LPSIMTKITRSPSENDDATA;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params Open Channel Response Params
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_ID                    (0x00000001)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_CHANNELSTATUSOFFSET   (0x00000002)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_CHANNELSTATUSSIZE     (0x00000004)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_BEARERTYPE            (0x00000008)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_BEAREROFFSET          (0x00000010)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_BEARERSIZE            (0x00000020)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_BUFFERSIZE            (0x00000040)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_OPENCHANNEL_ALL                   (0x0000007F)

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPOPENCHANNEL | Detailed information used for Open Channel
//                              command responses.
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrspopenchannel_tag
{
    DWORD cbSize;
    DWORD dwParams;
    DWORD dwChannelID;
    DWORD dwChannelStatusOffset;
    DWORD dwChannelStatusSize;
    DWORD dwBearerType;
    DWORD dwBearerOffset;
    DWORD dwBearerSize;
    DWORD dwBufferSize;
} SIMTKITRSPOPENCHANNEL, FAR *LPSIMTKITRSPOPENCHANNEL;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params GetChannelStatus Response Params
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_GETCHANNELSTATUS_CHANNELCOUNT           (0x00000001)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_GETCHANNELSTATUS_CHANNELSTATUSOFFSET    (0x00000002)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_GETCHANNELSTATUS_CHANNELSTATUSSIZE      (0x00000004)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_GETCHANNELSTATUS_ALL                    (0x00000007)

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKITRSPGETCHANNELSTATUS | Detailed information used for Get Channel 
//                                      Status command responses.
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkitrspgetchannelstatus_tag
{
      DWORD cbSize;
      DWORD dwParams;
      DWORD dwChannelCount;
      DWORD dwChannelStatusOffset;
      DWORD dwChannelStatusSize;
} SIMTKITRSPGETCHANNELSTATUS, FAR *LPSIMTKITRSPGETCHANNELSTATUS;

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @params Event Download Response Params
//
// @comm None
//
// -----------------------------------------------------------------------------
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_EVENTDOWNLOAD_CHANNELID           (0x00000001)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_EVENTDOWNLOAD_CHANNELSTATUSOFFSET (0x00000002)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_EVENTDOWNLOAD_CHANNELSTATUSSIZE   (0x00000004)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_EVENTDOWNLOAD_CHANNELDATALENGTH   (0x00000008)
#define RIL_PARAM_SIMTKIT_RSP_DETAILS_EVENTDOWNLOAD_ALL                 (0x0000000f)

// -----------------------------------------------------------------------------
//
// @doc EXTERNAL
//
// @struct SIMTKEVENTDOWNLOAD | Detailed information used for Event Download.
// @comm None
//
// -----------------------------------------------------------------------------
typedef struct simtkiteventdownload_tag {
      DWORD cbSize;
      DWORD dwParams;
      DWORD dwChannelID;
      DWORD dwChannelStatusOffset;
      DWORD dwChannelStatusSize;    
      DWORD dwChannelDataLength;
}SIMTKEVENTDOWNLOAD, FAR *LPSIMTKEVENTDOWNLOAD;

// Events used for BIP commands
#define DATAAVAILABLE_EVENT      TEXT("SIMToolkit/EventThread/DataAvailable")
#define CHANNELSTATUS_EVENT      TEXT("SIMToolkit/EventThread/ChannelStatus")
#define CHANNELDATASIZEBYTES     3 // Number of bytes channel data conatins for single channel
#define CHANNELSTATUS_LINKSTATE_ESTABLISHED             1   //Used in Channel Status Link State, Link was established
#define CHANNELSTATUS_CHANNELSTATE_NOINFOCANBEGIVEN     0   //Used in Channel Status Channel State, No Info can be given
