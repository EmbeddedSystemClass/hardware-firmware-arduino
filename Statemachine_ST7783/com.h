#define SIGNATURE_SIZE 4
#define DATA_SIZE 9
#define CMD_TIME 0
#define CMD_DATE 1
#define CMD_CURRENT_TEMP 2
#define CMD_TEMP_LOG 3
#define CMD_DIRECTORY 4
#define CMD_FILE 5

static byte Signature[] = { 0xCC, 0x33, 0x55, 0xAA };

class Comunication {
  public:
    byte sigOffset;
    byte dataOffset;
    byte checkSum;
    char data[DATA_SIZE];
    unsigned bValid:1;
    
    void receive(byte inData) {      
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
        case CMD_TIME:         
          //rtc.setTime(data[1], data[2], data[3]);
          break;
        case CMD_DATE:
          //rtc.setDate(data[4], data[3], data[2] * 1000 + data[1]);
          break;
        case CMD_CURRENT_TEMP:
          Serial.write(DS1821.temperature);
          Serial.flush();
          break;
        case CMD_TEMP_LOG:
          for(byte i = 0; i < 24; i++) {
            Serial.write(LogData.logOutTemperature[i]);
          }
          Serial.flush();
          break;
        case CMD_DIRECTORY:
          if (card.init() && Fat16::init(&card)) {
            // "Name          Modify Date/Time    Size";
            Fat16::ls(LS_DATE | LS_SIZE);
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
};

Comunication Com;
