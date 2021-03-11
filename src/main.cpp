#ifdef ESP8266
    #include <Arduino.h>
#endif
#include "HC12/HC12Config.h"

#ifndef ESP8266
    #include "SerialHAL.h"
    HardwareSerial Serial(0);
#endif

int main(){

    HC12Config config;
    config.set_default();

    config.parse_radio_channel("OK+C021");
    Serial.write(config.to_string().c_str());
    return 0;    
}

void setup(){

    Serial.begin(921600);
    delay(1000);
    Serial.println("\n\n=====\n");
    HC12Config config;
    config.set_default();

    config.parse_radio_channel("OK+C021");
    Serial.write(config.to_string().c_str());
}

void loop(){

}