#pragma once

#ifndef _CPU_H
#define _CPU_H

#include <cstdint>
#include <array>
#include <deque>
#include <fstream>

class VMemory;

namespace InstructionTable
{
	class InstructionPack;
};

class CPU
{
public:
	CPU();

	std::array<uint8_t, 0x800> RAM;
	VMemory* Memory;

	uint8_t A, SP, X, Y;
	uint16_t PC;
	uint8_t IR;

	std::deque<uint8_t> InterruptQueue;

	int Addr;
	int DMAAddr, DMACount;
	int AllocatedCycles;

	std::fstream logFile;

	enum
	{
		CPU_STATE_STARTUP,
		CPU_STATE_FETCHING,
		CPU_STATE_EXECUTING,
		CPU_STATE_INTERRUPT,
		CPU_STATE_DMA_START,
		CPU_STATE_DMA_EXECUTING,
		CPU_STATE_DMA_END
	};

	enum
	{
		INTERRUPT_NMI = 0,
		INTERRUPT_IRQ = 1,
		INTERRUPT_BRK = 2,
		INTERRUPT_RESET = 3
	};

	int State;
	bool Asserted;
	bool AwaitingNMI;
	int CycleOffset;

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

	void DumpRegisters();
	void Fetch();

	void Execute();

	uint8_t Read(int addr);

	uint8_t Write(int addr, uint8_t value);

	int Interrupt();
	int HandleInterrupt(int type);

	int Cycle();

	uint8_t Pop();
	void Push(uint8_t value);

	void SoftReset();

	void HardReset();
	void AllocateCycles(int nTicks);
	void Step();

	void TriggerNMI();
	void TriggerVINT();
	void ClearNMI();
};



#endif