#pragma once

#include "CPU.hpp"
#include "PPU.hpp"
#include "VMemory.hpp"
#include "Rom.hpp"


class NES
{
public:
	CPU mCPU;
	APU mAPU;
	PPU mPPU;
	Cartridge mCart;
	VMemory mMemory;

	NES()
	{
		mCPU.Memory = &mMemory;
		mMemory.mCPU = &mCPU;
		mMemory.mPPU = &mPPU;
		mMemory.mAPU = &mAPU;
		mMemory.mCart = &mCart;
	}


};
