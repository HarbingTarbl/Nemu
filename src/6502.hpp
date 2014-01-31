#pragma once
#ifndef _6502_H_PP
#define _6502_H_PP
#include <cstdint>
#include "OpCodes.hpp"

class CPUState
{
public:
	uint8_t SP;
	uint8_t IR;
	uint16_t PC;
	uint8_t X;
	uint8_t Y;
	uint8_t A;
	union 
	{
		bool CarryFlag : 1;
		bool ZeroFlag : 1;
		bool InterruptFlag : 1;
		bool DecimalFlag : 1;
		bool BreakFlag : 1;
		bool __UnusedFlag : 1;
		bool OverflowFlag : 1;
		bool SignFlag : 1;
		uint8_t Status;
	};
	bool HasAsserted;
};


#endif
