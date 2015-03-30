/*
  SD G-Force datalogger
 
   The circuit:
   * ADXL345 Accelerometer sensor via I2C
   
   * SD card attached to SPI bus as follows:
   ** MOSI - pin 11
   ** MISO - pin 12
   ** CLK - pin 13
   ** CS - pin 10
   
   This example code is in the public domain.

 */
 
#include <SPI.h>
#include <SdFat.h>
//#include <Fat16util.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

SdFat card;
SdFile f;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

const int chipSelect = 10;  // SD-Card chip select
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
const long debounceDelay = 50;    // the debounce time; increase if the output flickers

int ledState = HIGH;        // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
int logEnabled = 0;         // log state
long lastDebounceTime = 0;  // the last time the output pin was toggled
long lastLogTime = 0;       // the last time data logged

//void enableSD(void){
//  DDRB = DDRB & 0xDF; //pinMode(CS_LCD,INPUT);
//  DDRB = DDRB | 0x10; //pinMode(CS_SD,OUTPUT);
//}

void handleButtonState(void)
{
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
        logEnabled = !logEnabled;
      }
    }
  }
  
  digitalWrite(ledPin, ledState);

  lastButtonState = reading;
}

void handleFileState(void)
{
  if(logEnabled) {
    if(!f.isOpen()) {
      char buffer[11] = { 0 };
      strcpy(buffer, "GLOG00.CSV");
      for (int i = 0; i < 100; i++) {
        buffer[4] = '0' + i/10;
        buffer[5] = '0' + i%10;
        // create if does not exist, do not open existing, write
        if (f.open(buffer, O_CREAT | O_EXCL | O_WRITE)) break;
      }
      
      if(!f.isOpen()) {
        logEnabled = false;
        Serial.println(F("couldnt create ")); Serial.println(buffer);
      }
      Serial.println(F("writing to ")); Serial.println(buffer);
    }
  } else if(f.isOpen()) {
    f.close();
    Serial.println(F("file closed."));
  }
}

void setup()
{
  // Initialize button and led
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Initialize button state
  buttonState = digitalRead(buttonPin);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }  
  Serial.println();

  // Initialize SD card
  Serial.print(F("Initializing SD card..."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  if(!card.init()) {
    Serial.println(F("Card init. failed!"));
    return;
  }
  if(!Fat16::init(&card)) {
    Serial.println(F("No partition!"));
    return;
  }
  Serial.println(F("card initialized."));

  Serial.print(F("Initializing sensor ADXL345..."));
  // Initialize the sensor
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("No ADXL345 detected ... Check your wiring!"));
    return;
  }   
  // Set the range
  accel.setRange(ADXL345_RANGE_16_G);
  // Set data rate
  accel.setDataRate(ADXL345_DATARATE_800_HZ);
  Serial.println(F("sensor ADXL345 initialized."));
  
  Serial.println("");  
}

void loop()
{ 
  handleButtonState();
  handleFileState();
  
  if(logEnabled) {
    uint32_t currentMillis = millis();
    
    // log data interval
    if((currentMillis - lastLogTime) >= 20) {
      lastLogTime = currentMillis;
      sensors_event_t event; 
      accel.getEvent(&event);
      
      f.writeError = false;
      f.print(currentMillis);
      f.print(";");
      f.print(event.acceleration.x, 3);
      f.print(";");
      f.print(event.acceleration.y, 3);
      f.print(";");
      f.print(event.acceleration.z, 3);
      f.println();

      if (f.writeError || !f.sync()) {
        Serial.println(F("can't write data!"));
      }
    }
  } 
}

