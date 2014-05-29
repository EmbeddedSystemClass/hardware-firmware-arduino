#ifndef _MEASUREH_
#define _MEASUREH_

//OneWire ds(A5);

#define DEV_ID 0x90 >> 1 // shift required by wire.h

class DS1621 {
  public:
    unsigned bReady:1;       
    int8_t temperature;

  public:
    void begin() {
      Wire.begin();
      Wire.beginTransmission(DEV_ID); // connect to DS1621 (#0)
      Wire.write(0xAC); // Access Config
      Wire.write(0x02); // set for continuous conversion
      Wire.beginTransmission(DEV_ID); // restart
      Wire.write(0xEE); // start conversions
      Wire.endTransmission();
    }
    
    void dispatch() {
      bReady = false;
      
      if (MeasureEvents.bShtMeasure) {
         
        //delay(1000); // give time for measurement
         
        Wire.beginTransmission(DEV_ID);
        Wire.write(0xAA); // read temperature command
        Wire.endTransmission();
        Wire.requestFrom(DEV_ID, 2);	// request two bytes from DS1621 (0.5 deg. resolution)
         
        temperature = Wire.read();	// get first byte
        
        bReady = true;
      }
    }
};

DS1621 DS1621;

#endif
