/*
 * menu.c
 *
 *  Created on: 2024年1月29日
 *      Author: aw
 */
#include "zf_common_headfile.h"
#include "menu.h"

key_state_enum key_1,key_2,key_3,key_4,key_5;
uint8 menu_flag;
float can_flag[3]={0,0,0};
uint8 menu_key_flag[5];
uint32 gnss_speed_num=0;

void menu_keyget() //记录按键状态
{
    key_1=key_get_state(0);
    key_2=key_get_state(1);
    key_3=key_get_state(2);
    key_4=key_get_state(3);
    key_5=key_get_state(4);
}

void menu_main() //菜单主页面
{
    key_clear_all_state();
    ips200_clear();
    ips200_show_string(0, 50, "da dian");
    ips200_show_string(0, 150, "tiao can");
    ips200_show_string(0, 250, "cha kan");
    while(key_4==KEY_RELEASE)
    {
    if(key_1==KEY_SHORT_PRESS) 
    {
         menu_one();
    }
    if(key_2==KEY_SHORT_PRESS)
    {
         menu_two();
    }
    if(key_3==KEY_SHORT_PRESS)
    {
         menu_three();
    }
    }
    ips200_clear();
menu_flag=0;
dian_now_num=0;
dian_over_flag=0;
//yaw=180;
hc_sr04_flag=0;
}

void menu_one() 
{
    key_clear_all_state();
    dian_num_zero();
    while(key_4==KEY_RELEASE)
    {
    switch(key_1)
    {
        case KEY_SHORT_PRESS : 
          if(menu_key_flag[0]==0){
            menu_key_flag[0]=1;
            dian_add();}break;
        case KEY_LONG_PRESS : dian_central_set();break;
        default: menu_key_flag[0]=0;break;
    }
    if(key_3==KEY_SHORT_PRESS&&menu_key_flag[1]==0){
      dian_jilu();
    menu_key_flag[1]=1;
    }
    else
      menu_key_flag[1]=0;

    if(key_2==KEY_SHORT_PRESS&&menu_key_flag[2]==0){
         dian_minus();
         menu_key_flag[2]=1;
    }
    else
      menu_key_flag[2]=0;
    }
    ips200_clear();
menu_main();
}

void menu_two()
{
    ips200_clear();
    ips200_show_string(0, 50, "turn");
    ips200_show_string(0, 100, "straight");
    ips200_show_string(0, 200, "gnss_speed_num");
    ips200_show_string(0, 150, "speed");
    while(key_4==KEY_RELEASE)
    {
      if(key_1==KEY_SHORT_PRESS)
        menu_tiaocan_turn();
      if(key_2==KEY_SHORT_PRESS)
        menu_tiaocan_straight();
      if(key_3==KEY_SHORT_PRESS)
        menu_speed();
       if(key_5==KEY_SHORT_PRESS)
        gnss_speed_num_menu();
    }
    menu_main();
}

void menu_three()
{
    ips200_clear();
    dian_num_zero();
     while(key_4==KEY_RELEASE)
     {
       switch(key_1)
    {
        case KEY_SHORT_PRESS : 
          if(menu_key_flag[0]==0){
            menu_key_flag[0]=1;
            dian_add();}break;
        case KEY_LONG_PRESS : dian_central_set();break;
        default: menu_key_flag[0]=0;break;
    }
   if(key_2==KEY_SHORT_PRESS&&menu_key_flag[2]==0){
         dian_minus();
         menu_key_flag[2]=1;
    }
    else
      menu_key_flag[2]=0;
       
          
          while(key_2==KEY_LONG_PRESS)
          {
            ips200_show_string(0, 120, "now time:");                                            // 输出年月日时分秒
                    ips200_show_float(0, 140, dian_data[dian_num].time.year, 2, 3);
                    ips200_show_float(0, 160, dian_data[dian_num].time.month, 2, 3);
                    ips200_show_float(0, 180, dian_data[dian_num].time.day, 2, 3);           // 输出年月日
                    ips200_show_string(0, 200, "state");   ips200_show_uint(100, 200, dian_data[dian_num].state, 1);      //输出当前定位有效模式 1：定位有效  0：定位无效
                    ips200_show_string(0, 220, "latitude");  ips200_show_float(100, 220, dian_data[dian_num].latitude_second, 2, 3);
                    ips200_show_string(0, 240, "longitude");  ips200_show_float(100, 240, dian_data[dian_num].longitude_second, 2, 3);
                    ips200_show_string(0, 260, "speed");  ips200_show_float(100, 260, dian_data[dian_num].speed, 2, 3);
                    ips200_show_string(0, 280, "direction");  ips200_show_float(100, 280, dian_data[dian_num].direction, 2, 3);
                    ips200_show_string(0, 300, "height");  ips200_show_float(100, 300, dian_data[dian_num].height, 2, 3);
          }
     }
     
     menu_main();
}

void menu_tiaocan_turn()
{
  ips200_clear();
   while(key_4==KEY_RELEASE)
   {
     ips200_show_string(0, 50, "p");ips200_show_float(100, 50,Servo_turn.P, 2, 3);
     ips200_show_string(0, 100, "i");ips200_show_float(100, 100,Servo_turn.I, 2, 3);
     ips200_show_string(0, 150, "d");ips200_show_float(100, 150,Servo_turn.D, 2, 3);
     
     switch(key_1)
     {
     case KEY_SHORT_PRESS: 
          if(menu_key_flag[0]==0){
          Servo_turn.P=Servo_turn.P+0.1;
          menu_key_flag[0]=1;}break;
          
     case KEY_LONG_PRESS: if(can_flag[0]==0){
                                 can_flag[0]=1;
                                 Servo_turn.P=Servo_turn.P-0.1;}break;
                          
     case KEY_RELEASE:  can_flag[0]=0;
                         menu_key_flag[0]=0;break;
     }
     
     switch(key_2)
     {
     case KEY_SHORT_PRESS: 
          if(menu_key_flag[1]==0){
          Servo_turn.I=Servo_turn.I+0.1;
          menu_key_flag[1]=1;}break;
          
     case KEY_LONG_PRESS: if(can_flag[1]==0){
                                 can_flag[1]=1;
                                 Servo_turn.I=Servo_turn.I-0.1;}break;
                          
     case KEY_RELEASE:  can_flag[1]=0;
                         menu_key_flag[1]=0;break;
     }
     
     switch(key_3)
     {
     case KEY_SHORT_PRESS: 
          if(menu_key_flag[2]==0){
          Servo_turn.D=Servo_turn.D+0.1;
          menu_key_flag[2]=1;}break;
          
     case KEY_LONG_PRESS: if(can_flag[2]==0){
                                 can_flag[2]=1;
                                 Servo_turn.D=Servo_turn.D-0.1;}break;
                          
     case KEY_RELEASE:  can_flag[2]=0;
                         menu_key_flag[2]=0;break;
     }
   }
   canshu_save(2);
menu_two();
}
void  menu_tiaocan_straight()
{
  ips200_clear();
   while(key_4==KEY_RELEASE)
   {
     ips200_show_string(0, 50,  "p");ips200_show_float(100, 50, Servo_straight.P, 2, 3);
     ips200_show_string(0, 100, "i");ips200_show_float(100, 100,Servo_straight.I, 2, 3);
     ips200_show_string(0, 150, "d");ips200_show_float(100, 150,Servo_straight.D, 2, 3);
     
    switch(key_1)
     {
     case KEY_SHORT_PRESS: 
          if(menu_key_flag[0]==0){
          Servo_straight.P=Servo_straight.P+0.1;
          menu_key_flag[0]=1;}break;
          
     case KEY_LONG_PRESS: if(can_flag[0]==0){
                                 can_flag[0]=1;
                                 Servo_straight.P=Servo_straight.P-0.1;}break;
                          
     case KEY_RELEASE:  can_flag[0]=0;
                         menu_key_flag[0]=0;break;
     }
     
     switch(key_2)
     {
     case KEY_SHORT_PRESS: 
          if(menu_key_flag[1]==0){
          Servo_straight.I=Servo_straight.I+0.1;
          menu_key_flag[1]=1;}break;
          
     case KEY_LONG_PRESS: if(can_flag[1]==0){
                                 can_flag[1]=1;
                                 Servo_straight.I=Servo_straight.I-0.1;}break;
                          
     case KEY_RELEASE:  can_flag[1]=0;
                         menu_key_flag[1]=0;break;
     }
     
     switch(key_3)
     {
     case KEY_SHORT_PRESS: 
          if(menu_key_flag[2]==0){
          Servo_straight.D=Servo_straight.D+0.1;
          menu_key_flag[2]=1;}break;
          
     case KEY_LONG_PRESS: if(can_flag[2]==0){
                                 can_flag[2]=1;
                                 Servo_straight.D=Servo_straight.D-0.1;}break;
                          
     case KEY_RELEASE:  can_flag[2]=0;
                         menu_key_flag[2]=0;break;
     }
   }
   canshu_save(1);
   menu_two();
}


void menu_speed()
{
  ips200_clear();
  while(key_4==KEY_RELEASE)
  {
   
   ips200_show_string(0, 50, "speed");
   ips200_show_string(0, 100, "sound_speed");
   ips200_show_string(0, 150, "change_distance");
   if(key_1==KEY_SHORT_PRESS)
   {
     ips200_clear();
   while(key_4==KEY_RELEASE)
   {
     ips200_show_string(0, 50, "speed"); ips200_show_uint(50, 50, duty_num, 3);
    switch(key_1)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[0]==0){
          duty_num++;
          menu_key_flag[0]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[0]==0){
                                can_flag[0]=1;
                                duty_num=duty_num+10;}break;
     case KEY_RELEASE:  can_flag[0]=0;
                         menu_key_flag[0]=0;break;
     }
     
     
    switch(key_2)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[1]==0){
          duty_num--;
          menu_key_flag[1]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[1]==0){
                                can_flag[1]=1;
                                duty_num=duty_num-10;}break;
     case KEY_RELEASE:  can_flag[1]=0;
                         menu_key_flag[1]=0;break;
     }
   }
   speed_save();
   ips200_clear();
   }
   
   if(key_2==KEY_SHORT_PRESS)
   {
     ips200_clear();
     while(key_4==KEY_RELEASE)
     {
     ips200_show_string(0, 100, "sound_speed"); ips200_show_uint(100, 100, sound_duty_num, 3);
    switch(key_1)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[0]==0){
          sound_duty_num++;
          menu_key_flag[0]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[0]==0){
                                can_flag[0]=1;
                                sound_duty_num=sound_duty_num+10;}break;
     case KEY_RELEASE:  can_flag[0]=0;
                         menu_key_flag[0]=0;break;
     }
     
     
    switch(key_2)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[1]==0){
          sound_duty_num--;
          menu_key_flag[1]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[1]==0){
                                can_flag[1]=1;
                                sound_duty_num=sound_duty_num-10;}break;
     case KEY_RELEASE:  can_flag[1]=0;
                         menu_key_flag[1]=0;break;
     }
   }
   speed_sound_save();
   ips200_clear();
   }
   if(key_3==KEY_SHORT_PRESS)
   {
     ips200_clear();
     while(key_4==KEY_RELEASE)
     {
     ips200_show_string(0, 150, "change_distance");  ips200_show_float(130, 150, change_distance_num, 3,2);
     ips200_show_string(0, 100, "distance");  ips200_show_float(130, 100, duty_num_second/change_distance_num, 3,2);
     ips200_show_string(0, 120, "0~23 -> 4 ");   ips200_show_string(100, 120, "24~30 -> 6 ");
        switch(key_1)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[0]==0){
       change_distance_num=change_distance_num+0.1;
     menu_key_flag[0]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[0]==0){
                                can_flag[0]=1;
                                change_distance_num=change_distance_num-0.1;}break;
     case KEY_RELEASE:  can_flag[0]=0;
                          menu_key_flag[0]=0;break;
     }
     
     
     switch(key_2)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[1]==0){
       change_distance_num=change_distance_num+1;
     menu_key_flag[1]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[1]==0){
                                can_flag[1]=1;
                                change_distance_num=change_distance_num-1;}break;
     case KEY_RELEASE:  can_flag[1]=0;
                          menu_key_flag[1]=0;break;
     }
     if(key_3==KEY_LONG_PRESS)
       change_distance_num=0;
   }
   change_distance_save();
   ips200_clear();
   }
  }
 menu_two();
}

void menu_kf()
{
  ips200_clear();
   while(key_4==KEY_RELEASE)
   {
  ips200_show_string(0, 50, "distance");
   ips200_show_string(0, 100, "angle");
   if(key_1==KEY_SHORT_PRESS)
   {
     ips200_clear();
     while(key_4==KEY_RELEASE)
   {
      ips200_show_string(0, 50, "kf_q");
   ips200_show_string(0, 100, "kf_r");
   if(key_1==KEY_SHORT_PRESS)   
   {
      while(key_4==KEY_RELEASE)
     {
     ips200_show_string(0, 100, "kfs_distance.Q_cov"); ips200_show_float(0, 130, kfs_distance.Q_cov, 5,2);
     if(key_1==KEY_SHORT_PRESS)                                                      //十分位加减
        kfs_distance.Q_cov=kfs_distance.Q_cov+0.1;
     if(key_1==KEY_LONG_PRESS&&can_flag[0]==0)
     {
       can_flag[0]=1;
        kfs_distance.Q_cov= kfs_distance.Q_cov-0.1;
     }
     if(key_1==KEY_RELEASE)
       can_flag[0]=0;
     
     
     if(key_2==KEY_SHORT_PRESS)                                                     //个位加减
        kfs_distance.Q_cov=kfs_distance.Q_cov+1;
     if(key_2==KEY_LONG_PRESS&&can_flag[1]==0)
     {
       can_flag[1]=1;
        kfs_distance.Q_cov= kfs_distance.Q_cov-1;
     }
     if(key_2==KEY_RELEASE)
       can_flag[1]=0;
     
     if(key_3==KEY_SHORT_PRESS)                                                    //十位加减
        kfs_distance.Q_cov=kfs_distance.Q_cov+10;
     if(key_3==KEY_LONG_PRESS&&can_flag[2]==0)
     {
       can_flag[2]=1;
        kfs_distance.Q_cov= kfs_distance.Q_cov-10;
     }
     if(key_3==KEY_RELEASE)
       can_flag[2]=0;
     
     
     if(key_5==KEY_SHORT_PRESS)
       kfs_distance.Q_cov=0;
     
     while(kfs_distance.Q_cov>100)                  //Q_cov max
     kfs_distance.Q_cov=0;
   }
   kf_num_save();
   ips200_clear();
   }
   
   
   if(key_2==KEY_SHORT_PRESS)
   {
     ips200_clear();
     while(key_4==KEY_RELEASE)
     {
     ips200_show_string(0, 100, "kfs_distance.r_cov"); ips200_show_float(0, 130, kfs_distance.R_cov, 5,2);
     if(key_1==KEY_SHORT_PRESS)                                                      //十分位加减
        kfs_distance.R_cov=kfs_distance.R_cov+0.1;
     if(key_1==KEY_LONG_PRESS&&can_flag[0]==0)
     {
       can_flag[0]=1;
        kfs_distance.R_cov= kfs_distance.R_cov-0.1;
     }
     if(key_1==KEY_RELEASE)
       can_flag[0]=0;
     
     
     if(key_2==KEY_SHORT_PRESS)                                                     //个位加减
        kfs_distance.R_cov=kfs_distance.R_cov+1;
     if(key_2==KEY_LONG_PRESS&&can_flag[1]==0)
     {
       can_flag[1]=1;
        kfs_distance.R_cov= kfs_distance.R_cov-1;
     }
     if(key_2==KEY_RELEASE)
       can_flag[1]=0;
     
     if(key_3==KEY_SHORT_PRESS)                                                    //十位加减
        kfs_distance.R_cov=kfs_distance.R_cov+10;
     if(key_3==KEY_LONG_PRESS&&can_flag[2]==0)
     {
       can_flag[2]=1;
        kfs_distance.R_cov= kfs_distance.R_cov-10;
     }
     if(key_3==KEY_RELEASE)
       can_flag[2]=0;
     
     if(key_5==KEY_SHORT_PRESS)
       kfs_distance.R_cov=0;
     
     while(kfs_distance.R_cov>100)                  //R_cov max
     kfs_distance.R_cov=0;
   }
   
     kf_num_save();
   ips200_clear();
   }
   }
   }
   
   if(key_2==KEY_SHORT_PRESS)
   {
         ips200_clear();
  while(key_4==KEY_RELEASE)
  {
   
   ips200_show_string(0, 50, "kf_q");
   ips200_show_string(0, 100, "kf_r");
   if(key_1==KEY_SHORT_PRESS)
   {
     ips200_clear();
   while(key_4==KEY_RELEASE)
   {
     ips200_show_string(0, 100, "kfs_angle.Q_cov"); ips200_show_float(0, 130, kfs_angle.Q_cov, 5,2);
     if(key_1==KEY_SHORT_PRESS)                                                      //十分位加减
        kfs_angle.Q_cov=kfs_angle.Q_cov+0.1;
     if(key_1==KEY_LONG_PRESS&&can_flag[0]==0)
     {
       can_flag[0]=1;
        kfs_angle.Q_cov= kfs_angle.Q_cov-0.1;
     }
     if(key_1==KEY_RELEASE)
       can_flag[0]=0;
     
     
     if(key_2==KEY_SHORT_PRESS)                                                     //个位加减
        kfs_angle.Q_cov=kfs_angle.Q_cov+1;
     if(key_2==KEY_LONG_PRESS&&can_flag[1]==0)
     {
       can_flag[1]=1;
        kfs_angle.Q_cov= kfs_angle.Q_cov-1;
     }
     if(key_2==KEY_RELEASE)
       can_flag[1]=0;
     
     if(key_3==KEY_SHORT_PRESS)                                                    //十位加减
        kfs_angle.Q_cov=kfs_angle.Q_cov+10;
     if(key_3==KEY_LONG_PRESS&&can_flag[2]==0)
     {
       can_flag[2]=1;
        kfs_angle.Q_cov= kfs_angle.Q_cov-10;
     }
     if(key_3==KEY_RELEASE)
       can_flag[2]=0;
     
     if(key_5==KEY_SHORT_PRESS)
       kfs_angle.Q_cov=0;
     
     while(kfs_angle.Q_cov>100)                  //Q_cov max
     kfs_angle.Q_cov=0;
     
   }
   kf_num_save_2();
   ips200_clear();
   }
   
   if(key_2==KEY_SHORT_PRESS)
   {
     ips200_clear();
     while(key_4==KEY_RELEASE)
     {
     ips200_show_string(0, 100, "kfs_angle.r_cov"); ips200_show_float(0, 130, kfs_angle.R_cov, 5,2);
     if(key_1==KEY_SHORT_PRESS)                                                      //十分位加减
        kfs_angle.R_cov=kfs_angle.R_cov+0.1;
     if(key_1==KEY_LONG_PRESS&&can_flag[0]==0)
     {
       can_flag[0]=1;
        kfs_angle.R_cov= kfs_angle.R_cov-0.1;
     }
     if(key_1==KEY_RELEASE)
       can_flag[0]=0;
     
     
     if(key_2==KEY_SHORT_PRESS)                                                     //个位加减
        kfs_angle.R_cov=kfs_angle.R_cov+1;
     if(key_2==KEY_LONG_PRESS&&can_flag[1]==0)
     {
       can_flag[1]=1;
        kfs_angle.R_cov= kfs_angle.R_cov-1;
     }
     if(key_2==KEY_RELEASE)
       can_flag[1]=0;
     
     if(key_3==KEY_SHORT_PRESS)                                                    //十位加减
        kfs_angle.R_cov=kfs_angle.R_cov+10;
     if(key_3==KEY_LONG_PRESS&&can_flag[2]==0)
     {
       can_flag[2]=1;
        kfs_angle.R_cov= kfs_angle.R_cov-10;
     }
     if(key_3==KEY_RELEASE)
       can_flag[2]=0;
     
      if(key_5==KEY_SHORT_PRESS)
       kfs_angle.R_cov=0;
      
     while(kfs_angle.R_cov>100)                  //R_cov max
     kfs_angle.R_cov=0;
   }
     kf_num_save_2();
   ips200_clear();
   }
  }
   }
   }
   menu_two();
}

void gnss_speed_num_menu()
{
       ips200_clear();
     while(key_4==KEY_RELEASE)
     {
     ips200_show_string(0, 150, "gnss_speed_num");  ips200_show_float(130, 150, gnss_speed_num, 3,2);
        switch(key_1)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[0]==0){
       gnss_speed_num=gnss_speed_num+0.1;
     menu_key_flag[0]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[0]==0){
                                can_flag[0]=1;
                                gnss_speed_num=gnss_speed_num-0.1;}break;
     case KEY_RELEASE:  can_flag[0]=0;
                          menu_key_flag[0]=0;break;
     }
     
     
     switch(key_2)
     {
     case KEY_SHORT_PRESS:if(menu_key_flag[1]==0){
       gnss_speed_num=gnss_speed_num+1;
     menu_key_flag[1]=1;}break;
     case KEY_LONG_PRESS: if(can_flag[1]==0){
                                can_flag[1]=1;
                                gnss_speed_num=gnss_speed_num-1;}break;
     case KEY_RELEASE:  can_flag[1]=0;
                          menu_key_flag[1]=0;break;
     }
     if(key_3==KEY_LONG_PRESS)
       gnss_speed_num=0;
   }
   gnss_speed_num_save();
   ips200_clear();
   menu_two();
}