//------------------------------------------------------------------------------
// <copyright file="athtestcmd.c" company="Atheros">
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
#include <windows.h>
#include <winioctl.h>
#include <Ntddndis.h>
#include <Nuiouser.h>

#include <a_config.h>

// Driver specific includes
#include <a_drv.h>
#include <a_types.h>
#include <athdefs.h>
#include <a_osapi.h>
#include <wmi.h>
#include <testcmd.h>


// Declare global const

#define ERRX(x)                           x ## ",errno:%u\n"
#define ERR(x)                            printf(ERRX(x),GetLastError())

#define OPT_I_DEVICE                      __T("-i")

#define OPT_TX_CMD                        __T("--tx")
#define OPT_RX_CMD                        __T("--rx")
#define OPT_PM_CMD                        __T("--pm")
#define OPT_SETMAC_CMD                    __T("--setmac")
#define OPT_HELP_CMD                      __T("help")

#define OPT_TX_FREQ                       __T("--txfreq")
#define OPT_TX_RATE                       __T("--txrate")
#define OPT_TX_POWER                      __T("--txpwr")
#define OPT_TX_ANT                        __T("--txantenna")
#define OPT_TX_PKTSZ                      __T("--txpktsz")
#define OPT_TX_PAT                        __T("--txpattern")
#define OPT_TX_ANI                        __T("--ani")
#define OPT_TX_SCRAMBLEOFF                __T("--scrambleroff")
#define OPT_TX_AIFSN                      __T("--aifsn")
#define OPT_TX_MODE_SINE                  __T("sine")
#define OPT_TX_MODE_FRAME                 __T("frame")
#define OPT_TX_MODE_TX99                  __T("tx99")
#define OPT_TX_MODE_TX100                 __T("tx100")
#define OPT_TX_MODE_OFF                   __T("off")

#define OPT_RX_FREQ                       __T("--rxfreq")
#define OPT_RX_ANT                        __T("--rxantenna")
#define OPT_RX_ACT_PROMIS                 __T("promis")
#define OPT_RX_ACT_FILTER                 __T("filter")
#define OPT_RX_ACT_REPORT                 __T("report")
#define OPT_RX_ANI                        __T("--ani")
#define OPT_ACT_ANT_SWITCH                __T("--SetAntSwitchTable")

#define OPT_PM_WAKEUP                     __T("wakeup")
#define OPT_PM_SLEEP                      __T("sleep")

#define G_RATE_NUM                        12
#define INVALID_FREQ                      0
#define A_RATE_NUM                        8

#define RATE_STR_LEN                      7

typedef char RATE_STR[RATE_STR_LEN];
const RATE_STR  bgRateStrTbl[G_RATE_NUM] = {
    { "1   Mb" },
    { "2   Mb" },
    { "5.5 Mb" },
    { "11  Mb" },
    { "6   Mb" },
    { "9   Mb" },
    { "12  Mb" },
    { "18  Mb" },
    { "24  Mb" },
    { "36  Mb" },
    { "48  Mb" },
    { "54  Mb" }
};
const RATE_STR  aRateStrTbl[A_RATE_NUM] = {
    { "6   Mb" },
    { "9   Mb" },
    { "12  Mb" },
    { "18  Mb" },
    { "24  Mb" },
    { "36  Mb" },
    { "48  Mb" },
    { "54  Mb" }
};

/*
 * These frequency values are as per channel tags and regulatory domain
 * info. Please update them as database is updated.
 */
#define A_FREQ_MIN              4920
#define A_FREQ_MAX              5825

#define A_CHAN0_FREQ            5000
#define A_CHAN_MAX              ((A_FREQ_MAX - A_CHAN0_FREQ)/5)

#define BG_FREQ_MIN             2412
#define BG_FREQ_MAX             2484

#define BG_CHAN0_FREQ           2407
#define BG_CHAN_MIN             ((BG_FREQ_MIN - BG_CHAN0_FREQ)/5)
#define BG_CHAN_MAX             14      /* corresponding to 2484 MHz */

#define A_20MHZ_BAND_FREQ_MAX   5000

// Global vars

TCHAR  devName[128];
HANDLE hAdapter;
char buf[1024];
static A_BOOL needRxReport = FALSE;
PNDISUIO_SET_OID NdisCmd = (PNDISUIO_SET_OID)buf;

static void
prtRateTbl( A_UINT32 freq )
{
   int i;

   if ((INVALID_FREQ == freq) || ((freq >= BG_FREQ_MIN) &&
                      (freq <= BG_FREQ_MAX))) {
       if (INVALID_FREQ == freq)
           printf("Please choose <rate> as below table shows for 11b/g mode:\n");
       else
           printf("Please choose <rate> as below table shows for frequency %d (11b/g mode):\n", freq);

       for (i = 0; i < G_RATE_NUM; i++) {
           printf("<rate> %d \t \t %s \n", i, bgRateStrTbl[i]);
       }
       printf("\n");
   }

   if ((INVALID_FREQ == freq) || ((freq >= A_FREQ_MIN) &&
                       (freq <= A_FREQ_MAX))) {
       if (INVALID_FREQ == freq)
          printf("Please choose <rate> as below table shows for 11a mode:\n");
       else
          printf("Please choose <rate> as below table shows for frequency %d (11a mode):\n", freq);

       for (i = 0; i < A_RATE_NUM; i++) {
          printf("<rate> %d \t \t %s \n", i, aRateStrTbl[i]);
       }
   }
}


void
PrintUsage (void)
{
   printf("Usage: athtestcmd -i <device> command [options]\r\n");
   printf("\r\n");
   printf("Commands:\r\n");
   printf("--tx <sine/frame/tx99/tx100/off>\r\n");
   printf("\t --txfreq <Tx channel or freq(default 2412>\r\n");
   printf("\t --txrate <rate index> \r\n");
   printf("\t --txpwr <data: 0-30dBm sine: 0-1047\r\n");
   printf("\t --txantenna <1/2/0 (auto)>\r\n");
   printf("\t --txpktsz <pkt size, [32-1500](default 1500>\r\n");
   printf("\t --txpattern <tx data pattern, 0: all zeros; 1: all ones, 2:repeating 10, 3: PN7, 4:PN9, 5:PN15\r\n");
   printf("--ani (Enable ANI. The ANI is disabled if this option is not specified\r\n");
   printf("--scrambleroff (Disable scrambler. The scrambler is enabled by default\r\n");
   printf("--aifsn <AIFS slots num,[0-252](Used only under --tx frame mode)>\r\n");
   printf("--rx <promis/filter/report> --rxfreq <Rx channel or freq(default 2412)> --rxantenna <1/2/0 (auto)> \r\n");
   printf("--setmac <mac addr like 00:03:7f:be:ef:11> \r\n");
   printf("--pm <wakeup/sleep> \r\n");
   printf("--SetAntSwitchTable <table1 in decimal value> <table2 in decimal value>  (Set table1=0 and table2=0 will restore the default AntSwitchTable) \r\n");

   printf("help prints this message \r\n");
   prtRateTbl(INVALID_FREQ);
}

static A_UINT32
pktSzValid(A_UINT32 val)
{
    if (( val < 32 )||(val > 1500)){
        printf("Invalid package size! < 32 - 1500 >\n");
        exit(-1);
    }
    return val;
}

static A_UINT16
wmic_ieee2freq(A_UINT32 chan)
{
    if (chan == BG_CHAN_MAX) {
        return BG_FREQ_MAX;
    }
    if (chan < BG_CHAN_MAX) {    /* 0-13 */
        return (BG_CHAN0_FREQ + (chan*5));
    }
    if (chan <= A_CHAN_MAX) {
        return (A_CHAN0_FREQ + (chan*5));
    }
    else {
        return INVALID_FREQ;
    }
}


static int
freqValid(int val)
{
   do {
      if (val <= A_CHAN_MAX) {
      A_UINT16 freq;

      if (val < BG_CHAN_MIN)
          break;

          freq = wmic_ieee2freq(val);
          if (INVALID_FREQ == freq)
             break;
          else
             return freq;
      }

      if ((val == BG_FREQ_MAX) ||
         ((val < BG_FREQ_MAX) && (val >= BG_FREQ_MIN) &&
         !((val - BG_FREQ_MIN) % 5)))
            return val;
      else if ((val >= A_FREQ_MIN) && (val < A_20MHZ_BAND_FREQ_MAX) &&
              !((val - A_FREQ_MIN) % 20))
            return val;
      else if ((val >= A_20MHZ_BAND_FREQ_MAX) && (val <= A_FREQ_MAX) &&
              !((val - A_20MHZ_BAND_FREQ_MAX) % 5))
            return val;
      } while(FALSE);
   printf("Invalid channel or freq #: %d !\n", val);
   return(-1);
}

static A_UINT32
rateValid(A_UINT32 val, A_UINT32 freq)
{
   if (((freq >= A_FREQ_MIN) && (freq <= A_FREQ_MAX) && (val >= A_RATE_NUM)) ||
      ((freq >= BG_FREQ_MIN) && (freq <= BG_FREQ_MAX) && (val >= G_RATE_NUM))) {
      printf("Invalid rate value %d for frequency %d! \n", val, freq);
      prtRateTbl(freq);
      return -1;
   }
   return val;
}

static int
txPwrValid ( int pwr, int mode )
{
   if (mode == TCMD_CONT_TX_SINE) {
      if ((pwr >= 0) && (pwr <= 1047))
         return pwr;
   } else if (mode != TCMD_CONT_TX_OFF) {
      if ((pwr >= -15) && (pwr <= 30))
         return pwr;
   } else if (mode == TCMD_CONT_TX_OFF) {
      return 0;
   }
   printf("Invalid Tx Power value! \nTx data: [-15 - 30]dBm  \nTx sine: [-15 - 30]dBm  \n");
   return (-1);
}

static int
antValid(int val)
{
   if (val > 2) {
      printf("Invalid antenna setting! <0: auto;  1/2: ant 1/2>\n");
      return(-1);
   }
   return val;
}

static int
wmic_ether_aton(const TCHAR *orig, A_UINT8 *eth)
{
   const TCHAR *bufp;
   int i;

   i = 0;
   for (bufp = orig; *bufp != '\0'; ++bufp) {
      unsigned int val;
      TCHAR c = *bufp++;

      if (_istdigit(c)) val = c - __T('0');
      else if (c >= __T('a') && c <= __T('f')) val = c - __T('a') + 10;
      else if (c >= __T('A') && c <= __T('F')) val = c - __T('A') + 10;
      else break;

      val <<= 4;
      c = *bufp++;
      if (_istdigit(c)) val |= c - __T('0');
      else if (c >= __T('a') && c <= __T('f')) val |= c - __T('a') + 10;
      else if (c >= __T('A') && c <= __T('F')) val |= c - __T('A') + 10;
      else break;

      eth[i] = (unsigned char) (val & 0377);
      if (++i == ATH_MAC_LEN) {
         return 0;
      }
      if (*bufp != ':')
         break;
   }
   return(-1);
}

int
tx ( _TCHAR* argv[], int argc, int i )
{
   DWORD dwReturnedBytes = 0;
   TCMD_CONT_TX *txCmd = (TCMD_CONT_TX *)NdisCmd->Data;

   txCmd->testCmdId = TCMD_CONT_TX_ID;

   if ( _tcscmp(argv[i],OPT_TX_MODE_SINE) == 0 ) {
      txCmd->mode = TCMD_CONT_TX_SINE;
   } else if ( _tcscmp(argv[i],OPT_TX_MODE_FRAME) == 0 ) {
      txCmd->mode = TCMD_CONT_TX_FRAME;
   } else if ( _tcscmp(argv[i],OPT_TX_MODE_TX99) == 0 ) {
      txCmd->mode = TCMD_CONT_TX_TX99;
   } else if ( _tcscmp(argv[i],OPT_TX_MODE_TX100) == 0 ) {
      txCmd->mode = TCMD_CONT_TX_TX100;
   } else if ( _tcscmp(argv[i],OPT_TX_MODE_OFF) == 0 ) {
      txCmd->mode = TCMD_CONT_TX_OFF;
   } else {
      PrintUsage();
      return -1;
   }

   for ( ++i; i < argc; ++i ) {
      if ( _tcscmp(argv[i], OPT_TX_FREQ) == 0 ) {
         if ( (txCmd->freq = freqValid(_tcstoul(argv[++i],NULL,0))) == -1)
            return -1;
         continue;
      }

      if ( _tcscmp(argv[i], OPT_TX_RATE) == 0 ) {
         if ( (txCmd->dataRate = rateValid(_tcstoul(argv[++i],NULL,0),
                   txCmd->freq)) == -1)
            return -1;
         continue;
      }

      if ( _tcscmp(argv[i], OPT_TX_POWER) == 0 ) {
         if ( (txCmd->txPwr = txPwrValid(_tcstoul(argv[++i],NULL,0),
                  txCmd->mode)) == -1)
            return -1;
         continue;
      }

      if ( _tcscmp(argv[i], OPT_TX_ANT) == 0 ) {
         if ( (txCmd->antenna = antValid(_tcstoul(argv[++i],NULL,0))) == -1)
            return -1;
         continue;
      }

      if ( _tcscmp(argv[i], OPT_TX_PKTSZ) == 0 ) {
         if ( (txCmd->pktSz = pktSzValid(_tcstoul(argv[++i],NULL,0))) == -1)
            return -1;
         continue;
      }

      if ( _tcscmp(argv[i], OPT_TX_PAT) == 0 ) {
         if ( (txCmd->txPattern = (A_UINT16)_tcstoul(argv[++i],NULL,0)) == -1)
            return -1;
         continue;
      }

      if ( _tcscmp(argv[i], OPT_TX_AIFSN) == 0 ) {
         txCmd->aifsn = _tcstoul(argv[++i],NULL,0) & 0xff;
         printf("AIFS:%d\n",txCmd->aifsn);
      }

      if ( _tcscmp(argv[i], OPT_TX_ANI) == 0 ) {
         txCmd->enANI = TRUE;
      }

      if ( _tcscmp(argv[i], OPT_TX_SCRAMBLEOFF) == 0 ) {
         txCmd->scramblerOff = TRUE;
      }
   }

   NdisCmd->Oid = OID_CAR6K_TCMD_CONT_TX;
   NdisCmd->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + sizeof(TCHAR));
   if (NdisCmd->ptcDeviceName == NULL) {
       ERR("malloc failed,...");
       return -1;
   }
   _tcscpy(NdisCmd->ptcDeviceName, devName);

   if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, NdisCmd,
                         sizeof(NDISUIO_SET_OID) + sizeof(TCMD_CONT_TX),
                         NULL, 0, &dwReturnedBytes, NULL ) ) {
      ERR("Issueing TCMD Tx,...");
      free( NdisCmd->ptcDeviceName);
      return -1;
   }
   free( NdisCmd->ptcDeviceName);

   return 0;
}

int
setmac ( _TCHAR* argv[], int argc, int i )
{
   DWORD dwReturnedBytes = 0;
   TCMD_CONT_RX *rxCmd = (TCMD_CONT_RX *)NdisCmd->Data;
   A_UINT8 mac[ATH_MAC_LEN];

   rxCmd->testCmdId = TCMD_CONT_RX_ID;
   rxCmd->act = TCMD_CONT_RX_SETMAC;
   if (wmic_ether_aton(argv[i], mac) == -1) {
      printf("Invalid mac address format! \n");
      return -1;
   }
   memcpy(rxCmd->u.mac.addr, mac, ATH_MAC_LEN);
   printf("tcmd: setmac 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
                        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

   NdisCmd->Oid = OID_CAR6K_TCMD_CONT_RX;
   NdisCmd->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + sizeof(TCHAR));
   if (NdisCmd->ptcDeviceName == NULL) {
      ERR("malloc failed,...");
      return -1;
   }
   _tcscpy(NdisCmd->ptcDeviceName, devName);

   if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, NdisCmd,
                         sizeof(NDISUIO_SET_OID) + sizeof(TCMD_CONT_RX),
                         NULL, 0, &dwReturnedBytes, NULL ) ) {
      ERR("Issueing TCMD Rx,(setmac)...");
      free( NdisCmd->ptcDeviceName);
      return -1;
   }
   free( NdisCmd->ptcDeviceName);
   return 0;
}

int
rx ( _TCHAR* argv[], int argc, int i )
{
   DWORD dwReturnedBytes = 0;
   TCMD_CONT_RX *rxCmd = (TCMD_CONT_RX *)NdisCmd->Data;

   rxCmd->testCmdId = TCMD_CONT_RX_ID;

   if (_tcscmp(argv[i],OPT_RX_ACT_PROMIS) == 0) {
        rxCmd->act = TCMD_CONT_RX_PROMIS;
   } else if (_tcscmp(argv[i],OPT_RX_ACT_FILTER) == 0 ) {
        rxCmd->act = TCMD_CONT_RX_FILTER;
   } else if (_tcscmp(argv[i], OPT_RX_ACT_REPORT) == 0 ) {
        rxCmd->act = TCMD_CONT_RX_REPORT;
        needRxReport = TRUE;
   } else {
      PrintUsage();
      return -1;
   }
   

   for ( ++i; i < argc; ++i ) {
      if (_tcscmp(argv[i],OPT_RX_FREQ) == 0 ) {
         if ( (rxCmd->u.para.freq = freqValid(_tcstoul(argv[++i],NULL,0)))
                 == -1)
            return -1;
      }

      if (_tcscmp(argv[i],OPT_RX_ANT) == 0 ) {
         if ( (rxCmd->u.para.antenna = antValid(_tcstoul(argv[++i],NULL,0)))
              == -1)
            return -1;
      }

      if ( _tcscmp(argv[i], OPT_RX_ANI) == 0 ) {
         rxCmd->enANI = TRUE;
      }
   }

   if (rxCmd->act == TCMD_CONT_RX_PROMIS || rxCmd->act == TCMD_CONT_RX_FILTER) {
      if (rxCmd->u.para.freq == 0)
         rxCmd->u.para.freq = 2412;
   }

   NdisCmd->Oid = OID_CAR6K_TCMD_CONT_RX;
   NdisCmd->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + sizeof(TCHAR));
   if (NdisCmd->ptcDeviceName == NULL) {
       ERR("malloc failed,...");
       return -1;
   }
   _tcscpy(NdisCmd->ptcDeviceName, devName);

   if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, NdisCmd,
                         sizeof(NDISUIO_SET_OID) + sizeof(TCMD_CONT_RX),
                         NULL, 0, &dwReturnedBytes, NULL ) ) {
      ERR("Issueing TCMD Rx,...");
      free( NdisCmd->ptcDeviceName);
      return -1;
   }
   free( NdisCmd->ptcDeviceName);

   if (needRxReport) {
      A_UINT32 pkt = rxCmd->u.report.totalPkt;
      A_UINT32 crcError = rxCmd->u.report.crcErrPkt;
      A_UINT32 secErr = rxCmd->u.report.secErrPkt;
      A_INT32 rssi = rxCmd->u.report.rssiInDBm;

      printf("total pkt %d ; crcError pkt %d ; secErr pkt %d ; average rssi %d\n", pkt, crcError, secErr, (A_INT32)( pkt ? (rssi / (A_INT32)pkt) : 0));
   }

   return 0;
}

int
pm ( _TCHAR* argv[], int argc, int i )
{
   DWORD dwReturnedBytes = 0;
   TCMD_PM *pmCmd = (TCMD_PM *)NdisCmd->Data;

   pmCmd->testCmdId = TCMD_PM_ID;

   if (_tcscmp(argv[i], OPT_PM_WAKEUP) == 0 ) {
      pmCmd->mode = TCMD_PM_WAKEUP;
   } else if (_tcscmp(argv[i], OPT_PM_SLEEP) == 0 ) {
      pmCmd->mode = TCMD_PM_SLEEP;
   } else {
      PrintUsage();
      return -1;
   }

   NdisCmd->Oid = OID_CAR6K_TCMD_PM;
   NdisCmd->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + sizeof(TCHAR));
   if (NdisCmd->ptcDeviceName == NULL){
       ERR("malloc failed,...");
       return -1;
   }
   _tcscpy(NdisCmd->ptcDeviceName, devName);

   if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, NdisCmd,
                         sizeof(NDISUIO_SET_OID) + sizeof(TCMD_PM),
                         NULL, 0, &dwReturnedBytes, NULL ) ) {
      ERR("Issueing TCMD PM,...");
      free( NdisCmd->ptcDeviceName);
      return -1;
   }
   free( NdisCmd->ptcDeviceName);

   return 0;
}

int
setantswitchtable ( _TCHAR* argv[], int argc, int i )
{
   DWORD dwReturnedBytes = 0;
   TCMD_CONT_RX *rxCmd = (TCMD_CONT_RX *)NdisCmd->Data;

   rxCmd->testCmdId = TCMD_CONT_RX_ID;
   rxCmd->act = TCMD_CONT_RX_SET_ANT_SWITCH_TABLE;
   rxCmd->u.antswitchtable.antswitch1 = _tcstoul(argv[i],NULL,0);
   rxCmd->u.antswitchtable.antswitch2 = _tcstoul(argv[i+1],NULL,0);

   NdisCmd->Oid = OID_CAR6K_TCMD_CONT_RX;
   NdisCmd->ptcDeviceName = (PTCHAR)malloc(_tcslen(devName) * sizeof(TCHAR) + sizeof(TCHAR));
   if (NdisCmd->ptcDeviceName == NULL) {
       ERR("malloc failed,...");
       return -1;
   }
   _tcscpy(NdisCmd->ptcDeviceName, devName);

   if (! DeviceIoControl( hAdapter, IOCTL_NDISUIO_SET_OID_VALUE, NdisCmd,
                         sizeof(NDISUIO_SET_OID) + sizeof(TCMD_CONT_RX),
                         NULL, 0, &dwReturnedBytes, NULL ) ) {
      ERR("Issueing TCMD Rx,...");
      free( NdisCmd->ptcDeviceName);
      return -1;
   }
   free( NdisCmd->ptcDeviceName);

   return 0;
}

int
_tmain (
       int argc,
       _TCHAR *argv[]
       )
{
   int i=1, ret=0;
   DWORD dwReturnedBytes = 0;

   if ( argc < 2 ) {
      PrintUsage();
      return 0;
   }

   if ( _tcscmp( argv[i], OPT_I_DEVICE ) == 0 ) {
      i++;
      _tcsncpy( devName, argv[i++], 127);
      devName[127] = __T('\0');
     if(argc <= i)
     {
          PrintUsage();
          return 0;
     }
   } else if ( _tcscmp( argv[i], OPT_HELP_CMD ) == 0 ) {
      PrintUsage();
      return 0;
   } else {
      PrintUsage();
      return -1;
   }

   // Create the NDISUIO handle. This handle is not associated with a
   // device now. The device will be specified when an IOCTL is sent
   // to a device.

   hAdapter = CreateFile( NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                          INVALID_HANDLE_VALUE );

   printf("Handle: %p\n",(hAdapter));

   // Associate the handle with the adapter
   if ( !DeviceIoControl( hAdapter, IOCTL_NDISUIO_OPEN_DEVICE, devName,
                          _tcslen(devName) * sizeof(TCHAR), NULL, 0,
                        &dwReturnedBytes, NULL ) ) {
      ERR("IOCTL_NDISUIO_OPEN_DEVICE failed, continuing..");
   }

   if ( _tcscmp( argv[i], OPT_TX_CMD ) == 0 ) {
      ret = tx( argv, argc, ++i );
   } else if ( _tcscmp( argv[i], OPT_RX_CMD ) == 0 ) {
      ret = rx( argv, argc, ++i );
   } else if ( _tcscmp( argv[i], OPT_PM_CMD ) == 0 ) {
      ret = pm( argv, argc, ++i );
   } else if ( _tcscmp( argv[i], OPT_SETMAC_CMD ) == 0 ) {
      ret = setmac( argv, argc, ++i );
   } else if (_tcscmp(argv[i], OPT_ACT_ANT_SWITCH) == 0 ) {
       ret = setantswitchtable( argv, argc, ++i);
   } else {
      PrintUsage();
      ret = -1;
   }

   CloseHandle(hAdapter);

   return ret;
}

