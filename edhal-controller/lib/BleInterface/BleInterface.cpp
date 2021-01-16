#include "BleInterface.h"
#include "Device.h"

#define SERVICE_UUID_CONNECT            "2dacf9b8-8f25-4027-bf0e-9c68b319ef9a"
#define CHARACTERISTIC_UUID_ID          "c39ac656-9bb5-4498-80a6-b55ca7650c2a"
#define CHARACTERISTIC_UUID_V           "35636837-b272-471e-852e-679a3b3cc594"

#define SERVICE_UUID_TIMESYNC           "ce57aa1b-a763-4e73-b93b-048408ab600d"
#define CHARACTERISTIC_UUID_TIMESYNC    "26be97a6-2e25-4701-aba3-f7a17a3a1c74"

#define SERVICE_UUID_SENSOR             "0084cec8-e46c-484a-975a-b2534cfb0674"
#define CHARACTERISTIC_UUID_BME_DATA    "a72f65f8-9166-403e-b4b1-8a351e716dae"

// BleInterface class constructor
BleInterface::BleInterface(void){

}

// BleInterface class destructor
BleInterface::~BleInterface(void){

}

// initiate bluetooth interface and start
void BleInterface::begin( Device device ){

    // store device object for later use
    deviceObj = device;

    // get bluetooth name from device class
    char *ble_name = device.getDeviceBleName();
    
    // get device id from device class
    char *device_id = device.getDeviceId();
    
    // get version from device class
    uint8_t version[5] = {HARDWARE_VERSION_MAJOR, HARDWARE_VERSION_MINOR, FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR, FIRMWARE_VERSION_PATCH};
    
    // Create the Ble Device
    BLEDevice::init(ble_name);

    // Create Ble server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLECustomServerCallbacks());
    
    ////// CONNECT SERVICE BLOCK START //////
    ////////////////////////////////////////

    // init connect service
    pConnectService = pServer->createService(SERVICE_UUID_CONNECT);

    // ID Characteristic
    pIDCharacteristic = pConnectService->createCharacteristic(
        CHARACTERISTIC_UUID_ID,
        BLECharacteristic::PROPERTY_READ
    );
    // Version Characteristic
    pVCharacteristic = pConnectService->createCharacteristic(
        CHARACTERISTIC_UUID_V,
        BLECharacteristic::PROPERTY_READ
    );

    // set device id in id characteristic
    pIDCharacteristic->setValue(device_id);
    // set device version in version characteristic
    pVCharacteristic->setValue((uint8_t*)version, 5);

    ////// CONNECT SERVICE BLOCK END //////
    //////////////////////////////////////

    ////// SENSOR SERVICE BLOCK START //////
    ////////////////////////////////////////

    // init Sensor service
    pSensorService = pServer->createService(SERVICE_UUID_SENSOR);

    // bme sensor data charcteristic
    pBMECharacteristic = pSensorService->createCharacteristic(
      CHARACTERISTIC_UUID_BME_DATA,
      BLECharacteristic::PROPERTY_READ | 
      BLECharacteristic::PROPERTY_NOTIFY 
    );

    // add discriptor
    pBMECharacteristic->addDescriptor(new BLE2902());

    ////// SENSOR SERVICE BLOCK END //////
    //////////////////////////////////////


    // start connect service
    pConnectService->start();
    pSensorService->start();


    //// Start Advertising ////
    pServer->getAdvertising()->addServiceUUID(SERVICE_UUID_CONNECT);
    pServer->getAdvertising()->start();
    Serial.println("Bluetooth Advertising started...");


    delete[] ble_name;
    delete[] device_id;
}   

// notify sensor data function
void BleInterface::notifySensorData(void){
    
    // get data from bme sensor
    char* data = deviceObj.getBmeSensorData();

    // write sensor data to charcteristic
    pBMECharacteristic->setValue(data);

    // notify
    pBMECharacteristic->notify();

    // print data
    Serial.println(data);

}