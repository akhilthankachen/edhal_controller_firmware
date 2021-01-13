#include "BleInterface.h"
#include "Device.h"

#define SERVICE_UUID_CONNECT        "2dacf9b8-8f25-4027-bf0e-9c68b319ef9a"
#define CHARACTERISTIC_UUID_ID      "c39ac656-9bb5-4498-80a6-b55ca7650c2a"

// BleInterface class constructor
BleInterface::BleInterface(void){

}

// BleInterface class destructor
BleInterface::~BleInterface(void){

}

// initiate bluetooth interface and start
void BleInterface::begin( Device device ){

    // get bluetooth name from device class
    char *ble_name = new char[15];
    device.getDeviceBleName(ble_name);

    // get device id from device class
    char *device_id = new char[18];
    device.getDeviceId(device_id);
    
    // Create the Ble Device
    BLEDevice::init(ble_name);

    // Create Ble server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLECustomServerCallbacks());

    //// Create BLE Services ////
    
    // Connect Service
    pConnectService = pServer->createService(SERVICE_UUID_CONNECT);

    //// Create BLE Characteristics ////

    // ID Characteristic
    pIdCharacteristic = pConnectService->createCharacteristic(
        CHARACTERISTIC_UUID_ID,
        BLECharacteristic::PROPERTY_READ
    );

    //// Start Services ////

    // start connect service
    pConnectService->start();

    // set device id in  id characteristic
    pIdCharacteristic->setValue(device_id);

    //// Start Advertising ////
    pServer->getAdvertising()->addServiceUUID(SERVICE_UUID_CONNECT);
    pServer->getAdvertising()->start();
    Serial.println("Bluetooth Advertising started...");


    delete[] ble_name;
    delete[] device_id;
}   