#pragma once
#include "VMemory.hpp"
#include <string>
#include <fstream>

using std::string;

struct RomHeader
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
	
	
	uint8_t MapperID() 
	{
		return (Flags6 & 0xF0) | (Flags7 & 0xF0) << 8;
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