#include "PPU.hpp"


void PPU::AllocateCycles(int nTicks)
{
	AllocatedCycles += nTicks;

}

uint8_t PPU::ReadPRG(int addr)
{
	auto r = mRegisters[addr];
	switch (addr)
	{
	case CONTROL_REG:
	case MASK_REG:
	case SCROLL_REG:
	case ADDR_REG:
		return LastRegisterWrite;
	case STATUS_REG:
		mRegisters[STATUS_REG] &= 0x7F;
		break;
	case OAM_ADDR_REG:
		break;
	case OAM_DATA_REG:
		if (CurrentFrame >= 20 && CurrentFrame < 26 && CurrentCycle < 64)
			return 0xFF;

		r = mPrimaryOAM[mRegisters[OAM_ADDR_REG]]; ///TODO, This is wrong, need to simulate the proper latching behavior.
		mRegisters[OAM_ADDR_REG] += IsVBlanking();
		return r;
	case DATA_REG:
		break;
	}
	return r;
}

uint8_t PPU::WritePRG(int addr, uint8_t value)
{
	LastRegisterWrite = value;
	switch (addr)
	{
	case CONTROL_REG:
		NametableAddr = ((value & 0x03) << 10) | 0x2000;
		AddrIncAmount = 1 << ((value & 0x04) << 3);
		SpritePatternAddr = ((value & 0x08) << 9);
		BackgroundAddr = ((value & 0x10) << 8);
		SpriteSize = 64 + ((value & 0x20) << 1);
		GenerateNMI = (value & 0x80) >> 7;
		return value;
	case MASK_REG:
		MaskBits = value;
		return value;
	case STATUS_REG:
		return value;
	case OAM_ADDR_REG:
		break;
	case OAM_DATA_REG:
		mPrimaryOAM[mRegisters[OAM_ADDR_REG]] = value;
		mRegisters[OAM_ADDR_REG]++;
		return value;
	case SCROLL_REG:
		Scroll[ScrollIndex] = value;
		ScrollIndex = !ScrollIndex;
		return value;
	case ADDR_REG:
		Addr = (value << AddrShift) | (Addr & (0xFF00 >> AddrShift));
		AddrShift = (AddrShift + 8) % 16;
		return value;
	case DATA_REG:
		break;
	}

	mRegisters[addr] = value;
	return value;
}

uint8_t PPU::ReadCHR(int addr)
{
	return mVRAM[addr & 0x1FFF];
}

uint8_t PPU::WriteCHR(int addr, uint8_t value)
{
	mVRAM[addr & 0x1FFF] = value;
	return value;
}

void PPU::ClearVBlank()
{
	mRegisters[STATUS_REG] &= 0x7F;
}

int PPU::IsVBlanking()
{
	return (mRegisters[STATUS_REG] & 0x80) >> 7;
}

void PPU::SetVBlanking()
{
	mRegisters[STATUS_REG] |= 0x80;
}

uint8_t PPU::ReadSecondOAM(uint8_t addr)
{
	if (CurrentCycle < 64)
		return 0xFF;
	else
		return mSecondaryOAM[addr];
}

uint8_t PPU::WriteSecondOAM(uint8_t addr, uint8_t value)
{
	mSecondaryOAM[addr] = value;
	return value;
}


void PPU::SpriteEvaluation()
{
	throw std::bad_exception();

	if (CurrentCycle < 64)
	{
		//zzzz
	}
	else if (CurrentCycle < 256)
	{
		int n = 0;


	}
}

void PPU::Step()
{
	throw std::bad_exception();

	while (AllocatedCycles > 0)
		Cycle(0);
}

void PPU::Reset()
{
	WritePRG(CONTROL_REG, 0x00);
	WritePRG(MASK_REG, 0x00);
	mRegisters[STATUS_REG] = 0xA0;
	WritePRG(OAM_ADDR_REG, 00);

	WritePRG(ADDR_REG, 00);
	WritePRG(ADDR_REG, 00);

	WritePRG(SCROLL_REG, 0x00);
	WritePRG(SCROLL_REG, 0x00);

	static uint8_t defaultPalette[] = {
		0x09, 0x01, 0x00, 0x01,
		0x00, 0x02, 0x02, 0x0D,
		0x08, 0x10, 0x08, 0x24,
		0x00, 0x00, 0x04, 0x2C,
		0x09, 0x01, 0x34, 0x03,
		0x00, 0x04, 0x00, 0x14,
		0x08, 0x3A, 0x00, 0x02,
		0x00, 0x20, 0x2C, 0x08,
	};

	memcpy(&mVRAM[0x3F00], defaultPalette, 0xFF);

	AllocatedCycles = 0;
	CurrentCycle = 0;
	CurrentLine = LastLine = 241;
	CurrentFrame = 0;
}

void PPU::RenderPixel(int color)
{

}


void PPU::BackgroundScanline()
{
	uint16_t ntAddr, ptAddr, atAddr, tileAddr, attrAddr;
	uint8_t attrByte, tileIndex, groupIndex, paletteIndex;
	uint8_t patternLow, patternHigh, paletteHigh;
	uint8_t tileX, tileY, tileScrollX, tileScrollY;


	ptAddr = BackgroundAddr;
	for (int i = 0; i < 32; i++)
	{
		ntAddr = NametableAddr;
		atAddr = ntAddr + 0x3C0;
		tileX = Addr & 0x1F;
		tileY = (Addr >> 5) & 0x1F;
		tileScrollX = (Addr >> 12) & 0x07;
		tileScrollY = Scroll[0]; //Fine scroll X
		tileAddr = ntAddr | (Addr & 0x03FF);



		for (int p = 0; p < 8; p++)
		{
			tileIndex = Memory->ReadCHR(tileAddr);
			patternLow = Memory->ReadCHR(ptAddr + (tileIndex << 4) + tileScrollY);
			patternHigh = Memory->ReadCHR(ptAddr + (tileIndex << 4) + tileScrollY + 8);

			attrAddr = atAddr | (((((tileY * 8) + tileScrollY) / 32) * 8) + (((tileX * 8) + tileScrollX) / 32));
			attrByte = Memory->ReadCHR(attrAddr);
			groupIndex = (((tileX % 4) & 0x02) >> 1) + ((tileY % 4) & 0x02);
			paletteHigh = ((attrByte >> (groupIndex << 1)) & 0x3) << 2;

			paletteIndex = paletteHigh;
			paletteIndex |= patternLow & (0x80 >> tileScrollX) ? 0x1 : 0;
			paletteIndex |= patternHigh & (0x80 >> tileScrollX) ? 0x1 : 0;

			if (paletteIndex & 0x03 == 0)
			{
				//Handle transparent
				Render::PixelOut->Color = 0;
				Render::PixelOut++;
			}
			else
			{
				Render::PixelOut->Color = Memory->ReadPRG(Memory->mCPU->PC); ///HACK BLARRRG
				Render::PixelOut++;
			}

			tileScrollX++;

			if (tileScrollX >= 8)
			{
				tileScrollX = 0;
				tileAddr++;
				tileX++;

				if (tileAddr & 0x1F == 0)
				{
					tileAddr++;
					tileX--;
					tileAddr %= ~0x001F;
					tileAddr ^= 0x0400;
				}
			}
		}

		if (Addr & 0x001F == 31)
		{
			Addr %= ~0x001F;
			Addr ^= 0x0400;
		}
		else
		{
			Addr++;
		}

		if (Addr & 0x7000 != 0x7000)
		{
			Addr += 0x1000;
		}
		else
		{
			Addr &= 0x0FFF;
			uint16_t y = (Addr & 0x3E0) >> 5;
			if (y == 29)
			{
				y = 0;
				Addr ^= 0x0800;
			}
			else if (y == 31)
			{
				y = 0;
			}
			else
			{
				y++;
			}

			Addr = (Addr & ~0x03E0) | (y << 5);
		}
	}

}

void PPU::StartDMA(int addr)
{
	int spriteAddr = mRegisters[OAM_ADDR_REG];
	for (int i = 0; i < 256; i++)
	{
		mPrimaryOAM[(spriteAddr + i) & 0xFF] = Memory->ReadPRG(addr * 0x100 + i);
	}

	TotalCycles += 512 * 3;
}

void PPU::Cycle(unsigned nCycles)
{
	uint16_t cc = nCycles * 3;
	CurrentCycle += cc;

	if (CurrentCycle >= 341)
	{
		CurrentLine += CurrentCycle / 341;
		CurrentCycle = CurrentCycle % 341;
	}

	if (CurrentLine < 240)
	{
		if (CurrentLine != LastLine)
		{
			LastLine = CurrentLine;
			Render::BeginScanline(0); //Also ends the last scanline which is cool.
			//Sprite Scanline
			BackgroundScanline();
			//Sprite Scanline

			if (MaskBits[MASK_BACKGROUND] || MaskBits[MASK_SPRITES])
			{
				//Addr = (Addr & (~0x1F & ~(1 << 10)) | ())
			}
			Render::EndScanline();
		}
	}
	else if (CurrentLine == 240)
	{
		Render::EndFrame();
		if (!IsVBlanking())
		{
			SetVBlanking();
		}
	}
	else if (CurrentLine > 261)
	{
		mRegisters[STATUS_REG] &= ~0x40;
		mRegisters[STATUS_REG] &= ~0x20;

		///Todo latches?

		CurrentLine = LastLine = 0;
		NMIGenerated = false;
	}
	else if (CurrentLine > 259)
	{
		if (IsVBlanking())
		{
			ClearVBlank();
		}
	}

	if (IsVBlanking() &&
		!NMIGenerated &&
		(mRegisters[CONTROL_REG] & 0x80) &&
		(mRegisters[STATUS_REG] & 0x80))
	{
		NMIGenerated = true;
		Memory->mCPU->InterruptQueue.push_front(CPU::INTERRUPT_NMI);
	}
}
