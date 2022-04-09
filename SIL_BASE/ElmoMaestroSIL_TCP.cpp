/*
 * ElmoMaestroSIL_TCP.cpp
 *
 *  Created on: 30/06/2019
 *      Author: zivb
 */
#include <iostream>
#include <stdio.h>
#include "MMC_Definitions.h"
#include "mmcpplib.h"
#define ELMO_TCP
#include "ElmoMaestroSIL_TCP.h"
#include "ElmoMaestroSIL_Lib.h"

using namespace std;

client_tcp_s_t g_iClientList[MAX_CLIENTS];
////////////////////////////////////////////////////////////////////
///**
// * \fn 		TCPServerRcvClbk ()
// * \brief	Callback function called when TCP data received.
// * \param	void
// * \return  void.
// */
/////////////////////////////////////////////////////////////////////
int TCPServerRcvClbk(int iSock, short sEventID, unsigned char* ucBuffer, int iRc,struct sockaddr_in* pSockaddr)
{

	switch (sEventID)
	{
        case MMCPP_SOCK_NEW_EVENT:
			fprintf(stderr, "%s: MMCPP_SOCK_NEW_EVENT %d\n", __func__, iSock);
			{
			struct sockaddr_in sin;
			socklen_t len = sizeof(sin);
			if (getsockname(iSock, (struct sockaddr *)&sin, &len) == -1)
				perror("getsockname");
			else
				printf("port number %d\n", ntohs(sin.sin_port));

			AddClient2Hlp(iSock,ntohs(sin.sin_port));
			}
			break;
		case MMCPP_SOCK_READY_EVENT:
		{
//			fprintf(stderr, "%s: MMCPP_SOCK_READY_EVENT %d iRc = %d\n", __func__, iSock,iRc);

//			struct sockaddr_in sin;
//			socklen_t len = sizeof(sin);
//			if (getsockname(iSock, (struct sockaddr *)&sin, &len) == -1)
//			    perror("getsockname");
//			else
//			    printf("port number %d\n", ntohs(sin.sin_port));
			MoveData2Hlp(iSock, ucBuffer,iRc);
		}
			break;
		case MMCPP_SOCK_CLOSED_EVENT:
		{
			fprintf(stderr, "%s: MMCPP_SOCK_CLOSED_EVENT %d\n", __func__, iSock);

			struct sockaddr_in sin;
			socklen_t len = sizeof(sin);
			if (getsockname(iSock, (struct sockaddr *)&sin, &len) == -1)
				perror("getsockname");
			else
				printf("port number %d\n", ntohs(sin.sin_port));

			RemoveClient2Hlp(iSock,ntohs(sin.sin_port));
		}
			break;
		case MMCPP_SOCK_ABORT_EVENT:
			fprintf(stderr, "%s: MMCPP_SOCK_ABORT_EVENT\n", __func__);
			break;
		case MMCPP_SOCK_MAXLIM_EVENT:
			fprintf(stderr, "%s: MMCPP_SOCK_MAXLIM_EVENT\n", __func__);
			break;
		default:
			fprintf(stderr, "%s Unhandled(unknown) response event. %d\n", __func__, sEventID);
			break;
	}
	return 0;
}


void AddClient2Hlp(int iSock,int port)
{
	for(int j = 0; j < MAX_TCP_CONN;j++)
	{
		if(g_TCPServerList[j]._dataMsg.usServerPort == port)
		{
			for(int i = 0; i < MAX_CLIENTS; i++ )
			{
				if (g_TCPServerList[j].stClientList[i]._iSock == 0) {
					g_TCPServerList[j].stClientList[i]._iSock=iSock;
					g_TCPServerList[j].stClientList[i]._bReadFlag = false;
//					fprintf(stderr, "%s i = %d j = %d:server port %d iSock = %d\n", __func__, i,j, g_TCPServerList[j]._dataMsg.usServerPort,iSock);
					break;
				}
			}
			break;
		}
	}
}

void RemoveClient2Hlp(int iSock, int port)
{
	for(int j = 0; j < MAX_TCP_CONN;j++)
	{
		if(g_TCPServerList[j]._dataMsg.usServerPort == port)
		{
			for(int i = 0; i < MAX_CLIENTS; i++ )
			{
				if (g_TCPServerList[j].stClientList[i]._iSock == iSock) {
					g_TCPServerList[j].stClientList[i]._iSock=0;
					g_TCPServerList[j].stClientList[i]._bReadFlag = false;
//					fprintf(stderr, "%s i = %d j = %d:server port %d iSock = %d\n", __func__, i,j, g_TCPServerList[j]._dataMsg.usServerPort,iSock);
					break;
				}
			}
			break;
		}
	}

}

void ClearReadFlag2Hlp(int iSockID)
{
	g_iClientList[iSockID]._bReadFlag = false;
}

void MoveData2Hlp(int iSock, unsigned char* ucBuffer,int iDataSize)
{

	for(int j = 0; j < MAX_TCP_CONN;j++)
	{
		for(int i = 0; i < MAX_CLIENTS; i++ )
		{
			if (g_TCPServerList[j].stClientList[i]._iSock == iSock)
			{
				g_TCPServerList[j].stClientList[i]._bReadFlag = true;
//				g_TCPServerList[j]._dataMsg._cBuffId ^= 1;
				//Copy the data to the global data
				memcpy(&(g_TCPServerList[j]._dataMsg._szDataRcv[g_TCPServerList[j]._dataMsg._cBuffId]), ucBuffer, g_TCPServerList[j]._dataMsg.uiLen);
				g_TCPServerList[j]._dataMsg.iTcpRcvDataSize[g_TCPServerList[i]._dataMsg._cBuffId] = iDataSize;

//				printf("iTcpRcvDataSize = %d\n",g_TCPServerList[j]._dataMsg.iTcpRcvDataSize[g_TCPServerList[i]._dataMsg._cBuffId]);

				g_TCPServerList[j]._iLastSockReceivedFrom = iSock;
				atomic_set_bits(TCP_DATA_READY,&(g_TCPServerList[j]._dataMsg.uiRcvDataReady));

//							memcpy(&g_TCPServerList[j].stClientList[i]._tcpMsg, ucBuffer, sizeof(tcppmsg_t));
//							fprintf(stderr, "%s %d: iSock = %d iDataSize = %d ucBuffer[0] = %s\n", __func__, j, iSock,iDataSize,ucBuffer);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////
/**
 * \fn 		TCPServerSendThreadFunc ()
 * \brief	This function send Server TCP message according to user request.
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * TCPServerSendThreadFunc (void *)
{
	int i = 0;
	int rc;
	while(!g_iTerminate)
	{

		sem_wait(&SemTCP);
		for( i = 0 ; i < MAX_TCP_CONN ;i++)
		{
			if(g_TCPServerList[i]._dataMsg.uiDataReady)
			{
//				printf("server sending data _iLastSockReceivedFrom = %d\n",g_TCPServerList[i]._iLastSockReceivedFrom);
				// "uiSendData" holds the actual size to send ...
				// g_cUDPClient[i].Send((char*)&g_saClientList[i]._dataMsg._szData, g_saClientList[i]._dataMsg.uiLen     ,NULL);
				//rc = g_cTCPServer[i].Send(g_TCPServerList[i]._iLastSockReceivedFrom, g_TCPServerList[i]._dataMsg.uiLen, (void *)g_TCPServerList[i]._dataMsg._szData/*(void *)g_TCPServerList[i]._dataMsg._szDataRcv[g_TCPServerList[i]._dataMsg._cBuffId]*/);
				//
				// Benji Commented 02.02.2020 - Size should be _dataMsg.uiDataReady and NOT _dataMsg.uiLen
				rc = g_cTCPServer[i].Send(g_TCPServerList[i]._iLastSockReceivedFrom, g_TCPServerList[i]._dataMsg.uiDataReady, (void *)g_TCPServerList[i]._dataMsg._szData/*(void *)g_TCPServerList[i]._dataMsg._szDataRcv[g_TCPServerList[i]._dataMsg._cBuffId]*/);
				g_TCPServerList[i]._dataMsg.uiDataReady = 0;
			}
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \fn 		tcp_serv_sendThread_init()
 * \brief	start a thread for sending TCP
 * \param	void
 * \return  void.
 */
void tcp_serv_sendThread_init()
{
	pthread_attr_t hTCPServSendThreadAttr;
	pthread_t hTCPServSendThread;

	if(pthread_attr_init(&hTCPServSendThreadAttr))
	{
		printf("pthread_attr_init failure\n");
	}
	if(pthread_attr_setdetachstate(&hTCPServSendThreadAttr, PTHREAD_CREATE_DETACHED))
	{
		printf("pthread_attr_setdetachstate failure\n");
	}
	printf("TCP Server Send Thread Create\n");
	if(pthread_create(&hTCPServSendThread, &hTCPServSendThreadAttr, TCPServerSendThreadFunc, NULL) != 0)
	{
		printf("Unable to create TCPServerSendThreadFunc thread\n");
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \fn 		tcp_client_sendThread_init()
 * \brief	start a thread for sending TCP client message
 * \param	void
 * \return  void.
 */
void tcp_client_sendThread_init()
{
	pthread_attr_t hTCPClientSendThreadAttr;
	pthread_t hTCPClientSendThread;

	if(pthread_attr_init(&hTCPClientSendThreadAttr))
	{
		printf("pthread_attr_init failure\n");
	}
	if(pthread_attr_setdetachstate(&hTCPClientSendThreadAttr, PTHREAD_CREATE_DETACHED))
	{
		printf("pthread_attr_setdetachstate failure\n");
	}
	printf("TCP Client Send Thread Create\n");
	if(pthread_create(&hTCPClientSendThread, &hTCPClientSendThreadAttr, TCPClientSendThreadFunc, NULL) != 0)
	{
		printf("Unable to create TCPClientSendThreadFunc thread\n");
	}

}


//////////////////////////////////////////////////////////////////
/**
 * \fn 		TCPClientSendThreadFunc ()
 * \brief	This function sends Client TCP message according to user request.
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * TCPClientSendThreadFunc (void *)
{
	int i = 0;
	while(!g_iTerminate)
	{

		sem_wait(&SemTCPClient);
		for( i = 0 ; i < MAX_TCP_CONN ;i++)
		{
			if(g_TCPClientList[i]._dataMsg.uiDataReady)
			{
				// "uiSendData" holds the actual size to send ...
				// g_cUDPClient[i].Send((char*)&g_saClientList[i]._dataMsg._szData, g_saClientList[i]._dataMsg.uiLen     ,NULL);
				g_cTCPClient[i].Send(g_TCPClientList[i]._iClientSocket, g_TCPClientList[i]._dataMsg.uiDataReady, g_TCPClientList[i]._dataMsg._szData);
//				g_cTCPClient[i].Send(g_TCPClientList[i]._iLastSockReceivedFrom, g_TCPClientList[i]._dataMsg.uiLen, (void *)g_TCPClientList[i]._dataMsg._szDataRcv[g_TCPClientList[i]._dataMsg._cBuffId]);
				g_TCPClientList[i]._dataMsg.uiDataReady = 0;
			}
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////
/**
 * \fn 		TCPClientReceiveThreadFunc ()
 * \brief	This function receives Client TCP messages.
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * TCPClientReceiveThreadFunc (void* arguments)
{
	args_tcp_client_rcv_t *args = (args_tcp_client_rcv_t *)arguments;
 	unsigned short usID = args->usID;
	unsigned int i = 0;
	int rc = 0 ;

	client_TCP_s_t* pClientList = &g_TCPClientList[usID];
	while(!g_iTerminate)
	{
		rc = pClientList->_dataMsg.iTcpRcvDataSize[0] = g_cTCPClient[usID].Receive(pClientList->_iClientSocket, pClientList->_dataMsg.uiLen, &(pClientList->_dataMsg._szDataRcv[0][0]), (int32_t)7000);
		if(rc > 0)
		{
			atomic_set_bits(TCP_DATA_READY, &(pClientList->_dataMsg.uiRcvDataReady));
			//if(!(++i % 1000))
			//	printf("%s():  Socket ID = %d,  i = %6d,  RcvDataSize = %d,  _dataMsg.uiLen = %d\n", __func__, usID, i, pClientList->_dataMsg.iTcpRcvDataSize[0], pClientList->_dataMsg.uiLen);

		}
		else
		{
			g_iTcpClientDisconnected[usID] = 1;
			//printf("%s():  Socket ID = %d,  i = %6d,  RcvDataSize = %d,  _dataMsg.uiLen = %d  Closing thread...\n\n", __func__, usID, i, pClientList->_dataMsg.iTcpRcvDataSize[0], pClientList->_dataMsg.uiLen);
			return NULL;
		}
	}

	return NULL;
}
