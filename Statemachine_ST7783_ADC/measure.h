#ifndef _MEASUREH_
#define _MEASUREH_

/*
  TLC 2543 AnalogReadSerial
  Reads an analog input on pin AIN0 to AIN10 , prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */

/*
   Pin configuration
 
   Arduino      TLC2543
 
   13-SCK       18-I/O CLOCK 
   12-MISO      16-DATA OUT
   11-MOSI      17-DATA INPUT
   07-CS        A4-CS
 
   VCC          14-REF+
   GND          13-REF-
   VCC          20-VCC
   GND          10-GND
                19-EOC  NC
 */
 
#include <SPI.h>

#define chipSelectPin A4

uint16_t values[11] = { 0 };

class TLC2543 {
  public:
    unsigned bReady:1;       
    int8_t temperature;
    int8_t temperature2;
    
  public:
    void begin() {
      pinMode(chipSelectPin, OUTPUT);
      digitalWrite(chipSelectPin, HIGH);
    }
    
    void readAdcAll() {
      uint16_t value = 0;
      for(uint8_t channel = 1; channel < 11; channel++)
      {
        values[channel - 1] = readAdc(channel);
      }
      values[10] = readAdc(0);
    }
    
    uint16_t readAdc(uint8_t chx) {
      uint16_t ad;
      uint8_t ad_l; 
      
      digitalWrite(chipSelectPin, LOW);
      delayMicroseconds(10);
      
      ad = SPI.transfer((chx << 4) | 0x0C);  // 0x0C = 16-Bit, MSB-First, Unipolar
      ad_l = SPI.transfer(0);
      
      digitalWrite(chipSelectPin, HIGH);
      
      ad <<= 8;  
      ad |= ad_l;  
      ad >>= 4;
      
      delayMicroseconds(10);  // EOC
      
      return ad;
    }
    
    void dispatch() {
      bReady = false;
      
      if (Events.bT5S) {                
        readAdcAll();
        
        temperature = (uint8_t)values[3] & 0x000F;	// get first byte

        
        temperature2 = (uint8_t)values[2] & 0x000F;	// get first byte
        
        //Serial.print("T1:"); Serial.println(values[0]);
        //Serial.print("T2:"); Serial.println(values[1]);
        //Serial.print("T3:"); Serial.println(values[2]);
        
        bReady = true;
      }
    }
};

TLC2543 DS1621;



#endif
