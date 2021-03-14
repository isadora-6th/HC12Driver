# HC-12 Driver library

This library was made to surely configure HC-12 module on the go and use it without bothering about `Serial + Set_Pin` state.  
Made because local solution (R8) had problems with portability and reliability.  
Also large testing expected for this library. (Like using all set_configures and recheck all state change failure causes).

Target framework:  
- `Arduino Framework`

Target platfroms:
- `Esp8266`
    > Done and tested
- `Esp32`
    > Done, **NOT** tested
- `Arduino nano`
    > Done, **NOT** tested (done port std::string)

# Usage
Refer to `src/HC12/HC12Config.h` and `src/HC12/HC12Driver.h` for detailed info.  
Actually `HC12Driver` class replaces `Serial` usage for HC-12 communication.


# Example

```cpp

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
    

```

# Note
This lib not expected to use minimal `RAM` (low but not minimal possible), or `Flash Size` (A lot will be expected).  
Also compatibility issues expected ( Ardino::String <=> std::string );

Most portable part of program is:
- `src/HC12/HC12Config.h`
- `src/HC12/HC12ConfigParser.h`  

this actually works when built on PC;  
`HC12Driver` require `Arduino` features to work