//
// File: VeltoVel.cpp
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
#include "VeltoVel_capi.h"
#include "VeltoVel.h"
#include "VeltoVel_private.h"

// Real-time model
RT_MODEL_VeltoVel_T VeltoVel_M_ = RT_MODEL_VeltoVel_T();
RT_MODEL_VeltoVel_T *const VeltoVel_M = &VeltoVel_M_;
int32_T MultiWord2sLong(const uint32_T u[])
{
  return static_cast<int32_T>(u[0]);
}

void sMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T y[],
                   int32_T n)
{
  int32_T i;
  int32_T i1;
  int32_T nb;
  int32_T nc;
  uint32_T nr;
  uint32_T u1i;
  uint32_T yi;
  uint32_T ys;
  nb = static_cast<int32_T>(n2 >> 5);
  i = 0;
  ys = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  if (nb < n1) {
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - (static_cast<uint32_T>(nb) << 5);
    if (nr > 0U) {
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        yi = u1i >> nr;
        u1i = u1[i1];
        y[i] = u1i << (32U - nr) | yi;
        i++;
      }

      y[i] = (nc < n1 ? u1[nc] : ys) << (32U - nr) | u1i >> nr;
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

void sMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[], int32_T
                   n2, uint32_T y[], int32_T n)
{
  int32_T i;
  int32_T j;
  int32_T k;
  int32_T ni;
  uint32_T a0;
  uint32_T a1;
  uint32_T b1;
  uint32_T cb;
  uint32_T cb1;
  uint32_T cb2;
  uint32_T u1i;
  uint32_T w01;
  uint32_T w10;
  uint32_T yk;
  boolean_T isNegative1;
  boolean_T isNegative2;
  isNegative1 = ((u1[n1 - 1] & 2147483648U) != 0U);
  isNegative2 = ((u2[n2 - 1] & 2147483648U) != 0U);
  cb1 = 1U;

  // Initialize output to zero
  for (k = 0; k < n; k++) {
    y[k] = 0U;
  }

  for (i = 0; i < n1; i++) {
    cb = 0U;
    u1i = u1[i];
    if (isNegative1) {
      u1i = ~u1i + cb1;
      cb1 = (u1i < cb1);
    }

    a1 = u1i >> 16U;
    a0 = u1i & 65535U;
    cb2 = 1U;
    ni = n - i;
    ni = n2 <= ni ? n2 : ni;
    k = i;
    for (j = 0; j < ni; j++) {
      u1i = u2[j];
      if (isNegative2) {
        u1i = ~u1i + cb2;
        cb2 = (u1i < cb2);
      }

      b1 = u1i >> 16U;
      u1i &= 65535U;
      w10 = a1 * u1i;
      w01 = a0 * b1;
      yk = y[k] + cb;
      cb = (yk < cb);
      u1i *= a0;
      yk += u1i;
      cb += (yk < u1i);
      u1i = w10 << 16U;
      yk += u1i;
      cb += (yk < u1i);
      u1i = w01 << 16U;
      yk += u1i;
      cb += (yk < u1i);
      y[k] = yk;
      cb += w10 >> 16U;
      cb += w01 >> 16U;
      cb += a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  // Apply sign
  if (isNegative1 != isNegative2) {
    cb = 1U;
    for (k = 0; k < n; k++) {
      yk = ~y[k] + cb;
      y[k] = yk;
      cb = (yk < cb);
    }
  }
}

// Model step function
void VeltoVel_step(void)
{
  // local block i/o variables
  int32_T rtb_ReadPI_INT;
  int32_T rtb_DataTypeConversion1;
  int64m_T tmp;
  int64m_T tmp_0;
  int64m_T tmp_3;
  uint32_T tmp_1;
  uint32_T tmp_2;

  // S-Function (ReadPI_INT): '<Root>/ReadPI_INT'
  Elmo_Read_PI_Int(VeltoVel_P.ReadPI_INT_SParameter1, &rtb_ReadPI_INT);

  // Gain: '<Root>/Gain'
  tmp_1 = static_cast<uint32_T>(VeltoVel_P.Gain_Gain);
  tmp_2 = static_cast<uint32_T>(rtb_ReadPI_INT);
  sMultiWordMul(&tmp_1, 1, &tmp_2, 1, &tmp_0.chunks[0U], 2);

  // DataTypeConversion: '<Root>/Data Type Conversion'
  sMultiWordShr(&tmp_0.chunks[0U], 2, 29U, &tmp.chunks[0U], 2);

  // DataTypeConversion: '<Root>/Data Type Conversion1' incorporates:
  //   DataTypeConversion: '<Root>/Data Type Conversion'

  rtb_DataTypeConversion1 = MultiWord2sLong(&tmp.chunks[0U]);

  // S-Function (E60FFWrite): '<Root>/E60FFWrite'
  Elmo_Write_60FF(VeltoVel_P.E60FFWrite_p1, rtb_DataTypeConversion1);

  // Gain: '<Root>/Gain1'
  tmp_1 = static_cast<uint32_T>(VeltoVel_P.Gain1_Gain);
  tmp_2 = static_cast<uint32_T>(rtb_ReadPI_INT);
  sMultiWordMul(&tmp_1, 1, &tmp_2, 1, &tmp_3.chunks[0U], 2);

  // DataTypeConversion: '<Root>/Data Type Conversion1'
  sMultiWordShr(&tmp_3.chunks[0U], 2, 29U, &tmp_0.chunks[0U], 2);

  // DataTypeConversion: '<Root>/Data Type Conversion1'
  rtb_DataTypeConversion1 = MultiWord2sLong(&tmp_0.chunks[0U]);

  // S-Function (E60FFWrite): '<Root>/E60FFWrite1'
  Elmo_Write_60FF(VeltoVel_P.E60FFWrite1_p1, rtb_DataTypeConversion1);
}

// Model initialize function
void VeltoVel_initialize(void)
{
  // Registration code

  // Initialize DataMapInfo substructure containing ModelMap for C API
  VeltoVel_InitializeDataMapInfo();

  // Start for S-Function (InitSILModel): '<Root>/InitSILModel'
  ElmoSILMultiStepInitModel();

  // SystemInitialize for Atomic SubSystem: '<Root>/Initialize Function'

  // Start for S-Function (E60FFInit): '<S1>/E60FFInit'
  Elmo_Init_60FF(VeltoVel_P.E60FFInit_p1, VeltoVel_P.E60FFInit_p2);

  // Start for S-Function (E60FFInit): '<S1>/E60FFInit1'
  Elmo_Init_60FF(VeltoVel_P.E60FFInit1_p1, VeltoVel_P.E60FFInit1_p2);

  // Start for S-Function (InitProcessData): '<S1>/InitProcessData'
  Elmo_Init_PI_Var(VeltoVel_P.InitProcessData_p1,
                   VeltoVel_P.InitProcessData_SParameter2,
                   VeltoVel_P.InitProcessData_SParameter3,
                   VeltoVel_P.InitProcessData_p4);

  // End of SystemInitialize for SubSystem: '<Root>/Initialize Function'
}

// Model terminate function
void VeltoVel_terminate(void)
{
  // Terminate for S-Function (E60FFWrite): '<Root>/E60FFWrite'
  Elmo_Terminate_60FF(VeltoVel_P.E60FFWrite_p1);

  // Terminate for S-Function (E60FFWrite): '<Root>/E60FFWrite1'
  Elmo_Terminate_60FF(VeltoVel_P.E60FFWrite1_p1);
}

//
// File trailer for generated code.
//
// [EOF]
//
