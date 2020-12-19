#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <SPIFFS.h>
#include "Device.h"

Device device;

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


}

void loop() {
  // put your main code here, to run repeatedly:

}