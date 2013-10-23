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

// Ops ****************************************************************
#define alignDateTimeValue(a,b) ((a)>(b)?(0):(a))  // time and date 

// Buttons ****************************************************************
const byte btn1Pin = A1;     // pushbutton 1 pin
const byte btn2Pin = A2;     // pushbutton 2 pin

#define BTN1 0
#define BTN2 1

struct {
  unsigned bBtn1:1;
  unsigned bBtn2:1;
  byte lastButtonState;
} btnState;

// SD Card ****************************************************************
// pin 11 - MOSI
// pin 12 - MISO
// pin 13 - CLK
// pin 10 - CS
const int sdChipSelect = 10;

#define LOG_EDIT_NONE   0
#define LOG_EDIT_ENTER  1
#define LOG_DELETE_YES  2
#define LOG_DELETE_NO   3

byte logState;

// SHTxx  Humidity and Temperature Measurement ***************************
// pin 8  - Data Pin
// pin 9  - Serial Clock
Sensirion sht = Sensirion(8, 9);

// This version of the code checks return codes for errors
byte shtError = 0;

struct {
  unsigned bMeasure:1;
  unsigned bTempMeasure:1; 
  unsigned bHumiMeasure:1;
  unsigned bLog:1;
  unsigned bDisplay:1; 
  byte temperature;
  byte humidity;
  unsigned int rawData;
} shtState;


// Display Nokia 5110 ****************************************************

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Software RTC **********************************************************
swRTC rtc;

#define RTC_EDIT_NONE    0
#define RTC_EDIT_HOURS   1
#define RTC_EDIT_MINUTES 2
#define RTC_EDIT_SECONDS 3
#define RTC_EDIT_YEAR    4
#define RTC_EDIT_MONTH   5
#define RTC_EDIT_DAY     6

struct {  
  byte EditState;	
} rtcState;

// States ****************************************************************
#define MEASURE_STATE   1;
#define SET_TIME_STATE  2;
#define RESET_LOG_STATE 3;
byte state = MEASURE_STATE;

struct {  
  unsigned bT50MS:1;
  unsigned bT500MS:1;
  byte counter;
  unsigned long lastUpdateTime;
} eventState;

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
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print(F("Initializing SD card..."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  //pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(sdChipSelect)) {
    Serial.println(F("Card failed, or not present"));
    // don't do anything more:
    return;
  }
  Serial.println(F("card initialized."));
  
  byte stat;  
  if (shtError = sht.readSR(&stat))         // Read sensor status register
   logError(String(F("Read sensor status register:")) + shtError);
  if (shtError = sht.writeSR(LOW_RES))      // Set sensor to low resolution
   logError(String(F("Set sensor to low resolution:")) + shtError);
  if (shtError = sht.readSR(&stat))         // Read sensor status register again
   logError(String(F("Read sensor status register again:")) + shtError);
  
}

void loop() {
  updateEvents();
  updateButtonFlags();

  //Serial.println(state);	
  switch(state) {
  case 1:
    state = measure();
    break;
  case 2:
    state = setRtC();
    break;
  case 3:
    state = resetLog();
    break;
  }	
}

byte measure() {
  if (!shtState.bMeasure && millis() % 5000UL == 0) {      // Time for new measurements?
    shtState.bMeasure = true;
    shtState.bTempMeasure = true;
    sht.meas(TEMP, &shtState.rawData, NONBLOCK); // Start temp measurement
    //Serial.println("start temp measure");
  }
  if (shtState.bMeasure && (shtError = sht.measRdy())) { // Check measurement status
    if (shtState.bTempMeasure) {                    // Process temp or humi?
      shtState.bTempMeasure = false;
      shtState.bHumiMeasure = true;
      shtState.temperature = (byte)sht.calcTemp(shtState.rawData);     // Convert raw sensor data
      sht.meas(HUMI, &shtState.rawData, NONBLOCK); // Start humi measurement
      //Serial.println("start humi measure");
    } 
    else if (shtState.bHumiMeasure) 
    {
      shtState.bHumiMeasure = false;
      shtState.bMeasure = false;
      shtState.bLog = true;
      shtState.bDisplay = true;
      shtState.humidity = (byte)sht.calcHumi(shtState.rawData, shtState.temperature); // Convert raw sensor data
      //Serial.println("measure ready");
    }
  }
  
  if (shtState.bDisplay) {
    shtState.bDisplay = false;
    displayData();
    //Serial.println("display");
  }
  
  if (shtState.bLog) {
    shtState.bLog = false;
    logData();
    //Serial.println("log");
  }

  if (btnState.bBtn1) {
    shtState.bMeasure = false;
    shtState.bDisplay = true;  // if MEASURE_STATE reactivated, display last values
    return SET_TIME_STATE;
  }

  return MEASURE_STATE;
}

byte resetLog() {
  
  if (logState == LOG_EDIT_NONE) {
    logState = LOG_EDIT_ENTER;
  }
  
  switch(logState) {
    case LOG_EDIT_ENTER:
      display.clearDisplay(); 
      displayText(0,  0, 1, F("Reset Log"));      
      display.display();
      logState = LOG_DELETE_YES;
      break;
    case LOG_DELETE_YES:
      displayText(0, 22, 1, F("___      "));
      displayText(0, 20, 1, F("YES    NO"));
      display.display();
      if (btnState.bBtn2) {
        logState = LOG_DELETE_NO;
      }
      break;
    case LOG_DELETE_NO:
      displayText(0, 22, 1, F("       __"));
      displayText(0, 20, 1, F("YES    NO"));
      display.display();
      if (btnState.bBtn2) {
        logState = LOG_DELETE_YES;
      }
      break;
  }
  
  if (btnState.bBtn1) {
    logState = LOG_EDIT_NONE;
    return MEASURE_STATE;
  }
  return RESET_LOG_STATE;
}

byte setRtC() {
  
  if (rtcState.EditState == RTC_EDIT_NONE) {    
    rtcState.EditState = RTC_EDIT_HOURS;				// start with hours edit
  }
  
  display.clearDisplay(); 
  displayText(0,  0, 1, F("Set RTC"));
  
  byte i = btnState.bBtn2 ? 1 : 0;	// if Btn2 pressed, increment time
					//   else do not increment time
  unsigned long n = 0;
  switch(rtcState.EditState) {

    // Time ****************
	  
    case RTC_EDIT_HOURS:
      displayText(0, 12, 1, F("__"));			  // cursor
      n = alignDateTimeValue(rtc.getHours() + i, 23);     // increment
      rtc.setTime(n, rtc.getMinutes(), rtc.getSeconds()); // set time
      if (btnState.bBtn1) { 				  // if Btn1 pressed
	rtcState.EditState = RTC_EDIT_MINUTES;   	  //   edit minutes
      }
      break;
    case RTC_EDIT_MINUTES:
      displayText(18, 12, 1, F("__")); 
      n = alignDateTimeValue(rtc.getMinutes() + i, 59);      
      rtc.setTime(rtc.getHours(), n, rtc.getSeconds());
      if (btnState.bBtn1) {
	rtcState.EditState = RTC_EDIT_SECONDS;
      }
      break;
    case RTC_EDIT_SECONDS:
      n = alignDateTimeValue(rtc.getSeconds() + i, 59);      
      rtc.setTime(rtc.getHours(), rtc.getMinutes(), n);	  
      displayText(36, 12, 1, F("__"));
      if (btnState.bBtn1) {
        rtcState.EditState = RTC_EDIT_YEAR;
      }
      break;
 
    // Date ****************
    
    case RTC_EDIT_YEAR:
      displayText(0, 22, 1, F("____"));			  // cursor
      n = rtc.getYear() + i;      	                  // increment
      rtc.setDate(rtc.getDay(), rtc.getMonth(), n);       // set date
      if (btnState.bBtn1) { 				  // if Btn1 pressed
	rtcState.EditState = RTC_EDIT_MONTH;   		  //   edit month
      }
      break;
    case RTC_EDIT_MONTH:
      displayText(29, 22, 1, F("__")); 
      n = alignDateTimeValue(rtc.getMonth() + i, 12);      
      rtc.setDate(rtc.getDay(), n, rtc.getYear());
      if (btnState.bBtn1) {
	rtcState.EditState = RTC_EDIT_DAY;
      }
      break;
    case RTC_EDIT_DAY:
      n = alignDateTimeValue(rtc.getDay() + i, 31);      
      rtc.setDate(n, rtc.getMonth(), rtc.getYear());	  
      displayText(47, 22, 1, F("__"));
      if (btnState.bBtn1) {
        rtcState.EditState = RTC_EDIT_NONE;
	return RESET_LOG_STATE;
      }
      break;
  }  

  displayText(0, 10, 1, getTimeStr());
  displayText(0, 20, 1, getDateStr());
  display.display();
  
  return SET_TIME_STATE;
}

void logData()
{    
  // make a string for assembling the data to log:  
  String dataString;
  // 00:00:00,00.00,00.00
  dataString += getTimeStr() + F(";");
  dataString += String(shtState.temperature) + F(";");
  dataString += String(shtState.humidity) + F(";");

  switch (shtError) {
  case S_Meas_Rdy:
    // no error
    break;
  case S_Err_NoACK:
    dataString += F("Error: No response (ACK) received from sensor!");
    break;
  case S_Err_CRC:
    dataString += F("Error: CRC mismatch!");
    break;
  case S_Err_TO:
    dataString += F("Error: Measurement timeout!");
    break;
  default:
    dataString += F("Unknown shtError received!");
    break;
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }    
  // if the file isn't open, pop up an error:
  else {
    Serial.println(F("error opening datalog.txt"));
  }   

}

void displayData()
{
  display.clearDisplay();  

  displayText(0, 0 , 1, String(F("Temperature")));   
  displayText(0, 10, 2, String(shtState.temperature) + (char)255 + String(F("C")));  
  displayText(0, 25, 1, String(F("Humidity")));
  displayText(0, 34, 2, String(shtState.humidity) + String(F("%")));  

  display.display();  
}

void displayText(byte x, byte y, byte fontSize, String str)
{
  displayText(x, y, fontSize, str, BLACK, WHITE);
}

void displayText(byte x, byte y, byte fontSize, String str, byte textColor, byte backColor)
{
  display.setTextColor(textColor, backColor);
  display.setTextSize(fontSize);
  display.setCursor(x, y);
  display.println(str);	
}

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
  displayText(0, 0, 1, s);
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

