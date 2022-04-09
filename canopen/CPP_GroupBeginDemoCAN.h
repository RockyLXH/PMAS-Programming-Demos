/*
============================================================================
 Name : CPP_GroupBeginDemo.h
 Author  :		Benjamin Spitzer
 Version :
============================================================================
*/
/*
============================================================================
 Project general functions prototypes
============================================================================
*/
void MainInit();
void MachineSequences();
void MainClose();
void MachineSequencesInit();
void EnableMachineSequencesTimer(int TimerCycle);
void BackgroundProcesses();
void MachineSequencesClose();
int MachineSequencesTimer();
void ReadAllInputData();
void WriteAllOutputData();
void InsertLongVarToModbusShortArr(short* spArr, long lVal) ;
int OnRunTimeError(const char *msg,  unsigned int uiConnHndl, unsigned short usAxisRef, short sErrorID, unsigned short usStatus) ;
void Emergency_Received(unsigned short usAxisRef, short sEmcyCode) ;
void ModbusWrite_Received() ;
void TerminateApplication(int iSigNum);
int  CallbackFunc(unsigned char* recvBuffer, short recvBufferSize,void* lpsock);
/*
============================================================================
 States functions
============================================================================
*/

void StateFunction_PowerOn_01();
void StateFunction_PowerOff_01();
void StateFunction_PTP_01();
void StateFunction_Home_01 () ;
void StateFunction_Stop_01 () ;
void StateFunction_MoveVelocity_01 () ;

void StateFunction_PowerOn_02();
void StateFunction_PowerOff_02();
void StateFunction_PTP_02();
void StateFunction_Home_02 () ;
void StateFunction_Stop_02 () ;
void StateFunction_MoveVelocity_02 () ;

void MMCPP_InitConnection() ;
/*
============================================================================
 General constants
============================================================================
*/
#define 	MAX_AXES				2		// number of Physical axes in the system. TODO Update MAX_AXES accordingly
#define 	FALSE					0
#define 	TRUE					1
#define 	SYNC_MULTIPLIER			2
#define 	NUM_MOTIONS				10
//
// TODO: Modbus memory map offsets must be updated accordingly.
#define 	MODBUS_READ_OUTPUTS_INDEX	0	// Start of Modbus read address
#define 	MODBUS_READ_CNT				18	// Number of registers to read.
#define 	MODBUS_UPDATE_START_INDEX	20	// Start of Modbus write address (update to host)
#define 	MODBUS_UPDATE_CNT			16	// Number of registers to update
//
//  encode resolution
#define		ENCODE_RESOLUTION_01		262144
#define		ENCODE_RESOLUTION_02		65536
/*
============================================================================
 Project constantsA
============================================================================
*/
#define		SLEEP_TIME				10000	// Sleep time of the backround idle loop, in micro seconds
#define		TIMER_CYCLE				20		// Cycle time of the main sequences timer, in ms
/*
============================================================================
 States Machines constants
============================================================================
*/
#define 	FIRST_SUB_STATE			1

enum eMainStateMachines						// TODO: Change names of state machines to reflect dedicated project
{
	eIDLE		= 	0,
	eHome		,						// Main state machine #1 - Home
	ePTP		,						// Main state machine #2 - PTP
	eVelocity	,						// Main state machine #3 - velocity
	eStop		,						// Main state machine #4 - Stop
	ePowerOn	= 10,					// Main state machine #10 - PowerOn
	ePowerOff	,						// Main state machine #11 - PowerOff

} ;

//
// Following values the giSubState1 parameter can receive while giState1 = ePTP
enum eSubStateMachine_PTP
{
	/*eSubState_SM1_PowerOff  = 1	 ,	// Power off
	eSubState_SM1_WPowerOff	 ,	*/		// Wait power off
	eSubState_SM2_ChOPMode =1  ,		// Change Operation mode to PP
	eSubState_SM2_WChOPMode	 ,		// Wait till Operation mode == PP
	eSubState_SM2_PowerOn 	 ,		// Power on
	eSubState_SM2_WPowerOn 	 ,		// Wait until Power on
	eSubState_SM2_Move	 	 ,		// Perform  motion
	eSubState_SM2_WMove 	 ,		// Wait until end of motion

};
//
// Following values the giSubState1 parameter can receive while giState1 = eHome
enum eSubStateMachine_Home
{
	eSubState_SM1_PowerOff  = 1	 ,	// Power off
	eSubState_SM1_WPowerOff	 	 ,	// Wait power off
	eSubState_SM1_ChOPModeHome   ,	// Change Operation mode to Homing
	eSubState_SM1_WChOPModeHome	 ,	// Wait till Operation mode == Homing
	eSubState_SM1_PowerOn 	 ,		// Power on
	eSubState_SM1_WPowerOn 	 ,		// Wait until Power on
	eSubState_SM1_DOHOME 	 ,		// Perform Homing
	eSubState_SM1_WDOHOME 	 ,		// Wait until Homing ended
};

// Following values the giSubState1 parameter can receive while giState1 = ePowerOn
enum eSubStateMachine_PowerOn
{
	eSubState_SM10_PowerOn  = 1	 ,		// Stop
	eSubState_SM10_WPowerOn	 	 ,		// Wait until all came to a stop
};

// Following values the giSubState1 parameter can receive while giState1 = ePowerOff
enum eSubStateMachine_PowerOff
{
	eSubState_SM11_PowerOff  = 1	 ,		// Stop
	eSubState_SM11_WPowerOff	 	 ,		// Wait until all came to a stop
};

//									
// Following values the giSubState1 parameter can receive while giState1 = eStop
enum eSubStateMachine_Stop
{
	eSubState_SM4_Stop  = 1	 ,		// Stop
	eSubState_SM4_WStop	 	 ,		// Wait until all came to a stop
/*	eSubState_SM4_PowerOff ,		// Power off
	eSubState_SM4_WPowerOff ,	*/	// wait until power off
};
//
// Following values the giSubState1 parameter can receive while giState1 = eStop
enum eSubStateMachine_Velocity
{
	/*eSubState_SM3_PowerOff  = 1	 ,	// Power off
	eSubState_SM3_WPowerOff	 ,	*/	// Wait power off
	eSubState_SM3_ChOPMode   =1,		// Change Operation mode to PP
	eSubState_SM3_WChOPMode	 ,		// Wait till Operation mode == PP
	eSubState_SM3_PowerOn 	 ,		// Power on
	eSubState_SM3_WPowerOn 	 ,		// Wait until Power on
	eSubState_SM3_Move 	 ,		// Perform  motion velocity
};

// Following values the giSubState1 parameter can receive while giState1 = eGroupBeginMovePulses
/*enum eSubStateMachine_GroupBeginMovePulses
{
	eSubState_SM4_PowerOff  = 1	 ,	// Power off
	eSubState_SM4_WPowerOff	 ,		// Wait power off
	eSubState_SM4_ChOPMode   ,		// Change Operation mode to PP
	eSubState_SM4_WChOPMode	 ,		// Wait till Operation mode == PP
	eSubState_SM4_PowerOn 	 ,		// Power on
	eSubState_SM4_WPowerOn 	 ,		// Wait until Power on
	eSubState_SM4_Move 	 ,		// Perform  motion velocity
};*/

/*
============================================================================
 Application global variables
============================================================================
*/
int 	giTerminate;		// Flag to request program termination
int		giReentrance;		// Used to detect reentrancy to the main timer function
//
short 	giTempState01;		// Holds temp state1
short	giTempState02;		// Holds temp state2;
short 	giState01;			// Holds the current state of the main state machine
short 	giState02;			// Holds the current state of the main state machine
short 	giPrevState01;		// Holds the value of giState1 at previous cycle
short 	giPrevState02;		// Holds the value of giState1 at previous cycle
short	giSubState01;		// Holds the current state of the sub-state machine
short	giSubState02;		// Holds the current state of the sub-state machine
//
//
// 		Data read from the GMAS core regarding the X, Y drives
//int 	giStatus[MAX_AXES],giStatus01, giStatus02;// Holds PLCopen statuses for all axes
unsigned long 	giStatus01, giStatus02;// Holds PLCopen statuses for all axes
//float 	gfPos[MAX_AXES] , gfPos01, gfPos02;	// Holds position for all axes.
float 	gfPos01, gfPos02;	// Holds position for all axes.
//float	gfVel[MAX_AXES], gfVel01, gfVel02;		//Holds Velocity for axes.
float	gfVel01, gfVel02;	//Holds Velocity for axes.
//
//
//		Data read form modbus for motion parameters
/*
int 	giSetPosition;
int		giSetVelocity;
int		giSetAcceleration;
int		giSetDeceleration;
*/
double 	gdSetPosition01;
float	gfSetVelocity01;
float	gfSetAcceleration01;
float	gfSetDeceleration01;
float	gfSetStopDeceleration01;

double 	gdSetPosition02;
float	gfSetVelocity02;
float	gfSetAcceleration02;
float	gfSetDeceleration02;
float	gfSetStopDeceleration02;

unsigned long giCount1,giCount2;
/*
============================================================================
 Global structures for Elmo's Function Blocks
============================================================================
*/
long long llIO_Val ;
MMC_CONNECT_HNDL gConnHndl ;					// Connection Handle
CMMCConnection cConn ;							// Connection class
//CMMCSingleAxis 	cAxes[MAX_AXES],cAxis01,cAxis02 ;				// Single axis class
CMMCSingleAxis 	cAxis01,cAxis02 ;				// Single axis class
CMMCHostComm	cHost ;							// Modbus host class
CMMCDS401Axis	ds401 ;
MMC_MODBUSWRITEHOLDINGREGISTERSTABLE_IN 	mbus_write_in;
MMC_MODBUSWRITEHOLDINGREGISTERSTABLE_OUT 	mbus_write_out;
MMC_MOTIONPARAMS_SINGLE 	stSingleDefault01, stSingleDefault02 ;	// Single axis default data
MMC_HOMEDS402_IN			stDS402Home ;		// standard DS402 input structure
long UI1,UI2  ;
