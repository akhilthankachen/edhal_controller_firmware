#include "Device.h"

// constructor definition
Device::Device(){
    // fetch custom mac stored in OTP efuse blk3
    esp_err_t rc;
    rc = esp_efuse_mac_get_custom(device_id);
    if(rc != ESP_OK){
        // if custom mac not availble - use default mac as device id
        esp_efuse_mac_get_default(device_id);
    }

    // fetch default mac used in wifi
    esp_efuse_mac_get_default(mac);
}

// serial out device init - device id, hardware version, firmware version and bluetooth name
void Device::serialOutInit(void){
    Serial.println("");
    Serial.println("##################################");
    Serial.println("Edhal controller initializing....");

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
    Serial.print(device_mac_string);

    Serial.println("");
    Serial.println("##################################");

    delete[] device_id_string;
    delete[] device_mac_string;
    delete[] hwv;
    delete[] fwv;
}

// copy device id as string to the passed character array
void Device::getDeviceId(char *device_id_string){
    char *device_id_string_temp = new char[18];

    // converting 6 byte hex code mac into string in ':' format
    snprintf(device_id_string_temp, 18, "%02x:%02x:%02x:%02x:%02x:%02x", device_id[0], device_id[1], device_id[2], device_id[3], device_id[4], device_id[5]);

    strcpy(device_id_string, device_id_string_temp);

    delete[] device_id_string_temp;
}

// copy device mac as string to the passed character array
void Device::getDeviceMac(char *device_mac_string){
    char *device_mac_string_temp = new char[18];

    // converting 6 byte hex code mac into string in ':' format
    snprintf(device_mac_string_temp, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    strcpy(device_mac_string, device_mac_string_temp);

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