// Ops ******************************************************************
#define CHARTONUM(a, b) ((a - '0') * b)

#define KEY_NONE             1
#define KEY_PLUS             2
#define KEY_MINUS            3
#define KEY_ENTER            4

#define ST_MAIN              1
#define ST_MAIN_MENU         2
#define ST_DATE_TIME         3
#define ST_DATE_TIME_MENU    4
#define ST_DATE              5
#define ST_TIME              6
#define ST_EXIT_DT           7
#define ST_LOG_MENU          8
#define ST_LOG_STATE         9
#define ST_LOG_INTERVAL     10
#define ST_EXIT_LOG         11

#define ST_OK	           251
#define ST_CANCEL          252
#define ST_YES             253
#define ST_NO              254
#define ST_EXIT            255

const byte btn1Pin = A1;     // pushbutton 1 pin
const byte btn2Pin = A2;     // pushbutton 2 pin


byte showMenu(byte input);
byte exitMainMenu(byte input);
byte exitDateTimeMenu(byte input);
byte mainScreen(byte inp);
byte setLogState(byte input);
byte setLogInterval(byte input);
byte setRtcTime(byte input);
byte setRtcDate(byte input);
String getTimeStr();
String getDateStr();

void itochars(unsigned int value, char buffer[], byte digits);


// RTC **********************************************************
RTC_DS1307 rtc;


