from bind import biquad_allpass

import numpy as np
from matplotlib import pyplot as plt

x = np.zeros(2**16, np.float32)
x[0] = 1.

y = biquad_allpass(.3 * np.pi, 7.)(x)
print('DC gain:', sum(y))
(z := y.copy())[1::2] *= -1.
print('Nyquist gain:', sum(z))

freq = np.fft.rfftfreq(len(y))
resp = np.fft.rfft(y)

plt.style.use('dark_background')

plt.plot(freq, np.unwrap(np.angle(resp)))
plt.show()
