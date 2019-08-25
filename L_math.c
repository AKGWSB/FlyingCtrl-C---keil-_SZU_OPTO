#include "L_math.h"


//反正切二分搜索查表
int L_arcTan(float x, float y)
{
	int left = 0;
	int right = arcTanListLength;
	int mid;
	float tanValue = x/y;
	
	//迭代10次
	for(int i=0; i<10; i++)
	{
		mid = (left + right)/2;
		
		if(tanValue > arcTan[mid])
		{
			left = mid;
		}
		else if(tanValue < arcTan[mid])
		{
			right = mid;
		}
	}
	
	return mid+1;
}

//匿名快速平方根
float L_sqrt(float number)
{
	long i;
	float x, y;
	const float f = 1.5F;
	x = number * 0.5F;
	y = number;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 );

	y = * ( float * ) &i;
	y = y * ( f - ( x * y * y ) );
	y = y * ( f - ( x * y * y ) );
	return number * y;
}


//余弦查表
float L_cos(float angle)
{
	return cos_[L_abs((int)angle*10)];
}


//快速正弦
float L_sin(float angle)
{
	if(angle >= 0)
	{
		return L_sqrt(1 - (L_cos(angle) * L_cos(angle)));
	}
	else if(angle < 0)
	{
		return -1*L_sqrt(1 - (L_cos(angle) * L_cos(angle)));
	}
	return 0;
}


//
float To_180_(float x)
{
	return (x>180?(x-360):(x<-180?(x+360):x));
}


