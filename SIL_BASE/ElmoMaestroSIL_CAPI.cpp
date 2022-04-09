/*
 * SIL_CAPI.cpp
 *
 *  Created on: 08/08/2017
 *      Author: zivb
 */

#define SIL_REC
#include <Simulink\ElmoMaestroSIL_StepsAndRecordings.h>
#include <ElmoMaestroSIL_CAPI.h>

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define	  RAPIDXML_NO_EXCEPTIONS
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

extern xml_document<>	g_xDoc;
extern xml_node<>* root;


CAPI_MODIFY_PARA_STRUCT *g_stModifyParamData;

SIL_CAPI_PARAM_STRUCT g_stSILModifyParamData[1200];
int  			g_icapiPrameterIndex = 0;
int  			g_iTempFatherStructIndex = 0;
extern rtwCAPI_ModelMappingInfo *g_MMI;
extern const rtwCAPI_ModelParameters* g_pstModelParams;	//pointer to ModelParameter Structure
extern const rtwCAPI_DataTypeMap*     g_pDataTypeMap;		//Pointer to Data type map.
extern const rtwCAPI_DimensionMap*    g_pDimMap;
extern const rtwCAPI_FixPtMap*        g_pFxpMap;
extern const rtwCAPI_ElementMap*  	  g_pElementMap;
extern void**                         g_ppDataAddrMap;
/* ============================================================================
 * Function: capi_ModifyParameter
 *
 * Abstract:
 *   This function modifies a parameter.
 *       _param      : Parameter address that needs to be modified
 *       _newParam   : Address of the new parameter value
 *       orientation : orientation of the parameter (scalar|vector|matrix)
 *       dims        : the dimensions vector (Num Rows    = dims[0],
 *                                            Num Columns = dims[1],
 *                                            Num Pages   = dims[2] ...)
 *       nDims       : the number of dimesnions (for scalar, vector and
 *                     2-D matrix, nDims=2. For n-D params nDims > 2)
 *       slDatatype  : the data type ID define in simstruc.h. can be one of
 *                     the enumerated value
 *                     SS_DOUBLE, SS_SINGLE, SS_INT8, SS_UINT8, SS_INT16,
 *                     SS_UINT16, SS_INT32, SS_UINT32, SS_BOOLEAN
 *       isComplex   : if the parameter is complex (isComplex = 1, if complex)
 */

int_T capi_ModifyParameter(void                 *_param,
                           void                 *_newParam,
                           rtwCAPI_Orientation  orientation,
                           uint_T               *dims,
                           uint8_T              nDims,
                           uint8_T              slDataType,
                           unsigned short       isComplex) {
#ifdef BUILTIN_TYPEID_TYPES
    uint_T rowIdx;
    uint_T colIdx;
    uint_T pageIdx;
//only execute if C API selected in the model

    /* Switch over the slDataType to assess data type of the parameter */
    switch (slDataType) {
      case SS_DOUBLE: {
          if(isComplex == 0) {
              /* Type cast address to real_T: Non-Complex and Double */
              real_T *param     = (real_T *) (_param);
              real_T *newParam  = (real_T *) (_newParam);
//              fprintf(stderr,"SS_DOUBLE _newParam = %lf",*((real_T *)newParam));
              /* Based on the orientation, assign new value to the parameter */
              switch( orientation ) {
                case rtwCAPI_SCALAR:

                  *param = *newParam;
                  break;

                case rtwCAPI_VECTOR:

                  /* Vector: NumRows = dims[0], NumColumns = 1 */
                  /* Loop through rows to assign new values    */
                  for(rowIdx = 0; rowIdx < dims[0]*dims[1]; rowIdx++) {
                      param[rowIdx] = newParam[rowIdx];
                  }
                  break;

                case rtwCAPI_MATRIX_COL_MAJOR:
                   /* Matrix: NumRows = dims[0], NumColumns = dims[1]       *
                   * Loop through rows and columns to assign new values.   *
                   * Note: RTW declares a matrix data as column-major      *
                   * A column-major matrix is one in which indexing        *
                   * is column-wise instead of row-wise. For e.g           *
                   * A = | 1  |   2    3 |
                         |    |          |
                         | 4  |   5    6 |
                         |    |          |
                         | 7 \|/  8    9 |

                     A[0]=1; A[1]=4; A[2]=7; A[3]=2; A[4]=5; and so on
                  */
                  for(rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                      for(colIdx = 0; colIdx < dims[1]; colIdx++) {
                          param[colIdx*dims[0] + rowIdx] =
                              newParam[colIdx*dims[0] + rowIdx];
                      }
                  }
                  break;

                case rtwCAPI_MATRIX_ROW_MAJOR:
                  /* Matrix: NumRows = dims[0], NumColumns = dims[1]       *
                   * Loop through rows and columns to assign new values    *
                   * Note: RTW does not declare a matrix data as row       *
                   * major.                                                *
                   * A = | 1     2    3 |
                         |------------> |
                         | 4     5    6 |
                         |              |
                         | 7     8    9 |

                     A[0]=1; A[1]=2; A[2]=3; A[3]=4; A[4]=5; and so on
                  */
                  for(rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                      for(colIdx = 0; colIdx < dims[1]; colIdx++) {
                          param[rowIdx*dims[1] + colIdx] =
                              newParam[rowIdx*dims[1] + colIdx];
                      }
                  }
                  break;

                case rtwCAPI_MATRIX_COL_MAJOR_ND:
                  /* N-Dimensional Parameters                             *
                   * RTW always declares N-D Parameters as column-indexed */
                  if (nDims == 3) {
                      /* only handle 3-D parameters */
                      for (pageIdx = 0; pageIdx < dims[2]; pageIdx++) {
                          for (rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                              for (colIdx = 0; colIdx < dims[1]; colIdx++) {
                                  uint_T idx = rowIdx + dims[0] * colIdx +
                                    dims[0] * dims[1] * pageIdx;
                                  param[idx] = newParam[idx];
                              }
                          }
                      }
                  }
                  break;

                default:
                  return(0);
              } /* end switch(orientation) */
          } /* end if(isComplex == 0) */
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
          else {
              /* Type cast address to creal_T: Complex and Double */
              creal_T *param    = (creal_T *) (_param);
              creal_T *newParam = (creal_T *) (_newParam);
              /* Based on the orientation, assign new value to the parameter */
              switch( orientation ) {
                case rtwCAPI_SCALAR:
                  /* Assign real and complex values */
                  param->re = newParam->re;
                  param->im = newParam->im;
                  break;

                case rtwCAPI_VECTOR:

                  /* Vector: NumRows = dims[0], NumColumns = 1 */
                  /* Loop through rows to assign new values    */
                  for(rowIdx = 0; rowIdx < dims[0]*dims[1]; rowIdx++) {
                      param[rowIdx].re = newParam[rowIdx].re;
                      param[rowIdx].im = newParam[rowIdx].im;
                  }
                  break;

                case rtwCAPI_MATRIX_COL_MAJOR:
                	fprintf(stderr,"%s rtwCAPI_MATRIX_COL_MAJOR\n",__func__);
                  /* Matrix: NumRows = dims[0], NumColumns = dims[1]       *
                   * Loop through rows and columns to assign new values.   *
                   * Note: MATLAB declares a matrix data as column-major   *
                   * major. A column-major matrix is one in which indexing *
                   * is column-wise instead of row-wise. For e.g           *
                   * A = | 1  |   2    3 |
                         |    |          |
                         | 4  |   5    6 |
                         |    |          |
                         | 7 \|/  8    9 |

                     A[0]=1; A[1]=4; A[2]=7; A[3]=2; A[4]=5; and so on
                  */
                  for(rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                      for(colIdx = 0; colIdx < dims[1]; colIdx++) {
                        param[colIdx*dims[0] + rowIdx].re = \
                            newParam[colIdx*dims[0] + rowIdx].re;
                        param[colIdx*dims[0] + rowIdx].im = \
                            newParam[colIdx*dims[0] + rowIdx].im;
                      }
                  }
                  break;

                case rtwCAPI_MATRIX_ROW_MAJOR:
                	fprintf(stderr,"%s rtwCAPI_MATRIX_ROW_MAJOR\n",__func__);
                  /* Matrix: NumRows = dims[0], NumColumns = dims[1]       *
                   * Loop through rows and columns to assign new values    *
                   * Note: MATLAB does not declare a matrix data as row    *
                   * major.                                                *
                   * A = | 1     2    3 |
                         |------------> |
                         | 4     5    6 |
                         |              |
                         | 7     8    9 |

                     A[0]=1; A[1]=2; A[2]=3; A[3]=4; A[4]=5; and so on
                  */

                  for(rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                      for(colIdx = 0; colIdx < dims[1]; colIdx++) {
                          param[rowIdx*dims[1] + colIdx].re = \
                              newParam[rowIdx*dims[1] + colIdx].re;
                          param[rowIdx*dims[1] + colIdx].im = \
                              newParam[rowIdx*dims[1] + colIdx].im;
                      }
                  }
                  break;

                case rtwCAPI_MATRIX_COL_MAJOR_ND:
                	fprintf(stderr,"%s rtwCAPI_MATRIX_COL_MAJOR_ND\n",__func__);
                  /* N-Dimensional Parameters                             *
                   * RTW always declares N-D Parameters as column-indexed */
                  if (nDims == 3) {
                      /* only handle 3-D parameters */
                      for (pageIdx = 0; pageIdx < dims[2]; pageIdx++) {
                          for (rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                              for (colIdx = 0; colIdx < dims[1]; colIdx++) {
                                  uint_T idx = rowIdx + dims[0] * colIdx +
                                      dims[0] * dims[1] * pageIdx;
                                  param[idx].re = newParam[idx].re;
                                  param[idx].im = newParam[idx].im;
                              }
                          }
                      }
                  }
                  break;

                default:
                  return(0);
              } /* end switch(orientation) */
          } /* end else */
#endif
      } /* end case SS_DOUBLE*/
        break;

      case SS_INT32:  {
          /* Type cast address to int32_T: signed 32 bit integer     */
          /* Note: You could check for complexity here by using      *
           * isComplex == 1 (as done for case SS_DOUBLE).            */

          int32_T *param    = (int32_T *) (_param);
          int32_T *newParam = (int32_T *) (_newParam); /* param's new value */

          switch( orientation ) {
            case rtwCAPI_SCALAR:
              *param = *newParam;
              break;

            case rtwCAPI_VECTOR:
              for(rowIdx = 0; rowIdx < dims[0]*dims[1]; rowIdx++) {
                  param[rowIdx] = newParam[rowIdx];
              }
              break;

            case rtwCAPI_MATRIX_ROW_MAJOR:
              for(rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                  for(colIdx = 0; colIdx < dims[1]; colIdx++) {
                      param[rowIdx*dims[1] + colIdx] = \
                          newParam[rowIdx*dims[1] + colIdx];
                  }
              }
              break;

            case rtwCAPI_MATRIX_COL_MAJOR:
              for(rowIdx = 0; rowIdx < dims[0]; rowIdx++) {
                  for(colIdx = 0; colIdx < dims[1]; colIdx++) {
                      param[colIdx*dims[0] + rowIdx] = \
                          newParam[colIdx*dims[0] + rowIdx];
                  }
              }
              break;

            default:
              return(0);
          } /* end switch(orientation) */
      } /* end case SS_INT32 */
        break;

      default:
        /* You can extend the switch-case to other Simulink DataIds
         * case SS_SINGLE:
         * case SS_UINT32:
         * case SS_INT16:
         * case SS_UINT16:
         * case SS_INT8:
         * case SS_UINT8:
         * case SS_BOOLEAN
         *
         * This function skips other data types. */
        printf("Cannot handle parameter data type. \n");
        return(0);
    }
#endif
    return(1);
}





/* Function capi_PrintParameter ============================================
 * Abstract:
 *   Prints the name and value of the parameter.
 *   Scalar, Vector and matrix parameters are supported.
 *   N-Dimensional paramaters, where N > 2, are not supported.
 *
 * Note: The printing format assumes that the parameter is COLUMN-MAJOR
 *       indexing.
 */
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
                         const int 		iStructIndex
                         )
{
#ifdef BUILTIN_TYPEID_TYPES
    uint_T numRows  = actualDims[0];
    uint_T numCols  = actualDims[1];
    uint_T numPages = 0;
    uint_T rowIdx;
    uint_T colIdx;
    uint_T pageIdx;
    if (numDims == 3) numPages = actualDims[2];

    if(g_ucDebugMode)
    {
    	printf("%s %s  = ",__func__ ,paramName);
    }

    switch(slDataID) {
      case SS_DOUBLE :
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
        if (isComplex)
        {
            /* If is Complex and SS_DOUBLE, the data is of type creal_T */
            creal_T* paramVal = (creal_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                    printf("\t%.4lf + %.4lf*i",paramVal[colIdx*numRows + rowIdx].re,paramVal[colIdx*numRows + rowIdx].im);

//                    sil_xml_write_param(rowIdx,numCols,slDataID,"DOUBLE",paramVal[colIdx*numRows + rowIdx].im + paramVal[colIdx*numRows + rowIdx].re);
                }
                putchar('\n');
            }
        }
        else
#endif
        {
            real_T* paramVal = (real_T *) paramAddress;
            if (numDims == 3) {
                for (pageIdx = 0; pageIdx < numPages; pageIdx++) {
                    printf("ans(:,:,%d) =\n", pageIdx+1);
                    for (rowIdx = 0; rowIdx < numRows; rowIdx++) {
                        for (colIdx = 0; colIdx < numCols; colIdx++) {
                            uint_T idx = rowIdx + numRows * colIdx +
                                numRows * numCols * pageIdx;
                            printf("\t%.4g", paramVal[idx]);

                           	if(numRows == 1)
                           	{
                           		if(numCols == 1)
                           		{
                                    sil_capi_save_param(-1,-1,slDataID,"DOUBLE",paramVal[idx],(unsigned int)&paramVal[idx],paramName,paramStructName,iStructIndex);
                           		}
                           		else
                           		{
                                    sil_capi_save_param(-1,colIdx,slDataID,"DOUBLE",paramVal[idx],(unsigned int)&paramVal[idx],paramName,paramStructName,iStructIndex);
                           		}
                           	}
                           	else
                           	{
                           		sil_capi_save_param(rowIdx,colIdx,slDataID,"DOUBLE",paramVal[idx],(unsigned int)&paramVal[idx],paramName,paramStructName,iStructIndex);
                           	}

//                            sil_xml_write_param(rowIdx,numCols,slDataID,"DOUBLE",paramVal[idx]);
                        }
                        putchar('\n');
                    }
                    putchar('\n');
                }
            } else {
                for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                    for(colIdx = 0; colIdx < numCols; colIdx++)
                    {
                    	if(g_ucDebugMode)
                    	{
                    		printf("\t%.4g", paramVal[colIdx*numRows + rowIdx]);
                    	}

                       	if(numRows == 1)
                       	{
                       		if(numCols == 1)
                       		{
                                sil_capi_save_param(-1,-1,slDataID,"DOUBLE",paramVal[colIdx*numRows + rowIdx],(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                       		}
                       		else
                       		{
                                sil_capi_save_param(-1,colIdx,slDataID,"DOUBLE",paramVal[colIdx*numRows + rowIdx],(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                       		}
                       	}
                       	else
                       	{
                       		sil_capi_save_param(rowIdx,colIdx,slDataID,"DOUBLE",paramVal[colIdx*numRows + rowIdx],(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                       	}

//                        sil_xml_write_param(rowIdx,numCols,slDataID,"DOUBLE",paramVal[colIdx*numRows + rowIdx]);

                    }
                    putchar('\n');
                }
            }
        }
        break;
      case SS_INT32 :
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
        if (isComplex)
        {
            cint32_T* paramVal = (cint32_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++)
                {
                    printf("\ttype = SS_INT32\tvalue = %d + %d*i",
                           paramVal[colIdx*numRows + rowIdx].re,
                           paramVal[colIdx*numRows + rowIdx].im);

//                    sil_xml_write_param(rowIdx,numCols,slDataID,"INT32",(paramVal[colIdx*numRows + rowIdx].re +   paramVal[colIdx*numRows + rowIdx].im));
                }
                putchar('\n');
            }
        } else
#endif
        {
            int32_T* paramVal = (int32_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                    int32_T intVal  = paramVal[colIdx*numRows + rowIdx];
                    /* Real world value = (slope * stored integer) + bias */
                    real_T  realVal = slope*(intVal) + bias;

//                    printf("\ttype = SS_INT32\tvalue =  %.4g", realVal);
                    if(g_ucDebugMode)
                    {
                    	printf("\ttype = SS_INT32\tvalue =  %d", intVal);
                    }

                   	if(numRows == 1)
                   	{
                   		if(numCols == 1)
                   		{
                            sil_capi_save_param(-1,-1,slDataID,"INT32",intVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   		else
                   		{
                            sil_capi_save_param(-1,colIdx,slDataID,"INT32",intVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   	}
                   	else
                   	{
                   		sil_capi_save_param(rowIdx,colIdx,slDataID,"INT32",intVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   	}
//                    sil_xml_write_param(rowIdx,numCols,slDataID,"INT32",realVal);
                }
                putchar('\n');
            }
        }
        break;
      case SS_UINT32 :
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
        if (isComplex)
        {
        	cuint32_T* paramVal = (cuint32_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                    printf("\ttype = SS_UINT32\tvalue = %d + %d*i",
                           paramVal[colIdx*numRows + rowIdx].re,
                           paramVal[colIdx*numRows + rowIdx].im);
//                    sil_xml_write_param(rowIdx,numCols,slDataID,"UINT32",(paramVal[colIdx*numRows + rowIdx].re + paramVal[colIdx*numRows + rowIdx].im));
                }
                putchar('\n');
            }
        } else
#endif
        {
            uint32_T* paramVal = (uint32_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                    uint32_T intVal  = paramVal[colIdx*numRows + rowIdx];
                    /* Real world value = (slope * stored integer) + bias */
                    real_T  realVal = slope*(intVal) + bias;
                    if(g_ucDebugMode)
                    {
                    	printf("\ttype = SS_INT32\tvalue =  %.4g", realVal);
                    }

                   	if(numRows == 1)
                   	{
                   		if(numCols == 1)
                   		{
                            sil_capi_save_param(-1,-1,slDataID,"UINT32",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   		else
                   		{
                            sil_capi_save_param(-1,colIdx,slDataID,"UINT32",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   	}
                   	else
                   	{
                   		sil_capi_save_param(rowIdx,colIdx,slDataID,"UINT32",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   	}
//                    sil_xml_write_param(rowIdx,numCols,slDataID,"UINT32",realVal);
                }
                putchar('\n');
            }
        }
        break;
        //
      case SS_INT16 :
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
        if (isComplex) {
        	cint16_T* paramVal = (cint16_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                    printf("\ttype = SS_INT16\tvalue = %d + %d*i",
                           paramVal[colIdx*numRows + rowIdx].re,
                           paramVal[colIdx*numRows + rowIdx].im);
//                    sil_xml_write_param(rowIdx,numCols,slDataID,"INT16",(paramVal[colIdx*numRows + rowIdx].re + paramVal[colIdx*numRows + rowIdx].im));
                }
                putchar('\n');
            }
        } else
#endif
        {
        	int16_T* paramVal = (int16_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                	int16_T intVal  = paramVal[colIdx*numRows + rowIdx];
                    /* Real world value = (slope * stored integer) + bias */
                    real_T  realVal = slope*(intVal) + bias;
                    if(g_ucDebugMode)
                    {
                    	printf("\ttype = SS_INT16\tvalue =  %.4g", realVal);
                    }

                   	if(numRows == 1)
                   	{
                   		if(numCols == 1)
                   		{
                            sil_capi_save_param(-1,-1,slDataID,"INT16",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   		else
                   		{
                            sil_capi_save_param(-1,colIdx,slDataID,"INT16",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   	}
                   	else
                   	{
                   		sil_capi_save_param(rowIdx,colIdx,slDataID,"INT16",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   	}
//                    sil_xml_write_param(rowIdx,numCols,slDataID,"INT16",realVal);
                   }
                putchar('\n');
            }
        }
        break;
        //
      case SS_UINT16 :
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
        if (isComplex) {
        	cuint8_T* paramVal = (cuint8_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                    printf("\ttype = SS_UINT16\tvalue = %d + %d*i",
                           paramVal[colIdx*numRows + rowIdx].re,
                           paramVal[colIdx*numRows + rowIdx].im);
                    sil_xml_write_param(rowIdx,colIdx,slDataID,"UINT16",(paramVal[colIdx*numRows + rowIdx].re + paramVal[colIdx*numRows + rowIdx].im));
                }
                putchar('\n');
            }
        } else
#endif
        {
        	uint16_T* paramVal = (uint16_T *) paramAddress;
            for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                for(colIdx = 0; colIdx < numCols; colIdx++) {
                	uint16_T intVal  = paramVal[colIdx*numRows + rowIdx];
                    /* Real world value = (slope * stored integer) + bias */
                    real_T  realVal = slope*(intVal) + bias;
                    if(g_ucDebugMode)
                    {
                    	printf("\ttype = SS_INT16\tvalue =  %.4g", realVal);
                    }

                   	if(numRows == 1)
                   	{
                   		if(numCols == 1)
                   		{
                            sil_capi_save_param(-1,-1,slDataID,"UINT16",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   		else
                   		{
                            sil_capi_save_param(-1,colIdx,slDataID,"UINT16",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   		}
                   	}
                   	else
                   	{
                   		sil_capi_save_param(rowIdx,colIdx,slDataID,"UINT16",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                   	}
//                    sil_xml_write_param(rowIdx,numCols,slDataID,"UINT16",realVal);
                 }
                putchar('\n');
            }
        }
        break;
        //
      case SS_INT8 :
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
         if (isComplex) {
        	 cint8_T* paramVal = (cint8_T *) paramAddress;
             for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                 for(colIdx = 0; colIdx < numCols; colIdx++) {
                     printf("\ttype = SS_INT8\tvalue = %d + %d*i",
                            paramVal[colIdx*numRows + rowIdx].re,
                            paramVal[colIdx*numRows + rowIdx].im);
//                     sil_xml_write_param(rowIdx,numCols,slDataID,"INT8",(paramVal[colIdx*numRows + rowIdx].re + paramVal[colIdx*numRows + rowIdx].im));
                      }
                 putchar('\n');
             }
         } else
#endif
         {
        	 int8_T* paramVal = (int8_T *) paramAddress;
             for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                 for(colIdx = 0; colIdx < numCols; colIdx++) {
                	 int8_T intVal  = paramVal[colIdx*numRows + rowIdx];
                     /* Real world value = (slope * stored integer) + bias */
                     real_T  realVal = slope*(intVal) + bias;
                     if(g_ucDebugMode)
                     {
                    	 printf("\ttype = SS_INT8\tvalue =  %.4g", realVal);
                     }

                    	if(numRows == 1)
                    	{
                    		if(numCols == 1)
                    		{
                             sil_capi_save_param(-1,-1,slDataID,"INT8",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                    		}
                    		else
                    		{
                             sil_capi_save_param(-1,colIdx,slDataID,"INT8",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                    		}
                    	}
                    	else
                    	{
                    		sil_capi_save_param(rowIdx,colIdx,slDataID,"INT8",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                    	}
//                     sil_xml_write_param(rowIdx,numCols,slDataID,"INT8",realVal);
                 }
                 putchar('\n');
             }
         }
         break;
         //
      case SS_UINT8 :
//
// MATLAB Automatic CODE Generator will use this DEF is teh COMPLEX Numbers Support
// Was set in the Project Config Options. The definition is Automatic and in the "rtwtypes.h"
// 		#define CREAL_T
// So, All the support for Complex Numebsr is included in teh CODE ONLY IFDEF ...
//
#ifdef	CREAL_T
         if (isComplex) {
        	 cuint8_T* paramVal = (cuint8_T *) paramAddress;
             for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                 for(colIdx = 0; colIdx < numCols; colIdx++) {
                     printf("\ttype = SS_UINT8\tvalue = %d + %d*i",
                            paramVal[colIdx*numRows + rowIdx].re,
                            paramVal[colIdx*numRows + rowIdx].im);

//                     sil_xml_write_param(rowIdx,numCols,slDataID,"UINT8",(paramVal[colIdx*numRows + rowIdx].re + paramVal[colIdx*numRows + rowIdx].im));
                 }
                 putchar('\n');
             }
         } else
#endif
         {
        	 uint8_T* paramVal = (uint8_T *) paramAddress;
             for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
                 for(colIdx = 0; colIdx < numCols; colIdx++) {
                	 uint8_T intVal  = paramVal[colIdx*numRows + rowIdx];
                     /* Real world value = (slope * stored integer) + bias */
                     real_T  realVal = slope*(intVal) + bias;
                     if(g_ucDebugMode)
                     {
                    	 printf("\ttype = SS_UINT8\tvalue =  %.4g", realVal);
                     }

                    	if(numRows == 1)
                    	{
                    		if(numCols == 1)
                    		{
                             sil_capi_save_param(-1,-1,slDataID,"UINT8",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                    		}
                    		else
                    		{
                             sil_capi_save_param(-1,colIdx,slDataID,"UINT8",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                    		}
                    	}
                    	else
                    	{
                    		sil_capi_save_param(rowIdx,colIdx,slDataID,"UINT8",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                    	}
//                     sil_xml_write_param(rowIdx,numCols,slDataID,"UINT8",realVal);
                 }
                 putchar('\n');
             }
         }
         break;
         //
      case SS_BOOLEAN :
      {
    	  int8_T* paramVal = (int8_T *) paramAddress;
    	  for(rowIdx = 0; rowIdx < numRows; rowIdx++) {
    		  for(colIdx = 0; colIdx < numCols; colIdx++) {
    			  int8_T intVal  = paramVal[colIdx*numRows + rowIdx];
    			  /* Real world value = (slope * stored integer) + bias */
    			  real_T  realVal = slope*(intVal) + bias;
    			  if(g_ucDebugMode)
    			  {
    				  printf("\ttype = BOOLEAN\tvalue =  %.4g", realVal);
    			  }

                 	if(numRows == 1)
                 	{
                 		if(numCols == 1)
                 		{
                          sil_capi_save_param(-1,-1,slDataID,"BOOLEAN",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                 		}
                 		else
                 		{
                          sil_capi_save_param(-1,colIdx,slDataID,"BOOLEAN",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                 		}
                 	}
                 	else
                 	{
                 		sil_capi_save_param(rowIdx,colIdx,slDataID,"BOOLEAN",realVal,(unsigned int)&paramVal[colIdx*numRows + rowIdx],paramName,paramStructName,iStructIndex);
                 	}
    			  //                     sil_xml_write_param(rowIdx,numCols,slDataID,"BOOLEAN",realVal);
    		  }
    		  putchar('\n');
    	  }
      }
      break;
      //
      case SS_STRUCT :
      {

      }
      break;
      default :
        /* You can extend the switch-case to other Simulink DataIds
         * case SS_SINGLE:
         *
         * This function skips other data types. */
        printf("Unhandled Data Type: %s is skipped slDataID = %d\n", paramName,slDataID);
        break;
    }
#endif
    return;
}

/* Function sli_capi_LoadModelParameter ======================================= */
/* Abstract:
 *   This function takes in ModelMappingInfo structure prints the value of
 *   ModelParameter - rtModelParameters[paramIdx] to STDOUT
 *     capiMap  - Pointer to the ModelMappingInfo structure in Real-Time model.
 *                Map of all C-API arrays and structures.
 *     paramIdx - Index of the Parameter in the rtModelParameters array
 *                generated by RTW in <MODEL>_capi.c
 */
void sil_capi_LoadModelParameter(rtwCAPI_ModelMappingInfo* capiMap,
                              uint_T                    paramIdx) {

    const 	uint_T*     dimArray;
    const  	char_T* 	paramName;
    const 	char_T* 	TempparamName = NULL;
    uint_T         		addrIdx;
    uint16_T      		dataTypeIdx;
    uint16_T     		dimIndex;
    uint16_T     		fxpMapIdx;
    uint8_T       		slDataID;


    uint16_T						 usNumOfElement;
    uint16_T      					 usElemMapIndex;

    void*                paramAddress;
    void*                pInnerparamAddress;
    const char_T*        cDataName;
    unsigned short       isComplex;
    uint8_T              numDims;
    uint_T               dimArrayIdx;
    rtwCAPI_Orientation  orientation;
    uint_T*              actualDimensions;

    int    idx;
    real_T slope = 1.0;
    real_T bias  = 0.0;
    char strName[1024];
//    unsigned short modParamFlag = 1;

    /* Assert the parameter index is less than total number of parameters */
    assert(paramIdx < rtwCAPI_GetNumModelParameters(capiMap));

    /* Get Parameter Name */
    paramName   = rtwCAPI_GetModelParameterName(g_pstModelParams, paramIdx);

//    printf ("***%s: paramName = %s ***\n",__func__,paramName);
    /* Get Data Type attributes of the Model Parameter                       */
    /* Accessing the data type information from capiMap is a 3 step process  *
     * 1) Get the dataTypeMap Array from capiMap.                            *
     * 2) Get the index into the above array, dataTypeIdx, from ModelParameter*
     *    structure. The index will point to a rtwCAPI_DataTypeMap structure.*
     * 3) From the structure, get the member values of the structure, namely *
     *       o Simulink Data ID, can be one of the enumerated value          *
     *           SS_DOUBLE, SS_SINGLE, SS_INT8, SS_UINT8, SS_INT16, SS_UINT16*
     *            , SS_INT32, SS_UINT32, SS_BOOLEAN                          *
     *       o Complexity                                                    *
     *       o Data Size                                                     *
     *    For complete structure see matlabroot/rtw/c/src/rtw_capi.h         */

    dataTypeIdx 	= rtwCAPI_GetModelParameterDataTypeIdx(g_pstModelParams, paramIdx);
    slDataID    	= rtwCAPI_GetDataTypeSLId(g_pDataTypeMap, dataTypeIdx);
    cDataName   	= rtwCAPI_GetDataTypeCName(g_pDataTypeMap,dataTypeIdx);
    isComplex   	= rtwCAPI_GetDataIsComplex(g_pDataTypeMap,dataTypeIdx);

    //For type SS_STRUCT need to retrive number of element and elment index in map
//    usNumOfElement 	= rtwCAPI_GetDataTypeNumElements(g_pDataTypeMap,dataTypeIdx);
//    usElemMapIndex 	= rtwCAPI_GetDataTypeElemMapIndex(g_pDataTypeMap,dataTypeIdx);


    /* Get Dimensions of the Model Parameter                                 */
    /* Accessing the dimension from capiMap is a 4 step process              *
     * 1) Get the dimMap array, an array of CAPI_DimensionMap struct         *
     * 2) Get the dimArray array, an array of all the dimensions in the Map  *
     * 3) Get the index into the dimMap array (dimIdx) from ModelParameter   *
     *    structure. the index will point to rtwCAPI_Dimension structure     *
     * 4) From the rtwCAPI_Dimension structure, get the following information*
     *       o orientation (scalar | vector | matrix)                        *
     *       o Number of dimensions (numDims)                                *
     *       o index into the dimArray (dimArrayIdx)                         *
     * 5) Using numDims and the dimArrayIdx, get the actual dimensions from  *
     *    the dimArray                                                       *
     *       uint_T ActualDims[numDims] = {dimArray[dimArrayIdx],            *
     *                                     dimArray[dimArrayIdx+1],          *
     *				       ...                                   *
     *				       dimArray[dimArrayIdx+(numDims-1)]}    *
     *    For e.g, scalar and 2-D parameters will have numDims = 2, So       *
     *       uint_T ActualDims[2] = {dimArray[dimArrayIdx],                  *
     *                               dimArray[dimArrayIdx +1]}               */

    g_pDimMap   = rtwCAPI_GetDimensionMap(capiMap);
    dimArray = rtwCAPI_GetDimensionArray(capiMap);

    if ((g_pDimMap == NULL) || (dimArray == NULL)) return;

    dimIndex    = rtwCAPI_GetModelParameterDimensionIdx(g_pstModelParams, paramIdx);
    numDims     = rtwCAPI_GetNumDims(g_pDimMap, dimIndex);
    dimArrayIdx = rtwCAPI_GetDimArrayIndex(g_pDimMap, dimIndex);
    orientation = rtwCAPI_GetOrientation(g_pDimMap, dimIndex);

    actualDimensions = (uint_T *) malloc(numDims*sizeof(uint_T));

    for(idx=0; idx < numDims; idx++) {
        actualDimensions[idx] = dimArray[dimArrayIdx + idx];
    }

    /* Get fixed-point information of the parameter */
    g_pFxpMap = rtwCAPI_GetFixPtMap(capiMap);
    if (g_pFxpMap == NULL) return;

    fxpMapIdx = rtwCAPI_GetModelParameterFixPtIdx(g_pstModelParams, paramIdx);
    if(fxpMapIdx > 0) {
        /* Only Fixed-point parameters have fxpMapIdx > 0 */
        real_T fracslope = rtwCAPI_GetFxpFracSlope(g_pFxpMap,fxpMapIdx);
        int8_T expt      = rtwCAPI_GetFxpExponent(g_pFxpMap,fxpMapIdx);

        /* slope = fractional slope * 2^exponent
         * fractional slope is also called Slope Adjustment Factor.
         * Type "help fixdt" in MATLAB command window for more information
         * on fixed point data types
         */

        slope = fracslope*pow(2.0,expt);
        bias  = rtwCAPI_GetFxpBias(g_pFxpMap,fxpMapIdx);
    }

    /* Get the address to this parameter                                     */
    addrIdx     = rtwCAPI_GetModelParameterAddrIdx(g_pstModelParams,paramIdx);
    paramAddress = (void *) rtwCAPI_GetDataAddress(g_ppDataAddrMap,addrIdx);
    void* tParamAddress;
    tParamAddress = paramAddress;

    if (paramAddress == NULL) return;

    //TODO: Ziv temp
    if(slDataID == SS_STRUCT)
    {
        //For type SS_STRUCT need to retrive number of element and elment index in map
        usNumOfElement 	= rtwCAPI_GetDataTypeNumElements(g_pDataTypeMap,dataTypeIdx);

        usElemMapIndex 	= rtwCAPI_GetDataTypeElemMapIndex(g_pDataTypeMap,dataTypeIdx);
//        printf("%s : %d paramName = %s usNumOfElement = %d usElemMapIndex = %d\n",__func__,__LINE__,paramName,usNumOfElement,usElemMapIndex);
        if(g_ucDebugMode)
        {
        	printf("\33[0;31mSTRUCT:  paramName = %s  TempparamName = %s  Index = %d, StrFatherIndex = %d \33[0m\n", paramName, TempparamName, g_icapiPrameterIndex, g_iTempFatherStructIndex);
        }
    	sil_capi_save_param(0,0,99,"STRUCT",0,0,paramName,TempparamName,g_iTempFatherStructIndex);
    	g_iTempFatherStructIndex = g_icapiPrameterIndex - 1;
		TempparamName = (char *)paramName;

    	for(int i = /*1*/usElemMapIndex; i < usNumOfElement+usElemMapIndex; i++) // todo: should i start from 1?
    	{
    		paramName = rtwCAPI_GetElementName(g_pElementMap,i);
    		strcpy(strName,TempparamName);

    		dataTypeIdx = rtwCAPI_GetElementDataTypeIdx(g_pElementMap,i);
    		slDataID = rtwCAPI_GetDataTypeSLId(g_pDataTypeMap, dataTypeIdx);
    	    dimIndex    = rtwCAPI_GetElementDimensionIdx(g_pElementMap,i);

    	    numDims     = rtwCAPI_GetNumDims(g_pDimMap, dimIndex);
    	    dimArrayIdx = rtwCAPI_GetDimArrayIndex(g_pDimMap, dimIndex);
    	    orientation = rtwCAPI_GetOrientation(g_pDimMap, dimIndex);
    	    actualDimensions = (uint_T *) realloc(actualDimensions,numDims*sizeof(uint_T));
//    	    printf("%s : %d paramName = %s ElementNUm = %d numDims = %d\n",__func__,__LINE__,paramName,i,numDims);

    	    for(idx=0; idx < numDims; idx++) {
    	        actualDimensions[idx] = dimArray[dimArrayIdx + idx];
    	    }
//   	    printf("\33[0;31m%s : %d paramName = %s ID = %d numDims = %d numRows = %d numCols = %d \33[0m\n",__func__,__LINE__,paramName,i,numDims,actualDimensions[0],actualDimensions[1]);

//    	    paramAddress = (char*)paramAddress +  rtwCAPI_GetElementOffset(g_pElementMap,i);
    	    if(slDataID != SS_STRUCT)
    	    {
    	    	pInnerparamAddress = (char*)paramAddress +  rtwCAPI_GetElementOffset(g_pElementMap,i);
 //       	    printf("\33[0;34mparamName = %s ID = %d numDims = %d numRows = %d numCols = %d \33[0m\n",paramName,i,numDims,actualDimensions[0],actualDimensions[1]);
    	    	capi_PrintParameter(paramIdx,paramName, pInnerparamAddress, slDataID, isComplex,actualDimensions,
    	    			numDims, slope, bias,TempparamName, g_iTempFatherStructIndex);
    	    }
    	    else
    	    {
    	    	if(g_ucDebugMode)
    	    	{
    	    		printf("\33[0;34m   STRUCT:  paramName = %s  TempparamName = %s  Index = %d, StrFatherIndex = %d \33[0m\n", paramName, TempparamName, g_icapiPrameterIndex, g_iTempFatherStructIndex);
    	    	}
            	sil_capi_save_param(0,0,99,"STRUCT",0,0,paramName,TempparamName,g_iTempFatherStructIndex);

    	    	 tParamAddress = (char*)paramAddress +  rtwCAPI_GetElementOffset(g_pElementMap,i);
    	    	sil_capi_struct_handler(paramIdx, dataTypeIdx, dimArray, actualDimensions, /*paramAddress*/tParamAddress, paramName, g_icapiPrameterIndex - 1);
    	    }
    	}

    }
    else
    {
    /* Print the parameter value */
//    printf("\33[0;35mparamName = %s ID = %d numDims = %d numRows = %d numCols = %d \33[0m\n",paramName,g_icapiPrameterIndex,numDims,actualDimensions[0],actualDimensions[1]);
    capi_PrintParameter(paramIdx,paramName, paramAddress, slDataID, isComplex,
                        actualDimensions, numDims, slope, bias,TempparamName, g_iTempFatherStructIndex);
    }



    free(actualDimensions);
    return;
}


void sil_capi_struct_handler(uint16_T paramIdx,uint16_T dataTypeIdx,const uint_T* dimArray,uint_T* actualDimensions,   void* paramAddress,
		const char* structName, int iStructIndex)
{
    uint16_T usNumOfElement;
    uint16_T usElemMapIndex;
    const  	char_T* 	paramName;
//  char strName[1024];

    uint16_T     			dimIndex;
    unsigned short       	isComplex = 0;
    uint8_T              	numDims;
    uint_T               	dimArrayIdx;
    rtwCAPI_Orientation  	orientation;
    int idx;
    uint8_T       		slDataID;
    real_T slope = 1.0;
    real_T bias  = 0.0;

    void*                pInnerparamAddress;


    const char* pStructname;
    //get struct name
    pStructname   = rtwCAPI_GetElementName(g_pElementMap, dataTypeIdx);
    pStructname   = rtwCAPI_GetElementName(g_pElementMap, dataTypeIdx);

    usNumOfElement 	= rtwCAPI_GetDataTypeNumElements(g_pDataTypeMap,dataTypeIdx);
    usElemMapIndex 	= rtwCAPI_GetDataTypeElemMapIndex(g_pDataTypeMap,dataTypeIdx);

	for(int i = /*1*/usElemMapIndex; i < usNumOfElement + usElemMapIndex; i++)
	{
		char_T* 	TempparamName;
		TempparamName = (char *)structName;
		paramName = rtwCAPI_GetElementName(g_pElementMap,i);


		dataTypeIdx = rtwCAPI_GetElementDataTypeIdx(g_pElementMap,i);
		slDataID = rtwCAPI_GetDataTypeSLId(g_pDataTypeMap, dataTypeIdx);
	    dimIndex    = rtwCAPI_GetElementDimensionIdx(g_pElementMap,i);
	    numDims     = rtwCAPI_GetNumDims(g_pDimMap, dimIndex);
	    dimArrayIdx = rtwCAPI_GetDimArrayIndex(g_pDimMap, dimIndex);
	    orientation = rtwCAPI_GetOrientation(g_pDimMap, dimIndex);

//	    printf("%s : %d  paramName = %s TempparamName = %s slDataID= %d\n",__func__,__LINE__,paramName,TempparamName,slDataID);
	    actualDimensions = (uint_T *) realloc(actualDimensions,numDims*sizeof(uint_T));
	    for(idx=0; idx < numDims; idx++)
	    {
	        actualDimensions[idx] = dimArray[dimArrayIdx + idx];
	    }

//	    paramAddress = (char*)paramAddress +  rtwCAPI_GetElementOffset(g_pElementMap,i);

	    if(slDataID != SS_STRUCT)
	    {
	    	pInnerparamAddress = (char*)paramAddress +  rtwCAPI_GetElementOffset(g_pElementMap,i);
	    	capi_PrintParameter(1,paramName, pInnerparamAddress, slDataID, isComplex,
	                           	   actualDimensions, numDims, slope, bias,TempparamName, iStructIndex);
	    }
	    else
	    {
	    	if(g_ucDebugMode)
	    	{
	    		printf("\33[0;32m      STRUCT:  paramName = %s  TempparamName = %s  Index = %d, StrFatherIndex = %d \33[0m\n", paramName, TempparamName, g_icapiPrameterIndex, iStructIndex);
	    	}
        	sil_capi_save_param(0,0,99,"STRUCT",0,0,/*structName*/paramName,/*NULL*/TempparamName,iStructIndex);
	    	 void* tParamAddress;
	    	 tParamAddress =   (char*)paramAddress +  rtwCAPI_GetElementOffset(g_pElementMap,i);
	    	sil_capi_struct_handler(paramIdx,dataTypeIdx,dimArray, actualDimensions, /*paramAddress*/tParamAddress, /*structName*/paramName, g_icapiPrameterIndex - 1);
	    }

	}
	return;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_save_param
/// \brief  Save to our global parameter structure the relevant information regarding tunable parameter.
///	\param iRowIdx 		- Row index of the parameter
/// \	   iRowIdx 		- Column index of the parameter
/// \ 	   ucslDataID 	- built-in data types .reference : builtin_typeid_types.h BuiltInDTypeId
/// \ 	   strType 		- string of the type will be written in the XML
/// \ 	   dValue 		- Value of the parameter
/// \ 	   uiParamADD 	- Parameter address
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_capi_save_param(int iRowIdx,int iColIdx,unsigned char ucslDataID,char *strType,double dValue,unsigned int uiParamADD,const char *strName,const char *strStructName, const int iStructIndex)
{

//	printf("\n*** %s strName %s strStructName %s dValue = %lf\n",__func__,strName,strStructName,dValue);
    g_stSILModifyParamData[g_icapiPrameterIndex].uclDataType = ucslDataID;
    g_stSILModifyParamData[g_icapiPrameterIndex].pdTunableparamADD = uiParamADD;
    g_stSILModifyParamData[g_icapiPrameterIndex].iRowIdx = iRowIdx;
    g_stSILModifyParamData[g_icapiPrameterIndex].iColIdx = iColIdx;
    g_stSILModifyParamData[g_icapiPrameterIndex].dValue = dValue;
    strncpy(g_stSILModifyParamData[g_icapiPrameterIndex].strType,strType,100 * sizeof(char));

	if(strName != NULL)
	{
	    strncpy(g_stSILModifyParamData[g_icapiPrameterIndex].strParamterName,strName,strlen(strName));
	    g_stSILModifyParamData[g_icapiPrameterIndex].strParamterName[strlen(strName)] = '\0';
	}
	if(strStructName != NULL)
	{
	    strncpy(g_stSILModifyParamData[g_icapiPrameterIndex].strStructParamterName,strStructName,strlen(strStructName));
	    g_stSILModifyParamData[g_icapiPrameterIndex].strStructParamterName[strlen(strStructName)] = '\0';
	    g_stSILModifyParamData[g_icapiPrameterIndex].iStructParamterIndex = iStructIndex;
	}

    g_icapiPrameterIndex++;
	return;
}

void sil_capi_delete_param(void)
{
//	for(int i = 0; i< g_icapiPrameterIndex; i++)
//	{
//		g_stSILModifyParamData += g_icapiPrameterIndex;
//		delete[] g_stSILModifyParamData;
//	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_create_xml
/// \brief  Create Simulink.XML file.
///	\param
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_capi_create_xml()
{
//	sil_xml_init_header();

	sil_xml_init_header_ex();

//	sil_xml_set_version_data();

	sil_xml_set_version_data_ex();

	//TODO: add recording section
	sil_xml_set_recording_data();
	if(g_ucDebugMode)
	{
		printf("\33[0;34m%s() g_icapiPrameterIndex = %d \33[0m\n\n", __func__, g_icapiPrameterIndex);
	}
#ifdef BUILTIN_TYPEID_TYPES
	char strStructID[MAX_STR_SIZE];
	for(int i = 0; i<g_icapiPrameterIndex; i++)
	{
		sil_xml_set_param_name(g_stSILModifyParamData[i].strParamterName);

//		sil_xml_write_param(g_stSILModifyParamData[i].uiRowIdx,
//				 g_stSILModifyParamData[i].uiColIdx,
		sprintf(strStructID, "%d", g_stSILModifyParamData[i].iStructParamterIndex);
		strStructID[strlen(strStructID)] = '\0';
//				 g_stSILModifyParamData[i].dValue);

		sil_xml_write_param_ex(g_stSILModifyParamData[i].iRowIdx,
				 g_stSILModifyParamData[i].iColIdx,
				 g_stSILModifyParamData[i].uclDataType,
				 g_stSILModifyParamData[i].strType,
				 g_stSILModifyParamData[i].dValue,
				 g_stSILModifyParamData[i].strStructParamterName,
				 strStructID);
	}
#endif
//	sil_xml_save();

	sil_xml_save_ex();

//	sleep(20);

	return;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_create_xml
/// \brief  Create Simulink.XML file.
///	\param
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_capi_create_xml_old()
{
	sil_xml_init_header();



	sil_xml_set_version_data();



	//TODO: add recording section
//	sil_xml_set_recording_data();

#ifdef BUILTIN_TYPEID_TYPES
	for(int i = 0; i<g_icapiPrameterIndex; i++)
	{
		sil_xml_set_param_name(g_stSILModifyParamData[i].strParamterName);

		sil_xml_write_param(g_stSILModifyParamData[i].iRowIdx,
				 g_stSILModifyParamData[i].iColIdx,
				 g_stSILModifyParamData[i].uclDataType,
				 g_stSILModifyParamData[i].strType,
				 g_stSILModifyParamData[i].dValue);

	}
#endif
	sil_xml_save();

	return;
}

int sil_param_id_validation(int iParamid)
{
//	fprintf(stderr,"%s iParamid = %d g_icapiPrameterIndex = %d\n",__func__,iParamid,g_icapiPrameterIndex);
	if(iParamid > g_icapiPrameterIndex)
	{
		return SIL_PARAM_ID_OUT_OF_RANGE;
	}
	return NC_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter(unsigned int uiParameterID, double value,unsigned int uiDataType)
{

	int rc = 0;
#ifdef BUILTIN_TYPEID_TYPES
	rc = sil_param_id_validation(uiParameterID);

	if(rc != NC_OK)
	{
		return rc;
	}

	if(uiDataType != g_stSILModifyParamData[uiParameterID].uclDataType)
	{

		rc = SIL_PARAM_WRONG_TYPE;
	}

	switch(g_stSILModifyParamData[uiParameterID].uclDataType)
	{
	case SS_DOUBLE:
		*((double *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	case SS_INT32:
		*((int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	case SS_UINT32:
		*((unsigned int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	case SS_INT16:
		*((short *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	case SS_UINT16:
		*((unsigned int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	case SS_INT8:
		*((signed char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	case SS_UINT8:
		*((unsigned char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	case SS_BOOLEAN:
		*((bool *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;
		break;
	default:
		break;
	}

//	fprintf(stderr,"%s stage 7 uiParameterID = %d  value = %lf\n",__func__,uiParameterID,value);
	g_stSILModifyParamData[uiParameterID].dValue = value;
#endif
	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_double
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_double(unsigned int uiParameterID, double value)
{

	int rc = 0;
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_DOUBLE != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	*((double *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %lf\n",__func__,uiParameterID,value);
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_int
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_int(unsigned int uiParameterID, int value)
{

	int rc = 0;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_INT32 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	*((int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %d\n",__func__,uiParameterID,value);
	}

	return rc;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_uint
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_uint(unsigned int uiParameterID,unsigned int value)
{
	int rc = 0;
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_UINT32 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	*((unsigned int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %ud\n",__func__,uiParameterID,value);
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_short
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_short(unsigned int uiParameterID,short value)
{

	int rc = 0;
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_INT16 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	*((short *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %d\n",__func__,uiParameterID,value);
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_ushort
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_ushort(unsigned int uiParameterID,unsigned short value)
{

	int rc = 0;
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_UINT16 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	*((unsigned short *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %d\n",__func__,uiParameterID,value);
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_char
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_char(unsigned int uiParameterID,signed char value)
{

	int rc = 0;
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_INT8 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	*((signed char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %d\n",__func__,uiParameterID,value);
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_uchar
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_uchar(unsigned int uiParameterID,unsigned char value)
{

	int rc = 0;
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_UINT8 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif

	*((unsigned char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %d\n",__func__,uiParameterID,value);
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameter_bool
/// \brief  Set value to tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameter_bool(unsigned int uiParameterID,bool value)
{

	int rc = 0;
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_BOOLEAN != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif

	*((bool *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD)) = value;

	g_stSILModifyParamData[uiParameterID].dValue = value;

	if(g_ucDebugMode)
	{
		printf("%s uiParameterID = %d value = %d\n",__func__,uiParameterID,value);
	}

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
void sil_capi_get_parameter(unsigned int uiParameterID, double &value)
{
#ifdef BUILTIN_TYPEID_TYPES
	switch(g_stSILModifyParamData[uiParameterID].uclDataType)
	{
	case SS_DOUBLE:
		value = *((double *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	case SS_INT32:
		value = *((int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	case SS_UINT32:
		value = *((unsigned int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	case SS_INT16:
		value =  *((short *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	case SS_UINT16:
		value = *((unsigned int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	case SS_INT8:
		value = *((signed char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	case SS_UINT8:
		value = *((unsigned char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	case SS_BOOLEAN:
		value = *((bool *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
		break;
	default:
		break;
	}

//	g_stSILModifyParamData[uiParameterID].dValue = value;
#endif
	return;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_double
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_double(unsigned int uiParameterID, double &value)
{
	int rc = NC_OK;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_DOUBLE != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	value = *((double *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));

	return rc;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_int
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_int(unsigned int uiParameterID, int &value)
{
	int rc = NC_OK;
//	fprintf(stderr,"%s\n",__func__);
	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_INT32 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif

	value = *((int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));
//	fprintf(stderr,"%s value = %d\n",__func__,value);
	return rc;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_uint
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_uint(unsigned int uiParameterID,unsigned int &value)
{
	int rc = NC_OK;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_UINT32 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	value = *((unsigned int *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));

	return rc;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_short
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_short(unsigned int uiParameterID,short &value)
{
	int rc = NC_OK;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_INT16 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;

	}
#endif

	value = *((short *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));

	return rc;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_ushort
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_ushort(unsigned int uiParameterID,unsigned short &value)
{
	int rc = NC_OK;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_UINT16 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	value = *((unsigned short *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));

	return rc;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_char
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_char(unsigned int uiParameterID,signed char &value)
{
	int rc = NC_OK;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_INT8 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
	}
#endif
	value = *((char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));

	return rc;
}
////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_uchar
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_uchar(unsigned int uiParameterID,unsigned char &value)
{
	int rc = NC_OK;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_UINT8 != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
		return rc;
	}
#endif
	value = *((unsigned char *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));

	return rc;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_get_parameter_uchar
/// \brief  Get value of tunable parameter
///	\param uiParameterID - parameter ID
///	\param uiParameterID - parameter Value
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_get_parameter_bool(unsigned int uiParameterID,bool &value)
{
	int rc = NC_OK;

	rc = sil_param_id_validation(uiParameterID);
	if(rc != NC_OK)
	{
		return rc;
	}
#ifdef BUILTIN_TYPEID_TYPES
	if(SS_BOOLEAN != g_stSILModifyParamData[uiParameterID].uclDataType)
	{
		return SIL_PARAM_WRONG_TYPE;
		return rc;
	}
#endif
	value = *((bool *)(g_stSILModifyParamData[uiParameterID].pdTunableparamADD));

	return rc;
}


////////////////////////////////////////////////////////////////////////////////
/// \fn sil_capi_set_parameters_from_file
/// \brief  Set parameters value according to xml file SimulinkParamDef.xml if file exist
///	\param uiParameterID - parameter ID
/// \return	return - None
///
///
////////////////////////////////////////////////////////////////////////////////
int sil_capi_set_parameters_from_file()
{
	char compilationTImeFromFile[MAX_STR_SIZE];
	char compilationTIme[MAX_STR_SIZE];
	int rc = 0;
	double dValue = 0;

	//check if the compilation time and date in the SimulinkParamDef.xml is correct
	rc = sil_xml_getCompilationTimeFromXMLFile(SIMULINK_DEFAULT_XML,compilationTImeFromFile);

	if(rc != NC_OK)
		return rc;

	sil_xml_getCompilationTime(compilationTIme);
	rc = strcmp(compilationTIme,compilationTImeFromFile);

	if(rc != NC_OK)
		return rc;

	g_UserParams.Open(SIMULINK_DEFAULT_XML,UPXML_SET_DEF_REQ_FLG);

	for(int i = 0; i< g_icapiPrameterIndex; i++)
	{
		//get parameter value from XML
		sil_xml_read_param(i,dValue, g_stSILModifyParamData[i].strParamterName);

		//set the value to the global parameters data base
		sil_capi_set_parameter(i, dValue,g_stSILModifyParamData[i].uclDataType);
	}

	g_UserParams.Close();

	return rc;
}

int sil_capi_set_parameters_from_file_ex()
{
	char compilationTImeFromFile[MAX_STR_SIZE];
	char compilationTIme[MAX_STR_SIZE];
	int rc = 0;
	double dValue = 0;

	//check if the compilation time and date in the SimulinkParamDef.xml is correct
	rc = sil_xml_getCompilationTimeFromXMLFileEX(SIMULINK_DEFAULT_XML,compilationTImeFromFile);
//
	if(rc != NC_OK)
		return rc;

	sil_xml_getCompilationTime(compilationTIme);
	rc = strcmp(compilationTIme,compilationTImeFromFile);

	if(rc != NC_OK)
		return rc;

	for(int i = 0; i< g_icapiPrameterIndex; i++)
	{

		//get parameter value from XML
		sil_xml_read_param_ex(i, dValue, g_stSILModifyParamData[i].strParamterName);

//		printf("\33[0;34m XML_Def Value[%d] = %lf, \33[0m", i, g_stSILModifyParamData[i].dValue);
//		printf("\33[0;34m dValue[%d] = %lf \33[0m\n\n", i, dValue);

		//set the value to the global parameters data base
		sil_capi_set_parameter(i, dValue,g_stSILModifyParamData[i].uclDataType);
	}

//	printf("\33[0;34m g_icapiPrameterIndex = %d \33[0m\n", g_icapiPrameterIndex);

//	printf("\33[0;31msil_capi_set_parameters_from_file_ex():  Before g_xDoc.clear()\33[0m\n");
//	sleep(1);
	g_xDoc.clear();
//	printf("\33[0;31msil_capi_set_parameters_from_file_ex():  After g_xDoc.clear()\33[0m\n");
//	sleep(1);
	return rc;
}



int sil_capi_get_overflow( long &lvalue)
{
	int rc = NC_OK;

	lvalue = g_ulCycleOverFlow;

	return rc;
}

int sil_capi_get_TimeMeasurement(unsigned long &ulElapsedTime,unsigned long &ulMaxElapsedTime,unsigned long &ulAvgElapsedTime,long &lOverflow)
{
	int rc = NC_OK;

	ulElapsedTime = g_ulElapsedTime;
	ulMaxElapsedTime = g_ulElapsedTimeMax;
	ulAvgElapsedTime = g_ulAvgElapsedTime;
	lOverflow = g_ulCycleOverFlow;

	return rc;
}

int sil_capi_get_MaxElapsedTime(unsigned int uiParameterID, double &dvalue)
{
	int rc = NC_OK;

	dvalue = g_ulElapsedTimeMax;

	return rc;
}

int sil_capi_stopexec()
{
	int rc = NC_OK;
	printf(ANSI_COLOR_GREEN     "STOP SIL REQUEST"     ANSI_COLOR_GREEN "\n");
	g_iTerminate = 1;

	return rc;
}

int sil_capi_savelog()
{
	int rc = NC_OK;
	g_iLogMatFile = 1;

	return rc;
}

void set_Rec(unsigned short usValue)
{
	if(usValue)
	{
		g_cRecTrig = 1;
		g_iLogMatFile = 1;// save log file
	}
	else
	{
//		if( g_cRecTrig)
//		{
//			g_iLogMatFile = 1;// save log file
//		}
		g_cRecTrig = 0;
	}
}
int sil_capi_startrec()
{
	int rc = NC_OK;
	set_Rec(1);

	return rc;
}

int sil_capi_stoprec()
{
	int rc = NC_OK;
	set_Rec(0);

	return rc;
}

int sil_capi_recstat(unsigned char &ucValue)
{
	int rc = NC_OK;
	ucValue = g_cRecTrig;

	return rc;
}
