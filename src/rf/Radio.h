#pragma once
#include "AT1846S.h"

#define EU_APRS_FREQ 144.8

class Radio
{
    public:
        Radio(AT1846S *at1846s);
        void Start();
    private:
        AT1846S *at1846s;
};