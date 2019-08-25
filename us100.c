#include "us100.h"
#include "Drv_Uart.h"


int us100_distance = 0;
int us100_temp;
int us100_rc_flag = 0;

void getData(unsigned char data)
{
	us100_temp *= 256;
	us100_temp += (int)data;
	us100_rc_flag ++;
	
	if(us100_rc_flag == 2)
	{
		us100_rc_flag = 0;
		us100_distance = us100_temp;
	}
}

void us100_send_20ms_()
{
	u8 temp[3];
	temp[0] = 0xe8;
	temp[1] = 0x02;
	temp[2] = 0xbc;
	Drv_Uart5SendBuf(temp, 3);
}





