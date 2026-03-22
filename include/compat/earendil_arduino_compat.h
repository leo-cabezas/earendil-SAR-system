#pragma once

#include <Stream.h>

class EarendilSerial : public Stream {
  public:
    size_t write(uint8_t) override;
    int available() override;
    int read() override;
    int peek() override;
};

extern EarendilSerial Serial;
