#ifndef Device_h
#define Device_h

#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

// change hardware and software version here
#define FIRMWARE_VERSION_MAJOR   1
#define FIRMWARE_VERSION_MINOR   1
#define FIRMWARE_VERSION_PATCH   0
#define HARDWARE_VERSION_MAJOR   1
#define HARDWARE_VERSION_MINOR   1

// relay pins
#define CHANNEL_1 16
#define CHANNEL_2 17
#define CHANNEL_3 18
#define CHANNEL_4 19
#define CHANNEL_5 23
#define CHANNEL_6 25
#define CHANNEL_7 26
#define CHANNEL_8 27
#define CHANNEL_9 32
#define CHANNEL_10 33

// led pins
#define LED_ONE 2
#define LED_TWO 4

// I2C pins
#define I2C_SDA 21
#define I2C_SCL 22


class Device{
    public:
        // constructor
        Device(void);
        // destructor
        ~Device(void);
        // display init function
        void init(void);
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
        // init bme sensor
        void initBmeSensor(void);
        // get bme sensor data
        char *getBmeSensorData(void);

    private:
        uint8_t device_id[6] = {0};
        uint8_t mac[6] = {0};
        char *ble_name = new char[15];
        int FWV_major = FIRMWARE_VERSION_MAJOR;
        int FWV_minor = FIRMWARE_VERSION_MINOR;
        int FWV_patch = FIRMWARE_VERSION_PATCH;
        int HWV_major = HARDWARE_VERSION_MAJOR;
        int HWV_minor = HARDWARE_VERSION_MINOR;
        Adafruit_BME280 bme;
        bool bme_status;
};


#endif