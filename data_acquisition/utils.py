import struct

def to_float(b):
    return struct.unpack('f', b)[0]

def to_int(b):
    return struct.unpack('i', b)[0]