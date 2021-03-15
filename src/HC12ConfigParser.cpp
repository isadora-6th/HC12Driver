#include "HC12ConfigParser.h"
#include "HC12Config.h"

#ifdef ESP8266
    #include <Arduino.h>
    #include "stdlib_noniso.h"
    #define compat_std_stoi( IN_STRING, OUT_VAR ) OUT_VAR = atol(IN_STRING.c_str())

    #define HC12_CONFIG_PARSE_ERROR(X)// Serial.println(std::string(X).c_str())
#endif
#ifdef ESP32
    #include <Arduino.h>
    #include "stdlib_noniso.h"
    #define compat_std_stoi( IN_STRING, OUT_VAR ) OUT_VAR = atol(IN_STRING.c_str())

    #define HC12_CONFIG_PARSE_ERROR(X)// Serial.println(std::string(X).c_str())
#endif
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    #include <stdlib.h>
    #define compat_std_stoi( IN_STRING, OUT_VAR ) OUT_VAR = atol(IN_STRING.c_str())
    #define HC12_CONFIG_PARSE_ERROR(X)// Serial.println(std::string(X).c_str())
#endif

HC12ConfigParser::HC12ConfigParser(){

}

long HC12ConfigParser::parse_radio_fuse_AT(std::string text){
    // OK+FUx
    std::size_t start = text.find("OK+FU");
    if( start == std::string::npos ){
        HC12_CONFIG_PARSE_ERROR("fuse parsing error \"OK+FU\" not found");
        return -1;
    }
    std::string data = text.substr( start + 5, 1);

    long value;
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

long HC12ConfigParser::parse_radio_baudrate_AT(std::string text){
    // OK+Bxxxx

    std::size_t start = text.find("OK+B");
    if( start == std::string::npos ){
        HC12_CONFIG_PARSE_ERROR("baudrate parsing error \"OK+B\" not found");
        return -1;
    }
    std::size_t end = text.find(" ", start);
    if(end == std::string::npos){
        end = text.length();
    }
    std::string data = text.substr( start + 4, end - start);

    long value;
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

long HC12ConfigParser::parse_radio_channel_AT(std::string text){
    // OK+Cxxxx
    
    std::size_t start = text.find("OK+RC");
    if( start == std::string::npos ){
        HC12_CONFIG_PARSE_ERROR("channel parsing error \"OK+C\" not found");
        return -1;
    }
    
    std::size_t end = text.find(" ", start);
    if(end == std::string::npos){
        end = text.length();
    }
    std::string data = text.substr( start + 5, end - start);
    // data "001"

    long value;
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

long HC12ConfigParser::parse_radio_power_AT(std::string text){
    // OK+Pxxxx

    std::size_t start = text.find("OK+RP:");
    if( start == std::string::npos ){
        HC12_CONFIG_PARSE_ERROR("radio_power parsing error \"OK+RP:\" not found");
        return -1;
    }
    std::size_t end = text.find(" ", start);
    if(end == std::string::npos){
        end = text.length();
    }
    std::string data = text.substr( start + 6, end - start);
    
    long value = -1;
    if( text.find("-1dBm") != std::string::npos ){
        value = 1;
    } else if(text.find("+2dBm") != std::string::npos ){
        value = 2;
    } else if(text.find("+5dBm") != std::string::npos ){
        value = 3;
    } else if(text.find("+8dBm") != std::string::npos ){
        value = 4;
    } else if(text.find("+11dBm") != std::string::npos ){
        value = 5;
    } else if(text.find("+14dBm") != std::string::npos ){
        value = 6;
    } else if(text.find("+17dBm") != std::string::npos ){
        value = 7;
    } else if(text.find("+20dBm") != std::string::npos ){
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


HC12Config HC12ConfigParser::parse_parameters_AT_response(std::string text){

    // OK+FU3 \r\n    OK+B9600 \r\n   OK+C001 \r\n OK+RP：+20dBm \r\n
    int found_index = 0;
    std::size_t begin = 0;

    HC12Config out;

    while ( ( begin = text.find("OK+", begin) ) != std::string::npos){
        std::size_t end = text.find("\n", begin);
        if(end == std::string::npos){
            end = text.length();
        }

        switch (found_index){
        case 0:
            {
                long got = parse_radio_baudrate_AT( text.substr(begin, end - begin) );
                out.set_radio_baudrate(got);

            }
            break;
        case 1:
            {
                long got = parse_radio_channel_AT( text.substr(begin, end - begin) );
                out.set_radio_channel(got);
            }
            break;
        case 2:
            {
                long got = parse_radio_power_AT( text.substr(begin, end - begin) );
                out.set_radio_power(got);
            }
            break;
        case 3:
            {
                long got = parse_radio_fuse_AT( text.substr(begin, end - begin) );
                out.set_radio_fuse(got);
            }
            break;
        
        default:
            return out;
        }
        found_index += 1;
        begin += 1;
    }
    return out;
}