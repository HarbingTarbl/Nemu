#include "VMemory.hpp"
#include "CPU.hpp"
#include <iostream>

int main(int argc, const char* args[])
{
	using namespace std;
	

	VMemory Mem;
	Cartridge Cart;
	CPU Cpu;
	APU Apu;
	PPU Ppu;

	Mem.mAPU = &Apu;
	Mem.mCPU = &Cpu;
	Mem.mCart = &Cart;
	Mem.mPPU = &Ppu;

	Cpu.Memory = &Mem;

	Cart.LoadROM("nestest.nes");

	Mem.Write(0xBF80, 0x20);
	Mem.Write(0xBF81, 0x00);
	Mem.Write(0xBF82, 0xC0);

	Mem.Write(0xBF83, 0x4C);
	Mem.Write(0xBF84, 0x80);
	Mem.Write(0xBF85, 0xBF);

	Mem.SetRV(0xC000);
	Cpu.HardReset();
	

	while(!Cpu.Asserted)
	{
		Cpu.Cycle();
	}


	return 0;
}