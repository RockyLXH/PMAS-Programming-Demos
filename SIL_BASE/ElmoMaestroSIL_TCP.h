/*
 * ElmoMaestroSIL_TCP.h
 *
 *  Created on: 30/06/2019
 *      Author: zivb
 */

#ifndef ELMOMAESTROSIL_TCP_H_
#define ELMOMAESTROSIL_TCP_H_

#define MAX_TCP_CONN		6		// only 5 connections are supported, since we start from idx=1

#define TCP_DATA_CLR				0x10000000
#define TCP_DATA_READY				0x00000001
#define TCP_DATA_SEND				0x00000002
#define TCP_DATA_REQ				0x00000004

#define MMCPP_RETERROR	1
#define MMCPP_RETOK	0
#define MMCPP_INVSOCKET -1
#define MMCPP_SOCKERROR	-1

#define MMCPP_SOCK_INFINIT_BLOCK -1
#define MMCPP_SOCK_EOE_RETRIES	3
#define MMCPP_SOCK_NEW_EVENT 10
#define MMCPP_SOCK_READY_EVENT 11
#define MMCPP_SOCK_CLOSED_EVENT 12
#define MMCPP_SOCK_ABORT_EVENT 13
#define MMCPP_SOCK_MAXLIM_EVENT 14

#define MAX_CLIENTS 		3
#define TCP_MSG_MAX_SIZE	500

// Elmo TCP Interfaces
//
struct _tcppmsg_t {
	unsigned char _iID;
	char _szData[255];
};

typedef struct _tcppmsg_t tcppmsg_t;

// Data Struct for Rx
struct _msg_TCP_srv_data_t {
	int _iID;
	int iTcpRcvDataSize[2];
	unsigned int uiDataReady;
	unsigned int uiRcvDataReady;
	unsigned int uiLen;
	unsigned short usServerPort;
	unsigned char _szDataRcv[2][TCP_MSG_MAX_SIZE];
	char _szData[TCP_MSG_MAX_SIZE];
	unsigned char _cBuffId;
};

typedef struct client_tcp_s_t {
	int _iSock;
	bool _bReadFlag;
	tcppmsg_t _tcpMsg;
}client_tcp_s_t;

typedef struct server_TCP_s_t {
	int _iLastSockReceivedFrom;
	sockaddr_in _saddr;
	_msg_TCP_srv_data_t _dataMsg;
	client_tcp_s_t stClientList[MAX_CLIENTS];
}server_TCP_s_t;

typedef struct
{
	unsigned short usID;
	unsigned int uiDataLen;
	unsigned char * cHostIP;
	unsigned short usHostPort;

} args_tcp_client_rcv_t;

typedef struct client_TCP_s_t {
	int _iLastSockReceivedFrom;
	int _iClientSocket;
	sockaddr_in _saddr;
	_msg_TCP_srv_data_t _dataMsg;
	args_tcp_client_rcv_t  args_tcp_client_rcv;
	client_tcp_s_t stClientList[MAX_CLIENTS];
}client_TCP_s_t;

#ifdef ELMO_TCP
CMMCTCP						g_cTCPServer	[MAX_TCP_CONN];
CMMCTCP						g_cTCPClient	[MAX_TCP_CONN];
sem_t 						SemTCP;
sem_t 						SemTCPClient;
bool 						g_TCPbWait		[MAX_TCP_CONN];
server_TCP_s_t 				g_TCPServerList	[MAX_TCP_CONN];
client_TCP_s_t 				g_TCPClientList	[MAX_TCP_CONN];
#else
extern CMMCTCP				g_cTCPServer	[MAX_TCP_CONN];
extern CMMCTCP				g_cTCPClient	[MAX_TCP_CONN];
extern sem_t 				SemTCP;
extern sem_t 				SemTCPClient;
extern bool 				g_TCPbWait		[MAX_TCP_CONN];
extern server_TCP_s_t 		g_TCPServerList	[MAX_TCP_CONN];
extern client_TCP_s_t 		g_TCPClientList	[MAX_TCP_CONN];
#endif

extern void atomic_set_bits(unsigned int mask, unsigned int *addr);
extern void atomic_clear_bit(int nr, volatile unsigned int *addr);
extern int 	g_iTerminate;
extern int 	g_iTcpClientDisconnected[MAX_TCP_CONN];

void AddClient2Hlp(int iSock, int port);
void RemoveClient2Hlp(int iSock,int port);
void ClearReadFlag2Hlp(int iSockID);
void MoveData2Hlp(int iSock, unsigned char* ucBuffer,int iRcvDataSize);
void tcp_serv_sendThread_init();
void * TCPServerSendThreadFunc (void *);
void * TCPClientSendThreadFunc (void *);
void * TCPClientReceiveThreadFunc (void* args);
void tcp_client_sendThread_init();
void tcp_client_receiveThread_init();

//////////////////////////////////////////////////////////////////
/**
 * \fn 		TCPServerRcvClbk ()
 * \brief	Callback function called when TCP data received.
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
int TCPServerRcvClbk(int iSock, short sEventID, unsigned char* ucBuffer, int iRc,struct sockaddr_in* pSockaddr);

#endif /* ELMOMAESTROSIL_TCP_H_ */
