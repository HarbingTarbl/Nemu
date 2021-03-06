#include "APU.hpp"
#include "CPU.hpp"
#include "PPU.hpp"
#include "VMemory.hpp"
#include "Renderer.hpp"


APU::APU()
{

}

uint8_t APU::Write(int addr, uint8_t value)
{
	mData[addr] = value;
	switch (addr)
	{
	case SOUND_PULSE_0_0:
		break;
	case SOUND_PULSE_0_1:
		break;
	case SOUND_PULSE_0_2:
		break;
	case SOUND_PULSE_0_3:
		break;
	case SOUND_PULSE_1_0:
		break;
	case SOUND_PULSE_1_1:
		break;
	case SOUND_PULSE_1_2:
		break;
	case SOUND_PULSE_1_3:
		break;
	case SOUND_TRIANGLE_0:
		break;
	case SOUND_TRIANGLE_1:
		break;
	case SOUND_TRIANGLE_2:
		break;
	case SOUND_TRIANGLE_3:
		break;
	case SOUND_NOISE_0:
		break;
	case SOUND_NOISE_1:
		break;
	case SOUND_NOISE_2:
		break;
	case SOUND_NOISE_3:
		break;
	case SOUND_DMC_0:
		break;
	case SOUND_DMC_1:
		break;
	case SOUND_DMC_2:
		break;
	case SOUND_DMC_3:
		break;
	case IO_OAM_ADDR:
		mCPU->Memory->mPPU->StartDMA(value); ///TODO This is fucking horrible
		break;
	case SOUND_ENABLE_LENGTH:
		break;
	case IO_CONTROLLER_ADDR:
		Render::ControllerStrobe = (int)Render::ButtonState::A;
		break;
	case SOUND_FRAME_COUNTER:
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