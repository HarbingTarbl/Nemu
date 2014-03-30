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

	int CartAddr(int addr);

	int CPUAddr(int addr);
	
	int APUAddr(int addr);
	
	int PPUAddr(int addr);

	int CHRAddr(int addr);

	uint8_t ReadPRG(int addr);

	uint8_t WritePRG(int addr, uint8_t value);

	uint8_t WriteCHR(int addr, uint8_t value);

	uint8_t ReadCHR(int addr);


	uint16_t GetNMIV();
	uint16_t GetRV();
	uint16_t GetIRQ();
	void SetRV(uint16_t value);
	VMemory();
};

