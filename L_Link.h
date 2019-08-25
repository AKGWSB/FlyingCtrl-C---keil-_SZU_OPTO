#ifndef __L_LINK_H__

#define __L_LINK_H__

#include <posFilter.h>
#include <autoHightCtrl.h>
#include <posXYCtrl.h>
#include <skz.h>
#include "openMv.h"
#include "remoterRecive.h"
#include "postureCaculate.h"
#include <postureCtrl.h>
#include <flyMode.h>
#include "PID.h"
#include "openmv_.h"
//#include "L_math.h"
#include "flyTask.h"
#include "OF.h"


extern posFilter_DataStruct posFilterData;
extern hightCtrl_DataStruct hightCtrlData;
extern posXYCtrl_DataStruct posXYCtrlData;
extern skz_DataStruct skzData;
extern openMv_DataStruct openMvData;
extern remoterRecive_DataStruct remoterReciveData;
extern postureCaculate_DataStruct postureCaculateData;
extern postureCtrl_DataStruct postureCtrlData;
extern flyMode_DataStruct flyModeData;
extern PID_DataStruct PID_Data;
extern openmv_Data openmv;
extern flyTask_DataStruct flyTaskData;
extern OF_Datastruct OF_Data;

#endif

