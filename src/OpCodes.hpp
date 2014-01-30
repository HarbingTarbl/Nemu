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

struct instructionInfo
{
	char* name;
	int size;
	int cycles;
	int mode;


};

enum modeEnum
{
	NUL, ACC, ABS, ABX, ABY, IMM, IMP, IND, XIN, INY, REL, ZPG, ZPX, ZPY
};

char* namesTable[] =
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

#endif