#ifndef _MENUH_
#define _MENUH_

// States, Menu ********************************************************

typedef struct PROGMEM
{
    byte state;
    byte input;
    byte nextstate;
} MENU_NEXTSTATE;

typedef struct PROGMEM
{
    byte group;
    byte state;
    PGM_P pText;
    byte (*pFunc)(byte input);
} MENU_STATE;

const char MT_MAIN_MENU[] PROGMEM     = "Main Menu";
const char MT_DATE_TIME[] PROGMEM     = "Date Time";
const char MT_DATE[] PROGMEM          = "Date";
const char MT_TIME[] PROGMEM          = "Time";
const char MT_LOG[] PROGMEM           = "Logging";
const char MT_LOG_STATE[] PROGMEM     = "Log State";
const char MT_LOG_INTERVAL[] PROGMEM  = "Log Interval";
const char MT_EXIT[] PROGMEM          = "Exit";

const MENU_NEXTSTATE menu_nextstate[] PROGMEM = {
//  STATE                       INPUT       NEXT STATE
    {ST_MAIN,                   KEY_PLUS,   ST_SHOW_VALUES},
    {ST_SHOW_VALUES,            KEY_PLUS,   ST_MAIN_MENU},
    
// Main Menu    
    {ST_MAIN_MENU,              KEY_PLUS,   ST_DATE_TIME},
    {ST_DATE_TIME_MENU,         KEY_PLUS,   ST_LOG_MENU},
    {ST_LOG_MENU,               KEY_PLUS,   ST_EXIT},    
    {ST_EXIT,                   KEY_PLUS,   ST_DATE_TIME_MENU},
    
    {ST_DATE,                   KEY_PLUS,   ST_TIME},
    {ST_TIME,                   KEY_PLUS,   ST_EXIT_DT},
    {ST_EXIT_DT,                KEY_PLUS,   ST_DATE},   
    {0,                         0,          0}
};


const MENU_STATE menu_state[] PROGMEM = {
//  STATE GROUP                         STATE                       STATE TEXT                  STATE_FUNC
    {ST_MAIN,                           ST_MAIN,                    NULL,                       mainScreen},
    {ST_SHOW_VALUES,                    ST_SHOW_VALUES,             NULL,                       showValues},
    
    {ST_MAIN_MENU,                      ST_MAIN_MENU,               MT_MAIN_MENU,               showMenu},
    {ST_MAIN_MENU,                      ST_DATE_TIME_MENU,          MT_DATE_TIME,               NULL},
    {ST_MAIN_MENU,                      ST_LOG_MENU,                MT_LOG,                     NULL},    
    {ST_MAIN_MENU,                      ST_EXIT,                    MT_EXIT,                    mainScreen},
    
    {ST_DATE_TIME_MENU,                 ST_DATE_TIME_MENU,          MT_DATE_TIME,               showMenu},
    {ST_DATE_TIME_MENU,                 ST_DATE,                    MT_DATE,                    NULL},
    {ST_DATE_TIME_MENU,                 ST_TIME,                    MT_TIME,                    NULL},    
    {ST_DATE_TIME_MENU,                 ST_EXIT_DT,                 MT_EXIT,                    mainScreen},    
    
    {ST_LOG_MENU,			ST_LOG_MENU,		    MT_LOG,			showMenu},
    {ST_LOG_MENU,			ST_LOG_STATE,		    MT_LOG_STATE,		NULL},
    {ST_LOG_MENU,			ST_LOG_INTERVAL,	    MT_LOG_INTERVAL,		NULL},
    {ST_LOG_MENU,                       ST_EXIT_LOG,                MT_EXIT,                    mainScreen},    
    
    {ST_DATE,                           ST_DATE,                    NULL,                       setRtcDate},
    {ST_TIME,                           ST_TIME,                    NULL,                       setRtcTime},
    
    {ST_LOG_STATE,                      ST_LOG_STATE,               NULL,                       setLogState},
    {ST_LOG_INTERVAL,                   ST_LOG_INTERVAL,            NULL,                       setLogInterval},
    
    {ST_EXIT,                           ST_EXIT,                    NULL,                       exitMainMenu},
    {ST_EXIT_DT,                        ST_EXIT_DT,                 NULL,                       exitDateTimeMenu},    
    
    {0,                                 NULL,                       NULL,                       NULL}
};

#endif
