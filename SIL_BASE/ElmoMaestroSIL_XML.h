/*
 * SIL_XML.h
 *
 *  Created on: 07/08/2017
 *      Author: zivb
 */

#ifndef SIL_XML_H_
#define SIL_XML_H_

#define	  RAPIDXML_NO_EXCEPTIONS
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace rapidxml;
using namespace std;
#define 		SIL_BASE_VER						"1.0.0.1"
#define 		SIL_BASE_VER_EX						2/*"2.0.0.0"*/

extern MMCUserParams   g_UserParams;
#define SIMULINK_XML_PATH "SimulinkParam.xml"
//#define SIMULINK_XML_PATH1 "SimulinkParam1.xml"
#define SIMULINK_XML_PATH1 "SimulinkParam.xml"
#define SIMULINK_XML_FULL_PATH "/mnt/jffs/usr/SimulinkParam.xml"
#define SIMULINK_DEFAULT_XML "SimulinkParamDef.xml"
#define SIMULINK_DEF_XML_FULL_PATH "/mnt/jffs/usr/SimulinkParamDef.xml"
#define MAX_STR_SIZE 255
#define MAX_STRUCT_DEPTH 5
#define MODEL_VER_IDX 0
#define LAST_MODIFY_IDX 1
#define SIL_LIB_VER_IDX 2
#define SIL_LIB_LAST_MODIFY_IDX 3

int 	sil_xml_validation();
int 	sil_xml_getCompilationTimeFromXMLFile(char* strFileName,char* compilationTIme);
int 	sil_xml_getCompilationTimeFromXMLFileEX(char* strFileName,char* compilationTIme);
int 	sil_xml_getCompilationTime(char* compilationTIme);
void 	sil_xml_init_header();
void 	sil_xml_init_header_ex();
void 	sil_xml_set_version_data();
void 	sil_xml_set_version_data_ex();
void 	sil_xml_save();
void 	sil_xml_save_ex();
void 	sil_xml_set_param_name(const char *strName);
void 	sil_xml_set_id_name(int iID);
void 	sil_xml_write_param_double(unsigned int uiRowIdx,unsigned int uiColIdx,double value);
void 	sil_xml_write_param(unsigned int uiRowIdx,unsigned int uiColIdx,unsigned char ucslDataID,char *strType,double dValue);
void 	sil_xml_read_param(unsigned int uiParamID,double &dValue,char* ParamterName);
void 	sil_xml_read_param_ex(unsigned int uiParamID,double &dValue,char* ParamterName);

void 	sil_xml_write_param_ex(int iRowIdx,int iColIdx,unsigned char ucslDataID,char *strType,double dValue,char *strStructName,const char* strStructID);
void *sil_xml_search_struct_node(xml_node<> *node_traverse,const char* strNodeName,const char* strStructID);
void *sil_xml_search_node(xml_node<> *node_traverse,const char* strNodeName);
void *sil_xml_search_nodeID(xml_node<> *node_traverse, const char* strNodeName, const char* strNodeID);

void sil_xml_set_recording_data();
#endif /* SIL_XML_H_ */
