// Display Nokia 5110 ****************************************************

#define TEXTHEIGHT 8
#define TEXTWIDTH  6

class DisplayManager : public Adafruit_PCD8544 {

    // pin 7 - Serial clock out (SCLK)
    // pin 6 - Serial data out (DIN)
    // pin 5 - Data/Command select (D/C)
    // pin 4 - LCD chip select (CS)
    // pin 3 - LCD reset (RST)
  
  public:    
  
    DisplayManager() : Adafruit_PCD8544(7, 6, 5, 4, 3) {
    }
  
    void beginDisplay() {
       // Initialize Display
      begin();
      // you can change the contrast around to adapt the display
      // for the best viewing!
      setContrast(60);
      clearDisplay();   // clears the screen and buffer
    }
    
    void displayText_f(byte x, byte y, byte fontSize, const char *pFlashStr) {
      displayText_f(x, y, fontSize, BLACK, WHITE, pFlashStr);
    }
    
    void displayText_f(byte x, byte y, byte fontSize, byte textColor, byte backColor, const char *pFlashStr) {
      setTextColor(textColor, backColor);
      setTextSize(fontSize);
      
      for (byte i = 0; (const char)(pgm_read_byte(&pFlashStr[i])) && i < 40; i++) {
        setCursor(x + i * 6, y);
        char c = pgm_read_byte(&pFlashStr[i]);
        if (c == '\0') break;
        write(pgm_read_byte(&pFlashStr[i]));
      }
    }
    
    void displayText(byte x, byte y, byte fontSize, char str[])
    {
      displayText(x, y, fontSize, str, BLACK, WHITE);
    }
    
    void displayText(byte x, byte y, byte fontSize, char str[], byte textColor, byte backColor)
    {
      setTextColor(textColor, backColor);
      setTextSize(fontSize);
      setCursor(x, y);
      println(str);	
    }
};

DisplayManager Display;
