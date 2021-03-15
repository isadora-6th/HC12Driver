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

# Instalation

Want to add lib to platformIO index, but... to Lazy for it, so use direct Git link instead.
```ini
lib_deps = 
    HC12Driver ; NOT working now (NOT in INDEX yet)
    https://github.com/isadora-6th/RadioAPI.git ; Working, if you have access to repo
```
if you are using git, update lib in project:
 - open platformio tab
 - Quick Access -> Miscellaneous
 - PlatfromIO Core CLI
 - `pio lib update`
 - no indication  

also you can add to `lib` folder in your `platformIO` project  
or if you prefer Arduino, copy to `Arduino/libraries`  
> ofcource can copy to your sketch foleder, will work (better to place in folder and include with "folder_name/HC12Driver.h" +8 files in Arduino tabs will make mess)

# Usage
Refer to `src/HC12Config.h` and `src/HC12Driver.h` for detailed info.  
Actually `HC12Driver` class replaces `Serial` usage for HC-12 communication.


# Example

```cpp
#include "HC12Config.h"
#include "HC12Driver.h"

void setup(){
    #define SET_PIN        16
    #define CHANNEL_TO_SET 14
    #define RADIO_SERIAL   Serial

    RADIO_SERIAL.begin(921600);

    HC12Driver driver(&RADIO_SERIAL, SET_PIN, HC12Driver::CONFIGURE);
    driver.setExpectedBaudrate(9600);
    driver.checkPresence(); //Also updates saved config
    
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
        // Config change failed
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
```

# Note
This lib not expected to use minimal `RAM` (low but not minimal possible), or `Flash Size` (A lot will be expected).  
Also compatibility issues expected ( Ardino::String <=> std::string );

Most portable part of program is:
- `src/HC12/HC12Config.h`
- `src/HC12/HC12ConfigParser.h`  

this actually works when built on PC;  
`HC12Driver` require `Arduino` features to work

# How to add new controller support
  
Add `#define` for your platforms at following files:

- `HC12Driver.cpp`:  
    - `#define HC12Driver_COMPAT_BAUD_CHANGE(serial_ptr, baudrate)`
    - `#define HC12Driver_COMPAT_COMPARE_BAUD_SPEED(serial_ptr, speed)`

- `HC12_std_string_HAL.h`:
    - `std::string`
    - `std::to_string(long)`
 
- `HC12ConfigParser.cpp`:
    - `#define HC12_CONFIG_PARSE_ERROR(X)`
    > Note: Can be left empty
    - `#define compat_std_stoi( IN_STRING, OUT_VAR )`
    > Converts std::string to long (0 for failure)  
    https://www.cplusplus.com/reference/cstdlib/atol/

# Next stuff to expect
Create lib that makes abstraction for `Radio work`

Inputs Arduino `Stream` interface
- V0
    - RadioInV0
    - RadioOutV0
    - RadioGroupV0
- V1
    - RadioInV1
    - RadioOutV1
    - RadioGroupV1
    > That mode that has device address in each frame  
      Like allow controll by single radio multiple devices
      (that 4xDumaru project with single controll point)  

Create lib for mapApi Abstraction.
- MapCsv_v1 
    > already exist
- MapCsv_v2 / v3
    > R8, bind to project `*has idea problems*`  

Create lib of converters: 
- `Madrix <--> Csv`;
    > now have Madrix -> `MapCsv_v1`
- `Jinx <--> Csv`;  
- `Autocad <--> Csv`;
    > research showed that:  
        - Autocad allows saving for inkSpace -> free open source code  
        - other formats to parse  
        Want use `Qt` framework for this task, so GUI expected