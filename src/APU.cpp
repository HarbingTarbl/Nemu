#include "APU.hpp"
#include "CPU.hpp"
#include "PPU.hpp"
#include "VMemory.hpp"
#include "Renderer.hpp"

uint8_t APU::Write(int addr, uint8_t value)
{
	mData[addr] = value;
	switch (addr)
	{
	case IO_OAM_ADDR:
		mCPU->Memory->mPPU->StartDMA(value); ///TODO This is fucking horrible
		break;
	case IO_CONTROLLER_ADDR:
		Render::ControllerStrobe = (int)Render::ButtonState::A;
		break;
	}
	return value;
}


uint8_t APU::Read(int addr)
{
	switch (addr)
	{
	case IO_CONTROLLER_ADDR:
		return Render::ControllerStatus() & 0x01;
	}

	return mData[addr];
}