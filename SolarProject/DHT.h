/* Solar Project
 * Jeffrey Hsieh
 * 
 * The following code contains open-source references from MIT.
 * Several modications has been made to meet the objective of this research.
 * Credits can be found online by searching Adafruit tutorials. 
 */

// Define Device Name
#ifndef DHT_H
#define DHT_H

// Define Arduino Version
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


// Set debugging output to serial
#define DEBUG_PRINTER Serial


// Debugging Printer
#ifdef DHT_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__);}
  #define DEBUG_PRINTLN(...)
{ DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

// The type of sensor used in Solar Project is DHT 22.
#define DHT22 22

// Class Interface
class DHT {
  public:
    DHT(uint8_t pin, uint8_t type, uint8_t count=6);
    void begin(void);
    float readTemperature(bool S=false, bool force=false);
    float readHumidity(bool force=false);
    float convertCtoF(float);
    float convertFtoC(float);
    float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit=true);
    boolean read(bool force=false);

  private:
    uint8_t data[5];
    uint8_t _pin, _type;
    #ifdef __AVR
      uint8_t _bit, _port;
    #endif
    uint32_t _lastreadtime, _maxcycles;
    bool _lastresult;

    uint32_t expectPulse(bool level);

};

class InterruptLock {
  public:
    InterruptLock(){
      noInterrupts();
    }
    ~InterruptLock(){
      interrupts();
    }


};

#endif
