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
		VMemBaseAddress,
		VMemRamStart,
		VMemRomStart = 0x4020,
		VMemEndAddress = VMemBaseAddress + 0xC807,
		VRegBaseAddress,
		VRegEndAddress = VRegBaseAddress
	};

private:
	std::array<uint8_t, VRegEndAddress> mBytes;

public:
	uint8_t& operator[](int addr)
	{
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

	//Vectors!
	uint16_t &NVMIV, &RV, &IRQV;

	VMemory()
		:
		NVMIV(*reinterpret_cast<uint16_t*>(&at(0xFFFA))),
		RV(*reinterpret_cast<uint16_t*>(&at(0xFFFC))),
		IRQV(*reinterpret_cast<uint16_t*>(&at(0xFFFE)))

	{
		std::fill(mBytes.begin(), mBytes.end(), 0);
	}
};


