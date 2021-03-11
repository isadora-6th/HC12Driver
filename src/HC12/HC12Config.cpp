#include "HC12Config.h"
#include <algorithm>

#ifdef ESP8266
    #include <Arduino.h>
    #include "stdlib_noniso.h"
    #define compat_std_stoi( IN_STRING, OUT_VAR ) OUT_VAR = atol(IN_STRING.c_str())

    #define HC12_CONFIG_PARSE_ERROR(X) Serial.println(std::string(X).c_str())
    #define HC12_CONFIG_SET_OUT_OF_RANGE(X) Serial.println(std::string(X).c_str())
#else

    #define compat_std_stoi( IN_STRING, OUT_VAR ) OUT_VAR = std::stoi(IN_STRING.c_str())

    #include <iostream>
    #define HC12_CONFIG_PARSE_ERROR(X) std::cout << X << '\n'
    #define HC12_CONFIG_SET_OUT_OF_RANGE(X) std::cout << X << '\n'

#endif

void HC12Config::parse_parameters_AT_response(std::string text){
    // OK+FU3 \r\n    OK+B9600 \r\n   OK+C001 \r\n OK+RP：+20dBm \r\n

}

void HC12Config::set_default(){
    set_radio_fuse(3);
    set_radio_baudrate(9600);
    set_radio_channel(1);
    set_radio_power(8);
}

std::string HC12Config::radio_set_default_AT(){
    //.pdf page 8 -> 11
    return "AT+DEFAULT";
}

std::string HC12Config::radio_get_parameters_AT(){
    //.pdf page 7 -> 7
    return "AT+RX";
}

std::string HC12Config::to_string(){
    std::string out;
    out += radio_fuse_to_string();
    out += '\n';
    out += radio_baudrate_to_string();
    out += '\n';
    out += radio_channel_to_string();
    out += '\n';
    out += radio_power_to_string();
    out += '\n';
    return out;
}

/* === Individual parameter configuration below === */

void HC12Config::set_radio_fuse(int fuse){
    // [ 1, 2, 3, 4 ]
    if( fuse < 1 || fuse > 4 ){
        HC12_CONFIG_SET_OUT_OF_RANGE(
            std::string("Fuses only from [1-4] allowed not: ")+
            std::to_string(fuse)
            );
        return;
    }
    this->fuse = fuse;
}

int HC12Config::get_radio_fuse(){
    return fuse;
}

void HC12Config::parse_radio_fuse(std::string text){
    // OK+FUx

    int start = text.find("OK+FU");
    if( start == -1 ){
        HC12_CONFIG_PARSE_ERROR("fuse parsing error \"OK+FU\" not found");
        return;
    }
    std::string data = text.substr( start + 5, 1);

    int value;
    compat_std_stoi(data, value);

    if(value == -1){
        return;
    }
    fuse = value;
}

std::string HC12Config::radio_fuse_AT_string(){
    //.pdf page 6 -> 4
    std::string out = "AT+FU";
    out += std::to_string(fuse);
    return out;
}

std::string HC12Config::radio_fuse_to_string(){
    return std::to_string(fuse);
}

/* ======================================================= */

void HC12Config::set_radio_baudrate(int baudrate){
    // [ 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 ]
    switch (baudrate) {
    case 1200:
    case 2400:
    case 4800:
    case 9600:
    case 19200:
    case 38400:
    case 57600:
    case 115200:
        break;
    default:
        HC12_CONFIG_SET_OUT_OF_RANGE(
            std::string("Baudrate not allowed: ")+
            std::to_string(baudrate));
        return;
    }
    this->baudrate = baudrate;
}

int  HC12Config::get_radio_baudrate(){
    return baudrate;
}

void HC12Config::parse_radio_baudrate(std::string text){
    // TODO:
}

std::string HC12Config::radio_baudrate_AT_string(){
    //.pdf page 6 -> 2
    std::string out = "AT+B";
    out += std::to_string(baudrate);
    return out;
}

std::string HC12Config::radio_baudrate_to_string(){
    return std::to_string(baudrate);
}

/* ======================================================= */

void HC12Config::set_radio_channel(int radio_channel){
    // [1-127]
    if( radio_channel < 1 || radio_channel > 127 ){
        HC12_CONFIG_SET_OUT_OF_RANGE(
            std::string("Channel not allowed [1 - 127]: ")+
            std::to_string(radio_channel));
        return;
    }
    this->channel = radio_channel;
}

int  HC12Config::get_radio_channel(){
    return channel;
}

void HC12Config::parse_radio_channel(std::string text){
    // TODO:
}

std::string HC12Config::radio_channel_AT_string(){
    std::string out = "AT+C";
    int value1 = (channel / 100) % 10;
    int value2 = (channel / 10) % 10;
    int value3 = channel % 10;
    out += std::to_string(value1);
    out += std::to_string(value2);
    out += std::to_string(value3);
    return out;
}

std::string HC12Config::radio_channel_to_string(){
    return std::to_string(channel);
}

/* ======================================================= */

void HC12Config::set_radio_power(int radio_power){
    // [1-8] 
    if( radio_power < 1 || radio_power > 8 ){
        HC12_CONFIG_SET_OUT_OF_RANGE(
            std::string("Radio power not allowed [1 - 8]: ")+
            std::to_string(radio_power));
        return;
    }
    this->radio_power = radio_power;
}

int  HC12Config::get_radio_power(void){
    return radio_power;
}

void HC12Config::parse_radio_power(std::string text){
    // TODO:
}

std::string HC12Config::radio_power_AT_string(){
    //.pdf page 7 -> 5
    std::string out = "AT+P";
    out += std::to_string(radio_power);
    return out;
}

std::string HC12Config::radio_power_to_string(){
    return std::to_string(radio_power);
}
