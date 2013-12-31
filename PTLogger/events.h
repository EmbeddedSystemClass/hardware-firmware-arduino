#ifndef _EVENTSH_
#define _EVENTSH_

// Events ****************************************************************

#define BTN1 0
#define BTN2 1

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
    
class EventManager {
  private:
    int lastButtonState;
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
      int buttonState = read_LCD_buttons();

      if(bT50MS && lastButtonState != buttonState) {    
        lastButtonState = buttonState;    
        bBtn1 = buttonState == btnUP;
        bBtn2 = buttonState == btnDOWN;   
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
    
    

     
    // read the buttons
    int read_LCD_buttons()
    {
     int adc_key_in = analogRead(0);      // read the value from the sensor 
     // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
     // we add approx 50 to those values and check to see if we are close
     if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
     // For V1.1 us this threshold
     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 250)  return btnUP; 
     if (adc_key_in < 450)  return btnDOWN; 
     if (adc_key_in < 650)  return btnLEFT; 
     if (adc_key_in < 850)  return btnSELECT;  
     
     return btnNONE;  // when all others fail, return this...
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
        if (rtc.now().unixtime() - lastUpdate > interval) {
          lastUpdate = rtc.now().unixtime();
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
        if (rtc.now().unixtime() - lastUpdate > interval) {
          lastUpdate = rtc.now().unixtime();
          bLog = true;
        } else {
          bLog = false;
        }
      }
      
      void start() {
        bEnabled = true;
        lastUpdate = rtc.now().unixtime() - interval + 10;
      }
};

LogEventManager LogEvents;

#endif
