#pragma once
#include <array>
#include <cstdint>

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
		uint8_t r =  (Tile[byte] & shift) >> x;
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

	unsigned CurrentCyle;
	int CurrentLine;
	unsigned CurrentFrame;

	uint16_t Addr, TempAddr, FineX;
	uint16_t Bitmap1, Bitmap2;
	uint8_t Palette1, Palette2;

	uint8_t LoopyX, LoopyT, LoopV;
	bool LoopyAtX;



	uint8_t Read(int addr)
	{
		auto r = mRegisters[addr];
		switch(addr)
		{
		case CONTROL_REG:
		case MASK_REG:
			break;
		case STATUS_REG:
			mRegisters[STATUS_REG] &= 0x7F;
			break;
		case OAM_ADDR_REG:
			break;
		case OAM_DATA_REG:
			if(!IsVBlanking())
				mRegisters[OAM_ADDR_REG]++;
		case SCROLL_REG:
		case ADDR_REG:
		case DATA_REG:
			break;
		}

		return r;
	}

	uint8_t Write(int addr, uint8_t value)
	{
		switch(addr)
		{
		case CONTROL_REG:
		case MASK_REG:
		case STATUS_REG:
			return value;
		case OAM_ADDR_REG:
			break;
		case OAM_DATA_REG:
			mRegisters[OAM_ADDR_REG]++;
		case SCROLL_REG:
			if(LoopyAtX)
			{
				LoopyX = 0x07 & value | LoopyX & 0xF8;
				LoopyT = 0xF8 & value | LoopyT & 0x07;
			}
			else
			{

			}
			LoopyAtX = !LoopyAtX;
			break;
		case ADDR_REG:
		case DATA_REG:
			break;
		}

		mRegisters[addr] = value;
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

	void Frame()
	{

	}

	void Scanline()
	{

	}

	void VBlank()
	{
		SetVBlanking();
	}

	void Cycle()
	{
		if(CurrentCyle >= 257 && CurrentCyle <= 320)
			mRegisters[OAM_ADDR_REG] = 0;

	}

};