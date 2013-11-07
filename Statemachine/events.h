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
    unsigned bTP500MS:1;
  
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
      // generate 50ms, 500ms timer events
      if (millis() - lastTimerUpdate > 50) {
        lastTimerUpdate = millis();
        counter++;
        bT50MS = true;
        if (counter % 10 == 0) {
          bT500MS = true;
          bTP500MS = bTP500MS ? false : true;
        }
      } else {
        bT50MS = false;
        bT500MS = false;
      }
    }
};

EventManager Events;

class MeasureEventManager {
    private:
      unsigned long lastUpdate;
    
    public:
      unsigned long interval;    
      unsigned bShtMeasure:1;  
    
    public:
      MeasureEventManager() {
        interval = 5; // secounds
      }
      
      void doHandleEvents() {
        if (rtc.getTimestamp(0) - lastUpdate > interval) {
          lastUpdate = rtc.getTimestamp();
          bShtMeasure = true;
        } else {
          bShtMeasure = false;
        }
      }      
};

MeasureEventManager MeasureEvents;
