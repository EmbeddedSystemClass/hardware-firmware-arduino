
class Clock {
  public:
    byte px;
    byte py;
    byte size;
  
  public:
    void draw() {
      byte p = rtc.getSeconds();
      byte pp = p > 0 ? p - 1 : 0;
      
      drawPointer(pp, ST7735_BLACK, size);      
      drawPointer(p, ST7735_WHITE, size);
      
      p = rtc.getMinutes();
      byte pp = p > 0 ? p - 1 : 0;
      
      drawPointer(pp, ST7735_BLACK, size);      
      drawPointer(p, ST7735_WHITE, size);
      
      p = rtc.getHours();
      byte pp = p > 0 ? p - 1 : 0;
      
      drawPointer(pp * 5, ST7735_BLACK, size);      
      drawPointer(p * 5, ST7735_WHITE, size);
      
    }
  
    void drawPointer(byte value, uint_16 color, byte s) {
      byte p = (value + 45) % 60;
      
      byte x = sin_table[(p + 15) % 60] / ACCURACY * s;
      byte y = sin_table[p] / ACCURACY * s;
      
      Display.drawLine(px, py, x, y, color);
    }

    void drawScale() {
      for (byte i = 0; i < 60; i+=5) {
        byte x1 = sin_table[(i + 15) % 60] / ACCURACY * (size + 5) + px;
        byte y1 = sin_table[i] / ACCURACY * (size + 5) + py;
        
        byte x2 = sin_table[(i + 15) % 60] / ACCURACY * (size + 10) + px;
        byte y2 = sin_table[i] / ACCURACY * (size + 10) + py;
        
        Display.drawLine(x1, y1, x2, y2, ST7735_WHITE);
      }
    }
};