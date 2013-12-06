#ifndef _DISPLAYH_
#define _DISPLAYH_


class Display : public LiquidCrystal {
  public:    
    void print(byte x, byte y, char * str) {
      setCursor(x, y)
      print(str);
    }
    
    void print_f(byte x, byte y, const char *pFlashStr) {      
      for (byte i = 0; (const char)(pgm_read_byte(&pFlashStr[i])) && i < 40; i++) {
        setCursor(x + i, y);
        char c = pgm_read_byte(&pFlashStr[i]);
        if (c == '\0') break;
        write(pgm_read_byte(&pFlashStr[i]));
      }
    }
}

#endif