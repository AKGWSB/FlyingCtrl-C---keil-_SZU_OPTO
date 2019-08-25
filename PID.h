#ifndef __PID_H_

#define __PID_H_

typedef   signed short     int int16_t;
typedef int16_t s16;

//单个PID结构体
typedef struct
{
	float kp;
	float ki;
	float kd;
}PID_Group;


//整个PID
typedef struct
{
	PID_Group group01;
	PID_Group group02;
	PID_Group group03;
	PID_Group group04;
	PID_Group group05;
	PID_Group group06;
	PID_Group group07;
	PID_Group group08;
	PID_Group group09;
	PID_Group group10;
	PID_Group group11;
	PID_Group group12;
	PID_Group group13;
	PID_Group group14;
	PID_Group group15;
	PID_Group group16;
	
	s16 tempData[30];
	s16 usartData[30];
	s16 recStep;
	
	int OpenmvValue;
	
}PID_DataStruct;


//init
void PID_InitOneGroup(PID_Group group);

//init
void PID_Init();

//串口接收一位数据
void PID_DataRecive_It(char data);


//PID设置协议解包
void PID_SetAgreementDecode();


#endif 

