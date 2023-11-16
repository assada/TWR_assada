import math
import itertools
from bitarray import bitarray

MARK_HZ = 1200.0
SPACE_HZ = 2200.0
BAUD_RATE = 1200.0

FRAME_RATE=44100

TWO_PI = 2.0 * math.pi

def nrzi(data):
    current = True
    for bit in data:
        if not bit:
            current = not current
        yield current
def modulate(data):
    seconds_per_sample = 1.0 / FRAME_RATE
    phase, seconds, bits = 0, 0, 0

    clock = (x / BAUD_RATE for x in itertools.count(1))
    tones = (MARK_HZ if bit else SPACE_HZ for bit in data)

    for boundary, frequency in zip(clock, tones):
        phase_change_per_sample = TWO_PI / (FRAME_RATE / frequency)
        while seconds < boundary:
            yield math.sin(phase)

            seconds += seconds_per_sample
            phase += phase_change_per_sample

            if phase > TWO_PI:
                phase -= TWO_PI

        bits += 1

def silence(seconds=None):
    if seconds != None:
        for i in range(int(FRAME_RATE * seconds)):
            yield 0
    else:
        while True:
            yield 0

def encode(binary_data):
    framed_data = nrzi(binary_data)

    for sample in itertools.chain(
            silence(1.05),
            modulate(framed_data),
            silence(1.05),
    ):
        yield sample