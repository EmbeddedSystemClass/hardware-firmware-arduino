
#define LOG_DATA_SIZE    24

class LogData {
  public:
    int8_t logOutTemperature[LOG_DATA_SIZE];
    //int8_t logInTemperature[LOG_DATA_SIZE];
    int8_t count;
  public:
  
    void process() {
      if (LogEvents.bLog) {
	if (count < LOG_DATA_SIZE) {
          count++;
        }
          
        assignValues(logOutTemperature, DS1821.temperature, count);
        //assignValues(logInTemperature, ShtMeasure.temperature, count);
      }
    }
  
    void assignValues(int8_t values[], int8_t value, byte n) {
      byte i;
      if (n >= LOG_DATA_SIZE) {
	for (i = 0; i < LOG_DATA_SIZE - 1; i++) {
	  values[i] = values[i + 1];
	}
	n = LOG_DATA_SIZE - 1;
      }
      values[n] = value;
    }    
};


LogData LogData;
