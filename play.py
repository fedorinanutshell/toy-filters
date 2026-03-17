from bind import biquad_bandpass, biquad

import sounddevice as sd
import soundfile as sf
import numpy as np

data, fs = sf.read('birds.ogg', dtype=np.float32)

f = biquad_bandpass(.038 * np.pi, 4.)
data = biquad(data, f)

sd.play(data, fs)
