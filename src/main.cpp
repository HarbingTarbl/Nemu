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

	vector<uint16_t> image;
	fstream imageFile("image.clr", fstream::in | fstream::binary);
	imageFile.seekg(0, imageFile.end);
	image.resize(imageFile.tellp() / 2);
	imageFile.seekg(0, imageFile.beg);
	imageFile.read((char*)image.data(), image.size() * 2);
	imageFile.close();




	while (Render::WindowOpen())
	{
		Render::BeginFrame();

		for (int i = 0; i < 240; i++) //Render 240 scanlines
		{
			Render::BeginScanline(i * 341);
			//Render::BeginScanline(0);
			memcpy(Render::PixelOut, image.data() + i * 256, 256 * sizeof(Render::Pixel));
			Render::EndScanline();
		}

		Render::EndFrame();
	}

	Render::Terminate();


	return 0;
}