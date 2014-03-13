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
		A(memory.A),
		SP(memory.SP),
		X(memory.X),
		Y(memory.Y),
		PC(memory.PC)
	{
		HardReset();
	}

	VMemory& Memory;

	uint8_t &A, &SP, &X, &Y;
	uint16_t &PC;

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

	void DumpRegisters()
	{
		using std::left;
		using std::setw;
		cout 
			<< "OP = " << std::hex << (unsigned)IR << " : " << Tables::namesTable[IR] << endl
			<< "A = " << setw(10) << left << (int)A 
			<< "X = " << setw(10) << left << (unsigned)X
			<< "Y = " << setw(10) << left << (unsigned)Y
			<< endl
			<< "SP = " <<setw(10) << left << (unsigned)SP
			<< "PC = " << setw(10) << left << (unsigned)PC
			<< "Effective = " << setw(10) << left << Addr
			<< endl << endl;
	}

	void Fetch()
	{
		IR = Memory[PC];
		cout << "PreFetch : " << endl;
		DumpRegisters();
		PC++;
		Instruction = InstructionTable::GetInstruction(IR);
	}

	void Execute()
	{
		Instruction->Pre(*this);
		cout << "Post Addressing : " << endl;
		DumpRegisters();
		//Something something cycle timings
		Instruction->Exec(*this);
		cout << "Post Execution : " << endl;
		DumpRegisters();
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
		Status = 0x34;
		A = X = Y = 0;
		SP = 0xFD;

		Memory[0x8] = 0xF7;
		Memory[0x9] = 0xEF;
		Memory[0xa] = 0xDF;
		Memory[0xf] = 0xBF;

		Memory[0x4017] = 0x00;
		Memory[0x4015] = 0x00;
		
		for(int i = 0x4000; i <= 0x400F; i++)
			Memory[i] = 0x00;

		PC = Memory.RV;
	}
};


