#include <Arduino.h>
#include "HC12Config.h"
#include "HC12Driver.h"

void setup(){

    #define SET_PIN        16
    #define CHANNEL_TO_SET 14
    #define RADIO_SERIAL   Serial

    RADIO_SERIAL.begin(921600);

    HC12Driver driver(&RADIO_SERIAL, SET_PIN, HC12Driver::CONFIGURE);
    driver.setExpectedBaudrate(9600);
    driver.checkPresence();
    
    if( !driver.wasPresent() ){
        // device not connected (or not detected)
        return;
    }
 
    /* === Configuration Part ===*/
    HC12Config config = driver.readDeviceConfiguration();
    if( !config.is_valid() ){
        // Config read somehow failed, consider edit timings in HC12Driver.cpp
        // readDeviceConfiguration also edits wasPresent() state
    }

    config.set_radio_channel( CHANNEL_TO_SET );
    if( !driver.updateDeviceConfiguration(config) ){
        // Config change failed, consider edit timings in HC12Driver.cpp
        // updateDeviceConfiguration also edits getSavedDeviceConfiguration() state on success
    }

    driver.setState(HC12Driver::WORKING);
    config.set_radio_power( 7 );
    driver.updateDeviceConfiguration(config);

    #ifdef ESP32
        driver.getSerial()->updateBaudRate(921600);
    #endif
    #ifdef ESP8266
        driver.getSerial()->begin(921600);
    #endif
    #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        driver.getSerial()->begin(921600);
    #endif

    delay(150);

    driver.println("Hello my dear friend");
    driver.println("When state working => auto returns to working after update");   
    driver.println("All Serial functions now available from driver");

    driver.println(driver.getSavedDeviceConfiguration().to_string().c_str());    
}

void loop(){

}