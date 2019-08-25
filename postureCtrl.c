#include <postureCtrl.h>
#include "L_math.h"
#include <posXYCtrl.h>
#include <flyTask.h>

postureCtrl_DataStruct postureCtrlData;

//init
void postureCtrl_Init()
{
	//姿态角
	//postureCtrlData.YAW = 0;
	postureCtrlData.PITCH = 0;
	postureCtrlData.ROLL = 0;
		
	//目标角度
	postureCtrlData.target_angle_PITCH = 0;
	postureCtrlData.target_angle_ROLL = 0;
	//postureCtrlData.target_angle_YAW = 0;
	
	//角度误差
	postureCtrlData.err_angle_PITCH = 0;
	postureCtrlData.err_angle_ROLL = 0;
	postureCtrlData.err_angle_YAW = 0;

	//三轴角速度	
	postureCtrlData.angle_speed_YAW = 0;
	postureCtrlData.angle_speed_PITCH = 0;
	postureCtrlData.angle_speed_ROLL = 0;
	
	postureCtrlData.last_angle_speed_PITCH = 0;
	postureCtrlData.last_angle_speed_ROLL = 0;
	postureCtrlData.last_angle_speed_YAW = 0;
	
	//目标角速度
	postureCtrlData.target_angleSpeed_YAW = 0;
	postureCtrlData.target_angleSpeed_PITCH = 0;
	postureCtrlData.target_angleSpeed_ROLL = 0;

	//角速度误差
	postureCtrlData.err_angleSpeed_YAW = 0;
	postureCtrlData.err_angleSpeed_PITCH = 0;
	postureCtrlData.err_angleSpeed_ROLL = 0;
	
	//角速度误差积分
	postureCtrlData.errSum_angleSpeed_YAW = 0;
	postureCtrlData.errSum_angleSpeed_PITCH = 0;
	postureCtrlData.errSum_angleSpeed_ROLL = 0;
	
	//三方向输出
	postureCtrlData.outPITCH = 0;
	postureCtrlData.outROLL = 0;
	postureCtrlData.outYAW = 0;
}


//姿态控制获取数据--输入:三轴融合角度,角速度
void postureCtrl_GetData_2ms_(float PITCH, float ROLL, float YAW, float angleSpeed_PITCH, float angleSpeed_ROLL, float angleSpeed_YAW)
{
	postureCtrlData.YAW = YAW;
	postureCtrlData.PITCH = PITCH;
	postureCtrlData.ROLL = ROLL;
	
	postureCtrlData.angle_speed_YAW = angleSpeed_YAW;
	postureCtrlData.angle_speed_PITCH = angleSpeed_PITCH;
	postureCtrlData.angle_speed_ROLL = angleSpeed_ROLL;
}


//姿态控制四电机输出--输入:
// 2 1 :机头↑
// 3 4 :电机序号
void postureCtrl_Out4Moto_2ms_(float* moto)
{
	//计算角度误差
	postureCtrlData.err_angle_PITCH = postureCtrlData.target_angle_PITCH + flyTask_TargetAngleOut_Pitch() - postureCtrlData.PITCH;
	postureCtrlData.err_angle_ROLL = postureCtrlData.target_angle_ROLL + flyTask_TargetAngleOut_Roll() - postureCtrlData.ROLL;
	postureCtrlData.err_angle_YAW = postureCtrlData.target_angle_YAW + flyTask_TargetAngleOut_Yaw() - postureCtrlData.YAW;
	
	//得到目标角速度
	postureCtrlData.target_angleSpeed_PITCH = postureCtrlData.err_angle_PITCH * 43;
	postureCtrlData.target_angleSpeed_ROLL = postureCtrlData.err_angle_ROLL * 43;
	postureCtrlData.target_angleSpeed_YAW = postureCtrlData.err_angle_YAW * 33;
	
	//计算角速度误差
	postureCtrlData.err_angleSpeed_PITCH = postureCtrlData.target_angleSpeed_PITCH - postureCtrlData.angle_speed_PITCH;
	postureCtrlData.err_angleSpeed_ROLL = postureCtrlData.target_angleSpeed_ROLL - postureCtrlData.angle_speed_ROLL;
	postureCtrlData.err_angleSpeed_YAW = postureCtrlData.target_angleSpeed_YAW - postureCtrlData.angle_speed_YAW;
	
	//角速度误差积分
	postureCtrlData.errSum_angleSpeed_PITCH += postureCtrlData.err_angleSpeed_PITCH * 0.002;
	postureCtrlData.errSum_angleSpeed_ROLL += postureCtrlData.err_angleSpeed_ROLL * 0.002;
	postureCtrlData.errSum_angleSpeed_YAW += postureCtrlData.err_angleSpeed_YAW * 0.002;
	
	//角速度误差积分限幅
	postureCtrlData.errSum_angleSpeed_PITCH = LIMIT(postureCtrlData.errSum_angleSpeed_PITCH, 0, 4000);
	postureCtrlData.errSum_angleSpeed_ROLL = LIMIT(postureCtrlData.errSum_angleSpeed_ROLL, 0, 4000);
	postureCtrlData.errSum_angleSpeed_YAW = LIMIT(postureCtrlData.errSum_angleSpeed_YAW, 0, 4000);
	
	//直接差分计算角加速度
	float accPITCH = postureCtrlData.angle_speed_PITCH - postureCtrlData.last_angle_speed_PITCH;
	float accROLL = postureCtrlData.angle_speed_ROLL - postureCtrlData.last_angle_speed_ROLL;
	float accYAW = postureCtrlData.angle_speed_YAW - postureCtrlData.last_angle_speed_YAW;
		
	//保存上次角速度
	postureCtrlData.last_angle_speed_PITCH = postureCtrlData.angle_speed_PITCH;
	postureCtrlData.last_angle_speed_ROLL = postureCtrlData.angle_speed_ROLL;
	postureCtrlData.last_angle_speed_YAW = postureCtrlData.angle_speed_YAW;
	
	moto[0] = 0;
	moto[1] = 0;
	moto[2] = 0;
	moto[3] = 0;
	
	//计算俯仰输出--目标角度为正电机12+ , 34-
	postureCtrlData.outPITCH = postureCtrlData.err_angleSpeed_PITCH * 0.0964 + postureCtrlData.errSum_angleSpeed_PITCH * 0.3933 - accPITCH*4.7;
	postureCtrlData.outPITCH = LIMIT(postureCtrlData.outPITCH, -80, 80);
	moto[0] += postureCtrlData.outPITCH;
	moto[1] += postureCtrlData.outPITCH;
	moto[2] += -1*postureCtrlData.outPITCH;
	moto[3] += -1*postureCtrlData.outPITCH;
	
	//计算横滚输出--目标角度为正电机23+ , 14-
	postureCtrlData.outROLL = postureCtrlData.err_angleSpeed_ROLL * 0.0965 + postureCtrlData.errSum_angleSpeed_ROLL * 0.3933 - accROLL*4.7;
	postureCtrlData.outROLL = LIMIT(postureCtrlData.outROLL, -80, 80);
	moto[0] += -1*postureCtrlData.outROLL;
	moto[1] += postureCtrlData.outROLL;
	moto[2] += postureCtrlData.outROLL;
	moto[3] += -1*postureCtrlData.outROLL;
	
	//计算偏航输出
	postureCtrlData.outYAW = postureCtrlData.err_angleSpeed_YAW * 0.64 + postureCtrlData.errSum_angleSpeed_YAW * 0.0016 -  accYAW*0.16;
	postureCtrlData.outYAW = LIMIT(postureCtrlData.outYAW, -80, 80);
	moto[0] += postureCtrlData.outYAW;
	moto[1] += -1*postureCtrlData.outYAW;
	moto[2] += postureCtrlData.outYAW;
	moto[3] += -1*postureCtrlData.outYAW;
}




