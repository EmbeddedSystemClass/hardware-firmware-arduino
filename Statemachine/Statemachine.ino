/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 This example code is in the public domain.
 	 
 */

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
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
#include "statemanager.h"


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
  StateMachine.doHandleStates();  
}

byte showMenu(byte input) {
  byte i, j;
  byte y = 0;
  byte stateTemp = 0;
  PGM_P statetext;
  
  static byte menuState;
  
  Display.clearDisplay();
  
  for (i=0; (j=pgm_read_byte(&menu_state[i].group)); i++) {
    if (j == StateMachine.stateGroup) {
      stateTemp = pgm_read_byte(&menu_state[i].state);
      statetext = (PGM_P)pgm_read_word(&menu_state[i].pText);
      if (statetext != NULL) {
        if (menuState == 0) {
          menuState = stateTemp;
        }
        Display.displayText_f(2, y * (TEXTHEIGHT + 1) + 1, 1, statetext);
        if (menuState == stateTemp) {
          Display.drawRect(0, y * (TEXTHEIGHT + 1), LCDWIDTH, 10, BLACK);
        }
        y++;
      }
    }
  }
  
  Display.display();
  
  if (Events.bBtn2) {
    menuState=StateMachine.getNextState(menuState, KEY_PLUS);
  } else if (Events.bBtn1) {
    stateTemp = menuState;
    StateMachine.stateGroup = menuState;
    menuState = 0;
    return stateTemp;
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
  Display.clearDisplay();
  Display.displayText_f(0, 0, 1, PSTR("App Test"));  
  
  char time[]= { "00:00:00" };  
  itochars(rtc.getHours(), &time[0], 2);
  itochars(rtc.getMinutes(), &time[3], 2);
  itochars(rtc.getSeconds(), &time[6], 2);  
  Display.displayText(0, 20, 1, time);
    
  char date[]= { "0000.00.00" };  
  itochars(rtc.getYear(), &date[0], 4);
  itochars(rtc.getMonth(), &date[5], 2);
  itochars(rtc.getDay(), &date[8], 2);  
  Display.displayText(0, 30, 1, date);
  
  Display.display();
  
  if (input == KEY_ENTER) {
    return ST_MAIN_MENU;
  }  
  return ST_MAIN;
}

byte setLogging(byte input) {
  static byte logState;
  
  if (logState == 0) {
    logState = ST_YES;
  }
  
  Display.clearDisplay();
  Display.displayText_f(0,  0, 1, PSTR("Reset Log"));
  EditYesNo.getOption(input);  
  Display.display();
  
  if (input == KEY_ENTER) {
    return ST_MAIN; 
  }
  
  return ST_LOGGING;
}

byte setRtcTime(byte input) {
  Display.clearDisplay();
  Display.displayText_f(0,0,1,PSTR("Set RTC Time"));
  
  static EditTime edTime;
      
  if (!edTime.editTime(input)) {
    return ST_DATE_TIME_MENU;
  }

  Display.display();
    
  return StateMachine.state;
}

byte setRtcDate(byte input) {
  Display.clearDisplay();
  Display.displayText_f(0,0,1,PSTR("Set RTC Date"));
  
  static EditDate edDate;
       
  if (!edDate.editDate(input)) {
    return ST_DATE_TIME_MENU;
  }

  Display.display();
    
  return StateMachine.state;
}

byte temperatureChart(byte input) {
  
  Display.clearDisplay(); 
  
  for (int8_t i= -5; i < 24-5; i++)
    TemperatureChart.assignValue(i);
  
  
  TemperatureChart.drawTempChart(input);

  Display.display();
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

    
