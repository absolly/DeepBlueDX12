#include <iostream>

#include "mge/core/AbstractGame.hpp"
#include "Content/TestScene.hpp"
#include "mge/config.hpp"
#include <SFML\Graphics.hpp>
#include <Windows.h>

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
/**
 * Main entry point for the Micro Engine.

 * Design goals:
 * - correct division of OpenGL into appropriate wrapper classes
 * - simple design
 * - each class should know as little as possible about other classes and non related parts of OpenGL
 * - the engine must allow different objects with different transforms, meshes, materials (shaders) etc
 * - consistent coding conventions
 * - reusable and extendable core set of classes which should require no modification
 *   (in other words it is possible to have a working "empty" example)
 *
 * All documentation is contained within the HEADER files, not the CPP files in possible.
 *
 */
int main()
{
    std::cout << "Starting Game" << std::endl;


    AbstractGame* game = new TestScene();
	game->_initializeWindow();

	bool enteredGame = false;

	sf::Texture texture;
	
	if (!texture.loadFromFile(Config::MGE_TEXTURE_PATH + "DoorsMat_Base_Color.png", sf::IntRect(0,0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for background" << std::endl;
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);


	sf::Texture texture2;

	if (!texture2.loadFromFile(Config::MGE_TEXTURE_PATH + "menu_backdrop.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sf::Sprite sprite2;
	sprite2.setTexture(texture2);

	sf::Texture texture3;

	if (!texture3.loadFromFile(Config::MGE_TEXTURE_PATH + "Play.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sf::Sprite sprite3;
	sprite3.setTexture(texture3);
	sprite3.setPosition(40, 700);

	sf::Texture texture4;

	if (!texture4.loadFromFile(Config::MGE_TEXTURE_PATH + "quit.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sf::Sprite sprite4;
	sprite4.setTexture(texture4);
	sprite4.setPosition(40, 780);




	while (enteredGame == false)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// get global mouse position
			sf::Vector2i position = sf::Mouse::getPosition(*game->getRenderWindow());
			// set mouse position relative to a window
			std::cout << position.x << std::endl;

			if (sprite3.getGlobalBounds().contains(position.x,position.y))
			{
				enteredGame = true;
			}


			if (sprite4.getGlobalBounds().contains(position.x, position.y))
			{
				exit(EXIT_FAILURE);
			}
		}

		game->getRenderWindow()->draw(sprite);

		game->getRenderWindow()->draw(sprite2);

		game->getRenderWindow()->draw(sprite3);

		game->getRenderWindow()->draw(sprite4);

		game->getRenderWindow()->display();
	}

	game->getRenderWindow()->setMouseCursorVisible(false);
    game->initialize();
    game->run();

	//delete game;

    return 0;
}



