#define SIGNATURE_SIZE 4
#define FRAME_SIZE 10

static byte Signature[] = { 0xCC, 0x33, 0x55, 0xAA };

class Comunication {
  public:
    byte sigOffset;
    byte dataOffset;
    byte checkSum;
    byte data[FRAME_SIZE];
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
        if(dataOffset < FRAME_SIZE) {
          data[dataOffset] = inData;
          checkSum += inData;
          dataOffset++;
        } else {
          bValid = checkSum == inData;          
          sigOffset = 0;
          dataOffset = 0;
          
          if(data[0] == 0)
            rtc.setTime(data[1], data[2], data[3]);
          if(data[0] == 1)
            rtc.setDate(data[4], data[3], data[2] * 1000 + data[1]);
        }
      }
    }
};

Comunication Com;
