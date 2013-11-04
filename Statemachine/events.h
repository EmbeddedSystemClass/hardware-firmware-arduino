#define BTN1 0
#define BTN2 1

class EventManager {
  private:
    byte lastButtonState;
    byte counter;
    unsigned long lastUpdateTime;
  
  public:
    unsigned bBtn1:1;
    unsigned bBtn2:1;

    unsigned bT50MS:1;
    unsigned bT500MS:1;
    unsigned bTP500MS:1;

    void doHandleEvents() {
      updateEvents();
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
    
    void updateEvents() {
      // generate 50ms, 500ms timer events
      if (millis() - lastUpdateTime > 50) {
        lastUpdateTime = millis();
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
