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

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

struct DateTime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t weekday;
};

class RV3049 {
  public:    
    unsigned b1S:1;    
    uint8_t lastSecond;    
    DateTime now;
	
  public:
    RV3049() {      
      b1S = false;
    }
	
    void begin() {
      pinMode(CS_RTC, OUTPUT);
      digitalWrite(CS_RTC, LOW);
      
      initRTC();
      dispatch();
    }
    
    uint8_t isrunning(void) {
      return true;
    }
    
    void adjust(DateTime* dt) {
      SPI.setClockDivider(SPI_CLOCK_DIV4);
      
      uint8_t seconds = bin2bcd(dt->second);
      uint8_t minutes = bin2bcd(dt->minute);
      uint8_t hours = bin2bcd(dt->hour);
      //uint8_t weekdays = bin2bcd(0);
      uint8_t days = bin2bcd(dt->day);
      uint8_t months = bin2bcd(dt->month);
      uint8_t years = bin2bcd(dt->year - 2000);

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
      uint8_t page = 1;
      uint8_t pageaddr =0x0;
      uint8_t addr = ((page << 3) | pageaddr) | RTC_READ;

      SPI.setClockDivider(SPI_CLOCK_DIV4);
      
      digitalWrite(CS_RTC, HIGH);
      
      SPI.transfer(addr);
      now.second = SPI.transfer(0);
      now.minute = SPI.transfer(0);
      now.hour = SPI.transfer(0);      
      now.day = SPI.transfer(0);
      now.weekday = SPI.transfer(0);
      now.month = SPI.transfer(0);
      now.year = SPI.transfer(0);
              
      digitalWrite(CS_RTC, LOW);
	
      now.second = bcd2bin(now.second);
      now.minute = bcd2bin(now.minute);
      now.hour = bcd2bin(now.hour);      
      now.day =	bcd2bin(now.day);
      now.weekday = bcd2bin(now.weekday);
      now.month = bcd2bin(now.month);
      now.year = bcd2bin(now.year);
      
      b1S = false;
      if(now.second != lastSecond) {
        lastSecond = now.second;
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
