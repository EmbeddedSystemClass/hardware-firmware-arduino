#ifndef _EVENTSH_
#define _EVENTSH_

/*
  Events:	timer and logging events

 This example code is in the public domain.
 */

// Touche Screen
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 6   // can be a digital pin
#define XP 7   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Events ****************************************************************

class EventManager {
  private:
    uint8_t lastButtonState;
    uint8_t counter;
    unsigned long lastTimerUpdate;
  
  public:
    unsigned bOnTouch:1;
    unsigned bLock:1;
    int16_t touchX;
    int16_t touchY;

    unsigned bT50MS:1;
    unsigned bT500MS:1;
    unsigned bT1000MS:1;
    unsigned bT5S:1;

    void dispatch() {
      updateTimerEvents();      
      updateTouchEvents();
    }

  private:    
    void updateTimerEvents() {
      bT50MS = false;
      bT500MS = false;
      bT1000MS = false;
      bT5S = false;
      
      // generate 50ms, 500ms timer events
      if (millis() - lastTimerUpdate > 50) {
        lastTimerUpdate = millis();
        counter++;
        bT50MS = true;
        if (counter % 10 == 0) {
          bT500MS = true;
        }
        if (counter % 20 == 0) {
          bT1000MS = true;
        }
        if (counter % 100 == 0) {
          bT5S = true;
        }
      } 
    }
    
    void updateTouchEvents() {
      //digitalWrite(13, HIGH);
      TSPoint p = ts.getPoint();
      //digitalWrite(13, LOW);
    
      // if sharing pins, you'll need to fix the directions of the touchscreen pins
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      
      // we have some minimum pressure we consider 'valid'
      // pressure of 0 means no pressing!
    
      bOnTouch = false;
      
      if (!bLock && p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        bOnTouch = true;
        bLock = true;
        touchX = Display.width()-(map(p.x, TS_MINX, TS_MAXX, Display.width(), 0));
        touchY = Display.height()-(map(p.y, TS_MINY, TS_MAXY, Display.height(), 0));
        //Serial.println(touchX);
        //Serial.println(touchY);
      }
      
      if(bT500MS)
        bLock = false;
    }
};

EventManager Events;

#define LOG_INTERVAL_HOUR 0
#define LOG_INTERVAL_MINUTE 1
#define LOG_INTERVAL_SECOND 2

class LogEventManager {      
    public:
      uint16_t counter;
      uint16_t interval;    
      unsigned bLog:1;
      unsigned bEnabled:1;
      
      uint8_t mode;
         
    public:
      LogEventManager() {
        interval = 3600; // every hour, interval in seconds
        counter = interval;
        mode = 0;
      }
      
      void dispatch() {
        bLog = false;
        
        if (!bEnabled)
          return;
	
        if(RTC.b1S) {          
          if(--counter == 0) {
            counter = interval;
            bLog = true;
          }
        }        
      }
      
      void setMode(uint8_t mode) {
        switch(mode) {
          case LOG_INTERVAL_HOUR:
            interval = 3600;                        
            //counter = 3600;
            counter = 3600 - (60 * RTC.now.minute) - RTC.now.second;
            break;
            
          case LOG_INTERVAL_MINUTE:
            interval = 60;            
            //counter = 60;
            counter = 60 - RTC.now.second;
            break;
            
          case LOG_INTERVAL_SECOND:
            interval = 5;
            counter = 5;
            break;
        }
      }
      
      void start() {
        bEnabled = true;
        counter = interval;
        bLog = false;	
      }
      
      void reset() {        
        counter = interval;
        bLog = false;
      }
};

LogEventManager LogEvents;

#endif
