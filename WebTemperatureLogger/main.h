/*
  main functions

 This example code is in the public domain.
 */
 
#define SS_SD_CARD   4 	// PORTD Pin 2
#define SS_ETHERNET 10	// PORTB Pin 2

#define CHARTONUM(a, b) ((a - '0') * b)

//#define SD_ACTIVE() {  digitalWrite(SS_SD_CARD, LOW); digitalWrite(SS_ETHERNET, HIGH); }
//#define ETH_ACTIVE() {  digitalWrite(SS_SD_CARD, HIGH); digitalWrite(SS_ETHERNET, LOW); }

#define SD_ACTIVE() {  PORTD &= ~2; PORTB |= 2; }
#define ETH_ACTIVE() {  PORTD |= 2; PORTB &= ~2; }

swRTC RTC;

void bin2asc(int value, char buffer[], uint8_t digits);

