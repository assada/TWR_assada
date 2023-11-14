#pragma once

#include <Arduino.h>

class SA868 {
    public:
        SA868(HardwareSerial *hwser);
        String getVersion();
        String getHardware();
        bool waitResponse(String &data, String rsp, uint32_t timeout);
    private:
        HardwareSerial *ATSer;
};