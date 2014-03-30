#include "VMemory.hpp"
#include "CPU.hpp"
#include "NES.hpp"
#include "Renderer.hpp"
#include "OpCodes.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

int main(int argc, const char* args[])
{
	using namespace std;
	bool Ok = true;
	try
	{
		Render::Initalize(800, 600);
	}
	catch (exception& e)
	{
		Ok = false;
		cout << "Could not initalize window : " << e.what() << endl;
	}

	NES Nemu;
	if (!Ok)
	{
		cin.get();
		return 1;
	}

	try
	{
		Nemu.mCart.LoadROM("../roms/dkong.nes");
		Nemu.mCPU.HardReset();
	}
	catch (exception& e)
	{
		Ok = false;
		cout << "Could not load NES : " << e.what() << endl;
	}

	if (!Ok)
	{
		cin.get();
		return 2;
	}

	while (Render::WindowOpen())
	{
		while (Render::WindowOpen())
		{
			Nemu.mCPU.Cycle();
			for (int i = 0; i < Nemu.mCPU.Instruction->Cycles * 3; i++)
			{
				Nemu.mPPU.Cycle();
			}
		}

		if (Render::FrameComplete)
			Render::BeginFrame();
		else
			Render::EndFrame();
	}

	//vector<uint16_t> image;
	//fstream imageFile("image.clr", fstream::in | fstream::binary);
	//imageFile.seekg(0, imageFile.end);
	//image.resize(imageFile.tellp() / 2);
	//imageFile.seekg(0, imageFile.beg);
	//imageFile.read((char*)image.data(), image.size() * 2);
	//imageFile.close();

	//while (Render::WindowOpen())
	//{
	//	Render::BeginFrame();

	//	for (int i = 0; i < 240; i++) //Render 240 scanlines
	//	{
	//		Render::BeginScanline(0);
	//		memcpy(Render::PixelOut, image.data() + i * 256, 256 * sizeof(Render::Pixel));
	//		Render::EndScanline();
	//	}

	//	Render::EndFrame();
	//}

	//Render::Terminate();


	return 0;
}