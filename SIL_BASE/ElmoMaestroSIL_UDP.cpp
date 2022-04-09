/*
 * ElmoMasterSIL_UDP.cpp
 *
 *  Created on: 24/06/2018
 *      Author: zivb
 */

#define ELMO_UDP
#include "ElmoMaestroSIL_UDP.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \fn 		udp_sendThread_init()
 * \brief	start a thread for sending UDP
 * \param	void
 * \return  void.
 */
void udp_sendThread_init()
{
	pthread_attr_t hUDPSendThreadAttr;
	pthread_t hUDPSendThread;

	if(pthread_attr_init(&hUDPSendThreadAttr))
	{
		printf("pthread_attr_init failure\n");
	}
	if(pthread_attr_setdetachstate(&hUDPSendThreadAttr, PTHREAD_CREATE_DETACHED))
	{
		printf("pthread_attr_setdetachstate failure\n");
	}
	printf("UDP Send Thread Create\n");
	if(pthread_create(&hUDPSendThread, &hUDPSendThreadAttr, UDPSendThreadFunc, NULL) != 0)
	{
		printf("Unable to create APIExeThreadFunc thread\n");
	}

}

//////////////////////////////////////////////////////////////////
/**
 * \fn 		UDPSendThread ()
 * \brief	This function sends udp message according to user request.
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * UDPSendThreadFunc (void *)
{
	int i = 0;
	while(!g_iTerminate)
	{
		sem_wait(&SemUDP);
		for( i = 0 ; i < MAX_UDP_CONN ;i++)
		{
			if(g_saClientList[i]._dataMsg.uiSendData)
			{	// "uiSendData" holds the actual size to send ...
				// g_cUDPClient[i].Send((char*)&g_saClientList[i]._dataMsg._szData, g_saClientList[i]._dataMsg.uiLen     ,NULL);
				   g_cUDPClient[i].Send((char*)&g_saClientList[i]._dataMsg._szData, g_saClientList[i]._dataMsg.uiSendData,NULL);
				   g_saClientList[i]._dataMsg.uiSendData = 0;
			}
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////
///**
// * \fn 		UDPRcvClbk ()
// * \brief	Callback function called when UDP data received.
// * \param	void
// * \return  void.
// */
/////////////////////////////////////////////////////////////////////
int UDPRcvClbk(int iSock, short sEventID, unsigned char* ucBuffer, int iRc,struct sockaddr_in* pSockaddr)
{

	switch(sEventID)
	{
	case MMCPP_SOCK_READY_EVENT:
	{

		struct sockaddr_in sin;
		socklen_t len = sizeof(sin);
		int iServerPort;
		if (getsockname(iSock, (struct sockaddr *)&sin, &len) == -1)
		    perror("getsockname");
//		else
//		    printf("port number %d\n", ntohs(sin.sin_port));


		iServerPort = ntohs(sin.sin_port);

		for(int i = 0 ; i < MAX_UDP_CONN ;i++)
		{
			if(g_saServerList[i]._dataMsg.usServerPort == iServerPort)
			{
				g_saServerList[i]._dataMsg._cBuffId ^= 1;
				//Copy the data to the global data
				memcpy(&(g_saServerList[i]._dataMsg._szDataRcv[g_saServerList[i]._dataMsg._cBuffId]), ucBuffer, g_saServerList[i]._dataMsg.uiLen);
				g_saServerList[i]._dataMsg.iUdpRcvDataSize[g_saServerList[i]._dataMsg._cBuffId] = iRc;
				//Set data ready flag for the relvant client
				atomic_set_bits(UDP_DATA_READY,&(g_saServerList[i]._dataMsg.uiDataReady));

				if(g_saServerList[i].iEventDriven)
				{
					sem_post(&stepSem[g_saServerList[i].iEventDriven - 1]);  // iEventDriven = 1:  relates to stepRate[0] ...
																			 //	iEventDriven = 5:  relates to stepRate[4]
					printf("\ng_saServerList[%d].iEventDriven = %d  --->  sem_post(&stepSem[%d])\n\n", i, g_saServerList[i].iEventDriven, g_saServerList[i].iEventDriven - 1);
				}
			}
		}
	}
	break;

	//
	default:
		break;
	}
	return 0;
}
