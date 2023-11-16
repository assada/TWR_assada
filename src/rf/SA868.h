#pragma once

#include <Arduino.h>

class SA868 {
    public:
        SA868(HardwareSerial *hwser);
        String getVersion() const;
        String getHardware() const;
        bool poke(uint16_t reg, uint16_t val);
        unsigned int pick(uint16_t reg) const;
        bool waitResponse(String &data, String rsp, uint32_t timeout) const;
    private:
        HardwareSerial *ATSer;
};