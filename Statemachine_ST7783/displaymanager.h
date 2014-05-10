#ifndef _DISPLAYMANAGERH_
#define _DISPLAYMANAGERH_

// Display Nokia 5110 ****************************************************

#define TEXTHEIGHT 8
#define TEXTWIDTH  6
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define ST7735_TFTWIDTH  320
#define ST7735_TFTHEIGHT 240


class DisplayManager : public SWTFT {  
  public:    
    // CS  PIN 10
    // DC  PIN  9
    // RST PIN  8
    
    // (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
    // an output. This is much faster - also required if you want
    // to use the microSD card (see the image drawing example)
    DisplayManager() : SWTFT() {
    }
  
    void beginDisplay() {
       // Initialize Display
      reset();
      uint16_t identifier = readID();
      begin(identifier);
    }
    
    void clearDisplay() {
      fillScreen(BLACK);
    }
    
    void display() {
    };
    
    void displayText_f(int x, int y, int fontSize, const char *pFlashStr) {
      displayText_f(x, y, fontSize, BLACK, WHITE, pFlashStr);
    }
    
    void displayText_f(int x, int y, int fontSize, uint16_t textColor, uint16_t backColor, const char *pFlashStr) {
      setTextWrap(false);
      setTextColor(textColor, backColor);
      setTextSize(fontSize);
      
      for (byte i = 0; (const char)(pgm_read_byte(&pFlashStr[i])) && i < 40; i++) {
        setCursor(x + i * 6 * fontSize, y);
        char c = pgm_read_byte(&pFlashStr[i]);
        if (c == '\0') break;
        write(pgm_read_byte(&pFlashStr[i]));
      }
    }
    
    void displayText(int x, int y, int fontSize, char str[])
    {
      displayText(x, y, fontSize, str, BLACK, WHITE);
    }
    
    void displayText(int x, int y, int fontSize, char str[], uint16_t textColor, uint16_t backColor)
    {
      setTextColor(textColor, backColor);
      setTextSize(fontSize);
      setCursor(x, y);
      println(str);	
    }
};

DisplayManager Display;

#endif
