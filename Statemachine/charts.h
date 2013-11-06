// Charts ****************************************************************

#define TEMP_CHART_COUNT  24
#define DISPLAYHEIGHT     48
#define DISPLAYWIDTH      84
#define MARGIN		   8
#define CHARTHEIGHT       32

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
			
      int delta = max - min;      
      int offset = CHARTHEIGHT * min / delta;

      // draw axis ***********************************
      // y-axis max, min
      char buffer[4] = { "000" };
      itochars(max, buffer, 3);
      Display.displayText(0, 0, 1, buffer);
      itochars(abs(min), buffer, 3);
      Display.displayText(0, DISPLAYHEIGHT-TEXTHEIGHT, 1, buffer);
      // y-axis
      Display.drawLine(2, TEXTHEIGHT, 2, DISPLAYHEIGHT-TEXTHEIGHT, BLACK);
      // x-axis
      Serial.println(String(min)+","+String(max)+","+String(offset));
      
      Display.drawLine(2, offset, DISPLAYWIDTH, offset, BLACK);
      
      // draw values *********************************

      
      
      for (byte i = 0; i < count; i++) {
        byte y = CHARTHEIGHT * values[i] / delta - offset;    
        Display.fillRect(i * 3 + 3, 16, 3, y, BLACK);
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
