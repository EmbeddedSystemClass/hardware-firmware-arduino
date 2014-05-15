#ifndef _DISPLAYMANAGERH_
#define _DISPLAYMANAGERH_

// Display Driver ST7783 ****************************************************

#define TFTWIDTH  240
#define TFTHEIGHT 320

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
#define LIGHTGRAY 0xCDB6

class DisplayManager : public SWTFT {  
  public:        
    DisplayManager() : SWTFT() {
    }
  
    void beginDisplay() {
       // Initialize Display
      reset();
      uint16_t identifier = readID();
      begin(identifier);
      setRotation(0);
      clearDisplay();
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
      setCursor(x, y);
      for (byte i = 0; (const char)(pgm_read_byte(&pFlashStr[i])) && i < 40; i++) {
        //setCursor(x + i * 6 * fontSize, y);
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
    
    void displayText(int x, int y, int fontSize, char c, uint16_t textColor, uint16_t backColor)
    {
      setTextColor(textColor, backColor);
      setTextSize(fontSize);
      setCursor(x, y);
      write(c);	
    }
};

DisplayManager Display;

#endif
