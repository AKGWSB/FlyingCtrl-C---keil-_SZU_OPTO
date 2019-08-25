#include "openMv.h"

openMv_DataStruct openMvData;

//init
void openMv_DataInit()
{
	openMvData.rec_counter = 0;
	openMvData.rec_flag = 0;
	openMvData.x = 0;
	openMvData.y = 0;
}


//串口中断接收
void openMv_DataRecive_It(char data)
{
	if(openMvData.rec_counter == 0 && data == '6')
	{
		openMvData.rec_flag = 0;
		openMvData.temp[openMvData.rec_counter] = (s16)data;
		openMvData.rec_counter ++;
	}
	else if(openMvData.rec_counter == 1)
	{
		openMvData.temp[openMvData.rec_counter] = (s16)data;
		openMvData.rec_counter ++;
	}
	else if(openMvData.rec_counter == 2)
	{
		openMvData.temp[openMvData.rec_counter] = (s16)data;
		openMvData.rec_counter = 0;
		openMvData.rec_flag = 1;
	}
}


//协议解包
void openMv_Decode_2ms_()
{
	if(openMvData.rec_flag == 1 && openMvData.temp[0] == '6'  )
	{
		openMvData.x = openMvData.temp[1] - half_screen_length;
		openMvData.y = openMvData.temp[2] - half_screen_hight;
	}
}



