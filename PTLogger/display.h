#ifndef _DISPLAYH_
#define _DISPLAYH_


class Display : public LiquidCrystal {
  public:

    
    Display(uint8_t rs,  uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3):LiquidCrystal(rs, enable,  d0,  d1,  d2,  d3)
    {
      //init(1, rs, 255, enable, d0, d1, d2, d3, 0, 0, 0, 0);
    }
    
    void print(byte x, byte y, char *str) {            
      setCursor(x, y);
      LiquidCrystal::print(str);
    }
    
    void print_f(byte x, byte y, const char *pFlashStr) {      
      for (byte i = 0; (const char)(pgm_read_byte(&pFlashStr[i])) && i < 40; i++) {
        setCursor(x + i, y);
        char c = pgm_read_byte(&pFlashStr[i]);
        if (c == '\0') break;
        write(pgm_read_byte(&pFlashStr[i]));
      }
    }
};

Display lcd(8, 9, 4, 5, 6, 7);

#endif
