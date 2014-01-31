#pragma once
#ifndef _OPCODES_HPP_
#define _OPCODES_HPP_


enum modeEnum
{
	NUL, ACC, ABS, ABX, ABY, IMM, IMP, IND, XIN, INY, REL, ZPG, ZPX, ZPY
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
