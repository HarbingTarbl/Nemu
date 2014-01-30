#pragma once
#ifndef _6502_H_PP
#define _6502_H_PP
#include <cstdint>

template<int Size>
class MemoryPage
{
private:
	uint8_t m_Data[Size];

public:
	inline void Write(uint8_t offset, uint8_t value)
	{
		m_Data[offset] = value;
	}

	inline uint8_t Read(uint8_t offset)
	{
		return m_Data[offset];
	}

	inline uint8_t& operator[](int offset)
	{
		return m_Data[offset];
	}
};

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
};

#endif
