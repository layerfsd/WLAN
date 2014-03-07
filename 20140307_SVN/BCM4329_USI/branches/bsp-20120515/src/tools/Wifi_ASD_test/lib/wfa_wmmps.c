/****************************************************************************
 *  (c) Copyright 2007 Wi-Fi Alliance.  All Rights Reserved 
 *
 * 
 *  LICENSE
 *
 *  License is granted only to Wi-Fi Alliance members and designated 
 *  contractors (“Authorized Licensees”).  Authorized Licensees are granted 
 *  the non-exclusive, worldwide, limited right to use, copy, import, export 
 *  and distribute this software: 
 *  (i) solely for noncommercial applications and solely for testing Wi-Fi 
 *  equipment; and 
 *  (ii) solely for the purpose of embedding the software into Authorized 
 *  Licensee’s proprietary equipment and software products for distribution to 
 *  its customers under a license with at least the same restrictions as 
 *  contained in this License, including, without limitation, the disclaimer of 
 *  warranty and limitation of liability, below.  The distribution rights 
 *  granted in clause 
 *  (ii), above, include distribution to third party companies who will 
 *  redistribute the Authorized Licensee’s product to their customers with or 
 *  without such third party’s private label. Other than expressly granted 
 *  herein, this License is not transferable or sublicensable, and it does not 
 *  extend to and may not be used with non-Wi-Fi applications.  Wi-Fi Alliance 
 *  reserves all rights not expressly granted herein. 
 * 
 *  Except as specifically set forth above, commercial derivative works of 
 *  this software or applications that use the Wi-Fi scripts generated by this 
 *  software are NOT AUTHORIZED without specific prior written permission from 
 *  Wi-Fi Alliance.
 * 
 *  Non-Commercial derivative works of this software for internal use are 
 *  authorized and are limited by the same restrictions; provided, however, 
 *  that the Authorized Licensee shall provide Wi-Fi Alliance with a copy of 
 *  such derivative works under a perpetual, payment-free license to use, 
 *  modify, and distribute such derivative works for purposes of testing Wi-Fi 
 *  equipment.
 * 
 *  Neither the name of the author nor "Wi-Fi Alliance" may be used to endorse 
 *  or promote products that are derived from or that use this software without 
 *  specific prior written permission from Wi-Fi Alliance.
 *
 *  THIS SOFTWARE IS PROVIDED BY WI-FI ALLIANCE "AS IS" AND ANY EXPRESS OR 
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 *  OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE, 
 *  ARE DISCLAIMED. IN NO EVENT SHALL WI-FI ALLIANCE BE LIABLE FOR ANY DIRECT, 
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 *  (INCLUDING, BUT NOT LIMITED TO, THE COST OF PROCUREMENT OF SUBSTITUTE 
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) ARISING IN ANY WAY OUT OF 
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
 */

/*    Revision History:
 *    2007/07/25  -- Initial created by qhu
 *    2007/08/15  -- 02.10 WMM-Power Save Released by qhu
 *    2007/10/10 --  02.20 Voice SOHO beta -- qhu
 *    2007/11/07 --  02.30 Voice HSO -- qhu
 *
 */

#ifdef WFA_WMM_EXT
#ifdef WFA_WMM_PS_EXT
#ifndef WIN32
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#endif
#include <string.h>

#include "wfa_sock.h"
#include "wfa_types.h"
#include "wfa_tg.h"
#include "wfa_wmmps.h"
#include "wfa_miscs.h"
extern int psSockfd;

extern unsigned long psTxMsg[512];
extern unsigned long psRxMsg[512];

int gtgPsPktRecvd = 0;                    // need to reset
struct timeval time_ap;
struct timeval time_ul;

extern wfaWmmPS_t wmmps_info;
extern int gtgWmmPS;

extern int wfaTGSetPrio(int sockfd, int tgClass);

void wmmps_wait_state_proc();

struct apts_msg apts_msgs[] ={
   {0, -1, 0, 0, 0, },
   {"APTS TX         ", APTS_DEFAULT, 0, 0, 0, },
   {"APTS Hello      ", APTS_HELLO, 0, 0, 0, 0},
   {"APTS Hello Resp ", APTS_HELLO_RESP, 0, 0, 0, 0},
   {"APTS Confirm    ", APTS_CONFIRM, 0, 0, 0, 0},
   {"APTS STOP       ", APTS_STOP, 0, 0, 0, 0},
   {"APTS CK BE      ", APTS_CK_BE, 0, 0, 0, 0, },
   {"APTS CK BK      ", APTS_CK_BK, 0, 0, 0, 0, },
   {"APTS CK VI      ", APTS_CK_VI, 0, 0, 0, 0, },
   {"APTS CK VO      ", APTS_CK_VO, 0, 0, 0, 0, },
   {0, 10, 0, 0, 0, },		// APTS_TESTS
   {"B.D", B_D, 4, 1, 1, 0},	// 4 single packet exchanges
   {"B.2", B_2, -1, 1, 1, 0},	// continuous single packet exchanges
   {"B.H", B_H, 4, 1, 2, 0},	// 4 exchanges: 1 uplink, 2 downlink frames
   {"B.4", B_4, 4, 2, 1, 0},	// 4 exchanges: 2 uplink (trigger 2nd), 1 downlink frames
   { 0,    B_5, 4, 2, 1, 0},	// placeholder
   { 0,    B_6, 4, 2, 1, 0},	// placeholder
   {"B.B", B_B, 4, 1, 0, 0},	// 4 exchanges: 1 uplink, 0 downlink
   {"B.E", B_E, 4, 2, 0, 0},	// 4 exchanges: 2 uplink, 0 downlink
   {"B.G", B_G, 4, 2, 1, 0},	// 4 exchanges: 2 uplink, 1 downlink
   {"B.M", B_M, 4, 2, 1, 0},	// 
   {"M.D", M_D, 4, 1, 1, 0},	// 4 single packet exchanges (mixed legacy/U-APSD)
   {"M.G", M_G, 4, 2, 1, 0},	// 4 exchanges: 2 uplink, 1 downlink (mixed legacy/U-APSD)
   {"M.I", M_I, 4, 2, 2, 0},	// 4 exchanges: 2 uplink, 2 downlink (mixed legacy/U-APSD)
   {"B.Z", B_Z, 1, 1, 1, 0},	// 1 special exchange for Broadcast testing
   {"M.Y", M_Y, 4, 1, 1, 0},	// special exchange for Broadcast testing
   {"L.1", L_1, 3000, 3000, 3000, 20},	// bidirectional voip-like 60-second load test
   {"Downlink Load", DLOAD, 0, 0, 0, 0},	// label for downlink frames during load test
   {"Uplink Load", ULOAD, 0, 0, 0, 0},	// label for uplink frames during load test
   {"APTS PASS", APTS_PASS, 0, 0, 0, 0},
   {"APTS FAIL", APTS_FAIL, 0, 0, 0, 0},
   {"A.Y", A_Y, 5, 1, 1, 0},	// A_Y like M_Y, but staut retrieves frames in Active Mode
   {"B.W", B_W, 3, 1, 1, 0},	// 3 special exchange for Broadcast testing
   {"A.J", A_J, 1, 1, 4, 0},	// 1 exchanges: 1 uplink, 4 downlink frames
   {"M.V", M_V, 4, 1, 1, 0},	// 3 special exchange (but use to capture last one)
   {"M.U", M_U, 4, 1, 1, 0},	// 3 special exchange (but use to capture last one)
   {"A.U", A_U, 6, 1, 1, 0},	// 3 special exchange (but use to capture last one)
   {"M.L", M_L, 4, 1, 1, 0},	// 4 single packet exchanges
   {"B.K", B_K, 4, 1, 1, 0},	// 4 exchanges: 1 uplink, 0 downlink
   {"M.B", M_B, 4, 1, 0, 0},	// 4 exchanges: 1 uplink, 0 downlink
   {"M.K", M_K, 4, 1, 1, 0},	// 4 exchanges: 1 uplink, 0 downlink
   {"M.W", M_W, 3, 1, 1, 0},	// 3 special exchange for Broadcast testing
   {0, 0, 0, 0, 0, }		// APTS_LAST
};

int ac_seq[APTS_LAST][6] ={
   {0,      0,      0,      0,      0},
   {0},
   {0},
   {0},
   {0},
   {0},
   {0},
   {0},
   {0},
   {0},
   {0}, // APTS_TESTS
   {0}, // B.D
   {0}, // B.2
   {0}, // B.H
   {0}, // B.4
   {0}, // B_5
   {0, 0, 0, 0, 0}, // B_6
   {TG_WMM_AC_VO, TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_BK, 0}, // B.B B_B - 4 exchanges: 1 uplink, 0 downlink
   {0}, // B.E
   {0}, // B.G
   {0}, // B.I
   {0}, // M.D
   {0}, // M.G
   {0}, // M.I
   {0}, // B.Z  1, 1, 1, 0},	// 1 special exchange for Broadcast testing
   {TG_WMM_AC_VI, TG_WMM_AC_VO, TG_WMM_AC_BE, TG_WMM_AC_BE, 0}, //  M.Y  M_Y 2 special exchange for Broadcast testing
   {0}, // L.1
   {0}, // DLOAD
   {0}, // ULOAD
   {0}, // "APTS PASS"
   {0}, // "APTS FAIL"
   //{TOS_VI, TOS_VO, TOS_BE, TOS_BE, 0}, //  A.Y A_Y special exchange for Broadcast testing
   {TG_WMM_AC_VI, TG_WMM_AC_VO, TG_WMM_AC_BE, TG_WMM_AC_BE, TG_WMM_AC_BE}, //  A.Y A_Y special exchange for Broadcast testing
   {0}, //  B.W  2 special exchange for Broadcast testing
   {0}, //  A.J
   {TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_VI, TG_WMM_AC_VI, TG_WMM_AC_VI}, //  M.V M_V
   {TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_VO, TG_WMM_AC_VO, TG_WMM_AC_VO}, //  M.U M_U
   {TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_BE, TG_WMM_AC_BE, TG_WMM_AC_VO, TG_WMM_AC_VO},  //  A.U A_U
   {0}, //  M.L M_L
   {TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_VI, TG_WMM_AC_VI, 0}, // B.K B_K
   {TG_WMM_AC_VO, TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_BK, 0}, // M.B M_B - 4 exchanges: 1 uplink, 0 downlink
   {TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_VI, TG_WMM_AC_VI, 0}, // M.K M_K
   {TG_WMM_AC_VI, TG_WMM_AC_BE, TG_WMM_AC_VI, TG_WMM_AC_VI, 0} //  M.W M_W   special exchange for Broadcast testing
};

void BUILD_APTS_MSG(int msg, unsigned long *txbuf)
{   
    struct apts_msg *t;

    t = &apts_msgs[msg];
    txbuf[0] = wmmps_info.msgno++;
    txbuf[1] = 0;
    txbuf[2] = 0;
    txbuf[3] = 0;
    txbuf[4] = 0;
    txbuf[5] = 0;
    txbuf[6] = t->param0;
    txbuf[7] = t->param1;
    txbuf[8] = t->param2;
    txbuf[9] = t->param3;
    txbuf[10] = t->cmd;
    strcpy((char *)&txbuf[11], t->name); 
}

void send_txmsg(int new_prio_class)
{
    int r;
    int new_dscp = 0;

    if(new_prio_class > -1)
       new_dscp = wfaTGSetPrio(psSockfd, new_prio_class);

    psTxMsg[0] = wmmps_info.msgno++;
    psTxMsg[1] = new_dscp;
    psTxMsg[2] = wmmps_info.my_group_cookie;
    psTxMsg[3] = wmmps_info.my_cookie;
    psTxMsg[4] = wmmps_info.my_sta_id;

    if(psTxMsg[10] == APTS_DEFAULT)
    {
        psTxMsg[13] = (wmmps_info.msgno%10) + 0x20202030; 
    }

    r = wfaTrafficSendTo(psSockfd, (char *)psTxMsg, 200+(wmmps_info.msgno%200), (struct sockaddr *) &wmmps_info.psToAddr);

    wmmps_info.nsent++;
}

/*
 * This needs to adopt to the specific platform you port to.
 */
void wfaSetDUTPwrMgmt(int mode)
{
   static int curr_mode = -1;

   if(curr_mode == mode)
   {
       return;
   }

   if(mode == PS_OFF)
   {
#ifndef WIN32
       if( system("iwpriv ath2 sleep 0") < 0)
#else
#endif
       {
       }
   }
   else
   {
   #ifndef WIN32
       if( system("iwpriv ath2 sleep 1") < 0)
#else
#endif
       {
       }
   }

   curr_mode = mode;
}

int wfaWmmPowerSaveProcess(int sockfd)
{
   int rbytes = 0;
   int msg;
   struct sockaddr from;
   int len;

   rbytes = recvfrom(sockfd, (char *)psRxMsg, MAX_UDP_LEN, 0, &from, (socklen_t *)&len);
   if(rbytes < 0)
   {
      //perror("receive error");
	   printf("resceive error");
      return rbytes;
   }

   if(wmmps_info.sta_state == 0)
   {
#ifndef WIN32 
       signal(SIGALRM, SIG_IGN);
#else
#endif
       wmmps_info.sta_test = psRxMsg[10];

       if(wmmps_info.sta_test == APTS_STOP)
       {
           gtgWmmPS = 0;
           asd_closeSocket(psSockfd);
           psSockfd = -1;
#ifndef WIN32
           signal(SIGALRM, SIG_IGN);
#endif

	   wfaSetDUTPwrMgmt(PS_OFF);
       }

       wmmps_info.wait_state = WFA_WAIT_STAUT_02;
       wmmps_info.nextsleep = 1000000;  // 1 sec 
       switch(wmmps_info.sta_test)
       {
           case APTS_HELLO_RESP:
	   break;
           case L_1:
               wmmps_info.my_sta_id = psRxMsg[9];
	       apts_msgs[APTS_CONFIRM].param3 = wmmps_info.my_sta_id;
	       break;
           case B_D:
	   break;
           case M_D:
	   break;
           case B_H:
	   break;
           case B_B:
	   break;
           case M_B:
	   break;
           case B_K:
	   break;
           case M_L:
	   break;
           case M_G:
	   break;
           case M_I:
	   break;
           case B_Z:
	   break;
           case M_Y:
	   break;
           case M_V:
	   break;
           case M_U:
	   break;
           case A_U:
	   break;
           case A_Y:
	   break;
           case B_W:
	   break;
           case M_W:
	   break;
           case A_J:
	   break;
           case B_M:
           break;
           default:
	   printf("unknow\n");
           return TRUE;
       }

       BUILD_APTS_MSG(APTS_CONFIRM, psTxMsg);
       rbytes = wfaTrafficSendTo(psSockfd, (char *)psTxMsg, sizeof(psTxMsg), (struct sockaddr *) &wmmps_info.psToAddr);

       wmmps_info.sta_state = 2;
   }

   wfaSetDUTPwrMgmt(PS_ON);

   if(wmmps_info.sta_test != B_M)
   {
       BUILD_APTS_MSG(APTS_CK_VO, psTxMsg);
       send_txmsg(TG_WMM_AC_VO);

       usleep(100000);
       BUILD_APTS_MSG(APTS_CK_VI, psTxMsg);
       send_txmsg(TG_WMM_AC_VI); 

       usleep(100000);
       BUILD_APTS_MSG(APTS_CK_BE, psTxMsg);
       send_txmsg(TG_WMM_AC_BE); 

       usleep(100000);
       BUILD_APTS_MSG(APTS_CK_BK, psTxMsg);
       send_txmsg(TG_WMM_AC_BK);
   }
   else // B_M test
   {
       asd_sleep(30);
       BUILD_APTS_MSG(APTS_CK_VI, psTxMsg);
       send_txmsg(TG_WMM_AC_VI);

       asd_sleep(1);

       // retrieve the STOP command and clean up 
       BUILD_APTS_MSG(APTS_CK_BE, psTxMsg);
       send_txmsg(TG_WMM_AC_BE);

       return TRUE;
   }

   if(wmmps_info.sta_state == 2 )
   {
       asd_sleep(1);

       msg = APTS_DEFAULT;
       wmmps_info.wait_state = WFA_WAIT_STAUT_02;
       wmmps_info.nextsleep = 1000000;  // 1 sec 
       switch(wmmps_info.sta_test)
       {
           case L_1:
           wmmps_info.nextsleep = 20000;
           msg = ULOAD;
           wmmps_info.wait_state = WFA_WAIT_STAUT_VOLOAD;

           case B_D:
           case M_D:
           case B_B:
           case B_K:
           case M_B:
           case M_K:
           case B_H:
           case B_W:
           case M_W:
           case M_L:
           case M_G:
           case M_I:
           case B_Z:
           case M_Y:
           case M_V:
           case M_U:
           case A_U:
           case A_Y:
           case A_J:
           if(wmmps_info.sta_test == B_E) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_0E;
	   if(wmmps_info.sta_test == B_G) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_0E;
	   if(wmmps_info.sta_test == M_G) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_0E;
	   if(wmmps_info.sta_test == M_I) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_0E;
	   if(wmmps_info.sta_test == B_B) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == B_K) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == M_B) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == M_K) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == M_U) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == M_V) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == M_W) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == M_Y) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == A_U) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;
	   if(wmmps_info.sta_test == A_Y) 
	       wmmps_info.wait_state = WFA_WAIT_STAUT_SEQ;

	   BUILD_APTS_MSG(msg, psTxMsg);
	   gtgPsPktRecvd = 0;
	   wmmps_info.nsent = 0;

	   if(wmmps_info.sta_test == A_J)
	   {
	       wmmps_info.nextsleep = 5*1000000;
	   }

	   if(wmmps_info.sta_test == M_W)
	   {
	       wmmps_info.nextsleep = 500000;
	   }

           wmmps_wait_state_proc();

	   if(wmmps_info.sta_test == A_J)
	   {
	       asd_sleep(1);
	       wfaSetDUTPwrMgmt(PS_OFF);
               wmmps_wait_state_proc();
	   }

	   wmmps_info.sta_state = 3;
       }
   }

   if(wmmps_info.sta_state == 3)
   {
       gtgPsPktRecvd++;
       wmmps_info.my_cookie = psRxMsg[0];

       if(psRxMsg[10] == APTS_STOP)
       {
           gtgWmmPS = 0;
           asd_closeSocket(psSockfd);
           psSockfd = -1;
#ifndef WIN32
           signal(SIGALRM, SIG_IGN);
#endif
	   wfaSetDUTPwrMgmt(PS_OFF);
       }
   }

   return TRUE;
}

void wmmps_wait_state_proc()
{
    int ttypes[4], i, rbytes;

    for(i = 0; i<4; i++)
    {
        ttypes[i] = 0xFF;
    }

    switch(wmmps_info.wait_state)
    {
        case WFA_WAIT_NEXT_CODEC:
        ttypes[0] = -1;
        wmmps_info.wait_state = WFA_WAIT_NEXT_CODEC;
        break;

	case WFA_WAIT_FOR_AP_RESPONSE:
	wmmps_info.wait_state = WFA_WAIT_NEXT_CODEC;
	break;

        case WFA_WAIT_STAUT_00:
	rbytes = sendto(psSockfd, psTxMsg, 200+(wmmps_info.msgno%200), MSG_DONTWAIT,(struct sockaddr *) &wmmps_info.psToAddr, sizeof(wmmps_info.psToAddr));
        wmmps_info.nextsleep = 1000000;   /* one second */
        break;

        case WFA_WAIT_STAUT_02:
        ttypes[0] = TOS_VO;
        wmmps_info.nextsleep = 1000000;  // 1 sec 
	return;
        break;

        case WFA_WAIT_STAUT_04:
        ttypes[0] = TOS_BE;
        ttypes[1] = TOS_VO;
        break;

        case WFA_WAIT_STAUT_0E:
        ttypes[0] = TOS_VO;
        ttypes[1] = TOS_VO;
        break;

        case WFA_WAIT_STAUT_VOLOAD:
        ttypes[0] = TOS_VO;
        break;

        case WFA_WAIT_STAUT_SEQ:
        if( wmmps_info.nsent < 0 || wmmps_info.nsent > sizeof(ac_seq[wmmps_info.sta_test]) )
            wmmps_info.nsent = 0;

        if(( wmmps_info.sta_test == A_Y && wmmps_info.nsent == 3)
           || (wmmps_info.sta_test == A_U && wmmps_info.nsent == 1)
           || (wmmps_info.sta_test == A_U && wmmps_info.nsent == 3)
           || (wmmps_info.sta_test == A_U && wmmps_info.nsent == 5))
        {
           wfaSetDUTPwrMgmt(PS_OFF);
           wmmps_info.nextsleep = 100000;   // 100 ms
        }

        if((wmmps_info.sta_test == A_Y && wmmps_info.nsent == 4)
          || (wmmps_info.sta_test == A_U && wmmps_info.nsent == 2)
	  || (wmmps_info.sta_test == A_U && wmmps_info.nsent == 4))
	{
	   wfaSetDUTPwrMgmt(PS_ON);
           wmmps_info.nextsleep = 100000;   // 100 ms
        }

        ttypes[0] = ac_seq[wmmps_info.sta_test][wmmps_info.nsent];
	printf("ttypes 0x%x\n", ttypes[0]);

        // do we need sample time for one sec?
        wmmps_info.nextsleep = 1000000;
        break;

        default:
          printf("unknow wait state\n");
    }

    i=0;
    while(ttypes[i] != 0xFF)
       send_txmsg(ttypes[i++]);
}
#endif /* WFA_WMM_PS_EXT */
#endif /* WFA_WMM_EXT    */