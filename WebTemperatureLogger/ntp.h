
#ifndef NTPCLIENT_H_
#define NTPCLIENT_H_

#include <Ethernet.h>
#include <EthernetUdp.h>

#define NTP_PACKET_SIZE 48

class NtpClient {
  
  private:
    
    void sendNtpPacket(char* url, EthernetUDP* pUdp, byte* packetBuffer) {
      
      // set all bytes in the buffer to 0
      memset(packetBuffer, 0, NTP_PACKET_SIZE);
      // Initialize values needed to form NTP request
      // (see URL above for details on the packets)
      packetBuffer[0] = 0b11100011;   // LI, Version, Mode
      packetBuffer[1] = 0;     // Stratum, or type of clock
      packetBuffer[2] = 6;     // Polling Interval
      packetBuffer[3] = 0xEC;  // Peer Clock Precision
      // 8 bytes of zero for Root Delay & Root Dispersion
      packetBuffer[12]  = 49;
      packetBuffer[13]  = 0x4E;
      packetBuffer[14]  = 49;
      packetBuffer[15]  = 52;

      // all NTP fields have been given values, now
      // you can send a packet requesting a timestamp:
      pUdp->beginPacket(url, 123); //NTP requests are to port 123
      pUdp->write(packetBuffer,NTP_PACKET_SIZE);
      pUdp->endPacket();
    }
    
  public:
    
    unsigned long getEpoch() {      
      EthernetUDP Udp;
      byte packetBuffer[NTP_PACKET_SIZE];
      
      Udp.begin(123);
      
      sendNtpPacket("pool.ntp.org", &Udp, packetBuffer);	// send an NTP packet to a time server
      
      // wait to see if a reply is available
      delay(1000);
      
      if ( Udp.parsePacket() ) {	
        // We've received a packet, read the data from it
        Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

        //the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, esxtract the two words:

        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        
        // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
        const unsigned long seventyYears = 2208988800UL;
        // subtract seventy years:
        unsigned long epoch = secsSince1900 - seventyYears;
        
        return epoch + 3600; //  UTC +1h
      }
      
      return 0; //an error occurred
    }
    
  };


NtpClient Ntp;

#endif /* NTPCLIENT_H_ */
