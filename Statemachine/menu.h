#ifndef _MENUH_
#define _MENUH_

const char MT_MAIN[] PROGMEM          = "Main Menu";
const char MT_DATE_TIME[] PROGMEM     = "Date Time";
const char MT_DATE[] PROGMEM          = "Date";
const char MT_TIME[] PROGMEM          = "Time";
const char MT_LOG[] PROGMEM           = "Logging";
const char MT_TEMPERATURE[] PROGMEM   = "Temperature";
const char MT_HUMIDITY[] PROGMEM      = "Humidity";
const char MT_EXIT[] PROGMEM          = "Exit";

const MENU_NEXTSTATE menu_nextstate[] PROGMEM = {
//  STATE                       INPUT       NEXT STATE
    {ST_MAIN,                   KEY_PLUS,   ST_MAIN_MENU},
    
// Main Menu    
    {ST_MAIN_MENU,              KEY_PLUS,   ST_DATE_TIME},
    {ST_DATE_TIME_MENU,         KEY_PLUS,   ST_LOGGING},
    {ST_LOGGING,                KEY_PLUS,   ST_TEMP_CHART},
    {ST_TEMP_CHART,             KEY_PLUS,   ST_HUMI_CHART},
    {ST_HUMI_CHART,             KEY_PLUS,   ST_EXIT},
    {ST_EXIT,                   KEY_PLUS,   ST_DATE_TIME_MENU},
    
    {ST_DATE,                   KEY_PLUS,   ST_TIME},
    {ST_TIME,                   KEY_PLUS,   ST_EXIT_DT},
    {ST_EXIT_DT,                KEY_PLUS,   ST_DATE},
    
// Yes, No Dialog
    {ST_YES,                    KEY_PLUS,   ST_NO},
    {ST_NO,                     KEY_PLUS,   ST_YES},
    
    {0,                         0,          0}
};


const MENU_STATE menu_state[] PROGMEM = {
//  STATE GROUP                         STATE                       STATE TEXT                  STATE_FUNC
    {ST_MAIN,                           ST_MAIN,                    NULL,                       mainScreen},
    
    {ST_MAIN_MENU,                      ST_MAIN_MENU,               MT_MAIN,                    showMenu},
    {ST_MAIN_MENU,                      ST_DATE_TIME_MENU,          MT_DATE_TIME,               NULL},
    {ST_MAIN_MENU,                      ST_LOGGING,                 MT_LOG,                     NULL},
    {ST_MAIN_MENU,                      ST_TEMP_CHART,              MT_TEMPERATURE,             NULL},
    {ST_MAIN_MENU,                      ST_HUMI_CHART,              MT_HUMIDITY,                NULL},
    {ST_MAIN_MENU,                      ST_EXIT,                    MT_EXIT,                    mainScreen},
    
    {ST_DATE_TIME_MENU,                 ST_DATE_TIME_MENU,          MT_DATE_TIME,               showMenu},
    {ST_DATE_TIME_MENU,                 ST_DATE,                    MT_DATE,                    NULL},
    {ST_DATE_TIME_MENU,                 ST_TIME,                    MT_TIME,                    NULL},    
    {ST_DATE_TIME_MENU,                 ST_EXIT_DT,                 MT_EXIT,                    mainScreen},    
    
    {ST_DATE,                           ST_DATE,                    NULL,                       setRtcDate},
    {ST_TIME,                           ST_TIME,                    NULL,                       setRtcTime},
    
    {ST_LOGGING,                        ST_LOGGING,                 NULL,                       setLogging},
    
    {ST_EXIT,                           ST_EXIT,                    NULL,                       exitMainMenu},
    {ST_EXIT_DT,                        ST_EXIT_DT,                 NULL,                       exitDateTimeMenu},
    {ST_TEMP_CHART,                     ST_TEMP_CHART,              NULL,                       temperatureChart},    
    
    {0,                                 NULL,                       NULL,                       NULL}
};

#endif
