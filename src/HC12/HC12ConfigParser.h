#pragma once

#include <string>
class HC12Config;

class HC12ConfigParser {
private:
    HC12ConfigParser();
public:
    //returns -1 on error
    static int parse_radio_fuse_AT(std::string);
    //returns -1 on error
    static int parse_radio_baudrate_AT(std::string);
    //returns -1 on error
    static int parse_radio_channel_AT(std::string);
    //returns -1 on error
    static int parse_radio_power_AT(std::string);
    //returns parsed
    static HC12Config parse_parameters_AT_response(std::string text);
};