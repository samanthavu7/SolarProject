#include <Adafruit_TFTLCD.h>
#include <pin_magic_MEGA.h>
#include <pin_magic_UNO.h>
#include <registers.h>

/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pins 2-5 on the Arduino 
#define UNO 2 
#define DOS 3
#define TRES 4
#define CUATRO 5

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
/********************************************************************/
 Serial.print("Temperature 1 is: "); 
 Serial.println(unoSensor.getTempCByIndex(0)); // Why "byIndex"?  
  Serial.print("Temperature 2 is: "); 
 Serial.println(dosSensor.getTempCByIndex(0)); // Why "byIndex"? 
  Serial.print("Temperature 3 is: "); 
 Serial.println(tresSensor.getTempCByIndex(0)); // Why "byIndex"? 
  Serial.print("Temperature 4 is: "); 
 Serial.println(cuatroSensor.getTempCByIndex(0)); // Why "byIndex"? 
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   delay(1000); 
} 
