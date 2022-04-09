/*
 * ElmoMasterSIL_UDP.h
 *
 *  Created on: 24/06/2018
 *      Author: zivb
 */

#ifndef ELMOMASTERSIL_UDP_H_
#define ELMOMASTERSIL_UDP_H_

#include <MMC_Definitions.h>
#include <mmcpplib.h>

#define MAX_UDP_CONN		5

#define UDP_MSG_MAX_SIZE	500
#define IP_STR_SIZE			16

#define UDP_DATA_CLR				0x10000000
#define UDP_DATA_READY				0x00000001
#define UDP_DATA_SEND				0x00000002
#define UDP_DATA_REQ				0x00000004

#ifndef ELMO_MAX_MULTI_STEPS
#define ELMO_MAX_MULTI_STEPS        8
#endif

extern int 	g_iTerminate;
extern int 	g_iLogMatFile;
extern char g_cRecTrig;
extern char g_cSaveOnce;

extern void atomic_set_bits(unsigned int mask, unsigned int *addr);
extern void atomic_clear_bit(int nr, volatile unsigned int *addr);

extern sem_t  stepSem[ELMO_MAX_MULTI_STEPS];

// Data Struct for SEND
struct _msg_data_t {
	int _iID;
	unsigned int uiDataReady;
	unsigned int uiSendData;
	unsigned int uiLen;
	char _szData[UDP_MSG_MAX_SIZE];
	unsigned char _cBuffId;
};

// Data Struct for Rx
struct _msg_srv_data_t {
	int _iID;
	int iUdpRcvDataSize[2];
	unsigned int uiDataReady;
	unsigned int uiLen;
	unsigned short usServerPort;
	char _szData[UDP_MSG_MAX_SIZE];
	unsigned char _szDataRcv[2][UDP_MSG_MAX_SIZE];
	unsigned char _cBuffId;
};


typedef struct _tcppmsg_t tcppmsg_t;

typedef struct _client_s {
	sockaddr_in _saddr;
	_msg_data_t _dataMsg;
}client_s_t;

typedef struct _server_s {
	int iEventDriven;	// 0 = not event driven, otherwise - relates to the relevant stepFunction:
						// 1:  relates to stepRate[0] ...  5:  relates to stepRate[4]
	sockaddr_in _saddr;
	_msg_srv_data_t _dataMsg;
}server_s_t;

#ifdef ELMO_UDP
CMMCUDP						g_cUDPServer	[MAX_UDP_CONN];
CMMCUDP						g_cUDPClient	[MAX_UDP_CONN];
char 						g_cUDPSendReq	[MAX_UDP_CONN];
sem_t 						SemUDP;
bool 						g_bWait			[MAX_UDP_CONN];
client_s_t 					g_saClientList	[MAX_UDP_CONN];
server_s_t 					g_saServerList	[MAX_UDP_CONN];
#else
extern CMMCUDP				g_cUDPServer	[MAX_UDP_CONN];
extern CMMCUDP				g_cUDPClient	[MAX_UDP_CONN];
extern char 				g_cUDPSendReq	[MAX_UDP_CONN];
extern sem_t 				SemUDP;
extern bool 				g_bWait			[MAX_UDP_CONN];
extern client_s_t 			g_saClientList	[MAX_UDP_CONN];
extern server_s_t 			g_saServerList	[MAX_UDP_CONN];
#endif



///UDP FUNCTION
//////////////////////////////////////////////////////////////////
/**
 * \fn 		UDPSendThread ()
 * \brief	This function send udp message according to user request.
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * UDPSendThreadFunc (void *);

//////////////////////////////////////////////////////////////////
/**
 * \fn 		udp_sendThread_init()
 * \brief	start a thread for sending UDP
 * \param	void
 * \return  void.
 */
//////////////////////////////////////////////////////////////////
void udp_sendThread_init();

//////////////////////////////////////////////////////////////////
/**
 * \fn 		UDPRcvClbk ()
 * \brief	Callback function called when UDP data received.
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
int UDPRcvClbk(int iSock, short sEventID, unsigned char* ucBuffer, int iRc,struct sockaddr_in* pSockaddr);
#endif /* ELMOMASTERSIL_UDP_H_ */
