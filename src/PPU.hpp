#pragma once
#include <array>
#include <cstdint>
#include <bitset>

#include "CPU.hpp"	
#include "VMemory.hpp"
#include "Renderer.hpp"

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

	static inline int Get(uint8_t upper, uint8_t lower, uint8_t x)
	{
		x = 7 - x;
		return (((upper >> x) & 0x01) << 1) | ((lower >> x) & 0x01);
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
	uint8_t mTopScanline;
	uint8_t mTileIndex;
	uint8_t mAttributeFlags;
	uint8_t mLeftScanline;

	inline bool PaletteHigh()
	{
		return (mAttributeFlags & 0x02) >> 1;
	}

	inline bool PaletteLow()
	{
		return (mAttributeFlags & 0x01) >> 0;
	}

	inline bool ObjectPriority()
	{
		return (mAttributeFlags & 0x20) >> 5;
	}

	inline bool BitReversal()
	{
		return (mAttributeFlags & 0x40) >> 6;
	}

	inline bool Revert()
	{
		return (mAttributeFlags & 0x80) >> 7;
	}

	static inline uint8_t TopScanline(unsigned block)
	{
		return reinterpret_cast<OAMBlock*>(&block)->mTopScanline;
	}

	static inline uint8_t TileIndex(unsigned block)
	{
		return reinterpret_cast<OAMBlock*>(&block)->mTileIndex;
	}

	static inline uint8_t Flag(unsigned block, bool(OAMBlock::*func)())
	{
		return (reinterpret_cast<OAMBlock*>(&block)->*func)();
	}
	
	static inline uint8_t LeftScanline(unsigned block)
	{
		return reinterpret_cast<OAMBlock*>(&block)->mLeftScanline;
	}
};

class PPU
{
private:
	//Pattern Tables are on Cartridge
	//OAM Data is in RAM
	std::array<uint8_t, 0x08> mRegisters;
	std::array<uint8_t, 0xFF> mPrimaryOAM;
	std::array<uint8_t, 0x20> mSecondaryOAM;
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

	PPU()
	{
		CurrentCycle = 0;
		CurrentLine = 0;
	}

	unsigned CurrentCycle;
	int CurrentLine;
	unsigned CurrentFrame;

	uint8_t LastRegisterWrite;


	int AddrIncAmount;
	int State;

	int AllocatedCycles;


	int BackgroundAddr, BackgroundAddrTemp;
	int NametableAddr, NametableAddrTemp;
	int SpritePatternAddr, SpritePatternAddrTemp;
	int SpriteSize; //8x8 = 64, 8x16 = 128

	uint8_t Scroll[2]; //0 -> X, 1 -> Y
	uint8_t ScrollIndex;

	unsigned TotalCycles;



	uint16_t Addr;
	uint8_t AddrShift;

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

	enum
	{
		PPU_STATE_STARTUP,
		PPU_STATE_VINT,
		PPU_STATE_SCAN_PRE,
		PPU_STATE_SCAN_VISIBLE,
		PPU_STATE_SCAN_POST,
	};


	void AllocateCycles(int nTicks);

	uint8_t ReadPRG(int addr);

	uint8_t WritePRG(int addr, uint8_t value);

	uint8_t ReadCHR(int addr);

	uint8_t WriteCHR(int addr, uint8_t value);

	void StartDMA(int addr);

	void ClearVBlank();

	int IsVBlanking();

	void SetVBlanking();

	uint8_t ReadSecondOAM(uint8_t addr);

	uint8_t WriteSecondOAM(uint8_t addr, uint8_t value);

	void SpriteEvaluation();

	void Step();

	void Reset();

	void RenderPixel(int color);

	void Cycle();

	void Frame();

	void Scanline();

	void VBlank();

	void BackgroundScanline();
	void SpriteScanline();

};