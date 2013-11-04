class StateMachineManager {
  public:
    byte state;
    byte stateGroup;

  public:
    void doHandleStates() {
      byte nextstate;
    
      byte (*pStateFunc)(byte);
      byte input;
      byte i, j;
      
      state = ST_MAIN;
      stateGroup = ST_MAIN;
      nextstate = ST_MAIN;
      pStateFunc = mainScreen;
      
      for(;;) {
        //updateEvents();
        //updateButtonFlags();
    
        if (pStateFunc) {
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
