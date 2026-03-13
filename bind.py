import ctypes as c
from os import path

c_float_ptr = c.POINTER(c.c_float)

lib = c.CDLL(path.abspath('lib.so'))


class c_bilin(c.Structure):
    _fields_ = [(s, c.c_float) for s in
                ['a_0', 'a_1', 'b_0', 'b_1',
                'x_1', 'y_1']]


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
