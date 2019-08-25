#ifndef __POSXYCTRL_H__

#define __POSXYCTRL_H__

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

typedef struct 
{
	int posXYCtrlStartFlag;
	
	float posX;
	float posY;
	float targetSpeedX;
	float targetSpeedY;
	float speedX;
	float speedY;
	float err_speedX;
	float err_speedY;
	float target_roll;
	float target_pitch;
	short startFlag;
	
}posXYCtrl_DataStruct;

//init
void posXYCtrl_DataInit();

//位置控制数据接口
void posXYCtrl_GetData_2ms_(float posX, float posY, float speedX, float speedY);

//得到输出目标横滚角
float posXYCtrl_outputTargetAngle_Roll();

//得到输出目标俯仰角
float posXYCtrl_outputTargetAngle_Pitch();

//计算目标角度
void posXYCtrl_targetAngleCaculate_2ms_();

//开始
void posXYCtrl_Start();

//结束
void posXYCtrl_Eng();

#endif




