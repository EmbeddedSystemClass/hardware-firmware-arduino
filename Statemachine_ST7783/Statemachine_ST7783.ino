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
#include <SD.h>
#include <Sensirion.h>
#include <avr/pgmspace.h>
#include <swRTC.h>
#include <OneWire.h>

#include "main.h"
#include "display.h"
#include "event.h"
#include "measure.h"
#include "data.h"
#include "chart.h"
#include "screen.h"

void setup()
{  
  Display.beginDisplay();
 
  rtc.stopRTC(); //stop the RTC
  rtc.setTime(20, 42, 0); //set the time here
  rtc.setDate(1, 1, 2000); //set the date here
  rtc.startRTC(); //start the RTC

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  initScreens();
  
  LogEvents.start();
}

void loop() {  
  Events.doHandleEvents();
  MeasureEvents.doHandleEvents();
  pScreen->execute(0);
  
  LogEvents.doHandleEvents();

  //ShtMeasure.doMeasure();
  DS1821.doMeasure();
  LogData.process();  
}



void logError(String s) {
  //displayText(0, 0, 1, s);
}

void itochars(unsigned int value, char buffer[], byte digits) {
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


    
