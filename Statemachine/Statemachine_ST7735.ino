/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 This example code is in the public domain.
 	 
 */

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
//#include <SD.h>
#include <Sensirion.h>
#include <avr/pgmspace.h>
#include <swRTC.h>


#include "main.h"
#include "events.h"
#include "displaymanager.h"
#include "edit.h"
#include "charts.h"
#include "measure.h"

#include "menu.h"

#include "statemanager.h"
#include "screen.h"

void setup()
{  
  // initialize the pushbutton pin as an input:
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);

  Display.beginDisplay();
 
  rtc.stopRTC(); //stop the RTC
  rtc.setTime(20, 42, 0); //set the time here
  rtc.setDate(1, 1, 2000); //set the date here
  rtc.startRTC(); //start the RTC

  // Open serial communications and wait for port to open:
  Serial.begin(9600); 
}

void loop() {  
  Events.doHandleEvents();
  MeasureEvents.doHandleEvents();
  StateMachine.doHandleStates();
  ShtMeasure.doMeasure();
}

byte showMenu(byte input) {
  MenuScreen.show();
  MenuScreen.draw();
  return MenuScreen.input(input);
}

byte exitMainMenu(byte input) {
  return ST_MAIN;
}

byte exitDateTimeMenu(byte input) {
  return ST_MAIN_MENU;
}

byte mainScreen(byte input) {
  MainScreen.show();
  
  MainScreen.draw();
  
  if (input == KEY_ENTER) {
    MainScreen.hide();
    return ST_MAIN_MENU;
  }  
  return ST_MAIN;
}

byte setLogging(byte input) {
  LogSettingsScreen.show();
  
  LogSettingsScreen.edit(input);
  
  if (input == KEY_ENTER) {
    return ST_MAIN; 
  }
  
  return ST_LOGGING;
}

byte setRtcTime(byte input) {
  EditTimeScreen.show();
  byte r = EditTimeScreen.editTime(input);
  EditTimeScreen.draw();
  return r;
}

byte setRtcDate(byte input) {
  EditDateScreen.show();
  byte r = EditDateScreen.editDate(input);
  EditDateScreen.draw();
  return r;  
}

byte temperatureChart(byte input) {
  
  //Display.clearDisplay(); 
  
  for (int8_t i= -5; i < 24-5; i++)
    TemperatureChart.assignValue(i);
  
  
  TemperatureChart.drawTempChart(input);
  
  return ST_TEMP_CHART;
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

    
