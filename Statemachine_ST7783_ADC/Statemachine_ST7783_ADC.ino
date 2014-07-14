/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 This example code is in the public domain.
 	 
 */

#include <Adafruit_GFX.h>    // Core graphics library
#include <SWTFT.h> // Hardware-specific library
#include <TouchScreen.h>
#include <SPI.h>
#include <Fat16.h>
//#include <Fat16util.h> // use functions to print strings from flash memory
#include <avr/pgmspace.h>

#include "main.h"
#include "rtc.h"
#include "display.h"
#include "event.h"
#include "measure.h"
#include "data.h"
#include "com.h"
#include "chart.h"
#include "screen.h"

void setup()
{
  // Open serial communications
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println(F("Logger\r\n"));
  
  // Initialize Display
  Display.beginDisplay();

  SPI.begin();
  
  RTC.begin();

  Measure.begin();

  initScreens();  
  LogEvents.start();
  LogEvents.setMode(LOG_INTERVAL_HOUR);
}

void loop() {
  //unsigned long s1 = millis();
  Events.dispatch();
  pScreen->dispatch(0);  
  LogEvents.dispatch();  
  Measure.dispatch();
  LogData.dispatch();
  Com.dispatch();
  RTC.dispatch();  
  //Serial.println(millis()-s1);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new uint8_t:
    char inChar = (char)Serial.read(); 
    Com.receive(inChar); 
  }
}

void bin2asc(unsigned int value, char buffer[], uint8_t digits) {
  uint8_t i = 0; 
  uint8_t d;
  unsigned int k;
  
  unsigned int P[] = { 1, 10, 100, 1000, 10000 };
  
  k = P[digits-1]; 

  while(i < digits) {
    d = value / k;
    value -= (d * k);
    buffer[i] = d + '0';
    k /= 10;
    i++;
  }
}
 
