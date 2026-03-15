from bind import bilin_lp, bilin

import sounddevice as sd
import soundfile as sf
import numpy as np

data, fs = sf.read('radio.ogg', dtype=np.float32)

bl = bilin_lp(.5 * np.pi)
data = bilin(data, bl)

sd.play(data, fs)
