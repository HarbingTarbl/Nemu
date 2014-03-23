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
	PC(0)
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

	cout 
		<< hex << setw(4) << setfill('0') << PC  - Instruction->Size << " " 
		<< Instruction->Name << " A:" << setfill('0') << setw(2) << (int)A 
		<< " X:" << setfill('0') << setw(2) << (unsigned)X << " Y:" << setfill('0') << setw(2) << (unsigned)Y << " P:" << setfill('0') << setw(2) << (unsigned)Status
		<< " SP:" << setfill('0') << setw(2) << (unsigned)SP 
		<< endl; 

}

void CPU::Fetch()
{
	IR = Memory->Read(PC);
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

}

void CPU::Cycle()
{
	Fetch();
	Execute();
	Interrupt();
}


uint8_t CPU::Pop()
{
	return Memory->Read(++SP + 0x100);
}

void CPU::Push(uint8_t value)
{
	Memory->Write(SP-- + 0x100, value);
}

void CPU::SoftReset()
{
	SP -= 3;
	InterruptFlag = 1;
	Memory->Write(0x4015, 0x00);
}

void CPU::HardReset()
{
	Status = 0x24;
	A = X = Y = 0;
	SP = 0xFD;
	Asserted = false;

	Memory->Write(0x8, 0xF7);
	Memory->Write(0x9, 0xEF);
	Memory->Write(0xa, 0xDF);
	Memory->Write(0xf, 0xBF);

	Memory->Write(0x4017, 0x00);
	Memory->Write(0x4015, 0x00);

	for(int i = 0x4000; i <= 0x400F; i++)
		Memory->Write(i, 0x00);

	PC = Memory->GetRV();
	CurrentCyle = 0;
}