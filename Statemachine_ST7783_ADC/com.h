#ifndef _COMH_
#define _COMH_

/*
  Communication
  
 This example code is in the public domain.
 */

#define SIGNATURE_SIZE 4
#define DATA_SIZE 9

#define CMD_TIME 0
#define CMD_DATE 1
#define CMD_SENSOR_VALUES 2
#define CMD_TEMP_LOG 3
#define CMD_DIRECTORY 4
#define CMD_FILE 5

#define OP_DIR 0
#define OP_DELETE_ALL 1

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
            DateTime* dt = &RTC.now;
            dt->hour = data[1];
            dt->minute = data[2];
            dt->second = data[3];
            RTC.adjust(dt);
            break; 
          }
        case CMD_DATE: {
            DateTime* dt = &RTC.now;
            dt->day = data[4];
            dt->month = data[3];
            dt->year = 2000 + data[2];
            RTC.adjust(dt);
            break; 
          }
        case CMD_SENSOR_VALUES:
          switch(data[1]) {
            case 0:
              Serial.write(Measure.temperature);
              break;
            case 1:
                Serial.write(Measure.temperature2);
                break;
          }
          Serial.flush();
          break;
        case CMD_TEMP_LOG:          
          for(uint8_t i = 0; i < 24; i++) {
            Serial.write(LogData.temperature1Log[data[1]]);
          }                    
          Serial.flush();
          break;
        case CMD_DIRECTORY:
          if (card.init() && Fat16::init(&card)) {
            // "Name          Modify Date/Time    Size";
            //Fat16::ls(LS_DATE | LS_SIZE);
            sdOperation(OP_DIR);
            Serial.println(F("EOF"));
          } else {
            Serial.println(F("card failed!"));
          }
          break;
        case CMD_FILE:
          if (card.init() && Fat16::init(&card)) {
            char name[] = "01234567.TXT";
            name[0] = data[1];
            name[1] = data[2];
            name[2] = data[3];
            name[3] = data[4];
            name[4] = data[5];
            name[5] = data[6];
            name[6] = data[7];
            name[7] = data[8];            
            if(file.open(name, O_READ)) {
              int16_t c;
              while ((c = file.read()) > 0) 
                Serial.write((char)c);
              file.close();
            }
            Serial.println(F("EOF"));            
          } else {
            Serial.println(F("card failed!"));
          }
          break;
      }
      
      bValid = false;
    }
    
    void sdOperation(uint8_t op) {
      dir_t d;
      for (uint16_t index = 0; Fat16::readDir(&d, &index, DIR_ATT_VOLUME_ID); index++) {                   
        if(op == OP_DELETE_ALL) {
          Fat16::remove((char*)d.name);
        } else if(op == OP_DIR) {
          for (uint8_t i = 0; i < 11; i++) {
            if (d.name[i] == ' ') continue;
            if (i == 8) {
              Serial.write('.');            
            }
            Serial.write(d.name[i]);
          }          
          Serial.println();
        }
      }
    }
};

Comunication Com;

#endif
