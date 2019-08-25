#ifndef __POSTURECTRL_H_

#define __POSTURECTRL_H_

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

#define moto1 0
#define moto2 1
#define moto3 2
#define moto4 3

typedef   signed short     int int16_t;
typedef int16_t s16;

typedef struct
{
	//姿态角
	float YAW;
	float PITCH;
	float ROLL;
	
	//目标角度
	float target_angle_PITCH;
	float target_angle_ROLL;
	float target_angle_YAW;	
	float target_angle_ref;
	
	//角度误差
	float err_angle_YAW;
	float err_angle_PITCH;
	float err_angle_ROLL;
	
	//三轴角速度
	float angle_speed_YAW;
	float angle_speed_PITCH;
	float angle_speed_ROLL;
	
	float last_angle_speed_PITCH;
	float last_angle_speed_ROLL;
	float last_angle_speed_YAW;
	
	//目标角速度
	float target_angleSpeed_YAW;
	float target_angleSpeed_PITCH;
	float target_angleSpeed_ROLL;
	
	//角速度误差
	float err_angleSpeed_YAW;
	float err_angleSpeed_PITCH;
	float err_angleSpeed_ROLL;
	
	//角速度误差积分
	float errSum_angleSpeed_YAW;
	float errSum_angleSpeed_PITCH;
	float errSum_angleSpeed_ROLL;
	
	//三方向输出
	float outPITCH;
	float outROLL;
	float outYAW;
	
}postureCtrl_DataStruct;

//init
void postureCtrl_Init();

//姿态控制获取数据--输入:三轴融合角度,角速度
void postureCtrl_GetData_2ms_(float PITCH, float ROLL, float YAW, float angleSpeed_PITCH, float angleSpeed_ROLL, float angleSpeed_YAW);

//姿态控制四电机输出--输入:
// 2 1 :机头↑
// 3 4 :电机序号
void postureCtrl_Out4Moto_2ms_(float* moto);

#endif




