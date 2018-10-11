#ifndef __BUTTONS_H__
#define __BUTTONS_H__

//#include <Adafruit_GFX.h>
//#include <Adafruit_TFTLCD.h>
#include "screen.h"
/* Pins for manual buttons.
Emergency button currently not implemented. */
extern const int buttonUp;
extern const int buttonEnter;
extern const int buttonDown;
//const int buttonEmergency = A11;
//const int ledEmergency = A12;
//const int buzzerEmergency = 12;
extern int upState;
extern int downState;
extern long lastDebounceTime;
extern long debounceDelay;

//Variables for manual buttons. 
extern String currentButton;
extern String selectedButton;
extern bool barEntered; // If drying time bar is selected

void initializeButtons();
void highlight();  

#endif
