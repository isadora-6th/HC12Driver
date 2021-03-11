#include "HC12ConfigParser.h"

#ifdef ESP8266
    #include <Arduino.h>
    #include "stdlib_noniso.h"
    #define compat_std_stoi( IN_STRING, OUT_VAR ) OUT_VAR = atol(IN_STRING.c_str())

    #define HC12_CONFIG_PARSE_ERROR(X) Serial.println(std::string(X).c_str())
#else
    #include <stdexcept>
    #define compat_std_stoi( IN_STRING, OUT_VAR ) \
    try { \
        OUT_VAR = std::stoi(IN_STRING.c_str()); \
    } catch (std::invalid_argument& ia){ \
        OUT_VAR = 0; \
    }

    #include <iostream>
    #define HC12_CONFIG_PARSE_ERROR(X) std::cout << X << '\n'
#endif

HC12ConfigParser::HC12ConfigParser(){
    
}

int HC12ConfigParser::parse_radio_fuse_AT(std::string text){
    // OK+FUx
    int start = text.find("OK+FU");
    if( start == -1 ){
        HC12_CONFIG_PARSE_ERROR("fuse parsing error \"OK+FU\" not found");
        return -1;
    }
    std::string data = text.substr( start + 5, 1);

    int value;
    compat_std_stoi(data, value);

    if(value == 0){
        HC12_CONFIG_PARSE_ERROR(
            std::string("parse fuse error, can't process: ")+
            data
            );
        return -1;
    }
    return value;
}

int HC12ConfigParser::parse_radio_baudrate_AT(std::string text){
    // OK+Bxxxx

    int start = text.find("OK+B");
    if( start == -1 ){
        HC12_CONFIG_PARSE_ERROR("fuse parsing error \"OK+B\" not found");
        return -1;
    }
    int end = text.find(" ", start);
    std::string data = text.substr( start + 4, end - start);

    int value;
    compat_std_stoi(data, value);

    if(value == 0){
        HC12_CONFIG_PARSE_ERROR(
            std::string("parse baud error, can't process: ")+
            data
            );
        return -1;
    }
    return value;
}

int HC12ConfigParser::parse_radio_channel_AT(std::string text){
    // OK+Cxxxx

    int start = text.find("OK+C");
    if( start == -1 ){
        HC12_CONFIG_PARSE_ERROR("fuse parsing error \"OK+C\" not found");
        return -1;
    }
    int end = text.find(" ", start);
    std::string data = text.substr( start + 4, end - start);
    // data "001"

    int value;
    compat_std_stoi(data, value);

    if(value == 0){
        HC12_CONFIG_PARSE_ERROR(
            std::string("parse channel error, can't process: ")+
            data
            );
        return -1;
    }
    return value;
}

int HC12ConfigParser::parse_radio_power_AT(std::string text){
    // OK+Pxxxx

    int start = text.find("OK+RP:");
    if( start == -1 ){
        HC12_CONFIG_PARSE_ERROR("fuse parsing error \"OK+C\" not found");
        return -1;
    }
    int end = text.find(" ", start);
    std::string data = text.substr( start + 6, end - start);
    
    int value = -1;
    if( text.find("-1dBm") != -1 ){
        value = 1;
    } else if(text.find("+2dBm") != -1 ){
        value = 2;
    } else if(text.find("+5dBm") != -1 ){
        value = 3;
    } else if(text.find("+8dBm") != -1 ){
        value = 4;
    } else if(text.find("+11dBm") != -1 ){
        value = 5;
    } else if(text.find("+14dBm") != -1 ){
        value = 6;
    } else if(text.find("+17dBm") != -1 ){
        value = 7;
    } else if(text.find("+20dBm") != -1 ){
        value = 8;
    }

    if(value == -1){
        HC12_CONFIG_PARSE_ERROR(
            std::string("parse power error, can't process: ")+
            data
            );
        return -1;
    }
    return value;
}