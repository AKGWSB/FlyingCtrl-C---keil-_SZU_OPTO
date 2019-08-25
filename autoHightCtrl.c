#include <autoHightCtrl.h>

hightCtrl_DataStruct hightCtrlData;

//定高数据初始化
void hightCtrl_DataInit()
{
	hightCtrlData.hight = 0;
	hightCtrlData.modFlag = 0;
	hightCtrlData.speed_z = 0;
	hightCtrlData.acc_z = 0;
	
	hightCtrlData.hightTarget = 0;
	hightCtrlData.err_hight = 0;
	hightCtrlData.speedTarget = 0;
	hightCtrlData.err_speed = 0;
	hightCtrlData.err_sum_speed = 0;
	hightCtrlData.err_speed_last = 0;
	hightCtrlData.err_d_speed = 0;
	hightCtrlData.output = 0;
}


//定高数据接口--输入:posFilterData.posZ_filter_value*0.95, posFilterData.speedZ_predict_value*0.22
void hightCtrl_GetData_2ms_(float hight, float speed_z, float acc_z)
{
	hightCtrlData.hight = hight;
	hightCtrlData.speed_z = speed_z;
	hightCtrlData.acc_z = acc_z;
}


//模式状态标志位接口
int hightCtrl_GetModeFlag()
{
	return hightCtrlData.modFlag;
}


//转到起飞模式
void hightCtrl_ChangeTo_TakeOff()
{
	hightCtrlData.modFlag = 1;
}


//转到降落模式
void hightCtrl_ChangeTo_Down()
{
	hightCtrlData.modFlag = 2;
}


//转到待机模式
void hightCtrl_ChangeTo_Stay()
{
	hightCtrlData.modFlag = 0;
}


//转到就绪模式
void hightCtrl_ChangeTo_Ready()
{
	hightCtrlData.modFlag = 3;
}

//---------------------------------------------------------------------------------------------//
float lasterr_h = 0;
float d = 0;

//定高油门输出--外环P内环PI
float hightCtrlOutput_2ms_()
{

	//计算误差
	hightCtrlData.err_hight = hightCtrlData.hightTarget - hightCtrlData.hight;
	d = hightCtrlData.err_hight - lasterr_h;
	hightCtrlData.speedTarget = hightCtrlData.err_hight*0.4 - 0.51 * hightCtrlData.speed_z ;
	
	lasterr_h = hightCtrlData.err_hight;
	
	hightCtrlData.err_speed = hightCtrlData.speedTarget  - hightCtrlData.speed_z ;
	
	//速度误差积分/限幅
	hightCtrlData.err_sum_speed += hightCtrlData.err_speed * 0.02;
	hightCtrlData.err_sum_speed = LIMIT(hightCtrlData.err_sum_speed, -80000, 80000);
	
	hightCtrlData.err_d_speed = hightCtrlData.err_speed - hightCtrlData.err_speed_last;
	
	//PI计算油门
	//hightCtrlData.output = hightCtrlData.err_speed * 23 + hightCtrlData.err_sum_speed * 0.000 - hightCtrlData.acc_z*0.16;
	hightCtrlData.output = hightCtrlData.err_speed * 0.5 + LIMIT(hightCtrlData.err_sum_speed * 0.035, -400, 400) ;
	
	hightCtrlData.err_speed_last = hightCtrlData.err_speed;
	
	//return (LIMIT(hightCtrlData.output*0.02, 0, 100) + 600);
	return (hightCtrlData.output + 600);
	
//	//待机模式
//	if(hightCtrlData.modFlag == 0)
//	{
//		hightCtrl_DataInit();
//		return 0;
//	}
//	//就绪模式,电机低速
//	else if(hightCtrlData.modFlag == 3)
//	{
//		hightCtrl_DataInit();
//		return 200;
//	}
//	
//	//着陆判断,清除所有标志位和积分
//	if(hightCtrlData.modFlag == 2 && hightCtrlData.hight < 190)
//	{
//		hightCtrl_DataInit();
//		return 0;
//	}
//	//降落
//	else if(hightCtrlData.modFlag == 2 && hightCtrlData.hight >= 300)
//	{
//		
//		if(hightCtrlData.hightTarget >= -40)
//		{
//			hightCtrlData.hightTarget -= 0.3;
//		}
//		
//		return (LIMIT(hightCtrlData.output*0.02, -10, 70) + 540);
//	}
//	//起飞
//	else if(hightCtrlData.modFlag == 1)
//	{ 
//		
//		if(hightCtrlData.hightTarget <= 1000)
//		{
//			hightCtrlData.hightTarget += 1.5;
//		}
//		
//		return (LIMIT(hightCtrlData.output*0.02, 0, 100) + 570);
//	}
	
	//return 0;
}














