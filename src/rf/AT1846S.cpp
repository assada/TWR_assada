#include "AT1846S.h"

AT1846S::AT1846S(SA868* sa868)
{
    this->sa868 = sa868;
}

void AT1846S::mask_set_reg(unsigned int reg, unsigned int mask, unsigned int value)
{
    unsigned int regVal = this->sa868->pick(reg);
    regVal = (regVal & ~mask) | (value & mask);
    this->sa868->poke(reg, regVal);
}

void AT1846S::init()
{
    this->sa868->poke(0x30, 0x0001); // Soft reset
    delay(160);
    this->sa868->poke(0x30, 0x0004); // Set pdn_reg (power down pin)

    this->sa868->poke(0x04, 0x0FD0); // Set clk_mode to 25.6MHz/26MHz
    this->sa868->poke(0x0A, 0x7C20); // Set 0x0A to its default value
    this->sa868->poke(0x13, 0xA100);
    this->sa868->poke(0x1F, 0x1001); // Set gpio0 to ctcss_out/css_int/css_cmp
                                            // and gpio6 to sq, sq&ctcss/cdcss when sq_out_set=1
    this->sa868->poke(0x31, 0x0031);
    this->sa868->poke(0x33, 0x44A5);
    this->sa868->poke(0x34, 0x2B89);
    this->sa868->poke(0x41, 0x4122); // Set voice_gain_tx (voice digital gain) to 0x22
    this->sa868->poke(0x42, 0x1052);
    this->sa868->poke(0x43, 0x0100);
    this->sa868->poke(0x44, 0x07FF); // Set gain_tx (voice digital gain after tx ADC downsample) to 0x7
    this->sa868->poke(0x59, 0x0B90); // Set c_dev (CTCSS/CDCSS TX FM deviation) to 0x10
                                            // and xmitter_dev (voice/subaudio TX FM deviation) to 0x2E
    this->sa868->poke(0x47, 0x7F2F);
    this->sa868->poke(0x4F, 0x2C62);
    this->sa868->poke(0x53, 0x0094);
    this->sa868->poke(0x54, 0x2A3C);
    this->sa868->poke(0x55, 0x0081);
    this->sa868->poke(0x56, 0x0B02);
    this->sa868->poke(0x57, 0x1C00);
    this->sa868->poke(0x58, 0x9CDD); // Bit 0  = 1: CTCSS LPF bandwidth to 250Hz
                                            // Bit 3  = 1: bypass CTCSS HPF
                                            // Bit 4  = 1: bypass CTCSS LPF
                                            // Bit 5  = 0: enable voice LPF
                                            // Bit 6  = 1: bypass voice HPF
                                            // Bit 7  = 1: bypass pre/de-emphasis
                                            // Bit 11 = 1: bypass VOX HPF
                                            // Bit 12 = 1: bypass VOX LPF
                                            // Bit 13 = 0: normal RSSI LPF bandwidth
    this->sa868->poke(0x5A, 0x06DB);
    this->sa868->poke(0x63, 0x16AD);
    this->sa868->poke(0x67, 0x0628); // Set DTMF C0 697Hz to ???
    this->sa868->poke(0x68, 0x05E5); // Set DTMF C1 770Hz to 13MHz and 26MHz
    this->sa868->poke(0x69, 0x0555); // Set DTMF C2 852Hz to ???
    this->sa868->poke(0x6A, 0x04B8); // Set DTMF C3 941Hz to ???
    this->sa868->poke(0x6B, 0x02FE); // Set DTMF C4 1209Hz to 13MHz and 26MHz
    this->sa868->poke(0x6C, 0x01DD); // Set DTMF C5 1336Hz
    this->sa868->poke(0x6D, 0x00B1); // Set DTMF C6 1477Hz
    this->sa868->poke(0x6E, 0x0F82); // Set DTMF C7 1633Hz
    this->sa868->poke(0x6F, 0x017A); // Set DTMF C0 2nd harmonic
    this->sa868->poke(0x70, 0x004C); // Set DTMF C1 2nd harmonic
    this->sa868->poke(0x71, 0x0F1D); // Set DTMF C2 2nd harmonic
    this->sa868->poke(0x72, 0x0D91); // Set DTMF C3 2nd harmonic
    this->sa868->poke(0x73, 0x0A3E); // Set DTMF C4 2nd harmonic
    this->sa868->poke(0x74, 0x090F); // Set DTMF C5 2nd harmonic
    this->sa868->poke(0x75, 0x0833); // Set DTMF C6 2nd harmonic
    this->sa868->poke(0x76, 0x0806); // Set DTMF C7 2nd harmonic

    this->sa868->poke(0x30, 0x40A4); // Set pdn_pin (power down enable)
                                            // and set rx_on
                                            // and set mute when rxno
                                            // and set xtal_mode to 26MHz/13MHz
    delay(160);

    this->sa868->poke(0x30, 0x40A6); // Start calibration
    delay(160);
    this->sa868->poke(0x30, 0x4006); // Stop calibration
    delay(160);

    this->sa868->poke(0x40, 0x0031);
}

void AT1846S::reload()
{
    unsigned int mode = this->sa868->pick(0x30) & 0x0060; // Get current op. status
    mask_set_reg(0x30, 0x0060, 0x0000);             // RX and TX off
    mask_set_reg(0x30, 0x0060, mode);               // Restore op. status
}


void AT1846S::setBand(BAND band)
{
    switch (band)
    {
    case BW_25K0:
        this->sa868->poke(0x15, 0x1F00);
        this->sa868->poke(0x32, 0x7564);
        this->sa868->poke(0x3A, 0x40C3);
        this->sa868->poke(0x3C, 0x1B34);
        this->sa868->poke(0x3F, 0x29D1);
        this->sa868->poke(0x48, 0x1F3C);
        this->sa868->poke(0x60, 0x0F17);
        this->sa868->poke(0x62, 0x3263);
        this->sa868->poke(0x65, 0x248A);
        this->sa868->poke(0x66, 0xFFAE);
        this->sa868->poke(0x7F, 0x0001);
        this->sa868->poke(0x06, 0x0024);
        this->sa868->poke(0x07, 0x0214);
        this->sa868->poke(0x08, 0x0224);
        this->sa868->poke(0x09, 0x0314);
        this->sa868->poke(0x0A, 0x0324);
        this->sa868->poke(0x0B, 0x0344);
        this->sa868->poke(0x0C, 0x0384);
        this->sa868->poke(0x0D, 0x1384);
        this->sa868->poke(0x0E, 0x1B84);
        this->sa868->poke(0x0F, 0x3F84);
        this->sa868->poke(0x12, 0xE0EB);
        this->sa868->poke(0x7F, 0x0000);

        mask_set_reg(0x30, 0x3000, 0x3000);
    case BW_12K5:
        this->sa868->poke(0x15, 0x1100);
        this->sa868->poke(0x32, 0x4495);
        this->sa868->poke(0x3A, 0x40C3);
        this->sa868->poke(0x3F, 0x29D1);
        this->sa868->poke(0x3C, 0x1B34);
        this->sa868->poke(0x48, 0x19B1);
        this->sa868->poke(0x60, 0x0F17);
        this->sa868->poke(0x62, 0x1425);
        this->sa868->poke(0x65, 0x2494);
        this->sa868->poke(0x66, 0xEB2E);
        this->sa868->poke(0x7F, 0x0001);
        this->sa868->poke(0x06, 0x0014);
        this->sa868->poke(0x07, 0x020C);
        this->sa868->poke(0x08, 0x0214);
        this->sa868->poke(0x09, 0x030C);
        this->sa868->poke(0x0A, 0x0314);
        this->sa868->poke(0x0B, 0x0324);
        this->sa868->poke(0x0C, 0x0344);
        this->sa868->poke(0x0D, 0x1344);
        this->sa868->poke(0x0E, 0x1B44);
        this->sa868->poke(0x0F, 0x3F44);
        this->sa868->poke(0x12, 0xE0EB);
        this->sa868->poke(0x7F, 0x0000);

        mask_set_reg(0x30, 0x3000, 0x0000);
    }

    reload();
}

void AT1846S::setMode(MODE mode)
{
    switch (mode)
    {
    case MODE_FM:
        this->sa868->poke(0x58, 0x9C05);
        this->sa868->poke(0x40, 0x0030);
    case MODE_4FSK: //FOR WHAT? just fo lulz
        this->sa868->poke(0x3A, 0x00C2);
        this->sa868->poke(0x33, 0x45F5);
        this->sa868->poke(0x41, 0x4731);
        this->sa868->poke(0x42, 0x1036);
        this->sa868->poke(0x43, 0x00BB);
        this->sa868->poke(0x58, 0xBCFD);
        this->sa868->poke(0x44, 0x06CC);
        this->sa868->poke(0x40, 0x0031);
    }

    reload();
}

void AT1846S::setFrequency(double freq) //WHY double? why not khz int? TODO: test this shit
{
    uint32_t val = static_cast<uint32_t>((freq / 1000.0) * 16); //fuck casting. use round? lol
    uint16_t fHi = (val >> 16) & 0xFFFF;
    uint16_t fLo = val & 0xFFFF;

    this->sa868->poke(0x29, fHi);
    this->sa868->poke(0x2A, fLo);

    reload();
}
