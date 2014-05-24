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
#include <Fat16util.h> // use functions to print strings from flash memory
#include <Sensirion.h>
#include <avr/pgmspace.h>
#include <swRTC.h>
#include <OneWire.h>

#include "main.h"
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
  Serial.println(F("Logger\r\n"));
  
  // Initialize Display
  Display.beginDisplay();
  
  // Initialize RTC
  rtc.stopRTC(); 	   //stop the RTC
  rtc.setTime(20, 42, 0);  //set the time here
  rtc.setDate(1, 1, 2000); //set the date here
  rtc.startRTC(); 	   //start the RTC
  
  initScreens();  
  LogEvents.start();
//  LogData.initialize();
}

void loop() {  
  Events.dispatch();
  MeasureEvents.dispatch();  
  pScreen->dispatch(0);  
  LogEvents.dispatch();  
  DS1821.dispatch();
  LogData.dispatch();
  Com.dispatch();
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    Com.receive(inChar); 
  }
}

void bin2asc(unsigned int value, char buffer[], byte digits) {
  byte i = 0; 
  byte d;
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
 
