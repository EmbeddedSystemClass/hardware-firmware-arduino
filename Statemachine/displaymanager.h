// Display Nokia 5110 ****************************************************

#define TEXTHEIGHT 8
#define TEXTWIDTH  6
#define BLACK      ST7735_WHITE
#define WHITE      ST7735_BLACK

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160


class DisplayManager : public Adafruit_ST7735 {

    // pin 7 - Serial clock out (SCLK)
    // pin 6 - Serial data out (DIN)
    // pin 5 - Data/Command select (D/C)
    // pin 4 - LCD chip select (CS)
    // pin 3 - LCD reset (RST)
  
  public:    
  
    DisplayManager() : Adafruit_ST7735(10, 9, 8) {
    }
  
    void beginDisplay() {
       // Initialize Display
      initR(INITR_BLACKTAB);
      setRotation(0);
      clearDisplay();   // clears the screen and buffer
    }
    
    void clearDisplay() {
      fillScreen(ST7735_BLACK);
    }
    
    void display() {
    };
    
    void displayText_f(byte x, byte y, byte fontSize, const char *pFlashStr) {
      displayText_f(x, y, fontSize, BLACK, WHITE, pFlashStr);
    }
    
    void displayText_f(byte x, byte y, byte fontSize, uint16_t textColor, uint16_t backColor, const char *pFlashStr) {
      setTextWrap(false);
      setTextColor(textColor);
      setTextSize(fontSize);
      
      for (byte i = 0; (const char)(pgm_read_byte(&pFlashStr[i])) && i < 40; i++) {
        setCursor(x + i * 6 * fontSize, y);
        char c = pgm_read_byte(&pFlashStr[i]);
        if (c == '\0') break;
        write(pgm_read_byte(&pFlashStr[i]));
      }
    }
    
    void displayText(byte x, byte y, byte fontSize, char str[])
    {
      displayText(x, y, fontSize, str, BLACK, WHITE);
    }
    
    void displayText(byte x, byte y, byte fontSize, char str[], uint16_t textColor, uint16_t backColor)
    {
      setTextColor(textColor, backColor);
      setTextSize(fontSize);
      setCursor(x, y);
      println(str);	
    }
};

DisplayManager Display;
