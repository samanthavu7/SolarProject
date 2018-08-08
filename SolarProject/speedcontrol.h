#ifndef SPEEDCONTROL_H
#define SPEEDCONTROL_H

extern unsigned char channel_1;    // Output to Opto Triac pin, channel 1
extern unsigned char channel_2;    // Output to Opto Triac pin, channel 2
extern unsigned char dimming;      // Dimming level (0-100)
extern unsigned char i;
extern unsigned char flag;

void zero_crosss_int();

#endif
