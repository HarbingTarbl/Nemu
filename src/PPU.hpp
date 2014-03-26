#pragma once
#include <array>
#include <cstdint>
#include <bitset>

#include "CPU.hpp"	
#include "VMemory.hpp"

class VMemory;

class NameTable
{
public:


};


class PatternTile
{
public:
	std::array<uint8_t, 16> Tile;

public:
	inline uint8_t PaletteIndex(uint8_t x, uint8_t y) const
	{
		uint8_t byte = y + x / 8;
		x %= 8;
		x = (7 - x);
		uint8_t shift = 1 << x;
		uint8_t r = (Tile[byte] & shift) >> x;
		r |= ((Tile[byte + 8] & shift) >> x) << 1;
		return r;
	}
};


class PaletteTable
{
private:

public:
	std::array<uint8_t, 0x2000> Patterns;

	inline const PatternTile& Get(uint16_t addr)
	{
		return *(reinterpret_cast<PatternTile*>(Patterns.data() + addr));
	}

};

class AttributeEntry
{
private:
	uint8_t value;

public:
	inline uint8_t NW()
	{
		return (value & 0xC0) >> 6;
	}

	inline uint8_t NE()
	{
		return (value & 0x30) >> 4;
	}

	inline uint8_t SW()
	{

		return (value & 0x06) >> 2;
	}

	inline uint8_t SE()
	{
		return (value & 0x3);
	}
};

class AttributeTable
{
public:
	std::array<AttributeEntry, 64> Attributes;

	inline AttributeEntry& Get(int x, int y)
	{
		return Attributes[y * 8 + x];
	}
};

class OAMBlock
{
public:
	uint8_t TopScanline;
	uint8_t TileIndex;
	uint8_t AttributeFlags;
	uint8_t LeftScanline;


	inline bool PaletteHigh()
	{
		return (AttributeFlags & 0x02) >> 1;
	}

	inline bool PaletteLow()
	{
		return (AttributeFlags & 0x01) >> 0;
	}

	inline bool ObjectPriority()
	{
		return (AttributeFlags & 0x20) >> 5;
	}

	inline bool BitReversal()
	{
		return (AttributeFlags & 0x40) >> 6;
	}

	inline bool Revert()
	{
		return (AttributeFlags & 0x80) >> 7;
	}
};

class PPU
{
private:
	//Pattern Tables are on Cartridge
	//OAM Data is in RAM
	std::array<uint8_t, 0x08> mRegisters;
	std::array<uint8_t, 0xFF> mOAM;
	std::array<uint8_t, 0x4000> mVRAM;

public:
	VMemory* Memory;

	enum
	{
		CONTROL_REG,
		MASK_REG,
		STATUS_REG,
		OAM_ADDR_REG,
		OAM_DATA_REG,
		SCROLL_REG,
		ADDR_REG,
		DATA_REG
	};

	unsigned CurrentCycle;
	int CurrentLine;
	unsigned CurrentFrame;


	uint8_t NametableByte, AttributeTableByte, TilemapLow, TilemapHigh;
	uint8_t LastRegisterWrite;

	int TempAddr;
	int AddrIncAmount;

	int BackgroundAddr;
	int NametableAddr;
	int SpritePatternAddr;
	int SpriteSize; //8x8 = 64, 8x16 = 128

	bool GenerateNMI;

	std::bitset<8> MaskBits;
	enum
	{
		MASK_COLOR,
		MASK_LEFT_BACKGROUND,
		MASK_LEFT_SPRITES,
		MASK_BACKGROUND,
		MASK_SPRITES,
		MASK_RED,
		MASK_GREEN,
		MASK_BLUE
	};


	uint8_t ReadPRG(int addr)
	{
		auto r = mRegisters[addr];
		switch (addr)
		{
		case CONTROL_REG:
		case MASK_REG:
			return LastRegisterWrite;
		case STATUS_REG:
			mRegisters[STATUS_REG] &= 0x7F;
			break;
		case OAM_ADDR_REG:
			break;
		case OAM_DATA_REG:
			if (!IsVBlanking())
				mRegisters[OAM_ADDR_REG]++;
		case SCROLL_REG:
		case ADDR_REG:
		case DATA_REG:
			break;
		}

		return r;
	}

	uint8_t WritePRG(int addr, uint8_t value)
	{
		LastRegisterWrite = value;
		switch (addr)
		{
		case CONTROL_REG:
			NametableAddr = ((value & 0x03) << 10) | 0x2000;
			AddrIncAmount = 1 << ((value & 0x04) << 3);
			SpritePatternAddr = ((value & 0x08) << 9);
			BackgroundAddr = ((value & 0x10) << 8);
			SpriteSize = 64 + ((value & 0x20) << 1);
			GenerateNMI = (value & 0x80) >> 7;
			return;
		case MASK_REG:
			MaskBits = value;
			return;
		case STATUS_REG:
			return value;
		case OAM_ADDR_REG:
			break;
		case OAM_DATA_REG:
			mOAM[mRegisters[OAM_ADDR_REG]] = value;
			mRegisters[OAM_ADDR_REG]++;
			return;
		case SCROLL_REG:
			break;
		case ADDR_REG:
		case DATA_REG:
			break;
		}

		mRegisters[addr] = value;
		return value;
	}

	uint8_t ReadCHR(int addr)
	{
		return mVRAM[addr & 0x1FFF];
	}

	uint8_t WriteCHR(int addr, uint8_t value)
	{
		mVRAM[addr & 0x1FFF] = value;
		return value;
	}

	inline void ClearVBlank()
	{
		mRegisters[STATUS_REG] &= 0x7F;
	}

	inline int IsVBlanking()
	{
		return (mRegisters[STATUS_REG] & 0x80) >> 7;
	}

	inline void SetVBlanking()
	{
		mRegisters[STATUS_REG] |= 0x80;
	}

	void Cycle()
	{
		if (CurrentCycle == 0)
		{
			//Spin Cycle
		}
		else if (CurrentCycle < 256)
		{
			//Render Cycles
		}
		else if (CurrentCycle < 320)
		{
			//Garbage cycles
		}
		else if (CurrentCycle < 336)
		{
			//Nametable
		}
		else if (CurrentCycle < 340)
		{
			//Next scanline fetch?
		}
		CurrentCycle++;
	}

	void Frame()
	{
		CurrentLine = 0;
	}

	void Scanline()
	{
		CurrentCycle = 0;
		if (CurrentLine == -1 || CurrentLine == 261)
		{
			//Fetch Next, VBLANK Scanline
		}
		else if (CurrentLine < 239)
		{
			//Visible Scanlines
		}
		else if (CurrentLine == 240)
		{
			//Post render scanline
		}
		else if (CurrentLine < 260)
		{
			//Spinlines
		}
		else
		{
			CurrentLine = -2;
		}
		CurrentLine++;
	}

	void VBlank()
	{
		SetVBlanking();
	}

};