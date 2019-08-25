#include <L_Link.h>


#ifndef __FLYTASK_H__

#define __FLYTASK_H__

//状态枚举
typedef enum 
{
	pointStay,		//定点
	followLine,		//寻线
	followLine2,
	left,					//向左寻线
	right,				//向右寻线
	forward,			//向前寻线
	back,					//向后寻线
	back2,
	ctrlOff,			//不控制
	yawTurn	,			//偏航控制
	blindFly	,		//盲飞过弯
	yawTurn2
}stateEnum;


//任务枚举
typedef enum
{
	A,	//
	B,	//
	C,	//
	D,	//
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N
}taskEnum;

typedef struct 
{
	int timerCNT;
	stateEnum state;
	taskEnum task;
	postureAngle targetAngle;
	
	//位置数据接口，根据不同飞行状态改变位置数据
	int posInterfaceX;
	int posInterfaceY;
	
	//速度数据接口
	float speedInterfaceX;
	float speedInterfaceY;
	
}flyTask_DataStruct;


//init
void flyTast_Init();


//飞行任务检查
void flyTask_CheckDuty_2ms_();


//飞行输出更新
void flyTask_AngleOutPutUpdate_2ms_();


//输出目标俯仰角
float flyTask_TargetAngleOut_Pitch();


//输出目标横滚角
float flyTask_TargetAngleOut_Roll();


//输出目标偏航角
float flyTask_TargetAngleOut_Yaw();


//切换到定点
void flyTask_ChangeTo_pointStay();


//切换到寻线
void flyTask_ChangeTo_followLine();


//切换到不控制
void flyTask_ChangeTo_ctrlOff();



#endif 
