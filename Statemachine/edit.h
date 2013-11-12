#ifndef _EDITH_
#define _EDITH_

// Editors ***************************************************************

#define BACKCOLOR ST7735_BLACK

class Edit {
  public:
    unsigned bInvalidateText:1;
    unsigned bInvalidatePos:1;
    
    byte pos;
    
  public:
    Edit(): pos(0) {
      bInvalidateText = true;
      bInvalidatePos = true;
    }
    
    byte editStr(byte x, byte y, PGM_P pMask, char buffer[], char length, byte key) {      
      if (key == KEY_ENTER)
      {
        bInvalidatePos = true;
        
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
        bInvalidateText = true;
      }
      
      byte y2 = y + 4;
      byte y3 = y2 + TEXTHEIGHT * 2  + 2;
      
      if (bInvalidatePos) {
        Display.drawLine(x, y, x + strlen(buffer) * TEXTWIDTH * 2, y, BACKCOLOR);      
        Display.drawLine(x, y3, x + strlen(buffer) * TEXTWIDTH * 2, y3, BACKCOLOR);

        Display.drawLine(x + pos * TEXTWIDTH * 2, y, x + (pos + 1) * TEXTWIDTH * 2 - 2, y, ST7735_YELLOW);
        Display.drawLine(x + pos * TEXTWIDTH * 2, y3, x + (pos + 1) * TEXTWIDTH * 2 - 2, y3, ST7735_YELLOW);
        
        bInvalidatePos = false;
      }
      
      if (bInvalidateText) {
        Display.displayText(x, y2, 2, buffer, ST7735_YELLOW, BACKCOLOR);
        bInvalidateText = false;
      }
      
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
      if (!editStr(10, 30, PSTR("__:__"), buffer, BUFFER_SIZE, input)) {
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
      if (!editStr(10, 30, PSTR("____.__.__"), buffer, BUFFER_SIZE, input)) {
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

class EditOption {
  public:
    unsigned bInvalidateSelection:1;
    unsigned bInvalidateText:1;
    byte selected;

  public:
	  EditOption() {
      bInvalidateSelection = true;
      bInvalidateText = true;
    }
    
    void setOptions(byte x, byte y, char* options[], byte count, byte width, byte input) {
      if (input == KEY_PLUS) {
        selected = selected + 1 < count ? selected + 1 :  0;
        bInvalidateSelection = true;
      }
      
      if (bInvalidateSelection) {        
        Display.drawRect(x, y + selected * (TEXTHEIGHT * 2 + 2), width + 3, TEXTHEIGHT * 2 + 3, BLACK);
      }
      if (invalidateText) {
        for (int i=0; i < count; i++) {
            Display.displayText(x + 2, y + i * (TEXTHEIGHT + 2) + 2, 1, ST7735_YELLOW, BACKCOLOR, options[i]);        
        }
      }
    }
};

class EditYesNoOption : public EditOption {
    public:
      byte getOption(byte input) {
          char * OPT[] = { "YES", "NO" };
          
          if (input==KEY_ENTER) {
            return selected;
          }
          
          setOptions(5, 30, OPT, 2, TEXTWIDTH * 3, input);
          return 0;
      }
};

EditYesNoOption EditYesNo;

#endif
