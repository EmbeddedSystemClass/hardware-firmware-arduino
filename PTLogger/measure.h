#ifndef _MEASUREH_
#define _MEASUREH_

class PT1000 {
  public:
    unsigned bReady:1;       
    int8_t temperature;

  public:
    void doMeasure() {
      bReady = false;
      
      if (MeasureEvents.bMeasure) {
        // todo implement measure
        bReady = true;
      }
    }
};

PT1000 PT1000;

#endif
