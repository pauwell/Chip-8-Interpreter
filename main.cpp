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

/*
	TODO
	* Add 'beep'-sound
	* Add a frame-counter
	* Add an assembler?
	* Dynamic rom-selection.
*/

int main()
{
	// Create interpreter.
	Chip8 chip8;
	//chip8.loadRom("Roms/TETRIS");
	
	// Select a ROM and load it.
	std::string path{""};
	do 
	{
		std::cout << "Path to chip-8 ROM: ";
		std::cin >> path;
	} while (!chip8.loadRom(path));

	// Create window.
	sf::RenderWindow window(sf::VideoMode(768, 384, 32), "pauwell's chip-8 interpreter", sf::Style::Close);
	sf::Event event;
	sf::Clock clock;

	// Restrict framerate to 60.
	window.setFramerateLimit(40);

	// Loop window.
	while(window.isOpen())
	{
		// Process events.
		while(window.pollEvent(event))
		{
			// Keyboard-input.
			if(event.key.code == sf::Keyboard::X) chip8.setKey(0x0, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::Num1) chip8.setKey(0x1, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::Num2) chip8.setKey(0x2, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::Num3) chip8.setKey(0x3, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::Q) chip8.setKey(0x4, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::W) chip8.setKey(0x5, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::E) chip8.setKey(0x6, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::A) chip8.setKey(0x7, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::S) chip8.setKey(0x8, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::D) chip8.setKey(0x9, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::Z) chip8.setKey(0xA, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::C) chip8.setKey(0xB, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::Num4) chip8.setKey(0xC, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::R) chip8.setKey(0xD, (event.type==sf::Event::KeyPressed));
			else if(event.key.code == sf::Keyboard::F) chip8.setKey(0xE, (event.type==sf::Event::KeyPressed));
			else if (event.key.code == sf::Keyboard::Escape) window.close();
			if(event.type == sf::Event::Closed) window.close();
		}

		// Handle next instruction.
		chip8.runCycle();

		// Draw to screen if redraw-flag is set.
		if(chip8.isRedraw())
		{
			window.clear(sf::Color::Blue);
			window.draw(chip8);
			window.display();
		}
	}
	
	return 0;
}
