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

#include "Chip8.hpp"
#include "Chip8Debugger.hpp"

Chip8::Chip8()
{
	initialize();
}

void Chip8::initialize()
{
	// Seed for random.
	srand((unsigned)time(NULL));

	// Reset registers.
	_pc = 0;
	_sp = 0;
	_i = 0;
	_delayTimer = 0;
	_soundTimer = 0;
	for(unsigned j=0; j<MEMORY_SIZE; ++j) _memory[j] = 0;
	for(unsigned j=0; j<V_REGS_TOTAL; ++j) _v[j] = 0;
	for(unsigned j=0; j<STACK_SIZE; ++j) _stack[j] = 0;
	for(unsigned j=0; j<KEYPAD_SIZE; ++j) _keypad[j] = 0;
	for(unsigned j=0; j<(DISPLAY_W * DISPLAY_H); ++j) _display[j] = 0;
	_isRedraw = false;

	// Set program-counter to the start of most Chip-8 programs (0x200). 
	_pc = PROGRAM_START; 

	// Load fontset into memory (0x0 - 0x50).
	for(unsigned j=0; j<FONTSET_SIZE; ++j) _memory[j] = FONTSET[j];	

	// Clear the screen.
	clearScreen();
}

const bool Chip8::isRedraw()
{
	if(_isRedraw)
	{
		_isRedraw = false;
		return true;
	} 
	return false;
}

void Chip8::setKey(const unsigned index, const bool isPressed)
{
	_keypad[index] = (u8)isPressed;
}

const bool Chip8::getKey(const unsigned index) const
{
	return _keypad[index] != 0 ? true : false;
}

void Chip8::clearScreen()
{
	for(unsigned j=0; j<(DISPLAY_W * DISPLAY_H); ++j) _display[j] = 0;
	for(unsigned i=0; i<_pixels.size(); ++i)
	{
		_pixels[i].setSize(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
		_pixels[i].setPosition(float(i % DISPLAY_W * PIXEL_SIZE), float(i / DISPLAY_W * PIXEL_SIZE));
		_pixels[i].setFillColor(sf::Color::Black);
	}
	_isRedraw = true;
}

bool Chip8::loadRom(const std::string fileName)
{
	initialize();
	std::ifstream fileIn (fileName, std::ifstream::binary);

	if (!fileIn.is_open())
		return false;

	if(fileIn) 
	{
		// Get length of file.
		fileIn.seekg (0, fileIn.end);
		int length = (int)fileIn.tellg();
		fileIn.seekg (0, fileIn.beg);

		// Allocate memory.
		char* buffer = new char [length];

		// Read data as a block.
		fileIn.read (buffer,length);
		fileIn.close();

		// Copy buffer to chip8-memory.
		for(int i = 0; i < length; ++i)
			_memory[i + PROGRAM_START] = buffer[i];

		delete[] buffer;

#ifdef DEBUG
		std::cout << "Filesize: " << length << std::endl;
		for(unsigned j=0; j<length; ++j) std::cout << std::hex << (int)_memory[j + PROGRAM_START] << ", ";
#endif	
	}

	return true;
}

void Chip8::runCycle()
{
	updateTimers();
	executeInstruction();
}

void Chip8::updateTimers()
{
	// Subtract 1 from the value of DT at a rate of 60Hz. When DT reaches 0, it deactivates.
	_delayTimer -= (_delayTimer > 0 ? 1 : 0); 

	// Decrements at a rate of 60Hz, however, as long as ST's value is greater than zero, the Chip-8 buzzer will sound. When ST reaches zero, the sound timer deactivates.
	if(_soundTimer > 0)
	{
		// TODO: buzzer-sound here.
	}

	_soundTimer -= (_soundTimer > 0 ? 1 : 0);
}

void Chip8::executeInstruction()
{
	u16 instruction = _memory[_pc] << 8 | _memory[_pc + 1];	// Read instruction.

	u16 nnn = instruction & 0xFFF;		// nnn or addr - A 12-bit value, the lowest 12 bits of the instruction		----XXXXXXXXXXXX
	u8 n = instruction & 0xF;			// n or nibble - A 4-bit value, the lowest 4 bits of the instruction		------------XXXX
	u8 x = (instruction >> 8) & 0xF;	// x - A 4-bit value, the lower 4 bits of the high byte of the instruction	----XXXX--------
	u8 y = (instruction >> 4) & 0xF;	// y - A 4-bit value, the upper 4 bits of the low byte of the instruction	--------XXXX----
	u8 kk = instruction & 0xFF;			// kk or byte - An 8-bit value, the lowest 8 bits of the instruction		--------XXXXXXXX

	if(instruction == 0x0)
	{
#ifdef DEBUG
		std::cout << "Rom-file ended! Press 'return' to quit..." << std::endl;
		std::cin.get();
#endif
		exit(EXIT_SUCCESS);
	}

	switch(instruction & 0xF000)
	{
	case 0x0000:
		switch(instruction & 0xF)
		{
		case 0x0: // Clear the screen.
			clearScreen();
			_pc += 2;
			break;
		case 0xE: // Return from subroutine.
			--_sp;
			_pc = _stack[_sp];
			_pc += 2;
			break;
		default:
			std::cout << "Unknown instruction: " << instruction << std::endl;
		}
		break;
	case 0x1000: // Jump to location nnn.
		_pc = nnn;		
#ifdef ALTERNATIVE_REDRAW
		_isRedraw = true; // Moved here from DXYN (Draw) to prevent sprites in some ROMs from flickering.	
#endif
		break;
	case 0x2000: // Call subroutine at nnn.
		_stack[_sp] = _pc;
		++_sp;
		_pc = nnn;
		break;
	case 0x3000: // Skip next instruction if Vx = kk.
		_pc += (_v[x] == kk ? 4 : 2);
		break;
	case 0x4000: // Skip next instruction if Vx != kk.
		_pc += (_v[x] != kk ? 4 : 2);
		break;
	case 0x5000: // Skip next instruction if Vx = Vy.
		_pc += (_v[x] == _v[y] ? 4 : 2);
		break;
	case 0x6000: // Set Vx = kk.
		_v[x] = kk;
		_pc += 2;
		break;
	case 0x7000: // Set Vx = Vx + kk.
		_v[x] += kk;
		_pc += 2;
		break;
	case 0x8000: 
		switch(instruction & 0xF)
		{
		case 0x0: // Set Vx = Vy.
			_v[x] = _v[y];
			_pc += 2;
			break;
		case 0x1: // Set Vx = Vx OR Vy.
			_v[x] |= _v[y];
			_pc += 2;
			break;
		case 0x2: // Set Vx = Vx AND Vy.
			_v[x] &= _v[y];
			_pc += 2;
			break;
		case 0x3: // Set Vx = Vx XOR Vy.
			_v[x] ^= _v[y];
			_pc += 2;
			break;
		case 0x4: // Set Vx = Vx + Vy, set VF = carry.
			_v[0xF] = (_v[x] + _v[y] > 0xFF) ? 1 : 0;
			_v[x] = (_v[x] + _v[y]) & 0xFF;
			_pc += 2;
			break;
		case 0x5: // Set Vx = Vx - Vy, set VF = NOT borrow.
			_v[0xF] = (_v[x] > _v[y]) ? 1 : 0;
			_v[x] -= _v[y];
			_pc += 2;
			break;
		case 0x6: // Set Vx = Vx SHR 1.
			_v[0xF] = (_v[x] & 0x1) != 0 ? 1 : 0;
			_v[x] /= 2;
			_pc += 2;
			break;
		case 0x7: // Set Vx = Vy - Vx, set VF = NOT borrow.
			_v[0xF] = _v[y] > _v[x] ? 1 : 0;
			_v[x] = _v[y] - _v[x];
			_pc += 2;
			break;
		case 0xE: // Set Vx = Vx SHL 1.
			_v[0xF] = (_v[x] & 0x80) != 0 ? 1 : 0;
			_v[x] *= 2;
			_pc += 2;
			break;
		default:
			std::cout << "Unknown instruction: " << instruction << std::endl;
		}
		break;
	case 0x9000: // Skip next instruction if Vx != Vy.
		_pc += _v[x] != _v[y] ? 4 : 2;
		break;
	case 0xA000: // Set I = nnn.
		_i = nnn;
		_pc += 2;
		break;
	case 0xB000: // Jump to location nnn + V0.
		_pc = nnn + _v[0];
		break;
	case 0xC000: // Set Vx = random byte AND kk.
		_v[x] = kk & u8(rand() % 255);
		_pc += 2;
		break;
	case 0xD000: //  Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
		u16 pixel;
		_v[0xF] = 0x0;

		for (unsigned line = 0x0; line < n; line++)
		{
			pixel = _memory[_i + line];
			for(unsigned bitCount = 0x0; bitCount < 0x8; ++bitCount)
			{
				if((pixel & (0x80 >> bitCount)) != 0x0)
				{
					if(_display[(_v[x] + bitCount + ((_v[y] + line) * DISPLAY_W))] == 0x1) 
						_v[0xF] = 0x1;                                 
					_display[_v[x] + bitCount + ((_v[y] + line) * DISPLAY_W)] ^= 0x1;
				}
			}
		}

		for(unsigned j=0; j<(DISPLAY_W * DISPLAY_H); ++j)
		{
			if(_display[j] != 0) 
				_pixels[j].setFillColor(sf::Color::White);
			else 
				_pixels[j].setFillColor(sf::Color::Black);
		}

#ifndef ALTERNATIVE_REDRAW
		_isRedraw = true; // Moved to 1NNN to remove 'sprite-blinking'.
#endif
		_pc += 2;
		break;
	case 0xE000:
		switch(instruction & 0xFF)
		{
		case 0x9E: // Skip next instruction if key with the value of Vx is pressed. 
			if(getKey(_v[x])) _pc += 2;
			_pc += 2;
			break;
		case 0xA1: // Skip next instruction if key with the value of Vx is not pressed.
			if(!getKey(_v[x])) _pc += 2;
			_pc += 2; 
			break;
		default:
			std::cout << "Unknown instruction: " << instruction << std::endl;
		}
		break;
	case 0xF000:
		switch(instruction & 0xFF)
		{
		case 0x07: // Set Vx = delay timer value.
			_v[x] = _delayTimer;
			_pc += 2;
			break;
		case 0x0A: // Wait for a key press, store the value of the key in Vx.
			for(unsigned i=0; i<0x10; ++i)
			{
				if(_keypad[i] != 0)
				{ 
					_v[x] = i; 
					_pc += 2; 
				}
			}
			break;
		case 0x15: // Set delay timer = Vx.
			_delayTimer = _v[x];
			_pc += 2;
			break;
		case 0x18: // Set sound timer = Vx.
			_soundTimer = _v[x];
			_pc += 2;
			break;
		case 0x1E: // Set I = I + Vx.
			_i += _v[x];
			_pc += 2;
			break;
		case 0x29: // Set I = location of sprite for digit Vx.
			_i = _v[x] * 5; // Multiplied by sprite-size (5). 
			_pc += 2;
			break;
		case 0x33: // Store BCD representation of Vx in memory locations I, I+1, and I+2.
			// Ref: http://www.multigesture.net/wp-content/uploads/mirror/goldroad/chip8.shtml
			_memory[_i] = _v[x] / 100;
			_memory[_i + 1] = (_v[x] / 10) % 10;
			_memory[_i + 2] = (_v[x] % 100) % 10;
			_pc += 2;
			break;  
		case 0x55: // Store registers V0 through Vx in memory starting at location I.
			for(unsigned j=0; j<x; ++j) _memory[_i + j] = _v[j]; 
			_pc += 2;
			break;
		case 0x65: // Read registers V0 through Vx from memory starting at location I.
			for(unsigned j=0; j<x; ++j) _v[j] = _memory[_i + j]; 
			_pc += 2;
			break;
		default:
			std::cout << "Unknown instruction: " << instruction << std::endl;
		}
		break;
	default:
		std::cout << "Unknown instruction: " << instruction << std::endl;
	}

#ifdef DEBUG
	Chip8Debugger::analyseCycle(instruction, _pc, _sp, _i, _v, nnn, n, x, y, kk);
#endif
}

void Chip8::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(unsigned i=0; i<_pixels.size(); ++i) target.draw(_pixels[i], states);
}
