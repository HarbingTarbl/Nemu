#pragma once
#include <array>
#include <cstdint>
#include <bitset>

#include "CPU.hpp"	
#include "VMemory.hpp"
#include "Renderer.hpp"

class PPU
{
private:
	//Pattern Tables are on Cartridge
	//OAM Data is in RAM
	std::array<uint8_t, 0x08> mRegisters;
	std::array<uint8_t, 0x100> mPrimaryOAM;
	std::array<uint8_t, 0x20> mSecondaryOAM;
	std::array<uint8_t, 0x800> mNameRAM;
	std::array<uint8_t, 0x20> mPaletteRAM;

public:
	VMemory* Memory;

	enum
	{
		CONTROL_REG,
		MASK_REG,
		STATUS_REG,
		OAM_ADDR_REG,
		OAM_DATA_REG,
		SCROLL_REG,
		ADDR_REG,
		DATA_REG
	};

	PPU()
	{
		CurrentCycle = 0;
		CurrentLine = 0;
	}

	unsigned CurrentCycle;

	unsigned CurrentLine;
	unsigned LastLine;

	unsigned CurrentFrame;
	unsigned LastFrame;

	uint8_t LastRegisterWrite;

	uint16_t VRAMAddress;
	uint16_t VRAMAddressLatch;
	uint8_t VRAMIncAmount;
	uint8_t VRAMDataBuffer;
	bool VRAMLatched; //Address Latch Enable

	bool TransferLatch, TransferLatchScroll;

	int BackgroundAddr, BackgroundAddrTemp;
	int NametableAddr, NametableAddrTemp;
	int SpritePatternAddr, SpritePatternAddrTemp;
	int SpriteSize; //8x8 = 64, 8x16 = 128

	
	uint8_t FineScrollX, FineScrollY;
	uint16_t ScrollOrigin;
	uint8_t SpriteOnScanline;
	bool ScrollLatch;


	bool GenerateNMI;
	bool NMIGenerated;

	std::bitset<8> MaskBits;
	enum
	{
		MASK_COLOR,
		MASK_LEFT_BACKGROUND,
		MASK_LEFT_SPRITES,
		MASK_BACKGROUND,
		MASK_SPRITES,
		MASK_RED,
		MASK_GREEN,
		MASK_BLUE
	};

	enum
	{
		PPU_STATE_STARTUP,
		PPU_STATE_VINT,
		PPU_STATE_SCAN_PRE,
		PPU_STATE_SCAN_VISIBLE,
		PPU_STATE_SCAN_POST,
	};

	uint8_t ReadPRG(int addr);

	uint8_t WritePRG(int addr, uint8_t value);

	uint8_t ReadCHR(int addr);

	uint8_t WriteCHR(int addr, uint8_t value);

	void StartDMA(int addr);

	void ClearVBlank();

	int IsVBlanking();

	void SetVBlanking();

	uint8_t ReadSecondOAM(uint8_t addr);

	uint8_t WriteSecondOAM(uint8_t addr, uint8_t value);

	void SpriteEvaluation();


	void Reset();

	void RenderPixel(int color);

	void Cycle(unsigned nCycles);

	void BackgroundScanline();
	void SpriteScanline8(uint8_t);

};