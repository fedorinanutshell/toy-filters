from bind import biquad_peq, biquad

import sounddevice as sd
import soundfile as sf
import numpy as np

data, fs = sf.read('birds.ogg', dtype=np.float32)

f = biquad_peq(.1 * np.pi, 10., 5.)
data = biquad(data, f)

sd.play(data, fs)
