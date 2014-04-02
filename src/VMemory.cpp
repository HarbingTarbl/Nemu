#include "VMemory.hpp"
#include "CPU.hpp"
#include "PPU.hpp"
#include "Cartridge.hpp"
#include "APU.hpp"

int VMemory::CartAddr(int addr)
{
	return addr;
}

int VMemory::CPUAddr(int addr)
{
	return addr & 0x7FF;
}

int VMemory::APUAddr(int addr)
{
	return addr & 0x1F;
}

int VMemory::PPUAddr(int addr)
{
	return addr & 0x7;
}

int VMemory::CHRAddr(int addr)
{
	return addr & 0x3FFF;
}

uint8_t VMemory::ReadPRG(int addr)
{
	if (addr == -1)
		return mCPU->Read(addr);

	if (addr > std::numeric_limits<uint16_t>::max())
		addr &= std::numeric_limits<uint16_t>::max();

	if (addr >= 0x4020) //Signal Cartridge?
		return mCart->ReadPRG(CartAddr(addr));

	if (addr >= 0x4000) //Signal APU/IO?
		return mAPU->Read(APUAddr(addr)); //(808, 828]

	if (addr >= 0x2000) //Signal PPU?
		return mPPU->ReadPRG(PPUAddr(addr)); //(800, 808]

	return mCPU->Read(CPUAddr(addr)); //(0, 800]
}

uint8_t VMemory::WritePRG(int addr, uint8_t value)
{
	if (addr == -1)
		return mCPU->Write(addr, value);

	if (addr > std::numeric_limits<uint16_t>::max())
		addr &= std::numeric_limits<uint16_t>::max();

	if (addr >= 0x4020) //Signal Cartridge?
		return mCart->WritePRG(CartAddr(addr), value);

	if (addr >= 0x4000) //Signal APU/IO?
		return mAPU->Write(APUAddr(addr), value); //(808, 828]

	if (addr >= 0x2000) //Signal PPU?
		return mPPU->WritePRG(PPUAddr(addr), value); //(800, 808]

	return mCPU->Write(CPUAddr(addr), value); //(0, 800]

}

uint8_t VMemory::WriteCHR(int addr, uint8_t value)
{
	addr = CHRAddr(addr);
	if ((addr & 0x2000) >> 13)
		return mPPU->WriteCHR(addr, value);
	else
		return mCart->WriteCHR(addr, value);
}

uint8_t VMemory::ReadCHR(int addr)
{
	addr = CHRAddr(addr);
	if ((addr & 0x2000) >> 13)
		return mPPU->ReadCHR(addr);
	else
		return mCart->ReadCHR(addr);
}


uint16_t VMemory::GetNMIV()
{
	return ReadPRG(0xFFFA) | (ReadPRG(0xFFFB) << 8);
}

uint16_t VMemory::GetRV()
{
	return ReadPRG(0xFFFC) | (ReadPRG(0xFFFD) << 8);
}

uint16_t VMemory::GetIRQ()
{
	return ReadPRG(0xFFFE) | (ReadPRG(0xFFFF) >> 8);
}

void VMemory::SetRV(uint16_t value)
{
	WritePRG(0xFFFD, (value & 0xFF00) >> 8);
	WritePRG(0xFFFC, (value & 0x00FF));
}


//Vectors!

VMemory::VMemory()
{
}