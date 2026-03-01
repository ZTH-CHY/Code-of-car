/*
 * dadian.c
 *
 *  Created on: 2024年3月7日
 *      Author: aw
 */
#include "zf_common_headfile.h"
#include "dadian.h"

#define smooth_num    23
uint32 dian_num=0;
uint32 dian_numfirst=0;
uint32 dian_truely_num=0;  //已记录的点的个数
gnss_info_struct dian_data[10];
gnss_info_struct dian_datafirst[100];
char dadian_flag;
dadian_smoothing_struct dadian_smoothing[50];
dadian_smoothing_struct dadian_smoothing_two[50];
double dadian_long_first;
float duty_num_second;

void dian_add()  //当前选中打点点位加一
{
    dian_num++;
    ips200_clear();
         if(dian_num>1)
    {
        ips200_show_string(0, 50, "dian_num");
        ips200_show_uint(100, 50,dian_num-1, 2);
        ips200_show_string(0, 100, "ji lu");
    }

    else
    {
      ips200_show_string(0, 50, "ji zhun_num");
      ips200_show_uint(100, 50,dian_num+1, 2);
      ips200_show_string(0, 100, "ji lu");
    }
}

void dian_minus() //当前选中打点点位减一
{
    dian_num--;
    ips200_clear();
         if(dian_num>1)
    {
        ips200_show_string(0, 50, "dian_num");
        ips200_show_uint(100, 50,dian_num-1, 2);
        ips200_show_string(0, 100, "ji lu");
    }
    else
    {
      ips200_show_string(0, 50, "ji zhun_num");
      ips200_show_uint(100, 50,dian_num+1, 2);
      ips200_show_string(0, 100, "ji lu");
    }
}

void dian_num_zero()//当前选中打点点位清零
{
    dian_num=0;
    ips200_clear();
         if(dian_num>1)
    {
        ips200_show_string(0, 50, "dian_num");
        ips200_show_uint(100, 50,dian_num-1, 2);
        ips200_show_string(0, 100, "ji lu");
    }
    else
    {
      ips200_show_string(0, 50, "ji zhun_num");
      ips200_show_uint(100, 50,dian_num+1, 2);
      ips200_show_string(0, 100, "ji lu");
    }
}

void dian_central_set()
{
  dian_num=9;
   ips200_show_string(0, 50, "dian_central");
   ips200_show_string(0, 100, "ji lu");
}
void dian_jilu() //将此刻gps数据记录到当前选中打点点位中
{
  ips200_clear();
  if(dian_num>1)
  {
  while(dian_numfirst!=5)
   {
    ips200_show_string(0, 50, "dian_numfirst");
        ips200_show_uint(150, 50,dian_numfirst, 2);
        if(dadian_flag==1)
        {
          dadian_flag=0;
           dian_datafirst[dian_numfirst]=gnss;
           dian_numfirst++;
        }
  }
  
  
//  dadian_smoothing_f();
  
  dian_data[dian_num].time=dian_datafirst[0].time;
  dian_data[dian_num].state=dian_datafirst[0].state;
  dian_data[dian_num].latitude_degree=(dian_datafirst[0].latitude_degree+dian_datafirst[1].latitude_degree+dian_datafirst[2].latitude_degree+dian_datafirst[3].latitude_degree+dian_datafirst[4].latitude_degree)/5;
  dian_data[dian_num].latitude_cent=(dian_datafirst[0].latitude_cent+dian_datafirst[1].latitude_cent+dian_datafirst[2].latitude_cent+dian_datafirst[3].latitude_cent+dian_datafirst[4].latitude_cent)/5;
  dian_data[dian_num].latitude_second=(dian_datafirst[0].latitude_second+dian_datafirst[1].latitude_second+dian_datafirst[2].latitude_second+dian_datafirst[3].latitude_second+dian_datafirst[4].latitude_second)/5;
  dian_data[dian_num].longitude_degree=(dian_datafirst[0].longitude_degree+dian_datafirst[1].longitude_degree+dian_datafirst[2].longitude_degree+dian_datafirst[3].longitude_degree+dian_datafirst[4].longitude_degree)/5; 
  dian_data[dian_num].longitude_cent=(dian_datafirst[0].longitude_cent+dian_datafirst[1].longitude_cent+dian_datafirst[2].longitude_cent+dian_datafirst[3].longitude_cent+dian_datafirst[4].longitude_cent)/5; 
  dian_data[dian_num].longitude_second=(dian_datafirst[0].longitude_second+dian_datafirst[1].longitude_second+dian_datafirst[2].longitude_second+dian_datafirst[3].longitude_second+dian_datafirst[4].longitude_second)/5; 
  dian_data[dian_num].latitude=(dian_datafirst[0].latitude+dian_datafirst[1].latitude+dian_datafirst[2].latitude+dian_datafirst[3].latitude+dian_datafirst[4].latitude)/5; 
  dian_data[dian_num].longitude=(dian_datafirst[0].longitude+dian_datafirst[1].longitude+dian_datafirst[2].longitude+dian_datafirst[3].longitude+dian_datafirst[4].longitude)/5; 
  dian_data[dian_num].ns=dian_datafirst[0].ns;
  dian_data[dian_num].ew=dian_datafirst[0].ew;
  dian_data[dian_num].speed=dian_datafirst[0].speed;
  dian_data[dian_num].direction=dian_datafirst[0].direction;
  dian_data[dian_num].antenna_direction_state=dian_datafirst[0].antenna_direction_state;
  dian_data[dian_num].antenna_direction=dian_datafirst[0].antenna_direction;
  dian_data[dian_num].satellite_used=dian_datafirst[0].satellite_used;
  dian_data[dian_num].height=dian_datafirst[0].height;
  
  if(dian_numfirst==5&&dian_data[dian_num].state==1)
    {
      ips200_clear();
       while(key_4==KEY_RELEASE)
       ips200_show_string(0, 0, "ok"); 
       dian_numfirst=0;
    }
  }
  else
  {
    dian_data[dian_num]=gnss;
    if(dian_data[dian_num].state==1)
    {
      ips200_clear();
       while(key_4==KEY_RELEASE)
       ips200_show_string(0, 0, "ok"); 
    }
  }
  dian_truely_num=0;
  for(dian_num=0;dian_num<=9;dian_num++)
  {
    if(dian_data[dian_num].state==1)
       dian_truely_num++;
  }
    
}

void   dadian_smoothing_f()
{
  float max;
  int check;
  for(int i=0;i<50;i++)
  {
//    dadian_long_first=get_two_points_distance(dian_datafirst[i].latitude, dian_datafirst[i].longitude, dian_data[ 0].latitude, dian_data[0].longitude);
//    dadian_smoothing[i].smoothing_num=kmfilter( kfs_distance,dadian_long_first);
    dadian_smoothing[i].smoothing_num=dian_datafirst[i].latitude+dian_datafirst[i].longitude;
    dadian_smoothing[i].num=i;
  }

    for (int i = 0;i <49;i++) {
        check = i;
        for (int j = i + 1;j < 50;j++) {
            if (dadian_smoothing[j].smoothing_num < dadian_smoothing[check].smoothing_num) {
                check = j;
            }
        }
        if (i != check) {
          max=dadian_smoothing[check].smoothing_num;
          dadian_smoothing[check].smoothing_num=dadian_smoothing[i].smoothing_num;
          dadian_smoothing[i].smoothing_num=max;
          max=dadian_smoothing[i].num;
          dadian_smoothing[i].num=dadian_smoothing[check].num;
          dadian_smoothing[check].num=max;
        }
    }
//   for(int i=0;i<50;i++)
//  {
////    dadian_long_first=get_two_points_distance(dian_datafirst[i+25].latitude, dian_datafirst[i+25].longitude, dian_data[1].latitude, dian_data[1].longitude);
////    dadian_smoothing_two[i].smoothing_num=kmfilter( kfs_distance,dadian_long_first);
//    dadian_smoothing_two[i].smoothing_num=get_two_points_distance(dian_datafirst[i+25].latitude, dian_datafirst[i+25].longitude, dian_data[ 1].latitude, dian_data[1].longitude);
//    dadian_smoothing_two[i].num=dadian_smoothing[i+25].num;
//  }
//
//    for (int i = 0;i <49;i++) {
//        check = i;
//        for (int j = i + 1;j < 50;j++) {
//            if (dadian_smoothing_two[j].smoothing_num < dadian_smoothing_two[check].smoothing_num) {
//                check = j;
//            }
//        }
//        if (i != check) {
//          max=dadian_smoothing_two[check].smoothing_num;
//          dadian_smoothing_two[check].smoothing_num=dadian_smoothing_two[i].smoothing_num;
//          dadian_smoothing_two[i].smoothing_num=max;
//          max=dadian_smoothing_two[i].num;
//          dadian_smoothing_two[i].num=dadian_smoothing_two[check].num;
//          dadian_smoothing_two[check].num=max;
//        }
//    }
}

void dian_shuaxin(int num)
{
  while(dian_numfirst!=5)
   {
        if(dadian_flag==1)
        {
          dadian_flag=0;
           dian_datafirst[dian_numfirst]=gnss;
           dian_numfirst++;
        }
  }
 
  dian_data[num].latitude=(dian_datafirst[0].latitude+dian_datafirst[1].latitude+dian_datafirst[2].latitude+dian_datafirst[3].latitude+dian_datafirst[4].latitude)/5;
  dian_data[num].longitude=(dian_datafirst[0].longitude+dian_datafirst[1].longitude+dian_datafirst[2].longitude+dian_datafirst[3].longitude+dian_datafirst[4].longitude)/5;
  dian_numfirst=0;
}
