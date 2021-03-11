#pragma once
#ifndef ESP8266
#include <string>
void delay(int);

class HardwareSerial {
    unsigned long baudrate;
    std::string target;
    int index;
public:
    HardwareSerial(int val);
    void begin(unsigned long baud);
    void end();

    int available();
    
    int read();
    int readBytes(uint8_t* buffer, int size);

    void write(int n);
    void write(std::string);

    void debug_target(std::string);
};
#endif