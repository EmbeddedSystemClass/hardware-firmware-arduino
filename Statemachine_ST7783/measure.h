#ifndef _MEASUREH_
#define _MEASUREH_

//OneWire ds(A5);

#define DEV_ID 0x90 >> 1 // shift required by wire.h
#define DEV_ID2 0x92 >> 1 // shift required by wire.h

class DS1621 {
  public:
    unsigned bReady:1;       
    int8_t temperature;
    int8_t temperature2;

  public:
    void begin() {
      Wire.begin();
      Wire.beginTransmission(DEV_ID); // connect to DS1621 (#0)
      Wire.write(0xAC); // Access Config
      Wire.write(0x02); // set for continuous conversion
      Wire.beginTransmission(DEV_ID); // restart
      Wire.write(0xEE); // start conversions
      Wire.endTransmission();
      
      Wire.begin();
      Wire.beginTransmission(DEV_ID2); // connect to DS1621 (#0)
      Wire.write(0xAC); // Access Config
      Wire.write(0x02); // set for continuous conversion
      Wire.beginTransmission(DEV_ID2); // restart
      Wire.write(0xEE); // start conversions
      Wire.endTransmission();
    }
    
    void dispatch() {
      bReady = false;
      
      if (Events.bT5S) {                
        Wire.beginTransmission(DEV_ID);
        Wire.write(0xAA); // read temperature command
        Wire.endTransmission();
        Wire.requestFrom(DEV_ID, 2);	// request two bytes from DS1621 (0.5 deg. resolution)         
        temperature = Wire.read();	// get first byte

        Wire.beginTransmission(DEV_ID2);
        Wire.write(0xAA); // read temperature command
        Wire.endTransmission();
        Wire.requestFrom(DEV_ID2, 2);	// request two bytes from DS1621 (0.5 deg. resolution)         
        temperature2 = Wire.read();	// get first byte
        
        //Serial.print("T1:"); Serial.println(temperature);
        //Serial.print("T2:"); Serial.println(temperature2);
        
        bReady = true;
      }
    }
};

DS1621 DS1621;

#endif
