#pragma once

#include <string>

class HardwareSerial {
    unsigned long baudrate;
    std::string target;
    int index;
public:
    HardwareSerial();
    void begin(unsigned long baud);
    void end();

    int available();
    
    int read();
    int readBytes(uint8_t* buffer, int size);

    void write(int n);

    void debug_target(std::string);
};