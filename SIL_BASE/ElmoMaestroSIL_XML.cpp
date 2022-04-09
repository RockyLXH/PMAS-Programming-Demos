/*
 * SIL_XML.cpp
 *
 *  Created on: 07/08/2017
 *      Author: zivb
 */

#include <iostream>
#include <string>
#include <sstream>

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include <MMC_Definitions.h>
#include <mmcpplib.h>
#include "ElmoMaestroSIL_XML.h"
#include "ElmoMaestroSIL_Main.h"
//#define SIL_REC
#include <Simulink\ElmoMaestroSIL_StepsAndRecordings.h>
#include <ElmoMaestroSIL_CAPI.h>

using namespace rapidxml;
using namespace std;

xml_document<>	g_xDoc;
xml_node<>* root;
std::ofstream g_fsSimulink;
FILE *		g_fd;
///
MMCUserParams   g_UserParams;
char    		g_cProgVerBuf[MAX_STR_SIZE];

char * 			g_strRowIndex	=	"RowIndex";
char * 			g_strColIndex	=	"ColsIndex";
char * 			g_strValue		=	"Value";
char * 			g_strType		= 	"Type";
char   			g_cParamterName[MAX_STR_SIZE];
char  			g_cNodeId[MAX_STR_SIZE];

char 			g_buff[MAX_STR_SIZE];
int  			g_iPrameterIndex = 0;

//version related
char  			g_strgeneral[MAX_STR_SIZE]	=	"SIL_GENERAL_DATA";
char 			g_strverdata[MAX_STR_SIZE]	=	"VER_DATA";
char * 			g_strVer					=	"SIL_BASE_VER";
char * 			g_strModelName				=	"MODEL_NAME";
char * 			g_strCAPI					=	"C_API_SUPPORT";

#ifdef BUILTIN_TYPEID_TYPES
#define 		SIL_SUPPORT_CAPI						"1"
#else
#define 		SIL_SUPPORT_CAPI						"0"
#endif
//#define 		SIL_BASE_VER						1
extern unsigned char	g_ucDebugMode ;
////////////////////////////////////////////////////////////////////////////////
/// \fn static xml_validation()
/// \brief  Check the time and date in "SimulinkParam.xml" is the same as compilation time
///	\param
/// \return	return - 0 - in case of OK
///					 1 - in case of error .
///
////////////////////////////////////////////////////////////////////////////////
int sil_xml_validation()
{
	char  pAtt1[MAX_STR_SIZE] ;
	char  pAtt2[MAX_STR_SIZE] ;
	int rc ;

	g_UserParams.Open(SIMULINK_DEFAULT_XML,UPXML_SET_DEF_REQ_FLG);
	g_UserParams.GetXmlFileDescrp    (pAtt1, pAtt2, (int32_t)MAX_STR_SIZE);
	g_UserParams.Close();

	sprintf(g_cProgVerBuf, "%s %s", __DATE__, __TIME__);
	rc = strcmp(pAtt2,g_cProgVerBuf);

	if (0 == rc)
		return rc;
	else
		return 1;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_getCompTimeFromFile(char compilationTIme[100])
/// \brief  return compilation time and date from header in file Simulink.XML
///	\param compilationTIme - hold the value in case file exist
/// \return	return - 0 - in case of OK
///					 -1 - in case of error .
///
////////////////////////////////////////////////////////////////////////////////
int sil_xml_getCompilationTimeFromXMLFile(char* strFileName,char* compilationTIme)
{
	FILE *fp;
	char  pAtt1[MAX_STR_SIZE] ;
	char  pAtt2[MAX_STR_SIZE] ;
	char  strFullpath[MAX_STR_SIZE] ;

	int rc = 0;

    /* check if parameter file exist (xml file) */
	sprintf(strFullpath,"%s%s",DEFAULT_XML_RFILE_PATH,strFileName);
	fp = fopen(strFullpath, "r");
	if (fp == NULL)
		return -1;

//	g_UserParams.Open(SIMULINK_XML_PATH,UPXML_SET_DEF_REQ_FLG);
	g_UserParams.Open(strFileName,UPXML_SET_DEF_REQ_FLG);
	g_UserParams.GetXmlFileDescrp    (pAtt1, pAtt2,MAX_STR_SIZE);
	g_UserParams.Close();
	strncpy(compilationTIme, pAtt2 ,MAX_STR_SIZE * sizeof(char));

	if(g_ucDebugMode)
	{
		printf("%s: strlen(compilationTIme) = %d\n", __func__, strlen(compilationTIme));
	}

	fclose(fp);

	return rc;
}
int sil_xml_getCompilationTimeFromXMLFileEX(char* strFileName,char* compilationTIme)
{
	FILE *fp;
	char  strFullpath[MAX_STR_SIZE] ;
	int iSize = 0 ;
	int rc = 0;
	xml_node<> *compilation_time_node = NULL;
	xml_node<> *file_data_node = NULL;

    /* check if parameter file exist (xml file) */
	sprintf(strFullpath,"%s%s",DEFAULT_XML_RFILE_PATH,strFileName);
	fp = fopen(strFullpath, "r");
	if (fp == NULL)
	{
		return -1;
	}
	fclose(fp);

	//ifstream theFile (strFileName);
	ifstream theFile (strFullpath);


	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	g_xDoc.parse<0>(&buffer[0]);


	root = g_xDoc.first_node("rootnode");
	if(root == NULL)
	{
		return -1;
	}

	file_data_node = root->first_node("FILE_DATA");
	if(file_data_node == NULL)
	{
		return -1;
	}

	compilation_time_node = file_data_node->first_node("COMPILATION");

	if(compilation_time_node)
	{

		iSize = (compilation_time_node->value_size() > MAX_STR_SIZE)?MAX_STR_SIZE:compilation_time_node->value_size();
		strncpy(compilationTIme, compilation_time_node->value() ,iSize);
		compilationTIme[iSize] = NULL;

		if(g_ucDebugMode)
		{
			printf("%s: strlen(compilationTIme) = %d\n", __func__, strlen(compilationTIme));
		}
	}

	theFile.close();
	return rc;

}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_getCompTimeFromFile(char compilationTIme[100])
/// \brief  return compilation time and date from header in file Simulink.XML
///	\param compilationTIme - hold the value in case file exist
/// \return	return - 0 - in case of OK
///					 -1 - in case of error .
///
////////////////////////////////////////////////////////////////////////////////
int sil_xml_getCompilationTime(char* compilationTIme)
{
	int rc = 0;

	sprintf(g_cProgVerBuf, "%s %s", __DATE__, __TIME__);

	strncpy(compilationTIme, g_cProgVerBuf ,MAX_STR_SIZE * sizeof(char));

	if(g_ucDebugMode)
	{
		printf("%s: compilationTIme = %s\n", __func__, compilationTIme);
		printf("%s: strlen(compilationTIme) = %d\n", __func__, strlen(compilationTIme));
	}

	return rc;
}


////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_init_header()
/// \brief  Init XML header with time and date of compilation and zero the parameters index
///	\param
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_xml_init_header()
{
	char  cMsgBuff[MAX_STR_SIZE];

	g_iPrameterIndex = 0;
	sprintf(g_cProgVerBuf, "%s %s", __DATE__, __TIME__);
	sprintf(cMsgBuff, "Simulink Parameters file: %s",  SIMULINK_XML_PATH);
	g_UserParams.PutXmlFileRoot("SchemDefinition", "SchemFile.xsd");
	g_UserParams.PutXmlFileDescrp(cMsgBuff,  g_cProgVerBuf);

}

void sil_xml_init_header_ex()
{
//	char  cMsgBuff[MAX_STR_SIZE];

	g_iPrameterIndex = 0;
	sprintf(g_cProgVerBuf, "%s %s", __DATE__, __TIME__);

	g_fsSimulink.open(SIMULINK_XML_PATH1);
	//
	//
	// xml declaration e.g. <?xml version="1.0" encoding="utf-8"?>
	xml_node<>* decl = g_xDoc.allocate_node(node_declaration);
	decl->append_attribute(g_xDoc.allocate_attribute("version", "1.0"));
	decl->append_attribute(g_xDoc.allocate_attribute("encoding", "utf-8"));
	g_xDoc.append_node(decl);

	// root node <rootnode version="1.0" type="example">
	root = g_xDoc.allocate_node(node_element, "rootnode");
	root->append_attribute(g_xDoc.allocate_attribute("xmlns:xsi", "SchemDefinition"));
	root->append_attribute(g_xDoc.allocate_attribute("xsi:noNamespaceSchemaLocation", "SchemFile.xsd"));
	g_xDoc.append_node(root);
}


////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_set_version_data()
/// \brief  set parameters relvant to XML such as version number
///	\param
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_xml_set_version_data()
{
	g_UserParams.Write(g_strgeneral, g_strverdata, g_strVer, SIL_BASE_VER,strlen(SIL_BASE_VER));
	g_UserParams.Write(g_strgeneral, g_strverdata, g_strModelName, ELMO_MODEL_NAME,strlen(ELMO_MODEL_NAME));
	g_UserParams.Write(g_strgeneral, g_strverdata, g_strCAPI, SIL_SUPPORT_CAPI,strlen(SIL_SUPPORT_CAPI));
}

void sil_xml_set_version_data_ex()
{

	xml_node<> *file_data_node = g_xDoc.allocate_node(node_element, "FILE_DATA");
	xml_node<> *name_node = g_xDoc.allocate_node(node_element, "NAME");
	xml_node<> *compilation_time_node = g_xDoc.allocate_node(node_element, "COMPILATION");
	xml_node<> *sil_base_ver_node = g_xDoc.allocate_node(node_element, g_strVer);
	xml_node<> *model_name_node = g_xDoc.allocate_node(node_element, g_strModelName);
	xml_node<> *support_node = g_xDoc.allocate_node(node_element, g_strCAPI);

	xml_node<> *last_modify_by_node = g_xDoc.allocate_node(node_element, "LAST_MODIFY_BY");
	xml_node<> *model_version_node = g_xDoc.allocate_node(node_element, "MODEL_VERSION");
	xml_node<> *sil_lib_version_node = g_xDoc.allocate_node(node_element, "SIL_LIBRARY_VER");
	xml_node<> *sil_lib_last_modify_node = g_xDoc.allocate_node(node_element, "SIL_LIBRARY_LAST_MODIFY");

	name_node->value(SIMULINK_XML_PATH1);
	compilation_time_node->value(g_cProgVerBuf);

	sprintf(g_buff,"%d",SIL_BASE_VER_EX);
	sil_base_ver_node->value(g_xDoc.allocate_string(g_buff));
	model_name_node->value(ELMO_MODEL_NAME);
	support_node->value(SIL_SUPPORT_CAPI);

	last_modify_by_node->value(SIL_PRIVATE_DATA[LAST_MODIFY_IDX]);
	model_version_node->value(SIL_PRIVATE_DATA[MODEL_VER_IDX]);
	sil_lib_version_node->value(SIL_PRIVATE_DATA[SIL_LIB_VER_IDX]);
	sil_lib_last_modify_node->value(SIL_PRIVATE_DATA[SIL_LIB_LAST_MODIFY_IDX]);

	file_data_node->append_node(name_node);
	file_data_node->append_node(compilation_time_node);
	file_data_node->append_node(sil_base_ver_node);
	file_data_node->append_node(model_name_node);
	file_data_node->append_node(support_node);

	file_data_node->append_node(last_modify_by_node);
	file_data_node->append_node(model_version_node);
	file_data_node->append_node(sil_lib_version_node);
	file_data_node->append_node(sil_lib_last_modify_node);

	root->append_node(file_data_node);

}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_save()
/// \brief  Save Simulink.xml file and zero the parameters index
///	\param
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_xml_save()
{
	g_UserParams.Save(SIMULINK_XML_PATH);
	g_UserParams.Close();
	sync();
	g_iPrameterIndex = 0;
}

void sil_xml_save_ex()
{
	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	print(std::back_inserter(xml_as_string), g_xDoc);
	// xml_as_string now contains the XML in string form, indented
	// (in all its angle bracket glory)

	std::string xml_no_indent;
	// print_no_indenting is the only flag that print() knows about
	print(std::back_inserter(xml_no_indent), g_xDoc, print_no_indenting);
	// xml_no_indent now contains non-indented XML

	g_fsSimulink << g_xDoc;

	g_fsSimulink.close();

	if(g_ucDebugMode)
	{
		printf("\33[0;31msil_xml_save_ex():  Before g_xDoc.clear()\33[0m\n");
		sleep(1);
	}
	g_xDoc.clear();

	if(g_ucDebugMode)
	{
		printf("\33[0;31msil_xml_save_ex():  After g_xDoc.clear()\33[0m\n");
		sleep(1);
	}

	sync();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_set_param_name
/// \brief  set parameter name that we will write to the Simulink.XML
///	\param char* strName - Then name of the parameter which we have extract from Simulink map
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_xml_set_param_name(const char *strName)
{
	if(strName != NULL)
	{
	    strncpy(g_cParamterName,strName,strlen(strName));
	    g_cParamterName[strlen(strName)] = '\0';
	}
}
void sil_xml_set_id_name(int iID)
{
	sprintf(g_cNodeId, "%d", iID);
	g_cNodeId[strlen(g_cNodeId)] = '\0';
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_write_param
/// \brief  Write to XML file relevant data regarding the parameter
///	\param uiRowIdx 	- Row index of the parameter
/// \	   uiRowIdx 	- Column index of the parameter
/// \ 	   ucslDataID 	- built-in data types .reference : builtin_typeid_types.h BuiltInDTypeId
/// \ 	   strType 		- string of the type to be written in the XML
/// \ 	   dValue 		- Value of the parameter
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_xml_write_param(unsigned int uiRowIdx,unsigned int uiColIdx,unsigned char ucslDataID,char *strType,double dValue)
{
#ifdef BUILTIN_TYPEID_TYPES
    sprintf(g_buff,"%d",g_iPrameterIndex);
    g_UserParams.Write(g_cParamterName, g_buff, g_strRowIndex, (int32_t)(long int)uiRowIdx);
    g_UserParams.Write(g_cParamterName, g_buff, g_strColIndex, (int32_t)(long int)uiColIdx);
    g_UserParams.Write(g_cParamterName, g_buff, g_strType, strType, (int32_t)(long int)strlen(strType));
	switch(ucslDataID)
	{
	case SS_DOUBLE:
	    g_UserParams.Write(g_cParamterName, g_buff, g_strValue, dValue);
		break;
	case SS_INT32:
	case SS_UINT32:
	case SS_INT16:
	case SS_UINT16:
	case SS_INT8:
	case SS_UINT8:
		g_UserParams.Write(g_cParamterName, g_buff, g_strValue, (int32_t)(long int)dValue);
		break;
	case SS_BOOLEAN:
		 g_UserParams.Write(g_cParamterName, g_buff, g_strValue, (bool)dValue);
		break;
	default:
		break;

	}
#endif
    g_iPrameterIndex++;
}







void *sil_xml_search_struct_node(xml_node<> *node_traverse,const char* strNodeName,const char* strStructID)
{

	void *temp_node;
	for ( node_traverse = node_traverse->first_node("PARAMETER");
			node_traverse != NULL;
			node_traverse = node_traverse->next_sibling())
		{
		    const rapidxml::xml_attribute<> *type_attribute = node_traverse->first_attribute("Type");

		    //If the type of the node is not struct continue
		    if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") != 0) )
		    {
		        continue;
		    }
		    //
		    else if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") == 0) )
		    {
//		    	 const rapidxml::xml_attribute<> *name_attribute = node_traverse->first_attribute("NAME");
		    	 const rapidxml::xml_attribute<> *id_attribute = node_traverse->first_attribute("ID");

//				    if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) != 0) )
				    if ( (id_attribute) && (strcmp(id_attribute->value(), strStructID) != 0) )
				    {

				    	/*
				    	printf("%s found nod name %s\n",__func__,name_attribute->value());
//				    	if(node_traverse->next_sibling())
//				    	{
//				    		printf("%s recursive call\n",__func__);
				    		return sil_xml_search_struct_node(node_traverse,strNodeName);
//				    	}
//				    	else
//				    	{
//				    		printf("%s retrun NULL\n",__func__);
//				    		return NULL;
//				    	}
				    	*/
				    	temp_node =sil_xml_search_struct_node(node_traverse, strNodeName, strStructID);
				    		if(temp_node == NULL && node_traverse->next_sibling())
				    		{
				    			continue;
				    		}
				    		else
				    		{
				    			return temp_node;
				    		}

				    }
//				    else if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) == 0) )
					else if ( (id_attribute) && (strcmp(id_attribute->value(), strStructID) == 0) )
				    {
				        return node_traverse;
				    }
		    }
		    //
		}//end for
	// we did not found strcut return NULL
	return NULL;
}


//void *sil_xml_search_node(xml_node<> *node_traverse,const char* nodeID)
//{
//	printf("%s nodeID = %s\n",__func__,nodeID);
//	for ( node_traverse = node_traverse->first_node("PARAMETER");
//			node_traverse != NULL;
//			node_traverse = node_traverse->next_sibling())
//		{
//		    const rapidxml::xml_attribute<> *type_attribute = node_traverse->first_attribute("Type");
//
//		    //If the type of the node is not struct check if it is the requested node name
//		    if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") != 0) )
//		    {
//		    	const rapidxml::xml_attribute<> *name_attribute = node_traverse->first_attribute("ID");
//
//			    if ( (name_attribute) && (strcmp(name_attribute->value(), nodeID) != 0) )
//			    {
//			    	printf("%s continue\n",__func__);
//			    	continue;
//			    }
//			    else if ( (name_attribute) && (strcmp(name_attribute->value(), nodeID) == 0) )
//			    {
//			    	printf("%s return node_traverse\n",__func__);
//			    	return node_traverse;
//			    }
//		    }
//		    //
//		    else if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") == 0) )
//		    {
//		    	printf("%s found STRUCCT strNodeName = %s\n",__func__,nodeID);
//		    	return sil_xml_search_node(node_traverse,nodeID);
//
//			}
//	    //
//		}//end for
//	// we did not found node name return NULL
//	printf("%s return NULL;\n",__func__);
//	return NULL;
//}

//
void *sil_xml_search_node(xml_node<> *node_traverse, const char* strNodeName)
{

	for ( node_traverse = node_traverse->first_node("PARAMETER");
			node_traverse != NULL;
			node_traverse = node_traverse->next_sibling())
		{
		    const rapidxml::xml_attribute<> *type_attribute = node_traverse->first_attribute("Type");
		    if(g_ucDebugMode)
		    {
		    	printf("\33[0;34m strNodeName = %s, \33[0m", strNodeName);
		    }

		    //If the type of the node is not struct check if it is the requested node name
		    if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") != 0) )
		    {
		    	const rapidxml::xml_attribute<> *name_attribute = node_traverse->first_attribute("NAME");

			    if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) != 0) )
			    {
			    	if(g_ucDebugMode)
			    	{
			    		printf("\33[0;34m continue... \33[0m\n");
			    	}
			    	continue;
			    }
			    else if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) == 0) )
			    {
			    	if(g_ucDebugMode)
			    	{
			    		printf("\33[0;34m return node_traverse,  \33[0m\n");
			    	}
			    	return node_traverse;
			    }
		    }
		    else if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") == 0) )
		    {

		    	const rapidxml::xml_attribute<> *name_attribute = node_traverse->first_attribute("NAME");

		    	if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) != 0) )
		    	{
		    		if(g_ucDebugMode)
		    		{
		    			printf("\33[0;34m return sil_xml_search_node... \33[0m\n");
		    		}
		    		return sil_xml_search_node(node_traverse, strNodeName);
		    	}
		    	else if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) == 0) )
		    	{
		    		if(g_ucDebugMode)
		    		{
		    			printf("\33[0;34m STRUCT: return node_traverse... \33[0m\n");
		    		}
		    		return node_traverse;
		    	}
			}
	    //
		}//end for
	// we did not found node name return NULL

	return NULL;
}

void *sil_xml_search_nodeID(xml_node<> *node_traverse, const char* strNodeName, const char* strNodeID)
{
	static int 	iCurrentStructDepth = 0;
	static int  iTmpStructDepth = 0;
	static int	iStructSiblings[MAX_STRUCT_DEPTH] = {0};		// index zero is dummy.  Index one is the first relevant index
	static int	iTmpStructSiblings[MAX_STRUCT_DEPTH] = {0};

	static int	iLastSiblingFlag[MAX_STRUCT_DEPTH] = {0};
	static int	iLastIDFlag = 0;
	static int	iIDAfterStructFlag = 0;

	int	i;

	for ( node_traverse = node_traverse->first_node("PARAMETER");
			node_traverse != NULL;
			node_traverse = node_traverse->next_sibling())
	{

		if(g_ucDebugMode)
		{
			printf("\33[0;34m strNodeName = %s, strNodeID = %s, iCurrentStructDepth = %d, iTmpStructDepth = %d \33[0m", strNodeName, strNodeID, iCurrentStructDepth, iTmpStructDepth);

			printf("\n\33[0;34m    iStructSiblings\33[0m");

			for(i = 1; i <= iCurrentStructDepth; i++)
			{
				printf("\33[0;34m[%d] = %d, \33[0m", i, iStructSiblings[i]);
			}

			printf("\n\33[0;34m iTmpStructSiblings\33[0m");
			for(i = 1; i <= iCurrentStructDepth; i++)
			{
				printf("\33[0;34m[%d] = %d, \33[0m", i, iTmpStructSiblings[i]);
			}

			printf("\n\33[0;34m   iLastSiblingFlag\33[0m");
			for(i = 1; i <= iCurrentStructDepth; i++)
			{
				printf("\33[0;34m[%d] = %d, \33[0m", i, iLastSiblingFlag[i]);
			}

			if(iLastIDFlag)
				printf("\33[0;32m iLastIDFlag = %d, \33[0m", iLastIDFlag);

			if(iIDAfterStructFlag)
				printf("\33[0;32m iIDAfterStructFlag = %d, \33[0m", iIDAfterStructFlag);
		}

		const rapidxml::xml_attribute<> *type_attribute = node_traverse->first_attribute("Type");

	    if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") == 0) )
	    {
	    	const rapidxml::xml_attribute<> *name_attribute = node_traverse->first_attribute("NAME");

	    	if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) == 0) )
	    	{
	    		if(iIDAfterStructFlag)
	    		{
	    			iIDAfterStructFlag = 0;
	    			--iStructSiblings[iCurrentStructDepth];
	    			if(g_ucDebugMode)
	    			{
	    				printf("\033[1m\033[37m iStructSiblings[%d] = %d   !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! \33[0m", iCurrentStructDepth, iStructSiblings[iCurrentStructDepth]);
	    			}
	    		}

	    		if(iLastIDFlag)
    			{
    				iLastIDFlag = 0;
    			}
    			else
    			{
    				++iCurrentStructDepth;
    				++iStructSiblings[iCurrentStructDepth];
    			}

	    		if(node_traverse->next_sibling() == NULL)
		    	{
	    			iLastSiblingFlag[iCurrentStructDepth] = 1;
		    	}

    			iTmpStructDepth = 0;
     			iTmpStructSiblings[iCurrentStructDepth] = 0;

     			if(g_ucDebugMode)
     			{
     				printf("\33[0;34m STRUCT: return node_traverse... \33[0m\n\n");
     			}
    			return node_traverse;
	    	}
	    	else if ( (name_attribute) && (strcmp(name_attribute->value(), strNodeName) != 0) )
	    	{
	    		if(iTmpStructDepth < iCurrentStructDepth)
    			{
    				iTmpStructDepth++;
    			}

    			if(iTmpStructDepth == iCurrentStructDepth)
    			{
    				if(++iTmpStructSiblings[iTmpStructDepth] == iStructSiblings[iTmpStructDepth])
    				{
    					iTmpStructSiblings[iTmpStructDepth] = 0;
    					iTmpStructDepth = 0;

    					if(g_ucDebugMode)
    					{
    						printf("\033[1m\033[37m Recursion call... \33[0m\n");
    					}
            			return sil_xml_search_nodeID(node_traverse, strNodeName, strNodeID);	// Recursion call...
    				}

//	    			printf("\033[1m\033[32m continue...(next struct) \33[0m\n");
//	    			continue;
    			}
    			else if(iIDAfterStructFlag)
    			{
       				++iStructSiblings[iCurrentStructDepth];
   	    			printf("\033[1m\033[31m iStructSiblings[%d] = %d   !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! \33[0m", iCurrentStructDepth, iStructSiblings[iCurrentStructDepth]);

    				iIDAfterStructFlag = 0;
    				iLastIDFlag = 1;

    				if(g_ucDebugMode)
    				{
    					printf("\033[1m\033[32m continue...(next struct) \33[0m\n");
    				}
        			continue;
    			}
    			else if(iTmpStructDepth == iCurrentStructDepth - 1)
    			{
    				if(++iTmpStructSiblings[iCurrentStructDepth - 1] == iStructSiblings[iCurrentStructDepth - 1])
    				{
    					iTmpStructSiblings[iTmpStructDepth] = 0;

    					if(g_ucDebugMode)
    					{
    						printf("\33[0;34m Recursion call... \33[0m\n");
    					}
    					return sil_xml_search_nodeID(node_traverse, strNodeName, strNodeID);	// Recursion call...
    				}
    			}
    			else if(iTmpStructDepth == iCurrentStructDepth - 2)
    			{
    				if(++iTmpStructSiblings[iCurrentStructDepth - 2] == iStructSiblings[iCurrentStructDepth - 2])
    				{
        				iTmpStructSiblings[iTmpStructDepth] = 0;

        				if(g_ucDebugMode)
        				{
        					printf("\33[0;34m Recursion call... \33[0m\n");
        				}
        				return sil_xml_search_nodeID(node_traverse, strNodeName, strNodeID);	// Recursion call...
    				}
    			}

    			if(g_ucDebugMode)
    			{
    				printf("\033[1m\033[32m continue...(next struct) \33[0m\n");
    			}
    			continue;

//    			printf("\33[0;34m Recursion call... \33[0m\n");
//    			return sil_xml_search_nodeID(node_traverse, strNodeName, strNodeID);	// Recursion call...
    		}
		}
	    //If the type of the node is not struct check if it is the requested node name
	    else if ( (type_attribute) && (strcmp(type_attribute->value(), "STRUCT") != 0) )
	    {
	    	const rapidxml::xml_attribute<> *id_attribute = node_traverse->first_attribute("ID");

	    	if( (id_attribute) && (strcmp(id_attribute->value(), strNodeID) == 0) )
	    	{
	    		if(node_traverse->next_sibling() == NULL)
		    	{
		    		iLastIDFlag = 1;

	    			while(iLastSiblingFlag[iCurrentStructDepth])
    				{
    					iStructSiblings[iCurrentStructDepth] = 0;
    					iLastSiblingFlag[iCurrentStructDepth] = 0;

    					--iCurrentStructDepth;
    				}

    				++iStructSiblings[iCurrentStructDepth];
    				if(g_ucDebugMode)
    				{
    					printf("\033[1m\033[31m iStructSiblings[%d] = %d   !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! \33[0m", iCurrentStructDepth, iStructSiblings[iCurrentStructDepth]);
    				}
		    	}
	    		else if(iIDAfterStructFlag)
	    		{
	    			iIDAfterStructFlag = 0;
	    			iLastIDFlag = 1;

      				++iStructSiblings[iCurrentStructDepth];
      				if(g_ucDebugMode)
      				{
      					printf("\033[1m\033[31m iStructSiblings[%d] = %d   !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! \33[0m", iCurrentStructDepth, iStructSiblings[iCurrentStructDepth]);
      				}
	    		}

	    		if(iLastIDFlag)
	    		{
	    			for(i=1; i<iCurrentStructDepth; i++)
	    			{
	    				iTmpStructSiblings[i] = 0;
	    			}
	    		}

	    		iTmpStructDepth = 0;

	    		if(g_ucDebugMode)
	    		{
	    			printf("\33[0;34m return node_traverse... \33[0m\n\n");
	    		}
	    		return node_traverse;
	    	}
	    	else if ( (id_attribute) && (strcmp(id_attribute->value(), strNodeID) != 0) )
	    	{
	    		if(iLastIDFlag)
	    		{
	    			iLastIDFlag = 0;
	    			--iStructSiblings[iCurrentStructDepth];
	    			if(g_ucDebugMode)
	    			{
	    				printf("\033[1m\033[37m iStructSiblings[%d] = %d   !! !! !! !! !! !! !! !! !! !! !! !! !! !! !! \33[0m", iCurrentStructDepth, iStructSiblings[iCurrentStructDepth]);
	    			}

	    			iIDAfterStructFlag = 1;
	    		}
	    		if(g_ucDebugMode)
	    		{
	    			printf("\33[0;34m continue... \33[0m\n");
	    		}
    			continue;
	    	}
	    }
    //
	}//end for

return NULL;	// we did not found node name return NULL
}


////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_write_param_ex
/// \brief  Write to XML file relevant data regarding the parameter
///	\param uiRowIdx 	- Row index of the parameter
/// \	   uiRowIdx 	- Column index of the parameter
/// \ 	   ucslDataID 	- built-in data types .reference : builtin_typeid_types.h BuiltInDTypeId
/// \ 	   strType 		- string of the type to be written in the XML
/// \ 	   dValue 		- Value of the parameter
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_xml_write_param_ex(int iRowIdx,int iColIdx,unsigned char ucslDataID,char *strType,double dValue,char *strStructName, const char* strStructID)
{
#ifdef BUILTIN_TYPEID_TYPES
	////
	xml_node<> *parameter_node;
	xml_node<>* nodeP = g_xDoc.allocate_node(node_element, "PARAMETER");

	nodeP->append_attribute(g_xDoc.allocate_attribute("NAME", g_xDoc.allocate_string(g_cParamterName)));

	if(g_ucDebugMode)
	{
		printf("**************************\n");
		printf("para name = %s ,type = %s ,strStructName = '%s'\n",g_cParamterName,strType,strStructName);
	}
	if( strStructName[0] == '\0')
		strStructName = NULL;

	if(g_ucDebugMode)
	{
		printf("**************************\n");
	}
	sprintf(g_buff,"%d",g_iPrameterIndex);

	char * idxStr = g_xDoc.allocate_string(g_buff);

	nodeP->append_attribute(g_xDoc.allocate_attribute("ID", idxStr));

	if(iRowIdx != -1)
	{
		sprintf(g_buff,"%d",iRowIdx);
		nodeP->append_attribute(g_xDoc.allocate_attribute("RowIndex", g_xDoc.allocate_string(g_buff)));
	}

	if(iColIdx != -1)
	{
		sprintf(g_buff,"%d",iColIdx);
		nodeP->append_attribute(g_xDoc.allocate_attribute("ColsIndex", g_xDoc.allocate_string(g_buff)));
	}

	nodeP->append_attribute(g_xDoc.allocate_attribute("Type", strType));
//	sprintf(g_buff,"%lf",dValue);

	sprintf(g_buff,"%.16g",dValue);

	if(g_ucDebugMode)
	{
		printf("sil_xml_write_param_ex:  dValue = %.16g,  g_buff = %s\n\n", dValue, g_buff);
	}

	nodeP->append_attribute(g_xDoc.allocate_attribute("Value", g_xDoc.allocate_string(g_buff)));


	if(!strcmp("STRUCT",strType))
	{
		if(g_ucDebugMode)
		{
			printf("** IDENTIFY STRUCT **** \n");
		}

		for (parameter_node = root->first_node("PARAMETER");
			parameter_node != NULL;
			parameter_node = parameter_node->next_sibling())
		{
		    const rapidxml::xml_attribute<> *name_attribute = parameter_node->first_attribute("NAME");

		    if( (name_attribute) && (strcmp(name_attribute->value(), g_cParamterName) != 0) )
		    {
		        continue;
		    }
		    if(g_ucDebugMode)
		    {
		    	printf("Index = %d, strStructID = %s, StrName = %s, StrFatherName = %s\n", g_iPrameterIndex, strStructID, g_cParamterName, strStructName);
		    }
		    break;

		}

		//if  node of this struct type exist appen to it else create a new node node
		if ( parameter_node )
		{
			if(g_ucDebugMode)
			{
				printf("\33[0;31mIndex = %d, strStructID = %s, StrName = %s, StrFatherName = %s\33[0m\n", g_iPrameterIndex, strStructID, g_cParamterName, strStructName);
			}
			parameter_node->append_node(nodeP);
//			return;
		}
		else
		{
			if(g_ucDebugMode)
			{
				printf("**IDENTIFY STRUCT creating node of %s\n", g_cParamterName);
			}
			xml_node<>* nodeStructAppend = g_xDoc.allocate_node(node_element, "PARAMETER");
			nodeStructAppend->append_attribute(g_xDoc.allocate_attribute("NAME", g_xDoc.allocate_string(g_cParamterName)));
			nodeStructAppend->append_attribute(g_xDoc.allocate_attribute("ID", idxStr));
			nodeStructAppend->append_attribute(g_xDoc.allocate_attribute("Type", "STRUCT"));
			if(!strStructName)
			{
				if(g_ucDebugMode)
				{
					printf("\33[0;34mIndex = %d, strStructID = %s, StrName = %s, StrFatherName = %s\33[0m\n", g_iPrameterIndex, strStructID, g_cParamterName, strStructName);
				}
//				printf("appendig nodet struct with parameter name %s to root\n",g_cParamterName);
				root->append_node(nodeStructAppend);
			}
			else
			{
				//Append it to its father node which name is strStructName
				if(g_ucDebugMode)
				{
					printf("\33[0;32mIndex = %d, strStructID = %s, StrName = %s, StrFatherName = %s\33[0m\n", g_iPrameterIndex, strStructID, g_cParamterName, strStructName);
				}
				parameter_node = (xml_node<>*)sil_xml_search_struct_node(root, strStructName, strStructID);
				if(parameter_node != NULL)
				{
					if(g_ucDebugMode)
					{
						printf("appendig nodet g_cParamterName %s to strStructName %s\n",g_cParamterName,strStructName);
					}
					parameter_node->append_node(nodeStructAppend);
				}
			}
		}



	}
	//
	if(strStructName && (strcmp("STRUCT",strType) != 0))
	{
		if(g_ucDebugMode)
		{
			printf("** paramter that have a struct father **** \n");
			printf("searching for %s\n",strStructName);
		}
		parameter_node = (xml_node<>*)sil_xml_search_struct_node(root, strStructName, strStructID);
		if ( parameter_node )
		{
			if(g_ucDebugMode)
			{
				printf("found node with attribute NAME %s\n",strStructName);
			}
			parameter_node->append_node(nodeP);
		}
		else
		{
			if(g_ucDebugMode)
			{
				printf("did not found node with attribute NAME %s\n",strStructName);
			}
			xml_node<>* nodeStructAppend = g_xDoc.allocate_node(node_element, "PARAMETER");
			nodeStructAppend->append_attribute(g_xDoc.allocate_attribute("NAME", g_xDoc.allocate_string(strStructName)));
			nodeStructAppend->append_attribute(g_xDoc.allocate_attribute("ID", strStructID));
			nodeStructAppend->append_attribute(g_xDoc.allocate_attribute("Type", "STRUCT"));

			if(g_ucDebugMode)
			{
				printf("%s create node name = %s\n",__func__,strStructName);
			}

			root->append_node(nodeStructAppend);
		}

	}
	//
	else if ((!strStructName) && (strcmp("STRUCT",strType) != 0))
	{
		root->append_node(nodeP);
	}
#endif
    g_iPrameterIndex++;

	if(g_ucDebugMode)
	{
		printf("g_iPrameterIndex = %d\n", g_iPrameterIndex);
	}
}


////////////////////////////////////////////////////////////////////////////////
/// \fn sil_xml_write_param
/// \brief  Write to XML file relevant data regarding the parameter
///	\param uiRowIdx 	- Row index of the parameter
/// \	   uiRowIdx 	- Column index of the parameter
/// \ 	   ucslDataID 	- built-in data types .reference : builtin_typeid_types.h BuiltInDTypeId
/// \ 	   strType 		- string of the type to be written in the XML
/// \ 	   dValue 		- Value of the parameter
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_xml_read_param(unsigned int uiParamID,double &dValue,char* ParamterName)
{
    sprintf(g_buff,"%d",uiParamID);
    g_UserParams.Read(ParamterName,g_buff,g_strValue,dValue,0);

    return;
}

void sil_xml_read_param_ex(unsigned int uiParamID, double &dValue, char* ParamterName)
{
	//xml_node<>* parameter_node;
	//char strNodeID[MAX_STR_SIZE];
	char strNodeID[256];
	sprintf(strNodeID, "%d", uiParamID);

	//sprintf(g_buff,"%d",uiParamID);
    //g_UserParams.Read(ParamterName,g_buff,g_strValue,dValue,0);
	//parameter_node = (xml_node<>*)sil_xml_search_node(root, ParamterName);

//	parameter_node = (xml_node<>*)sil_xml_search_nodeID(root, ParamterName, strNodeID);
//	if(parameter_node != NULL)
//	{
//		const rapidxml::xml_attribute<> *type_attribute = parameter_node->first_attribute("Type");
//
//		if ( (type_attribute) && (strncmp(type_attribute->value(), "STRUCT",sizeof("STRUCT")) == 0) )
//		{
//			dValue = 0;
//			return;
//		}
//
//		const rapidxml::xml_attribute<> *name_attribute = parameter_node->first_attribute("Value");
//		strncpy(g_buff, name_attribute->value(), name_attribute->value_size());
//
//		printf("\33[0;34m name_attribute->value[%d] = %s, \33[0m", uiParamID, name_attribute->value());
//		printf("\33[0;34m name_attribute->value_size[%d] = %d, \33[0m", uiParamID, name_attribute->value_size());
//		printf("\33[0;34m g_buff[%d] = %s \33[0m\n", uiParamID, g_buff);
////	printf("\33[0;34m parent_name[%d] = %s, \33[0m", uiParamID, (char*)parameter_node->parent()->last_attribute("NAME"));
//
//		dValue = atof(g_buff);
//		memset(g_buff, 0, name_attribute->value_size());
//	}

	   FILE *fp;
	   char strFromXML[256];
	   char* tmp;
	   const char delim[2] = "\"";

	   sprintf(strNodeID, "PARAMETER NAME=\"%s\" ID=\"%d\"", ParamterName, uiParamID);

	   fp = fopen(SIMULINK_DEF_XML_FULL_PATH, "r");
	   if(fp == NULL)
	   {
	      perror("Error opening file");
	      return;
	   }

	   dValue = 0;

		while(fgets(strFromXML, 256, fp) != NULL)
		{
			if(strstr(strFromXML, strNodeID) != NULL && strstr(strFromXML, "Type=\"STRUCT\"") == NULL)
			{
//				printf("\n\033[1m\033[32m %s \33[0m\n", strFromXML);

				memcpy(strFromXML, strstr(strFromXML, "Value=\"") + 7, 70);

				tmp = strtok(strFromXML, delim);
//				printf("sil_xml_read_param_ex:  tmp = %s,  ", tmp);
				
				dValue = atof(tmp);
//				printf("dValue = %.16g\n", dValue);

				break;
			}
		}

		if(fp)	//Close the file if still open
		{
			fclose(fp);
		}

    return;
}



void sil_xml_set_recording_data()
{

	xml_node<> *recording_data_node = g_xDoc.allocate_node(node_element, "RECORDING_DATA");
	xml_node<> *sil_long_node = g_xDoc.allocate_node(node_element, "SIL_LONG");
	xml_node<> *sil_double_node = g_xDoc.allocate_node(node_element, "SIL_DOUBLE");

	for(int i = 0; i < SIL_REC_MAX_SIG;i++)
	{
		xml_node<> *rec_param_node = g_xDoc.allocate_node(node_element, "REC_PARAM");
		sprintf(g_buff,"%d",i+1);
		rec_param_node->append_attribute(g_xDoc.allocate_attribute("ID", g_xDoc.allocate_string(g_buff)));

		rec_param_node->append_attribute(g_xDoc.allocate_attribute("Alias", /*g_xDoc.allocate_string(g_buff)*/SIL_LONG_ALIAS[i]));
		sil_long_node->append_node(rec_param_node);
	}

	recording_data_node->append_node(sil_long_node);

	for(int i = 0; i < SIL_REC_MAX_SIG;i++)
	{
		xml_node<> *rec_param_node = g_xDoc.allocate_node(node_element, "REC_PARAM");
		sprintf(g_buff,"%d",i+1);
		rec_param_node->append_attribute(g_xDoc.allocate_attribute("ID", g_xDoc.allocate_string(g_buff)));

		rec_param_node->append_attribute(g_xDoc.allocate_attribute("Alias", /*g_xDoc.allocate_string(g_buff)*/SIL_DOUBLE_ALIAS[i]));
		sil_double_node->append_node(rec_param_node);
	}

	recording_data_node->append_node(sil_double_node);

	root->append_node(recording_data_node);
}


