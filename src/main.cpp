#include <Arduino.h>
#define XPOWERS_CHIP_AXP2101
#include <XPowersLib.h>
#include <Wire.h>
#include "ttwr2.h"
#include "gui/guiDisplay.h"

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(1, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

XPowersPMU PMU;
GuiDisplay guiDisplay;

TaskHandle_t mainDisplayHandle;

#define SerialMon Serial
#define SerialAT  Serial1

void setupPower()
{
    bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
    if (result == false) {
        while (1) {
            Serial.println("PMU is not online...");
            delay(500);
        }
    }

    PMU.setVbusVoltageLimit(XPOWERS_AXP2101_VBUS_VOL_LIM_3V88);
    PMU.setVbusCurrentLimit(XPOWERS_AXP2101_VBUS_CUR_LIM_2000MA);
    PMU.setSysPowerDownVoltage(2600);

    //! DC3 Radio Pixels VDD , Don't change
    PMU.setDC3Voltage(3400);

    PMU.enableDC3();

    PMU.disableTSPinMeasure();

    PMU.enableBattDetection();
    PMU.enableVbusVoltageMeasure();
    PMU.enableBattVoltageMeasure();
    PMU.enableSystemVoltageMeasure();

    PMU.setChargingLedMode(XPOWERS_CHG_LED_BLINK_1HZ);

    pinMode(PMU_IRQ, INPUT_PULLUP);

    PMU.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);

    PMU.clearIrqStatus();

    PMU.enableIRQ(
      XPOWERS_AXP2101_BAT_INSERT_IRQ | XPOWERS_AXP2101_BAT_REMOVE_IRQ |    // BATTERY
      XPOWERS_AXP2101_VBUS_INSERT_IRQ | XPOWERS_AXP2101_VBUS_REMOVE_IRQ |  // VBUS
      XPOWERS_AXP2101_PKEY_SHORT_IRQ | XPOWERS_AXP2101_PKEY_LONG_IRQ |     // POWER KEY
      XPOWERS_AXP2101_BAT_CHG_DONE_IRQ | XPOWERS_AXP2101_BAT_CHG_START_IRQ // CHARGE
    );

    PMU.setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_150MA);
    PMU.setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_1000MA);
    PMU.setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_150MA);
    PMU.setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V2);
    PMU.disableLongPressShutdown();
}


void setup()
{
    Wire.begin(I2C_SDA, I2C_SCL, 400000L);
    pinMode(SA868_PD_PIN, OUTPUT);
    digitalWrite(SA868_PD_PIN, HIGH);

    //! IO39 Can only be set to LOW or floating,
    //! can not be set to HIGH, otherwise it will be damaged
    pinMode(SA868_RF_PIN, OUTPUT);
    digitalWrite(SA868_RF_PIN, LOW);

    pinMode(SA868_PTT_PIN, OUTPUT);
    digitalWrite(SA868_PTT_PIN, HIGH);

    setupPower();

    SerialMon.begin(115200);

    SerialAT.begin(9600, SERIAL_8N1, SA868_RX_PIN, SA868_TX_PIN);

    delay(1000);

    guiDisplay.Start();

    strip.begin();
    strip.show();
}

long timeCheck = 0;

void loop()
{
    vTaskDelay(10 / portTICK_PERIOD_MS);

    if (millis() > timeCheck)
    {
        timeCheck = millis() + 1000;
        if (ESP.getFreeHeap() < 60000)
            esp_restart();
    }

    while (SerialAT.available()) {
        SerialMon.write(SerialAT.read());
    }
    while (SerialMon.available()) {
        SerialAT.write(SerialMon.read());
    }
}


