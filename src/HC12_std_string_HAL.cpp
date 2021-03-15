#include "HC12_std_string_HAL.h"

#ifdef ESP32
    #include <stdio.h>
    namespace std {
        std::string to_string(long value){
            char buf[2 + 8 * sizeof(long)];
            sprintf(buf, "%ld", value);
            return std::string(buf);
        }
    }
    
#endif




#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Code in here will only be compiled if an Arduino Uno (or older) is used.
#include <Arduino.h>
#include <stddef.h>

namespace std {
    const std::size_t std::string::npos = 0xFFFF;

    string::string(){
        arduino_string = "";
    }
    string::string(const char* in){
        arduino_string = String(in);
    }
    string::string(char* in){
        arduino_string = String(in);
    }

    std::size_t string::find(const char* in){
        return find(in, 0);
    }

    std::size_t string::find(const char* in, std::size_t pos){
        int index = arduino_string.indexOf(in,pos);
        if(index == -1){
            return npos;
        }
        return index;
    }
    
    string string::substr(std::size_t from, std::size_t count){
        String s = arduino_string.substring(from, from + count);
        return string(s.c_str());
    }

    std::size_t string::length(){
        return arduino_string.length();
    }

    const char* string::c_str(){
        return arduino_string.c_str();
    }

    string string::operator+(const string& other){
        arduino_string = (arduino_string + other.arduino_string);
        return *this;
    }

    string string::operator+(const char* c_text){
        arduino_string = (arduino_string + c_text);
        return *this;
    }
    string string::operator+=(const string& other){
        arduino_string += other.arduino_string;
        return *this;
    }
    string string::operator+=(const char& other){
        arduino_string += other;
        return *this;
    }

    string string::operator=(const String& other){
        arduino_string = other;
        return *this;
    }

    string to_string(int in){
        String s = String(in);
        return string(s.c_str());
    }
    string to_string(long in){
        String s = String(in);
        return string(s.c_str());
    }
}

#endif