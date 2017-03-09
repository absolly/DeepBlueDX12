#include "Menu.h"
#include "mge\config.hpp"




Menu::Menu(sf::RenderWindow * pWindow)
{
	_window = pWindow;

	if (!texture.loadFromFile(Config::MGE_TEXTURE_PATH + "DoorsMat_Base_Color.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for background" << std::endl;
	}

	sprite.setTexture(texture);


	if (!texture2.loadFromFile(Config::MGE_TEXTURE_PATH + "menu_backdrop.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sprite2.setTexture(texture2);


	if (!texture3.loadFromFile(Config::MGE_TEXTURE_PATH + "Play.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sprite3.setTexture(texture3);
	sprite3.setPosition(40, 700);


	if (!texture4.loadFromFile(Config::MGE_TEXTURE_PATH + "quit.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sprite4.setTexture(texture4);
	sprite4.setPosition(40, 780);
}


Menu::~Menu()
{
}

void Menu::RenderMenu()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// get global mouse position
		sf::Vector2i position = sf::Mouse::getPosition(*_window);
		// set mouse position relative to a window

		if (sprite3.getGlobalBounds().contains(position.x, position.y))
		{
			std::cout << "enter Game" << std::endl;
			_window->setMouseCursorVisible(false);
		}


		if (sprite4.getGlobalBounds().contains(position.x, position.y))
		{
			exit(EXIT_FAILURE);
		}
	}

	_window->draw(sprite);

	_window->draw(sprite2);

	_window->draw(sprite3);

	_window->draw(sprite4);

	_window->display();
}
