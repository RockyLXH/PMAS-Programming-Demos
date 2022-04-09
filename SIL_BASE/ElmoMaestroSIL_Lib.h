/*
 * ElmoMaestroSIL_Lib.h
 *
 *  Created on: 07/03/2017
 *      Author: zivb
 */

//  Define for our SIL H
#ifndef ELMO_SIL_H_
#define ELMO_SIL_H_

// Define GLobal Sfunction Structures for Busses

// ------------------------------------------------

typedef struct {
       double		dPosition;
       double		dVelocity;
       double		dCurrentIq;
       double		dCurrentId;
	   unsigned int uiIsEnabled ;   
} AxisData ;

typedef struct {
       double		dPositionCmd;
       double		dVelocityCmd;
	   double		dTorqueCmd;
       unsigned int uiIsStandStill ; 
	   unsigned int uiIsEnabled ;   
} ProfilerData ;

typedef struct {
		unsigned int uiPLCStatus ; 
		unsigned int uiIsStatusReg ;
		unsigned int uiIsDisabled ;
		unsigned int uiIsEnabled ;
		unsigned int uiIsStandStill ;
		unsigned int uiIsInError ;
} PLCAxiStatus ;

typedef struct {
       double dAIn1;
       double dAIn2;
       double dAIn3;
       double dAIn4;
} MaestroAnalogInputs ;

typedef struct {
		long	lMotionMode;
		long	lPosition;
		double	dPosition;
		float	fVel;
		float	fAcc;
		float	fJrk;
} MotionData ;

typedef struct {
		int		uiHomingMethod;	
		double	dPosition;
		float	fVelHi;   //speed search for switch
		float	fVelLo;   //speed search for zero
		float	fAcc;
		float	fDistanceLimit;
		float	fDetectVelLimit;
		float	fTorqueLimit;
		unsigned int	uiTimeLimit;
		unsigned int	uiDetectTimeLimit;
		//	MC_BUFFERED_MODE_ENUM eBufferMode;
} HomeData ;

typedef struct {
		int iGyroX;
		int iGyroY;
		int iGyroZ;
		int accelerometerX;
		int accelerometerY;
		int accelerometerZ;
} IMUData ;
// Defines for MDS Only ...
#ifdef _PLATINUM

#include <MMC_Definitions.h>
#include <mmcpplib.h>
#define SIL_UDP
#include "ElmoMaestroSIL_UDP.h"
#include "ElmoMaestroSIL_TCP.h"

#define MAX_PI	50
#define SIL_DATA_LONG_NUM	32
#define SIL_DATA_DOUBLE_NUM	32

#define POWER_ON 1
#define POWER_OFF 2
#define HOME 3

#define INTERNAL_STATE_INIT 0
#define INTERNAL_STATE_RUN 1
//
#define MAX_MATLAB_UDPMSG_BUFFER_SIZE 256
#define MAX_MATLAB_SERMSG_BUFFER_SIZE 256
//
#define MAX_SERIAL_CONN		10
//
#define SERIAL_PORT_INITIALIZED_FOR_RX	 1
#define SERIAL_PORT_INITIALIZED_FOR_TX	 2
#define SERIAL_PORT_INITIALIZED_FOR_RXTX 3
#define SERIAL_PORT_INITIALIZED_ERR		-1

//Home index
#define MB_CMD_HOME_POS_IDX 				3
#define MB_CMD_HOME_METH_IDX 				4
#define MB_CMD_HOME_VEl_HI_IDX 				5
#define MB_CMD_HOME_VEL_LOW_IDX 			6
#define MB_CMD_HOME_ACC_IDX 				7
#define MB_CMD_HOME_DIST_LIM_IDX 			8
#define MB_CMD_HOME_DETECT_VEL_LIM_IDX 		9
#define MB_CMD_HOME_TORQ_LIM_IDX 			10
#define MB_CMD_HOME_TIME_LIM_IDX 			11
#define MB_CMD_HOME_DETECT_TIME_LIM_IDX 	12


typedef struct api_status
{
	unsigned int uiIsInSequence;
	unsigned int uiLastAPIComn;
	unsigned int uiLastError;
	unsigned int uiAxesInError;

} API_STATUS;

extern CMMCConnection 		g_cConn;
extern MMC_CONNECT_HNDL 	g_hConn;
extern CMMCRTSingleAxis* 	g_cRTSingleAxisArr;
extern unsigned int			g_uiNumOfAxes;
extern CMMCPIVarRT 			g_cPIVarRT[50];
extern CMMCHostComm			g_cHostComm;
extern MMCPPGlobalRT		g_cGloblaRT;
extern API_STATUS			g_stApiStat;
extern CMMCPPIMU			g_cIMU[10];
void   atomic_set_bits(unsigned int mask, unsigned int *addr);
extern CMaestroSerialPort	g_cSerialPort[MAX_SERIAL_CONN];


//Time Measurement global parameters
extern unsigned long		g_ulCycleCountPrev;
extern unsigned long		g_ulCycleOverFlow;
extern unsigned char 		g_ucAllowTimeMeasurement;

extern unsigned long		g_ulElapsedTime	    ;
extern unsigned long		g_ulElapsedTimeMax  ;
extern unsigned long		g_ulCycleCountElTime;
extern unsigned long 		g_ulSumElapsedTime 	;
extern unsigned long		g_ulAvgElapsedTime  ;

//////////////////////////////////////////////////////////////////////////////////////////////////
//IMU GYRO




//////////////////////////////////////////////////////////////////////////////////////////////////
#endif

typedef struct
{
      char id;
      char gyroX[3];
      char gyroY[3];
      char gyroZ[3];
      char gyroStatus;
      char accelerometerX[3];
      char accelerometerY[3];
      char accelerometerZ[3];
      char accelerometerStatus;
      char gyroTemperatureX[2];
      char gyroTemperatureY[2];
      char gyroTemperatureZ[2];
      char gyroTemperatureStatus;
      char accelerometerTemperatureX[2];
      char accelerometerTemperatureY[2];
      char accelerometerTemperatureZ[2];
      char accelerometerTemperatureStatus;
      unsigned char counter;
      char latency[2];
      char crc[4];
}STIM300Datagram;
#define GYRO_CONVERT(A1, A2, A3)  (((A1* 65536 + A2 * 256 + A3)<<8)>>8)
#define ACCELEROMETER_CONVERT(A1, A2, A3)  (((A1* 65536 + A2 * 256 + A3)<<8)>>8)

//
// Dummy S-Func Decleration for the INIT MOdel Callback ...
//
void ElmoSILInitModel(void) ;
void ElmoSILMultiStepInitModel(void);
void ElmoSILInitModelReference(void) ;

//
// Start S Functions Declerations ...
//
void Elmo_Init_607A(unsigned int uiAxis, unsigned int uiSrc);
void Elmo_Write_607A(unsigned int uiAxis, int iUser607Aval);
void Elmo_Read_607A(unsigned int uiAxis, int *iUser607Aval);

void Elmo_Init_60FF(unsigned int uiAxis, unsigned int uiSrc);
void Elmo_Write_60FF(unsigned int uiAxis, int iUser60FFval);
void Elmo_Terminate_60FF(unsigned int uiAxis);
void Elmo_Read_60FF(unsigned int uiAxis, int *iUser60FFval);

void Elmo_Init_60B1(unsigned int uiAxis, unsigned int uiSrc);
void Elmo_Write_60B1(unsigned int uiAxis, int iUser60B1val);
void Elmo_Terminate_60B1(unsigned int uiAxis);
void Elmo_Read_60B1(unsigned int uiAxis, int *iUser60B1val);

void Elmo_Init_60B2(unsigned int uiAxis, unsigned int uiSrc);
void Elmo_Write_60B2(unsigned int uiAxis,double iUser60B2val);
void Elmo_Terminate_60B2(unsigned int uiAxis);
void Elmo_Read_60B2(unsigned int uiAxis, double *dUser60B2val);

void Elmo_Write_6071(unsigned int uiAxis,double dUser6071val);
void Elmo_Init_6071(unsigned int uiAxis, unsigned int uiSrc);
void Elmo_Terminate_6071(unsigned int uiAxis);
void Elmo_Read_6071(unsigned int uiAxis, double *dUser6071val);

void Elmo_Write_MB1	(unsigned int uiAxis, int iUserMB1);
void Elmo_Read_MB1	(unsigned int uiAxis, int *iUserMB1val);
void Elmo_Write_MB2	(unsigned int uiAxis, int iUserMB2);
void Elmo_Read_MB2  (unsigned int uiAxis, int *iUserMB2val);

void Elmo_Write_SIL_DATA_Double	(int iIndex,double dVal);
void Elmo_Write_SIL_DATA_Long	(int  iIndex,long lVal);
void Elmo_Write_UserParam_Int32(unsigned int uiAxis, unsigned int uiPrmIndex, int    iPrmVal);
void Elmo_Read_UserParam_Int32 (unsigned int uiAxis, unsigned int uiPrmIndex, int   *iPrmVal);
void Elmo_Write_UserParam_Float(unsigned int uiAxis, unsigned int uiPrmIndex, float  fPrmVal);
void Elmo_Read_UserParam_Float (unsigned int uiAxis, unsigned int uiPrmIndex, float *fPrmVal);

void Elmo_Write_DS402_CW(unsigned int uiAxis, unsigned short  usUserCWval);
void Elmo_Read_DS402_CW	(unsigned int uiAxis, unsigned short *usUserCWval);
void Elmo_Read_DS402_SW	(unsigned int uiAxis, unsigned short *usVal);

void Elmo_Read_Actual_Position		(unsigned int uiAxis, double *dActPos);
void Elmo_Read_Actual_Velocity		(unsigned int uiAxis, double *dActVel);
void Elmo_Read_Act_current			(unsigned int uiAxis, double *dVal);
void Elmo_Read_Actual_PosVelCurrent	(unsigned int uiAxis, AxisData * pAxData);
//
void Elmo_Read_Actual_Position_INT	(unsigned int uiAxis, int *iActPos);
void Elmo_Read_Actual_Velocity_INT	(unsigned int uiAxis, int *iActVel);

void Elmo_Read_dpos					(unsigned int uiAxis, double *pDesiredPos);
void Elmo_Read_dvel					(unsigned int uiAxis, double *pDesiredVel);
void Elmo_Read_dtorque				(unsigned int uiAxis, double *pDesiredTorque);
void Elmo_Read_Profiler_PosVelStat	(unsigned int uiAxis, ProfilerData * pProfilerData);

void Elmo_Read_Analog_in(unsigned int uiAxis, short *sVel);
void Elmo_Read_Dig_in	(unsigned int uiAxis, unsigned int *uiVel);
void Elmo_Write_Dig_out	(unsigned int uiAxis, unsigned int uiVal);
void Elmo_Terminate_Dig_out(unsigned int uiAxis);

void Elmo_Read_PLC_StatusEx (unsigned int uiAxis, PLCAxiStatus * pPLCAxiStatus);
void Elmo_Read_PLC_Status	(unsigned int uiAxis, unsigned int	*uiVal);
void Elmo_Read_Cycle_Count	(unsigned int uiAxis, unsigned long *ulVal);
void Elmo_Is_Disabled		(unsigned int uiAxis, unsigned int	*uiVal);
void Elmo_Is_StandStill		(unsigned int uiAxis, unsigned int	*uiVal);
void Elmo_Read_Status_reg	(unsigned int uiAxis, unsigned int	*uiVal);

void Elmo_Read_CPLD_ANG_IN_CH1	(double *dVal);
void Elmo_Read_CPLD_ANG_IN_CH2	(double *dVal);
void Elmo_Read_CPLD_ANG_IN_CH3	(double *dVal);
void Elmo_Read_CPLD_ANG_IN_CH4	(double *dVal);
void Elmo_Read_CPLD_ANG_INPUTS	(MaestroAnalogInputs * pMaestroAnalogInputs);

void Elmo_Write_CPLD_ANG_OUT_CH1(double dVal);
void Elmo_Write_CPLD_ANG_OUT_CH2(double dVal);
void Elmo_Write_CPLD_ANG_OUT_CH3(double dVal);
void Elmo_Write_CPLD_ANG_OUT_CH4(double dVal);

void Elmo_Terminate_CPLD_ANG_OUT_CH1(void);
void Elmo_Terminate_CPLD_ANG_OUT_CH2(void);
void Elmo_Terminate_CPLD_ANG_OUT_CH3(void);
void Elmo_Terminate_CPLD_ANG_OUT_CH4(void);
void Elmo_Read_CPLD_EXTIO_IN1	(unsigned int  *ulVal);
void Elmo_Write_CPLD_EXTIO_OUT1	(unsigned long ulVal);
void Elmo_Terminate_CPLD_EXTIO_OUT1(void);
void Elmo_Write_CPLD_EXTIO_OUT2	(unsigned long ulVal);
void Elmo_Write_CPLD_EXTIO_OUT3	(unsigned long ulVal);

void Elmo_Read_CPLD_Quad1PosUU(double *dVal);
void Elmo_Read_CPLD_Quad2PosUU(double *dVal);
void Elmo_Read_CPLD_Quad1VelUU(double *dVal);
void Elmo_Read_CPLD_Quad2VelUU(double *dVal);

void Elmo_Read_CPLD_Abs1PosUU(double *dVal);
void Elmo_Read_CPLD_Abs2PosUU(double *dVal);
void Elmo_Read_CPLD_Abs1VelUU(double *dVal);
void Elmo_Read_CPLD_Abs2VelUU(double *dVal);

void Elmo_Read_CPLD_DIFF_IO(unsigned short *usVal);
void Elmo_Write_CPLD_DIFF_IO(unsigned short usVal);

//void Elmo_Read_GroupOfParameters(MMC_READGROUPOFPARAMETERS_IN* stIn, MMC_READGROUPOFPARAMETERS_OUT* stOut);
//void Elmo_Write_GroupOfParametersEX(MMC_AXIS_REF_HNDL usAxisRef, MMC_WRITEGROUPOFPARAMETERSEX_IN* stIn, MMC_WRITEGROUPOFPARAMETERSEX_OUT* stOut);

// PI Vars
void Elmo_Init_PI_Var(unsigned int uiAxis,unsigned short usIndex,unsigned short usPIVarOffset,unsigned int uiDirection);
//

void Elmo_Write_PI_UChar(unsigned short usIndex ,unsigned char ucVal);
// RONEN
// The Simulink need this as :
void Elmo_Write_PI_Char  (unsigned short usIndex , signed char scVal);
//Whereas the C++ MDS need without the sign ... need to resolve
//void Elmo_WRITE_PI_CHAR(unsigned short usIndex , char scVal);
void Elmo_Write_PI_UShort(unsigned short usIndex , unsigned short usVal);
void Elmo_Write_PI_Short (unsigned short usIndex , short sVal);
void Elmo_Write_PI_UInt	 (unsigned short usIndex , unsigned int uiVal);
void Elmo_Write_PI_Int	 (unsigned short usIndex , int iVal);
void Elmo_Write_PI_Float (unsigned short usIndex , float fVal);
void Elmo_Write_PI_Double(unsigned short usIndex , double dVal);
//

void Elmo_Read_PI_Char	(unsigned short usIndex , signed char *cVal);
void Elmo_Read_PI_UChar	(unsigned short usIndex ,unsigned char *ucVal);
void Elmo_Read_PI_UShort(unsigned short usIndex , unsigned short *usVal);
void Elmo_Read_PI_Short	(unsigned short usIndex  , short *sVal);
void Elmo_Read_PI_UInt	(unsigned short usIndex , unsigned int *uiVal);
void Elmo_Read_PI_Int	(unsigned short usIndex ,int *iVal);
void Elmo_Read_PI_Float	(unsigned short usIndex ,float *fVal);
void Elmo_Read_PI_Double(unsigned short usIndex  ,double *dVal);

//
// Axis State Machine Handling Functions
//
void AxisClearError	 (unsigned short uiAxis) ;
void AxisEnable		 (unsigned short uiAxis, short action) ;
void AxisHome		 (unsigned short uiAxis, HomeData *pHomeDataStruct) ;
void AxisChangeOpMode(unsigned short uiAxis, short opmod ) ;
void AxisMotion		 (unsigned short uiAxis, MotionData *pMotioDataStruct) ;
void IsDoneProcessing(unsigned short uiAxis, unsigned short * pusDone) ;

//
// STIM-300 IMU Data Access
//
void Stim300IMU_Init	 (unsigned short usIndex,unsigned char cPort_index,int iBaud,unsigned int iSize);
void Stim300IMU_ReadGyroX(unsigned short usIndex,int *iValue);
void Stim300IMU_ReadGyroY(unsigned short usIndex,int *iValue);
void Stim300IMU_ReadGyroZ(unsigned short usIndex,int *iValue);
void Stim300IMU_ReadAccX (unsigned short usIndex,int *iValue);
void Stim300IMU_ReadAccY (unsigned short usIndex,int *iValue);
void Stim300IMU_ReadAccZ (unsigned short usIndex,int *iValue);
void Stim300IMU_ReadData (unsigned short usIndex,IMUData *stData);

//
// UDP Communication S-Functions declerations
//
void Elmo_UDP_Receive_Init	(unsigned short usID, unsigned int uiDataLen, unsigned short usLocalPort);
void Elmo_UDP_Receive		(unsigned short usID, /*unsigned int uiDataLen,*/ int * iNumByteReceived, unsigned char * pBuffer);
//
// NO definition of Size PARAMETER. It is provided in run time by "uiSendDataLen", so teh user
// can determine how much data to send on each cycle ...
void Elmo_UDP_Send_Init		(unsigned short usID, unsigned int uiDataLen, unsigned char * cHostIP, unsigned short usHostPort);
void Elmo_UDP_Send			(unsigned short usID, /*unsigned int uiDataLen,*/ unsigned int uiSendDataLen, unsigned char * pBuffer);

// TCP Communication S-Functions declerations
void Elmo_TCP_Server_Init (unsigned short usID, unsigned int uiDataLen, unsigned short usLocalPort);
void Elmo_TCP_Server_Receive(unsigned short usID, int * iNumByteReceived, unsigned char * pBuffer);
void Elmo_TCP_Server_Send(unsigned short usID,  unsigned int uiSendDataLen, unsigned char * pBuffer);
void Elmo_TCP_Client_Init (unsigned short usID, unsigned int uiDataLen, unsigned char * cHostIP, unsigned short usHostPort);
void Elmo_TCP_Client_Send(unsigned short usID,  unsigned int uiSendDataLen, unsigned char * pBuffer);
void Elmo_TCP_Client_Received(unsigned short usID,  int * iNumByteReceived, unsigned char * pBuffer);
//
// Elmo Serial Interfaces S-Functions declerations 
//
void Elmo_Ser_Receive_Init	(unsigned int ittyPortNum, unsigned int uiRXFixedPacketSize, int iMode, int iStopBits, int iParity, int iBaud);
void Elmo_Ser_Receive		(unsigned int ittyPortNum, int * iNumByteReceived, unsigned char * pBuffer);
//
void Elmo_Ser_Send_Init     (unsigned int ittyPortNum, int iMode, int iStopBits, int iParity, int iBaud);
void Elmo_Ser_Send			(unsigned int ittyPortNum, unsigned int uiSendDataLen, unsigned char * pBuffer);

//
// Elmo Serial Port Configure / Send / Receive Functions
//
void Elmo_Ser_Port_Config	(unsigned int ittyPortNum, unsigned int uiRXFixedPacketSize, int iMode, int iStopBits, int iParity, int iBaud);
void Elmo_Ser_Port_Receive	(unsigned int ittyPortNum, int * iNumByteReceived, unsigned char * pBuffer);
void Elmo_Ser_Port_Send		(unsigned int ittyPortNum, unsigned int uiSendDataLen, unsigned char * pBuffer);



//
// This is an example for the INIT Parameters for Recive from STIM_300 IMU
//// Elmo_Serial_Init         (				  1,           0,         1,             0,           0,   1843200, STIM300_DATAGRAM_SIZE);
//// void Elmo_Serial_Init (unsigned short usID, int ittyNum, int iMode, int iStopBits, int iParity, int iBaud, unsigned int uiRXFixedPacketSize)
//
// uiRXFixedPacketSize : Size of the data sent by the external device, to be used by the READ function, ONly in Enhance MODE !
//						 in Normal Mode this paramter can be 0 !
//						 It is NON relevant for the SEND FUnction.
//

//Time Measurement S-Func
void Elmo_Time_Measurement_Init();
void Elmo_Time_Measurement_Read(unsigned int *ulElapsedTime, unsigned int *ulElapsedTimeMax, unsigned int *ulOverFlow);

// Ronen : 2018_08_27 Ronen & Ziv, Time Capture S-Funcs
void Elmo_Maestro_Input_TimeCapture_Read(unsigned short usIndex, unsigned int* uiEventCounter, unsigned int* uiCaptureTime) ;

//Recording S-Func
void Elmo_Start_Rec();
void Elmo_stop_Rec();
void Elmo_Set_Rec(unsigned short usValue);
void Elmo_Get_Rec_Status(int *iTriger);
void Elmo_SetEvent(int index);



//
// End Of H File 
//
#endif /* ELMO_SIL_H_ */





