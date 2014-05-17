
#define LOG_DATA_SIZE    24

class LogData {
  public:
    int8_t logOutTemperature[LOG_DATA_SIZE];
    int8_t count;
  public:
  
    void process() {
      if (LogEvents.bLog) {
        assignValues(logOutTemperature, DS1821.temperature, count);
        
	if (count < LOG_DATA_SIZE) {
          count++;
        }
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
      
      //Serial.println(values[n]);
      //Serial.println(n);
      //Serial.println("---");
    }
    
    void getStat(int8_t values[], byte count, int8_t* min, int8_t* max, int8_t* avg) {
      int avgSum = 0;
      for (byte i = 0; i < count; i++) {
        *min = min(values[i], *min);
        *max = max(values[i], *max);
        avgSum += values[i];
      }
      *avg = avgSum / count;      
    }
    
    void reset(int8_t values[]) {
      for (byte i = 0; i < LOG_DATA_SIZE; i++) {
        values[i] = 0;
      }
      count = 0;
    }
};


LogData LogData;
