#include <Earendil_ArduinoCompat.h>
#include <cstdio>

size_t EarendilSerial::write(uint8_t b){
  putchar(b);
  return 1;
}

int EarendilSerial::available(){
  return 0;
}

int EarendilSerial::read(){
  return 0;
}

int EarendilSerial::peek(){
  return 0;
}

EarendilSerial Serial;
