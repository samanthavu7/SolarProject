#ifndef __BUTTONS_H__
#define __BUTTONS_H__

//#include <Adafruit_GFX.h>
//#include <Adafruit_TFTLCD.h>
#include "screen.h"
/* Pins for manual buttons.
Emergency button currently not implemented. */
const int buttonUp = 53;
const int buttonEnter = 51;
const int buttonDown = 49;
//const int buttonEmergency = A11;
//const int ledEmergency = A12;
//const int buzzerEmergency = 12;
int upState = LOW;
int downState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 1000;

//Variables for manual buttons. 
String currentButton = "";
String selectedButton = "";
bool barEntered = false; // If drying time bar is selected

void initializeButtons();
void highlight();  

#endif __BUTTONS_H__
