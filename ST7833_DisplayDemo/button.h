class Button {
  public:    
    static bool hitTest(int16_t x, int16_t y, int16_t width, int16_t height) {
      if(Events.bOnTouch && Events.touchX > x && Events.touchX < (x + width)) {
        if(Events.touchY > y && Events.touchY < (y + height)) {
          return true;
        }
      }
      return false;
    }
        
    static void drawButton(int16_t x, int16_t y, int16_t width, int16_t height, const char *pFlashStr1, const char *pFlashStr2) {
      int16_t r = min(width, height) >> 3;    
      Display.drawRoundRect(x, y, width, height, r, WHITE);
      drawButtonText(x, y, width, height, pFlashStr1, pFlashStr2);
    }
    
    static void drawButtonText(int16_t x, int16_t y, int16_t width, int16_t height, const char *pFlashStr1, const char *pFlashStr2) {
      uint8_t yd = pFlashStr1 && pFlashStr2 ? 3 : 2;
      
      int16_t tx = x + (width / 2) - (strlen_P(pFlashStr1) * TEXTWIDTH * 3 / 2);
      int16_t ty = y + (height / yd) - (TEXTHEIGHT * 3 / 2); 
      
      Display.displayText_f(tx, ty, 3, WHITE, BLACK, pFlashStr1);
      
      if(pFlashStr2) {
        tx = x + (width / 2) - (strlen_P(pFlashStr2) * TEXTWIDTH * 3 / 2);
        ty = y + 2 * (height / yd) - (TEXTHEIGHT * 3 / 2); 
        Display.displayText_f(tx, ty, 3, WHITE, BLACK, pFlashStr2);
      }
    }
};
