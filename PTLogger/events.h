#ifndef _EVENTSH_
#define _EVENTSH_

// Events ****************************************************************

#define BTN1 0
#define BTN2 1

class EventManager {
  private:
    byte lastButtonState;
    byte counter;
    unsigned long lastTimerUpdate;
  
  public:
    unsigned bBtn1:1;
    unsigned bBtn2:1;

    unsigned bT50MS:1;
    unsigned bT500MS:1;
    unsigned bT1000MS:1;
    unsigned bTP500MS:1;
    unsigned bTP1000MS:1;
  
    unsigned bMeasure:1;

    void doHandleEvents() {
      updateTimerEvents();
      updateButtonFlags();
    }

  private:
    void updateButtonFlags() {
      // read the state of the pushbuttons
      byte buttonState = ~PINC & 3;

      if(bT50MS && lastButtonState != buttonState) {    
        lastButtonState = buttonState;    
        bBtn1 = bitRead(lastButtonState, BTN1);
        bBtn2 = bitRead(lastButtonState, BTN2);    
      } else {
        bBtn1 = false;
        bBtn2 = false;   
      }  
    }
    
    void updateTimerEvents() {
      bT50MS = false;
      bT500MS = false;
      bT1000MS = false;
      
      // generate 50ms, 500ms timer events
      if (millis() - lastTimerUpdate > 50) {
        lastTimerUpdate = millis();
        counter++;
        bT50MS = true;
        if (counter % 10 == 0) {
          bT500MS = true;
          bTP500MS = bTP500MS ? false : true;
        }
        if (counter % 20 == 0) {
          bT1000MS = true;
          bTP1000MS = bTP1000MS ? false : true;
        }
      } 
    }
};

EventManager Events;

class MeasureEventManager {
    private:
      unsigned long lastUpdate;
    
    public:
      unsigned long interval;    
      unsigned bMeasure:1;  
    
    public:
      MeasureEventManager() {
        interval = 5; // seconds
      }
      
      void doHandleEvents() {
        if (rtc.getTimestamp() - lastUpdate > interval) {
          lastUpdate = rtc.getTimestamp();
          bMeasure = true;
        } else {
          bMeasure = false;
        }
      }      
};

MeasureEventManager MeasureEvents;

class LogEventManager {
    private:
      unsigned long lastUpdate;
    
    public:
      unsigned long interval;    
      unsigned bLog:1;
      unsigned bEnabled:1;
         
    public:
      LogEventManager() {
        interval = 3600; // seconds
      }
      
      void doHandleEvents() {
        if (!bEnabled)
          return;
        if (rtc.getTimestamp() - lastUpdate > interval) {
          lastUpdate = rtc.getTimestamp();
          bLog = true;
        } else {
          bLog = false;
        }
      }
      
      void start() {
        bEnabled = true;
        lastUpdate = rtc.getTimestamp() - interval + 10;
      }
};

LogEventManager LogEvents;

#endif
