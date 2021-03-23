#include "Arduino.h"
#include <SPIFFS.h>
#include "Device.h"
#include "BleInterface.h"
#include "ExecuteChannel.h"

// device object init
Device device;
// ble interface object init
BleInterface ble;
// init execute object
ExecuteChannel execute;

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

  pinMode(CHANNEL_1, OUTPUT);
  pinMode(CHANNEL_2, OUTPUT);
  pinMode(CHANNEL_3, OUTPUT);
  pinMode(CHANNEL_4, OUTPUT);
  pinMode(CHANNEL_5, OUTPUT);
  pinMode(CHANNEL_6, OUTPUT);
  pinMode(CHANNEL_8, OUTPUT);

  digitalWrite(CHANNEL_1, LOW);
  digitalWrite(CHANNEL_2, LOW);
  digitalWrite(CHANNEL_3, LOW);
  digitalWrite(CHANNEL_4, LOW);
  digitalWrite(CHANNEL_5, LOW);
  digitalWrite(CHANNEL_6, LOW);
  

  // start ble with device object passed
  ble.begin(&device);

  // init execute object with device object
  execute.init(&device);

  // print new line
  Serial.println();
}

// date time variable to store data from rtc
DateTime now;

void loop() {
  Serial.println(ESP.getFreeHeap());
  // get date time from rtc
  now = device.getDateTimeNow();
  

  // notify sensor data in every 10 seconds
  ble.notifySensorData(now);

  // execute all channels
  execute.execute(device.ch1, now);
  execute.execute(device.ch2, now);
  execute.execute(device.ch3, now);
  execute.execute(device.ch4, now);
  execute.execute(device.ch5, now);
  execute.execute(device.ch6, now);
  execute.execute(device.ch7, now);
  execute.execute(device.ch8, now);

  delay(1000);
}