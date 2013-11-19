
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
      byte i = 0;
      for (; i < LOG_DATA_SIZE - 1; i++) {
        logOutTemperature[i] = logInTemperature[i + 1];
      }
      logOutTemperature[i] = value;
    }
    
    void assignInTemperature(int8_t value) {
      byte i = 0;
      for (; i < LOG_DATA_SIZE - 1; i++) {
        logInTemperature[i] = logInTemperature[i + 1];
      }
      logInTemperature[i] = value;
    }
};

LogData LogData;
