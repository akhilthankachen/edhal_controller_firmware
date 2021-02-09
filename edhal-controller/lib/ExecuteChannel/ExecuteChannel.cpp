#include "ExecuteChannel.h"
#include "Device.h"

// Execute constructor
ExecuteChannel::ExecuteChannel(){
    // assign device object
}

// Execute destructor
ExecuteChannel::~ExecuteChannel(void){

}

// init execute class with device object
void ExecuteChannel::init(Device *device){
    deviceObj = device;
}

// channel execution entry function
void ExecuteChannel::execute(char *json, DateTime now){
    // make copy of json
    char* jsonCopy = new char[512];
    strcpy(jsonCopy, json);
    // init json doc
    StaticJsonDocument<512> doc;
    // deserialize json
    DeserializationError error = deserializeJson(doc, jsonCopy);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        free(jsonCopy);
        return;
    }

    // extract mode 
    int mode = doc["m"];

    // run appropriate function based on appropriate mode
    switch(mode){
        case -1:
            // do nothing - no mode
            break;
        case 0:
            // do nothing - manual mode
            break;
        case 1:
            // time to time mode
            timeToTime(json, now);
            break;
        case 2:
            // time duration mode
            timeDuration(json, now);
            break;
        case 3:
            // temperature control mode
            temperatureControl(json, now);
            break;
        case 4:
            // humidity control mode
            humidityControl(json, now);
            break;
        case 5:
            // mirror mode
            mirror(json, now);
            break;
        default: 
            // do nothing
            break;
    }

    // free the allocated heap space
    free(jsonCopy);
}

// time to time mode execution function
void ExecuteChannel::timeToTime(char *json, DateTime now){
    // make copy of the json
    char *jsonCopy = new char[512];
    strcpy(jsonCopy, json);
    // init json doc
    StaticJsonDocument<512> doc;
    // deserialize json
    DeserializationError error = deserializeJson(doc, jsonCopy);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    int channel = doc["c"];
    // start time
    int sth = doc["st"]["h"];
    int stm = doc["st"]["m"];
    //end time
    int eth = doc["et"]["h"];
    int etm = doc["et"]["m"];

    // switch on at the starting time
    if(now.hour() == sth && now.minute() == stm && !deviceObj->getStateOfChannel(channel)){
        deviceObj->toggle(channel, now);
    }

    // switch off at the end time
    if(eth == now.hour() && etm == now.minute() && deviceObj->getStateOfChannel(channel)){
        deviceObj->toggle(channel, now);
    }

    // free the allocated heap space
    free(jsonCopy);
}

// time duration mode execution function
void ExecuteChannel::timeDuration(char *json, DateTime now){
    // make copy of the json
    char *jsonCopy = new char[512];
    strcpy(jsonCopy, json);
    // init json doc
    StaticJsonDocument<512> doc;
    // deserialize json
    DeserializationError error = deserializeJson(doc, jsonCopy);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    // extract channel
    int channel = doc["c"];

    JsonArray td = doc["td"];
    int arraySize = td.size();

    for(int i = 0; i < arraySize; i++){
        JsonObject temp = td[i];
        int h = temp["h"];
        int m = temp["m"];
        int d = temp["d"];

        // switch on at the starting time
        if(now.hour() == h && now.minute() == m && !deviceObj->getStateOfChannel(channel)){
            deviceObj->toggle(channel, now);
        }

        // switch off at the ending minute
        if( m + d < 60){
            // switch off at the end time
            if(h == now.hour() && (m + d) == now.minute() && deviceObj->getStateOfChannel(channel)){
                deviceObj->toggle(channel, now);
            }
        }else if( m + d >= 60 ){
            // increment hour for switching off
            h = h + 1;

            if(h == now.hour() && (m + d - 60) == now.minute() && deviceObj->getStateOfChannel(channel)){
                deviceObj->toggle(channel, now);
            }
        }
    }

    // free the allocated heap space
    free(jsonCopy);
}

// temperature control mode execution function
void ExecuteChannel::temperatureControl(char *json, DateTime now){
    // make copy of the json
    char *jsonCopy = new char[512];
    strcpy(jsonCopy, json);
    // init json doc
    StaticJsonDocument<512> doc;
    // deserialize json
    DeserializationError error = deserializeJson(doc, jsonCopy);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    // get bme temperature
    float temperature = deviceObj->getBmeTemperature();

    // get channel from config json
    int channel = doc["c"];
    float ut = doc["ut"];
    float lt = doc["lt"];
    
    if( ut > lt && temperature != 0){
        if( temperature >= ut && !deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }

        if( temperature <= lt && deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }
    }

    if( ut < lt && temperature != 0){
        if( temperature <= ut && !deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }

        if( temperature >= lt && deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }
    } 

    free(jsonCopy);
}

// humidity control mode execution function
void ExecuteChannel::humidityControl(char *json, DateTime now){
    // make copy of the json
    char *jsonCopy = new char[512];
    strcpy(jsonCopy, json);
    // init json doc
    StaticJsonDocument<512> doc;
    // deserialize json
    DeserializationError error = deserializeJson(doc, jsonCopy);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    // get bme humidity
    float humidity = deviceObj->getBmeHumidity();

    // get channel from config json
    int channel = doc["c"];
    float uh = doc["uh"];
    float lh = doc["lh"];
    
    if( uh > lh && humidity != 0){
        if( humidity >= uh && !deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }

        if( humidity <= lh && deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }
    }

    if( uh < lh && humidity != 0){
        if( humidity <= uh && !deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }

        if( humidity >= lh && deviceObj->getStateOfChannel(channel) ){
            deviceObj->toggle(channel, now);
        }
    } 

    free(jsonCopy);
}

// mirror mode execution function
void ExecuteChannel::mirror(char *json, DateTime now){
    // make copy of the json
    char *jsonCopy = new char[512];
    strcpy(jsonCopy, json);
    // init json doc
    StaticJsonDocument<512> doc;
    // deserialize json
    DeserializationError error = deserializeJson(doc, jsonCopy);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    // get channel from config json
    int channel = doc["c"];
    // get array of master channels
    JsonArray mcs = doc["mcs"];
    int arraySize = mcs.size();

    // toggle channel if any master channel is on
    bool switchOn = false;
    for(int i = 0; i < arraySize; i++){
        int mc = mcs[i];
        if( switchOn || deviceObj->getStateOfChannel(mc) ){
            switchOn = true;
        }
    }

    if( switchOn && !deviceObj->getStateOfChannel(channel) ){
        deviceObj->toggle(channel, now);
    }

    // toggle channel if all master channels are off
    bool switchOff = true;
    for(int i = 0; i < arraySize; i++){
        int mc = mcs[i];
        if( switchOff && !deviceObj->getStateOfChannel(mc) ){
            switchOff = true;
        }else{
            switchOff = false;
        }
    }

    if( switchOff && deviceObj->getStateOfChannel(channel) ){
        deviceObj->toggle(channel, now);
    }

    free(jsonCopy);
}