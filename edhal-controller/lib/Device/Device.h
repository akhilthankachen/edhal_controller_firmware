#ifndef Device_h
#define Device_h

#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "RTClib.h"
#include "ArduinoJson.h"

// change hardware and software version here
#define FIRMWARE_VERSION_MAJOR   1
#define FIRMWARE_VERSION_MINOR   1
#define FIRMWARE_VERSION_PATCH   0
#define HARDWARE_VERSION_MAJOR   1
#define HARDWARE_VERSION_MINOR   1

// relay pins
#define CHANNEL_1 18
#define CHANNEL_2 19
#define CHANNEL_3 23
#define CHANNEL_4 25
#define CHANNEL_5 26
#define CHANNEL_6 27
#define CHANNEL_7 32
#define CHANNEL_8 33

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
        char* getDeviceBleName(void);
        // get device id in string ( custom mac burned in efuse )
        char* getDeviceId(void);
        // get device mac in string ( default mac )
        char* getDeviceMac(void);
        // get device hardware version in string ( dotted format )
        char* getDeviceHardwareVersion(void);
        // get device software version in string ( dotted format )
        char* getDeviceFirmwareVersion(void);
        // init bme sensor
        void initBmeSensor(void);
        // get bme sensor data
        char *getBmeSensorData(void);
        // init rtc module
        void initRTC(void);
        // get date time now
        DateTime getDateTimeNow(void);
        // store new configurations to spiffs
        void storeChannelConfig(int, const char*);
        // load configuration of specific channel
        void loadChannelConfig(int);
        // handle configuration json
        void handleConfigJson(const char*);
        // config json of each channel
        StaticJsonDocument<512> ch1;
        StaticJsonDocument<512> ch2;
        StaticJsonDocument<512> ch3;
        StaticJsonDocument<512> ch4;
        StaticJsonDocument<512> ch5;
        StaticJsonDocument<512> ch6;
        StaticJsonDocument<512> ch7;
        StaticJsonDocument<512> ch8;

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
        RTC_DS3231 rtc;
        bool bme_status;
        bool rtc_status;

        // configurations of channels
        
};


#endif