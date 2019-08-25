#include "flyMode.h"

flyMode_DataStruct flyModeData;

//init
void flyMode_Init()
{
	flyModeData.thr_out[0] = 0;
	flyModeData.thr_out[1] = 0;
	flyModeData.thr_out[2] = 0;
	flyModeData.thr_out[3] = 0;
	
	//开机自锁
	flyModeData.Mode = Mode_Lock;
}


//切换到锁定模式
void flyMode_ChangeTo_Lock()
{
	flyModeData.Mode = Mode_Lock;
}


//切换到就绪模式
void flyMode_ChangeTo_Ready()
{
	flyModeData.Mode = Mode_Ready;
}


//切换到定高模式-起飞
void flyMode_ChangeTo_AutoHightCtrl_TakeOff()
{
	flyModeData.Mode = Mode_AutoHightCtrl_TakeOff;
}


//切换到定高模式-降落
void flyMode_ChangeTo_AutoHightCtrl_Down()
{
	flyModeData.Mode = Mode_AutoHightCtrl_Down;
}


//切换到定点模式
void flyMode_ChangeTo_AutoTrace()
{
	flyModeData.Mode = Mode_AutoTrace;
}


//
void flyMode_ChangeTo_mod1()
{
	flyModeData.Mode = Mode_mod1;
}


//
void flyMode_ChangeTo_mod2()
{
	flyModeData.Mode = Mode_mod2;
}


//四油门输出,未限幅
void flyMode_Thr4Out_2ms_(float* thr)
{
	//---------------------------------------------------------//
	//如果是锁定状态
	if(flyModeData.Mode == Mode_Lock)
	{
		for(int i=0; i<4; i++)
		{
			//清积分
			hightCtrl_DataInit();
			postureCtrl_Init();
			thr[i] = 0;
		}
		return;
	}
	//---------------------------------------------------------//
	//如果是就绪状态
	else if(flyModeData.Mode == Mode_Ready)
	{
		//清积分
		hightCtrl_DataInit();
		postureCtrl_Init();
		for(int i=0; i<4; i++)
		{
			thr[i] = 150;
		}
		//记录当前偏航角
		postureCtrlData.target_angle_YAW = postureCtrlData.YAW;
		return;
	}
	//---------------------------------------------------------//
	//如果是自动定高模式-起飞
	else if(flyModeData.Mode == Mode_AutoHightCtrl_TakeOff)
	{
		if(hightCtrlData.hightTarget <= 1000)
		{
			hightCtrlData.hightTarget += 2.0;
		}
	}
	//---------------------------------------------------------//
	//如果是自动定高模式-降落
	else if(flyModeData.Mode == Mode_AutoHightCtrl_Down)
	{
		if(hightCtrlData.hightTarget >= -440)
		{
			hightCtrlData.hightTarget -= 1.0;
		}
		
		//如果已经降落,跳出定高控制,锁定机体
		if(hightCtrlData.hight < 250 && hightCtrlData.hightTarget <= -440)
		{
			//清除积分
			hightCtrl_DataInit();
			postureCtrl_Init();
			flyMode_ChangeTo_Lock();
			return;
		}
	}
	//---------------------------------------------------------//
	//如果是模式1
	else if(flyModeData.Mode == Mode_mod1)
	{
		
	}
	//---------------------------------------------------------//
	//如果是模式2
	else if(flyModeData.Mode == Mode_mod2)
	{
		
	}
	
	//---------------------------------------------------------//

	//计算定高控制油门
	float thr_value = LIMIT(hightCtrlOutput_2ms_(), 0, 799);
		
	//计算姿态控制油门
	float posture_value[4];
	postureCtrl_Out4Moto_2ms_(posture_value);
		
	//计算总油门
	float all_thr_ave = (posture_value[0]+posture_value[1]+posture_value[2]+posture_value[3])/4;
	for(int i=0; i<4; i++)
	{
		thr[i] = thr_value + LIMIT(posture_value[i], -200, 200) - all_thr_ave;
	}
}


//得到当前模式
int flyMode_GetMode()
{
	return flyModeData.Mode;
}



