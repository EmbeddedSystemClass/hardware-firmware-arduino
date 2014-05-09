#ifndef _MEASUREH_
#define _MEASUREH_

// DATA  PIN 7
// SCLK  PIN 8
Sensirion sht = Sensirion(7, 8);

class ShtMeasureManager {
  private:
    unsigned int rawData;
    byte shtError;
  public:
    unsigned bMeasure:1;
    unsigned bTempMeasure:1; 
    unsigned bHumiMeasure:1;   
    unsigned bReady:1;   
    int8_t temperature;
    uint8_t humidity;

  void doMeasure() {
    bReady = false;
    if (!bMeasure && MeasureEvents.bShtMeasure) {      // Time for new measurements?
      bMeasure = true;
      bTempMeasure = true;
      sht.meas(TEMP, &rawData, NONBLOCK); // Start temp measurement
      //Serial.println("start temp measure");
    }
    
    if (bMeasure && (shtError = sht.measRdy())) { // Check measurement status
      if (bTempMeasure) {                    // Process temp or humi?
        bTempMeasure = false;
        bHumiMeasure = true;
        temperature = (int8_t)sht.calcTemp(rawData);     // Convert raw sensor data
        sht.meas(HUMI, &rawData, NONBLOCK); // Start humi measurement
        //Serial.println("start humi measure");
        //Serial.println(temperature);
      } 
      else if (bHumiMeasure) 
      {
        bHumiMeasure = false;
        bMeasure = false;
        bReady = true;
        humidity = (uint8_t)sht.calcHumi(rawData, temperature); // Convert raw sensor data
        //Serial.println("measure ready");
      }
    }
  }

};

ShtMeasureManager ShtMeasure;


OneWire ds(6);

class DS1821 {
  public:
    unsigned bReady:1;       
    int8_t temperature;

  public:
    void doMeasure() {
      bReady = false;
      
      if (MeasureEvents.bShtMeasure) {
        ds.reset();
        ds.write(0xEE);
        ds.reset();
        ds.write(0xAA);
        temperature = ds.read();
        bReady = true;
      }
    }
};

DS1821 DS1821;

#endif
