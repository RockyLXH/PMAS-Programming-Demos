/*
============================================================================
 Name : SIL_BASE.cpp
 Author :
 Version :
 Description : GMAS C++ project source file
============================================================================
*/

#include <assert.h>
#include <math.h>
#include <time.h>

#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <signal.h>
#include "ElmoMaestroSIL_Main.h"
#include <iostream>
#include <MMC_Definitions.h>
#include <mmcpplib.h>
#include "ElmoMaestroSIL_UDP.h"
#include "ElmoMaestroSIL_Lib.h"
#include "ElmoMaestroSIL_XML.h"
#include "ElmoMaestroSIL_CAPI.h"
#include <oncrpc_server.h>

using namespace std;

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define SLEEP_T 			100000
#define MAX_PI				50
#define SIL_DATA_LONG_NUM	32
#define SIL_DATA_DOUBLE_NUM	32
#define SIl_STR_LEN			100

/////////////////////////////////////////////////////////////////
//Modbus state flow request handler
#define MB_START_REF		251
#define MB_REF_CNT			250

//Commands request values
#define AXIS_ENABLE_CMD		1
#define AXIS_HOME_CMD		2
#define AXIS_CHGOP_CMD		3
#define AXIS_MOTION_CMD		4
#define AXIS_CLR_ERR_CMD	5

//Commands comman index
#define MB_FUNC_IDX			0
#define MB_AXIS_IDX			1

//axis enable command index
#define MB_CMD_ENABLE_VAL_IDX			2

//Change operation mode index
#define MB_CMD_CHOP_VAL_IDX				2


//Motion command index
#define MB_CMD_MOTION_MODE_IDX 			2
#define MB_CMD_MOTION_DPOS_IDX 			3
#define MB_CMD_MOTION_FVEL_IDX 			4
#define MB_CMD_MOTION_FAC_IDX 			5
#define MB_CMD_MOTION_FJERK_IDX 		6

///////////////////////////////////////////////////////////////////
typedef void (*CLBK_HANDLER)(int iIndex,unsigned short usAxisRef);
void sm_admin(int iAxisID);

void StartModBus();
void BusDataExchange();
void modbus_data_exhange();
void * ModbusxeThreadFuncReq(void *);

void TerminateApplication(int iSigNum);
void sigAction();
void onExit();

void stepThreads_init();
void* StepThreadFunc(void* arguments);

///////////////////////////////////////////////////////////////////
typedef struct {
  int stepIndex;
} args_stepFunc_t;

typedef void (*STEP_HANDLER)(void);
STEP_HANDLER  ptrFuncSteps[ELMO_MAX_MULTI_STEPS] = {NULL};

pthread_t   stepThread[ELMO_MAX_MULTI_STEPS];
sem_t  stepSem[ELMO_MAX_MULTI_STEPS];

int stepRate[ELMO_MAX_MULTI_STEPS] = {ELMO_STEPRATE_1, ELMO_STEPRATE_2, ELMO_STEPRATE_3, ELMO_STEPRATE_4,
									  ELMO_STEPRATE_5, ELMO_STEPRATE_6, ELMO_STEPRATE_7, ELMO_STEPRATE_8};

int udpEventId[ELMO_MAX_MULTI_STEPS] = {ELMO_UDPEVENTID_1, ELMO_UDPEVENTID_2, ELMO_UDPEVENTID_3, ELMO_UDPEVENTID_4,
									  ELMO_UDPEVENTID_5, ELMO_UDPEVENTID_6, ELMO_UDPEVENTID_7, ELMO_UDPEVENTID_8};

int iEventDrivenOffset;

//////////////////////////////////////////////////////////////
/// Global Parameters ///
CMMCConnection 		g_cConn;
MMC_CONNECT_HNDL 	g_hConn;
CMMCRTSingleAxis* 	g_cRTSingleAxisArr;
unsigned int		g_uiNumOfAxes;
CMMCPIVarRT 		g_cPIVarRT[MAX_PI];
MMCPPGlobalRT		g_cGloblaRT;
CMMCHostComm		g_cHostComm;
unsigned char		g_ucExecutingCommand = 0;
API_COMN			g_stAPICom;
API_STATUS			g_stApiStat;
MMC_MODBUSWRITEHOLDINGREGISTERSTABLE_IN g_stWriteHoldingReg;
MMC_MODBUSREADHOLDINGREGISTERSTABLE_OUT g_stReadHoldingReg;
unsigned char 		g_ucModBusEvtFlag = 0;
unsigned char		g_ucDebugMode = 0;
long				g_lCycleTime = 0;

CMMCPPIMU			g_cIMU[10];

// Serial Communication global parameters.
CMaestroSerialPort	g_cSerialPort[MAX_SERIAL_CONN];

int 	g_iTerminate  = 0;		// Flag to request program termination
int 	g_iLogMatFile = 0;		// Flag to request save to a log file
char 	g_cRecTrig	  = 0;		// Flag to start \ stop recording
char 	g_cSaveOnce	  = 0;		// Flag to that we cannot start recording after we have one save executed.

int 	g_iTcpClientDisconnected[MAX_TCP_CONN] = {0};	// Flags for re-initializing TCP clients connections

/////////////////////////////////////////////////////////////////
/// MATLAB C API Parameters
unsigned int g_iNumberOfModleParameters = 0;
rtwCAPI_ModelMappingInfo *g_MMI;

const rtwCAPI_ModelParameters* g_pstModelParams;	//pointer to ModelParameter Structure
const rtwCAPI_DataTypeMap*     g_pDataTypeMap;		//Pointer to Data type map.
const rtwCAPI_DimensionMap*    g_pDimMap;
const rtwCAPI_FixPtMap*        g_pFxpMap;
const rtwCAPI_ElementMap*  	   g_pElementMap;
void**                         g_ppDataAddrMap;


//Time Measurement global parameters
unsigned long		g_ulCycleCountPrev 			= 0;
unsigned long		g_ulCycleCountCurrent 		= 0;
unsigned long		g_ulCycleOverFlow 			= 0;
unsigned char	 	g_ucAllowTimeMeasurement 	= 0;
//struct timeval 	g_tv_curr;
//struct timeval 	g_tv_prev;
struct timespec		g_ts_OnSyncFunc;
struct timespec		g_ts_OnSyncFunc_prev;
struct timespec		g_ts_ELMO_STEP_end;

unsigned long		g_ulElapsedTime				= 0;
unsigned long		g_ulElapsedTimeMax			= 0;

unsigned long		g_ulCycleCountElTime;
unsigned long 		g_ulSumElapsedTime 			= 0;
unsigned long		g_ulAvgElapsedTime 			= 0;

//Time Measurement global parameters for MultiStep Functions
unsigned long		_g_ulCycleCountPrev[ELMO_MAX_MULTI_STEPS]    = {0};
unsigned long		_g_ulCycleCountCurrent[ELMO_MAX_MULTI_STEPS] = {0};
unsigned long		_g_ulCycleOverFlow[ELMO_MAX_MULTI_STEPS] 	 = {0};

struct timeval 		_g_tv_curr[ELMO_MAX_MULTI_STEPS];
struct timeval 		_g_tv_prev[ELMO_MAX_MULTI_STEPS];
//struct timespec	_g_ts_curr[ELMO_MAX_MULTI_STEPS];
//struct timespec	_g_ts_prev[ELMO_MAX_MULTI_STEPS];

unsigned long		_g_ulElapsedTime[ELMO_MAX_MULTI_STEPS]	     = {0};
unsigned long		_g_ulElapsedTimeMax[ELMO_MAX_MULTI_STEPS]	 = {0};

unsigned long		_g_ulCycleCountElTime[ELMO_MAX_MULTI_STEPS];
unsigned long 		_g_ulSumElapsedTime[ELMO_MAX_MULTI_STEPS]    = {0};
unsigned long		_g_ulAvgElapsedTime[ELMO_MAX_MULTI_STEPS]    = {0};


/////////////////////////////////////////////////////////////////
// Define the Input/output Arguments Structures
// The structs are listed here for reference only.
// They are declared by the MATLAB modle Function ...

typedef struct {
  uint32_T ui32MsgCntr;
  uint32_T ui32ModRqst;
  uint32_T ui32PowerRqst;
  real32_T fCurCmd_ID;
  real32_T fCurCmd_IQ;
  real_T fvelCmd;
  real32_T dPosCmd;
} SIL_InMessage;
//
typedef struct {
  uint32_T ui32MsgCntr;
  uint32_T ui32ModAct;
  uint32_T ui32PowerAct;
  real32_T fCurAct_ID;
  real32_T fCurAct_IQ;
  real_T fvelAct;
  real32_T dPosAct;
} SIL_OutMessage;
//
SIL_InMessage	SILInpMsgStruct	;
SIL_OutMessage	SILOutMsgStruct	;


///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// Global functions ///
void sil_system_init();
int OnSyncFunc();
void start_syncfunc();
void api_execution();
void *APIExeThreadFuncReq(void *);
void udp_sendThread_init();
int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus);
void register_exception_handler();

void PowerOffReq(int iIndex);
void PowerOnReq(int iIndex);
void MoveReq(int iIndex);
void ChgOpModReq(int iIndex);
void ResetReq(int iIndex);
void HomeReq(int iIndex);
void DummyReq(int iIndex);

typedef void (*REQ_HANDLER)(int index);

REQ_HANDLER req_hand_func_array[]=
{
		DummyReq,
		PowerOffReq,
		PowerOnReq,
		MoveReq,
		ChgOpModReq,
		ResetReq,
		HomeReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq,
		DummyReq

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
char GetOptions(int argc, char** argv)
{
      int opt;
      char retval =0 ;
      while ((opt = getopt(argc, argv, "d:")) != -1)
      {
        switch (opt)
        {
        case 'd': //debug mode
        	g_ucDebugMode = atoi(optarg);
        	printf("setting g_ucDebugMode = %d\n",g_ucDebugMode);
            break;
         default:
            fprintf(stderr, "\n\nUsage: %s [-d debug mode 1 - ON , 0 -off , default = 0] \n\n", argv[0]);
            break;
            //
        }
    }
      if(!retval)
      {
    	  fprintf(stderr, "\n\nUsage: %s [-d debug mode 1 - ON , 0 -off , default = 0] \n\n", argv[0]);
    	  //fprintf(stderr, "%sNo Axis name was stated. Exiting !\n\n\n\n",RED);
    	  //cout << NOCOLOR ;
    	  //exit(EXIT_FAILURE);
      }
      return retval ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void init_tunable_param()
{
	//only execute if C API selected in model
#ifdef BUILTIN_TYPEID_TYPES
	//g_MMI = &(rtmGetDataMapInfo(Junk2_M).mmi);
	ELMO_MAP_INFO

	g_iNumberOfModleParameters = rtwCAPI_GetNumModelParameters(g_MMI);

	g_stModifyParamData =(CAPI_MODIFY_PARA_STRUCT *)malloc( g_iNumberOfModleParameters * sizeof (CAPI_MODIFY_PARA_STRUCT));

	//init global tunable parameters maps
    /* Get modelParams, an array of rtwCAPI_ModelParameters structure  */
	g_pstModelParams = rtwCAPI_GetModelParameters(g_MMI);
    if (g_pstModelParams == NULL) return;


    g_pDataTypeMap = rtwCAPI_GetDataTypeMap(g_MMI);
    if (g_pDataTypeMap == NULL) return;

    g_pElementMap = rtwCAPI_GetElementMap(g_MMI);

    g_ppDataAddrMap = rtwCAPI_GetDataAddressMap(g_MMI);

	for(unsigned int z = 0; z < g_iNumberOfModleParameters; z++)
	{
		sil_capi_LoadModelParameter(g_MMI,z);
	}
#endif
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{

	char strCompilationTime[MAX_STR_SIZE];
	int idx = 1;

	try
	{
		sigAction();

		GetOptions(argc,argv);

#ifdef ELMO_MODEL_NAME
		printf(ANSI_COLOR_GREEN     "Model name: %s"     ANSI_COLOR_RESET "\n",ELMO_MODEL_NAME);
#endif
		printf(ANSI_COLOR_GREEN     "SIL BASE VERSION %s"     ANSI_COLOR_RESET "\n",SIL_BASE_VER);
		sil_xml_getCompilationTime(strCompilationTime);

		printf("SIL BASE compilation: %s\n",strCompilationTime);

		sil_system_init();

		api_execution();

		udp_sendThread_init();
		tcp_serv_sendThread_init();
		tcp_client_sendThread_init();

		stepThreads_init();

		//matlab init step
		ELMO_INITIALIZE

		//execute only if MODEL set to use tunable paramters
#ifdef BUILTIN_TYPEID_TYPES
		init_tunable_param();

//		sil_capi_set_parameters_from_file();
		sil_capi_set_parameters_from_file_ex();

#endif
		onc_srv_create();

//		Elmo_Time_Measurement_Init();

		start_syncfunc();

		StartModBus();

		modbus_data_exhange();

		sil_capi_create_xml();


		if(g_ucDebugMode)
		{
			double ang[4];
			double encdr[8] = {0};
			unsigned short diff_in = 0, diff_out = 0x55;

			for(int i=0; i<1; i++)
			{
//				ang[0] = g_cGloblaRT.GetCpldAdcCh1();
//				ang[1] = g_cGloblaRT.GetCpldAdcCh2();
//				ang[2] = g_cGloblaRT.GetCpldAdcCh3();
//				ang[3] = g_cGloblaRT.GetCpldAdcCh4();

				Elmo_Read_CPLD_ANG_IN_CH1(&ang[0]);
				Elmo_Read_CPLD_ANG_IN_CH2(&ang[1]);
				Elmo_Read_CPLD_ANG_IN_CH3(&ang[2]);
				Elmo_Read_CPLD_ANG_IN_CH4(&ang[3]);

				Elmo_Read_CPLD_Quad1PosUU(&encdr[0]);
				Elmo_Read_CPLD_Quad2PosUU(&encdr[1]);
				Elmo_Read_CPLD_Quad1VelUU(&encdr[2]);
				Elmo_Read_CPLD_Quad2VelUU(&encdr[3]);

				Elmo_Read_CPLD_Abs1PosUU(&encdr[4]);
				Elmo_Read_CPLD_Abs2PosUU(&encdr[5]);
				Elmo_Read_CPLD_Abs1VelUU(&encdr[6]);
				Elmo_Read_CPLD_Abs2VelUU(&encdr[7]);

				Elmo_Read_CPLD_DIFF_IO(&diff_in);
				Elmo_Write_CPLD_DIFF_IO(diff_out);

				printf("Analog Inputs 1-4:  %.2f, %.2f, %.2f, %.2f\n", ang[0], ang[1], ang[2], ang[3]);

				printf("Quad1 PosUU/Vel = %.2f, %.2f\t", encdr[0], encdr[2]);
				printf("Quad2 PosUU/Vel = %.2f, %.2f\n", encdr[1], encdr[3]);

				printf("Abs1 PosUU/Vel = %.2f, %.2f\t", encdr[4], encdr[6]);
				printf("Abs2 PosUU/Vel = %.2f, %.2f\n", encdr[5], encdr[7]);

				printf("Diff_in = 0x%X,   Diff_out = 0x%X\n", diff_in, diff_out);

				/*
				CMMCConnection cConn;   // PMAS connection
				MMC_CONNECT_HNDL gConnHndl = cConn.ConnectIPC(0x7fffffff, NULL);
				MMC_READGROUPOFPARAMETERS_IN stReadGroupOfParamIn;
				MMC_READGROUPOFPARAMETERS_OUT stReadGroupOfParamOut;

				stReadGroupOfParamIn.ucNumberOfParameters = 4;
				stReadGroupOfParamIn.sParameters[0].eParameterNumber = MMC_IUSER60B2;
				stReadGroupOfParamIn.sParameters[0].iParameterIndex = 0;
				stReadGroupOfParamIn.sParameters[0].usAxisRef = 2;
				stReadGroupOfParamIn.sParameters[1].eParameterNumber = MMC_UCUSER60B2_SRC;
				stReadGroupOfParamIn.sParameters[1].iParameterIndex = 0;
				stReadGroupOfParamIn.sParameters[1].usAxisRef = 2;
				stReadGroupOfParamIn.sParameters[2].eParameterNumber = MMC_MOTORRATEDCURRENT;
				stReadGroupOfParamIn.sParameters[2].iParameterIndex = 0;
				stReadGroupOfParamIn.sParameters[2].usAxisRef = 2;
				stReadGroupOfParamIn.sParameters[3].eParameterNumber = MMC_CYCLE_TIME_PARAM;
				stReadGroupOfParamIn.sParameters[3].iParameterIndex = 0;
				stReadGroupOfParamIn.sParameters[3].usAxisRef = 2;

				Elmo_Read_GroupOfParameters(gConnHndl, &stReadGroupOfParamIn, &stReadGroupOfParamOut);

				printf("Elmo_Read_GroupOfParameters():  ");
				printf("User60B2 = %d,  User60B2_SRC = %d,  ", (int)stReadGroupOfParamOut.dbValue[0], (int)stReadGroupOfParamOut.dbValue[1]);
				printf("MotorRatedCurrent = %.2f,  CycleTime = %d\n", stReadGroupOfParamOut.dbValue[2], (int)stReadGroupOfParamOut.dbValue[3]);
				

				MMC_WRITEGROUPOFPARAMETERSEX_IN stWriteGroupOfParamEXIn;
				MMC_WRITEGROUPOFPARAMETERSEX_OUT stWriteGroupOfParamEXOut;

				stWriteGroupOfParamEXIn.eExecutionMode = eMMC_EXECUTION_MODE_IMMEDIATE;	//queued or immediate
				stWriteGroupOfParamEXIn.ucNumberOfParameters = 1;
				stWriteGroupOfParamEXIn.ucExecute = 1;
				stWriteGroupOfParamEXIn.ucMode = 0;    // currently not in use
			    stWriteGroupOfParamEXIn.sParameters[0].eParameterNumber = (MMC_PARAMETER_LIST_ENUM)MMC_IUSER60B2;
			    stWriteGroupOfParamEXIn.sParameters[0].dbValue = 88;		//the value to set
			    stWriteGroupOfParamEXIn.sParameters[0].iParameterIndex = 0;	//?
			    stWriteGroupOfParamEXIn.sParameters[0].usAxisRef = 2;		//?
			    stWriteGroupOfParamEXIn.sParameters[0].usPIVarOffset = 0;	//?
			    stWriteGroupOfParamEXIn.sParameters[0].ucPiDirection = 3;	//?

			    Elmo_Write_GroupOfParametersEX(gConnHndl, 2, &stWriteGroupOfParamEXIn, &stWriteGroupOfParamEXOut);

			    sleep(1);

				stReadGroupOfParamIn.ucNumberOfParameters = 1;
				stReadGroupOfParamIn.sParameters[0].eParameterNumber = MMC_IUSER60B2;
				stReadGroupOfParamIn.sParameters[0].iParameterIndex = 0;
				stReadGroupOfParamIn.sParameters[0].usAxisRef = 2;

				Elmo_Read_GroupOfParameters(gConnHndl, &stReadGroupOfParamIn, &stReadGroupOfParamOut);

				printf("After Elmo_Write_GroupOfParametersEX():  User60B2 = %d\n\n", (int)stReadGroupOfParamOut.dbValue[0]);
				*/

			}
		}

		while(!g_iTerminate)
		{
			usleep(SLEEP_T);

			for(idx = 1; idx < MAX_TCP_CONN; idx++)
			{
				if(g_iTcpClientDisconnected[idx])
				{
					printf(ANSI_COLOR_GREEN     "Re-Initializing TCP Client #%d connection..."     ANSI_COLOR_RESET "\n", idx);
					sleep(3);

					Elmo_TCP_Client_Init (g_TCPClientList[idx].args_tcp_client_rcv.usID,
										  g_TCPClientList[idx].args_tcp_client_rcv.uiDataLen,
										  g_TCPClientList[idx].args_tcp_client_rcv.cHostIP,
										  g_TCPClientList[idx].args_tcp_client_rcv.usHostPort);

				}
			}

//#ifdef RTW_MATLOGGING_H__
//			if (g_iLogMatFile)
//			{
//				ELMO_LOGGER
//				g_iLogMatFile = 0;
//			}
//#endif
		}
	}

	catch(CMMCException &ex)
	{
		cout<<ex.what()<<ex.error()<<endl;
		onExit();

	}

	onExit();
	printf("SIL exit ...\n");
	return 1;
}

int CallBackFunction(unsigned char* recvBuffer, short recvBufferSize,void* lpsock)
{

	long Data=0;
	unsigned short usAxisRef;
	usAxisRef = (unsigned short)(*(unsigned short*)(&recvBuffer[12]));
	Data=*((long*)(&recvBuffer[14]));
	MMC_CAN_REPLY_DATA_OUT* pOutParam=(MMC_CAN_REPLY_DATA_OUT*) recvBuffer;


//	printf("***** pOutParam->usFunctionID = %d\n ******\n",pOutParam->usFunctionID);

	switch(pOutParam->usFunctionID)
	{
		case MODBUS_WRITE_EVT:
		{
			printf("***** MODBUS_WRITE_EVT ******\n");
			g_ucModBusEvtFlag = 1;
		}
			break;
	}
	return 1;
}


/**
 * \fn 		sil_system_init()
 * \brief	* Open IPC connection
 * 			* Initialized global parameter array of RT single axis according to axes on system.
 * 			* Initialized instance of MMCPPGlobalRT
 * \param	void
 * \return  void.
 */
void sil_system_init()
{
	unsigned int i;
	MMC_GETAXESNAME_IN st_getAxisNameInParam;
	MMC_GETAXESNAME_OUT st_getAxisNameOutParam;

	MMC_GETCOMMSTATISTICSEX_IN st_getCommStatInParam;
	MMC_GETCOMMSTATISTICSEX_OUT st_getCommStatOutParam;

	g_hConn = g_cConn.ConnectIPCEx(0x7fffffff,(MMC_MB_CLBK)CallBackFunction);

	st_getCommStatInParam.ucSlavesNum = 60;
	for(i = 0; i< 60; i++)
	{
		st_getCommStatInParam.pwSlaveId[i] = i;
	}

	MMC_GetEthercatCommStatistics(g_hConn,&st_getCommStatInParam,&st_getCommStatOutParam);

	GetAxesName(g_hConn,&st_getAxisNameInParam,&st_getAxisNameOutParam);

	for(i = 0; i < NC_NODES_SING_AXIS_NUM; i++)
	{
		if(st_getAxisNameOutParam.pAxesNames[i][0] == 0)
			break;
	}
	g_uiNumOfAxes = i;

	g_cRTSingleAxisArr = new CMMCRTSingleAxis[g_uiNumOfAxes];

	fprintf(stderr,"%s: usNumOfSlaves = %d, g_uiNumOfAxes = %d\n",__func__, st_getCommStatOutParam.usNumOfSlaves, g_uiNumOfAxes);

	for(i = 0; i < g_uiNumOfAxes; i++)
	{
		fprintf(stderr,"%s: pAxesNames[%d] = %s\n", __func__, i, st_getAxisNameOutParam.pAxesNames[i]);
//		g_cRTSingleAxisArr[i + 1].InitAxisData(st_getAxisNameOutParam.pAxesNames[i],g_hConn);
		g_cRTSingleAxisArr[i].InitAxisData(st_getAxisNameOutParam.pAxesNames[i],g_hConn);
	}
	g_cGloblaRT.Init(g_hConn);

	//Get cycle time
	MMC_READBOOLPARAMETER_IN stReadBoolParamInParam;
	MMC_READBOOLPARAMETER_OUT stReadBoolParamOutParam;
	stReadBoolParamInParam.eParameterNumber = MMC_CYCLE_TIME_PARAM;
	stReadBoolParamInParam.iParameterArrIndex = 0;
	stReadBoolParamInParam.ucEnable = 1;
	MMC_GlobalReadBoolParameter(g_hConn,&stReadBoolParamInParam,&stReadBoolParamOutParam);
	g_lCycleTime = stReadBoolParamOutParam.lValue;

	register_exception_handler();

	if (SIL_BASE_VER_EX > g_cGloblaRT.GetSILBAEver())
	{
		printf(ANSI_COLOR_RED     "SIL BASE VERSION %d IS NOT SUPPORTED , PLEASE UPDATE FIRMWARE"     ANSI_COLOR_RESET "\n",SIL_BASE_VER_EX);
		onExit();
		exit(0);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \fn 		stepThreads_init()
 * \brief	stepThreads Initialization
 * \param	void
 * \return  void.
 */
void stepThreads_init()
{
	pthread_attr_t  stepThreadThreadAttr[ELMO_MAX_MULTI_STEPS];
	struct sched_param schedp[ELMO_MAX_MULTI_STEPS];
	args_stepFunc_t args_stepFunc[ELMO_MAX_MULTI_STEPS];
	int	iStepFuncType = 0, flag = 0;

	for (int i = 0 ; i < ELMO_NUM_MULTI_STEPS ; i++)
	{
		switch (iStepFuncType)
		{
			case 0:			// Periodical threads
			{
				switch (i)
				{
					case 0:
						ptrFuncSteps[i] = ELMO_STEPFUNC_1;
						break;
					case 1:
						ptrFuncSteps[i] = ELMO_STEPFUNC_2;
						break;
					case 2:
						ptrFuncSteps[i] = ELMO_STEPFUNC_3;
						break;
					case 3:
						ptrFuncSteps[i] = ELMO_STEPFUNC_4;
						break;
					case 4:
						ptrFuncSteps[i] = ELMO_STEPFUNC_5;
						break;
					case 5:
						ptrFuncSteps[i] = ELMO_STEPFUNC_6;
						break;
					case 6:
						ptrFuncSteps[i] = ELMO_STEPFUNC_7;
						break;
					case 7:
						ptrFuncSteps[i] = ELMO_STEPFUNC_8;
				}

				if(ptrFuncSteps[i] == NULL)
				{
					iStepFuncType++;
				}
				else
				{
					break;
				}

			}	// end Periodical threads

			case 1:		// Event Driven
			{
				switch (i)
				{
					case 0:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_1;
						break;
					case 1:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_2;
						break;
					case 2:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_3;
						break;
					case 3:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_4;
						break;
					case 4:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_5;
						break;
					case 5:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_6;
						break;
					case 6:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_7;
						break;
					case 7:
						ptrFuncSteps[i] = ELMO_EVENTFUNC_8;
				}

				if(ptrFuncSteps[i] == NULL)
				{
					iStepFuncType++;
				}
				else
				{
					stepRate[i] = 0;

					if (flag == 0)
					{
						iEventDrivenOffset = i;		// used in:  sem_post(&stepSem[iEventDrivenOffset + index]);
						flag = 1;
					}

					break;
				}

			}	//end Event Driven

			case 2:			// UDP Event Driven
			{
				switch (i)
				{
					case 0:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_1;
						break;
					case 1:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_2;
						break;
					case 2:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_3;
						break;
					case 3:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_4;
						break;
					case 4:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_5;
						break;
					case 5:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_6;
						break;
					case 6:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_7;
						break;
					case 7:
						ptrFuncSteps[i] = ELMO_UDPEVENTFUNC_8;
				}

				if(ptrFuncSteps[i] == NULL)
				{
					printf("\nptrFuncSteps[%d] is NULL. \33[0;31mTerminating SIL_BASE...\33[0m\n\n", i);
					TerminateApplication(SIGTERM);
					return;
				}

				if(udpEventId[i] >= MAX_UDP_CONN)
				{
					printf("udpEventId[%d] = %d is out of range.  Must be < %d \n", i, udpEventId[i], MAX_UDP_CONN);
				}
				else
				{
					g_saServerList[udpEventId[i]].iEventDriven = i+1;	// udpEventId[i] = 0 to 4 relates to g_saServerList[0] to g_saServerList[4]
					printf("\nudpEventId[%d] = %d  --->  g_saServerList[%d].iEventDriven = %d\n\n", i, udpEventId[i], udpEventId[i], g_saServerList[udpEventId[i]].iEventDriven);
				}

			}	//end UDP Event Driven

		}	//end switch (iStepFuncType)


		if(pthread_attr_init(&stepThreadThreadAttr[i]))
		{
			printf("pthread_attr_init failure for stepThreadThreadAttr[%d]\n", i);
			TerminateApplication(SIGTERM);
			return;
		}

//		if(pthread_attr_setdetachstate(&stepThreadThreadAttr[i], PTHREAD_CREATE_DETACHED))
//		{
//			printf("pthread_attr_setdetachstate failure for stepThreadThreadAttr[%d]\n", i);
//			TerminateApplication(SIGTERM);
//			return;
//		}
		printf("Step Thread Create\n");
		args_stepFunc[i].stepIndex = i;
		if(pthread_create(&stepThread[i], &stepThreadThreadAttr[i], StepThreadFunc, (void*)&args_stepFunc[i]))
		{
			printf("Unable to create thread for stepThread[%d]\n", i);
			TerminateApplication(SIGTERM);
			return;
		}

		schedp[i].sched_priority = 89 + ELMO_NUM_MULTI_STEPS - i;
		if(pthread_setschedparam(stepThread[i], SCHED_FIFO, &schedp[i]))
		{
			printf("pthread_attr_setdetachstate failure for stepThread[%d]\n", i);
			TerminateApplication(SIGTERM);
			return;
		}
	}	// end for loop
}


/**
 * \fn 		start_syncfunc()
 * \brief	start sync time function
 * \param	void
 * \return  void.
 */
void start_syncfunc()
{
	unsigned short usSYNCTimerTime;
	int rc;
	MMC_READBOOLPARAMETER_IN stReadBoolParamInParam;
	MMC_READBOOLPARAMETER_OUT stReadBoolParamOutParam;
	stReadBoolParamInParam.eParameterNumber = MMC_CYCLE_TIME_PARAM;
	stReadBoolParamInParam.iParameterArrIndex = 0;
	stReadBoolParamInParam.ucEnable = 1;
	rc = MMC_GlobalReadBoolParameter(g_hConn,&stReadBoolParamInParam,&stReadBoolParamOutParam);

//	if (rc< NC_OK)
//	{
//		printf("rc = %d\n",rc);
////		goto start_sync_err;
//	}
//	else
//	{
//		printf(ANSI_COLOR_GREEN     "SIL_TS = %lf MAESTRO CYCLE_TIME: %ld"     ANSI_COLOR_RESET "\n",(SIL_TS * 1000000),stReadBoolParamOutParam.lValue);
//	}
//
//	if((SIL_TS * 1000000) < stReadBoolParamOutParam.lValue)
//	{
//		printf(ANSI_COLOR_GREEN     "SIL_TS INVALID EXIT"     ANSI_COLOR_RESET "\n");
//		onExit();
//		exit(0);
//	}
//
//	usSYNCTimerTime = (SIL_TS * 1000000) / (stReadBoolParamOutParam.lValue);

//	printf(ANSI_COLOR_GREEN     "usSYNCTimerTime: %d"     ANSI_COLOR_RESET "\n",usSYNCTimerTime);
	MMC_SetRTUserCallback(g_hConn,1);
	MMC_CreateSYNCTimer(g_hConn ,OnSyncFunc,1);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \fn 		OnSyncFunc()
 * \brief	Called each cycle
 * \param	void
 * \return  void.
 */
int OnSyncFunc()
{
	static int   taskCounter[ELMO_MAX_MULTI_STEPS] = {0};

	//	BusDataExchange();
	//	call MATLAB step
	//	SIL_InMessage	SILInpMsgStruct	;
	//	SIL_OutMessage	SILOutMsgStruct	;

	// If requested, Do Cycle Time Measurements
	if(g_ucAllowTimeMeasurement)
	{
		static int flag = 0;

		//lock start time
//		gettimeofday(&g_tv_prev, NULL);
//	    timespec_get(&g_ts_OnSyncFunc, TIME_UTC);
	    clock_gettime(CLOCK_BOOTTIME, &g_ts_OnSyncFunc);

	    if(flag)
	    {
			g_ulElapsedTime =  (g_ts_OnSyncFunc.tv_sec - g_ts_OnSyncFunc_prev.tv_sec) * 1E9;    // sec to ns
		    g_ulElapsedTime += (g_ts_OnSyncFunc.tv_nsec - g_ts_OnSyncFunc_prev.tv_nsec) ;		// add the ns
	    }
	    else
	    {
	    	flag = 1;
	    }

	    Elmo_Write_SIL_DATA_Long(9, g_ulElapsedTime);

	    g_ts_OnSyncFunc_prev.tv_sec = g_ts_OnSyncFunc.tv_sec;
	    g_ts_OnSyncFunc_prev.tv_nsec = g_ts_OnSyncFunc.tv_nsec;
	}
	// Get the Cycle Counter for the OverFlow Calculations
	g_ulCycleCountPrev = g_cGloblaRT.GetCycleCounter();

	// Call teh MATLAB Step Function
	ELMO_STEP

	// Check Overflow
	g_ulCycleCountCurrent = g_cGloblaRT.GetCycleCounter();
	if ( (g_ulCycleCountCurrent - g_ulCycleCountPrev) )
	{
		g_ulCycleOverFlow++;
	}
	g_ulCycleCountPrev = g_ulCycleCountCurrent;

 	// Calculate Cycle Time Measurements
	if(g_ucAllowTimeMeasurement)
	{
		//lock end time time
		//gettimeofday(&g_tv_curr, NULL);
		clock_gettime(CLOCK_BOOTTIME, &g_ts_ELMO_STEP_end);

	    // compute  the elapsed time in millisec
	    // g_dElapsedTime = (g_tv_curr.tv_sec - g_tv_prev.tv_sec) * 1000.0;     // sec to ms
	    // g_dElapsedTime += (g_tv_curr.tv_usec - g_tv_prev.tv_usec) / 1000.0;  // us to ms
//		g_ulElapsedTime =  (g_ts_ELMO_STEP_end.tv_sec - g_ts_OnSyncFunc.tv_sec) * 1E9; 	    // sec to ns
//	    g_ulElapsedTime += (g_ts_ELMO_STEP_end.tv_nsec - g_ts_OnSyncFunc.tv_nsec) ;			// add the ns

		// Lock max elapsed time
	    g_ulElapsedTimeMax = (g_ulElapsedTimeMax < g_ulElapsedTime ) ? g_ulElapsedTime : g_ulElapsedTimeMax;

	    g_ulCycleCountElTime++;
	    //Calculate avg
	    if(g_ulCycleCountElTime <= 128)
	    {
            g_ulSumElapsedTime += g_ulElapsedTime;
            g_ulAvgElapsedTime = g_ulSumElapsedTime / g_ulCycleCountElTime;
	    }
	    else
	    {
            g_ulSumElapsedTime -= g_ulAvgElapsedTime;
            g_ulSumElapsedTime += g_ulElapsedTime;
            g_ulAvgElapsedTime = (g_ulSumElapsedTime >> 7);
	    }
	}

	for (int i = 0 ; i < ELMO_NUM_MULTI_STEPS ; i++)
	{
		if(ptrFuncSteps[i] != NULL)
		{
			if (stepRate[i] > 0)		// otherwise, it means event driven
			{
				if (++taskCounter[i] == stepRate[i])
				{
					taskCounter[i] = 0;
					sem_post(&stepSem[i]);
				}
			}
			else if((stepRate[i] == 0) && (++taskCounter[i] == 7))   // Event Driven Step: for debugging only !!!
			{
				taskCounter[i] = 0;
				Elmo_SetEvent(i - iEventDrivenOffset);
			}
		}
	}

	return 0;
}

int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus)
{
	printf("MMCPPExitClbk: Run time Error in function %s, axis ref=%d, err=%d, status=%d, bye\n", msg, usAxisRef, sErrorID, usStatus);
	return 0;
}

void register_exception_handler()
{
	CMMCPPGlobal::Instance()->RegisterRTE(OnRunTimeError,false);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \fn 		api_execution()
 * \brief	start a thread to execute API
 * \param	void
 * \return  void.
 */
void api_execution()
{
	pthread_attr_t hApiExeThreadAttr;
	pthread_t hApiExerThread;

	if(pthread_attr_init(&hApiExeThreadAttr))
	{
		printf("pthread_attr_init failure\n");
	}
//	if(pthread_attr_setdetachstate(&hApiExeThreadAttr, PTHREAD_CREATE_DETACHED))
//	{
//		printf("pthread_attr_setdetachstate failure\n");
//	}
	if(pthread_create(&hApiExerThread, &hApiExeThreadAttr, APIExeThreadFuncReq, NULL) != 0)
	{
		printf("Unable to create APIExeThreadFunc thread\n");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \fn 		modbus_data_exhange()
 * \brief	start a thread to execute API
 * \param	void
 * \return  void.
 */
void modbus_data_exhange()
{
	pthread_attr_t hModbusExeThreadAttr;
	pthread_t hModbusExeThread;

	if(pthread_attr_init(&hModbusExeThreadAttr))
	{
		printf("pthread_attr_init failure\n");
	}
//	if(pthread_attr_setdetachstate(&hModbusExeThreadAttr, PTHREAD_CREATE_DETACHED))
//	{
//		printf("pthread_attr_setdetachstate failure\n");
//	}
	if(pthread_create(&hModbusExeThread, &hModbusExeThreadAttr, ModbusxeThreadFuncReq, NULL) != 0)
	{
		printf("Unable to create APIExeThreadFunc thread\n");
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn set_bits
/// \brief The function sets specific bits atomically.
/// \param	unsigned int mask - Bit mask.
///         unsigned int *addr - Target parameter address.
/// \return	N/A
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void atomic_set_bits(unsigned int mask, unsigned int *addr)
{
	unsigned long tmp;
	unsigned int old;

	__asm__ __volatile__("@ set_bits\n"
"1:	ldrex %0, [%3]\n"
"	orr %0, %2\n"
"	strex %1, %0, [%3]\n"
"	teq %1, #0\n"
"	bne 1b"
	: "=&r" (old), "=&r" (tmp)
	: "r" (mask), "r" (addr)
	: "cc");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn atomic_clear_bit
/// \brief The function clears specific bit atomically.
/// \param	int nr - Target bit index.
///         volatile unsigned int *addr - Target parameter address.
/// \return	N/A
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void atomic_clear_bit(int nr, volatile unsigned int *addr)
{
	/*
	unsigned int old;
	unsigned int mask = MMC_BITOP_MASK(nr);
	unsigned int *p = ((unsigned int *)addr) + MMC_BITOP_WORD(nr);

	__asm__ __volatile__(
"1:	lwarx %0,0,%3	# clear_bit\n\
	andc	%0,%0,%2\n\
	stwcx. %0,0,%3\n\
	bne-	1b"
	: "=&r" (old), "+m" (*p)
	: "r" (mask), "r" (p)
	: "cc" );
	*/
}


void PowerOffReq(int iIndex)
{
	atomic_set_bits(NC_SIL_POWER_OFF_REQ,&(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence));
	g_cRTSingleAxisArr[iIndex].PowerOff();
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiApiReq &= ~NC_SIL_POWER_OFF_REQ;
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence &= ~NC_SIL_POWER_OFF_REQ;
}

void PowerOnReq(int iIndex)
{
	atomic_set_bits(NC_SIL_POWER_ON_REQ,&(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence));

	g_cRTSingleAxisArr[iIndex].PowerOn();
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiApiReq &= ~NC_SIL_POWER_ON_REQ;
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence &= ~NC_SIL_POWER_ON_REQ;
}


void MoveReq(int iIndex)
{
	atomic_set_bits(NC_SIL_MOVE_REQ,&(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence));
	g_cRTSingleAxisArr[iIndex].MoveRelative(g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MOTION_DPOS_IDX],
			g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MOTION_VEL_IDX],
			g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MOTION_AC_IDX],
			g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MOTION_AC_IDX],
			g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MOTION_JERK_IDX],
			(MC_BUFFERED_MODE_ENUM)g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MOTION_BUFF_MOD_IDX]);
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiApiReq &= ~NC_SIL_MOVE_REQ;
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence &= ~NC_SIL_MOVE_REQ;
}



void ChgOpModReq(int iIndex)
{
	switch(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiInternalState)
	{
	case INTERNAL_STATE_INIT:
	{
		atomic_set_bits(NC_SIL_CHG_MOD_REQ,&(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence));
		g_cRTSingleAxisArr[iIndex].SetOpMode((OPM402)g_cRTSingleAxisArr[iIndex].m_stAPICom.data[OPMOD_REQ_IDX]);

		g_cRTSingleAxisArr[iIndex].m_stAPICom.uiInternalState = INTERNAL_STATE_RUN;
	}
	break;
	//
	case INTERNAL_STATE_RUN:
	{
		if(g_cRTSingleAxisArr[iIndex].GetOpMode() == (OPM402)g_cRTSingleAxisArr[iIndex].m_stAPICom.data[OPMOD_REQ_IDX])
		{
			g_cRTSingleAxisArr[iIndex].m_stAPICom.uiInternalState = INTERNAL_STATE_INIT;

			g_cRTSingleAxisArr[iIndex].m_stAPICom.uiApiReq &= ~NC_SIL_CHG_MOD_REQ;
			g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence &= ~NC_SIL_CHG_MOD_REQ;
		}
	}
	break;
	//
	default:
		break;
	}
	//end switch
}

void ResetReq(int iIndex)
{
	atomic_set_bits(NC_SIL_RESET_REQ,&(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence));
	g_cRTSingleAxisArr[iIndex].Reset();
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiApiReq &= ~NC_SIL_RESET_REQ;
	g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence &= ~NC_SIL_RESET_REQ;
}

void HomeReq(int iIndex)
{
	switch(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiInternalState)
	{
	case INTERNAL_STATE_INIT:
	{
		MMC_HOMEDS402EX_IN stHomeDS402Params;
		stHomeDS402Params.dbPosition 					= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_POS_IDX];
		stHomeDS402Params.dbDetectionVelocityLimit 		= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_DETECT_VEL_LIM_IDX];
		stHomeDS402Params.fAcceleration 				= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_ACC_IDX];
		stHomeDS402Params.fDistanceLimit 				= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_DIST_LIM_IDX];
		stHomeDS402Params.fTorqueLimit 					= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_TORQ_LIM_IDX];
		stHomeDS402Params.fVelocityHi 					= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_VEl_HI_IDX];
		stHomeDS402Params.fVelocityLo					= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_VEL_LOW_IDX];
		stHomeDS402Params.uiHomingMethod 				= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_METH_IDX];
		stHomeDS402Params.uiTimeLimit 					= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_TIME_LIM_IDX];
		stHomeDS402Params.uiDetectionTimeLimit			= 	g_cRTSingleAxisArr[iIndex].m_stAPICom.data[MB_CMD_HOME_DETECT_VEL_LIM_IDX];
		stHomeDS402Params.eBufferMode 					= 	(MC_BUFFERED_MODE_ENUM)1;
		stHomeDS402Params.ucExecute 					= 	1;

		atomic_set_bits(NC_SIL_HOME_REQ,&(g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence));
		g_cRTSingleAxisArr[iIndex].HomeDS402Ex(stHomeDS402Params);

		g_cRTSingleAxisArr[iIndex].m_stAPICom.uiInternalState = INTERNAL_STATE_RUN;
	}
	break;
	//
	case INTERNAL_STATE_RUN:
	{
		if(g_cRTSingleAxisArr[iIndex].ReadStatus() & (NC_AXIS_ERROR_MASK | NC_AXIS_STAND_STILL_MASK) )
		{
			g_cRTSingleAxisArr[iIndex].m_stAPICom.uiInternalState = INTERNAL_STATE_INIT;

			g_cRTSingleAxisArr[iIndex].m_stAPICom.uiApiReq &= ~NC_SIL_HOME_REQ;
			g_cRTSingleAxisArr[iIndex].m_stAPICom.uiIsInSequence &= ~NC_SIL_HOME_REQ;
		}
	}
	break;
	//
	default:
		break;
	}
	//end switch
}

void DummyReq(int iIndex)
{
	return;
}
//////////////////////////////////////////////////////////////////
/**
 * \fn 		APIExeThreadFuncReq()
 * \brief	Execute API for each axis in the system according to requested command
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * APIExeThreadFuncReq(void *)
{
	unsigned int iAxisIndex = 0;
	// unsigned int uiIndex  = 0;
	while(!g_iTerminate)
	{
		for(iAxisIndex = 0; iAxisIndex < g_uiNumOfAxes; iAxisIndex++)
		{
			if(g_cRTSingleAxisArr[iAxisIndex].m_stAPICom.uiApiReq)
			{
				req_hand_func_array[ffs(g_cRTSingleAxisArr[iAxisIndex].m_stAPICom.uiApiReq)](iAxisIndex);
			}
		}
	}
	printf("APIExeThreadFunc Exit ...\n");
	return NULL;
}


//////////////////////////////////////////////////////////////////
/**
 * \fn 		ModbusxeThreadFuncReq()
 * \brief	Execute Modbus Data Exchange
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void * ModbusxeThreadFuncReq(void *)
{
	while(!g_iTerminate)
	{
		BusDataExchange();
	}
	printf("ModbusxeThreadFuncReq Exit ...\n");
	return NULL;
}


//////////////////////////////////////////////////////////////////
/**
 * \fn 		StepThreadFunc(void* arguments)
 * \brief	Execute Multi Step thread (same thread for all steps):
 * \param	void
 * \return  void.
 */
///////////////////////////////////////////////////////////////////
void* StepThreadFunc(void* arguments)
{
	args_stepFunc_t *args = (args_stepFunc_t*)arguments;
	int stepIndex = args->stepIndex;

	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, NULL);

	while(!g_iTerminate)
	{
		pthread_testcancel();

		sem_wait(&stepSem[stepIndex]);	// wait for semaphore

		// If requested, Do Cycle Time Measurements
		if(g_ucAllowTimeMeasurement)
		{
			//lock start time
			gettimeofday(&_g_tv_prev[stepIndex], NULL);
		}
		// Get the Cycle Counter for the OverFlow Calculations
		_g_ulCycleCountPrev[stepIndex] = g_cGloblaRT.GetCycleCounter();


		ptrFuncSteps[stepIndex]();		// Call Step Function


		// Check Overflow
		_g_ulCycleCountCurrent[stepIndex] = g_cGloblaRT.GetCycleCounter();
		if ( (_g_ulCycleCountCurrent[stepIndex] - _g_ulCycleCountPrev[stepIndex]) )
		{
			_g_ulCycleOverFlow[stepIndex]++;
		}
		_g_ulCycleCountPrev[stepIndex] = _g_ulCycleCountCurrent[stepIndex];

	 	// Calculate Cycle Time Measurements
		if(g_ucAllowTimeMeasurement)
		{
			//lock end time time
			gettimeofday(&_g_tv_curr[stepIndex], NULL);

		    // compute  the elapsed time in millisec
			_g_ulElapsedTime[stepIndex] =  (_g_tv_curr[stepIndex].tv_sec - _g_tv_prev[stepIndex].tv_sec) * 1000000L;    // sec to us
		    _g_ulElapsedTime[stepIndex] += (_g_tv_curr[stepIndex].tv_usec - _g_tv_prev[stepIndex].tv_usec) ;			// add the us
			// Lock max elapsed time
		    _g_ulElapsedTimeMax[stepIndex] = (_g_ulElapsedTimeMax[stepIndex] < _g_ulElapsedTime[stepIndex] ) ? _g_ulElapsedTime[stepIndex] : _g_ulElapsedTimeMax[stepIndex];

		    _g_ulCycleCountElTime[stepIndex]++;
		    //Calculate avg
		    if(_g_ulCycleCountElTime[stepIndex] <= 128)
		    {
	            _g_ulSumElapsedTime[stepIndex] += _g_ulElapsedTime[stepIndex];
	            _g_ulAvgElapsedTime[stepIndex] = _g_ulSumElapsedTime[stepIndex] / _g_ulCycleCountElTime[stepIndex];
		    }
		    else
		    {
	            _g_ulSumElapsedTime[stepIndex] -= _g_ulAvgElapsedTime[stepIndex];
	            _g_ulSumElapsedTime[stepIndex] += _g_ulElapsedTime[stepIndex];
	            _g_ulAvgElapsedTime[stepIndex] = (_g_ulSumElapsedTime[stepIndex] >> 7);
		    }
		}
	}
	printf("StepThreadFunc #%d Exit ...   ", stepIndex + 1);
	if(g_ucAllowTimeMeasurement)
	{
		printf("MaxElapsedTime = %lu [uS], AvgElapsedTime = %lu [uS]\n\n", _g_ulElapsedTimeMax[stepIndex], _g_ulAvgElapsedTime[stepIndex]);
	}
	else
	{
		printf("\n");
	}

	return NULL;
}


void StartModBus()
{
	g_cHostComm.MbusStartServer(g_hConn,1);
}

void BusDataExchange()
{

	if(g_ucModBusEvtFlag)
	{
		g_cHostComm.MbusReadHoldingRegisterTable(MB_START_REF,MB_REF_CNT,g_stReadHoldingReg);

		switch(g_stReadHoldingReg.regArr[MB_FUNC_IDX])
		{
		case AXIS_ENABLE_CMD:
		{
//			printf("AXIS_ENABLE_CMD\n");
			AxisEnable(g_stReadHoldingReg.regArr[MB_AXIS_IDX], g_stReadHoldingReg.regArr[MB_CMD_ENABLE_VAL_IDX]);
		}
			break;
		case AXIS_HOME_CMD:
		{
//			printf("AXIS_HOME_CMD\n");
			HomeData stHomeData;
			stHomeData.dPosition 			= g_stReadHoldingReg.regArr[MB_CMD_HOME_POS_IDX];
			stHomeData.uiHomingMethod 		= g_stReadHoldingReg.regArr[MB_CMD_HOME_METH_IDX];
			stHomeData.fVelHi 				= g_stReadHoldingReg.regArr[MB_CMD_HOME_VEl_HI_IDX];   //speed search for switch
			stHomeData.fVelLo 				= g_stReadHoldingReg.regArr[MB_CMD_HOME_VEL_LOW_IDX];   //speed search for zero
			stHomeData.fAcc 				= g_stReadHoldingReg.regArr[MB_CMD_HOME_ACC_IDX];
			stHomeData.fDistanceLimit 		= g_stReadHoldingReg.regArr[MB_CMD_HOME_DIST_LIM_IDX];
			stHomeData.fDetectVelLimit 		= g_stReadHoldingReg.regArr[MB_CMD_HOME_DETECT_VEL_LIM_IDX];
			stHomeData.fTorqueLimit 		= g_stReadHoldingReg.regArr[MB_CMD_HOME_TORQ_LIM_IDX];
			stHomeData.uiTimeLimit 			= g_stReadHoldingReg.regArr[MB_CMD_HOME_TIME_LIM_IDX];
			stHomeData.uiDetectTimeLimit 	= g_stReadHoldingReg.regArr[MB_CMD_HOME_DETECT_TIME_LIM_IDX];
			AxisHome(g_stReadHoldingReg.regArr[MB_AXIS_IDX], &stHomeData);
		}
		break;
		case AXIS_CHGOP_CMD:
		{
//			printf("AXIS_CHGOP_CMD\n");
			AxisChangeOpMode(g_stReadHoldingReg.regArr[MB_AXIS_IDX], g_stReadHoldingReg.regArr[MB_CMD_CHOP_VAL_IDX]);
		}
		break;
		case AXIS_MOTION_CMD:
		{
//			printf("AXIS_MOTION_CMD\n");
			MotionData stMotioDataStruct;

			stMotioDataStruct.lMotionMode 	= g_stReadHoldingReg.regArr[MB_CMD_MOTION_MODE_IDX];
			stMotioDataStruct.dPosition 	= g_stReadHoldingReg.regArr[MB_CMD_MOTION_DPOS_IDX];
			stMotioDataStruct.fVel 			= g_stReadHoldingReg.regArr[MB_CMD_MOTION_FVEL_IDX];
			stMotioDataStruct.fAcc 			= g_stReadHoldingReg.regArr[MB_CMD_MOTION_FAC_IDX];
			stMotioDataStruct.fJrk 			= g_stReadHoldingReg.regArr[MB_CMD_MOTION_FJERK_IDX] ;

			AxisMotion(g_stReadHoldingReg.regArr[MB_AXIS_IDX],&stMotioDataStruct);
		}
		break;
		case AXIS_CLR_ERR_CMD:
		{
			AxisClearError(g_stReadHoldingReg.regArr[MB_AXIS_IDX]);
		}
		break;
		default:
			break;
		}
		g_ucModBusEvtFlag = 0;
//		double x;
//		x = g_stReadHoldingReg.regArr[1];
//    capi_ModifyParameter(g_stModifyParamData[g_stReadHoldingReg.regArr[0]]._param,
//    			/*paramAddress*//*(void *)(&(g_stReadHoldingReg.regArr[1]))*/(void *)&x,
//    			g_stModifyParamData[g_stReadHoldingReg.regArr[0]].orientation,
//    			g_stModifyParamData[g_stReadHoldingReg.regArr[0]].dims,
//    			g_stModifyParamData[g_stReadHoldingReg.regArr[0]].nDims,
//    			g_stModifyParamData[g_stReadHoldingReg.regArr[0]].slDataType,
//    			g_stModifyParamData[g_stReadHoldingReg.regArr[0]].isComplex);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

void sigAction()
{
	struct sigaction stSigAction;

	memset(&stSigAction, 0, sizeof(stSigAction));
	// Whenever a signal is caught, call TerminateApplication function
	stSigAction.sa_handler = &TerminateApplication;

	sigaction(SIGINT, &stSigAction, NULL);
	sigaction(SIGTERM, &stSigAction, NULL);
	sigaction(SIGABRT, &stSigAction, NULL);
	sigaction(SIGQUIT, &stSigAction, NULL);
}

//////////////////////////////////////////////////////////////////////
void TerminateApplication(int iSigNum)
{
	printf("In Terminate Application ...\n");

//	sigignore(SIGALRM);	// This API is obsolete: new applications should use the POSIX signal API (sigaction & sigprocmask)

	sigset_t sigS;
	sigemptyset (&sigS);
	sigaddset(&sigS, SIGALRM);
	sigprocmask(SIG_BLOCK, &sigS, NULL);

	g_iTerminate = 1;

	// Wait till other threads exit properly.

	usleep(SLEEP_T);
	return;
}

void onExit()
{
	if(g_ucAllowTimeMeasurement)
	{
		printf("MainStep:  CycleOverFlow = %lu, MaxElapsedTime = %lu [nS], AvgElapsedTime = %lu [nS]\n\n", g_ulCycleOverFlow, g_ulElapsedTimeMax, g_ulAvgElapsedTime);
	}

	for (int i = 0 ; i < ELMO_NUM_MULTI_STEPS ; i++)
	{
		if(ptrFuncSteps[i] != NULL)
		{
			if(g_ucAllowTimeMeasurement)
			{
				printf("stepThread #%d Exit...  CycleOverFlow = %lu, MaxElapsedTime = %lu [nS], AvgElapsedTime = %lu [nS]\n\n", i+1, _g_ulCycleOverFlow[i], _g_ulElapsedTimeMax[i], _g_ulAvgElapsedTime[i]);
			}

			sem_post(&stepSem[i]);			// occasionally causes segmentaion fault
			pthread_cancel(stepThread[i]);
			pthread_join(stepThread[i], NULL);
		}
	}

	MMC_DestroySYNCTimer(g_hConn);
	usleep(g_lCycleTime);

	ELMO_TERMINATE

#ifdef RTW_MATLOGGING_H__
			if (g_iLogMatFile)
			{
				ELMO_LOGGER
				g_iLogMatFile = 0;
			}
#endif

	g_cConn.CloseConnection();
	svc_exit();
	delete[] g_cRTSingleAxisArr;
	//close UDP sockets.
	for(int i = 0; i < MAX_UDP_CONN ;i++)
	{
		g_cUDPServer[i].Close();
		g_cUDPClient[i].Close();
	}

	//close TCP sockets.
	for(int i = 0; i < MAX_UDP_CONN ;i++)
	{
		g_cTCPServer[i].Close();
		g_cTCPClient[i].Close();
	}
}

