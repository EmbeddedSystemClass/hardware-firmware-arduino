
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
   07-CS        15-CS
 
   VCC          14-REF+
   GND          13-REF-
   VCC          20-VCC
   GND          10-GND
                19-EOC  NC
 */
 
#include <SPI.h>
#include "TLC2543.h"
#include "RV3049.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  SPI.begin();
 
  ADC2543.begin();
  RTC.begin();
  
}

void loop() {
  
  RTC.getDateTime();
  ADC2543.readAdcAll();

  Serial.println(String(DateTime.hours)+":"+String(DateTime.minutes)+":"+String(DateTime.seconds));

  for(uint8_t channel = 0; channel < 11; channel++)
  {    
    Serial.print("Channel " + String(channel) + "=" + String(values[channel]) + "\t");
  }
  
  Serial.println();
 
  delay(500);  

}

