/*
  RTC RV3049

 This example code is in the public domain.
 */

/*
   Pin configuration
 
   Arduino      RV3049
 
   13-SCK       4-SCL   (orange)
   12-MISO      9-SDI   (brown)
   11-MOSI      5-SDO   (yellow)
   A5-CS        3-CS    (violet)
 
   VCC          1-VDD
   GND          6-VSS
   
 */
 
#include <SPI.h>

#define RTC_READ	0x80
#define RTC_WRITE	0x0
#define RTC_SR		0x10
#define RTC_PON		0x20

#define chipSelectPin A5

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

uint8_t bcdToDecimal (uint8_t bcdByte)
{
	return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

uint8_t decimalToBCD (uint8_t decimalByte)
{
	return (((decimalByte / 10) << 4) | (decimalByte % 10));
}

class RV3049 {
  public:
    void begin() {
      pinMode(chipSelectPin, OUTPUT);
      digitalWrite(chipSelectPin, LOW);
    }
    
    byte isrunning(void) {
      return true;
    }
    
    DateTime now() {
      uint8_t page = 1;
      uint8_t pageaddr =0x0;
      uint8_t addr = ((page << 3) | pageaddr) | RTC_READ;
      
      digitalWrite(chipSelectPin, HIGH);
      
      SPI.transfer(addr);
      uint8_t seconds = SPI.transfer(0);
      uint8_t minutes = SPI.transfer(0);
      uint8_t hours = SPI.transfer(0);
      uint8_t weekdays = SPI.transfer(0);
      uint8_t days = SPI.transfer(0);
      uint8_t months = SPI.transfer(0);
      uint8_t years = SPI.transfer(0);
              
      digitalWrite(chipSelectPin, LOW);
	
      seconds = bcdToDecimal(seconds);
      minutes = bcdToDecimal(minutes);
      hours = bcdToDecimal(hours);
      weekdays = bcdToDecimal(weekdays);
      days =	bcdToDecimal(days);
      months = bcdToDecimal(months);
      years = bcdToDecimal(years);
      
      return DateTime (years, months, days, hours, minutes, seconds);
    }
  
    void adjust(const DateTime& dt) {
      setDateTime(dt.hour, dt.minute, 0);
    }
  
    void setDateTime(uint8_t h, uint8_t m, uint8_t s){
      uint8_t seconds = decimalToBCD(s);
      uint8_t minutes = decimalToBCD(m);
      uint8_t hours = decimalToBCD(h);
      uint8_t weekdays = decimalToBCD(0);
      uint8_t days = decimalToBCD(1);
      uint8_t months = decimalToBCD(1);
      uint8_t years = decimalToBCD(0);
      
      uint8_t page = 1;
      uint8_t pageaddr =0x0;
      uint8_t addr = ((page << 3) | pageaddr) | RTC_WRITE;
      
      digitalWrite(chipSelectPin, HIGH);
       
      SPI.transfer(addr);
      SPI.transfer(seconds);
      SPI.transfer(minutes);
      SPI.transfer(hours);
      SPI.transfer(weekdays);
      SPI.transfer(days);
      SPI.transfer(months);
      SPI.transfer(years);
      
      digitalWrite(chipSelectPin, LOW);
    }

    void dispatch() {
      
    }
    
    static void initRTC(void){
	
	bool bSetDefaultTime=false;
	
	uint8_t ctrl_Status = 0;
	uint8_t page = 0;
	uint8_t pageaddr =0x3;
	uint8_t addr = (page << 3 | pageaddr) | RTC_READ;

	digitalWrite(chipSelectPin, HIGH);
	ctrl_Status = SPI.transfer(addr);
	digitalWrite(chipSelectPin, LOW);
	
	if(ctrl_Status & RTC_PON){
		//POWER-ON Bit löschen
		page = 0;
		pageaddr =0x3;
		addr = (page << 3 | pageaddr) | RTC_WRITE;
		ctrl_Status &= ~RTC_PON;
		
		digitalWrite(chipSelectPin, HIGH);
		SPI.transfer(addr);
		SPI.transfer(ctrl_Status);
		digitalWrite(chipSelectPin, LOW);		
		bSetDefaultTime = true;
	}
	if(ctrl_Status & RTC_SR){
		//SELF-RECOVERY Bit löschen
		page = 0;
		pageaddr =0x3;
		addr = (page << 3 | pageaddr) | RTC_WRITE;
		ctrl_Status &= ~RTC_SR;
		
		digitalWrite(chipSelectPin, HIGH);
		SPI.transfer(addr);
		SPI.transfer(ctrl_Status);
		digitalWrite(chipSelectPin, LOW);
		
		bSetDefaultTime = true;
	}
	//if(bSetDefaultTime){
	//	SetDateTime(m_DefaultDateTime);
	//}
        //
     }

};

RV3049 rtc;
