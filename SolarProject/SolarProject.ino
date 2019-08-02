/* Solar Project
 * Jeffrey Hsieh
 * 
 * The following code contains open-source references from MIT.
 * Several modications has been made to meet the objective of this research.
 * Credits can be found in header files or online by searching Adafruit tutorials. 
 */

#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <Adafruit_TFTLCD.h>
#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <pin_magic_MEGA.h>
#include <registers.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define YP A3  // Analog only
#define XM A2  // Analog only
#define YM 9   // Digital
#define XP 8   // Digital

// calibration mins and max for raw data when touching edges of screen
#define TS_MINX 210
#define TS_MINY 210
#define TS_MAXX 915
#define TS_MAXY 910

//SPI Communication
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// Reset pin: optional
#define LCD_RESET A4

#if defined __AVR_ATmega2560__
#define SD_SCK 13
#define SD_MISO 12
#define SD_MOSI 11
#endif

#define SD_CS 10

//Color Definitons
#define PASTELGREEN 0x6F92
#define BLACK     0x0000
#define WHITE   0xFFFF

#define MINPRESSURE 1
#define MAXPRESSURE 1000

// Pins A2-A6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 364);

//Size of key containers 70px
#define BOXSIZE 40

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//Container variables for touch coordinates
int X, Y, Z;

// Padding
double buttonPadding = 5;

// Alignment
double timeBox = tft.width() - BOXSIZE - 10;

// Runtime varibles.
int solarTime = 0;
int nextPosition = 0;
bool start = false;

// Define the air sensor type and pins.
#define DHTTYPE DHT22

// Temperature sensors
#define DHT1 22 // ambient
#define DHT2 24 // after solar collector
#define DHT3 26 // center of closet

// Water sensors
#define W1 30 // ADD
#define W2 32 // ADD

DHT solar1(DHT1, DHTTYPE);
DHT solar2(DHT2, DHTTYPE);
DHT solar3(DHT3, DHTTYPE);

OneWire w_wire1(W1);
OneWire w_wire2(W2);

DallasTemperature w_sensor1(&w_wire1);
DallasTemperature w_sensor2(&w_wire2);

float t1, t2, t3 = 0.0; // air temperatures
float h1, h2, h3 = 0.0; // air humidities
float w1, w2 = 0.0; // water temperatures
char degree = '*';

// Fan speed controller pins
int sync_pin = 21;
int channel_1_pin = 44; 
int channel_2_pin = 45; 

int power_level_ch1 = 0;  
int power_level_ch2 = 0;

// Button
int buttonState = 0;
const int buttonOn = 51;
const int buttonOff = 49;

// Light
const int light = 53;

// Time
unsigned long pM = 0;
unsigned long index = 0;
bool secBox = false;
// Change here for time debug: 60000
const long interval = 60000;

// Initialize SD card.
// 140 bytes per log session.
File solarData;
File waterData;

// State Machine.
enum Solar{Initial, Wait, Execute, Pause}phase;
void solar(){
  switch(phase)
  {
    // Setting up the program.
    case Initial:
      solarTime = 0;
      nextPosition = 0;
      start = false;
      initializeButtons();
      phase = Wait;
      break;

    // Listening for inputs.
    case Wait:
      buttonState = digitalRead(buttonOn);
      if(buttonState == HIGH){
        digitalWrite(light, HIGH);
        Y = 140;
        tft.fillRect(timeBox, Y, BOXSIZE, tft.height() - Y, WHITE);
        tft.fillRect(timeBox, 0, BOXSIZE, Y + 1, PASTELGREEN);
        
        solarTime = tft.height() - Y;
        nextPosition = Y;
        createInterface(solarTime);
        start = true;
      }
    
      if (Z > MINPRESSURE && Z < MAXPRESSURE) {
        if(X > timeBox && Y > 139){
          
          // Create process bar.
          tft.fillRect(timeBox, Y, BOXSIZE, tft.height() - Y, WHITE);
          tft.fillRect(timeBox, 0, BOXSIZE, Y + 1, PASTELGREEN);
          
          solarTime = tft.height() - Y;
          nextPosition = Y;
          createInterface(solarTime);
        }
        // Listening for start.
        if(X > 0 && X < 100){
          if(Y > 160 && Y < 190){
            start = true;
            startButton();
          }
        }
      }
      phase = start ? Execute : Wait;
      break;
      
    case Execute:
        power_level_ch1 = 100;
        power_level_ch2 = 100;
        zero_cross_independent();
      
      buttonState = digitalRead(buttonOff);
      if(buttonState == HIGH){
        digitalWrite(light, LOW);
        start = false;
      }
      
      if (Z > MINPRESSURE && Z < MAXPRESSURE) {
        // Listening for stop.
        if(X > 0 && X < 100){
          if(Y > 200 && Y < 230){
            start = false;
            stopButton();
          }
        }
      }

      if(!start){
        phase = Pause;
        break;
      }
      
      // Refresh time.
      phase = (solarTime > 0) ? Execute : Pause;
      break;
      
    case Pause:
      power_level_ch1 = 0; // turn off fans
      power_level_ch2 = 0;
      zero_cross_independent();
        
      buttonState = digitalRead(buttonOn);
      if(buttonState == HIGH){
        digitalWrite(light, HIGH);
        start = true;
      }
      if(solarTime <= 0){
        phase = Initial;
        break;
      }
      if (Z > MINPRESSURE && Z < MAXPRESSURE) {
        // Listening for start.
        if(X > 0 && X < 100){
          if(Y > 160 && Y < 190){
            start = true;
            startButton();
          }
        }
      }
      phase = start ? Execute : Pause;
      break;
  }
}

void water() {
  Serial.println("R'Garden Temperature Readings"); 
  Serial.print(" Requesting temperatures..."); 
  w_sensor1.requestTemperatures(); // Send the command to get temperature readings
  w_sensor2.requestTemperatures(); 
  Serial.println("DONE");

  t1 = w_sensor1.getTempCByIndex(0); 
  t2 = w_sensor2.getTempCByIndex(0);
  delay(1000);

  if(waterData){
    write_waterData();
    waterData.close();
  }
  else{
    Serial.println("Error: unable to open water.txt.");
 }
}

void setup() {
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
  
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);

  //Background color
  tft.fillScreen(PASTELGREEN);
  tft.setRotation(2);

  markers();
  
  // Begin DHT sensors.
  solar1.begin();
  solar2.begin();
  solar3.begin();

  w_sensor1.begin();
  w_sensor2.begin();

  createInterface(180);
  initializeButtons();
  
  //pinMode(button, INPUT);

  // Setup devices
  pinMode(buttonOn, INPUT);
  pinMode(buttonOff, INPUT);
  pinMode(light, OUTPUT);
  digitalWrite(light, LOW);

  pinMode(channel_1_pin, OUTPUT);
  pinMode(channel_2_pin, OUTPUT);
  pinMode(sync_pin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(sync_pin), zero_cross_independent, RISING);
}

void loop() {
  unsigned long cM = millis();
  if (cM - pM >= interval) {
    pM = cM;
    // Update interface every minute.
    if(index % 1 == 0){
      if(phase == Execute){
        nextPosition++;
        solarTime--;
        createInterface(nextPosition);
        tft.fillRect(timeBox, nextPosition, BOXSIZE, 1, PASTELGREEN);
      }
    }
    index++;
  }
  if(phase == Execute){
    if(cM % 1000 == 0){
      if(!secBox){
        tft.drawRect(200, 10, 15, 15, WHITE);
        secBox = true;
      }
      else{
        tft.drawRect(200, 10, 15, 15, PASTELGREEN);
        secBox = false;
      }
    }
    if(cM % 2000 == 0){
      readSensors();
      printData();
      createInterface(nextPosition);
      // Write data to SD card.
      solarData = SD.open("solar.txt", FILE_WRITE);
      if(solarData){
        writeData();
        solarData.close();
      }
      else{
        Serial.println("Error: unable to open solar.txt.");
      }
    }
    
  }
  touch();
  solar();
  water();
}

// Function used to debug touch points.
void debugCoordinates(){
    tft.fillScreen(PASTELGREEN);
    tft.setCursor(10, 150);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("X = "); tft.println(X);
    tft.print("Y = "); tft.println(Y);
}

// Function used to capture touch inputs.
void touch(){
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  X = tft.width() - map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  Y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  // Horizontal
  //X = tft.width() - map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
  //Y = tft.height() - map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
  Z = p.z;
  
}

void initializeButtons(){
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
}

void startButton(){
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
}

void stopButton(){
  tft.drawRect(10, 160, 100, 30, WHITE);
  tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("Start");
  tft.drawRect(10, 200, 100, 30, BLACK);
  tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.print("Stop");
}

// Function used to call once for creating interface.
void createInterface(int y){
  // Refresh
  tft.fillRect(80, 10, 85, 55, PASTELGREEN);
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
//  tft.setTextSize(1);
  tft.print(solarEfficiency()); tft.println("% energy saved");  
}

// Read function: temperature and humidity.
void readSensors(){
  // Temperature.
  t1 = solar1.readTemperature();
  t2 = solar2.readTemperature();
  t3 = solar3.readTemperature();

  // Humidity.
  h1 = solar1.readHumidity();
  h2 = solar2.readHumidity();
  h3 = solar3.readHumidity();
}

float solarEfficiency(){
  // Standard dryer uses about 3k~ Watts, assume 1 hour usage
  const float standard = 3000;

  // Practical value contains energy spent on controller, sensors, fans, magnetics (add more here); the sum of device powers.
  // Practical = E(D_i)
  // P = IV
  // Controller
  // P = (20 mA * 34 pins) * 5 V = 3.4W
  // Screen
  // P = (120 mA * 5 V) = 0.6 W
  // Fan Speed Controller
  // P = (654 mA * 120 V) = 78.48 W
  // Sensors
  // P = 4 * (2.5 mA * 5 V) = 0.05 W
  // Fan (prototype)
  // P = 37 Watts
  // Practical = 119.53
  
  float practical = 119.53 * 3;
  
  float energySaved = 100 - ((practical / standard) * 100);
  return energySaved;
}

float averageTemp(){
  return (t1 + t2 + t3) / 3;
}

float averageHumi(){
  return (h1 + h2 + h3) / 3;
}

// Print function.
void printData(){
  Serial.println("1T(*C)\t1H(%)\t2T\t2H\t3T\t3H\n");
  Serial.print(t1);
  Serial.print("\t");
  Serial.print(h1);
  Serial.print("\t");
  Serial.print(t2);
  Serial.print("\t");
  Serial.print(h2);
  Serial.print("\t");
  Serial.print(t3);
  Serial.print("\t");
  Serial.print(h3);
  Serial.print("\n");
}

// Write function.
void writeData(){
  solarData.println("1T(*C)\t1H(%)\t2T\t2H\t3T\t3H\n");
  solarData.print(t1);
  solarData.print("\t");
  solarData.print(h1);
  solarData.print("\t");
  solarData.print(t2);
  solarData.print("\t");
  solarData.print(h2);
  solarData.print("\t");
  solarData.print(t3);
  solarData.print("\t");
  solarData.print(h3);
  solarData.print("\n");
}

void write_waterData(){
  waterData.println("\tS1\tS2\t");
  waterData.print("T\t"); 
  waterData.print(t1);
  waterData.print(degree);
  waterData.print("C\t");
  waterData.print(t2);
  waterData.print(degree);
  waterData.print("C\t");
  waterData.println("\n");
}

void markers(){
  // Markers
  tft.fillRect(tft.width() - 70, 140, 10, 2, WHITE);
  tft.fillRect(tft.width() - 70, 200, 10, 2, WHITE);
  tft.fillRect(tft.width() - 70, 260, 10, 2, WHITE);
}

void zero_cross() {
  if (power_level_ch1 < 0) {
    return;
  }
  if (power_level_ch1 > 98) {
    power_level_ch1 = 98;
  }
  /*int delay_time_ch1 = 8330 - (83 * power_level_ch1);
  int delay_time_ch2 = 8330 - (83 * power_level_ch2); 
  delayMicroseconds(delay_time_ch1);  
  digitalWrite(channel_1_pin, HIGH);   
  digitalWrite(channel_2_pin, HIGH);   
  delayMicroseconds(1);      
  digitalWrite(channel_1_pin, LOW);   
  digitalWrite(channel_2_pin, LOW);    */
  int pwm = power_level_ch1 * 255 / 100.0;
  analogWrite(channel_1_pin, pwm);
}

void zero_cross_independent() {
  if (power_level_ch1 < 0) {
    return;
  }
  if (power_level_ch1 > 98) {
    power_level_ch1 = 98;
  }

  if (power_level_ch2 < 0) {
    return;
  }
  if (power_level_ch2 > 98) {
    power_level_ch2 = 98;
  }
  /*int delay_time_ch1 = 8330 - (83 * power_level_ch1);
  int delay_time_ch2 = 8330 - (83 * power_level_ch2);
  if (delay_time_ch2 > delay_time_ch1) {
    delayMicroseconds(delay_time_ch1); 
    digitalWrite(channel_1_pin, HIGH); 
    delayMicroseconds(delay_time_ch2 - delay_time_ch1); 
    digitalWrite(channel_2_pin, HIGH); 
    delayMicroseconds(1);        
    digitalWrite(channel_1_pin, LOW);  
    digitalWrite(channel_2_pin, LOW);  
  }
  else {
    delayMicroseconds(delay_time_ch2);  
    digitalWrite(channel_2_pin, HIGH);  
    delayMicroseconds(delay_time_ch1 - delay_time_ch2); 
    digitalWrite(channel_1_pin, HIGH);  
    delayMicroseconds(1);        
    digitalWrite(channel_2_pin, LOW);   
    digitalWrite(channel_1_pin, LOW);   
  }*/
  int pwm1 = power_level_ch1 * 255 / 100.0;
  analogWrite(channel_1_pin, pwm1);
  int pwm2 = power_level_ch2 * 255 / 100.0;
  analogWrite(channel_2_pin, pwm2);
}
