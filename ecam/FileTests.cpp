/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Name:
Description:
Creation date:		8/03/2017
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "MMC_definitions.h"
#include "mmcpplib.h"
#include "header.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
functions prototypes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int ECAM_File_Test1();
int ECAM_File_Test2();
int ECAM_File_Test3();
int ECAM_File_Test4();
int ECAM_File_Test5();
int ECAM_File_Test11();



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Global declarations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function:				ECAM_File_Test1()
Description:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int ECAM_File_Test1()
{
	int rc=0;
	static bool bCamFlag[MAX_AXES];
	int pos[MAX_AXES];

	MC_CamRef stCamRef;
	MC_PATH_REF uiCamHandle;

	double dbMasterSyncPosition; 	//relative to first row in CAM table
	double dbMasterStartDistance;	//for now, until we have Ram-In implemented.
	unsigned int uiStartMode;
	double dbMasterOffset;
	double dbSlaveOffset;
	double dbMasterScaling;
	double dbSlaveScaling;

	unsigned char ucMasterPosAbs;
	unsigned char ucSlavePosAbs;

	/* **********************************
	Reset Tables
	********************************** */
	cAxis[2].UnloadCamTable(-1);


	/* **********************************
		Cam Table Select
	 ********************************** */
	stCamRef.dbGap = 10000;
	stCamRef.dbMasterStartPosition = 0;
	stCamRef.eCoordSystem = MC_NONE_COORD;
	stCamRef.eTableMode = eCAMT_ROMode;
	stCamRef.iCamTableID = -1 ;
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/Untitled01.T1.G.cam");

	ucMasterPosAbs = 1;
	ucSlavePosAbs = 1;
	uiStartMode = 1;
	uiCamHandle = cAxis[2].CamTableSelect( stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode	);



	/****************************************
		axis01  and  axis02 Cam IN
	****************************************/

	dbMasterSyncPosition = 0.0;		//relative to first row in CAM table
	dbMasterStartDistance = 0;		//for now, until we have Ram-In implemented.
	uiStartMode = 1;
	dbMasterOffset = 0;
	dbSlaveOffset = 0;
	dbMasterScaling = 1;
	dbSlaveScaling = 1;

	while (1)
	{

		try
		{
//			pos[1] = cAxis[3].GetActualPosition();

			/*
				Set  CAMIN for axis2 (1st slave)  when:
				CAMIN command has not been called  yet and
				axis Position is between 2000 to 5000 and
				axis is not in CAM mode yet
			*/
//			if ( bCamFlag[1] == false		&&
//				 (pos[1] % 10000) >= 2000.0 &&
//				 (pos[1] % 10000) < 5000.0  &&
//				 !(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK) )
//			{
//				bCamFlag[1]= true;
				rc = cAxis[2].CamIn(	cAxis[1].GetRef(),
										MC_BUFFERED_MODE,
										uiCamHandle,
										ePolynom5Interp,
										0,
										eCAM_PERIODIC,
										dbMasterSyncPosition,
										dbMasterStartDistance,
										uiStartMode,
										dbMasterOffset,
										dbSlaveOffset,
										dbMasterScaling,
										dbSlaveScaling,
										eECAM_SET_VALUE);

				cAxis[2].CamOut();
//				if (rc < 0)
//					return -1;

//			}
			/*

			*/
//			else
//			{
//				//perform CAMIN command only once and exit while loop
//				if (bCamFlag[1])
//					break;
//				// for endless while loop - reset CAM mmode flag
//				bCamFlag[1] = false;
//			}

		}
		catch (CMMCException exp)
		{
			cout << "enableAxis exception error" << exp.error() << "\t" << exp.what() << endl;
			return -1;
		}
	}

	return 0;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function:				ECAM_File_Test2()
Description :
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int ECAM_File_Test2()
{
	int rc = 0;
	static bool bCamFlag[MAX_AXES];
	int pos[MAX_AXES];

	MC_CamRef stCamRef;
	MC_PATH_REF uiCamHandle;

	double dbMasterSyncPosition; 	//relative to first row in CAM table
	double dbMasterStartDistance;	//for now, until we have Ram-In implemented.
	unsigned int uiStartMode;
	double dbMasterOffset;
	double dbSlaveOffset;
	double dbMasterScaling;
	double dbSlaveScaling;

	unsigned char ucMasterPosAbs;
	unsigned char ucSlavePosAbs;

	/* **********************************
	Reset Tables
	********************************** */
	cAxis[1].UnloadCamTable(-1);


	/* **********************************
	Cam Table Select
	********************************** */
	stCamRef.dbGap = 1000;
	stCamRef.dbMasterStartPosition = 0;
	stCamRef.eCoordSystem = MC_NONE_COORD;
	stCamRef.eTableMode = eCAMT_ROMode;
	stCamRef.iCamTableID = -1;
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle = cAxis[1].CamTableSelect(	stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode	);



	/****************************************
	axis01  and  axis02 Cam IN
	****************************************/

	dbMasterSyncPosition = 0.0;		//relative to first row in CAM table
	dbMasterStartDistance = 0;		//for now, until we have Ram-In implemented.
	uiStartMode = 1;
	dbMasterOffset = 0;
	dbSlaveOffset = 0;
	dbMasterScaling = 1;
	dbSlaveScaling = 1;

	while (1)
	{

		try
		{
			// Move axis1 to offset 0 when Master Position is over 5000


			pos[1] = cAxis[3].GetActualPosition();
			if (pos[1] % 10000 > 5000 && !(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[1].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}


			/*
			Set  CAMIN for axis2 (1st slave)  when:
			CAMIN command has not been called  yet and
			axis Position is between 2000 to 5000 and
			axis is not in CAM mode yet
			*/
			if (bCamFlag[1] == false &&
				(pos[1] % 10000) >= 2000.0 &&
				(pos[1] % 10000) < 5000.0 &&
				!(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				bCamFlag[1] = true;
				rc = cAxis[1].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
				reset CAM mode flag and avoid multiple calls to
				CAMIN command when positin less than 5000
				*/
				if (pos[1] % 10000> 5000)
					bCamFlag[1] = false;
			}


		}
		catch (CMMCException exp)
		{
			cout << "enableAxis exception error" << exp.error() << "\t" << exp.what() << endl;
			return -1;
		}
	}

	return 0;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function:				ECAM_File_Test3()
Description :
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int ECAM_File_Test3()
{
	int rc = 0;
	static bool bCamFlag[MAX_AXES];
	int pos[MAX_AXES];

	MC_CamRef stCamRef;
	MC_PATH_REF uiCamHandle;

	double dbMasterSyncPosition; 	//relative to first row in CAM table
	double dbMasterStartDistance;	//for now, until we have Ram-In implemented.
	unsigned int uiStartMode;
	double dbMasterOffset;
	double dbSlaveOffset;
	double dbMasterScaling;
	double dbSlaveScaling;

	unsigned char ucMasterPosAbs;
	unsigned char ucSlavePosAbs;

	/* **********************************
	Reset Tables
	********************************** */
	cAxis[1].UnloadCamTable(-1);


	/* **********************************
	Cam Table Select
	********************************** */
	stCamRef.dbGap = 1000;
	stCamRef.dbMasterStartPosition = 0;
	stCamRef.eCoordSystem = MC_NONE_COORD;
	stCamRef.eTableMode = eCAMT_ROMode;
	stCamRef.iCamTableID = -1;
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle = cAxis[1].CamTableSelect(	stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode);



	/****************************************
	axis1  and  axis2 Cam IN
	****************************************/

	dbMasterSyncPosition = 0.0;		//relative to first row in CAM table
	dbMasterStartDistance = 0;		//for now, until we have Ram-In implemented.
	uiStartMode = 1;
	dbMasterOffset = 0;
	dbSlaveOffset = 0;
	dbMasterScaling = 1;
	dbSlaveScaling = 1;

	while (1)
	{

		try
		{

			// Move axis2 to offset 0 when Master Position is over 5000
			pos[1] = cAxis[3].GetActualPosition();
			if (pos[1] % 10000 > 5000 && !(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[1].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}

			/*
			Set CAMIN for axis2 (1st salve) and axis1 (3rd salve):
			CAMIN command has not been called  yet and
			axis Position is between 2000 to 5000 and
			axis is not in CAM mode yet
			*/

			if (bCamFlag[1] == false &&
				(pos[1] % 10000) >= 2000.0 &&
				(pos[1] % 10000) < 5000.0 &&
				!(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				bCamFlag[1] = true;

				//Axis2 CAM table1
				rc = cAxis[1].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);

				if (rc < 0)
					return -1;

				//Axis1 CAM table1
				rc = cAxis[0].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
					Reset axis2 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 5000
				*/
				if (pos[1] % 10000> 5000)
					bCamFlag[1] = false;
			}



			// Move axis3 to offset 0 when Master Position is over 9000
			if (pos[1] % 10000 > 9000 && !(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[2].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}

			/*
			Set CAMIN for axis3 (2nd salve) and axis1 (3rd salve):
			CAMIN command has not been called  yet and
			axis Position is between 2000 to 5000 and
			axis is not in CAM mode yet
			*/

			if ( bCamFlag[2] == false &&
				(pos[1] % 10000) >= 6000.0 &&
				(pos[1] % 10000) < 9000.0  &&
				!(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK) )
			{
				bCamFlag[2] = true;

				//Axis3 CAM table1
				rc = cAxis[2].CamIn(	cAxis[3].GetRef(),
										MC_BUFFERED_MODE,
										uiCamHandle,
										ePolynom5Interp,
										0,
										eCAM_NON_PERIODIC,
										dbMasterSyncPosition,
										dbMasterStartDistance,
										uiStartMode,
										dbMasterOffset,
										dbSlaveOffset,
										dbMasterScaling,
										dbSlaveScaling,
										eECAM_SET_VALUE		);

				if (rc < 0)
					return -1;

				//Axis1 CAM table1
				rc = cAxis[0].CamIn(	cAxis[3].GetRef(),
										MC_BUFFERED_MODE,
										uiCamHandle,
										ePolynom5Interp,
										0,
										eCAM_NON_PERIODIC,
										dbMasterSyncPosition,
										dbMasterStartDistance,
										uiStartMode,
										dbMasterOffset,
										dbSlaveOffset,
										dbMasterScaling,
										dbSlaveScaling,
										eECAM_SET_VALUE		);
			}
			else
			{
				/*
					Reset axis3 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 9000
				*/
				if (pos[1] % 10000> 9000)
					bCamFlag[2] = false;
			}
		}
		catch (CMMCException exp)
		{
			cout << "enableAxis exception error" << exp.error() << "\t" << exp.what() << endl;
			return -1;
		}
	}

	return 0;
}




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function:				ECAM_File_Test4()
Description :
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int ECAM_File_Test4()
{
	int rc = 0;
	static bool bCamFlag[MAX_AXES];
	int pos[MAX_AXES];

	MC_CamRef stCamRef;
	MC_PATH_REF uiCamHandle1, uiCamHandle2 ;

	double dbMasterSyncPosition; 	//relative to first row in CAM table
	double dbMasterStartDistance;	//for now, until we have Ram-In implemented.
	unsigned int uiStartMode;
	double dbMasterOffset;
	double dbSlaveOffset;
	double dbMasterScaling;
	double dbSlaveScaling;

	unsigned char ucMasterPosAbs;
	unsigned char ucSlavePosAbs;

	/* **********************************
	Reset Tables
	********************************** */
	cAxis[1].UnloadCamTable(-1);


	/* **********************************
	Cam Table 1 Select
	********************************** */
	stCamRef.dbGap = 1000;
	stCamRef.dbMasterStartPosition = 0;
	stCamRef.eCoordSystem = MC_NONE_COORD;
	stCamRef.eTableMode = eCAMT_ROMode;
	stCamRef.iCamTableID = -1;
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle1 = cAxis[1].CamTableSelect(	stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode);


	/* **********************************
	Cam Table 2  Select
	********************************** */
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash01.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle2 = cAxis[1].CamTableSelect(	stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode);

	/****************************************
	axis01  and  axis02 Cam IN
	****************************************/

	dbMasterSyncPosition = 0.0;		//relative to first row in CAM table
	dbMasterStartDistance = 0;		//for now, until we have Ram-In implemented.
	uiStartMode = 1;
	dbMasterOffset = 0;
	dbSlaveOffset = 0;
	dbMasterScaling = 1;
	dbSlaveScaling = 1;

	while (1)
	{
		try
		{
			// Move axis2 to offset 0 when Master Position is over 5000

			pos[1] = cAxis[3].GetActualPosition();
			if (pos[1] % 10000 > 5000 && !(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[1].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}

			/*
			Set CAMIN for axis2 (1st salve) and axis1 (3rd salve):
			CAMIN command has not been called  yet and
			axis Position is between 2000 to 5000 and
			axis is not in CAM mode yet
			*/

			if (bCamFlag[1] == false &&
				(pos[1] % 10000) >= 2000.0 &&
				(pos[1] % 10000) < 5000.0 &&
				!(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				bCamFlag[1] = true;

				//Axis2 CAM table1
				rc = cAxis[1].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle1,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);

				if (rc < 0)
					return -1;

				//Axis1 CAM table2
				rc = cAxis[0].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle2,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
					Reset axis2 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 5000
				*/
				if (pos[1] % 10000> 5000)
					bCamFlag[1] = false;
			}


			// Move axis3 to offset 0 when Master Position is over 9000
			if (pos[1] % 10000 > 9000 && !(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[2].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}


			/*
			Set CAMIN for axis3 (2nd salve) and axis1 (3rd salve):
			CAMIN command has not been called  yet and
			axis Position is between 2000 to 5000 and
			axis is not in CAM mode yet
			*/
			if (bCamFlag[2] == false &&
				(pos[1] % 10000) >= 6000.0 &&
				(pos[1] % 10000) < 9000.0 &&
				!(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				bCamFlag[2] = true;

				//Axis3 CAM table1
				rc = cAxis[2].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle1,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);

				if (rc < 0)
					return -1;

				//Axis1 CAM table2
				rc = cAxis[0].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle2,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
					Reset axis3 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 9000
				*/
				if (pos[1] % 10000> 9000)
					bCamFlag[2] = false;
			}
		}
		catch (CMMCException exp)
		{
			cout << "enableAxis exception error" << exp.error() << "\t" << exp.what() << endl;
			return -1;
		}
	}

	return 0;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function:				ECAM_File_Test5()
Description :
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int ECAM_File_Test5()
{
	int rc = 0;
	static bool bCamFlag[MAX_AXES];
	int pos;

	MC_CamRef stCamRef;
	MC_PATH_REF uiCamHandle1, uiCamHandle2;

	double dbMasterSyncPosition; 	//relative to first row in CAM table
	double dbMasterStartDistance;	//for now, until we have Ram-In implemented.
	unsigned int uiStartMode;
	double dbMasterOffset;
	double dbSlaveOffset;
	double dbMasterScaling;
	double dbSlaveScaling;

	unsigned char ucMasterPosAbs;
	unsigned char ucSlavePosAbs;

	/* **********************************
	Reset Tables
	********************************** */
	cAxis[1].UnloadCamTable(-1);


	/* **********************************
	Cam Table 1 Select
	********************************** */
	stCamRef.dbGap = 1000;
	stCamRef.dbMasterStartPosition = 0;
	stCamRef.eCoordSystem = MC_NONE_COORD;
	stCamRef.eTableMode = eCAMT_ROMode;
	stCamRef.iCamTableID = -1;
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle1 = cAxis[1].CamTableSelect(stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode);


	/* **********************************
	Cam Table 2  Select
	********************************** */
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash01.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle2 = cAxis[1].CamTableSelect(stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode);

	/****************************************
	axis01  and  axis02 Cam IN
	****************************************/

	dbMasterSyncPosition = 0.0;	//relative to first row in CAM table
	dbMasterStartDistance = 0;		//for now, until we have Ram-In implemented.
	uiStartMode = 1;
	dbMasterOffset = 0;
	dbSlaveOffset = 0;
	dbMasterScaling = 1;
	dbSlaveScaling = 1;

	while (1)
	{

		try
		{
			/*
				Move axis2 to offset 0 when Master Position is over 5000
			*/
			pos = cAxis[3].GetActualPosition();
			if (pos % 10000 > 5000 && !(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[1].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}

			/*
				Set CAMIN for axis2 (1st salve) and axis1 (3rd salve):
				CAMIN command has not been called  yet and
				axis Position is between 2000 to 5000 and
				axis is not in CAM mode yet
			*/

			if (bCamFlag[1] == false &&
				(pos % 10000) >= 2000.0 &&
				(pos % 10000) < 5000.0 &&
				!(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				/*
					Axis2 CAM table1
				*/
				bCamFlag[1] = true;
				rc = cAxis[1].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle1,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);

				if (rc < 0)
					return -1;

				/*
					Axis1 CAM table2
				*/
				rc = cAxis[0].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle2,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
					Reset axis2 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 5000
				*/
				if (pos% 10000> 5000)
					bCamFlag[1] = false;
			}


			/*
				Move axis3 to offset 0 when Master Position is over 9000
			*/
			if (pos % 10000 > 9000 && !(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[2].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}


			/*
				Set CAMIN for axis3 (2nd salve) and axis1 (3rd salve):
				CAMIN command has not been called  yet and
				axis Position is between 2000 to 5000 and
				axis is not in CAM mode yet
			*/
			if (bCamFlag[2] == false &&
				(pos % 10000) >= 6000.0 &&
				(pos % 10000) < 9000.0 &&
				!(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				bCamFlag[2] = true;

				//Axis3 CAM table1
				rc = cAxis[2].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle1,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);

				if (rc < 0)
					return -1;

				//Axis1 CAM table2
				rc = cAxis[0].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle2,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
					Reset axis3 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 9000
				*/
				if (pos % 10000> 9000)
					bCamFlag[2] = false;
			}
		}
		catch (CMMCException exp)
		{
			cout << "enableAxis exception error" << exp.error() << "\t" << exp.what() << endl;
			return -1;
		}
	}

	return 0;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function:				ECAM_File_Test11()
Description :
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int ECAM_File_Test11()
{
	int rc = 0;
	static bool bCamFlag[MAX_AXES];
	int pos;

	MC_CamRef stCamRef;
	MC_PATH_REF uiCamHandle1, uiCamHandle2;

	double dbMasterSyncPosition; 	//relative to first row in CAM table
	double dbMasterStartDistance;	//for now, until we have Ram-In implemented.
	unsigned int uiStartMode;
	double dbMasterOffset;
	double dbSlaveOffset;
	double dbMasterScaling;
	double dbSlaveScaling;

	unsigned char ucMasterPosAbs;
	unsigned char ucSlavePosAbs;

	/* **********************************
	Reset Tables
	********************************** */
	cAxis[1].UnloadCamTable(-1);


	/* **********************************
	Cam Table 1 Select
	********************************** */
	stCamRef.dbGap = 1000;
	stCamRef.dbMasterStartPosition = 0;
	stCamRef.eCoordSystem = MC_NONE_COORD;
	stCamRef.eTableMode = eCAMT_ROMode;
	stCamRef.iCamTableID = -1;
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle1 = cAxis[1].CamTableSelect(stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode);


	/* **********************************
	Cam Table 2  Select
	********************************** */
	strcpy(stCamRef.pPathToTableFile, "/mnt/jffs/usr/EcamTableCrash01.cam");

	ucMasterPosAbs = 0;
	ucSlavePosAbs = 0;
	uiStartMode = 1;
	uiCamHandle2 = cAxis[1].CamTableSelect(stCamRef,
											ucMasterPosAbs,
											ucSlavePosAbs,
											uiStartMode);

	/****************************************
	axis01  and  axis02 Cam IN
	****************************************/

	dbMasterSyncPosition = 0.0;	//relative to first row in CAM table
	dbMasterStartDistance = 0;		//for now, until we have Ram-In implemented.
	uiStartMode = 1;
	dbMasterOffset = 0;
	dbSlaveOffset = 0;
	dbMasterScaling = 1;
	dbSlaveScaling = 1;

	while (1)
	{

		try
		{
			/*
				Move axis2 to offset 0 when Master Position is over 5000
			*/
			pos = cAxis[3].GetActualPosition();
			if (pos % 10000 > 5000 && !(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[1].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}

			/*
				Set CAMIN for axis2 (1st salve) and axis1 (3rd salve):
				CAMIN command has not been called  yet and
				axis Position is between 2000 to 5000 and
				axis is not in CAM mode yet
			*/

			if (bCamFlag[1] == false &&
				(pos % 10000) >= 2000.0 &&
				(pos % 10000) < 5000.0 &&
				!(cAxis[1].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				/*
					Axis2 CAM table1
				*/
				bCamFlag[1] = true;
				rc = cAxis[1].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle1,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);

				if (rc < 0)
					return -1;

				/*
					Axis1 CAM table2
				*/
				rc = cAxis[0].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle2,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
					Reset axis2 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 5000
				*/
				if (pos% 10000> 5000)
					bCamFlag[1] = false;
			}


			/*
				Move axis3 to offset 0 when Master Position is over 9000
			*/
			if (pos % 10000 > 9000 && !(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				cAxis[2].MoveAbsolute(0, 1e+05, 1e+06, 1e+06, 1e+07, MC_ABORTING_MODE);
			}


			/*
				Set CAMIN for axis3 (2nd salve) and axis1 (3rd salve):
				CAMIN command has not been called  yet and
				axis Position is between 2000 to 5000 and
				axis is not in CAM mode yet
			*/
			if (bCamFlag[2] == false &&
				(pos % 10000) >= 6000.0 &&
				(pos % 10000) < 9000.0 &&
				!(cAxis[2].ReadStatus() & NC_AXIS_SYNCHRONIZED_MOTION_MASK))
			{
				bCamFlag[2] = true;

				//Axis3 CAM table1
				rc = cAxis[2].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle1,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);

				if (rc < 0)
					return -1;

				//Axis1 CAM table2
				rc = cAxis[0].CamIn(cAxis[3].GetRef(),
									MC_BUFFERED_MODE,
									uiCamHandle2,
									ePolynom5Interp,
									0,
									eCAM_NON_PERIODIC,
									dbMasterSyncPosition,
									dbMasterStartDistance,
									uiStartMode,
									dbMasterOffset,
									dbSlaveOffset,
									dbMasterScaling,
									dbSlaveScaling,
									eECAM_SET_VALUE);
			}
			else
			{
				/*
					Reset axis3 CAM mode flag and avoid multiple calls to
					CAMIN command when master positin less than 9000
				*/
				if (pos % 10000> 9000)
					bCamFlag[2] = false;
			}
		}
		catch (CMMCException exp)
		{
			cout << "enableAxis exception error" << exp.error() << "\t" << exp.what() << endl;
			return -1;
		}
	}

	return 0;
}
