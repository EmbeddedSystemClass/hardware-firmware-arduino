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


// Events ****************************************************************

class EventManager {
  private:
    uint8_t lastButtonState;
    uint16_t counter;
    unsigned long lastTimerUpdate;
  
  public:
    unsigned bOnTouch:1;
    unsigned bLock:1;

    unsigned bT50MS:1;
    unsigned bT500MS:1;
    unsigned bT1000MS:1;
    unsigned bT5S:1;
    unsigned bT1MIN:1;

    void dispatch() {
      updateTimerEvents();      
    }

  private:    
    void updateTimerEvents() {
      bT50MS = false;
      bT500MS = false;
      bT1000MS = false;
      bT5S = false;
      bT1MIN = false;
      
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
        if (counter % 1200 == 0) {
          bT1MIN = true;
        }
      } 
    }
};

EventManager Events;

#define LOG_INTERVAL_HOUR 0
#define LOG_INTERVAL_MINUTE 1
#define LOG_INTERVAL_SECOND 2
#define LOG_INTERVAL_5S 5

class LogEventManager {      
    public:        
      unsigned bLog:1;
      unsigned bEnabled:1;
      
      uint8_t mode;
      uint8_t last;
         
    public:
      LogEventManager() {        
        setMode(LOG_INTERVAL_HOUR);
        bEnabled = false;
      }

      void dispatch() {
        bLog = false;
        
        if(!bEnabled)
          return;

        switch(mode) {			
          case LOG_INTERVAL_HOUR:
            if(RTC.getHours() != last) {              
              last = RTC.getHours();
              bLog = true;              
            }
            break;
          case LOG_INTERVAL_MINUTE:
            if(RTC.getMinutes() != last) {              
              last = RTC.getMinutes();
              bLog = true;              
            }
            break;
//          case LOG_INTERVAL_SECOND:
//            if(RTC.b1S) {					
//              if(--last == 0) {                
//                bLog = true;
//                last = LOG_INTERVAL_5S;
//              }
//            }
//            break;
        }
      }
      
      void setMode(uint8_t newMode) {
        mode = newMode;
        switch(mode) {
          case LOG_INTERVAL_HOUR:
            last = RTC.getHours();
            break;
          case LOG_INTERVAL_MINUTE:
            last = RTC.getMinutes();
            break;
//          case LOG_INTERVAL_SECOND:
//            last = LOG_INTERVAL_5S;
//            break;
        }
      }
      
      void begin() {
        bEnabled = true;        
        bLog = false;
      }
      
      void stop() {        
        bEnabled = false;
        bLog = false;
      }
};

LogEventManager LogEvents;

#endif

