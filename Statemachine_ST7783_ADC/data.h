#define LOG_DATA_SIZE    24

SdCard card;
Fat16 file;

char name[] = "yyyymmnn.TXT";

class LogData {
  public:
    byte month;
    unsigned bLog2File;
    int8_t temperature1Log[LOG_DATA_SIZE];
    int8_t temperature2Log[LOG_DATA_SIZE];
    int8_t count;
  public:
  
    void dispatch() {
      if (LogEvents.bLog) {                
        assignValues(temperature1Log, Measure.temperature, count);
        assignValues(temperature2Log, Measure.temperature2, count);
        
        // create new file every month
        DateTime dt = RTC.now();
        if(dt.month != month) {
          month = dt.month;
          createNewLogFile();
        }
        
        if(bLog2File) {
          log2File(Measure.temperature, Measure.temperature2);
        }
        
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
    
    void log2File(byte value1, byte value2) {
      // initialize the SD card
      if (!card.init()) {
        Serial.println(F("Error: int SD card"));
        file.close();
        return;
      }
      // initialize a FAT16 volume
      if (!Fat16::init(&card)) {
        file.close();
        Serial.println(F("Fat16::init"));
        return;
      }
      
      if (file.open(name, O_APPEND | O_EXCL | O_WRITE)) {
        DateTime dt = RTC.now();
                      // 01234567890123456789012345678
        char buffer[] = "yyyy-mm-dd hh:mm:ss; 000; 000";
        bin2asc(dt.year, buffer, 4);
        bin2asc(dt.month, &buffer[5], 2);
        bin2asc(dt.day, &buffer[8], 2);
        bin2asc(dt.hour,&buffer[11], 2);
        bin2asc(dt.minute, &buffer[14], 2);
        bin2asc(dt.second, &buffer[17], 2);
        bin2asc(value1, &buffer[21], 3);
        bin2asc(value2, &buffer[26], 3);
        file.println(buffer);      
        file.close();
        //Serial.println(F("Write to log"));
        //Serial.println(buffer);
      }
    }
    
    void createNewLogFile() {
      // initialize the SD card
      if (!card.init()) Serial.println(F("Error: int SD card"));
      // initialize a FAT16 volume
      if (!Fat16::init(&card)) Serial.println(F("Fat16::init"));
      
      // create a new file
      //                012345678901
      // char name[] = "yyyymmnn.TXT";
      DateTime dt = RTC.now();
      
      for (uint8_t i = 0; i < 100; i++) {
        bin2asc(dt.year, name, 4);
        bin2asc(dt.month, &name[4], 2);
        name[6] = i/10 + '0';
        name[7] = i%10 + '0';
        //Serial.println(name);
        // O_CREAT - create the file if it does not exist
        // O_EXCL - fail if the file exists
        // O_WRITE - open for write
        //if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
        if (file.open(name, O_CREAT | O_WRITE)) break;
      }
      if (file.isOpen()) {
        file.write_P(PSTR("Logger\r\n"));
        file.close();
        bLog2File = true;
      } else {
        bLog2File = false;
        Serial.println(F("Error: file.open"));
      }
    }
};


LogData LogData;
