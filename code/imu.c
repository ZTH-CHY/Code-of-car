#include"imu.h"

uint8 IMUOK;
IMU gyro;
// ****************************陀螺仪校正参数****************************
//零漂
int16 zero_gyro_x,zero_gyro_y,zero_gyro_z;
//零漂和
float gyro_x_sum,gyro_y_sum,gyro_z_sum;
//陀螺仪滤波用
int16 icm42688_gyro_x_old,icm42688_gyro_y_old,icm42688_gyro_z_old;
int16 imu_capture=0;
// ***********************************************************************

void imu_init(void){//IMU初始化
    Init_ICM42688();

    //计算陀螺仪零漂
    gyro_x_sum=0;
    gyro_y_sum=0;
    gyro_z_sum=0;

    for(uint16 i;i<400;i++){
        system_delay_ms(5);
        Get_Gyro_ICM42688();     
        gyro_x_sum+=icm42688_gyro_x;
        gyro_y_sum+=icm42688_gyro_y;
        gyro_z_sum+=icm42688_gyro_z;
    }

    zero_gyro_x=(int16)(1.0f*gyro_x_sum/400);
    zero_gyro_y=(int16)(1.0f*gyro_y_sum/400);
    zero_gyro_z=(int16)(1.0f*gyro_z_sum/400);
    IMUOK=1;

}


void imu_get(void){//获取三个传感器数据


    //获得校正后的载体坐标系陀螺仪计数据
    Get_Gyro_ICM42688();        
    gyro.y=-(icm42688_gyro_x*0.5+icm42688_gyro_x_old*0.5-zero_gyro_x);
    gyro.x=icm42688_gyro_y*0.5+icm42688_gyro_y_old*0.5-zero_gyro_y;
    gyro.z=icm42688_gyro_z*0.5+icm42688_gyro_z_old*0.5-zero_gyro_z;

    pose.interface.data.g_x=&gyro.x;
    pose.interface.data.g_y=&gyro.y;
    pose.interface.data.g_z=&gyro.z;


    icm42688_gyro_x_old = icm42688_gyro_x;
    icm42688_gyro_y_old = icm42688_gyro_y;
    icm42688_gyro_z_old = icm42688_gyro_z;


}