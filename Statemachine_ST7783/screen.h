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

class NumberEditor : Screen {
  public:
    unsigned bInvalidateValue:1;
    unsigned bDot:1;
    char str[12];
    byte pos;
    byte maxPos;
    
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
    
    virtual void intialize() {
      for(byte i = 0; i < 12; i++)
          str[i] = 0;
      pos = 0;
      maxPos = 11;
      bDot = false;
    }
    
    virtual byte onNumberButton(byte number) {
      str[pos++] = '0' + number;
      return true;
    }
    
    virtual byte onDotButton() {
      if(bDot)
        return;
      str[pos++] = '.';
      bDot = true;
      return true;
    }
    
    virtual byte onBackSpaceButton() {
      if(pos > 0) pos--;
      if(str[pos] == '.') bDot = false;
      str[pos] = 0;
      return true;      
    }
    
    byte execute(byte input) {      
      if(Button::hitTest(180, 192, 60,  128)) {   // Exit pressed?
        hide();
        return ST_MAIN;
      }
      
      if (!bVisible) {
        intialize();
        show();
      }       
      
      if(Events.bOnTouch) {
        // Number Matrix test
        int x = Events.touchX / 60;
        int y = Events.touchY / 64 - 1;
        
        int n = x + 7 - y * 3;

        if(n > 0 && n <= 9) {
          bInvalidateValue = onNumberButton(n);
        } else if(n == -2) {
          bInvalidateValue = onNumberButton(0);
        } else if(n == -1) {
          bInvalidateValue = onDotButton();
        } else if(n == 0) {
          bInvalidateValue = onBackSpaceButton();
        }
        
        pos = pos % maxPos;        
      }
      
      if (bInvalidate) {
        drawEditor();
        bInvalidate = false;
        bInvalidateValue = true;
      }
      
      if(bInvalidateValue) {
        Display.displayText_f(18, 21, 3, WHITE, BLACK, PSTR("            "));
        Display.displayText(18, 21, 3, str, WHITE, BLACK);
        bInvalidateValue = false;
      }      
    }
};

NumberEditor MenuScreen;

class DateEditor : NumberEditor {
  public:
    void intialize() {
      str = { "__.__.____" };
      pos = 0;
      maxPos = 10;
      bDot = false;
    }
    
    byte onDotButton() {
      pos++;
    }
    
    byte onBackSpaceButton() {
      if(pos > 0) pos--;
      if(str[pos] == '.') bDot = false;
      str[pos] = '_';
      return true;      
    }
};

#endif
