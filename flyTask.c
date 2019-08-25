#include <flyTask.h>

flyTask_DataStruct flyTaskData;


//init
void flyTast_Init()
{
	flyTaskData.timerCNT = 0;
	flyTaskData.state = ctrlOff;
	flyTaskData.task = A;
	flyTaskData.targetAngle.PITCH = 0;
	flyTaskData.targetAngle.ROLL = 0;
	flyTaskData.targetAngle.YAW = 0;
	flyTaskData.posInterfaceX = 0;
	flyTaskData.posInterfaceY = 0;
	flyTaskData.speedInterfaceX = 0;
	flyTaskData.speedInterfaceY = 0;
}


//飞行任务检查
void flyTask_CheckDuty_2ms_()
{
	//如果按下按键，未解锁，并且未起飞(任务A，则起飞
	if(remoterReciveData.remoterCH[4] > 1900 && flyMode_GetMode() == Mode_Ready && flyTaskData.task == A)
	{
		//清空计数器
		flyTaskData.timerCNT = 0;
		//切换到定点识别
		flyTaskData.state = followLine;
		//进入下一个任务
		flyTaskData.task = B;
		//进入起飞
		flyMode_ChangeTo_AutoHightCtrl_TakeOff();
		
		return;
	}
	
	
	//任务B走线
	if(flyTaskData.task == B)
	{
		if(remoterReciveData.remoterCH[4] <= 1900)
		{
			flyTaskData.state = ctrlOff;
			flyTaskData.task = H;
			return;
		}
		
		flyTaskData.timerCNT ++;
		//flyTaskData.timerCNT ++;flyTaskData.timerCNT > 3000
		
		//如果遇到直角直接定点
		if(openmv.circle_flag == 1 && flyTaskData.timerCNT > 2000)
		{
			//清空计数器
			flyTaskData.timerCNT = 0;
			//切换到定点
			flyTaskData.state = back;
			//进入下一个任务
			flyTaskData.task = C;
			//降落
			//flyMode_ChangeTo_AutoHightCtrl_Down();
			return;
		}

		
		return;
	}
	
	
	//任务C刹车
	if(flyTaskData.task == C)
	{
		flyTaskData.timerCNT ++;
		//flyTaskData.timerCNT ++;flyTaskData.timerCNT > 3000
		
		//1s
		if(flyTaskData.timerCNT > 500)
		{
			//清空计数器
			flyTaskData.timerCNT = 0;
			//切换到转弯
			flyTaskData.state = yawTurn;
			//进入下一个任务
			flyTaskData.task = D;
			return;
		}
		return;
	}
	
	//任务D转弯
	if(flyTaskData.task == D)
	{
			if(remoterReciveData.remoterCH[4] <= 1900)
		{
			flyTaskData.state = ctrlOff;
			flyTaskData.task = H;
			return;
		}
		//flyTaskData.timerCNT ++;
		
		//转90度
		if(flyTaskData.targetAngle.YAW <= -89)
		{
			//清空计数器
			flyTaskData.timerCNT = 0;
			//盲飞
			flyTaskData.state = blindFly;
			//进入下一个任务
			flyTaskData.task = E;
		}
		
		return;
	}
	
	
	//任务E盲飞
	if(flyTaskData.task == E)
	{
			if(remoterReciveData.remoterCH[4] <= 1900)
		{
			flyTaskData.state = ctrlOff;
			flyTaskData.task = H;
			return;
		}
		flyTaskData.timerCNT ++;
		
		//如果看到直角,2s
		if(openmv.circle_flag == 1 && flyTaskData.timerCNT > 100)
		{
			//清空计数器
			flyTaskData.timerCNT = 0;
			//切换到定点
			flyTaskData.state = back2;
			//进入下一个任务
			flyTaskData.task = F;
		}
	}
	
		
	if(flyTaskData.task == F)
	{
		flyTaskData.timerCNT ++;
		//flyTaskData.timerCNT ++;flyTaskData.timerCNT > 3000
		
		//1s
		if(flyTaskData.timerCNT > 150)
		{
			//清空计数器
			flyTaskData.timerCNT = 0;
			//切换到转弯
			flyTaskData.state = yawTurn;
			//进入下一个任务
			flyTaskData.task = G;
			return;
		}
		return;
	}
		
	//任务G转弯
	if(flyTaskData.task == G)
	{
			if(remoterReciveData.remoterCH[4] <= 1900)
		{
			flyTaskData.state = ctrlOff;
			flyTaskData.task = H;
			return;
		}
		//flyTaskData.timerCNT ++;
		
		//转90度
		if(flyTaskData.targetAngle.YAW <= -198)
		{
			//清空计数器
			flyTaskData.timerCNT = 0;
			//寻线
			flyTaskData.state = followLine2;
			//进入下一个任务
			flyTaskData.task = H;
		}
		
		return;
	}
	
	//任务H
	if(flyTaskData.task == H)
	{
		flyTaskData.timerCNT ++;
		
		//
		if(flyTaskData.timerCNT > 2000)
		{
			//清空计数器
			flyTaskData.timerCNT = 0;
			//切换到寻线
			flyTaskData.state = followLine2;
			//降落
			flyMode_ChangeTo_AutoHightCtrl_Down();
			//进入下一个任务
			flyTaskData.task = I;
		}
		
		return;
	}
	

}

float anglePitch = -0.6;
void foward(float* angle)
{
	float angle_ = *angle;
	if(angle_ == -0.6)
	{
		angle_ = 0.5;
	}
	else if(angle_ == 0.5)
	{
		angle_ = -0.6;
	}
}

//飞行输出更新
void flyTask_AngleOutPutUpdate_2ms_()
{
	if(flyTaskData.state == ctrlOff)
	{
//		flyTaskData.posInterfaceX = 0;
//		flyTaskData.posInterfaceY = 0;
		flyTaskData.targetAngle.PITCH = 0;
		flyTaskData.targetAngle.ROLL = 0;
//	flyTaskData.targetAngle.YAW = 0;
		return ;
	}
	
	if(flyTaskData.state == pointStay)
	{	
		//切换数据接口
		flyTaskData.posInterfaceX = -1*openmv.circle_x+80;
		flyTaskData.posInterfaceY = openmv.circle_y-20;
		flyTaskData.speedInterfaceX = posFilterData.speedX_d_value;					//互补速度
		flyTaskData.speedInterfaceY = posFilterData.speedY_d_value;	
		
		posXYCtrl_GetData_2ms_(flyTaskData.posInterfaceX, flyTaskData.posInterfaceY, flyTaskData.speedInterfaceX, flyTaskData.speedInterfaceY);
		
		//定点识别具有高度限制
		if(posFilterData.posZ_real_value > 300)
		{
			flyTaskData.targetAngle.PITCH = posXYCtrl_outputTargetAngle_Pitch();
			flyTaskData.targetAngle.ROLL = posXYCtrl_outputTargetAngle_Roll();
			//flyTaskData.targetAngle.YAW = 0;
			return;
		}	
		else
		{
			flyTaskData.targetAngle.PITCH = 0;
			flyTaskData.targetAngle.ROLL = 0;
			flyTaskData.targetAngle.YAW = 0;
			return;
		}
		return;
	}
	
	if(flyTaskData.state == followLine)
	{
		//切换数据接口
		flyTaskData.posInterfaceX = -1*openmv.mid_x+80;											//边线坐标
		flyTaskData.posInterfaceY = -20;																		//给目标位置=目标给速度	
		flyTaskData.speedInterfaceX = posFilterData.speedX_predict_value*0.03;					//互补速度
		flyTaskData.speedInterfaceY = OF_Data.speedY_filter_value*0.025;		//光流速度
		
		posXYCtrl_GetData_2ms_(flyTaskData.posInterfaceX, flyTaskData.posInterfaceY, flyTaskData.speedInterfaceX, flyTaskData.speedInterfaceY);
		
		//寻线具有高度限制
		if(posFilterData.posZ_real_value > 400)
		{	
			//flyTaskData.targetAngle.PITCH = posXYCtrl_outputTargetAngle_Pitch();
			flyTaskData.targetAngle.PITCH = -0.10;
			flyTaskData.targetAngle.ROLL = posXYCtrl_outputTargetAngle_Roll()-0.0;
			//flyTaskData.targetAngle.YAW = 0;
			return;
		}
		else
		{
			flyTaskData.targetAngle.PITCH = -2.40;
			flyTaskData.targetAngle.ROLL = 0;
			//flyTaskData.targetAngle.YAW = 0;
			return;
		}
		
		
		return;
	}
	
	if(flyTaskData.state == followLine2)
	{
		//切换数据接口
		flyTaskData.posInterfaceX = -1*openmv.mid_x+80;											//边线坐标
		flyTaskData.posInterfaceY = -20;																		//给目标位置=目标给速度	
		flyTaskData.speedInterfaceX = posFilterData.speedX_predict_value*0.03;					//互补速度
		flyTaskData.speedInterfaceY = OF_Data.speedY_filter_value*0.025;		//光流速度
		
		posXYCtrl_GetData_2ms_(flyTaskData.posInterfaceX, flyTaskData.posInterfaceY, flyTaskData.speedInterfaceX, flyTaskData.speedInterfaceY);
		
		//寻线具有高度限制
		if(posFilterData.posZ_real_value > 400)
		{	
			//flyTaskData.targetAngle.PITCH = posXYCtrl_outputTargetAngle_Pitch();
			flyTaskData.targetAngle.PITCH = -1.0;
			flyTaskData.targetAngle.ROLL = posXYCtrl_outputTargetAngle_Roll()-0.0;
			//flyTaskData.targetAngle.YAW = 0;
			return;
		}
		else
		{
			flyTaskData.targetAngle.PITCH = -2.40;
			flyTaskData.targetAngle.ROLL = 0;
			//flyTaskData.targetAngle.YAW = 0;
			return;
		}
		
		
		return;
	}
	
	if(flyTaskData.state == yawTurn)
	{
		flyTaskData.targetAngle.PITCH = 0;
		flyTaskData.targetAngle.ROLL = -0;
		flyTaskData.targetAngle.YAW -= 0.25;
		
		return;
	}
	
	
	if(flyTaskData.state == blindFly)
	{
		//切换数据接口
		flyTaskData.posInterfaceX = -1*openmv.mid_x+80;											//
		flyTaskData.posInterfaceY = -20;																		//给目标位置=目标给速度	
		flyTaskData.speedInterfaceX = posFilterData.speedX_predict_value*0.03;					//
		flyTaskData.speedInterfaceY = 0;		
		
		posXYCtrl_GetData_2ms_(flyTaskData.posInterfaceX, flyTaskData.posInterfaceY, flyTaskData.speedInterfaceX, flyTaskData.speedInterfaceY);
		
		foward(&anglePitch);
		//flyTaskData.targetAngle.PITCH = posXYCtrl_outputTargetAngle_Pitch();
		flyTaskData.targetAngle.PITCH = -1.0;
		flyTaskData.targetAngle.ROLL = posXYCtrl_outputTargetAngle_Roll();
//		flyTaskData.targetAngle.YAW -= 0.5;
		
		return;
	}
	
	if(flyTaskData.state == left)
	{
		flyTaskData.targetAngle.PITCH = 0;
		flyTaskData.targetAngle.ROLL = -2.5;
		flyTaskData.targetAngle.YAW = 0;
		return;
	}
	
	if(flyTaskData.state == right)
	{
		flyTaskData.targetAngle.PITCH = posXYCtrl_outputTargetAngle_Pitch();
		flyTaskData.targetAngle.ROLL = 2.5;
		flyTaskData.targetAngle.YAW = 0;
		return;
	}
	
	if(flyTaskData.state == forward)
	{
		flyTaskData.targetAngle.PITCH = -2.5;
		flyTaskData.targetAngle.ROLL = posXYCtrl_outputTargetAngle_Roll();
		flyTaskData.targetAngle.YAW = 0;
		return;
	}
	
	if(flyTaskData.state == back)
	{
		//切换数据接口
		flyTaskData.posInterfaceX = -1*openmv.mid_x+80;											//边线坐标
		flyTaskData.posInterfaceY = -20;																		//给目标位置=目标给速度	
		flyTaskData.speedInterfaceX = posFilterData.speedX_predict_value*0.03;					//互补速度
		flyTaskData.speedInterfaceY = OF_Data.speedY_filter_value*0.025;		//光流速度
		
		posXYCtrl_GetData_2ms_(flyTaskData.posInterfaceX, flyTaskData.posInterfaceY, flyTaskData.speedInterfaceX, flyTaskData.speedInterfaceY);
		
		flyTaskData.targetAngle.PITCH = 3.0;
		flyTaskData.targetAngle.ROLL = posXYCtrl_outputTargetAngle_Roll();
		//flyTaskData.targetAngle.YAW = 0;
		return;
	}
	
	if(flyTaskData.state == back2)
	{
		//切换数据接口
		flyTaskData.posInterfaceX = -1*openmv.mid_x+80;											//边线坐标
		flyTaskData.posInterfaceY = -20;																		//给目标位置=目标给速度	
		flyTaskData.speedInterfaceX = posFilterData.speedX_predict_value*0.03;					//互补速度
		flyTaskData.speedInterfaceY = OF_Data.speedY_filter_value*0.025;		//光流速度
		
		posXYCtrl_GetData_2ms_(flyTaskData.posInterfaceX, flyTaskData.posInterfaceY, flyTaskData.speedInterfaceX, flyTaskData.speedInterfaceY);
		
		flyTaskData.targetAngle.PITCH = 6.5;
		flyTaskData.targetAngle.ROLL = posXYCtrl_outputTargetAngle_Roll()-0.5;
		//flyTaskData.targetAngle.YAW = 0;
		return;
	}

}


//输出目标俯仰角
float flyTask_TargetAngleOut_Pitch()
{
	return flyTaskData.targetAngle.PITCH;
}


//输出目标横滚角
float flyTask_TargetAngleOut_Roll()
{
	return flyTaskData.targetAngle.ROLL;
}


//输出目标偏航角
float flyTask_TargetAngleOut_Yaw()
{
	return flyTaskData.targetAngle.YAW;
}

