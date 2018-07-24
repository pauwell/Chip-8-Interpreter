#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>  
#include <array>
#include <fstream>
#include <vector>
#include <SFML\Graphics.hpp>

//#ifndef DEBUG
//	#define DEBUG
//#endif

//#ifndef ALTERNATIVE_REDRAW
//	#define ALTERNATIVE_REDRAW
//#endif

#define MEMORY_SIZE 0x1000
#define V_REGS_TOTAL 0x10
#define STACK_SIZE 0x10
#define KEYPAD_SIZE 0x10
#define DISPLAY_W 0x40
#define DISPLAY_H 0x20
#define FONTSET_SIZE 0x50
#define PROGRAM_START 0x200
#define PIXEL_SIZE 0xC

typedef sf::Uint8 u8;
typedef sf::Uint16 u16;

const u8 FONTSET[FONTSET_SIZE] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Chip8 : public sf::Drawable, public sf::Transformable
{
	u8	_memory[MEMORY_SIZE]; 
	u8	_v[V_REGS_TOTAL];
	u16 _i;
	u8	_delayTimer;
	u8	_soundTimer;
	u16 _pc;
	u8	_sp;
	u16	_stack[STACK_SIZE];
	u8 _keypad[KEYPAD_SIZE];
	u8 _display[DISPLAY_W * DISPLAY_H];

	std::array<sf::RectangleShape, (DISPLAY_W * DISPLAY_H)> _pixels;
	bool _isRedraw;

public:
	Chip8();
	
	void initialize();
	void loadRom(const std::string fileName);
	void runCycle();

	const bool isRedraw();

	void setKey(const unsigned index, const bool isPressed);
	const bool getKey(const unsigned index) const;

private:
	void clearScreen();
	void updateTimers();
	void executeInstruction();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};