#include "bt.h"
#include <sstream>

BLEServer *pAprsBLEServer = NULL;
BLECharacteristic *pTx;
BLECharacteristic *pRx;
bool AprsBTdeviceConnected = false;

Bt::Bt(Stream *monSerial)
{
    this->monSerial = monSerial;
}

void Bt::Start(GuiDisplay *guiDisplay)
{
    BLEDevice::init(BLE_DEVICE_NAME);

    pAprsBLEServer = BLEDevice::createServer();
    pAprsBLEServer->setCallbacks(new AprsBTServerCallbacks(this));

    BLEService *pService = pAprsBLEServer->createService(BLE_DEVICE_UUID);

    pTx = pService->createCharacteristic(
        BLE_DEVICE_UUID_TX,
        BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY);

    pRx = pService->createCharacteristic(
        BLE_DEVICE_UUID_RX,
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ);

    pTx->setAccessPermissions(ESP_GATT_PERM_WRITE);
    pRx->setAccessPermissions(ESP_GATT_PERM_READ);
    pRx->addDescriptor(new BLE2902());
    pRx->setCallbacks(new AprsBTCharacteristicCallbacks(this));
    pTx->setCallbacks(new AprsBTCharacteristicCallbacks(this));
    
    pService->start();

    this->startAdvertising(pAprsBLEServer);

    // BLESecurity *pSecurity = new BLESecurity();
    // pSecurity->setStaticPIN(123456); 

    this->guiDisplay->clearDisplay();
    this->guiDisplay->putText("BT_R", 1, 45, 1);
}

void Bt::startAdvertising(BLEServer *pAprsBLEServer)
{
    BLEAdvertising *pAdvertising = pAprsBLEServer->getAdvertising();
    pAdvertising->addServiceUUID(BLE_DEVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    // pAdvertising->setMinPreferred(0x12);
    pAdvertising->start();
}

AprsBTServerCallbacks::AprsBTServerCallbacks(Bt *bt) {
    this->bt = bt;
}

void AprsBTServerCallbacks::onConnect(BLEServer *pServer)
{
    AprsBTdeviceConnected = true;
    this->bt->guiDisplay->clearDisplay();
    this->bt->guiDisplay->putText("BT_C", 1, 45, 1);
    this->bt->monSerial->println("BT Connected");
};

void AprsBTServerCallbacks::onDisconnect(BLEServer *pServer)
{
    AprsBTdeviceConnected = false;
    this->bt->guiDisplay->clearDisplay();
    this->bt->guiDisplay->putText("BT_DC", 1, 45, 1);
    this->bt->startAdvertising(pServer);
    this->bt->monSerial->println("BT Disconnected");
}

AprsBTCharacteristicCallbacks::AprsBTCharacteristicCallbacks(Bt *bt) {
    this->bt = bt;
}

void AprsBTCharacteristicCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
    std::ostringstream os;

    os << "Hello World!";

    pCharacteristic->setValue(os.str());
}

void AprsBTCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    Serial.println("onWrite");
    std::string rxValue = pCharacteristic->getValue(); // AX.25 KISS frame

    if (rxValue.length() > 0) {
        // for (int n = 0; n < rxValue.length(); n++)
        //   kiss_serial((uint8_t)rxValue[n]);
        // TODO: Push bytes array to Queue
    }
}