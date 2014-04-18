#include "PPU.hpp"

#include "CPU.hpp"	
#include "VMemory.hpp"
#include "Renderer.hpp"


uint8_t PPU::ReadPRG(int addr)
{
	static bool scrollLatch = false;
	uint8_t temp;
	switch (addr)
	{
	case CONTROL_REG:
	case MASK_REG:
	case SCROLL_REG:
	case ADDR_REG:
		return mRegisters[addr];
	case STATUS_REG:
		temp = mRegisters[STATUS_REG];
		temp = (0xF0 & temp) | (0x0F & LastRegisterWrite);
		ClearVBlank();
		LoopyW = false;
		return temp;
	case OAM_ADDR_REG:
		return mRegisters[addr];
	case OAM_DATA_REG:
		if (IsVBlanking())
			return mPrimaryOAM[mRegisters[OAM_ADDR_REG]]; ///TODO, This is wrong, need to simulate the proper latching behavior.
		else
			return mPrimaryOAM[mRegisters[OAM_ADDR_REG]++];
	case DATA_REG:
		if (LoopyV >= 0 && LoopyV < 0x3F00)
		{
			temp = VRAMDataBuffer;
			VRAMDataBuffer = ReadCHR(LoopyV);
		}
		else
		{
			VRAMDataBuffer = ReadCHR(LoopyV & ~0x1000);
			temp = ReadCHR(LoopyV);
		}

		LoopyV += VRAMIncAmount;
		VRAMAddress += VRAMIncAmount;
		return temp;
	default:
		throw std::bad_exception();
	}
}

uint8_t PPU::WritePRG(int addr, uint8_t value)
{
	LastRegisterWrite = value;
	switch (addr)
	{
	case CONTROL_REG:
		LoopyT = (LoopyT & ~(0x03 << 10)) | ((value & 0x03) << 10);
		//^^^ You prob didn't know that happened, the docs sure didn't
		NametableAddr = ((value & 0x03) << 10) | 0x2000;
		VRAMIncAmount = ((value & 0x04) ? 0x20 : 0x01);
		SpritePatternAddr = ((value & 0x08) << 9);
		BackgroundAddr = ((value & 0x10) << 8);
		SpriteSize = 64 + ((value & 0x20) << 1);
		GenerateNMI = (value & 0x80) >> 7;
		mRegisters[CONTROL_REG] = value;
		using std::cout;
		using std::endl;
		//printf("CONTROL WRITE\n\tNameTable Address %X\n\tVRAM Increment %hhX\n\tSprite Address %X\n\tBackground Address %X\n\tSprite Size %d\n\n",
		//	NametableAddr,
		//	VRAMIncAmount,
		//	SpritePatternAddr,
		//	BackgroundAddr,
		//	SpriteSize);
		return value;
	case MASK_REG:
		MaskBits = value;
		return value;
	case STATUS_REG:
		return value;
	case OAM_ADDR_REG:
		mRegisters[OAM_ADDR_REG] = value;
		return value;
	case OAM_DATA_REG:
		mPrimaryOAM[mRegisters[OAM_ADDR_REG]] = value; ///Todo place in individual variables
		mRegisters[OAM_ADDR_REG]++;
		return value;
	case SCROLL_REG:
		if (!LoopyW)
		{
			LoopyT &= ~0x1F;
			LoopyT |= (value >> 3);
			LoopyX = value & 0x07;

			//FineScrollX = value;
		}
		else
		{
			LoopyT &= 0xC1F;
			LoopyT |= (value & 0x03) << 12;
			LoopyT |= (value & 0xF8) << 2;
		}
		LoopyW = !LoopyW;
		return value;
	case ADDR_REG:
		if (!LoopyW)
		{
			LoopyT &= 0x00FF;
			LoopyT |= (value & 0x3F) << 8;
			VRAMAddress = value;
		}
		else
		{
			LoopyT &= 0x7F00;
			LoopyT |= value;
			LoopyV = LoopyT;
			VRAMAddress = (VRAMAddress << 8) | value;
		}
		LoopyW = !LoopyW;
		return value;
	case DATA_REG:
		WriteCHR(LoopyV, value);
		LoopyV += VRAMIncAmount;
		VRAMAddress += VRAMIncAmount;
		return value;
		break;
	default:
		throw std::bad_exception();
	}
}

uint8_t PPU::ReadCHR(int addr)
{
	if (addr >= 0x3F00)
	{
		addr &= 0x1F;
		switch (addr)
		{
		case 0x10:
			addr = 0x00;
			break;
		case 0x14:
			addr = 0x04;
			break;
		case 0x18:
			addr = 0x08;
			break;
		case 0x1C:
			addr = 0x0C;
			break;
		}

		return mPaletteRAM[addr];
	}
	else if (addr >= 0x3000)
	{
		addr &= 0x0EFF;
	}

	switch (Memory->mCart->VMirroring)
	{
	case Mirroring::Vertical:
		if (addr >= 0x2C00 && addr <= 0x2FFF) //Todo nametable mirroring
		{
			//Name Table #3
			return mNameRAM[addr & 0x7FF];
		}
		else if (addr >= 0x2800 && addr <= 0x2BFF)
		{
			//Name Table #2
			return mNameRAM[addr & 0x3FF];
		}
		else if (addr >= 0x2400 && addr <= 0x27FF)
		{
			//Name Table #1
			return mNameRAM[addr & 0x7FF];
		}
		else if (addr >= 0x2000)
		{
			//Name Table #0
			return mNameRAM[addr & 0x3FF];
		}
		else
		{
			//Not in RAM
			return Memory->ReadCHR(addr);
		}
		break;
	case Mirroring::Horizontal:
		if (addr >= 0x2C00 && addr <= 0x2FFF)
		{
			return mNameRAM[addr & 0x7FF];
		}
		else if (addr >= 0x2800 && addr <= 0x2BFF)
		{
			return mNameRAM[(addr - 0x400) & 0x7FF];
		}
		else if (addr >= 0x2400 && addr <= 0x7FF)
		{
			return mNameRAM[addr & 0x3FF];
		}
		else if (addr >= 0x2000)
		{
			return mNameRAM[addr & 0x3FF];
		}
		else
		{
			return Memory->ReadCHR(addr);
		}
		break;
	}

}

uint8_t PPU::WriteCHR(int addr, uint8_t value)
{
	if (addr >= 0x3F00)
	{
		//printf("Color @ %X = %hhX\n", addr, value);
		addr &= 0x1F;
		switch (addr)
		{
		case 0x10:
			addr = 0x00;
			break;
		case 0x14:
			addr = 0x04;
			break;
		case 0x18:
			addr = 0x08;
			break;
		case 0x1C:
			addr = 0x0C;
			break;
		}

		switch (addr)
		{
		case 0x00:
			Render::SetClearColor(value);
			break;
		}
		mPaletteRAM[addr] = value;
		return  value;
	}
	else if (addr >= 0x3000)
	{
		addr &= 0x0EFF;
	}

	switch (Memory->mCart->VMirroring)
	{
	case Mirroring::Vertical:
		if (addr >= 0x2C00 && addr <= 0x2FFF) //Todo nametable mirroring
		{
			//Name Table #3
			mNameRAM[addr & 0x7FF] = value;
		}
		else if (addr >= 0x2800 && addr <= 0x2BFF)
		{
			//Name Table #2
			mNameRAM[addr & 0x3FF] = value;
		}
		else if (addr >= 0x2400 && addr <= 0x27FF)
		{
			//Name Table #1
			mNameRAM[addr & 0x7FF] = value;
		}
		else if (addr >= 0x2000)
		{
			//Name Table #0
			mNameRAM[addr & 0x3FF] = value;
		}
		else
		{
			//Not in RAM
			return Memory->WriteCHR(addr, value);
		}
		break;
	case Mirroring::Horizontal:
		if (addr >= 0x2C00 && addr <= 0x2FFF)
		{
			mNameRAM[addr & 0x7FF] = value;
		}
		else if (addr >= 0x2800 && addr <= 0x2BFF)
		{
			mNameRAM[(addr - 0x400) & 0x7FF] = value;
		}
		else if (addr >= 0x2400 && addr <= 0x7FF)
		{
			mNameRAM[addr & 0x3FF] = value;
		}
		else if (addr >= 0x2000)
		{
			mNameRAM[addr & 0x3FF] = value;
		}
		else
		{
			return Memory->WriteCHR(addr, value);
		}
		break;
	}

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


void PPU::Reset()
{
	std::fill(mRegisters.begin(), mRegisters.end(), 0);


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


	for (int i = 0; i < 0x20; i++)
	{
		WriteCHR(0x3F00 + i, defaultPalette[i]);
	}


	CurrentCycle = 0;
	CurrentLine = LastLine = 241;
	CurrentFrame = 0;
	VRAMAddress = 0;
	NMIGenerated = false;
	WaitVBlank = false;
	VRAMDataBuffer = 0;
	LoopyV = LoopyT = LoopyX = LoopyW = 0;

}

void PPU::RenderPixel(int color)
{

}


void PPU::DumpVRAM()
{
	using namespace std;
	FILE* f;
	if (!fopen_s(&f, "VRAM-Dump.log", "w"))
	{
		for (int i = 0; i < 0x4000; i++)
		{
			uint8_t byte = ReadCHR(i);
			fwrite(&byte, 1, 1, f);
		}

		fclose(f);
	}
}


static bool raiseZeroHit = false;
void PPU::SpriteScanline8(uint8_t priority)
{
	uint16_t ptAddr;
	uint8_t patternLow, patternHigh, paletteHigh, paletteIndex;
	uint8_t inRange, spriteX, spriteY, tileIndex, attribute;
	SpriteOnScanline = 0;

	if (CurrentLine < 8)
		return;

	for (int i = 252; i >= 0; i -= 4)
	{

		spriteY = mPrimaryOAM[i] + 1;
		tileIndex = mPrimaryOAM[i + 1];
		attribute = mPrimaryOAM[i + 2];
		spriteX = mPrimaryOAM[i + 3];

		if (spriteY >= 0xFE)
			continue;

		if (mPrimaryOAM[i] == 239)
		{
			//mRegisters[STATUS_REG] |= 0x20; //Overflow
		}
		else if (mPrimaryOAM[i] == 255)
		{
			mRegisters[STATUS_REG] &= ~0x20;
		}

		if (priority != (attribute & 0x20))
		{
			continue;
		}

		

		inRange = CurrentLine - spriteY;
		if (inRange < 8) ///TODO this is prob 900% wrong
		{
			if (SpriteOnScanline++ >= 8)
			{
				mRegisters[STATUS_REG] |= 0x20;
			}

			if (attribute & 0x80) //Vertical Flip
			{
				inRange ^= 0x07;
			}

			patternLow = ReadCHR(SpritePatternAddr + (tileIndex << 4) + inRange);
			patternHigh = ReadCHR(SpritePatternAddr + (tileIndex << 4) + inRange + 8);
			paletteHigh = (attribute & 0x03) << 2;
			for (int p = 0; p < 8; p++)
			{
				paletteIndex = paletteHigh;

				if (attribute & 0x40) //Horzontal Flip
				{
					paletteIndex |= (patternLow & (1 << p)) ? 1 : 0;
					paletteIndex |= (patternHigh & (1 << p)) ? 2 : 0;
				}
				else
				{
					paletteIndex |= (patternLow & (0x80 >> p)) ? 1 : 0;
					paletteIndex |= (patternHigh & (0x80 >> p)) ? 2 : 0;
				}

				if ((paletteIndex & 0x3) == 0)
				{
					//Transparent
					
				}
				else
				{

					if ((i == 0 &&
						MaskBits[MASK_BACKGROUND] &&
						MaskBits[MASK_SPRITES]))
					{
						if (Render::BasePixel[spriteX + p + CurrentLine * 256].Color != 0xFFFF)
						{
							raiseZeroHit = true;
							//Set Zero Hit
						}
					}

					Render::BasePixel[spriteX + p + CurrentLine * 256].Color = ReadCHR(0x3F10 + paletteIndex);
				}
			}
		}
	}


}


void PPU::BackgroundScanline()
{
	uint16_t tileAddr = 0, attrAddr = 0, oldAttrAddr = 0;
	uint8_t attrByte = 0, tileIndex = 0, groupIndex = 0, paletteIndex = 0;
	uint8_t paletteHigh = 0;
	Render::CurrentScanline = CurrentLine;
	uint16_t patternLow, patternHigh;

	uint16_t bgaddr = BackgroundAddr + CurrentLine % 8;

	tileAddr = 0x2000 | (LoopyV & 0x0FFF);
	attrAddr = 0x23C0 | (LoopyV & 0x0C00) | ((LoopyV >> 4) & 0x38) | ((LoopyV >> 2) & 0x07);

	uint8_t tileX = tileAddr & 0x1F;
	uint8_t tileY = (tileAddr >> 5) & 0x1F;

	tileIndex = ReadCHR(tileAddr);

	if (((CurrentLine / 16) % 2) == 0)
		attrByte = ReadCHR(attrAddr) & 0x0F;
	else
		attrByte = ReadCHR(attrAddr) >> 4;

	oldAttrAddr = attrAddr;

	patternLow = ReadCHR(bgaddr + tileIndex * 16) << 8;
	patternHigh = ReadCHR(bgaddr + tileIndex * 16 + 8) << 8;

	for (int tile = 0; tile < 32; tile++)
	{
		if ((LoopyV & 0x001F) == 31)
		{
			LoopyV &= ~0x001F;
			LoopyV ^= 0x0400;
		}
		else
		{
			LoopyV++;
		}

		tileAddr = 0x2000 | (LoopyV & 0x0FFF);
		attrAddr = 0x23C0 | (LoopyV & 0x0C00) | ((LoopyV >> 4) & 0x38) | ((LoopyV >> 2) & 0x07);

		tileIndex = ReadCHR(tileAddr);

		if (oldAttrAddr != attrAddr)//Prefetch NEXT tile
		{

			oldAttrAddr = attrAddr;
			if (((CurrentLine / 16) % 2) == 0)
				attrByte |= (ReadCHR(attrAddr) << 4); 
			else
				attrByte |= (ReadCHR(attrAddr) & 0xF0);
		}

		patternLow |= ReadCHR(bgaddr + tileIndex * 16);
		patternHigh |= ReadCHR(bgaddr + tileIndex * 16 + 8);
		for (int pixel = 0; pixel < 8; pixel++)
		{
			uint8_t group = ((((tileX % 4) + ((LoopyX + pixel) / 8)) >> 1) << 1);
			
			paletteIndex = (((attrByte >> group) & 0x3) << 2);

			paletteIndex |= (patternLow & (0x8000 >> (LoopyX))) ? 1 : 0;
			paletteIndex |= (patternHigh & (0x8000 >> (LoopyX))) ? 2 : 0;

			if ((paletteIndex & 0x03) == 0)
			{
				Render::PixelOut->Color = 0xFFFF;
			}
			else
			{
				Render::PixelOut->Color = ReadCHR(0x3F00 + paletteIndex);
			}

			Render::PixelOut++;

			patternLow <<= 1;
			patternHigh <<= 1;
		}

		tileX = tileAddr & 0x1F;
		tileY = (tileAddr >> 5) & 0x1F;

		if (tileX % 4 == 0)
			attrByte >>= 4;

	}



	CurrentLine = Render::CurrentScanline;
}

void PPU::StartDMA(int addr)
{
	int spriteAddr = mRegisters[OAM_ADDR_REG];
	for (int i = 0; i < 256; i++)
	{
		mPrimaryOAM[(spriteAddr + i) & 0xFF] = Memory->ReadPRG(addr * 0x100 + i);
	}

	for (int i = 0; i < 512; i++)
	{
		Cycle(1);
	}
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



	if (CurrentLine >= 0 && CurrentLine <= 239)
	{
		if (LoopyLatch && (MaskBits[MASK_BACKGROUND] || MaskBits[MASK_SPRITES]))
		{
			LoopyV &= 0x41F;
			LoopyV |= LoopyT & 0x7BE0;
			LoopyLatch = false;
		}

		if (CurrentLine != LastLine)
		{
			LastLine = CurrentLine;
			Render::CurrentScanline = CurrentLine;
			Render::BeginScanline(0); //Also ends the last scanline which is cool.

			if (MaskBits[MASK_SPRITES])
			{
				SpriteScanline8(0x20);
			}

			if (raiseZeroHit)
			{
				mRegisters[STATUS_REG] |= 0x40;
				raiseZeroHit = false;
			}

			if (MaskBits[MASK_BACKGROUND])
			{
				BackgroundScanline();
			}

			if (MaskBits[MASK_SPRITES])
			{
				SpriteScanline8(0x0);
			}

			if (CurrentLine == 239)
			{
				Render::CurrentScanline = -1;
				Render::EndScanline();
			}


			if (MaskBits[MASK_BACKGROUND] || MaskBits[MASK_SPRITES])
			{
				//Inc Vertical
				if ((LoopyV & 0x7000) != 0x7000)
					LoopyV += 0x1000;
				else
				{
					LoopyV &= ~0x7000;
					int what = (LoopyV & 0x03E0) >> 5;
					if (what == 29)
					{
						what = 0;
						LoopyV ^= 0x0800;
					}
					else if (what == 31)
					{
						what = 0;
					}
					else
					{
						what++;
					}

					LoopyV = (LoopyV & ~0x03E0) | (what << 5);
				}

				//Copy Horizontal
				LoopyV &= 0x7BE0;
				LoopyV |= (LoopyT & 0x41F);
			}
		}
	}
	else if (CurrentLine == 240)
	{
		if (!WaitVBlank)
		{
			WaitVBlank = true;
			mRegisters[STATUS_REG] |= 0x80;
			LoopyLatch = true;
		}
	}
	else if (CurrentLine > 261)
	{
		mRegisters[STATUS_REG] &= ~0x40;
		mRegisters[STATUS_REG] &= ~0x20;

		///Todo latches?
		mRegisters[STATUS_REG] &= ~0x80;
		CurrentLine = LastLine = -1;
		NMIGenerated = false;
		LoopyW = false;
	}
	else if (CurrentLine > 259)
	{
		if (WaitVBlank)
		{
			WaitVBlank = false;
			mRegisters[STATUS_REG] &= ~0x80;
		}
	}

	if (WaitVBlank &&
		!NMIGenerated &&
		(mRegisters[CONTROL_REG] & 0x80) &&
		(mRegisters[STATUS_REG] & 0x80))
	{
		NMIGenerated = true;
		Memory->mCPU->InterruptQueue.push_front(CPU::INTERRUPT_NMI);
	}
}
