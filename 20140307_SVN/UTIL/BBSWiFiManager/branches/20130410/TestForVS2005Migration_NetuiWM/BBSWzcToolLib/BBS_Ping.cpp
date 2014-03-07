#include "StdAfx.h"
#include "BBS_Ping.h"


CBBS_Ping::CBBS_Ping(void)
{
	memset( m_pcDestIPAddr, 0, sizeof(m_pcDestIPAddr) );
	memset( &m_wsaData, 0, sizeof(WSADATA) );
	memset( &m_sockAddress, 0, sizeof(SOCKADDR_STORAGE) );
	memset( &m_sendOption, 0, sizeof(IP_OPTION_INFORMATION) );
	memset( &m_pcLiteralIP, 0, sizeof(m_pcLiteralIP) );
	

	m_pucSendBuffer = NULL;
	m_pucRcvBuffer = NULL;
	m_dwSendSize = 0;
	m_dwRecvSize = 0;
	m_ucTtl = 0;
	m_ucTos = 0;
	m_dwTimeOut = 0;
	
	m_sockAddressLen = 0;

	m_hIcmp = NULL;

}

CBBS_Ping::~CBBS_Ping(void)
{
	DeinitPing();
}


BOOL CBBS_Ping::InitPing(BBS_PING_OPTION pingOption, LPTSTR strLiteralIP)
{
	memset( &m_wsaData, 0, sizeof(WSADATA) );


	m_dwSendSize = pingOption.dwSendBufSize;
	m_ucTtl = pingOption.ucTtl;
	m_ucTos = pingOption.ucTos;
	m_dwTimeOut = pingOption.dwTimeOut;
	


	// Ask fr Winsock version 2.2 (currently getaddrinfo() etc only exported from WS2)

	if (WSAStartup(MAKEWORD(2,2), &m_wsaData) != 0 )
	{
		RETAILMSG(1, (TEXT("[BBS_PING] WSAStartup Failed \r\n")));
		goto error;
	}


	if(! ResolveTarget( pingOption.tszDestIP, (LPSOCKADDR)&m_sockAddress, &m_sockAddressLen ) )
	{	
		RETAILMSG(1, (TEXT("[BBS_PING] ResolveTarget Failed \r\n")));
        goto error;
	}

	m_pucSendBuffer = (UCHAR*)LocalAlloc(LMEM_FIXED, m_dwSendSize);

	if (m_pucSendBuffer == NULL)
	{
		RETAILMSG(1, (TEXT("[BBS_PING] No memory(SendBuffer) \r\n")));
        goto error;
    }

	//
    // Calculate receive buffer size and try to allocate it.
    //
    if (m_dwSendSize <= DEFAULT_SEND_SIZE)
	{
        m_dwRecvSize = DEFAULT_BUFFER_SIZE;
    }
	else
	{
        m_dwRecvSize = MAX_BUFFER_SIZE;
    }


	m_pucRcvBuffer = (UCHAR*)LocalAlloc(LMEM_FIXED, m_dwRecvSize);

	if (m_pucRcvBuffer == NULL)
	{
		RETAILMSG(1, (TEXT("[BBS_PING] No memory(RecvBuffer) \r\n")));
        LocalFree(m_pucSendBuffer);
        goto error;
    }


	m_hIcmp = IcmpCreateFile();

	if( m_hIcmp == INVALID_HANDLE_VALUE)
	{
		RETAILMSG(1, (TEXT("[BBS_PING] IcmpCreateFile Failed \r\n")));
		goto error;
	}


	//
    // Initialize the send buffer pattern.
    //
    for (UINT i = 0; i < m_dwSendSize; i++) {
        m_pucSendBuffer[i] = 'a' + (i % 23);
    }


	m_sendOption.OptionsData = NULL;
	m_sendOption.OptionsSize = 0;
	m_sendOption.Ttl = m_ucTtl;
	m_sendOption.Tos = m_ucTos;
	m_sendOption.Flags = 0;



	if( getnameinfo((LPSOCKADDR)&m_sockAddress, m_sockAddressLen, m_pcLiteralIP, sizeof(m_pcLiteralIP), NULL, 0, NI_NUMERICHOST) != 0 )
	{
		RETAILMSG(1, (TEXT("[BBS_PING] IcmpCreateFile Failed \r\n")));
		goto error;
	}

	mbstowcs( strLiteralIP, m_pcLiteralIP,sizeof(m_pcLiteralIP) );

	return TRUE;

error:
	DeinitPing();
	return FALSE;
}

void CBBS_Ping::DeinitPing()
{
	if(m_pucSendBuffer)
	{
		LocalFree(m_pucSendBuffer);
	}

	if(m_pucRcvBuffer)
	{
		LocalFree(m_pucRcvBuffer);
	}

	if(m_hIcmp)
	{
		CloseHandle(m_hIcmp);
	}

	WSACleanup();
}


BOOL CBBS_Ping::StartPing(PBBS_PING_RESULT pPingResult)
{
	DWORD dwNumOfReplies = 0;
	PICMP_ECHO_REPLY   reply4;

	memset( pPingResult, 0, sizeof(BBS_PING_RESULT) );

	dwNumOfReplies = IcmpSendEcho(m_hIcmp, ((LPSOCKADDR_IN)&m_sockAddress)->sin_addr.s_addr, m_pucSendBuffer, (WORD)m_dwSendSize, &m_sendOption, m_pucRcvBuffer, m_dwRecvSize, m_dwTimeOut);

	if (dwNumOfReplies <= 0)
	{
		pPingResult->dwErrCode = GetLastError();
		RETAILMSG(1, (TEXT("[BBS_PING] IcmpSendEcho Failed (%d) \r\n"), pPingResult->dwErrCode ));
		return FALSE;
	}
	else
	{

		reply4 = (PICMP_ECHO_REPLY)m_pucRcvBuffer;

		while (dwNumOfReplies--)
		{

			if (reply4->Status == IP_SUCCESS)
			{

				pPingResult->dwDataSize = reply4->DataSize;

				if (reply4->RoundTripTime)
				{
					pPingResult->dwTime = reply4->RoundTripTime;
				}
				else
				{
					pPingResult->dwTime = 1;
				}

				pPingResult->ucTtl = reply4->Options.Ttl;
			}
			else
			{
				pPingResult->dwErrCode = reply4->Status;
				RETAILMSG(1, (TEXT("[BBS_PING] Ping Test Failed (%d) \r\n"), pPingResult->dwErrCode ));
				return FALSE;
			}

		}
	}


	return TRUE;
}

BOOL CBBS_Ping::ResolveTarget( wchar_t *wTargetString, SOCKADDR *TargetAddress, socklen_t *TargetAddressLen )
{
    int              i;
    addrinfo  hints, *ai;
	char	szDestIP[INET_ADDRSTRLEN]="";


    if (0 == WideCharToMultiByte(CP_ACP, 0, wTargetString, -1, szDestIP, sizeof(szDestIP), NULL, NULL))
	{
        return FALSE;
	}


    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags = AI_NUMERICHOST;
    i = getaddrinfo(szDestIP, "", &hints, &ai);

    if(i == NO_ERROR)
    {
        *TargetAddressLen = ai->ai_addrlen;
        memcpy(TargetAddress, ai->ai_addr, ai->ai_addrlen);

        freeaddrinfo(ai);
        return(TRUE);
    }
    else
    {
        hints.ai_flags = AI_CANONNAME;

        if (getaddrinfo(szDestIP, NULL, &hints, &ai) == 0)
        {
            *TargetAddressLen = ai->ai_addrlen;
            memcpy(TargetAddress, ai->ai_addr, ai->ai_addrlen);
            return(TRUE);
        }
    }

    return(FALSE);

} // ResolveTarget
