#include "buttons.h"

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

void initializeButtons(){ //also functions as deselect
  // Create start button.
  tft.drawRect(10, 160, 100, 30, WHITE);
  tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("Start");
  
  // Create stop button.
  tft.drawRect(10, 200, 100, 30, WHITE);
  tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("Stop");

  // Create initial time bar.
  tft.fillRect(timeBox, tft.height()-10, BOXSIZE, 10, WHITE); //hardcoding ok?
}

void highlight() { 
  if(currentButton == "START" && selectedButton == "") {
    tft.drawRect(10, 160, 100, 30, GRAY);
    tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(GRAY);
    tft.print("Start");
    tft.drawRect(10, 200, 100, 30, WHITE);
    tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Stop");
    tft.fillRect(timeBox, tft.height()-solarTime-10, BOXSIZE, solarTime+10, WHITE);
  }
  else if(currentButton == "BAR" && selectedButton == "") {
    tft.drawRect(10, 160, 100, 30, WHITE);
    tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Start");
    tft.drawRect(10, 200, 100, 30, WHITE);
    tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Stop");
    tft.fillRect(timeBox, tft.height()-solarTime-10, BOXSIZE, solarTime+10, GRAY);
  }
  else if(currentButton == "START" && selectedButton == "START") {
    tft.drawRect(10, 160, 100, 30, BLACK);
    tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.print("Start");
    tft.drawRect(10, 200, 100, 30, WHITE);
    tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Stop");
    tft.fillRect(timeBox, tft.height()-solarTime-10, BOXSIZE, solarTime+10, WHITE);
  }
  else if(currentButton == "STOP" && selectedButton == "START") {
    tft.drawRect(10, 160, 100, 30, BLACK);
    tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.print("Start");
    tft.drawRect(10, 200, 100, 30, GRAY);
    tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(GRAY);
    tft.print("Stop");
    tft.fillRect(timeBox, tft.height()-solarTime-10, BOXSIZE, solarTime+10, WHITE);
  }
  else if(currentButton == "BAR" && selectedButton == "BAR") {
    tft.drawRect(10, 160, 100, 30, WHITE);
    tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Start");
    tft.drawRect(10, 200, 100, 30, WHITE);
    tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Stop");
    tft.fillRect(timeBox, tft.height()-solarTime-10, BOXSIZE, solarTime+10, BLACK);
  }
  else if(currentButton == "START" && selectedButton == "STOP") {
    tft.drawRect(10, 160, 100, 30, GRAY);
    tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(GRAY);
    tft.print("Start");
    tft.drawRect(10, 200, 100, 30, BLACK);
    tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.print("Stop");
    tft.fillRect(timeBox, tft.height()-solarTime-10, BOXSIZE, solarTime+10, WHITE);
  }
}

//Draws Emergency Screen
/*void emergencyScreen() {
  tft.fillScreen(RED);
  tft.setTextSize(5);
  tft.setTextColor(WHITE);
  tft.setCursor(10, 10);
  tft.print("EMERGENCY");
  tft.setCursor(10, 60);
  tft.print("STOP");
  tft.setTextSize(2);
  tft.setCursor(10, 150);
  tft.print("Motion sensors have detected someone inside of the dryer.");
  tft.setCursor(10, 170);
  tft.print("Close the door and press Start to resume drying.");
  //tft.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
  //tft.print("!"); //set size and cursor
}*/

//void soundAlarm() { //TODO: change alarm jingle
//  int numTones = 10;
//  int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//  //            mid C  C#   D    D#   E    F    F#   G    G#   A
//  for (int i = 0; i < numTones; i++)
//  {
//    tone(speakerPin, tones[i]);
//    delay(500);
//  }
//  noTone(speakerPin);
//}
  
