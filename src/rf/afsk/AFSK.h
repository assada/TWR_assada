#include <vector>

#define MARK_HZ 1200
#define SPACE_HZ 2200
#define AFSK_BD 1200.0

#define FRAME_RATE 44100

#define TWO_PI2 6.28318530718

#define SECCONDS_PER_SAMPLE (1.0 / FRAME_RATE)

#define SWITCH_TONE(inc) (((inc) == MARK_HZ) ? SPACE_HZ : MARK_HZ)



class AFSK {
    public:
        std::vector<double> encode(const std::vector<bool>& binary_data);
    private:
        std::vector<bool> nrzi(const std::vector<bool>& data);
        std::vector<double> modulate(const std::vector<bool>& data);
        std::vector<double> silence(double seconds);
        std::vector<double> multiply(const std::vector<double>& data, double multiplier);
};

