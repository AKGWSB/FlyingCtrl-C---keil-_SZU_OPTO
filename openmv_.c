#include "openmv_.h"

openmv_Data openmv;


void openmv_DataInit()//初始化
{
	openmv.recFinishFlag = 0;
	openmv.recstep = 0;
	openmv.target_angle = 0;
	openmv.mid_x = 0;
	openmv.mid_y = 0;
	openmv.circle_x = 0;
	openmv.circle_y = 0;
	openmv.circle_flag = 0;


}



void openmv_DataDecode_11ms_()//判断两种类型
{
	int i, x_last, y_last;
	if (openmv.recFinishFlag == 1)
	{

			x_last = openmv.circle_x;
			y_last = openmv.circle_y;

		
			openmv.mid_x = 0;
			openmv.mid_y = 0;
			openmv.circle_x = 0;
			openmv.circle_y = 0;
			openmv.circle_flag = 0;
			openmv.target_angle = 0;

			for (i = 0; i < 3; i++)
			{
				openmv.mid_x *= 10;
				openmv.mid_y *= 10;
				openmv.target_angle *= 10;
				openmv.circle_x *= 10;
				openmv.circle_y *= 10;
				openmv.mid_x += ((s16)(openmv.data[1 + i] - '0'));
				openmv.mid_y += ((s16)(openmv.data[4 + i] - '0'));
				openmv.target_angle += ((s16)(openmv.data[7 + i] - '0'));
				openmv.circle_x += ((s16)(openmv.data[10 + i] - '0'));
				openmv.circle_y += ((s16)(openmv.data[13 + i] - '0'));
			}
			openmv.circle_flag = (s16)(openmv.data[16] - '0');
			openmv.target_angle -= 90;
			if (openmv.circle_flag == 0)
			{
				openmv.circle_x = x_last;
				openmv.circle_y = y_last;
			}
			
		

		openmv.recFinishFlag = 0;
	}

	else
	{
		return;

	}
}

void openmv_DataRecive_It(char data)//接收单个函数
{
	if ( (openmv.recstep == 0) && (data == '#'))
	{
		openmv.data[openmv.recstep] = data;
		openmv.recstep++;
	}
	
	else if ((openmv.recstep < 17) && (openmv.recstep >= 1))
	{
		openmv.data[openmv.recstep] = data;
		openmv.recstep++;
	}

	else if(data == '#' && openmv.recstep == 17)
	{
		openmv.recFinishFlag = 1;
		openmv.recstep = 0;
	}

}
