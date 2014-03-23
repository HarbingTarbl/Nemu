#include "OpCodes.hpp"
#include "CPU.hpp"

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
		unsigned a = cpu.A;
		unsigned m = cpu.Memory[cpu.Addr];

		unsigned r = a;
		r += m;
		r += cpu.CarryFlag;

		cpu.CarryFlag = r > 0xFF;
		cpu.ZeroFlag = (r & 0xFF) == 0;
		cpu.SignFlag = (r & 0x80) >> 7;
		cpu.OverflowFlag = (!((a ^ m) & 0x80) && ((r ^ a) & 0x80));

		cpu.A = r & 0xFF;
	}

	void AND(CPU& cpu)
	{
		cpu.A = cpu.A & cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = (cpu.A & 0x80) >> 7;
	}

	void ASL(CPU& cpu)
	{
		unsigned m = cpu[cpu.Addr];
		cpu.CarryFlag = (m & 0x80) >> 7;
		m = cpu[cpu.Addr] = (m << 1) & 0xFF;

		cpu.SignFlag = (m & 0x80) >> 7;
		cpu.ZeroFlag = m == 0;
	}

	void BCC(CPU& cpu)
	{
		if (!cpu.CarryFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
	}

	void BCS(CPU& cpu)
	{
		if (cpu.CarryFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
	}

	void BEQ(CPU& cpu)
	{
		if (cpu.ZeroFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
	}

	void BIT(CPU& cpu)
	{
		unsigned a = cpu.A;
		unsigned m = cpu.Memory[cpu.Addr];
		unsigned r = a & m;

		cpu.ZeroFlag = r == 0;
		cpu.SignFlag = (m & 0x80) >> 7;
		cpu.OverflowFlag = (m & 0x40) >> 6;
	}

	void BMI(CPU& cpu)
	{
		if (cpu.SignFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
	}

	void BNE(CPU& cpu)
	{
		if (!cpu.ZeroFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
	}

	void BPL(CPU& cpu)
	{
		if (!cpu.SignFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
	}

	void BRK(CPU& cpu)
	{
		PHP(cpu);
		SEI(cpu);
		cpu.Asserted = true;
		///TODO Interrupt Handling, Push
	}

	void BVC(CPU& cpu)
	{
		if (!cpu.OverflowFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
	}

	void BVS(CPU& cpu)
	{
		if (cpu.OverflowFlag)
			cpu.PC = cpu.Addr & 0xFFFF;
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

		cpu.SignFlag = (r & 0x80) >> 7;
		cpu.ZeroFlag = r == 0;
		cpu.CarryFlag = m <= a;
	}

	void CPX(CPU& cpu)
	{
		unsigned x = cpu.X;
		unsigned m = cpu.Memory[cpu.Addr];
		unsigned r = x - m;

		cpu.SignFlag = (r & 0x80) >> 7;
		cpu.ZeroFlag = r == 0;
		cpu.CarryFlag = r < 0x100;
	}

	void CPY(CPU& cpu)
	{
		unsigned y = cpu.Y;
		unsigned m = cpu.Memory[cpu.Addr];
		unsigned r = y - m;

		cpu.SignFlag = (r & 0x80) >> 7;
		cpu.ZeroFlag = r == 0;
		cpu.CarryFlag = r < 0x100;
	}


	void DEC(CPU& cpu)
	{
		auto& m = cpu.Memory[cpu.Addr];
		m--;
		cpu.ZeroFlag = m == 0;
		cpu.SignFlag = (m & 0x80) >> 7;
	}

	void DEX(CPU& cpu)
	{
		cpu.X -= 1;
		cpu.SignFlag = (cpu.X & 0x80) >> 7;
		cpu.ZeroFlag = cpu.X == 0;
	}

	void DEY(CPU& cpu)
	{
		cpu.Y -= 1;
		cpu.SignFlag = (cpu.Y & 0x80) >> 7;
		cpu.ZeroFlag = cpu.Y == 0;
	}

	void DCP(CPU& cpu)
	{
		auto m = cpu.Memory[cpu.Addr];
		auto a = cpu.A;

		m  -= 1;
		auto r = a - m;

		cpu.SignFlag = (r & 0x80) >> 7;
		cpu.ZeroFlag = r == 0;
		cpu.CarryFlag = m <= a;

		cpu.Memory[cpu.Addr] = m;
	}

	void EOR(CPU& cpu)
	{
		cpu.A = cpu.A ^ cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = (cpu.A & 0x80) >> 7;
	}

	void INC(CPU& cpu)
	{
		auto& m = cpu.Memory[cpu.Addr];
		m++;
		cpu.SignFlag = (m & 0x80) >> 7;
		cpu.ZeroFlag = m == 0;
	}

	void INX(CPU& cpu)
	{
		cpu.X += 1;
		cpu.SignFlag = (cpu.X & 0x80) >> 7;
		cpu.ZeroFlag = cpu.X == 0;
	}

	void INY(CPU& cpu)
	{
		cpu.Y += 1;
		cpu.SignFlag = (cpu.Y & 0x80) >> 7;
		cpu.ZeroFlag = cpu.Y == 0;
	}

	void ISB(CPU& cpu)
	{
		INC(cpu);
		SBC(cpu);
	}

	void JMP(CPU& cpu)
	{
		cpu.PC = cpu.Addr & 0xFFFF;
	}

	void JSR(CPU& cpu)
	{
		cpu.PC--;

		cpu.Push((cpu.PC & 0xFF00) >> 8);

		cpu.Push((cpu.PC & 0x00FF) >> 0);


		cpu.PC = cpu.Addr & 0xFFFF;
	}


	void LDA(CPU& cpu)
	{
		cpu.A = cpu.Memory[cpu.Addr];
		cpu.SignFlag = (cpu.A & 0x80) >> 7;
		cpu.ZeroFlag = cpu.A == 0;
	}

	void LDX(CPU& cpu)
	{
		cpu.X = cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.X == 0;
		cpu.SignFlag = (cpu.X & 0x80) >> 7;
	}

	void LDY(CPU& cpu)
	{
		cpu.Y = cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.Y == 0;
		cpu.SignFlag = (cpu.Y & 0x80) >> 7;
	}

	void LSR(CPU& cpu)
	{
		unsigned m = cpu[cpu.Addr];
		cpu.CarryFlag = m & 0x01;
		m = cpu[cpu.Addr] = (m >> 1) & 0xFF;

		cpu.ZeroFlag = m == 0;
		cpu.SignFlag = (m & 0x80) >> 7;
	}

	void LAX(CPU& cpu)
	{
		uint8_t m = cpu.Memory[cpu.Addr];
		cpu.A = m;
		cpu.X = m;
		cpu.ZeroFlag = m == 0;
		cpu.SignFlag = (m & 0x80) >> 7;
	}

	void NOP(CPU& cpu)
	{

	}


	void ORA(CPU& cpu)
	{
		cpu.A = cpu.A | cpu.Memory[cpu.Addr];
		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = (cpu.A & 0x80) >> 7;
	}


	void PHA(CPU& cpu)
	{
		cpu.Push(cpu.A);
	}

	void PHP(CPU& cpu)
	{
		cpu.Push(cpu.Status | 0x30);
	}

	void PLA(CPU& cpu)
	{
		auto a = cpu.Pop();
		cpu.ZeroFlag = a == 0;
		cpu.SignFlag = (a & 0x80) >> 7;
		cpu.A = a;
	}

	void PLP(CPU& cpu)
	{
		cpu.Status &= 0x30;
		cpu.Status |= (cpu.Pop() & 0xCF);
	}


	void ROL(CPU& cpu)
	{
		unsigned m = cpu[cpu.Addr];
		cpu[cpu.Addr] = ((m << 1) | cpu.CarryFlag) & 0xFF;
		cpu.CarryFlag = (m & 0x80) >> 7;

		m = cpu[cpu.Addr];
		cpu.ZeroFlag = m == 0;
		cpu.SignFlag = (m & 0x80) >> 7;
	}

	void ROR(CPU& cpu)
	{
		unsigned m = cpu[cpu.Addr];
		cpu[cpu.Addr] = ((m >> 1) | cpu.CarryFlag << 7) & 0xFF;
		cpu.CarryFlag = m & 0x01;

		m = cpu[cpu.Addr];
		cpu.ZeroFlag = m == 0;
		cpu.SignFlag = (m & 0x80) >> 7;
	}

	void RTI(CPU& cpu)
	{
		//cpu.Status = cpu.Pop() & 0xCF;
		PLP(cpu);
		cpu.PC = cpu.Pop();
		cpu.PC |= cpu.Pop() << 8;
	}

	void RTS(CPU& cpu)
	{
		cpu.PC = cpu.Pop();
		cpu.PC |= cpu.Pop() << 8;
		cpu.PC += 1;
	}

	void RLA(CPU& cpu)
	{
		ROL(cpu);
		AND(cpu);
	}

	void RRA(CPU& cpu)
	{
		ROR(cpu);
		ADC(cpu);
	}

	void SBC(CPU& cpu)
	{
		unsigned a = cpu.A;
		unsigned m = cpu.Memory[cpu.Addr];

		unsigned r = a;
		r -= m;
		r -= !cpu.CarryFlag;

		cpu.CarryFlag = r < 0x100;
		cpu.ZeroFlag = r == 0;
		cpu.SignFlag = (r & 0x80) >> 7;
		cpu.OverflowFlag = ((r ^ a) & 0x80) && ((a ^ m) & 0x80);

		cpu.A = r & 0xFF;
	}

	void SEC(CPU& cpu)
	{
		cpu.CarryFlag = 1;
	}

	void SED(CPU& cpu)
	{
		cpu.DecimalFlag = 1;
	}

	void SEI(CPU& cpu)
	{
		cpu.InterruptFlag = 1;
	}

	void STA(CPU& cpu)
	{
		cpu.Memory[cpu.Addr] = cpu.A;
	}

	void STX(CPU& cpu)
	{
		cpu.Memory[cpu.Addr] = cpu.X;
	}

	void STY(CPU& cpu)
	{
		cpu.Memory[cpu.Addr] = cpu.Y;
	}

	void SAX(CPU& cpu)
	{
		cpu.Memory[cpu.Addr] = cpu.A & cpu.X;
	}

	void SLO(CPU& cpu)
	{
		ASL(cpu);
		ORA(cpu);
	}

	void SRE(CPU& cpu)
	{
		LSR(cpu);
		EOR(cpu);
	}

	void TAX(CPU& cpu)
	{
		cpu.X = cpu.A;

		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = (cpu.A & 0x80) >> 7;
	}

	void TAY(CPU& cpu)
	{
		cpu.Y = cpu.A;

		cpu.ZeroFlag = cpu.A == 0;
		cpu.SignFlag = (cpu.A & 0x80) >> 7;
	}

	void TSX(CPU& cpu)
	{
		cpu.ZeroFlag = cpu.SP == 0;
		cpu.SignFlag = (cpu.SP & 0x80) >> 7;
		cpu.X = cpu.SP;
	}

	void TXA(CPU& cpu)
	{
		cpu.A = cpu.X;

		cpu.ZeroFlag = cpu.X == 0;
		cpu.SignFlag = (cpu.X & 0x80) >> 7;
	}

	void TXS(CPU& cpu)
	{
		cpu.SP = cpu.X;
	}

	void TYA(CPU& cpu)
	{
		cpu.A = cpu.Y;

		cpu.ZeroFlag = cpu.Y == 0;
		cpu.SignFlag = (cpu.Y & 0x80) >> 7;
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
		//IND Jump Bug
		if((cpu.Addr & 0x00FF) == 0xFF) 
		{
			cpu.Addr = (cpu.Memory[cpu.Addr] | cpu.Memory[cpu.Addr & 0xFF00] << 8);
		}
		else
		{
			cpu.Addr = (cpu.Memory[cpu.Addr] | cpu.Memory[cpu.Addr + 1] << 8);
		}
		cpu.PC += 2;
	}

	void IMP(CPU& cpu) 
	{
		//NOP?
	}

	void ACC(CPU& cpu)
	{
		cpu.Addr = -1;
	}

	void IMM(CPU& cpu)
	{
		cpu.Addr = cpu.PC++;
	}

	void REL(CPU& cpu)
	{
		cpu.Addr = cpu.PC + (int8_t)cpu.Memory[cpu.PC] + 1;
		cpu.PC += 1;
	}

	void IIX(CPU& cpu)
	{
		cpu.Addr = cpu.Memory[cpu.PC];
		cpu.Addr += cpu.X;
		cpu.Addr = cpu.Memory[cpu.Addr & 0xFF] | cpu.Memory[(cpu.Addr + 1) & 0xFF] << 8;
		cpu.PC += 1;
	}

	void IIY(CPU& cpu)
	{
		cpu.Addr = cpu.Memory[cpu.PC];
		cpu.Addr = (cpu.Memory[cpu.Addr & 0xFF] | cpu.Memory[(cpu.Addr + 1) & 0xFF] << 8);
		cpu.Addr += cpu.Y;
		cpu.PC += 1;
	}

	void NOP2(CPU& cpu)
	{
		cpu.PC++;
	}

	void NOP3(CPU& cpu)
	{
		cpu.PC += 2;
	}
};