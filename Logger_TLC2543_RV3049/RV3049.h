
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
   07-CS        3-CS    (violet)
 
   VCC          1-VDD
   GND          6-VSS
   
 */
 
#include <SPI.h>

#define RTC_READ	0x80
#define RTC_WRITE	0x0
#define RTC_SR		0x10
#define RTC_PON		0x20

#define chipSelectPin 6

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t weekdays;
  uint8_t days;
  uint8_t months;
  uint8_t years;
} TDATETIME;

TDATETIME DateTime;

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
    }
  
    void getDateTime(void){
      uint8_t page = 1;
      uint8_t pageaddr =0x0;
      uint8_t addr = ((page << 3) | pageaddr) | RTC_READ;
      
      digitalWrite(chipSelectPin, HIGH);
      
      SPI.transfer(addr);
      DateTime.seconds = SPI.transfer(0);
      DateTime.minutes = SPI.transfer(0);
      DateTime.hours = SPI.transfer(0);
      DateTime.weekdays = SPI.transfer(0);
      DateTime.days = SPI.transfer(0);
      DateTime.months = SPI.transfer(0);
      DateTime.years = SPI.transfer(0);
              
      digitalWrite(chipSelectPin, LOW);
      	
      DateTime.seconds = bcdToDecimal(DateTime.seconds);
      DateTime.minutes = bcdToDecimal(DateTime.minutes);
      DateTime.hours = bcdToDecimal(DateTime.hours);
      DateTime.weekdays = bcdToDecimal(DateTime.weekdays);
      DateTime.days =	bcdToDecimal(DateTime.days);
      DateTime.months = bcdToDecimal(DateTime.months);
      DateTime.years = bcdToDecimal(DateTime.years);	
    }
  
    void setDateTime(uint8_t h, uint8_t m, uint8_t s){
      DateTime.seconds = decimalToBCD(s);
      DateTime.minutes = decimalToBCD(m);
      DateTime.hours = decimalToBCD(h);
      DateTime.weekdays = decimalToBCD(0);
      DateTime.days = decimalToBCD(1);
      DateTime.months = decimalToBCD(1);
      DateTime.years = decimalToBCD(0);
      
      uint8_t page = 1;
      uint8_t pageaddr =0x0;
      uint8_t addr = ((page << 3) | pageaddr) | RTC_WRITE;
      
      digitalWrite(chipSelectPin, HIGH);
       
      SPI.transfer(addr);
      SPI.transfer(DateTime.seconds);
      SPI.transfer(DateTime.minutes);
      SPI.transfer(DateTime.hours);
      SPI.transfer(DateTime.weekdays);
      SPI.transfer(DateTime.days);
      SPI.transfer(DateTime.months);
      SPI.transfer(DateTime.years);
      
      digitalWrite(chipSelectPin, LOW);
    }


//static void initRTC(void){
//	
//	bool bSetDefaultTime=false;
//	
//	uint8_t ctrl_Status = 0;
//	uint8_t page = 0;
//	uint8_t pageaddr =0x3;
//	uint8_t addr = (page << 3 | pageaddr) | RTC_READ;
//
//	spi_select_device(SPI, &SpiDeviceConfig);
//	spi_write_packet(SPI,&addr,1);
//	spi_read_packet(SPI,&ctrl_Status,1);
//	spi_deselect_device(SPI, &SpiDeviceConfig);
//	
//	if(ctrl_Status & RTC_PON){
//		//POWER-ON Bit löschen
//		page = 0;
//		pageaddr =0x3;
//		addr = (page << 3 | pageaddr) | RTC_WRITE;
//		ctrl_Status &= ~RTC_PON;
//		
//		spi_select_device(SPI, &SpiDeviceConfig);
//		spi_write_packet(SPI,&addr,1);
//		spi_write_packet(SPI, &ctrl_Status ,1);
//		spi_deselect_device(SPI, &SpiDeviceConfig);		
//		bSetDefaultTime = true;
//	}
//	if(ctrl_Status & RTC_SR){
//		//SELF-RECOVERY Bit löschen
//		page = 0;
//		pageaddr =0x3;
//		addr = (page << 3 | pageaddr) | RTC_WRITE;
//		ctrl_Status &= ~RTC_SR;
//		
//		spi_select_device(SPI, &SpiDeviceConfig);
//		spi_write_packet(SPI,&addr,1);
//		spi_write_packet(SPI, &ctrl_Status ,1);
//		spi_deselect_device(SPI, &SpiDeviceConfig);
//		
//		bSetDefaultTime = true;
//	}
//	if(bSetDefaultTime){
//		SetDateTime(m_DefaultDateTime);
//	}
////
//}

};

RV3049 RTC;
