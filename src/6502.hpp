#pragma once
#ifndef _6502_H_PP
#define _6502_H_PP
#include <cstdint>
#include <vector>
#include <algorithm>
#include <exception>
#include "OpCodes.hpp"

class MemoryBus;

class MemoryRange
{
private:
	friend class MemoryBus;

	uint16_t m_Start;
	uint16_t m_End;

	MemoryBus* m_Bus;

public:
	MemoryRange(uint16_t start, uint16_t end)
		: m_Start(start), m_End(end)
	{

	}

	virtual ~MemoryRange()
	{

	}

	inline uint16_t Begin() const { return m_Start; }
	inline uint16_t End() const { return m_End; }
	inline uint16_t Size() const { return End() - Begin(); }

	inline bool Contains(uint16_t addr) const
	{
		return addr >= Begin() && addr <= End();
	}

	virtual uint8_t Read(uint16_t addr ) const = 0;
	virtual void Write(uint16_t addr, uint8_t val) = 0;
};

class MirroredMemoryRange
	: public MemoryRange
{
private:
	MemoryRange* m_Target;
	int16_t m_Delta;

public:
	MirroredMemoryRange(MemoryRange* target, uint16_t start, uint16_t end)
		: MemoryRange(start, end), m_Target(target), m_Delta(start - m_Target->Begin())
	{

	}

	uint8_t Read(uint16_t addr) const
	{
		return m_Target->Read(addr - m_Delta);
	}

	void Write(uint16_t addr, uint8_t val)
	{
		return m_Target->Write(addr - m_Delta, val);
	}
};

template<int BufferSize>
class BufferedMemoryRange
	: public MemoryRange
{
private:
	uint8_t m_Data[BufferSize];

public:
	BufferedMemoryRange(uint16_t start, uint16_t end)
		: MemoryRange(start, end)
	{

	}

	uint8_t Read(uint16_t addr) const
	{
		return m_Data[addr - Begin()];
	}

	void Write(uint16_t addr, uint8_t val)
	{
		m_Data[addr - Begin()] = val;
	}
};


class MemoryBus
{
private:
	uint16_t m_StartAddress;
	uint16_t m_EndAddress;
	std::vector<MemoryRange*> m_Ranges;

public:
	MemoryBus(uint16_t start, uint16_t end)
		: m_StartAddress(start), m_EndAddress(end)
	{

	}

	~MemoryBus()
	{
		for(auto& range : m_Ranges)
			delete range;
	}

	inline uint16_t Begin() { return m_StartAddress; }
	inline uint16_t End() { return m_EndAddress; }
	inline uint16_t Size() { return Begin() - End(); }

	void AddRange(MemoryRange* range)
	{
		range->m_Bus = this;
		m_Ranges.push_back(range);
	}

	MemoryRange* GetRange(uint16_t addr) const
	{

		for(auto& range : m_Ranges)
			if(range->Contains(addr))
				return range;

		return nullptr;
	}

	MemoryRange* RemoveRange(MemoryRange* range)
	{
		if(!range)
			return nullptr;

		std::remove(m_Ranges.begin(), m_Ranges.end(), range);
		return range;
	}

	void Write(uint16_t addr, uint8_t val)
	{
		for(auto& range : m_Ranges)
		{
			if(range->Contains(addr))
				return range->Write(addr, val);
		}

		throw std::runtime_error("Attempt to write outside of mapped memory range");
	}

	//Todo add large reads/writes This may not even be needed honestly. 
	uint8_t Read(uint16_t addr)
	{
		for(auto& range : m_Ranges)
		{
			if(range->Contains(addr))
				return range->Read(addr);
		}

		throw std::runtime_error("Attempt to read outside of mapped memory range");
	}
};

class CPUState
{
public:
	uint8_t SP;
	uint8_t IR;
	union
	{
		uint8_t Arg8[4];
		uint16_t Arg16[2];
	};
	uint16_t Out;
	uint16_t PC;
	uint8_t X;
	uint8_t Y;
	int8_t A;
	uint16_t EffectiveAddr;
	union 
	{
		bool CarryFlag : 1;
		bool ZeroFlag : 1;
		bool InterruptFlag : 1;
		bool DecimalFlag : 1;
		bool BreakFlag : 1;
		bool __UnusedFlag : 1;
		bool OverflowFlag : 1;
		bool SignFlag : 1;
		uint8_t Status;
	};
	bool HasAsserted;
};

class CPU
{
private:





public:
	CPUState State;
	MemoryBus Bus;





};


#endif
