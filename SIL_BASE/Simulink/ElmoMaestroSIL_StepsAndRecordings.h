/* 
 * ElmoMaestroSIL_StepsAndRecordings.h 
 * 
 * Created automatcailly on dd/mm/yy, time: 00:00:00
 * By the SIL Post Exec Function. 
 * 
 * This file is used by the SIL to export Data Recording
 * Signal names to the EAS Recorder ...
*/ 

#ifndef ELMOMAESTROSIL_REC_H_
#define ELMOMAESTROSIL_REC_H_
// 
#define SIL_REC_MAX_SIG 		32
#define SIL_REC_MAX_ALIAS_STR 	32

#define SIL_PRIVATE_DATA_SIZE   10
#define SIL_PRIVATE_MAX_STR     50


/* 
 * Multi Step Support 
 */ 

#define ELMO_NUM_MULTI_STEPS   0
#define ELMO_STEP           VeltoVel_step();
#define ELMO_STEPFUNC_1     NULL
#define ELMO_STEPFUNC_2     NULL
#define ELMO_STEPFUNC_3     NULL
#define ELMO_STEPFUNC_4     NULL
#define ELMO_STEPFUNC_5     NULL
#define ELMO_STEPFUNC_6     NULL
#define ELMO_STEPFUNC_7     NULL
#define ELMO_STEPFUNC_8     NULL

#define ELMO_STEPRATE_1     555
#define ELMO_STEPRATE_2     555
#define ELMO_STEPRATE_3     555
#define ELMO_STEPRATE_4     555
#define ELMO_STEPRATE_5     555
#define ELMO_STEPRATE_6     555
#define ELMO_STEPRATE_7     555
#define ELMO_STEPRATE_8     555

#define ELMO_EVENTFUNC_1     NULL
#define ELMO_EVENTFUNC_2     NULL
#define ELMO_EVENTFUNC_3     NULL
#define ELMO_EVENTFUNC_4     NULL
#define ELMO_EVENTFUNC_5     NULL
#define ELMO_EVENTFUNC_6     NULL
#define ELMO_EVENTFUNC_7     NULL
#define ELMO_EVENTFUNC_8     NULL

#define ELMO_UDPEVENTFUNC_1     NULL
#define ELMO_UDPEVENTFUNC_2     NULL
#define ELMO_UDPEVENTFUNC_3     NULL
#define ELMO_UDPEVENTFUNC_4     NULL
#define ELMO_UDPEVENTFUNC_5     NULL
#define ELMO_UDPEVENTFUNC_6     NULL
#define ELMO_UDPEVENTFUNC_7     NULL
#define ELMO_UDPEVENTFUNC_8     NULL

#define ELMO_UDPEVENTID_1       555
#define ELMO_UDPEVENTID_2       555
#define ELMO_UDPEVENTID_3       555
#define ELMO_UDPEVENTID_4       555
#define ELMO_UDPEVENTID_5       555
#define ELMO_UDPEVENTID_6       555
#define ELMO_UDPEVENTID_7       555
#define ELMO_UDPEVENTID_8       555

/* 
 * Recording Signals 
 */ 
#ifdef SIL_REC
// Array or Strings for version data ...
char SIL_PRIVATE_DATA[SIL_PRIVATE_DATA_SIZE][SIL_PRIVATE_MAX_STR] =
{
        "3.15",
        "Elmo Motion Control Ltd",
        "0.0.0.0",
        "Sun May 09 14:42:44 2021",
        "",
        "",
        "",
        "",
        "",
        "",
};

// Array or Strings for the LONG Signals ...
char SIL_LONG_ALIAS[SIL_REC_MAX_SIG][SIL_REC_MAX_ALIAS_STR] =
{
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
};
// Array or Strings for the DOUBLE Signals ...
char SIL_DOUBLE_ALIAS[SIL_REC_MAX_SIG][SIL_REC_MAX_ALIAS_STR] =
{
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
};
#else
extern char SIL_PRIVATE_DATA[SIL_PRIVATE_DATA_SIZE][SIL_PRIVATE_MAX_STR];
extern char SIL_LONG_ALIAS[SIL_REC_MAX_SIG][SIL_REC_MAX_ALIAS_STR];
extern char SIL_DOUBLE_ALIAS[SIL_REC_MAX_SIG][SIL_REC_MAX_ALIAS_STR];
#endif

#endif /* ELMOMAESTROSIL_REC_H_ */ 
