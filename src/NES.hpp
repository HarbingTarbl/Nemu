#pragma once

#ifndef _NES_H
#define _NES_H

#include "CPU.hpp"
#include "PPU.hpp"
#include "APU.hpp"
#include "VMemory.hpp"
#include "Cartridge.hpp"
#include "Rom.hpp"


class NES
{
public:
	CPU mCPU;
	APU mAPU;
	PPU mPPU;
	Cartridge mCart;
	VMemory mMemory;

	static NES* Instance;


	NES();
};




#endif