#include "OpCodes.hpp"
#include "6502.hpp"

#include <iostream>

int main(int argc, const char* args[])
{
	using namespace std;
	MemoryBus bus(0x0, 0x1FF);
	bus.AddRange(new BufferedMemoryRange<0x100>(0x100, 0x1FF));
	bus.AddRange(new MirroredMemoryRange(bus.GetRange(0x100), 0x000, 0x0FF));



	try
	{
		bus.Write(0x54, 0xFF);
		cout << (int)bus.Read(0x154) << endl;
	}
	catch(exception& e)
	{
		cout << "ERR:\t" << e.what() << endl;
	}


	return 0;
}