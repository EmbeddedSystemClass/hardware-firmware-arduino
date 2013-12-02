// Ops ******************************************************************
#define CHARTONUM(a, b) ((a - '0') * b)

#define KEY_NONE          1
#define KEY_PLUS          2
#define KEY_ENTER         3

#define ST_MAIN           1
#define ST_MAIN_MENU      2
#define ST_DATE_TIME      3
#define ST_DATE_TIME_MENU 4
#define ST_DATE           5
#define ST_TIME           6
#define ST_EXIT_DT        7
#define ST_LOGGING        8

#define ST_OK	        251
#define ST_CANCEL       252
#define ST_YES          253
#define ST_NO           254
#define ST_EXIT         255

const byte btn1Pin = A1;     // pushbutton 1 pin
const byte btn2Pin = A2;     // pushbutton 2 pin


byte showMenu(byte input);
byte exitMainMenu(byte input);
byte exitDateTimeMenu(byte input);
byte mainScreen(byte inp);
byte setLogging(byte input);
byte setRtcTime(byte input);
byte setRtcDate(byte input);

void itochars(unsigned int value, char buffer[], byte digits);


// Software RTC **********************************************************
swRTC rtc;

