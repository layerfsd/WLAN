//------------------------------------------------------------------------------
// <copyright file="AR6K_version.h" company="Atheros">
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

#define __VER_MAJOR_ 2
#define __VER_MINOR_ 2
#define __VER_PATCH_ 1

  
/* The makear6ksdk script (used for release builds) modifies the following line. */
#define __BUILD_NUMBER_ 196

//buildercheck
#define MMC_BUILD_DATE	TEXT("2013.08.26")
#define MMC_BUILD_DATEs	"2013.08.26"
#define MMC_BUILD_NUM	58

//builderver
/*
058:20130826
  change In ad-hoc mode has been modified to send event to host.
057:20121123
  add eapol tx/rx key nonce,iv,mic debug print
056:20121019
  add assoc channel save : m_ChannelLast => todo: registry setting, ssid matching
055:20120614
  add targetfailEvent(bb req)
054:20120608
  sleep test: remove power on/off lock check
054:20120419
  add lockSSID,lockEnable
053:20120413
  add nodage timer
052:20120404
  2.2.1.83 bluebird windowmobile
051:20120322
  change htcstart disalbleint to maskint
  remove bluebird patch
  etc setpnppower
050:20120316
  remove htc_stopping lock
  remove tx/rx flush lock
  add htc lock htc_stop
  add 100ms sleep after htc_stopping
046:20120315b
  htcstop rx lock->htc lock
  remove resetdevice
045:20120315
  htcstop rx lock removed
043:20120313b
  bluebird suspend test
  remove power state change notify in cut power
042:20120313a
  bluebird suspend test
  remove test messages
041:20120313
  bluebird suspend test
  remove int disable
040:20120312b
  bluebird suspend test
  change suspend process (remove tx/rx flush)
039:20120312
  bluebird suspend test
039:20120228
  g only mode
038:20120207a
  change roam timeout 5=>2sec
037:20120207
  remove tp recover
  add m_Roamtimeout for os dis ind
036:20120203d
  change tp setting for configure_ar6000
035:20120203c
  tp setting for startendpoint
034:20120203b
  restore thread priority set
033:20120203a
  tp regparse buf fix
  remove thread priority set
032:20120203
  sdio full clock
031:20120126a
  probe ssid 1 set connect cmd ssid
  add probe ssid 2 set registry 
  add scan ch dwell time registry
030:20120126
  remove add bss rssi check -80
029:20120118
  change ssid probe 3 => 0
028:20120118
  remove roaming retry, disconnect event to ndis
027:20120113b
  add sleep 100ms in discon ind re connect
  change rssi timer 3sec => 2sec
026:20120113a
  move clear rssi to discon ind
  add sleep 20ms in discon ind re connect
025:20120113
  remove RSSI < -80dB AP add bss
  connect compare buf 16K->32K
024:20120112a
  add remove bss info log, roaming compare list log
  roaming compare buf 16K->32K
023:20120112
  DOTEL:remove probe setting when connect command
  roaming retry period 2->5secs
  remove ssid cache in bss info rx, ignore hidden ap beacon
022:20120111
  remove force long scan when roaming target not avail
021:20120111
  clear rssi history in firm when connected event
020:20120110
  change ps param setting
019:20120110
  mbox0 block size 128, half clock mode
018:20120110
  add thread priority registry setting
  mbox0 block size 1, full clock mode
017:20120106
  add probe SSID setting for hidden SSID
016:20120103
  change default setting
015:20111230
  change thread priority to 100
014:20111215
  change roaming handler(stat event => rssi timer)
  add long scan when roaming target unavail
013:20111118
  change rssi timer, nodeage, roaming command
012:20111102
  change roaming retry, add disconnect
011:20111028
  add BSSID match channel hint
010:20111019
  change roaming connect command in periodic retry, timer
009:20111014
  add hostassist roaming 2sec periodic retry
008:20111007
  remove low rssi force scan
007:20110916
  registry psmode set in startendpoints(old in configparam)
  30us resumedelay D0 set before startendpoints
  force rssi update 1secs
006:20110902
  merge 2.2.1.192 patch
005:20110825 from 2.1.2.16(b19)
	add file log path registry...
	add error message log
	registry channel setting
004: 20110721
  change driver roaming
  add roaminprogress state
003: 20110720
  remove channel hint
002: 20110719
  merge 2.1.2.16_b15
001: 20110608
  merge 2.1.2.16_b13
*/

/* Format of the version number. */
#define VER_MAJOR_BIT_OFFSET		28
#define VER_MINOR_BIT_OFFSET		24
#define VER_PATCH_BIT_OFFSET		16
#define VER_BUILD_NUM_BIT_OFFSET	0

#define VER_BUILD_MMC_BIT_OFFSET	8

/* 
 * The version has the following format:
 * Bits 28-31: Major version
 * Bits 24-27: Minor version
 * Bits 16-23: Patch version
 * Bits 0-15:  Build number (automatically generated during build process ) 
 * E.g. Build 1.1.3.7 would be represented as 0x11030007.
 *  
 * DO NOT split the following macro into multiple lines as this may confuse the build scripts. 
 */
#define AR6K_SW_VERSION     ( ( __VER_MAJOR_ << VER_MAJOR_BIT_OFFSET ) + ( __VER_MINOR_ << VER_MINOR_BIT_OFFSET ) + ( __VER_PATCH_ << VER_PATCH_BIT_OFFSET ) + ( MMC_BUILD_NUM << VER_BUILD_MMC_BIT_OFFSET ) + ( __BUILD_NUMBER_ << VER_BUILD_NUM_BIT_OFFSET ) )
//#define AR6K_SW_VERSION 	( ( __VER_MAJOR_ << VER_MAJOR_BIT_OFFSET ) + ( __VER_MINOR_ << VER_MINOR_BIT_OFFSET ) + ( __VER_PATCH_ << VER_PATCH_BIT_OFFSET ) + ( __BUILD_NUMBER_ << VER_BUILD_NUM_BIT_OFFSET ) )


