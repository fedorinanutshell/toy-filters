from bind import biquad_peq, biquad

import numpy as np
from matplotlib import pyplot as plt

x = np.zeros(2**16, np.float32)
x[0] = 1.

f = biquad_peq(.1 * np.pi, 4., 2.)

y = biquad(x, f)
print(sum(y))

freq = np.fft.rfftfreq(len(y))
resp = np.fft.rfft(y)

plt.style.use('dark_background')

plt.plot(freq, np.abs(resp))
plt.show()
