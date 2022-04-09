/*
============================================================================
 Name : ecam_tests2.cpp
 Author :	Elmo Motion Control
 Version :	1.00
 Description : The following example supports the following functionalities:

- Power On.
- Power Off.

 The program works with 2 axes - a01 and a02.
 The program works with CAN and EtherCAT.


============================================================================ */
#define __EXTERN__ 1


#include "mmc_definitions.h"
#include "mmcpplib.h"
#include "header.h"		// Application header file.
#include <iostream>
#include <sys/time.h>			// For time structure
#include <signal.h>				// For Timer mechanism

extern int ECAM_File_Test1();
extern int ECAM_File_Test2();
extern int ECAM_File_Test3();
extern int ECAM_File_Test4();
extern int ECAM_File_Test5();
extern int ECAM_File_Test11();

extern int ECAM_MEM_Test1();
extern int ECAM_MEM_Test2();
/* ============================================================================
 Function:				main()
 Description:

 The main function of this sample project.
============================================================================*/
int main()
{
	try
	{
		//
		// Initialize system, axes and all needed initializations
		MainInit();

		ECAM_File_Test1();

		MainClose();
		return 1;

	}
	catch(CMMCException& exception)
	{
		printf("Exception in function %s, axis ref=%s, err=%d, status=%d, bye\n", exception.what(), exception.axisName(), exception.error(), exception.status());
		MainClose();
		exit(0);
	}
}


/* ============================================================================
 Function:				MainInit()
 Description:

 Initilaize the system, including axes, communication, etc.
============================================================================ */
void MainInit()
{
	//
	// Here will come initialization code for all system, axes, communication etc.
	//
	struct sigaction stSigAction;
	//
	// Init the sigAction structure.
	memset(&stSigAction, 0, sizeof(stSigAction));
	// Initialized case of CTRL+C.
	sigaction(SIGINT, &stSigAction, NULL);
	//
	// InitializeCommunication to the GMAS:
	gConnHndl = cConn.ConnectIPCEx(0x7fffffff,(MMC_MB_CLBK)NULL) ;
	//
	// 	Enable throw feature.
	CMMCPPGlobal::Instance()->SetThrowFlag(true);

	// Initialize default parameters. This is not a must. Each parameter may be initialized individually.
	stSingleDefault.fEndVelocity	= 0 ;
	stSingleDefault.dbDistance 		= 100000 ;
	stSingleDefault.dbPosition 		= 0 ;
	stSingleDefault.fVelocity 		= 100000 ;
	stSingleDefault.fAcceleration 	= 1000000 ;
	stSingleDefault.fDeceleration 	= 1000000 ;
	stSingleDefault.fJerk 			= 20000000 ;
	stSingleDefault.eDirection 		= MC_POSITIVE_DIRECTION ;
	stSingleDefault.eBufferMode 	= MC_BUFFERED_MODE ;
	stSingleDefault.ucExecute 		= 1 ;

	cAxis[0].InitAxisData("a01",gConnHndl);
	cAxis[1].InitAxisData("a02",gConnHndl);
	cAxis[2].InitAxisData("a03",gConnHndl);
	return;
}


/* ============================================================================
 Function:				MainClose()
 Description:

 Close all that needs to be closed before the application progra, is
 terminated.
============================================================================ */
void MainClose()
{
//
//	Here will come code for all closing processes
//
	MMC_CloseConnection(gConnHndl) ;
	return;
}


