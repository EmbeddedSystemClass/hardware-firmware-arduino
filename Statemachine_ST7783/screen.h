#ifndef _SCREENH_
#define _SCREENH_

#define BACKCOLOR BLACK
#define DEGREE_CHAR 127

class Screen {
  public:
    unsigned bInvalidate:1;
    unsigned bVisible:1;   

    //static Screen* pCurrent;
    //static Screen* pDateEditor;

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

static Screen* pScreen;
static Screen* pMainScreen;
static Screen* pDateEditScreen;
static Screen* pTimeEditScreen;
static Screen* pTempChartScreen;
static Screen* pLogSettingsScreen;
static Screen* pMenuScreen;


class Button {
  public:    
    static bool hitTest(int x, int y, int width, int height) {
      if(Events.bOnTouch && Events.touchX > x && Events.touchX < (x + width)) {
        if(Events.touchY > y && Events.touchY < (y + height)) {
          return true;
        }
      }
      return false;
    }
    
    static void drawButton(int x, int y, int width, int height) {
      int r = min(width, height) >> 3;
    
      Display.drawRoundRect(x, y, width, height, r, WHITE);
      //for(byte i=0; i < 3; i++) {
      //  Display.drawRoundRect(x + i, y + i, width - i - i, height - i - i, r, Display.color565(80, 160 - i*8, 240));
      //}
    }
    
    static void drawButton(int x, int y, int width, int height, const char *pFlashStr1, const char *pFlashStr2) {
      drawButton(x, y, width, height);
      
      byte yd = pFlashStr1 && pFlashStr2 ? 3 : 2;
      
      int tx = x + (width / 2) - (strlen_P(pFlashStr1) * TEXTWIDTH * 3 / 2);
      int ty = y + (height / yd) - (TEXTHEIGHT * 3 / 2); 
      
      Display.displayText_f(tx, ty, 3, WHITE, BLACK, pFlashStr1);
      
      if(pFlashStr2) {
        tx = x + (width / 2) - (strlen_P(pFlashStr2) * TEXTWIDTH * 3 / 2);
        ty = y + 2 * (height / yd) - (TEXTHEIGHT * 3 / 2); 
        Display.displayText_f(tx, ty, 3, WHITE, BLACK, pFlashStr2);
      }
    }
};

class TempGauge {
  public:
    
  public:
    TempGauge() {
    }
    
    void draw(byte x, byte y, byte value) {
      Display.fillCircle(x + 6, y + 2, 6, WHITE);
      Display.fillRect(x, y , 13, 100, WHITE);
      Display.fillCircle(x + 6, y + 106, 12, WHITE);
      
      uint16_t color = value > 20 ? RED : BLUE;
      
      Display.fillRect(x + 2, y + 2, 9, 100, color);
      Display.fillCircle(x + 6, y + 106, 10, color);
      
      Display.fillRect(x + 2, y, 9, map(value, -20, 100, 0, 100), BLACK);
    }
};

class MainScreen : public Screen {
  public:
    TempGauge outTempGauge;
 
  public:
    MainScreen() {      
    }
    
    void draw() {

      if (bInvalidate) {
        Button::drawButton(0,   0, 240, 50, PSTR("Logger"), NULL);
        Button::drawButton(0, 270, 240, 50, PSTR("Menu"), NULL);
      }
      
      char buffer[9]= { "00:00:00" };  
      if (bInvalidate || Events.bT1000MS) {
        itochars(rtc.getHours(), &buffer[0], 2);
        itochars(rtc.getMinutes(), &buffer[3], 2);
        itochars(rtc.getSeconds(), &buffer[6], 2);  
        
        Display.displayText(70, 70, 2, buffer, GREEN, BACKCOLOR);
      }
      
      if (bInvalidate || DS1821.bReady) {        
        itochars(DS1821.temperature, buffer, 2);
        buffer[2] = DEGREE_CHAR;
        buffer[3] = 0;
        Display.displayText(98, 120, 4, buffer, RED, BACKCOLOR);
        outTempGauge.draw(65, 120, DS1821.temperature);
        
        if(LogData.count > 0) {
          int8_t min = 120;
          int8_t max = -120;
          int8_t avg = 0;
          LogData.getStat(LogData.logOutTemperature, LogData.count, &min, &max, &avg);
          strcpy_P(buffer, PSTR("Min:"));
          itochars(min, &buffer[4], 2);
          buffer[6] = DEGREE_CHAR;
          buffer[7] = 0;          
          Display.displayText(98, 160, 2, buffer, LIGHTGRAY, BACKCOLOR);
          
          strcpy_P(buffer, PSTR("Max:"));
          itochars(max, &buffer[4], 2);
          buffer[6] = DEGREE_CHAR;
          buffer[7] = 0;
          Display.displayText(98, 180, 2, buffer, LIGHTGRAY, BACKCOLOR);
          
          strcpy_P(buffer, PSTR("Avg:"));
          itochars(avg, &buffer[4], 2);
          buffer[6] = DEGREE_CHAR;
          buffer[7] = 0;
          Display.displayText(98, 200, 2, buffer, LIGHTGRAY, BACKCOLOR);
        }
      }
      
      bInvalidate = false;
    }
    
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }
      
      draw();
  
      if(Button::hitTest(0, 240, 240,  80)) {
        hide();        
        pScreen = pMenuScreen;
      }
      
      return true;
    }
};

MainScreen MainScreen;

class TempChartScreen : public Screen {
  public:    
    OutTemperatureChartDiagram outChart;  	// temperature    

  public:
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }  
      
      if (bInvalidate) {
        Button::drawButton(0,   0, 240, 50, PSTR("Temprature"), NULL);      
        Button::drawButton(0, 270, 240,  50, PSTR("Exit"), NULL);                     
        outChart.drawTempChart(input);        
        bInvalidate = false;
      }
      
      if(Button::hitTest(0, 240, 240,  80)) {
        hide();        
        pScreen = pMenuScreen;
      }
        
      return 0;      
    }
};

TempChartScreen TempChartScreen;

class TileMenu : public Screen {  
  public:
    void draw() {
      if (!bInvalidate) {
        return;
      }
      
      Button::drawButton(  0,   0, 118, 118, PSTR("Date"), NULL);
      Button::drawButton(120,   0, 118, 118, PSTR("Time"),  NULL);
      Button::drawButton(  0, 120, 118, 118, PSTR("Log"), NULL);
      Button::drawButton(120, 120, 118, 118, PSTR("Temp"), NULL);
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
        pScreen = pMainScreen;
      } else if(Button::hitTest(0, 0, 118, 118)) {
        hide();
        pScreen = pDateEditScreen;        
      } else if(Button::hitTest(120, 0, 118, 118)) {
        hide();
        pScreen = pTimeEditScreen;
      } else if(Button::hitTest(120, 120, 118, 118)) {
        hide();
        pScreen = pTempChartScreen;
      } else if(Button::hitTest(0, 120, 118, 118)) {
        hide();
        pScreen = pLogSettingsScreen;
      }
      
      return true;
    }  
};

TileMenu MenuScreen;

class NumberEditor : public Screen {
  public:
    unsigned bInvalidateValue:1;
    unsigned bDot:1;
    char str[12];
    byte pos;
    byte maxPos;
    
  public:
    
    void drawEditor() {
      Display.drawFastHLine(0,   0, TFTWIDTH, WHITE);
      Display.drawFastHLine(0,  64, TFTWIDTH, WHITE);
      Display.drawFastHLine(0, 128, TFTWIDTH, WHITE);
      Display.drawFastHLine(0, 192, TFTWIDTH, WHITE);
      Display.drawFastHLine(0, 256, TFTWIDTH - 62, WHITE);
      Display.drawFastHLine(0, 319, TFTWIDTH, WHITE);
      
      Display.drawFastVLine(  0,  0, TFTHEIGHT, WHITE);
      Display.drawFastVLine( 60, 64, TFTHEIGHT, WHITE);
      Display.drawFastVLine(120, 64, TFTHEIGHT, WHITE);
      Display.drawFastVLine(180, 64, TFTHEIGHT, WHITE);
      Display.drawFastVLine(239,  0, TFTHEIGHT, WHITE);
      
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
      strcpy_P(str, PSTR("           "));
      pos = 0;
      maxPos = 10;
      bDot = false;
    }
    
    virtual byte onNumberButton(byte number) {
      if(pos > maxPos) return false;
      str[pos++] = '0' + number;
      return true;
    }
    
    virtual byte onDotButton() {
      if(bDot || pos > maxPos) return false;
      str[pos++] = '.';
      bDot = true;
      return true;
    }
    
    virtual byte onBackSpaceButton() {
      if(pos > 0) pos--;
      if(str[pos] == '.') bDot = false;
      str[pos] = ' ';
      return true;      
    }
    
    virtual void onExit() { }
    
    byte execute(byte input) {      
      if (!bVisible) {
        intialize();
        show();
      }       
      
      if(Button::hitTest(180, 192, 60,  128)) {   // Exit pressed?
        onExit();
        hide();
        pScreen = pMenuScreen;
        return 0;
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
      }
      
      if (bInvalidate) {
        drawEditor();
        bInvalidate = false;
        bInvalidateValue = true;
      }
      
      if(bInvalidateValue) {
        Display.displayText(18, 21, 3, str, WHITE, BLACK);
        bInvalidateValue = false;
      }
      
      
    }
};

class MaskEditor : public NumberEditor {
  public:
    virtual void intialize() {
      strcpy_P(str, PSTR("_______.__"));
      pos = 0;
      maxPos = 9;
      bDot = false;
    }
    
    virtual byte onNumberButton(byte number) {
      if(pos > maxPos) return false;
      str[pos++] = '0' + number;      
      if(str[pos] == '.') pos++;      
      return true;
    }
    
    byte onDotButton() {
      return false;
    }
    
    byte onBackSpaceButton() {
      if(pos > 0) pos--;
      if(str[pos] == '.' && pos > 1) pos--;
      str[pos] = '_';
      return true;      
    }
};

class DateEditor : public MaskEditor {
  public:
    
    virtual void intialize() {
      strcpy_P(str, PSTR("__.__.____"));
      pos = 0;
      maxPos = 9;
      bDot = false;
    }
    
    void onExit() {
      int y = CHARTONUM(str[0], 1000) + CHARTONUM(str[1], 100) + CHARTONUM(str[2], 10) + CHARTONUM(str[3], 1);
      byte m = CHARTONUM(str[5], 10) + CHARTONUM(str[6], 1);
      byte d = CHARTONUM(str[8], 10) + CHARTONUM(str[9], 1);
      rtc.setDate(d, m, y);
    }
};

static DateEditor DateEditor;


class TimeEditor : public MaskEditor {
  public:
    virtual void intialize() {
      strcpy_P(str, PSTR("__.__"));
      pos = 0;
      maxPos = 4;
      bDot = false;
    }
    
    void onExit() {
      byte h = CHARTONUM(str[0], 10) + CHARTONUM(str[1], 1);
      byte m = CHARTONUM(str[3], 10) + CHARTONUM(str[4], 1);
      rtc.setTime(h, m, 0);
    }
};

TimeEditor TimeEditor;

class LogSettingsScreen : public Screen {
  
  public:
    byte execute(byte input) {
      if (!bVisible) {
        show();
      }  
      
      if (bInvalidate) {
        Button::drawButton(0,   0, 240, 50, PSTR("Logging"), NULL);
        Button::drawButton(0,  52, 240, 50, PSTR("Reset"), NULL);
        Button::drawButton(0, 270, 240,  50, PSTR("Exit"), NULL);                     
        bInvalidate = false;
      }
      
      if(Button::hitTest(0, 240, 240,  80)) {  // Exit
        hide();        
        pScreen = pMenuScreen;
      } else if(Button::hitTest(0,  52, 240, 50)) {  // Reset
        hide();
        pScreen = pMenuScreen;
        
        LogData.reset(LogData.logOutTemperature);
        LogEvents.reset();
      }
        
      return 0;      
    }
};

LogSettingsScreen LogSettingsScreen;


void initScreens() {
  pScreen = &MainScreen;
  pMainScreen = &MainScreen;
  pDateEditScreen = &DateEditor;
  pTimeEditScreen = &TimeEditor;
  pTempChartScreen = &TempChartScreen;
  pLogSettingsScreen = &LogSettingsScreen;
  pMenuScreen = &MenuScreen;
}

#endif
