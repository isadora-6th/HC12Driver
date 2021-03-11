#pragma once

#include <Arduino.h>
#include "HC12/HC12Config.h"


/*
    Expected usage:
    
    #define SET_PIN        16;
    #define CHANNEL_TO_SET 14;
    #define RADIO_SERIAL   Serial;

    HC12Driver driver(Serial, SET_PIN, HC12Driver::CONFIGURE);
    driver.setExpectedBaudrate(9600);
    driver.checkPresence();

    HC12Config config;

    if(driver.isPresent()){
        config = readDeviceConfiguration();
        if(config.is_valid()){
            config.set_radio_channel( CHANNEL_TO_SET  );
            if( !driver.updateDeviceConfiguration(config) ){
                // Config change failed
            }
        } else {
            // Config read somehow failed
        }
    } else {
        // device not connected
    }
    
    driver.setState(HC12Driver::WORKING);
    
    driver.println("Hello my dear friend");
    config.set_radio_power( 7 );
    driver.updateDeviceConfiguration(config);

    driver.println("When state working => auto returns to working after update");   
    driver.println("All Serial functions now available from driver");

*/
class HC12Driver : public Stream {
public:
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
    enum STATE : int {
        CONFIGURE = 0,
        WORKING = 1
    };

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

    void factoryReset();

    void setState(STATE);
    STATE getState();

public:
    /* Stream api */
    int available() override;
    int read() override;
    int peek() override;
    /* Print api */
    size_t write(uint8_t) override;

public:

    // Ignore presence state (if RX not connected, or manual work using buttons)
    void forcePresence(bool);
    bool isForcedPresence();
};