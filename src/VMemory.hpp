#pragma once

#ifndef _VMEM_H
#define _VMEM_H

#include <cstdint>


class CPU;
class PPU;
class APU;
class Cartridge;

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



#endif