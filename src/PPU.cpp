#include "PPU.hpp"

uint8_t PPU::ReadPRG(int addr)
{
	uint8_t temp; ///TODO Does LRW work here? Or is it a different buffer?
	switch (addr)
	{
	case CONTROL_REG:	
	case MASK_REG:
	case SCROLL_REG:
	case ADDR_REG:
		return mRegisters[addr];
	case STATUS_REG:
		ClearVBlank();
		VRAMLatched = false; 
		return mRegisters[STATUS_REG];
	case OAM_ADDR_REG:
		return mRegisters[addr];
	case OAM_DATA_REG:
		return mPrimaryOAM[mRegisters[OAM_ADDR_REG]]; ///TODO, This is wrong, need to simulate the proper latching behavior.
	case DATA_REG:
		if (VRAMAddress >= 0 && VRAMAddress < 0x3F00)
		{
			temp = VRAMDataBuffer;
			VRAMDataBuffer = ReadCHR(VRAMAddress);
		}
		else
		{
			VRAMDataBuffer = ReadCHR(VRAMAddress & ~0x1000);
			temp = ReadCHR(VRAMAddress);
		}

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
		VRAMAddressLatch = (VRAMAddressLatch & (0x03 << 10)) | ((value & 3) << 10); 
		//^^^ You prob didn't know that happened, the docs sure didn't
		NametableAddr = ((value & 0x03) << 10) | 0x2000;
		VRAMIncAmount = 1 << ((value & 0x04) << 3);
		SpritePatternAddr = ((value & 0x08) << 9);
		BackgroundAddr = ((value & 0x10) << 8);
		SpriteSize = 64 + ((value & 0x20) << 1);
		GenerateNMI = (value & 0x80) >> 7;
		return value;
	case MASK_REG:
		MaskBits = value;
		//MaskBits.flip(MASK_BACKGROUND);
		//MaskBits.flip(MASK_SPRITES);
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
		if (!VRAMLatched)
		{
			ScrollOrigin = value << 8;
			FineScrollX = value & 0x07;
			VRAMAddressLatch = (VRAMAddressLatch & ~0x1F) | ((value >> 3) & 0x1F);
		}
		else
		{
			ScrollOrigin |= value;
			FineScrollY = value;
			VRAMAddressLatch = (VRAMAddressLatch & ~(0x1F << 5)) | (((value >> 3) & 0x1F) << 5);
			VRAMAddressLatch = (VRAMAddressLatch & ~(0x07 << 12)) | ((value & 0x07) << 12);
		}
		VRAMLatched = !VRAMLatched;
		return value;
	case ADDR_REG:
		if (!VRAMLatched)
		{
			VRAMAddress = value << 8;
			VRAMAddressLatch = (VRAMAddressLatch & 0xFF) | ((value & 0x3F) << 8);
		}
		else
		{
			VRAMAddressLatch = (VRAMAddressLatch & ~0xFF) | value;
			VRAMAddress = VRAMAddressLatch;
		}
		VRAMLatched = !VRAMLatched;
		return value;
	case DATA_REG:
		WriteCHR(VRAMAddress, value);
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

	if (addr >= 0x2C00) //Todo nametable mirroring
	{
		//Name Table #3
		return mNameRAM[addr & 0x7FF];
	}
	else if (addr >= 0x2800)
	{
		//Name Table #2
		return mNameRAM[addr & 0x3FF];
	}
	else if (addr >= 0x2400)
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
}

uint8_t PPU::WriteCHR(int addr, uint8_t value)
{
	if (addr >= 0x3F00)
	{
		addr &= 0x1F;
		mPaletteRAM[addr] = value;
		return  value;
	}
	else if (addr >= 0x3000)
	{
		addr &= 0x0EFF;
	}

	if (addr >= 0x2C00) //Todo nametable mirroring
	{
		//Name Table #3
		mNameRAM[addr & 0x7FF] = value;
	}
	else if (addr >= 0x2800)
	{
		//Name Table #2
		mNameRAM[addr & 0x3FF] = value;
	}
	else if (addr >= 0x2400)
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



void PPU::Reset()
{
	std::fill(mRegisters.begin(), mRegisters.end(), 0);

	//static uint8_t defaultPalette[] = {
	//	0x09, 0x01, 0x00, 0x01,
	//	0x00, 0x02, 0x02, 0x0D,
	//	0x08, 0x10, 0x08, 0x24,
	//	0x00, 0x00, 0x04, 0x2C,
	//	0x09, 0x01, 0x34, 0x03,
	//	0x00, 0x04, 0x00, 0x14,
	//	0x08, 0x3A, 0x00, 0x02,
	//	0x00, 0x20, 0x2C, 0x08,
	//};


	//for (int i = 0; i < 0x20; i++)
	//{
	//	WriteCHR(0x3F00 + i, defaultPalette[i]);
	//}

	
	CurrentCycle = 0;
	CurrentLine = LastLine = 241;
	CurrentFrame = 0;
	VRAMAddress = VRAMAddressLatch = 0;
	VRAMLatched = false;
	TransferLatch = TransferLatchScroll = false;
	NMIGenerated = false;


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
	Render::CurrentScanline = CurrentLine;

	if (mRegisters[CONTROL_REG] & 0x10)
	{
		ptAddr = 0x1000;
	}
	else
	{
		ptAddr = 0x0000;
	}

	for (int i = 0; i < 32; i++)
	{
		switch ((VRAMAddress >> 10) & 0x3)
		{
		case 0:
			ntAddr = 0x2000;
			break;
		case 1:
			ntAddr = 0x2400;
			break;
		case 2:
			ntAddr = 0x2800;
			break;
		case 3:
			ntAddr = 0x2C00;
			break;
		}

		atAddr = ntAddr + 0x3C0;
		tileX = VRAMAddress & 0x1F;
		tileY = (VRAMAddress >> 5) & 0x1F;
		tileScrollY = (VRAMAddress >> 12) & 0x07;
		tileScrollX = FineScrollX; //Fine scroll X
		tileAddr = ntAddr | (VRAMAddress & 0x03FF);



		for (int p = 0; p < 8; p++)
		{
			tileIndex = ReadCHR(tileAddr);
			patternLow = ReadCHR(ptAddr + (tileIndex << 4) + tileScrollY);
			patternHigh = ReadCHR(ptAddr + (tileIndex << 4) + tileScrollY + 8);

			attrAddr = atAddr | (((((tileY * 8) + tileScrollY) / 32) * 8) + (((tileX * 8) + tileScrollX) / 32));
			attrByte = ReadCHR(attrAddr);
			groupIndex = (((tileX % 4) & 0x02) >> 1) + ((tileY % 4) & 0x02);
			paletteHigh = ((attrByte >> (groupIndex << 1)) & 0x3) << 2;

			paletteIndex = paletteHigh;
			paletteIndex |= patternLow & (0x80 >> tileScrollX) ? 0x1 : 0;
			paletteIndex |= patternHigh & (0x80 >> tileScrollX) ? 0x2 : 0;

			if (paletteIndex & 0x03 == 0)
			{
				//Handle transparent
				Render::PixelOut->Color = 0x10;
				Render::PixelOut++;
			}
			else
			{
				Render::PixelOut->Color = ReadCHR(0x3F00 + paletteIndex); // | MaskBits[MASK_BLUE] << 6 | MaskBits[MASK_RED] << 7 | MaskBits[MASK_GREEN] << 8; ///HACK BLARRRG
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

		if (VRAMAddress & 0x001F == 31)
		{
			VRAMAddress %= ~0x001F;
			VRAMAddress ^= 0x0400;
		}
		else
		{
			VRAMAddress++;
		}

		if (VRAMAddress & 0x7000 != 0x7000)
		{
			VRAMAddress += 0x1000;
		}
		else
		{
			VRAMAddress &= 0x0FFF;
			uint16_t y = (VRAMAddress & 0x3E0) >> 5;
			if (y == 29)
			{
				y = 0;
				VRAMAddress ^= 0x0800;
			}
			else if (y == 31)
			{
				y = 0;
			}
			else
			{
				y++;
			}

			VRAMAddress = (VRAMAddress & ~0x03E0) | (y << 5);
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

	CurrentCycle += 512 * 3;

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

		if (!TransferLatch && (MaskBits[MASK_SPRITES] || MaskBits[MASK_BACKGROUND]))
		{
			VRAMAddress = VRAMAddressLatch;;
			TransferLatch = true;
		}

		if (CurrentLine != LastLine)
		{
			LastLine = CurrentLine;
			Render::BeginScanline(0); //Also ends the last scanline which is cool.
			//Sprite Scanline
			BackgroundScanline();
			//Sprite Scanline

			if (MaskBits[MASK_BACKGROUND] || MaskBits[MASK_SPRITES])
			{
				VRAMAddress = (VRAMAddress & (~0x1F & ~(1 << 10))) | (VRAMAddressLatch & (0x1F | (1 << 10)));
				///Todo mapper IRQ
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

		CurrentLine = LastLine = -1;
		TransferLatch = TransferLatchScroll = false;
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
