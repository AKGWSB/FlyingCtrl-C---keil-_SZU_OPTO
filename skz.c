#include <skz.h>

skz_DataStruct skzData;

//init
void skz_DataInit()
{
	skzData.distance = 0;
	skzData.dis_ref_flag = 0;
	skzData.recFinishFlag = 0;
	skzData.recStep = 0;
	skzData.tempDistance = 0;
}

//接收数据解码
void skz_DataDecode_10ms_()
{
	//如果没有在接收中，并且上一次已经接收完成
	if(skzData.recFinishFlag)
	{
		int i;
		for(i=0; i<4; i++)
		{
			//如果是数字
			if(skzData.tempData[i+9] >= '0' 
			&& skzData.tempData[i+9] <= '9')
			{
				skzData.distance = skzData.distance*16 + (skzData.tempData[i+9]-48);
			}
			//如果是字母
			else if(skzData.tempData[i+9] >= 'A' 
					 && skzData.tempData[i+9] <= 'Z')
			{
				skzData.distance = skzData.distance*16 + (skzData.tempData[i+9]-(int)('A')+10);
			}
		}
		skzData.dis_ref_flag = 0;
	}
	else
	{
		return;
	}
}


//串口接收数据
void skz_DataRecive_It(char data)
{
	//接收包头
	if(data == '~' && skzData.recStep == 0)
	{
		skzData.recFinishFlag = 0;				//清除标志位
		skzData.tempData[skzData.recStep] = data;
		skzData.recStep++;
	}
	//接收协议内容
	else if( skzData.recStep <=16 && skzData.recStep >= 1)
	{
		skzData.tempData[skzData.recStep] = data;
		skzData.recStep++;
	}
	else if( skzData.recStep == 17 && data == '\r')
	{
		skzData.recStep++;
	}
	//结束接收
	else if( skzData.recStep == 18 && data == '\n')
	{
		skzData.recStep = 0;
		skzData.recFinishFlag = 1;	
	}
}

