//#include "events.h"

class Edit {
  public:
    byte pos;
    
  public:
    Edit(): pos(0) {
    }
    
    byte editStr(byte x, byte y, PGM_P pMask, char buffer[], char length, byte key) {      
      if (key == KEY_NEXT)
      {
        length--;
        if (++pos >= length)
        {
          pos = 0;
          return false;
        }
    
        while ('_' != pgm_read_byte(pMask + pos))
        {
          if (++pos >= length)
          {
            pos = 0;
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
      }
      
      
      
      Display.setTextColor(BLACK, WHITE);
      
      if (Events.bTP500MS) {
        Display.setCursor(x + pos * TEXTWIDTH, y + 2);
        Display.write('_');
      }
      
      Display.setTextSize(1);
      Display.setCursor(x, y);
      Display.println(buffer);
              
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
      if (!editStr(10, 15, PSTR("__:__"), buffer, BUFFER_SIZE, input)) {
        byte h = CHARTONUM(buffer[0], 10) + CHARTONUM(buffer[1], 1);
        byte m = CHARTONUM(buffer[3], 10) + CHARTONUM(buffer[4], 1);
        rtc.setTime(h, m, 0);
        return false;
      }
      return true;
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
      if (!editStr(0, 22, PSTR("____.__.__"), buffer, BUFFER_SIZE, input)) {
        int y = CHARTONUM(buffer[0], 1000) + CHARTONUM(buffer[1], 100) + CHARTONUM(buffer[2], 10) + CHARTONUM(buffer[3], 1);
        byte m = CHARTONUM(buffer[5], 10) + CHARTONUM(buffer[6], 1);
        byte d = CHARTONUM(buffer[8], 10) + CHARTONUM(buffer[9], 1);
        rtc.setDate(d, m, y);
        return false;
      }
      return true;
    }
  
  
  private :
    char validateStr(char* buffer, byte pos, char c) {    
      if (c > '9') return '0';
      return c;
    }
};
