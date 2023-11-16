#pragma once
#include <vector>

#include "afsk/AFSK.h"

#include "AT1846S.h"

#define EU_APRS_FREQ 144.8

class Radio
{
    public:
        Radio(AT1846S *at1846s);
        void Start();
        void Test();
    private:
        std::vector<bool> stringToBoolVector(const std::string& binaryString);
        AT1846S *at1846s;
};