#include "VMemory.hpp"
#include "6502.hpp"
#include <iostream>

int main(int argc, const char* args[])
{
	using namespace std;
	

	VMemory TestMemory;
	TestMemory[0] = 30;
	TestMemory[1] = 25;
	TestMemory[2] = -5;

	int addr = VMemory::VMemRomStart;
	TestMemory.RV = addr;

	TestMemory[addr++] = 0xAD; 
	TestMemory[addr++] = 0x00;
	TestMemory[addr++] = 0x00; //LDA Absolute@0x0000

	TestMemory[addr++] = 0x6D;
	TestMemory[addr++] = 0x01;
	TestMemory[addr++] = 0x00; //ADC Absolute@0x0001

	TestMemory[addr++] = 0x6D;
	TestMemory[addr++] = 0x02;
	TestMemory[addr++] = 0x00; //ADC Absolute@0x0002

	TestMemory[addr++] = 0x8D;
	TestMemory[addr++] = 0x00;
	TestMemory[addr++] = 0x00; //STA Absolute@0x0000

	TestMemory[addr++] = 0x90;
	TestMemory[addr++] = -14; //BCC Relative@VMemRomStart

	TestMemory[addr++] = 0xA2;
	TestMemory[addr++] = 0x01; //LDX Immidiate@0x01

	CPU TestCPU(TestMemory);

	while(TestCPU.X == 0)
		TestCPU.Cycle();

	
	///This be the unit test to make sure the entire range is addressable.
	VMemory Mem1;
	VMemory Mem2;

	for(int i = 0; i <= 0xFFFF; i++)
	{
		Mem1[i] = Mem2[i] = 1;
	}

	for(int i = 0; i <= 0xFFFF; i++)
	{
		if(Mem1[i] == 0 || Mem2[i] == 0)
			cout << "OES NOES @ " << i << endl;
	}


	///This be the unit test that tests high/low byte, registers and addressing
	VMemory Memory;
	try
	{
		Memory.PC = (15) | (124 << 8);
		if(Memory[Memory.PCLow] != 15 || Memory[Memory.PCHigh] != 124)
			throw std::logic_error("High/Low PC Addressing");
		
		for(int i = 0; i < 0x800; i++)
		{
			Memory[i] = rand();
		}

		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 0x800; j++)
			{
				if(Memory[0x800 * i + j] != Memory[j])
				{
					cout << "RAM Memory Mirror Incorrect @ (" << hex << i * 0x800 + j << "," << j << ")" << endl;
					throw std::logic_error("RAM Mirror");
				}
			}

		}

		for(int i = 0x2000; i < 0x2008; i++)
		{
			Memory[i] = rand();
		}
		
		for(int i = 0; i < 1023; i++)
		{
			for(int j = 0x2000; j < 0x2008; j++)
			{
				if(Memory[0x8 * i + j] != Memory[j])
				{
					cout << "PPU Memory Mirror Incorrect @ (" << hex << i * 0x8 + j<< "," << j << ")" << endl;
					throw std::logic_error("PPU Mirror");
				}
			}
		}

		
	}
	catch(exception& e)
	{
		cout << "ERR:\t" << e.what() << endl;
	}


	return 0;
}