#include"servo.h"
//一队一车舵机
uint16 dir_smotor_max=900;
uint16 dir_smotor_min=600;//越小越往右
uint16 dir_smotor_mid=740;
//一队二车舵机
//uint16 dir_smotor_max=900;
//uint16 dir_smotor_min=600;//越小越往右
//uint16 dir_smotor_mid=760;
int16 servo_duty;
//int16 servo_basis;
double bias_angle;
double bias_angle_old;
double target_angle;
double target_angle_first;
double eskf_angle=0;
double init_angle=0;
double turn_flag=0;
uint8 bias_flag=0; //跳变标志位
uint16 bias_change_flag=0;    //跳变后计数
float fwj_receive;
uint8 sound_gps_flag=0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      舵机初始化
//  @note
//-------------------------------------------------------------------------------------------------------------------
void servo_init(void)
{
//    //dir_smotor_mid = dir_smotor_min + (dir_smotor_max - dir_smotor_min) / 2;

    pwm_init(servo,50, dir_smotor_min);      //检测舵机最右边是否合适
    system_delay_ms(500);

    pwm_init(servo,50, dir_smotor_max);      //检测舵机最左边是否合适
    system_delay_ms(500);

    pwm_init(servo, 50, dir_smotor_mid);      //舵机归中
    system_delay_ms(500);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      舵机占空比复制
//  @param duty   ：需要给的占空比
//  @param direct ：舵机方向
//  @note  舵机方向给0或1，观察到舵机反打就更改这个
//-------------------------------------------------------------------------------------------------------------------
void Servo_Duty(int32 duty,uint8 direct)
{
  int32 servo_duty;
  if(direct==1)
      servo_duty = dir_smotor_mid+duty;
  else servo_duty = dir_smotor_mid-duty;

  servo_duty = (servo_duty > dir_smotor_max ? dir_smotor_max : servo_duty);
  servo_duty = (servo_duty < dir_smotor_min ? dir_smotor_min : servo_duty);

  pwm_set_duty(servo, servo_duty);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取舵机偏差
//  @note
//-------------------------------------------------------------------------------------------------------------------
void get_basis(float target_angle){
      bias_angle_old=bias_angle;
      if(bias_change_flag>10){
         bias_flag=0;
      }
      if(bias_flag==0){
        if(fabs(yaw-target_angle)>=180)
         {
             if(yaw>target_angle)
                 bias_angle=360-yaw+target_angle; 
             else
                bias_angle=-yaw+target_angle-360;    //目标角与实际角的偏差
          }
          else
          {
            bias_angle= target_angle-yaw;
          }    
        if(fabs(yaw-yaw_old)>350){   //当检测到航向角的值可能发生跳变
          if(yaw>yaw_old){       //从0向360跳变
             yaw_increase=yaw-yaw_old-360;
             if(bias_angle_old>(180+yaw_increase)&&bias_angle_old<=180){   //判断出会出现跳变
               bias_angle=bias_angle_old-yaw_increase;
               bias_flag=1;
             }
         }  
      }
//        else{       //从360到0跳变,举例偏差从19跳变到-1没有影响，甚至使车更加稳定。
//           yaw_increase=yaw-yaw_old+360;    
//           if(bias_angle_old>0&&bias_angle_old<(0+yaw_increase))
//           
//        }
      }     
      else if(bias_flag==1){
          bias_change_flag++;
      }
}


void servo_contral(uint8 servo_patern)
{
    if(servo_patern==servo_gps)    //gps定位模式
    {        
     gnss_data_filter();
        if(dian_data[get_data[1]+1].state==0)
        {
//            target_angle_first=  get_two_points_azimuth(gnss.latitude, gnss.longitude, dian_data[9].latitude, dian_data[9].longitude);
//            target_angle=kmfilter( kfs_angle,target_angle_first);
            target_angle=get_two_points_azimuth(gnss_latitude_filter, gnss_longitude_filter, dian_data[9].latitude, dian_data[9].longitude);
        }
        else
        {
//            target_angle_first=  get_two_points_azimuth(gnss.latitude, gnss.longitude, dian_data[get_data[1]+1].latitude, dian_data[get_data[1]+1].longitude);
//            target_angle=kmfilter( kfs_angle,target_angle_first);
          target_angle=get_two_points_azimuth(gnss_latitude_filter, gnss_longitude_filter, dian_data[get_data[1]+1].latitude, dian_data[get_data[1]+1].longitude);
        }
        get_basis(target_angle);
        servo_duty=(int16)PID_Positional_Servo(&Servo_straight,bias_angle, 0);
    }
    else if(servo_patern==servo_sound) //声音定位模式
    {     
        bias_angle=fwj;
        servo_duty=(int16)PID_Positional_Servo(&Servo_turn,bias_angle, 0);
    }
    if(motor_control.motor_set_dir==REVERSE){
        Servo_Duty(servo_duty,1);
    }
    else{
        Servo_Duty(servo_duty,0);
    }
    

}