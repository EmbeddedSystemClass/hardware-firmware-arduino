#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "stdint.h"

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

///////////////////////////////////////////////////////////////////////////////
//  Definitionen  /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define DCF_T0    6             // T0:    1     .. DCF_T1-1 [sÃ·100]
#define DCF_T1   15             // T1: DCF_T0+1 .. DCF_T2-1 [sÃ·100]
#define DCF_T2   17             // T2: DCF_T1+1 .. DCF_T3-1 [sÃ·100]
#define DCF_T3   25             // T3: DCF_T2+1 .. DCF_T4-1 [sÃ·100]
#define DCF_T4   95             // T4: DCF_T3+1 .. DCF_T5-1 [sÃ·100]
#define DCF_T5  120             // T5: DCF_T4+1 .. DCF_T6-1 [sÃ·100]
#define DCF_T6  220             // T6: DCF_T5+1 ..   250    [sÃ·100]

typedef struct _TDATETIME {
  uint8_t secound;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} TDateTime;

static TDateTime dateTime;

///////////////////////////////////////////////////////////////////////////////
//  Typen  ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef enum                    // Ereignisse der DCF-PWM Abtastung
{   DCF_0    = '0',             // Â· Null erkannt
    DCF_1    = '1',             // Â· Eins erkannt
    DCF_MARK = 'm',             // Â· Minutenanfang erkannt
    DCF_FAIL = 'e',             // Â· Abtastfehler
    DCF_NONE = 'x'              // Â· Kein Ereignis
} DCFEvent;

///////////////////////////////////////////////////////////////////////////////
//  Datenobjekte  /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
volatile DCFEvent dcfEvent;

///////////////////////////////////////////////////////////////////////////////
//  Funktionen  ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void     EnableIRQs(void)  { __asm__ __volatile__ ("sei" ::); }
static void     DisableIRQs(void) { __asm__ __volatile__ ("cli" ::); }
static uint8_t  GetDCFPin(void)   { 
  return  !digitalRead(buttonPin);            
}

///////////////////////////////////////////////////////////////////////////////
//  10ms Timer Interrupt  /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SIGNAL(TIMER1_COMPA_vect)
{
    static uint8_t s = 0;
    digitalWrite(ledPin, s);  
    s=!s;
    
    typedef enum
    {   TI = 0,
        HI = 1,
        LO = 2
    } Input;

    typedef struct
    {   uint8_t  state;
        char     output;
    } ZetaValue;

    static const ZetaValue zeta[][3] PROGMEM =
    {   //+--Ti--+ +--Hi--+ +--Lo--+ +--------+
        { { 0,'e'},{ 1,'a'},{ 0,'x'} }, // S0
        { { 2,'x'},{ 1,'x'},{ 0,'e'} }, // S1
        { { 7,'x'},{ 2,'x'},{ 3,'x'} }, // S2
        { { 4,'x'},{ 0,'e'},{ 3,'x'} }, // S3
        { { 5,'x'},{ 0,'e'},{ 4,'x'} }, // S4
        { { 6,'x'},{ 0,'e'},{ 5,'x'} }, // S5
        { {11,'0'},{ 0,'e'},{ 6,'x'} }, // S6
        { { 8,'x'},{ 7,'x'},{ 0,'e'} }, // S7
        { { 0,'e'},{ 8,'x'},{ 9,'x'} }, // S8
        { {10,'x'},{ 0,'e'},{ 9,'x'} }, // S9
        { {11,'1'},{ 0,'e'},{10,'x'} }, // S10
        { {12,'x'},{ 1,'a'},{11,'x'} }, // S11
        { { 0,'e'},{ 1,'m'},{12,'x'} }  // S12
    };
    static uint8_t  state = 0;      // Aktueller Zustand S = { 0,...,12 }
    static uint8_t  tenMs = 0;      // HundertstelsekundenzÃ¤hler [sÃ·100]
    Input           input;          // Eingabe: I = { TI,HI,LO }
    char            output;         // Ausgabe: O = { 'x','a','0','1','m','e' }

    if( tenMs == DCF_T0 || tenMs == DCF_T1 || tenMs == DCF_T2 || tenMs == DCF_T3 || tenMs == DCF_T4 || tenMs == DCF_T5 || tenMs == DCF_T6  ) 
      input = TI;  // NB: Eingabe Ti
    else if( GetDCFPin()) 
      input = HI;  //     dominiert Hi
    else                   
      input = LO;  //     und Lo
    
    output = pgm_read_byte(&(zeta[state][(uint8_t)input].output));
    state  = pgm_read_byte(&(zeta[state][(uint8_t)input].state));
    if( output == 'm' ) 
      tenMs = 0;
    if( output == 'a' ) 
      tenMs = 0;
    else if( output != 'x' ) 
      dcfEvent = (DCFEvent)output;
    
    tenMs++;
}

///////////////////////////////////////////////////////////////////////////////
void
ProceedDCFDecoding(void)
{
    static const uint8_t  FAIL    = 0xFF;
    static const uint8_t  BCD[]   = { 1, 2, 4, 8, 10, 20, 40, 80 };
    static uint8_t        parity  = 0;
    static uint8_t        dlsTime = 0;
    static uint8_t        bitNo   = 0;      // 0 .. 58 oder FAIL
    static uint8_t        minute  = 0;      // 0 .. 59
    static uint8_t        hour    = 0;      // 0 .. 23
    static uint8_t        day     = 1;      // 1 .. 31
    static uint8_t        month   = 1;      // 1 .. 12
    static uint8_t        year    = 0;      // (2000+) 0 .. 255
    uint8_t               dcfBit  = 0;
    DCFEvent              event;

    DisableIRQs();
    event    = dcfEvent;
    dcfEvent = DCF_NONE;
    EnableIRQs();
    
    if( event == DCF_MARK && bitNo == 59 )
    {   // Neue Zeit:
        // Sekunde  = 0;
        // Minute   = minute;
        // Stunde   = hour;
        // Tag      = day;
        // Monat    = month;
        // Jahr     = 2000 + year;
        // MEZ/MESZ = dlsTime == 0/1
        dateTime.secound = 59;            
        dateTime.minute = minute;
        dateTime.hour = hour;
        dateTime.day = day;
        dateTime.month = month;
        dateTime.year = year;
    }
    switch( event )
    {   case DCF_NONE: return;
        case DCF_0:    dcfBit = 0; break;
        case DCF_1:    dcfBit = 1; break;
        default:       bitNo  = FAIL;
    }
    parity ^= dcfBit;
    switch( bitNo )
    {   case  0:                                              // -+
        case  1:                                              //  |
        case  2:                                              //  |
        case  3:                                              //  |
        case  4:                                              //  |
        case  5:                                              //  |
        case  6:                                              //  |
        case  7:                                              //  | Bits 0 - 16
        case  8:                                              //  | ignorieren
        case  9:                                              //  |
        case 10:                                              //  |
        case 11:                                              //  |
        case 12:                                              //  |
        case 13:                                              //  |
        case 14:                                              //  |
        case 15:                                              //  |
        case 16:                                       break; // -+
        case 17: dlsTime = dcfBit;                     break; // MESZ
        case 18: if( dcfBit == dlsTime ) bitNo = FAIL; break; // MEZ
        case 19:                                       break; // Schaltsekunde
        case 20: if( !dcfBit ) bitNo = FAIL;           break; // Startbit (1)
        case 21: parity = dcfBit; minute = 0;                 // -+
        case 22:                                              //  |
        case 23:                                              //  | Minute
        case 24:                                              //  |
        case 25:                                              //  |
        case 26:                                              //  |
        case 27: if( dcfBit ) minute += BCD[bitNo-21]; break; // -+
        case 28: if( parity ) bitNo = FAIL;            break; // ParitÃ¤t Minute
        case 29: parity = dcfBit; hour = 0;                   // -+
        case 30:                                              //  |
        case 31:                                              //  | Stunde
        case 32:                                              //  |
        case 33:                                              //  |
        case 34: if( dcfBit ) hour += BCD[bitNo-29];   break; // -+
        case 35: if( parity ) bitNo = FAIL;            break; // ParitÃ¤t Stunde
        case 36: parity = dcfBit; day  = 0;                   // -+
        case 37:                                              //  | Kalender-
        case 38:                                              //  | tag:
        case 39:                                              //  | 1 .. 31
        case 40:                                              //  |
        case 41: if( dcfBit ) day += BCD[bitNo-36];    break; // -+
        case 42:                                              // -+ Wochentag
        case 43:                                              //  |  ignorieren
        case 44:                                       break; // -+
        case 45: month = 0;                                   // -+
        case 46:                                              //  | Monat:
        case 47:                                              //  |  1 .. 12
        case 48:                                              //  |
        case 49: if( dcfBit ) month += BCD[bitNo-45];  break; // -+
        case 50: year = 0;                                    // -+
        case 51:                                              //  |
        case 52:                                              //  |
        case 53:                                              //  |  Jahr:
        case 54:                                              //  |  0 .. 99
        case 55:                                              //  |
        case 56:                                              //  |
        case 57: if( dcfBit ) year += BCD[bitNo-50];   break; // -+
        case 58: if( parity ) bitNo = FAIL;            break; // ParitÃ¤t Datum
        default: bitNo = FAIL;
    }    
    bitNo++;
    dateTime.secound = (dateTime.secound + 1) % 60;  
}


inline void timer_init(void)
{
    
  TCCR1A = 0;
  TCCR1B = (1<<WGM12) | (1<<CS12);
  OCR1A  = 625;                 // 100 = 100 Hz
  TIMSK1 |= (1 << OCIE0A);
   
}
  
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  timer_init();  
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop()
{
  ProceedDCFDecoding();
  static uint8_t lastSec = 0;
  if(lastSec != dateTime.secound)
  {
    lastSec = dateTime.secound;
    char buf[30] = { 0 };
    sprintf(buf, "%02d:%02d:%02d %02d.%02d.%02d", dateTime.hour, dateTime.minute, dateTime.secound, dateTime.day, dateTime.month, dateTime.year);
    Serial.println(buf);
  }
}

