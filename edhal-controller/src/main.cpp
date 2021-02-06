#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <SPIFFS.h>
#include "Device.h"
#include "BleInterface.h"

// device object init
Device device;
// ble interface object init
BleInterface ble;

void setup() { 
  // put your setup code here, to run once:
  Serial.begin(9600);

  // mount file system
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }

  // Serial outputs hw_version, fw_version, mac address, device id, blutooth name
  // important - should be called just after SPIFFS mounted 
  // bluetooth name can be only initialized after SPIFFS mounted
  device.init();

  // start ble with device object passed
  ble.begin(device);

  // print new line
  Serial.println();
}

// date time variable to store data from rtc
DateTime now;

void loop() {
  // get date time from rtc
  now = device.getDateTimeNow();
  

  // notify sensor data in every 10 seconds
  ble.notifySensorData(now.second());
}