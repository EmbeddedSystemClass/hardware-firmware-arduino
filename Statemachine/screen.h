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

class Component {
  public:
    unsigned bVisible:1;
    unsigned bInvalidate:1;
   
  public:
    Component() {
      bVisible = true;
      bInvalidate = true;
    }
    
    virtual void draw() {
    } 
    
    void setVisible(byte state) {
      if (bVisible == state)
        return;
        
      //Serial.println(state);
      bInvalidate = true;
      bVisible = state;
    }
};

class Frame : public Component {
  private:
    byte oldX;
    byte oldY;
    byte oldW;
    byte oldH;
  public:
    byte x;
    byte y;
    byte w;
    byte h;
    
  public:
    Frame() : Component() {
      
    }
   
   void setRect(byte px, byte py, byte pw, byte ph) {
     x = px;
     y = py;
     w = pw;
     h = ph;
     if (x != oldX || y != oldY || w != oldW || h != oldH) {
       bInvalidate = true;
     }
   }
   
   void draw() {
     if (!bInvalidate)
       return;
       
     Display.drawRect(oldX, oldY, oldW, oldH, BACKCOLOR);
     
     if (bVisible)  
       Display.drawRect(x, y, w, h, ST7735_YELLOW);
     
     oldX = x;
     oldY = y;
     oldW = w;     
     oldH = h;
     
     bInvalidate = false;
   }
};

class TempGauge {
  public:
    
  public:
    TempGauge() {
    }
    
    void draw(byte x, byte y, byte value) {
      Display.fillCircle(x + 3, y, 3, ST7735_WHITE);
      Display.fillRect(x, y , 7, 40, ST7735_WHITE);
      Display.fillCircle(x + 3, y + 46, 8, ST7735_WHITE);
      
      uint16_t color = value > 20 ? ST7735_RED : ST7735_BLUE;
      
      Display.fillRect(x + 2, y + 2, 3, 40, color);
      Display.fillCircle(x + 3, y + 46, 6, color);
      
      Display.fillRect(x + 2, y, 3, map(value, -20, 40, 0, 40), ST7735_BLACK);
    }
};

class MainScreen : public Screen {
  public:
    TempGauge tempGauge;
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
      
      if (bInvalidate || ShtMeasure.bReady) {        
        itochars(ShtMeasure.temperature, buffer, 2);
        strcpy(&buffer[2], " C");
        Display.displayText(25, 60, 2, buffer, ST7735_RED, BACKCOLOR);
        
        itochars(ShtMeasure.humidity, buffer, 2);
        strcpy(&buffer[2], " %");
        Display.displayText(25, 80, 2, buffer, ST7735_RED, BACKCOLOR);
        
        tempGauge.draw(10, 60, ShtMeasure.temperature);
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

class TempChartScreen : public Screen {
  public:
    TemperatureChartDiagram chart;
  public:
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }  
      
      if (bInvalidate) {        
        for (int8_t i= 0; i < 24; i++)
          chart.assignValue(random(-20, 30));
    
        chart.drawTempChart(input);
        
        bInvalidate = false;
      }
      
      if (input == KEY_ENTER) {
        hide();
        return ST_MAIN_MENU;
      }
        
      return StateMachine.state;
    }
};

TempChartScreen TempChartScreen;

#endif
