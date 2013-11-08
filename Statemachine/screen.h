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
    
  public:
    MainScreen() {      
      label1.setText("App Test");
    }
    
    void draw() {
      label1.draw();
      
      if (Events.bT1000MS) {
        char time[9]= { "00:00:00" };  
        itochars(rtc.getHours(), &time[0], 2);
        itochars(rtc.getMinutes(), &time[3], 2);
        itochars(rtc.getSeconds(), &time[6], 2);  
        
        Display.displayText(5, 20, 2, time);
      }
    }
};

MainScreen MainScreen;


class MenuScreen {
  private:
    unsigned bInvalidate:1;
    byte menuState;
    byte stateTemp;
    
  public:    
    Frame frame1;
    
  public:
    MenuScreen() {      
      //frame1.setRect(0,0, ST7735_TFTWIDTH, TEXTHEIGHT + 2);
      bInvalidate = true;
    }
    
    byte input(byte input) {
      
      
      if (Events.bBtn2) {
        menuState=StateMachine.getNextState(menuState, KEY_PLUS);
        bInvalidate = true;
      } else if (Events.bBtn1) {
        stateTemp = menuState;
        StateMachine.stateGroup = menuState;
        menuState = 0;
        Display.clearDisplay();
        bInvalidate = true;
        return stateTemp;
      } 
      
      return StateMachine.state;
    }
    
    void draw() {
      if (!bInvalidate)
        return;
      
      byte i, j;
      byte y = 0;
      
      PGM_P statetext;
      
      for (i=0; (j=pgm_read_byte(&menu_state[i].group)); i++) {
        if (j == StateMachine.stateGroup) {
          stateTemp = pgm_read_byte(&menu_state[i].state);
          statetext = (PGM_P)pgm_read_word(&menu_state[i].pText);
          if (statetext != NULL) {
            if (menuState == 0) {
              menuState = stateTemp;
            }
            Display.displayText_f(2, y * (TEXTHEIGHT + 1) + 1, 1, statetext);
            if (menuState == stateTemp) {
              frame1.setRect(0, y * (TEXTHEIGHT + 1), ST7735_TFTWIDTH, 10);
            }
            y++;
          }
        }
      }
      
      frame1.draw();
      
      bInvalidate = false;
    }
};

MenuScreen MenuScreen;
