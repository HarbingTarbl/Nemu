#pragma once
#ifndef _6502_H_PP
#define _6502_H_PP
#include <cstdint>
#include "OpCodes.hpp"

class MemoryRange
{
private:
	uint16_t m_Start;
	uint16_t m_End;

public:
	inline uint16_t Begin() { return m_Start; }
	inline uint16_t End() { return m_End; }
	inline uint16_t Size() { return End() - Begin(); }

	inline bool Contains(uint16_t addr)
	{
		return addr > Begin() && addr < End();
	}

	void Read();
	void Write();
};

class MemoryBus
{
private:
	uint16_t m_StartAddress;
	uint16_t m_EndAddress;

public:
	inline uint16_t Begin() { return m_StartAddress; }
	inline uint16_t End() { return m_EndAddress; }
	inline uint16_t Size() { return Begin() - End(); }

	void Read();
	void Write();
};

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
