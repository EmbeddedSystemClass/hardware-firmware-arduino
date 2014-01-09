#include <SD.h>

class LogData {      
  public:
  
    void process() {
      if (LogEvents.bLog) {
        logData();
      }
    }  
    
    void logData()
    {    
      // make a string for assembling the data to log:  
      String dataString;
      // 00:00:00,00.00,00.00
      dataString += getTimeStr() + F(";");
      //dataString += "values..."; // test dummy
      
      // dataString += String(shtState.temperature) + F(";");
      // dataString += String(shtState.humidity) + F(";");

      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
 /*     File dataFile = SD.open("datalog.txt", FILE_WRITE);

      // if the file is available, write to it:
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(dataString);
      }    
      // if the file isn't open, pop up an error:
      else {
        Serial.println(F("error opening datalog.txt"));
      }   
*/
    }
};


LogData LogData;
