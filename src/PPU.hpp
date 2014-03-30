#pragma once
#include <array>
#include <cstdint>
#include <bitset>

#include "CPU.hpp"	
#include "VMemory.hpp"
#include "Renderer.hpp"

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

	uint8_t NametableByte, AttributeTableByte, TilemapLow, TilemapHigh;
	uint8_t LastRegisterWrite;

	int TempAddr;
	int AddrIncAmount;

	int TilemapAddr;
	int BackgroundAddr, BackgroundAddrTemp;
	int NametableAddr, NametableAddrTemp;
	int SpritePatternAddr, SpritePatternAddrTemp;
	int SpriteSize; //8x8 = 64, 8x16 = 128

	uint8_t Scroll[2]; //0 -> X, 1 -> Y
	uint8_t ScrollIndex;

	uint8_t Sprite8[8];
	uint8_t SpriteLatches[8];
	uint8_t SpriteCounters[8];

	uint16_t Back16[2];
	uint16_t Back8[2];

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


	uint8_t ReadPRG(int addr)
	{
		auto r = mRegisters[addr];
		switch (addr)
		{
		case CONTROL_REG:
		case MASK_REG:
		case SCROLL_REG:
		case ADDR_REG:
			return LastRegisterWrite;
		case STATUS_REG:
			mRegisters[STATUS_REG] &= 0x7F;
			break;
		case OAM_ADDR_REG:
			break;
		case OAM_DATA_REG:
			if (CurrentFrame >= 20 && CurrentFrame < 26 && CurrentCycle < 64)
				return 0xFF;

			r = mPrimaryOAM[mRegisters[OAM_ADDR_REG]]; ///TODO, This is wrong, need to simulate the proper latching behavior.
			mRegisters[OAM_ADDR_REG] += IsVBlanking();
			return r;
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
			return value;
		case MASK_REG:
			MaskBits = value;
			return value;
		case STATUS_REG:
			return value;
		case OAM_ADDR_REG:
			break;
		case OAM_DATA_REG:
			mPrimaryOAM[mRegisters[OAM_ADDR_REG]] = value;
			mRegisters[OAM_ADDR_REG]++;
			return value;
		case SCROLL_REG:
			Scroll[ScrollIndex] = value;
			ScrollIndex = !ScrollIndex;
			return value;
		case ADDR_REG:
			Addr = (value << AddrShift) | (Addr & (0xFF00 >> AddrShift));
			AddrShift = (AddrShift + 8) % 16;
			return value;
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

	inline uint8_t ReadSecondOAM(uint8_t addr)
	{
		if (CurrentCycle < 64)
			return 0xFF;
		else
			return mSecondaryOAM[addr];
	}

	inline uint8_t WriteSecondOAM(uint8_t addr, uint8_t value)
	{
		mSecondaryOAM[addr] = value;
		return value;
	}


	void SpriteEvaluation()
	{
		if (CurrentCycle < 64)
		{
			//zzzz
		}
		else if (CurrentCycle < 256)
		{
			int n = 0;


		}
	}

	void Cycle()
	{
		if (CurrentLine == 0)
		{
			Render::BeginFrame();
		}
		else if (CurrentLine >= 241)
		{
			Render::EndFrame();
			CurrentLine = 0;
			CurrentCycle = 0;
			return;
		}
		else
		{

		}

		if (CurrentCycle == 0)
		{
			CurrentLine++;

			mRegisters[STATUS_REG] =  0;
			NametableAddrTemp = NametableAddr;
			SpritePatternAddrTemp = SpritePatternAddr;
			BackgroundAddrTemp = BackgroundAddr;
			Render::BeginScanline(0); //Should use actual scanline timing

			//Spin Cycle
		}
		else if (CurrentCycle <= 256)
		{
			static int attribMask, attribIndex;
			uint16_t paletteAddr;
			int cycleOffset = (CurrentCycle - 1) % 8;
			if (cycleOffset == 0)
			{
				NametableAddrTemp += 8;
				NametableByte = ReadCHR(NametableAddrTemp);
				AttributeTableByte = ReadCHR(NametableAddrTemp + 0x3C0);
				SpritePatternAddrTemp = SpritePatternAddr + 16 * NametableByte;
				TilemapLow = ReadCHR(SpritePatternAddrTemp);
				TilemapHigh = ReadCHR(SpritePatternAddrTemp + 8);
				//2bits of attribute data per 16 pixels or 4 tiles
				//2 pixels per tile
				//xAttrb = CurrentCycle / 16
				//yAttrb = CurrentScanline / 16
				int attribX = (CurrentCycle / 16) % 2;
				int attribY = (CurrentLine / 16) % 2;
				attribIndex = (attribY * 2 + attribX);
				attribMask = 0x03 << attribIndex;
			}
			else
			{
				
			}

			

			Render::PixelOut->Color =
				PatternTile::Get(TilemapHigh, TilemapLow, cycleOffset)
				| (((AttributeTableByte & attribMask) >> attribIndex) << 2);
			Render::PixelOut++;
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
			Render::EndScanline();
			CurrentLine++;
			CurrentCycle = -1;
		}
		CurrentCycle++;
	}

	void Frame()
	{
		CurrentLine = 0;
		TempAddr = Addr;
	}

	void Scanline()
	{
		CurrentCycle = 0;
		if (CurrentLine == -1 || CurrentLine == 261)
		{
			//Fetch Next, VBLANK Scanline
			Render::BeginFrame();
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
			Render::EndFrame();
		}
		CurrentLine++;
	}

	void VBlank()
	{
		SetVBlanking();
	}

};