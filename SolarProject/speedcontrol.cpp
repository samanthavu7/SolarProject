#include "speedcontrol.h"
#include <Arduino.h>

unsigned char channel_1 = 47;    // Output to Opto Triac pin, channel 1
unsigned char channel_2;    // Output to Opto Triac pin, channel 2
unsigned char dimming = 3;      // Dimming level (0-100)
unsigned char i;
unsigned char flag=0;


void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms 
  // Every zerocrossing : (50Hz)-> 10ms (1/2 Cycle) For 60Hz (1/2 Cycle) => 8.33ms 
  // 10ms=10000us
  
  int dimtime = (83*dimming);    // For 60Hz =>83   
  delayMicroseconds(dimtime);    // Off cycle
  
  if (flag==0)
   {
    digitalWrite(channel_1, HIGH);   // triac firing
    delayMicroseconds(8);         // triac On propogation delay (for 60Hz use 8.33)
    digitalWrite(channel_1, LOW);    // triac Off
   }
  
  else
   {
    digitalWrite(channel_2, HIGH);   // triac firing
    delayMicroseconds(8.33);         // triac On propogation delay (for 60Hz use 8.33)
    digitalWrite(channel_2, LOW);    // triac Off
   }
  
}
