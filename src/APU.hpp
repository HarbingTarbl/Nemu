#pragma once

#include <array>
#include <cstdint>
#include "VMemory.hpp"

class APU
{
private:
	std::array<uint8_t, 0x20> mData;

public:
	uint8_t Read(int addr)
	{
		return mData[addr];
	}

	uint8_t Write(int addr, uint8_t value)
	{
		mData[addr] = value;
		return value;
	}

};