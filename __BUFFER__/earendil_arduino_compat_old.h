#pragma once

#include <cstdio>
#include <cstdint>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

struct EarendilSerial {
    inline void print(int value)        { print((int32_t)value); }
    inline void println(int value)      { println((int32_t)value); }

    // ============================
    // Canonical integer types
    // ============================
    inline void print(int32_t value, int base = DEC) {
        if (base == DEC)      printf("%d", value);
        else if (base == HEX) printf("%X", value);
        else if (base == OCT) printf("%o", value);
        else if (base == BIN) {
            for (int i = sizeof(int32_t) * 8 - 1; i >= 0; --i)
                printf("%c", (value & (1 << i)) ? '1' : '0');
        }
    }

    inline void print(uint32_t value, int base = DEC) {
        if (base == DEC)      printf("%u", value);
        else if (base == HEX) printf("%X", value);
        else if (base == OCT) printf("%o", value);
        else if (base == BIN) {
            for (int i = sizeof(uint32_t) * 8 - 1; i >= 0; --i)
                printf("%c", (value & (1u << i)) ? '1' : '0');
        }
    }

    inline void println(int32_t value, int base = DEC) { print(value, base); printf("\n"); }
    inline void println(uint32_t value, int base = DEC) { print(value, base); printf("\n"); }

    // ============================
    // Funnel smaller integer types
    // ============================
    inline void print(int8_t value, int base = DEC)   { print((int32_t)value, base); }
    inline void print(int16_t value, int base = DEC)  { print((int32_t)value, base); }
    inline void print(uint8_t value, int base = DEC)  { print((uint32_t)value, base); }
    inline void print(uint16_t value, int base = DEC) { print((uint32_t)value, base); }

    inline void println(int8_t value, int base = DEC)   { println((int32_t)value, base); }
    inline void println(int16_t value, int base = DEC)  { println((int32_t)value, base); }
    inline void println(uint8_t value, int base = DEC)  { println((uint32_t)value, base); }
    inline void println(uint16_t value, int base = DEC) { println((uint32_t)value, base); }

    // ============================
    // Floating point types
    // ============================
    inline void print(float value, int precision = 2)  { printf("%.*f", precision, value); }
    inline void print(double value, int precision = 2) { printf("%.*f", precision, (float)value); }

    inline void println(float value, int precision = 2)  { print(value, precision); printf("\n"); }
    inline void println(double value, int precision = 2) { print(value, precision); printf("\n"); }

    // ============================
    // Char and string types
    // ============================
    inline void print(char c)        { printf("%c", c); }
    inline void println(char c)      { printf("%c\n", c); }
    inline void print(const char* s) { printf("%s", s); }
    inline void println(const char* s) { printf("%s\n", s); }
    inline void println() { printf("\n"); }

    // ============================
    // Raw byte write (like Arduino Serial.write)
    // ============================

    // Write a single byte
    inline size_t write(uint8_t byte) {
        putchar(byte);
        return 1;
    }

    // Write a buffer of bytes
    inline size_t write(const uint8_t* buffer, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            putchar(buffer[i]);
        }
        return size;
    }

    // Write a C-string as raw bytes (no formatting)
    inline size_t write(const char* str) {
        size_t count = 0;
        while (*str) {
            putchar(*str++);
            ++count;
        }
        return count;
    } 
};

extern EarendilSerial Serial;
