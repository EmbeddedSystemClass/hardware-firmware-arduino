/*
  Display Demo 
 	
 This example code is in the public domain.
 	 
 */

#include <Adafruit_GFX.h>    // Core graphics library
#include <SWTFT.h> // Hardware-specific library
#include <TouchScreen.h>
#include <SPI.h>
#include <avr/pgmspace.h>


#include "display.h"
#include "event.h"
#include "button.h"

void setup()
{
  // Open serial communications
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println(F("Display Demo\r\n"));
  
  // Initialize Display
  Display.beginDisplay();
  
  Display.displayText_f(0, 80, 2, WHITE, BLACK, PSTR("Test 2"));
  Display.displayText_f(0, 100, 3, GREEN, BLACK, PSTR("Test 3"));
  Display.drawLine(10, 150, 230, 150, WHITE);
}

byte mode = false;
byte counter = 0;
  
void loop() {
  
  Events.dispatch();  
  
  if(mode)
    Button::drawButton(0, 2, 240, 60, PSTR("Test On "), NULL);
  else
    Button::drawButton(0, 2, 240, 60, PSTR("Test Off"), NULL);
    
  if(Button::hitTest(0, 2, 240, 60))
    mode = !mode;
    
  if(Events.bT1000MS) {
    counter++;  
    char buffer[4] = { 0 };
    bin2asc(counter, buffer, 3);
    Display.displayText(2, 200, 3, buffer, WHITE, BLACK);
  }
}


void bin2asc(unsigned int value, char buffer[], uint8_t digits) {
  uint8_t i = 0; 
  uint8_t d;
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
 

