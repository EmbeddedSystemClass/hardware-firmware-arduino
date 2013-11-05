// Ops ******************************************************************
#define CHARTONUM(a, b) ((a - '0') * b)

const byte btn1Pin = A1;     // pushbutton 1 pin
const byte btn2Pin = A2;     // pushbutton 2 pin

byte showMenu(byte input);
byte exitMainMenu(byte input);
byte exitDateTimeMenu(byte input);
byte mainScreen(byte inp);
byte setLogging(byte input);
byte setRtcTime(byte input);
byte setRtcDate(byte input);
byte temperatureChart(byte input);
void itochars(unsigned int value, char buffer[], byte digits);

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

#define KEY_NONE  1
#define KEY_PLUS  2
#define KEY_ENTER 3

#define ST_MAIN           1
#define ST_MAIN_MENU      2
#define ST_DATE_TIME      3
#define ST_DATE_TIME_MENU 4
#define ST_DATE           5
#define ST_TIME           6
#define ST_EXIT_DT        7
#define ST_LOGGING        8
#define ST_TEMP_CHART     9
#define ST_HUMI_CHART    10

#define ST_OK	       251
#define ST_CANCEL      252
#define ST_YES         253
#define ST_NO          254
#define ST_EXIT        255

const char MT_MAIN[] PROGMEM          = "Menu Test";
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
    
    {ST_MAIN_MENU,                      ST_MAIN_MENU,               NULL,                       showMenu},
    {ST_MAIN_MENU,                      ST_DATE_TIME_MENU,          MT_DATE_TIME,               NULL},
    {ST_MAIN_MENU,                      ST_LOGGING,                 MT_LOG,                     NULL},
    {ST_MAIN_MENU,                      ST_TEMP_CHART,              MT_TEMPERATURE,             NULL},
    {ST_MAIN_MENU,                      ST_HUMI_CHART,              MT_HUMIDITY,                NULL},
    {ST_MAIN_MENU,                      ST_EXIT,                    MT_EXIT,                    mainScreen},
    
    {ST_DATE_TIME_MENU,                 ST_DATE_TIME_MENU,          NULL,                       showMenu},
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

//Adafruit_PCD8544 adaDisplay = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Software RTC **********************************************************
swRTC rtc;

