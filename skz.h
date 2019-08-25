#ifndef __SKZ_H__

#define __SKZ_H__

typedef   signed short     int int16_t;
typedef int16_t s16;

typedef struct
{
	char tempData[17];
	int tempDistance;
	s16 distance;
	int recStep;
	int recFinishFlag;
	int dis_ref_flag;
	
}skz_DataStruct;

//init
void skz_DataInit();

//串口接收一个字符
void skz_DataRecive_It(char data);

//接收数据解码
void skz_DataDecode_10ms_();

#endif 




