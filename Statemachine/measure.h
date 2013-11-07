
class THMeasureManager {
  private:
    unsigned int rawData;
  public:
    unsigned bMeasure:1;
    unsigned bTempMeasure:1; 
    unsigned bHumiMeasure:1;   
    unsigned bReady:1;   
    int8_t temperature;
    uint8_t humidity;
    unsigned int rawData;
  
  void doMeasure() {
    if (!bMeasure && millis() % 5000UL == 0) {      // Time for new measurements?
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

THMeasureManager TempHumiMeasure;