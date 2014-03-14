#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>

class VMemory
{
public:
	enum AddrSections
	{
		Accum = -6,
		StackP,
		PCLow,
		PCHigh,
		XReg,
		YReg,
		VMemBaseAddress,
		VMemRamStart,
		VMemRomStart = 0x4020,
		VMemEndAddress = VMemBaseAddress + 0xC807,
		VRegBaseAddress,
		VRegEndAddress = VRegBaseAddress - Accum
	};

private:
	std::array<uint8_t, VRegEndAddress> mBytes;
	std::fstream debugFile;


public:
	uint8_t& operator[](int addr)
	{
		if(debugFile.is_open())
			debugFile << "PC:" << std::hex << PC << " Mem:" << std::hex << std::setw(4) << std::setfill('0') << addr << std::endl;

		if (addr < 0)
			return mBytes[addr + VRegEndAddress];

		if (addr > std::numeric_limits<uint16_t>::max())
			addr &= std::numeric_limits<uint16_t>::max();

		if (addr >= 0x4020) //Signal Cartridge?
			return mBytes[addr - 0x37F8];

		if (addr >= 0x4000) //Signal APU/IO?
			return mBytes[addr - 0x37F8]; //(808, 828]

		if (addr >= 0x2000) //Signal PPU?
			return mBytes[(addr & 0x2007) - 0x1800]; //(800, 808]

		return mBytes[addr & 0x07FF]; //(0, 800]
	}

	inline uint8_t& at(int addr)
	{
		return (*this)[addr];
	}

	//Registers
	uint8_t &A, &SP, &X, &Y;
	uint16_t &PC;

	//Vectors!
	uint16_t &NVMIV, &RV, &IRQV;

	VMemory()
		:
		A(at(Accum)),
		SP(at(StackP)),
		X(at(XReg)),
		Y(at(YReg)),
		PC(*reinterpret_cast<uint16_t*>(&at(PCLow))),
		NVMIV(*reinterpret_cast<uint16_t*>(&at(0xFFFA))),
		RV(*reinterpret_cast<uint16_t*>(&at(0xFFFC))),
		IRQV(*reinterpret_cast<uint16_t*>(&at(0xFFFE)))

	{
		std::fill(mBytes.begin(), mBytes.end(), 0);
		debugFile.open("memory.log", std::fstream::trunc | std::fstream::out);
	}
};


