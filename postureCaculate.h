#ifndef __POSTURECACULATE_H__

#define __POSTURECACULATE_H__

//三维向量
typedef struct
{
	float x;
	float y;
	float z;
	
}Vec3;

//姿态角
typedef struct
{
	float PITCH;
	float ROLL;
	float YAW;
	
}postureAngle;


typedef struct
{
	//加速度计
	Vec3 acc_real_value;
	Vec3 acc_filter_value;
	Vec3 acc_withoutGravity;
	
	//陀螺仪角速度
	Vec3 gyro_real_value;
	Vec3 gyro_filter_value;
	
	//电子罗盘
	Vec3 mag_real_value;
	Vec3 mag_filter_value;
	
	//加速度计预测姿态角
	postureAngle acc_postureAngle;
	
	//最终姿态角
	postureAngle postureAngle;
	
	//陀螺仪矫正标志位
	short gyroCorrectFlag;
	short gyroCorrectCounter;
	
	//陀螺仪,加速度计偏差
	Vec3 gyroCorrectErrSum;
	Vec3 accCorrectErrSum;
	Vec3 gyroCorrectErr;
	Vec3 accCorrectErr;
	
}postureCaculate_DataStruct;


//init
void postureCaculate_Init();


//姿态解算数据获取--输入三轴角速度,加速度原始数据
//陀螺仪最大量程+-2000度/s
//加计最大量程+-8G -- +-32768
void postureCaculate_GetData_2ms_(float gyroX, float gyroY, float gyroZ, float acc_x_real_value, float acc_y_real_value, float acc_z_real_value, float magX, float magY, float magZ);


//姿态解算数据滤波--周期1ms
void postureCaculate_DataFilter_1ms_();


//互补滤波姿态解算
void postureCaculate_CaculatePosture_2ms_();

//陀螺仪矫正
void postureCaculate_SetGyroCorrect();

#endif


