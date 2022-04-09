/*
 * SIL_CAPI.h
 *
 *  Created on: 08/08/2017
 *      Author: zivb
 */

#ifndef SIL_CAPI_H_
#define SIL_CAPI_H_

#include <assert.h>
#include <math.h>

#include <MMC_Definitions.h>
#include <mmcpplib.h>
#include "ElmoMaestroSIL_Main.h"
#include "ElmoMaestroSIL_Lib.h"
#include "ElmoMaestroSIL_XML.h"


extern unsigned int g_iNumberOfModleParameters;
extern unsigned char g_ucDebugMode;
extern FILE *		g_fd;
extern std::ofstream g_fsSimulink;

typedef struct{
	void                 *_param;
    void                 *_newParam;
    rtwCAPI_Orientation  orientation;
    uint_T               *dims;
    uint8_T              nDims;
    uint8_T              slDataType;
    unsigned short       isComplex;
}CAPI_MODIFY_PARA_STRUCT;

typedef struct{
    uint8_T              	uclDataType;
    unsigned short       	usIsComplex;
	rtwCAPI_Orientation  	eOrientation;
	unsigned int        	pdTunableparamADD;
	int 					iRowIdx;
	int 					iColIdx;
	double 					dValue;
	char 					strType[MAX_STR_SIZE];
	char   					strParamterName[MAX_STR_SIZE];
	char   					strStructParamterName[MAX_STR_SIZE];
	int   					iStructParamterIndex;
}SIL_CAPI_PARAM_STRUCT;


extern CAPI_MODIFY_PARA_STRUCT *g_stModifyParamData;

int_T capi_ModifyParameter(void                 *_param,
                           void                 *_newParam,
                           rtwCAPI_Orientation  orientation,
                           uint_T               *dims,
                           uint8_T              nDims,
                           uint8_T              slDataType,
                           unsigned short       isComplex);


void capi_PrintParameter(int iElmoIndex,
						 const char_T*  paramName,
                         void*          paramAddress,
                         uint8_T        slDataID,
                         unsigned short isComplex,
                         uint_T*        actualDims,
                         uint_T         numDims,
                         real_T         slope,
                         real_T         bias,
                         const char_T*  paramStructName,
                         const int 		iStructIndex);

void sil_capi_LoadModelParameter(rtwCAPI_ModelMappingInfo* capiMap,
                              uint_T                    paramIdx);

void sil_capi_struct_handler(uint16_T paramIdx,uint16_T dataTypeIdx,const uint_T* dimArray,uint_T* actualDimensions,   void*                paramAddress,
		const char* structName, int iStructIndex);

void capi_getParam(int iElmoIndex,
					 const char_T*  paramName,
	                 void*          paramAddress,
	                 uint8_T        slDataID,
	                 unsigned short isComplex,
	                 uint_T*        actualDims,
	                 uint_T         numDims,
	                 real_T         slope,
	                 real_T         bias,
	                 void*			returnValue,
	                 uint_T rowIdxIn,
	                 uint_T colIdxIn);

void sil_capi_save_param(int iRowIdx,int iColIdx,unsigned char ucslDataID,char *strType,double dValue,unsigned int uiParamADD,const char *strName,const char *strStructName,const int iStructIndex);
void sil_capi_delete_param(void);
void sil_capi_create_xml();
void sil_capi_create_xml_old();
int sil_capi_set_parameter(unsigned int uiParameterID, double value,unsigned int uiDataType);
int sil_capi_set_parameter_double(unsigned int uiParameterID, double value);
int sil_capi_set_parameter_int(unsigned int uiParameterID, int value);
int sil_capi_set_parameter_uint(unsigned int uiParameterID,unsigned int value);
int sil_capi_set_parameter_short(unsigned int uiParameterID,short value);
int sil_capi_set_parameter_ushort(unsigned int uiParameterID,unsigned short value);
int sil_capi_set_parameter_char(unsigned int uiParameterID,signed char value);
int sil_capi_set_parameter_uchar(unsigned int uiParameterID,unsigned char value);
int sil_capi_set_parameter_bool(unsigned int uiParameterID,bool value);
void sil_capi_get_parameter(unsigned int uiParameterID, double &value);
int sil_capi_get_parameter_double(unsigned int uiParameterID, double &value);
int sil_capi_get_parameter_int(unsigned int uiParameterID, int &value);
int sil_capi_get_parameter_uint(unsigned int uiParameterID,unsigned int &value);
int sil_capi_get_parameter_short(unsigned int uiParameterID,short &value);
int sil_capi_get_parameter_ushort(unsigned int uiParameterID,unsigned short &value);
int sil_capi_get_parameter_char(unsigned int uiParameterID,signed char &value);
int sil_capi_get_parameter_uchar(unsigned int uiParameterID,unsigned char &value);
int sil_capi_get_parameter_bool(unsigned int uiParameterID,bool &value);
int sil_capi_set_parameters_from_file();
int sil_capi_set_parameters_from_file_ex();
int sil_capi_get_overflow( long &lvalue);
int sil_capi_get_TimeMeasurement(unsigned long &ulElapsedTime,unsigned long &ulMaxElapsedTime,unsigned long &ulAvgElapsedTime,long &lOverflow);
int sil_capi_get_MaxElapsedTime(unsigned int uiParameterID, double &dvalue);
int sil_capi_stopexec();
int sil_capi_savelog();
void set_Rec(unsigned short usValue);
int sil_capi_startrec();
int sil_capi_stoprec();
int sil_capi_recstat(unsigned char &ucValue);



///
//Error define
#define SIL_PARAM_ID_OUT_OF_RANGE	-6000
#define SIL_PARAM_WRONG_TYPE		-6001

#endif /* SIL_CAPI_H_ */
