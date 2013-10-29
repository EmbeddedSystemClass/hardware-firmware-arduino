/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 This example code is in the public domain.
 	 
 */
#include <SPI.h>
#include <SD.h>
#include <Sensirion.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>
#include <swRTC.h>


// Ops ******************************************************************
#define alignDateTimeValue(a, b) (a > b ? 0 :a)  // time and date 
#define alignValue(a, b) (a > b ? 0 : a)


// Statemachine *********************************************************

byte state;
byte stateGroup;

typedef struct PROGMEM
{
    byte state;
    byte input;
    byte nextstate;
} MENU_NEXTSTATE;

typedef struct PROGMEM
{
    byte group;
    byte state;
    PGM_P pText;
    byte (*pFunc)(byte input);
} MENU_STATE;

#define KEY_PLUS 1

#define ST_MAIN          1
#define ST_MAIN_MENU     2
#define ST_DATE_TIME     3
#define ST_LOGGING       4
#define ST_TEMP_CHART    5
#define ST_HUMI_CHART    6

#define ST_EDIT_HOURS    9
#define ST_EDIT_MINUTES 10
#define ST_EDIT_SECONDS 11
#define ST_EDIT_YEAR    12
#define ST_EDIT_MONTH   13
#define ST_EDIT_DAY     14

#define ST_OK	       251
#define ST_CANCEL      252
#define ST_YES         253
#define ST_NO          254
#define ST_EXIT        255

const char MT_MAIN[] PROGMEM          = "Menu Test";
const char MT_DATE_TIME[] PROGMEM     = "Date Time";
const char MT_LOG[] PROGMEM           = "Logging";
const char MT_TEMPERATURE[] PROGMEM   = "Temperature";
const char MT_HUMIDITY[] PROGMEM      = "Humidity";
const char MT_EXIT[] PROGMEM          = "Exit";

const MENU_NEXTSTATE menu_nextstate[] PROGMEM = {
//  STATE                       INPUT       NEXT STATE
    {ST_MAIN,                   KEY_PLUS,   ST_MAIN_MENU},
    
// Main Menu    
    {ST_MAIN_MENU,              KEY_PLUS,   ST_DATE_TIME},
    {ST_DATE_TIME,              KEY_PLUS,   ST_LOGGING},
    {ST_LOGGING,                KEY_PLUS,   ST_TEMP_CHART},
    {ST_TEMP_CHART,             KEY_PLUS,   ST_HUMI_CHART},
    {ST_HUMI_CHART,             KEY_PLUS,   ST_EXIT},
    {ST_EXIT,                   KEY_PLUS,   ST_DATE_TIME},
    
// Yes, No Dialog
    {ST_YES,                    KEY_PLUS,   ST_NO},
    {ST_NO,                     KEY_PLUS,   ST_YES},

// Date Time Edit
    {ST_EDIT_HOURS,             KEY_PLUS,   ST_EDIT_MINUTES},
    {ST_EDIT_MINUTES,           KEY_PLUS,   ST_EDIT_SECONDS},
    {ST_EDIT_SECONDS,           KEY_PLUS,   ST_EDIT_YEAR},
    {ST_EDIT_YEAR,              KEY_PLUS,   ST_EDIT_MONTH},
    {ST_EDIT_MONTH,             KEY_PLUS,   ST_EDIT_DAY},
    {ST_EDIT_DAY,               KEY_PLUS,   ST_OK},
    {ST_OK,                     KEY_PLUS,   ST_CANCEL},
    {ST_CANCEL,                 KEY_PLUS,   ST_EDIT_HOURS},
    
    {0,                         0,          0}
};

const MENU_STATE menu_state[] PROGMEM = {
//  STATE GROUP                         STATE                       STATE TEXT                  STATE_FUNC
    {ST_MAIN,                           ST_MAIN,                    NULL,                       mainScreen},
    {ST_MAIN_MENU,                      ST_MAIN_MENU,               NULL,                       showMenu},
    {ST_MAIN_MENU,                      ST_DATE_TIME,               MT_DATE_TIME,               setRTC},
    {ST_MAIN_MENU,                      ST_LOGGING,                 MT_LOG,                     setLogging},
    {ST_MAIN_MENU,                      ST_TEMP_CHART,              MT_TEMPERATURE,             NULL},
    {ST_MAIN_MENU,                      ST_HUMI_CHART,              MT_HUMIDITY,                NULL},
    {ST_MAIN_MENU,                      ST_EXIT,                    MT_EXIT,                    mainScreen},
    {0,                                 NULL,                       NULL,                       NULL}
};

// Buttons ***************************************************************
const byte btn1Pin = A1;     // pushbutton 1 pin
const byte btn2Pin = A2;     // pushbutton 2 pin

#define BTN1 0
#define BTN2 1

struct {
  unsigned bBtn1:1;
  unsigned bBtn2:1;
  byte lastButtonState;
} btnState;


// Display Nokia 5110 ****************************************************

#define TEXTHEIGHT 8
#define TEXTWIDTH  6

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Software RTC **********************************************************
swRTC rtc;

// Events ****************************************************************

struct {  
  unsigned bT50MS:1;
  unsigned bT500MS:1;
  byte counter;
  unsigned long lastUpdateTime;
} eventState;

// Program ***************************************************************

void setup()
{  
  // initialize the pushbutton pin as an input:
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);

  // Initialize Display
  display.begin();
  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(60);
  display.clearDisplay();   // clears the screen and buffer


  rtc.stopRTC(); //stop the RTC
  rtc.setTime(0, 0, 0); //set the time here
  rtc.setDate(1, 1, 2000); //set the date here
  rtc.startRTC(); //start the RTC

    // Open serial communications and wait for port to open:
//  Serial.begin(9600);
//  while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  
}

void loop() {
  byte nextstate;

  byte (*pStateFunc)(byte);
  byte input;
  byte i, j;
  
  state = ST_MAIN;
  nextstate = ST_MAIN;
  pStateFunc = mainScreen;
  
  for(;;) {
    updateEvents();
    updateButtonFlags();

    if (pStateFunc) {
      // When in this state, we must call the state function
      nextstate = pStateFunc(0);
    } else {
      nextstate = ST_MAIN;
    }
    
    if (nextstate != state) {
      state = nextstate;
      for (i=0; (j=pgm_read_byte(&menu_state[i].state)); i++) {
        if (j == state) {
          stateGroup = pgm_read_byte(&menu_state[i].group);
          pStateFunc = (byte (*)(byte))(PGM_VOID_P) pgm_read_word(&menu_state[i].pFunc);
          break;
        }
      }
    }    
  }
  
}

byte showMenu(byte input) {
  byte i, j;
  byte y = 0;
  byte stateTemp = 0;
  PGM_P statetext;
  
  static byte menuState;
  
  display.clearDisplay();
  
  for (i=0; (j=pgm_read_byte(&menu_state[i].group)); i++) {
    if (j == stateGroup) {
      stateTemp = pgm_read_byte(&menu_state[i].state);
      statetext = (PGM_P)pgm_read_word(&menu_state[i].pText);
      if (statetext != NULL) {
        if (menuState == 0) {
          menuState = stateTemp;
        }
//        byte textColor = menuState == stateTemp ? WHITE : BLACK;
//        byte backColor = menuState == stateTemp ? BLACK : WHITE;
        displayText_f(2, y * (TEXTHEIGHT + 1) + 1, 1, statetext);
        if (menuState == stateTemp) {
          display.drawRect(0, y * (TEXTHEIGHT + 1), LCDWIDTH, 10, BLACK);
        }
        y++;
      }
    }
  }
  
  display.display();
  
  if (btnState.bBtn2) {
    menuState=stateMachine(menuState, KEY_PLUS);
  } else if (btnState.bBtn1) {
    stateTemp = menuState;
    menuState = 0;
    return stateTemp;
  }
  
  return state;
}

unsigned char stateMachine(byte state, byte stimuli)
{
    byte nextstate = state;    // Default stay in same state
    byte i, j;
    for (i=0; ( j=pgm_read_byte(&menu_nextstate[i].state) ); i++ )
    {
        if ( j == state && 
             pgm_read_byte(&menu_nextstate[i].input) == stimuli)
        {
            nextstate = pgm_read_byte(&menu_nextstate[i].nextstate);
            break;
        }
    }
    return nextstate;
}

byte mainScreen(byte inp) {
  display.clearDisplay();
  displayText_f(0, 0, 1, PSTR("Menu Test"));
  display.display();
  
  if (btnState.bBtn1) {
    return ST_MAIN_MENU;
  }
  
  return ST_MAIN;
}

byte setLogging(byte input) {
  static byte logState;
  
  if (logState == 0) {
    logState = ST_YES;
  }
  
  display.clearDisplay();
  displayText_f(0,  0, 1, PSTR("Reset Log"));
  displayText_f(2, 20, 1, PSTR("YES    NO"));
  
  switch(logState) {
    case ST_YES:
      display.drawRect(0, 18, 3 * TEXTWIDTH + 3, TEXTHEIGHT + 3, BLACK);
      break;
    case ST_NO:
      display.drawRect(6 * (TEXTWIDTH + 1), 18, 2 * TEXTWIDTH + 3, TEXTHEIGHT + 3, BLACK);
      break;
  }
  
  display.display();
  
  if (btnState.bBtn2) {
    logState = stateMachine(logState, KEY_PLUS);
  } else if (btnState.bBtn1) {
    return ST_MAIN; 
  }
  
  return ST_LOGGING;
}

byte setRTC(byte input) {
  display.clearDisplay();
  displayText_f(0,0,1,PSTR("Set RTC"));
  display.display();
  
  if (btnState.bBtn1) {
    return ST_MAIN;
  }
  
  return state;
}

void displayText_f(byte x, byte y, byte fontSize, const char *pFlashStr) {
  displayText_f(x, y, fontSize, BLACK, WHITE, pFlashStr);
}

void displayText_f(byte x, byte y, byte fontSize, byte textColor, byte backColor, const char *pFlashStr) {
  display.setTextColor(textColor, backColor);
  display.setTextSize(fontSize);
  
  for (byte i = 0; (const char)(pgm_read_byte(&pFlashStr[i])) && i < 40; i++) {
    display.setCursor(x + i * 6, y);
    char c = pgm_read_byte(&pFlashStr[i]);
    if (c == '\0') break;
    display.write(pgm_read_byte(&pFlashStr[i]));
  }
}

//void displayText(byte x, byte y, byte fontSize, String str)
//{
//  displayText(x, y, fontSize, str, BLACK, WHITE);
//}
//
//void displayText(byte x, byte y, byte fontSize, String str, byte textColor, byte backColor)
//{
//  display.setTextColor(textColor, backColor);
//  display.setTextSize(fontSize);
//  display.setCursor(x, y);
//  display.println(str);	
//}

void updateEvents() {
  // generate 50ms, 500ms timer events
  if (millis() - eventState.lastUpdateTime > 50) {
    eventState.lastUpdateTime = millis();
    eventState.counter++;
    eventState.bT50MS = true;
    if (eventState.counter % 10 == 0) {
	eventState.bT500MS = true;
    }
  } else {
    eventState.bT50MS = false;
    eventState.bT500MS = false;
  }
}

void updateButtonFlags() {
  // read the state of the pushbuttons
  byte buttonState = ~PINC & 3;

  if(eventState.bT50MS && btnState.lastButtonState != buttonState) {    
    btnState.lastButtonState = buttonState;    
    btnState.bBtn1 = bitRead(btnState.lastButtonState, BTN1);
    btnState.bBtn2 = bitRead(btnState.lastButtonState, BTN2);    
  } 
  else {
    btnState.bBtn1 = false;
    btnState.bBtn2 = false;   
  }  
}

void logError(String s) {
  //displayText(0, 0, 1, s);
}

String formatNumber(int n, byte count) {
  String s = String(n);
  if (s.length() >= count)
    return s;
  while(s.length() != count)
    s = String(F("0")) + s;
  return s;
}

String getTimeStr() {
  String s;
  s += formatNumber(rtc.getHours(), 2) + F(":");
  s += formatNumber(rtc.getMinutes(), 2) + F(":");
  s += formatNumber(rtc.getSeconds(), 2);
  return s;
}

String getDateStr() {
  String s;
  s += formatNumber(rtc.getYear(), 4) + F("-");
  s += formatNumber(rtc.getMonth(), 2) + F("-");
  s += formatNumber(rtc.getDay(), 2);
  return s;
}
    
