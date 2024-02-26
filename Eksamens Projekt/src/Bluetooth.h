#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic);

  bool get_bit(uint32_t num, uint32_t position);

  void print_binary(uint32_t num);
};

class Bluetooth{
  public:
  Bluetooth(char* Service_UUID, char* Characteristic_UUID);

  void Setup();
  void SendMessage(char* message);
  

  private:

  char* Service_UUID;
  char* Characteristic_UUID;
}