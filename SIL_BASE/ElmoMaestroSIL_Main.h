#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\builtin_typeid_types.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\ElmoMaestroSIL_StepsAndRecordings.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\multiword_types.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\rtwtypes.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\rtw_capi.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\rtw_modelmap.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\rtw_modelmap_logging.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\sysran_types.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\VeltoVel.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\VeltoVel_capi.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\VeltoVel_capi_host.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\VeltoVel_private.h>
#include <C:\Users\tjbli\Documents\MaestroDeveloperStudio\Maestro_workspace\SIL_BASE\Simulink\VeltoVel_types.h>
#define ELMO_INITIALIZE 	VeltoVel_initialize();
#define ELMO_TERMINATE		VeltoVel_terminate();
#define ELMO_MAP_INFO		g_MMI = &(rtmGetDataMapInfo(VeltoVel_M).mmi);
#define ELMO_LOGGER			rt_StopDataLogging("elmo_log.mat", (VeltoVel_M)->rtwLogInfo);
#define ELMO_MODEL_NAME		"VeltoVel"
