/*
*	MIT License
*
*	Copyright(c) 2018 Paul Bernitz
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files(the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions :
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/

#pragma once

#include "Chip8.hpp"

class Chip8Debugger
{
public:
	static void analyseCycle(const u16 instruction, const u16 pc, const u8 sp, const u16 i, const u8 v[0x10], const u16 nnn, const u8 n, const u8 x, const u8 y, const u8 kk)
	{
		// Print type of instruction.
		std::cout << "Instruction 0x" << std::hex << instruction << '\n';

		// Print instruction-variables.
		std::cout << "nnn: 0x"<< std::hex << nnn << ", n: 0x"<< std::hex << (unsigned)n << ", x: 0x"<< std::hex << (unsigned)x << ", y: 0x"<< std::hex << (unsigned)y << ", kk: 0x"<< std::hex << (unsigned)kk << '\n';

		// Print program-counter, stack-pointer and I.
		std::cout << "Register: \npc: 0x"<< std::hex << pc << ", sp: 0x"<< std::hex << (unsigned)sp << ", i: 0x"<< std::hex << i << '\n';

		// Print the content of all V-registers.
		std::cout << "V: ";
		for(unsigned j=0; j<0x10; ++j) std::cout << (unsigned)v[j] << ", ";
		std::cout << std::endl;
		
		// Wait for 'return' to continue.
		std::cin.get();
	}
};
