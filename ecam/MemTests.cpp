/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Name:
Description:
Creation date:		8/03/2017
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define __EXTERN__ 1

#include "mmcpplib.h"
#include "header.h"

double data[]= {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
functions prototypes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int ECAM_MEM_Test1();
int ECAM_MEM_Test2();



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Global declarations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int ECAM_MEM_Test1()
{

	int rc;
	static int cnt = 0;

	MMC_CAMSTATUS_IN stCAMStatusIn;
	MMC_CAMSTATUS_OUT stCAMStatusOut;

	MMC_CAMTABLEINIT_IN stCAMInitIn;
	MMC_INITTABLE_OUT 	stCAMInitOut;


	MMC_CAMTABLESET_IN 		stCAMAddIn;
	MMC_CAMTABLESET_OUT 	stCAMAddOut;

	MMC_CAMTABLESELECT_IN 	stCAMSelectIn;
	MMC_CAMTABLESELECT_OUT 	stCAMSelectOut;

	MMC_CAMIN_IN 		stCAMINIn;
	MMC_CAMIN_OUT 		stCAMINOut;

	MMC_UNLOADTABLE_IN 	stUnloadIn;
	MMC_UNLOADTABLE_OUT stUnloadOut;

	MMC_CAMOUT_IN 		stCAMOUTIn;
	MMC_CAMOUT_OUT 		stCAMOUTOut;

	/* CAM INIT */
	/* -------- */
	stCAMInitIn.eBufferMode 			= 	MC_BUFFERED_MODE;
	stCAMInitIn.ucExecute 				=  	1 ;
	stCAMInitIn.ucIsFixedGap			=  	1 ;
	stCAMInitIn.ucSuperimposed			=  	1;
	stCAMInitIn.ulMaxNumberOfPoints		=  	10;
	stCAMInitIn.ulUnderflowThreshold	= 	0 ;
	stCAMInitIn.usDimension 			=  1 ;
	stCAMInitIn.eCurveType				=  ePolynom5Interp;
	rc = MMC_CamTableInitCmd(gConnHndl,&stCAMInitIn, &stCAMInitOut);
	if (rc < 0)
	{
		printf("CAM init error %d\n",stCAMInitOut.usErrorID);
		return -1;
	}

	/* CAM ADD */
	/* ------- */
	stCAMAddIn.hMemHandle 		= stCAMInitOut.hMemHandle;
	stCAMAddIn.ulNumberOfPoints = 10;
	stCAMAddIn.ulStartIndex		= 0;	//irrelevant, operates always in auto append mode
	memcpy(stCAMAddIn.dbTable, data,stCAMAddIn.ulNumberOfPoints *sizeof(double));
	rc = MMC_CamTableAddCmd(gConnHndl,&stCAMAddIn, &stCAMAddOut);
	if (rc < 0)
	{
		printf("CAM Add error %d\n",stCAMAddOut.usErrorID);
		return -1;
	}


	/* CAM TABLE SELECT */
	/* ---------------- */
	stCAMSelectIn.ucIsMasterPosAbsolute = 1;
	stCAMSelectIn.ucIsSlavePosAbsolute = 1;
	stCAMSelectIn.uiStartMode =0;
	stCAMSelectIn.CamTable.dbGap = 1000;
	stCAMSelectIn.CamTable.dbMasterStartPosition = 0;


	stCAMSelectIn.CamTable.eCoordSystem = MC_NONE_COORD;
	stCAMSelectIn.CamTable.eTableMode = eCAMT_RWMode;
	stCAMSelectIn.CamTable.iCamTableID = stCAMInitOut.hMemHandle;
	strncpy(stCAMSelectIn.CamTable.pPathToTableFile,"",  10);

	rc = MMC_CamTableSelectCmd(gConnHndl,&stCAMSelectIn, &stCAMSelectOut);
	if (rc < 0)
	{
		printf("CAM select error %d\n",(short)stCAMSelectOut.usErrorID);
		return -1;
	}


	/* CAM IN */
	/* ------ */
	stCAMINIn.ucExecute 				= 1 ;
	stCAMINIn.dbMasterOffset 			= 0 ;
	stCAMINIn.dbMasterScaling			= 1.0 ;
	stCAMINIn.dbMasterStartDistance 	= 0 ;
	stCAMINIn.dbMasterSyncPosition 		= 0 ;
	stCAMINIn.dbSlaveOffset 			= 0;
	stCAMINIn.dbSlaveScaling 			= 1.0;
	stCAMINIn.eBufferMode 				= MC_BUFFERED_MODE ;
	stCAMINIn.eCurveType 				= ePolynom5Interp ;
	stCAMINIn.eMasterValueSource 		= eECAM_SET_VALUE ;
	stCAMINIn.ePeriodicMode 			= eCAM_LINEAR ;
	stCAMINIn.ucAutoOffset 				= 0 ;
	stCAMINIn.usMaster					= cAxis[0].GetRef();
	stCAMINIn.uiCamTableID				= stCAMInitOut.hMemHandle;

	rc = MMC_CamInCmd(gConnHndl,cAxis[1].GetRef(), &stCAMINIn, &stCAMINOut);
	if (rc < 0)
	{
		printf("CAM select error %d\n",stCAMINOut.sErrorID);
		return -1;

	}

	do
	{
		/* READ STATUS */
		/* ----------- */
		rc = MMC_CamStatusCmd(gConnHndl,cAxis[1].GetRef(), &stCAMStatusIn, &stCAMStatusOut);

		if (rc <0)
		{
			printf("CAM status error %d\n",stCAMStatusOut.sErrorID);
			return -1;

		}
		printf("CAM table ID:%d\n", stCAMStatusOut.uiCamTableID);
		printf("CAM current index:%ld\n", stCAMStatusOut.ulCurrentIndex);
		printf("CAM cycle count:%ld\n", stCAMStatusOut.ulCycle);
		printf("CAM num of profiles:%ld\n", stCAMStatusOut.ulEndOfProfile);
		printf("CAM status %d\n", stCAMStatusOut.usStatus);

		usleep (500000);
		cnt++;
	} while (cnt < 5);


	/* CAM OUT */
	/* ------- */

	stCAMOUTIn.ucExecute = 1;

	rc = MMC_CamOutCmd(gConnHndl, cAxis[1].GetRef(), &stCAMOUTIn, &stCAMOUTOut);
	if (rc < 0)
	{
		printf("CAM out  error %d\n",stCAMOUTOut.sErrorID);
		return -1;
	}


	/* Table Unload */
	/* ------------ */

	stUnloadIn.ucExecute 	= 1 ;
	stUnloadIn.hMemHandle =-1;

	rc = MMC_UnloadTableCmd(gConnHndl, &stUnloadIn, &stUnloadOut);
	if (rc < 0)
	{
		printf("unload error %d\n",stUnloadOut.usErrorID);
		return -1;
	}


	return 0;

}

int ECAM_MEM_Test2()
{
	MC_PATH_REF uiCamHandle;
	MC_CamRef stCamRef;
	unsigned long max_point;
	unsigned short dimension;
	unsigned char is_fixed_gap, is_master_pos_abs, is_slave_pos_abs;

	cAxis[2].UnloadCamTable(-1);

	max_point = 10;
	dimension = 1;
	is_fixed_gap = 1;
	uiCamHandle = cAxis[2].CamTableInit(max_point, dimension, is_fixed_gap);

	stCamRef.dbGap = 5000;
	stCamRef.dbMasterStartPosition = 0;
	stCamRef.eCoordSystem = MC_NONE_COORD;
	stCamRef.eTableMode = eCAMT_ROMode;
	stCamRef.iCamTableID = uiCamHandle ;
	strcpy(stCamRef.pPathToTableFile, "");
	is_master_pos_abs = 1;
	is_slave_pos_abs = 1;
	uiCamHandle = cAxis[2].CamTableSelect(stCamRef, is_master_pos_abs, is_slave_pos_abs);

	cAxis[2].CamTableAdd(uiCamHandle, data, dimension, max_point);

	cAxis[2].CamIn(	cAxis[1].GetRef(),
					MC_BUFFERED_MODE,
					uiCamHandle,
					ePolynom5Interp,
					0,
					eCAM_LINEAR);

	return 0;
}

