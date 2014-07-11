#ifndef _RTCH_
#define _RTCH_

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

#define CS_RTC A5

static byte bcd2bin (byte val) { return val - 6 * (val >> 4); }
static byte bin2bcd (byte val) { return val + 6 * (val / 10); }

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
      year = y;
      month = m;
      day = d;
      hour = h;
      minute = mn;
      second = s;
    }
		
    uint32_t getTimeStamp() {      
      return ((uint32_t)hour * 3600) + ((uint32_t)minute * 60) + (uint32_t)second;
    }		
};

class RV3049 {
  public:
    uint32_t maxTimeStamp;
    unsigned b1S:1;
    
    uint8_t lastSecond;
    
    DateTime now1;
	
  public:
    RV3049() {
      maxTimeStamp = 24 * 3600; //maximum  in 24hour mode, 24h x 3600s = 86400s
      b1S = false;
    }
	
    void begin() {
      pinMode(CS_RTC, OUTPUT);
      digitalWrite(CS_RTC, LOW);
    }
    
    byte isrunning(void) {
      return true;
    }
    
    DateTime now() {
      uint8_t page = 1;
      uint8_t pageaddr =0x0;
      uint8_t addr = ((page << 3) | pageaddr) | RTC_READ;

      SPI.setClockDivider(SPI_CLOCK_DIV4);
      
      digitalWrite(CS_RTC, HIGH);
      
      SPI.transfer(addr);
      uint8_t seconds = SPI.transfer(0);
      uint8_t minutes = SPI.transfer(0);
      uint8_t hours = SPI.transfer(0);      
      uint8_t days = SPI.transfer(0);
      uint8_t weekdays = SPI.transfer(0);
      uint8_t months = SPI.transfer(0);
      uint8_t years = SPI.transfer(0);
              
      digitalWrite(CS_RTC, LOW);
	
      seconds = bcd2bin(seconds);
      minutes = bcd2bin(minutes);
      hours = bcd2bin(hours);      
      days =	bcd2bin(days);
      weekdays = bcd2bin(weekdays);
      months = bcd2bin(months);
      years = bcd2bin(years);
      
      return DateTime (years, months, days, hours, minutes, seconds);

    }
  
    void adjust(const DateTime& dt) {
      SPI.setClockDivider(SPI_CLOCK_DIV4);
      
      uint8_t seconds = bin2bcd(dt.second);
      uint8_t minutes = bin2bcd(dt.minute);
      uint8_t hours = bin2bcd(dt.hour);
      //uint8_t weekdays = bin2bcd(0);
      uint8_t days = bin2bcd(dt.day);
      uint8_t months = bin2bcd(dt.month);
      uint8_t years = bin2bcd(dt.year - 2000);

      uint8_t page = 1;
      uint8_t pageaddr =0x0;
      uint8_t addr = ((page << 3) | pageaddr) | RTC_WRITE;
      
      digitalWrite(CS_RTC, HIGH);
       
      SPI.transfer(addr);
      SPI.transfer(seconds);
      SPI.transfer(minutes);
      SPI.transfer(hours);
      SPI.transfer(days);
      SPI.transfer(0);      
      SPI.transfer(months);
      SPI.transfer(years);
      
      digitalWrite(CS_RTC, LOW);
    }		

    void dispatch() {
      now1 = now();
      
      b1S = false;
      if(now1.second != lastSecond) {
        lastSecond = now1.second;
        b1S = true;
      }
    }
    
    static void initRTC(void) {
	
      bool bSetDefaultTime=false;
	
      uint8_t ctrl_Status = 0;
      uint8_t page = 0;
      uint8_t pageaddr =0x3;
      uint8_t addr = (page << 3 | pageaddr) | RTC_READ;

      digitalWrite(CS_RTC, HIGH);
      ctrl_Status = SPI.transfer(addr);
      digitalWrite(CS_RTC, LOW);
      
      if(ctrl_Status & RTC_PON) {
        //POWER-ON Bit löschen
        page = 0;
        pageaddr =0x3;
        addr = (page << 3 | pageaddr) | RTC_WRITE;
        ctrl_Status &= ~RTC_PON;
        
        digitalWrite(CS_RTC, HIGH);
        SPI.transfer(addr);
        SPI.transfer(ctrl_Status);
        digitalWrite(CS_RTC, LOW);		
        bSetDefaultTime = true;
      }
      if(ctrl_Status & RTC_SR) {
        //SELF-RECOVERY Bit löschen
        page = 0;
        pageaddr =0x3;
        addr = (page << 3 | pageaddr) | RTC_WRITE;
        ctrl_Status &= ~RTC_SR;
        
        digitalWrite(CS_RTC, HIGH);
        SPI.transfer(addr);
        SPI.transfer(ctrl_Status);
        digitalWrite(CS_RTC, LOW);
        
        bSetDefaultTime = true;
      }
    }

};

RV3049 RTC;

#endif
