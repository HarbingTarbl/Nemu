#pragma once
#include <algorithm>
#include <array>
#include <limits>
#include <cstdint>

namespace Bus
{
	class VMemory
	{
	public:
		enum AddrSections
		{
			Accum = -8,
			StackP,
			PCLow,
			PCHigh,
			XLow,
			XHigh,
			YLow,
			YHigh,
			VMemBaseAddress = 0,
			VMemEndAddress = VMemBaseAddress + 0xC807,
			VRegBaseAddress,
			VRegEndAddress = VRegBaseAddress + 8
		};

	private:
		std::array<uint8_t, 0xC808 + 8> mBytes;

	public:
		inline uint8_t& operator[](int addr)
		{
			if(addr < 0)
				return mBytes[addr + VRegEndAddress];

			if(addr > std::numeric_limits<uint16_t>::max())
				addr &= std::numeric_limits<uint16_t>::max();
			
			if(addr >= 0x4020) //Signal Cartridge?
				return mBytes[addr - 0x37F8];

			if(addr >= 0x4000) //Signal APU/IO?
				return mBytes[addr - 0x37F8]; //(808, 828]

			if(addr >= 0x2000) //Signal PPU?
				return mBytes[(addr & 0x2007) - 0x1800]; //(800, 808]

			return mBytes[addr & 0x07FF]; //(0, 800]
		}

		inline uint8_t& at(int addr)
		{
			return (*this)[addr];
		}

		//Registers
		uint8_t &A, &SP;
		uint16_t &X, &Y, &PC;
	
		//Vectors!
		uint16_t &NVMIV, &RV, &IRQV;

		VMemory()
			: 
			A(at(Accum)), 
			SP(at(StackP)),
			PC(*reinterpret_cast<uint16_t*>(&at(PCLow))),
			X(*reinterpret_cast<uint16_t*>(&at(XLow))),
			Y(*reinterpret_cast<uint16_t*>(&at(YLow))),
			NVMIV(*reinterpret_cast<uint16_t*>(&at(0xFFFA))),
			RV(*reinterpret_cast<uint16_t*>(&at(0xFFFC))),
			IRQV(*reinterpret_cast<uint16_t*>(&at(0xFFFE)))

		{
			std::fill(mBytes.begin(), mBytes.end(), 0);
		}
	};

};