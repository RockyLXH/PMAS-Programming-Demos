//
// File: VeltoVel.h
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
#ifndef RTW_HEADER_VeltoVel_h_
#define RTW_HEADER_VeltoVel_h_
#include "rtw_modelmap.h"
#include "rtwtypes.h"
#include "ElmoMaestroSIL_Lib.h"
#include "VeltoVel_types.h"

// Shared type includes
#include "multiword_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm)         ((rtm)->DataMapInfo)
#endif

#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val)    ((rtm)->DataMapInfo = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Parameters (default storage)
struct P_VeltoVel_T_ {
  uint32_T ReadPI_INT_SParameter1;     // Mask Parameter: ReadPI_INT_SParameter1
                                          //  Referenced by: '<Root>/ReadPI_INT'

  uint16_T InitProcessData_SParameter2;
                                  // Mask Parameter: InitProcessData_SParameter2
                                     //  Referenced by: '<S1>/InitProcessData'

  uint16_T InitProcessData_SParameter3;
                                  // Mask Parameter: InitProcessData_SParameter3
                                     //  Referenced by: '<S1>/InitProcessData'

  int32_T Gain_Gain;                   // Computed Parameter: Gain_Gain
                                          //  Referenced by: '<Root>/Gain'

  int32_T Gain1_Gain;                  // Computed Parameter: Gain1_Gain
                                          //  Referenced by: '<Root>/Gain1'

  uint32_T E60FFInit_p1;               // Computed Parameter: E60FFInit_p1
                                          //  Referenced by: '<S1>/E60FFInit'

  uint32_T E60FFInit_p2;               // Computed Parameter: E60FFInit_p2
                                          //  Referenced by: '<S1>/E60FFInit'

  uint32_T E60FFInit1_p1;              // Computed Parameter: E60FFInit1_p1
                                          //  Referenced by: '<S1>/E60FFInit1'

  uint32_T E60FFInit1_p2;              // Computed Parameter: E60FFInit1_p2
                                          //  Referenced by: '<S1>/E60FFInit1'

  uint32_T InitProcessData_p1;         // Computed Parameter: InitProcessData_p1
                                          //  Referenced by: '<S1>/InitProcessData'

  uint32_T InitProcessData_p4;         // Computed Parameter: InitProcessData_p4
                                          //  Referenced by: '<S1>/InitProcessData'

  uint32_T E60FFWrite_p1;              // Computed Parameter: E60FFWrite_p1
                                          //  Referenced by: '<Root>/E60FFWrite'

  uint32_T E60FFWrite1_p1;             // Computed Parameter: E60FFWrite1_p1
                                          //  Referenced by: '<Root>/E60FFWrite1'

};

// Real-time Model Data Structure
struct tag_RTM_VeltoVel_T {
  const char_T * volatile errorStatus;

  //
  //  DataMapInfo:
  //  The following substructure contains information regarding
  //  structures generated in the model's C API.

  struct {
    rtwCAPI_ModelMappingInfo mmi;
  } DataMapInfo;
};

// Block parameters (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern P_VeltoVel_T VeltoVel_P;

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void VeltoVel_initialize(void);
  extern void VeltoVel_step(void);
  extern void VeltoVel_terminate(void);

#ifdef __cplusplus

}
#endif

// Function to get C API Model Mapping Static Info
extern const rtwCAPI_ModelMappingStaticInfo*
  VeltoVel_GetCAPIStaticMap(void);

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_VeltoVel_T *const VeltoVel_M;

#ifdef __cplusplus

}
#endif

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'VeltoVel'
//  '<S1>'   : 'VeltoVel/Initialize Function'

#endif                                 // RTW_HEADER_VeltoVel_h_

//
// File trailer for generated code.
//
// [EOF]
//
