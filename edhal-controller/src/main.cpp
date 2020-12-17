#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <SPIFFS.h>
#include "Device.h"

Device device;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);

  // mount file system
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }

  // Serial outputs hw_version, fw_version, mac address, device id, blutooth name
  device.serialOutInit();

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(26, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  delay(1000);
}