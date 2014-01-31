#include "OpCodes.hpp"
#include "6502.hpp"

#include <iostream>

int main(int argc, const char* args[])
{
	using namespace std;
	MemoryBus bus(0x0, 0xFF);
	bus.AddRange(new BufferedMemoryRange<0xFF>(0x0, 0xFF));



	try
	{
		bus.Write(0x54, 0xFF);
		cout << (int)bus.Read(0xF4) << endl;
	}
	catch(exception& e)
	{
		cout << "ERR:\t" << e.what() << endl;
	}


	return 0;
}