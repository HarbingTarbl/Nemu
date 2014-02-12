#include "OpCodes.hpp"
namespace InstructionTable
{
	using namespace AddressingModes;
#include "InstructionTable.inc"
};

namespace InstructionTable
{
	void ADC(CPU& cpu)
	{
		int8_t r = cpu.State.A + (int8_t)cpu.State.Arg8[0] + cpu.State.CarryFlag;
		cpu.State.OverflowFlag = (r  ^ cpu.State.A) & (r ^ cpu.State.Arg8[0]) & 0x80;
		cpu.State.ZeroFlag = r == 0;
	}

	void AND(CPU& cpu)
	{

	}

	void ASL(CPU& cpu)
	{

	}


	void BCC(CPU& cpu)
	{

	}

	void BCS(CPU& cpu)
	{

	}

	void BEQ(CPU& cpu)
	{

	}

	void BIT(CPU& cpu)
	{

	}

	void BMI(CPU& cpu)
	{

	}

	void BNE(CPU& cpu)
	{

	}

	void BPL(CPU& cpu)
	{

	}

	void BRK(CPU& cpu)
	{

	}

	void BVC(CPU& cpu)
	{

	}

	void BVS(CPU& cpu)
	{

	}


	void CLC(CPU& cpu)
	{

	}

	void CLD(CPU& cpu)
	{

	}

	void CLI(CPU& cpu)
	{

	}

	void CLV(CPU& cpu)
	{

	}

	void CMP(CPU& cpu)
	{

	}

	void CPX(CPU& cpu)
	{

	}

	void CPY(CPU& cpu)
	{

	}


	void DEC(CPU& cpu)
	{

	}

	void DEX(CPU& cpu)
	{

	}

	void DEY(CPU& cpu)
	{

	}


	void EOR(CPU& cpu)
	{

	}


	void INC(CPU& cpu)
	{

	}

	void INX(CPU& cpu)
	{

	}

	void INY(CPU& cpu)
	{

	}


	void JMP(CPU& cpu)
	{

	}

	void JSR(CPU& cpu)
	{

	}


	void LDA(CPU& cpu)
	{

	}

	void LDX(CPU& cpu)
	{

	}

	void LDY(CPU& cpu)
	{

	}

	void LSR(CPU& cpu)
	{

	}


	void NOP(CPU& cpu)
	{

	}


	void ORA(CPU& cpu)
	{

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

	}

	void SEC(CPU& cpu)
	{

	}

	void SED(CPU& cpu)
	{

	}

	void SEI(CPU& cpu)
	{

	}

	void STA(CPU& cpu)
	{

	}

	void STX(CPU& cpu)
	{

	}

	void STY(CPU& cpu)
	{

	}


	void TAX(CPU& cpu)
	{

	}

	void TAY(CPU& cpu)
	{

	}

	void TSX(CPU& cpu)
	{

	}

	void TXA(CPU& cpu)
	{

	}

	void TXS(CPU& cpu)
	{

	}

	void TYA(CPU& cpu)
	{

	}

};

namespace AddressingModes
{
	void ZP(CPU& cpu)
	{
		cpu.State.EffectiveAddr = cpu.Bus.Read(cpu.State.PC + 1);
		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.PC+= 2;
	}

	void ZPX(CPU& cpu)
	{
		cpu.State.EffectiveAddr = 
			(cpu.State.X + cpu.Bus.Read(cpu.State.PC + 1)) & 0x00FF;
		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.PC += 2;
	}

	void ZPY(CPU& cpu)
	{
		cpu.State.EffectiveAddr = 
			(cpu.State.Y + cpu.Bus.Read(cpu.State.PC + 1)) & 0x00FF;
		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.PC += 2;
	}

	void ABS(CPU& cpu)
	{
		cpu.State.EffectiveAddr =
			(cpu.Bus.Read(cpu.State.PC + 1) | cpu.Bus.Read(cpu.State.PC + 2) << 8);
		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);

		cpu.State.PC += 3;
	}

	void ABX(CPU& cpu)
	{
		cpu.State.EffectiveAddr = 
			(cpu.State.X + (cpu.Bus.Read(cpu.State.PC + 1) | cpu.Bus.Read(cpu.State.PC + 2) << 8 ));
		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);

		cpu.State.PC += 3;
	}

	void ABY(CPU& cpu)
	{
		cpu.State.EffectiveAddr = 
			(cpu.State.Y + (cpu.Bus.Read(cpu.State.PC + 1) | cpu.Bus.Read(cpu.State.PC + 2) << 8 ));
		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);

		cpu.State.PC += 3;
	}

	void IND(CPU& cpu)
	{
		cpu.State.EffectiveAddr = 
			(cpu.Bus.Read(cpu.State.PC + 1) | cpu.Bus.Read(cpu.State.PC + 2) << 8);

		cpu.State.EffectiveAddr = 
			(cpu.Bus.Read(cpu.State.EffectiveAddr) | cpu.Bus.Read(cpu.State.EffectiveAddr + 1) << 8);

		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.PC += 3;
	}

	void IMP(CPU& cpu) 
	{
		cpu.State.PC++;
	}

	void ACC(CPU& cpu)
	{
		cpu.State.Arg8[0] = cpu.State.A;
		cpu.State.PC++;
	}

	void IMM(CPU& cpu)
	{
		cpu.State.EffectiveAddr = cpu.State.PC + 1;
		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);

		cpu.State.PC += 2;
	}

	void REL(CPU& cpu)
	{
		cpu.State.EffectiveAddr = cpu.State.PC + 1;
		cpu.State.PC += 2;
		cpu.State.Arg8[0] = cpu.State.PC + (int8_t)cpu.Bus.Read(cpu.State.EffectiveAddr);
	}

	void IIX(CPU& cpu)
	{
		cpu.State.EffectiveAddr = cpu.State.PC + 1;
		cpu.State.EffectiveAddr = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.EffectiveAddr += cpu.State.X;
		cpu.State.EffectiveAddr = 
			(cpu.Bus.Read(cpu.State.EffectiveAddr) | cpu.Bus.Read(cpu.State.EffectiveAddr + 1) << 8);

		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.PC += 2;
	}

	void IIY(CPU& cpu)
	{
		cpu.State.EffectiveAddr = cpu.State.PC + 1;
		cpu.State.EffectiveAddr = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.EffectiveAddr = 
			(cpu.Bus.Read(cpu.State.EffectiveAddr) | cpu.Bus.Read(cpu.State.EffectiveAddr + 1) << 8)
			+ cpu.State.Y;

		cpu.State.Arg8[0] = cpu.Bus.Read(cpu.State.EffectiveAddr);
		cpu.State.PC += 2;
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