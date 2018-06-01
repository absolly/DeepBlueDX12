#include "mge/core/Menu.hpp"
#include "mge\config.hpp"


Menu::Menu(sf::RenderWindow * pWindow)
{
	_window = pWindow;

	if (!texture.loadFromFile(Config::MGE_SPRITES_PATH + "menubg.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for background" << std::endl;
	}

	sprite.setTexture(texture);


	if (!texture3.loadFromFile(Config::MGE_SPRITES_PATH + "Play.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sprite3.setTexture(texture3);
	sprite3.setPosition(98, 750);


	if (!texture4.loadFromFile(Config::MGE_SPRITES_PATH + "quit.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sprite4.setTexture(texture4);
	sprite4.setPosition(98, 830);

	if (!texture5.loadFromFile(Config::MGE_SPRITES_PATH + "selected_button.png", sf::IntRect(0, 0, (int)Config::SCREEN_RESOLUTION.x, (int)Config::SCREEN_RESOLUTION.y)))
	{
		std::cout << "Cant find image for menu shadow" << std::endl;
	}

	sprite5.setTexture(texture5);
	sprite5.setPosition(30, 800);
}


Menu::~Menu()
{
}

bool Menu::RenderMenu()
{
	sprite5.setPosition(0, -20);
	sf::Vector2i position = sf::Mouse::getPosition(*_window);
	// set mouse position relative to a window

	if (sprite3.getGlobalBounds().contains(position.x, position.y))
	{
		sprite5.setPosition(30, 800);
	}


	if (sprite4.getGlobalBounds().contains(position.x, position.y))
	{
		sprite5.setPosition(30, 880);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// set mouse position relative to a window

		if (sprite3.getGlobalBounds().contains(position.x, position.y))
		{
			_window->setMouseCursorVisible(false);
			return 0;
		}


		if (sprite4.getGlobalBounds().contains(position.x, position.y))
		{
			exit(EXIT_SUCCESS);
		}
	}

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);

	_window->pushGLStates();
	_window->draw(sprite);
	_window->draw(sprite3);
	_window->draw(sprite4);
	_window->draw(sprite5);
	_window->popGLStates();

	return 1;


}