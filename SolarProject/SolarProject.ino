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

// Define the sensor type and pins.
#define DHTTYPE DHT22

// There are total of 8 sensors in the system, create new class objects in respect to its pin number.
#define DHT1 22
#define DHT2 24
#define DHT3 26
#define DHT4 28
#define DHT5 30
#define DHT6 32
#define DHT7 34
#define DHT8 36

DHT solar1(DHT1, DHTTYPE);
DHT solar2(DHT2, DHTTYPE);
DHT solar3(DHT3, DHTTYPE);
DHT solar4(DHT4, DHTTYPE);
DHT solar5(DHT5, DHTTYPE);
DHT solar6(DHT6, DHTTYPE);
DHT solar7(DHT7, DHTTYPE);
DHT solar8(DHT8, DHTTYPE);

// Define variables for: temperature, humidity, LEDs, relays, time, and symbols.
float t1, t2, t3, t4, t5, t6, t7, t8 = 0.0;
float h1, h2, h3, h4, h5, h6, h7, h8 = 0.0;
char degree = '*';

// Device pins: Odd numbers
int device1 = 23;
int device2 = 25;
int device3 = 27;
int device4 = 29;
int device5 = 31;
int device6 = 33;
int device7 = 35;

//Phase LED pins: set pinMode() in setup(). digitalWrite() to HIGH at the beginning of each phase
int initialPhase = 37;
int waitPhase = 38;
int executePhase = 39;
int pausePhase = 40;

//Sensor fail LED pins
int sensor1fail = 41;
int sensor2fail = 42;
int sensor3fail = 43;
int sensor4fail = 44;
int sensor5fail = 45;
int sensor6fail = 46;
int sensor7fail = 47;
int sensor8fail = 48;

// Button
string currentButton = "";
bool barEntered = false;
const int buttonUp = 53;
const int buttonDown = 51;
const int buttonEnter = 49;
const int buttonEmergency = A11; 

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

// State Machine.
enum Solar{Initial, Wait, Execute, Pause}phase;
void solar(){
  switch(phase)
  {
    // Setting up the program.
    case Initial:
      digitalWrite(initialPhase, HIGH); //light corresponding LED
      currentButton = "START";
      highlight();
      solarTime = 0;
      nextPosition = 0;
      start = false;
      initializeButtons();
      phase = Wait;
      break;

    // Listening for inputs.
    case Wait:
      digitalWrite(waitPhase, HIGH);
      
//       buttonState = digitalRead(buttonOn);
//       if(buttonState == HIGH){
//         digitalWrite(light, HIGH);
//         Y = 140;
//         tft.fillRect(timeBox, Y, BOXSIZE, tft.height() - Y, WHITE);
//         tft.fillRect(timeBox, 0, BOXSIZE, Y + 1, PASTELGREEN);
        
//         solarTime = tft.height() - Y;
//         nextPosition = Y;
//         createInterface(solarTime);
//         start = true;
//       }
      
      if(currentButton == "START") {
        if(digitalRead(buttonDown) == HIGH) {
          currentButton = "BAR";
          highlight();
        }
        else if(digitalRead(buttonEnter) == HIGH) {
          Y = 140;
          tft.fillRect(timeBox, Y, BOXSIZE, tft.height() - Y, WHITE);
          tft.fillRect(timeBox, 0, BOXSIZE, Y + 1, PASTELGREEN);
          solarTime = tft.height() - Y;
          nextPosition = Y;
          createInterface(solarTime);
          start = true;
        }
      }
      else if(currentButton == "BAR") {
        if(barEntered) {
          if(digitalRead(buttonUp) == HIGH) {
            tft.fillRect(timeBox, Y, BOXSIZE, tft.height() - Y, WHITE);
            tft.fillRect(timeBox, 0, BOXSIZE, Y + 1, PASTELGREEN);

            ++solarTime;
            --nextPosition;
            createInterface(solarTime);
          }
          else if(digitalRead(buttonDown) == HIGH) {
            tft.fillRect(timeBox, Y, BOXSIZE, tft.height() - Y, WHITE);
            tft.fillRect(timeBox, 0, BOXSIZE, Y + 1, PASTELGREEN);

            ++solarTime;
            --nextPosition;
            createInterface(solarTime);
          }
          else { //digitalRead(buttonEnter) == HIGH
            barEntered = false;
          }
        }
        else { bar not entered
          if(digitalRead(buttonEnter) == HIGH) {
            barEntered = true;
          }
          else if(digitalRead(buttonUp) == HIGH) {
            currentButton = "START";
            highlight();
          }
      }
      
      digitalWrite(device1, LOW);
      digitalWrite(device2, LOW);
      digitalWrite(device3, LOW);
      digitalWrite(device4, LOW);
      digitalWrite(device5, LOW);
      digitalWrite(device6, LOW);
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
      digitalWrite(executePhase, HIGH);
      
      currentButton = "STOP";
      highlight();
      if(digitalRead(buttonEnter) == HIGH) {
        start = false;
        stopButton();
      }
      
      digitalWrite(device1, HIGH);
      digitalWrite(device2, HIGH);
      digitalWrite(device3, HIGH);
      digitalWrite(device4, HIGH);
      digitalWrite(device5, HIGH);
      digitalWrite(device6, HIGH);
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
      digitalWrite(pausePhase, HIGH);
      
      currentButton = "START";
      highlight();
      if(digitalRead(buttonEnter) == HIGH) {
        start = true;
        startButton();
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
  solar4.begin();
  solar5.begin();
  solar6.begin();
  solar7.begin();
  solar8.begin();

  createInterface(180);
  initializeButtons();
  
  //pinMode(button, INPUT);

  // Setup devices
  pinMode(device1, OUTPUT);
  pinMode(device2, OUTPUT);
  pinMode(device3, OUTPUT);
  pinMode(device4, OUTPUT);
  pinMode(device5, OUTPUT);
  pinMode(device6, OUTPUT);

  digitalWrite(device1, LOW);
  digitalWrite(device2, LOW);
  digitalWrite(device3, LOW);
  digitalWrite(device4, LOW);
  digitalWrite(device5, LOW);
  digitalWrite(device6, LOW);
  
  pinMode(initialPhase, OUTPUT);
  pinMode(waitPhase, OUTPUT);
  pinMode(executePhase, OUTPUT);
  pinMode(pausePhase, OUTPUT);
  
  digitalWrite(initialPhase, LOW);
  digitalWrite(waitPhase, LOW);
  digitalWrite(executePhase, LOW);
  digitalWrite(pausePhase, LOW);
  
  pinMode(sensor1fail, OUTPUT);
  pinMode(sensor2fail, OUTPUT);
  pinMode(sensor3fail, OUTPUT);
  pinMode(sensor4fail, OUTPUT);
  pinMode(sensor5fail, OUTPUT);
  pinMode(sensor6fail, OUTPUT);
  pinMode(sensor7fail, OUTPUT);
  pinMode(sensor8fail, OUTPUT);
  
  digitalWrite(sensor1fail, LOW);
  digitalWrite(sensor2fail, LOW);
  digitalWrite(sensor3fail, LOW);
  digitalWrite(sensor4fail, LOW);
  digitalWrite(sensor5fail, LOW);
  digitalWrite(sensor6fail, LOW);
  digitalWrite(sensor7fail, LOW);
  digitalWrite(sensor8fail, LOW);
  
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonEnter, INPUT);
  pinMode(buttonEmergency, INPUT);
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
    // Log data every 2 minutes.
    if(index % 2 == 0){
      readSensors();
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
  }
  touch();
  solar();
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

void highlight() {
  if(currentButton == "START") {
    tft.drawRect(10, 160, 100, 30, BLACK);
    tft.setCursor(10 + buttonPadding, 160 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Start");
  }
  else if(currentButton == "STOP") {
    tft.drawRect(10, 200, 100, 30, BLACK);
    tft.setCursor(10 + buttonPadding, 200 + buttonPadding);
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.print("Stop");
  }
  else { //currentButton == "BAR"
    tft.fillRect(timeBox, Y, BOXSIZE, tft.height() - Y, BLACK);
    //tft.fillRect(timeBox, 0, BOXSIZE, Y + 1, PASTELGREEN);
  }
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
  tft.setTextSize(1);
  tft.print(solarEfficiency()); tft.println(" % energy saved.");  
}

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

// Print function.
void printData(){
  Serial.println("\tS1\t\tS2\tS3\tS4\tS5\tS6\tS7\tS8\t");
  Serial.print("T\t"); 
  Serial.print(t1);
  Serial.print(degree);
  Serial.print("C\t\t");
  Serial.print(t2);
  Serial.print(degree);
  Serial.print("C\t\t");
  Serial.print(t3);
  Serial.print(degree);
  Serial.print("C\t\t");
  Serial.print(t4);
  Serial.print(degree);
  Serial.print("C\t\t");
  Serial.print(t5);
  Serial.print(degree);
  Serial.print("C\t\t");
  Serial.print(t6);
  Serial.print(degree);
  Serial.print("C\t\t");
  Serial.print(t7);
  Serial.print(degree);
  Serial.print("C\t\t");
  Serial.print(t8);
  Serial.print(degree);
  Serial.println("C");
  Serial.print("H\t");
  Serial.print(h1);
  Serial.print("%\t\t");
  Serial.print(h2);
  Serial.print("%\t\t");
  Serial.print(h3);
  Serial.print("%\t\t");
  Serial.print(h4);
  Serial.print("%\t\t");
  Serial.print(h5);
  Serial.print("%\t\t");
  Serial.print(h6);
  Serial.print("%\t\t");
  Serial.print(h7);
  Serial.print("%\t\t");
  Serial.print(h8);
  Serial.println("%\n");
  Serial.println("\n");
}

// Write function.
void writeData(){
  solarData.println("\tS1\tS2\tS3\tS4\tS5\tS6\tS7\tS8\t");
  solarData.print("T\t"); 
  solarData.print(t1);
  solarData.print(degree);
  solarData.print("C\t");
  solarData.print(t2);
  solarData.print(degree);
  solarData.print("C\t");
  solarData.print(t3);
  solarData.print(degree);
  solarData.print("C\t");
  solarData.print(t4);
  solarData.print(degree);
  solarData.print("C\t");
  solarData.print(t5);
  solarData.print(degree);
  solarData.print("C\t");
  solarData.print(t6);
  solarData.print(degree);
  solarData.print("C\t");
  solarData.print(t7);
  solarData.print(degree);
  solarData.print("C\t");
  solarData.print(t8);
  solarData.print(degree);
  solarData.println("C");
  solarData.print("H\t");
  solarData.print(h1);
  solarData.print("%\t");
  solarData.print(h2);
  solarData.print("%\t");
  solarData.print(h3);
  solarData.print("%\t");
  solarData.print(h4);
  solarData.print("%\t");
  solarData.print(h5);
  solarData.print("%\t");
  solarData.print(h6);
  solarData.print("%\t");
  solarData.print(h7);
  solarData.print("%\t");
  solarData.print(h8);
  solarData.println("%\n");
  solarData.println("\n");
}

void markers(){
  // Markers
  tft.fillRect(tft.width() - 70, 140, 10, 2, WHITE);
  tft.fillRect(tft.width() - 70, 200, 10, 2, WHITE);
  tft.fillRect(tft.width() - 70, 260, 10, 2, WHITE);
}

