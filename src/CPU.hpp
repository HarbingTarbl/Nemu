#pragma once

#include <cstdint>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
#include "VMemory.hpp"
#include "OpCodes.hpp"

class CPU
{
public:
	CPU(VMemory& memory)
		:
		Memory(memory),
		A(0),
		SP(0),
		X(0),
		Y(0),
		PC(0)
	{
		HardReset();
	}

	VMemory& Memory;

	uint8_t A, SP, X, Y;
	uint16_t PC;
	uint8_t IR;

	int Addr;

	bool Asserted;

	unsigned CurrentCyle;
	const InstructionTable::InstructionPack* Instruction;

	union
	{
		struct
		{
			uint8_t CarryFlag : 1;
			uint8_t ZeroFlag : 1;
			uint8_t InterruptFlag : 1;
			uint8_t DecimalFlag : 1;

			uint8_t BreakFlag : 1;
			uint8_t __UnusedFlag : 1;
			uint8_t OverflowFlag : 1;
			uint8_t SignFlag : 1;
		};
		uint8_t Status;
	};

	void DumpRegisters()
	{
		using std::left;
		using std::setw;
		using std::hex;
		using std::setfill;

		cout 
			<< hex << setw(4) << setfill('0') << PC  - Instruction->Size << " " 
			<< Instruction->Name << " A:" << setfill('0') << setw(2) << (int)A 
			<< " X:" << setfill('0') << setw(2) << (unsigned)X << " Y:" << setfill('0') << setw(2) << (unsigned)Y << " P:" << setfill('0') << setw(2) << (unsigned)Status
			<< " SP:" << setfill('0') << setw(2) << (unsigned)SP 
			<< endl; 

	}

	void Fetch()
	{
		IR = Memory[PC];
		PC++;
		Instruction = InstructionTable::GetInstruction(IR);
	}

	void Execute()
	{
		Instruction->Pre(*this);
		DumpRegisters();
		//Something something cycle timings
		Instruction->Exec(*this);
		CurrentCyle += Instruction->Cycles;
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

	uint8_t Pop()
	{
		return Memory[++SP + 0x100];
	}

	uint8_t& operator[](int addr)
	{
		if(addr == -1) //Accumulator
			return A;

		return Memory[addr];
	}

	void Push(uint8_t value)
	{
		Memory[SP-- + 0x100] = value;
	}

	void SoftReset()
	{
		SP -= 3;
		InterruptFlag = 1;
		Memory[0x4015] = 0x00;
	}

	void HardReset()
	{
		Status = 0x24;
		A = X = Y = 0;
		SP = 0xFD;
		Asserted = false;

		Memory[0x8] = 0xF7;
		Memory[0x9] = 0xEF;
		Memory[0xa] = 0xDF;
		Memory[0xf] = 0xBF;

		Memory[0x4017] = 0x00;
		Memory[0x4015] = 0x00;
		
		for(int i = 0x4000; i <= 0x400F; i++)
			Memory[i] = 0x00;

		PC = Memory.RV;
		CurrentCyle = 0;
	}
};


