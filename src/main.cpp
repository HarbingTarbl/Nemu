#include "VMemory.hpp"
#include "CPU.hpp"
#include "NES.hpp"
#include "Renderer.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

int main(int argc, const char* args[])
{
	using namespace std;

	//NES Nemu;

	//Nemu.mCart.LoadROM("nestest.nes");
	//Nemu.mMemory.SetRV(0xC000);
	//Nemu.mCPU.HardReset();
	//while(!Nemu.mCPU.Asserted)
	//	Nemu.mCPU.Cycle();

	PatternTile tile;
	uint8_t t[] = 
	{ 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x3C,
		0x3C, 0x7E, 0x7E, 0xFF, 0xFF, 0xFF, 0x42, 0x00
	};

	std::copy_n(t, sizeof(t), tile.Tile.begin());

	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			cout << (int)tile.PaletteIndex(j, i) << " ";
		}
		cout << endl;
	}

	Render::Initalize();
	std::this_thread::sleep_for(std::chrono::seconds(10));
	Render::Terminate();


	return 0;
}