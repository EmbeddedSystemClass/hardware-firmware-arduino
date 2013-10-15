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
const unsigned long TRHSTEP   = 5000UL;  	// Sensor query period

unsigned int shtRawData;
unsigned long trhMillis = 0;             	// Time interval tracking
float temperature;
float humidity;

byte shtMeasActive = false;
byte sthMeasType = TEMP;

// This version of the code checks return codes for errors
byte shtError = 0;

// Display Nokia 5110 ****************************************************

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// States ****************************************************************
#define MEASURE_STATE   1;
#define RESET_LOG_STATE 2;
#define SET_TIME_STATE  3;
byte state = MEASURE_STATE;

void setup()
{  
  // Initialize Display
  display.begin();
  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(60);
  display.clearDisplay();   // clears the screen and buffer

	
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
    logError(shtError);
  if (shtError = sht.writeSR(LOW_RES))      // Set sensor to low resolution
    logError(shtError);
  if (shtError = sht.readSR(&stat))         // Read sensor status register again
    logError(shtError);
}

void loop() {
	byte input = getInput();
	
	switch(state) {
		case 1:
			state = measure(input);
			break;
		case 2:
			state = resetLog(input);
			break;
		case 3:
			state = setTime(input);
			break;
	}	
}


byte measure(byte input) {
  unsigned long curMillis = millis();          // Get current time

  
  // Demonstrate non-blocking calls
  if (curMillis - trhMillis >= TRHSTEP) {      // Time for new measurements?
    shtMeasActive = true;
    sthMeasType = TEMP;
    if (shtError = sht.meas(TEMP, &shtRawData, NONBLOCK)) // Start temp measurement
      logError(shtError);
    trhMillis = curMillis;
  }
  if (shtMeasActive && (shtError = sht.measRdy())) { // Check measurement status
    if (shtError != S_Meas_Rdy)
      logError(shtError);
    if (sthMeasType == TEMP) {                    // Process temp or humi?
      sthMeasType = HUMI;
      temperature = sht.calcTemp(shtRawData);     // Convert raw sensor data
      if (shtError = sht.meas(HUMI, &shtRawData, NONBLOCK)) // Start humi measurement
        logError(shtError);
    } else {
      //Serial.println("Measure");
      shtMeasActive = false;
      humidity = sht.calcHumi(shtRawData, temperature); // Convert raw sensor data
      logData();
      displayData();
    }
  }
  return MEASURE_STATE;
}

byte resetLog(byte input) {
	return RESET_LOG_STATE;
}

byte setTime(byte input) {
	return SET_TIME_STATE;
}

void logData()
{
    
  // make a string for assembling the data to log:
  String dataString = "";

  dataString += String(temperature) + ";";
  dataString += String(humidity) + ";";

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
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Temperature"));
  display.setTextSize(2);
  display.setCursor(0, 10);        
  display.print(temperature); display.print((char)255); display.println(F("C"));
  
  display.setCursor(0, 25);
  display.setTextSize(1);
  display.println(F("Humidity"));
  display.setTextSize(2);
  display.setCursor(0, 34);
  display.print(humidity); display.println(F("%"));
  display.display();  
}

// The following code is only used with shtError checking enabled
void logError(byte shtError) {
  String errorString = "";
  switch (shtError) {
  case S_Err_NoACK:
    errorString = F("Error: No response (ACK) received from sensor!");
    break;
  case S_Err_CRC:
    errorString = F("Error: CRC mismatch!");
    break;
  case S_Err_TO:
    errorString = F("Error: Measurement timeout!");
    break;
  default:
    errorString = F("Unknown shtError received!");
    break;
  }
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("error.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(errorString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(errorString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println(F("error opening errorString.txt"));
  } 
}

byte stateMachine(byte state, byte stimuli) {
}

byte getInput() {
}




