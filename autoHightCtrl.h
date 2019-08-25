#ifndef __AUTOHIGHTCTRL_H_

#define __AUTOHIGHTCTRL_H_

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

typedef struct
{
	int modFlag;
	
	float speed_z;
	float hight;
	float acc_z;
	
	float hightTarget;
	float err_hight;
	float speedTarget;
	float err_speed;
	double err_sum_speed;
	float err_speed_last;
	float err_d_speed;
	float output;
	
}hightCtrl_DataStruct;

//-------------------------------------------//

//定高数据初始化
void hightCtrl_DataInit();

//定高数据接口
void hightCtrl_GetData_2ms_(float hight, float speed_z, float acc_z);

//定高油门输出
float hightCtrlOutput_2ms_();

//-------------------------------------------//

//模式状态标志位接口
int hightCtrl_GetModeFlag();

//转到起飞模式
void hightCtrl_ChangeTo_TakeOff();

//转到降落模式
void hightCtrl_ChangeTo_Down();

//转到待机模式
void hightCtrl_ChangeTo_Stay();

//转到就绪模式
void hightCtrl_ChangeTo_Ready();

#endif
