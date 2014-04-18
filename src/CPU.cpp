#include "CPU.hpp"
#include "VMemory.hpp"
#include "OpCodes.hpp"
#include "Renderer.hpp"


CPU::CPU()
	:
	A(0),
	SP(0),
	X(0),
	Y(0),
	PC(0),
	State(CPU_STATE_STARTUP)
{
	AwaitingNMI = false;
}

void CPU::DumpRegisters()
{

}

void CPU::Fetch()
{
	CycleOffset = 0;
	IR = Memory->ReadPRG(PC);
	PC++;
	Instruction = InstructionTable::GetInstruction(IR);
}

void CPU::Execute()
{
	Instruction->Pre(*this);
	DumpRegisters();
	//Something something cycle timings
	Instruction->Exec(*this);
	CurrentCyle += Instruction->Cycles + CycleOffset;
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

int CPU::Interrupt()
{
	if (InterruptQueue.front() != 0 && (Status & 0x04))
	{
		return 0;
	}

	PC = HandleInterrupt(InterruptQueue.front());
	InterruptQueue.pop_front();

	return 7;
}

int CPU::HandleInterrupt(int type)
{
	switch (type)
	{
	case 0: //NMI
		Push(PC >> 8);
		Push(PC & 0xFF);
		Push(Status & ~0x10);
		Status |= 0x04;
		return Memory->GetNMIV();
	case 1: //IRQ
		Push(PC >> 8);
		Push(PC & 0xFF);
		Push(Status & ~0x10);
		Status |= 0x04;
		return Memory->GetIRQ();
	case 2: //BRK
		Push(PC >> 8);
		Push(PC & 0xFF);
		Push(Status | 0x10);
		Status |= 0x04;
		return Memory->GetIRQ();
	case 3: //RESET
		return Memory->GetRV();
	}
}

int CPU::Cycle()
{
	Render::BeginFrame();
	if (InterruptQueue.size() > 0)
	{
		if (Interrupt())
			return 7; //?
	}

	Fetch();
	Execute();
	return Instruction->Cycles + CycleOffset;
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

	PC = HandleInterrupt(INTERRUPT_RESET);
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
