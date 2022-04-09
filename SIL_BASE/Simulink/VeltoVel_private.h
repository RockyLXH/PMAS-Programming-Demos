//
// File: VeltoVel_private.h
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
#ifndef RTW_HEADER_VeltoVel_private_h_
#define RTW_HEADER_VeltoVel_private_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

extern int32_T MultiWord2sLong(const uint32_T u[]);
extern void sMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);
extern void sMultiWordMul(const uint32_T u1[], int32_T n1, const uint32_T u2[],
  int32_T n2, uint32_T y[], int32_T n);

#endif                                 // RTW_HEADER_VeltoVel_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
