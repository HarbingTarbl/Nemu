#pragma once
#include <array>
#include <cstdint>

#include "CPU.hpp"	
#include "VMemory.hpp"

class VMemory;

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