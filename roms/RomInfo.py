__author__ = 'HarbingTarbl'


class RomInfo:
    def __init__(self, file):
        import struct
        import io
        buffer = bytearray(16)
        with io.FileIO(file) as f:
            f.readinto(buffer)

        data = struct.unpack("4sBBBBBBBBBB2s", buffer)
        self.is_rom = data[0] == b"NES\x1a"
        self.prg_pages = data[1]
        self.chr_pages = data[2]
        self.flags6 = data[3]
        self.flags7 = data[4]
        self.mapper = ((self.flags6 & 0xF0) >> 4) | (self.flags7 & 0xF0)
        self.variant = data[5]
        self.rom_size = data[6]
        self.ram_size = data[7]
        self.vram_size = data[8]
        self.tv_code = data[9]
        self.vs_hardware = data[10]


if __name__ == "__main__":
    import sys
    print(vars(RomInfo(sys.argv[1])))

