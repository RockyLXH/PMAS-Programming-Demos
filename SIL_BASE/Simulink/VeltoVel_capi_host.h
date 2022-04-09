#ifndef RTW_HEADER_VeltoVel_cap_host_h_
#define RTW_HEADER_VeltoVel_cap_host_h_
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"

typedef struct {
  rtwCAPI_ModelMappingInfo mmi;
} VeltoVel_host_DataMapInfo_T;

#ifdef __cplusplus

extern "C" {

#endif

  void VeltoVel_host_InitializeDataMapInfo(VeltoVel_host_DataMapInfo_T *dataMap,
    const char *path);

#ifdef __cplusplus

}
#endif
#endif                                 // HOST_CAPI_BUILD
#endif                                 // RTW_HEADER_VeltoVel_cap_host_h_

// EOF: VeltoVel_capi_host.h
