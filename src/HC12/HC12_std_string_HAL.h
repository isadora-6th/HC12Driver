#pragma once

#ifdef ESP8266
    #include <string>
    #include <algorithm>
#endif

#ifdef ESP32
    #include <string>
    #include <algorithm>
    namespace std {
        std::string to_string(long);
    }
    
#endif
/*
    Arduino nano compat problems occured so...
    I wrote my std::string to work...
*/

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Code in here will only be compiled if an Arduino Uno (or older) is used.
#include <Arduino.h>
#include <stddef.h>

namespace std {
    typedef size_t size_t;

    class string {
        String arduino_string;
    public:    
        static const std::size_t npos;
        string();
        string(const char*);
        string(char*);
        std::size_t find(const char*);
        std::size_t find(const char*, std::size_t pos);
        
        string substr(std::size_t from, std::size_t count);
        std::size_t length();

        const char* c_str();

        string operator+(const string& other);
        string operator+(const char* c_text);
        string operator+=(const string& other);
        string operator+=(const char& other);

        string operator=(const String& other);
    };

    string to_string(int);
    string to_string(long);
}

#endif