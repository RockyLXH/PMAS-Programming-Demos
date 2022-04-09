/*
============================================================================
 Name : PID_Sample.cpp
 Author :	Elmo Motion Control
 Version :	1.00
 Description : The following example supports the following functionalities:

- Power On.
- Power Off.

 The program works with 2 axes - a01 and a02.
 The program works with CAN and EtherCAT.

============================================================================
*/
#include "mmc_definitions.h"
#include "mmcpplib.h"
#include <iostream>
#include <sys/time.h>			// For time structure
#include <signal.h>				// For Timer mechanism
#include "PID_Sample.h"		// Application header file.
#include <chrono>

using namespace std;

/*
 * Using Timer in function for measuring the time elapsed
 */
struct Timer
{
	chrono::time_point<std::chrono::steady_clock> start;

	Timer()
	{
		start = chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		auto end = chrono::high_resolution_clock::now();
		auto duration = end - start;
		cout << duration.count()*1000.0f<< " ms\n";
	}
};
//
//void func()
//{
//	// add 'Timer timer' for measuring the whole function duration.
//	Timer timer;
//
//	for (int i=0; i<100; i++)
//		cout << "hello" << endl;
//}

/*
============================================================================
 Function:				main()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				04/10/2015
 Modifications:			N/A

 Description:

 The main function of this sample project.
============================================================================
*/


int main()
{
	try
	{
		//
		// Initialize system, axes and all needed initializations
		MainInit();

		for(int i = 0; i<1; i++)
		{

			// 0 - NC profiler
			// 1 - 0;
			// 2 - User
			cRTaxis[i].SetBoolParameter(0, MMC_UCUSER60FF_SRC, 0);
			//cRTaxis[i].SetBoolParameter(0,MMC_UCUSER6071_SRC,0) ;

			// set a01 to CSP mode
			cRTaxis[i].SetOpMode(OPM402_CYCLIC_SYNC_POSITION_MODE);
			//cRTaxis[i].SetOpMode(OPM402_CYCLIC_SYNC_TORQUE_MODE);

			while(cRTaxis[i].GetOpMode() != OPM402_CYCLIC_SYNC_POSITION_MODE)
			{
				usleep(100);
			}

			//set pos to 0

//			cRTaxis[i].GetActualCurrent();
//			cRTaxis[i].GetActualTorque();

			cRTaxis[i].PowerOn();

			while(!(cRTaxis[0].ReadStatus() & NC_AXIS_STAND_STILL_MASK));

			cRTaxis[0].MoveAbsolute(5000, 10000.0);

			while(!(cRTaxis[0].ReadStatus() & NC_AXIS_STAND_STILL_MASK));

		}

		MMC_DestroySYNCTimer(gConnHndl);


		for(int i=0;i<1;i++)
		{
			//cRTaxis[i].SetUser6071(0.0);
			cRTaxis[0].SetUser607A(5000);
			cRTaxis[i].SetBoolParameter(2,MMC_UCUSER607A_SRC,0);
			//cRTaxis[i].SetBoolParameter(2,MMC_UCUSER6071_SRC,0);
			//cRTaxis[0].SetUser6071(0.2);
		}



		MMC_CreateSYNCTimer(gConnHndl,SILCallBackFun,10);

		MMC_SetRTUserCallback(gConnHndl, 1); //set user call-back function to highest priority -> 1.

		while(1)
		{
			usleep(100);
		}

		MainClose();
		//
		// Terminate the application program back to the Operating System
		return 1;

	}
	catch(CMMCException& exception)
	{
		printf("Exception in function %s, axis ref=%s, err=%d, status=%d, bye\n", exception.what(), exception.axisName(), exception.error(), exception.status());
		MainClose();
		exit(0);
	}
	catch (...)
	{
		std::cerr << "Unknown exception caught\n";
		MainClose();
		exit(0);
	}
}
/*
============================================================================
 Function:				MainInit()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Initilaize the system, including axes, communication, etc.
============================================================================
*/
void MainInit()
{
	//
	// Here will come initialization code for all system, axes, communication etc.
	//
//	struct sigaction stSigAction;
//	//
//	// Init the sigAction structure.
//	memset(&stSigAction, 0, sizeof(stSigAction));
//	stSigAction.sa_handler = &TerminateApplication;
//	// Initialized case of CTRL+C.
//	sigaction(SIGINT, &stSigAction, NULL);
	//
	// InitializeCommunication to the GMAS:
	gConnHndl = cConn.ConnectIPCEx(0x7fffffff,(MMC_MB_CLBK)CallbackFunc) ;
	//
	// 	Enable throw feature.
	CMMCPPGlobal::Instance()->SetThrowFlag(true,false);
	//
	// Register Run Time Error Callback function
	CMMCPPGlobal::Instance()->RegisterRTE(OnRunTimeError);
	//
	// Register the callback function for Emergency:
	cConn.RegisterEventCallback(MMCPP_EMCY,(void*)Emergency_Received) ;
	//
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
	//
	// TODO: Update number of necessary axes:
	//
	cRTaxis[0].InitAxisData("a01",gConnHndl);
	cRTaxis[1].InitAxisData("a02",gConnHndl);
	//cRTaxis[2].InitAxisData("a03",gConnHndl);
	//
	// Set default motion parameters. TODO: Update for all axes.
	cRTaxis[0].SetDefaultParams(stSingleDefault) ;
	cRTaxis[1].SetDefaultParams(stSingleDefault) ;
	//cRTaxis[2].SetDefaultParams(stSingleDefault) ;


	//
	//
	return;
}
/*
============================================================================
 Function:				MainClose()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Close all that needs to be closed before the application progra, is
 terminated.
============================================================================
*/
void MainClose()
{
//
//	Here will come code for all closing processes
//
	MMC_CloseConnection(gConnHndl) ;
	return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Function name	:	void callback function																		//
//	Created			:	Version 1.00																				//
//	Updated			:	3/12/2010																					//
//	Modifications	:	N/A																							//
//	Purpose			:	interupt function 																			//
//																													//
//	Input			:	N/A																							//
//	Output			:	N/A																							//
//	Return Value	:	int																							//
//	Modifications:	:	N/A																							//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CallbackFunc(unsigned char* recvBuffer, short recvBufferSize,void* lpsock)
{
	// Which function ID was received ...
	switch(recvBuffer[1])
	{
	case EMCY_EVT:
		//
		// Please note - The emergency event was registered.
		// printf("Emergency Event received\r\n") ;
		break ;
	case MOTIONENDED_EVT:
		printf("Motion Ended Event received\r\n") ;
		break ;
	case HBEAT_EVT:
		printf("H Beat Fail Event received\r\n") ;
		break ;
	case PDORCV_EVT:
		printf("PDO Received Event received - Updating Inputs\r\n") ;
		break ;
	case DRVERROR_EVT:
		printf("Drive Error Received Event received\r\n") ;
		break ;
	case HOME_ENDED_EVT:
		printf("Home Ended Event received\r\n") ;
		break ;
	case SYSTEMERROR_EVT:
		printf("System Error Event received\r\n") ;
		break ;
	/* This is commented as a specific event was written for this function. Once it occurs
	 * the ModbusWrite_Received will be called
		case MODBUS_WRITE_EVT:
		// TODO Update additional data to be read such as function parameters.
		// TODO Remove return 0 if you want to handle as part of callback.
		return 0;
		printf("Modbus Write Event received - Updating Outputs\r\n") ;

		break ;
	*/
	}
	//
	return 1 ;
}

///////////////////////////////////////////////////////////////////////
//	Function name	:	int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus)
//	Created			:	Version 1.00
//	Updated			:	20/05/2010
//	Modifications	:	N/A
//	Purpose			:	Callback function in case a Run Time Error was received.
//	Input			:	const char - *msg, unsigned int - uiConnHndl, unsigned short - usAxisRef, short - sErrorID, unsigned short - usStatus.
//	Output			:	N/A
//	Return Value	:	void
//
//	Modifications:	:	N/A
//////////////////////////////////////////////////////////////////////
int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus)
{
	MMC_CloseConnection(uiConnHndl);
	printf("MMCPPExitClbk: Run time Error in function %s, axis ref=%d, err=%d, status=%d, bye\n", msg, usAxisRef, sErrorID, usStatus);
	exit(0);
}

///////////////////////////////////////////////////////////////////////
//	Function name	:	void terminate_application(int iSigNum)
//	Created			:	Version 1.00
//	Updated			:	20/05/2010
//	Modifications	:	N/A
//	Purpose			:	Called in case application is terminated, stop modbus, engines, and power off engines.
//	Input			:	int iSigNum - Signal Num.
//	Output			:	N/A
//	Return Value	:	void
//
//	Modifications:	:	N/A
//////////////////////////////////////////////////////////////////////
void TerminateApplication(int iSigNum)
{
	//
	printf("In Terminate Application ...\n");
	giTerminate = 1 ;
	sigignore(SIGALRM);
	//
	switch(iSigNum)
	{
		// Handle ctrl+c.
		case SIGINT:
			// TODO Close what needs to be closed before program termination.
			exit(0);
			break;
		default:
			break;
	}
	return ;
}

///////////////////////////////////////////////////////////////////////
//	Function name	:	void Emergency_Received(unsigned short usAxisRef, short sEmcyCode)
//	Created			:	Version 1.00
//	Updated			:	20/05/2010
//	Modifications	:	N/A
//	Purpose			:	Callback function in case an Emergency event was received.
//	Input			:	unsigned short - usAxisRef, short - sEmcyCode.
//	Output			:	N/A
//	Return Value	:	void
//
//	Modifications:	:	N/A
//////////////////////////////////////////////////////////////////////
void Emergency_Received(unsigned short usAxisRef, short sEmcyCode)
{
	printf("Emergency Message Received on Axis %d. Code: %x\n",usAxisRef,sEmcyCode) ;
}

int SILCallBackFun(void)
{
	Timer time();
//	if (b) {
//		cRTaxis[1].EthercatWritePIVar(3, 0);
//		b = false;
//	} else {
//		cRTaxis[1].EthercatWritePIVar(3, 65536);
//		b = true;
//	}

//	Timer timer;


/*
 * Analog inputs commands for velocity loop
 */
//---------------------------------------------
//	short aiValue = 0;
//	cRTaxis[0].EthercatReadPIVar(6,0,aiValue);
//
//
////	cout << aiValue << endl;
//	if ((aiValue >=-1000) && (aiValue <=1000))
//		aiValue = 0;
//
////	cout << aiValue << endl;
//	double Kp = 50000.0f;
//	double dOutput1 = Kp * aiValue / 1000.0f;
//
//
//
//	cRTaxis[0].SetUser60FF(dOutput1);
//	cRTaxis[1].SetUser60FF(dOutput1);
//	cRTaxis[2].SetUser60FF(dOutput1);
//----------------------------------------------

/*
 * Simple P controller for Position loop
 */
//-----------------------------------------------
	Kp = (double)cRTaxis[0].GetUF(1);
//	Ki = (double)cRTaxis[0].GetUF(2);
	target_pos = (double)cRTaxis[0].GetUF(3);
	pos_err = target_pos - cRTaxis[0].GetActualPosition();


	cout << "Kp=" << Kp << endl;
	cout << "Ki=" << Ki << endl;
	cout << "Target Postion=" << target_pos << endl;
	cout << "Pos Err=" << pos_err << endl;

	output = Kp * pos_err;

	if (abs((int)output) > velocity_limit)
		output = output > 0 ? velocity_limit : -velocity_limit;

	cout << "Velocity Target =" << output << endl;

	cRTaxis[0].SetUser60FF(output);
//-------------------------------------------------
//	usleep(100000);

	return 0;
}
