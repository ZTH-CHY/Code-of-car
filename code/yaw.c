#include "yaw.h"

unsigned char yawflag;
unsigned char gps_over; //gps方向信号更新标志 1：gps方向信号更新，应用gps修正偏航角
float yaw;
float yaw_old;
float yaw_increase;
//float imu_dt=0.005;

void yaw_get(void){ //放进5ms中断
    //获得校正后的载体坐标系陀螺仪计数据
    if(yawflag==1){
      yaw_old=yaw;
        if(gnss.speed<0.5){//速度小时利用陀螺仪惯导持续积分保证发车时偏航角准确；
            yaw-=gyro.z*0.002;
              if(yaw>=360)yaw-=360;     //限幅
              if(yaw<0)yaw+=360;

        }
        else {                   //速度增大时GPS偏航角稳定，利用GPS偏航角修正，yaw信息，GPS偏航角更新间隔使用陀螺仪积分保证更新连续；
            if(gps_over==1){
                yaw=gnss.direction;
                gps_over=0;
            }
            yaw-=gyro.z*0.002;
            if(yaw>=360)yaw-=360;//限幅
            if(yaw<0)yaw+=360;

        }
    }
    else{
      
    }
}


void yaw_init(void){

    yaw=eskf_angle;
    yawflag=1;

}
