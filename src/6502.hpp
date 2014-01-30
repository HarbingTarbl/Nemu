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
		StatusReg,
		SPReg;

	uint16_t 
		PCReg;

	uint8_t StackSpace;

};

#endif