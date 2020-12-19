#include "Device.h"
#include <SPIFFS.h>

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
    delete[] ble_name;
}

// serial out device init - device id, hardware version, firmware version and bluetooth name
void Device::init(void){

    Serial.println("");
    Serial.println("##################################");
    Serial.println("Edhal controller initializing...");

    // serial out hardware version 
    Serial.println("");
    Serial.print("Hardware Version : ");
    char *hwv = new char[11];
    getDeviceHardwareVersion(hwv);
    Serial.print(hwv);

    // serial out firmware version 
    Serial.println("");
    Serial.print("Firmware Version : ");
    char *fwv = new char[17];
    getDeviceFirmwareVersion(fwv);
    Serial.print(fwv);

    // serial out device id 
    Serial.println("");
    Serial.print("Device ID : ");
    char *device_id_string = new char[18];
    getDeviceId(device_id_string);
    Serial.print(device_id_string);

    // serial out device mac
    Serial.println("");
    Serial.print("Device Mac : ");
    char *device_mac_string = new char[18];
    getDeviceMac(device_mac_string);
    Serial.println(device_mac_string);

    // load ble_name from SPIFFS, only after SPIFFS mounted
    loadDeviceBleName();
    // displaying ble name
    Serial.print("Ble name : ");
    Serial.print(ble_name);

    Serial.println("");
    Serial.println("##################################");

    // delete initialized character arrays
    delete[] device_id_string;
    delete[] device_mac_string;
    delete[] hwv;
    delete[] fwv;

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

        // deleting initialized character pointers
        delete[] firstSix;

    }
}

// get ble_name into string
// should be called only after ble name is loaded
// else error
void Device::getDeviceBleName( char *ble_name_para ){
    // copy ble name to the passed character array pointer
    strcpy(ble_name_para, ble_name);
}

// copy device id as string to the passed character array
void Device::getDeviceId(char *device_id_string){
    char *device_id_string_temp = new char[18];

    // converting 6 byte hex code mac into string in ':' format
    snprintf(device_id_string_temp, 18, "%02x:%02x:%02x:%02x:%02x:%02x", device_id[0], device_id[1], device_id[2], device_id[3], device_id[4], device_id[5]);

    strcpy(device_id_string, device_id_string_temp);

    // delete initialized character array
    delete[] device_id_string_temp;
}

// copy device mac as string to the passed character array
void Device::getDeviceMac(char *device_mac_string){
    char *device_mac_string_temp = new char[18];

    // converting 6 byte hex code mac into string in ':' format
    snprintf(device_mac_string_temp, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    strcpy(device_mac_string, device_mac_string_temp);

    // delete initialized character array
    delete[] device_mac_string_temp;
}

// copy hardware version as string to the passed character array
void Device::getDeviceHardwareVersion(char *hardware_version){
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
    strcpy(hardware_version, version);

    // delete initialized character arrays
    delete[] major;
    delete[] minor;
    delete[] version;
}

// copy software version as string to the passed character array
void Device::getDeviceFirmwareVersion(char *firmware_version){
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
    strcpy(firmware_version, version);

    // delete initialized character arrays
    delete[] major;
    delete[] minor;
    delete[] patch;
    delete[] version;
}