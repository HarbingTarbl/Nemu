#include "VMemory.hpp"
#include "6502.hpp"
#include <iostream>

int main(int argc, const char* args[])
{
	using namespace std;
	

	VMemory TestMemory;
	FILE* f = fopen("nestest.nes", "r");
	fseek(f, 16, SEEK_SET);
	fread(&TestMemory[0xC000], 0x4000, 1, f);
	fclose(f);

	TestMemory[0xBF80] = 0x20;
	TestMemory[0xBF81] = 0x00;
	TestMemory[0xBF82] = 0xC0;

	TestMemory[0xBF83] = 0x4C;
	TestMemory[0xBF84] = 0x80;
	TestMemory[0xBF85] = 0xBF;

	TestMemory.RV = 0xC000;
	
	
	CPU TestCPU(TestMemory);

	while(!TestCPU.Asserted)
	{
		TestCPU.Cycle();
	}


	return 0;
}