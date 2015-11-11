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
            // 012345678901
char name[] = "00000000.LOG";

class LogData {
  public:
    unsigned bLog2SdEnabled:1;
    unsigned bLogFileAvailable:1;
       
    int8_t temperature1Log[LOG_DATA_SIZE];
    int8_t temperature2Log[LOG_DATA_SIZE];
    int8_t count;
  
  public:   
    LogData() {
      bLog2SdEnabled = false;
      bLogFileAvailable = false;      
      count = 0;
    }   
  
    void stopLog() {
      bLog2SdEnabled = false;
      bLogFileAvailable = false; 
    }
    
    void startLog() {
      bLog2SdEnabled = true;
      createNewLogFile();
    }
    
    void dispatch() {
      if (LogEvents.bLog) {       
        pushBack(temperature1Log, Measure.temperature, count);
        pushBack(temperature2Log, Measure.temperature2, count);        
        
        if(bLog2SdEnabled) {          
          if(bLogFileAvailable) {
            log2File(Measure.temperature, Measure.temperature2);
          }          
        }
        
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
      for (uint8_t i = 0; i < count; i++) {
        *min = min(values[i], *min);
        *max = max(values[i], *max);
      }
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
    
    void log2File(int8_t value1, int8_t value2) {
      SD_ACTIVE();
      
      // initialize the SD card
      if (!card.init(0, SS_SD_CARD)) {
        Serial.println(F("Error: SD card init"));
        file.close();
        ETH_ACTIVE();
        return;
      }
      // initialize a FAT16 volume
      if (!Fat16::init(&card)) {
        file.close();
        Serial.println(F("Error: Fat16::init"));
        ETH_ACTIVE();
        return;
      }      
      
      if (file.open(name, O_APPEND | O_EXCL | O_WRITE)) {
                      // 0123456789012345678901234567890
        char buffer[] = "yyyy-mm-dd hh:mm:ss; 0000; 0000";
        bin2asc(RTC.getYear(), buffer, 4);
        bin2asc(RTC.getMonth(), &buffer[5], 2);
        bin2asc(RTC.getDay(), &buffer[8], 2);
        bin2asc(RTC.getHours(),&buffer[11], 2);
        bin2asc(RTC.getMinutes(), &buffer[14], 2);
        bin2asc(RTC.getSeconds(), &buffer[17], 2);
        bin2asc(value1, &buffer[21], 4);
        bin2asc(value2, &buffer[27], 4);
        file.println(buffer);      
        file.close();
      }
      
      ETH_ACTIVE();
    }
    
    void createNewLogFile() {
      SD_ACTIVE();
      
      // initialize the SD card
      if (!card.init(0, SS_SD_CARD)) Serial.println(F("Error: int SD card"));
      // initialize a FAT16 volume
      if (!Fat16::init(&card)) Serial.println(F("Fat16::init"));
      
      // create a new file
      //     012345678901
      //    "00001.LOG"
      
      for (int i = 1; i < 10000; i++) {
        bin2asc(i, name, 5);
        name[5] = '.';
        name[6] = 'L';
        name[7] = 'O';
        name[8] = 'G';
        name[9] = 0;
        //Serial.println(name);
        // O_CREAT - create the file if it does not exist
        // O_EXCL - fail if the file exists
        // O_WRITE - open for write
        if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
        //if (file.open(name, O_CREAT | O_WRITE)) break;
      }
      if (file.isOpen()) {
        Serial.println("Log start");
        file.write_P(PSTR("Logger\r\n"));
        file.close();
        bLogFileAvailable = true;
      } else {
        bLogFileAvailable = false;
        Serial.println(F("Error: file.open"));
      }
      
      ETH_ACTIVE();
    }
};

LogData LogData;

#endif
