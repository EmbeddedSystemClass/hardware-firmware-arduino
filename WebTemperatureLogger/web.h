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
IPAddress ip(192, 168, 2, 86); // 192.168.2.86

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

class WebManager {
  private:
    int m_values[5];
    int m_temperatureLog[20];
    char m_ethBuffer[30];
    bool m_logState;
  public:
    void begin() {
      Rnd();
      
      memset(m_ethBuffer, 0, 10);
      
      m_logState = false;
      
      Ethernet.begin(mac, ip);
      server.begin();
    }
    
    void Rnd() {
      for(int i = 0; i < 5; i++) {
        m_values[i] = 0; //random(10) * 10;
        //Serial.println(m_values[i]);
      }
      
      for(int i = 0; i < 20; i++) {
        m_temperatureLog[i] = random(12) * 10 - 20;
      }
      
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
          Rnd();
          break;							
        } else if (strncasecmp_P("%?", page_pointer, 2)==0) {
          page_pointer += 2;
          uint8_t idx = pgm_read_byte(page_pointer) - '0' - 1;

          char buffer[5] = { 0 };
          bin2asc(m_values[idx], buffer, 3);
          client->print(buffer);
          
          page_pointer++;
        } else if (strncasecmp_P("%LOGSTATE", page_pointer, 9)==0) {
          if(m_logState)
            client->print("checked");
          page_pointer += 9;
        } else if (strncasecmp_P("%FILES", page_pointer, 6)==0) {
          files(client);
          page_pointer += 6;        
        } else {                
          client->write(b);
        }
      }
    }
    
    void sendPage2(EthernetClient* client) {     

      client->println(F("HTTP/1.1 200 OK"));
      client->println(F("Content-Type: text/html"));
      client->println(F("Connection: close"));  // the connection will be closed after completion of the response
      //client->println("Refresh: 5");  // refresh the page automatically every 5 sec
      client->println();

      
      SD_ACTIVE();

      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {
                   
          if(file.open("INDEX.HTM", O_READ)) {                    
            Serial.println("file");
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
    
    void xmlResponse(EthernetClient* client) {
      client->println(F("HTTP/1.0 200 OK"));
      client->println("Content-Type: text/xml");
      client->println("Connection: keep-alive");
      client->println();
      // send XML file containing temperature
      
      client->print("<?xml version = \"1.0\" ?>");
      client->print("<data>");
      for(int i = 0; i < 20; i++) {
        client->print("<analog>");
        client->print(m_temperatureLog[i]);
        client->print("</analog>");
      }
      client->print("</data>");
      client->flush();
      client->stop();
      
      for(int i = 0; i < 19; i++) {
        m_temperatureLog[i] = m_temperatureLog[i+1];
      }
      m_temperatureLog[18] = random(12) * 10 - 20;
    }
    
    void xmlDirResponse(EthernetClient* client) {      
      client->println(F("HTTP/1.0 200 OK"));
      client->println("Content-Type: text/xml");
      client->println("Connection: keep-alive");
      client->println();
      
      // send XML file containing sd card directory
      
      client->print("<?xml version = \"1.0\" ?>");
      
      SD_ACTIVE();
      if (card.init(0, SS_SD_CARD) && Fat16::init(&card)) {
        // "Name          Modify Date/Time    Size";
        //Fat16::ls(LS_DATE | LS_SIZE);
        
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
            
            if (strncasecmp_P(m_ethBuffer, PSTR("GET /file.txt"), 13)==0) {
              Serial.println("match file");
              sendPage2(&client);
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /?SUB=Start+Log"), 19)==0) {
              m_logState = true;
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /?SUB=Stopp+Log"), 19)==0) {
              m_logState = false;
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /temperatur_data"), 16)==0) {
              xmlResponse(&client);
            } else if (strncasecmp_P(m_ethBuffer, PSTR("GET /dir"), 8)==0) {
              xmlDirResponse(&client);
            } else {
              // if you've gotten to the end of the line (received a newline
              // character) and the line is blank, the http request has ended,
              // so you can send a reply
              if (c == '\n' && currentLineIsBlank) {
                sendPage2(&client);
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
    
    
    
//    void files(Stream* stream) {
//    //<option>file.csv</option>      
//      for (uint8_t i = 0; i < 10; i++)
//      {		
//        char buffer[50] = { 0 };        
//        strcpy_P(buffer, PSTR("<option>????.csv</option>"));
//        bin2asc(i, &buffer[8], 4);        
//        stream->println(buffer);
//      }
//    }
    
    void files(Stream* stream) {
      
    }
};

WebManager Web;
