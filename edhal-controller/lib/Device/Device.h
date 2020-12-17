#ifndef Device_h
#define Device_h

#include <Arduino.h>

// change hardware and software version here
#define FIRMWARE_VERSION_MAJOR 0
#define FIRMWARE_VERSION_MINOR 1
#define FIRMWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 0
#define HARDWARE_VERSION_MINOR 1


class Device{
    public:
        // constructor
        Device(void);
        // destructor
        ~Device(void);
        // display init function
        void serialOutInit(void);
        // load ble_name from spiffs
        void loadDeviceBleName(void);
        // get ble device name in string
        void getDeviceBleName(char *ble_name_para);
        // get device id in string ( custom mac burned in efuse )
        void getDeviceId(char *device_id_string);
        // get device mac in string ( default mac )
        void getDeviceMac(char *device_mac_string);
        // get device hardware version in string ( dotted format )
        void getDeviceHardwareVersion(char *hardware_version);
        // get device software version in string ( dotted format )
        void getDeviceFirmwareVersion(char *firmware_version);

    private:
        uint8_t device_id[6] = {0};
        uint8_t mac[6] = {0};
        char *ble_name = new char[15];
        int FWV_major = FIRMWARE_VERSION_MAJOR;
        int FWV_minor = FIRMWARE_VERSION_MINOR;
        int FWV_patch = FIRMWARE_VERSION_PATCH;
        int HWV_major = HARDWARE_VERSION_MAJOR;
        int HWV_minor = HARDWARE_VERSION_MINOR;
};


#endif