#ifndef _SCREENH_
#define _SCREENH_

/*
  Screens

 This example code is in the public domain.
 */

#define BACKCOLOR BLACK
#define DEGREE_CHAR 127

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
    
    virtual byte dispatch(byte input) {
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
    int8_t lastValue;
    uint16_t color;
    unsigned bInvalidateAll:1;
    unsigned bOutOfRange:1;
  
  public:
    TempGauge() {        
        bInvalidateAll = true;
        bOutOfRange = false;
        color = BACKCOLOR;
    }
    
    void draw(byte x, byte y, int8_t value) {      
      bOutOfRange = false;
      
      // clamp value to range
      if(value < -30) {
        value = -30;
        bOutOfRange = true;
      } else if(value > 110) {
        value = 110;
        bOutOfRange = true;
      }
      
      int8_t delta = abs(lastValue - value);
            
      if(bOutOfRange) {
         Display.displayText(x + 25, y, 3, "Err", RED, BACKCOLOR);
      } else if (bInvalidateAll || delta != 0) {
        lastValue = value;
        char buffer[5] = {'+', 0, 0, DEGREE_CHAR, 0 };      
        bin2asc(abs(value), &buffer[1], 2); 
        buffer[0] = value < 0 ? '-' : '+';
        Display.displayText(x + 25, y, 3, buffer, RED, BACKCOLOR);
      } else {
        return;
      }
      
      uint16_t newColor = value > 20 ? RED : BLUE;            
      
      if (bInvalidateAll || delta > 2 || color != newColor) {        
        // draw border
        if(bInvalidateAll) {          
          Display.fillCircle(x + 6, y + 2, 6, WHITE);
          Display.fillRect(x, y , 13, 100, WHITE);
          Display.fillCircle(x + 6, y + 106, 12, WHITE);
        }      
        
        // map range : -30°C - 60°C to pixel height 100 - 0px
        if(value > 60) value = 60;
        uint8_t h = map(value, -30, 60, 100, 0); 
        
        // draw temperature
        Display.fillRect(x + 2, y + h, 9, 100 - h, newColor);
        if(bInvalidateAll || color != newColor) {
          color = newColor;
          Display.fillCircle(x + 6, y + 106, 10, newColor);
        }
        
        // draw blank        
        Display.fillRect(x + 2, y, 9, h, BLACK);
      }
      
      bInvalidateAll = false;
    }
    
    void invalidate() {
      lastValue = -127;
      bInvalidateAll = true;
    }
};

class Statistic {
  public:
    int8_t min;
    int8_t max;
    
  public:
    Statistic() {               
    }
    
    void draw(byte x, byte y, int8_t values[]) {      
      if(LogData.count > 0) {
        int8_t newMin;
        int8_t newMax;
        int8_t newAvg;
              
        LogData.getStat(values, LogData.count, &newMin, &newMax, &newAvg);
        
        char buffer[9]= { 0 };  
        if(newMin != min) {
          min = newMin;
          strcpy_P(buffer, PSTR("Min:"));
          bin2asc(min, &buffer[4], 2);
          buffer[6] = DEGREE_CHAR;
          buffer[7] = 0;          
          Display.displayText(x, y, 1, buffer, LIGHTGRAY, BACKCOLOR);
        }
        if(newMax != max) {
          max = newMax;
          strcpy_P(buffer, PSTR("Max:"));
          bin2asc(max, &buffer[4], 2);
          buffer[6] = DEGREE_CHAR;
          buffer[7] = 0;
          Display.displayText(x, y + 20, 1, buffer, LIGHTGRAY, BACKCOLOR);
        }
      }
    }    
};

class Statusbar {
  public:
    unsigned bInvalidate:1;
	
  public:
    Statusbar() {
      bInvalidate = true;
    }
    
    void draw(byte x, byte y, byte width, byte height) {
      if(bInvalidate) {        
        Display.drawRoundRect(x, y, 10, 10, 2, WHITE);
        if(LogData.bLog2SdEnabled)
          Display.displayText_f(x + 12, y, 1, LIGHTGRAY, BACKCOLOR, PSTR("SD Enabled"));
        else
          Display.displayText_f(x + 12, y, 1, LIGHTGRAY, BACKCOLOR, PSTR("SD Disabled"));
      }			
      bInvalidate = false;
    }    
};

class MainScreen : public Screen {
  public:
    TempGauge tempGauge1;
    TempGauge tempGauge2;	
    Statistic	statistic1;   
    Statusbar statusbar;
  
  public:
    MainScreen() {      
    }
    
    void draw() {
      if (bInvalidate) {
        Button::drawButton(0, 0, 240, 50, PSTR("Logger"), NULL);        
      }      
        
      if (bInvalidate || Events.bT1000MS) {
        char buffer[9]= { "00:00:00" };
        DateTime dt = RTC.now();
        bin2asc(dt.hour, &buffer[0], 2);
        bin2asc(dt.minute, &buffer[3], 2);
        bin2asc(dt.second, &buffer[6], 2);  
        
        Display.displayText(70, 70, 2, buffer, GREEN, BACKCOLOR);
      }
      
      if (bInvalidate || Measure.bReady) {        
        if(bInvalidate) {
          tempGauge1.invalidate();
          tempGauge2.invalidate();
        }
        
        tempGauge1.draw(25, 120, Measure.temperature);
        tempGauge2.draw(135, 120, Measure.temperature2);         
        statistic1.draw(58, 160, LogData.temperature1Log);
      }
      
      if (bInvalidate) {
        statusbar.draw(0, 270, 240, 50);
      }
      
      bInvalidate = false;
    }
    
    byte dispatch(byte input) {
      if (!bVisible) {
        show();
      }
      
      draw();
  
      if(Button::hitTest(0, 0, 240, 80)) {
        hide();
        tempGauge1.invalidate();
        tempGauge2.invalidate();
        statusbar.bInvalidate = true;
        pScreen = pMenuScreen;
      }
      
      return true;
    }
};

MainScreen MainScreen;

class TempChartScreen : public Screen {
  public:    
    Chart chart;
    byte chartIndex;
    
  public:
    byte dispatch(byte input) {
      if (!bVisible) {
        show();
      }
      
      if(LogEvents.bLog) {
        bInvalidate = LogEvents.bLog;
        Display.clearDisplay();
      }
      
      if(Button::hitTest(0, 240, 240,  80)) {
        hide();        
        pScreen = pMenuScreen;
      } else if(Button::hitTest(0, 0, 240, 50)) {
        chartIndex = (chartIndex + 1) % 2;
        Display.clearDisplay();
        bInvalidate = true;
      }
      
      if (bInvalidate) {             
        Button::drawButton(0, 270, 240,  50, PSTR("Exit"), NULL);
        if(chartIndex == 0) {
          Button::drawButton(0,   0, 240, 50, PSTR("Temperature In"), NULL); 
          chart.drawChart(LogData.temperature1Log, LOG_DATA_SIZE, -120, 120);
        } else {
          Button::drawButton(0,   0, 240, 50, PSTR("Temperature Out"), NULL); 
          chart.drawChart(LogData.temperature2Log, LOG_DATA_SIZE, -120, 120);
        }
        bInvalidate = false;
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
      Button::drawButton(120, 120, 118, 118, PSTR("Chart"), NULL);
      Button::drawButton(  0, 240, 240,  80, PSTR("Exit"), NULL);
      
      bInvalidate = false;
    }
    
    byte dispatch(byte input) {
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
    
    byte dispatch(byte input) {      
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
      DateTime dt = RTC.now();
      dt.year = CHARTONUM(str[6], 1000) + CHARTONUM(str[7], 100) + CHARTONUM(str[8], 10) + CHARTONUM(str[9], 1);
      dt.month = CHARTONUM(str[3], 10) + CHARTONUM(str[4], 1);
      dt.day = CHARTONUM(str[0], 10) + CHARTONUM(str[1], 1);
      //Serial.print(y);Serial.print(".");Serial.print(m);Serial.print(".");Serial.println(d);
      RTC.adjust(dt);
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
      DateTime dt = RTC.now();
      dt.hour = CHARTONUM(str[0], 10) + CHARTONUM(str[1], 1);
      dt.minute = CHARTONUM(str[3], 10) + CHARTONUM(str[4], 1);
      dt.second = 0;
      RTC.adjust(dt);
    }
};

TimeEditor TimeEditor;

class LogSettingsScreen : public Screen {
  public:
    byte dispatch(byte input) {
      if (!bVisible) {
        show();
      }  
      
      if (bInvalidate) {
        Button::drawButton(0,   0, 240, 50, PSTR("Logging"), NULL);
        Button::drawButton(0,  52, 240, 50, PSTR("Reset"),   NULL);
        if(LogData.bLog2SdEnabled) {
          Button::drawButton(0, 104, 240, 50, PSTR("SD Enabled"), NULL);
        } else {
          Button::drawButton(0, 104, 240, 50, PSTR("SD Disabled"), NULL);
        }
        Button::drawButton(0, 270, 240, 50, PSTR("Exit"),    NULL);                     
        bInvalidate = false;
      }
      
      if(Button::hitTest(0, 240, 240,  80)) {  // Exit
        hide();        
        pScreen = pMenuScreen;
      } else if(Button::hitTest(0, 52, 240, 50)) {  // Reset
        hide();
        pScreen = pMenuScreen;
        
        LogData.reset(LogData.temperature1Log);
        LogData.reset(LogData.temperature2Log);
        LogEvents.reset();
      } else if(Button::hitTest(0, 104, 240, 50)) {
        LogData.bLog2SdEnabled = !LogData.bLog2SdEnabled;
        bInvalidate = true;
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
