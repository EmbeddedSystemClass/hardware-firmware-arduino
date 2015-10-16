/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>
#include <avr/pgmspace.h>
//#include <Wire.h>
#include <OneWire.h>
#include <Fat16.h>
#include <swRTC.h>

#include "main.h"
//#include "rtc.h"
#include "event.h"
#include "measure.h"
#include "data.h"
#include "com.h"
#include "web.h"


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  RTC.stopRTC(); //stop the RTC
  RTC.setTime(12,0,0); //set the time here
  RTC.setDate(11,11,2015); //set the date here
  RTC.startRTC(); //start the RTC

  //Wire.begin();
  Measure.begin();
  Measure.dispatch();
  
  // start the Ethernet connection and the server:
  Web.begin();
  
  LogEvents.begin();
  LogEvents.setMode(LOG_INTERVAL_HOUR); 
  
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  Events.dispatch();
  LogEvents.dispatch();  
  Measure.dispatch();
  LogData.dispatch();
  Com.dispatch();
  Web.dispatch();
}

void bin2asc(int value, char buffer[], uint8_t digits) {
  if(digits > 5) {
    buffer[0] = 'Err';
    return;
  }
  
  uint8_t i = 0;
  uint8_t n = 0;
  uint8_t d;
  unsigned int k;

  if(value < 0) {
    value = -value;
    buffer[n] = '-';
    n++;
    digits--;
  }
  
  unsigned int P[] = { 1, 10, 100, 1000, 10000 };
  
  k = P[digits-1];

  while(i < digits) {
    d = value / k;
    value -= (d * k);
    buffer[n] = d + '0';
    k /= 10;
    i++;
    n++;
  }
}
