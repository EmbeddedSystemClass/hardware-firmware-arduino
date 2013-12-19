/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 This example code is in the public domain.
 	 
 */

#include <LiquidCrystal.h>
#include <SPI.h>
//#include <SD.h>
#include <avr/pgmspace.h>
#include <swRTC.h>
#include "main.h"
#include "display.h"
#include "events.h"
#include "measure.h"
#include "data.h"
#include "menu.h"
#include "statemanager.h"
#include "edit.h"

void setup()
{  
  // initialize the pushbutton pin as an input:
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);

  lcd.begin(16, 2);
 
  rtc.stopRTC(); //stop the RTC
  rtc.setTime(20, 42, 0); //set the time here
  rtc.setDate(1, 1, 2000); //set the date here
  rtc.startRTC(); //start the RTC

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  LogEvents.start();
}

void loop() {  
  Events.doHandleEvents();
  MeasureEvents.doHandleEvents();
  LogEvents.doHandleEvents();
  StateMachine.doHandleStates();
//  PT1000.doMeasure();  
  LogData.process();  
}

byte showMenu(byte input) {
  
  byte y = 2;
  byte group;
  byte state;
  
  PGM_P menuText;
  
  static byte count = 0;
  
  for (byte i=0; (group = pgm_read_byte(&menu_state[i].group)); i++) {
    if (group == StateMachine.stateGroup) {
      state = pgm_read_byte(&menu_state[i].state);
      menuText = (PGM_P)pgm_read_word(&menu_state[i].pText);
      if (menuText != NULL) {
        if (state == group) {          
          lcd.print_f(0, 0, menuText);   // draw menu title                     
        } else {
          lcd.print_f(0, y, menuText);  // draw menu item          
        }
        y++;
      }
    }
  }
  return StateMachine.state;
}

byte exitMainMenu(byte input) {
  return ST_MAIN;
}

byte exitDateTimeMenu(byte input) {
  return ST_MAIN_MENU;
}

byte mainScreen(byte input) {
  static byte enter = 0;
  if(!enter) {  
    lcd.clear();
    lcd.print_f(0, 0, PSTR("Logger V1.0"));
    enter = true;
  }


  char buffer[9]= { "00:00:00" };  
  if (Events.bT1000MS) {
    itochars(rtc.getHours(), &buffer[0], 2);
    itochars(rtc.getMinutes(), &buffer[3], 2);
    itochars(rtc.getSeconds(), &buffer[6], 2);  
    
    lcd.print(0, 1, buffer);
  }


  if (input == KEY_ENTER) {
    enter = false;
    return ST_TIME;
  }  
  return ST_MAIN;
}

byte setLogging(byte input) {
  //return LogSettingsScreen.execute(input);
  return ST_MAIN;
}

byte setRtcTime(byte input) {
  static EditTime edTime;
  static byte enter = 0;
  
  if(!enter) {
    lcd.clear();
    lcd.print_f(0, 0, PSTR("Set Time"));
    enter = true;
  }
  
  if (!edTime.editTime(input)) {
    enter = false;
    return ST_MAIN;
  }
  return ST_TIME;
}

byte setRtcDate(byte input) {
  //return EditDateScreen.execute(input);  
  return ST_MAIN;
}

byte temperatureChart(byte input) {
  //return TempChartScreen.execute(input);
  return ST_MAIN;
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


    
