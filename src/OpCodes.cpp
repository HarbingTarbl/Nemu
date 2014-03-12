#include "OpCodes.hpp"
#ifndef __INTELLISENSE__
namespace InstructionTable
{
	using namespace AddressingModes;
#include "InstructionTable.inc"
};
#endif

namespace InstructionTable
{
	const InstructionPack* GetInstruction(uint8_t opcode)
	{
		return Table + opcode;
	}

	void ADC(CPU& cpu)
	{
		unsigned r = cpu.A;
		r += cpu.Memory[cpu.Addr];
		r += cpu.CarryFlag;
	
		cpu.CarryFlag = r & 0x100;
		cpu.ZeroFlag = r == 0;
		cpu.SignFlag = r & 0x80;
		cpu.OverflowFlag = (r ^ cpu.A) & 0x80;
		
		cpu.A = r;
	}

	void AND(CPU& cpu)
	{
		cpu.A = cpu.A & cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = cpu.A & 0x80;
	}

	void ASL(CPU& cpu)
	{
		///TODO The rotate instructions will be difficult. Maybe. Idk

	}

	void BCC(CPU& cpu)
	{
		if (!cpu.CarryFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
	}

	void BCS(CPU& cpu)
	{
		if (cpu.CarryFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
	}

	void BEQ(CPU& cpu)
	{
		if (cpu.ZeroFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
	}

	void BIT(CPU& cpu)
	{
		unsigned a = cpu.A;
		unsigned m = cpu.Memory[cpu.Addr];
		unsigned r = a & m;

		cpu.ZeroFlag = r == 0;
		cpu.SignFlag = m & 0x80;
		cpu.OverflowFlag = m & 0x40;
	}

	void BMI(CPU& cpu)
	{
		if (cpu.SignFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
	}

	void BNE(CPU& cpu)
	{
		if (!cpu.ZeroFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
	}

	void BPL(CPU& cpu)
	{
		if (!cpu.CarryFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
		cpu.PC += !cpu.SignFlag * cpu.Memory[cpu.Addr];
	}

	void BRK(CPU& cpu)
	{
		cpu.InterruptFlag = true;
		///TODO Interrupt Handling, Push
	}

	void BVC(CPU& cpu)
	{
		if (!cpu.OverflowFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
	}

	void BVS(CPU& cpu)
	{
		if (cpu.OverflowFlag)
			cpu.PC += cpu.Memory[cpu.Addr];
	}


	void CLC(CPU& cpu)
	{
		cpu.CarryFlag = false;
	}

	void CLD(CPU& cpu)
	{
		cpu.DecimalFlag = false;
	}

	void CLI(CPU& cpu)
	{
		cpu.InterruptFlag = false;
	}

	void CLV(CPU& cpu)
	{
		cpu.OverflowFlag = false;
	}

	void CMP(CPU& cpu)
	{
		unsigned a = cpu.A;
		unsigned m = cpu.Memory[cpu.Addr];
		unsigned r = a - m;

		cpu.SignFlag = r & 0x80;
		cpu.ZeroFlag = r == 0;
		cpu.CarryFlag = m <= a;
	}

	void CPX(CPU& cpu)
	{
		const int8_t r = cpu.State.X - cpu.State.Arg8[0];
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.State.CarryFlag = cpu.State.X >= cpu.State.Arg8[0];
	}

	void CPY(CPU& cpu)
	{
		const int8_t r = cpu.State.Y - cpu.State.Arg8[0];
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.State.CarryFlag = cpu.State.Y >= cpu.State.Arg8[0];
	}


	void DEC(CPU& cpu)
	{
		const int8_t r = cpu.State.Arg8[0] - 1;
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.Bus.Write(cpu.State.EffectiveAddr, r);
	}

	void DEX(CPU& cpu)
	{
		const int8_t r = cpu.State.X - 1;
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.Bus.Write(cpu.State.X, r);
	}

	void DEY(CPU& cpu)
	{
		const int8_t r = cpu.State.Y - 1;
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.Bus.Write(cpu.State.Y, r);
	}

	void EOR(CPU& cpu)
	{
		cpu.A = cpu.A ^ cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = cpu.A & 0x80;
	}

	void INC(CPU& cpu)
	{
		const int8_t r = cpu.State.Arg8[0] + 1;
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.Bus.Write(cpu.State.EffectiveAddr, r);
	}

	void INX(CPU& cpu)
	{
		const int8_t r = cpu.State.X + 1;
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.Bus.Write(cpu.State.X, r);
	}

	void INY(CPU& cpu)
	{
		const int8_t r = cpu.State.Y + 1;
		cpu.State.SignFlag = r & 0x80;
		cpu.State.ZeroFlag = r == 0;
		cpu.Bus.Write(cpu.State.Y, r);
	}


	void JMP(CPU& cpu)
	{
		cpu.PC = cpu.Memory[cpu.Addr] | cpu.Memory[cpu.Addr + 1] << 8;
	}

	void JSR(CPU& cpu)
	{

	}


	void LDA(CPU& cpu)
	{
		cpu.A = cpu.Memory[cpu.Addr];
		cpu.SignFlag = cpu.A & 0x80;
		cpu.ZeroFlag = cpu.A == 0;
	}

	void LDX(CPU& cpu)
	{
		cpu.X = cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.X == 0;
		cpu.SignFlag = cpu.X & 0x80;
	}

	void LDY(CPU& cpu)
	{
		cpu.Y = cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.Y == 0;
		cpu.SignFlag = cpu.Y & 0x80;
	}

	void LSR(CPU& cpu)
	{

	}


	void NOP(CPU& cpu)
	{

	}


	void ORA(CPU& cpu)
	{
		cpu.A = cpu.A | cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = cpu.A & 0x80;
	}


	void PHA(CPU& cpu)
	{

	}

	void PHP(CPU& cpu)
	{

	}

	void PLA(CPU& cpu)
	{

	}

	void PLP(CPU& cpu)
	{

	}


	void ROL(CPU& cpu)
	{

	}

	void ROR(CPU& cpu)
	{

	}

	void RTI(CPU& cpu)
	{

	}

	void RTS(CPU& cpu)
	{

	}


	void SBC(CPU& cpu)
	{
		unsigned r = cpu.A;
		r -= cpu.Memory[cpu.Addr];
		r -= cpu.CarryFlag;

		cpu.CarryFlag = r >= 0;
		cpu.ZeroFlag = r == 0;
		cpu.SignFlag = r & 0x80;
		cpu.OverflowFlag = r & 0x100;

		cpu.A = r;
	}

	void SEC(CPU& cpu)
	{
		cpu.State.CarryFlag = 1;
	}

	void SED(CPU& cpu)
	{
		cpu.State.DecimalFlag = 1;
	}

	void SEI(CPU& cpu)
	{
		cpu.State.InterruptFlag = 1;
	}

	void STA(CPU& cpu)
	{
		cpu.Memory[cpu.Addr] = cpu.A;
	}

	void STX(CPU& cpu)
	{
		cpu.Bus.Write(cpu.State.EffectiveAddr, cpu.State.X);
	}

	void STY(CPU& cpu)
	{
		cpu.Memory[cpu.Addr] = cpu.Y;
		cpu.Bus.Write(cpu.State.EffectiveAddr, cpu.State.Y);
	}

	void TAX(CPU& cpu)
	{
		cpu.State.ZeroFlag = cpu.State.A == 0;
		cpu.State.SignFlag = cpu.State.A & 0x80;
		cpu.State.X = cpu.State.A;
	}
	
	void TAY(CPU& cpu)
	{
		cpu.State.ZeroFlag = cpu.State.A == 0;
		cpu.State.SignFlag = cpu.State.A & 0x80;
		cpu.State.Y = cpu.State.A;
	}

	void TSX(CPU& cpu)
	{
		cpu.State.ZeroFlag = cpu.State.SP == 0;
		cpu.State.SignFlag = cpu.State.SP & 0x80;
		cpu.State.X = cpu.State.SP;
	}

	void TXA(CPU& cpu)
	{
		cpu.State.ZeroFlag = cpu.State.X == 0;
		cpu.State.SignFlag = cpu.State.X & 0x80;
		cpu.State.A = cpu.State.X;
	}

	void TXS(CPU& cpu)
	{
		cpu.State.SP = cpu.State.X;
	}

	void TYA(CPU& cpu)
	{
		cpu.State.ZeroFlag = cpu.State.Y == 0;
		cpu.State.SignFlag = cpu.State.Y & 0x80;
		cpu.State.A = cpu.State.Y;
	}

};

namespace AddressingModes
{
	void ZP(CPU& cpu)
	{
		cpu.Addr = cpu.Memory[cpu.PC++];
	}

	void ZPX(CPU& cpu)
	{
		cpu.Addr = (cpu.X + cpu.Memory[cpu.PC++]) & 0x00FF;
	}

	void ZPY(CPU& cpu)
	{
		cpu.Addr = (cpu.Y + cpu.Memory[cpu.PC++]) & 0xFF;
	}

	void ABS(CPU& cpu)
	{
		cpu.Addr = (cpu.Memory[cpu.PC] | cpu.Memory[cpu.PC + 1] << 8);
		cpu.PC += 2;
	}

	void ABX(CPU& cpu)
	{
		cpu.Addr = (cpu.X + (cpu.Memory[cpu.PC] | cpu.Memory[cpu.PC + 1] << 8));
		cpu.PC += 2;
	}

	void ABY(CPU& cpu)
	{
		cpu.Addr = (cpu.Y + (cpu.Memory[cpu.PC] | cpu.Memory[cpu.PC + 1] << 8));
		cpu.PC += 2;
	}

	void IND(CPU& cpu)
	{
		cpu.Addr = (cpu.Memory[cpu.PC] | cpu.Memory[cpu.PC + 1] << 8);
		cpu.Addr = (cpu.Memory[cpu.Addr] | cpu.Memory[cpu.Addr + 1] << 8);
		cpu.PC += 2;
	}

	void IMP(CPU& cpu) 
	{
		//NOP?
	}

	void ACC(CPU& cpu)
	{
		cpu.Addr = VMemory::Accum;
	}

	void IMM(CPU& cpu)
	{
		cpu.Addr = cpu.PC++;
	}

	void REL(CPU& cpu)
	{
		cpu.Addr = cpu.PC + cpu.Memory[cpu.PC];
		cpu.PC += 1;
	}

	void IIX(CPU& cpu)
	{
		cpu.Addr = cpu.Memory[cpu.PC];
		cpu.Addr += cpu.X;
		cpu.Addr = cpu.Memory[cpu.Addr] | cpu.Memory[cpu.Addr + 1] << 8;
		cpu.PC += 1;
	}

	void IIY(CPU& cpu)
	{
		cpu.Addr = cpu.Memory[cpu.PC];
		cpu.Addr = (cpu.Memory[cpu.Addr] | cpu.Memory[cpu.Addr + 1] << 8);
		cpu.Addr += cpu.Y;
		cpu.PC += 1;
	}
};

namespace Tables
{
	int mode[] = 
	{
		IMP, XIN, NUL, NUL, // 0x00-0x03
		NUL, ZPG, ZPG, NUL, // 0x04-0x07
		IMP, IMM, ACC, NUL, // 0x08-0x0b
		NUL, ABS, ABS, NUL, // 0x0c-0x0f
		REL, INY, NUL, NUL, // 0x10-0x13
		NUL, ZPX, ZPX, NUL, // 0x14-0x17
		IMP, ABY, NUL, NUL, // 0x18-0x1b
		NUL, ABX, ABX, NUL, // 0x1c-0x1f
		ABS, XIN, NUL, NUL, // 0x20-0x23
		ZPG, ZPG, ZPG, NUL, // 0x24-0x27
		IMP, IMM, ACC, NUL, // 0x28-0x2b
		ABS, ABS, ABS, NUL, // 0x2c-0x2f
		REL, INY, NUL, NUL, // 0x30-0x33
		NUL, ZPX, ZPX, NUL, // 0x34-0x37
		IMP, ABY, NUL, NUL, // 0x38-0x3b
		NUL, ABX, ABX, NUL, // 0x3c-0x3f
		IMP, XIN, NUL, NUL, // 0x40-0x43
		NUL, ZPG, ZPG, NUL, // 0x44-0x47
		IMP, IMM, ACC, NUL, // 0x48-0x4b
		ABS, ABS, ABS, NUL, // 0x4c-0x4f
		REL, INY, NUL, NUL, // 0x50-0x53
		NUL, ZPX, ZPX, NUL, // 0x54-0x57
		IMP, ABY, NUL, NUL, // 0x58-0x5b
		NUL, ABX, ABX, NUL, // 0x5c-0x5f
		IMP, XIN, NUL, NUL, // 0x60-0x63
		NUL, ZPG, ZPG, NUL, // 0x64-0x67
		IMP, IMM, ACC, NUL, // 0x68-0x6b
		IND, ABS, ABS, NUL, // 0x6c-0x6f
		REL, INY, NUL, NUL, // 0x70-0x73
		NUL, ZPX, ZPX, NUL, // 0x74-0x77
		IMP, ABY, NUL, NUL, // 0x78-0x7b
		NUL, ABX, ABX, NUL, // 0x7c-0x7f
		REL, XIN, NUL, NUL, // 0x80-0x83
		ZPG, ZPG, ZPG, NUL, // 0x84-0x87
		IMP, NUL, IMP, NUL, // 0x88-0x8b
		ABS, ABS, ABS, NUL, // 0x8c-0x8f
		REL, INY, NUL, NUL, // 0x90-0x93
		ZPX, ZPX, ZPY, NUL, // 0x94-0x97
		IMP, ABY, IMP, NUL, // 0x98-0x9b
		NUL, ABX, NUL, NUL, // 0x9c-0x9f
		IMM, XIN, IMM, NUL, // 0xa0-0xa3
		ZPG, ZPG, ZPG, NUL, // 0xa4-0xa7
		IMP, IMM, IMP, NUL, // 0xa8-0xab
		ABS, ABS, ABS, NUL, // 0xac-0xaf
		REL, INY, NUL, NUL, // 0xb0-0xb3
		ZPX, ZPX, ZPY, NUL, // 0xb4-0xb7
		IMP, ABY, IMP, NUL, // 0xb8-0xbb
		ABX, ABX, ABY, NUL, // 0xbc-0xbf
		IMM, XIN, NUL, NUL, // 0xc0-0xc3
		ZPG, ZPG, ZPG, NUL, // 0xc4-0xc7
		IMP, IMM, IMP, NUL, // 0xc8-0xcb
		ABS, ABS, ABS, NUL, // 0xcc-0xcf
		REL, INY, NUL, NUL, // 0xd0-0xd3
		NUL, ZPX, ZPX, NUL, // 0xd4-0xd7
		IMP, ABY, NUL, NUL, // 0xd8-0xdb
		NUL, ABX, ABX, NUL, // 0xdc-0xdf
		IMM, XIN, NUL, NUL, // 0xe0-0xe3
		ZPG, ZPG, ZPG, NUL, // 0xe4-0xe7
		IMP, IMM, IMP, NUL, // 0xe8-0xeb
		ABS, ABS, ABS, NUL, // 0xec-0xef
		REL, INY, NUL, NUL, // 0xf0-0xf3
		NUL, ZPX, ZPX, NUL, // 0xf4-0xf7
		IMP, ABY, NUL, NUL, // 0xf8-0xfb
		NUL, ABX, ABX, NUL // 0xfc-0xff
	};


	const char* namesTable[] =
	{
		"BRK", "ORA", nullptr, nullptr, nullptr, "ORA", "ASL", nullptr,
		"PHP", "ORA", "ASL", nullptr, nullptr, "ORA", "ASL", nullptr,
		"BPL", "ORA", nullptr, nullptr, nullptr, "ORA", "ASL", nullptr,
		"CLC", "ORA", nullptr, nullptr, nullptr, "ORA", "ASL", nullptr,
		"JSR", "AND", nullptr, nullptr, "BIT", "AND", "ROL", nullptr,
		"PLP", "AND", "ROL", nullptr, "BIT", "AND", "ROL", nullptr,
		"BMI", "AND", nullptr, nullptr, nullptr, "AND", "ROL", nullptr,
		"SEC", "AND", nullptr, nullptr, nullptr, "AND", "ROL", nullptr,
		"RTI", "EOR", nullptr, nullptr, nullptr, "EOR", "LSR", nullptr,
		"PHA", "EOR", "LSR", nullptr, "JMP", "EOR", "LSR", nullptr,
		"BVC", "EOR", nullptr, nullptr, nullptr, "EOR", "LSR", nullptr,
		"CLI", "EOR", nullptr, nullptr, nullptr, "EOR", "LSR", nullptr,
		"RTS", "ADC", nullptr, nullptr, nullptr, "ADC", "ROR", nullptr,
		"PLA", "ADC", "ROR", nullptr, "JMP", "ADC", "ROR", nullptr,
		"BVS", "ADC", nullptr, nullptr, nullptr, "ADC", "ROR", nullptr,
		"SEI", "ADC", nullptr, nullptr, nullptr, "ADC", "ROR", nullptr,
		"BCS", "STA", nullptr, nullptr, "STY", "STA", "STX", nullptr,
		"DEY", nullptr, "TXA", nullptr, "STY", "STA", "STX", nullptr,
		"BCC", "STA", nullptr, nullptr, "STY", "STA", "STX", nullptr,
		"TYA", "STA", "TXS", nullptr, nullptr, "STA", nullptr, nullptr,
		"LDY", "LDA", "LDX", nullptr, "LDY", "LDA", "LDX", nullptr,
		"TAY", "LDA", "TAX", nullptr, "LDY", "LDA", "LDX", nullptr,
		"BCS", "LDA", nullptr, nullptr, "LDY", "LDA", "LDX", nullptr,
		"CLV", "LDA", "TSX", nullptr, "LDY", "LDA", "LDX", nullptr,
		"CPY", "CMP", nullptr, nullptr, "CPY", "CMP", "DEC", nullptr,
		"INY", "CMP", "DEX", nullptr, "CPY", "CMP", "DEC", nullptr,
		"BNE", "CMP", nullptr, nullptr, nullptr, "CMP", "DEC", nullptr,
		"CLD", "CMP", nullptr, nullptr, nullptr, "CMP", "DEC", nullptr,
		"CPX", "SBC", nullptr, nullptr, "CPX", "SBC", "INC", nullptr,
		"INX", "SBC", "NOP", nullptr, "CPX", "SBC", "INC", nullptr,
		"BEQ", "SBC", nullptr, nullptr, nullptr, "SBC", "INC", nullptr,
		"SED", "SBC", nullptr, nullptr, nullptr, "SBC", "INC", nullptr
	};

	int cyclesTable[] =
	{
		7, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 0, 4, 6, 0,
		2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
		6, 6, 0, 0, 3, 3, 5, 0, 4, 2, 2, 0, 4, 4, 6, 0,
		2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
		6, 6, 0, 0, 0, 3, 5, 0, 3, 2, 2, 0, 3, 4, 6, 0,
		2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
		6, 6, 0, 0, 0, 3, 5, 0, 4, 2, 2, 0, 5, 4, 6, 0,
		2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
		2, 6, 0, 0, 3, 3, 3, 0, 2, 0, 2, 0, 4, 4, 4, 0,
		2, 6, 0, 0, 4, 4, 4, 0, 2, 5, 2, 0, 0, 5, 0, 0,
		2, 6, 2, 0, 3, 3, 3, 0, 2, 2, 2, 0, 4, 4, 4, 0,
		2, 5, 0, 0, 4, 4, 4, 0, 2, 4, 2, 0, 4, 4, 4, 0,
		2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0,
		2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0,
		2, 6, 0, 0, 3, 3, 5, 0, 2, 2, 2, 0, 4, 4, 6, 0,
		2, 5, 0, 0, 0, 4, 6, 0, 2, 4, 0, 0, 0, 4, 7, 0
	};

	int sizeTable[] = 
	{
		1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 0, 3, 3, 0,
		2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
		3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
		1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
		1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
		2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
		2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
		2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0
	};



};