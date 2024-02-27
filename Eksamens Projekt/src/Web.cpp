#include <Web.h>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define ble_ServerName "Min Robot"
#define Service_UUID "ce2240b9-dce8-4fb5-8a91-c58eb1905869"
#define Characteristic_UUID "d6efa987-2a50-4c40-bf6e-953baf9b916c"

callbackChange newChangeCallback;
callbackUpdate newUpdateCallback;

class MyServerCallbacks : public BLEServerCallbacks
{
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    // Get the new WIFI & Password on the form "SSID;Password"
    if (WiFi.status() == WL_CONNECTED){
      return;
    }

    std::string command = pCharacteristic->getValue();

    size_t colonPos = command.find(':');

    // Extract SSID (substring before colon)
    std::string SSID = command.substr(0, colonPos);

    // Extract password (substring after colon)
    std::string password = command.substr(colonPos + 1);

    log_e("%s, %s", SSID.c_str(), password.c_str());

    init_web(SSID.c_str(), password.c_str(), newChangeCallback, newUpdateCallback);
  };

  void onRead(BLECharacteristic *pCharacteristic){
    log_e("Hi mor");
  };
};

void BeginParing(callbackChange newChangeCallback, callbackUpdate newUpdateCallback)
{
  newChangeCallback = newChangeCallback;
  newUpdateCallback = newUpdateCallback;

  // Start paring with bluetooth.
  BLEDevice::init(ble_ServerName);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(Service_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      Characteristic_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(Service_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  log_i("Started advertising!!!");
};

void GotWIFILogin(std::string SSID, std::string Password, BLECharacteristic *pCharacteristic)
{
  // Got WIFI login. Check if it works. If it works, then start WIFI and end bluetooth. If it doesn't work then continue bluetooth.


  //Indicate the gotten IP address & port on WIFI


  //End Bluetooth on Connection
  //BLEDevice::stopAdvertising();
};