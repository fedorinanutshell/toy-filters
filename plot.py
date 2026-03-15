from bind import bilin_lp, bilin

import ctypes as c
import numpy as np
from matplotlib import pyplot as plt

x = np.zeros(2**16, np.float32)
x[0] = 1.

bl = bilin_lp(.2 * np.pi)

y = bilin(x, bl)

freq = np.fft.rfftfreq(len(y))
resp = np.fft.rfft(y)

plt.style.use('dark_background')

plt.plot(freq, np.abs(resp))
plt.show()
