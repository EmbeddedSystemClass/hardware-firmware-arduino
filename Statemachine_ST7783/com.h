#define SIGNATURE_SIZE 4
#define DATA_SIZE 6
#define CMD_TIME 0
#define CMD_DATE 1
#define CMD_CURRENT_TEMP 2
#define CMD_TEMP_LOG 3

static byte Signature[] = { 0xCC, 0x33, 0x55, 0xAA };

class Comunication {
  public:
    byte sigOffset;
    byte dataOffset;
    byte checkSum;
    byte data[DATA_SIZE];
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
          rtc.setTime(data[1], data[2], data[3]);
          break;
        case CMD_DATE:
          rtc.setDate(data[4], data[3], data[2] * 1000 + data[1]);
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
      }
      
      bValid = false;
    }
};

Comunication Com;
