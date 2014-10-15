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
   NC           19-EOC
 */
 
#include <SPI.h>
#include <avr/pgmspace.h>

#define CS_ADC A5

#define TEMPERATURE_ADC_SIZE  154
#define TEMPERATURE_OFFSET -24

uint16_t values[11] = { 0 };

// Temperature table of ADC Digit Values, the first value is equal to offset value
// Example:  
//            ADC Digit    Temperatue  Inc.
//                  6        -24 °C    
//                 39        -23 °C    +1
//                 75        -22 °C    +1
//                ...        ...       +n 
//                796          0 °C
//                ...        ...
//               3572        130 °C
//            

const uint16_t temperatureADC[] PROGMEM = { 
  6, 39, 72, 104, 137, 169, 201, 233, 265, 297, 329, 361, 393, 424, 456, 487, 519, 550, 581, 
  612, 643, 674, 704, 735, 766, 796, 827, 857, 887, 917, 947, 977, 1007, 1037, 1067, 1096, 1126, 
  1155, 1185, 1214, 1243, 1273, 1302, 1331, 1359, 1388, 1417, 1446, 1474, 1503, 1531, 1559, 1588, 
  1616, 1644, 1672, 1700, 1728, 1755, 1783, 1811, 1838, 1866, 1893, 1920, 1948, 1975, 2002, 2029, 
  2056, 2083, 2110, 2136, 2163, 2190, 2216, 2243, 2269, 2295, 2322, 2348, 2374, 2400, 2426, 2452, 
  2477, 2503, 2529, 2554, 2580, 2605, 2631, 2656, 2682, 2707, 2732, 2757, 2782, 2807, 2832, 2857, 
  2881, 2906, 2931, 2955, 2980, 3004, 3028, 3053, 3077, 3101, 3125, 3149, 3173, 3197, 3221, 3245, 
  3269, 3292, 3316, 3339, 3363, 3386, 3410, 3433, 3456, 3479, 3503, 3526, 3549, 3572
};

class TLC2543 {
  public:
    unsigned bReady:1;       
    int8_t temperature;
    int8_t temperature2;
    
  public:
    void begin() {
      pinMode(CS_ADC, OUTPUT);
      digitalWrite(CS_ADC, HIGH);
      readAdcAll();
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
      
      digitalWrite(CS_ADC, LOW);
      delayMicroseconds(10);
      
      ad = SPI.transfer((chx << 4) | 0x0C);  // 0x0C = 16-Bit, MSB-First, Unipolar
      ad_l = SPI.transfer(0);
      
      digitalWrite(CS_ADC, HIGH);
      
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
        
        //temperature = (int8_t)(values[0] * 0.036569 - 27.662130);        
        //temperature2 = (int8_t)(values[1] * 0.036569 - 27.662130);
      
        temperature = -127;
        temperature2 = -127;
      
        for(uint8_t i = 0; i < TEMPERATURE_ADC_SIZE; i++) {
          
          uint16_t value = pgm_read_word_near(temperatureADC + i);
          
          if(temperature == -127 && values[0] < value) {
            temperature = i + TEMPERATURE_OFFSET;            
          }
          if(temperature2 == -127 && values[1] < value) {
            temperature2 = i + TEMPERATURE_OFFSET;            
          }
          
          if(temperature != -127 && temperature2 != -127) {
            break;
          }
        }
                
//        Serial.print("T1:"); Serial.println(values[0]);
//        Serial.print("T2:"); Serial.println(values[1]);
//        Serial.print("T3:"); Serial.println(values[2]);
//        
//        Serial.print("T1:"); Serial.println(temperature);
//        Serial.print("T1:"); Serial.println(t);
        
        bReady = true;
      }
    }
};

TLC2543 Measure;

#endif
