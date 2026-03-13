from bind import c_float_ptr, bilin_lp, bilin_proc

import ctypes as c
import numpy as np
from matplotlib import pyplot as plt

x = np.zeros(2**16, np.float32)
x[0] = 1.
y = np.empty_like(x)

bl = bilin_lp(.2 * np.pi)

bilin_proc(x.ctypes.data_as(c_float_ptr),
           y.ctypes.data_as(c_float_ptr),
           len(x), c.pointer(bl))

freq = np.fft.rfftfreq(len(y))
resp = np.fft.rfft(y)

plt.style.use('dark_background')

plt.plot(freq, np.abs(resp))
plt.show()
