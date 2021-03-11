#include <Arduino.h>
#include "HC12/HC12Config.h"
#include "HC12/HC12Driver.h"


void setup(){

    #define SET_PIN        16
    #define CHANNEL_TO_SET 14
    #define RADIO_SERIAL   Serial

    HC12Driver driver(&Serial, SET_PIN, HC12Driver::CONFIGURE);
    driver.setExpectedBaudrate(9600);
    driver.checkPresence();
    
    if( !driver.wasPresent() ){
        // device not connected (or not detected)
        return;
    }

    /* === Configuration Part ===*/
    HC12Config config = driver.checkDeviceConfiguration();
    if( !config.is_valid() ){
        // Config read somehow failed, consider edit timings in HC12Driver.cpp
        // checkDeviceConfiguration also edits wasPresent() state
    }

    config.set_radio_channel( CHANNEL_TO_SET );
    if( !driver.updateDeviceConfiguration(config) ){
        // Config change failed, consider edit timings in HC12Driver.cpp
        // updateDeviceConfiguration also edits getSavedDeviceConfiguration() state on success
    }

    driver.setState(HC12Driver::WORKING);
    
    driver.println("Hello my dear friend");
    config.set_radio_power( 7 );
    driver.updateDeviceConfiguration(config);

    driver.println("When state working => auto returns to working after update");   
    driver.println("All Serial functions now available from driver");
    
}

void loop(){

}