#pragma once
#ifndef _OPCODES_HPP_
#define _OPCODES_HPP_

enum OpCodes
{
	//Add With Carry
	ADC_IM= 0x69,
	ADC_ZP = 0x65,
	ADC_ZP_X = 0x75,
	ADC_AB = 0x6D,
	ADC_AB_X = 0x7D,
	ADC_AB_Y = 0x79,
	ADC_IN_X = 0x61,
	ADC_IN_Y = 0x71,

	//Binary And
	AND_IM = 0x29,
	AND_ZP = 0x25,
	AND_ZP_X = 0x35,
	AND_AB = 0x2D,
	AND_AB_X = 0x3D,
	AND_AB_Y = 0x39,
	AND_IN_X = 0x21,
	AND_IN_Y = 0x31,

	//Arithmatic Shift Left
	ASL_AC = 0x0A,
	ASL_ZP = 0x06,
	ASL_ZP_X = 0x16,
	ASL_AB = 0x0E,
	ASL_AB_X = 0x1E,

	//Branching
	BCC_REL = 0x90,
	BCS_REL = 0xB0,
	BEQ_REL = 0xF0,
	BIT_ZP = 0x24,
	BIT_AB = 0x2C,
	BMI_REL = 0x30,
	BNE_REL = 0xD0,
	BPL_REL = 0x10,
	BVC_REL = 0x50,
	BVS_REL = 0x70,

	//Compare
	CMP_IM = 0xC9,
	CML_ZP = 0xC5,
	CMP_ZP_X = 0xD5,
	CMP_AB = 0xCD,
	CMP_AB_X = 0xDD,
	CMP_AB_Y = 0xD9,
	CMP_IN_X = 0xC1,
	CMP_IN_Y = 0xD1,

	//Compare X
	CPX_IM = 0xE0,
	CPX_ZP = 0xE4,
	CPX_AB = 0xEC,

	//Compare Y
	CPY_IM = 0xC0,
	CPY_ZP = 0xC4,
	CPY_AB = 0xCC,

	//Decrement
	DEC_ZP = 0xC6,
	DEC_ZP_X = 0xD6,
	DEC_AB = 0xCE,
	DEC_AB_X = 0xDE,

	//Decrement X
	DEX = 0xCA,

	//Decrement Y
	DEY = 0x88,

	//Exclusive Or
	EOR_IM = 0x49,
	EOR_ZP = 0x45,
	EOR_ZP_X = 0x55,
	EOR_AB = 0x4D,
	EOR_AB_X = 0x5D,
	EOR_AB_Y = 0x59,
	EOR_IN_X = 0x41,
	EOR_IN_Y = 0x51,

	//Increment
	INC_ZP = 0xE6,
	INC_ZP_X = 0xF6,
	INC_AB = 0xEE,
	INT_AB_X = 0xFE,

	//Increment X
	INX = 0xE8,

	//Increment Y
	INY = 0xC8,

	//Jump
	JMP_AB = 0x4C,
	JMP_IN = 0x6C,

	//Call (Jump To Sub)
	JSR_AB = 0x20,

	//Load Accum
	LDA_IM = 0xA9,
	LDA_ZP = 0xA5,
	LDA_ZP_X = 0xB5,
	LDA_AB = 0xAD,
	LDA_AB_X = 0xBD,
	LDA_AB_Y = 0xB9,
	LDA_IN_X = 0xA1,
	LDA_IN_Y = 0xB1,

	//Load X Reg
	LDX_IM = 0xA2,
	LDX_ZP = 0xA6,
	LDX_ZP_Y = 0xB6,
	LDX_AB = 0xAE,
	LDX_AB_Y = 0xBE,

	//Load Y Reg
	LDY_IM = 0xA0,
	LDY_ZP = 0xA4,
	LDY_ZP_X = 0xB4,
	LDY_AB = 0xAC,
	LDY_AB_X = 0xBC,

	//Logical Shift Right
	LSR_AC = 0x4A,
	LSR_ZP = 0x46,
	LSR_ZP_X = 0x56,
	LSR_AB = 0x4E,
	LSR_AB_X = 0x5E,

	//No Op
	NOP = 0xEA,

	//Logical Inclusive Or
	ORA_IM = 0x09,
	ORA_ZP = 0x05,
	ORA_ZP_X = 0x15,
	ORA_AB = 0x0D,
	ORA_AB_X = 0x1D,
	ORA_AB_Y = 0x19,
	ORA_IN_X = 0x01,
	ORA_IN_Y = 0x11,

	//Push Accum
	PHA = 0x48,

	//Push Status
	PHP = 0x08,

	//Pull Accum
	PLA = 0x68,

	//Pull Status
	PLP = 0x28,

	//Rotate Left
	ROL_AC = 0x2A,
	ROL_ZP = 0x26,
	ROL_ZP_X = 0x36,
	ROL_AB = 0x2E,
	ROL_AB_X = 0x3E,

	//Rotate Right
	ROR_AC = 0x6A,
	ROR_ZP = 0x66,
	ROR_ZP_X = 0x76,
	ROR_AB = 0x6E,
	ROR_AB_X = 0x7E,

	//Return From Interrupt
	RTI = 0x40,

	//Return From Sub
	RTS = 0x60,

	//Subtract With Carry
	SBC_IM = 0xE9,
	SBC_ZP = 0xE5,
	SBC_ZP_X = 0xF5,
	SBC_AB = 0xED,
	SBC_AB_X = 0xFD,
	SBC_AB_Y = 0xF9,
	SBC_IN_X = 0xE1,
	SBC_IN_Y = 0xF1,

	//Store Accum
	STA_ZP = 0x85,
	STA_ZP_X = 0x95,
	STA_AB = 0x8D,
	STA_AB_X = 0x9D,
	STA_AB_Y = 0x99,
	STA_IN_X = 0x81,
	STA_IN_Y = 0x91,

	//Store X Reg
	STX_ZP = 0x86,
	STX_ZP_Y = 0x96,
	STX_AB = 0x8E,

	//Store Y Reg
	STX_ZP = 0x84,
	STX_ZP_X = 0x94,
	STX_AB = 0x8C,

	//Transfer Accum to Reg X
	TAX = 0xAA,

	//Transfer Accum to Reg Y
	TAY = 0xA8,

	//Transfer SP to Reg X
	TSX = 0xBA,

	//Transfer Reg X to Accum
	TXA = 0x8A,

	//Transfer Reg X to SP
	TXS = 0x9A,

	//Transfer Reg Y to Accum
	TYA = 0x98,

	//Set Flags Flag
	SEC = 0x38, //Carry
	SED = 0xF8,
	SEI = 0x78,

	//Clear Flags
	CLV = 0xB8,
	CLI = 0x58, //Interrupt Disable
	CLD = 0xD8, //Decimal
	CLC = 0x18, //Carry

	BRK = 0,
};

namespace decoding
{
	namespace c01
	{
		enum class a : unsigned char
		{
			ORA = 0,
			AND = 1,
			EOR = 2,
			ADC = 3,
			STA = 4,
			LDA = 5,
			CMP = 6,
			SBC = 7
		};

		enum class b : unsigned char
		{
			ZP_X = 0,
			ZP = 1,
			IM = 2,
			AB = 3,
			ZP_Y = 4,
			ZP_X = 5,
		};
	};

	namespace c10
	{


	};
};

#endif