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


// SD Card ****************************************************************
 /* SD card attached to SPI bus as follows:
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
 ** CS - pin 4
 */
 
const int sdChipSelect = 4;


// SHTxx  Humidity and Temperature Measurement ***************************
const byte shtDataPin =  2;                 // SHTxx serial data
const byte shtSclkPin =  3;                 // SHTxx serial clock
const unsigned long TRHSTEP   = 5000UL;  // Sensor query period

Sensirion sht = Sensirion(shtDataPin, shtSclkPin);

unsigned int shtRawData;
unsigned long trhMillis = 0;             // Time interval tracking
float temperature;
float humidity;
float dewpoint;

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


void setup()
{
  byte stat;
  
  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
	
	
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(sdChipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  if (shtError = sht.readSR(&stat))         // Read sensor status register
    logError(shtError);
  if (shtError = sht.writeSR(LOW_RES))      // Set sensor to low resolution
    logError(shtError);
  if (shtError = sht.readSR(&stat))         // Read sensor status register again
    logError(shtError);
}

void loop() {
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
      shtMeasActive = false;
      humidity = sht.calcHumi(shtRawData, temperature); // Convert raw sensor data
      dewpoint = sht.calcDewpoint(humidity, temperature);
      logData();
      displayData();
    }
  }
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
    Serial.println("error opening datalog.txt");
  } 
}

void displayData()
{
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
}

// The following code is only used with shtError checking enabled
void logError(byte shtError) {
  String errorString = "";
  switch (shtError) {
  case S_Err_NoACK:
    errorString = "Error: No response (ACK) received from sensor!";
    break;
  case S_Err_CRC:
    errorString = "Error: CRC mismatch!";
    break;
  case S_Err_TO:
    errorString = "Error: Measurement timeout!";
    break;
  default:
    errorString = "Unknown shtError received!";
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
    Serial.println("error opening errorString.txt");
  } 
}







