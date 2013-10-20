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

// Bit Ops ****************************************************************
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))

// Buttons ****************************************************************
const byte btn1Pin = A1;     // the number of the pushbutton pin
const byte btn2Pin = A2;     // the number of the pushbutton pin

byte buttonState = 0;         // variable for reading the pushbutton status
byte lastButtonState = 0;
byte buttonFlags = 0;
long lastDebounceTime = 0;

#define BTN1 0
#define BTN2 1

// SD Card ****************************************************************
// pin 11 - MOSI
// pin 12 - MISO
// pin 13 - CLK
// pin 10 - CS
const int sdChipSelect = 10;

// SHTxx  Humidity and Temperature Measurement ***************************
// pin 8  - Data Pin
// pin 9  - Serial Clock
Sensirion sht = Sensirion(8, 9);
unsigned int shtRawData;
byte temperature;
byte humidity;

// This version of the code checks return codes for errors
byte shtError = 0;

byte shtFlags = 0;
// Flags
#define SHT_TEMP    0
#define SHT_HUMI    1
#define SHT_MEASURE 2
#define SHT_LOG     3
#define SHT_DISPLAY 4

// Display Nokia 5110 ****************************************************

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Software RTC **********************************************************
swRTC rtc;

// States ****************************************************************
#define MEASURE_STATE   1;
#define SET_TIME_STATE  2;
#define RESET_LOG_STATE 3;
byte state = MEASURE_STATE;

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
  rtc.setTime(21,55,0); //set the time here
  rtc.setDate(16,10,2013); //set the date here
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
  updateButtonFlags();

  //Serial.println(state);	
  switch(state) {
  case 1:
    state = measure(buttonFlags);
    break;
  case 2:
    state = setRtC(buttonFlags);
    break;
  case 3:
    state = resetLog(buttonFlags);
    break;
  }	
}

byte measure(byte input) {
  if (!bitRead(shtFlags, SHT_MEASURE) && millis() % 5000UL == 0) {      // Time for new measurements?
    bitSet(shtFlags, SHT_MEASURE);
    bitSet(shtFlags, SHT_TEMP);
    sht.meas(TEMP, &shtRawData, NONBLOCK); // Start temp measurement
    // Serial.println("start temp measure");
  }
  if (bitRead(shtFlags, SHT_MEASURE) && (shtError = sht.measRdy())) { // Check measurement status
    if (bitRead(shtFlags, SHT_TEMP)) {                    // Process temp or humi?
      bitClear(shtFlags, SHT_TEMP);
      bitSet(shtFlags, SHT_HUMI);
      temperature = (byte)sht.calcTemp(shtRawData);     // Convert raw sensor data
      sht.meas(HUMI, &shtRawData, NONBLOCK); // Start humi measurement
      // Serial.println("start humi measure");
    } 
    else if (bitRead(shtFlags, SHT_HUMI)) 
    {
      bitClear(shtFlags, SHT_HUMI);
      bitClear(shtFlags, SHT_MEASURE);
      bitSet(shtFlags, SHT_LOG);
      bitSet(shtFlags, SHT_DISPLAY);
      humidity = (byte)sht.calcHumi(shtRawData, temperature); // Convert raw sensor data
      // Serial.println("measure ready");
    }
  }
  
  if (bitRead(shtFlags, SHT_DISPLAY)) {
    bitClear(shtFlags, SHT_DISPLAY);
    displayData();
    // Serial.println("display");
  }
  
  if (bitRead(shtFlags, SHT_LOG)) {
    bitClear(shtFlags, SHT_LOG);
    logData();
    // Serial.println("log");
  }

  if (bitRead(input, BTN1)) {
    bitClear(shtFlags, SHT_MEASURE);
    bitSet(shtFlags, SHT_DISPLAY);  // if MEASURE_STATE reactivated, display last values
    return SET_TIME_STATE;
  }

  return MEASURE_STATE;
}

byte resetLog(byte input) {
  display.clearDisplay(); 
  displayText(0, 0, 1, F("Reset Log"));
  display.display();

  if (bitRead(input, BTN1)) {
    return MEASURE_STATE;
  }
  return RESET_LOG_STATE;
}

byte setRtC(byte input) {
  display.clearDisplay(); 
  displayText(0, 0, 1, F("Set RTC"));
  display.display();

  if (bitRead(input, BTN1)) {
    return RESET_LOG_STATE;
  }
  return SET_TIME_STATE;
}

void logData()
{    
  // make a string for assembling the data to log:  
  String dataString;
  // 00:00:00,00.00,00.00
  dataString += formatNumber(rtc.getHours(), 2) + ":";

  dataString += formatNumber(rtc.getMinutes(), 2) + ":";
  dataString += formatNumber(rtc.getSeconds(), 2) + ";";
  dataString += String(temperature) + ";";
  dataString += String(humidity) + ";";

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

String formatNumber(byte n, byte count) {
  String s = String(n);
  while(s.length() != count)
    s = String(F("0")) + s;
  return s;
}

void displayData()
{
  display.clearDisplay();  

  displayText(0, 0 , 1, String(F("Temperature")));   
  displayText(0, 10, 2, String(temperature) + (char)255 + String(F("C")));  
  displayText(0, 25, 1, String(F("Humidity")));
  displayText(0, 34, 2, String(humidity) + String(F("%")));  

  display.display();  
}

void displayText(byte x, byte y, byte size, String str)
{
  display.setTextColor(BLACK);
  display.setTextSize(size);
  display.setCursor(x, y);
  display.println(str);	
}

void updateButtonFlags() {
  // read the state of the pushbutton value:
  buttonState = ~PINC & 3;

  if(millis() - lastDebounceTime > 75 && lastButtonState != buttonState) {
    lastDebounceTime = millis();
    lastButtonState = buttonState;

    if (bitRead(lastButtonState, BTN1)) {
      bitSet(buttonFlags, BTN1);
    }
    if (bitRead(lastButtonState, BTN2)) {
      bitSet(buttonFlags, BTN2);
    }
  } 
  else {
    bitClear(buttonFlags, BTN1);
    bitClear(buttonFlags, BTN2);      
  }  
}

void logError(String s) {
  displayText(0, 0, 1, s);
}

