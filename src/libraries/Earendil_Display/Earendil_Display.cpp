#include <Earendil_Display.h>

Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);

void vDisplay(void* pvParameters){
  (void) pvParameters;

  vTaskDelay(pdMS_TO_TICKS(5000));
  
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(GC9A01A_WHITE);
  
  while (1){
    tft.setCursor(40, 160);
    tft.setTextColor(GC9A01A_BLACK);
    tft.setTextSize(3);
    tft.print("DER JAGER");
    vTaskDelay(pdMS_TO_TICKS(1000));
    tft.fillScreen(GC9A01A_WHITE);
  }
}
