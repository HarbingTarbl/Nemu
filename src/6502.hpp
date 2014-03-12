#pragma once

#include <cstdint>
#include "VMemory.hpp"
#include "OpCodes.hpp"

class Flags
{
public:
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
};

class CPU
{
public:
	CPU(VMemory& memory)
		:
		Memory(memory),
		A(memory.A),
		SP(memory.SP),
		X(memory.X),
		Y(memory.Y),
		PC(memory.PC)
	{

	}

	VMemory& Memory;

	uint8_t &A, &SP;
	uint16_t &X, &Y, &PC;

	uint8_t IR;
	int Addr;

	unsigned CurrentCyle;
	const InstructionTable::InstructionPack* Instruction;

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

	void Fetch()
	{
		IR = Memory[PC];
		PC++;
		Instruction = InstructionTable::GetInstruction(IR);
	}

	void Execute()
	{
		Instruction->Pre(*this);
		//Something something cycle timings
		Instruction->Exec(*this);
	}

	void Interrupt()
	{

	}

	void Cycle()
	{
		Fetch();
		Execute();
		Interrupt();
	}

};


