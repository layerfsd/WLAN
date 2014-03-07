//------------------------------------------------------------------------------
// <copyright file="art.c" company="Atheros">
//    Copyright (c) 2008 Atheros Corporation.  All rights reserved.
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
// ART
//
// Author(s): ="Atheros"
//==============================================================================

#include <windows.h>
#include <Winsock2.h>

#include "hif.h"
#include "bmi.h"
#include "athdrv_wince.h"
#include "htc_api.h"
#include "htc_raw_drv.h"
#include "common_drv.h"
#include "wince_common_drv.h"
#include "art.h"

#ifdef WINCE_ART

HTC_HANDLE                         art_pHTCTarget;
HIF_DEVICE                         *art_pHIFDevice;
A_UINT32                           art_TargetId;
A_UINT32                           art_TargetType;
struct {
    HTC_HANDLE      htcTarget;
    HIF_DEVICE      *hifDevice;
    A_UINT32        targetType;
    A_UINT32        targetId;
} stream_drv_t;

static HANDLE hAdapter;
char line[LINE_ARRAY_SIZE];
char DRGOperation[DRG_Write_size+sizeof(HTC_RAW_READ_PARAM)];
struct sockaddr_in	serv_addr;
SOCKET	sockfd, clientSocket;
unsigned short artBMIDone = 0;


A_INT32 socketListen()
{
	A_INT32 	res;
	A_INT32 	i;
	A_INT32 	j;
	
	WORD   wVersionRequested;
	WSADATA wsaData;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	res = WSAStartup( wVersionRequested, &wsaData );
	if ( res != 0 )
	{
		RETAILMSG (1,(L"WSAStartup ERROR\n"));
		return -1;
	}
	
	sockfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (sockfd == -1)
	{
		RETAILMSG (1,(L"socket(PF_INET,SOCK_STREAM,IPPROTO_TCP) ERROR\n"));
		WSACleanup();
		return -1;
	}
	
	// Allow immediate reuse of port
	i = 1;
	j = sizeof(i);
	res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (A_INT8 *)&i, j);
	if (res == -1)
	{
		RETAILMSG (1,(L"setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (A_INT8 *)&i, j) ERROR\n"));
		WSACleanup();
		return -1;
	}
	
	i = 1;
	j = sizeof(i);
	res = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (A_INT8 *)&i, j);
	if (res == -1)
	{
		RETAILMSG (1,(L"setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (A_INT8 *)&i, j) ERROR\n"));
		WSACleanup();
		return -1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
	serv_addr.sin_port = htons(ART_PORT);
	res = bind(sockfd, (const struct sockaddr FAR *) &serv_addr, sizeof(serv_addr)); 
	if (res == -1)
	{
		RETAILMSG (1,(L"bind ERROR\n"));
		return -1; 
	}
	res = listen(sockfd, 4);
	if (res == -1)
	{
		RETAILMSG (1,(L"listen ERROR\n"));
		WSACleanup();
		return -1; 
	}
	
	RETAILMSG (1,(L"**** WAITING FOR CLENT TO ACCEPT ****\n"));
	i = sizeof(struct sockaddr_in);
	clientSocket = accept(sockfd, (struct sockaddr *) &serv_addr, &i);
	if (clientSocket == -1)
	{
		RETAILMSG (1,(L"accept ERROR"));
		WSACleanup( );
		return -1;
	}
	RETAILMSG (1,(L"**** Client Connected ****\n"));
	
	return sockfd;
	
}

void ART_HTC_RAW_OPEN()
{                
	A_STATUS status = A_ERROR;

	// set htc block size
	status = ar6000_set_htc_params (art_pHIFDevice, art_TargetType, 0, 0);
	if (status == A_OK )
	{
		status = BMIDone (art_pHIFDevice);
	}
	if (status == A_OK )
	{
		if (ar6000_htc_raw_open(art_pHTCTarget) == -1) {
		status = A_ERROR;
		}
	}
	
}


void ART_htc_raw_read(HTC_RAW_READ_PARAM *buffer) 
{
	if (ar6000_htc_raw_read(art_pHTCTarget, (HTC_RAW_STREAM_ID)buffer->endPointId, 
		buffer->buffer, buffer->length) == -1 ) {
		RETAILMSG (1,(L"ERROR !!! ART_htc_raw_read\n"));	
	}
}

void	ART_htc_raw_write(HTC_RAW_STREAM_ID StreamID, char *buffer, A_INT32 length)
{
	if ( ar6000_htc_raw_write(art_pHTCTarget, StreamID, buffer, length) == -1) {
		RETAILMSG (1,(L"ERROR !!! ART_htc_raw_write\n"));	
	}
}

void ART_SocketSend(char *buf, int length)
{
	int sendbytes =0;	
	if ((sendbytes = send(clientSocket, buf, length, 0)) < 0) {
		RETAILMSG (1,(L"cannot send data\n"));
		closesocket(clientSocket);
		exit(1);
	}
//	RETAILMSG (1,(L"ART_SocketSend %d\n",sendbytes));
}

DWORD ART_Thread (PVOID Context)
{
	HTC_RAW_READ_PARAM *art_HTC_RAW_READ;
	int recvbytes=0;
	int chunkLen = 0;
	DWORD readLength = 0;
	unsigned char	*bufpos;  
	int reducedARTPacket = 1;

	ART_HTC_RAW_OPEN();
	
	if (-1 == socketListen())
	{
		RETAILMSG (1,(L"unable to create socket\n"));
		exit(1);
	}
	
	if ((recvbytes = recv(clientSocket, line, LINE_ARRAY_SIZE, 0)) < 0) {
		RETAILMSG (1,(L"FAILED: IO Problem\n"));
		return -1;
	}
	if (1 == (*(unsigned int *)line))
	{
		reducedARTPacket = 1;
		RETAILMSG (1,(L"ART Packet size (reduced)\n"));
	}
	else
	{
		reducedARTPacket = 0;
		RETAILMSG (1,(L"ART Packet size (normal)\n"));
	}
	ART_SocketSend(&(line[0]), 1);
	
	while(1)
	{	
		if ((recvbytes = recv(clientSocket, line, LINE_ARRAY_SIZE, 0)) < 0) {
			RETAILMSG (1,(L"FAILED: IO Problem\n"));
			return -1;
		}
//		RETAILMSG (1,(L"ART_Receive %d\n",recvbytes));

		if (!reducedARTPacket)
		{
		bufpos = line;
		while (recvbytes)
		{
			if (recvbytes > DRG_Write_size)
			{
				chunkLen = DRG_Write_size;
			}
			else
			{
				chunkLen = recvbytes;
			}

			ART_htc_raw_write(SEND_ENDPOINT,bufpos,chunkLen);
			
			recvbytes-=chunkLen;
			bufpos+=chunkLen;
		}
		}
		else
		{
			ART_htc_raw_write(SEND_ENDPOINT,line,recvbytes);
	
			while(DRG_Write_size == recvbytes)
			{
				ART_SocketSend(&(line[0]), 1);
				if ((recvbytes = recv(clientSocket, line, LINE_ARRAY_SIZE, 0)) < 0) {
					RETAILMSG (1,(L"FAILED: IO Problem\n"));
					return -1;
				}
				//RETAILMSG (1,(L"ART_Receive %d\n",recvbytes));
				if (0xbeef == *((A_UINT16 *)line))
				{
					// end marker
					break;
				}
				else
					ART_htc_raw_write(SEND_ENDPOINT,line,recvbytes);
			}
		}
				art_HTC_RAW_READ = (HTC_RAW_READ_PARAM *)line;
				art_HTC_RAW_READ->endPointId = SEND_ENDPOINT;
				art_HTC_RAW_READ->length = 4;

				ART_htc_raw_read(art_HTC_RAW_READ);
				memcpy(&readLength,&(art_HTC_RAW_READ->buffer),4);

				art_HTC_RAW_READ = (HTC_RAW_READ_PARAM *)line;
				art_HTC_RAW_READ->endPointId = SEND_ENDPOINT;
				art_HTC_RAW_READ->length = readLength;

				ART_htc_raw_read(art_HTC_RAW_READ);
		if ((REG_WRITE_CMD_ID != art_HTC_RAW_READ->buffer[0]) && (MEM_WRITE_CMD_ID != art_HTC_RAW_READ->buffer[0]) &&
			(M_PCI_WRITE_CMD_ID != art_HTC_RAW_READ->buffer[0]) && (M_PLL_PROGRAM_CMD_ID != art_HTC_RAW_READ->buffer[0]) &&
			(M_CREATE_DESC_CMD_ID != art_HTC_RAW_READ->buffer[0]))
				{
			ART_SocketSend(line+sizeof(DWORD)+sizeof(DWORD), art_HTC_RAW_READ->length);
				}
				else
				{
					ART_SocketSend(&(line[0]), 1);
				}
			}
		
	return 1;
}

void ART_STOP()
{
	closesocket(clientSocket);
	closesocket(sockfd);
	WSACleanup();
}

void ART_START()
{
	HANDLE 	hThread = NULL;
	A_STATUS status = A_ERROR;
	art_pHTCTarget = cmnGetHTCHandle();
	art_pHIFDevice = (HIF_DEVICE *)HTCGetHifDevice(art_pHTCTarget);
	cmnGetTargetInfo(&art_TargetType, &art_TargetId);


	stream_drv_t.hifDevice = art_pHIFDevice;
	stream_drv_t.htcTarget = art_pHTCTarget;
	stream_drv_t.targetId = art_TargetId;
	stream_drv_t.targetType = art_TargetType;

	status =  configure_ar6000(art_pHIFDevice,
							art_TargetType,
							art_TargetId,
							FALSE,
							FALSE,
							26000000,
							TEXT("\\device.bin"),
							TEXT("\\Windows"),
							FALSE,
							TRUE,
							TEXT("fakeBoardData_AR6002.bin"));
	if (status != A_OK )
	{
		RETAILMSG (1,(L"EROR !!! configure_ar6000\n"));
		return;
	}


	hThread = CreateThread (NULL, 0, ART_Thread, NULL, 0, NULL);
	if (hThread)
	{
		CeSetThreadPriority (hThread, 255);
		CloseHandle (hThread);
	}
	
}

#endif

//ART
