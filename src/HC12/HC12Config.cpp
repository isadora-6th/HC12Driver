#include "HC12Config.h"
#include <algorithm>

#ifdef ESP8266
    #include <Arduino.h>
#else
    #include <stdexcept>
    #include <iostream>
#endif

bool HC12Config::parse_parameters_AT_response(std::string text){
    // OK+FU3 \r\n    OK+B9600 \r\n   OK+C001 \r\n OK+RP：+20dBm \r\n
    HC12Config in = HC12ConfigParser::parse_parameters_AT_response(text);
    this->fuse = in.fuse;
    this->baudrate = in.baudrate;
    this->channel = in.channel;
    this->radio_power = in.radio_power;

    return is_valid();
}

void HC12Config::set_factory_default(){
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
    out += "Fuse: ";
    out += radio_fuse_to_string();
    out += '\n';
    out += "Baud: ";
    out += radio_baudrate_to_string();
    out += '\n';
    out += "Channel: ";
    out += radio_channel_to_string();
    out += '\n';
    out += "Power: ";
    out += radio_power_to_string();
    out += '\n';
    return out;
}

bool HC12Config::is_valid(){
    return is_valid_radio_fuse(fuse) &&
           is_valid_radio_baudrate(baudrate) &&
           is_valid_radio_channel(channel) &&
           is_valid_radio_power(radio_power);
}

bool HC12Config::operator==(const HC12Config& other){
    return 
        fuse == other.fuse &&
        baudrate == other.baudrate &&
        channel == other.channel &&
        radio_power == other.radio_power;
}

/* === Individual parameter configuration below === */

void HC12Config::set_radio_fuse(int fuse){
    // [ 1, 2, 3, 4 ]
    this->fuse = fuse;
}

int HC12Config::get_radio_fuse(){
    return fuse;
}

bool HC12Config::is_valid_radio_fuse(int fuse){
    return fuse > 1 && fuse < 5;
}

void HC12Config::parse_radio_fuse_AT(std::string text){
    int value = HC12ConfigParser::parse_radio_fuse_AT(text);
    set_radio_fuse(value);
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
    this->baudrate = baudrate;
}

int  HC12Config::get_radio_baudrate(){
    return baudrate;
}
int  HC12Config::get_factory_default_baudrate(){
    return 9600;
}

bool HC12Config::is_valid_radio_baudrate(int baudrate){
    switch (baudrate) {
        case 1200:
        case 2400:
        case 4800:
        case 9600:
        case 19200:
        case 38400:
        case 57600:
        case 115200:
            return true;
    default:
        return false;
    }
}

void HC12Config::parse_radio_baudrate_AT(std::string text){
    int value = HC12ConfigParser::parse_radio_baudrate_AT(text);
    set_radio_baudrate(value);
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
    this->channel = radio_channel;
}

int  HC12Config::get_radio_channel(){
    return channel;
}

bool HC12Config::is_valid_radio_channel(int radio_channel){
    return radio_channel > 0 && radio_channel < 128;
}

void HC12Config::parse_radio_channel_AT(std::string text){
    int value = HC12ConfigParser::parse_radio_baudrate_AT(text);
    set_radio_channel(value);
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
    this->radio_power = radio_power;
}

int  HC12Config::get_radio_power(void){
    return radio_power;
}

bool HC12Config::is_valid_radio_power(int radio_power){
    return radio_power > 0 && radio_power < 9;
}

void HC12Config::parse_radio_power_AT(std::string text){
    int value = HC12ConfigParser::parse_radio_baudrate_AT(text);
    set_radio_power(value);
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
