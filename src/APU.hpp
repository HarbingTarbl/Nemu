#pragma once

#ifndef _APU_H
#define _APU_H

#include <array>
#include <cstdint>

class CPU;

class APU
{
private:
	std::array<uint8_t, 0x20> mData;

public:
	CPU* mCPU;

	unsigned CurrentCycle;


	enum
	{
		SOUND_PULSE_0_0 = 0,
		SOUND_PULSE_0_1,
		SOUND_PULSE_0_2,
		SOUND_PULSE_0_3,

		SOUND_PULSE_1_0 = 0x04,
		SOUND_PULSE_1_1,
		SOUND_PULSE_1_2, 
		SOUND_PULSE_1_3,

		SOUND_TRIANGLE_0 = 0x08,
		SOUND_TRIANGLE_1,
		SOUND_TRIANGLE_2,
		SOUND_TRIANGLE_3,

		SOUND_NOISE_0 = 0x0C,
		SOUND_NOISE_1,
		SOUND_NOISE_2,
		SOUND_NOISE_3,

		SOUND_DMC_0 = 0x10,
		SOUND_DMC_1,
		SOUND_DMC_2,
		SOUND_DMC_3,

		IO_OAM_ADDR = 0x14,
		SOUND_ENABLE_LENGTH = 0x015,
		IO_CONTROLLER_ADDR = 0x16,
		SOUND_FRAME_COUNTER = 0x17,
	};

	APU();

	uint8_t Read(int addr);
	uint8_t Write(int addr, uint8_t value);
};


#endif