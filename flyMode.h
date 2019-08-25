#include <L_Link.h>

#ifndef __FLYMOD_H__

#define __FLYMOD_H__

typedef enum 
{
	Mode_Lock,
	Mode_Ready,
	Mode_AutoHightCtrl_TakeOff,
	Mode_AutoHightCtrl_Down,
	Mode_AutoTrace,
	Mode_mod1,
	Mode_mod2
}ModeEnum;

typedef struct
{
	float thr_out[4];
	ModeEnum Mode;
	
}flyMode_DataStruct;

//init
void flyMode_Init();

//切换到锁定模式
void flyMode_ChangeTo_Lock();

//切换到就绪模式
void flyMode_ChangeTo_Ready();

//切换到定高模式-起飞
void flyMode_ChangeTo_AutoHightCtrl_TakeOff();

//切换到定高模式-降落
void flyMode_ChangeTo_AutoHightCtrl_Down();

//切换到定点模式
void flyMode_ChangeTo_AutoTrace();

//
void flyMode_ChangeTo_mod1();

//
void flyMode_ChangeTo_mod2();

//四油门输出,未限幅
void flyMode_Thr4Out_2ms_(float* thr);

//得到当前模式
int flyMode_GetMode();

#endif





