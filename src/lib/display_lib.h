#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>
#include <string>
#include <cmath>

#define X_OFFSET 120
#define Y_OFFSET 120
#define SINE_OFFSET -1

#define TFT_CS       25
#define TFT_DC       24
#define TFT_RST      12
#define TFT_MOSI     3 
#define TFT_MISO     0
#define TFT_SCLK     2

class Text {
  public:
    std::string text;
    unsigned int cursorX;
    unsigned int cursorY;
    unsigned int colorPrint;
    unsigned int colorClear;
    unsigned int size;
    
    Text(std::string text, unsigned int cursorX, unsigned int cursorY, unsigned int colorPrint, unsigned int colorClear, unsigned int size);
      
    void print(Adafruit_GC9A01A tft);

    void clear(Adafruit_GC9A01A tft);
};

class Compass {
  public:
    unsigned int north;
    unsigned int colorPrint;
    unsigned int size;
    
    compass(unsigned int north, unsigned int colorPrint, unsigned int size);
      
    void print_compass(Adafruit_GC9A01A tft);

    void clear_compass(Adafruit_GC9A01A tft);
};

class Notches{
    public:
        float notch_rad;
        float notch_angle;

    void draw_notches();

    void fill_notch();

    void clear_notch(int notch_num);
}

class Thermometer{
    public:
        float temp;
    thermometer(float temp);

    void draw_thermometer();

    void print_temp();

}

class Digiclock{ //make inherit from text
    public:
        unsigned int hour;
        unsigned int minute;

    digi_clock(int hour, int minute);

    void draw_clock();

    void update_time();

    void print_time();
}
#endif // DISPLAY-LIB_H
