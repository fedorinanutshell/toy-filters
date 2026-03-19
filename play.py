from bind import biquad_bandpass

import sounddevice as sd
import soundfile as sf
import numpy as np

data, fs = sf.read('birds.ogg', dtype=np.float32)

data = biquad_bandpass(.038 * np.pi, 4.)(data)

sd.play(data, fs)
