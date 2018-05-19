from struct import unpack
from collections import namedtuple
import zlib


header = namedtuple('Header', ['Magic', 'Key', 'FileTableOffset', 'Seed', 'FilesCount', 'Version'])

with open('hpal.grf', 'rb') as f:
    header_data = f.read(46)

    header = header._make(unpack('<16s14sIIII', header_data))
    print(header)

    f.seek(header.FileTableOffset, 1)
    pack_size, real_size = unpack('<LL', f.read(0x08))
    print(pack_size, real_size)

    fileTable = zlib.decompress(f.read(pack_size))
    print(fileTable)
