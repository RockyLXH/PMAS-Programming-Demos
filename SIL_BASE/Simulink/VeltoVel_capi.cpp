//
// File: VeltoVel_capi.cpp
//
// Code generated for Simulink model 'VeltoVel'.
//
// Model version                  : 3.15
// Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
// C/C++ source code generated on : Wed Mar 23 17:28:50 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 11
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "VeltoVel_capi_host.h"
#define sizeof(s)                      ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el)              ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s)               (s)
#else                                  // HOST_CAPI_BUILD
#include "builtin_typeid_types.h"
#include "VeltoVel.h"
#include "VeltoVel_capi.h"
#include "VeltoVel_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               (NULL)
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif                                 // HOST_CAPI_BUILD

static const rtwCAPI_BlockParameters rtBlockParameters[] = {
  // addrMapIndex, blockPath,
  //  paramName, dataTypeIndex, dimIndex, fixPtIdx

  { 0, TARGET_STRING("VeltoVel/Gain"),
    TARGET_STRING("Gain"), 0, 0, 1 },

  { 1, TARGET_STRING("VeltoVel/Gain1"),
    TARGET_STRING("Gain"), 0, 0, 1 },

  { 2, TARGET_STRING("VeltoVel/E60FFWrite"),
    TARGET_STRING("p1"), 1, 0, 0 },

  { 3, TARGET_STRING("VeltoVel/E60FFWrite1"),
    TARGET_STRING("p1"), 1, 0, 0 },

  { 4, TARGET_STRING("VeltoVel/ReadPI_INT"),
    TARGET_STRING("SParameter1"), 1, 0, 0 },

  { 5, TARGET_STRING("VeltoVel/Initialize Function/E60FFInit"),
    TARGET_STRING("p1"), 1, 0, 0 },

  { 6, TARGET_STRING("VeltoVel/Initialize Function/E60FFInit"),
    TARGET_STRING("p2"), 1, 0, 0 },

  { 7, TARGET_STRING("VeltoVel/Initialize Function/E60FFInit1"),
    TARGET_STRING("p1"), 1, 0, 0 },

  { 8, TARGET_STRING("VeltoVel/Initialize Function/E60FFInit1"),
    TARGET_STRING("p2"), 1, 0, 0 },

  { 9, TARGET_STRING("VeltoVel/Initialize Function/InitProcessData"),
    TARGET_STRING("SParameter2"), 2, 0, 0 },

  { 10, TARGET_STRING("VeltoVel/Initialize Function/InitProcessData"),
    TARGET_STRING("SParameter3"), 2, 0, 0 },

  { 11, TARGET_STRING("VeltoVel/Initialize Function/InitProcessData"),
    TARGET_STRING("p1"), 1, 0, 0 },

  { 12, TARGET_STRING("VeltoVel/Initialize Function/InitProcessData"),
    TARGET_STRING("p4"), 1, 0, 0 },

  {
    0, (NULL), (NULL), 0, 0, 0
  }
};

// Root Inputs information
static const rtwCAPI_Signals rtRootInputs[] = {
  // addrMapIndex, sysNum, blockPath,
  //  signalName, portNumber, dataTypeIndex, dimIndex, fxpIndex, sTimeIndex

  {
    0, 0, (NULL), (NULL), 0, 0, 0, 0, 0
  }
};

// Root Outputs information
static const rtwCAPI_Signals rtRootOutputs[] = {
  // addrMapIndex, sysNum, blockPath,
  //  signalName, portNumber, dataTypeIndex, dimIndex, fxpIndex, sTimeIndex

  {
    0, 0, (NULL), (NULL), 0, 0, 0, 0, 0
  }
};

// Tunable variable parameters
static const rtwCAPI_ModelParameters rtModelParameters[] = {
  // addrMapIndex, varName, dataTypeIndex, dimIndex, fixPtIndex
  { 0, (NULL), 0, 0, 0 }
};

#ifndef HOST_CAPI_BUILD

// Declare Data Addresses statically
static void* rtDataAddrMap[] = {
  &VeltoVel_P.Gain_Gain,               // 0: Block Parameter
  &VeltoVel_P.Gain1_Gain,              // 1: Block Parameter
  &VeltoVel_P.E60FFWrite_p1,           // 2: Block Parameter
  &VeltoVel_P.E60FFWrite1_p1,          // 3: Block Parameter
  &VeltoVel_P.ReadPI_INT_SParameter1,  // 4: Block Parameter
  &VeltoVel_P.E60FFInit_p1,            // 5: Block Parameter
  &VeltoVel_P.E60FFInit_p2,            // 6: Block Parameter
  &VeltoVel_P.E60FFInit1_p1,           // 7: Block Parameter
  &VeltoVel_P.E60FFInit1_p2,           // 8: Block Parameter
  &VeltoVel_P.InitProcessData_SParameter2,// 9: Block Parameter
  &VeltoVel_P.InitProcessData_SParameter3,// 10: Block Parameter
  &VeltoVel_P.InitProcessData_p1,      // 11: Block Parameter
  &VeltoVel_P.InitProcessData_p4,      // 12: Block Parameter
};

// Declare Data Run-Time Dimension Buffer Addresses statically
static int32_T* rtVarDimsAddrMap[] = {
  (NULL)
};

#endif

// Data Type Map - use dataTypeMapIndex to access this structure
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap[] = {
  // cName, mwName, numElements, elemMapIndex, dataSize, slDataId, *
  //  isComplex, isPointer, enumStorageType
  { "int", "int32_T", 0, 0, sizeof(int32_T), SS_INT32, 0, 0, 0 },

  { "unsigned int", "uint32_T", 0, 0, sizeof(uint32_T), SS_UINT32, 0, 0, 0 },

  { "unsigned short", "uint16_T", 0, 0, sizeof(uint16_T), SS_UINT16, 0, 0, 0 }
};

#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif

// Structure Element Map - use elemMapIndex to access this structure
static TARGET_CONST rtwCAPI_ElementMap rtElementMap[] = {
  // elementName, elementOffset, dataTypeIndex, dimIndex, fxpIndex
  { (NULL), 0, 0, 0, 0 },
};

// Dimension Map - use dimensionMapIndex to access elements of ths structure
static const rtwCAPI_DimensionMap rtDimensionMap[] = {
  // dataOrientation, dimArrayIndex, numDims, vardimsIndex
  { rtwCAPI_SCALAR, 0, 2, 0 }
};

// Dimension Array- use dimArrayIndex to access elements of this array
static const uint_T rtDimensionArray[] = {
  1,                                   // 0
  1                                    // 1
};

// C-API stores floating point values in an array. The elements of this  *
//  are unique. This ensures that values which are shared across the model*
//  are stored in the most efficient way. These values are referenced by  *
//            - rtwCAPI_FixPtMap.fracSlopePtr,                            *
//            - rtwCAPI_FixPtMap.biasPtr,                                 *
//            - rtwCAPI_SampleTimeMap.samplePeriodPtr,                    *
//            - rtwCAPI_SampleTimeMap.sampleOffsetPtr
static const real_T rtcapiStoredFloats[] = {
  0.0, 1.0
};

// Fixed Point Map
static const rtwCAPI_FixPtMap rtFixPtMap[] = {
  // fracSlopePtr, biasPtr, scaleType, wordLength, exponent, isSigned
  { (NULL), (NULL), rtwCAPI_FIX_RESERVED, 0, 0, 0 },

  { static_cast<const void *>(&rtcapiStoredFloats[1]), static_cast<const void *>
    (&rtcapiStoredFloats[0]), rtwCAPI_FIX_UNIFORM_SCALING, 32, -29, 1 }
};

// Sample Time Map - use sTimeIndex to access elements of ths structure
static const rtwCAPI_SampleTimeMap rtSampleTimeMap[] = {
  // samplePeriodPtr, sampleOffsetPtr, tid, samplingMode
  {
    (NULL), (NULL), 0, 0
  }
};

static rtwCAPI_ModelMappingStaticInfo mmiStatic = {
  // Signals:{signals, numSignals,
  //            rootInputs, numRootInputs,
  //            rootOutputs, numRootOutputs},
  //  Params: {blockParameters, numBlockParameters,
  //           modelParameters, numModelParameters},
  //  States: {states, numStates},
  //  Maps:   {dataTypeMap, dimensionMap, fixPtMap,
  //           elementMap, sampleTimeMap, dimensionArray},
  //  TargetType: targetType

  { (NULL), 0,
    rtRootInputs, 0,
    rtRootOutputs, 0 },

  { rtBlockParameters, 13,
    rtModelParameters, 0 },

  { (NULL), 0 },

  { rtDataTypeMap, rtDimensionMap, rtFixPtMap,
    rtElementMap, rtSampleTimeMap, rtDimensionArray },
  "float",

  { 2534585745U,
    423862163U,
    2228517672U,
    1125024108U },
  (NULL), 0,
  0
};

// Function to get C API Model Mapping Static Info
const rtwCAPI_ModelMappingStaticInfo*
  VeltoVel_GetCAPIStaticMap(void)
{
  return &mmiStatic;
}

// Cache pointers into DataMapInfo substructure of RTModel
#ifndef HOST_CAPI_BUILD

void VeltoVel_InitializeDataMapInfo(void)
{
  // Set C-API version
  rtwCAPI_SetVersion(VeltoVel_M->DataMapInfo.mmi, 1);

  // Cache static C-API data into the Real-time Model Data structure
  rtwCAPI_SetStaticMap(VeltoVel_M->DataMapInfo.mmi, &mmiStatic);

  // Cache static C-API logging data into the Real-time Model Data structure
  rtwCAPI_SetLoggingStaticMap(VeltoVel_M->DataMapInfo.mmi, (NULL));

  // Cache C-API Data Addresses into the Real-Time Model Data structure
  rtwCAPI_SetDataAddressMap(VeltoVel_M->DataMapInfo.mmi, rtDataAddrMap);

  // Cache C-API Data Run-Time Dimension Buffer Addresses into the Real-Time Model Data structure 
  rtwCAPI_SetVarDimsAddressMap(VeltoVel_M->DataMapInfo.mmi, rtVarDimsAddrMap);

  // Cache the instance C-API logging pointer
  rtwCAPI_SetInstanceLoggingInfo(VeltoVel_M->DataMapInfo.mmi, (NULL));

  // Set reference to submodels
  rtwCAPI_SetChildMMIArray(VeltoVel_M->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetChildMMIArrayLen(VeltoVel_M->DataMapInfo.mmi, 0);
}

#else                                  // HOST_CAPI_BUILD
#ifdef __cplusplus

extern "C" {

#endif

  void VeltoVel_host_InitializeDataMapInfo(VeltoVel_host_DataMapInfo_T *dataMap,
    const char *path)
  {
    // Set C-API version
    rtwCAPI_SetVersion(dataMap->mmi, 1);

    // Cache static C-API data into the Real-time Model Data structure
    rtwCAPI_SetStaticMap(dataMap->mmi, &mmiStatic);

    // host data address map is NULL
    rtwCAPI_SetDataAddressMap(dataMap->mmi, NULL);

    // host vardims address map is NULL
    rtwCAPI_SetVarDimsAddressMap(dataMap->mmi, NULL);

    // Set Instance specific path
    rtwCAPI_SetPath(dataMap->mmi, path);
    rtwCAPI_SetFullPath(dataMap->mmi, NULL);

    // Set reference to submodels
    rtwCAPI_SetChildMMIArray(dataMap->mmi, (NULL));
    rtwCAPI_SetChildMMIArrayLen(dataMap->mmi, 0);
  }

#ifdef __cplusplus

}
#endif
#endif                                 // HOST_CAPI_BUILD

//
// File trailer for generated code.
//
// [EOF]
//
