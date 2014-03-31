#pragma once

#include <array>
#include <cstdint>
#include "VMemory.hpp"
#include "CPU.hpp"

class APU
{
private:
	std::array<uint8_t, 0x20> mData;

public:
	CPU* mCPU;

	enum
	{
		IO_OAM_ADDR = 0x14,
	};

	uint8_t Read(int addr);
	uint8_t Write(int addr, uint8_t value);
};