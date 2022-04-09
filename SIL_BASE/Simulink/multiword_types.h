//
// File: multiword_types.h
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

#ifndef MULTIWORD_TYPES_H
#define MULTIWORD_TYPES_H
#include "rtwtypes.h"

//
//  MultiWord supporting definitions
typedef long int long_T;

//
//  MultiWord types
typedef struct {
  uint32_T chunks[2];
} int64m_T;

typedef struct {
  uint32_T chunks[2];
} uint64m_T;

typedef struct {
  uint32_T chunks[3];
} int96m_T;

typedef struct {
  uint32_T chunks[3];
} uint96m_T;

typedef struct {
  uint32_T chunks[4];
} int128m_T;

typedef struct {
  uint32_T chunks[4];
} uint128m_T;

typedef struct {
  uint32_T chunks[5];
} int160m_T;

typedef struct {
  uint32_T chunks[5];
} uint160m_T;

typedef struct {
  uint32_T chunks[6];
} int192m_T;

typedef struct {
  uint32_T chunks[6];
} uint192m_T;

typedef struct {
  uint32_T chunks[7];
} int224m_T;

typedef struct {
  uint32_T chunks[7];
} uint224m_T;

typedef struct {
  uint32_T chunks[8];
} int256m_T;

typedef struct {
  uint32_T chunks[8];
} uint256m_T;

#endif                                 // MULTIWORD_TYPES_H

//
// File trailer for generated code.
//
// [EOF]
//
