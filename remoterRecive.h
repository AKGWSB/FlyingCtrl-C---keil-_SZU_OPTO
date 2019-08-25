#include <L_Link.h>

#ifndef __REMOTERRECIVER_H__

#define __REMOTERRECIVER_H__

typedef   signed short     int int16_t;
typedef int16_t s16;

typedef struct
{
	//陀螺仪矫正标志位
	short Gyro_ref_flag;
	//全部油门输出
	short all_out_ctrl;
	
	//是否解锁
	short isLock;
	int lockCounter;
	
	//遥控器通道矫正计数器,累加和
	s16 remoterCorrectCounter;
	s16 remoterCorrectFlag;
	s16 PITCH_Correct;
	s16 ROLL_Correct;
	
	//通道矫正后的值
	s16 ch0_correct;
	s16 ch1_correct;
	
	//接收机通道
	s16 remoterCH[8];
	s16 remoterCH_filter[8];
}remoterRecive_DataStruct;

//init
void remoterRecive_Init();

//接收机信号输入
void remoterRecive_GetData_2ms_(s16* CH_In);

//接收机信号滤波
void remoterRecive_CH_filter_1ms_();

//检查当前模式
void remoterRecive_FlyModDuty_2ms_();

//是否紧急停机
short remoterRecive_Emergency_out_();

//解锁控制
short remoterRecive_IsLock_out_();

//矫正遥控器PITCH/ROLL舵量
void remoterRecive_remoterCorrect();

#endif
