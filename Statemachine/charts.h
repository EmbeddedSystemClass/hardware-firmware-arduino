// Charts ****************************************************************

#define TEMP_CHART_COUNT  24
#define DISPLAYHEIGHT     48
#define DISPLAYWIDTH      84
#define MARGIN		   8
#define CHARTHEIGHT       47
#define CHARTWIDTH        76
#define CHART_X            8
#define CHART_Y            0

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
      
      // draw axis ***********************************
      // y-axis max, min
//      char buffer[4] = { "000" };
//      itochars(max, buffer, 3);
//      Display.displayText(0, 0, 1, buffer);
//      itochars(abs(min), buffer, 3);
//      Display.displayText(0, DISPLAYHEIGHT-TEXTHEIGHT, 1, buffer);

      // range ***************************************
      byte range = max - min;
      // y-axis
      Display.drawLine(CHART_X, CHART_Y, CHART_X, CHARTHEIGHT + CHART_Y, BLACK);
      for (int8_t i = min; i <= max; i+=5) {
        byte y = (i - min) * CHARTHEIGHT / range;
        Display.drawLine(0, y, CHART_X, y, BLACK);
      }
      
      // x-axis (zero line)
      byte zero = (0 - min) * CHARTHEIGHT / range;
      Display.drawLine(CHART_X, zero, CHARTWIDTH, zero, BLACK);
      
      // draw values *********************************
      byte w = (CHARTWIDTH) / count;
      for (byte i = 0; i < count; i++) {
        byte y = (values[i] - min) * CHARTHEIGHT / range;
        Display.fillRect(i * w + CHART_X, zero, w, y - zero, BLACK);
      }
    }

		
};

class TemperatureChartDiagram : public Chart {
  public:    
    int8_t values[TEMP_CHART_COUNT];
  
  public:
    void drawTempChart(byte input) {
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

TemperatureChartDiagram TemperatureChart;
