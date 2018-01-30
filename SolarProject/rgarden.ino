#include <Adafruit_TFTLCD.h>
#include <pin_magic_MEGA.h>
#include <pin_magic_UNO.h>
#include <registers.h>

// SD card initialization
#include <SPI.h>
#include <SD.h>

#if defined __AVR_ATmega2560__
#define SD_SCK 13
#define SD_MISO 12
#define SD_MOSI 11
#endif

#define SD_CS 10

File waterData;

/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pins 2-5 on the Arduino 
#define UNO 24 
#define DOS 26
#define TRES 28
#define CUATRO 30

float t1, t2, t3, t4 = 0.0;
char degree = '*';
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) - ie constructors
OneWire unoWire(UNO);
OneWire dosWire(DOS);
OneWire tresWire(TRES);
OneWire cuatroWire(CUATRO); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature unoSensor(&unoWire);
DallasTemperature dosSensor(&dosWire);
DallasTemperature tresSensor(&tresWire);
DallasTemperature cuatroSensor(&cuatroWire);
/********************************************************************/ 
void setup(void) 
{ 
 // start serial port 
 Serial.begin(9600); 
 
 #if defined __AVR_ATmega2560__
  // Begin SD usage.
  if(!SD.begin(SD_CS, SD_MOSI, SD_MISO, SD_SCK)){
     Serial.println("Error: unable to detect SD card.");
  }
  #else
  
  // Begin SD usage.
  if(!SD.begin(SD_CS)){
     Serial.println("Error: unable to detect SD card.");
  }
  #endif
 
 Serial.println("Dallas Temperature IC Control Library Demo"); 
 // Start up the library 
 unoSensor.begin(); 
 dosSensor.begin();
 tresSensor.begin();
 cuatroSensor.begin();
} 
void loop(void) 
{ 
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 Serial.print(" Requesting temperatures..."); 
 unoSensor.requestTemperatures(); // Send the command to get temperature readings
 dosSensor.requestTemperatures(); // Send the command to get temperature readings 
 tresSensor.requestTemperatures(); // Send the command to get temperature readings 
 cuatroSensor.requestTemperatures(); // Send the command to get temperature readings  
 Serial.println("DONE"); 

t1 = unoSensor.getTempCByIndex(0); // Why "byIndex"?  
t2 = dosSensor.getTempCByIndex(0); // Why "byIndex"? 
t3 = tresSensor.getTempCByIndex(0); // Why "byIndex"? 
t4 = cuatroSensor.getTempCByIndex(0); // Why "byIndex"? 
   delay(1000); 
 
/********************************************************************/
      waterData = SD.open("water.txt", FILE_WRITE);
      if(waterData){
        writeData();
        waterData.close();
      }
      else{
        Serial.println("Error: unable to open water.txt.");
      }
 }
 /*******************************************************************/
 void writeData(){
  waterData.println("\tS1\tS2\tS3\tS4\t");
  waterData.print("T\t"); 
  waterData.print(t1);
  waterData.print(degree);
  waterData.print("C\t");
  waterData.print(t2);
  waterData.print(degree);
  waterData.print("C\t");
  waterData.print(t3);
  waterData.print(degree);
  waterData.print("C\t");
  waterData.print(t4);
  waterData.print(degree);
  waterData.print("C\t");
  waterData.println("\n");
}
/**************************************************************/
