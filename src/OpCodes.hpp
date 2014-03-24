#pragma once
#include <cstdint>

class CPU;

namespace InstructionTable
{
	typedef void(*Callback)(CPU&);
	struct InstructionPack
	{
		Callback Pre;
		Callback Exec;
		uint8_t Size;
		uint8_t Cycles;
		const char* Name;
	};

	const extern InstructionPack Table[256];

	const InstructionPack* GetInstruction(uint8_t opcode);

	static void ADC(CPU& cpu);
	static void AND(CPU& cpu);
	static void ASL(CPU& cpu);

	static void BCC(CPU& cpu);
	static void BCS(CPU& cpu);
	static void BEQ(CPU& cpu);
	static void BIT(CPU& cpu);
	static void BMI(CPU& cpu);
	static void BNE(CPU& cpu);
	static void BPL(CPU& cpu);
	static void BRK(CPU& cpu);
	static void BVC(CPU& cpu);
	static void BVS(CPU& cpu);

	static void CLC(CPU& cpu);
	static void CLD(CPU& cpu);
	static void CLI(CPU& cpu);
	static void CLV(CPU& cpu);
	static void CMP(CPU& cpu);
	static void CPX(CPU& cpu);
	static void CPY(CPU& cpu);

	static void DEC(CPU& cpu);
	static void DEX(CPU& cpu);
	static void DEY(CPU& cpu);
	static void DCP(CPU& cpu);
	
	static void EOR(CPU& cpu);
	
	static void INC(CPU& cpu);
	static void INX(CPU& cpu);
	static void INY(CPU& cpu);
	static void ISB(CPU& cpu);

	static void JMP(CPU& cpu);
	static void JSR(CPU& cpu);

	static void LDA(CPU& cpu);
	static void LDX(CPU& cpu);
	static void LDY(CPU& cpu);
	static void LSR(CPU& cpu);
	static void LAX(CPU& cpu);
	
	static void NOP(CPU& cpu);
	
	static void ORA(CPU& cpu);

	static void PHA(CPU& cpu);
	static void PHP(CPU& cpu);
	static void PLA(CPU& cpu);
	static void PLP(CPU& cpu);

	static void ROL(CPU& cpu);
	static void ROR(CPU& cpu);
	static void RTI(CPU& cpu);
	static void RTS(CPU& cpu);
	static void RLA(CPU& cpu);
	static void RRA(CPU& cpu);

	static void SBC(CPU& cpu);
	static void SEC(CPU& cpu);
	static void SED(CPU& cpu);
	static void SEI(CPU& cpu);
	static void STA(CPU& cpu);
	static void STX(CPU& cpu);
	static void STY(CPU& cpu);
	static void SAX(CPU& cpu);
	static void SLO(CPU& cpu);
	static void SRE(CPU& cpu);

	static void TAX(CPU& cpu);
	static void TAY(CPU& cpu);
	static void TSX(CPU& cpu);
	static void TXA(CPU& cpu);
	static void TXS(CPU& cpu);
	static void TYA(CPU& cpu);
};

namespace AddressingModes
{
	using InstructionTable::Callback;

	static void ZP(CPU& cpu);

	static void ZPX(CPU& cpu);
	static void ZPY(CPU& cpu);

	static void ABS(CPU& cpu);

	static void ABX(CPU& cpu);
	static void ABY(CPU& cpu);

	static void IMP(CPU& cpu);
	
	static void ACC(CPU& cpu);
	
	static void IMM(CPU& cpu);
	
	static void REL(CPU& cpu);
	
	static void IND(CPU& cpu);
	
	static void IIX(CPU& cpu);
	static void IIY(CPU& cpu);

	static void NOP2(CPU& cpu);
	static void NOP3(CPU& cpu);
};
