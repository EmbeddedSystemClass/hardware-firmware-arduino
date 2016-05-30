#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

// FT800 Register Addresses
#define REG_CLOCK             0x102408UL
#define REG_CMD_DL            0x1024ecUL
#define REG_CMD_READ          0x1024e4UL
#define REG_CMD_WRITE         0x1024e8UL
#define REG_CPURESET          0x10241cUL
#define REG_CSPREAD           0x102464UL
#define REG_DITHER            0x10245cUL
#define REG_DLSWAP            0x102450UL
#define REG_FRAMES            0x102404UL
#define REG_FREQUENCY         0x10240cUL
#define REG_GPIO              0x102490UL
#define REG_GPIO_DIR          0x10248cUL
#define REG_HCYCLE            0x102428UL
#define REG_HOFFSET           0x10242cUL
#define REG_HSIZE             0x102430UL
#define REG_HSYNC0            0x102434UL
#define REG_HSYNC1            0x102438UL
#define REG_ID                0x102400UL
#define REG_INT_EN            0x10249cUL
#define REG_INT_FLAGS         0x102498UL
#define REG_INT_MASK          0x1024a0UL
#define REG_MACRO_0           0x1024c8UL
#define REG_MACRO_1           0x1024ccUL
#define REG_OUTBITS           0x102458UL
#define REG_PCLK              0x10246cUL
#define REG_PCLK_POL          0x102468UL
#define REG_PLAY              0x102488UL
#define REG_PLAYBACK_FORMAT   0x1024b4UL
#define REG_PLAYBACK_FREQ     0x1024b0UL
#define REG_PLAYBACK_LENGTH   0x1024a8UL
#define REG_PLAYBACK_LOOP     0x1024b8UL
#define REG_PLAYBACK_PLAY     0x1024bcUL
#define REG_PLAYBACK_READPTR  0x1024acUL
#define REG_PLAYBACK_START    0x1024a4UL
#define REG_PWM_DUTY          0x1024c4UL
#define REG_PWM_HZ            0x1024c0UL
#define REG_RENDERMODE        0x102410UL
#define REG_ROTATE            0x102454UL
#define REG_SNAPSHOT          0x102418UL
#define REG_SNAPY             0x102414UL
#define REG_SOUND             0x102484UL
#define REG_SWIZZLE           0x102460UL
#define REG_TAG               0x102478UL
#define REG_TAG_X             0x102470UL
#define REG_TAG_Y             0x102474UL
#define REG_TAP_CRC           0x102420UL
#define REG_TAP_MASK          0x102424UL
#define REG_TOUCH_ADC_MODE    0x1024f4UL
#define REG_TOUCH_CHARGE      0x1024f8UL
#define REG_TOUCH_DIRECT_XY   0x102574UL
#define REG_TOUCH_DIRECT_Z1Z2 0x102578UL
#define REG_TOUCH_MODE        0x1024f0UL
#define REG_TOUCH_OVERSAMPLE  0x102500UL
#define REG_TOUCH_RAW_XY      0x102508UL
#define REG_TOUCH_RZ          0x10250cUL
#define REG_TOUCH_RZTHRESH    0x102504UL
#define REG_TOUCH_SCREEN_XY   0x102510UL
#define REG_TOUCH_SETTLE      0x1024fcUL
#define REG_TOUCH_TAG         0x102518UL
#define REG_TOUCH_TAG_XY      0x102514UL
#define REG_TOUCH_TRANSFORM_A 0x10251cUL
#define REG_TOUCH_TRANSFORM_B 0x102520UL
#define REG_TOUCH_TRANSFORM_C 0x102524UL
#define REG_TOUCH_TRANSFORM_D 0x102528UL
#define REG_TOUCH_TRANSFORM_E 0x10252cUL
#define REG_TOUCH_TRANSFORM_F 0x102530UL
#define REG_TRACKER           0x109000UL
#define REG_VCYCLE            0x10243cUL
#define REG_VOFFSET           0x102440UL
#define REG_VOL_PB            0x10247cUL
#define REG_VOL_SOUND         0x102480UL
#define REG_VSIZE             0x102444UL
#define REG_VSYNC0            0x102448UL
#define REG_VSYNC1            0x10244cUL

unsigned int triggerPin;			// Used for oscilloscope/logic analyzer trigger
unsigned int ft800irqPin;			// Interrupt from FT800 to Arduino - not used here
unsigned int ft800pwrPin;			// PD_N from Arduino to FT800 - effectively FT800 reset
unsigned int ft800csPin;

unsigned int lcdWidth   = 320;				// Active width of LCD display
unsigned int lcdHeight  = 240;				// Active height of LCD display
unsigned int lcdHcycle  = 429;				// Total number of clocks per line
unsigned int lcdHoffset = 20;				// Start of active line
unsigned int lcdHsync0  = 0;				// Start of horizontal sync pulse
unsigned int lcdHsync1  = 10;				// End of horizontal sync pulse
unsigned int lcdVcycle  = 263;				// Total number of lines per screen
unsigned int lcdVoffset = 10;				// Start of active screen
unsigned int lcdVsync0  = 1;				// Start of vertical sync pulse
unsigned int lcdVsync1  = 0;				// End of vertical sync pulse
unsigned int lcdPclk    = 8;				// Pixel Clock
unsigned int lcdSwizzle = 0;				// Define RGB output pins
unsigned int lcdPclkpol = 0;

void setup()
{
  triggerPin = 2;				// Used for oscilloscope/logic analyzer trigger
  ft800irqPin = 3;				// Interrupt from FT800 to Arduino - not used here
  ft800pwrPin = 4;				// PD_N from Arduino to FT800 - effectively FT800 reset
  ft800csPin  = 10;
  
  pinMode(triggerPin, OUTPUT);			// Arduino pin used for oscilloscope triggering
  pinMode(ft800irqPin, INPUT_PULLUP);		// FT800 interrupt output (not used in this example)
  pinMode(ft800pwrPin, OUTPUT);			// FT800 Power Down (reset) input
  pinMode(ft800csPin, OUTPUT);			// FT800 SPI bus CS# input
  
  digitalWrite(triggerPin, LOW);		// Initialize the oscilloscope trigger
  digitalWrite(ft800csPin, HIGH);		// Set CS# high to start - SPI inactive
  digitalWrite(ft800pwrPin, HIGH);		// Set PD# high to start
  delay(20);					// Wait a few MS before waking the FT800

//***************************************
// Wake-up FT800
  
  digitalWrite(ft800pwrPin, LOW);		// 1) lower PD#
  delay(20);					// 2) hold for 20ms
  digitalWrite(ft800pwrPin, HIGH);		// 3) raise PD#
  delay(20);	
  
  GD.begin(0);
  GD.wr(1057876UL, 0);
  //GD.wr16(1057836UL, 20);
  //GD.wr16(1057840UL, 320);
  //GD.wr16(1057860UL, 240);
  
  GD.wr16(REG_HSIZE,   lcdWidth);	// active display width
  GD.wr16(REG_HCYCLE,  lcdHcycle);	// total number of clocks per line, incl front/back porch
  GD.wr16(REG_HOFFSET, lcdHoffset);	// start of active line
  GD.wr16(REG_HSYNC0,  lcdHsync0);	// start of horizontal sync pulse
  GD.wr16(REG_HSYNC1,  lcdHsync1);	// end of horizontal sync pulse
  GD.wr16(REG_VSIZE,   lcdHeight);	// active display height
  GD.wr16(REG_VCYCLE,  lcdVcycle);	// total number of lines per screen, incl pre/post
  GD.wr16(REG_VOFFSET, lcdVoffset);	// start of active screen
  GD.wr16(REG_VSYNC0,  lcdVsync0);	// start of vertical sync pulse
  GD.wr16(REG_VSYNC1,  lcdVsync1);	// end of vertical sync pulse
  GD.wr(REG_SWIZZLE,  lcdSwizzle);	// FT800 output to LCD - pin order
  GD.wr(REG_PCLK_POL, lcdPclkpol);
  
  
//  GD.self_calibrate();
}

void loop()
{
//  GD.ClearColorRGB(0x103000);
  GD.ClearColorRGB(0, 0, 255);
  GD.Clear();
  GD.cmd_text(160, 120, 30, OPT_CENTER, "Hello world");
  
  GD.Begin(POINTS);
  GD.PointSize(50*16);
  GD.Vertex2ii(160, 50);
  GD.swap();
}
