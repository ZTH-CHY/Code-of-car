#include "hc_sr04.h"

uint8 hc_sr04_flag=0;

void hc_sr04_init(){
      gpio_init(echo, GPI, 0, GPI_FLOATING_IN); 
      gpio_init(trig, GPO, 0, GPO_PUSH_PULL); 
      timer_init(TC_TIME2_CH0, TIMER_US);
}

void hc_sr04_send(){
      gpio_set_level(trig, 1);
      system_delay_us(20);
      gpio_set_level(trig, 0);
}

uint16 hc_sr04_mesure(){
  uint32 time,distance;
  timer_clear(TC_TIME2_CH0);
  if( gpio_get_level(echo)==0){
         timer_start(TC_TIME2_CH0);
  }
  if(gpio_get_level(echo)==1){
         timer_stop(TC_TIME2_CH0);
  }
  time= timer_get(TC_TIME2_CH0); 
  distance=time*1.7/100;//CM
  return distance;
}

void hc_sr04_deal_with()
{
  uint32 distance;
   hc_sr04_send();
   distance=hc_sr04_mesure();
   if(distance < 20)
     hc_sr04_flag=1;
}
