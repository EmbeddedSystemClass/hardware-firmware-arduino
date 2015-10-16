/*
  main functions

 This example code is in the public domain.
 */

#define CHARTONUM(a, b) ((a - '0') * b)

swRTC RTC;

//void bin2asc(uint16_t value, char buffer[], uint8_t digits);
void bin2asc(int value, char buffer[], uint8_t digits);

