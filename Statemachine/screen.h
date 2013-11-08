#define BACKCOLOR ST7735_BLACK

class Screen {
  public:
    virtual void draw() {
    }
};

class Component {
  public:
    unsigned bVisible:1;
    unsigned bInvalidate:1;
   
  public:
    Component() {
      bVisible = true;
      bInvalidate = true;
    }
    
    virtual void draw() {
    } 
    
    void setVisible(byte state) {
      if (bVisible == state)
        return;
        
      //Serial.println(state);
      bInvalidate = true;
      bVisible = state;
    }
};

class Label : public Component {
    private:
      byte oldX;
      byte oldY;
      byte oldLength;
    public:
      char* text;
      byte x;
      byte y;
    public:
      Label() : Component() {
      }
    
      void setText(char* newtext) {
        bInvalidate = true;
        text = newtext;
      }
      
      void setText(byte px, byte py, char* newText) {
        
        if (px == oldX && py == oldY && strcmp(text, newText) == 0)
          return;

        x = px;
        y = py;
        oldLength = strlen(text);
        text = newText;
        bInvalidate = true;
      }
    
      void draw() {
        if (!bInvalidate)
          return;
        
        Display.fillRect(oldX, oldY, oldLength * TEXTWIDTH, TEXTHEIGHT, BACKCOLOR);
        
        if (bVisible)  
          Display.displayText(x, y, 1, text, ST7735_YELLOW, BACKCOLOR);          
        
        oldX = x;
        oldY = y;
        
        bInvalidate = false;
      }
};


class Frame : public Component {
  private:
    byte oldX;
    byte oldY;
    byte oldW;
    byte oldH;
  public:
    byte x;
    byte y;
    byte w;
    byte h;
    
  public:
    Frame() : Component() {
      
    }
   
   void setRect(byte px, byte py, byte pw, byte ph) {
     x = px;
     y = py;
     w = pw;
     h = ph;
     bInvalidate = !(x == oldX && y == oldY && w == oldW && h == oldH);
   }
   
   void draw() {
     if (!bInvalidate)
       return;
       
     Display.drawRect(oldX, oldY, oldW, oldH, BACKCOLOR);
     
     if (bVisible)  
       Display.drawRect(x, y, w, h, ST7735_WHITE);
     
     oldX = x;
     oldY = y;
     oldW = w;     
     oldH = h;
     
     bInvalidate = false;
   }
};

class MainScreen {
  public:
    Label label1;
    Frame frame1;
    
  public:
    MainScreen() {      
      label1.setText("Hallo");
      frame1.setRect(20,20, 30, 10);
    }
    
    void draw() {
      frame1.draw();
      label1.draw();
    }
};

MainScreen MainScreen;


class MenuScreen {
  public:    
    Frame frame1;
    
  public:
    MenuScreen() {      
      frame1.setRect(0,0, ST7735_TFTWIDTH, TEXTHEIGHT + 2);
    }
    
    void draw() {
      if (!frame1.bInvalidate) return;
      
      Display.displayText_f(2, TEXTHEIGHT * 0 + 2, 1, PSTR("Date Time")); 
      Display.displayText_f(2, TEXTHEIGHT * 1 + 2, 1, PSTR("Logging"));
      Display.displayText_f(2, TEXTHEIGHT * 2 + 2, 1, PSTR("Temperature"));
      Display.displayText_f(2, TEXTHEIGHT * 3 + 2, 1, PSTR("Humidity"));
      Display.displayText_f(2, TEXTHEIGHT * 4 + 2, 1, PSTR("Exit"));
      frame1.draw();
    }
};

MenuScreen MenuScreen;
