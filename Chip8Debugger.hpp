#pragma once
#include "Chip8.hpp"

class Chip8Debugger
{
public:
	static void analyseCycle(const u16 instruction, const u16 pc, const u8 sp, const u16 i, const u8 v[0x10], const u16 nnn, const u8 n, const u8 x, const u8 y, const u8 kk)
	{
		// Print type of instruction.
		std::cout << "Instruction 0x" << std::hex << instruction << std::endl;

		// Print instruction-variables.
		std::cout << "nnn: 0x"<< std::hex << nnn << ", n: 0x"<< std::hex << (unsigned)n << ", x: 0x"<< std::hex << (unsigned)x << ", y: 0x"<< std::hex << (unsigned)y << ", kk: 0x"<< std::hex << (unsigned)kk << std::endl;

		// Print program-counter, stack-pointer and I.
		std::cout << "Register: \npc: 0x"<< std::hex << pc << ", sp: 0x"<< std::hex << (unsigned)sp << ", i: 0x"<< std::hex << i << std::endl;

		// Print the content of all V-registers.
		std::cout << "V: ";
		for(unsigned j=0; j<0x10; ++j) std::cout << (unsigned)v[j] << ", ";
		std::cout << std::endl;
		
		// Wait for 'return' to continue.
		std::cin.get();
	}
};

