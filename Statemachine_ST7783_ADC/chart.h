#ifndef _CHARTSH_
#define _CHARTSH_

/*
  Charts
  
 This example code is in the public domain.
 */

#define DISPLAYHEIGHT TFTWIDTH
#define DISPLAYWIDTH  TFTHEIGHT
#define MARGIN		    8
#define CHARTHEIGHT   250
#define CHARTWIDTH    240
#define CHART_X       0
#define CHART_Y       75

class Chart {    
  public:
    void drawChart(int8_t values[], uint8_t count, int16_t rangeMin, int16_t rangeMax) {
      int8_t min = rangeMax;
      int8_t max = rangeMin;
			
      LogData.getStat(values, count, &min, &max);
      
      // fit min, max in range of 10th steps *********
      if(min >= 0) { 
        min = 0;
      } else {
        for (int16_t i = 0; i > rangeMin; i -= 10) {
          if (i < min) {
            min = i;
            break;
          }
        }
      }
      
      if(max <= 0) {
        max = 0;
      } else {
        for (int16_t i = 0; i < rangeMax; i += 10) {
          if (i > max) {
            max = i;
            break;
          }
        }
      }
      
      // range ***************************************
      uint8_t range = max - min;
      uint8_t zero = map(0, min, max, CHARTHEIGHT, CHART_Y);   
      
      // draw y-Grid lines ***************************
      for (int8_t i = min; i <= max; i+=5) {
        uint8_t y = map(i, min, max, CHARTHEIGHT, CHART_Y);
        Display.drawFastHLine(CHART_X , y, CHARTWIDTH, LIGHTGRAY);
       
        char buffer[4] = { "000" };
        bin2asc(abs(i), buffer, 3);          
        Display.displayText(CHART_X + CHARTWIDTH - 3 * TEXTWIDTH - 2 , y - TEXTHEIGHT - 2, 1, buffer, LIGHTGRAY, BLACK);
      }
      
      // draw values *********************************
      uint8_t w = (CHARTWIDTH) / count - 1;
      for (uint8_t i = 0; i < count; i++) {
        uint8_t y = map(values[i], min, max, CHARTHEIGHT, CHART_Y);
        //Serial.println(String(values[i]) + ";" + String(y) + ";" + String(zero));
        if (y < zero) 
          Display.fillRect(i * w + CHART_X, y, w, zero - y, RED);
        else
          Display.fillRect(i * w + CHART_X, zero, w, y - zero, BLUE);
      }
    }

      
		
};

#endif
