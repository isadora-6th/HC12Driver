#pragma once
#include <string>

/*
    Struct that stores parameters:
    - Fuse
    - Channel
    - Power
    - Baudrate

    Only parses and converts to device readable strings 
    not working with actual device, use overlay instead.

    Based on HC-12_english_datasheets.pdf nearby
*/
class HC12Config {
public:
    /* Parses line in format:*/
    // OK+FU3 \r\n    OK+B9600 \r\n   OK+C001 \r\n OK+RP：+20dBm \r\n
    void parse_parameters_AT_response(std::string);
    
    /* factory default setting, FU3, Baud 9600, Channel 1, Power 8 */
    void set_default();

    std::string radio_set_default_AT();
    std::string radio_get_parameters_AT();

    std::string to_string();

/* === Individual parameter configuration below === */

private:
    int fuse;
    /*
    AT+FUx
    FU1: low current(3.6mA), short comm distance
    FU2: B1200, B2400, B4800 only 20 byte max (2 sec CD) 0.08 mA idle
    FU3: default
    FU4: B1200 only, 60 bytes (2 sec CD), ultra long range

    return OK+FU<x>;
    */
public:
    // [ 1, 2, 3, 4 ]
    void set_radio_fuse(int fuse);
    int  get_radio_fuse();
    // OK+FUx
    void parse_radio_fuse(std::string);
    // AT+FUx
    std::string radio_fuse_AT_string();
    // out fuse FUx
    std::string radio_fuse_to_string();

/* ======================================================= */

private:
    int baudrate;
    /*
    AT+B<baudrate>

    1200 bps     -- 5000bps in air (-116dBm sens) base * ~1.7
    2400 bps     --/
    4800 bps     --- 15000bps in air (-111dBm sens) base distance
    9600 bps     ---/
    19200 bps    ---- 58000bps in air (-106dBm sens) base / ~1.7
    38400 bps    ----/
    57600 bps    ----- 236000bps in air (-100dBm sens) base / ~4.5
    115200 bps   -----/

    returns OK+B<baudrate>
    */
public:
    // [ 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 ]
    void set_radio_baudrate(int baudrate);
    int  get_radio_baudrate();
    // OK+C001
    void parse_radio_baudrate(std::string);
    // AT+Cx
    std::string radio_baudrate_AT_string();
    // out baudrate value
    std::string radio_baudrate_to_string();

/* ======================================================= */

private:
    int channel;
    /*
    AT+C<channel> [1-127] (>100 not reliable range)
    Close range (<10m -> use with spaces between used channels [skipping 1])
    ! doc has sth strange about nearby channels when short distance used

    returns OK+RC<channel> [ %3b actually with starting zeros 21 -> 021]
    */
public:
    // [1-127]
    void set_radio_channel(int radio_channel);
    int  get_radio_channel();
    // OK+C001
    void parse_radio_channel(std::string);
    // AT+Cx
    std::string radio_channel_AT_string();
    // out radio channel setting
    std::string radio_channel_to_string();

/* ======================================================= */

private:
    int radio_power;
    /*
        AT+P<radio_power> [1-8]
        where <radio_power> = 1 number:
        1= :-1dBm (0,8 мВт)
        2= :+2dBm (1,6 мВт)
        3= :+5dBm (3,2 мВт)
        4= :+8dBm (6,3 мВт)
        5= :+11dBm (12 мВт)
        6= :+14dBm (25 мВт)
        7= :+17dBm (50 мВт)
        8= :+20dBm (100 мВт)

        returns OK+RP:<+20dBm for 8>
    */
public:
    // [1-8]
    void set_radio_power(int radio_power);
    int  get_radio_power(void);
    // OK+RP:+20dBm
    void parse_radio_power(std::string);
    //AT+Px
    std::string radio_power_AT_string();
    //out number for power setting
    std::string radio_power_to_string();
};