#define WIRE Wire

#define DS1307_ADDRESS 0x68

class DateTime {
 
  public:
    unsigned int year;
    byte month;
    byte day;
    byte hour;
    byte minute;
    byte second;
    
    DateTime() { }
   
    DateTime (uint16_t y, byte m, byte d, byte h, byte mn, byte s) {
//      if (y >= 2000)
//          y -= 2000;
      year = y;
      month = m;
      day = d;
      hour = h;
      minute = mn;
      second = s;
    }
};

////////////////////////////////////////////////////////////////////////////////
// RTC_DS1307 implementation

static byte bcd2bin (byte val) { return val - 6 * (val >> 4); }
static byte bin2bcd (byte val) { return val + 6 * (val / 10); }

class RTCDS1307 {
  public:
    unsigned long lastUpdate;
  
  public:  
    byte isrunning(void) {
      WIRE.beginTransmission(DS1307_ADDRESS);
      WIRE.write(0);
      WIRE.endTransmission();
  
      WIRE.requestFrom(DS1307_ADDRESS, 1);
      byte ss = WIRE.read();
      return !(ss>>7);
    }
  
    void adjust(const DateTime& dt) {
        WIRE.beginTransmission(DS1307_ADDRESS);
        WIRE.write(0);
        WIRE.write(bin2bcd(dt.second));
        WIRE.write(bin2bcd(dt.minute));
        WIRE.write(bin2bcd(dt.hour));
        WIRE.write(bin2bcd(0));
        WIRE.write(bin2bcd(dt.day));
        WIRE.write(bin2bcd(dt.month));
        WIRE.write(bin2bcd(dt.year - 2000));
        WIRE.write(0);
        WIRE.endTransmission();
    }
  
    DateTime now() {
      WIRE.beginTransmission(DS1307_ADDRESS);
      WIRE.write(0);	
      WIRE.endTransmission();
  
      WIRE.requestFrom(DS1307_ADDRESS, 7);
      byte ss = bcd2bin(WIRE.read() & 0x7F);
      byte mm = bcd2bin(WIRE.read());
      byte hh = bcd2bin(WIRE.read());
      WIRE.read();
      byte d = bcd2bin(WIRE.read());
      byte m = bcd2bin(WIRE.read());
      uint16_t y = bcd2bin(WIRE.read()) + 2000;
      
      return DateTime (y, m, d, hh, mm, ss);
    }
    
    void dispatch() {
      if (millis() - lastUpdate > 3599000) {
          lastUpdate = millis();
          DateTime dt = now();
          dt.second-=2;
          adjust(dt);
        }
    }
};

RTCDS1307 rtc;
