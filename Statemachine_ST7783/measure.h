#ifndef _MEASUREH_
#define _MEASUREH_

//OneWire ds(A5);

class DS1821 {
  public:
    unsigned bReady:1;       
    int8_t temperature;

  public:
    void dispatch() {
      bReady = false;
      
      if (MeasureEvents.bShtMeasure) {
//        ds.reset();
//        ds.write(0xEE);
//        ds.reset();
//        ds.write(0xAA);
//        temperature = ds.read();
//        bReady = true;
      }
    }
};

DS1821 DS1821;

#endif
