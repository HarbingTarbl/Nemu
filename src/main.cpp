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
	TestMemory.RV = 0xC000;
	
	CPU TestCPU(TestMemory);

	while(!TestCPU.Asserted)
		TestCPU.Cycle();


	return 0;
}