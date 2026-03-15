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


bilin_lp = lib.bilin_lp
bilin_hp = lib.bilin_hp
bilin_ap = lib.bilin_ap
for bilin in bilin_lp, bilin_hp, bilin_ap:
    bilin.argtypes = [c.c_float]
    bilin.restype = c_bilin

bilin_proc = lib.bilin_proc
bilin_proc.argtypes = [c_float_ptr,
                       c_float_ptr,
                       c.c_uint64,
                       c.POINTER(c_bilin)]
bilin_proc.restype = None


def bilin(signal, filter):
    result = np.empty_like(signal)
    bilin_proc(signal.ctypes.data_as(c_float_ptr),
               result.ctypes.data_as(c_float_ptr),
               len(signal), c.pointer(filter))
    return result


biquad_peq = lib.biquad_peq
for biquad in biquad_peq, :
    biquad.argtypes = [c.c_float, c.c_float, c.c_float]
    biquad.restype = c_biquad

biquad_proc = lib.biquad_proc
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
