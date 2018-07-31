#ifndef __SOLARPROJECT_H__
#define __SOLARPROJECT_H__

//main header file

// Runtime varibles.
int solarTime = 0; //drying time
int nextPosition = 0;

/* Pins for fan relays. Relays control the correct amount of voltage to turn the fans on.
Currently, fan speed is controlled by a manual knob. */
int relay1 = 31;
int relay2 = 33;

// Time
unsigned long pM = 0;
unsigned long pS = 0;
unsigned long index = 0;
bool secBox = false;
// Change here for time debug: 60000
const long intervalM = 60000;
const long intervalS = 1000;

#endif
