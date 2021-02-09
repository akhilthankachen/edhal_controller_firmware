#include "Device.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"

// constructor definition
Device::Device(){

    esp_err_t rc;

    // fetch custom mac stored in OTP efuse blk3 and use as device id
    rc = esp_efuse_mac_get_custom(device_id);

    // if failed to fetch custom mac
    if(rc != ESP_OK){
        // if custom mac not availble - use default mac as device id
        rc = esp_efuse_mac_get_default(device_id);

        // failed to fetch default mac
        if( rc != ESP_OK){
            device_id[0] = 0x00;
            device_id[1] = 0x00;
            device_id[2] = 0x00;
            device_id[3] = 0x00;
            device_id[4] = 0x00;
            device_id[5] = 0x00;
        }
    }

    // fetch default mac used in wifi
    rc = esp_efuse_mac_get_default(mac);

    // if failed to fetch default mac
    if(rc != ESP_OK){
        mac[0] = 0x00;
        mac[1] = 0x00;
        mac[2] = 0x00;
        mac[3] = 0x00;
        mac[4] = 0x00;
        mac[5] = 0x00; 
    }

}

// destructor
Device::~Device(void){
    free(ble_name);
    free(ch1);
    free(ch2);
    free(ch3);
    free(ch4);
    free(ch5);
    free(ch6);
    free(ch7);
    free(ch8);
}

// serial out device init - device id, hardware version, firmware version and bluetooth name
void Device::init(void){

    Serial.println("");
    Serial.println("##################################");
    Serial.println("Edhal controller initializing...");

    // serial out hardware version 
    Serial.println("");
    Serial.print("Hardware Version : ");
    char *hwv = getDeviceHardwareVersion();
    Serial.print(hwv);

    // serial out firmware version 
    Serial.println("");
    Serial.print("Firmware Version : ");
    char *fwv = getDeviceFirmwareVersion();
    Serial.print(fwv);

    // serial out device id 
    Serial.println("");
    Serial.print("Device ID : ");
    char *device_id_string = getDeviceId();
    Serial.print(device_id_string);

    // serial out device mac
    Serial.println("");
    Serial.print("Device Mac : ");
    char *device_mac_string = getDeviceMac();
    Serial.println(device_mac_string);

    // load ble_name from SPIFFS, only after SPIFFS mounted
    loadDeviceBleName();
    // displaying ble name
    Serial.print("Ble name : ");
    Serial.print(ble_name);

    Serial.println("");
    Serial.println("##################################");

    // init bme sensor
    initBmeSensor();

    // init RTC
    initRTC();

    // load config files from spiffs
    loadChannelConfig(1);
    loadChannelConfig(2);
    loadChannelConfig(3);
    loadChannelConfig(4);
    loadChannelConfig(5);
    loadChannelConfig(6);
    loadChannelConfig(7);
    loadChannelConfig(8);

    free(hwv);
    free(fwv);
    free(device_id_string);
    free(device_mac_string);
    
}

// load ble_name from spiffs
void Device::loadDeviceBleName(void){
    if(SPIFFS.exists("/ble_name.txt")){

        File file = SPIFFS.open("/ble_name.txt", "r");
        if(!file){
            Serial.println("error : failed to open ble_name.txt from spiffs");

            // if SPIFFS fail use a random name each time
            // generating 6 digit random number 
            uint32_t random = esp_random();
            char *firstSix = new char[7];
            snprintf(firstSix, 7, "%u", random);

            // patching up ble name
            strcpy(ble_name, "EDHAL-");
            strcat(ble_name, firstSix);

        }else{
            // get ble name from file
            String s = file.readStringUntil('\n');
            // copy ble name from string to char *
            strcpy(ble_name, s.c_str());
        }
        file.close();

    }else{

        Serial.println();
        Serial.println("Ble_name.txt not found in spiffs");
        Serial.println("Generating new one...");

        // generating 6 digit random number 
        uint32_t random = esp_random();
        char *firstSix = new char[7];
        snprintf(firstSix, 7, "%u", random);

        // patching up ble name
        strcpy(ble_name, "EDHAL-");
        strcat(ble_name, firstSix);

        // writing ble name to spiffs for later use
        File file = SPIFFS.open("/ble_name.txt", "w");
        if(!file){
            Serial.println("error : failed to open ble_name.txt from spiffs");
        }else{
            // file open writing to file
            int bytesWritten = file.print(ble_name);
            if (bytesWritten > 0) {
                Serial.println("Ble name written to spiffs");
            } else {
                Serial.println("error : failed to open ble_name.txt from spiffs");
            }
        }
        file.close();


        // deleting initialized character pointers
        free(firstSix);

    }
}

// get ble_name into string
// should be called only after ble name is loaded
// else error
char* Device::getDeviceBleName(void){
    // copy ble name to the passed character array pointer
    return ble_name;
}

// copy device id as string to the passed character array
char* Device::getDeviceId(void){
    char *device_id_string_temp = new char[18];

    // converting 6 byte hex code mac into string in ':' format
    snprintf(device_id_string_temp, 18, "%02x:%02x:%02x:%02x:%02x:%02x", device_id[0], device_id[1], device_id[2], device_id[3], device_id[4], device_id[5]);

    return device_id_string_temp;
}

// copy device mac as string to the passed character array
char* Device::getDeviceMac(void){
    char *device_mac_string_temp = new char[18];

    // converting 6 byte hex code mac into string in ':' format
    snprintf(device_mac_string_temp, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return device_mac_string_temp;
}

// copy hardware version as string to the passed character array
char* Device::getDeviceHardwareVersion(void){
    // character array initialization
    char *major = new char[5];
    char *minor = new char[5];
    char *version = new char[11];

    // convert integer to string literal
    sprintf(major, "%d", HWV_major);
    sprintf(minor, "%d", HWV_minor);

    // concat major minor in dot format and copy to passed charcter array
    strcpy(version, major);
    strcat(version, ".");
    strcat(version, minor);
    
    return version;
}

// copy software version as string to the passed character array
char* Device::getDeviceFirmwareVersion(void){
    // character array initialization
    char *major = new char[5];
    char *minor = new char[5];
    char *patch = new char[5];
    char *version = new char[17];

    // convert integer to string literal
    sprintf(major, "%d", FWV_major);
    sprintf(minor, "%d", FWV_minor);
    sprintf(patch, "%d", FWV_patch);

    // concat major minor in dot format and copy to passed character array
    strcpy(version, major);
    strcat(version, ".");
    strcat(version, minor);
    strcat(version, ".");
    strcat(version, patch);
  
    return version;
}

// init bme sensor
void Device::initBmeSensor(void){
    // try to init bme sensor and store status
    bme_status = bme.begin(0x76);
    if(bme_status){
        Serial.println();
        Serial.println("BME Sensor Loaded...");
    }else{
        Serial.println();
        Serial.println("Could not load BME Sensor...");
    }
}

// get and return bme sensor data
char *Device::getBmeSensorData(void){
    
    // initialized data string variale
    char *data = new char[96];
    size_t dataSize = 96;

    // initialize static json object for storing sensor data
    StaticJsonDocument<96> doc;

    if(bme_status){
        // set sea level pressure
        float SEALEVELPRESSURE_HPA = 1013.25;
        
        // set data json field
        doc["s"] = true;
        doc["t"] = bme.readTemperature();
        doc["h"] = bme.readHumidity();
        doc["p"] = bme.readPressure() / 100.0F;
        doc["a"] = bme.readAltitude(SEALEVELPRESSURE_HPA);

        serializeJson(doc, data, dataSize);
        return data;

    }else{
        
        // set data json field
        doc["s"] = false;
        doc["t"] = 0;
        doc["h"] = 0;
        doc["p"] = 0;
        doc["a"] = 0;

        serializeJson(doc, data, dataSize);
        return data;
    }

}

// get temperature alone
float Device::getBmeTemperature(void){
    // return temperature
    if(bme_status){
        return bme.readTemperature();
    }else{
        return 0;
    }
    
}

// get humidity alone
float Device::getBmeHumidity(void){
    // return humidty
    if(bme_status){
        return bme.readHumidity();
    }else{
        return 0;
    }
}

// init real time clock (DS3231)
void Device::initRTC(void){
    // try to start read from rtc
    rtc_status = rtc.begin();
    if(rtc_status){
        Serial.println("RTC loaded...");

        DateTime now = rtc.now();

        Serial.print(now.year());
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" (");
        Serial.print(now.dayOfTheWeek());
        Serial.print(") ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();

    }else{
        Serial.println("Couldn't load RTC...");
    }

}

// get date time now
DateTime Device::getDateTimeNow(void){

    // if only rtc is loaded
    if(rtc_status){
        DateTime now = rtc.now();
        
        return now;
    }else{
        DateTime now = DateTime(0,0,0,0,0,0);

        return now;
    }
    
}

// store config json to spiffs
void Device::storeChannelConfig(int channel, const char *jsonStr){
    // convert channel into character
    char *channelStr = new char[2];
    snprintf(channelStr, 2, "%i", channel);

    // init file name
    char *filename = new char[9];
    strcpy(filename, "/ch");
    strcat(filename, channelStr);
    strcat(filename, ".txt");

    // store the new config
    File file = SPIFFS.open(filename, "w");
    if(!file){
        Serial.println("error : failed to open config file from spiffs");
    }else{
        // file open writing to file
        int bytesWritten = file.print(jsonStr);
        if (bytesWritten > 0) {
            Serial.println("Config file was written to spiffs");
        } else {
            Serial.println("error : failed to open config file from spiffs");
        }
    }
    file.close();

    // delete char pointers
    free(filename);
    free(channelStr);
}

// load config file from SPIFFS
void Device::loadChannelConfig(int channel){
    // convert channel into character
    char *channelStr = new char[2];
    snprintf(channelStr, 2, "%i", channel);

    // init file name
    char *filename = new char[9];
    strcpy(filename, "/ch");
    strcat(filename, channelStr);
    strcat(filename, ".txt");

    // init json doc
    StaticJsonDocument<512> doc;
    // json string
    char *json = new char[512];

    if(SPIFFS.exists(filename)){
        // load the config file
        File file = SPIFFS.open(filename, "r");
        if(!file){
            Serial.println("error : failed to open config file from spiffs (while loading)");
        }else{

            Serial.println();
            Serial.print("config file found for channel : ");
            Serial.println(channel);
            Serial.print("Config : ");

            // get config json from file
            String s = file.readStringUntil('\n');
            strcpy(json, s.c_str());

            Serial.println(json);
        }
        file.close();

    }else{
        // init json with no mode 
        doc["c"] = channel;
        doc["m"] = -1;

        serializeJson(doc, json, 512);

        Serial.println();
        Serial.print("No config file found for channel : ");
        Serial.println(channel);
        Serial.println("Initing with no mode json for the channel ...");
    }
    

    // copy json to appropriate variable
    switch(channel){
        case 1:
            strcpy(ch1, json);
            break;
        case 2:
            strcpy(ch2, json);
            break;
        case 3:
            strcpy(ch3, json);
            break;
        case 4:
            strcpy(ch4, json);
            break;
        case 5:
            strcpy(ch5, json);
            break;
        case 6:
            strcpy(ch6, json);
            break;
        case 7:
            strcpy(ch7, json);
            break;
        case 8:
            strcpy(ch8, json);
            break;
        default:
            Serial.println("Error: No channel found in the config json while loading");
    }

    // delete char arrays
    free(filename);
    free(channelStr);
    free(json);
}

// handle configuration json
void Device::handleConfigJson(const char *jsonStr){
    Serial.println();
    Serial.print("Config received : ");
    Serial.println(jsonStr);

    // deserilize json
    StaticJsonDocument<512> doc;
    deserializeJson(doc, jsonStr);
    int c = doc["c"];


    // store json to appropriate file
    switch(c){
        case 1:
            strcpy(ch1, jsonStr);
            storeChannelConfig(1, jsonStr);
            break;
        case 2:
            strcpy(ch2, jsonStr);
            storeChannelConfig(2, jsonStr);
            break;
        case 3:
            strcpy(ch3, jsonStr);
            storeChannelConfig(3, jsonStr);
            break;
        case 4:
            strcpy(ch4, jsonStr);
            storeChannelConfig(4, jsonStr);
            break;
        case 5:
            strcpy(ch5, jsonStr);
            storeChannelConfig(5, jsonStr);
            break;
        case 6:
            strcpy(ch6, jsonStr);
            storeChannelConfig(6, jsonStr);
            break;
        case 7:
            strcpy(ch7, jsonStr);
            storeChannelConfig(7, jsonStr);
            break;
        case 8:
            strcpy(ch8, jsonStr);
            storeChannelConfig(8, jsonStr);
            break;
        default:
            Serial.println("Error: No channel found in the config json");
    }
    
}

// returns state of specific channel
bool Device::getStateOfChannel(int channel){

    switch(channel){
        case 1:
            return digitalRead(CHANNEL_1);
            break;
        case 2:
            return digitalRead(CHANNEL_2);
            break;
        case 3:
            return digitalRead(CHANNEL_3);
            break;
        case 4:
            return digitalRead(CHANNEL_4);
            break;
        case 5:
            return digitalRead(CHANNEL_5);
            break;
        case 6:
            return digitalRead(CHANNEL_6);
            break;
        case 7:
            return digitalRead(CHANNEL_7);
            break;
        case 8:
            return digitalRead(CHANNEL_8);
            break;
        default:
            // do nothing
            break;
    }
}

// toggle channel
void Device::toggle(int channel, DateTime now){
    if(!getStateOfChannel(channel)){
        switch(channel){
            case 1:
                digitalWrite(CHANNEL_1, HIGH);
                ch1Time = now;
                break;
            case 2:
                digitalWrite(CHANNEL_2, HIGH);
                ch2Time = now;
                break;
            case 3:
                digitalWrite(CHANNEL_3, HIGH);
                ch3Time = now;
                break;
            case 4:
                digitalWrite(CHANNEL_4, HIGH);
                ch4Time = now;
                break;
            case 5:
                digitalWrite(CHANNEL_5, HIGH);
                ch5Time = now;
                break;
            case 6:
                digitalWrite(CHANNEL_6, HIGH);
                ch6Time = now;
                break;
            case 7:
                digitalWrite(CHANNEL_7, HIGH);
                ch7Time = now;
                break;
            case 8:
                digitalWrite(CHANNEL_8, HIGH);
                ch8Time = now;
                break;
            default:
                break;
        }
        Serial.println();
        Serial.print("Switching on channel : ");
        Serial.println(channel);
    }else{
        switch(channel){
            case 1:
                digitalWrite(CHANNEL_1, LOW);
                break;
            case 2:
                digitalWrite(CHANNEL_2, LOW);
                break;
            case 3:
                digitalWrite(CHANNEL_3, LOW);
                break;
            case 4:
                digitalWrite(CHANNEL_4, LOW);
                break;
            case 5:
                digitalWrite(CHANNEL_5, LOW);
                break;
            case 6:
                digitalWrite(CHANNEL_6, LOW);
                break;
            case 7:
                digitalWrite(CHANNEL_7, LOW);
                break;
            case 8:
                digitalWrite(CHANNEL_8, LOW);
                break;
            default:
                break;
        }
        Serial.println();
        Serial.print("Switching off channel : ");
        Serial.println(channel);
    }
}


// get state of channels
char* Device::getState(){
    // json string 
    char* json = new char[512];
    // json doc
    StaticJsonDocument<512> doc;
    // array on on channels
    JsonArray s = doc.createNestedArray("s");
    for( int i = 1; i<=8; i++){
        if(getStateOfChannel(i)){
            JsonObject temp = s.createNestedObject();
            temp["c"] = i;
            switch(i){
                case 1:
                    temp["h"] = ch1Time.hour();
                    temp["m"] = ch1Time.minute();
                    break;
                case 2:
                    temp["h"] = ch2Time.hour();
                    temp["m"] = ch2Time.minute();
                    break;
                case 3:
                    temp["h"] = ch3Time.hour();
                    temp["m"] = ch3Time.minute();
                    break;
                case 4:
                    temp["h"] = ch4Time.hour();
                    temp["m"] = ch4Time.minute();
                    break;
                case 5:
                    temp["h"] = ch5Time.hour();
                    temp["m"] = ch5Time.minute();
                    break;
                case 6:
                    temp["h"] = ch6Time.hour();
                    temp["m"] = ch6Time.minute();
                    break;
                case 7:
                    temp["h"] = ch7Time.hour();
                    temp["m"] = ch7Time.minute();
                    break;
                case 8:
                    temp["h"] = ch8Time.hour();
                    temp["m"] = ch8Time.minute();
                    break;
                default:
                    break;
            }
        }
    }

    serializeJson(doc, json, 512);
    return json;
}


