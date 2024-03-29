#include "BleInterface.h"
#include "Device.h"

#define SERVICE_UUID_CONNECT              "2dacf9b8-8f25-4027-bf0e-9c68b319ef9a"
#define CHARACTERISTIC_UUID_ID            "c39ac656-9bb5-4498-80a6-b55ca7650c2a"
#define CHARACTERISTIC_UUID_V             "35636837-b272-471e-852e-679a3b3cc594"

#define SERVICE_UUID_TIMESYNC             "ce57aa1b-a763-4e73-b93b-048408ab600d"
#define CHARACTERISTIC_UUID_TIMESYNC      "26be97a6-2e25-4701-aba3-f7a17a3a1c74"

#define SERVICE_UUID_SENSOR               "0084cec8-e46c-484a-975a-b2534cfb0674"
#define CHARACTERISTIC_UUID_BME_DATA      "a72f65f8-9166-403e-b4b1-8a351e716dae"

#define SERVICE_UUID_CHANNEL              "188a838a-58d0-4864-b355-20a91270b3c2"
#define CHARACTERISTIC_UUID_SETCONFIG     "54d01acb-2d44-41d2-8c3b-ef514ec965db"
#define CHARACTERISTIC_UUID_GETSTATE      "a3691268-7c5c-4ef6-bada-128981758bb9"
#define CHARACTERISTIC_UUID_SETSTATE      "84f771eb-e00b-4dc4-b02d-7f4e99163fe5"

// setConfigCustomCallback onwrite function
void setConfigCustomCallback::onWrite(BLECharacteristic *pCharacteristic){
    // when a new value is written on this characteristic
    std::string value = pCharacteristic->getValue();

    // send the string to device class's handleConfigJson function
    bleObj->deviceObj->handleConfigJson(value.c_str());
}

// getStateCustomCallback onRead function
void getStateCustomCallback::onRead(BLECharacteristic *pCharacteristic){
    // get state json from device class
    char *json = bleObj->deviceObj->getState();
    // set current state
    pCharacteristic->setValue(json);
    // free inited character array
    free(json);
}

// setStateCustomCallback onwrite function
void setStateCustomCallback::onWrite(BLECharacteristic *pCharacteristic){
    // when a new value is written on this characteristic
    std::string value = pCharacteristic->getValue();

    // send the string to device class's handleConfigJson function
    bleObj->deviceObj->setState(value.c_str());
}

// BleInterface class constructor
BleInterface::BleInterface(void){

}

// BleInterface class destructor
BleInterface::~BleInterface(void){

}

// initiate bluetooth interface and start
void BleInterface::begin( Device *device ){

    // store device object for later use
    deviceObj = device;

    // get bluetooth name from device class
    char *ble_name = device->getDeviceBleName();
    
    // get device id from device class
    char *device_id = device->getDeviceId();
    
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

    ////// TIMESYNC SERVICE BLOCK START //////
    //////////////////////////////////////////

    // init timesync service
    pTimeSyncService = pServer->createService(SERVICE_UUID_TIMESYNC);

    // TimeSync characteristic
    pTimeSyncCharacteristic = pTimeSyncService->createCharacteristic(
        CHARACTERISTIC_UUID_TIMESYNC,
        BLECharacteristic::PROPERTY_WRITE
    );

    ////// TIMESYNC SERVICE BLOCK END //////
    ////////////////////////////////////////

    ////// SENSOR SERVICE BLOCK START //////
    ////////////////////////////////////////

    // init Sensor service
    pSensorService = pServer->createService(SERVICE_UUID_SENSOR);

    // bme sensor data charcteristic
    pBMECharacteristic = pSensorService->createCharacteristic(
      CHARACTERISTIC_UUID_BME_DATA,
      BLECharacteristic::PROPERTY_READ 
    );

    ////// SENSOR SERVICE BLOCK END //////
    //////////////////////////////////////

    ////// CHANNEL SERVICE BLOCK START //////
    ////////////////////////////////////////

    // init channel service
    pChannelService = pServer->createService(SERVICE_UUID_CHANNEL);

    // channel set configuration characteristic
    pSetConfigCharacteristic = pChannelService->createCharacteristic(
        CHARACTERISTIC_UUID_SETCONFIG,
        BLECharacteristic::PROPERTY_WRITE
    );

    pGetStateCharacteristic = pChannelService->createCharacteristic(
        CHARACTERISTIC_UUID_GETSTATE,
        BLECharacteristic::PROPERTY_READ
    );

    pSetStateCharacteristic = pChannelService->createCharacteristic(
        CHARACTERISTIC_UUID_SETSTATE,
        BLECharacteristic::PROPERTY_WRITE
    );

    // setting callback for when new value is written
    pSetConfigCharacteristic->setCallbacks(new setConfigCustomCallback(this));

    // getstate callback for when read is called
    pGetStateCharacteristic->setCallbacks(new getStateCustomCallback(this));

    // setState callback for when new channel is written
    pSetStateCharacteristic->setCallbacks(new setStateCustomCallback(this));

    ////// CHANNEL SERVICE BLOCK START //////
    ////////////////////////////////////////


    // start connect service
    pConnectService->start();
    pSensorService->start();
    pChannelService->start();


    //// Start Advertising ////
    pServer->getAdvertising()->addServiceUUID(SERVICE_UUID_CONNECT);
    pServer->getAdvertising()->start();
    Serial.println();
    Serial.println("Bluetooth Advertising started...");

}   

// last seconds point
int lastSecond = 0;

void BleInterface::notifySensorData(DateTime now){
    //notify only in every 10 seconds
    if( now.second() % 10 == 0 && lastSecond != ( now.second() / 10 )){
        // get data from bme sensor
        char* data = deviceObj->getBmeSensorData();

        // write sensor data to charcteristic
        pBMECharacteristic->setValue(data);

        // notify
        //pBMECharacteristic->notify();

        // print data
        Serial.println();
        Serial.println();
        Serial.println(data);

        lastSecond = now.second()/10;

        Serial.print(now.year());
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" (");
        Serial.print(now.dayOfTheWeek());
        Serial.print(") ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
        Serial.println();

        free(data);
    }
}