/*
class Clock {
  public:
    byte px;
    byte py;
    byte size;
    byte last;
  public:
    Clock() {
      px = 100;
      py = 80;
      size = 40;
    }
  
    void draw() {
      byte p = rtc.getSeconds();
      

      drawPointer(last, ST7735_BLACK, size);      
      drawPointer(p, ST7735_WHITE, size);
      
      
    }
  
    void drawPointer(byte value, uint16_t color, byte s) {
      byte p = (value + 45) % 60;
      
      int8_t x = cos(p * 6 * PI / 180) * s;
      int8_t y = sin(p * 6 * PI / 180) * s;
      
      Display.drawLine(px, py, px + x, py + y, color);
    }

    void drawScale() {
      for (byte i = 0; i < 60; i+=5) {
        int8_t x1 = cos(i * PI / 180) * (size + 5) + px;
        int8_t y1 = sin(i * PI / 180) * (size + 5) + py;
        
        int8_t x2 = cos(i * PI / 180) * (size + 10) + px;
        int8_t y2 = sin(i * PI / 180) * (size + 10) + py;
        
        Display.drawLine(x1, y1, x2, y2, ST7735_WHITE);
      }
    }
};

Clock Clock;

*/
