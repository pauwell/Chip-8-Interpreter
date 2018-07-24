/*
	Bug list:
	* Rom: *WIPEOFF* if paddle moves out right screen side. There is a problem with the draw function.
	
	todolist:
	* Add 'beep'-sound
	* Add a frame-counter
	* Add an assembler?
	* Dynamic rom-selection.
*/
#include "Chip8.hpp"

int main()
{
	// Create interpreter.
	Chip8 chip8;
	chip8.initialize();
	chip8.loadRom("Roms/TETRIS");

	// Create window.
	sf::RenderWindow window(sf::VideoMode(768, 384, 32), "", sf::Style::Close);
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
			if(event.key.code == sf::Keyboard::X) chip8.setKey(0x0, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::Num1) chip8.setKey(0x1, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::Num2) chip8.setKey(0x2, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::Num3) chip8.setKey(0x3, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::Q) chip8.setKey(0x4, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::W) chip8.setKey(0x5, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::E) chip8.setKey(0x6, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::A) chip8.setKey(0x7, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::S) chip8.setKey(0x8, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::D) chip8.setKey(0x9, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::Z) chip8.setKey(0xA, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::C) chip8.setKey(0xB, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::Num4) chip8.setKey(0xC, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::R) chip8.setKey(0xD, (event.type==sf::Event::KeyPressed ? true : false));
			else if(event.key.code == sf::Keyboard::F) chip8.setKey(0xE, (event.type==sf::Event::KeyPressed ? true : false));
			
			if(event.type == sf::Event::Closed) window.close();
		}

		// Update interpreter.
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