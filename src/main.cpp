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

	try
	{
		Render::Initalize(800, 600);
	}
	catch (exception& e)
	{
		cout << "Could not initalize window : " << e.what() << endl;
	}

	while (Render::WindowOpen())
	{
		Render::BeginFrame();

		for (int i = 0; i < 240; i++) //Render 240 scanlines
		{
			//Render::BeginScanline(i * 341 + std::rand() & 0x03);
			Render::BeginScanline(0);
			for (int k = 0; k < 256; k++) // 256 Pixels
			{
				Render::PixelOut[k].Color = k & 0x0F | ((k & 0x03) << 4);
			}
			Render::EndScanline();
		}

		Render::EndFrame();
	}

	Render::Terminate();


	return 0;
}