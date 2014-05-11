#ifndef _SCREENH_
#define _SCREENH_

#define BACKCOLOR BLACK

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
       Display.drawRect(x, y, w, h, YELLOW);
     
     oldX = x;
     oldY = y;
     oldW = w;     
     oldH = h;
     
     bInvalidate = false;
   }
};

class Button {
  public: 
    Button() {
    }
    
    int x, y, width, height;
    
    bool hitTest(int tx, int ty) {
      if(tx > x && tx < (x + width)) {
        if(ty > y && ty < (y + height)) {
          return true;
        }
      }
      return false;
    }
    
    static bool hitTest(int x, int y, int width, int height) {
      if(Events.touchX > x && Events.touchX < (x + width)) {
        if(Events.touchY > y && Events.touchY < (y + height)) {
          return true;
        }
      }
      return false;
    }
    
    void draw(int x, int y, int width, int height) {
      this->x = x;
      this->y = y;
      this->height = height;
      this->width = width;
      Button::drawButton(x, y, width, height);
    }
    
    static void drawButton(int x, int y, int width, int height) {
      int r = min(width, height) >> 3;
    
      Display.fillRoundRect(x, y, width, height, r, Display.color565(80, 160, 240));
      for(byte i=0; i < 3; i++) {
        Display.drawRoundRect(x + i, y + i, width - i - i, height - i - i, r, Display.color565(80, 160 - i*8, 240));
      }
    }
    
    static void drawButton(int x, int y, int width, int height, const char *pFlashStr1, const char *pFlashStr2) {
      drawButton(x, y, width, height);
      
      byte yd = pFlashStr1 && pFlashStr2 ? 3 : 2;
      
      int tx = x + (width / 2) - (strlen_P(pFlashStr1) * TEXTWIDTH * 3 / 2);
      int ty = y + (height / yd) - (TEXTHEIGHT * 3 / 2); 
      
      Display.displayText_f(tx, ty, 3, WHITE, Display.color565(80, 160, 240), pFlashStr1);
      
      if(pFlashStr2) {
        tx = x + (width / 2) - (strlen_P(pFlashStr2) * TEXTWIDTH * 3 / 2);
        ty = y + 2 * (height / yd) - (TEXTHEIGHT * 3 / 2); 
        Display.displayText_f(tx, ty, 3, WHITE, Display.color565(80, 160, 240), pFlashStr2);
      }
    }
};

class TempGauge {
  public:
    
  public:
    TempGauge() {
    }
    
    void draw(byte x, byte y, byte value) {
      Display.fillCircle(x + 3, y, 3, WHITE);
      Display.fillRect(x, y , 7, 40, WHITE);
      Display.fillCircle(x + 3, y + 46, 8, WHITE);
      
      uint16_t color = value > 20 ? RED : BLUE;
      
      Display.fillRect(x + 2, y + 2, 3, 40, color);
      Display.fillCircle(x + 3, y + 46, 6, color);
      
      Display.fillRect(x + 2, y, 3, map(value, -20, 40, 0, 40), BLACK);
    }
};

class MainScreen : public Screen {
  public:
    TempGauge tempGauge;
    TempGauge outTempGauge;
    Button    menuButton;
  public:
    MainScreen() {      
    }
    
    void draw() {

      if (bInvalidate) {
        Display.displayText_f(0, 0, 2, YELLOW, BACKCOLOR, PSTR("App Test"));
        menuButton.draw(5, 170, 60, 60);
      }
      
      char buffer[9]= { "00:00:00" };  
      if (bInvalidate || Events.bT1000MS) {
        itochars(rtc.getHours(), &buffer[0], 2);
        itochars(rtc.getMinutes(), &buffer[3], 2);
        itochars(rtc.getSeconds(), &buffer[6], 2);  
        
        Display.displayText(5, 30, 2, buffer, GREEN, BACKCOLOR);
      }
      
      if (bInvalidate || ShtMeasure.bReady) {        
        itochars(ShtMeasure.temperature, buffer, 2);
        strcpy(&buffer[2], " C");
        Display.displayText(25, 60, 2, buffer, RED, BACKCOLOR);
        
        itochars(ShtMeasure.humidity, buffer, 2);
        strcpy(&buffer[2], " %");
        Display.displayText(25, 80, 2, buffer, RED, BACKCOLOR);
        
        tempGauge.draw(10, 60, ShtMeasure.temperature);
      }
      
      if (bInvalidate || DS1821.bReady) {        
        itochars(DS1821.temperature, buffer, 2);
        strcpy(&buffer[2], " C");
        Display.displayText(105, 60, 2, buffer, RED, BACKCOLOR);
        
        outTempGauge.draw(90, 60, DS1821.temperature);
      }
      
      bInvalidate = false;
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }
      
      draw();
  
      if(Events.bOnTouch && menuButton.hitTest(Events.touchX, Events.touchY)) {
        hide();
        return ST_MAIN_MENU;
      }
  
      if (input == KEY_ENTER) {
        //hide();
        //return ST_MAIN_MENU;
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
              Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, WHITE);  // draw menu title
              Display.displayText_f(4, y, 2, BACKCOLOR, WHITE, menuText);              
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

//MenuScreen MenuScreen;

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
      
      Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, WHITE);  // draw menu title
      Display.displayText_f(4, 2, 2, BACKCOLOR, WHITE, PSTR("SET TIME"));
      
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
      
      Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, WHITE);  // draw menu title
      Display.displayText_f(4, 2, 2, BACKCOLOR, WHITE, PSTR("SET Date"));
      
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
      
      Display.fillRect(0, 0, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, WHITE);  // draw menu title
      Display.displayText_f(4, 2, 2, BACKCOLOR, WHITE, PSTR("Set Logging"));
      
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
    byte selected;
    InTemperatureChartDiagram inChart;  	// in temperature
    OutTemperatureChartDiagram outChart;   // out temperature

  public:
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }  
      
      if (bInvalidate) {
        if (selected == 0) {                
          inChart.drawTempChart(input);
        } else if (selected == 1) {
          outChart.drawTempChart(input);
        }        
        bInvalidate = false;
      }
      
      if (input == KEY_ENTER) {
        hide();
        return ST_MAIN_MENU;
      } else if (input == KEY_PLUS) {
        selected++;
        selected %= 2;
        Display.clearDisplay();
        bInvalidate = true;
      }
        
      return StateMachine.state;
    }
};

TempChartScreen TempChartScreen;

class TileMenu : Screen {
  public:
    void draw() {
      if (!bInvalidate) {
        return;
      }
      
      Button::drawButton(  0,   0, 118, 118, PSTR("Date"), PSTR("Time"));
      Button::drawButton(120,   0, 118, 118, PSTR("Log"),  NULL);
      Button::drawButton(  0, 120, 118, 118, PSTR("Temp"), NULL);
      Button::drawButton(120, 120, 118, 118, PSTR("Humi"), NULL);
      Button::drawButton(  0, 240, 240,  80, PSTR("Exit"), NULL);
      
      bInvalidate = false;
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      } 
      
      draw();
      
      if(Button::hitTest(0, 240, 240,  80)) {
        hide();
        return ST_MAIN;
      }
    }  
};

TileMenu MenuScreen1;

class Editor : Screen {
  public:
    unsigned bInvalidateValue:1;
    unsigned bDot:1;
    char str[12];
    byte pos;
    
  public:
    
    void drawEditor() {
      Display.drawFastHLine(0,   0, ST7735_TFTWIDTH, WHITE);
      Display.drawFastHLine(0,  64, ST7735_TFTWIDTH, WHITE);
      Display.drawFastHLine(0, 128, ST7735_TFTWIDTH, WHITE);
      Display.drawFastHLine(0, 192, ST7735_TFTWIDTH, WHITE);
      Display.drawFastHLine(0, 256, ST7735_TFTWIDTH - 62, WHITE);
      Display.drawFastHLine(0, 319, ST7735_TFTWIDTH, WHITE);
      
      Display.drawFastVLine(  0,  0, ST7735_TFTHEIGHT, WHITE);
      Display.drawFastVLine( 60, 64, ST7735_TFTHEIGHT, WHITE);
      Display.drawFastVLine(120, 64, ST7735_TFTHEIGHT, WHITE);
      Display.drawFastVLine(180, 64, ST7735_TFTHEIGHT, WHITE);
      Display.drawFastVLine(239,  0, ST7735_TFTHEIGHT, WHITE);
      
      Display.displayText_f(22,  85, 3, WHITE, BLACK, PSTR("7"));
      Display.displayText_f(22, 149, 3, WHITE, BLACK, PSTR("4"));
      Display.displayText_f(22, 213, 3, WHITE, BLACK, PSTR("1"));
      Display.displayText_f(22, 277, 3, WHITE, BLACK, PSTR("0"));
      
      Display.displayText_f(82,  85, 3, WHITE, BLACK, PSTR("8"));
      Display.displayText_f(82, 149, 3, WHITE, BLACK, PSTR("5"));
      Display.displayText_f(82, 213, 3, WHITE, BLACK, PSTR("2"));
      Display.displayText_f(82, 277, 3, WHITE, BLACK, PSTR("."));
      
      Display.displayText_f(142,  85, 3, WHITE, BLACK, PSTR("9"));
      Display.displayText_f(142, 149, 3, WHITE, BLACK, PSTR("6"));
      Display.displayText_f(142, 213, 3, WHITE, BLACK, PSTR("3"));
      Display.displayText_f(142, 277, 3, WHITE, BLACK, PSTR("<"));
      
      Display.displayText_f(204,  85, 3, WHITE, BLACK, PSTR("-"));
      Display.displayText_f(204, 149, 3, WHITE, BLACK, PSTR("+"));
      Display.displayText_f(206, 211, 2, WHITE, BLACK, PSTR("E"));
      Display.displayText_f(206, 229, 2, WHITE, BLACK, PSTR("N"));
      Display.displayText_f(206, 247, 2, WHITE, BLACK, PSTR("T"));
      Display.displayText_f(206, 265, 2, WHITE, BLACK, PSTR("E"));
      Display.displayText_f(206, 283, 2, WHITE, BLACK, PSTR("R"));      
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        for(byte i = 0; i < 12; i++)
          str[i] = 0;
        pos = 0;
        bDot = false;
        show();
      } 
      
      if (bInvalidate) {
        drawEditor();
        bInvalidate = false;
      }
      
      if(Events.bOnTouch) {
        int x = Events.touchX / 60;
        int y = Events.touchY / 64 - 1;
        
        int v = x + 7 - y * 3;

        if(v > 0 && v <= 9) {
          str[pos++] = '0' + v;
        } else if(v == -2) {
          str[pos++] = '0';
        } else if(!bDot && v == -1) {
          str[pos++] = '.';
          bDot = true;
        } else if(v == 0) {
          if(pos > 0) pos--;
          if(str[pos] == '.') bDot = false;
          str[pos] = 0;
        }
        
        pos = pos % 11;
        
        Display.displayText_f(18, 21, 3, WHITE, BLACK, PSTR("            "));
        Display.displayText(18, 21, 3, str, WHITE, BLACK);
      }
      
      if(Button::hitTest(180, 192, 60,  128)) {
        hide();
        return ST_MAIN;
      }
    }
};

Editor MenuScreen;
#endif
