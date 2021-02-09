#ifndef Device_h
#define Device_h

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
        // get temperature alone
        float getBmeTemperature(void);
        // get humidity alone
        float getBmeHumidity(void);
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
        char *ch1 = new char[512];
        char *ch2 = new char[512];
        char *ch3 = new char[512];
        char *ch4 = new char[512];
        char *ch5 = new char[512];
        char *ch6 = new char[512];
        char *ch7 = new char[512];
        char *ch8 = new char[512];

        // get state of specific channel
        bool getStateOfChannel(int);
        // toggle channel
        void toggle(int, DateTime);
        // get states of configured channel
        char* getState(void);
        // set state of channel
        void setState(const char *);

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

        // channel on time
        DateTime ch1Time;
        DateTime ch2Time;
        DateTime ch3Time;
        DateTime ch4Time;
        DateTime ch5Time;
        DateTime ch6Time;
        DateTime ch7Time;
        DateTime ch8Time;
};


#endif