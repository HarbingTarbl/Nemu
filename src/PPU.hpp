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

	unsigned CurrentCyle;
	int CurrentLine;
	unsigned CurrentFrame;

	uint16_t Addr, TempAddr, FineX;
	uint16_t Bitmap1, Bitmap2;
	uint8_t Palette1, Palette2;

	uint8_t Read(int addr)
	{
		return mRegisters[addr];
	}

	uint8_t Write(int addr, uint8_t value)
	{
		mRegisters[addr] = value;
		return value;
	}

	void Frame()
	{

	}

	void Scanline()
	{

	}

	void VBlank()
	{

	}

	void Cycle()
	{



	}

};