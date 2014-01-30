#pragma once
#ifndef _6502_H_PP
#define _6502_H_PP
#include <cstdint>

class NesCPU
{
private:


public:
	uint8_t 
		AccumReg,
		XIndexReg,
		YIndexReg,
		SPReg;

	union
	{
		uint8_t Value;
		struct
		{
			uint8_t Carry : 1;
			uint8_t Zero : 1;
			uint8_t Decimal : 1;
			uint8_t InterruptDisable : 1;
			uint8_t Breakpoint : 1;
			uint8_t : 1;
			uint8_t Overflow : 1;
			uint8_t Sign : 1;
		} Flags;
	} StatusReg;

	uint16_t PCReg;

	uint8_t Stack[0xFF];
	uint8_t 
};

#endif