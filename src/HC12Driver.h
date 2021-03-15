#pragma once

#include <Arduino.h>
#include "HC12Config.h"


/*
    Check ReadMe.md for usage
*/
class HC12Driver : public Stream {
public:
    /* Also works as HIGH/LOW for set pin */
    enum STATE : int {
        CONFIGURE = 0,
        WORKING = 1
    };
private:
    int set_pin;
    STATE state;
    HardwareSerial* serial;
    HC12Config current_config;    
    bool is_device_present;
    bool is_forced_presence;
public:

    HC12Driver(HardwareSerial*, int set_pin = 16, STATE state_start = CONFIGURE);

    HardwareSerial* getSerial();
    void setSerial(HardwareSerial*);
    /* Will be used for connections */
    void setExpectedBaudrate(int);

    /* Stops work, to check presence*/
    bool checkPresence();
    /* Return last state */
    bool wasPresent();

    int getSetPin();
    void setSetPin(int);

    /* Stops work, to get parameters */
    HC12Config readDeviceConfiguration();
    HC12Config getSavedDeviceConfiguration();

    /* Apply changes to external module if changes spoted */
    bool updateDeviceConfiguration(HC12Config);

    bool factoryReset();

    void setState(STATE);
    STATE getState();

public:
    /* Stream api */
    int available() override;
    int read() override;
    int peek() override;
    /* Print api */
    size_t write(uint8_t) override;

    #ifdef ESP32
        void flush() override;
    #endif

public:

    // Ignore presence state (if RX not connected, or manual work using buttons)
    void forcePresence(bool);
    bool isForcedPresence();
private:
    std::string send_AT(std::string at_string, int time_wait);
    void clean_input_output();
};