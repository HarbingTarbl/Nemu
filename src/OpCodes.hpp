#pragma once
#ifndef _OPCODES_HPP_
#define _OPCODES_HPP_

class OpTable
{
private:


public:

};



namespace Tables
{

	enum modeEnum
	{
		NUL, ACC, ABS, ABX, ABY, IMM, IMP, IND, XIN, INY, REL, ZPG, ZPX, ZPY
	};

	extern const char* namesTable[];
	extern int mode[];
	extern int sizeTable[];
	extern int cyclesTable[];

};

#endif
