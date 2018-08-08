#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "DHT.h" // DHT sensor header file
/* DHT sensors are the type of sensor used in the system to read the temperature and humidity of the air.
Define the sensor type.*/
#define DHTTYPE DHT22

/* There are total of 8 DHT sensors in the V'Garden system and 4 in the R'Garden system.
Create new class objects in respect to their pin numbers. */
#define DHT1 22
#define DHT2 24
#define DHT3 26
#define DHT4 28
#define DHT5 30
#define DHT6 32
#define DHT7 34
#define DHT8 36

extern DHT solar1;
extern DHT solar2;
extern DHT solar3;
extern DHT solar4;
extern DHT solar5;
extern DHT solar6;
extern DHT solar7;
extern DHT solar8;
//DHT solar1(DHT1, DHTTYPE);
//DHT solar2(DHT2, DHTTYPE);
//DHT solar3(DHT3, DHTTYPE);
//DHT solar4(DHT4, DHTTYPE);
//DHT solar5(DHT5, DHTTYPE);
//DHT solar6(DHT6, DHTTYPE);
//DHT solar7(DHT7, DHTTYPE);
//DHT solar8(DHT8, DHTTYPE);


// Define variables for DHT temperature and humidity data...
extern float t1, t2, t3, t4, t5, t6, t7, t8;
extern float h1, h2, h3, h4, h5, h6, h7, h8;
extern char degree;

void readSensors();
float solarEfficiency();
float averageTemp();
float averageHumi();
void writeData();

#endif
