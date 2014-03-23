#pragma once

#include <cstdint>
#include <memory>
#include "Rom.hpp"

class MMC
{
public:
	virtual uint8_t WritePRG(int addr, uint8_t value) = 0;
	virtual uint8_t ReadPRG(int addr) = 0;
	virtual ~MMC() {}
};

class MMC0128
	: public MMC
{
public:
	std::array<uint8_t, 0x2000> PRGRAM;
	std::array<uint8_t, 0x4000> PRGROM;
	std::array<uint8_t, 0x2000> CHRROM;


	MMC0128(RomInfo& info)
	{
		info.file.read((char*)PRGROM.data(), 0x4000);
	}

	uint8_t WritePRG(int addr, uint8_t value)
	{
		if(addr >= 0x8000)
			return PRGROM[addr & 0x3FFF] = value;

		return PRGRAM[addr & 0x1FFF] = value;
	}

	uint8_t ReadPRG(int addr)
	{
		if(addr >= 0x8000)
			return PRGROM[addr & 0x3FFF];
		
		return PRGRAM[addr & 0x1FFF];
	}

};

class MMC0256
{

};

class Cartridge
{
private:

public:
	std::unique_ptr<MMC> CurrentMMC;

	uint8_t ReadPRG(int addr)
	{
		return CurrentMMC->ReadPRG(addr);
	}

	uint8_t WritePRG(int addr, uint8_t value)
	{
		return CurrentMMC->WritePRG(addr, value);
	}

	void LoadROM(const string& filename)
	{
		RomInfo info = RomInfo::FromFile(filename);
		switch(info.MapperID())
		{
		case 0:
			if(info.NumPRG() == 1)
				CurrentMMC.reset(new MMC0128(info));
			break;
		}

	}

};