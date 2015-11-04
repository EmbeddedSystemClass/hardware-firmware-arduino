#ifndef _COMH_
#define _COMH_

/*
  Communication
  
 This example code is in the public domain.
 */


#define SIGNATURE_SIZE        4
#define DATA_SIZE             15

#define CMD_TIME              0
#define CMD_DATE              1
#define CMD_SENSOR_VALUES     2
#define CMD_TEMP_LOG          3
#define CMD_DIRECTORY         4
#define CMD_FILE              5
#define CMD_FILE_UPLOAD       6
#define CMD_DELETE_ALLFILES   7

#define OP_DIR                0
#define OP_DELETE_ALL         1

static uint8_t Signature[] = { 0xCC, 0x33, 0x55, 0xAA };

class Comunication {
  public:
    uint8_t sigOffset;
    uint8_t dataOffset;
    uint8_t checkSum;
    char data[DATA_SIZE];
    unsigned bValid:1;
    
    void receive(uint8_t inData) {      
      if(sigOffset < SIGNATURE_SIZE) {
        checkSum = 0;
        if(Signature[sigOffset] == inData) {
          sigOffset++;
        } else {
          sigOffset = 0;  // wrong signature
        }
      } else {
        if(dataOffset < DATA_SIZE) {
          data[dataOffset] = inData;
          checkSum += inData;
          dataOffset++;
        } else {
          bValid = true;//checkSum == inData;          
          sigOffset = 0;
          dataOffset = 0;          
        }
      }
    }
    
    void dispatch() {
      if(!bValid) 
        return;
      
      switch(data[0]) {
        case CMD_TIME: {
//            DateTime* dt = &RTC.now;
//            dt->hour = data[1];
//            dt->minute = data[2];
//            dt->second = data[3];
//            RTC.adjust(dt);
            break; 
          }
        case CMD_DATE: {
//            DateTime* dt = &RTC.now;
//            dt->day = data[4];
//            dt->month = data[3];
//            dt->year = 2000 + data[2];
//            RTC.adjust(dt);
            break; 
          }
        case CMD_SENSOR_VALUES: {
          switch(data[1]) {
            case 0:
              Serial.print(F("<TEMP>"));
              Serial.write(Measure.temperature);
              Serial.print(F("</TEMP>"));
              break;
            case 1:
              Serial.print(F("<TEMP>"));
              //Serial.write(Measure.temperature2);
              Serial.print(F("</TEMP>"));
              break;
          }
          Serial.flush();
          break;
        }
        case CMD_TEMP_LOG: {
          switch(data[1]) {
            case 0:
              sendDayLogData(LogData.temperature1Log, LOG_DATA_SIZE);
              break;
            case 1:
              sendDayLogData(LogData.temperature2Log, LOG_DATA_SIZE);
              break;
          }         
          break;
        }
        case CMD_DIRECTORY: {
          sendDir();
          break;
        }
        case CMD_DELETE_ALLFILES: {
          deleteFile();
          break;
        }
        case CMD_FILE: {
          sendFile();
          break;
        }
        case CMD_FILE_UPLOAD: {          
          receiveFile();
          break;
        }
      }
      
      bValid = false;
    }
    
    void sendFile(void) {
      SD_ACTIVE();
      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {
        char name[13] = { 0 };
        strncpy(name, data + 1, 12);
        
        if(file.open(name, O_READ)) {
          uint32_t nFileSize = file.fileSize();
          uint8_t* pFileSize = (uint8_t*)&nFileSize;
          Serial.print(F("<SIZE>"));
          Serial.write(*pFileSize++);
          Serial.write(*pFileSize++);
          Serial.write(*pFileSize++);
          Serial.write(*pFileSize);
          Serial.print(F("</SIZE>"));
          
          Serial.print(F("<FILE>"));
          for(int i = 0; i < file.fileSize(); i++) {
            Serial.write((char)file.read());
          }
          Serial.print(F("</FILE>"));
          
          file.close();
        }                        
      } else {
        Serial.println(F("card failed!"));
      }
      ETH_ACTIVE();
    }
    
    void receiveFile(void) {
      SD_ACTIVE();          
      Serial.println("<FILEUPLOAD>");
      char buffer[20] = { 0 };
      uint8_t state = 0;
      
      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {
        char name[13] = { 0 };
        strncpy(name, data + 1, 12);       
        
        file.remove(name);   
        if(file.open(name, O_CREAT | O_EXCL | O_WRITE)) {                    
          while(state != 2) {
            if(Serial.available() > 0) {
              char c = (char)Serial.read();
              
              buffer[sizeof(buffer) - 2] = c;
              
              switch(state) {
                case 0: {                 
                  if (strncasecmp_P(buffer, PSTR("<FILE>"), 6)==0) {                  
                    state = 1;
                    file.write(buffer[6]);
                  }
                  break;
                }
                
                case 1: {
                  if (strncasecmp_P(buffer + 6, PSTR("</FILE>"), 7)==0) {
                    state = 2;
                  } else {
                    file.write(buffer[6]);
                  }
                  break;
                }
                
                case 2: {
                    break; // for
                  break;
                }
              }
              
              memmove(buffer, buffer + 1, sizeof(buffer) - 2);
            }
          }              
          file.close();
          Serial.println("</FILEUPLOAD>");
        }                        
      } else {
        Serial.println(F("card failed!"));
      }
      
      ETH_ACTIVE();
    }
    
    void deleteFile(void) {
      SD_ACTIVE();
      
      char buffer[20] = { 0 };
      uint8_t state = 0;
      
      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {
        char name[13] = { 0 };
        strncpy(name, data + 1, 12);       
        
        file.remove(name);
      }
      
      ETH_ACTIVE();
    }
    
    void sendDir() {
      SD_ACTIVE();
      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {
        Serial.print(F("<DIR>"));
        dir_t d;
        for (uint16_t index = 0; Fat16::readDir(&d, &index, DIR_ATT_VOLUME_ID); index++) {        
          for (uint8_t i = 0; i < 11; i++) {
            if (d.name[i] == ' ') continue;
            if (i == 8) {
              Serial.write('.');            
            }
            Serial.write(d.name[i]);
          }          
          Serial.println();        
        }
      
        Serial.print(F("</DIR>"));
      } else {
        Serial.println(F("card failed!"));
      }
      ETH_ACTIVE();
    }
    
    void sendDayLogData(int8_t* pData, uint32_t nSize) {
      Serial.print(F("<DAYLOG>"));
      for(int i; i < nSize; i++) {
        Serial.write(*pData);        
        pData++;
      }
      Serial.print(F("</DAYLOG>"));
    }
};

Comunication Com;

#endif
