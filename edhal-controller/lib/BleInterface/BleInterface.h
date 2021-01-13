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
      Serial.println("Ble device connected");
    };

    void onDisconnect(BLEServer* pServer) {
      // deviceConnected = false;
      // // code here for when device disconnects
      Serial.println("Ble device disconnected");
    }
};

class BleInterface{

    public:
        // constructor
        BleInterface(void);
        // destructor
        ~BleInterface(void);
        // ble start function
        void begin( Device device );
    private:

        bool deviceConnected = false;
        BLEServer *pServer = NULL;
        BLEService *pConnectService = NULL;
        BLECharacteristic *pIdCharacteristic = NULL;


};


#endif