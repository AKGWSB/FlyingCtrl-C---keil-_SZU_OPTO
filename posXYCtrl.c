#include <posXYCtrl.h>

posXYCtrl_DataStruct posXYCtrlData;

//init
void posXYCtrl_DataInit()
{
	posXYCtrlData.posXYCtrlStartFlag = 0;
	
	posXYCtrlData.posX = 0;
	posXYCtrlData.posY = 0;
	posXYCtrlData.targetSpeedX = 0;
	posXYCtrlData.targetSpeedY = 0;
	posXYCtrlData.speedX = 0;
	posXYCtrlData.speedY = 0;
	posXYCtrlData.err_speedX = 0;
	posXYCtrlData.err_speedY = 0;
	posXYCtrlData.target_roll = 0;
	posXYCtrlData.target_pitch = 0;
}


//位置控制数据接口--输入:posFilterData.posX_filter_value, -1*posFilterData.posY_filter_value, posFilterData.speedX_predict_value*0.0099, posFilterData.speedY_predict_value*0.0099
void posXYCtrl_GetData_2ms_(float posX, float posY, float speedX, float speedY)
{
	posXYCtrlData.posX = posX;
	posXYCtrlData.posY = posY;
	posXYCtrlData.speedX = speedX;
	posXYCtrlData.speedY = speedY;
}


//输出目标横滚角
float posXYCtrl_outputTargetAngle_Roll()
{
	return posXYCtrlData.target_roll;
}


//输出目标俯仰角
float posXYCtrl_outputTargetAngle_Pitch()
{
	return posXYCtrlData.target_pitch;
}

float last_err = 0;

//计算目标角度--外环P内环P
void posXYCtrl_targetAngleCaculate_2ms_()
{
	//计算目标速度
	posXYCtrlData.targetSpeedX = 0.064 * (0 - posXYCtrlData.posX);
	posXYCtrlData.targetSpeedY = 0.121 * (0 - posXYCtrlData.posY);
//	
//	//计算目标速度
//	posXYCtrlData.targetSpeedX = 0;
//	posXYCtrlData.targetSpeedY = 0;
	
	//目标速度限幅
	posXYCtrlData.targetSpeedX = LIMIT(posXYCtrlData.targetSpeedX, -5, 5);
	posXYCtrlData.targetSpeedY = LIMIT(posXYCtrlData.targetSpeedY, -5, 5);
	
	//计算速度误差
	posXYCtrlData.err_speedX = posXYCtrlData.targetSpeedX - posXYCtrlData.speedX;
	posXYCtrlData.err_speedY = posXYCtrlData.targetSpeedY - posXYCtrlData.speedY;
	
	float err_d = posXYCtrlData.err_speedX - last_err;
	
	//计算目标角度
	posXYCtrlData.target_roll = (posXYCtrlData.err_speedX)*1.4 + err_d*2.2;
	posXYCtrlData.target_pitch = (posXYCtrlData.err_speedY*-1);
	
	//目标角度限幅
	posXYCtrlData.target_roll = LIMIT(posXYCtrlData.target_roll, -6, 6);
	posXYCtrlData.target_pitch = LIMIT(posXYCtrlData.target_pitch, -5, 5);
	
	last_err = posXYCtrlData.err_speedX;
}


