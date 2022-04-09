/*
 * oncrpc_server.cpp
 *
 *  Created on: 21/08/2017
 *      Author: zivb
 */

#include "msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

int make_socket (uint16_t port);

static void
messageprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	char *cArgsAdd = NULL;
	char *result;

	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	char cMesgIn[1000];
	sil_set_parameter_double_in stSilSetParaminDouble;
	sil_set_parameter_int_in stSilSetParaminInt;
	sil_set_parameter_uint_in stSilSetParaminUInt;
	sil_set_parameter_short_in stSilSetParaminShort;
	sil_set_parameter_ushort_in stSilSetParaminUShort;
	sil_set_parameter_char_in stSilSetParaminChar;
	sil_get_parameter_in stSilGetParamin;

	cArgsAdd = cMesgIn;

//	printf("rqstp->rq_proc = %ld\n", rqstp->rq_proc);
//	sleep(2);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;
	case SIL_GET_COMPILATION_DATE:
		_xdr_argument = (xdrproc_t) xdr_silGetCompDate_in;//xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_silGetCompDate_out;//xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) getCompdate;
		break;
	case SIL_GET_COMPILATION_DATE_FROM_XML:
		_xdr_argument = (xdrproc_t) xdr_silGetCompDate_in;
		_xdr_result = (xdrproc_t) xdr_silGetCompDate_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getCompDateFromXML;
		break;
	case SIL_SET_PARAM_DOUBLE:
		_xdr_argument = (xdrproc_t) xdr_silSetParameterDouble_in;
		_xdr_result = (xdrproc_t) xdr_silSetParameter_out;
		local = (char *(*)(char *, struct svc_req *)) sil_setParameterDouble;
		cArgsAdd = (char *)&stSilSetParaminDouble;
		break;
	case SIL_SET_PARAM_INT:
		_xdr_argument = (xdrproc_t) xdr_silSetParameterInt_in;
		_xdr_result = (xdrproc_t) xdr_silSetParameter_out;
		local = (char *(*)(char *, struct svc_req *)) sil_setParameterInt;
		cArgsAdd = (char *)&stSilSetParaminInt;
		break;
	case SIL_SET_PARAM_UINT:
		_xdr_argument = (xdrproc_t) xdr_silSetParameterUInt_in;
		_xdr_result = (xdrproc_t) xdr_silSetParameter_out;
		local = (char *(*)(char *, struct svc_req *)) sil_setParameterUInt;
		cArgsAdd = (char *)&stSilSetParaminUInt;
		break;
	case SIL_SET_PARAM_SHORT:
		_xdr_argument = (xdrproc_t) xdr_silSetParameterShort_in;
		_xdr_result = (xdrproc_t) xdr_silSetParameter_out;
		local = (char *(*)(char *, struct svc_req *)) sil_setParameterShort;
		cArgsAdd = (char *)&stSilSetParaminShort;
		break;
	case SIL_SET_PARAM_USHORT:
		_xdr_argument = (xdrproc_t) xdr_silSetParameterUShort_in;
		_xdr_result = (xdrproc_t) xdr_silSetParameter_out;
		local = (char *(*)(char *, struct svc_req *)) sil_setParameterUShort;
		cArgsAdd = (char *)&stSilSetParaminUShort;
		break;
	case SIL_SET_PARAM_CHAR:
		_xdr_argument = (xdrproc_t) xdr_silSetParameterChar_in;
		_xdr_result = (xdrproc_t) xdr_silSetParameter_out;
		local = (char *(*)(char *, struct svc_req *)) sil_setParameterChar;
		cArgsAdd = (char *)&stSilSetParaminChar;
		break;
	case SIL_SET_PARAM_UCHAR:
		_xdr_argument = (xdrproc_t) xdr_silSetParameterUChar_in;
		_xdr_result = (xdrproc_t) xdr_silSetParameter_out;
		local = (char *(*)(char *, struct svc_req *)) sil_setParameterUChar;
		cArgsAdd = (char *)&stSilSetParaminChar;
		break;
	case SIL_CREATE_XML:
		_xdr_argument = (xdrproc_t) xdr_silCreateXML_in;
		_xdr_result = (xdrproc_t) xdr_silCreateXML_out;
		local = (char *(*)(char *, struct svc_req *)) sil_CreateXML;
		break;
	case SIL_LOAD_PARAM_FROM_XML:
		_xdr_argument = (xdrproc_t) xdr_silLoadParamFromXML_in;
		_xdr_result = (xdrproc_t) xdr_silLoadParamFromXML_out;
		local = (char *(*)(char *, struct svc_req *)) sil_loadParamFromXML;
		break;
	case SIL_GET_PARAM_DOUBLE:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameterDouble_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterDouble;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_PARAM_INT:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameterInt_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterInt;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_PARAM_UINT:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameteruInt_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterUInt;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_PARAM_SHORT:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameterShort_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterShort;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_PARAM_USHORT:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameteruShort_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterUShort;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_PARAM_CHAR:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameterChar_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterChar;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_PARAM_UCHAR:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameterUChar_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterUChar;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_PARAM_BOOL:
		_xdr_argument = (xdrproc_t) xdr_readparam_in;
		_xdr_result = (xdrproc_t) xdr_silGetParameterBool_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getParameterBool;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_GET_TIME_MEASUREMENT:
		_xdr_argument = (xdrproc_t) xdr_silGetTimeMeasurement_in;
		_xdr_result = (xdrproc_t) xdr_silGetTimeMeasurement_out;
		local = (char *(*)(char *, struct svc_req *)) sil_getTimeMeasurement;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_STOP_EXE:
		_xdr_argument = (xdrproc_t) xdr_silStopExe_in;
		_xdr_result = (xdrproc_t) xdr_silStopExe_out;
		local = (char *(*)(char *, struct svc_req *)) sil_stopexe;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_SAVE_LOG_EXE:
		_xdr_argument = (xdrproc_t) xdr_silSaveLogExe_in;
		_xdr_result = (xdrproc_t) xdr_silSaveLogExe_out;
		local = (char *(*)(char *, struct svc_req *)) sil_savelog;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_START_REC:
		_xdr_argument = (xdrproc_t) xdr_silStartRec_in;
		_xdr_result = (xdrproc_t) xdr_silStartRec_out;
		local = (char *(*)(char *, struct svc_req *)) sil_startrec;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_STOP_REC:
		_xdr_argument = (xdrproc_t) xdr_silStopRec_in;
		_xdr_result = (xdrproc_t) xdr_silStopRec_out;
		local = (char *(*)(char *, struct svc_req *)) sil_stoprec;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
	case SIL_REC_STAT:
		_xdr_argument = (xdrproc_t) xdr_silRecStat_in;
		_xdr_result = (xdrproc_t) xdr_silRecStat_out;
		local = (char *(*)(char *, struct svc_req *)) sil_recstat;
		cArgsAdd = (char *)&stSilGetParamin;
		break;
//	case ARRAY_TEST:
//		_xdr_argument = (xdrproc_t) xdr_intarr;
//		_xdr_result = (xdrproc_t) xdr_int;
//		local = (char *(*)(char *, struct svc_req *)) arrayTst;
//		break;
	default:
		svcerr_noproc (transp);
		return;
	}

//	memset ((char *)&cArgsAdd, 0, sizeof (cArgsAdd));

	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) cArgsAdd)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)/*&argument*/cArgsAdd, rqstp);

	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
//	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
//		fprintf (stderr, "%s", "unable to free arguments");
//		exit (1);
//	}
	return;
}

/*!
 * \fn make_socket (uint16_t port)
 * \brief	creates the socket on which this server will listten.
 * \param port the on which this server will listten.
 * \return 0 if completed successfully, otherwise error.
 */
int make_socket (uint16_t port)
{
	int sock;
	struct sockaddr_in name;

	fprintf(stderr, "%s: port=%u\n", __func__, port);
	/* Create the socket. */
	sock = socket (PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror ("socket");
		exit (EXIT_FAILURE);
	}

	/* Give the socket a name. */
	name.sin_family = AF_INET;
	name.sin_port = htons (port);
	name.sin_addr.s_addr = htonl (INADDR_ANY);
	if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
	{
		perror ("bind");
		exit (EXIT_FAILURE);
	}

	return sock;
}


//////////////////////////////////////////////////////////////////
/**
 * \fn 		APIExeThreadFuncReq()
 * \brief	Execute API for each axis in the system according to requested command
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * OncRPCServerThreadFunc(void *)
{
		int  sock;
		register SVCXPRT *transp;

		pmap_unset (MESSAGEPROG, MESSAGEVERS);

//		transp = svcudp_create(RPC_ANYSOCK);
//		if (transp == NULL) {
//			fprintf (stderr, "%s", "cannot create udp service.");
//			exit(1);
//		}
//		if (!svc_register(transp, MESSAGEPROG, MESSAGEVERS, messageprog_1, IPPROTO_UDP)) {
//			fprintf (stderr, "%s", "unable to register (MESSAGEPROG, MESSAGEVERS, udp).");
//			exit(1);
//		}
		sock = make_socket(6666);
		transp = svctcp_create(/*RPC_ANYSOCK*/sock, 0, 0);
		if (transp == NULL) {
			fprintf (stderr, "%s", "cannot create tcp service.");
			exit(1);
		}
		printf("Server PORT = %d\n",transp->xp_port);

		if (!svc_register(transp, MESSAGEPROG, MESSAGEVERS, messageprog_1, IPPROTO_TCP)) {
			fprintf (stderr, "%s", "unable to register (MESSAGEPROG, MESSAGEVERS, tcp).");
			exit(1);
		}

		svc_run ();
		fprintf (stderr, "%s", "svc_run returned");
		exit (1);
		/* NOTREACHED */

	return NULL;
}


/*!
 * \fn onc_srv_create ()
 * \brief	creates onc rpc server thread function.
 * \param .
 * \return 0 if completed successfully, otherwise error.
 */
int onc_srv_create ()
{
	pthread_attr_t hCreateSrvThreadAttr;
	pthread_t hCreateSrvThread;

	if(pthread_attr_init(&hCreateSrvThreadAttr))
	{
		printf("pthread_attr_init failure\n");
	}
	if(pthread_attr_setdetachstate(&hCreateSrvThreadAttr, PTHREAD_CREATE_DETACHED))
	{
		printf("pthread_attr_setdetachstate failure\n");
	}
	if(pthread_create(&hCreateSrvThread, &hCreateSrvThreadAttr, OncRPCServerThreadFunc, NULL) != 0)
	{
		printf("Unable to create APIExeThreadFunc thread\n");
	}
	//todo return error
	return 0;
}



