/*
============================================================================
 Name : PID_Sample.cpp
 Author  :	Elmo Motion Control
 Version :
 Description : 	GMAS C/C++ project header file for Template
============================================================================
*/

/*
============================================================================
 Project general functions prototypes
============================================================================
*/
void MainInit();
void MainClose();
int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus) ;
void TerminateApplication(int iSigNum);
void Emergency_Received(unsigned short usAxisRef, short sEmcyCode) ;;
int  CallbackFunc(unsigned char* recvBuffer, short recvBufferSize,void* lpsock);
int SILCallBackFun(void);

/*
============================================================================
 General constants
============================================================================
*/
#define 	MAX_AXES				3		// number of Physical axes in the system. TODO Update MAX_AXES accordingly
/*
============================================================================
 Application global variables
============================================================================
*/

//double dSetPoint[3];
//double dPositionValue[3];
//double dKp = 2;
//double dKi = 0;
//double dKd = 0;
//double dDt = 0.00025;
//double dSetPointMin = -10000;
//double dSetPointMax = 10000;
//double dOutLimitMin = -1.4;
//double dOutLimitMax = 1.4;
//double dOutRatio = 10000.0;
//bool bAbortCmd;
//double dOutput1,dOutput2,dOutput3;
//bool bDone;
//bool bBusy;
//bool bError;
//short	usErrID;


struct timeval tm,tt;
bool b;


double Kp = 0;
double Ki = 0;
double target_pos = 0; // unit: cnt
double pos_err = 0; // unit: cnt
double output = 0;
double velocity_limit=500000; // unit: cnt/s






int 	giTerminate;		// Flag to request program termination
// 	Examples for data read from the GMAS core about the X, Y drives
int 	giXStatus ;
int 	giYStatus ;
//
/*
============================================================================
 Global structures for Elmo's Function Blocks
============================================================================
*/
MMC_CONNECT_HNDL gConnHndl ;					// Connection Handle
CMMCConnection cConn ;
CMMCSingleAxis a1,a2 ;							// TODO : Update the names and number of the axes in the system
MMC_MOTIONPARAMS_SINGLE 	stSingleDefault ;	// Single axis default data

CMMCRTSingleAxis 			cRTaxis[3];
MMC_SYNC_TIMER_CB_FUNC 		pSyncFunc;
