// Web ****************************************************************
#include <avr/pgmspace.h>
#include "webpage.h"
#include "measure.h"

#define CHART_X_INTERVALS	  20
#define CHART_Y_INTERVALS	  13
#define CHART_TOP			      50
#define CHART_LEFT			    100
#define CHART_RIGHT			    600
#define CHART_BOTTOM		    350
#define CHART_X_STEP		    (CHART_RIGHT - CHART_LEFT) / CHART_X_INTERVALS
#define CHART_Y_STEP		    (CHART_BOTTOM - CHART_TOP) / (CHART_Y_INTERVALS - 1)
#define CHART_Y_ZERO 		250 + CHART_TOP

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 2, 87); // 192.168.2.87

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

class WebManager {
  private:
    char m_ethBuffer[30];
    bool m_logState;    
  public:
    void begin() {
      memset(m_ethBuffer, 0, 10);
      
      m_logState = false;
      
      Ethernet.begin(mac, ip);
      server.begin();
    }
    
    void sendPage1(EthernetClient* client) {      
      // send a standard http response header
      client->println(F("HTTP/1.1 200 OK"));
      client->println(F("Content-Type: text/html"));
      client->println(F("Connection: close"));  // the connection will be closed after completion of the response
      //client->println("Refresh: 5");  // refresh the page automatically every 5 sec
      client->println();
      
      PGM_P page_pointer = NULL;//Page1;
              
      for(;;) {
        unsigned char b = pgm_read_byte(page_pointer++);
        if (strncasecmp_P("%END",page_pointer,4)==0) {                  
          client->flush();
          client->stop();          
          break;							              
        } else {                
          client->write(b);
        }
      }
    }
    
    void sendPageFromSDCard(EthernetClient* client) {     

      client->println(F("HTTP/1.1 200 OK"));
      client->println(F("Content-Type: text/html"));
      client->println(F("Connection: close"));  // the connection will be closed after completion of the response
      //client->println("Refresh: 5");  // refresh the page automatically every 5 sec
      client->println();


      SD_ACTIVE();

      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) 
      {                   
          if(file.open("INDEX.HTM", O_READ)) {
            char buffer[160];
            size_t size;
            
            for(;;) {              
              SD_ACTIVE();
              size = file.read(buffer, sizeof(buffer));
              
              ETH_ACTIVE();
              if (size == 0 || !client->connected()) {
                break;
              }              
              client->write(buffer, size);              
            }          
            
            file.close();
          }          
      }
       
      ETH_ACTIVE();
      
      client->flush();
      client->stop();
    }   
        
    void sendFile(EthernetClient* client, char* fileName) {
      client->println(F("HTTP/1.0 200 OK"));
      client->println(F("Content-Type: text/csv"));
      client->println(F("Connnection: close"));
      client->println(F("Content-disposition: attachment;filename=file.csv"));
      client->println();
      
      SD_ACTIVE();

      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {                  
          if(file.open(fileName, O_READ)) {
            for(int i = 0; i < file.fileSize(); i++) {
              char c = (char)file.read();
              //Serial.print(c);
              ETH_ACTIVE();
              client->write(c);
              SD_ACTIVE();
            }                        
            file.close();
          }
      }       
      
      ETH_ACTIVE();
      
      client->flush();
      client->stop();
    }
    
    void sendTemperatureRamLog(EthernetClient* client) {
      client->println(F("HTTP/1.0 200 OK"));
      client->println(F("Content-Type: text/xml"));
      client->println(F("Connection: keep-alive"));
      client->println();
      // send XML file containing temperature
      
      client->print(F("<?xml version = \"1.0\" ?>"));
      client->print(F("<data>"));
      for(int i = 0; i < LOG_DATA_SIZE; i++) {
        client->print(F("<analog>"));
        client->print(LogData.temperature1Log[i]);
        client->print(F("</analog>"));
      }
      client->print(F("</data>"));
      client->flush();
      client->stop();
    }
    
    void sendSystemState(EthernetClient* client) {
      client->println(F("HTTP/1.0 200 OK"));
      client->println(F("Content-Type: text/xml"));
      client->println(F("Connection: keep-alive"));
      client->println();
      // send XML file containing status
      
      client->print(F("<?xml version = \"1.0\" ?>"));
      client->print(F("<state>"));
      
        client->print(F("<logstate>"));
        client->print(m_logState);
        client->print(F("</logstate>"));
      
        client->print(F("<loginterval>"));
        client->print(LogEvents.interval);
        client->print(F("</loginterval>"));
      
        client->print(F("<systemtime>"));
        char buffer[] = "yyyy-mm-dd hh:mm:ss";
        bin2asc(RTC.getYear(), buffer, 4);
        bin2asc(RTC.getMonth(), &buffer[5], 2);
        bin2asc(RTC.getDay(), &buffer[8], 2);
        bin2asc(RTC.getHours(),&buffer[11], 2);
        bin2asc(RTC.getMinutes(), &buffer[14], 2);
        bin2asc(RTC.getSeconds(), &buffer[17], 2);        
        client->print(buffer); 
        client->print(F("</systemtime>"));

      client->print(F("</state>"));
      client->flush();
      client->stop();
    }
    
    void sendLogFilesDir(EthernetClient* client) {      
      client->println(F("HTTP/1.0 200 OK"));
      client->println(F("Content-Type: text/xml"));
      client->println(F("Connection: keep-alive"));
      client->println();
      
      // send XML file containing sd card directory
      
      client->print(F("<?xml version = \"1.0\" ?>"));
      
      SD_ACTIVE();
      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {       
        ETH_ACTIVE();
        client->print(F("<dir>"));        
        SD_ACTIVE();
        
        dir_t d;
        for (uint16_t index = 0; Fat16::readDir(&d, &index, DIR_ATT_VOLUME_ID); index++) {
          ETH_ACTIVE();
          client->print(F("<file>"));
          SD_ACTIVE();
          for (uint8_t i = 0; i < 11; i++) {
            if (d.name[i] == ' ') 
              continue;
            if (i == 8) {
              ETH_ACTIVE();
              client->write('.');
              SD_ACTIVE();
            }
            ETH_ACTIVE();
            client->write(d.name[i]);
            SD_ACTIVE();
          }
          ETH_ACTIVE();
          client->print(F("</file>"));
          client->println();
          SD_ACTIVE();
        }
        
        ETH_ACTIVE();
        client->print(F("</dir>"));
               
      } else {
        Serial.println(F("card failed!"));
      }
      ETH_ACTIVE();
      
      client->flush();
      client->stop();
    }
    
    uint8_t GetFileNameLength(char* buffer) {
      char* pDot = strstr(buffer, ".");
      if (pDot) {
        return pDot - buffer + 4;		
      }
      return 0;
    }
    
    bool GetUInt16FromString(char* buffer, uint16_t* pInterval, char* sep) {
      *pInterval = 0;
      char* pNChar = strstr(buffer, sep);
      if (pNChar) {
        uint16_t n = 1;
        while (pNChar-- > buffer) {          
          *pInterval += (*pNChar - '0')  * n;
          n *= 10;
        }
        return true;
      }
      return false;
    }
    
    void setLogInterval() {      
      uint16_t interval = 0;
      if(GetUInt16FromString(m_ethBuffer + 21, &interval, " ")) {
        LogEvents.interval = interval;
      }      
    }
    
    void deleteFile(char* fileName) {
      SD_ACTIVE();   
           
      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {        
        file.remove(fileName);
      }
      
      ETH_ACTIVE();
    }
    
    void dispatch() {
      
      EthernetClient client = server.available();
      
      if (client) {
        //Serial.println("new client");
        // an http request ends with a blank line
        static boolean currentLineIsBlank = true;
        if (client.connected()) {
          if (client.available()) {
            char c = client.read();
            Serial.write(c);
            
            m_ethBuffer[sizeof(m_ethBuffer) - 2] = c;
            
            if (strncasecmp_P(m_ethBuffer, PSTR("GET /file"), 9)==0) {              
              uint8_t l = GetFileNameLength(m_ethBuffer + 10);
              if (l > 0 && l < 13) {                
                char name[13] = { 0 };
                strncpy(name, m_ethBuffer + 10, l);
                sendFile(&client, name);
              } else {
                Serial.println("file not found");
              }              
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /delete"), 11)==0) {              
              uint8_t l = GetFileNameLength(m_ethBuffer + 12);
              if (l > 0 && l < 13) {                
                char name[13] = { 0 };
                strncpy(name, m_ethBuffer + 12, l);
                deleteFile(name);
              } else {
                Serial.println("file not found");
              }
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /start_log"), 14)==0) {
              m_logState = true;
              LogData.startLog();
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /stopp_log"), 14)==0) {
              m_logState = false;
              LogData.stopLog();
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /set_loginterval"), 20)==0) {
              setLogInterval();
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /temperatur_data"), 20)==0) {
              sendTemperatureRamLog(&client);
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /dir"), 8)==0) {
              sendLogFilesDir(&client);
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /system_state"), 17)==0) {
              sendSystemState(&client);
            } else {
              // if you've gotten to the end of the line (received a newline
              // character) and the line is blank, the http request has ended,
              // so you can send a reply
              if (c == '\n' && currentLineIsBlank) {
                sendPageFromSDCard(&client);
              }
              if (c == '\n') {
                // you're starting a new line
                currentLineIsBlank = true;
              }
              else if (c != '\r') {
                // you've gotten a character on the current line
                currentLineIsBlank = false;
              }
            }
            
            memmove(m_ethBuffer, m_ethBuffer + 1, sizeof(m_ethBuffer) - 2);
            
          }
        } else {
          // give the web browser time to receive the data
          //delay(1);
          // close the connection:
          client.stop();          
          Serial.println("client disconnected");
        }
      }
    }
   
};

WebManager Web;

