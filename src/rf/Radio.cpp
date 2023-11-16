#include "Radio.h"

Radio::Radio(AT1846S *at1846s)
{
    this->at1846s = at1846s;
}

void Radio::Start()
{
    this->at1846s->init();
    this->at1846s->setBand(BAND::BW_25K0);
    this->at1846s->setMode(MODE::MODE_FM);
    this->at1846s->setFrequency(EU_APRS_FREQ);
}