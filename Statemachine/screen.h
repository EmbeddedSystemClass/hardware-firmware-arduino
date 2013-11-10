#define BACKCOLOR ST7735_BLACK

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
      char *text;
      byte x;
      byte y;
      byte textSize;
      
    public:
      Label() : Component() {
        textSize = 1;
      }
    
      void setText(char* newtext) {
        bInvalidate = true;
        text = newtext;
      }
      
      void setText(byte px, byte py, char* newText) {
        x = px;
        y = py;
        oldLength = strlen(text);
        text = newText;
        bInvalidate = true;
      }
    
      void draw() {
        if (!bInvalidate)
          return;
        
        Display.fillRect(oldX, oldY, oldLength * TEXTWIDTH, textSize * TEXTHEIGHT, BACKCOLOR);
        
        if (bVisible)  
          Display.displayText(x, y, textSize, text, ST7735_YELLOW, BACKCOLOR);          
        
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
       Display.drawRect(x, y, w, h, ST7735_YELLOW);
     
     oldX = x;
     oldY = y;
     oldW = w;     
     oldH = h;
     
     bInvalidate = false;
   }
};

class MainScreen : public Screen {
  
  public:
    MainScreen() {      
    }
    
    void draw() {

      if (bInvalidate) {
        Display.displayText_f(0, 0, 2, ST7735_YELLOW, BACKCOLOR, PSTR("App Test"));
      }
      
      if (Events.bT1000MS) {
        char time[9]= { "00:00:00" };  
        itochars(rtc.getHours(), &time[0], 2);
        itochars(rtc.getMinutes(), &time[3], 2);
        itochars(rtc.getSeconds(), &time[6], 2);  
        
        Display.displayText(5, 30, 2, time, ST7735_GREEN, BACKCOLOR);
      }
      
      bInvalidate = false;
    }
    
};

MainScreen MainScreen;


class MenuScreen : public Screen {
  private:
    byte menuState;
    
  public: 
    unsigned bInvalidateTitle:1;  
    Frame frame1;
    
  public:
    MenuScreen() {
      bInvalidateTitle = true;
    }
    
    byte input(byte input) {      
      if (Events.bBtn2) {
        menuState=StateMachine.getNextState(menuState, KEY_PLUS);
        bInvalidate = true;
      } else if (Events.bBtn1) {
        StateMachine.stateGroup = menuState;
        menuState = 0;
        hide();
        return StateMachine.stateGroup;
      } 
      
      return StateMachine.state;
    }
    
    void draw() {
      if (!bInvalidate) { 
        return;
      }
        
      byte i;
      byte y = 0;
      byte group;
      byte state;
      
      PGM_P statetext;
      
      for (i=0; (group = pgm_read_byte(&menu_state[i].group)); i++) {
        if (group == StateMachine.stateGroup) {
          state = pgm_read_byte(&menu_state[i].state);
          statetext = (PGM_P)pgm_read_word(&menu_state[i].pText);
          if (statetext != NULL) {
            if (state == group) {
              if (bInvalidateTitle) {
                Display.fillRect(0, y, ST7735_TFTHEIGHT, TEXTHEIGHT + 10, ST7735_WHITE);
                Display.displayText_f(4, y + 2, 2, BACKCOLOR, ST7735_WHITE, statetext);
                bInvalidateTitle = false;
              }
              y += 3;
            } else {
              if (menuState == 0) {
                menuState = state;              
              }
              if (menuState == state) {
                frame1.setRect(0, y + 3, ST7735_TFTHEIGHT, TEXTHEIGHT * 2 + 3);
                frame1.bInvalidate = true;
                frame1.draw();
                Display.displayText_f(4, y + 5, 2, ST7735_YELLOW, BACKCOLOR, statetext);
              } else {
                Display.displayText_f(4, y + 5, 2, statetext);
              }
            }
            y += TEXTHEIGHT * 2 + 3;
          }
        }
      }
     
      
      bInvalidate = false;
    }
    
    void show() {
      if (!bVisible) {
        bInvalidate = true;
        bInvalidateTitle = true;
      }
      bVisible = true;
    }
};

MenuScreen MenuScreen;
