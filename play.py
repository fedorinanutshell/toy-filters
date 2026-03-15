from bind import biquad_bp, biquad

import sounddevice as sd
import soundfile as sf
import numpy as np

data, fs = sf.read('birds.ogg', dtype=np.float32)

f = biquad_bp(.038 * np.pi, 4.)
data = biquad(data, f)

sd.play(data, fs)
