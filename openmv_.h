#ifndef _OPENMV_H
#define _OPENMV_H

typedef   signed short     int int16_t;
typedef int16_t s16;

typedef struct
{
	int mid_x;
	int mid_y;
	int target_angle;
	int circle_x;
	int circle_y;
	int circle_flag;

	char data[18];
	int recstep;
	int recFinishFlag;

}openmv_Data;


void openmv_DataInit();

void openmv_DataRecive_It(char data);

void openmv_DataDecode_11ms_();
















#endif