#include "sensors.h"

// Define variables for DHT temperature and humidity data...
float t1, t2, t3, t4, t5, t6, t7, t8 = 0.0;
float h1, h2, h3, h4, h5, h6, h7, h8 = 0.0;
char degree = '*';

DHT solar1(DHT1, DHTTYPE);
DHT solar2(DHT2, DHTTYPE);
DHT solar3(DHT3, DHTTYPE);
DHT solar4(DHT4, DHTTYPE);
DHT solar5(DHT5, DHTTYPE);
DHT solar6(DHT6, DHTTYPE);
DHT solar7(DHT7, DHTTYPE);
DHT solar8(DHT8, DHTTYPE);

// Read function: temperature and humidity.
void readSensors(){
  // Temperature.
  t1 = solar1.readTemperature();
  t2 = solar2.readTemperature();
  t3 = solar3.readTemperature();
  t4 = solar4.readTemperature();
  t5 = solar5.readTemperature();
  t6 = solar6.readTemperature();
  t7 = solar7.readTemperature();
  t8 = solar8.readTemperature();

  // Humidity.
  h1 = solar1.readHumidity();
  h2 = solar2.readHumidity();
  h3 = solar3.readHumidity();
  h4 = solar4.readHumidity();
  h5 = solar5.readHumidity();
  h6 = solar6.readHumidity();
  h7 = solar7.readHumidity();
  h8 = solar8.readHumidity();
}

float solarEfficiency(){
  // Standard dryer uses about 3k~ Watts, assume 1 hour usage
  const float standard = 3000;

  // Practical value contains energy spent on controller, sensors, fans, magnetics (add more here); the sum of device powers.
  // Practical = E(D_i)
  // P = IV
  // Controller
  // P = 12 * 0.05 = 0.6 Watts
  // Sensor
  // P = 6 * 0.0015 = 0.009 Watts
  // Fan 6"
  // P = 37 Watts
  // Fan 10"
  // P = 200 Watts
  // Practical = 0.6 + (8 * 0.009) + 37 + 200 = 237.672
  
  float practical = 237.672 * 3;
  
  float energySaved = 100 - ((practical / standard) * 100);
  return energySaved;
}

float averageTemp(){
  return t1;
  //return (t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8) / 8;
}

float averageHumi(){
  return h1;
  //return (h1 + h2 + h3 + h4 + h5 + h6 + h7 + h8) / 8;
}
