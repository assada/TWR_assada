#include <cmath>
#include "AFSK.h"

std::vector<bool> AFSK::nrzi(const std::vector<bool>& data) {
    bool current = true;
    std::vector<bool> output;

    for (bool bit : data) {
        if (!bit) {
            current = !current;
        }
        output.push_back((bool)current);
    }

    return output;
}

std::vector<double> AFSK::modulate(const std::vector<bool>& data) {
    constexpr double seconds_per_sample = 1.0 / FRAME_RATE;
    double phase = 0, seconds = 0;
    std::vector<double> samples;

    for (size_t x = 1; ; ++x) {
        double boundary = x / AFSK_BD;

        if (x - 1 >= data.size()) {
            break;
        }

        double frequency = data[x - 1] ? MARK_HZ : SPACE_HZ;
        double phase_change_per_sample = TWO_PI2 * frequency / FRAME_RATE;

        while (seconds < boundary) {
            samples.push_back(sin(phase));

            seconds += seconds_per_sample;
            phase += phase_change_per_sample;

            if (phase > TWO_PI2) {
                phase -= TWO_PI2;
            }
        }
    }

    return samples;
}

std::vector<double> AFSK::silence(double seconds) {
    size_t numSamples = static_cast<size_t>(FRAME_RATE * seconds);

    return std::vector<double>(numSamples, 0.0);
}

std::vector<double> AFSK::multiply(const std::vector<double>& data, double multiplier)
{
    std::vector<double> output;

    for (double sample : data) {
        output.push_back(sample * multiplier);
    }

    return output;
}

std::vector<double> AFSK::encode(const std::vector<bool>& binary_data) {
    std::vector<double> modulated_data = this->multiply(this->modulate(nrzi(binary_data)), 0.5);

    std::vector<double> encoded_data;

    std::vector<double> pre_silence = silence(1.05);
    encoded_data.insert(encoded_data.end(), pre_silence.begin(), pre_silence.end());

    encoded_data.insert(encoded_data.end(), modulated_data.begin(), modulated_data.end());

    std::vector<double> post_silence = silence(1.05);
    encoded_data.insert(encoded_data.end(), post_silence.begin(), post_silence.end());

    return encoded_data;
}
