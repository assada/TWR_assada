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
//UT3USW-4>APFIIO,WIDE1-1,WIDE2-1:/140623h5031.21N/03030.46Ea/A=000390!w?k!



//TODO: Temp (shit) solution

typedef char passtime_t[30];

#define KISS_FEND 0xC0
#define KISS_FESC 0xDB
#define KISS_TFEND 0xDC
#define KISS_TFESC 0xDD
#define MARK_GSTIME 0xBD
bool decode_kiss(const char *in, const size_t in_len, size_t *in_ptr, char *out, size_t *out_len, size_t out_capacity, passtime_t *pt)
{
	bool fend = false;
	bool esc = false;
	*out_len = 0;
	(*pt)[0] = 0;
	for (; *in_ptr < in_len; ++*in_ptr) {
		unsigned char c = in[*in_ptr];
        Serial.println(c, HEX);
		if (c == KISS_FEND && !fend) {
			fend = true;
            Serial.println("KISS_TFEND");
			continue;
		}
		if (esc) {
			if (c == KISS_TFEND) {
                Serial.println("KISS_TFEND");
				c = KISS_FEND;
			} else if (c == KISS_TFESC) {
                Serial.println("KISS_TFESC");
				c = KISS_FESC;
			} else if (c == MARK_GSTIME) {
                Serial.println("MARK_GSTIME");
				size_t i;
				for (i = 0; i < sizeof(passtime_t) - 1; i++) {
					++*in_ptr;
					if (*in_ptr == in_len) {
						i = 0;
						break;
					}
					char x = in[*in_ptr];
					(*pt)[i] = x;
					if (x == 0) {
						break;
					}
				}
				(*pt)[i] = 0;
				esc = false;
				continue;
			} else {
				return false;
			}
		} else if (c == KISS_FESC) {
            Serial.println("KISS_FESC");
			esc = true;
			continue;
		} else if (c == KISS_FEND) {
            Serial.println("KISS_FEND");
			if (*out_len > 0) {
				break;
			} else {
				continue;
			}
		}
		esc = false;
		if (*out_len == out_capacity) {
			return false;
		}
		out[(*out_len)++] = c;
	}
	return fend;
}


void AprsBTCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    uint8_t* rxData = pCharacteristic->getData();
    size_t rxLength = pCharacteristic->getLength();

    if (rxData != nullptr && rxLength > 0)
    {

        for(size_t i = 0; i < rxLength; i++) {
            Serial.printf("%02X ", rxData[i]); //JUST FOR DEBUG PURPOSES
        }

        for(size_t i = 0; i < rxLength; i++) {
            Serial.printf("%u", rxData[i]); //JUST FOR DEBUG PURPOSES
        }

        char out_buf[329];
        size_t out_len = 0;
        size_t in_ptr = 0;
        passtime_t pt;

        if (decode_kiss((const char*)rxData, rxLength, &in_ptr, out_buf, &out_len, sizeof(out_buf), &pt)) // REAL THING
        {
            String decodedString(out_buf, out_len);
            this->bt->monSerial->println(decodedString);
        }
        else
        {
            this->bt->monSerial->println("Error in KISS decoding");
        }
    }
}