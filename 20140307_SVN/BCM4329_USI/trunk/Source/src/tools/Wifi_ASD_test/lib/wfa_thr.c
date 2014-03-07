
/****************************************************************************
 *  (c) Copyright 2007 Wi-Fi Alliance.  All Rights Reserved
 *
 *
 *  LICENSE
 *
 *  License is granted only to Wi-Fi Alliance members and designated
 *  contractors ($B!H(BAuthorized Licensees$B!I(B)..AN  Authorized Licensees are granted
 *  the non-exclusive, worldwide, limited right to use, copy, import, export
 *  and distribute this software:
 *  (i) solely for noncommercial applications and solely for testing Wi-Fi
 *  equipment; and
 *  (ii) solely for the purpose of embedding the software into Authorized
 *  Licensee$B!G(Bs proprietary equipment and software products for distribution to
 *  its customers under a license with at least the same restrictions as
 *  contained in this License, including, without limitation, the disclaimer of
 *  warranty and limitation of liability, below..AN  The distribution rights
 *  granted in clause
 *  (ii), above, include distribution to third party companies who will
 *  redistribute the Authorized Licensee$B!G(Bs product to their customers with or
 *  without such third party$B!G(Bs private label. Other than expressly granted
 *  herein, this License is not transferable or sublicensable, and it does not
 *  extend to and may not be used with non-Wi-Fi applications..AN  Wi-Fi Alliance
 *  reserves all rights not expressly granted herein..AN
 *.AN
 *  Except as specifically set forth above, commercial derivative works of
 *  this software or applications that use the Wi-Fi scripts generated by this
 *  software are NOT AUTHORIZED without specific prior written permission from
 *  Wi-Fi Alliance.
 *.AN
 *  Non-Commercial derivative works of this software for internal use are
 *  authorized and are limited by the same restrictions; provided, however,
 *  that the Authorized Licensee shall provide Wi-Fi Alliance with a copy of
 *  such derivative works under a perpetual, payment-free license to use,
 *  modify, and distribute such derivative works for purposes of testing Wi-Fi
 *  equipment.
 *.AN
 *  Neither the name of the author nor "Wi-Fi Alliance" may be used to endorse
 *  or promote products that are derived from or that use this software without
 *  specific prior written permission from Wi-Fi Alliance.
 *
 *  THIS SOFTWARE IS PROVIDED BY WI-FI ALLIANCE "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A.AN PARTICULAR PURPOSE,
 *  ARE DISCLAIMED. IN NO EVENT SHALL WI-FI ALLIANCE BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, THE COST OF PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. ******************************************************************************
 */
/*       Revision History:
 *       2006/11/10  -- initial created by qhu
 *       2007/02/15  -- WMM Extension Beta released by qhu, mkaroshi
 *       2007/03/30  -- 01.40 WPA2 and Official WMM Beta Release by qhu
 *       2007/04/20 -- 02.00 WPA2 and Official WMM Release by qhu
 *       2007/08/15 --  02.10 WMM-Power Save release by qhu
 *       2007/10/10 --  02.20 Voice SOHO beta -- qhu
 *       2007/11/07 --  02.30 Voice HSO -- qhu
 *
 */

/*
 * For MADWIFI driver, the TOS to 11E queue mapping as:
 *    0x08, 0x20            ----> WME_AC_BK;
 *    0x28, 0xa0            ----> WMC_AC_VI;
 *    0x30, 0xe0 0x88, 0xb8 ----> WME_AC_VO
 *      here 0x88 for UPSD, will be implemented later
 *    all other/default     ----> WME_AC_BE;
 */


#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#ifndef WIN32
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#endif
#include <wfa_debug.h>

#include "wfa_types.h"
#include "wfa_main.h"
#include "wfa_tg.h"
#include "wfa_debug.h"
#include "wfa_tlv.h"
#include "wfa_sock.h"
#include "wfa_rsp.h"
#include "wfa_wmmps.h"
#include "wfa_miscs.h"
#include "wfa_ca_resp.h"
/*
 * external global thread sync variables
 */
#ifdef WIN32
	HANDLE hThread3 =NULL;
	DWORD dwGenericThread3;
#endif
#ifdef WFA_WMM_EXT
extern tgWMM_t wmm_thr[];
#endif
extern tgStream_t *findStreamProfile(int id);
extern int gxcSockfd;
extern int wfaSetProcPriority(int);
extern tgStream_t *gStreams;
extern int wfa_defined_debug;
extern int IPTVprof;

#ifdef WFA_WMM_PS_EXT
extern int gtgWmmPS;
extern int psSockfd;
extern int **ac_seq;
extern wfaWmmPS_t wmmps_info;

extern void wfaSetDUTPwrMgmt(int mode);
extern void BUILD_APTS_MSG(int msg, unsigned long *txbuf);
extern void wmmps_wait_state_proc();

extern unsigned long psTxMsg[];
extern unsigned long psRxMsg[];
#endif /* WFA_WMM_PS_EXT */

extern void tmout_stop_send(int);
#ifdef WIN32
extern HANDLE ThrHandle;
extern void Win32_tmout_stop_send(LPVOID num ) ;
#endif

/*
 * wfaTGSetPrio(): This depends on the network interface card.
 *               So you might want to remap according to the driver
 *               provided.
 *               The current implementation is to set the TOS/DSCP bits
 *               in the IP header
 */
int wfaTGSetPrio(int sockfd, int tgClass)
{
	int tosval;

	socklen_t size = sizeof(tosval);
#ifndef WIN32
	getsockopt(sockfd, IPPROTO_IP, IP_TOS, &tosval, &size);
#else    
	getsockopt(sockfd, IPPROTO_IP, IP_TOS, (char FAR*)&tosval, &size);

#endif
	switch(tgClass)
	{
	case TG_WMM_AC_BK:
		//tosval = 0x08;
		tosval = 0x20;
		break;
	case TG_WMM_AC_VI:
		//tosval = 0x28;
		tosval = 0xA0;
		break;
	case TG_WMM_AC_UAPSD:
		tosval = 0x88;
		break;
	case TG_WMM_AC_VO:
		tosval = 0xE0;
		//       tosval = 0x30;
		//#       tosval = 0x88;
		break;
	case TG_WMM_AC_BE:
		tosval = 0x00;
	default:

		tosval = 0x00;

		/* default */
		;
	}
#ifndef WIN32
	if(setsockopt ( sockfd, IPPROTO_IP, IP_TOS, &tosval, sizeof(tosval)) != 0)
#else    
	if(setsockopt ( sockfd, IPPROTO_IP, IP_TOS, (char FAR*)&tosval, sizeof(tosval)) != 0)

#endif
	{
		DPRINT_ERR(WFA_ERR, "Failed to set IP_TOS\n");
	}
	return (tosval == 0xE0)?0xD8:tosval;
}

#ifdef WFA_WMM_EXT
/*
 * wfaSetThreadPrio():
 *    Set thread priorities
 *    It is an optional experiment if you decide not necessary.
 */
void wfaSetThreadPrio(int tid, int class)
{
	struct sched_param tschedParam;
	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	pthread_attr_setschedpolicy(&tattr, SCHED_RR);

	switch(class)
	{
	case TG_WMM_AC_BK:
		tschedParam.sched_priority = 70-3;
		break;
	case TG_WMM_AC_VI:
		tschedParam.sched_priority = 70-1;
		break;
	case TG_WMM_AC_VO:
		tschedParam.sched_priority = 70;
		break;
	case TG_WMM_AC_BE:
		tschedParam.sched_priority = 70-2;
	default:
		/* default */
		;
	}

	pthread_attr_setschedparam(&tattr, &tschedParam);
}

void * wfa_wmm_thread(void *thr_param)
{
	int myId = ((tgThrData_t *)thr_param)->tid;
	tgWMM_t *my_wmm = &wmm_thr[myId];
	tgStream_t *myStream = NULL;
	int myStreamId;
	int mySock = -1; 
	int status, respLen;
	tgProfile_t *myProfile;

	BYTE respBuf[WFA_BUFF_1K];
	pthread_attr_t tattr;

	pthread_attr_init(&tattr);
	pthread_attr_setschedpolicy(&tattr, SCHED_RR);
	while(1)
	{
		pthread_mutex_lock(&my_wmm->thr_flag_mutex);

		/* it needs to reset the thr_flag to wait again */
		while(!my_wmm->thr_flag)
		{
			/*
			* in normal situation, the signal indicates the thr_flag changed to
			* a valid number (stream id), then it will go out the loop and do
			* work.
			*/
			pthread_cond_wait(&my_wmm->thr_flag_cond, &my_wmm->thr_flag_mutex);
		}
		pthread_mutex_unlock(&my_wmm->thr_flag_mutex);

		myStreamId = my_wmm->thr_flag;
		/* use the flag as a stream id to file the profile */
		myStream = findStreamProfile(myStreamId);
		myProfile = &myStream->profile;

		if(myProfile == NULL)
		{
			status = STATUS_INVALID;
			wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, 4, (BYTE *)&status, respBuf);
			respLen = WFA_TLV_HDR_LEN+4;
			/*
			* send it back to control agent.
			*/
			continue;
		}

		switch(myProfile->direction)
		{
		case DIRECT_SEND:
			mySock = wfaCreateUDPSock(myProfile->sipaddr, myProfile->sport);
			mySock = wfaConnectUDPPeer(mySock, myProfile->dipaddr, myProfile->dport);
			/*
			* Set packet/socket priority TOS field
			*/
			wfaTGSetPrio(mySock, myProfile->trafficClass);

			/*
			* set a proper priority
			*/
			wfaSetThreadPrio(myId, myProfile->trafficClass);
#ifdef DEBUG
			printf("wfa_wmm_thread: myProfile->startdelay %i\n", myProfile->startdelay);
#endif
			/* if delay is too long, it must be something wrong */
			if(myProfile->startdelay > 0 && myProfile->startdelay < 50)
			{
				asd_sleep(myProfile->startdelay);
			}

			/*
			* set timer fire up
			*/
			signal(SIGALRM, tmout_stop_send);
			alarm(myProfile->duration);
			
			/* Whenever frameRate is 0,sending data at the maximum possible rate */	
			if (myProfile->rate != 0) 
				wfaSendLongFile(mySock, myStreamId, respBuf, &respLen );
			else
				wfaImprovePerfSendLongFile(mySock, myStreamId, respBuf, &respLen );

			memset(respBuf, 0, WFA_BUFF_1K);
			if (mySock != -1) {
				asd_closeSocket(mySock);
				mySock = -1;
			}

			/*
			* uses thread 0 to pack the items and ships it to CA.
			*/
			if(myId == 0) {
				//		wfaSentStatsResp(gxcSockfd, respBuf);
				printf("respBuf = %s    sent from thr\n", respBuf);
			}
			break;

		case DIRECT_RECV:
#ifdef WFA_WMM_PS_EXT
			/*
			* Test WMM-PS
			*/
			if(myProfile->profile == PROF_UAPSD)
			{
				/*
				* default timer value
				*/
				wmmps_info.nextsleep = WFA_DEFAULT_CODEC_SEC*1000000 + WFA_DEFAULT_CODEC_USEC;

				BUILD_APTS_MSG(APTS_HELLO, psTxMsg);
				wmmps_info.sta_state = 0;
				wmmps_info.wait_state = WFA_WAIT_STAUT_00;
				sendto(psSockfd, psTxMsg, 200+(wmmps_info.msgno%200), MSG_DONTWAIT,(struct sockaddr *) &wmmps_info.psToAddr, sizeof(wmmps_info.psToAddr));

				while(gtgWmmPS>0)
				{
					usleep(wmmps_info.nextsleep);
					wmmps_wait_state_proc(); // reset next sleep time
				}
			}
			break;
#endif /* WFA_WMM_PS_EXT */
		default:
			DPRINT_ERR(WFA_ERR, "Unknown covered case\n");
		}
	}
}

#endif