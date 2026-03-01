#include "zf_common_headfile.h"
    

uint8 change_flag;
     char time_flag=0,dian_over_flag=0;
     int time_num=0;
     double dian_long=5;
     double dian_long_first;
    uint32 dian_now_num=0;   //当前目标点位
   float gnss_latitude_filter;
   float gnss_longitude_filter;
   float gnss_latitude_last=0;
   float gnss_longitude_last=0;
   
   
   void gnss_data_filter()
   {
     gnss_latitude_filter=0.2*gnss_latitude_last+0.8*gnss.latitude;
      gnss_longitude_filter=0.2*gnss_longitude_last+0.8*gnss.longitude;
      gnss_latitude_last=gnss_latitude_filter;
      gnss_longitude_last=gnss_longitude_filter;
   }
   void motor_test()
    {
      gnss_data_filter();
      if(dian_data[get_data[1]+1].state==0)
      {
//        dian_long_first=get_two_points_distance(gnss_latitude_filter, gnss_longitude_filter, dian_data[9].latitude, dian_data[9].longitude);
//        dian_long=kmfilter( kfs_distance,dian_long_first);
        dian_long=get_two_points_distance(gnss_latitude_filter, gnss_longitude_filter, dian_data[9].latitude, dian_data[9].longitude);
      }
      else
      {
//    dian_long_first=get_two_points_distance(gnss_latitude_filter, gnss_longitude_filter, dian_data[ get_data[1]+1].latitude, dian_data[get_data[1]+1].longitude);
//    dian_long=kmfilter( kfs_distance,dian_long_first);
        dian_long=get_two_points_distance(gnss_latitude_filter, gnss_longitude_filter, dian_data[get_data[1]+1].latitude, dian_data[get_data[1]+1].longitude);
      }
  if(change_flag==1)
  {
     dian_now_num++;
//    ips200_show_string(0, 120, "dian over");
     change_flag=0;
//     ipc_send_data(change_flag);
  }
  if(dian_now_num==dian_truely_num||dian_truely_num==0){   //当跑到最后一个点附近时，结束标志置一
     dian_over_flag=1;
  }
    }
