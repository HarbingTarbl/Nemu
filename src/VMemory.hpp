#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <functional>

#include "CPU.hpp"
#include "PPU.hpp"
#include "APU.hpp"
#include "Cartridge.hpp"

class CPU;
class PPU;
class APU;
class mCart;

class VMemory
{
public:
	PPU* mPPU;
	CPU* mCPU;
	APU* mAPU;
	Cartridge* mCart;

	inline int CartAddr(int addr)
	{
		return addr;
	}

	inline int CPUAddr(int addr)
	{
		return addr & 0x7FF;
	}
	
	inline int APUAddr(int addr)
	{
		return addr & 0x1F;
	}
	
	inline int PPUAddr(int addr)
	{
		return addr & 0x7;
	}

	inline int CHRAddr(int addr)
	{
		return addr & 0x3FFF;
	}

	uint8_t ReadPRG(int addr)
	{
		if(addr == -1)
			return mCPU->Read(addr);

		if (addr > std::numeric_limits<uint16_t>::max())
			addr &= std::numeric_limits<uint16_t>::max();

		if (addr >= 0x4020) //Signal Cartridge?
			return mCart->ReadPRG(CartAddr(addr));

		if (addr >= 0x4000) //Signal APU/IO?
			return mAPU->Read(APUAddr(addr)); //(808, 828]

		if (addr >= 0x2000) //Signal PPU?
			return mPPU->ReadPRG(PPUAddr(addr)); //(800, 808]

		return mCPU->Read(CPUAddr(addr)); //(0, 800]
	}

	uint8_t WritePRG(int addr, uint8_t value)
	{
		if(addr == -1)
			return mCPU->Write(addr, value);

		if (addr > std::numeric_limits<uint16_t>::max())
			addr &= std::numeric_limits<uint16_t>::max();

		if (addr >= 0x4020) //Signal Cartridge?
			return mCart->WritePRG(CartAddr(addr), value);

		if (addr >= 0x4000) //Signal APU/IO?
			return mAPU->Write(APUAddr(addr), value); //(808, 828]

		if (addr >= 0x2000) //Signal PPU?
			return mPPU->WritePRG(PPUAddr(addr), value); //(800, 808]

		return mCPU->Write(CPUAddr(addr), value); //(0, 800]

	}

	uint8_t WriteCHR(int addr, uint8_t value)
	{
		addr = CHRAddr(addr);
		if ((addr & 0x2000) >> 15)
			return mPPU->WriteCHR(addr, value);
		else
			return mCart->WriteCHR(addr, value);
	}

	uint8_t ReadCHR(int addr)
	{
		addr = CHRAddr(addr);
		if ((addr & 0x2000) >> 15)
			return mPPU->ReadCHR(addr);
		else
			return mCart->ReadCHR(addr);
	}


	uint16_t GetNMIV()
	{
		return (ReadPRG(0xFFFA) << 8) | ReadPRG(0xFFFB);
	}

	uint16_t GetRV()
	{
		return (ReadPRG(0xFFFC) << 8) | ReadPRG(0xFFFD);
	}

	uint16_t GetIRQ()
	{
		return (ReadPRG(0xFFFE) << 8) | ReadPRG(0xFFFF);
	}

	void SetRV(uint16_t value)
	{
		WritePRG(0xFFFC, (value & 0xFF00) >> 8);
		WritePRG(0xFFFD, (value & 0x00FF));
	}


	//Vectors!

	VMemory()
	{
	}
};

