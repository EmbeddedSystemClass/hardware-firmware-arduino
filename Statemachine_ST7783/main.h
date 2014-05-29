// Ops ******************************************************************
#define CHARTONUM(a, b) ((a - '0') * b)

void bin2asc(unsigned int value, char buffer[], byte digits);


//class RTC : public RTC_DS1307 {
//  public:
//    RTC_DS1307 rtcDS1307;
//    DateTime dt;
//    
//  boolean setDate(byte dayT, byte monthT, int yearT) {
//    dt = rtcDS1307.now();
//    rtcDS1307.adjust(DateTime(yearT, monthT, dayT, dt.hour(), dt.minute(), dt.second()));
//  }
//  
//  boolean setTime(byte hourT, byte minuteT, byte secondT) {
//    dt = rtcDS1307.now();
//    rtcDS1307.adjust(DateTime(dt.year(), dt.month(), dt.day(), hourT, minuteT, secondT));
//  }
//  
//  unsigned long getTimestamp() {
//    dt = rtcDS1307.now();
//    return dt.unixtime();
//  }
//  
//  byte getDay() {
//    dt = rtcDS1307.now();
//    return dt.day();
//  }
//  
//  byte getHours() {
//    dt = rtcDS1307.now();
//    return dt.hour();
//  }
//  
//  byte getMinutes() {
//    dt = rtcDS1307.now();
//    return dt.minute();
//  }
//  
//  byte getSeconds() {
//    dt = rtcDS1307.now();
//    return dt.second();
//  }
//  
//  unsigned int getYear() {
//    dt = rtcDS1307.now();
//    return dt.year();
//  }
//  
//  byte getMonth() {
//    dt = rtcDS1307.now();
//    return dt.month();
//  }
//};
//
//class RTC :public RTC_DS1307 {
//  public:
//  DateTime now2() {
//    DateTime dt;
//    //digitalWrite(A3, HIGH);
//    //pinMode(A3, OUTPUT);
//    dt = now();
//    
//    //digitalWrite(A3, LOW);
//    //pinMode(A3, OUTPUT);
//    return dt;
//  }
//};


// Software RTC **********************************************************
//RTC rtc;
//RTC_DS1307 rtc;
//RTC rtc;
