#include "HC12Driver.h"

#define HC12Driver_STATE_CHANGE_TIME 50
#define HC12Driver_AT_SEND_WAIT_TIME 300

HC12Driver::HC12Driver(HardwareSerial* serial, int set_pin, STATE state_start){
    setSerial(serial);
    setSetPin(set_pin);
    setState(state_start);

    current_config.set_factory_default();
}

HardwareSerial* HC12Driver::getSerial(){
    return serial;
}
void HC12Driver::setSerial(HardwareSerial* serial){
    this->serial = serial;
}
/* Will be used for connections */
void HC12Driver::setExpectedBaudrate(int expected){
    this->current_config.set_radio_baudrate(expected);
}

/* Stops work, to check presence*/
bool HC12Driver::checkPresence(){
    HC12Config config = readDeviceConfiguration();
    is_device_present = config.is_valid();
    return is_device_present;
}

/* Return last state */
bool HC12Driver::wasPresent(){
    return is_device_present || is_forced_presence;
}

int HC12Driver::getSetPin(){
    return set_pin;
}

void HC12Driver::setSetPin(int pin){
    this->set_pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
}

/* Stops work, to get parameters */
HC12Config HC12Driver::readDeviceConfiguration(){
    STATE last_state = state;
    setState(STATE::CONFIGURE);
    
    std::string at_to_send = current_config.radio_get_parameters_AT();
    std::string recieved = send_AT(at_to_send, HC12Driver_AT_SEND_WAIT_TIME);

    current_config.parse_parameters_AT_response(recieved);

    setState(last_state);
    return current_config;
}

HC12Config HC12Driver::getSavedDeviceConfiguration(){
    return current_config;
}

/* Apply changes to external module if changes spoted */
bool HC12Driver::updateDeviceConfiguration(HC12Config input_config){
    STATE last_state = state;

    setState(STATE::CONFIGURE);

    if(input_config.get_radio_channel() != current_config.get_radio_channel()){
        std::string at_to_send = input_config.radio_channel_AT_string();
        std::string response = send_AT(at_to_send, HC12Driver_AT_SEND_WAIT_TIME);
        int channel = HC12ConfigParser::parse_radio_channel_AT(response);
        
        current_config.set_radio_channel(channel);
    }
    
    if(input_config.get_radio_power() != current_config.get_radio_power()){
        std::string at_to_send = input_config.radio_power_AT_string();
        std::string response = send_AT(at_to_send, HC12Driver_AT_SEND_WAIT_TIME);
        int power = HC12ConfigParser::parse_radio_power_AT(response);
        
        current_config.set_radio_power(power);
    }

    if(input_config.get_radio_fuse() != current_config.get_radio_fuse()){
        std::string at_to_send = input_config.radio_fuse_AT_string();
        std::string response = send_AT(at_to_send, HC12Driver_AT_SEND_WAIT_TIME);
        int fuse = HC12ConfigParser::parse_radio_fuse_AT(response);
        
        current_config.set_radio_fuse(fuse);
    }

    if(input_config.get_radio_baudrate() != current_config.get_radio_baudrate()){
        std::string at_to_send = input_config.radio_baudrate_AT_string();
        std::string response = send_AT(at_to_send, HC12Driver_AT_SEND_WAIT_TIME);
        int baud = HC12ConfigParser::parse_radio_baudrate_AT(response);
        
        current_config.set_radio_baudrate(baud);
    }

    setState(last_state);

    return current_config == input_config;
}

bool HC12Driver::factoryReset(){
    STATE last_state = state;
    setState(STATE::CONFIGURE);

    std::string at_to_send = current_config.radio_set_default_AT();
    std::string recieved = send_AT(at_to_send, HC12Driver_AT_SEND_WAIT_TIME);

    setState(last_state);

    readDeviceConfiguration();
    HC12Config default_config;
    default_config.set_factory_default();

    return current_config == default_config;
}

void HC12Driver::setState(STATE){
    digitalWrite(set_pin, state);
    delay(HC12Driver_STATE_CHANGE_TIME);
}

HC12Driver::STATE HC12Driver::getState(){
    return state;
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

void HC12Driver::forcePresence(bool is_forced_presence){
    this->is_forced_presence = is_forced_presence;
}

bool HC12Driver::isForcedPresence(){
    return is_forced_presence;
}

std::string HC12Driver::send_AT(std::string at_string, int time_wait) {
    //Algo add here
    if( serial->baudRate() != current_config.get_radio_baudrate() ){
        //serial->updateBaudRate(current_config.get_radio_baudrate());
    }
    clean_input_output();

    serial->write(at_string.c_str());
    serial->flush();
    delay(time_wait);
    
    std::string out = "";
    
    int available = serial->available();
    if(available){
        char buffer[available];
        serial->readBytes(buffer, available);
        out += buffer;
    }

    return out;
}

void HC12Driver::clean_input_output(){
    serial->flush();
    
    while(serial->available()){
        serial->read();
        ::yield();
    }
}