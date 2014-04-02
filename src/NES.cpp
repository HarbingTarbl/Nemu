#include "NES.hpp"


NES* NES::Instance = nullptr;

NES::NES()
{
	mCPU.Memory = &mMemory;
	mAPU.mCPU = &mCPU;
	mMemory.mCPU = &mCPU;
	mMemory.mPPU = &mPPU;
	mMemory.mAPU = &mAPU;
	mMemory.mCart = &mCart;
	mPPU.Memory = &mMemory;
	Instance = this;
}