# HC-12 Driver library

This library was made to surely configure HC-12 module on the go and use it without bothering about `Serial + Set_Pin` state.  
Made because local solution (R8) had problems with portability and reliability.  
Also large testing expected for this library. (Like using all set_configures and recheck all state change failure causes).

Target framework:  
- `Arduino Framework`

Target platfroms:
- `Esp8266`
    > Currently in progress
- `Esp32`
    > Not checked yet
- `Arduino nano`
    > Expected problems with `std::string` maybe converter functions will be added;


## Warning... work in progress
Driver still needs to be written, and all tests must be done before usage
70% done, but here is still some to go.  

Also Warnings from... `#define XXX_WARNING(X)` has some... problems
could be replaced by #define before lib include still thinking.  

# Usage
Refer to `src/HC12/HC12Config.h` and `src/HC12/HC12Driver.h` for detailed info.  
Actually `HC12Driver` class replaces `Serial` usage for HC-12 communication.


# Example

```cpp
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
    HC12Config config = driver.readDeviceConfiguration();
    if( !config.is_valid() ){
        // Config read somehow failed, consider edit timings in HC12Driver.cpp
        // readDeviceConfiguration also edits wasPresent() state
    }

    config.set_radio_channel( CHANNEL_TO_SET );
    if( !driver.updateDeviceConfiguration(config) ){
        // Config change failed, consider edit timings in HC12Driver.cpp
        /* updateDeviceConfiguration also 
            edits getSavedDeviceConfiguration() state on success*/
    }

    driver.setState(HC12Driver::WORKING);
    
    driver.println("Hello my dear friend");
    config.set_radio_power( 7 );
    driver.updateDeviceConfiguration(config);

    driver.println("When state::working => auto returns to working after config update");   
    driver.println("All Serial functions now available from driver");
    

```

# Note
This lib not expected to use minimal `RAM` (low but not minimal possible), or `Flash Size` (A lot will be expected).  
Also compatibility issues expected ( Ardino::String <=> std::string );

Most portable part of program is:
- `src/HC12/HC12Config.h`
- `src/HC12/HC12ConfigParser.h`  

this actually works when built on PC;  
`HC12Driver` require `Arduino` features to work