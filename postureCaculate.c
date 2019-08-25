#include "postureCaculate.h"
#include "L_math.h"

postureCaculate_DataStruct postureCaculateData;

//init
void postureCaculate_Init()
{
	//矫正标志位,计数器
	postureCaculateData.gyroCorrectFlag = 0;
	postureCaculateData.gyroCorrectCounter = 0;
	
	//acc
	postureCaculateData.acc_real_value.x = 0;
	postureCaculateData.acc_real_value.y = 0;
	postureCaculateData.acc_real_value.z = 0;
	postureCaculateData.acc_filter_value.x = 0;
	postureCaculateData.acc_filter_value.y = 0;
	postureCaculateData.acc_filter_value.z = 0;
	postureCaculateData.acc_withoutGravity.x = 0;
	postureCaculateData.acc_withoutGravity.y = 0;
	postureCaculateData.acc_withoutGravity.z = 0;
	
	//gyro
	postureCaculateData.gyro_real_value.x = 0;
	postureCaculateData.gyro_real_value.y = 0;
	postureCaculateData.gyro_real_value.z = 0;
	postureCaculateData.gyro_filter_value.x = 0;
	postureCaculateData.gyro_filter_value.y = 0;
	postureCaculateData.gyro_filter_value.z = 0;
	
	//mag
	postureCaculateData.mag_real_value.x = 0;
	postureCaculateData.mag_real_value.y = 0;
	postureCaculateData.mag_real_value.z = 0;
	postureCaculateData.mag_filter_value.x = 0;
	postureCaculateData.mag_filter_value.y = 0;
	postureCaculateData.mag_filter_value.z = 0;
	
	//加速度计反三角计算角度acc->postureAngle
	postureCaculateData.acc_postureAngle.PITCH = 0;
	postureCaculateData.acc_postureAngle.ROLL = 0;
	postureCaculateData.acc_postureAngle.YAW = 0;
	
	//陀螺仪积分计算角度final->postureAngle
	postureCaculateData.postureAngle.PITCH = 0;
	postureCaculateData.postureAngle.ROLL = 0;
	postureCaculateData.postureAngle.YAW = 0;
	
	//计算误差和
	postureCaculateData.gyroCorrectErrSum.x = 0;
	postureCaculateData.gyroCorrectErrSum.y = 0;
	postureCaculateData.gyroCorrectErrSum.z = 0;
	postureCaculateData.accCorrectErrSum.x = 0;
	postureCaculateData.accCorrectErrSum.y = 0;
	postureCaculateData.accCorrectErrSum.z = 0;
	
	//矫正误差
	postureCaculateData.gyroCorrectErr.x = 0;
	postureCaculateData.gyroCorrectErr.y = 0;
	postureCaculateData.gyroCorrectErr.z = 0;
	postureCaculateData.accCorrectErr.x = 0;
	postureCaculateData.accCorrectErr.x = 0;
	postureCaculateData.accCorrectErr.z = 0;
}


//姿态解算数据获取--输入三轴角速度,加速度原始数据
//陀螺仪最大量程+-2000度/s
//加计最大量程+-8G -- +-32768
void postureCaculate_GetData_2ms_(float gyroX, float gyroY, float gyroZ, float acc_x_real_value, float acc_y_real_value, float acc_z_real_value, float magX, float magY, float magZ)
{
	postureCaculateData.gyro_real_value.x = gyroX;
	postureCaculateData.gyro_real_value.y = gyroY;
	postureCaculateData.gyro_real_value.z = gyroZ;
	
	postureCaculateData.acc_real_value.x = acc_x_real_value;
	postureCaculateData.acc_real_value.y = acc_y_real_value;
	postureCaculateData.acc_real_value.z = acc_z_real_value;
	
	postureCaculateData.mag_real_value.x = magX;
	postureCaculateData.mag_real_value.y = magY;
	postureCaculateData.mag_real_value.z = magZ;
}

//姿态解算数据滤波--周期1ms
void postureCaculate_DataFilter_1ms_()
{
	//加计滤波
	postureCaculateData.acc_filter_value.x += 3.14f * 2 * (postureCaculateData.acc_real_value.x - postureCaculateData.acc_filter_value.x) * 0.001;
	postureCaculateData.acc_filter_value.y += 3.14f * 2 * (postureCaculateData.acc_real_value.y - postureCaculateData.acc_filter_value.y) * 0.001;
	postureCaculateData.acc_filter_value.z += 3.14f * 2 * (postureCaculateData.acc_real_value.z - postureCaculateData.acc_filter_value.z) * 0.001;
	
	//陀螺仪滤波
	postureCaculateData.gyro_filter_value.x += 3.14f * 2 * (postureCaculateData.gyro_real_value.x + postureCaculateData.gyroCorrectErr.x - postureCaculateData.gyro_filter_value.x) * 0.001;
	postureCaculateData.gyro_filter_value.y += 3.14f * 2 * (postureCaculateData.gyro_real_value.y + postureCaculateData.gyroCorrectErr.y - postureCaculateData.gyro_filter_value.y) * 0.001;
	postureCaculateData.gyro_filter_value.z += 3.14f * 2 * (postureCaculateData.gyro_real_value.z + postureCaculateData.gyroCorrectErr.z - postureCaculateData.gyro_filter_value.z) * 0.001;
	
	//电子罗盘滤波
	postureCaculateData.mag_filter_value.x += 3.14f * 20 * (postureCaculateData.mag_real_value.x - postureCaculateData.mag_filter_value.x) * 0.001;
	postureCaculateData.mag_filter_value.y += 3.14f * 20 * (postureCaculateData.mag_real_value.y - postureCaculateData.mag_filter_value.y) * 0.001;
	postureCaculateData.mag_filter_value.z += 3.14f * 20 * (postureCaculateData.mag_real_value.z - postureCaculateData.mag_filter_value.z) * 0.001;
}


//互补滤波姿态解算
void postureCaculate_CaculatePosture_2ms_()
{
	Vec3 acc_ref;
	Vec3 gyro_ref;
	//根据矫正结果修正陀螺仪&加计
	acc_ref.x = postureCaculateData.acc_filter_value.x + postureCaculateData.accCorrectErr.x;
	acc_ref.y = postureCaculateData.acc_filter_value.y + postureCaculateData.accCorrectErr.y;
	acc_ref.z = postureCaculateData.acc_filter_value.z + postureCaculateData.accCorrectErr.z;
	gyro_ref.x = postureCaculateData.gyro_real_value.x + postureCaculateData.gyroCorrectErr.x;
	gyro_ref.y = postureCaculateData.gyro_real_value.y + postureCaculateData.gyroCorrectErr.y;
	gyro_ref.z = postureCaculateData.gyro_real_value.z + postureCaculateData.gyroCorrectErr.z;
	
	
	//加速度计归一化
	float acc_vecMod = 0;
	acc_vecMod += acc_ref.x * acc_ref.x;
	acc_vecMod += acc_ref.y * acc_ref.y;
	acc_vecMod += acc_ref.z * acc_ref.z;
	acc_vecMod = L_sqrt(acc_vecMod);
	
	Vec3 acc_normalize;
	acc_normalize.x = acc_ref.x / acc_vecMod;
	acc_normalize.y = acc_ref.y / acc_vecMod;
	acc_normalize.z = acc_ref.z / acc_vecMod;
	
	//加速度计计算PITCH,精确到0.1度
	if(acc_normalize.x < 0)
	{
		postureCaculateData.acc_postureAngle.PITCH = -0.1 * (float)L_arcTan(L_abs(acc_normalize.x), acc_normalize.z);
	}
	else if(acc_normalize.x >= 0)
	{
		postureCaculateData.acc_postureAngle.PITCH = 0.1 * (float)L_arcTan(acc_normalize.x, acc_normalize.z);
	}
	//加速度计计算ROLL,精确到0.1度
	if(acc_normalize.y < 0)
	{
		postureCaculateData.acc_postureAngle.ROLL = -0.1 * (float)L_arcTan(L_abs(acc_normalize.y), acc_normalize.z);
	}
	else if(acc_normalize.y >= 0)
	{
		postureCaculateData.acc_postureAngle.ROLL = 0.1 * (float)L_arcTan(acc_normalize.y, acc_normalize.z);
	}
	
	
	//陀螺仪积分计算角度
	postureCaculateData.postureAngle.PITCH -= 3.64 * (gyro_ref.y * 0.002) * PI / 180;
	postureCaculateData.postureAngle.ROLL += 3.64 * (gyro_ref.x * 0.002) * PI / 180;
	postureCaculateData.postureAngle.YAW += 3.64 * (gyro_ref.z * 0.002) * PI / 180;
	
	//计算加计可信度权值
	float acc_weight = (4096/acc_vecMod)*(4096/acc_vecMod);
	//积分角度收敛于加计角度
	postureCaculateData.postureAngle.PITCH += 3.14f * 0.15 * acc_weight * (postureCaculateData.acc_postureAngle.PITCH - postureCaculateData.postureAngle.PITCH) * 0.002;
	postureCaculateData.postureAngle.ROLL += 3.14f * 0.15 * acc_weight * (postureCaculateData.acc_postureAngle.ROLL - postureCaculateData.postureAngle.ROLL) * 0.002;  
	
	//计算无重力ACC
	postureCaculateData.acc_withoutGravity.z = ((postureCaculateData.acc_real_value.z + postureCaculateData.accCorrectErr.z) / (L_cos(postureCaculateData.postureAngle.PITCH) * L_cos(postureCaculateData.postureAngle.ROLL)))-4096;
	postureCaculateData.acc_withoutGravity.x = postureCaculateData.acc_real_value.x + postureCaculateData.accCorrectErr.x - ((postureCaculateData.acc_withoutGravity.z+4096) * L_sin(postureCaculateData.postureAngle.PITCH));
	postureCaculateData.acc_withoutGravity.y = postureCaculateData.acc_real_value.y + postureCaculateData.accCorrectErr.y - ((postureCaculateData.acc_withoutGravity.z+4096) * L_sin(postureCaculateData.postureAngle.ROLL));
	
	//-------------------------------------------------------------------------------------------//
	//如果开始矫正,迭代50次矫正
	if(postureCaculateData.gyroCorrectFlag == 1 )
	{
		//迭代
		postureCaculateData.gyroCorrectCounter ++;
		
		//加速度计矫正误差计算
		postureCaculateData.accCorrectErrSum.x += 0.00 - postureCaculateData.acc_filter_value.x;
		postureCaculateData.accCorrectErrSum.y += 0.00 - postureCaculateData.acc_filter_value.y;
		postureCaculateData.accCorrectErrSum.z += 4096 - postureCaculateData.acc_filter_value.z;
		
		//陀螺仪矫正误差计算
		postureCaculateData.gyroCorrectErrSum.x += 0 - postureCaculateData.gyro_real_value.x;
		postureCaculateData.gyroCorrectErrSum.y += 0 - postureCaculateData.gyro_real_value.y;
		postureCaculateData.gyroCorrectErrSum.z += 0 - postureCaculateData.gyro_real_value.z;
		
		//停止矫正
		if(postureCaculateData.gyroCorrectCounter > 50)
		{
			postureCaculateData.gyroCorrectFlag = 2;
			
			//计算均值
			postureCaculateData.accCorrectErr.x = postureCaculateData.accCorrectErrSum.x / 50;
			postureCaculateData.accCorrectErr.y = postureCaculateData.accCorrectErrSum.y / 50;
			postureCaculateData.accCorrectErr.z = postureCaculateData.accCorrectErrSum.z / 50;
			postureCaculateData.gyroCorrectErr.x = postureCaculateData.gyroCorrectErrSum.x / 50;
			postureCaculateData.gyroCorrectErr.y = postureCaculateData.gyroCorrectErrSum.y / 50;
			postureCaculateData.gyroCorrectErr.z = postureCaculateData.gyroCorrectErrSum.z / 50;
			
			//清空陀螺仪积分
			postureCaculateData.postureAngle.PITCH = 0;
			postureCaculateData.postureAngle.ROLL = 0;
			postureCaculateData.postureAngle.YAW = 0;
		}
	}
	//-------------------------------------------------------------------------------------------//
}


//陀螺仪矫正
void postureCaculate_SetGyroCorrect()
{
	postureCaculateData.gyroCorrectFlag = 1;
}






