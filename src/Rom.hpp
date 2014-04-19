#pragma once

#ifndef _ROM_H
#define _ROM_H

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

enum class Mirroring
{
	Horizontal,
	Vertical
};


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
	
private:
	RomInfo() {}

public:
	RomInfo(RomInfo&& other)
		: 
		filename(std::move(other.filename)),
		file(std::move(other.file))
	{
		std::copy_n(other.___, 16, ___);
	}


	string filename;
	std::unique_ptr<std::fstream> file;



	bool IsRom() const
	{
		return 
			NES[0] == 'N' &&
			NES[1] == 'E' &&
			NES[2] == 'S' &&
			NES[3] == 0x1A;
	}

	uint8_t NumPRG() const
	{
		return PRGPages;
	}

	uint8_t NumCHR() const
	{
		return CHRPages;
	}

	Mirroring GetMirroring() const ///HACK 4 Screen VRAM
	{
		return (Flags6 & 0x01) ? Mirroring::Vertical : Mirroring::Horizontal;
	}

	uint8_t MapperID() const
	{
		return ((Flags6 & 0xF0) >> 4) | (Flags7 & 0xF0); 
	}

	static RomInfo FromFile(const string& name)
	{
		using std::fstream;
		auto file = std::unique_ptr<std::fstream>(new std::fstream(name, fstream::in | fstream::binary));
		RomInfo info;
		file->read(info.___, 4);
		if(!info.IsRom())
			throw std::logic_error("Must load a ROM");
		else
			file->read(info.___ + 4, 12);

		info.filename = name;
		info.file = std::move(file);
		return std::move(info);
	}
};


#endif