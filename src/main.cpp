#include "VirtualMemory.hpp"
#include <iostream>

int main(int argc, const char* args[])
{
	using namespace std;

	///This be the unit test to make sure the entire range is addressable.
	Bus::VMemory Mem1;
	Bus::VMemory Mem2;

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
	Bus::VMemory Memory;
	try
	{
		Memory.PC = (15) | (124 << 8);
		cout << (int)Memory[Memory.PCLow] << endl; //15
		cout << (int)Memory[Memory.PCHigh] << endl; //124

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