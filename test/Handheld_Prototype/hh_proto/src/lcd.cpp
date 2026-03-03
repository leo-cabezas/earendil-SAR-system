#include "../includes/lcd.h"
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

#define TFT_CS   25
#define TFT_DC   24
#define TFT_RST  12
#define TFT_MOSI 15
#define TFT_MISO 8
#define TFT_SCK  14
#define X_OFFSET 120
#define Y_OFFSET 120
#define OUTLINE_BLU 0x2CB6
#define OUTLINE_DARKBLU 0x1188

Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS,TFT_DC,TFT_RST);//initialization for the adafruit screen structure
GFXcanvas16 canvas(240, 240);

void lcdSetup(){ //LCD Screen Setup Function
  tft.begin();
  tft.setRotation(2);//0-3, 0 being orientated to view with the pins at the bottom
  tft.fillScreen(GC9A01A_BLACK);
}

void lcdWrite(moduleData& sensorData){ //Print data to the LCD Screen
  tft.fillScreen(GC9A01A_BLACK);
  tft.setCursor(50, 50);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(3);//scale the text to be bigger
  tft.print(sensorData.bmpMetrics[0], 2);//prints out the temperature in Celsius to 2 decimal places
  tft.setCursor(50, 90);
  tft.print(sensorData.bmpMetrics[2], 2);
  tft.setCursor(50, 130);
  tft.print(sensorData.magMetrics[3],2);
}