#include <iostream>
#include "SerialHAL.h"

int main(){
    HardwareSerial Serial;
    Serial.debug_target("endpoint");
    while(Serial.available()){
        std::cout << (char) Serial.read();
    }
    return 0;
}