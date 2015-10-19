#ifndef _MEASUREH_
#define _MEASUREH_

// DS1821 on digital pin 2 external
OneWire ds(2);

// DS1621
class DS1621 {
  public:
    unsigned bReady:1;       
    int8_t temperature;
    int8_t temperature2;

  public:
    void begin() {
      //start 1821 conversion external
     ds.reset();
     ds.write(0xEE);
    }
    
    void dispatch() {
      bReady = false;
      
      if (Events.bT5S) {                
        // external
       byte temp_;
       byte r;
       byte t;
       int i;
       byte present;
      
      
       // Read value external
       present = ds.reset();
       ds.reset();
       ds.write(0xAA);
       temp_ = ds.read();
      
       
      
       //start 1821 conversion external
       ds.reset();
       ds.write(0xEE);
      
       
       // negative temp external
       if (temp_ >> 7 == 1){
         r = temp_ ^ 0xFF; // bit inversion external
         t = r + 1;        // external
      
         Serial.print("T");
         Serial.print("-");
         Serial.print(t);
         Serial.print("C");
         Serial.println();
       }
       // positive temp external
       else
       {
         t = temp_; // external
      
         Serial.print("T");
         Serial.print("+");
         Serial.print(t);
         Serial.print("C");
         Serial.println();
       }
       
       temperature = t;
       
      }
    }
};

DS1621 Measure;

#endif
