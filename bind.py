import ctypes as c
from os import path
import numpy as np

c_float_ptr = c.POINTER(c.c_float)

lib = c.CDLL(path.abspath('lib.so'))


class c_bilin(c.Structure):
    _fields_ = [(s, c.c_float) for s in
                ['a_0', 'a_1', 'b_0', 'b_1',
                'x_1', 'y_1']]


class c_biquad(c.Structure):
    _fields_ = [(s, c.c_float) for s in
                ['a_0', 'a_1', 'a_2', 'b_0', 'b_1', 'b_2'
                'x_1', 'x_2', 'y_1', 'y_2']]


bilin_lowpass = lib.bilin_lowpass
bilin_highpass = lib.bilin_highpass
bilin_allpass = lib.bilin_allpass
for bilin in bilin_lowpass, bilin_highpass, bilin_allpass:
    bilin.argtypes = [c.c_float]
    bilin.restype = c_bilin
bilin_lowshelf = lib.bilin_lowshelf
bilin_highshelf = lib.bilin_highshelf
for bilin in bilin_lowshelf, bilin_highshelf:
    bilin.argtypes = [c.c_float, c.c_float]
    bilin.restype = c_bilin

bilin_process = lib.bilin_process
bilin_process.argtypes = [c_float_ptr,
                          c_float_ptr,
                          c.c_uint64,
                          c.POINTER(c_bilin)]
bilin_process.restype = None


def bilin(signal, filter):
    result = np.empty_like(signal)
    bilin_process(signal.ctypes.data_as(c_float_ptr),
                  result.ctypes.data_as(c_float_ptr),
                  len(signal), c.pointer(filter))
    return result


biquad_lowpass = lib.biquad_lowpass
biquad_highpass = lib.biquad_highpass
biquad_bandpass = lib.biquad_bandpass
biquad_notch = lib.biquad_notch
biquad_allpass = lib.biquad_allpass
for biquad in biquad_lowpass, biquad_highpass, biquad_bandpass, biquad_notch, biquad_allpass:
    biquad.argtypes = [c.c_float, c.c_float]
    biquad.restype = c_biquad
biquad_peakeq = lib.biquad_peakeq
biquad_lowshelf = lib.biquad_lowshelf
biquad_highshelf = lib.biquad_highshelf
for biquad in biquad_peakeq, biquad_lowshelf, biquad_highshelf:
    biquad.argtypes = [c.c_float, c.c_float, c.c_float]
    biquad.restype = c_biquad

biquad_proc = lib.biquad_process
biquad_proc.argtypes = [c_float_ptr,
                        c_float_ptr,
                        c.c_uint64,
                        c.POINTER(c_biquad)]
biquad_proc.restype = None


def biquad(signal, filter):
    result = np.empty_like(signal)
    biquad_proc(signal.ctypes.data_as(c_float_ptr),
                result.ctypes.data_as(c_float_ptr),
                len(signal), c.pointer(filter))
    return result
