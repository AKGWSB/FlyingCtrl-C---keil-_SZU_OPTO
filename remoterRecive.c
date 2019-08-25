#include <remoterRecive.h>
#include "L_math.h"
#include "posFilter.h"

remoterRecive_DataStruct remoterReciveData;

//init
void remoterRecive_Init()
{
	remoterReciveData.Gyro_ref_flag = 0;
	remoterReciveData.all_out_ctrl = 1;
	
	remoterReciveData.isLock = 1;
	remoterReciveData.lockCounter = 0;
	
	remoterReciveData.remoterCorrectCounter = 0;
	remoterReciveData.remoterCorrectFlag = 0;
	remoterReciveData.PITCH_Correct = 0;
	remoterReciveData.ROLL_Correct = 0;
	
	remoterReciveData.ch0_correct = 0;
	remoterReciveData.ch1_correct = 0;
}


//接收机信号输入--输入:八通道捕获PWM原始数据
void remoterRecive_GetData_2ms_(s16* CH_In)
{
	for(int i=0; i<8; i++)
	{
		remoterReciveData.remoterCH[i] = CH_In[i];
	}
}


//接收机信号滤波
void remoterRecive_CH_filter_1ms_()
{
	for(int i=0; i<8; i++)
	{
		remoterReciveData.remoterCH_filter[i] += (s16)( 6 * (remoterReciveData.remoterCH[i] - remoterReciveData.remoterCH_filter[i]) * 0.002);
	}
}


//检查当前模式
void remoterRecive_FlyModDuty_2ms_()
{
	//6050矫正
	if(remoterReciveData.remoterCH[0] > 1800 && remoterReciveData.remoterCH[1] > 1800 && postureCaculateData.gyroCorrectFlag == 0)
	{
		//矫正位:置1
		postureCaculate_SetGyroCorrect();
	}
	
	//解锁:如果已经锁定，并且矫正过6050
	if(remoterReciveData.remoterCH[2] < 1200 && remoterReciveData.remoterCH[3] > 1800 && remoterReciveData.isLock == 1 && postureCaculateData.gyroCorrectFlag == 2)
	{
		remoterReciveData.lockCounter ++;
		//超过500ms,开锁
		if(remoterReciveData.lockCounter > 250)
		{
			remoterReciveData.isLock = 0;
		}
	}		
	
	
	//如果已经解锁但是没有矫正遥控器
	if(remoterReciveData.isLock == 0 && remoterReciveData.remoterCorrectFlag == 0)
	{
		//迭代矫正遥控器
		remoterRecive_remoterCorrect();
	}
	//如果矫正完成,一直矫正通道值
	else if(remoterReciveData.remoterCorrectFlag == 1)
	{
		remoterReciveData.ch0_correct = remoterReciveData.remoterCH[0] + remoterReciveData.ROLL_Correct;
		remoterReciveData.ch1_correct = remoterReciveData.remoterCH[1] + remoterReciveData.PITCH_Correct;
		
		//如果解锁完成,但是没有发出起飞指令,切换到就绪模式
		if(flyMode_GetMode() == Mode_Lock && remoterReciveData.remoterCH[6] < 1500)
		{
			flyMode_ChangeTo_Ready();
		}
	}
	
	
	//如果矫正完成,已经解锁,映射目标角度控制+-30°,+-180°
	if(remoterReciveData.remoterCorrectFlag == 1 && remoterReciveData.isLock == 0)
	{
		postureCtrlData.target_angle_PITCH = LIMIT((1500-remoterReciveData.ch1_correct)*0.1, -30, 30);
		postureCtrlData.target_angle_ROLL  = LIMIT((remoterReciveData.ch0_correct-1500)*0.1, -30, 30);
		//postureCtrlData.target_angle_YAW = LIMIT((remoterReciveData.remoterCH[3]-1500)*0.02, -20, 20);
		//死区
		if(remoterReciveData.remoterCH[3] < 1400 || remoterReciveData.remoterCH[3] > 1600)
		{
			postureCtrlData.target_angle_YAW += LIMIT((remoterReciveData.remoterCH[3]-1500), -400, 400)*0.001;
		}
	}
	
	
	//定点识别
	if(remoterReciveData.remoterCH[4] > 1900 && posFilterData.posZ_real_value > 300)
	{
		posXYCtrlData.posXYCtrlStartFlag = 1;
	}
	else
	{
		posXYCtrlData.posXYCtrlStartFlag = 0;
	}
	
	
	//定高:起飞--就绪状态下起飞
	if((remoterReciveData.remoterCH[6] > 1500) && flyMode_GetMode() == Mode_Ready)
	{
		flyMode_ChangeTo_AutoHightCtrl_TakeOff();
	}	
	//如果已经起飞，拉杆打下，切换到降落
	else if((remoterReciveData.remoterCH[6] < 1500) && flyModeData.Mode == Mode_AutoHightCtrl_TakeOff)
	{
		flyMode_ChangeTo_AutoHightCtrl_Down();
	}
	
	
	//紧急停机
	if(remoterReciveData.remoterCH[5] > 1500)
	{
		remoterReciveData.all_out_ctrl = 0;
	}
	else
	{
		remoterReciveData.all_out_ctrl = 1;
	}
	
}


//是否紧急停机--输出
short remoterRecive_Emergency_out_()
{
	return remoterReciveData.all_out_ctrl;
}


//解锁控制,双保险
short remoterRecive_IsLock_out_()
{
	return (1-remoterReciveData.isLock);
}


//矫正遥控器PITCH/ROLL舵量
void remoterRecive_remoterCorrect()
{
	//如果没有矫正遥控器
	if(remoterReciveData.remoterCorrectFlag == 0)
	{
		remoterReciveData.remoterCorrectCounter++;
		remoterReciveData.PITCH_Correct += 1500 - remoterReciveData.remoterCH[1];
		remoterReciveData.ROLL_Correct += 1500 - remoterReciveData.remoterCH[0];
		
		//迭代10次
		if(remoterReciveData.remoterCorrectCounter >10)
		{
			remoterReciveData.remoterCorrectFlag = 1;
			remoterReciveData.PITCH_Correct /= 10;
			remoterReciveData.ROLL_Correct /= 10;
		}
	}
}



