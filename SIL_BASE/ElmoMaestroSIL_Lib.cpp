/*
 * ElmoMaestroSIL_Lib.cpp
 *
 *  Created on: 07/03/2017
 *      Author: zivb
 */

#include "ElmoMaestroSIL_Lib.h"
//#include <sys/ioctl.h>

#ifdef _PLATINUM
#include <sys/time.h>
#include <pthread.h>
#endif

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern int iEventDrivenOffset;


#ifdef _PLATINUM
extern void set_Rec(unsigned short usValue);
// Local Definitions for the Serial Port Managmemt for LINUX
int	iSerialBufferSizeArray[MAX_SERIAL_CONN] = {0,0,0,0,0,0,0,0,0,0} ;
int	iSerialPortConfigFlags[MAX_SERIAL_CONN] = {0,0,0,0,0,0,0,0,0,0} ;
//
int iStopBitsLUT[4] = {	0 , CMaestroSerialPort::SB_1 , CMaestroSerialPort::SB_2 ,0 } ;
int iParityLUT  [8] = {	0 ,							// 0 
						CMaestroSerialPort::NONE,	// 1
						CMaestroSerialPort::ODD	,	// 2
						CMaestroSerialPort::EVEN,	// 3
						CMaestroSerialPort::MARK,	// 4
						CMaestroSerialPort::SPACE,	// 5 
						CMaestroSerialPort::NONE,	// 6
						0} ;						// 7
/*
	if (iStopBits == 2) iStopBits = CMaestroSerialPort::SB_2 ;
	else				iStopBits = CMaestroSerialPort::SB_1 ;
	if		(iParity == 1) iParity = CMaestroSerialPort::NONE ;
	else if (iParity == 2) iParity = CMaestroSerialPort::ODD  ;
	else if (iParity == 3) iParity = CMaestroSerialPort::EVEN ;
	else if (iParity == 4) iParity = CMaestroSerialPort::MARK ;
	else if (iParity == 5) iParity = CMaestroSerialPort::SPACE;
	else				   iParity = CMaestroSerialPort::NONE ;
*/
#endif

// MODEL INIT EMPTY FUNCTION
void ElmoSILInitModel(void)
{
	//
	// RONEN-DEBUG
	// This is an EMpty Function just to provide a STUB for the INIT Model CallBack ...
	// We need to handle this mode clean ...
	//
	return;
}

void ElmoSILMultiStepInitModel(void)
{
	//
	// RONEN-DEBUG
	// This is an EMpty Function just to provide a STUB for the INIT Model CallBack ...
	// We need to handle this mode clean ...
	//
	return;
}
// REFERENCE MODEL INIT EMPTY FUNCTION
void ElmoSILInitModelReference(void)
{
	//
	// RONEN-DEBUG
	// This is an EMpty Function just to provide a STUB for the INIT Model CallBack ...
	// We need to handle this mode clean ...
	//
	return;
}

//INIT FUNCTION
void Elmo_Init_607A(unsigned int uiAxis, unsigned int uiSrc)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiSrc-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetBoolParameter(uiSrc,MMC_UCUSER607A_SRC,0);
#endif
	return;
}

void Elmo_Init_60FF(unsigned int uiAxis, unsigned int uiSrc)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiSrc-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetBoolParameter(uiSrc,MMC_UCUSER60FF_SRC,0);
	return;
#endif
}

void Elmo_Init_60B1(unsigned int uiAxis, unsigned int uiSrc)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiSrc-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetBoolParameter(uiSrc,MMC_UCUSER60B1_SRC,0);
#endif
	return;
}
void Elmo_Init_6071(unsigned int uiAxis, unsigned int uiSrc)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiSrc-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
	{
		g_cRTSingleAxisArr[uiAxis].SetBoolParameter(uiSrc,MMC_UCUSER6071_SRC,0);
	}
#endif
	return;
}
void Elmo_Init_60B2(unsigned int uiAxis, unsigned int uiSrc)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiSrc-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetBoolParameter(uiSrc,MMC_UCUSER60B2_SRC,0);
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
//Write function
void Elmo_Write_607A(unsigned int uiAxis, int iUser607Aval)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser607A(iUser607Aval);
#endif
	return;
}
void Elmo_Write_60FF(unsigned int uiAxis, int iUser60FFval)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser60FF(iUser60FFval);
#endif
	return;
}
void Elmo_Write_60B1(unsigned int uiAxis, int iUser60B1val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser60B1(iUser60B1val);
#endif
	return;
}
void Elmo_Write_60B2(unsigned int uiAxis,double iUser60B2val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser60B2(iUser60B2val);
#endif
	return;
}
void Elmo_Write_6071(unsigned int uiAxis,double dUser6071val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser6071(dUser6071val);
#endif
	return;
}

void Elmo_Terminate_60FF(unsigned int uiAxis)
{
	// uiAxis-- ;
#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser60FF(0);
#endif
	return;
}
void Elmo_Terminate_60B1(unsigned int uiAxis)
{
	// uiAxis-- ;
#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser60B1(0);
#endif
	return;
}
void Elmo_Terminate_60B2(unsigned int uiAxis)
{
	// uiAxis-- ;
#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser60B2(0);
#endif
	return;
}
void Elmo_Terminate_6071(unsigned int uiAxis)
{
	// uiAxis-- ;
#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUser6071(0);
#endif
	return;
}
///////////////////////////////////////////////////////////////////////////////
//Read function
void Elmo_Read_607A(unsigned int uiAxis, int *iUser607Aval)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*iUser607Aval) = g_cRTSingleAxisArr[uiAxis].GetUser607A();
#endif
	return;
}
void Elmo_Read_60FF(unsigned int uiAxis, int *iUser60FFval)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*iUser60FFval) = g_cRTSingleAxisArr[uiAxis].GetUser60FF();
#endif
	return;
}
void Elmo_Read_60B1(unsigned int uiAxis, int *iUser60B1val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*iUser60B1val) = g_cRTSingleAxisArr[uiAxis].GetUser60B1();
#endif
	return;
}
void Elmo_Read_60B2(unsigned int uiAxis, double *dUser60B2val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*dUser60B2val) = g_cRTSingleAxisArr[uiAxis].GetUser60B2();
#endif
	return;
}
void Elmo_Read_6071(unsigned int uiAxis, double *dUser6071val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*dUser6071val) = g_cRTSingleAxisArr[uiAxis].GetUser6071();
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// MB1
void Elmo_Write_MB1(unsigned int uiAxis, int iUserMB1)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUserMB1(iUserMB1);
#endif
	return;
}
void Elmo_Read_MB1(unsigned int uiAxis, int *iUserMB1val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*iUserMB1val) = g_cRTSingleAxisArr[uiAxis].GetUserMB1();
#endif
	return;
}
// MB2
void Elmo_Write_MB2(unsigned int uiAxis, int iUserMB2)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUserMB2(iUserMB2);
#endif
	return;
}
void Elmo_Read_MB2(unsigned int uiAxis, int *iUserMB2val)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*iUserMB2val) = g_cRTSingleAxisArr[uiAxis].GetUserMB2();
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Data Recording Double and Long Interfaces
void Elmo_Write_SIL_DATA_Double(int iIndex ,double dVal)
{
#ifdef _PLATINUM
	// iIndex--;
	if(iIndex < SIL_DATA_DOUBLE_NUM)
		g_cGloblaRT.SetSILDoubleData(iIndex,dVal);
#endif
	return;
}
void Elmo_Write_SIL_DATA_Long(int iIndex ,long lVal)
{
#ifdef _PLATINUM
	// iIndex--;
	if(iIndex < SIL_DATA_LONG_NUM)
		g_cGloblaRT.SetSILLongData(iIndex,lVal);
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Read/Write Axis Parameters Interfaces TODO: Ziv Set UI UF
void Elmo_Write_UserParam_Int32(unsigned int uiAxis, unsigned int uiPrmIndex, int iPrmVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if( (uiAxis < g_uiNumOfAxes) && (uiPrmIndex <= UI_UF_MAX_IDX) )
		g_cRTSingleAxisArr[uiAxis].SetUI(iPrmVal,uiPrmIndex);
#endif
	return;
}
void Elmo_Read_UserParam_Int32(unsigned int uiAxis, unsigned int uiPrmIndex, int *iPrmVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;
#ifdef _PLATINUM
	if( (uiAxis < g_uiNumOfAxes) && (uiPrmIndex <= UI_UF_MAX_IDX) )
		(*iPrmVal) = g_cRTSingleAxisArr[uiAxis].GetUI(uiPrmIndex);
#endif
	return;
}
//
void Elmo_Write_UserParam_Float(unsigned int uiAxis, unsigned int uiPrmIndex, float fPrmVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	#ifdef _PLATINUM
	if( (uiAxis < g_uiNumOfAxes) && (uiPrmIndex <= UI_UF_MAX_IDX) )
		g_cRTSingleAxisArr[uiAxis].SetUF(fPrmVal,uiPrmIndex);
	#endif
	return;
}
void Elmo_Read_UserParam_Float(unsigned int uiAxis, unsigned int uiPrmIndex, float *fPrmVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	#ifdef _PLATINUM
	if( (uiAxis < g_uiNumOfAxes) && (uiPrmIndex <= UI_UF_MAX_IDX) )
		(*fPrmVal) = g_cRTSingleAxisArr[uiAxis].GetUF(uiPrmIndex);
	#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// DS402 CW Read Write and SW Read
void Elmo_Write_DS402_CW(unsigned int uiAxis, unsigned short usUserCWval)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetUserCW(usUserCWval);
#endif
	return;
}
void Elmo_Read_DS402_CW(unsigned int uiAxis, unsigned short *usUserCWval)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*usUserCWval) = g_cRTSingleAxisArr[uiAxis].GetControlWord();
#endif
	return;
}
void Elmo_Read_DS402_SW(unsigned int uiAxis, unsigned short *usVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*usVal) = g_cRTSingleAxisArr[uiAxis].GetStatusWord();
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
//Read Axis Positions Velocity Currents Functions
void Elmo_Read_Actual_Position(unsigned int uiAxis, double *dActPos)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*dActPos) = g_cRTSingleAxisArr[uiAxis].GetActualPosition();
#endif
	return;
}
void Elmo_Read_Actual_Velocity(unsigned int uiAxis, double *dActVel)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*dActVel) = g_cRTSingleAxisArr[uiAxis].GetActualVelocity();
#endif
	return;
}
void Elmo_Read_Act_current(unsigned int uiAxis, double *dVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*dVal) = g_cRTSingleAxisArr[uiAxis].GetActualCurrent();
	#endif
	return;
}
void Elmo_Read_Actual_PosVelCurrent(unsigned int uiAxis, AxisData * pAxData)
{	
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

 	#ifdef _PLATINUM
	// NEED TO COMPLETE THE IMPLEMENTATION ...
	// RONEN-DEBUG
	// Optimize the Code to directtly access Shared Memory Data for all members ...
	if(uiAxis < g_uiNumOfAxes )
	{
		pAxData->dPosition	= g_cRTSingleAxisArr[uiAxis].GetActualPosition() ;
		pAxData->dVelocity	= g_cRTSingleAxisArr[uiAxis].GetActualVelocity() ;
		pAxData->dCurrentIq	= g_cRTSingleAxisArr[uiAxis].GetActualCurrent()  ;
		pAxData->dCurrentId	= 0.0	;
		pAxData->uiIsEnabled = !(((g_cRTSingleAxisArr[uiAxis].GetPLCOpenStatus() & NC_AXIS_DISABLED_MASK) == NC_AXIS_DISABLED_MASK)) ;
	}
	#endif
	return;
}

void Elmo_Read_Actual_Position_INT(unsigned int uiAxis, int *iActPos)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*iActPos) = g_cRTSingleAxisArr[uiAxis].GetActualPositionINT();
#endif
	return;
}
void Elmo_Read_Actual_Velocity_INT(unsigned int uiAxis, int *iActVel)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*iActVel) = g_cRTSingleAxisArr[uiAxis].GetActualVelocity();
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Read Profiler Target Position Velocity and Stand Still Status
void Elmo_Read_dpos(unsigned int uiAxis, double *pDesiredPos)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*pDesiredPos) = g_cRTSingleAxisArr[uiAxis].GetdPos();
#endif
	return;
}
void Elmo_Read_dvel(unsigned int uiAxis, double *pDesiredVel)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*pDesiredVel) = g_cRTSingleAxisArr[uiAxis].GetdVel();
#endif
	return;
}
void Elmo_Read_dtorque(unsigned int uiAxis, double *pDesiredTorque)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*pDesiredTorque) = (g_cRTSingleAxisArr[uiAxis].GetdTorqueUU()) ;
	#endif
	return;
}
void Elmo_Read_Profiler_PosVelStat(unsigned int uiAxis, ProfilerData * pProfilerData)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	#ifdef _PLATINUM
	unsigned int uiPLCStatus ;

	//		NEED TO COMPLETE THE IMPLEMENTATION ...
	// RONEN-DEBUG
	// Optimize the Code to directtly access Shared Memory Data for all members ...
	if(uiAxis < g_uiNumOfAxes )
	{
		uiPLCStatus = g_cRTSingleAxisArr[uiAxis].GetPLCOpenStatus();

	    pProfilerData->dPositionCmd		=  g_cRTSingleAxisArr[uiAxis].GetdPos() ;
		pProfilerData->dVelocityCmd		=  g_cRTSingleAxisArr[uiAxis].GetdVel() ;
		pProfilerData->dTorqueCmd		=  (g_cRTSingleAxisArr[uiAxis].GetdTorqueUU()) ;
		pProfilerData->uiIsStandStill	=  (uiPLCStatus & NC_AXIS_STAND_STILL_MASK) ;
		pProfilerData->uiIsEnabled		= !(uiPLCStatus & NC_AXIS_DISABLED_MASK) ;
	}
	#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Read/Write Drive IO'S
void Elmo_Read_Analog_in(unsigned int uiAxis, short *sVel)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*sVel) = g_cRTSingleAxisArr[uiAxis].GetAnalogInput();
#endif
	return;
}
void Elmo_Read_Dig_in(unsigned int uiAxis, unsigned int *uiVel)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*uiVel) = g_cRTSingleAxisArr[uiAxis].GetDigitalInputs();
#endif
	return;
}
void Elmo_Write_Dig_out(unsigned int uiAxis, unsigned int uiVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetDigitalOutPuts(uiVal);
#endif
	return;
}

void Elmo_Terminate_Dig_out(unsigned int uiAxis)
{
	// uiAxis-- ;
#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		g_cRTSingleAxisArr[uiAxis].SetDigitalOutPuts(0);
#endif
	return;
}
///////////////////////////////////////////////////////////////////////////////
// Read Axis Status
void Elmo_Read_PLC_Status(unsigned int uiAxis, unsigned int *uiVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*uiVal) = g_cRTSingleAxisArr[uiAxis].GetPLCOpenStatus();
#endif
	return;
}
void Elmo_Read_Cycle_Count(unsigned int uiAxis, unsigned long *ulVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*ulVal) = g_cRTSingleAxisArr[uiAxis].GetCycleCounter();
#endif
	return;
}
void Elmo_Is_Disabled(unsigned int uiAxis, unsigned int *uiVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*uiVal) = (g_cRTSingleAxisArr[uiAxis].GetPLCOpenStatus() & NC_AXIS_DISABLED_MASK);
#endif
	return;
}
void Elmo_Is_StandStill(unsigned int uiAxis, unsigned int *uiVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*uiVal) = ((g_cRTSingleAxisArr[uiAxis].GetPLCOpenStatus() & NC_AXIS_STAND_STILL_MASK) == NC_AXIS_STAND_STILL_MASK);
#endif
	return;
}
void Elmo_Read_Status_reg(unsigned int uiAxis, unsigned int *uiVal)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	if(uiAxis < g_uiNumOfAxes )
		(*uiVal) = g_cRTSingleAxisArr[uiAxis].GetStatusRegister();
#endif
	return;
}
void Elmo_Read_PLC_StatusEx(unsigned int uiAxis, PLCAxiStatus * pPLCAxiStatus)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

#ifdef _PLATINUM
	unsigned int uiPLCStat ;
	if(uiAxis < g_uiNumOfAxes )
	{
		uiPLCStat = g_cRTSingleAxisArr[uiAxis].GetPLCOpenStatus() ;

		pPLCAxiStatus->uiPLCStatus		= uiPLCStat ;
		pPLCAxiStatus->uiIsStatusReg	= g_cRTSingleAxisArr[uiAxis].GetStatusRegister();
		pPLCAxiStatus->uiIsDisabled		= uiPLCStat & NC_AXIS_DISABLED_MASK ;
		pPLCAxiStatus->uiIsEnabled		= uiPLCStat & NC_AXIS_STAND_STILL_MASK ;
		pPLCAxiStatus->uiIsStandStill	= uiPLCStat & NC_AXIS_STAND_STILL_MASK ;
		pPLCAxiStatus->uiIsInError		= uiPLCStat & NC_AXIS_ERROR_STOP_MASK ;
	}
#endif
	return;
}


///////////////////////////////////////////////////////////////////////////////
// Read Maestro Analog Inputs
void Elmo_Read_CPLD_ANG_IN_CH1(double *dVal)
{
#ifdef _PLATINUM
		(*dVal) = g_cGloblaRT.GetCpldAdcCh1();
#endif
	return;
}
void Elmo_Read_CPLD_ANG_IN_CH2( double *dVal)
{
#ifdef _PLATINUM
		(*dVal) = g_cGloblaRT.GetCpldAdcCh2();
#endif
	return;
}
void Elmo_Read_CPLD_ANG_IN_CH3(double *dVal)
{
#ifdef _PLATINUM
		(*dVal) = g_cGloblaRT.GetCpldAdcCh3();
#endif
	return;
}
void Elmo_Read_CPLD_ANG_IN_CH4( double *dVal)
{
#ifdef _PLATINUM
		(*dVal) = g_cGloblaRT.GetCpldAdcCh4();
#endif
	return;
}
void Elmo_Read_CPLD_ANG_INPUTS	(MaestroAnalogInputs * pMaestroAnalogInputs)
{
#ifdef _PLATINUM
	pMaestroAnalogInputs->dAIn1 = g_cGloblaRT.GetCpldAdcCh1();
	pMaestroAnalogInputs->dAIn2 = g_cGloblaRT.GetCpldAdcCh2();
	pMaestroAnalogInputs->dAIn3 = g_cGloblaRT.GetCpldAdcCh3();
	pMaestroAnalogInputs->dAIn4 = g_cGloblaRT.GetCpldAdcCh4();
#endif
	return ;
}


///////////////////////////////////////////////////////////////////////////////
// Write Maestro Analog Outputs
void Elmo_Write_CPLD_ANG_OUT_CH1(double dVal)
{
#ifdef _PLATINUM
		g_cGloblaRT.SetCpldAdcOUTCh1(dVal);
#endif
	return;
}
void Elmo_Write_CPLD_ANG_OUT_CH2(double dVal)
{
#ifdef _PLATINUM
		g_cGloblaRT.SetCpldAdcOUTCh2(dVal);
#endif
	return;
}
void Elmo_Write_CPLD_ANG_OUT_CH3(double dVal)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldAdcOUTCh3(dVal);
#endif
	return;
}
void Elmo_Write_CPLD_ANG_OUT_CH4(double dVal)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldAdcOUTCh4(dVal);
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
void Elmo_Terminate_CPLD_ANG_OUT_CH1(void)
{
#ifdef _PLATINUM
		g_cGloblaRT.SetCpldAdcOUTCh1(0);
#endif
	return;
}
void Elmo_Terminate_CPLD_ANG_OUT_CH2(void)
{
#ifdef _PLATINUM
		g_cGloblaRT.SetCpldAdcOUTCh2(0);
#endif
	return;
}
void Elmo_Terminate_CPLD_ANG_OUT_CH3(void)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldAdcOUTCh3(0);
#endif
	return;
}
void Elmo_Terminate_CPLD_ANG_OUT_CH4(void)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldAdcOUTCh4(0);
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Read/Write Maestro Extended Inputs
void Elmo_Read_CPLD_EXTIO_IN1(unsigned int *ulVal)
{
#ifdef _PLATINUM
		(*ulVal) = g_cGloblaRT.GetCpldExtendedIOIn1();
#endif
	return;
}
void Elmo_Write_CPLD_EXTIO_OUT1(unsigned long ulVal)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldExtendedIOOut1((uint32_t)ulVal);
#endif
	return;
}
void Elmo_Write_CPLD_EXTIO_OUT2(unsigned long ulVal)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldExtendedIOOut2((uint32_t)ulVal);
#endif
	return;
}
void Elmo_Write_CPLD_EXTIO_OUT3(unsigned long ulVal)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldExtendedIOOut3((uint32_t)ulVal);
#endif
	return;
}

void Elmo_Terminate_CPLD_EXTIO_OUT1(void)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldExtendedIOOut1(0);
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Read Maestro Quad Encoders Pos & Vel
void Elmo_Read_CPLD_Quad1PosUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldQuad1PosUU();
#endif
     return;
}

void Elmo_Read_CPLD_Quad2PosUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldQuad2PosUU();
#endif
     return;
}

void Elmo_Read_CPLD_Quad1VelUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldQuad1Vel();
#endif
     return;
}

void Elmo_Read_CPLD_Quad2VelUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldQuad2Vel();
#endif
     return;
}

///////////////////////////////////////////////////////////////////////////////
// Read Maestro Abs Encoders Pos & Vel
void Elmo_Read_CPLD_Abs1PosUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldAbs1PosUU();
#endif
     return;
}

void Elmo_Read_CPLD_Abs2PosUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldAbs2PosUU();
#endif
     return;
}

void Elmo_Read_CPLD_Abs1VelUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldAbs1Vel();
#endif
     return;
}

void Elmo_Read_CPLD_Abs2VelUU(double *dVal)
{
#ifdef _PLATINUM
          *dVal = g_cGloblaRT.GetCpldAbs2Vel();
#endif
     return;
}

///////////////////////////////////////////////////////////////////////////////
// Read/Write Maestro Differential IOs
void Elmo_Read_CPLD_DIFF_IO(unsigned short *usVal)
{
#ifdef _PLATINUM
		(*usVal) = g_cGloblaRT.GetCpldDiffIOIn();
#endif
	return;
}
void Elmo_Write_CPLD_DIFF_IO(unsigned short usVal)
{
#ifdef _PLATINUM
	g_cGloblaRT.SetCpldDiffIOout(usVal);
#endif
	return;
}

/*//////////////////////////////////////////////////////////////////////////////
// Read/Write PMAS Group Of Parameters
void Elmo_Read_GroupOfParameters(MMC_READGROUPOFPARAMETERS_IN* stIn, MMC_READGROUPOFPARAMETERS_OUT* stOut)
{
#ifdef _PLATINUM
	int rc = MMC_ReadGroupOfParameters(g_hConn, stIn, stOut);

	if (rc != 0)
	{
		char cMsg[60];
		sprintf(cMsg, "MMC_ReadGroupOfParameters : <Problematic entry = %d>\n", stOut->ucProblematicEntry);
		CMMCPPGlobal::Instance()->MMCPPThrow(cMsg, hConnIn, stIn->sParameters[0].usAxisRef, rc, stOut->usErrorID, stOut->usStatus);
	}
#endif
	return;
}

void Elmo_Write_GroupOfParametersEX(MMC_AXIS_REF_HNDL usAxisRef, MMC_WRITEGROUPOFPARAMETERSEX_IN* stIn, MMC_WRITEGROUPOFPARAMETERSEX_OUT* stOut)
{
#ifdef _PLATINUM
	int rc = MMC_WriteGroupOfParametersEX(g_hConn, usAxisRef, stIn, stOut);

	if (rc != 0)
	{
		char cMsg[60];
		sprintf(cMsg, "MMC_WriteGroupOfParametersEX : <Problematic entry = %d>\n",stOut->ucProblematicEntry);
		CMMCPPGlobal::Instance()->MMCPPThrow(cMsg, hConnIn, usAxisRef, rc, stOut->usErrorID, stOut->usStatus);
	}
#endif
	return;
}*/

///////////////////////////////////////////////////////////////////////////////
//PI write
void Elmo_Init_PI_Var(unsigned int uiAxis,unsigned short usIndex,unsigned short usPIVarOffset,unsigned int uiDirection)
{
	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	// uiAxis-- ;

	// RONEN-DEBUG
	// uiDirection--;

#ifdef _PLATINUM
	fprintf(stderr,"%s uiAxis = %d usIndex = %d usPIVarOffset = %d uiDirection =%d\n",__func__,uiAxis,usIndex,usPIVarOffset,uiDirection);
	//	if ( ((g_cPIVarRT[usIndex].GetPIDirecton() == ePI_INPUT) && (g_cPIVarRT[usIndex].GetNumOfPIIn() >= usPIVarOffset)) ||
	//	((g_cPIVarRT[usIndex].GetPIDirecton() == ePI_OUTPUT) && (g_cPIVarRT[usIndex].GetNumOfPIOut() >= usPIVarOffset)) )
	g_cPIVarRT[usIndex].Init(g_hConn,/*uiAxis*/g_cRTSingleAxisArr[uiAxis].GetRef(),usPIVarOffset,(PI_DIRECTIONS)uiDirection);
#endif
	return;
}

void Elmo_Write_PI_UChar(unsigned short usIndex ,unsigned char ucVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(ucVal);
#endif
	return;
}
void Elmo_Write_PI_Char(unsigned short usIndex ,signed char scVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(scVal);
#endif
	return;
}
void Elmo_Write_PI_UShort(unsigned short usIndex , unsigned short usVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(usVal);
#endif
	return;
}
void Elmo_Write_PI_Short(unsigned short usIndex , short sVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(sVal);
#endif
	return;
}
void Elmo_Write_PI_UInt(unsigned short usIndex , unsigned int uiVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(uiVal);
#endif
	return;
}
void Elmo_Write_PI_Int(unsigned short usIndex ,int iVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(iVal);
#endif
	return;
}
void Elmo_Write_PI_Float(unsigned short usIndex ,float fVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(fVal);
#endif
	return;
}
void Elmo_Write_PI_Double(unsigned short usIndex ,double dVal)
{

#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].WritePI(dVal);
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
//PI read


void Elmo_Read_PI_Char(unsigned short usIndex, signed char *cVal)
{
#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].ReadPI( *cVal);
#endif
	return;
}
void Elmo_Read_PI_UChar(unsigned short usIndex,unsigned char *ucVal)
{
#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].ReadPI(*ucVal);
#endif
	return;
}
void Elmo_Read_PI_UShort(unsigned short usIndex,unsigned short *usVal)
{
#ifdef _PLATINUM
	g_cPIVarRT[usIndex].ReadPI(*usVal);
#endif
	return;
}
void Elmo_Read_PI_Short(unsigned short usIndex, short *sVal)
{
#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].ReadPI(*sVal);
#endif
	return;
}
void Elmo_Read_PI_UInt(unsigned short usIndex, unsigned int *uiVal)
{
#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].ReadPI(*uiVal);
#endif
	return;
}
void Elmo_Read_PI_Int(unsigned short usIndex,int *iVal)
{
#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].ReadPI(*iVal);
#endif
	return;
}
void Elmo_Read_PI_Float(unsigned short usIndex,float *fVal)
{
#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].ReadPI(*fVal);
#endif
	return;
}
void Elmo_Read_PI_Double(unsigned short usIndex,double *dVal)
{
#ifdef _PLATINUM
	if(usIndex < MAX_PI)
		g_cPIVarRT[usIndex].ReadPI(*dVal);
#endif
	return;
}


///////////////////////////////////////////////////////////////////////////////
void Elmo_Init_ModBus()
{
#ifdef _PLATINUM
	g_cHostComm.MbusStartServer(g_hConn,0);
#endif
	return;
}

void Elmo_Read_ModBus(int startRef, int refCnt,short sArr[250])
{
//	MMC_MODBUSREADHOLDINGREGISTERSTABLE_OUT stOutParams;
//	g_cHostComm.MbusReadHoldingRegisterTable(startRef,refCnt, stOutParams);
//	memcpy(sArr,stOutParams.regArr,sizeof(sArr));
//	return;
}

///////////////////////////////////////////////////////////////////////////////
// Axis State Machine Handling Functions
//
void AxisClearError(unsigned short uiAxis)
{	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	uiAxis-- ;

#ifdef _PLATINUM
	if( ( uiAxis < g_uiNumOfAxes ) &&  !(g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiIsInSequence & NC_SIL_RESET_REQ) )
	{
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiApiReq |= NC_SIL_RESET_REQ;
	}
#endif
	return;
}

void AxisEnable(unsigned short uiAxis, short action)
{	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	uiAxis-- ;
	
#ifdef _PLATINUM
	if(action)
	{
		printf("%s NC_SIL_POWER_ON_REQ\n",__func__);
		if(( uiAxis < g_uiNumOfAxes ) && !(g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiIsInSequence & NC_SIL_POWER_ON_REQ) )
		{
			g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiApiReq |= NC_SIL_POWER_ON_REQ;
		}
	}
	else
	{
		printf("%s NC_SIL_POWER_OFF_REQ\n",__func__);
		if(( uiAxis < g_uiNumOfAxes ) && !(g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiIsInSequence & NC_SIL_POWER_OFF_REQ) )
		{
			g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiApiReq |= NC_SIL_POWER_OFF_REQ;
		}
	}
#endif
	return;
}

void AxisHome(unsigned short uiAxis, HomeData *pHomeDataStruct)
{	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	uiAxis-- ;

#ifdef _PLATINUM
	if(( uiAxis < g_uiNumOfAxes ) && !(g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiIsInSequence & NC_SIL_HOME_REQ) )
	{
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_POS_IDX] = pHomeDataStruct->dPosition;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_METH_IDX] = pHomeDataStruct->uiHomingMethod;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_VEl_HI_IDX] = pHomeDataStruct->fVelHi;   //speed search for switch
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_VEL_LOW_IDX] = pHomeDataStruct->fVelLo;   //speed search for zero
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_ACC_IDX] = pHomeDataStruct->fAcc;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_DIST_LIM_IDX] = pHomeDataStruct->fDistanceLimit;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_DETECT_VEL_LIM_IDX] = pHomeDataStruct->fDetectVelLimit;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_TORQ_LIM_IDX] = pHomeDataStruct->fTorqueLimit;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_TIME_LIM_IDX] = pHomeDataStruct->uiTimeLimit;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MB_CMD_HOME_DETECT_TIME_LIM_IDX] = pHomeDataStruct->uiDetectTimeLimit;

		g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiApiReq |= NC_SIL_HOME_REQ;
	}
#endif
	return;
}

void AxisChangeOpMode(unsigned short uiAxis, short opmod)
{	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
/*
 * typedef enum OP_MODE_DS402
{
	OPM402_NO = -1,
	OPM402_PROFILE_POSITION_MODE = 1,
	OPM402_PROFILE_VELOCITY_MODE = 3,
	OPM402_TORQUE_PROFILE_MODE = 4,
	OPM402_HOMING_MODE = 6,
	OPM402_INTERPOLATED_POSITION_MODE = 7,
	OPM402_CYCLIC_SYNC_POSITION_MODE = 8,
	OPM402_CYCLIC_SYNC_VELOCITY_MODE = 9,
	OPM402_CYCLIC_SYNC_TORQUE_MODE = 10,
}OPM402;
 */
		
	uiAxis-- ;
	
#ifdef _PLATINUM
	printf("****** %s *********\n",__func__);
	if( ( uiAxis < g_uiNumOfAxes ) && !(g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiIsInSequence & NC_SIL_CHG_MOD_REQ) )
	{
		switch(opmod)
		{
		case OPM402_HOMING_MODE:
		case OPM402_CYCLIC_SYNC_POSITION_MODE:
		case OPM402_CYCLIC_SYNC_VELOCITY_MODE:
		case OPM402_CYCLIC_SYNC_TORQUE_MODE:
			g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[OPMOD_REQ_IDX] = opmod;
			g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiApiReq |= NC_SIL_CHG_MOD_REQ;
			printf("Op Mode OK\n");
			break ;
		default:
			printf("Op Mode Not Leagal. Command Ignored.\n");
			break ;
		}
	}
#endif
	return;
}

void AxisMotion(unsigned short uiAxis, MotionData *pMotioDataStruct) 
{	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	uiAxis-- ;
	//
	//data should be a structure that hold the motion data
	//we will set this data in the RT axis data
#ifdef _PLATINUM
	if( ( uiAxis < g_uiNumOfAxes ) &&  !(g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiIsInSequence & NC_SIL_MOVE_REQ) )
	{
		//	TODO: set motion input data
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MOTION_BUFF_MOD_IDX] = pMotioDataStruct->lMotionMode;
		// g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MOTION_LPOS_IDX] 	= pMotioDataStruct->lPosition;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MOTION_DPOS_IDX]		= pMotioDataStruct->dPosition;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MOTION_VEL_IDX] 		= pMotioDataStruct->fVel;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MOTION_AC_IDX] 		= pMotioDataStruct->fAcc;
		g_cRTSingleAxisArr[uiAxis].m_stAPICom.data[MOTION_JERK_IDX] 	= pMotioDataStruct->fJrk;

		g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiApiReq |= NC_SIL_MOVE_REQ;
	}
#endif
	return;
}

void IsDoneProcessing(unsigned short uiAxis, unsigned short * pusDone)
{	//
	// RONEN-DEBUG
	// Need to adress the ENUM 0 Start Point on all Functions getting an Axis as an Input ...
	uiAxis-- ;

#ifdef _PLATINUM
	if( uiAxis < g_uiNumOfAxes )
	{
		*pusDone = !(g_cRTSingleAxisArr[uiAxis].m_stAPICom.uiIsInSequence);
	}
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 2018_01_07_ Ronen / Ziv / Bebji
// STIM-300 IMU New Support for Gyro Interfaces ...
//
//IMU FUNCTION
void Stim300IMU_Init(unsigned short usIndex,unsigned char cPort_index,int iBaud,unsigned int iSize)
{
#ifdef _PLATINUM
	g_cIMU[usIndex].init(cPort_index,iBaud,iSize);
#endif
	return;
}

void Stim300IMU_ReadGyroX(unsigned short usIndex,int *iValue)
{
#ifdef _PLATINUM
	STIM300Datagram *pSTIMDATA;
	pSTIMDATA = (STIM300Datagram *)(g_cIMU[usIndex].m_Buff);
	*iValue = GYRO_CONVERT(pSTIMDATA->gyroX[0],pSTIMDATA->gyroX[1],pSTIMDATA->gyroX[2]);
#endif
	return;
}

void Stim300IMU_ReadGyroY(unsigned short usIndex,int *iValue)
{
#ifdef _PLATINUM
	STIM300Datagram *pSTIMDATA;
	pSTIMDATA = (STIM300Datagram *)(g_cIMU[usIndex].m_Buff);
	*iValue = GYRO_CONVERT(pSTIMDATA->gyroY[0],pSTIMDATA->gyroY[1],pSTIMDATA->gyroY[2]);
#endif
	return;
}

void Stim300IMU_ReadGyroZ(unsigned short usIndex,int *iValue)
{
#ifdef _PLATINUM
	STIM300Datagram *pSTIMDATA;
	pSTIMDATA = (STIM300Datagram *)(g_cIMU[usIndex].m_Buff);
	*iValue = GYRO_CONVERT(pSTIMDATA->gyroZ[0],pSTIMDATA->gyroZ[1],pSTIMDATA->gyroZ[2]);
#endif
	return;
}

void Stim300IMU_ReadAccX(unsigned short usIndex,int *iValue)
{
#ifdef _PLATINUM
	STIM300Datagram *pSTIMDATA;
	pSTIMDATA = (STIM300Datagram *)(g_cIMU[usIndex].m_Buff);
	*iValue = ACCELEROMETER_CONVERT(pSTIMDATA->accelerometerX[0],pSTIMDATA->accelerometerX[1],pSTIMDATA->accelerometerX[2]);
#endif
	return;
}

void Stim300IMU_ReadAccY(unsigned short usIndex,int *iValue)
{
#ifdef _PLATINUM
	STIM300Datagram *pSTIMDATA;
	pSTIMDATA = (STIM300Datagram *)(g_cIMU[usIndex].m_Buff);
	*iValue = ACCELEROMETER_CONVERT(pSTIMDATA->accelerometerY[0],pSTIMDATA->accelerometerY[1],pSTIMDATA->accelerometerY[2]);
#endif
	return;
}

void Stim300IMU_ReadAccZ(unsigned short usIndex,int *iValue)
{
#ifdef _PLATINUM
	STIM300Datagram *pSTIMDATA;
	pSTIMDATA = (STIM300Datagram *)(g_cIMU[usIndex].m_Buff);
	*iValue = ACCELEROMETER_CONVERT(pSTIMDATA->accelerometerZ[0],pSTIMDATA->accelerometerZ[1],pSTIMDATA->accelerometerZ[2]);
#endif
	return;
}
void Stim300IMU_ReadData(unsigned short usIndex,IMUData *stData)
{
#ifdef _PLATINUM
	STIM300Datagram *pSTIMDATA;
	pSTIMDATA = (STIM300Datagram *)(g_cIMU[usIndex].m_Buff);
	stData->iGyroX = GYRO_CONVERT(pSTIMDATA->gyroX[0],pSTIMDATA->gyroX[1],pSTIMDATA->gyroX[2]);
	stData->iGyroY = GYRO_CONVERT(pSTIMDATA->gyroY[0],pSTIMDATA->gyroY[1],pSTIMDATA->gyroY[2]);
	stData->iGyroZ = GYRO_CONVERT(pSTIMDATA->gyroZ[0],pSTIMDATA->gyroZ[1],pSTIMDATA->gyroZ[2]);
	stData->accelerometerX= ACCELEROMETER_CONVERT(pSTIMDATA->accelerometerX[0],pSTIMDATA->accelerometerX[1],pSTIMDATA->accelerometerX[2]);
	stData->accelerometerY= ACCELEROMETER_CONVERT(pSTIMDATA->accelerometerY[0],pSTIMDATA->accelerometerY[1],pSTIMDATA->accelerometerY[2]);
	stData->accelerometerZ= ACCELEROMETER_CONVERT(pSTIMDATA->accelerometerZ[0],pSTIMDATA->accelerometerZ[1],pSTIMDATA->accelerometerZ[2]);
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
// 2018_02_20 Ronen
//
// Elmo UDP Interfaces
//
void Elmo_UDP_Send_Init (unsigned short usID, unsigned int uiDataLen, unsigned char * cHostIP, unsigned short usHostPort)
{
#ifdef _PLATINUM
	// Save identifaction
	if (usID > MAX_UDP_CONN) return ;
	// printf("Enter UDP SEND Init \n");
	// Save data length
	g_saClientList[usID]._dataMsg.uiLen = (uiDataLen > MAX_MATLAB_UDPMSG_BUFFER_SIZE) ? MAX_MATLAB_UDPMSG_BUFFER_SIZE:uiDataLen ;

	// Connect to Server Host
	g_cUDPClient[usID].Connect((char *)cHostIP, usHostPort, g_bWait[usID]);
	// Print Send IP & Port
	printf("%s IP is %s , Port is %d \n",__func__,cHostIP,usHostPort);
#endif
}

void Elmo_UDP_Receive_Init (unsigned short usID, unsigned int uiDataLen, unsigned short usLocalPort)
{
#ifdef _PLATINUM
	int rc = 0;
	// Save identifaction
	if (usID > MAX_UDP_CONN) return ;
	// printf("Enter UDP RCV Init \n");
	// Create UDP server to receive data.
	g_saServerList[usID]._dataMsg.uiLen = (uiDataLen > MAX_MATLAB_UDPMSG_BUFFER_SIZE) ? MAX_MATLAB_UDPMSG_BUFFER_SIZE:uiDataLen;
	g_saServerList[usID]._dataMsg.usServerPort = usLocalPort; // will be use in callback function to identify
	rc = g_cUDPServer[usID].Create (usLocalPort, (SOCK_CLBK)UDPRcvClbk, g_saServerList[usID]._dataMsg.uiLen);
	// Print our Local Port
	printf("%s Local Port is: %d\n",__func__,usLocalPort);
#endif
}

void Elmo_UDP_Send(unsigned short usID, /*unsigned int uiDataLen,*/ unsigned int uiSendDataLen, unsigned char * pBuffer)
{
#ifdef _PLATINUM
	// Check if we have somthing to Send ...
	if (uiSendDataLen)
	{	// Validate Port
		if (usID > MAX_UDP_CONN) return ;
		// Check Sizes
		if (uiSendDataLen > g_saClientList[usID]._dataMsg.uiLen) uiSendDataLen = g_saClientList[usID]._dataMsg.uiLen ;
		// COpy the Local SFUNC Buffer to the UDP Thread Buffer ...
		memcpy(g_saClientList[usID]._dataMsg._szData, pBuffer,uiSendDataLen);
		// Set the Size to Send ...
		g_saClientList[usID]._dataMsg.uiSendData = uiSendDataLen ;
		// Wake up send thread
		// atomic_set_bits(UDP_DATA_SEND,&(g_saClientList[usID]._dataMsg.uiSendData));
		sem_post(&SemUDP);
	}
	
#endif
	return ;
}

void Elmo_UDP_Receive (unsigned short usID, /*unsigned int uiDataLen,*/ int * iNumByteReceived, unsigned char * pBuffer)
{
#ifdef _PLATINUM
	// Validate Sizes
	if (usID > MAX_UDP_CONN) return ;
	// if (uiDataLen > MAX_MATLAB_UDPMSG_BUFFER_SIZE) uiDataLen = MAX_MATLAB_UDPMSG_BUFFER_SIZE ;
	//
	if( g_saServerList[usID]._dataMsg.uiDataReady & UDP_DATA_READY )
		{
			// Mark to the SFUNC that there is New Data available in the Buffer 
			(*iNumByteReceived) = g_saServerList[usID]._dataMsg.iUdpRcvDataSize[g_saServerList[usID]._dataMsg._cBuffId];
			// Copy the New Data from the correct UDP Thread Buffer to the SFUNC buffer ...

			if ( (unsigned int)(*iNumByteReceived) > g_saServerList[usID]._dataMsg.uiLen)
			{
				(*iNumByteReceived) = g_saServerList[usID]._dataMsg.uiLen;
			}
			memcpy(pBuffer,&g_saServerList[usID]._dataMsg._szDataRcv[g_saServerList[usID]._dataMsg._cBuffId],(*iNumByteReceived) );

			//clear data ready
			//atomic_clear_bit(UDP_DATA_READY,&(g_saClientList[usID]._dataMsg.uiDataReady));
			g_saServerList[usID]._dataMsg.uiDataReady =0;
		}
	else (*iNumByteReceived) = 0 ;

#endif
	return ;
}

void Elmo_TCP_Server_Init (unsigned short usID, unsigned int uiDataLen, unsigned short usLocalPort)
{
#ifdef _PLATINUM
	int rc = 0;
	// Save identifaction
	if (usID >= MAX_TCP_CONN)
	{
		return ;
	}

	// Create TCP server to receive data.
	g_TCPServerList[usID]._dataMsg.uiLen = (uiDataLen > MAX_MATLAB_UDPMSG_BUFFER_SIZE) ? MAX_MATLAB_UDPMSG_BUFFER_SIZE:uiDataLen;
	g_TCPServerList[usID]._dataMsg.usServerPort = usLocalPort; // will be use in callback function to identify

	rc = g_cTCPServer[usID].Create (usLocalPort, (SOCK_CLBK)TCPServerRcvClbk, g_TCPServerList[usID]._dataMsg.uiLen);

	// Print Port & Socket ID
	printf(ANSI_COLOR_GREEN		"%s() Port = %d, Socket ID = %d, SERVER Size %d"		 ANSI_COLOR_RESET "\n", __func__, usLocalPort, usID,g_TCPServerList[usID]._dataMsg.uiLen);

#endif
}

void Elmo_TCP_Server_Receive(unsigned short usID, int * iNumByteReceived, unsigned char * pBuffer)
{
#ifdef _PLATINUM

	// Save identifaction
	if (usID >= MAX_TCP_CONN)
	{
		return ;
	}

	server_TCP_s_t* pServerListReceive = &g_TCPServerList[usID];

	//check if instace has been init the data length should not be zero
	if(pServerListReceive->_dataMsg.uiLen == 0)
	{
		return;
	}

	if( pServerListReceive->_dataMsg.uiRcvDataReady & TCP_DATA_READY )
		{
			// Mark to the SFUNC that there is New Data available in the Buffer
			(*iNumByteReceived) = pServerListReceive->_dataMsg.iTcpRcvDataSize[pServerListReceive->_dataMsg._cBuffId];

			// Copy the New Data from the correct TCP Thread Buffer to the SFUNC buffer ...

			if ( (unsigned int)(*iNumByteReceived) > pServerListReceive->_dataMsg.uiLen)
			{
				(*iNumByteReceived) = pServerListReceive->_dataMsg.uiLen;
			}

			memcpy(pBuffer,&pServerListReceive->_dataMsg._szDataRcv[pServerListReceive->_dataMsg._cBuffId],(*iNumByteReceived) );
			//Clear data ready. No need for atomic_clear_bit() since we're running with the highest priority
			pServerListReceive->_dataMsg.uiRcvDataReady = 0;
		}
	else (*iNumByteReceived) = 0 ;
#endif
}

void Elmo_TCP_Server_Send(unsigned short usID, /*unsigned int uiDataLen,*/ unsigned int uiSendDataLen, unsigned char * pBuffer)
{
#ifdef _PLATINUM

	if (usID >= MAX_TCP_CONN)
	{
		return ;
	}

	server_TCP_s_t* pServerListSend = &g_TCPServerList[usID];

	//check if instace has been init the data length should not be zero
	if(pServerListSend->_dataMsg.uiLen == 0)
	{
		return;
	}

	// Check if we have somthing to Send ...
	if (uiSendDataLen)
	{	// Check Sizes
		if (uiSendDataLen > pServerListSend->_dataMsg.uiLen) uiSendDataLen = pServerListSend->_dataMsg.uiLen ;
		// COpy the Local SFUNC Buffer to the UDP Thread Buffer ...
		memcpy(pServerListSend->_dataMsg._szData, pBuffer,uiSendDataLen);
		// Set the Size to Send ...
		pServerListSend->_dataMsg.uiDataReady = uiSendDataLen ;
		// Wake up send thread
		// atomic_set_bits(UDP_DATA_SEND,&(g_saClientList[usID]._dataMsg.uiSendData));
		sem_post(&SemTCP);
	}
#endif
}

//TCP Client
void Elmo_TCP_Client_Init (unsigned short usID, unsigned int uiDataLen, unsigned char * cHostIP, unsigned short usHostPort)
{
#ifdef _PLATINUM
	int rc = 0;

	pthread_t 		hTCPClientReceiveThread[MAX_TCP_CONN];
	pthread_attr_t 	hTCPClientReceiveThreadAttr[MAX_TCP_CONN];

	g_TCPClientList[usID].args_tcp_client_rcv.usID		 = usID;
	g_TCPClientList[usID].args_tcp_client_rcv.uiDataLen  = uiDataLen;
	g_TCPClientList[usID].args_tcp_client_rcv.cHostIP 	 = cHostIP;
	g_TCPClientList[usID].args_tcp_client_rcv.usHostPort = usHostPort;

	// Save identifaction
	if (usID >= MAX_TCP_CONN)
	{
		return;
	}

	// Create TCP server to receive data.
	if(g_TCPClientList[usID]._dataMsg.uiLen)	// If Client was opened before - close it before reopening
	{
//		pthread_join(hTCPClientReceiveThread[usID], NULL);
		g_cTCPClient[usID].Close(g_TCPClientList[usID]._iClientSocket);
	}
	g_TCPClientList[usID]._dataMsg.uiLen = (uiDataLen > MAX_MATLAB_UDPMSG_BUFFER_SIZE) ? MAX_MATLAB_UDPMSG_BUFFER_SIZE:uiDataLen;
	g_TCPClientList[usID]._dataMsg.usServerPort = usHostPort; // will be use in callback function to identify

	// Connect to Server Host
	rc = g_cTCPClient[usID].Connect((char *)cHostIP, usHostPort, g_TCPClientList[usID]._iClientSocket, g_TCPbWait[usID]);

	if (rc)
	{
		cerr << __func__ << ": connecting to server failed. g_usPort = " << usHostPort  << endl;
	}
	// Print IP, Port & Socket ID
	printf(ANSI_COLOR_GREEN		"%s() IP = %s , Port = %d, Socket ID = %d"		 ANSI_COLOR_RESET "\n", __func__, cHostIP, usHostPort, usID);

	if(pthread_attr_init(&hTCPClientReceiveThreadAttr[usID]))
	{
		printf("pthread_attr_init failure\n");
	}
	if(pthread_attr_setdetachstate(&hTCPClientReceiveThreadAttr[usID], PTHREAD_CREATE_DETACHED/*PTHREAD_CREATE_JOINABLE*/))
	{
		printf("pthread_attr_setdetachstate failure\n");
	}
	printf("TCP Thread Create\n");
	if(pthread_create(&hTCPClientReceiveThread[usID], &hTCPClientReceiveThreadAttr[usID], TCPClientReceiveThreadFunc, (void*) &g_TCPClientList[usID].args_tcp_client_rcv))
	{
		printf("Unable to create TCPClientReceiveThreadFunc thread\n");
		g_iTcpClientDisconnected[usID] = 1;
	}
	else
	{
		g_iTcpClientDisconnected[usID] = 0;
	}

#endif
}


void Elmo_TCP_Client_Send(unsigned short usID,  unsigned int uiSendDataLen, unsigned char * pBuffer)
{
#ifdef _PLATINUM
	if (usID >= MAX_TCP_CONN)
	{
		return;
	}

	client_TCP_s_t* pClientListSend = &g_TCPClientList[usID];

	//check if instace has been init the data length should not be zero
	if(pClientListSend->_dataMsg.uiLen == 0)
	{
		return;
	}


	// Check if we have somthing to Send ...
	if (uiSendDataLen)
	{	// Check Sizes
		if (uiSendDataLen > pClientListSend->_dataMsg.uiLen)
			uiSendDataLen = pClientListSend->_dataMsg.uiLen ;
		// COpy the Local SFUNC Buffer to the UDP Thread Buffer ...
		memcpy(pClientListSend->_dataMsg._szData, pBuffer,uiSendDataLen);
		// Set the Size to Send ...
		pClientListSend->_dataMsg.uiDataReady = uiSendDataLen ;
		// Wake up send thread
		sem_post(&SemTCPClient);
	}
#endif
	return ;
}

void Elmo_TCP_Client_Received(unsigned short usID,  int * piNumByteReceived, unsigned char * pBuffer)
{

#ifdef _PLATINUM

	// Save identifaction
	if (usID >= MAX_TCP_CONN)
	{
		return ;
	}

	client_TCP_s_t* pClientListReceive = &g_TCPClientList[usID];

	//check if the instace has been initialized. The data length should not be zero
	if(pClientListReceive->_dataMsg.uiLen == 0)
	{
		return;
	}

	if( pClientListReceive->_dataMsg.uiRcvDataReady & TCP_DATA_READY )
	{
		// Mark to the SFUNC that there is New Data available in the Buffer
		*piNumByteReceived = pClientListReceive->_dataMsg.iTcpRcvDataSize[0];

		// Copy the New Data from the correct TCP Thread Buffer to the SFUNC buffer ...

		if ((unsigned int)(*piNumByteReceived) > pClientListReceive->_dataMsg.uiLen)
		{
			*piNumByteReceived = pClientListReceive->_dataMsg.uiLen;
		}

//		memcpy(pClientListReceive->_dataMsg._szData, pClientListReceive->_dataMsg._szDataRcv, pClientListReceive->_dataMsg.iTcpRcvDataSize[0] );
		memcpy(pBuffer, pClientListReceive->_dataMsg._szDataRcv, pClientListReceive->_dataMsg.iTcpRcvDataSize[0] );

		//Clear data ready. No need for atomic_clear_bit() since we're running with the highest priority
		pClientListReceive->_dataMsg.uiRcvDataReady = 0;
	}
	else
	{
		*piNumByteReceived = 0 ;
	}

#endif
	return ;
}


///////////////////////////////////////////////////////////////////////////////
// 2018_08_07 Ronen
//
// Elmo Serial Interfaces
//

//
// Initialize Port for Recieve function.
void Elmo_Ser_Receive_Init(unsigned int ittyPortNum, unsigned int uiRXFixedPacketSize, int iMode, int iStopBits, int iParity, int iBaud)
{
#ifdef _PLATINUM
	int rc = 0;

	// Verify port # in range
	// ittyPortNum-- ;
	if (ittyPortNum >= MAX_SERIAL_CONN)
	{
		printf("*** %s() returned an error. Port %d Out of range ***\n",__func__, (ittyPortNum+1) );
		return ;
	}

	// Check if port not allready configured ...
	if ( iSerialPortConfigFlags[ittyPortNum] ) 
	{
		printf("*** %s() returned an error. Port %d allready in used ***\n",__func__, (ittyPortNum+1) );
		return ;
	}

	// protect the Fix Size MATLAB Buffer from Overflow
	if (uiRXFixedPacketSize > MAX_MATLAB_SERMSG_BUFFER_SIZE)  
	{
		uiRXFixedPacketSize = MAX_MATLAB_SERMSG_BUFFER_SIZE ;
		printf("%s() Packet size must be <=256\n",__func__);
	}

	// Verify Mode in range. Must be 0-1.
	// iMode--;
	if ( (iMode < 0) || (iMode > 1) ) iMode = CMaestroSerialPort::Normal ; 

	// Store the Initialized Size as a global to avoid the check on each runtime ...
	iSerialBufferSizeArray[ittyPortNum] = uiRXFixedPacketSize ;

	// build CFG and open the port
	CMaestroSerialPort::SMaestroSerialPortCfg cfg;
	cfg.blockingMode = 0;
	cfg.lineMode = 0;
	cfg.mode = (CMaestroSerialPort::Mode) iMode;
	cfg.parity = (CMaestroSerialPort::Parity) iParityLUT[iParity] ;			// iParity;
	cfg.rxFixedPacketSize = uiRXFixedPacketSize;
	cfg.speed = iBaud;
	cfg.stopBits = (CMaestroSerialPort::StopBits) iStopBitsLUT[iStopBits];	// iStopBits;
	cfg.timestampFormat = CMaestroSerialPort::TS_NONE;

	if( (rc = g_cSerialPort[ittyPortNum].Open(ittyPortNum,cfg)) < 0 )
	{
		//TODO: Handle fail to open serial communication
		iSerialPortConfigFlags[ittyPortNum] = SERIAL_PORT_INITIALIZED_ERR ;
		printf("*** %s() returned an error. Open Function Failed ***\n",__func__);
		return ;
	}

	// Mark Configuration OK
	iSerialPortConfigFlags[ittyPortNum] = SERIAL_PORT_INITIALIZED_FOR_RX ;
	printf("%s() Init Succcesfully. Port # %d\n",__func__,(ittyPortNum+1));

//	g_cSerialPort[ittyPortNum].Ioctl(TIOCFLUSH, TCIFLUSH);
	tcflush(g_cSerialPort[ittyPortNum].FdActiveGet(), TCIFLUSH);   // discards data received but not read

#endif
}
//
void Elmo_Ser_Receive(unsigned int ittyPortNum, int * iNumByteReceived, unsigned char * pBuffer)
{
#ifdef _PLATINUM
	// Verify port # in range - This should not happen, but just in case ... 
	// ittyPortNum-- ;
	if (ittyPortNum >= MAX_SERIAL_CONN) return ;

	// If Port was not configured specifically for READ, and config was OK, return without read
	if (iSerialPortConfigFlags[ittyPortNum] != SERIAL_PORT_INITIALIZED_FOR_RX)
		return ;

	// Read the data. Size allready initialized in the INIT_SEND Function.
	(*iNumByteReceived) = g_cSerialPort[ittyPortNum].Read(pBuffer,iSerialBufferSizeArray[ittyPortNum]) ;
	/*
	if( ((*iNumByteReceived) = g_cSerialPort[ittyPortNum].Read(pBuffer,iSerialBufferSizeArray[ittyPortNum])) > 0 )
		(*iNewDataReady) = 1;
	else
		(*iNewDataReady) = 0;
	*/

	/* PRINT FOR DEBUG ONLY !!
	if ( (*iNumByteReceived) )
		printf("%d # Character Recived. Data is : '%c' \n",*iNumByteReceived,pBuffer[0]);
	*/

#endif
}

// Initialize Port for SEND function. 
void Elmo_Ser_Send_Init(unsigned int ittyPortNum, int iMode, int iStopBits, int iParity, int iBaud)
{
#ifdef _PLATINUM
	int rc = 0;
	
	// Verify port # in range
	// ittyPortNum-- ;
	if (ittyPortNum >= MAX_SERIAL_CONN)
	{
		printf("*** %s() returned an error. Port %d Out of range ***\n",__func__, (ittyPortNum+1) );
		return ;
	}

	// Check if port not allready configured ...
	if ( iSerialPortConfigFlags[ittyPortNum] ) 
	{
		printf("*** %s() returned an error. Port %d allready in used ***\n",__func__, (ittyPortNum+1) );
		return ;
	}

	// Verify Mode in range. Must be 0-1.
	// iMode--;
	if ( (iMode < 0) || (iMode > 1) ) iMode = CMaestroSerialPort::Normal ; 

	// Store the Initialized Size as a global to avoid the check on each runtime ...
	iSerialBufferSizeArray[ittyPortNum] = 0 ;	// Indicate that Port was opened for Send

	// build CFG and open the port
	CMaestroSerialPort::SMaestroSerialPortCfg cfg;
	cfg.blockingMode = 0;
	cfg.lineMode = 0;
	cfg.mode = (CMaestroSerialPort::Mode) iMode;			
	cfg.parity = (CMaestroSerialPort::Parity) iParityLUT[iParity] ;			// iParity;;
	cfg.rxFixedPacketSize = 0;												// Size = 0 for INIT Send Function
	cfg.speed = iBaud;
	cfg.stopBits = (CMaestroSerialPort::StopBits) iStopBitsLUT[iStopBits];	// iStopBits;
	cfg.timestampFormat = CMaestroSerialPort::TS_NONE;

	if( (rc = g_cSerialPort[ittyPortNum].Open(ittyPortNum,cfg)) < 0 )
	{
		//TODO: Handle fail to open serial communication
		iSerialPortConfigFlags[ittyPortNum] = SERIAL_PORT_INITIALIZED_ERR ;
		printf("*** %s() returned an error. Open Function Failed ***\n",__func__);
		return ;
	}

	// Mark Configuration OK
	iSerialPortConfigFlags[ittyPortNum] = SERIAL_PORT_INITIALIZED_FOR_TX ;
	printf("%s() Init Succcesfully. Port # %d\n",__func__,(ittyPortNum+1));

#endif
}
//
void Elmo_Ser_Send(unsigned int ittyPortNum, unsigned int uiSendDataLen, unsigned char * pBuffer)
{
#ifdef _PLATINUM
	// Verify port # in range - This should not happen, but just in case ... 
	// ittyPortNum-- ;
	if (ittyPortNum >= MAX_SERIAL_CONN) return ;

	// If Port was not configured specifically for SEND, and config was OK, return without read
	if (iSerialPortConfigFlags[ittyPortNum] != SERIAL_PORT_INITIALIZED_FOR_TX)
		return ;

	// protect the Fix Size MATLAB Buffer from Overflow
	if (uiSendDataLen > MAX_MATLAB_SERMSG_BUFFER_SIZE) uiSendDataLen = MAX_MATLAB_SERMSG_BUFFER_SIZE ;

	// Send the data, size to send is stored in Global , allready initialized in the INIT_SEND Function.
	if (uiSendDataLen)
		g_cSerialPort[ittyPortNum].Write(pBuffer,uiSendDataLen);

	/* PRINT FOR DEBUG ONLY !!
	if (uiSendDataLen)
		printf("%d # Character Sent. Data is : '%c' \n",uiSendDataLen,pBuffer[0]  );
	*/
#endif
}


///////////////////////////////////////////////////////////////////////////////
// 2018_08_23 , RONEN 
// Add Global Configure Function for both RS422 and 232 Rx/Tx
//
void Elmo_Ser_Port_Config(unsigned int ittyPortNum, unsigned int uiRXFixedPacketSize, int iMode, int iStopBits, int iParity, int iBaud)
{
#ifdef _PLATINUM
	int rc = 0;

	// Verify port # in range
	// ittyPortNum-- ;
	if (ittyPortNum >= MAX_SERIAL_CONN)
	{
		printf("*** %s() returned an error. Port %d Out of range ***\n",__func__, (ittyPortNum+1) );
		return ;
	}

	// Check if port not allready configured ...
	if ( iSerialPortConfigFlags[ittyPortNum] ) 
	{
		printf("*** %s() returned an error. Port %d allready in used ***\n",__func__, (ittyPortNum+1) );
		return ;
	}

	// protect the Fix Size MATLAB Buffer from Overflow
	if (uiRXFixedPacketSize > MAX_MATLAB_SERMSG_BUFFER_SIZE)  
	{
		uiRXFixedPacketSize = MAX_MATLAB_SERMSG_BUFFER_SIZE ;
		printf("%s() Packet size must be <=256\n",__func__);
	}

	// Verify Mode in range. Must be 0-1.
	// iMode--;
	if ( (iMode < 0) || (iMode > 1) ) iMode = CMaestroSerialPort::Normal ; 

	// Store the Initialized Size as a global to avoid the check on each runtime ...
	iSerialBufferSizeArray[ittyPortNum] = uiRXFixedPacketSize ;

	// build CFG and open the port
	CMaestroSerialPort::SMaestroSerialPortCfg cfg;
	cfg.blockingMode = 0;
	cfg.lineMode = 0;
	cfg.mode = (CMaestroSerialPort::Mode) iMode;
	cfg.parity = (CMaestroSerialPort::Parity) iParityLUT[iParity] ;			// iParity;
	cfg.rxFixedPacketSize = uiRXFixedPacketSize;
	cfg.speed = iBaud;
	cfg.stopBits = (CMaestroSerialPort::StopBits) iStopBitsLUT[iStopBits];	// iStopBits;
	cfg.timestampFormat = CMaestroSerialPort::TS_NONE;

	//printf("%s() Before g_cSerialPort[%d].Open()\n",__func__, ittyPortNum);
	if( (rc = g_cSerialPort[ittyPortNum].Open(ittyPortNum,cfg)) < 0 )
	{
		//TODO: Handle fail to open serial communication
		iSerialPortConfigFlags[ittyPortNum] = SERIAL_PORT_INITIALIZED_ERR ;
		printf("*** %s() returned an error. Open Function Failed ***\n",__func__);
		return ;
	}

	// Mark Configuration OK
	iSerialPortConfigFlags[ittyPortNum] = SERIAL_PORT_INITIALIZED_FOR_RXTX ;
	printf("%s() Init Succcesfully. Port # %d\n",__func__,(ittyPortNum+1));

#endif
}

void Elmo_Ser_Port_Receive	(unsigned int ittyPortNum, int * iNumByteReceived, unsigned char * pBuffer)
{
	#ifdef _PLATINUM
	// static int iAlreadyHere = 0 ;
	// Verify port # in range - This should not happen, but just in case ... 
	// ittyPortNum-- ;
	if (ittyPortNum >= MAX_SERIAL_CONN) return ;

	// If Port was not configured specifically for READ, and config was OK, return without read
	if (iSerialPortConfigFlags[ittyPortNum] != SERIAL_PORT_INITIALIZED_FOR_RXTX)
	{		
		return ;
	}

	// Read the data. Size allready initialized in the INIT_SEND Function.
	(*iNumByteReceived) = g_cSerialPort[ittyPortNum].Read(pBuffer,iSerialBufferSizeArray[ittyPortNum]) ;
	/*
	if ( (*iNumByteReceived) )
		printf("%d # Character Recived. Data is : '%c' \n",*iNumByteReceived,pBuffer[0]);
	if (!iAlreadyHere)
	{
		iAlreadyHere = 1 ;
		printf("Serial RX, Port: %d \n",ittyPortNum+1);
	}
	*/
#endif
}

void Elmo_Ser_Port_Send		(unsigned int ittyPortNum, unsigned int uiSendDataLen, unsigned char * pBuffer)
{
	#ifdef _PLATINUM
	// static int iAlreadyHereS = 0 ;
	// Verify port # in range - This should not happen, but just in case ... 
	// ittyPortNum-- ;
	if (ittyPortNum >= MAX_SERIAL_CONN) return ;

	// If Port was not configured specifically for SEND, and config was OK, return without read
	if (iSerialPortConfigFlags[ittyPortNum] != SERIAL_PORT_INITIALIZED_FOR_RXTX)
	{
		return ;
	}

	// protect the Fix Size MATLAB Buffer from Overflow
	if (uiSendDataLen > MAX_MATLAB_SERMSG_BUFFER_SIZE) uiSendDataLen = MAX_MATLAB_SERMSG_BUFFER_SIZE ;

	// Send the data, size to send is stored in Global , allready initialized in the INIT_SEND Function.
	if (uiSendDataLen)
		g_cSerialPort[ittyPortNum].Write(pBuffer,uiSendDataLen);

	/* PRINT FOR DEBUG ONLY !!
	if (uiSendDataLen)
		printf("%d # Character Sent. Data is : '%c' \n",uiSendDataLen,pBuffer[0]  )
	if (!iAlreadyHereS)
	{
		iAlreadyHereS = 1 ;
		printf("Serial Tx, Port: %d \n",ittyPortNum+1);
	}
	*/
#endif
}

///////////////////////////////////////////////////////////////////////////////
// 2018_08_18 Ronen & Ziv
//
// Elmo SIL Time Measurement Functions
//
void Elmo_Time_Measurement_Init()
{
#ifdef _PLATINUM
	g_ucAllowTimeMeasurement = 1;
#endif
	return;
}

void Elmo_Time_Measurement_Read(unsigned int *ulElapsedTime, unsigned int *ulElapsedTimeMax, unsigned int *ulOverFlow)
{
#ifdef _PLATINUM
	(*ulElapsedTime)	= g_ulElapsedTime;
	(*ulElapsedTimeMax)	= g_ulElapsedTimeMax;
	(*ulOverFlow)		= g_ulCycleOverFlow;
#endif
	return;
}


///////////////////////////////////////////////////////////////////////////////
// 2018_08_27 Ronen & Ziv
//
// Elmo Maestro Input Time Capture Functions
//
void Elmo_Maestro_Input_TimeCapture_Read(unsigned short usIndex, unsigned int* uiEventCounter, unsigned int* uiCaptureTime)
{
#ifdef _PLATINUM
      g_cGloblaRT.GetInputCapture(usIndex,uiEventCounter,uiCaptureTime);
#endif
      return;
}

//MAT file logger

void Elmo_Start_Rec()
{
#ifdef _PLATINUM
	g_cRecTrig = 1;
#endif
}

void Elmo_stop_Rec()
{
#ifdef _PLATINUM
	if( g_cRecTrig)
	{
		g_iLogMatFile = 1;// save log file
	}
	g_cRecTrig = 0;
#endif
	return;
}

void Elmo_Set_Rec(unsigned short usValue)
{

#ifdef _PLATINUM
	set_Rec(usValue);
//	if((!g_cSaveOnce) &&  (usValue))
//	{
//		g_cRecTrig = 1;
//	}
//	else
//	{
//		if( g_cRecTrig)
//		{
//			g_iLogMatFile = 1;// save log file
//			g_cSaveOnce = 1;
//		}
//		g_cRecTrig = 0;
//	}
#endif
}

void Elmo_Get_Rec_Status(int *iTriger)
{
#ifdef _PLATINUM
	(*iTriger) = g_cRecTrig ;
#endif
	return;
}

void Elmo_SetEvent(int index)	// Event Driven Steps
{
#ifdef _PLATINUM
	sem_post(&stepSem[iEventDrivenOffset + index]);
#endif
}



