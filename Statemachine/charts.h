// Charts ****************************************************************

#define TEMP_CHART_COUNT  24
#define DISPLAYHIGHT      84
#define DISPLAYWIDTH      48
#define MARGIN						 8

class Chart {  
  public:
    void drawChart(byte values[], byte count, int rangeMin, int rangeMax) {            
      int min = rangeMax;
			int max = rangeMin;
			
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
      
			// calculate delta and offset ******************
			
      byte delta = abs(max - min);      
      byte offset = DISPLAYHIGHT * min / d;

			// draw axis ***********************************
			// y-axis max, min
			Display.drawText(0, 0, rangeMax, 1, BLACK);
			Display.drawText(0, DISPLAYHEIGHT-TEXTHEIGHT, rangeMin, 1, BLACK);
			// y-axis
			Display.drawLine(2, TEXTHEIGHT, 2, DISPLAYHEIGHT-TEXTHEIGHT, BLACK);
			// x-axis
			byte y = DISPLAYHIGHT * 0 / delta - offset;


			// draw values *********************************

      
      
      for (byte i = 0; i < count; i++) {
        byte y = DISPLAYHIGHT * values[i] / delta - offset;    
//        Display.fillRect(i * 14, 0, 14, y, BLACK);
      }
    }

		
};

class TemperatureChartDiagram : public Chart {
  public:    
    byte values[TEMP_CHART_COUNT];
  
  public:
    void drawTempChart(byte input) {
      drawChart(values, TEMP_CHART_COUNT, -120, 120);
    }
    
    void assignValue(byte value) {
      byte i = 0;
      for (; i < TEMP_CHART_COUNT - 1; i++) {
        values[i] = values[i + 1];
      }
      values[i] = value;
    }
};

TemperatureChartDiagram TemperatureChart;
