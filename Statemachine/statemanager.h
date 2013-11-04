class StateMachineManager {
  private:
    byte nextstate;    
    byte (*pStateFunc)(byte);
  
  public:
    byte state;
    byte stateGroup;

  public:
    StateMachineManager() {
      state = ST_MAIN;
      stateGroup = ST_MAIN;
      nextstate = ST_MAIN;
      pStateFunc = mainScreen;
      Serial.println("Init Statemachine");
    }
    
    void doHandleStates() {
      byte i, j;      

      if (pStateFunc != NULL) {
        // When in this state, we must call the state function
        nextstate = pStateFunc(0);
      } else {
        nextstate = ST_MAIN;
      }
      
      if (nextstate != state) {
        state = nextstate;
        Serial.println("Search State:" + String(state));
        for (i=0; (j=pgm_read_byte(&menu_state[i].state)); i++) {
          stateGroup =pgm_read_byte(&menu_state[i].group);
          Serial.println("Found Group" + String(stateGroup));
          if (j == state && stateGroup == state) {
            pStateFunc = (byte (*)(byte))(PGM_VOID_P) pgm_read_word(&menu_state[i].pFunc);
            Serial.println(String(state) + ";" + String(stateGroup));
            break;
          }
        }
      }    

    }
        
    unsigned char getNextState(byte state, byte stimuli)
    {
        byte nextstate = state;    // Default stay in same state
        byte i, j;
        for (i=0; ( j=pgm_read_byte(&menu_nextstate[i].state) ); i++ )
        {
            if ( j == state && 
                 pgm_read_byte(&menu_nextstate[i].input) == stimuli)
            {
                nextstate = pgm_read_byte(&menu_nextstate[i].nextstate);
                break;
            }
        }
        return nextstate;
    }
};

StateMachineManager StateMachine;
