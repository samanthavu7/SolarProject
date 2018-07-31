#include "screen.h"

// Function used to call once for creating interface.
void createInterface(int y){ //purpose of y?
  // Refresh
  tft.fillRect(80, 10, 85, 55, PASTELGREEN); //what is this?
  // Create data values: Time, Temperature, etc.
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(10, 10);
  tft.print("Time: "); tft.print(solarTime); tft.println(" min");
  tft.setCursor(10, 30);
  tft.print("Temp: "); tft.print(averageTemp()); tft.print(" "); tft.print((char)247); tft.println("C");
  tft.setCursor(10, 50);
  tft.print("Humi: "); tft.print(averageHumi()); tft.println(" %");
  tft.setCursor(10, 70);
  tft.setTextSize(1);
  tft.print(solarEfficiency()); tft.println(" % energy saved.");  
}

void markers(){
  // Markers
  tft.fillRect(tft.width() - 70, 130, 10, 2, WHITE);
  tft.fillRect(tft.width() - 70, 190, 10, 2, WHITE);
  tft.fillRect(tft.width() - 70, 250, 10, 2, WHITE);
  tft.fillRect(tft.width() - 70, 310, 10, 2, WHITE);
}

