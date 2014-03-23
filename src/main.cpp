#include "VMemory.hpp"
#include "CPU.hpp"
#include "NES.hpp"
#include <iostream>

int main(int argc, const char* args[])
{
	using namespace std;

	NES Nemu;

	Nemu.mCart.LoadROM("nestest.nes");
	Nemu.mMemory.SetRV(0xC000);
	Nemu.mCPU.HardReset();
	while(!Nemu.mCPU.Asserted)
		Nemu.mCPU.Cycle();

	return 0;
}