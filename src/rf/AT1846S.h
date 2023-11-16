#pragma once
#include "SA868.h"

enum BAND
{
    BW_25K0 = 0,
    BW_12K5 = 1
};

enum MODE
{
    MODE_FM = 0,
    MODE_4FSK = 1
};

class AT1846S
{
    public:
        AT1846S(SA868 *sa868);
        void init();
        void setBand(BAND band);
        void setMode(MODE mode);
        void setFrequency(double freq);
        void reload();
    private:
        SA868 *sa868;
        void mask_set_reg(unsigned int reg, unsigned int mask, unsigned int value);
};
