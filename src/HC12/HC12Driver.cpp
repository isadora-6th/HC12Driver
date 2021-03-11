#include "HC12Driver.h"

#define HC12Driver_STATE_CHANGE_TIME 50
#define HC12Driver_AT_RX_WAIT_TIME   300
#define HC12Driver_CONFIGURE_RESULT_WAIT 100

HC12Driver::HC12Driver(HardwareSerial* serial, int set_pin, STATE state_start){
    setSerial(serial);
    setSetPin(set_pin);
    setState(state_start);
}

int HC12Driver::available() {
    return serial->available();
}

int HC12Driver::read() {
    return serial->read();
}

int HC12Driver::peek() {
    return serial->peek();
}

size_t HC12Driver::write(uint8_t data){
    return serial->write(data);
}