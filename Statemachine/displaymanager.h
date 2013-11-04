//#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>

// Display Nokia 5110 ****************************************************

#define TEXTHEIGHT 8
#define TEXTWIDTH  6

//Adafruit_PCD8544 adaDisplay = Adafruit_PCD8544(7, 6, 5, 4, 3);

class DisplayManager : public Adafruit_PCD8544 {
//  private:
    // pin 7 - Serial clock out (SCLK)
    // pin 6 - Serial data out (DIN)
    // pin 5 - Data/Command select (D/C)
    // pin 4 - LCD chip select (CS)
    // pin 3 - LCD reset (RST)
//    Adafruit_PCD8544 adaDisplay = Adafruit_PCD8544(7, 6, 5, 4, 3);
  
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
  
//    void clearDisplay() {
//      adaDisplay.clearDisplay();
//    }
//    
//    void display() {
//      adaDisplay.display();
//    }
//    
//    void setTextColor(byte textColor, byte backColor) {
//      adaDisplay.setTextColor(textColor, backColor);
//    }    
//    
//    void setTextSize(byte textSize) {
//      adaDisplay.setTextSize(textSize);
//    }
//    
//    void setCursor(byte x, byte y) {
//      adaDisplay.setCursor(x, y);
//    }
//    
//    void println(char* buffer) {
//      adaDisplay.println(buffer);
//    }
//    
//    void write(char c) {
//      adaDisplay.write(c);
//    }
//    
//    void drawRect(byte x, byte y, byte width, byte hight, byte color) {
//      adaDisplay.drawRect(x, y, width, hight, color);
//    }
    
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
