#include "zf_common_headfile.h"

float sound_duty_num;
float change_distance_num;

void canshu_save(int num)//num =1为直行舵机pid，num=2为转向舵机pid
{
      uint32 buf[3];
      flash_erase_page(0, num);
      switch(num)
      {
      case 1 :
      buf[0]=(uint32)(Servo_straight.P*1000);//*100目的是保存浮点数据
      buf[1]=(uint32)(Servo_straight.I*1000);
      buf[2]=(uint32)(Servo_straight.D*1000);break;
      case 2 :
      buf[0]=(uint32)(Servo_turn.P*1000);//*100目的是保存浮点数据
      buf[1]=(uint32)(Servo_turn.I*1000);
      buf[2]=(uint32)(Servo_turn.D*1000);break;
default :break;
      }
      flash_write_page(0, num, buf, 3);
}

void canshu_read(int num)
{
      uint32 data_buffer[3];
      flash_read_page(0, num, data_buffer, 3);
      if(  flash_check(0, num))
      {
      switch(num)
      {
      case 1 :
      Servo_straight.P=((float)data_buffer[0])/1000;break;
//      Servo_straight.I=((float)data_buffer[1])/1000;
//      Servo_straight.D=((float)data_buffer[2])/1000;break;
      case 2 :
      Servo_turn.P=((float)data_buffer[0])/1000;break;
//      Servo_turn.I=((float)data_buffer[1])/1000;
//      Servo_turn.D=((float)data_buffer[2])/1000;break;
default :break;
      }
      }
           
}

void speed_save()
{
  uint32 speed_buf;
  flash_erase_page(0, 3);
  speed_buf=duty_num;
  flash_write_page(0, 3, &speed_buf, 1);
}

void speed_sound_save(){
  uint32 speed_buf;
  flash_erase_page(0, 4);
  speed_buf=sound_duty_num;
  flash_write_page(0, 4, &speed_buf, 1);
}

void speed_read()
{
  uint32 speed_buf;
  flash_read_page(0, 3, &speed_buf, 1);
  if(  flash_check(0, 3))
  {
    duty_num=speed_buf;
  }
}
void speed_sound_read()
{
  uint32 speed_buf;
  flash_read_page(0, 4, &speed_buf, 1);
  if(  flash_check(0, 4))
  {
    sound_duty_num=speed_buf;
  }
}

void change_distance_save()
{
  uint32 speed_buf;
  flash_erase_page(0, 5);
  speed_buf=change_distance_num;
  flash_write_page(0, 5, &speed_buf, 1);
}


void change_distance_read()
{
  uint32 speed_buf;
  flash_read_page(0, 5, &speed_buf, 1);
  if(  flash_check(0, 5))
  {
    change_distance_num=speed_buf;
  }
}

void kf_num_save()
{
   uint32 buf[2];
      flash_erase_page(0, 6);
      buf[0]=(uint32)kfs_distance.Q_cov*1000;
      buf[1]=(uint32)kfs_distance.R_cov*1000;
      flash_write_page(0, 6, buf, 2);
}


void kf_num_read()
{
   uint32 buf[2];
  flash_read_page(0, 6, buf, 2);
  if(  flash_check(0, 6))
  {
    kfs_distance.Q_cov=buf[0]/1000;
    kfs_distance.R_cov=buf[1]/1000;
  }
}

void kf_num_save_2()
{
   uint32 buf[2];
      flash_erase_page(0, 7);
      buf[0]=(uint32)kfs_angle.Q_cov*1000;
      buf[1]=(uint32)kfs_angle.R_cov*1000;
      flash_write_page(0, 7, buf, 2);
}


void kf_num_read_2()
{
   uint32 buf[2];
  flash_read_page(0, 7, buf, 2);
  if(  flash_check(0, 7))
  {
    kfs_angle.Q_cov=buf[0]/1000;
    kfs_angle.R_cov=buf[1]/1000;
  }
}

void gnss_speed_num_save(){
  uint32 speed_buf;
  flash_erase_page(0, 8);
  speed_buf=gnss_speed_num;
  flash_write_page(0, 8, &speed_buf, 1);
}

void gnss_speed_num_read()
{
  uint32 speed_buf;
  flash_read_page(0, 8, &speed_buf, 1);
  if(  flash_check(0, 8))
  {
    
    gnss_speed_num=speed_buf;
  }
}

void data_save_init()
{
   flash_init();
   canshu_read(1);
   canshu_read(2);
   speed_read();
   speed_sound_read();
   change_distance_read();
   kf_num_read();
   kf_num_read_2();
   gnss_speed_num_read();
}