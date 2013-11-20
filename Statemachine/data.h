
#define LOG_DATA_SIZE    24

class LogData {
  public:
    int8_t logOutTemperature[LOG_DATA_SIZE];
    int8_t logInTemperature[LOG_DATA_SIZE];
    
  public:
  
    void process() {
      if (LogEvents.bLog) {
        assignInTemperature(ShtMeasure.temperature);
        assignOutTemperature(DS1821.temperature);
      }
    }
  
    void assignOutTemperature(int8_t value) {
      for (byte i = 0; i < LOG_DATA_SIZE - 1; i++) {
        logOutTemperature[i + 1] = logOutTemperature[i];
      }
      logOutTemperature[0] = value;
    }
    
    void assignInTemperature(int8_t value) {
      for (byte i = 0; i < LOG_DATA_SIZE - 1; i++) {
        logInTemperature[i + 1] = logInTemperature[i];
      }
      logInTemperature[0] = value;
    }
};

LogData LogData;
