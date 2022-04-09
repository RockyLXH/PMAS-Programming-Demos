/*
============================================================================
 Name : 	CPP_GroupBeginDemo.cpp
 Author :	Benjamin Spitzer
 Version :	1.00
 Description : The following example supports the following functionalities:

	 - Modbus callback registration.
	 - Emergency callback registration.
	 - PDO3 and SYNC initializations.
	 - Modbus reading and updates of axis status and positions.
 	 - Point to Point
 	 - Homing
 	 - Stop All axes
 	 - Move Pulses

 The program works with MAX_AXES in DS402 Profile Position and DS402 Homing motion modes. These are configured automatically by
 the called functions.
 For the above functions, the following modbus 'codes' are to be sent to address 40001:

 	 - Point to Point 	- 1. Performs a PP motion, back and fourth, and sets power off to all MAX_AXES.
 	 - Homing			- 2. Performs a Homing on index, and sets power off to all MAX_AXES.
 	 - Stop All axes	- 3. Stops current motion and sets all motors off to all MAX_AXES.
 	 - Move Pulses 		- 4. Performs synchronized motions for MAX_AXES, NUM_MOTIONS times. And then changes direction NUM_MOTIONS times.
 	  	  	  	  	  	  	  This works infinately until stopped (by calling Stop All axes).

- PDO3 is configured to update, per SYNC, all positions and velocities of axes.
- A sync is set in the system to SYNC_MULTIPLIER cycle time.

	The following information is updated to the Modbus at address MODBUS_UPDATE_START_INDEX:
		- All positions of axes, depending on MAX_AXES.

============================================================================
*/
#include "mmc_definitions.h"
#include "mmcpplib.h"
#include "CPP_GroupBeginDemoCAN.h"	// Application header file.
#include <iostream>
#include <sys/time.h>			// For time structure
#include <signal.h>				// For Timer mechanism
/*
============================================================================
 Function:				main()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 The main function of this  project.
============================================================================
*/

int main()
{
	//
	//	Initialize system, axes and all needed initializations
	//
	MainInit();
	//
	//	Execute the state machine to handle the system sequences and control
	//
	MachineSequences();
	//
	//	Close what needs to be closed before program termination
	//
	MainClose();
	//
	return 1;		// Terminate the application program back to the Operating System
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
//	Here will come initialization code for all system, axes, communication etc.
//
	giCount1=0;
	giCount2=0;
// 	InitializeCommunication to the GMAS:
//
	char 	sAxisName[20] ;
	char 	sErr[50];
	long 	lStatus ;
	//
	// Perform IPC connection and enable ALL
	gConnHndl = cConn.ConnectIPCEx(0x7fffffff,(MMC_MB_CLBK)CallbackFunc) ;
	//
	// Start the Modbus Server on device 1.
	cHost.MbusStartServer(gConnHndl,1) ;
	//
	// Register Run Time Error Callback function
	CMMCPPGlobal::Instance()->RegisterRTE(OnRunTimeError);
	//
	// Register the callback function for Modbus and Emergency:
	cConn.RegisterEventCallback(MMCPP_MODBUS_WRITE,(void*)ModbusWrite_Received) ;
	cConn.RegisterEventCallback(MMCPP_EMCY,(void*)Emergency_Received) ;

	//initialize the stop deceleration
	gfSetStopDeceleration01	= 60.0*ENCODE_RESOLUTION_01/360.0;
	gfSetStopDeceleration02 = 60.0*ENCODE_RESOLUTION_02/360.0;

	// Set default motion parameters of azimuth axis
	stSingleDefault01.fEndVelocity		= 0 ;
	stSingleDefault01.dbDistance 		= 0 ;
	stSingleDefault01.dbPosition 		= 0 ;
	stSingleDefault01.fVelocity 		= 0*ENCODE_RESOLUTION_01/360.0 ;	//0 deg/s
	stSingleDefault01.fAcceleration 	= 30*ENCODE_RESOLUTION_01/360.0 ;	//30 deg/s2
	stSingleDefault01.fDeceleration 	= 30*ENCODE_RESOLUTION_01/360.0 ;	//30 deg/s2
	stSingleDefault01.fJerk 			= 20000 ;
	stSingleDefault01.eDirection 		= MC_POSITIVE_DIRECTION ;
	stSingleDefault01.eBufferMode 		= MC_BUFFERED_MODE ;
	stSingleDefault01.ucExecute 		= 1 ;

	// Set default motion parameters of elevation axis
	stSingleDefault02.fEndVelocity		= 0 ;
	stSingleDefault02.dbDistance 		= 0 ;
	stSingleDefault02.dbPosition 		= 0 ;
	stSingleDefault02.fVelocity 		= 0*ENCODE_RESOLUTION_02/360.0 ;	//0 deg/s
	stSingleDefault02.fAcceleration 	= 30*ENCODE_RESOLUTION_02/360.0 ;	//30 deg/s2
	stSingleDefault02.fDeceleration 	= 30*ENCODE_RESOLUTION_02/360.0 ;	// 30 deg/s2
	stSingleDefault02.fJerk 			= 20000 ;
	stSingleDefault02.eDirection 		= MC_POSITIVE_DIRECTION ;
	stSingleDefault02.eBufferMode 		= MC_BUFFERED_MODE ;
	stSingleDefault02.ucExecute 		= 1 ;

	//
	// Initialize azimuth axis name and default parameter.
	sprintf(sAxisName, "b01");
	cAxis01.InitAxisData(sAxisName,gConnHndl) ;
	cAxis01.SetDefaultParams(stSingleDefault01) ;
	lStatus = cAxis01.ReadStatus() ;
	if(lStatus & NC_AXIS_ERROR_STOP_MASK)
	{
		cAxis01.Reset() ;
	}

	sleep(1) ;			// Wait a bit and recheck if in Error state:

	lStatus = cAxis01.ReadStatus() ;
	if(lStatus & NC_AXIS_ERROR_STOP_MASK)
	{
		sprintf( sErr,"axes a01 is in error. Exiting.\n");
		cout <<sErr;
		exit(EXIT_FAILURE) ;
	}

	// Initialize elevation axis name and default parameter.
//	sprintf(sAxisName, "a02");
//	cAxis02.InitAxisData(sAxisName,gConnHndl) ;
//	cAxis02.SetDefaultParams(stSingleDefault02) ;
//	lStatus = cAxis02.ReadStatus() ;
//	if(lStatus & NC_AXIS_ERROR_STOP_MASK)
//	{
//		cAxis02.Reset() ;
//
//	}
//
//	sleep(1) ;			// Wait a bit and recheck if in Error state:
//
//	lStatus = cAxis02.ReadStatus() ;
//	if(lStatus & NC_AXIS_ERROR_STOP_MASK)
//	{
//		sprintf( sErr,"axes a02 is in error. Exiting.\n");
//		cout <<sErr;
//		exit(EXIT_FAILURE) ;
//	}
		// Config PDO Example for PDO3 - Event Group1
		// cAxes[i].ConfigPDO(PDO_NUM_3,PDO_PARAM_REG,NC_COMM_EVENT_GROUP1,1,1,1,1,1) ;

		//		PDO_NUMBER_ENUM ePDONum,
		//		PDO_PARAM_TYPE_ENUM eParamType,
		//		unsigned int uiPDOCommParamEvent- object 2f20 sub index 3. If on event, then states the mask to emit PDO
		//		unsigned short usEventTimer -
		//		unsigned char ucEventGroup - Event group type.
		//		unsigned char ucPDOCommParam,
		//		unsigned char ucSubIndex,
		//		unsigned char ucPDOType)
		// Config UI1 and UI2 to be sent OnChange from Drive to Host after emit is sent.
		cAxis01.ConfigPDO(PDO_NUM_3,PDO_PARAM_USE,
				0x0,	// Bit 5. User program 揺mit�command emits PDO.
				2 ,		// Timer
				NC_COMM_EVENT_GROUP12,
				1,	// ucPDOCommParam
				1,		// Sub Index
				1) ;	// PDO Type. 0 RPDO. 1 TPDO


		cAxis01.ConfigPDO(PDO_NUM_4,PDO_PARAM_USE,
						0x0,	// PDO Config
						3 ,		// Timer
						NC_COMM_EVENT_GROUP13, // UIs
						1,	// ucPDOCommParam
						3,		// Sub Insdex
						1) ;	// PDO Type. 0 RPDO. 1 TPDO


//		cAxis01.ConfigPDO(PDO_NUM_3,PDO_PARAM_REG,
//				0x20,	// Bit 5. User program 揺mit�command emits PDO.
//				0 ,		// Timer
//				NC_COMM_EVENT_GROUP1,
//				PDO_COM_PARAM_SYNC,	// ucPDOCommParam
//				1,		// Sub Index
//				1) ;	// PDO Type. 0 RPDO. 1 TPDO
//		//cAxes[i].ConfigPDOEventMode(MC_PDO_EVENT_CYCLIC_NOTIF,PDO_NUM_3);;
//
//		cAxis01.ConfigPDO(PDO_NUM_3,PDO_PARAM_USE,
//						0x0,	// PDO Config
//						0 ,		// Timer
//						NC_COMM_RXEVENT_GROUP1, // UIs
//						255,	// ucPDOCommParam
//						3,		// Sub Index
//						0) ;	// PDO Type: 0-RPDO. 1-TPDOlllla;
//
//
//		cAxis02.ConfigPDO(PDO_NUM_3,PDO_PARAM_REG,
//					0x20,	// Bit 5. User program 揺mit�command emits PDO.
//					0 ,		// Timer
//					NC_COMM_EVENT_GROUP1,
//					PDO_COM_PARAM_SYNC,	// ucPDOCommParam
//					1,		// Sub Index
//					1) ;	// PDO Type. 0 RPDO. 1 TPDO
//			//cAxes[i].ConfigPDOEventMode(MC_PDO_EVENT_CYCLIC_NOTIF,PDO_NUM_3);;
//
//		cAxis02.ConfigPDO(PDO_NUM_3,PDO_PARAM_USE,
//						0x0,	// PDO Config
//						0 ,		// Timer
//						NC_COMM_RXEVENT_GROUP1, // UIs
//						255,	// ucPDOCommParam
//						3,		// Sub Index
//						0) ;	// PDO Type. 0 RPDO. 1 TPDOlllla;';


	//
	// Initialize SYNC's in the system:
	//
	CMMCPPGlobal::Instance()->SetSyncTime(gConnHndl, SYNC_MULTIPLIER) ;
	//
	// Option if DS401 in system, Init the IO. Reset if in Error.
	// ds401.InitAxisData("i01",gConnHndl);
	//	lStatus = ds401.ReadStatus() ;
	//	if(lStatus & NC_AXIS_ERROR_STOP_MASK)
	//	{
	//		ds401.Reset() ;
	//		sleep(1) ;
	//		lStatus   = ds401.ReadStatus() ;
	//		if(lStatus & NC_AXIS_ERROR_STOP_MASK)
	//		{
	//			cout << "Axis a1 in Error Stop. Aborting." ;
	//			exit(0) ;
	//		}
	//	}
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
	cHost.MbusStopServer() ;
	MMC_CloseConnection(gConnHndl) ;
	return;
}
/*
============================================================================
 Function:				MachineSequences()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Starts the Main Timer function that will execute the states machines
 to control the system. Also performs a slow background loop for
 less time-critical background processes and monitoring of requests
 to terminate the application.
============================================================================
*/
void MachineSequences()
{
//
//	Init all variables of the states machines
//
	MachineSequencesInit();
//
//	Enable MachineSequencesTimer() every TIMER_CYCLE ms
//
//	EnableMachineSequencesTimer(TIMER_CYCLE);
//
//	Background loop. Handles termination request and other less time-critical background proceses
//
	while (!giTerminate)
	{
		giCount1++;
		//
		// Comment this line if need of timer or synchronised timer:
		MachineSequencesTimer();
//
//		Execute background process if required
//
		BackgroundProcesses();
//
//		Sleep for ~SLEEP_TIME micro-seconds to reduce CPU load
//
		usleep(SLEEP_TIME);
	}
//
//	Termination requested. Close what needs to be cloased at the states machines
//
	MachineSequencesClose();

	return;		// Back to the main() for program termination
}
/*
============================================================================
 Function:				MachineSequencesInit()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Initilaize the states machines variables
============================================================================
*/
void MachineSequencesInit()
{
//
//	Initializing all variables for the states machines
//
	giTerminate 	= FALSE;

	giState01 		= eIDLE;
	giPrevState01 	= eIDLE;
	giSubState01 	= eIDLE;

	giState02 		= eIDLE;
	giPrevState02	= eIDLE;
	giSubState02 	= eIDLE;
	//
	giReentrance = FALSE;

	return;
}
/*
============================================================================
 Function:				MachineSequencesClose()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Close all that needs to be closed at the states machines before the
 application program is terminated.
============================================================================
*/
void MachineSequencesClose()
{
//
//	Here will come code for all closing processes
//
	return;
}
/*
============================================================================
 Function:				BackgroundProcesses()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Performs background processes that are not managed by the States Machines.
 This can be saving logs to the FLASH, performing background calculations etc.
 Generally speaking, this function, although colored in red in the Close all that needs to be closed at the states machines before the
 application program is terminated.
============================================================================
*/
void BackgroundProcesses()
{
//
//	Here will come code for all closing processes
//
	return;
}
/*
============================================================================
 Function:				EnableMachineSequencesTimer()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Enables the main machine sequences timer function, to be executed each
 TIMER_CYCLE ms.
============================================================================
*/
void EnableMachineSequencesTimer(int TimerCycle)
{
	struct itimerval timer;
	struct sigaction stSigAction;

	// Whenever a signal is caught, call TerminateApplication function
	stSigAction.sa_handler = TerminateApplication;

	sigaction(SIGINT, &stSigAction, NULL);
	sigaction(SIGTERM, &stSigAction, NULL);
	sigaction(SIGABRT, &stSigAction, NULL);
	sigaction(SIGQUIT, &stSigAction, NULL);
//
//	Enable the main machine sequences timer function
//
	timer.it_interval.tv_sec 	= 0;
	timer.it_interval.tv_usec 	= TimerCycle * 1000;// From ms to micro seconds
	timer.it_value.tv_sec 		= 0;
	timer.it_value.tv_usec 		= TimerCycle * 1000;// From ms to micro seconds

	// setitimer(ITIMER_REAL, &timer, NULL);

	// signal(SIGALRM, MachineSequencesTimer); 		// every TIMER_CYCLE ms SIGALRM is received which calls MachineSequencesTimer()
	//
	// Start syncronized timer in system - only with firmware supporting this.  Currently only supported in CAN
	//	MMC_DestroySYNCTimer(gConnHndl) ;
	//	MMC_CreateSYNCTimer(gConnHndl,MachineSequencesTimer,1) ;
	return;
}
/*
============================================================================
 Function:				MachineSequencesTimer()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 A timer function that is called by the OS every TIMER_CYCLE ms.
 It executes the machine sequences states machines and actully controls
 the sequences and behavior of the machine.
============================================================================
*/
int MachineSequencesTimer(void)
{
//
//	In case the application is waiting for termination, do nothing.
//	This can happen if giTerminate has been set, but the background loop
//	didn't handle it yet (it has a long sleep every loop)
//
	if (giTerminate == TRUE) return 0;
//
//	Avoid reentrance of this time function
//
//	Reentrance can theoretically happen if the execution of this timer function
//	is wrongly longer than TIMER_CYCLE. In such case, reentrance should be avoided
//	to prevent unexpected behavior (the code is not designed for reentrance).
//
//	In addition, some error handling should be taken by the user.
//
	if (giReentrance)
	{
//
//		Print an error message and return. Actual code should take application related error handling
//
		printf("Reentrancy!\n");

		return 0;
	}

	giReentrance = TRUE;		// to enable detection of reentrancy. The flag is cleared at teh end of this function
	//
//
//	Read all input data.
//
//	Here, every TIMER_CYCLE ms, the user should read all input data that may be
//	required for the states machine code and copy them into "mirror" variables.
//
//	The states machines code, below, should use only the mirror variables, to ensure
//	that all input data is synchronized to the timer event.
//
//	Input data can be from the Host (MODBUS) or from the drives or I/Os units
//	(readingfrom the GMAS core firmware using one of the Function Blocks library
//	functions) or from any other source.
//
	ReadAllInputData();
/*
============================================================================

	States Machines code starts here!

============================================================================
*/

	if(giStatus01 & NC_AXIS_ERROR_STOP_MASK)
	{
		printf("Axis01 Error Stop!\r\n giState01 = %d, giSubState01 = %d, giStatus01 = %x", giState01, giSubState01,giStatus01);
		return 0;
	}
	if(giStatus02 & NC_AXIS_ERROR_STOP_MASK)
	{
		printf("Axis02 Error Stop!\r\n giState02 = %d, giSubState02 = %d, giStatus02 =%x", giState02, giSubState02,giStatus02);
		return 0;
	}

// ****************axis 01***************
//	In case it is a new state value, clear also the value of the sub-state
//	to ensure it will start from its beginning (from the first ssub-state)
//	we will not recept the new state and do nothing when statemachine is working on one running state ,
//	and during this time the varable "giState" is not "eIDEL";

	if(giTempState01)
		printf("01: giTempState01 = %d \r\n", giTempState01);

	switch(giTempState01)
	{
	case eIDLE:
		break;
	case eHome:
		if(giStatus01 & NC_AXIS_STAND_STILL_MASK)
		{
			giState01 		= giTempState01;
			giSubState01	= FIRST_SUB_STATE;
			printf("01: eHome assigment success, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01,giStatus01);
		}
		else
		{
			printf("01: eHome assigment error, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
		}
		break;
	case ePTP:
		if(giStatus01 & NC_AXIS_STAND_STILL_MASK)
		{
			giState01 		= giTempState01;
			giSubState01	= FIRST_SUB_STATE;
			printf("01: ePTP assigment success, giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01,giStatus01);
		}
		else
		{
			printf("01: ePTP assigment error, giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
		}
		break;
	case eVelocity:
		if(giStatus01 & NC_AXIS_STAND_STILL_MASK)
		{
			giState01 		= giTempState01;
			giSubState01	= FIRST_SUB_STATE;
			printf("01: eVelocity assigment success, giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
		}
		else
		{
			printf("01: eVelocity assigment error, giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
		}
		break;
	case eStop:
		if((~giStatus01 & NC_AXIS_DISABLED_MASK )&&(~giStatus01 & NC_AXIS_STOPPING_MASK))
		{
			giState01 		= giTempState01;
			giSubState01	= FIRST_SUB_STATE;
			printf("01: eStop assigment success, giState01 = %d, giSubState01 = %d, giSatus01 = %x \r\n", giState01, giSubState01,giStatus01);
		}
		else
		{
			printf("01: eStop assigment error,  giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01,giStatus01);
		}
		break;
	case ePowerOn:
		giState01 		= giTempState01;
		giSubState01	= FIRST_SUB_STATE;
		printf("01: ePowerOn assigment , giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
		break;
	case ePowerOff:
		giState01		= giTempState01;
		giSubState01	= FIRST_SUB_STATE;
		printf("01: ePowerOff assigment , giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
		break;
	default:
		printf("01: Error control velue, giTempState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giTempState01, giSubState01,giStatus01);
		break;
	}

	//********* axis 02*******
	//	In case it is a new state value, clear also the value of the sub-state
	//	to ensure it will start from its beginning (from the first ssub-state)
	//	we will not recept the new state and do nothing when statemachine is working on one running state ,
	//	and during this time the varable "giState" is not "eIDEL";


	if(giTempState02)
		printf("02: giTempState02 = %d \r\n", giTempState02);

	switch(giTempState02)
	{
	case eIDLE:
		break;
	case eHome:
		if(giStatus02 & NC_AXIS_STAND_STILL_MASK)
		{
			giState02 		= giTempState02;
			giSubState02	= FIRST_SUB_STATE;
			printf("02: eHome assigment success, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
		}
		else
		{
			printf("02: eHome assigment error, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
		}
		break;
	case ePTP:
		if(giStatus02 & NC_AXIS_STAND_STILL_MASK)
		{
			giState02 		= giTempState02;
			giSubState02	= FIRST_SUB_STATE;
			printf("02: ePTP assigment success, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
		}
		else
		{
			printf("02: ePTP assigment error, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
		}
		break;
	case eVelocity:
		if(giStatus02 & NC_AXIS_STAND_STILL_MASK)
		{
			giState02 		= giTempState02;
			giSubState02	= FIRST_SUB_STATE;
			printf("02: eVelocity assigment success, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02,giStatus02);
		}
		else
		{
			printf("02: eVelocity assigment error, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
		}
		break;
	case eStop:
		if((~giStatus02 & NC_AXIS_DISABLED_MASK )&&(~giStatus02 & NC_AXIS_STOPPING_MASK))
		{
			giState02 		= giTempState02;
			giSubState02	= FIRST_SUB_STATE;
			printf("02: eStop assigment success, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02,giStatus02);
		}
		else
		{
			printf("02: eStop assigment error, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
		}
		break;
	case ePowerOn:
		giState02 		= giTempState02;
		giSubState02	= FIRST_SUB_STATE;
		printf("02: ePowerOn assigment success, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02,giStatus02);
		break;
	case ePowerOff:
		giState02		= giTempState02;
		giSubState02	= FIRST_SUB_STATE;
		printf("02: ePowerOff assigment success, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02,giStatus02);
		break;
	default:
		printf("02: Error control velue, giTempState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giTempState02, giSubState02,giStatus02);
		break;
	}

	mbus_write_in.startRef 		= 0;       // Reset Current state we are running on in Modbus so we do not return to it.
	mbus_write_in.refCnt 		= 2;
	mbus_write_in.regArr[0] 	= 0;
	mbus_write_in.regArr[1] 	= 0;
	cHost.MbusWriteHoldingRegisterTable(mbus_write_in) ;
	//printf("clear control words\n");


//	Handle the main state machine of Azimuth
//
//	The value of the State variable is used to make decisions of the main states machine and to call,
//	as necessary, the relevant function that handles to process itslef in a sub-state machine.
//
	switch (giState01)
	{
		case eIDLE:						//Do nothing, waiting for commands
		{
			break;
		}
		case eHome:
		{
			StateFunction_Home_01();
			break;
		}
		case ePTP:
		{
			StateFunction_PTP_01();
			break;
		}
		case eVelocity:
		{
			StateFunction_MoveVelocity_01();
			break;
		}
		case eStop:
		{
			StateFunction_Stop_01();
			break;
		}
		case ePowerOn:
		{
			StateFunction_PowerOn_01();
			break;
		}
		case ePowerOff:
		{
			StateFunction_PowerOff_01();
			break;
		}
		default:					//		The default case. Should not happen, the user can implement error handling.
		{
			printf("01:Eorror giState has been assignment! giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			break;
		}
	}

// handle the state machine of Elevation
	switch (giState02)
	{
		case eIDLE:
		{
			break;
		}
		case eHome:
		{
			StateFunction_Home_02();
			break;
		}
		case ePTP:
		{
			StateFunction_PTP_02();
			break;
		}
		case eVelocity:
		{
			StateFunction_MoveVelocity_02();
			break;
		}
		case eStop:
		{
			StateFunction_Stop_02();
			break;
		}
		case ePowerOn:
		{
			StateFunction_PowerOn_02();
			break;
		}
		case ePowerOff:
		{
			StateFunction_PowerOff_02();
			break;
		}
		default:
		{
			printf("02:Eorror giState has been assignment! giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			break;
		}
	}
//
//	Write all output data
//
//	Here, every TIMER_CYCLE ms, after the execution of all states machines
//	(based on all the Input Data as read from all sources at teh top of this function)
//	the user should write all output data (written into mirror variables within the
//	states machines code) to the "external world" (MODBUS, GMAS FW core, ...).
//
//	After all states machines were executed and generated their outputs, the outputs
//	are writen to the "external world" to actually execute the states machines "decisions"
//
	giCount2++;
	WriteAllOutputData();
//
//	Clear the reentrancy flag. Now next execution of this function is allowed
//
	giReentrance = FALSE;
//
	return 1;		// End of the sequences timer function. The function will be triggered again upon the next timer event.
}
/*
============================================================================
 Function:				ReadAllInputData()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 The data used during the processing of the states machines must be synchronized with the
 timer event and must be "frozen" during the processing of all states machines code, so that
 all decisions and calculations will use the same data.

 This function is called at the beginning of the timer function, each TIMER_CYCLE ms,
 to collect all required input data (as relevant to each application) and to copy
 it into "mirror" variables, that will be used by the state machines code.
 ============================================================================
*/
void ReadAllInputData()
{
	MMC_MODBUSREADHOLDINGREGISTERSTABLE_OUT 	mbus_read_out;
	MMC_MODBUSREADHOLDINGREGISTERSTABLE_IN 		mbus_read_in;
	int iTemp;
	float *pfTemp;

//
//	Here should come the code to read all required input data, for instance:
//
// The data read here may arrive from different sources:
// 	- Host Communication (Modbus, Ethernet-IP. This can be read on a cyclic basis, or from a callback.
//	- GMAS Firmware. Such as actual positions, torque, velocities.

	mbus_read_in.startRef 	= MODBUS_READ_OUTPUTS_INDEX;
	mbus_read_in.refCnt 	= MODBUS_READ_CNT ;	// TODO: Change the number of registers to read.

	cHost.MbusReadHoldingRegisterTable(MODBUS_READ_OUTPUTS_INDEX,MODBUS_READ_CNT,mbus_read_out) ;

	giTempState01= mbus_read_out.regArr[0] & 0xFFFF;
	giTempState02= mbus_read_out.regArr[1] & 0xFFFF;

	//get the parameter that translate from modus,it is a float data with format fo 'CDAB'
	//get the azimuth position,velocity,acceleration,deceleration from the modbus
	iTemp=(mbus_read_out.regArr[3] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[2] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gdSetPosition01=(double)((*pfTemp)*ENCODE_RESOLUTION_01/360.0);

	iTemp=(mbus_read_out.regArr[5] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[4] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gfSetVelocity01=(*pfTemp)*ENCODE_RESOLUTION_01/360.0;

	iTemp=(mbus_read_out.regArr[7] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[6] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gfSetAcceleration01=(*pfTemp)*ENCODE_RESOLUTION_01/360.0;

	iTemp=(mbus_read_out.regArr[9] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[8] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gfSetDeceleration01=(*pfTemp)*ENCODE_RESOLUTION_01/360.0;

	//get the elevation data: position,velocity,acceleration,deceleration from the modbus

	iTemp=(mbus_read_out.regArr[11] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[10] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gdSetPosition02=(double)((*pfTemp)*ENCODE_RESOLUTION_02/360.0);

	iTemp=(mbus_read_out.regArr[13] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[12] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gfSetVelocity02=(*pfTemp)*ENCODE_RESOLUTION_02/360.0;

	iTemp=(mbus_read_out.regArr[15] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[14] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gfSetAcceleration02=(*pfTemp)*ENCODE_RESOLUTION_02/360.0;

	iTemp=(mbus_read_out.regArr[17] << 16 & 0xFFFF0000) | (mbus_read_out.regArr[16] & 0xFFFF);
	pfTemp=(float*)(&iTemp);
	gfSetDeceleration02=(*pfTemp)*ENCODE_RESOLUTION_02/360.0;

	giStatus01	=	cAxis01.ReadStatus();
	gfPos01		=	(float)cAxis01.GetActualPosition();
	gfVel01		=	(float)cAxis01.GetActualVelocity();

	giStatus02	=	cAxis02.ReadStatus();
	gfPos02		=	cAxis02.GetActualPosition();
	gfVel02		=	cAxis02.GetActualVelocity();
	//
	// In case a DS401 is defined in the system. Read Inputs.
	//	llIO_Val = ds401.ReadDS401DInput();
	//
	// Read UI From Drive:
	//UI1 = cAxes[0].GetBoolParameter(MMC_I_COMM_EV_USR_1_PARAM,0) ;
	//UI2 = cAxes[0].GetBoolParameter(MMC_I_COMM_EV_USR_1_AUX_PARAM,0) ;
	UI1 = cAxis01.GetBoolParameter(MMC_I_COMM_EV_USR_1_PARAM,0) ;
	UI2 = cAxis01.GetBoolParameter(MMC_I_COMM_EV_USR_1_AUX_PARAM,0) ;
	return;
}
/*
============================================================================
 Function:				WriteAllOutputData()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 Write all the data, generated by the states machines code, to the "external
 world".

 This is done here (and not inside the states machines code) to ensure that
 all data is updated simultaneously and in a synchronous way.

 The states machines code write the data into "mirror" variables, that are here
 copied or sent to the "external world" (Host via MODBUS, GMAS core firmware, etc.)
  ============================================================================
*/
void WriteAllOutputData()
{
	int *piTemp,iTemp ;
	float fTemp;
	static int j = 1 ;
	j++ ;

	//	Here should come the code to write/send all ouput data
	mbus_write_in.startRef 		= MODBUS_UPDATE_START_INDEX	;       // index of start write modbus register.
	mbus_write_in.refCnt 		= MODBUS_UPDATE_CNT			;			// number of indexes to write

	// insert the azimuth status data
	InsertLongVarToModbusShortArr(mbus_write_in.regArr+0,(long)giStatus01);

	//insert the azimuth position data
	fTemp	=	gfPos01*360.0/ENCODE_RESOLUTION_01;
	piTemp	=	(int*)(&fTemp);
	iTemp	=	*piTemp;
	InsertLongVarToModbusShortArr(mbus_write_in.regArr+2,(long)iTemp);

	//insert the azimuth velocity data
	fTemp	=	gfVel01*360.0/ENCODE_RESOLUTION_01;
	piTemp	=	(int*)(&fTemp);
	iTemp	=	*piTemp;
	InsertLongVarToModbusShortArr(mbus_write_in.regArr+4,(long)iTemp);

	// insert the elevation status data
	InsertLongVarToModbusShortArr(mbus_write_in.regArr+6,(long)giStatus02);

	//insert the elevation position data
	fTemp	=	gfPos02*360.0/ENCODE_RESOLUTION_02;
	piTemp	=	(int*)(&fTemp);
	iTemp	=	*piTemp;
	InsertLongVarToModbusShortArr(mbus_write_in.regArr+8,(long)iTemp);

	//insert the elevatioin velocity data
	fTemp	=	gfVel02*360.0/ENCODE_RESOLUTION_02;
	piTemp	=	(int*)(&fTemp);
	iTemp	=	*piTemp;
	InsertLongVarToModbusShortArr(mbus_write_in.regArr+10,(long)iTemp);

	InsertLongVarToModbusShortArr(mbus_write_in.regArr+12,(long)giCount1);
	InsertLongVarToModbusShortArr(mbus_write_in.regArr+14,(long)giCount2);

	//write the data to modbus holding register
	cHost.MbusWriteHoldingRegisterTable(mbus_write_in) ;

	// Option to write DS401 Outputs
	// ds401.WriteDS401DOutput(llIO_Val);

	//cAxes[0].SetBoolParameter(j,MMC_I_COMM_EV_USR_3_PARAM,0);
	cAxis01.SetBoolParameter(j,MMC_I_COMM_EV_USR_3_PARAM,0);
	return;
}
/*============================================================
 Function:	StateFunction_PowerOn_01()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				06/08/2020
 Modifications:			N/A
 =============================================================
*/
void StateFunction_PowerOn_01()
{
	switch(giSubState01)
	{
		case eSubState_SM10_PowerOn:
		{
			printf("01: POWON PowerOn, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			if(giStatus01 & NC_AXIS_DISABLED_MASK)
				cAxis01.PowerOn();
			giSubState01 = eSubState_SM10_WPowerOn;
			break;
		}
		case eSubState_SM10_WPowerOn:
		{
			printf("01: POWON WPowerOn, giState01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			if(giStatus01 & NC_AXIS_STAND_STILL_MASK)
				giState01 = eIDLE;
			break;
		}
		default:
			break;
	}
}

/*============================================================
 Function:	StateFunction_PowerOff_01()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				06/08/2020
 Modifications:			N/A
 =============================================================
*/
void StateFunction_PowerOff_01()
{
	switch(giSubState01)
	{
 	 	 case eSubState_SM11_PowerOff:
 	 	 {
 	 		printf("01: POWOFF PowerOff, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
 	 		if(~giStatus01 & NC_AXIS_DISABLED_MASK)
 	 		 	 			 cAxis01.PowerOff();
 	 		giSubState01 = eSubState_SM11_WPowerOff;
 	 		break;
 	 	 }
 	 	 case eSubState_SM11_WPowerOff:
 	 	 {
 	 		printf("01: POWOFF WPowerOff, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
 	 		if(giStatus01 & NC_AXIS_DISABLED_MASK)
 	 		 	 giState01 = eIDLE;
 	 		break;
 	 	 }
 	 	 default:
 	 		 break;
	}
}


/*============================================================
 Function:	StateFunction_PowerOn_02()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				06/08/2020
 Modifications:			N/A
 =============================================================
*/
void StateFunction_PowerOn_02()
{
	switch(giSubState02)
	{
		case eSubState_SM10_PowerOn:
		{
			printf("02: POWON PowerOn, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			if(giStatus02 & NC_AXIS_DISABLED_MASK)
				cAxis02.PowerOn();
			giSubState02 = eSubState_SM10_WPowerOn;
			break;
		}
		case eSubState_SM10_WPowerOn:
		{
			printf("02: POWON WPowerOn, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			if(giStatus02 & NC_AXIS_STAND_STILL_MASK)
				giState02 = eIDLE;
			break;
		}
		default:
			break;
	}
}

/*============================================================
 Function:	StateFunction_PowerOff_01()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				06/08/2020
 Modifications:			N/A
 =============================================================
*/
void StateFunction_PowerOff_02()
{
	switch(giSubState02)
	{
 	 	 case eSubState_SM11_PowerOff:
 	 	 {
 	 		printf("02: POWOFF PowerOff, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
 	 		if(~giStatus02 & NC_AXIS_DISABLED_MASK)
 	 		 	 cAxis02.PowerOff();
 	 		giSubState02 = eSubState_SM11_WPowerOff;
 	 		break;
 	 	 }
 	 	 case eSubState_SM11_WPowerOff:
 	 	 {
 	 		printf("02: POWOFF WPowerOff, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
 	 		if(giStatus02 & NC_AXIS_DISABLED_MASK)
 	 		 	 giState02 = eIDLE;
 	 		break;
 	 	 }
 	 	 default:
 	 		 break;
	}
}






/*
============================================================================
 Function:				StateFunction_PTP_01()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 A sub-states machine function. This function executes the sub-states machine
 of the PTP process.

	 Change Operation Mode.
	 Power Enable
	 Start Point To Point
	 Wait for end of Point to Point
	 Change Direction back
	 Wait for End of Point to Point
	 Power Off

 Each step is handled by a dedicated function. However, calling a function
 is not a must and the relevant code for each sub-state can be directly
 written within the switch-case structure.
============================================================================
*/


void StateFunction_PTP_01()
{
//
//	Handle the sub-state machine.
//
//	The value of the Sub-State variable is used to make decisions of the sub-states machine and to call,
//	as necessary, the relevant function that handles to process itself.
//
try {
	switch (giSubState01)
	{
		case eSubState_SM2_ChOPMode:

			// Change OP mode to Profile Position.
			printf("01: PTP ChOPMode, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			if(cAxis01.GetOpMode() != OPM402_PROFILE_POSITION_MODE)
			{
				cAxis01.SetOpMode(OPM402_PROFILE_POSITION_MODE) ;
			}
			giSubState01 	= eSubState_SM2_WChOPMode ;
			break ;

		case eSubState_SM2_WChOPMode:

			// Check we are in Profile Position in all axes:
			printf("01: PTP WChOPMode, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			if(cAxis01.GetOpMode() == OPM402_PROFILE_POSITION_MODE)
			{
				giSubState01 = eSubState_SM2_Move ;		// If we reached here, then all axes are in Profile Position.
			}
			break ;

		/*case eSubState_SM2_PowerOn:

			if (giStatus01 & NC_AXIS_DISABLED_MASK)
				cAxis01.PowerOn() ;
			giSubState01 = 	eSubState_SM2_WPowerOn;		//	Changing to the next sub-state and wait until power is on.
			break;

		case eSubState_SM2_WPowerOn:
		{
			// Check we are in standstill (power is on) for all axes:
			if (giStatus01 & NC_AXIS_STAND_STILL_MASK)
			{
				giSubState01 = eSubState_SM2_Move;		// If we reached here, then all axes are in standstill
			}
			break;
		}*/
		case eSubState_SM2_Move:
			printf("01: PTP Move, gistate01 = %d, giSubState01 = %d, giStatus01 = %08x \r\n", giState01, giSubState01, giStatus01);
			// Perform motion for all axes.
			//cAxis01.m_fAcceleration =gfSetAcceleration01;
			//cAxis01.m_fDeceleration=gfSetDeceleration01;
			//cAxis01.MoveAbsolute(gdSetPosition01,gfSetVelocity01,MC_ABORTING_MODE) ;
			cAxis01.MoveAbsolute(gdSetPosition01,gfSetVelocity01,gfSetAcceleration01,gfSetDeceleration01) ;
			//	Changing to the next sub-state
			giSubState01 = eSubState_SM2_WMove;
			break;
		case eSubState_SM2_WMove:
			printf("01: PTP WMove, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			//	Changing to the next sub-state only if axis changed to standstill.
			if ( (giStatus01 & NC_AXIS_STAND_STILL_MASK) || (giStatus01 & NC_AXIS_DONE_MASK) )
			{
				giState01 = eIDLE;
			}
			break;

//		The default case. Should not happen, the user can implement error handling.
		default:
		{
			break;
		}
	}
}
catch(CMMCException exp)
{
	cout << "exception error" << exp.error() << "\t" << exp.what() << endl;
}
	return;
}



void StateFunction_PTP_02()
{
//	Handle the sub-state machine.
//
//	The value of the Sub-State variable is used to make decisions of the sub-states machine and to call,
//	as necessary, the relevant function that handles to process itself.
//
try {
	switch (giSubState02)
	{
		case eSubState_SM2_ChOPMode:

			// Change OP mode to Profile Position.
			printf("02: PTP ChOPMode, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			if(cAxis02.GetOpMode() != OPM402_PROFILE_POSITION_MODE)
			{
				cAxis02.SetOpMode(OPM402_PROFILE_POSITION_MODE) ;
			}

			giSubState02 	= eSubState_SM2_WChOPMode ;
			break ;
		case eSubState_SM2_WChOPMode:

			// Check we are in Profile Position in all axes:
			printf("02: PTP WChOPMode, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			if(cAxis02.GetOpMode() == OPM402_PROFILE_POSITION_MODE)
			{
				// If we reached here, then all axes are in Profile Position.
				giSubState02 = eSubState_SM2_Move ;
			}
			break ;
		/*case eSubState_SM2_PowerOn:
			// Performs power on for all axes

			if (giStatus02 & NC_AXIS_DISABLED_MASK)
				cAxis02.PowerOn() ;

			//	Changing to the next sub-state and wait until power is on.
			giSubState02 = eSubState_SM2_WPowerOn;
			break;
		case eSubState_SM2_WPowerOn:
		{
			// Check we are in standstill (power is on) for all axes:
			if (giStatus02 & NC_AXIS_STAND_STILL_MASK)
			{
				// If we reached here, then all axes are in standstill
				giSubState02 = eSubState_SM2_Move;
			}
			break;
		}*/
		case eSubState_SM2_Move:
		{
			// Perform motion for all axes.
			printf("02: PTP Move, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			//cAxis02.m_fAcceleration =gfSetAcceleration02;
			//cAxis02.m_fDeceleration=gfSetDeceleration02;
			//cAxis02.MoveAbsolute(gdSetPosition02,gfSetVelocity02,MC_ABORTING_MODE);
			cAxis02.MoveAbsolute(gdSetPosition02,gfSetVelocity02,gfSetAcceleration02, gfSetDeceleration02);
			//	Changing to the next sub-state
			giSubState02 = eSubState_SM2_WMove;
			break;
		}
		case eSubState_SM2_WMove:
		{
			//	Changing to the next sub-state only if axis changed to standstill.
			printf("02: PTP WMove, giState02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			if ( (giStatus02 & NC_AXIS_STAND_STILL_MASK)|| (giStatus02 & NC_AXIS_DONE_MASK) )
			{
				giState02 = eIDLE;
			}
			break;
		}
//		The default case. Should not happen, the user can implement error handling.
		default:
		{
			break;
		}
	}
}
catch(CMMCException exp) {
	cout << "exception error" << exp.error() << "\t" << exp.what() << endl;
}
//
	return;
}



/*
============================================================================
 Function:				StateFunction_2()
 Input arguments:		None.
 Output arguments: 		None.
 Returned value:		None.
 Version:				Version 1.00
 Updated:				10/03/2011
 Modifications:			N/A

 Description:

 A sub-states machine function. This function executes the sub-states machine
 of the XY move process.

 The move prcess, in this simplified example consists of the following steps:

 Begin move
 Wait for end of motion

 Each step is handled by a dedicated function. However, calling a function
 is not a must and the relevant code for each sub-state can be directly
 written within the switch-case structure.
============================================================================
*/
void StateFunction_Home_01()
{
	// as our system is absolute, we don't need home operaton
	giState01=eIDLE;
	return;


}

void StateFunction_Home_02()
{
	//because the system is absoluted, we don't need  homeing operation,
	//we do nothing except returing;
	giState02 = eIDLE;
	return;
}

/*
void StateFunction_Stop()
{
	int i ,iCnt;
	iCnt = 0 ;
//
//	Handle the sub-state machine.
//
//	The value of the Sub-State variable is used to make decisions of the sub-states machine and to call,
//	as necessary, the relevant function that handles to process itslef.
//
	switch (giSubState1)
	{
		case eSubState_SM3_Stop:
			//
			// Call stop to all axes.
			for(i = 0 ; i < MAX_AXES ; i++)
			{
				cAxes[i].Stop() ;
			}
			giSubState1 = eSubState_SM3_WStop ;
			break ;
		case eSubState_SM3_WStop:
			//
			// Wait until all axes stopped
			for(i = 0 ; i < MAX_AXES ; i++)
			{
				if (giStatus[i] & NC_AXIS_STAND_STILL_MASK)
				{
					iCnt++ ;
				}
			}
			if(iCnt == MAX_AXES)
			{
				//
				// If we reached here, then all axes came to a stop
				giSubState1 	= eSubState_SM3_PowerOff ;
			}
			break ;
		case eSubState_SM3_PowerOff:
			for(i = 0 ; i < MAX_AXES ; i++)
			{
				// Simply power off.
				cAxes[i].PowerOff() ;
			}
			break ;
			default:
			{
				break;
			}
		}
//
//		The default case. Should not happen, the user can implement error handling.
//
//
	return;
}
*/

void StateFunction_Stop_01()
{

//	Handle the sub-state machine.
//
//	The value of the Sub-State variable is used to make decisions of the sub-states machine and to call,
//	as necessary, the relevant function that handles to process itslef.
//
	switch (giSubState01)
	{
		case eSubState_SM4_Stop:
		{
			// Call stop to all axes.
			printf("01: STOP Stop, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			if(~giStatus01 & NC_AXIS_STOPPING_MASK)
			{
				cAxis01.Stop(gfSetStopDeceleration01) ;
			}
			giSubState01 = eSubState_SM4_WStop ;
			break ;
		}

		case eSubState_SM4_WStop:
		{
			// Wait until all axes stopped
			printf("01: STOP WStop, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
			if (giStatus01 & NC_AXIS_STAND_STILL_MASK)
			{
				// If we reached here, then all axes came to a stop
				giState01		= eIDLE;
			}
				break ;
		}

		default:
		{
				break;
		}
	}
//		The default case. Should not happen, the user can implement error handling.
//
	return;
}

void StateFunction_Stop_02()
{

//	Handle the sub-state machine.
//
//	The value of the Sub-State variable is used to make decisions of the sub-states machine and to call,
//	as necessary, the relevant function that handles to process itslef.
//
	switch (giSubState02)
	{
		case eSubState_SM4_Stop:
			// Call stop to all axes.
			printf("02: STOP Stop, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			if(~giStatus01 & NC_AXIS_STOPPING_MASK)
			{
				cAxis02.Stop(gfSetStopDeceleration02) ;
			}
			giSubState02 = eSubState_SM4_WStop ;
			break ;
		case eSubState_SM4_WStop:
			// Wait until all axes stopped
			printf("02: STOP WStop, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
			if (giStatus02 & NC_AXIS_STAND_STILL_MASK)
			{
				// If we reached here, then all axes came to a stop
				giState02	= eIDLE;
			}
			break ;

/*			if (giStatus02 & NC_AXIS_STAND_STILL_MASK)
			{
				giSubState02 	= eSubState_SM4_PowerOff ;	// If we reached here, then all axes came to a stop
			}
			break ;
		case eSubState_SM4_PowerOff:

			cAxis02.PowerOff() ;	// Simply power off.
			break ;
*/

			//	The default case. Should not happen, the user can implement error handling.
			default:
			{
				break;
			}
		}

	return;
}



void StateFunction_MoveVelocity_01()
{
	//	Handle the sub-state machine.
	//
	//	The value of the Sub-State variable is used to make decisions of the sub-states machine and to call,
	//	as necessary, the relevant function that handles to process itself.
	//
		switch (giSubState01)
		{

			case eSubState_SM3_ChOPMode:
				//
				// change the axes OPMode to Profiile Position
				printf("01: Velocity ChOPMode, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);

				if(cAxis01.GetOpMode() != OPM402_PROFILE_VELOCITY_MODE)
				{
					cAxis01.SetOpMode(OPM402_PROFILE_VELOCITY_MODE) ;
				}
				giSubState01 	= eSubState_SM3_WChOPMode ;
				break ;
			case eSubState_SM3_WChOPMode:
				//
				// Check all axes are in Profiile Position
				printf("01: Velocity WChOPMode, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
				if(cAxis01.GetOpMode() == OPM402_PROFILE_VELOCITY_MODE)
				{
						//
						// If in Profile poition - change rx PDO1 to be On Sync
						// In order for all axes to begin together.
						// PDO1, RX change default to OnSync
					//	cAxes[i].ChangeDefaultPDOConfig(1,0,1) ;
						//cAxes[i].SendSdoCmd(1,1,2,1,0x1400,0) ;
					// If we reached here, then all of the axes are in Profile Position
					giSubState01 	= eSubState_SM3_Move ;
				}
				break ;

			case eSubState_SM3_Move:
			{
				// Goto velocity
				printf("01: Velocity Move, gistate01 = %d, giSubState01 = %d, giStatus01 = %x \r\n", giState01, giSubState01, giStatus01);
				cAxis01.m_fAcceleration = gfSetAcceleration01;
				cAxis01.m_fDeceleration = gfSetDeceleration01;
				cAxis01.MoveVelocity(gfSetVelocity01) ;

				//giSubState1 = eSubState_SM4_WMove1 ;
				giState01=eIDLE;

				break;
			}

			default:	//		The default case. Should not happen, the user can implement error handling.
			{
				break;
			}
		}
	//
		return;

}

void StateFunction_MoveVelocity_02()
{

	//	Handle the sub-state machine.
	//
	//	The value of the Sub-State variable is used to make decisions of the sub-states machine and to call,
	//	as necessary, the relevant function that handles to process itself.
	//
		switch (giSubState02)
		{
			/*case eSubState_SM3_PowerOff:
				//
				// Power Off all axes
				if (!(giStatus02 & NC_AXIS_DISABLED_MASK))
				{
					cAxis02.PowerOff() ;
				}
				giSubState02 	= eSubState_SM3_WPowerOff ;
				break ;
			case eSubState_SM3_WPowerOff:
				//
				// Check all axes are powered off
				if (giStatus02 & NC_AXIS_DISABLED_MASK)
					// If we reached here, then all axes are powered off.
				{
					giSubState02 	= eSubState_SM3_ChOPMode ;
				}
				break ;
				*/
			case eSubState_SM3_ChOPMode:

				//change the axis to profile velocity mode
				printf("02: Velocity ChOPMode, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
				if(cAxis02.GetOpMode() != OPM402_PROFILE_VELOCITY_MODE)
				{
					cAxis02.SetOpMode(OPM402_PROFILE_VELOCITY_MODE) ;
				}
				giSubState02 	= eSubState_SM3_WChOPMode ;
				break ;
			case eSubState_SM3_WChOPMode:
				//
				// Check all axes are in Profiile Position
				printf("02: Velocity WChOPMode, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
				if(cAxis02.GetOpMode() == OPM402_PROFILE_VELOCITY_MODE)
				{
						//
						// If in Profile poition - change rx PDO1 to be On Sync
						// In order for all axes to begin together.
						// PDO1, RX change default to OnSync
					//	cAxes[i].ChangeDefaultPDOConfig(1,0,1) ;
						//cAxes[i].SendSdoCmd(1,1,2,1,0x1400,0) ;
					// If we reached here, then all of the axes are in Profile Position
					giSubState02 	= eSubState_SM3_Move ;
				}
				break ;
			/*
			case eSubState_SM3_PowerOn:

				// Power On all axes.

				if (giStatus02 & NC_AXIS_DISABLED_MASK)
					cAxis02.PowerOn() ;
				//	Changing to the next sub-state
				giSubState02 = eSubState_SM3_WPowerOn;
				break;

			case eSubState_SM3_WPowerOn:
			{
				//
				// wait until all axes are powered on:
				if(giStatus02 & NC_AXIS_STAND_STILL_MASK)
				{
					giSubState02 = eSubState_SM3_Move;
				}
				break;
			}*/
			case eSubState_SM3_Move:
			{
				// Goto velocity
				printf("02: Velocity Move, gistate02 = %d, giSubState02 = %d, giStatus02 = %x \r\n", giState02, giSubState02, giStatus02);
				cAxis02.m_fAcceleration = gfSetAcceleration02;
				cAxis02.m_fDeceleration = gfSetDeceleration02;
				cAxis02.MoveVelocity(gfSetVelocity02) ;

				//giSubState1 = eSubState_SM4_WMove1 ;
				giState02=eIDLE;

				break;
			}

			default:	//		The default case. Should not happen, the user can implement error handling.
			{
				break;
			}
		}
	//
		return;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Function name	:	void callback function																		//
//	Created			:	Version 1.00																				//
//	Updated			:	3/12/2010																					//
//	Modifications	:	N/A																							//
//	Purpose			:	interuprt function 																			//
//																													//
//	Input			:	N/A																							//
//	Output			:	N/A																							//
//	Return Value	:	int																							//
//	Modifications:	:	N/A																							//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CallbackFunc(unsigned char* recvBuffer, short recvBufferSize,void* lpsock)
{
	// Whcih function ID was received ...
	switch(recvBuffer[1])
	{
	case ASYNC_REPLY_EVT:
		printf("ASYNC event Reply\r\n") ;
		break ;
	case EMCY_EVT:
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

void MMC_MbusWriteHoldingRegisterTable_wrapper(MMC_MODBUSWRITEHOLDINGREGISTERSTABLE_IN *mbus_write_in,MMC_MODBUSWRITEHOLDINGREGISTERSTABLE_OUT *mbus_write_out)
{
	if (MMC_MbusWriteHoldingRegisterTable(gConnHndl, mbus_write_in,
			mbus_write_out) != 0) {
		printf("ERROR:%s: MMC_MbusWriteHoldingRegisterTable fail \n",
				__func__);
		exit(-1) ;
	}
}

void MMC_MbusReadHoldingRegisterTable_wrapper(MMC_MODBUSREADHOLDINGREGISTERSTABLE_IN *mbus_read_in, MMC_MODBUSREADHOLDINGREGISTERSTABLE_OUT *mbus_read_out)
{
	if (MMC_MbusReadHoldingRegisterTable(gConnHndl, mbus_read_in,
			mbus_read_out) != 0) {
		printf("ERROR:%s: MMC_MbusReadHoldingRegisterTable fail \n",
				__func__);
		exit(1);
	}
}

void MMC_MbusStopServer_wrapper()
{
	MMC_MODBUSSTOPSERVER_IN mbus_stopserver_in;
	MMC_MODBUSSTOPSERVER_OUT mbus_stopserver_out;

	if(MMC_MbusStopServer(gConnHndl, &mbus_stopserver_in, &mbus_stopserver_out)!=0)
	{
		printf("ERROR:%s: MMC_MbusStoptServer fail \n", __func__);
					MMC_CloseConnection(gConnHndl);
					exit(-1);
	}
}

void InsertLongVarToModbusShortArr(short* spArr, long lVal)
{
	*spArr 		= (short) (lVal	& 0xFFFF);
	*(spArr + 1)= (short)((lVal >> 16) 	& 0xFFFF);
}
int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus)
{
	giState01 = eIDLE;
	giState02 = eIDLE;
	printf("OnRunTimeError: %s,axis ref=%d, err=%d, status=%d, bye\n", msg, usAxisRef, sErrorID, usStatus);
	/*
	MMC_CloseConnection(uiConnHndl);
	printf("OnRunTimeError: %s,axis ref=%d, err=%d, status=%d, bye\n", msg, usAxisRef, sErrorID, usStatus);
	exit(0);
	*/
}

///////////////////////////////////////////////////////////////////////
//	Function name	:	void terminate_application(int iSigNum)
//	Created			:	Version 1.00
//	Updated			:	20/05/2010
//	Modifications	:	N/A
//	Purpose			:	Called in case application is terminated, stop modbus, engines, and power off engines
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
	// Wait till other threads exit properly.
	sleep(1) ;
	return ;
}
//
// Callback Function once a Modbus message is received.
void ModbusWrite_Received()
{
	printf("\nModbus Write Received: giTempState01 = %d, giTempState02 = %d \n\n", giTempState01, giTempState02) ;

}
//
// Callback Function once an Emergency is received.
void Emergency_Received(unsigned short usAxisRef, short sEmcyCode)
{
	printf("Emergency Message Received on Axis %d. Code: %x\n",usAxisRef,sEmcyCode) ;
}
