#ifndef __OPENMV_H_

#define __OPENMV_H_

#define half_screen_hight 60
#define half_screen_length 80

typedef   signed short     int int16_t;
typedef int16_t s16;

typedef struct
{
	s16 temp[3];
	int rec_counter;
	int rec_flag;
	s16 x;
	s16 y;
	
}openMv_DataStruct;


//init
void openMv_DataInit();

//串口中断接收
void openMv_DataRecive_It(char data);

//协议解包
void openMv_Decode_2ms_();

#endif


