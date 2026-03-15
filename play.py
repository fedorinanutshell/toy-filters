from bind import biquad_hp, biquad

import sounddevice as sd
import soundfile as sf
import numpy as np

data, fs = sf.read('birds.ogg', dtype=np.float32)

f = biquad_hp(.2 * np.pi, 2.)
data = biquad(data, f)

sd.play(data, fs)
