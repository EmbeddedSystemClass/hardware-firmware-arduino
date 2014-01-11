#ifndef _EDITH_
#define _EDITH_

// Editors ***************************************************************

#define BACKCOLOR ST7735_BLACK

class Edit {
  public:
    unsigned bInvalidate:1;        
    byte pos;
    
  public:
    Edit(): pos(0) {
      bInvalidate = true;      
    }
    
    byte editStr(byte x, byte y, PGM_P pMask, char buffer[], char length, byte key) {      
      if (key == KEY_ENTER)
      {
        length--;
        if (++pos >= length)
        {
          pos = 0;
					lcd.noBlink();
          return false;
        }
    
        while ('_' != pgm_read_byte(pMask + pos))
        {
          if (++pos >= length)
          {
            pos = 0;
						lcd.noBlink();
            return false;
          }
        }
      } 
      else if (key == KEY_PLUS)
      {			
        char edit = pgm_read_byte(pMask + pos);
    
        if (edit == '_') 
        {   
          char c = buffer[pos] + 1;
          buffer[pos] = validateStr(buffer, pos, c);
        }
        bInvalidate = true;
      }
      
      if (bInvalidate) {
        lcd.print(0, 1, buffer);
        lcd.blink();
        bInvalidate = false;
      }      
      
      lcd.setCursor(pos, 1);			
      return true;
    }
    
    virtual char validateStr(char* buffer, byte pos, char c) = 0;
};

class EditTime : public Edit {
  public:
    static const byte BUFFER_SIZE = 6;
    char buffer[BUFFER_SIZE];
    
  public:
    EditTime()  {
      strcpy_P(buffer, PSTR("00:00"));
      pos = 0;
    }
  
    byte editTime(byte input) {
      if (!editStr(0, 1, PSTR("__:__"), buffer, BUFFER_SIZE, input)) {
        byte h = CHARTONUM(buffer[0], 10) + CHARTONUM(buffer[1], 1);
        byte m = CHARTONUM(buffer[3], 10) + CHARTONUM(buffer[4], 1);
        DateTime dt = rtc.now();
        rtc.adjust(DateTime(dt.year(), dt.month(), dt.day(), h, m, 0));
        return false;
      }
      return true;
    }
    
    void setTime(DateTime dt) {
      itochars(dt.hour(), buffer, 2);
      itochars(dt.minute(), &buffer[3], 2);
      bInvalidate = true;
    }
  
  private :
    char validateStr(char* buffer, byte pos, char c) {    
      if (c > '9') return '0';
      
      buffer[pos] = c;
      
      byte h = CHARTONUM(buffer[0], 10) + CHARTONUM(buffer[1], 1);
      byte m = CHARTONUM(buffer[3], 10) + CHARTONUM(buffer[4], 1);
      return h <= 23 && m <= 59 ? c : '0';
    }
};

class EditDate : public Edit {
  public:
    static const byte BUFFER_SIZE = 11;
    char buffer[BUFFER_SIZE];
    
  public:
    EditDate()  {
      strcpy_P(buffer, PSTR("0000.00.00"));
      pos = 0;
    }
  
    byte editDate(byte input) {
      if (!editStr(0, 1, PSTR("____.__.__"), buffer, BUFFER_SIZE, input)) {
        int y = CHARTONUM(buffer[0], 1000) + CHARTONUM(buffer[1], 100) + CHARTONUM(buffer[2], 10) + CHARTONUM(buffer[3], 1);
        byte m = CHARTONUM(buffer[5], 10) + CHARTONUM(buffer[6], 1);
        byte d = CHARTONUM(buffer[8], 10) + CHARTONUM(buffer[9], 1);
        DateTime dt = rtc.now();
        rtc.adjust(DateTime(y, m, d, dt.hour(), dt.minute(), dt.second()));
        return false;
      }
      return true;
    }
  
    void setDate(DateTime dt) {
      itochars(dt.year(), buffer, 4);
      itochars(dt.month(), &buffer[5], 2);
      itochars(dt.day(), &buffer[8], 2);
      bInvalidate = true;      
    }
  
  private :
    char validateStr(char* buffer, byte pos, char c) {    
      if (c > '9') return '0';
      return c;
    }
};

class EditNumber : public Edit {
  public:
    static const byte BUFFER_SIZE = 5;
    char buffer[BUFFER_SIZE];
    
  public:
    EditNumber()  {
      strcpy_P(buffer, PSTR("0000"));
      pos = 0;
    }
  
    byte editNumber(byte input) {
      if (!editStr(0, 1, PSTR("____"), buffer, BUFFER_SIZE, input)) {        
        return false;
      }
      return true;
    }
  
  
  private :
    char validateStr(char* buffer, byte pos, char c) {          
      return c >= '0' && c <= '9' ? c : '0';
    }
};

class EditOption {
  public:
    unsigned bInvalidate:1;
    byte selected;

  public:
    EditOption() {
      bInvalidate = true;
    }
    
    void setOptions(char* options[], byte count, byte input) {
      if (input == KEY_PLUS) {
        selected = selected + 1 < count ? selected + 1 :  0;
        bInvalidate = true;
      }
      
      if (bInvalidate) {
        lcd.print(0, 1, "                ");
        lcd.print(0, 1, options[selected]);
        bInvalidate = false;
      }
    }
};

class EditYesNoOption : public EditOption {
    public:
      byte editOption(byte input) {
        char * OPT[] = { "NO", "YES" };
          
        if (input==KEY_ENTER) {
          return false;
        }
          
        setOptions(OPT, 2, input);
        return true;
      }      
};


#endif
