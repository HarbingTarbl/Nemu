#include "APU.hpp"
#include "CPU.hpp"
#include "PPU.hpp"
#include "VMemory.hpp"

uint8_t APU::Write(int addr, uint8_t value)
{
	mData[addr] = value;
	switch (addr)
	{
	case IO_OAM_ADDR:
		mCPU->Memory->mPPU->StartDMA(value); ///TODO This is fucking horrible
		break;
	}
	return value;
}


uint8_t APU::Read(int addr)
{
	return mData[addr];
}