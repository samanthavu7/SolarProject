#ifndef __SOLARPROJECT_H__
#define __SOLARPROJECT_H__

//main header file

// Runtime varibles.
extern int solarTime; //drying time
extern int nextPosition;

/* Pins for fan relays. Relays control the correct amount of voltage to turn the fans on.
Currently, fan speed is controlled by a manual knob. */
extern int relay1;
extern int relay2;

// Time
extern unsigned long pM;
extern unsigned long pS;
extern unsigned long index;
extern bool secBox;
// Change here for time debug: 60000
extern const long intervalM;
extern const long intervalS;

#endif
