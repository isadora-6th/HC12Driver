#pragma once
#include <string>

/*Refer to .pdf nearby to know parameters*/

/*
    Struct that stores parameters:
*/
class HC12Config {
    /* Parses line in format: OK+FU3\r\nOK+B9600\r\nOK+C001\r\nOK+RP:+20dBm\r\n */
    void parse(std::string);
    int fuse;





    int baudrate;
    /*
    AT+B<baudrate>

    1200 bps     -- 5000bps in air (-117dBm sens) base * ~1.7
    2400 bps     --/
    4800 bps     --- 15000bps in air (-112dBm sens) base distance
    9600 bps     ---/
    19200 bps    ---- 58000bps in air (-107dBm sens) base / ~1.7
    38400 bps    ----/
    57600 bps    ----- 236000bps in air (-100dBm sens) base / ~4.5
    115200 bps   -----/

    returns OK+B<baudrate>
    */
    // [ 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 ]
    void set_radio_baudrate(int radio_channel);
    int  get_radio_baudrate();
    // OK+C001
    void parse_radio_baudrate(std::string);
    // AT+Cx
    std::string radio_baudrate_to_string();
    // x
    std::string radio_baudrate_to_human_readible_string();
/* ======================================================= */

private:
    int channel;
    /*
    AT+C<channel> [1-127] (>100 not reliable range)
    Close range (<10m -> use with spaces between used channels [skipping 1])
    ! doc has sth strange about nearby channels when short distance used

    returns COK+C<channel> [ %3b actually with starting zeros 21 -> 021]
    */
public:
    // [1-127]
    void set_radio_channel(int radio_channel);
    int  get_radio_channel();
    // OK+C001
    void parse_radio_channel(std::string);
    // AT+Cx
    std::string radio_channel_to_string();
    // x
    std::string radio_channel_to_human_readible_string();

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
    std::string radio_power_to_string();
    //x
    std::string radio_power_to_human_readable_string();
};