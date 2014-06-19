
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

const int chipSelectPin = 7;

uint16_t values[11] = { 0 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  SPI.begin();
  
  pinMode(chipSelectPin, OUTPUT);
  
}

void loop() {
  
  readAdcAll();
  
  for(uint8_t channel = 0; channel < 11; channel++)
  {    
    Serial.print("Channel " + String(channel) + "=" + String(values[channel]) + "\t");
  }
  
  Serial.println();
 
  delay(500);  

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
