#include "Radio.h"

AFSK afsk;

Radio::Radio(AT1846S *at1846s)
{
    this->at1846s = at1846s;
}

void Radio::Start()
{
    // this->at1846s->init();
    // this->at1846s->setBand(BAND::BW_25K0);
    // this->at1846s->setMode(MODE::MODE_FM);
    // this->at1846s->setFrequency(EU_APRS_FREQ);
}

void Radio::Test() {
    //TODO: Example AFSK modulation for AX.25 binary frame
    std::string AX25binFrame = "01111110010000010000010100110001010010010100100101111001000001100101010100010101011001100101010101100101011101010000011001110101010010010001000101010001010001100000001001000110011101010100100100010001010100010010011000000010101001101100000000001111101110100100011000010110000001100011011000100110011001100000101101010110000001100110011001000110001110100010011001000110001110010111101000000110011001100000011001100110000001100011101000010110001101100101000101000011011110100100000101011110000001100000011000000110011001100100111000000110010000100111011101111101001101011010000100110110101111010001111110";
    std::vector<bool> dataVector = stringToBoolVector(AX25binFrame);

    std::vector<double> modulated = afsk.encode(dataVector);

    Serial.println(dataVector.size());
    Serial.println(modulated.size());
}

std::vector<bool> Radio::stringToBoolVector(const std::string& binaryString) { //TODO: only for tesing
    std::vector<bool> boolVector;

    for (char c : binaryString) {
        boolVector.push_back(c == '1');
    }

    return boolVector;
}