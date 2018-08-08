#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include "SolarProject.h"
#include "sensors.h"
// Define screen's SPI Communication**
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// Reset pin: optional
#define LCD_RESET A4
extern Adafruit_TFTLCD tft;

// Define colors of the screen
#define PASTELGREEN 0x6F92
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xFF0000
#define GRAY 0xC618 //http://www.barth-dev.de/online/rgb565-color-picker/

//Unknown screen stuff
// Size of key containers 70px
#define BOXSIZE 40
// Padding
extern double buttonPadding;
// Alignment
extern double timeBox;

void createInterface(int y);
void markers();

#endif
