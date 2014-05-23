#define LOG_DATA_SIZE    24

SdCard card;
Fat16 file;

char name[] = "LOG00.TXT";

class LogData {
  public:
    int8_t logOutTemperature[LOG_DATA_SIZE];
    int8_t count;
  public:
  
    void dispatch() {
      if (LogEvents.bLog) {
        assignValues(logOutTemperature, DS1821.temperature, count);
        log2File(DS1821.temperature);
        
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
    
    /*
     * Write an unsigned number to file
     */
    void writeNumber(uint32_t n) {
      uint8_t buf[10];
      uint8_t i = 0;
      do {
        i++;
        buf[sizeof(buf) - i] = n%10 + '0';
        n /= 10;
      } while (n);
      file.write(&buf[sizeof(buf) - i], i); // write the part of buf with the number
    }
    
    void log2File(byte value) {
      if (file.open(name, O_APPEND | O_EXCL | O_WRITE)) {
        //file.write("line "); // write string from RAM
        writeNumber(value);
        //file.write_P(PSTR(" millis = ")); // write string from flash
        //writeNumber(millis());
        file.write("\r\n"); // file.println() would work also
        file.close();
        Serial.println("Write to log");
      }
    }
    
    void initialize() {
      // initialize the SD card
      if (!card.init()) Serial.println("Error: int SD card");
      // initialize a FAT16 volume
      if (!Fat16::init(&card)) Serial.println("Fat16::init");
      
      // create a new file
      
      for (uint8_t i = 0; i < 100; i++) {
        name[3] = i/10 + '0';
        name[4] = i%10 + '0';
        // O_CREAT - create the file if it does not exist
        // O_EXCL - fail if the file exists
        // O_WRITE - open for write
        if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
      }
      if (!file.isOpen()) Serial.println("file.open");
      file.write("Logger\r\n");
      file.close();
    }
};


LogData LogData;
