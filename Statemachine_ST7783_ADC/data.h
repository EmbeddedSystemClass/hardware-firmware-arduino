#ifndef _DATAH_
#define _DATAH_

/*
  Data logging
  - chart datasource array
  - logging to SD Card

 This example code is in the public domain.
 */

#define LOG_DATA_SIZE    24

SdCard card;
Fat16 file;

char name[] = "yyyymmnn.TXT";

class LogData {
  public:
    unsigned bLog2SdEnabled:1;
    unsigned bLogFileAvailable:1;
       
    int8_t temperature1Log[LOG_DATA_SIZE];
    int8_t temperature2Log[LOG_DATA_SIZE];
    
    int16_t avgTemperatur1;
    int16_t avgTemperatur2;
    uint8_t avgCount;
  
    uint8_t month; 
    int8_t count;
	
  public:   
    LogData() {
      bLog2SdEnabled = true;
      bLogFileAvailable = false;
      month = 0;
      count = 0;
      avgCount = 0;
      
      avgTemperatur1 = 0;
      avgTemperatur2 = 0;
    }
  
    void begin() {
      avgTemperatur1 = Measure.temperature;
      avgTemperatur2 = Measure.temperature2;
      avgCount = 1;
    }
  
    void dispatch() {
      if (Events.bT1MIN) {
        avgTemperatur1 += Measure.temperature;
        avgTemperatur2 += Measure.temperature2;
        avgCount++;
      }
      
      if (LogEvents.bLog) {
        int8_t avg1 = avgTemperatur1 / avgCount;
        int8_t avg2 = avgTemperatur2 / avgCount;
        
        pushBack(temperature1Log, avg1, count);
        pushBack(temperature2Log, avg2, count);        
        
        if(bLog2SdEnabled) {
          // create new file every month          
          if(RTC.now.month != month) {
            month = RTC.now.month;
            createNewLogFile();
          }
          
          if(bLogFileAvailable) {
            log2File(avg1, avg2);
          }          
        }
        
        avgTemperatur1 = 0;
        avgTemperatur2 = 0;
        avgCount = 0;
        
        if (count < LOG_DATA_SIZE) {
          count++;
        }
      }
    }
  
    void pushBack(int8_t values[], int8_t value, uint8_t n) {
      uint8_t i;
      if (n >= LOG_DATA_SIZE) {
        for (i = 0; i < LOG_DATA_SIZE - 1; i++) {
          values[i] = values[i + 1];
        }
        n = LOG_DATA_SIZE - 1;
      }
      values[n] = value;
    }
    
    void getStat(int8_t values[], uint8_t count, int8_t* min, int8_t* max /*, int8_t* avg*/) {
      //int avgSum = 0;
      for (uint8_t i = 0; i < count; i++) {
        *min = min(values[i], *min);
        *max = max(values[i], *max);
        //avgSum += values[i];
      }
      //*avg = avgSum / count;      
    }
    
    void reset(int8_t values[]) {
      for (uint8_t i = 0; i < LOG_DATA_SIZE; i++) {
        values[i] = 0;
      }
      count = 0;
    }    
   
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
    
    void log2File(uint8_t value1, uint8_t value2) {
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
        DateTime* dt = &RTC.now;
                      // 01234567890123456789012345678
        char buffer[] = "yyyy-mm-dd hh:mm:ss; 000; 000";
        bin2asc(dt->year, buffer, 4);
        bin2asc(dt->month, &buffer[5], 2);
        bin2asc(dt->day, &buffer[8], 2);
        bin2asc(dt->hour,&buffer[11], 2);
        bin2asc(dt->minute, &buffer[14], 2);
        bin2asc(dt->second, &buffer[17], 2);
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
      DateTime* dt = &RTC.now;
      
      for (uint8_t i = 0; i < 100; i++) {
        bin2asc(dt->year, name, 4);
        bin2asc(dt->month, &name[4], 2);
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
        bLogFileAvailable = true;
      } else {
        bLogFileAvailable = false;
        Serial.println(F("Error: file.open"));
      }
    }
};

LogData LogData;

#endif
