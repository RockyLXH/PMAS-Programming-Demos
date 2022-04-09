/*
============================================================================
 Name : ecam_tests2.cpp
 Author  :	Elmo Motion Control
 Version :
 Description : 	GMAS C/C++ project header file for Template
============================================================================
*/
#ifdef __EXTERN__
#define EXT extern
#else
#define EXT
#endif

/* ============================================================================
 Project general functions prototypes
============================================================================ */
void MainInit();
void MainClose();
int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus) ;
void TerminateApplication(int iSigNum);
void Emergency_Received(unsigned short usAxisRef, short sEmcyCode) ;;
int  CallbackFunc(unsigned char* recvBuffer, short recvBufferSize,void* lpsock);

/* ============================================================================
 General constants
============================================================================ */
#define 	MAX_AXES				3		// number of Physical axes in the system. TODO Update MAX_AXES accordingly
/*
============================================================================
 Application global variables
============================================================================
*/
EXT int 	giTerminate;		// Flag to request program termination
EXT int 	giXStatus ;
EXT int 	giYStatus ;
//
/*
============================================================================
 Global structures for Elmo's Function Blocks
============================================================================
*/
					// Connection Handle
EXT MMC_MOTIONPARAMS_SINGLE 	stSingleDefault ;	// Single axis default data

EXT MMC_CONNECT_HNDL			gConnHndl;
EXT MMC_CONNECTION_PARAM_STRUCT conn_param;
EXT CMMCConnection 				cConn;
EXT CMMCSingleAxis 				cAxis[MAX_AXES];
