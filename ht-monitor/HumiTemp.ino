/*
  Humidity and Temperature monitor
 

 This sketch prints Humidity and Temperature to the LCD
 and shows the time.
 
  The circuit:
 * LCD connections
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 * Dht11 connections
 * +5V and ground
 * OneWire bus to digital pin 10
 

 This example code is in the public domain.

*/

// include the library code:
#include <LiquidCrystal.h>
#include "Dht11.h"

enum {
    // The data I/O pin connected to the DHT11 sensor
    DHT_DATA_PIN = 10,

    // The baud rate of the serial interface
    SERIAL_BAUD  = 9600,

    // The delay between sensor polls.
    POLL_DELAY   = 2000,
};


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.print("Dht11 Lib version ");
  Serial.println(Dht11::VERSION);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("*** H&T Monitor ***");
  lcd.setCursor(0, 1);
  lcd.print("Temperature:");
  lcd.setCursor(0, 2);
  lcd.print("Humidity   :");
}

void loop() {  
  static Dht11 sensor(DHT_DATA_PIN);

  switch (sensor.read()) {
  case Dht11::OK:
      lcd.setCursor(12, 1);
      lcd.print(sensor.getTemperature());
      lcd.print(" C");
      
      lcd.setCursor(12, 2);
      lcd.print(sensor.getHumidity());
      lcd.print(" %");

      lcdClearLine(3);
      
      Serial.print("Temperature (C): ");
      Serial.println(sensor.getTemperature());
      
      Serial.print("Humidity (%): ");
      Serial.println(sensor.getHumidity());
      break;

  case Dht11::ERROR_CHECKSUM:
      lcd.setCursor(0, 3);
      lcd.print("Checksum error");

      Serial.println("Checksum error");
      break;

  case Dht11::ERROR_TIMEOUT:
      lcd.setCursor(0, 3);
      lcd.print("Timeout error");

      Serial.println("Timeout error");
      break;

  default:
      lcd.setCursor(0, 3);
      lcd.print("Unknown error");

      Serial.println("Unknown error");
      break;
  }
  
  delay(POLL_DELAY);
}

void lcdClearLine(byte line) {
  lcd.setCursor(0, line);
  lcd.print("                ");
}

