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
#include "statemanager.h"


//EventManager Events;
//StateMachineManager StateMachine;

void setup()
{  
  // initialize the pushbutton pin as an input:
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);

 
  rtc.stopRTC(); //stop the RTC
  rtc.setTime(20, 42, 0); //set the time here
  rtc.setDate(1, 1, 2000); //set the date here
  rtc.startRTC(); //start the RTC

    // Open serial communications and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  
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
//        byte textColor = menuState == stateTemp ? WHITE : BLACK;
//        byte backColor = menuState == stateTemp ? BLACK : WHITE;
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

byte mainScreen(byte inp) {
  Display.clearDisplay();
  Display.displayText_f(0, 0, 1, PSTR("Menu Test"));
  
  
  char buffer[8];
  buffer[0] = rtc.getHours() / 10 + '0';
  buffer[1] = rtc.getHours() % 10 + '0';
  buffer[2] = ':';
  buffer[3] = rtc.getMinutes() / 10 + '0';
  buffer[4] = rtc.getMinutes() % 10 + '0';  
  buffer[5] = ':';
  buffer[6] = rtc.getSeconds() / 10 + '0';
  buffer[7] = rtc.getSeconds() % 10 + '0';
  buffer[8] = 0;
  
  Display.displayText(0, 20, 1, buffer);
  
  Display.display();
  
  if (Events.bBtn1) {
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
  Display.displayText_f(2, 20, 1, PSTR("YES    NO"));
  
  switch(logState) {
    case ST_YES:
      Display.drawRect(0, 18, 3 * TEXTWIDTH + 3, TEXTHEIGHT + 3, BLACK);
      break;
    case ST_NO:
      Display.drawRect(6 * (TEXTWIDTH + 1), 18, 2 * TEXTWIDTH + 3, TEXTHEIGHT + 3, BLACK);
      break;
  }
  
  Display.display();
  
  if (Events.bBtn2) {
    logState = StateMachine.getNextState(logState, KEY_PLUS);
  } else if (Events.bBtn1) {
    return ST_MAIN; 
  }
  
  return ST_LOGGING;
}

byte setRtcTime(byte input) {
  Display.clearDisplay();
  Display.displayText_f(0,0,1,PSTR("Set RTC Time"));
  
  static EditTime edTime;
  
  char key = 0;
  
  if (Events.bBtn1)
    key = KEY_NEXT;
  else if (Events.bBtn2)
    key = KEY_PLUS;
  
  if (!edTime.editTime(key)) {
    return ST_DATE_TIME_MENU;
  }

  Display.display();
    
  return StateMachine.state;
}

byte setRtcDate(byte input) {
  Display.clearDisplay();
  Display.displayText_f(0,0,1,PSTR("Set RTC Date"));
  
  static EditDate edDate;
  
  char key = 0;
  
  if (Events.bBtn1)
    key = KEY_NEXT;
  else if (Events.bBtn2)
    key = KEY_PLUS;
  
  if (!edDate.editDate(key)) {
    return ST_DATE_TIME_MENU;
  }

  Display.display();
    
  return StateMachine.state;
}


void logError(String s) {
  //displayText(0, 0, 1, s);
}

    
