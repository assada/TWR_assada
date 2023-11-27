#include "Radio.h"
#include "Arduino.h"
#include <ttwr2.h>

#include <driver/i2s.h>
#define I2S_PORT I2S_NUM_0

AFSK afsk;

Radio::Radio(AT1846S *at1846s)
{
    this->at1846s = at1846s;
}

#define PIN_I2S_BCLK    26
#define PIN_I2S_LRC     27
#define PIN_I2S_DIN     35
#define PIN_I2S_DOUT    25

#define bufferCnt 10
#define bufferLen 1024
int16_t sBuffer[bufferLen];

void i2s_install() {
    // Set up I2S Processor configuration
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
        .sample_rate = 44100,
        //.sample_rate = 16000,
        .bits_per_sample = i2s_bits_per_sample_t(16),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0,
        .dma_buf_count = bufferCnt,
        .dma_buf_len = bufferLen,
        .use_apll = false
      };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
    // Set I2S pin configuration
    const i2s_pin_config_t pin_config = {
        .bck_io_num = PIN_I2S_BCLK,
        .ws_io_num = PIN_I2S_LRC,
        .data_out_num = PIN_I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
      };

    i2s_set_pin(I2S_PORT, &pin_config);
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

    i2s_install();
    i2s_setpin();
    i2s_start(I2S_PORT);

    i2s_write(I2S_PORT, modulated.data(), modulated.size() * sizeof(double), nullptr, portMAX_DELAY);


    //Ok. lets try sent it to the radio speker lol, why not?

    //ledcAttachPin(SA8682ESP_AUDIO, 0);

    // for (double sample : modulated) {
        // int scaled_sample = std::max(std::min(static_cast<int>(sample * 32767.0), 32767), -32768);

        // analogWrite(SA8682ESP_AUDIO, scaled_sample);
        //ledcWrite(0, scaled_sample);
        //delay(1);
    // }
    //ledcDetachPin(SA8682ESP_AUDIO);
}

std::vector<bool> Radio::stringToBoolVector(const std::string& binaryString) { //TODO: only for tesing
    std::vector<bool> boolVector;

    for (char c : binaryString) {
        boolVector.push_back(c == '1');
    }

    return boolVector;
}