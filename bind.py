import ctypes as c
from os import path
import numpy as np

c_float_ptr = c.POINTER(c.c_float)

lib = c.CDLL(path.abspath('lib.so'))


class c_bilin(c.Structure):
    _fields_ = [(s, c.c_float) for s in
                ['a_0', 'a_1', 'b_0', 'b_1',
                'x_1', 'y_1']]


class bilin:
    def __init__(self, c):
        self.c = c

    def __call__(self, signal):
        result = np.empty_like(signal)
        bilin_process(signal.ctypes.data_as(c_float_ptr),
                      result.ctypes.data_as(c_float_ptr),
                      len(signal), c.pointer(self.c))
        return result


for name in 'bilin_lowpass', 'bilin_highpass', 'bilin_allpass':
    fun = getattr(lib, name)
    fun.argtypes = [c.c_float]
    fun.restype = c_bilin
    globals()[name] = lambda o, f=fun: bilin(f(o))
for name in 'bilin_lowshelf', 'bilin_highshelf':
    fun = getattr(lib, name)
    fun.argtypes = [c.c_float, c.c_float]
    fun.restype = c_bilin
    globals()[name] = lambda o, g, f=fun: bilin(f(o, g))

bilin_process = lib.bilin_process
bilin_process.argtypes = [c_float_ptr,
                          c_float_ptr,
                          c.c_uint64,
                          c.POINTER(c_bilin)]
bilin_process.restype = None


class c_biquad(c.Structure):
    _fields_ = [(s, c.c_float) for s in
                ['a_0', 'a_1', 'a_2', 'b_0', 'b_1', 'b_2',
                'x_1', 'x_2', 'y_1', 'y_2']]


class biquad:
    def __init__(self, c):
        self.c = c

    def __call__(self, signal):
        result = np.empty_like(signal)
        biquad_proc(signal.ctypes.data_as(c_float_ptr),
                    result.ctypes.data_as(c_float_ptr),
                    len(signal), c.pointer(self.c))
        return result


for name in 'biquad_lowpass', 'biquad_highpass', \
        'biquad_bandpass', 'biquad_notch', 'biquad_allpass':
    fun = getattr(lib, name)
    fun.argtypes = [c.c_float, c.c_float]
    fun.restype = c_biquad
    globals()[name] = lambda o, q, f=fun: biquad(f(o, q))
for name in 'biquad_lowshelf', 'biquad_highshelf', 'biquad_peakeq':
    fun = getattr(lib, name)
    fun.argtypes = [c.c_float, c.c_float, c.c_float]
    fun.restype = c_biquad
    globals()[name] = lambda o, q, g, f=fun: biquad(f(o, q, g))

biquad_proc = lib.biquad_process
biquad_proc.argtypes = [c_float_ptr,
                        c_float_ptr,
                        c.c_uint64,
                        c.POINTER(c_biquad)]
biquad_proc.restype = None
