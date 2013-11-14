#ifndef _CHARTSH_
#define _CHARTSH_

// Charts ****************************************************************

#define TEMP_CHART_COUNT  24

#define DISPLAYHEIGHT     ST7735_TFTWIDTH
#define DISPLAYWIDTH      ST7735_TFTHEIGHT
#define MARGIN		   8
#define CHARTHEIGHT       112
#define CHARTWIDTH        138
#define CHART_X            22
#define CHART_Y            12

class Chart {    
  public:
    void drawChart(int8_t values[], byte count, int rangeMin, int rangeMax) {
      int8_t min = rangeMax;
      int8_t max = rangeMin;
			
			// find min and max values *********************
      for (byte i = 0; i < count; i++) {
        min = min(values[i], min);
        max = max(values[i], max);
      }
      
      // fit min, max in range of 10th steps *********
      for (int i = 0; i > rangeMin; i -= 10) {
        if (i < min) {
          min = i;
          break;
        }
      }
      for (int i = 0; i < rangeMax; i += 10) {
        if (i > max) {
          max = i;
          break;
        }
      }
      
      // range ***************************************
      byte range = max - min;
      byte zero = map(0, min, max, CHARTHEIGHT, CHART_Y);
      
      // draw values *********************************
      byte w = (CHARTWIDTH) / count - 1;
      for (byte i = 0; i < count; i++) {
        byte y = map(values[i], min, max, CHARTHEIGHT, CHART_Y);
        //Serial.println(String(values[i]) + ";" + String(y) + ";" + String(zero));
        if (y < zero) 
          Display.fillRect(i * w + CHART_X, y, w, zero - y, ST7735_RED);
        else
          Display.fillRect(i * w + CHART_X, zero, w, y - zero, ST7735_BLUE);
      }
      
      // y-axis
      Display.drawLine(CHART_X, CHART_Y - 3, CHART_X, CHARTHEIGHT + 3, ST7735_WHITE);
      //Display.fillTriangle(CHART_X, CHART_Y - 10, CHART_X - 3, CHART_Y - 3, CHART_X + 3, CHART_Y - 3, ST7735_WHITE);
      for (int8_t i = min; i <= max; i+=5) {
        byte y = map(i, min, max, CHARTHEIGHT, CHART_Y);
        Display.drawLine(CHART_X - 2, y, CHART_X, y, BLACK);
        if ( i % 10 == 0 ) {
          char buffer[4] = { "000" };
          itochars(abs(i), buffer, 3);
          Display.displayText(CHART_X - 3 * TEXTWIDTH - 2, y - TEXTHEIGHT / 2, 1, buffer, ST7735_WHITE, ST7735_BLACK);
        }
      }
      
      // x-axis (zero line)
      Display.drawLine(CHART_X, zero, CHARTWIDTH, zero, ST7735_WHITE);
      
      for(byte i = 0; i < count; i++) {
        Display.drawLine(CHART_X + i * w, zero + 2, CHART_X + i * w, zero, ST7735_WHITE); 
      }
      
      Display.displayText_f(CHARTWIDTH , zero - TEXTHEIGHT / 2, 1, PSTR("t"));
    }

		
};

class TemperatureChartDiagram : public Chart {
  public:    
    int8_t values[TEMP_CHART_COUNT];
  
  public:
    void drawTempChart(byte input) {
      Display.displayText_f(45, 5, 1, PSTR("Temperature"));
      drawChart(values, TEMP_CHART_COUNT, -120, 120);
    }
    
    void assignValue(int8_t value) {
      byte i = 0;
      for (; i < TEMP_CHART_COUNT - 1; i++) {
        values[i] = values[i + 1];
      }
      values[i] = value;
    }
};

#endif
