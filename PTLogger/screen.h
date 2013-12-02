#ifndef _SCREENH_
#define _SCREENH_

#define BACKCOLOR ST7735_BLACK

class Screen {
  public:
    unsigned bInvalidate:1;
    unsigned bVisible:1;
  public:
    Screen() {
    }
    
    virtual void hide() {
      Display.clearDisplay();
      bVisible = false;
    }
    
    virtual void show() {
      if (!bVisible) {
        bInvalidate = true;
      }
      bVisible = true;
    }
    
    virtual byte execute(byte input) {
    }
};

class MainScreen : public Screen {
  public:
    
  public:
    MainScreen() {      
    }
    
    void draw() {

      if (bInvalidate) {
        Display.displayText_f(0, 0, 2, ST7735_YELLOW, BACKCOLOR, PSTR("App Test"));
      }
      
      char buffer[9]= { "00:00:00" };  
      if (bInvalidate || Events.bT1000MS) {
        itochars(rtc.getHours(), &buffer[0], 2);
        itochars(rtc.getMinutes(), &buffer[3], 2);
        itochars(rtc.getSeconds(), &buffer[6], 2);  
        
        Display.displayText(5, 30, 2, buffer, ST7735_GREEN, BACKCOLOR);
      }
      
      if (bInvalidate || PT1000.bReady) {        
/*         itochars(PT1000.temperature, buffer, 2);
 *         strcpy(&buffer[2], " C");
 *         Display.displayText(25, 60, 2, buffer, ST7735_RED, BACKCOLOR);       
 */
        
      }
      
      
      bInvalidate = false;
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }
      
      draw();
  
      if (input == KEY_ENTER) {
        hide();
        return ST_MAIN_MENU;
      }  
      return ST_MAIN;
    }
};

MainScreen MainScreen;


class MenuScreen : public Screen {
  private:
    byte selected;  // selected menu item
    byte count;     // number of menu items
    
  public: 
    unsigned bInvalidateText:1;  
    Frame selectedFrame;
    
  public:   
    void draw() {
      if (!bInvalidate) {
        return;
      }
      if (bInvalidateText) { 
        drawMenu();
        bInvalidateText = false;
        selectedFrame.bInvalidate = true;
      }
      
      selectedFrame.setRect(0, 4 + selected * TEXTHEIGHT * 2 + selected * 4, ST7735_TFTHEIGHT, TEXTHEIGHT * 2 + 4);
      selectedFrame.draw();
    }
    
    void drawMenu() {        
      byte y = 2;
      byte group;
      byte state;
      
      PGM_P menuText;
      
      count = 0;
      
      for (byte i=0; (group = pgm_read_byte(&menu_state[i].group)); i++) {
        if (group == StateMachine.stateGroup) {
          state = pgm_read_byte(&menu_state[i].state);
          menuText = (PGM_P)pgm_read_word(&menu_state[i].pText);
          if (menuText != NULL) {
            if (state == group) {
              Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, ST7735_WHITE);  // draw menu title
              Display.displayText_f(4, y, 2, BACKCOLOR, ST7735_WHITE, menuText);              
              y += 4;
            } else {
              Display.displayText_f(4, y, 2, menuText);  // draw menu item
              count++;
            }
            y += TEXTHEIGHT * 2 + 4;
          }
        }
      }
      
      selected = 1;
    }
    
    byte getSelectedState() {
      byte group;
            
      for (byte i = 0; (group = pgm_read_byte(&menu_state[i].group)); i++) {
        if (group == StateMachine.stateGroup) {
          return pgm_read_byte(&menu_state[i + selected].state);
        }
      }
      
      return 0;
    }
    
    void show() {
      if (!bVisible) {
        bInvalidate = true;
        bInvalidateText = true;
      }
      bVisible = true;
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }
  
      if (input == KEY_PLUS) {
        selected %= count;
        selected++;
        bInvalidate = true;
      } else if (input == KEY_ENTER) {
        StateMachine.stateGroup = getSelectedState();
        hide();
        return StateMachine.stateGroup;
      } 
      
      draw();
      
      return StateMachine.state;
    }
    
    static byte xy(byte intput) {
    }
};

MenuScreen MenuScreen;

class EditTimeScreen : public Screen {
  public:
    EditTime edTime;

  public:
    byte editTime(byte input) {      
      if (!edTime.editTime(input)) {
        hide();
        return ST_DATE_TIME_MENU;
      }
      
      return StateMachine.state;
    } 
    
    void show() {
      if (!bVisible) {
        bInvalidate = true;        
        edTime.bInvalidateText = true;
        edTime.bInvalidatePos = true;
      }
      bVisible = true;
    }
    
    void draw() {
      if (!bInvalidate) {
        return;
      }
      
      Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, ST7735_WHITE);  // draw menu title
      Display.displayText_f(4, 2, 2, BACKCOLOR, ST7735_WHITE, PSTR("SET TIME"));
      
      bInvalidate = false;    
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }     
      
      draw();
      
      return editTime(input);      
    }
};

EditTimeScreen EditTimeScreen;

class EditDateScreen : public Screen {
  public:
    EditDate edDate;

  public:
    byte editDate(byte input) {      
      if (!edDate.editDate(input)) {
        hide();
        return ST_DATE_TIME_MENU;
      }
      
      return StateMachine.state;
    } 
    
    void show() {
      if (!bVisible) {
        bInvalidate = true;        
        edDate.bInvalidateText = true;
        edDate.bInvalidatePos = true;
      }
      bVisible = true;
    }
    
    void draw() {
      if (!bInvalidate) {
        return;
      }
      
      Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, ST7735_WHITE);  // draw menu title
      Display.displayText_f(4, 2, 2, BACKCOLOR, ST7735_WHITE, PSTR("SET Date"));
      
      bInvalidate = false;    
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }     
      
      draw();
      
      return editDate(input);      
    }
};

EditDateScreen EditDateScreen;

class LogSettingsScreen : public Screen {
  public:
    EditYesNoOption logOption;
  
  public:    
    void show() {
      if (!bVisible) {
        bInvalidate = true;        
        logOption.bInvalidateText = true;
        logOption.bInvalidateSelection = true;
      }
      bVisible = true;
    }
    
    void draw() {
      if (!bInvalidate) {
        return;
      }
      
      Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, ST7735_WHITE);  // draw menu title
      Display.displayText_f(4, 2, 2, BACKCOLOR, ST7735_WHITE, PSTR("Set Logging"));
      
      bInvalidate = false;    
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }     
      
      draw();
      
      logOption.getOption(input);
      
      if (input == KEY_ENTER) {
        hide();
        return ST_MAIN_MENU;
      }
    }
};

LogSettingsScreen LogSettingsScreen;

#endif
