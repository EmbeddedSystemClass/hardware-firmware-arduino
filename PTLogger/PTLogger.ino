/*
	SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 This example code is in the public domain.
 	 
 */

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <avr/pgmspace.h>
#include "main.h"
#include "display.h"
#include "events.h"
#include "measure.h"
#include "data.h"
#include "menu.h"
#include "statemanager.h"
#include "edit.h"

// SD Card ****************************************************************
// pin 11 - MOSI
// pin 12 - MISO
// pin 13 - CLK
// pin 10 - CS
const int sdChipSelect = 10;

void setup()
{  
  Wire.begin();  

  lcd.begin(16, 2);


  // Open serial communications and wait for port to open:
  Serial.begin(9600);


  if (! rtc.isrunning()) {
    lcd.print_f(1, 1, PSTR("RTC is NOT running!"));
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  /*
  // see if the card is present and can be initialized:
  if (!SD.begin(sdChipSelect)) {
    Serial.println(F("Card failed, or not present"));
    // don't do anything more:
    return;
  } else {
    Serial.println(F("card initialized."));
  }
*/
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

  char buffer[9]= { 
    "00:00:00"   };  
  if (Events.bT1000MS) {
    DateTime dt = rtc.now();
    itochars(dt.hour(), &buffer[0], 2);
    itochars(dt.minute(), &buffer[3], 2);
    itochars(dt.second(), &buffer[6], 2);  

    lcd.print(0, 1, buffer);
  }

  if (input == KEY_ENTER) {
    enter = false;
    return ST_MAIN_MENU;
  }  
  return ST_MAIN;
}

byte setLogState(byte input) {
  static byte enter = 0;
  static EditYesNoOption edOption;

  if(!enter) {
    edOption.selected = LogEvents.bEnabled ? 1 : 0;
    edOption.bInvalidate = true;
    lcd.clear();
    lcd.print_f(0, 0, PSTR("Enable Logging"));
    enter = true;
  }

  if (!edOption.editOption(input)) {
    LogEvents.bEnabled = edOption.selected == 1;
    enter = false;
    return ST_MAIN;
  }
  return ST_LOG_STATE;
}

byte setLogInterval(byte input) {
  static byte enter = 0;
  static EditNumber edNumber;

  if(!enter) {
    itochars(LogEvents.interval, edNumber.buffer, edNumber.BUFFER_SIZE);
    edNumber.bInvalidate = true;
    lcd.clear();
    lcd.print_f(0, 0, PSTR("Logging Interval"));
    enter = true;
  }

  if (!edNumber.editNumber(input)) {
    LogEvents.interval = atoi(edNumber.buffer);
    enter = false;
    return ST_MAIN;
  }
  return ST_LOG_INTERVAL;
}

byte setRtcTime(byte input) {
  static EditTime edTime;
  static byte enter = 0;

  if(!enter) {
    edTime.bInvalidate = true;
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
  static EditDate edDate;
  static byte enter = 0;

  if(!enter) {
    edDate.bInvalidate = true;
    lcd.clear();
    lcd.print_f(0, 0, PSTR("Set Date"));
    enter = true;
  }

  if (!edDate.editDate(input)) {
    enter = false;
    return ST_MAIN;
  }
  return ST_DATE;
}

byte showMenu(byte input) {

  static byte state; 
  static byte invalidate = true;
  static byte selected = 1;

  if (invalidate) { 
    byte group = 0;	
    byte count = 0;
    byte n = 0;
    byte error = 0;
    PGM_P menuText;

    invalidate = false;

    lcd.clear();

    for (byte i = 0; (group = pgm_read_byte(&menu_state[i].group)); i++) {
      if (group == StateMachine.stateGroup) {
        if (n == 0) n = i;
        count++;
        Serial.println(String(count));
      }
    }	

    if (count < 2) {
      error = 1;
    } 
    else {
      if (selected >= count) selected = 1;						
      menuText = (PGM_P)pgm_read_word(&menu_state[n].pText);
      if (menuText != NULL) {         
        lcd.print_f(0, 0, menuText);   // draw menu title
        state = pgm_read_byte(&menu_state[n + selected].state);
        menuText = (PGM_P)pgm_read_word(&menu_state[n + selected].pText);
        byte textLength = strlen_P(menuText);
        if (textLength > 0) {						
          lcd.print_f((LCD_SIZE - textLength) / 2, 1, menuText);   // draw menu item
          if (count > 1) {						
            lcd.print_f(0, 1, PSTR("<"));
            lcd.print_f(LCD_SIZE - 1, 1, PSTR(">"));					
          }
        } 
        else {
          error = 2;
        }
      } 
      else {
        error = 3;
      }			
    }	

    if (error > 0) {
      lcd.print_f(0, 0, PSTR("Menu Error"));
      lcd.println(String(error));
    }
  }

  if (input == KEY_PLUS) {
    selected++;
    invalidate = true;
  } 
  else if (input == KEY_MINUS) {
    selected--;
    invalidate = true;
  } 
  else if (input == KEY_ENTER) {
    invalidate = true;
    selected = 1;
    StateMachine.stateGroup = state;
    return StateMachine.stateGroup;
  }

  return StateMachine.state;
}


void logError(String s) {
  //displayText(0, 0, 1, s);
}

void itochars(unsigned int value, char buffer[], byte digits) {
  byte i = 0; 
  byte d;
  unsigned int k;

  unsigned int P[] = { 
    1, 10, 100, 1000, 10000   };

  k = P[digits-1]; 

  while(i < digits) {
    d = value / k;
    value -= (d * k);
    buffer[i] = d + '0';
    k /= 10;
    i++;
  }
}

String getTimeStr() {
  String s;
  //s += formatNumber(rtc.now().Hours(), 2) + F(":");
  //s += formatNumber(rtc.now().Minutes(), 2) + F(":");
  //s += formatNumber(rtc.now().Seconds(), 2);
  return s;
}

String getDateStr() {
  String s;
  //s += formatNumber(rtc.now().Year(), 4) + F("-");
  //s += formatNumber(rtc.now().Month(), 2) + F("-");
  //s += formatNumber(rtc.now().Day(), 2);
  return s;
}


