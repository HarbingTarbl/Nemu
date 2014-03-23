#pragma once
#include "VMemory.hpp"
#include <string>
#include <fstream>

using std::string;

struct RomInfo
{
private:
	union
	{
		struct
		{
			char NES[4];
			uint8_t PRGPages;
			uint8_t CHRPages;
			uint8_t Flags6;
			uint8_t Flags7;
			uint8_t MapperVarient;
			uint8_t ROMSize;
			uint8_t RAMSize;
			uint8_t VRAMSize;
			uint8_t TVCode;
			uint8_t VSHardware;
			uint8_t __None1;
			uint8_t __None2;
		};
		char ___[16];
	};
public:
	string filename;

	bool IsRom()
	{
		return 
			NES[0] == 'N' &&
			NES[1] == 'E' &&
			NES[2] == 'S' &&
			NES[3] == 0x1A;
	}

	uint8_t NumPRG()
	{
		return PRGPages;
	}

	uint8_t NumCHR()
	{
		return CHRPages;
	}

	uint8_t MapperID() 
	{
		return ((Flags6 & 0xF0) >> 4) | (Flags7 & 0xF0);
	}

	static RomInfo FromFile(const string& name)
	{
		using std::fstream;
		fstream file(name, fstream::in);
		RomInfo info;
		file.read(info.___, 16);
		info.filename = name;
		return info;
	}
};


class NESRom
{
private:

public:

	bool LoadFile(const string& name)
	{


	}

	NESRom()
	{

	}


};