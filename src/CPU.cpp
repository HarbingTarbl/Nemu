#include "CPU.hpp"
#include <iostream>
#include <iomanip>
#include "VMemory.hpp"
#include "OpCodes.hpp"


CPU::CPU()
	:
	A(0),
	SP(0),
	X(0),
	Y(0),
	PC(0),
	State(CPU_STATE_STARTUP)
{

}

void CPU::DumpRegisters()
{
	using std::cout;
	using std::endl;
	using std::left;
	using std::setw;
	using std::hex;
	using std::setfill;

	/*cout 
		<< hex << setw(4) << setfill('0') << PC  - Instruction->Size << " " 
		<< Instruction->Name << " A:" << setfill('0') << setw(2) << (int)A 
		<< " X:" << setfill('0') << setw(2) << (unsigned)X << " Y:" << setfill('0') << setw(2) << (unsigned)Y << " P:" << setfill('0') << setw(2) << (unsigned)Status
		<< " SP:" << setfill('0') << setw(2) << (unsigned)SP 
		<< endl; */

}

void CPU::Fetch()
{
	IR = Memory->ReadPRG(PC);
	if (AwaitingNMI)
		ClearNMI();
	PC++;
	Instruction = InstructionTable::GetInstruction(IR);
}

void CPU::Execute()
{
	Instruction->Pre(*this);
	DumpRegisters();
	//Something something cycle timings
	Instruction->Exec(*this);
	CurrentCyle += Instruction->Cycles;
}

uint8_t CPU::Read(int addr)
{
	if(addr == -1)
		return A;
	else
		return RAM[addr];
}

uint8_t CPU::Write(int addr, uint8_t value)
{
	if(addr == -1)
		A = value;
	else
		RAM[addr] = value;
	return value;
}

void CPU::Interrupt()
{
	State = CPU_STATE_FETCHING;
}

void CPU::Cycle()
{
	switch (State)
	{
	case CPU_STATE_STARTUP:
		State = CPU_STATE_FETCHING;
	case CPU_STATE_FETCHING:
		if (AllocatedCycles > 0)
		{
			State = CPU_STATE_EXECUTING;
			Fetch();
		}
		break;
	case CPU_STATE_EXECUTING:
		if (AllocatedCycles >= Instruction->Cycles)
		{
			State = CPU_STATE_FETCHING;
			Execute();
			AllocatedCycles -= Instruction->Cycles;
		}
		break;
	case CPU_STATE_INTERRUPT:
		Interrupt();
		break;
	case CPU_STATE_DMA_START:
		DMAAddr = Memory->ReadPRG(0x4014) + 0x100;
		State = CPU_STATE_DMA_START;
		AllocatedCycles--;
		DMACount = 0;
		break;
	case CPU_STATE_DMA_EXECUTING:
		if (AllocatedCycles >= 4)
		{
			AllocatedCycles -= 4;
			DMACount++;
			Memory->WriteCHR(0x2004, Memory->ReadPRG(DMAAddr++));
			if (DMACount == 256)
			{
				State = CPU_STATE_FETCHING;
			}
		}
		break;
	case CPU_STATE_DMA_END:
		break;
	}
}

void CPU::Step()
{
	while (AllocatedCycles > 0)
		Cycle();
}


uint8_t CPU::Pop()
{
	return Memory->ReadPRG(++SP + 0x100);
}

void CPU::Push(uint8_t value)
{
	Memory->WritePRG(SP-- + 0x100, value);
}

void CPU::SoftReset()
{
	SP -= 3;
	InterruptFlag = 1;
	Memory->WritePRG(0x4015, 0x00);
}

void CPU::HardReset()
{
	Status = 0x24;
	A = X = Y = 0;
	SP = 0xFD;
	Asserted = false;
	AwaitingNMI = false;
	State = CPU_STATE_STARTUP;
	AllocatedCycles = 1;

	Memory->WritePRG(0x8, 0xF7);
	Memory->WritePRG(0x9, 0xEF);
	Memory->WritePRG(0xa, 0xDF);
	Memory->WritePRG(0xf, 0xBF);

	Memory->WritePRG(0x4017, 0x00);
	Memory->WritePRG(0x4015, 0x00);

	for(int i = 0x4000; i <= 0x400F; i++)
		Memory->WritePRG(i, 0x00);

	std::fill_n(RAM.data(), 0x800, 0);

	PC = Memory->GetRV();
	CurrentCyle = 0;
}

void CPU::AllocateCycles(int nTicks)
{
	AllocatedCycles += nTicks;
}

void CPU::TriggerNMI()
{
	AwaitingNMI = true;
	PC = Memory->GetNMIV();
}

void CPU::ClearNMI()
{
	AwaitingNMI = false;
}

void CPU::TriggerVINT()
{
	TriggerNMI();
}
