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
    
      uint32_t last;    
      uint16_t interval;  // seconds
    
    private:
      int16_t delta;
         
    public:
      LogEventManager() {
        bEnabled = false;
      }

      void dispatch() {
        bLog = false;
        
        if(!bEnabled || interval == 0)
          return;
        
        uint32_t current = (RTC.getHours() * 3600) + 
          (RTC.getMinutes() * 60) + RTC.getSeconds();
        
        delta -= (current - last);
        last = current;
        
        if(delta > interval) {
          delta = interval;
        } else if(delta <= 0) {
          delta = interval;
          bLog = true;
        }
        
      }
      
      void begin() {
        bEnabled = true;        
        bLog = false;
        last = 0;
        delta = 0;
        interval = 0;
      }
      
      void stop() {        
        bEnabled = false;
        bLog = false;
      }
};

LogEventManager LogEvents;

#endif

