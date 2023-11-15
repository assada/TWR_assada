#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLESecurity.h>

#include "gui/guiDisplay.h"

#define BLE_DEVICE_NAME "DeadAPRS"
#define BLE_DEVICE_UUID "00000001-ba2a-46c9-ae49-01b0961f68bb"
#define BLE_DEVICE_UUID_TX "00000002-ba2a-46c9-ae49-01b0961f68bb"
#define BLE_DEVICE_UUID_RX "00000003-ba2a-46c9-ae49-01b0961f68bb"

class Bt
{
    public:
        Bt(Stream *monSerial);
        void Start(GuiDisplay *guiDisplay);
        void startAdvertising(BLEServer *pAprsBLEServer);
        GuiDisplay *guiDisplay;
        Stream *monSerial;
};

class AprsBTServerCallbacks final : public BLEServerCallbacks
{

    public:
        AprsBTServerCallbacks(Bt *bt);
        void onConnect(BLEServer *pServer) override;
        void onDisconnect(BLEServer *pServer) override;
    private:
        Bt *bt;
};

class AprsBTCharacteristicCallbacks final : public BLECharacteristicCallbacks
{
    public:
        AprsBTCharacteristicCallbacks(Bt *bt);
        void onWrite(BLECharacteristic *pCharacteristic) override;
        void onRead(BLECharacteristic *pCharacteristic) override;
    private:
        Bt *bt;
};