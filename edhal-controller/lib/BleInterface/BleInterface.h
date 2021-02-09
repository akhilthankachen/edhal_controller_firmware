#ifndef BleInterface_h
#define BleInterface_h

#include "Arduino.h"
#include "Device.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// froward declaration
class BleInterface;

// ble server callback for when a device is connected
class BLECustomServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      // deviceConnected = true;
      // // code here for when device connects
      Serial.println();
      Serial.println("Ble device connected");
    };

    void onDisconnect(BLEServer* pServer) {
      // deviceConnected = false;
      // // code here for when device disconnects
      Serial.println();
      Serial.println("Ble device disconnected");
    }
};

// ble channel set configuration callback
class setConfigCustomCallback: public BLECharacteristicCallbacks {

  public:
    setConfigCustomCallback(BleInterface* ble){
      bleObj = ble;
    }
    BleInterface* bleObj;
    void onWrite(BLECharacteristic *pCharacteristic);

};

// ble channel get state callback
class getStateCustomCallback: public BLECharacteristicCallbacks {

  public:
    getStateCustomCallback(BleInterface* ble){
      bleObj = ble;
    }
    BleInterface* bleObj;
    void onRead(BLECharacteristic *pCharacteristic);

};

// ble channel set state callback
class setStateCustomCallback: public BLECharacteristicCallbacks {

  public:
    setStateCustomCallback(BleInterface* ble){
      bleObj = ble;
    }
    BleInterface* bleObj;
    void onWrite(BLECharacteristic *pCharacteristic);

};

class BleInterface{

    public:
        // device class object
        Device *deviceObj;
        // constructor
        BleInterface(void);
        // destructor
        ~BleInterface(void);
        // ble start function
        void begin( Device *device );
        // notify sensor data
        void notifySensorData(DateTime);

    private:
    
        bool deviceConnected = false;
        BLEServer *pServer = NULL;
        BLEService *pConnectService = NULL;
        BLECharacteristic *pIDCharacteristic = NULL;
        BLECharacteristic *pVCharacteristic = NULL;
        BLEService *pSensorService = NULL;
        BLECharacteristic *pBMECharacteristic = NULL;
        BLEService *pTimeSyncService = NULL;
        BLECharacteristic *pTimeSyncCharacteristic = NULL;
        BLEService *pChannelService = NULL;
        BLECharacteristic *pSetConfigCharacteristic = NULL;
        BLECharacteristic *pGetStateCharacteristic = NULL;
        BLECharacteristic *pSetStateCharacteristic = NULL;

};


#endif