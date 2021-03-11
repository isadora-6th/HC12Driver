#include "SerialHAL.h"
#include <iostream>
#ifndef ESP8266

void delay(int){
    
}

HardwareSerial::HardwareSerial(int val){
    baudrate = 0;
    target = "";
    index = 0;
}

void HardwareSerial::begin(unsigned long baud){
    this->baudrate = baud;
}

void HardwareSerial::end(){

}

int HardwareSerial::available(){
    return target.size() - index;
}

int HardwareSerial::read(){
    uint8_t out = target[index];
    index += 1;
    return out;
}

int HardwareSerial::readBytes(uint8_t* buffer, int length){
    int was_read = 0;
    while (was_read < length && available() > 0){
        buffer[was_read] = target[index];
        was_read += 1;
        index += 1;
    }
    return was_read;
}

void HardwareSerial::write(int n){
    std::cout << "n" << std::endl;
}

void HardwareSerial::write(std::string t){
    std::cout << t << std::endl;
}

void HardwareSerial::debug_target(std::string t){
    target = t;
    index = 0;
}
#endif