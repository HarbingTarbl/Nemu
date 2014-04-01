#pragma once

#include <cstdint>
#include <exception>
#include <memory>

#include "Rom.hpp"

class MMC
{
public:
	virtual uint8_t WritePRG(int addr, uint8_t value) = 0;
	virtual uint8_t ReadPRG(int addr) = 0;

	virtual uint8_t WriteCHR(int addr, uint8_t value) = 0;
	virtual uint8_t ReadCHR(int addr) = 0;

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
		info.file.read((char*)CHRROM.data(), 0x2000);
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

	uint8_t WriteCHR(int addr, uint8_t value)
	{
		return CHRROM[addr] = value;
	}

	uint8_t ReadCHR(int addr)
	{
		return CHRROM[addr];
	}
};

class MMC0256
	: public MMC
{
public:
	std::array<uint8_t, 0x2000> PRGRAM;
	std::array<uint8_t, 0x8000> PRGROM;
	std::array<uint8_t, 0x2000> CHRROM;


	MMC0256(RomInfo& info)
	{
		info.file.read((char*)PRGROM.data(), 0x8000);
		info.file.read((char*)CHRROM.data(), 0x2000);
	}

	uint8_t WritePRG(int addr, uint8_t value)
	{
		if(addr >= 0x8000)
			return PRGROM[addr & 0xBFDF] = value;

		return PRGRAM[addr & 0x1FFF] = value;
	}

	uint8_t ReadPRG(int addr)
	{
		if(addr >= 0x8000)
			return PRGROM[addr & 0xBFDF];
		
		return PRGRAM[addr & 0x1FFF];
	}

	uint8_t WriteCHR(int addr, uint8_t value)
	{
		return CHRROM[addr] = value;
	}

	uint8_t ReadCHR(int addr)
	{
		return CHRROM[addr];
	}
};

class Cartridge
{
public:
	std::unique_ptr<MMC> CurrentMMC;
	std::string Name;

	uint8_t ReadPRG(int addr)
	{
		return CurrentMMC->ReadPRG(addr);
	}

	uint8_t WritePRG(int addr, uint8_t value)
	{
		return CurrentMMC->WritePRG(addr, value);
	}

	uint8_t ReadCHR(int addr)
	{
		return CurrentMMC->ReadCHR(addr);
	}

	uint8_t WriteCHR(int addr, uint8_t value)
	{
		return CurrentMMC->WriteCHR(addr, value);
	}

	void LoadROM(const string& filename)
	{
		RomInfo info = RomInfo::FromFile(filename);
		switch(info.MapperID())
		{
		case 0:
			if(info.NumPRG() == 1)
				CurrentMMC.reset(new MMC0128(info));
			else if(info.NumPRG() == 2)
				CurrentMMC.reset(new MMC0256(info));
			else
				throw std::runtime_error("Invalid MMC0 ROM");
			break;
		default:
			throw std::runtime_error("Unsupported mapper");
		}
		Name = filename;
	}
};

class MMC4
	: public MMC
{
	virtual uint8_t Read(int addr)
	{
		return addr;
	}

	virtual uint8_t Write(int addr, uint8_t value)
	{
		
		return value;
	}
};